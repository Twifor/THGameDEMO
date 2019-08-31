#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D mainTexture;
uniform bool isBlur;

vec4 blur(vec2 _uv) {
    float disp = 0.;
    float intensity = .2;
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

void main()
{
    FragColor = texture(mainTexture, TexCoord);
    if(isBlur)FragColor = blur(TexCoord);
}
