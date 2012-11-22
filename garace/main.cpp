#include <string>
#include <iostream>>
#include <vector>
#include <algorithm>
using namespace std;

typedef std::vector<char*>::iterator vec_iter;
#define RANDOM_VALID_ELEMENT (rand() % 26) + 65;		// Valid ASCII code's for candidate elements

const int INITIAL_POP_SIZE = 100;						// Size of candidate population
const int BREEDING_POOL_SIZE = INITIAL_POP_SIZE / 5;	// How many individuals from the population to breed this generation
const char *fully_evolved = "HIDAD";					// This is the final solution we are trying to evolve to.
const int CANDIDATE_SIZE = strlen(fully_evolved);		// Length of candidate string

vector<char*> population;								// The population we are evolving

// Function prototypes
void CreateInitialPopulation(int, int);
char **PerformSelection();		// Selects section of population to breed
void PerformCrossover(char *, char *);
void PerformMutation(char *);
void PrintBest();				// Prints best individual of current population
bool TestForEnd();				// Tests if any member of the population is 'perfect', ie. matches fully evolved example

int main(int argc, int args)
{
	while(true)
	{
		CreateInitialPopulation(INITIAL_POP_SIZE, CANDIDATE_SIZE);
		char** breeding_pool = PerformSelection();
		for(int i = 0; i < BREEDING_POOL_SIZE; i += 2)
		{
			PerformCrossover(breeding_pool[i], breeding_pool[i+1]);
			PerformMutation(breeding_pool[i]);
			PerformMutation(breeding_pool[i+1]);
		}

		delete breeding_pool;
		PrintBest();

		if (TestForEnd() == true)
		{
			break;
		}
	}


	return 0;
}

void CreateInitialPopulation(const int amount, const int size)
{
	for(int i = 0; i < amount; ++i)
	{
		char *candidate = new char[size + 1];
		candidate[size] = '\0';	// null terminater

		// Randomly fill string with valid elements
		for(int j = 0; j < size; ++j)
		{
			candidate[j] = RANDOM_VALID_ELEMENT;
		}
		population.push_back(candidate);
	}
}

int TestFitness(char *candidate)
{
	int distance = 0;
	for(int i = 0; i < CANDIDATE_SIZE; ++i)
	{
		distance += abs(fully_evolved[i] - candidate[i]);
	}
	return distance;
}

// Selects candidates for crossover/mutation
char** PerformSelection()
{
	char **new_pop = new char*[BREEDING_POOL_SIZE];

	for(int i = 0; i < BREEDING_POOL_SIZE; ++i)
	{
		new_pop[i] = population[i];
	}
	// Build new population from top 20% of current
	for(vec_iter it = population.begin() + BREEDING_POOL_SIZE; it != population.end(); ++it)
	{
		char *candidate = *it;
		for (int j = 0; j < BREEDING_POOL_SIZE; ++j)
		{
			if (TestFitness(candidate) < TestFitness(new_pop[j]) )
			{
				new_pop[j] = candidate;
				break;
			}
		}
	}
	return new_pop;
}


void PerformCrossover(char *mummy, char *daddy)
{
	// Crossover point is at least a quarter from end
	int crossover_point = (rand() % CANDIDATE_SIZE) - (CANDIDATE_SIZE/4) + 2;
	char *temp = new char[crossover_point + 1];
	temp[crossover_point + 1] = '\0';
	// Swap everything to left of crossover point between mum and dad
	memcpy(temp, mummy, crossover_point);
	memcpy(mummy, daddy, crossover_point);
	memcpy(daddy, temp, crossover_point);
}
bool TestForEnd()
{
	for(int i = 0; i < population.size(); ++i)
	{
		char *candidate = population[i];
		if (strcmp(candidate, fully_evolved) == 0)
		{
			cout << "Evolution complete! " << fully_evolved << endl;
			cin.get();
			return true;
		}
	}
	return false;
}

void PerformMutation(char *candidate)
{
	if((rand() % 100) == 1)
	{
	const int random_index = rand() % CANDIDATE_SIZE;
	candidate[random_index] = RANDOM_VALID_ELEMENT;
	}
}

//Print current best candidate
void PrintBest()
{
	char *best = nullptr;
	int best_so_far = INT_MAX;
	for(int i = 0; i < population.size(); ++i)
	{
		int fitness = TestFitness(population[i]);
		if(best_so_far > fitness)
		{
			best_so_far = fitness;
			best = population[i];
		}
	}
	cout << "Best so far: " << best << " Has fitness: " << best_so_far << endl;
}