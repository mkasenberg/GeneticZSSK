#include "Genetic.h"

Matrix* Individual::allelesValues = NULL;

Individual::Individual()
{
	fitnessValue = 0;
	genotypeValue = 0;
}

Individual::Individual(uint numOfGenes)
{
	for (uint i = 0; i < numOfGenes; ++i)
		genotype.push_back(i);

	fitnessValue = 0;
	genotypeValue = 0;
	mutateTotal();
	updateFitness();
}

Individual::Individual(const vector<Gene> &genotype)
{
	born(genotype);
	updateFitness();
}

Individual::~Individual()
{
	die();
}

void Individual::born(const vector<Gene> &genotype)
{
	this->genotype = genotype;
}

void Individual::die()
{
	genotype.clear();
}

vector<Gene>& Individual::getGenotype()
{
	return genotype;
}
void Individual::mutateTotal()
{
	random_shuffle(genotype.begin(), genotype.end());
	updateFitness();
}

void Individual::mutateSwap(uint numOfSwaps)
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

void Individual::mutateScramble(uint numOfScrams)
{
	uint firstIndex = 0, lastIndex = 0;

	for (uint i = 0; i < numOfScrams; ++i) {
		do {
			firstIndex = rand() % (genotype.size());
			lastIndex = rand() % (genotype.size());
		} while (firstIndex == lastIndex);

		if(firstIndex > lastIndex)
			swap(firstIndex, lastIndex);

		random_shuffle(genotype.begin() + firstIndex,
				genotype.begin() + lastIndex);
	}
	updateFitness();
}

double Individual::getGenotypeValue()
{
	uint i;
	genotypeValue = 0;
	for (i = 0; i < genotype.size() - 1; ++i) {
		genotypeValue += (*allelesValues)[genotype[i]][genotype[i + 1]];
	}
	genotypeValue += (*allelesValues)[genotype[i]][genotype[0]];
	return genotypeValue;
}

void Individual::updateFitness()
{
	fitnessValue = 1.0 / getGenotypeValue();
}

double Individual::getFitness()
{
	return fitnessValue;
}

uint Individual::operator[](uint index)
{
	return genotype[index];
}

uint Individual::operator[](uint index) const
{
	return genotype[index];
}

void Individual::operator=(Individual& ind)
{
	genotype = ind.genotype;
	fitnessValue = ind.fitnessValue;
	genotypeValue = ind.genotypeValue;
}

bool Individual::operator<(const Individual& otherIndividual)
{
	return this->fitnessValue < otherIndividual.fitnessValue;
}

void Individual::crossoverPMX(Individual *otherParent, Individual **childPtr)
{
	uint smallerCrossPoint, biggerCrossPoint;
	vector<Gene> child;
	vector<Gene> parentA = genotype;
	vector<Gene> parentB = otherParent->getGenotype();
	uint genotypeSize = genotype.size();
	vector<uint>::iterator iteratorVector;
	uint tmpCounter;
	uint tmpNumber;

	uniform_int_distribution<uint> dis(0, genotypeSize - 1);
	smallerCrossPoint = dis(randomEngine);
	biggerCrossPoint = dis(randomEngine);

	child.resize(genotypeSize);
	fill(child.begin(), child.end(), UINT32_MAX);

	if (smallerCrossPoint > biggerCrossPoint)
		swap(smallerCrossPoint, biggerCrossPoint);

	// copy parent A genes to child genotype from range
	// copy[first, last) why +1
	copy(parentA.begin() + smallerCrossPoint,
			parentA.begin() + biggerCrossPoint + 1,
			child.begin() + smallerCrossPoint);

	for (uint i = smallerCrossPoint; i <= biggerCrossPoint; ++i) {
		iteratorVector = find(child.begin(), child.end(), parentB[i]);

		// if it isn't in child genotype
		if (iteratorVector == child.end()) {
			// take number from parent A from the same index
			// and looking for position this number in parent B
			tmpCounter = i;
			do {
				tmpNumber = parentA[tmpCounter];
				tmpCounter = 0;
				while (tmpCounter < genotypeSize
						&& tmpNumber != parentB[tmpCounter])
					++tmpCounter;
			} while (child[tmpCounter] != UINT32_MAX);

			child[tmpCounter] = parentB[i];
		}
	}

	// fill the rest of numbers
	tmpCounter = 0;
	while (tmpCounter < genotypeSize) {
		// find number that child doesn't have
		if (find(child.begin(), child.end(), parentB[tmpCounter])
				!= child.end()) {
			++tmpCounter;
			continue;
		}

		// find empty space and place it there
		iteratorVector = find(child.begin(), child.end(), UINT32_MAX);
		if (iteratorVector != child.end())
			*iteratorVector = parentB[tmpCounter];
	}

	*childPtr = new Individual(child);
}

