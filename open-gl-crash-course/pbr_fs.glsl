#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

uniform vec3 cameraPos;

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao; // amibient occlusion

#define PI 3.14159265359
#define NR_POINT_LIGHTS 4
uniform vec3 lightPositions[NR_POINT_LIGHTS];
uniform vec3 lightColors[NR_POINT_LIGHTS];

uniform samplerCube irradianceMap;

// Reflection at different angles
vec3 FresnelSchlick(float cosTheta, vec3 F0 /*base reflectivity*/, float roughness) {
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}
// The area of microfacets that aligned with the Halfway vector
float DistributionGGX(vec3 N, vec3 H, float roughness) {
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;
	float num = a2;
	float denom = NdotH2 * (a2 - 1.0) + 1.0;
	denom = PI * pow(denom, 2);

	return num / denom;
}
// Self-shadowing calculating
float GeometrySchlickGGX(float NdotV, float roughness) {
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);

	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);
	
	return ggx1 * ggx2;
}

void main() {
	vec3 N = normalize(Normal);
	vec3 V = normalize(cameraPos - WorldPos); // View direction
	// Reflection radiance
	vec3 Lo = vec3(0.0);
	vec3 F0 = vec3(0.04); // as a non-metallic surface
	F0 = mix(F0, albedo, metallic); // interpolate with the albedo color if metallic surface
	// PBR (Physics-based Rendering)
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		vec3 L = normalize(lightPositions[i] - WorldPos); // Light direction
		vec3 H = normalize(L + V); // halfway vector
		// Distance from the point light to the fragment
		float d = length(lightPositions[i] - WorldPos);
		float attenuation = 1.0 / (d * d);
		vec3 radiance = lightColors[i] * attenuation;
		// F in DFG in term of PBR
		vec3 F = FresnelSchlick(max(dot(V, H), 0.0), F0, roughness);
		// D in DFG in term of PBR
		float NDF = DistributionGGX(N, H, roughness);
		// G in DFG in term of PBR
		float G = GeometrySmith(N, V, L, roughness);
		// Cook-Torance BRDF
		vec3 numerator = NDF * G * F;
		float denominator = 4 * max(dot(V, N), 0.0) * max(dot(L, N), 0.0);
		vec3 specular = numerator / max(denominator, 0.0001) /*prevent division by zero*/;

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic; // nullify kD if surface is metallic - metallic not refract the light

		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}
	
	// Ambient Lighting (IBL)
	vec3 kS = FresnelSchlick(max(dot(V, N), 0.0), F0, roughness);
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	vec3 irradiance = texture(irradianceMap, N).rgb;
	vec3 diffuse = irradiance * albedo;
	vec3 ambient = (kD * diffuse) * ao;

	vec3 color = ambient + Lo;
	// HDR capture
	color = color / (color + vec3(1.0));
	// Gamma correction
	color = pow(color, vec3(1.0 / 2.2));
	
	FragColor = vec4(color, 1.0);
}