#pragma once
#include <iostream>
#include <algorithm> // std::random_shuffle
#include <vector>
#include <ctime>
#include <random>
#include <functional>
#include <pthread.h>
#include <ncurses.h>
#include <unistd.h>

#include "Matrix.h"
#include "Timer.h"
#include "Debug.h"

using namespace std;

static random_device randomDevice;
static mt19937 randomEngine;

//https://www.tutorialspoint.com/genetic_algorithms/genetic_algorithms_fitness_function.htm

/* Unit of heredity, encodes information */
typedef uint Gene;

enum MutationType {NO_MUT, SWAP_MUT, SCRAM_MUT, SWAP_SCRAM_MUT};

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

	void mutate(MutationType muttype, uint numOfMutations);

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
	vector<pthread_t*> solvingThreads;
	pthread_mutex_t mutexBestIndividual;
	pthread_mutexattr_t mutexattr;

	/* Best found individual, result of algorithm */
	Individual bestIndividual;

	/* Time criterion of stop */
	double timeLimit;

	/* Max size of population */
	uint populationMaxSize;

	/* Max number of generations - criterion of stop */
	uint maxNumOfGenerations;

	bool stopSignal;

	uint genotypeSize;

public:
	Genetic();

	~Genetic();

	void solvingThreadsInit(uint numOfThreads);

	void solvingThreadsDestroy();

	void solveTSP_MultiThreads(uint numOfThreads);

	static void* solve(void*);

	void solveTSP();

	bool updateBestIndividual(Individual *individual);

	double getTimeLimit();

	void setUp(Matrix *matrix);

	/* Return true if at least one of stop criterions has been reached. */
	bool checkCriterionsOfStop();

	void setMaxNumOfGenerations(uint numOfGenerations);

	uint getMaxNumOfGenerations();

	void setPopulationMaxSize(uint populationSize);

	uint getPopulationMaxSize();

	void setNumOfGenerations(double timeLimit);

	double getNumOfGenerations(uint numOfGenerations);
};
