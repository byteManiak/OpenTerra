#ifndef TEXT_H
#define TEXT_H

#include <cstring>
#include <vector>
#include <memory>
#include <GL/gl.h>
#include <freetype/freetype.h>
#include "structs.h"
#include "character.h"

extern FT_Library ftlib;

struct Text
{
    Text(const char*, GLfloat, GLfloat);
    ~Text() {}

    void Draw();

    FT_Face font;
    const char *text;

    GLfloat x, y, init_x;

    std::vector< std::unique_ptr<Character> > characters;
};

Text::Text(const char *text, GLfloat x, GLfloat y) : text(text), x(x), y(y), init_x(x)
{
    FT_New_Face(ftlib, "LessPerfectDOSVGA.ttf", 0, &font);
    FT_Set_Pixel_Sizes(font, 0, 24);

    for(size_t i = 0; i<strlen(text); i++)
        characters.emplace_back(new Character(text[i], font, x, y, init_x));
}

void Text::Draw()
{
    for(size_t i=0; i<strlen(text); i++)
        characters[i]->Draw();
}
#endif // TEXT_H
