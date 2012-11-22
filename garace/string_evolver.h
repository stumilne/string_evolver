#pragma once
#ifndef INCLUDED_STU_string_evolver.h
#define INCLUDED_STU_string_evolver.h

#include <string>

class StringEvolver
{
	typedef unsigned int uint;
public:
	StringEvolver(const std::string &theGoal, const uint thePopulationSize );
	~StringEvolver();

	void SetGoal(const std::string &theGoal);	// Set the evolutionary target, fitness function compares to this
	void AdvanceGeneration();					// Breeds the next generation into the population
	bool IsFullyEvolved();						// True if a population candidate matches the evolutionary goal

	void GetFittest();							// Returns the candidate with the best fitness - the most evolved
	void GetPopulation();
	void GetCurrentBreeders();

private:
	void InitialisePopulation();
	void SelectBreeders();
	void PerformCrossover();
	void PerformMutation();
	
	string mGoal;								// The evolutionary goal, this is what we are trying to evolve to
};
#endif // INCLUDED_STU_string_evolver.h