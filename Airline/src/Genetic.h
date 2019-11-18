#pragma once
#include <iostream>
#include <algorithm> // std::random_shuffle
#include <vector>    // std::vector
#include <ctime>     //
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

	Individual()
	{
		fitnessValue = 0;
		genotypeValue = 0;
	}

	Individual(uint numOfGenes)
	{
		for (uint i = 0; i < numOfGenes; ++i)
			genotype.push_back(i);

		fitnessValue = 0;
		genotypeValue = 0;
		mutateTotal();
		updateFitness();
	}

	Individual(const vector<Gene> &genotype)
	{
		born(genotype);
		updateFitness();
	}

	~Individual()
	{
		die();
	}

	void born(const vector<Gene> &genotype)
	{
		this->genotype = genotype;
	}

	void die()
	{
		genotype.clear();
	}

	void mutateTotal()
	{
		random_shuffle(genotype.begin(), genotype.end());
		updateFitness();
	}

	void mutateSwap(uint numOfSwaps)
	{
		uint x, y;

		for (uint i = 0; i < numOfSwaps; ++i) {
			do {
				x = rand() % genotype.size();
				y = rand() % genotype.size();
			} while (x == y);

			swap(genotype[x], genotype[y]);
		}
		updateFitness();
	}

	void mutateScramble(uint numOfScrams)
	{
		uint firstIndex = 0, lastIndex = 0;

		for (uint i = 0; i < numOfScrams; ++i) {
			do {
				firstIndex = rand() % (genotype.size());
				lastIndex = rand() % (genotype.size());
			} while (firstIndex == lastIndex);

			random_shuffle(genotype.begin() + firstIndex,
					genotype.begin() + lastIndex);
		}
		updateFitness();
	}

	double getGenotypeValue()
	{
		uint i;
		genotypeValue = 0;
		for (i = 0; i < genotype.size() - 1; ++i) {
			genotypeValue += (*allelesValues)[genotype[i]][genotype[i + 1]];
		}
		genotypeValue += (*allelesValues)[genotype[i]][genotype[0]];
		return genotypeValue;
	}

	void updateFitness()
	{
		fitnessValue = 1.0 / getGenotypeValue();
	}

	double getFitness()
	{
		return fitnessValue;
	}

	uint operator[](uint index)
	{
		return genotype[index];
	}

	uint operator[](uint index) const
	{
		return genotype[index];
	}

	void operator=(Individual& ind)
	{
		genotype = ind.genotype;
		fitnessValue = ind.fitnessValue;
		genotypeValue = ind.genotypeValue;
	}

	bool operator<(const Individual& otherIndividual)
	{
		return this->fitnessValue < otherIndividual.fitnessValue;
	}

	void coupleReproduce(const Individual &parentB, Individual **childPtr)
	{
		crossoverTwoPoints(parentB, childPtr);
	}

//	Individual* trioReproduce(Individual *partnerA, Individual *partnerB)
//	{
//		return NULL;
//	}

	void crossoverOnePoint(const Individual &parentB, Individual *child)
	{
	}

	//PMX crossover (partially matched crossover)
	//http://aragorn.pb.bialystok.pl/~wkwedlo/EA5.pdf
	void crossoverTwoPoints(const Individual &parentB, Individual **childPtr)
	{
		Individual &parentA = *this;
		uint genotypeSize = genotype.size();
		vector<Gene> childGenotype;
		vector<uint> genesPosInA;
		vector<uint> genesPosInB;
		uint crossPoint1, crossPoint2;
		if (childPtr == NULL) {
			return;
		}

		if (*childPtr != NULL) {
			delete (*childPtr);
			(*childPtr) = NULL;
		}

		genesPosInA.resize(genotypeSize);
		genesPosInB.resize(genotypeSize);
		childGenotype.resize(genotypeSize);

		for (uint i = 0; i < genotypeSize; ++i) {
			childGenotype[i] = UINT32_MAX;
			genesPosInA[parentA[i]] = i;
			genesPosInB[parentB[i]] = i;
		}

		crossPoint1 = rand() % genotypeSize;
		crossPoint2 = rand() % genotypeSize;

		while (crossPoint1 == crossPoint2 ||
				(crossPoint1 == 0 && crossPoint2 == genotypeSize-1) ||
				(crossPoint2 == 0 && crossPoint1 == genotypeSize-1))
			crossPoint2 = rand() % genotypeSize;
		if (crossPoint2 < crossPoint1)
			swap(crossPoint1, crossPoint2);

		for (uint i = crossPoint1; i <= crossPoint2; ++i) {
			childGenotype[i] = parentA[i];
		}

		debug("cp1=%u, cp2=%u", crossPoint1, crossPoint2);
		uint genVal = parentA[crossPoint1], i = crossPoint1;

		for (uint i = crossPoint1; i <= crossPoint2; ++i) {

			//If gene value has been copied to child already
			if (crossPoint1 <= genesPosInA[parentB[i]] && genesPosInA[parentB[i]] <= crossPoint2) {
				++i;
				genVal = parentA[i];
				continue;
			}
			//(genesPosInA[genVal] == i || genVal == parentB[i]) &&
			//As long as do not find place for gene outside off crosspoints
			while (((crossPoint1 <= genesPosInB[genVal] && genesPosInB[genVal] <= crossPoint2) ||
				childGenotype[genesPosInB[genVal]] != UINT32_MAX))
			{
				genVal = parentA[genesPosInB[genVal]];
			}

			if(childGenotype[genesPosInB[genVal]] == UINT32_MAX)
				childGenotype[genesPosInB[genVal]] = parentB[i];
			genVal = parentA[i];
		}

		for (uint i = 0; i < genotypeSize; ++i) {
			if (childGenotype[i] == UINT32_MAX)
				childGenotype[i] = parentB[i];
		}

//		for (uint i = crossPoint1 + 1; i < genotypeSize; ++i) {
//			if (childGenotype[i] == UINT32_MAX)
//				childGenotype[i] = parentB[i];
//		}

		(*childPtr) = new Individual(childGenotype);
	}

	void printDebugInfo()
	{
		if(genotypeValue > 1000)
			debug("Some Error\n");
		debug("Size=%lu, gentpVal=%f\n", genotype.size(), genotypeValue);
		for (uint i = 0; i < genotype.size(); ++i)
			debug("%u->", genotype[i]);
		debug("%u\n", genotype[0]);
	}
};

