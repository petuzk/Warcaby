#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
// in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
// uniform vec4 colDiffuse;

uniform float offset[3] = float[](0.0, 1.3846153846, 3.2307692308);
uniform float weight[3] = float[](0.2270270270, 0.3162162162, 0.0702702703);
uniform float blurWeight = 0.0;
uniform float renderWidth = 800;
uniform float renderHeight = 450;

// Output fragment color
out vec4 finalColor;

float sdSquare(vec2 point, float width) {
	vec2 d = abs(point) - width;
	return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

// https://github.com/TyLindberg/glsl-vignette
float vignette(vec2 uv, vec2 size, float roundness, float smoothness) {
	// Center UVs
	uv -= 0.5;

	// Shift UVs based on the larger of width or height
	float minWidth = min(size.x, size.y);
	uv.x = sign(uv.x) * clamp(abs(uv.x) - abs(minWidth - size.x), 0.0, 1.0);
	uv.y = sign(uv.y) * clamp(abs(uv.y) - abs(minWidth - size.y), 0.0, 1.0);

	// Signed distance calculation
	float boxSize = minWidth * (1.0 - roundness);
	float dist = sdSquare(uv, boxSize) - (minWidth * roundness);

	return 1.0 - smoothstep(0.0, smoothness, dist);
}

void main() {
	vec4 texelColor = texture(texture0, fragTexCoord);

	if (blurWeight != 0.0) {
		texelColor = texelColor * ((1 - blurWeight) * (1 - weight[0]) + weight[0]);

		for (int i=1; i<3; i++) {
			texelColor += texture(texture0, (vec2(fragTexCoord) + vec2(0.0, offset[i]/renderHeight)*3)) * blurWeight * weight[i] / 2;
			texelColor += texture(texture0, (vec2(fragTexCoord) - vec2(0.0, offset[i]/renderHeight)*3)) * blurWeight * weight[i] / 2;
			texelColor += texture(texture0, (vec2(fragTexCoord) + vec2(offset[i]/renderWidth,  0.0)*3)) * blurWeight * weight[i] / 2;
			texelColor += texture(texture0, (vec2(fragTexCoord) - vec2(offset[i]/renderWidth,  0.0)*3)) * blurWeight * weight[i] / 2;
		}
	}

	float vignetteWeight = 0.1;
	float vignette = (vignette(fragTexCoord, vec2(0.3, 0.3), 1.0, 0.5) + 1.0/vignetteWeight - 1.0) * vignetteWeight;

	// Calculate final fragment color
	finalColor = texelColor * vignette;
}