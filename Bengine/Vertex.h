#pragma once

#include <glew.h>

namespace Bengine {

	struct Position {
		float x;
		float y;
	};

	struct ColorRGBA8 {

		ColorRGBA8() :	r(0), g(0), b(0), a(0)	{ }
		ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A) :
			r(R), g(G), b(B), a(A) { }

		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};

	struct UV {
		float u;
		float v;
	};

	struct Vertex {
		struct Position position;
		struct ColorRGBA8 color;
		struct UV uv;

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
}