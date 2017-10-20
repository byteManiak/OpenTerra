#ifndef SHADERS_H
#define SHADERS_H

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

#endif // SHADERS_H
