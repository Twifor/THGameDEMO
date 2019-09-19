#version 330 core
in vec2 TexCoord;
in float lock;

uniform sampler2D mainTexture;
uniform vec2 alpha;
uniform bool isBlur;
uniform bool isBlack;

uniform float totAlpha;
vec4 blur(vec2 _uv) {
    float disp = 0.;
    float intensity = .8;
    const int passes = 6;
    vec4 c1 = vec4(0.0);
    disp = intensity*(0.5-distance(0.5, .1));

    for (int xi=0; xi<passes; xi++) {
        float x = float(xi) / float(passes) - 0.5;
        for (int yi=0; yi<passes; yi++) {
            float y = float(yi) / float(passes) - 0.5;
            vec2 v = vec2(x, y);
            float d = disp;
            c1 += texture(mainTexture, _uv + d*v);
        }
    }
    c1 /= float(passes*passes);
    return c1;
}
out vec4 FragColor;

void main(){
    if(isBlur)FragColor = blur(TexCoord);
    else FragColor = texture(mainTexture,TexCoord);
    FragColor.a = FragColor.a * alpha.x;
    if(lock < 0.0)FragColor.a = 0;
    FragColor.a *= totAlpha;
    if(isBlack){
        FragColor.r = FragColor.r * 0.5f;
        FragColor.g = FragColor.g * 0.5f;
        FragColor.b = FragColor.b * 0.5f;
    }

}
