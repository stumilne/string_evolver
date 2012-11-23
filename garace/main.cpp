#include <iostream>
#include <time.h>
#include "string_evolver.h"

using std::cout;
using std::endl;
using std::cin;

const char *DEFAULT_EVOLUTION_TARGET = "Hello, world";
const int DEFAULT_POPULATION_SIZE = 10000;

int main(int argc, char *argv[])
{
	char *string_to_evolve = nullptr;
	int population_size = DEFAULT_POPULATION_SIZE;

	// First argument if present is the string to evolve to
	if(argc > 1)
	{
		size_t str_length = strlen(argv[1]) + 1;
		string_to_evolve = new char[str_length];
		strcpy_s(string_to_evolve, str_length, argv[1]);

		// Second argument if present is the population size
		if(argc == 2)
		{
			population_size = atoi(argv[2]);
		}
	}
	else	// Use default string and population size
	{
		size_t str_length = strlen(DEFAULT_EVOLUTION_TARGET) + 1;
		string_to_evolve = new char[str_length];
		strcpy_s(string_to_evolve, str_length, DEFAULT_EVOLUTION_TARGET);
	}

	srand(static_cast<int>( time(NULL) ));	// Seed random number generator

	StringEvolver evolver(string_to_evolve, population_size);

	// Start the genetic algorithm!
	while(!evolver.IsFullyEvolved())
	{
		evolver.AdvanceGeneration();

		// Print best candidate info. every 10 generations
		if(evolver.GetGenerationCount() % 10 == 0)
		{
		cout << "Generation " << evolver.GetGenerationCount() << "- Best candidate: \"" << evolver.GetFittest() <<
			"\" Fitness: " << evolver.GetBestFitness() << "." << endl;
	
		}
	}
	// Evolution complete!
	cout << "Evolution complete! \"" << evolver.GetFittest() << "\" found in " << evolver.GetGenerationCount() << " generations." << endl;
	cout << "Press return to exit" << endl;
	cin.get();

	return 0;
}