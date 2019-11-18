#include "Matrix.h"

Matrix::Matrix()
{
}

Matrix::~Matrix()
{
	clearMatrix();
}

void Matrix::display()
{
	for (auto array : matrix) {
		for (auto element : array) {
			cout << setw(5) << element << " ";
		}
		cout << endl;
	}
}

void Matrix::clearMatrix()
{
	matrix.clear();
}

bool Matrix::readFromFile(const string &inputFileName)
{
	int dimension = 0, pos = -1;
	string inputLine = "";

	fstream fstrInput;
	fstrInput.open(inputFileName.c_str(), fstream::in);

	if (!fstrInput.is_open() || !fstrInput.good()) {
		cout << "Error: Couldn't open the file!" << endl;
		fstrInput.close();
		return false;
	}

	if ((int) inputFileName.find(".atsp") > 0) {
		while (pos < 0 && fstrInput.good()) {
			if (!getline(fstrInput, inputLine)) {
				cout << "Error during read from file" << endl;
				fstrInput.close();
				return false;
			}

			pos = inputLine.find("DIMENSION:");
		}
		inputLine.erase(pos, string("DIMENSION:").length());
		dimension = atoi(inputLine.c_str());

		if (dimension == 0) {
			cout << "Error: dimension=0" << endl;
			fstrInput.close();
			return false;
		}

		pos = -1;
		while (pos < 0 && fstrInput.good()) {
			if (!getline(fstrInput, inputLine)) {
				cout << "Error during read from file" << endl;
				fstrInput.close();
				return false;
			}

			pos = inputLine.find("EDGE_WEIGHT_SECTION");
		}
	} else if ((int) inputFileName.find(".txt") > 0) {
		fstrInput >> dimension;
	} else {
		cout << "Error: File extension does not match any known!" << endl;
	}

	resize(dimension);

	for (int i = 0; i < dimension; ++i)
		for (int j = 0; j < dimension; ++j)
			if (fstrInput.good()) {
				fstrInput >> matrix[i][j];
				if(i==j)
					matrix[i][j] = DBL_MAX;
			}
			else {
				matrix.clear();
				fstrInput.close();
				cout << "Error during read from file" << endl;
				return false;
			}

	fstrInput.close();
	return true;
}

vector<vector<double>>& Matrix::getMatrix()
{
	return matrix;
}

void Matrix::setMatrix(vector<vector<double>> *matrix)
{
	this->matrix = *matrix;
}

void Matrix::resize(uint size)
{
	matrix.resize(size);
	for(uint i = 0; i < size; ++i)
		matrix[i].resize(size);
}

vector<double>& Matrix::operator[](uint index)
{
	return matrix[index];
}

uint Matrix::size() {
	return matrix.size();
}
