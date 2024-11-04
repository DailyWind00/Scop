#version 400 core

in vec3 fragmentColor;
in vec2 fragmentTexCoord;

out vec4 screenColor;

uniform float		RenderTexture;
uniform sampler2D	textureDiffuse;

// Function to adjust colors for Tritanopia
vec4 adjustForTritanopia(vec4 color) {
    float newR = 0.95 * color.r + 0.05 * color.g;  // Slight shift in red channel
    float newG = 0.0 * color.r + 0.43 * color.g + 0.57 * color.b;  // Major adjustment in green channel
    float newB = 0.0 * color.r + 0.475 * color.g + 0.525 * color.b;  // Small shift in blue channel
    
    return vec4(newR, newG, newB, color.a);  // Return the adjusted color
}

void main() {
	vec4 used_texture = texture(textureDiffuse, fragmentTexCoord);
	vec4 tritanopia_texture = adjustForTritanopia(used_texture);
	screenColor = mix(vec4(fragmentColor, 1), tritanopia_texture, RenderTexture);
}