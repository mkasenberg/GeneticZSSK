#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "Genetic.h"
#include "Matrix.h"

#define WORK_ON_LINUX 1

using namespace std;

string fileMenu()
{
	int choice;
	cout << "Ktory plik otworzyc?" << endl << "1: ftv47.atsp" << endl
			<< "2: ftv170.atsp" << endl << "3: rbg403.atsp" << endl
			<< "4: inny" << endl;
	cin >> choice;
	switch (choice) {
	case 0:
		return "tsp10.atsp";
		break;
	case 1:
		return "ftv47.atsp";
		break;
	case 2:
		return "ftv170.atsp";
		break;
	case 3:
		return "rbg403.atsp";
		break;
	case 4: {
		string nazwa;
		cout << "Podaj nazwe pliku: ";
		cin >> nazwa;
		return nazwa;
	}
		break;
	default:
		break;
	}
	return "ftv47.atsp";
}

void manualTest(Matrix &costMatrix, Genetic* genetycznySolver)
{
//	string fileName;
//	int choice = 1;
//	double maxTime = 60000;
//	int count = 30;
//	double mutation = 0.01;
//	double cross = 0.8;
//
//	while (choice) {
//		cout << "0: Wyjscie" << endl
//				<< "1: Wczytaj z dane" << endl
//				<< "2: Kryterium stopu" << endl
//				<< "3: Wilekosc populacji" << endl
//				<< "4: Wspolczynnik mutacji" << endl
//				<< "5: Wspolczynnik krzyzowania" << endl
//				<< "6: Metoda mutacji" << endl
//				<< "7: Metoda krzyzowania" << endl
//				<< "8: Start Algorytm Genetyczny" << endl;
//		cin >> choice;
//
//		switch (choice) {
//		case 0:
//			break;
//		case 1:
//			fileName = fileMenu();
//			costMatrix.readFromFile(fileName);
//			costMatrix.display();
//			genetycznySolver->setUp(&costMatrix);
//			break;
//		case 2:
//			cout << "Stare kryterium stopu: " << genetycznySolver->getTimeLimit()
//					<< endl;
//			cout << "Podaj kryterium stopu (w sekunach): " << endl;
//			cin >> maxTime;
//			genetycznySolver->setTime(maxTime);
//			break;
//		case 3:
//			cout << "Stara wielkosc populacji poczatkowej: "
//					<< genetycznySolver->getPopulationCount() << endl;
//			cout << "Podaj wielkosc populacji poczatkowej: " << endl;
//			cin >> count;
//			genetycznySolver->setPopulatinCount(count);
//			break;
//		case 4:
//			cout << "Stary wspolczynnik mutacji: "
//					<< genetycznySolver->getMutationCoefficient() << endl;
//			cout << "Podaj wspolczynnik mutacji: " << endl;
//			cin >> mutation;
//			genetycznySolver->setMutationCoefficient(mutation);
//			break;
//		case 5:
//			cout << "Stary wspolczynnik krzyzowania: "
//					<< genetycznySolver->getCrossCoefficient() << endl;
//			cout << "Podaj wspolczynnik krzyzowania: " << endl;
//			cin >> cross;
//			genetycznySolver->setMutationCoefficient(cross);
//			break;
//		case 6: {
//			int choice = 1;
//			cout << "Wybierz metode mutacji" << endl
//					<< "1: Swap mutation - zamiana miejsc dwoch losowych elemntow "
//					<< endl
//					<< "2: Scramble mutation - wymieszanie losowego podzbioru danego osobnika z populacji"
//					<< endl;
//			cin >> choice;
//			if (choice == 1 || choice == 2)
//				genetycznySolver->setMutationChoice(choice);
//		}
//			break;
//		case 7: {
//			int choice = 1;
//			cout << "Wybierz metode krzyzowania" << endl << "1: crossover - "
//					<< endl << "2: crossover - " << endl;
//			cin >> choice;
//			if (choice == 1 || choice == 2)
//				genetycznySolver->setCrossoverChoice(choice);
//		}
//			break;
//		case 8:
//			genetycznySolver->solveTSP();
//			genetycznySolver->DisplayPopulation();
//			break;
//		default:
//			break;
//		}
//	}
}

int main(int argc, char **argv)
{
	Matrix costMatrix;
	Genetic* geneticSolver;
	geneticSolver = new Genetic();

	srand(time(NULL));

//	manualTest(costMatrix, geneticSolver);

	costMatrix.readFromFile("ftv47.atsp");//fileMenu());
	costMatrix.display();
	geneticSolver->setUp(&costMatrix);
	geneticSolver->solveTSP_MultiThreads(3);

	if(geneticSolver)
		delete geneticSolver;
	//system("PAUSE");
	return 0;
}
