#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D floorTexture;

#define NR_POINT_LIGHTS 4
uniform vec3 lightPositions[NR_POINT_LIGHTS];
uniform vec3 lightColors[NR_POINT_LIGHTS];
uniform vec3 viewPos;
uniform bool gamma;

vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor);

void main()
{
    vec3 color = texture(floorTexture, fs_in.TexCoords).rgb;
    vec3 lighting = vec3(0.0);
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        lighting += BlinnPhong(fs_in.Normal, fs_in.Position, lightPositions[i], lightColors[i]);
    }
    color *= lighting;

    if (gamma) {
        color = pow(color, vec3(1/2.2));
    }

    FragColor = vec4(color, 1.0);
}

vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor) 
{
    // ambient
    float ambientStrength = 0.05;
    vec3 ambient = ambientStrength * lightColor;
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm); // negate lightDir because we want the light direction from the light source to the fragment vertices
    int shininess = 64;
    vec3 halfWay = normalize(viewDir + lightDir);
    float spec = pow(max(dot(viewDir, halfWay), 0.0), shininess);
    vec3 specular = spec * lightColor;    
    // Attenuation
    float max_distance = 1.5;
    float distance = length(lightPos - fragPos);
    float attenuation = 1.0 / (gamma ? distance * distance : distance);

    return (ambient + diffuse + specular) * attenuation;
}


