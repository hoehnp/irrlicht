// Copyright (C) 2002-2012 Nikolaus Gebhardt / Thomas Alten
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"
#include "IBurningShader.h"

#ifdef _IRR_COMPILE_WITH_BURNINGSVIDEO_

// compile flag for this file
#undef USE_ZBUFFER
#undef IPOL_Z
#undef CMP_Z
#undef WRITE_Z

#undef IPOL_W
#undef CMP_W
#undef WRITE_W

#undef SUBTEXEL
#undef INVERSE_W

#undef IPOL_C0
#undef IPOL_T0
#undef IPOL_T1

// define render case
#define SUBTEXEL
#define INVERSE_W

//#define USE_ZBUFFER
#define IPOL_W
//#define CMP_W
//#define WRITE_W


#define IPOL_C0
//#define IPOL_T0
//#define IPOL_T1

// apply global override
#ifndef SOFTWARE_DRIVER_2_PERSPECTIVE_CORRECT
	#undef INVERSE_W
#endif

#ifndef SOFTWARE_DRIVER_2_SUBTEXEL
	#undef SUBTEXEL
#endif

#ifndef SOFTWARE_DRIVER_2_USE_VERTEX_COLOR
	#undef IPOL_C0
#endif

#if !defined ( SOFTWARE_DRIVER_2_USE_WBUFFER ) && defined ( USE_ZBUFFER )
	#ifndef SOFTWARE_DRIVER_2_PERSPECTIVE_CORRECT
		#undef IPOL_W
	#endif
	#define IPOL_Z

	#ifdef CMP_W
		#undef CMP_W
		#define CMP_Z
	#endif

	#ifdef WRITE_W
		#undef WRITE_W
		#define WRITE_Z
	#endif

#endif


namespace irr
{

namespace video
{

class CTRTextureWire2 : public IBurningShader
{
public:

	//! constructor
	CTRTextureWire2(CBurningVideoDriver* driver);

	//! draws an indexed triangle list
	virtual void drawTriangle ( const s4DVertex *a,const s4DVertex *b,const s4DVertex *c );
	virtual void drawLine ( const s4DVertex *a,const s4DVertex *b);
	virtual bool canWireFrame () { return true; }

protected:
	virtual void scanline_bilinear ();

