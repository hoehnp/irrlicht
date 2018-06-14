// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_OPENGL_SHADER_LANGUAGE_MATERIAL_RENDERER_H_INCLUDED__
#define __C_OPENGL_SHADER_LANGUAGE_MATERIAL_RENDERER_H_INCLUDED__

#include "IrrCompileConfig.h"

#ifdef _IRR_COMPILE_WITH_OPENGL_

#include "EMaterialTypes.h"
#include "IMaterialRenderer.h"
#include "IMaterialRendererServices.h"
#include "IGPUProgrammingServices.h"
#include "irrArray.h"
#include "irrString.h"

#include "COpenGLCommon.h"

namespace irr
{
namespace video
{

class COpenGLDriver;
class IShaderConstantSetCallBack;

//! Class for using GLSL shaders with OpenGL
//! Please note: This renderer implements its own IMaterialRendererServices
class COpenGLSLMaterialRenderer : public IMaterialRenderer, public IMaterialRendererServices
{
public:

	//! Constructor
	COpenGLSLMaterialRenderer(
		COpenGLDriver* driver,
		s32& outMaterialTypeNr,
		const c8* vertexShaderProgram = 0,
		const c8* vertexShaderEntryPointName = 0,
		E_VERTEX_SHADER_TYPE vsCompileTarget = video::EVST_VS_1_1,
		const c8* pixelShaderProgram = 0,
		const c8* pixelShaderEntryPointName = 0,
		E_PIXEL_SHADER_TYPE psCompileTarget = video::EPST_PS_1_1,
		const c8* geometryShaderProgram = 0,
		const c8* geometryShaderEntryPointName = "main",
		E_GEOMETRY_SHADER_TYPE gsCompileTarget = EGST_GS_4_0,
		scene::E_PRIMITIVE_TYPE inType = scene::EPT_TRIANGLES,
		scene::E_PRIMITIVE_TYPE outType = scene::EPT_TRIANGLE_STRIP,
		u32 verticesOut = 0,
		IShaderConstantSetCallBack* callback = 0,
		E_MATERIAL_TYPE baseMaterial = EMT_SOLID,
		s32 userData = 0);

	//! Destructor
	virtual ~COpenGLSLMaterialRenderer();

	virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services) _IRR_OVERRIDE_;

	virtual bool OnRender(IMaterialRendererServices* service, E_VERTEX_TYPE vtxtype) _IRR_OVERRIDE_;

	virtual void OnUnsetMaterial() _IRR_OVERRIDE_;

	//! Returns if the material is transparent.
	virtual bool isTransparent() const _IRR_OVERRIDE_;

	// implementations for the render services
	virtual void setBasicRenderStates(const SMaterial& material, const SMaterial& lastMaterial, bool resetAllRenderstates) _IRR_OVERRIDE_;
	virtual s32 getVertexShaderConstantID(const c8* name) _IRR_OVERRIDE_;
	virtual s32 getPixelShaderConstantID(const c8* name) _IRR_OVERRIDE_;
	virtual void setVertexShaderConstant(const f32* data, s32 startRegister, s32 constantAmount=1) _IRR_OVERRIDE_;
	virtual void setPixelShaderConstant(const f32* data, s32 startRegister, s32 constantAmount=1) _IRR_OVERRIDE_;
	virtual bool setVertexShaderConstant(s32 index, const f32* floats, int count) _IRR_OVERRIDE_;
	virtual bool setVertexShaderConstant(s32 index, const s32* ints, int count) _IRR_OVERRIDE_;
	virtual bool setPixelShaderConstant(s32 index, const f32* floats, int count) _IRR_OVERRIDE_;
	virtual bool setPixelShaderConstant(s32 index, const s32* ints, int count) _IRR_OVERRIDE_;
	virtual IVideoDriver* getVideoDriver() _IRR_OVERRIDE_;

protected:

	//! constructor only for use by derived classes who want to
	//! create a fall back material for example.
	COpenGLSLMaterialRenderer(COpenGLDriver* driver,
					IShaderConstantSetCallBack* callback,
					E_MATERIAL_TYPE baseMaterial,
					s32 userData=0);

	void init(s32& outMaterialTypeNr,
		const c8* vertexShaderProgram,
		const c8* pixelShaderProgram,
		const c8* geometryShaderProgram,
		scene::E_PRIMITIVE_TYPE inType=scene::EPT_TRIANGLES,
		scene::E_PRIMITIVE_TYPE outType=scene::EPT_TRIANGLE_STRIP,
		u32 verticesOut=0);

	bool createProgram();
	bool createShader(GLenum shaderType, const char* shader);
	bool linkProgram();

	COpenGLDriver* Driver;
	IShaderConstantSetCallBack* CallBack;

	bool Alpha;
	bool Blending;
	bool FixedBlending;
	bool AlphaTest;

	struct SUniformInfo
	{
		core::stringc name;
		GLenum type;
		GLint location;
	};

	GLhandleARB Program;
	GLuint Program2;
	core::array<SUniformInfo> UniformInfo;
	s32 UserData;
};


} // end namespace video
} // end namespace irr

#endif // compile with OpenGL
#endif // if included

