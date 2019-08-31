#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec2 fog;
uniform sampler2D mainTexture;
uniform vec2 alpha;

void main()
{
    FragColor = texture(mainTexture, TexCoord);
    FragColor = mix(FragColor,vec4(0.0,0.0,0.0,0.0),fog.x)*alpha.x;
}
