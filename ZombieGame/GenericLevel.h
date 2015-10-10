#pragma once

#include "stdafx.h"
#include <glm.hpp>

struct GenericRoom {
	static bool overlaps(GenericRoom a, GenericRoom b);
	static bool GenericRoom::pointInTriangle(const GenericRoom& r, const GenericRoom& r1, const GenericRoom& r2, const GenericRoom& r3);
	static int sign(glm::ivec2 p1, glm::ivec2 p2, glm::ivec2 p3);
	static int distance(GenericRoom a, GenericRoom b);
	static GenericRoom roomFromPoints(glm::ivec2 p1, glm::ivec2 p2);

	static glm::ivec2 center(GenericRoom a);

	glm::ivec2 m_position;
	glm::ivec2 m_dimension;
};

typedef std::pair<GenericRoom, GenericRoom> GenericEdge;

struct Triangle {
	Triangle& operator=(const Triangle& other) {
		a = other.a;
		b = other.b;
		c = other.c;

		return *this;
	};

	GenericRoom& a;
	GenericRoom& b;
	GenericRoom& c;
	bool toBeDeleted;
};

struct GenericLevel {
	void init(size_t numRooms, int maxRoomSize);
	void spreadRooms();
	void makeGraph();
	void createConnections();

	void printToFile(std::string fileName);

	std::vector<GenericRoom> m_rooms;
	std::vector<GenericEdge> m_edges;

	int m_maxRoomSize;
};

