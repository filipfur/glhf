#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aUV;
layout (location=3) in vec3 aTangent;
layout (location=4) in uvec4 aJoints;
layout (location=5) in vec4 aWeights;

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

out vec2 UV;
out vec3 C;
out vec3 P;
out vec3 L;

void main()
{
    mat4 skinMatrix = u_bones[int(aJoints.x)] * aWeights.x
        + u_bones[int(aJoints.y)] * aWeights.y
        + u_bones[int(aJoints.z)] * aWeights.z
        + u_bones[int(aJoints.w)] * aWeights.w;

    mat4 world = u_model * skinMatrix;

    UV = aUV;

    mat3 normalMatrix = transpose(inverse(mat3(world)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));  

    vec3 p = vec3(world * vec4(aPos, 1.0));
    P = TBN * p;
    L = TBN * vec3(0, 10, 0);
    C = TBN * u_cameraPos;

    
    gl_Position = u_projection * u_view  * vec4(p, 1.0);
}
