#version 330 core

out vec4 FragColor;

in vec3 localPosition;

uniform samplerCube envMap;
uniform float roughness;

const float PI = 3.14159265359;
const uint SAMPLE_COUNT = 1024u;

// Hammersely sequence helps generate Low-Discrepancy Sequence
float RadicalInverse_VdC(uint bits)
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	
	return float(bits) * 2.3283064365386963e-10;
}
// Get a random sample over the total of N samples
vec2 Hammersley(uint i, uint N) {
	return vec2(float(i) / float(N), RadicalInverse_VdC(i));	
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
	float a = roughness*roughness;
	float phi = 2.0 * PI * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
	// from spherical coordinates to cartesian coordinates
	vec3 H;
	H.x = cos(phi) * sinTheta;
	H.y = sin(phi) * sinTheta;
	H.z = cosTheta;
	// from tangent-space vector to world-space sample vector
	vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);
	vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
	
	return normalize(sampleVec);
}

float DistributionGGX(float NdotH, float roughness) {
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH2 = NdotH * NdotH;
	float num = a2;
	float denom = NdotH2 * (a2 - 1.0) + 1.0;
	denom = PI * pow(denom, 2);

	return num / denom;
}

void main() 
{
	vec3 N = normalize(localPosition);
	vec3 R = N;
	vec3 V = R;

	float totalWeight = 0.0;
	vec3 prefilteredColor = vec3(0.0);
	for (uint i = 0u; i < SAMPLE_COUNT; i++) {
		// Get a random sample vector
		vec2 Xi = Hammersley(i, SAMPLE_COUNT);
		// Bias the sample vector to the Specular Lope
		vec3 H = ImportanceSampleGGX(Xi, N, roughness);		
		vec3 L = normalize(2.0 * dot(V, H) * H - V);

		float NdotL = max(dot(N, L), 0.0);
		if (NdotL > 0.0) {
			// Sample the environment map with a specified mipmap level
			float NdotH = max(dot(N, H), 0.0);
			float HdotV = max(dot(H, V), 0.0);
			float D = DistributionGGX(NdotH, roughness);
			float pdf = (D * NdotH / (4.0 * HdotV)) + 0.0001;
			float resolution = 512.0; // source cubemap perface resolution
			float saTexel = 4.0 * PI / (6.0 * resolution * resolution);
			float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);
			float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);

			prefilteredColor += textureLod(envMap, L, mipLevel).rgb * NdotL;
			totalWeight += NdotL;
		}
	}

	prefilteredColor /= totalWeight;

	FragColor = vec4(prefilteredColor, 1.0);
}