void Individual::printDebugInfo()
{
	debug("Size=%lu, gentpVal=%f\n", genotype.size(), genotypeValue);
	for (uint i = 0; i < genotype.size(); ++i)
		debug("%u->", genotype[i]);
	debug("%u\n", genotype[0]);
}

Population::Population(uint numOfIndividuals, uint numOfGenes)
{
	for (uint i = 0; i < numOfIndividuals; ++i)
		individuals.push_back(new Individual(numOfGenes));
}

Population::Population()
{
}

Population::~Population()
{
	extinctAll();
}

void Population::extinctAll()
{
	for (uint i = 0; i < individuals.size(); ++i) {
		delete individuals[i];
		individuals[i] = NULL;
	}
	individuals.clear();
}

/* In ascending order */
void Population::sortIndividuals()
{
	sort(individuals.begin(), individuals.end(),
			[](Individual* a, Individual* b) -> bool
			{	return a->getFitness() < b->getFitness();});
}

Individual* Population::bestIndividual()
{
	sortIndividuals();
	return individuals.back();
}

Individual& Population::operator[](uint index)
{
	individuals[index];
	return *(individuals[index]);
}

uint Population::size()
{
	return individuals.size();
}

void Population::pushBackIndividual(Individual* individual)
{
	individuals.push_back(individual);
}

void Population::parentSelectRouletteWheel(Individual** parents, uint numOfParents)
{
	double fitSumAll = 0, fitSum = 0, fitSumThreshold = 0;
	uniform_real_distribution<double> dis(0.0, 1.0);

	sortIndividuals();

	for (auto const &ind : individuals)
		fitSumAll += ind->getFitness();

	dis = uniform_real_distribution<double>(0.0, fitSumAll);

	for (uint i = 0; i < numOfParents; ++i) {
		//fitSumThreshold = rand() % (long long) round(fitSumAll) + 1;
		fitSumThreshold = dis(randomEngine);
		fitSum = 0;

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

void Population::parentSelectRank(Individual** parents, uint numOfParents)
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

void Population::printDebugInfo()
{
	if (individuals.size() == 0)
		cout << "Population is empty" << endl;
	for (Individual* ind : individuals) {
		ind->printDebugInfo();
	}
}

Genetic::Genetic()
{
	timeLimit = 60000;
	populationMaxSize = 10;
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
//			currentGeneration->parentSelectRank(parents, 2);
			currentGeneration->parentSelectRouletteWheel(parents, 2);
			debug("P1:\n");
			parents[0]->printDebugInfo();
			debug("P2:\n");
			parents[1]->printDebugInfo();

			parents[0]->crossoverPMX(parents[1], &child);
			//child->mutateSwap(1);
			child->mutateScramble(1);
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

bool Genetic::checkCriterionsOfStop(uint generationNum)
{
	double t = Timer::millis();
	debug("time=%f\n", t);
	return t >= timeLimit;// || maxNumOfGenerations < generationNum;
}
