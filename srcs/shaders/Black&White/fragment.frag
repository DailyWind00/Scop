#version 400 core

in vec3 fragmentColor;
in vec2 fragmentTexCoord;

out vec4 screenColor;

uniform float		RenderTexture;
uniform sampler2D	textureDiffuse;

void main() {
	vec4 used_texture = texture(textureDiffuse, fragmentTexCoord);
	float shade_texture = 0.2989 * used_texture.r + 0.5870 * used_texture.g + 0.1140 * used_texture.b;
	screenColor = mix(vec4(fragmentColor, 1), vec4(shade_texture), RenderTexture);
}