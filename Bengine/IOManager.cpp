#include "IOManager.h"

namespace Bengine {
	bool IOManager::readFileToBuffer(const std::string filePath, std::vector<unsigned char>& buffer)
	{
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail())
		{
			perror(filePath.c_str());
			return false;
		}

		// seek to the end & get size
		file.seekg(0, std::ios::end);
		int fileSize = static_cast<int>(file.tellg());
		file.seekg(0, std::ios::beg);

		// Reduce filesize by any header bytes (maybe be useless?)
		fileSize -= static_cast<int>(file.tellg());

		buffer.resize(fileSize);
		file.read((char*)&(buffer[0]), fileSize);

		return true;
	}
}