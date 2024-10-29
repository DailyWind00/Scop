#version 400 core

in vec3 fragmentColor;
in vec2 fragmentTexCoord;

out vec4 screenColor;

uniform sampler2D texture_diffuse;

void main() {
	// screenColor = vec4(fragmentColor, 1.0);
	screenColor = texture(texture_diffuse, fragmentTexCoord);
}