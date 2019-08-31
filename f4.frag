#version 330 core
in vec2 TexCoord;
in float A;

uniform sampler2D mainTexture;

out vec4 FragColor;

void main(){
    FragColor = texture(mainTexture,TexCoord);
    FragColor.a *= A;
}
