// Copyright (C) 2008-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef IRR_I_VERTEX_BUFFER_H_INCLUDED
#define IRR_I_VERTEX_BUFFER_H_INCLUDED

#include "IReferenceCounted.h"
#include "irrArray.h"
#include "EHardwareBufferFlags.h"
#include "S3DVertex.h"

namespace irr
{
namespace scene
{

	class IVertexBuffer : public virtual IReferenceCounted
	{
	public:
		//! Pointer to first element of vertex data
		virtual void* getData() =0;

		virtual video::E_VERTEX_TYPE getType() const =0;
		virtual void setType(video::E_VERTEX_TYPE vertexType) =0;

		//! Number of bytes per element
		virtual u32 stride() const =0;

		//! Number of elements
		virtual u32 size() const =0;

		//! Add vertex to end. Note that depending on vertex type this will be one of the types derived from video::S3DVertex.
		virtual void push_back(const video::S3DVertex &element) =0;
		virtual video::S3DVertex& operator [](const u32 index) const =0;
		virtual video::S3DVertex& getLast() =0;
		virtual void set_used(u32 usedNow) =0;
		virtual void reallocate(u32 new_size, bool canShrink=true) =0;
		virtual u32 allocated_size() const =0;

		//! Same as getData() - not sure why we got 2, should probably deprecate (and we don't always have video::S3DVertex*, so just confusing)
		virtual video::S3DVertex* pointer() =0;

		//! get the current hardware mapping hint
		virtual E_HARDWARE_MAPPING getHardwareMappingHint() const =0;

		//! set the hardware mapping hint, for driver
		virtual void setHardwareMappingHint( E_HARDWARE_MAPPING NewMappingHint ) =0;

		//! flags the meshbuffer as changed, reloads hardware buffers
		virtual void setDirty() =0;

		//! Get the currently used ID for identification of changes.
		/** This shouldn't be used for anything outside the VideoDriver. */
		virtual u32 getChangedID() const = 0;
	};


} // end namespace scene
} // end namespace irr

#endif
