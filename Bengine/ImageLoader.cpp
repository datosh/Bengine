#include "ImageLoader.h"

namespace Bengine {
	GLTexture ImageLoader::loadPNG(std::string filePath)
	{
		// Init to zero
		GLTexture texture = {};

		std::vector<unsigned char> in;
		std::vector<unsigned char> out;
		unsigned long width, height;

		if (IOManager::readFileToBuffer(filePath, in) == false)
		{
			fatalError("Failed to load PNG file (" + filePath + ") to buffer!");
		}

		int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
		if (errorCode != 0)
		{
			fatalError("decodePNG failed with error: " + std::to_string(errorCode) + "!");
		}

		glGenTextures(1, &(texture.id));
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		texture.width = width;
		texture.height = height;

		glBindTexture(GL_TEXTURE_2D, 0);

		return texture;
	}
}