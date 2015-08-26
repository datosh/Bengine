#pragma once

#include <glew.h>

struct Vertex {
	struct Position {
		float x;
		float y;
	} position;
	struct Color {
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	} color;
	struct UV {
		float u;
		float v;
	} uv;

	void setPosition(float x, float y) {
		position.x = x;
		position.y = y;
	}

	void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}

	void setUV(float u, float v) {
		uv.u = u;
		uv.v = v;
	}
};
