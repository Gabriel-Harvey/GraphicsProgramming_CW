//Version number
#version 400
//Layout Qualifer
layout( location = 0 ) out vec4 fragcolor;
//Unfrom variable

uniform sampler2D diffuse;

in vec2 TexCoords;

void main()
{
//Setting each vector component to uniform varaible then setting final colour

	vec4 colour = texture2D(diffuse, TexCoords);
    fragcolor = colour;
}