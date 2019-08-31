#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float alpha;

out vec2 TexCoord;
out float A;
void main()
{
    gl_Position = vec4(aPos, 1.0);
    A = alpha;
    TexCoord = aTexCoord;
}
