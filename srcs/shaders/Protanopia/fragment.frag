#version 400 core

in vec3 fragmentColor;
in vec2 fragmentTexCoord;

out vec4 screenColor;

uniform float		RenderTexture;
uniform sampler2D	textureDiffuse;

// Function to adjust colors for Protanopia
vec4 adjustForProtanopia(vec4 color) {
    float newR = 0.56667 * color.r + 0.43333 * color.g;  // Shift red heavily towards green
    float newG = 0.55833 * color.g + 0.44167 * color.b;  // Shift green towards blue
    float newB = color.b;                                // Blue remains unchanged
    
    return vec4(newR, newG, newB, color.a);  // Return the adjusted color
}

void main() {
	vec4 used_texture = texture(textureDiffuse, fragmentTexCoord);
	vec4 protanopia_texture = adjustForProtanopia(used_texture);
	screenColor = mix(vec4(fragmentColor, 1), protanopia_texture, RenderTexture);
}