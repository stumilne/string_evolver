#include "string_evolver.h"
#include <algorithm>
#include <cmath>
//#define RANDOM_VALID_ELEMENT (rand() % 26) + 65;		// Valid ASCII code's for candidate elements
#define RANDOM_VALID_ELEMENT (rand() % 94) + 32;		// Valid ASCII code's for candidate elements

StringEvolver::StringEvolver(const std::string &theGoal, const uint thePopulationSize )
	: mGoal(theGoal), mGenerationCount(0)
{
	mPopulation.reserve(thePopulationSize);
	mPopulation.resize(thePopulationSize);
	InitialisePopulation();	
}

StringEvolver::~StringEvolver()
{
	mPopulation.clear();
}

void StringEvolver::AdvanceGeneration()
{
	// Partially sort population so breeders
	std::partial_sort(mPopulation.begin(), mPopulation.begin() + mPopulation.size()/5 /*mBreederCount*/, mPopulation.end());
	// Breed in pairs
	for(int i = 0; i < mPopulation.size()/5; i += 2)
	{
		PerformCrossover(&mPopulation[i], &mPopulation[i+1]);
	}
	PerformMutation();

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
	int candidate_size = mGoal.length() + 1;	// Candidate size is equal to goal + 1 for null terminator
	// Initialise all candidates to correct size and null terminate c-strings
	for(popIter it = mPopulation.begin(); it != mPopulation.end(); ++it)
	{
		it->str = new char[candidate_size];
		it->str[candidate_size - 1] = '\0';
		for(int i = 0; i < candidate_size-1; ++i)
		{
			it->str[i] = RANDOM_VALID_ELEMENT;
		}
		CalculateFitness(&(*it));
	}
	//Ensure every letter in goal word is present at least once in seed population
	for(int i = 0; i < mGoal.size(); ++i)
	{
		int random_candidate = rand() % mPopulation.size();
		int random_index = rand() % mGoal.size();
		mPopulation[random_candidate].str[random_index] = mGoal[i];
	}
	// Put the fittest initial candidate at the front
	partial_sort(mPopulation.begin(), mPopulation.begin() + 1, mPopulation.end());
}

void StringEvolver::PerformCrossover(Candidate *mum, Candidate *dad)
{
	// Crossover point is at least a quarter from end
	int crossover_point = rand() % (mGoal.size() - 1);
	char *temp = new char[mGoal.size()];
	temp[crossover_point + 1] = '\0';

	if(rand() % 100 > 50)
	{
		// Swap everything to left of crossover point between mum and dad
		memcpy(temp, mum->str, crossover_point);
		memcpy(mum->str, dad->str, crossover_point);
		memcpy(dad->str, temp, crossover_point);
	} else
	{
		// Swap everything to the right of crossover point
		memcpy(temp, mum->str + crossover_point, mGoal.size() - crossover_point);
		memcpy(mum->str + crossover_point, dad->str + crossover_point, mGoal.size() - crossover_point);
		memcpy(dad->str + crossover_point, temp, mGoal.size() - crossover_point);
	}
	delete [] temp;

	// Calculate the fitness of these evolved children
	CalculateFitness(dad);
	CalculateFitness(mum);
}

void StringEvolver::PerformMutation()
{
	// 1 % chance of mutation on any candidate
	for(popIter it = mPopulation.begin(); it != mPopulation.end(); ++it)
	{
		if((rand() % 100) == 1)
		{
			const int random_part = rand() % mGoal.size();
			it->str[random_part] = RANDOM_VALID_ELEMENT;
			CalculateFitness(&(*it));
		}
	}
}

void StringEvolver::CalculateFitness(Candidate *candidate )
{
	// Fitness is determined by the euclidean distance of each candidate character 
	// from corresponding goal character
	int distance = 0;
	for(uint i = 0; i < mGoal.size(); ++i)
	{
		distance += abs(mGoal[i] - candidate->str[i]);
	}
	candidate->fitness = distance;
}

const int StringEvolver::GetBestFitness() const
{
	return mPopulation[0].fitness;
}
