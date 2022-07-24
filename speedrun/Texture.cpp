#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"
#include "includes.h"



void Texture::Bind(const unsigned int& slot)const {

	report(glActiveTexture(GL_TEXTURE0 + slot));
	report(glBindTexture(GL_TEXTURE_2D, m_textureId));

}

Texture::~Texture() {


	if (m_locationBuffer) {
		stbi_image_free(m_locationBuffer);
	}

	report(glDeleteTextures(1, &m_textureId));
}

void Texture::UnBind() const{

	report(glBindTexture(GL_TEXTURE_2D, 0));


}

Texture::Texture(const std::string filePath) 
:m_filePath(filePath), m_textureId(0), m_locationBuffer(nullptr)
{

	report(glGenTextures(1, &m_textureId));
	report(glBindTexture(GL_TEXTURE_2D, m_textureId));

	report(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	report(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

	report(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	report(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));


	stbi_set_flip_vertically_on_load(true);

	m_locationBuffer = stbi_load(m_filePath.c_str(), &width, &height, &channels, 4);

	report(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_locationBuffer));

	report(glGenerateMipmap(GL_TEXTURE_2D));


}

void Texture::operator=(Texture&& rvalue) noexcept{

	m_textureId = rvalue.m_textureId;
	m_locationBuffer = rvalue.m_locationBuffer;
	width = rvalue.width;
	height = rvalue.height;
	channels = rvalue.channels;
	m_filePath = rvalue.m_filePath;

	rvalue.m_locationBuffer = nullptr;
	rvalue.m_textureId = 0;
	rvalue.m_filePath = "";
	rvalue.width = 0;
	rvalue.height = 0;
	rvalue.channels = 0;

	report(glGenTextures(1, &m_textureId));
	report(glBindTexture(GL_TEXTURE_2D, m_textureId));

	report(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	report(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

	report(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	report(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	report(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_locationBuffer));

	report(glGenerateMipmap(GL_TEXTURE_2D));
}
