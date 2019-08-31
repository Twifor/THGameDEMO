#version 330 core
in vec2 TexCoord;
in float lock;

uniform sampler2D mainTexture;
uniform vec2 alpha;

out vec4 FragColor;

void main(){
    FragColor = texture(mainTexture,TexCoord);
    FragColor.a = FragColor.a * alpha.x;
    if(lock < 0.0)FragColor.a = 0;
}
