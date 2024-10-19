#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D groundTex;

void main()
{
    FragColor = texture(groundTex, TexCoord);
} 
