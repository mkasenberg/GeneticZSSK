#include "Genetic.h"

Matrix* Individual::allelesValues = NULL;

Genetic::Genetic()
{
	timeLimit = 60000;
	populationMaxSize = 100;
	maxNumOfGenerations = 100;
}

Genetic::~Genetic()
{
}

void Genetic::solveTSP()
{
	Individual *child, *bestIndCandidate = NULL;
	Individual *parents[2];
	Population *currentGeneration;
	Population *nextGeneration;
	uint generationNum = 1;

	currentGeneration = new Population(populationMaxSize,
			Individual::allelesValues->size());
	nextGeneration = new Population();

	printf("Current generation:\n");
	currentGeneration->printDebugInfo();
	printf("Next generation:\n");
	nextGeneration->printDebugInfo();

	Timer::startTimer();

	while (checkCriterionsOfStop(generationNum) == false) {

		while (nextGeneration->size() < populationMaxSize) {
			child = NULL;
			currentGeneration->parentSelectRank(parents, 2);
			debug("P1:\n");
			parents[0]->printDebugInfo();
			debug("P2:\n");
			parents[1]->printDebugInfo();

			parents[0]->coupleReproduce(*(parents[1]), &child);
			nextGeneration->pushBackIndividual(child);
			debug("Child:\n");
			child->printDebugInfo();
		}

		delete currentGeneration;
		currentGeneration = nextGeneration;
		nextGeneration = new Population();

		bestIndCandidate = currentGeneration->bestIndividual();
		if(bestIndividual.getFitness() < bestIndCandidate->getFitness()) {
			bestIndividual = *bestIndCandidate;
		}
		bestIndCandidate = NULL;

		debug("Best individual:\n");
		bestIndividual.printDebugInfo();
		++generationNum;
	}

	delete currentGeneration;
	currentGeneration = NULL;
	debug("Best individual:\n");
	bestIndividual.printDebugInfo();
}

double Genetic::getTimeLimit()
{
	return timeLimit;
}

void Genetic::setUp(Matrix *matrix)
{
	Individual::allelesValues = matrix;
	randomEngine = mt19937(randomDevice());
}
