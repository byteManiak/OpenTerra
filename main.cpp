#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include "background.h"
#include "block.h"

const char* vert[] =
{
    "#version 330 core\n"
    "out vec4 pos;\n"
    "void main()"
    "{"
    "   const vec4 verts[4] = vec4[4]( vec4(-1, -1, 0, 1),"
    "                                vec4(1, -1, 0, 1),"
    "                                vec4(1, 1, 0, 1),"
    "                                vec4(-1, 1, 0, 1));"
    "   pos = gl_Position = verts[gl_VertexID]; }"
};

const char* frag[] =
{
    "#version 330 core\n"
    "uniform float time;\n"
    "in vec4 pos;\n"
    "void main()"
    "{ gl_FragColor = mix(vec4(.35, 0, .7, 1), vec4(0, 0, .55, 1), pos.y+1); }"
};

const char* tex_vert[] =
{
    "#version 330 core\n"
    "layout (location = 0) in vec2 coord;\n"
    "layout (location = 1) in vec2 texCoord;\n"
    "out vec2 texC;\n"
    "void main()"
    "{ texC = texCoord; gl_Position = vec4(coord, 1, 1);\n }"
};

const char* tex_frag[] =
{
    "#version 330 core\n"
    "uniform sampler2D tex;"
    "in vec2 texC;"
    "void main()"
    "{ gl_FragColor = texture(tex, texC);\n }"
};

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

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    // creating a vertex array object is necessary so we can send vertex data to our shaders

    glClearColor(0, .4, .7, 1);
    /* in case something fails, the background or the entire screen will be cyan
     * in case something fails *really* badly, the screen will be black
     */

    int w, h;

    GLuint bg = compile_shader(vert, frag);
    GLuint tex_shader = compile_shader(tex_vert, tex_frag);

    Background b;

    std::vector<Block*> blocks;
    for(int i = 0; i<400; i++) blocks.push_back(new Block("dirt.tga"));

    glfwShowWindow(window);

    do
    {
        glfwGetWindowSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClear(GL_COLOR_BUFFER_BIT);

        b.Draw(&bg);

        for(auto i : blocks)
            i->Draw(&tex_shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    while( !glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE));

    glfwDestroyWindow(window);

    return 0;
}
