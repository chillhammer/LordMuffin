#pragma once
#include <cstdlib>
#include <ctime>
namespace Skel
{
	// Static class to hold random number functions and seed
	class Random
	{
	public:
		static void Seed(int seed = 0)
		{
			srand(0);
		}

		static float Float1()
		{
			return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		}

		// Rolls dice, returns whether 0-1 float is below or equal to chance
		static bool Roll(float chance)
		{
			return Float1() <= chance;
		}

		static float Float(float num)
		{
			return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / num));
		}
	};
}