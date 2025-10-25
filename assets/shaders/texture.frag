#version 330 core
precision mediump float;

in vec2 UV;

out vec4 FragColor;

uniform sampler2D u_texture;

void main() {
    vec4 color = texture(u_texture, UV);
    FragColor = color;
}