#pragma once

#include "stdafx.h"
#include "GLTexture.h"
#include "ImageLoader.h"

class TextureCache
{
public:
	TextureCache();
	~TextureCache();

	GLTexture getTexture(std::string texturePath);

private:
	std::map<std::string, GLTexture> m_textureMap;
};

