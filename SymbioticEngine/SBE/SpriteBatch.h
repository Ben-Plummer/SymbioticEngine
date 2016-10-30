#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include "Vertex.h"
#include <vector>

namespace sbe {

struct Glyph {
public:
	GLuint texture;
	float depth;

	Vertex topLeft;
	Vertex topRight;
	Vertex bottomLeft;
	Vertex bottomRight;
};

struct GlyphBatch {
	GlyphBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) :
		offset(Offset),
		numVertices(NumVertices),
		texture(Texture) { }
	GLuint offset;
	GLuint numVertices;
	GLuint texture;
};

class SpriteBatch
{
public:
	SpriteBatch();
	~SpriteBatch();
	void initialize();

	void startUsing();
	void stopUsing();

	void draw(const glm::vec4& destRect, GLuint texture, const glm::vec4& uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), const Colour& colour = Colour(255,255,255,255), float depth = 1.0f);
	void renderBatch();

private:
	void createGlyphBatches();
	void createVertexArray();
	void sortGlyphs();

	static bool compareTexture(Glyph* a, Glyph* b);

	GLuint _vboID;
	GLuint _vaoID;
	std::vector<Glyph*> _glyphs;
	std::vector<GlyphBatch> _glyphBatches;
};

}