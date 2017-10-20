#ifndef TEXT_H
#define TEXT_H

#include <GL/gl.h>
#include <freetype/freetype.h>

extern FT_Library ftlib;

struct Text
{
    Text(const char *text) : text(text) { FT_New_Face(ftlib, "LessPerfectDOSVGA.ttf", 0, &font); }
    ~Text() {}

    FT_Face font;
    const char *text;
    GLuint texture;
    GLuint vert_buffer, coord_buffer;
};
#endif // TEXT_H
