#include "string_evolver.h"
#include <algorithm>
#include <cmath>
#include <cassert>
//#define RANDOM_VALID_ELEMENT (rand() % 26) + 65;		// Valid ASCII code's for candidate elements - uppercase letters only
#define RANDOM_VALID_ELEMENT (rand() % 94) + 32;		// Valid ASCII code's for candidate elements - entire printable ascii set
const int COST_PER_LETTER_MISMATCH = 94;				// Fitness cost when incorrect number of letters

StringEvolver::StringEvolver(const std::string &theGoal, const uint thePopulationSize )
	: mGoal(theGoal), mGenerationCount(0), mCrossoverProbability(1.0f), mMutationProbability(0.01f)
{
	mPopulation.reserve(thePopulationSize);
	mPopulation.resize(thePopulationSize);
	InitialisePopulation();
	// Partially sort population so breeders are at front for next breeding pass
	std::partial_sort(mPopulation.begin(), mPopulation.begin() + mPopulation.size()/5, mPopulation.end());
}

StringEvolver::~StringEvolver()
{
	mPopulation.clear();
}

void StringEvolver::AdvanceGeneration()
{
	PerformMutation();
	// Breed in pairs
	for(unsigned int i = 0; i < mPopulation.size()/5; i += 2)
	{
		if((rand() % 100) < static_cast<int>(mCrossoverProbability * 100.0f))
		{
			PerformMultipleCrossover(&mPopulation[i], &mPopulation[i+1]);
		}
	}

	// Partially sort population so breeders are at front for next breeding pass
	std::partial_sort(mPopulation.begin(), mPopulation.begin() + mPopulation.size()/5, mPopulation.end());
	// Shuffle breeders to ensure breeding pairs are unique each generation
	// Slows down evolution horrendously -- prevents best individuals breeding with each other, best 20% breeds randomly
	//std::random_shuffle(mPopulation.begin()  + 1, mPopulation.begin() + mPopulation.size()/5);

	mGenerationCount++;
}

bool StringEvolver::IsFullyEvolved() const
{
	return ( GetFittest() == mGoal );
}

const std::string StringEvolver::GetFittest() const
{
	return std::string(mPopulation[0].str);
}

void StringEvolver::InitialisePopulation()
{
	assert(mGoal.length() > 0);

	int candidate_size = mGoal.length();	
	// Initialise all candidates to correct size and null terminate c-strings
	for(popIter it = mPopulation.begin(); it != mPopulation.end(); ++it)
	{
		it->str = new char[candidate_size + 1];	// Candidate size is equal to goal + 1 for null terminator
		it->str[candidate_size] = '\0';
		for(int i = 0; i < candidate_size; ++i)
		{
			it->str[i] = RANDOM_VALID_ELEMENT;
		}
		CalculateFitness(&(*it));
	}
	// Put the fittest initial candidate at the front - so we can easily GetFittest()
	partial_sort(mPopulation.begin(), mPopulation.begin() + 1, mPopulation.end());
}

void StringEvolver::PerformCrossover(Candidate *mum, Candidate *dad)
{
	assert(mum != nullptr && dad != nullptr);
	assert(mum->str != nullptr && dad->str != nullptr);
	assert(strlen(mum->str) == strlen(dad->str));

	// Crossover point is at least a quarter from end
	const int crossover_point = rand() % (strlen(mum->str) - 1);
	char *temp = new char[crossover_point];

	// Swap everything to left of crossover point between mum and dad
	memcpy(temp, mum->str, crossover_point);
	memcpy(mum->str, dad->str, crossover_point);
	memcpy(dad->str, temp, crossover_point);

	delete [] temp;

	// Calculate the fitness of these evolved children
	CalculateFitness(dad);
	CalculateFitness(mum);
}

void StringEvolver::PerformMultipleCrossover( Candidate *mum, Candidate *dad )
{
	assert(mum != nullptr && dad != nullptr);
	assert(mum->str != nullptr && dad->str != nullptr);
	assert(strlen(mum->str) == strlen(dad->str));

	// Two random points within genome
	int crossover_start = rand() % (strlen(mum->str) - 1);
	int crossover_end = rand() % (strlen(mum->str));

	// Ensure start is first
	(crossover_start > crossover_end) ? std::swap(crossover_start, crossover_end) : 0;
	const size_t bytes_to_cross = crossover_end - crossover_start;

	char *mum_start = mum->str + crossover_start;
	char *dad_start = dad->str + crossover_start; 

	assert(strlen(mum_start) == strlen(dad_start));

	// Swap everything between crossover points
	char *temp = new char[bytes_to_cross];

	memcpy(temp, mum_start, bytes_to_cross);
	memcpy(mum_start, dad_start, bytes_to_cross);
	memcpy(dad_start, temp, bytes_to_cross);

	delete [] temp;

	// Calculate the fitness of these evolved children
	CalculateFitness(dad);
	CalculateFitness(mum);
}

