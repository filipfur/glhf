#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aUV;
layout (location=3) in uvec4 aJoints;
layout (location=4) in vec4 aWeights;

layout (std140) uniform CameraBlock
{
    mat4 u_projection;
    mat4 u_view;
    vec3 u_cameraPos;
};
//uniform mat4 u_projection;
//uniform mat4 u_view;
uniform mat4 u_model;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
layout (std140) uniform SkinBlock {
    mat4 u_bones[MAX_BONES];
};

out vec3 N;
out vec2 UV;
out vec3 C;
out vec3 P;

void main()
{
    mat4 skinMatrix = u_bones[int(aJoints.x)] * aWeights.x
        + u_bones[int(aJoints.y)] * aWeights.y
        + u_bones[int(aJoints.z)] * aWeights.z
        + u_bones[int(aJoints.w)] * aWeights.w;

    mat4 world = u_model * skinMatrix;

    UV = aUV;
    vec3 p = vec3(world * vec4(aPos, 1.0));
    N = normalize(vec3(world * vec4(aNormal, 0.0)));
    C = u_cameraPos;
    P = p;
    
    gl_Position = u_projection * u_view  * vec4(p, 1.0);
}
