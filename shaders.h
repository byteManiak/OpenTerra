#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>
#include <GL/gl.h>

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
    "uniform vec2 resolution;"
    "out vec2 texC;\n"
    "void main()"
    "{ texC = texCoord; gl_Position = vec4(vec2(coord.x, -coord.y)/resolution*2, 1, 1);\n }"
};

const char* tex_frag[] =
{
    "#version 330 core\n"
    "uniform sampler2D tex;"
    "in vec2 texC;"
    "uniform vec4 color;\n"
    "void main()"
    "{ gl_FragColor = texture(tex, texC) * color;\n }"
};

const char *text_frag[] =
{
    "#version 330 core\n"
    "uniform sampler2D tex;"
    "in vec2 texC;"
    "void main()"
    "{ gl_FragColor = vec4( vec3(1), texture(tex, texC).r );\n }"
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

GLuint bg, tex_shader, text_shader;

GLfloat *resolution;

#endif // SHADERS_H
