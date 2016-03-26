#include "utility.h"

namespace util {
	/// Returns a random integer in range [min, max]
	int randInt(int min, int max) {
		static std::random_device r;
		static std::default_random_engine rand_generator(r());
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(rand_generator);
	}

	/// Returns a random float in range [min, max]
	float randFloat(float min, float max) {
		static std::random_device r;
		static std::default_random_engine rand_generator(r());
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(rand_generator);
	}

	/// Implementas a fuzzy float equal comparison
	bool ffeq(float a, float b, float delta) {
		return std::abs(a - b) < delta;
	}

	/// Implementas a fuzzy float a < b
	bool fflt(float a, float b, float delta) {
		return a + delta < b || ffeq(a, b, delta);
	}

	/// Implementas a fuzzy float a > b
	bool ffgt(float a, float b, float delta) {
		return a > b + delta || ffeq(a, b, delta);
	}

	time get_time()
	{
		return std::chrono::high_resolution_clock::now();
	}

	bool time_dif_gt(time t1, time t2, long long x)
	{
		return std::abs(std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t2).count()) > x;
	}
}
