#ifndef STRUCTS_H
#define STRUCTS_H

#include <GL/gl.h>

// in c++, structs act just like classes, but without private/protected keywords

struct vec2
{
    GLfloat x, y;

    vec2(){}
    vec2(GLfloat x, GLfloat y) : x(x), y(y) {}
};

struct vec3
{
    GLfloat x, y, z;

    vec3(){}
    vec3(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {}
};

struct vec4
{
    GLfloat x, y, z, w;

    vec4(){}
    vec4(GLfloat x, GLfloat y, GLfloat z, GLfloat w) : x(x), y(y), z(z), w(w) {}
};

struct vec2_buffer
{
    vec2 tl, tr, br, bl; // top-left, top-right, bottom-right, bottom-left

    vec2_buffer(){}
    vec2_buffer(vec2 tl, vec2 tr, vec2 br, vec2 bl) : tl(tl), tr(tr), br(br), bl(bl) {}
}; // a vec2_buffer basically acts as a SDL rectangle, but with one extra dimension added

bool onscreen(vec2 t)
{
    if(t.x < -1 || t.x > 1 || t.y < -1 || t.y > 1 ) return false;
    return true;
    // to do: rewrite function based on actual vectorial math
}

#endif // STRUCTS_H
