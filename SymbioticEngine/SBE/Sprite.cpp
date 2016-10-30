#include "Sprite.h"
#include "Vertex.h"
#include "ResourceManager.h"

#include <cstddef> //For offsetof

namespace sbe {

//Constructor initializes the vboID to 0
Sprite::Sprite() : _vboID(0) {

}

Sprite::~Sprite() {
	//Deletes buffer associated with the vboID if present
	if (_vboID != 0) {
		glDeleteBuffers(1, &_vboID);
	}
}

void Sprite::initialize(float x, float y, float width, float height, std::string textureFilePath) {
	//Initialize private variables
	_x = x;
	_y = y;
	_width = width;
	_height = height;
	_texture = ResourceManager::getTexture(textureFilePath);

	//Generate virtual buffer object
	if (_vboID == 0) {
		glGenBuffers(1, &_vboID);
	}

	//Create 6 vertexs for two triangles to make rectangle
	Vertex vertices[6];
	//First triangle
	vertices[0].position.set(x + width, y + height);	//Top right
	vertices[0].uv.set(1.0f, 1.0f);

	vertices[1].position.set(x, y + height);			//Top left
	vertices[1].uv.set(0.0f, 1.0f);

	vertices[2].position.set(x, y);						//Bottom left
	vertices[2].uv.set(0.0f, 0.0f);

	//Second triangle
	vertices[3].position.set(x, y);						//Bottom left
	vertices[3].uv.set(0.0f, 0.0f);

	vertices[4].position.set(x + width, y);				//Bottom right
	vertices[4].uv.set(1.0f, 0.0f);

	vertices[5].position.set(x + width, y + height);	//Top right
	vertices[5].uv.set(1.0f, 1.0f);

	//Sets every vertex to magenta
	for (int i = 0; i < 6; i++) {
		vertices[i].colour.set(255, 0, 255, 255);
	}

	vertices[1].colour.set(0, 0, 255, 255);
	vertices[4].colour.set(255, 0, 0, 255);

	//Bind array buffer to the _vboID
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	//Upload data to the array buffer in the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices /*Arrays can be pointers*/, GL_STATIC_DRAW);

	//Unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::draw() {
	//If texture hasnt been bound, it is
	glBindTexture(GL_TEXTURE_2D, _texture.id);

	//Bind array buffer to the _vboID
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);

	//Tell OpenGL what attribute arrays we need (2 for position and colour)
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//Tell OpenGL where attribute pointer for position is
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	//Tell OpenGL where attribute pointer for colour is
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, colour));
	//Tell OpenGL where attribute pointer for uv is
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	//Draw 6 vertices with triangles
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Disable vertex attrib arrays to free up GPU space
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	//Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}