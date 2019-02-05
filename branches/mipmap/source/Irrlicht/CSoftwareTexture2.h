// Copyright (C) 2002-2012 Nikolaus Gebhardt / Thomas Alten
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_SOFTWARE_2_TEXTURE_H_INCLUDED__
#define __C_SOFTWARE_2_TEXTURE_H_INCLUDED__

#include "SoftwareDriver2_compile_config.h"

#include "ITexture.h"
#include "IRenderTarget.h"
#include "CImage.h"

namespace irr
{
namespace video
{

class CBurningVideoDriver;

/*!
	interface for a Video Driver dependent Texture.
*/
struct CSoftwareTexture2_Bound
{
	f32 w;  // width - 0.5f;
	f32 h;  // height- 0.5f;
	f32 cx; // texelcenter x 1.f/width*0.5f
	f32 cy; // texelcenter y 1.f/height*0.5f
};

class CSoftwareTexture2 : public ITexture
{
public:

	//! constructor
	enum eTex2Flags
	{
		GEN_MIPMAP	= 1,
		IS_RENDERTARGET	= 2,
		NP2_SIZE	= 4,
	};
	CSoftwareTexture2(IImage* surface, const io::path& name, u32 flags);

	//! destructor
	virtual ~CSoftwareTexture2();

	//! lock function
	virtual void* lock(E_TEXTURE_LOCK_MODE mode, u32 mipmapLevel, u32 layer, E_TEXTURE_LOCK_FLAGS lockFlags = ETLF_FLIP_Y_UP_RTT) _IRR_OVERRIDE_
	{
		if (Flags & GEN_MIPMAP)
		{
			MipMapLOD = mipmapLevel;
			Size = MipMap[MipMapLOD]->getDimension();
			Pitch = MipMap[MipMapLOD]->getPitch();
		}

		return MipMap[MipMapLOD]->getData();
	}

	//! unlock function
	virtual void unlock() _IRR_OVERRIDE_
	{
	}

	//! compare the area drawn with the area of the texture
	f32 getLODFactor( const f32 texArea ) const
	{
		return OrigImageDataSizeInPixels * texArea;
		//return MipMap[0]->getImageDataSizeInPixels () * texArea;
	}

	//! returns unoptimized surface (misleading name)
	virtual CImage* getImage() const
	{
		return MipMap[0];
	}

	//! returns texture surface
	virtual CImage* getTexture() const
	{
		return MipMap[MipMapLOD];
	}

	//precalculated dimx-1/dimx*0.5f
	const CSoftwareTexture2_Bound& getTexBound() const
	{
		return TexBound[MipMapLOD];
	}

	virtual void regenerateMipMapLevels(void* data = 0, u32 layer = 0) _IRR_OVERRIDE_;

private:
	void calcDerivative();
	f32 OrigImageDataSizeInPixels;

	CImage * MipMap[SOFTWARE_DRIVER_2_MIPMAPPING_MAX];

	u32 MipMapLOD;
	u32 Flags;
	ECOLOR_FORMAT OriginalFormat;

	CSoftwareTexture2_Bound TexBound[SOFTWARE_DRIVER_2_MIPMAPPING_MAX];

};

/*!
interface for a Video Driver dependent render target.
*/
class CSoftwareRenderTarget2 : public IRenderTarget
{
public:
	CSoftwareRenderTarget2(CBurningVideoDriver* driver);
	virtual ~CSoftwareRenderTarget2();

	virtual void setTexture(const core::array<ITexture*>& texture, ITexture* depthStencil, const core::array<E_CUBE_SURFACE>& cubeSurfaces) _IRR_OVERRIDE_;

	ITexture* getTexture() const;

protected:
	CBurningVideoDriver* Driver;
};


} // end namespace video
} // end namespace irr

#endif

