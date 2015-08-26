#pragma once

#include "stdafx.h"
#include "Errors.h"
#include "IOManager.h"
#include "GLTexture.h"
#include "picopng.h"

class ImageLoader
{
public:
	static GLTexture loadPNG(std::string filePath);
};

