#pragma once

#include "stdafx.h"

namespace Bengine {
	class IOManager
	{
	public:
		static bool readFileToBuffer(const std::string filePath, std::vector<unsigned char>& buffer);
	};
}

