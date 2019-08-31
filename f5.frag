#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in float A;
uniform sampler2D mainTexture;

void main()
{
    FragColor = texture(mainTexture, TexCoord);
    FragColor.a *= A;
}
