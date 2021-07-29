#version 330 core

uniform sampler2D debugTexture;

out vec4 FragColor;

void main() {
	vec2 texSize = textureSize(debugTexture, 0).xy;
	vec2 texCoord = gl_FragCoord.xy / texSize;

	FragColor = vec4(texture(debugTexture, texCoord).xyz, 1.0f);
}