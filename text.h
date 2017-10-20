#ifndef TEXT_H
#define TEXT_H

#include <GL/gl.h>

class Text
{
public:
    Text(const char *text) : text(text) {}
    ~Text() {}

private:
    const char *text;
    GLuint texture;
    GLuint vert_buffer, coord_buffer;
};
#endif // TEXT_H
