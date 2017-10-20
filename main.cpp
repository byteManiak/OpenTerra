#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include "shaders.h"
#include "background.h"
#include "block.h"
#include "text.h"

FT_Library ftlib;

GLuint compile_shader(const char** v, const char **f)
{
    GLuint v_shad = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v_shad, 1, v, NULL);
    glCompileShader(v_shad);

    GLint err;

    glGetShaderiv(v_shad, GL_INFO_LOG_LENGTH, &err);

    GLchar out_v[err];
    glGetShaderInfoLog(v_shad, err, NULL, out_v);

    if(err) std::cerr << out_v;

    GLuint f_shad = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f_shad, 1, f, NULL);
    glCompileShader(f_shad);

    glGetShaderiv(f_shad, GL_INFO_LOG_LENGTH, &err);

    GLchar out_f[err];
    glGetShaderInfoLog(f_shad, err, NULL, out_f);

    if(err) std::cerr << out_f;

    GLuint program = glCreateProgram();
    glAttachShader(program, v_shad);
    glAttachShader(program, f_shad);

    glLinkProgram(program);

    glDeleteShader(v_shad);
    glDeleteShader(f_shad);

    return program;
}

int main()
{
    srand(time(NULL));

    if(!glfwInit())
    {
        std::cerr << "Could not initialise GLFW";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* this needs to be specified before window creation,
     * otherwise the context will have a different GL version
     * NOTE: keeping this to GL3.3 for backwards compabiility with older hardware.
     */

    int num_modes;
    const GLFWvidmode *vid_modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &num_modes);

    GLFWwindow *window =
        glfwCreateWindow(vid_modes[num_modes-1].width,
                         vid_modes[num_modes-1].height, // last video mode in the array is the maximum monitor resolution
                         "OpenTerra",
                         NULL, // setting this to glfwGetPrimaryMonitor() will make the window fullscreen
                         NULL /* this parameter can be changed to a second window
                               * to share contents with. No need for it though. */ );

    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK)
    {
        std::cerr << "Could not initialise GLEW";
        return 2;
    }

    glewExperimental = GL_TRUE;

    FT_Init_FreeType(&ftlib);

    glClearColor(0, .4, .7, 1);
    /* in case something fails, the background or the entire screen will be cyan
     * in case something fails *really* badly, the screen will be black
     */

    int w, h;

    GLuint bg = compile_shader(vert, frag);
    GLuint tex_shader = compile_shader(tex_vert, tex_frag);

    Background b;

    std::vector< std::unique_ptr<Block> > blocks;
    for(int i = 0; i<400; i++) blocks.emplace_back(new Block("dirt.tga"));

    glfwShowWindow(window);

    time_t current_time = time(NULL);

    do
    {
        glfwGetWindowSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClear(GL_COLOR_BUFFER_BIT);

        b.Draw(&bg);

        for(int i = 0; i < blocks.size(); i++)
            blocks[i]->Draw(tex_shader);

        if(time(NULL) - current_time == 1)
        {
            current_time = time(NULL);
            while(blocks.size() > 0) blocks.erase(blocks.end()-1);
            for(int i = 0; i<400; i++) blocks.emplace_back(new Block("dirt.tga"));
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    while( !glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE));

    glfwDestroyWindow(window);

    return 0;
}
