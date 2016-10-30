#pragma once
#include <GL/glew.h>

namespace sbe {

struct Position {
	Position() { }
	Position(float X, float Y) : x(X), y(Y) { }
	void set(float X, float Y) {
		x = X;
		y = Y;
	}
	float x, y;
};

struct Colour {
	Colour() { }
	Colour(GLubyte R, GLubyte G, GLubyte B, GLubyte A) : r(R), g(G), b(B), a(A) { }
	void set(GLubyte R, GLubyte G, GLubyte B, GLubyte A) {
		r = R;
		g = G;
		b = B;
		a = A;
	}
	GLubyte r, g, b, a;
};

struct UV {
	UV() { };
	UV(float U, float V) : u(U), v(V) { }
	void set(float U, float V) {
		u = U;
		v = V;
	}
	float u, v;
};

struct Vertex {
	Position position;
	Colour colour;
	UV uv;
};

}