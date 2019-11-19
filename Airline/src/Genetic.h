#pragma once
#include <iostream>
#include <algorithm> // std::random_shuffle
#include <vector>
#include <ctime>
#include <random>
#include <functional>

#include "Matrix.h"
#include "Timer.h"
#include "Debug.h"

using namespace std;

static random_device randomDevice;
static mt19937 randomEngine;

//https://www.tutorialspoint.com/genetic_algorithms/genetic_algorithms_fitness_function.htm

/* Unit of heredity, encodes information */
typedef uint Gene;

/* Candidate solution (creature, phenotype) */
class Individual {
	/* A set of genes, complete genetic information of a given organism */
	vector<Gene> genotype;
	double fitnessValue;
	double genotypeValue;

public:
	/* Values of genes versions(alleles) */
	static Matrix* allelesValues;

	Individual();

	Individual(uint numOfGenes);

	Individual(const vector<Gene> &genotype);

	~Individual();

	void born(const vector<Gene> &genotype);

	void die();

	vector<Gene>& getGenotype();

	void mutateTotal();

	void mutateSwap(uint numOfSwaps);

	void mutateScramble(uint numOfScrams);

	double getGenotypeValue();

	void updateFitness();

	double getFitness();

	uint operator[](uint index);

	uint operator[](uint index) const;

	void operator=(Individual& ind);

	bool operator<(const Individual& otherIndividual);

	//PMX crossover (partially matched crossover)
	//http://aragorn.pb.bialystok.pl/~wkwedlo/EA5.pdf
	void crossoverPMX(Individual *otherParent, Individual **childPtr);

	void printDebugInfo();
};

/* Population of candidate solutions */
class Population {
	vector<Individual*> individuals;

public:
	Population(uint numOfIndividuals, uint numOfGenes);

	Population();

	~Population();

	void extinctAll();

	/* In ascending order */
	void sortIndividuals();

	Individual* bestIndividual();

	Individual& operator[](uint index);

	uint size();

	void pushBackIndividual(Individual* individual);

	void parentSelectRouletteWheel(Individual** parents, uint numOfParents);

	void parentSelectRank(Individual** parents, uint numOfParents);
	//TODO:
	//parentSelectUniversalSampling();
	//parentSelectTournament();
	//parentSelectRandom();

	void printDebugInfo();
};

/* Genetic algorithm solver */
class Genetic {
private:
	/* Best found individual, result of algorithm */
	Individual bestIndividual;

	/* Time criterion of stop */
	double timeLimit;

	/* Max size of population */
	uint populationMaxSize;

	/* Max number of generations - criterion of stop */
	uint maxNumOfGenerations;

public:
	Genetic();

	~Genetic();

	void solveTSP();

	double getTimeLimit();

	void setUp(Matrix *matrix);

	/* Return true if at least one of stop criterions has been reached. */
	bool checkCriterionsOfStop(uint generationNum);
};
