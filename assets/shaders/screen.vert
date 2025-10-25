#version 330 core

layout(location = 0) in vec4 aXYUV;

out vec2 UV;

void main() {
    UV = aXYUV.zw;
    gl_Position = vec4(aXYUV.xy, 0.0, 1.0);
}