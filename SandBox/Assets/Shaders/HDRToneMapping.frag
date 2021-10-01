#version 430 core
out vec4 FragColor;

in vec2 fragTexcoords;

uniform sampler2D hdrBuffer;
uniform float exposure;

void main()
{             
    vec3 hdrColor = texture(hdrBuffer, fragTexcoords).rgb;
	vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // gamma correction 
	 mapped = pow(mapped, vec3(1.0 / 1.2));
    FragColor = vec4(mapped,1.0f);
}   