/* Population of candidate solutions */
class Population {
	vector<Individual*> individuals;
	int populationCost;

public:
	Population(uint numOfIndividuals, uint numOfGenes)
	{
		for (uint i = 0; i < numOfIndividuals; ++i)
			individuals.push_back(new Individual(numOfGenes));

		populationCost = 0;
	}

	Population()
	{
		populationCost = 0;
	}

	~Population()
	{
		extinctAll();
	}

	void extinctAll()
	{
		for (uint i = 0; i < individuals.size(); ++i) {
			delete individuals[i];
			individuals[i] = NULL;
		}
		individuals.clear();
	}

	/* In ascending order */
	void sortIndividuals()
	{
		sort(individuals.begin(), individuals.end(),
				[](Individual* a, Individual* b) -> bool
				{ return a->getFitness() < b->getFitness(); });
	}

	Individual* bestIndividual()
	{
		sortIndividuals();
		return individuals.back();
	}

	Individual& operator[](uint index)
	{
		individuals[index];
		return *(individuals[index]);
	}

	uint size()
	{
		return individuals.size();
	}

	void pushBackIndividual(Individual* individual)
	{
		individuals.push_back(individual);
	}

	void parentSelectRouletteWheel(Individual** parents, uint numOfParents)
	{
		double fitSumAll = 0, fitSum = 0, fitSumThreshold = 0;
		uniform_real_distribution<double> dis(0.0, 1.0);

		sortIndividuals();

		for (auto const &ind : individuals)
			fitSumAll += ind->getFitness();

		dis = uniform_real_distribution<double>(0.0, fitSum);

		for (uint i = 0; i < numOfParents; ++i) {
			//fitSumThreshold = rand() % (long long) round(fitSumAll) + 1;
			fitSumThreshold = dis(randomEngine);

			for (Individual* ind : individuals) {
				fitSum += ind->getFitness();
				if (fitSum >= fitSumThreshold) {
					parents[i] = ind;
					break;
				}
			}
			for (uint j = 0; j < i; ++j) {
				if (parents[j] == parents[i]) {
					--i;
					break;
				}
			}
		}
	}

	void parentSelectRank(Individual** parents, uint numOfParents)
	{
		ulong fitSumAll = 0, fitSumThreshold = 0;
		uint parentId = 0;

		if (numOfParents == 0 || numOfParents > individuals.size())
			return;

		sortIndividuals();

		//1 + 2 + ... + N = (N + 1) * N / 2
		fitSumAll = (individuals.size() + 1) * individuals.size() / 2;

		for (uint i = 0; i < numOfParents; ++i) {
			fitSumThreshold = rand() % fitSumAll + 1;
			//sum = (N + 1) * N / 2  <=>  0=N^2 + N - 2 * sum
			//Δ=b^2−4*a*c, b=1, a=1, c=-2*sum
			ulong delta = 1 + 8 * fitSumThreshold;
			parentId = ceil((-1 + sqrt(delta)) / 2);
			--parentId;
			cout << "Parent Id=" << parentId << endl;
			parents[i] = individuals[parentId];
			for (uint j = 0; j < i; ++j) {
				if (parents[j] == parents[i]) {
					--i;
					break;
				}
			}
		}
	}
	//parentSelectUniversalSampling();
	//parentSelectTournament();
	//parentSelectRandom();

	void printDebugInfo()
	{
		if (individuals.size() == 0)
			cout << "Population is empty" << endl;
		for (Individual* ind : individuals) {
			ind->printDebugInfo();
		}
	}
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

	/* Return true if at least one of stop criterions has been reached. */
	bool checkCriterionsOfStop(uint generationNum)
	{
		double t = Timer::millis();
		debug("time=%f", t);
		return t >= timeLimit || maxNumOfGenerations < generationNum;
	}

public:
	Genetic();

	~Genetic();

	void solveTSP();

	double getTimeLimit();

	void setUp(Matrix *matrix);
};
