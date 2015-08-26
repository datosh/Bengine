#pragma once

#include "stdafx.h"

class IOManager
{
public:
	static bool readFileToBuffer(const std::string filePath, std::vector<unsigned char>& buffer);
};

