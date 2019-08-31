#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;
out vec2 fog;
uniform mat4 projection;
uniform float div;
void main()
{
    gl_Position = projection * vec4(aPos, 1.0);
    gl_Position.y = gl_Position.y * div;
    fog.x=gl_Position.z/25.0;
    fog.y=0;
    TexCoord = aTexCoord;
}
