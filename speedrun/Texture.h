#include <string>
#include <iostream>
#include "GL/glew.h"
#include "notMine/STB/stb_image.h"


class Texture {

private:
	std::string m_filePath;
	unsigned int m_textureId;
	unsigned char* m_locationBuffer;
	int width, height, channels;
public:

	Texture(const std::string filePath);
	void operator=(Texture&& rvalue)noexcept;
	~Texture();

	void Bind(const unsigned int& slot)const;
	void UnBind()const;



};