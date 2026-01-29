#version 330 core
precision mediump float;

in vec3 N;

out vec4 FragColor;

void main() {
    vec3 color = N;
    for(int i=0; i < 3; ++i) {
        if(N[i] < 0) {
            color += vec3(1, 1, 1);
            break;
        } 
    }
    FragColor = vec4(color, 1.0);
}