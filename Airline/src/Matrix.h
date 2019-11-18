#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <float.h>

using namespace std;

class Matrix
{
protected:
	vector<vector<double>> matrix;

public:
	Matrix();
	~Matrix();

	void display();

	void clearMatrix();

	bool readFromFile(const string &inputFileName);

	vector<vector<double>>& getMatrix();

	void setMatrix(vector<vector<double>> *matrix);

	vector<double>& operator[](uint index);

	void resize(uint size);

	uint size();
};
