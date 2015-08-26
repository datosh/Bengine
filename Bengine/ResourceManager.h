#pragma once

#include "stdafx.h"
#include "TextureCache.h"
#include "GLTexture.h"

namespace Bengine {
	class ResourceManager
	{
	public:
		static GLTexture getTexture(std::string texturePath);
	private:
		static TextureCache m_textureCache;
	};
}