void StringEvolver::PerformCutSpliceCrossover( Candidate *mum, Candidate *dad )
{
	assert(mum != nullptr && dad != nullptr);
	assert(mum->str != nullptr && dad->str != nullptr);

	const int size_of_mum = strlen(mum->str);
	const int size_of_dad = strlen(dad->str);

	assert(size_of_mum > 0 && size_of_dad > 0);

	// Random crossover point for each breeder - this is also equal to size of everything to left of this point in bytes
	const int crossover_mum = rand() % (size_of_mum - 1);
	const int crossover_dad = rand() % (size_of_dad - 1);

	// Allocate space for the children - they are likely to change length after crossover
	// Child is made up of left of one parent and right of other, crossover part is left part
	const size_t mum_right = size_of_mum - crossover_mum;	// Size of part to the right of crossover point
	const size_t dad_right = size_of_dad - crossover_dad;
	const size_t size_of_first_child = crossover_dad + mum_right;
	const size_t size_of_second_child = crossover_mum + dad_right;

	char *first_child = new char[size_of_first_child + 1];
	char *second_child = new char[size_of_second_child + 1];
	
	// Null terminate!
	first_child[size_of_first_child] = '\0';
	second_child[size_of_second_child] = '\0';

	// Create children by splicing in respective parts of parents
	memcpy(first_child, dad->str, crossover_dad);		// First part of first child made up of everything to left of father crossover point
	memcpy(first_child + crossover_dad, mum->str + crossover_mum, mum_right);

	memcpy(second_child, mum->str, crossover_mum);
	memcpy(second_child + crossover_mum, dad->str + crossover_dad, dad_right);

	assert(strlen(first_child) == size_of_first_child);
	assert(strlen(second_child) == size_of_second_child);

	// Add children to population by swapping them in for their parents
	std::swap(dad->str, first_child);
	std::swap(mum->str, second_child);

	// Cleanup parents (have been swapped into children pointers)
	delete [] first_child;
	delete [] second_child;
}

void StringEvolver::PerformMutation()
{
	// 1 % chance of mutation on any candidate
	for(popIter it = mPopulation.begin(); it != mPopulation.end(); ++it)
	{
		if((rand() % 100) < static_cast<int>(mMutationProbability * 100.0f))
		{
			ApplyMutation(&(*it));
		}
	}
}

void StringEvolver::ApplyMutation( Candidate *individual )
{
	assert(individual != nullptr && individual->str != nullptr);

	const int random_part = rand() % strlen(individual->str);
	individual->str[random_part] = RANDOM_VALID_ELEMENT;
	CalculateFitness(individual);
}

void StringEvolver::CalculateFitness(Candidate *candidate )
{
	// Fitness is determined by the euclidean distance of each candidate character 
	// from corresponding goal character
	int distance = 0;
	
	int candidate_size = strlen(candidate->str);
	int compare_amount = std::min(static_cast<int>(mGoal.size()), candidate_size);

	for(int i = 0; i < compare_amount; ++i)
	{
		distance += abs(mGoal[i] - candidate->str[i]);
	}

	// Add a cost for incorrect amount of letters
	int letter_mismatch = std::abs(static_cast<int>(mGoal.size()) - candidate_size);
	distance += letter_mismatch * COST_PER_LETTER_MISMATCH; 
	
	candidate->fitness = distance;
}

const int StringEvolver::GetBestFitness() const
{
	return mPopulation[0].fitness;
}

void StringEvolver::SetCrossoverProbability( const float p )
{
	assert(p > 0.0f);

	(p > 1.0f) ? mCrossoverProbability = 1.0f : mCrossoverProbability = p;
}

void StringEvolver::SetMutationProbability( const float p )
{
	assert(p > 0.0f);

	(p > 1.0f) ? mMutationProbability = 1.0f : mMutationProbability = p;
}
