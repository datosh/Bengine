#include "GenericLevel.h"

const int MIN_ROOM_SIZE = 15;
const int PUSH_DELTA = 15;
const int MIN_CORR_SIZE = 7;

// AABB Collision
bool GenericRoom::overlaps(GenericRoom a, GenericRoom b)
{
	auto ax = a.m_position.x;
	auto ay = a.m_position.y;
	auto aX = a.m_dimension.x + ax;
	auto aY = a.m_dimension.y + ay;

	auto bx = b.m_position.x;
	auto by = b.m_position.y;
	auto bX = b.m_dimension.x + bx;
	auto bY = b.m_dimension.y + by;

	if (aX < bx || aY < by || ax > bX || ay > bY)
	{
		return false;
	}
	return true;
}

int GenericRoom::sign(glm::ivec2 p1, glm::ivec2 p2, glm::ivec2 p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

int GenericRoom::distance(GenericRoom a, GenericRoom b)
{
	int deltax = a.m_position.x - b.m_position.x;
	int deltay = a.m_position.y - b.m_position.y;
	return std::sqrt(deltax * deltax + deltay * deltay);
}

glm::ivec2 GenericRoom::center(GenericRoom a)
{
	return glm::ivec2(a.m_position.x + a.m_dimension.x / 2, a.m_position.y + a.m_dimension.x / 2);
}

bool GenericRoom::pointInTriangle(const GenericRoom& r, const GenericRoom& r1, const GenericRoom& r2, const GenericRoom& r3)
{
	bool b1, b2, b3;

	b1 = sign(r.m_position, r1.m_position, r2.m_position) < 0;
	b2 = sign(r.m_position, r2.m_position, r3.m_position) < 0;
	b3 = sign(r.m_position, r3.m_position, r1.m_position) < 0;

	return ((b1 == b2) && (b2 == b3));
}

void GenericLevel::init(size_t numRooms, int maxRoomSize)
{
	m_maxRoomSize = maxRoomSize;

	std::random_device rd;
	std::mt19937 re(rd());
	std::uniform_int_distribution<int> rand(MIN_ROOM_SIZE, maxRoomSize);

	for (size_t i = 0; i < numRooms; ++i)
	{
		glm::ivec2 pos(rand(re), rand(re));
		glm::ivec2 dim(rand(re), rand(re));
		m_rooms.push_back({pos, dim});
	}

	spreadRooms();
	this->printToFile("step1.txt");
	createConnections();
	this->printToFile("step2.txt");
}

void GenericLevel::spreadRooms()
{
	for (auto& r : m_rooms)
	{
		for (auto& other : m_rooms)
		{
			if (&r == &other)
			{
				continue;
			}

			while (GenericRoom::overlaps(r, other))
			{
				if (r.m_position.x < other.m_position.x)
				{
					// push r to left
					r.m_position.x -= PUSH_DELTA;
					other.m_position.x += PUSH_DELTA;
				}
				else
				{
					// push r to right
					r.m_position.x += PUSH_DELTA;
					other.m_position.x -= PUSH_DELTA;
				}

				if (r.m_position.y < other.m_position.y)
				{
					// push r to top
					r.m_position.y -= PUSH_DELTA;
					other.m_position.y += PUSH_DELTA;
				}
				else
				{
					// push r to down
					r.m_position.y += PUSH_DELTA;
					other.m_position.y -= PUSH_DELTA;
				}
			}
		}
	}

	for (const auto& r : m_rooms)
	{
		for (const auto& other : m_rooms)
		{
			if (&r == &other)
			{
				continue;
			}
			if (GenericRoom::overlaps(r, other))
			{
				spreadRooms();
			}
		}
	}
}

void GenericLevel::makeGraph()
{
	std::vector<Triangle> triangles;

	// Create triangle including all rooms
	GenericRoom r0 = { glm::ivec2(std::numeric_limits<int>::min(), std::numeric_limits<int>::max()), glm::ivec2(0) };
	GenericRoom r1 = { glm::ivec2(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()), glm::ivec2(0) };
	GenericRoom r2 = { glm::ivec2(std::numeric_limits<int>::max() / 2, std::numeric_limits<int>::min()), glm::ivec2(0) };

	/*
	GenericRoom& r2 = *std::min_element(m_rooms.begin(), m_rooms.end(), [](const GenericRoom& a, const GenericRoom& b) {
		return a.m_position.y < b.m_position.y;
	}); */

	triangles.push_back({ r0, r1, r2 });
	
	for (auto& r : m_rooms)
	{
		for (auto& t : triangles)
		{
			if (GenericRoom::pointInTriangle(r, t.a, t.b, t.c))
			{
				triangles.push_back({ r, t.a, t.b });
				triangles.push_back({ r, t.b, t.c });
				triangles.push_back({ r, t.c, t.a });
				t.toBeDeleted = true;

				break;
			}
		}

		std::remove_if(triangles.begin(), triangles.end(), [](const Triangle& current) {
			return current.toBeDeleted;
		});
	}

	std::remove_if(triangles.begin(), triangles.end(), [&](const Triangle& t) {
		return &t.a == &r0 || &t.a == &r1 || &t.b == &r0 || &t.b == &r1 || &t.c == &r0 || &t.c == &r1
			|| &t.a == &r2 || &t.b == &r2 || &t.c == &r2;
	});

	return;
}

GenericRoom GenericRoom::roomFromPoints(glm::ivec2 p1, glm::ivec2 p2)
{
	

	int newX = std::min(p1.x, p2.x);
	int newY = std::min(p1.y, p2.y);
	int newW = std::abs(p1.x - p2.x) + MIN_CORR_SIZE;
	int newH = std::abs(p1.y - p2.y) + MIN_CORR_SIZE;

	return{ glm::ivec2(newX, newY), glm::ivec2(newW, newH) };
}

void GenericLevel::createConnections()
{
	std::vector<GenericRoom> corridors;

	std::vector<int> indicies(m_rooms.size());
	for (int i = 0; i < indicies.size(); ++i) {	indicies[i] = i; }
	std::random_shuffle(indicies.begin(), indicies.end());

	for (int i = 0; i < indicies.size(); i += 2)
	{
		glm::ivec2 centerR = GenericRoom::center(m_rooms[indicies[i]]);
		glm::ivec2 centerN = GenericRoom::center(m_rooms[indicies[i+1]]);
		glm::ivec2 destination = glm::ivec2(centerR.x, centerN.y);

		corridors.push_back(GenericRoom::roomFromPoints(centerR, destination));
		corridors.push_back(GenericRoom::roomFromPoints(centerN, destination));
	}

	m_rooms.insert(m_rooms.end(), corridors.begin(), corridors.end());
}

void GenericLevel::printToFile(std::string fileName)
{
	const char WALL = 'R';
	const char FREE = '.';
	const char OOB = '_';

	std::ofstream oStream(fileName);

	if (oStream.fail())
	{
		std::cerr << "PAAAAAAAANIC" << std::endl;
		return;
	}

	int smallestX = 0;
	int smallestY = 0;
	int biggestX = 0;
	int biggestY = 0;

	for (const auto& r : m_rooms)
	{
		if (r.m_position.x < smallestX)
		{
			smallestX = r.m_position.x;
		}
		if (r.m_position.x > biggestX)
		{
			biggestX = r.m_position.x;
		}
		if (r.m_position.y < smallestY)
		{
			smallestY = r.m_position.y;
		}
		if (r.m_position.y > biggestY)
		{
			biggestY = r.m_position.y;
		}
	}
	for (auto& r : m_rooms)
	{
		r.m_position.x += std::abs(smallestX);
		r.m_position.y += std::abs(smallestY);
	}
	biggestX += std::abs(smallestX) + m_maxRoomSize;
	biggestY += std::abs(smallestY) + m_maxRoomSize;



	std::vector<std::vector<char>> ASCIILevel;
	ASCIILevel.resize(biggestX);
	for (auto& v : ASCIILevel)
	{
		v.resize(biggestY);
		std::fill(v.begin(), v.end(), OOB);
	}
	

	for (const auto& r : m_rooms)
	{
		for (auto i = r.m_position.x; i < r.m_position.x + r.m_dimension.x; ++i)
		{
			for (auto j = r.m_position.y; j < r.m_position.y + r.m_dimension.y; ++j)
			{
				if (i == r.m_position.x || i == r.m_position.x + r.m_dimension.x - 1 ||
					j == r.m_position.y || j == r.m_position.y + r.m_dimension.y - 1)
				{
					if (ASCIILevel[i][j] == OOB)
					{
						ASCIILevel[i][j] = WALL;
					}
				}
				else
				{
					ASCIILevel[i][j] = FREE;
				}
			}
		}
	}


	oStream << "Humans: 100" << std::endl;
	for (const auto& line : ASCIILevel)
	{
		for (const auto& c : line)
		{
			oStream << c;
		}
		oStream << std::endl;
	}
}