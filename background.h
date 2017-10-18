#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <GL/glew.h>
#include <GL/gl.h>

class Background
{
public:
    Background() : time(0) {}
    ~Background() {}

    void Draw(GLuint*);

private:
    float time; // to do: base this variable on real time
};

void Background::Draw(GLuint *shader)
{
    glUseProgram(*shader);

    time += 0.01f;
    glUniform1f(glGetUniformLocation(*shader, "time"), time);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

#endif // BACKGROUND_H
