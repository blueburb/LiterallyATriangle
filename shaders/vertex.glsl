#version 460

layout(location = 0) in vec3 Pos;
uniform float uTime;
uniform mat4 transformationMatrix;
out float fragZ;
out float time;


void main(){
    gl_Position = transformationMatrix * vec4(Pos,1.0);
    fragZ = gl_Position.z;
    time = uTime;
}