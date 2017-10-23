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
        glfwCreateWindow(vid_modes[1].width,
                         vid_modes[1].height, // last video mode in the array is the maximum monitor resolution
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    bg = compile_shader(vert, frag);
    tex_shader = compile_shader(tex_vert, tex_frag);
    text_shader = compile_shader(tex_vert, text_frag);

    FT_Init_FreeType(&ftlib);

    glClearColor(0, .4, .7, 1);
    /* in case something fails, the background or the entire screen will be cyan
     * in case something fails *really* badly, the screen will be black
     */

    int w, h;

    Background b;

    std::vector< std::unique_ptr<Block> > blocks;
    for(int i = -10; i<10; i++)
        for(int j = -10; j < 10; j++)
        {
            if((i > -10 && i < 9) && (j > -10 && j < 9)) continue;
            blocks.emplace_back(new Block("block.tga", i*32, j*32, 32, 32,
                                          (i+10)*32/360.f, (j+10)*32/360.f, 1));
        }

    glfwShowWindow(window);

    Text text("S", -5, 0);

    resolution = new GLfloat[2];

    do
    {
        glfwGetWindowSize(window, &w, &h);
        glViewport(0, 0, w, h);
        resolution[0] = w; resolution[1] = h;
        glClear(GL_COLOR_BUFFER_BIT);

        b.Draw();

        for(int i = 0; i < blocks.size(); i++)
            blocks[i]->Draw();

        text.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    while( !glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE));

    glfwDestroyWindow(window);

    return 0;
}
