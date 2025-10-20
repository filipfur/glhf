#version 330 core
precision mediump float;

in vec3 N;
in vec2 UV;
in vec3 C;
in vec3 P;

uniform sampler2D u_texture;
uniform vec4 u_color;

layout(location=0) out vec4 FragColor;

void main() {
    const vec3 lightPos = vec3(0, 10, 0);
    const bool blinn = true;

    vec4 albedo = texture(u_texture, UV) * u_color;
    // ambient
    vec3 ambient = 0.05 * albedo.rgb;
    // diffuse
    vec3 lightDir = normalize(lightPos - P);
    vec3 normal = normalize(N);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * albedo.rgb;
    // specular
    vec3 viewDir = normalize(C - P);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }
    vec3 specular = vec3(0.3) * spec; // assuming bright white light color
    FragColor = vec4(ambient + diffuse + specular, 1.0);
    //FragColor = albedo;
    //FragColor = vec4(1,0,0,1);
}