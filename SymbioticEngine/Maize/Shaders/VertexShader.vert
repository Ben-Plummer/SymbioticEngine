//The vertex shader operates on each vertex
#version 130

//Input data from the VBO. Each vertex is 2 floats and each colour has 4 values, RGBA
in vec2 vertexPosition;
in vec4 vertexColour;
in vec2 vertexUV;

//Output data that is sent to fragment shader
out vec2 fragmentPosition;
out vec4 fragmentColour;
out vec2 fragmentUV;

uniform mat4 cameraMatrix;

void main() {
	//Set the xy position to the vertexPosition multiplied by the camera matrix
	gl_Position.xy = (cameraMatrix * vec4(vertexPosition, 0.0, 1.0)).xy;
	//Set the z position to 0 has we are in 2 dimensions
	gl_Position.z = 0.0;
	//Set w to 1 to indicate normalized coordinates
	gl_Position.w = 1.0;
	
	//Set fragmentPosition to vertexPosition
	fragmentPosition = vertexPosition;
	
	//Set fragmentColour to vertexColour
	fragmentColour = vertexColour;

	//Set fragmentUV to vertexUV flip the coordinates so its the right way round
	fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
}