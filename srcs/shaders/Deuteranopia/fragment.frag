#version 400 core

in vec3 fragmentColor;
in vec2 fragmentTexCoord;

out vec4 screenColor;

uniform float		RenderTexture;
uniform sampler2D	textureDiffuse;

// Function to adjust colors for Deuteranopia
vec4 adjustForDeuteranopia(vec4 color) {
    float newR = 0.625 * color.r + 0.375 * color.g;  // Shift red towards green
    float newG = 0.7 * color.g + 0.3 * color.b;      // Shift green towards blue
    float newB = color.b;                            // Blue stays unchanged
    
    return vec4(newR, newG, newB, color.a);  // Return the adjusted color
}

void main() {
	vec4 used_texture = texture(textureDiffuse, fragmentTexCoord);
	vec4 deuteranopia_texture = adjustForDeuteranopia(used_texture);
	screenColor = mix(vec4(fragmentColor, 1), deuteranopia_texture, RenderTexture);
}