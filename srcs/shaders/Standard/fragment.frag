#version 400 core

in vec3 fragmentColor;
in vec2 fragmentTexCoord;

out vec4 screenColor;

uniform float		RenderTexture;
uniform sampler2D	texture_diffuse;

void main() {
	vec4 used_texture = texture(texture_diffuse, fragmentTexCoord);
	screenColor = mix(vec4(fragmentColor, 1), used_texture, RenderTexture);
}