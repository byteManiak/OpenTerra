#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <GL/glew.h>
#include <GL/gl.h>

struct Background
{
    Background() : time(0) { glGenVertexArrays(1, &vao); }
    ~Background() { glDeleteVertexArrays(1, &vao); }

    void Draw();

    float time; // to do: base this variable on real time

    GLuint vao;
};

void Background::Draw()
{
    glUseProgram(bg);
    glBindVertexArray(vao);

    time += 0.01f;
    glUniform1f(glGetUniformLocation(bg, "time"), time);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(0);
    glUseProgram(0);
}

#endif // BACKGROUND_H
