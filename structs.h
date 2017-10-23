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

struct rec
{
    vec2 tl, tr, br, bl; // top-left, top-right, bottom-right, bottom-left

    rec(){}
    rec(vec2 tl, vec2 tr, vec2 br, vec2 bl) : tl(tl), tr(tr), br(br), bl(bl) {}
}; // a rec basically acts as a SDL rectangle, but with one extra dimension added

bool onscreen(vec2 t)
{
    if(t.x < -1 || t.x > 1 || t.y < -1 || t.y > 1 ) return false;
    return true;
    // to do: rewrite function based on actual vectorial math
}

rec* operator+(rec *src, vec2 t)
{
    src->bl.x += t.x;
    src->bl.y += t.y;
    src->br.x += t.x;
    src->br.y += t.y;
    src->tr.x += t.x;
    src->tr.y += t.y;
    src->tl.x += t.x;
    src->tl.y += t.y;
    return src;
}

rec *new_rec(int x, int y, int w, int h)
{
    rec *t = new rec();
    t->tl.x = x;
    t->tl.y = y;
    t->tr.x = x+w;
    t->tr.y = y;
    t->br.x = x+w;
    t->br.y = y+h;
    t->bl.x = x;
    t->bl.y = y+h;
}

#endif // STRUCTS_H
