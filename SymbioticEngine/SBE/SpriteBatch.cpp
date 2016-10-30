#include "SpriteBatch.h"
#include <algorithm>

namespace sbe {

SpriteBatch::SpriteBatch() :
	_vboID(0),
	_vaoID(0) {

}

SpriteBatch::~SpriteBatch() {

}

void SpriteBatch::initialize() {
	createVertexArray();
}

void SpriteBatch::startUsing() {
	//Clear vectors of glyphs and batches
	_glyphs.clear();
	_glyphBatches.clear();
}

void SpriteBatch::stopUsing() {
	sortGlyphs();
	createGlyphBatches();
}

void SpriteBatch::draw(const glm::vec4& destRect, GLuint texture, const glm::vec4& uvRect, const Colour& colour, float depth) {
	Glyph* newGlyph = new Glyph;
	newGlyph->texture = texture;
	newGlyph->depth = depth;

	newGlyph->topLeft.colour = colour;
	newGlyph->topLeft.position.set(destRect.x, destRect.y + destRect.w);
	newGlyph->topLeft.uv.set(uvRect.x, uvRect.y + uvRect.w);

	newGlyph->topRight.colour = colour;
	newGlyph->topRight.position.set(destRect.x + destRect.z, destRect.y + destRect.w);
	newGlyph->topRight.uv.set(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

	newGlyph->bottomLeft.colour = colour;
	newGlyph->bottomLeft.position.set(destRect.x, destRect.y);
	newGlyph->bottomLeft.uv.set(uvRect.x, uvRect.y);

	newGlyph->bottomRight.colour = colour;
	newGlyph->bottomRight.position.set(destRect.x + destRect.z, destRect.y);
	newGlyph->bottomRight.uv.set(uvRect.x + uvRect.z, uvRect.y);

	_glyphs.push_back(newGlyph);
}

void SpriteBatch::renderBatch() {
	//Bind vertex array
	glBindVertexArray(_vaoID);
	//Loop through render batches and draw them
	for (size_t i = 0; i < _glyphBatches.size(); i++) {
		//Bind the batch texture
		glBindTexture(GL_TEXTURE_2D, _glyphBatches[i].texture);
		//Draw it
		glDrawArrays(GL_TRIANGLES, _glyphBatches[i].offset, _glyphBatches[i].numVertices);
	}
	//Unbind vertex array
	glBindVertexArray(0);
}

void SpriteBatch::createGlyphBatches() {
	std::vector<Vertex> vertices;
	vertices.resize(_glyphs.size() * 6);


	if (_glyphs.empty()) {
		return;
	}

	
	//Tracks which vertex is being added
	int currentVertex = 0;
	int offset = 0;
	//First render batch
	_glyphBatches.emplace_back(offset, 6, _glyphs[0]->texture);
	//Adds six vertexs to batch for sqaure sprite
	vertices[currentVertex++] = _glyphs[offset]->topLeft;
	vertices[currentVertex++] = _glyphs[offset]->bottomLeft;
	vertices[currentVertex++] = _glyphs[offset]->bottomRight;
	vertices[currentVertex++] = _glyphs[offset]->bottomRight;
	vertices[currentVertex++] = _glyphs[offset]->topRight;
	vertices[currentVertex++] = _glyphs[offset]->topLeft;
	offset += 6;

	for (size_t currentGlyph = 1; currentGlyph < _glyphs.size(); currentGlyph++) {
		if (_glyphs[currentGlyph]->texture != _glyphs[currentGlyph - 1]->texture) {
			_glyphBatches.emplace_back(offset, 6, _glyphs[currentGlyph]->texture);
		}
		else {
			_glyphBatches.back().numVertices += 6;
		}
		vertices[currentVertex++] = _glyphs[currentGlyph]->topLeft;
		vertices[currentVertex++] = _glyphs[currentGlyph]->bottomLeft;
		vertices[currentVertex++] = _glyphs[currentGlyph]->bottomRight;
		vertices[currentVertex++] = _glyphs[currentGlyph]->topLeft;
		vertices[currentVertex++] = _glyphs[currentGlyph]->topRight;
		vertices[currentVertex++] = _glyphs[currentGlyph]->bottomRight;
		offset += 6;
	}

	//Bind array buffer to our vboID
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	//Orphans the buffer
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	//Upload the data
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
	//Unbind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteBatch::createVertexArray() {
	//Generate vertex array
	if (_vaoID == 0) {
		glGenVertexArrays(1, &_vaoID);
	}
	//Bind vertex array so everything from now is stored inside it
	glBindVertexArray(_vaoID);

	//Generate virtual buffer object
	if (_vboID == 0) {
		glGenBuffers(1, &_vboID);
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vboID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, colour));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	//Unbinds and disables everything and stop adding glCalls to the VAO
	glBindVertexArray(0);
}

void SpriteBatch::sortGlyphs() {
	std::stable_sort(_glyphs.begin(), _glyphs.end(), compareTexture);
}

bool SpriteBatch::compareTexture(Glyph* a, Glyph* b) {
	return (a->texture < b->texture);
}

}