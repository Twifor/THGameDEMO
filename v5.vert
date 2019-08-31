#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 rotate;
layout (location = 3) in float alpha;

out vec2 TexCoord;
out float A;

mat4 getRotateMatrix(){
    mat4 t = mat4(vec4(1.0,0.0,0.0,0.0),vec4(0.0,1.0,0.0,0.0),vec4(0.0,0.0,1.0,0.0),vec4(rotate.x,rotate.y,0.0,1.0));
    mat4 r = mat4(vec4(cos(rotate.z),sin(rotate.z),0.0,0.0),vec4(-sin(rotate.z),cos(rotate.z),0.0,0.0),vec4(0.0,0.0,1.0,0.0),vec4(0.0,0.0,0.0,1.0));
    return t * r;
}

void main(){
    gl_Position = getRotateMatrix() * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    TexCoord = aTexCoord;
    A = alpha;
}
