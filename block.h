#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <GL/gl.h>
#include <FreeImage.h>
#include "structs.h"

class Block
{
public:
    Block(const char*);

    void Draw(GLuint*);

private:
    GLfloat x, y;
    GLuint vert_buffer, coord_buffer, texture;
    vec2_buffer *verts;
    vec2_buffer *coords;
};

Block::Block(const char* path) : x(rand()%48 * 32), y(rand()%48 * 16)
{
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path);

    FIBITMAP *bitmap = FreeImage_Load(format, path);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, // target
                 0, // level, leave at 0
                 GL_RGBA, // internal color bit mode
                 FreeImage_GetWidth(bitmap),
                 FreeImage_GetHeight(bitmap),
                 0, // border, leave at 0
                 GL_BGRA, // external color bit mode
                 GL_UNSIGNED_BYTE, // data type
                 FreeImage_GetBits(bitmap) /* pointer to data */ );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // without setting these two parameters, there will be no texture on screen (i learned this the hard way)

    FreeImage_Unload(bitmap);

    verts = new vec2_buffer( vec2(-1 + x/1366*2, -1 + y/768*2),
                     vec2(-1 + x/1366*2 + .02, -1 + y/768*2),
                     vec2(-1 + x/1366*2 + .02, -1 + y/768*2 + .04),
                     vec2(-1 + x/1366*2, -1 + y/768*2 + .04));
    // to do: base vertex data on position in a map

    coords = new vec2_buffer( vec2(0, 0), vec2(1, 0), vec2(1, 1), vec2(0, 1) );

    glGenBuffers(1, &vert_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vert_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2_buffer), verts, GL_STATIC_DRAW);

    glGenBuffers(1, &coord_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, coord_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2_buffer), coords, GL_STATIC_DRAW);

    /* we will use two vertex buffers:
     * one for the position of the texture on the screen,
     * and the other one for the offset of the texture to be drawn on screen
     */
}

void Block::Draw(GLuint *shader)
{
    glUseProgram(*shader);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vert_buffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    // shader location 0 stores the position data

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(*shader, "tex"), 0);
    // bound texture to unit 0, and then unit 0 to shader sampler

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, coord_buffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    // shader location 1 stores the texture offset data

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

#endif // BLOCK_H
