#pragma once

#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

namespace fs = std::filesystem;

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

class TexturesManager 
{
private:
	Texture folder_icon;
	Texture word_icon;
	Texture power_point_icon;
	Texture exe_icon;
	Texture music_icon;
	Texture pdf_icon;
	Texture picture_icon;
	Texture zip_icon;
	Texture txt_file_icon;
	Texture code_icon;
	Texture default_icon;
	
	Texture LargeThumbnail_button_icon;
	Texture Rows_button_icon;
	Texture reset_button_icon;
	Texture backwards_button_icon;
	Texture forward_button_icon;
	Texture Settings_button_icon;

	Texture DarkMode_button_icon;
	Texture LightMode_button_icon;

public:

	TexturesManager() 
	{

	}

	void load_images()
	{
		folder_icon.loadTexture("assets/Images/Folder.png");
		word_icon.loadTexture("assets/Images/word.png");
		exe_icon.loadTexture("assets/Images/exe.png");
		power_point_icon.loadTexture("assets/Images/powerpoint.png");
		music_icon.loadTexture("assets/Images/music.png");
		pdf_icon.loadTexture("assets/Images/pdf.png");
		picture_icon.loadTexture("assets/Images/picture.png");
		zip_icon.loadTexture("assets/Images/rar-format.png");
		txt_file_icon.loadTexture("assets/Images/txt-file.png");
		code_icon.loadTexture("assets/Images/Code.png");
		default_icon.loadTexture("assets/Images/default.png");
		LargeThumbnail_button_icon.loadTexture("assets/Images/tile.png");
		Rows_button_icon.loadTexture("assets/Images/list.png");
		reset_button_icon.loadTexture("assets/Images/reset.png");
		backwards_button_icon.loadTexture("assets/Images/backwards.png");
		forward_button_icon.loadTexture("assets/Images/forward.png");
		Settings_button_icon.loadTexture("assets/Images/Settings.png");
		DarkMode_button_icon.loadTexture("assets/Images/DarkMode.png");
		LightMode_button_icon.loadTexture("assets/Images/LightMode.png");
	}

	const GLuint get_LargeThumbnail_button_icon_texture_id()
	{
		return LargeThumbnail_button_icon.get_texture_id();
	}

	const GLuint get_Rows_button_icon_texture_id()
	{
		return Rows_button_icon.get_texture_id();
	}

	const GLuint get_backwards_button_icon_texture_id()
	{
		return backwards_button_icon.get_texture_id();
	}

	const GLuint get_forward_button_icon_texture_id()
	{
		return forward_button_icon.get_texture_id();
	}

	const GLuint get_reset_button_icon_texture_id()
	{
		return reset_button_icon.get_texture_id();
	}

	const GLuint get_settings_button_icon_texture_id()
	{
		return Settings_button_icon.get_texture_id();
	}

	const GLuint get_DarkMode_button_icon_texture_id()
	{
		return DarkMode_button_icon.get_texture_id();
	}

	const GLuint get_LightMode_button_icon_texture_id()
	{
		return LightMode_button_icon.get_texture_id();
	}

	const GLuint decide_icon(const fs::directory_entry& entry)
	{
		if (entry.is_directory())
		{
			return folder_icon.get_texture_id();
		}
		else if (entry.is_regular_file())
		{

			fs::path extention = entry.path().extension();

			if (extention == ".pdf") return pdf_icon.get_texture_id();
			else if (extention == ".docx") return word_icon.get_texture_id();
			else if (extention == ".pptx") return power_point_icon.get_texture_id();
			else if (extention == ".exe") return exe_icon.get_texture_id();
			else if 
				(
				extention == L".cpp"   || 
				extention == L".hpp"   || 
				extention == L".c"     || 
				extention == L".h"     || 
				extention == L".py"    || 
				extention == L".js"    || 
				extention == L".json"  || 
				extention == L".xml"
				)
			return code_icon.get_texture_id();
			else if (extention == ".mp3") return music_icon.get_texture_id();
			else if (extention == ".png" || extention == ".jpeg" || extention == ".jpg") return picture_icon.get_texture_id();
			else if (extention == ".zip" || extention == ".rar") return zip_icon.get_texture_id();
			else if (extention == ".txt") return txt_file_icon.get_texture_id();
			else return default_icon.get_texture_id();
		}
		else return default_icon.get_texture_id();
	}

};