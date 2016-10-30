//The fragment shader operates on every pixel in a given area
#version 130

in vec2 fragmentPosition;
in vec4 fragmentColour;
in vec2 fragmentUV;

//4 component float value that gets outputted for each pixel
out vec4 colour;

uniform sampler2D mySampler;

void main() {
	//Get the colour from the texture sampler using a function called texture()
	vec4 textureColour = texture(mySampler, fragmentUV);
	
	colour = textureColour * fragmentColour;
}