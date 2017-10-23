#ifndef CHARACTER_H
#define CHARACTER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <freetype/freetype.h>
#include "shaders.h"
#include "structs.h"

struct Character
{
    Character(const char, FT_Face&, GLfloat&, GLfloat&, GLfloat);
    ~Character() {}

    void Draw();

    GLuint vao;
    GLuint texture;
    GLuint vbo, tbo;
    rec *verts;
    rec *coords;
};

Character::Character(const char letter, FT_Face &font, GLfloat &x, GLfloat &y, GLfloat init_x)
{
    if(letter == '\n')
    {
        y += 24;
        x = init_x;
        return;
    }
    if(letter == ' ')
    {
        x += 12;
        return;
    }
    FT_Load_Char(font, letter, FT_LOAD_RENDER);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                 font->glyph->bitmap.width, font->glyph->bitmap.rows,
                 0, GL_RED, GL_UNSIGNED_BYTE,
                 font->glyph->bitmap.buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    verts = new_rec(x, y, font->glyph->bitmap.width, -font->glyph->bitmap.rows);

    if(letter == 'q' || letter == 'y' || letter == 'p' || letter == 'g' || letter == 'j')
        verts = verts + vec2(0, 5);
    if(letter == 'Q') verts = verts + vec2(0, 3);

    x += font->glyph->bitmap.width + 2;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rec), verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    coords = new rec( vec2(0, 1), vec2(1, 1), vec2(1, 0), vec2(0, 0) );
    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rec), coords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
}

void Character::Draw()
{
    glUseProgram(text_shader);
    glBindVertexArray(vao);

    glUniform2fv(glGetUniformLocation(text_shader, "resolution"), 1, resolution);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(text_shader, "tex"), 1);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(0);
    glUseProgram(0);
}

#endif // CHARACTER_H
