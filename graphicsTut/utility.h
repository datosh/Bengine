#pragma once

#include <cmath>
#include <chrono>
#include <random>

namespace util {
	/// Returns a random integer in range [min, max]
	int randInt(int min, int max);

	/// Returns a random float in range [min, max]
	float randFloat(float min, float max);

	/// Implementas a fuzzy float equal comparison
	bool ffeq(float a, float b, float delta = 4);

	/// Implementas a fuzzy float a < b
	bool fflt(float a, float b, float delta = 4);

	/// Implementas a fuzzy float a > b
	bool ffgt(float a, float b, float delta = 4);

	typedef std::chrono::steady_clock::time_point time;
	/// Returns the current time
	time get_time();
	/// Tests if the time between two times is greater than x ms
	bool time_dif_gt(time t1, time t2, long long x);
	
}

