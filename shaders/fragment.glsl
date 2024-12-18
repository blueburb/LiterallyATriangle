#version 460


out vec4 FragColor;
in float fragZ;
in float time;

void main() {
    FragColor = vec4(0.5*(sin((time+2)*1.8+1)), 0.5*(sin(time)+1.0), 0.5*(cos(1.3*time)+1), 1.0)/fragZ;
}