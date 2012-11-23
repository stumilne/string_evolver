#pragma once
#ifndef INCLUDED_STU_string_evolver_
#define INCLUDED_STU_string_evolver_

#include <string>
#include <vector>
#include <limits>	// For INT_MAX

class StringEvolver
{
	typedef unsigned int uint;

	// Internal data-type for storing population members
	struct Candidate
	{
		Candidate() : str(nullptr), fitness(INT_MAX) {}
		//~Candidate() { if(str != nullptr) delete [] str;} // TODO - cleanup corrupts the heap..
		char *str;
		int fitness;
		inline bool operator < (const Candidate& other) const { return fitness < other.fitness; }	// For stl sorting
	};

public:
	StringEvolver(const std::string &theGoal, const uint thePopulationSize );
	~StringEvolver();

	inline void SetGoal(const std::string &theGoal) { mGoal = theGoal; }	// Set the evolutionary target, fitness function compares to this
	void AdvanceGeneration();					// Breeds the next generation into the population
	bool IsFullyEvolved() const;				// True if a population candidate matches the evolutionary goal

	const std::string GetFittest() const;		// Returns the candidate with the best fitness - the most evolved
	const int GetBestFitness() const;
	inline int GetGenerationCount() { return mGenerationCount; }
	inline char *GetPopulation()	{ return mPopulation[0].str; }

private:
	void InitialisePopulation();
	void PerformCrossover(Candidate *mother, Candidate *father);
	void PerformMutation();	
	void CalculateFitness(Candidate *);		// Calculates the current fitness of the candidate and stores it
	
	std::vector<Candidate> mPopulation;
	std::string mGoal;						// The evolutionary goal, this is what we are trying to evolve to
	int mGenerationCount;					// How many generations have passed since initial state

	typedef std::vector<Candidate>::iterator popIter;
};
#endif // INCLUDED_STU_string_evolver.h