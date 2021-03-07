#version 330 core
out vec4 FragColor; 

uniform sampler2D shadowMap;

void main()
{ 
//    float depthValue = texture2D(shadowMap, TexCoords.xy).x;
    FragColor = vec4(vec3(1.0), 1.0);
}