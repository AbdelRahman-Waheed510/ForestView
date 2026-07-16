#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

class Texture 
{
private:

	GLuint loadTexture_helper(const char* file_path, int* width, int* height)
	{
		int channels;

		unsigned char* data = stbi_load(file_path, width, height, &channels, 4);

		if (data == nullptr)
		{
			std::cerr << "Filed to load images" << std::endl;
			return 0;
		}

		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);

		return textureID;
	}

	GLuint TextureID;
	int width = 0;
	int height = 0;

public:

	Texture() = default;

	Texture(const char* path) 
	{
		//TextureID = loadTexture(path, &width, &height);
	}

	~Texture() 
	{
		if (TextureID != 0) 
		{
			glDeleteTextures(1, &TextureID);
		}
	}

	void loadTexture(const char* path) 
	{
		TextureID = loadTexture_helper(path, &width, &height);
	}

	const GLuint get_texture_id() 
	{
		return TextureID;
	}

	const int get_width()
	{
		return width;
	}

	const int get_height() 
	{
		return height;
	}
};