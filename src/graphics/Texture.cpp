#include "../../include/graphics/Texture.h"

#include <iostream>

#include <png.h>
#include <GL/glew.h>

Texture::Texture(uint id, int width, int height) : id(id), width(width), height(height)
{

}

Texture::~Texture() 
{
	glDeleteTextures(1, &id);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}
// reading png image from file 
int LoadPngFromFile(const char* file, unsigned int& width, unsigned int& height) 
{
    FILE* f;
    if (fopen_s(&f, file, "rb") != 0) // check if file was opened
    {
        std::cerr << "ERROR::TEXTURE::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return 0;
    }
    // check header of file to be png format
    png_byte header[8];
    fread_s(header, 8 * sizeof(png_byte), 1, 8, f);
    int isNotPng = png_sig_cmp(header, 0, 8);
    if (isNotPng)
    {
        fclose(f);
        return 0;
    }
    // create png reader struct
    png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!pngPtr) 
    {
        fclose(f);
        return 0;
    }
    //
    png_infop infoPtr = png_create_info_struct(pngPtr);
    if (!infoPtr) 
    {
        png_destroy_read_struct(&pngPtr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(f);
        return 0;
    }
    png_infop endInfo = png_create_info_struct(pngPtr);
    if (!endInfo) 
    {
        png_destroy_read_struct(&pngPtr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(f);
        return 0;
    }

    if (setjmp(png_jmpbuf(pngPtr))) 
    {
        png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);
        fclose(f);
        return 0;
    }

    png_init_io(pngPtr, f);
    png_set_sig_bytes(pngPtr, 8);
    png_read_info(pngPtr, infoPtr);

    int bit_depth, color_type;
    png_get_IHDR(pngPtr, infoPtr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);

    png_read_update_info(pngPtr, infoPtr);
    int row_bytes = png_get_rowbytes(pngPtr, infoPtr);
    png_bytep image_data = (png_bytep)malloc(row_bytes * height * sizeof(png_byte));
    if (!image_data) 
    {
        png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);
        fclose(f);
        return 0;
    }

    png_bytepp row_pointers = (png_bytepp)malloc(height * sizeof(png_bytep));
    if (!row_pointers) 
    {
        png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);
        free(image_data);
        fclose(f);
        return 0;
    }
    for (unsigned int i = 0; i < height; ++i) 
    {
        row_pointers[height - 1 - i] = image_data + i * row_bytes;
    }
    png_read_image(pngPtr, row_pointers);

    int alpha; // check if image have alpha canal
    switch (png_get_color_type(pngPtr, infoPtr)) 
    {
    case PNG_COLOR_TYPE_RGBA:
        alpha = GL_RGBA;
        break;
    case PNG_COLOR_TYPE_RGB:
        alpha = GL_RGB;
        break;
    default:
        printf("Color type %d not supported!\n", png_get_color_type(pngPtr, infoPtr));
        png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);
        return 0;
    }
    // generate and load to GPU openGL texture from image data
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, alpha, GL_UNSIGNED_BYTE, (GLvoid*)image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);
    free(image_data);
    free(row_pointers);
    fclose(f);
    return texture;
}

Texture* LoadTexture(const std::string& filename)
{
    unsigned int width, height;
    GLuint texture = LoadPngFromFile(filename.c_str(), width, height);
    if (texture == 0) 
    {
        std::cerr << "Could not load texture " << filename << std::endl;
        return nullptr;
    }
    return new Texture(texture, width, height);
}