	void renderAlphaLine ( const s4DVertex *a,const s4DVertex *b ) const;
	void renderLine ( const s4DVertex *a,const s4DVertex *b ) const;

};

//! constructor
CTRTextureWire2::CTRTextureWire2(CBurningVideoDriver* driver)
: IBurningShader(driver)
{
	#ifdef _DEBUG
	setDebugName("CTRTextureWire2");
	#endif
}



/*!
*/
void CTRTextureWire2::renderLine ( const s4DVertex *a,const s4DVertex *b ) const
{
	int pitch0 = RenderTarget->getDimension().Width << VIDEO_SAMPLE_GRANULARITY;
	int pitch1 = RenderTarget->getDimension().Width << 2;

	int aposx = (int) a->Pos.x;
	int aposy = (int) a->Pos.y;
	int bposx = (int) b->Pos.x;
	int bposy = (int) b->Pos.y;

	int dx = bposx - aposx;
	int dy = bposy - aposy;

	int c;
	int m;
	int d = 0;
	int run;

	tVideoSample *dst;
#ifdef USE_ZBUFFER
	fp24 *z;
#endif

	int xInc0 = 1 << VIDEO_SAMPLE_GRANULARITY;
	int yInc0 = pitch0;

	int xInc1 = 4;
	int yInc1 = pitch1;

	if ( dx < 0 )
	{
		xInc0 = - ( 1 << VIDEO_SAMPLE_GRANULARITY);
		xInc1 = -4;
		dx = -dx;
	}

	if ( dy > dx )
	{
		//swap
		register s32 t;
		t = dx;dx=dy;dy=t;
		t = xInc0;xInc0=yInc0;yInc0=t;
		t = xInc1;xInc1=yInc1;yInc1=t;
	}

	if ( 0 == dx )
		return;

	SOFTWARE_DRIVER_2_CLIPCHECK_WIRE;
	dst = (tVideoSample*) ( (u8*) (tVideoSample*)RenderTarget->getData() + ( aposy * pitch0 ) + (aposx << VIDEO_SAMPLE_GRANULARITY ) );
#ifdef USE_ZBUFFER
	z = (fp24*) ( (u8*) (fp24*) DepthBuffer->lock() + ( aposy * pitch1 ) + (aposx << 2 ) );
#endif

	c = dx << 1;
	m = dy << 1;

	// slopes
	const f32 invDeltaX = core::reciprocal_approxim ( (f32)dx );

#ifdef IPOL_Z
	f32 slopeZ = (b->Pos.z - a->Pos.z) * invDeltaX;
	f32 dataZ = a->Pos.z;
#endif

#ifdef IPOL_W
	fp24 slopeW = (b->Pos.w - a->Pos.w) * invDeltaX;
	fp24 dataW = a->Pos.w;
#endif

#ifdef INVERSE_W
	f32 inversew;
#endif

	tVideoSample color;
#ifdef SOFTWARE_DRIVER_2_USE_VERTEX_COLOR
	tFixPoint r0, g0, b0;
#ifdef IPOL_C0
	sVec4 slopeC;
	sVec4 C;
	slopeC = (b->Color[0] - a->Color[0]) * invDeltaX;
	C = a->Color[0];
#endif
	getSample_color ( r0, g0, b0, a->Color[0] );
	color = fix_to_color ( r0, g0, b0 );
#else
	color = (tVideoSample) 0xFFFFFFFF;
#endif


	run = dx;
	while ( run )
	{
#ifdef CMP_Z
		if ( *z >= dataZ )
#endif
#ifdef CMP_W
		if ( dataW >= *z )
#endif
		{
#ifdef WRITE_Z
			*z = dataZ;
#endif
#ifdef WRITE_W
			*z = dataW;
#endif

#ifdef IPOL_C0
			inversew = reciprocal_zero_no ( dataW );
			getSample_color ( r0, g0, b0, C * inversew );
			color = fix_to_color ( r0, g0, b0 );
#endif
			*dst = color;

		}

		dst = (tVideoSample*) ( (u8*) dst + xInc0 );	// x += xInc
#ifdef CMP_Z
		z = (fp24*) ( (u8*) z + xInc1 );
#endif
#ifdef CMP_W
		z = (fp24*) ( (u8*) z + xInc1 );
#endif

		d += m;
		if ( d > dx )
		{
			dst = (tVideoSample*) ( (u8*) dst + yInc0 );	// y += yInc
#ifdef CMP_Z
			z = (fp24*) ( (u8*) z + yInc1 );
#endif
#ifdef CMP_W
			z = (fp24*) ( (u8*) z + yInc1 );
#endif

			d -= c;
		}
		run -= 1;
#ifdef IPOL_Z
		dataZ += slopeZ;
#endif
#ifdef IPOL_W
		dataW += slopeW;
#endif
#ifdef IPOL_C0
		C += slopeC;
#endif


	}

}

void CTRTextureWire2::scanline_bilinear()
{
}

void CTRTextureWire2::drawTriangle ( const s4DVertex *a,const s4DVertex *b,const s4DVertex *c )
{
	// sort on height, y
	if ( F32_A_GREATER_B ( a->Pos.y , b->Pos.y ) ) swapVertexPointer(&a, &b);
	if ( F32_A_GREATER_B ( b->Pos.y , c->Pos.y ) ) swapVertexPointer(&b, &c);
	if ( F32_A_GREATER_B ( a->Pos.y , b->Pos.y ) ) swapVertexPointer(&a, &b);

	renderLine ( a, b );
	renderLine ( b, c );
	renderLine ( a, c );

}


void CTRTextureWire2::drawLine ( const s4DVertex *a,const s4DVertex *b)
{
	// query access to TexMaps

	// sort on height, y
	if ( a->Pos.y > b->Pos.y ) swapVertexPointer(&a, &b);

	renderLine ( a, b );
}


} // end namespace video
} // end namespace irr

#endif // _IRR_COMPILE_WITH_BURNINGSVIDEO_

namespace irr
{
namespace video
{


//! creates a flat triangle renderer
IBurningShader* createTriangleRendererTextureGouraudWire2(CBurningVideoDriver* driver)
{
	#ifdef _IRR_COMPILE_WITH_BURNINGSVIDEO_
	return new CTRTextureWire2(driver);
	#else
	return 0;
	#endif // _IRR_COMPILE_WITH_BURNINGSVIDEO_
}


} // end namespace video
} // end namespace irr


