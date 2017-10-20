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
    ~Block();
    void Draw(GLuint);

private:
    float x, y;
    GLuint texture;
    GLuint vbo, tbo;
    GLuint vao;
    vec2_buffer *verts;
    vec2_buffer *coords;
};

Block::Block(const char* path) : x(rand()%48 * 48), y(rand()%48 * 24)
{
    FIBITMAP *bitmap = FreeImage_Load(FIF_TARGA, path);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, // target
                 0, // level, leave at 0
                 GL_RGBA, // internal color bits
                 FreeImage_GetWidth(bitmap),
                 FreeImage_GetHeight(bitmap),
                 0, // border, leave at 0
                 GL_BGRA, // external color bits
                 GL_UNSIGNED_BYTE, // data type
                 FreeImage_GetBits(bitmap) /* pointer to data */ );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    /* without setting these two parameters, there will be no texture on screen
     * GL_NEAREST provides "blocky" pixel transition
     * GL_LINEAR provides "smooth" pixel transition (it also looks blurry)
     */

    FreeImage_Unload(bitmap);

    verts = new vec2_buffer( vec2(-1 + x/1366*2, -1 + y/768*2 + 48.f/768),
                             vec2(-1 + x/1366*2 + 48.f/1366, -1 + y/768*2 + 48.f/768),
                             vec2(-1 + x/1366*2 + 48.f/1366, -1 + y/768*2),
                             vec2(-1 + x/1366*2, -1 + y/768*2));
    // to do: base vertex data on position in a map

    coords = new vec2_buffer( vec2(0, 1), vec2(1, 1), vec2(1, 0), vec2(0, 0) );

    // creating a vertex array object is necessary so we can send vertex data to our shaders
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    /* we will use two vertex buffers:
     * one for the position of the rectangle on the screen,
     * and the other one for the offset of the texture to be drawn inside the rectangle */

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2_buffer), verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    // shader location 0 stores the position data

    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2_buffer), coords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    // shader location 1 stores the texture offset data

    glBindVertexArray(0);
}

void Block::Draw(GLuint shader)
{
    glUseProgram(shader);
    glBindVertexArray(vao);

    /* only the vertex array needs to be bound, not the buffers too
     * bind the buffers as well only if you need to update them */

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shader, "tex"), 0);
    // bound texture to unit 0, and then unit 0 to shader sampler2d

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(0);
    glUseProgram(0);
}

Block::~Block()
{
    delete verts;
    delete coords;
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &tbo);
    glDeleteTextures(1, &texture);
    glDeleteVertexArrays(1, &vao);
}
#endif // BLOCK_H
