#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;

const int N = 1000;
const int rnd = 1000;

void showMatrix(vector<double>& md, vector<double>& td, vector<double>& bd, vector<double>& f, vector<double>& fc, vector<double>& sc)
{
	for (int i = 0; i < N; i++)
	{
		cout << fc[i] << '\t' << sc[i] << '\t';
		if (i == 0)
		{
			for (int j = 2; j < N; j++)
			{
				cout << '0' << '\t';
			}
		}
		else if (i == 1)
		{
			cout << td[2] << '\t';
			for (int j = 3; j < N; j++)
			{
				cout << '0' << '\t';
			}
		}
		else if (i == 2)
		{
			cout << md[2] << '\t' << td[3];
			for (int j = 4; j < N; j++)
			{
				cout << '0' << '\t';
			}
		}
		else
		{
			for (int j = 3; j < i; j++)
			{
				cout << '0' << '\t';
			}
			cout << bd[i] << '\t' << md[i] << '\t';
			if (i < N - 1)
			{
				cout << td[i + 1] << '\t';
			}
			for (int j = i + 2; j < N; j++)
			{
				cout << '0' << '\t';
			}
		}
		cout << endl;
	}
}

void setUnitarySolutionVector(vector<double>& md, vector<double>& td, vector<double>& bd, vector<double>& fc, vector<double>& sc, vector<double>& uf)
{
	double sum = 0;
	for (int i = 0; i < N; i++)
	{
		uf.push_back(fc[i] + sc[i]);
	}
	uf[1] += td[2];
	for (int i = 2; i < N; i++)
	{
		uf[i] += md[i];
	}
	for (int i = 3; i < N; i++)
	{
		uf[i - 1] += td[i];
		uf[i] += bd[i];
	}
}
void setRandomSolution(vector<double>& x)
{
	for (int i = 0; i < N; i++)
	{
		x.push_back(static_cast<double>(rand()) / static_cast<double>(RAND_MAX) * (rnd - 1) - rnd / 2 + 1);
	}
}
void setRandomSolutionVector(vector<double>& md, vector<double>& td, vector<double>& bd, vector<double>& fc, vector<double>& sc, vector<double>& rf, vector<double>& x)
{
	double sum = 0;
	for (int i = 0; i < N; i++)
	{
		rf.push_back(fc[i] * x[0] + sc[i] * x[1]);
	}
	rf[1] += td[2] * x[2];
	for (int i = 2; i < N; i++)
	{
		rf[i] += md[i] * x[i];
	}
	for (int i = 3; i < N; i++)
	{
		rf[i - 1] += td[i] * x[i];
		rf[i] += bd[i] * x[i - 1];
	}
}
void setRandomData(vector<double>& md, vector<double>& td, vector<double>& bd, vector<double>& f, vector<double>& fc,
                   vector<double>& sc)
{
	for (int i = 0; i < N; i++)
	{
		md.push_back(static_cast<double>(rand()) / static_cast<double>(RAND_MAX) * (rnd - 1) - rnd / 2 + 1);
		td.push_back(static_cast<double>(rand()) / static_cast<double>(RAND_MAX) * (rnd - 1) - rnd / 2 + 1);
		bd.push_back(static_cast<double>(rand()) / static_cast<double>(RAND_MAX) * (rnd - 1) - rnd / 2 + 1);
		f.push_back(static_cast<double>(rand()) / static_cast<double>(RAND_MAX) * (rnd - 1) - rnd / 2 + 1);
	}
	td[0] = bd[0] = 0;
	fc.push_back(md[0]);
	fc.push_back(bd[1]);
	fc.push_back(static_cast<double>(rand()) / static_cast<double>(RAND_MAX) * (rnd - 1) - rnd / 2 + 1);
	sc.push_back(td[1]);
	sc.push_back(md[1]);
	sc.push_back(bd[2]);
	for (int i = 3; i < N; i++)
	{
		fc.push_back(static_cast<double>(rand()) / static_cast<double>(RAND_MAX) * (rnd - 1) - rnd / 2 + 1);
		sc.push_back(static_cast<double>(rand()) / static_cast<double>(RAND_MAX) * (rnd - 1) - rnd / 2 + 1);
	}
}
void readDataFromFile(vector<double>& md, vector<double>& td, vector<double>& bd, vector<double>& f, vector<double>& fc, vector<double>& sc)
{
	string path = "Example.txt";
	ifstream fin(path);
	double num;
	double arr[N][N];
	if (fin.is_open())
	{
		int i, j;
		i = j = 0;
		while (!fin.eof())
		{
			if (i < N)
			{
				fin >> arr[i][j];
				j++;
				if (j >= N)
				{
					i++;
					j = 0;
				}
			}
			else
			{
				fin >> num;
				f.push_back(num);
			}
		}
		f.pop_back();
		for (int i = 0; i < N; i++)
		{
			md.push_back(arr[i][i]);
			fc.push_back(arr[i][0]);
			sc.push_back(arr[i][1]);
		}
		td.push_back(0);
		bd.push_back(0);
		for (int i = 0; i < N - 1; i++)
		{
			td.push_back(arr[i][i + 1]);
			bd.push_back(arr[i + 1][i]);
		}
	}
}
void refreshDiagonals(vector<double>& md, vector<double>& td, vector<double>& bd, vector<double>& fc, vector<double>& sc)
{
	md[0] = fc[0];
	md[1] = sc[1];
	bd[1] = fc[1];
	bd[2] = sc[2];
	td[1] = sc[0];
}
void stepTwo(vector<double>& md, vector<double>& td, vector<double>& bd, vector<double>& f, vector<double>& fc, vector<double>& sc)
{
	double div, mult;
	// Делим на первый элемент первого столбца
	div = fc[0];
	if (div == 0)
	{
		return;
	}
	fc[0] /= div;
	sc[0] /= div;
	f[0] /= div;
	// Избавляемся от неизвестных в первом столбце
	for (int i = 1; i < N; i++)
	{
		mult = fc[i];
		fc[i] -= mult;
		f[i] -= f[0] * mult;
		sc[i] -= sc[0] * mult;
	}
	// Копируем равные значение согласну условию
	refreshDiagonals(md, td, bd, fc, sc);

	// Делим на второй элемент второго столбца
	div = sc[1];
	if (div == 0)
	{
		return;
	}
	sc[1] /= div;
	td[2] /= div;
	f[1] /= div;
	// Копируем равные значение согласну условию
	refreshDiagonals(md, td, bd, fc, sc);
	// Избавляемся от неизвестных во втором столбце
	md[2] -= sc[2] * td[2];//////////
	bd[3] -= sc[3] * td[2];//////////
	for (int i = 2; i < N; i++)
	{
		mult = sc[i];
		sc[i] -= mult;
		f[i] -= f[1] * mult;
	}
	// Копируем равные значение согласну условию
	refreshDiagonals(md, td, bd, fc, sc);
}
void stepOne(vector<double>& md, vector<double>& td, vector<double>& bd, vector<double>& f, vector<double>& fc, vector<double>& sc)
{
	double div, mult;

	// Избавляемся от коэффициентов на нижней диагонали
	for (int i = 2; i < N - 1; i++)
	{
		// Приведение коэффициента главной диагонали к единице
		div = md[i];
		if (div == 0)
		{
			return;
		}
		md[i] /= div;
		td[i + 1] /= div;
		bd[i] /= div;
		f[i] /= div;
		fc[i] /= div;
		sc[i] /= div;

		refreshDiagonals(md, td, bd, fc, sc);

		// Вычитание из нижней диагонали для исключения коэффициента на ней
		mult = bd[i + 1];
		bd[i + 1] -= mult;
		md[i + 1] -= td[i + 1] * mult;
		f[i + 1] -= f[i] * mult;
		fc[i + 1] -= fc[i] * mult;
		sc[i + 1] -= sc[i] * mult;

		refreshDiagonals(md, td, bd, fc, sc);
	}

	//Избавляемся от коэффициентов на верхней диагонали
	for (int i = N - 1; i >= 1; i--)
	{
		// Приведение коэффициента главной диагонали к единице
		div = md[i];
		if (div == 0)
		{
			return;
		}
		md[i] /= div;
		if (i != N - 1)
		{
			td[i + 1] /= div;
		}
		bd[i] /= div;
		f[i] /= div;
		fc[i] /= div;
		sc[i] /= div;

		refreshDiagonals(md, td, bd, fc, sc);
		// Вычитание из верхней диагонали для исключения коэффициента на ней
		mult = td[i];
		td[i] -= mult;
		if (i != 2)
		{
			md[i - 1] -= bd[i] * mult;
		}
		f[i - 1] -= f[i] * mult;
		fc[i - 1] -= fc[i] * mult;
		sc[i - 1] -= sc[i] * mult;

		refreshDiagonals(md, td, bd, fc, sc);
	}
}
void stepThree(vector<double>& md, vector<double>& td, vector<double>& bd, vector<double>& f, vector<double>& fc, vector<double>& sc)
{
	double mult = td[1];
	td[1] -= mult;
	f[0] -= f[1] * mult;

	sc[0] = td[1];
}

bool isSolutionCorrect(vector<double>& md, vector<double>& td, vector<double>& bd, vector<double>& f, vector<double>& fc, vector<double>& sc)
{
	for (int i = 0; i < N; i++)
	{
		if (md[i] + td[i] + bd[i] != 1)
		{
			return false;
		}
	}
	if (fc[0] != 1 && sc[1] != 1)
	{
		return false;
	}
	for (int i = 2; i < N; i++)
	{
		if (fc[i] + sc[i] != 0) return false;
	}
	return true;
}
double evaluationAccuracy(vector<double> md, vector<double> td, vector<double> bd, vector<double>& uf, vector<double> fc, vector<double> sc)
{
	stepOne(md, td, bd, uf, fc, sc);
	stepTwo(md, td, bd, uf, fc, sc);
	stepThree(md, td, bd, uf, fc, sc);

	double max = -1;
	double current;
	for (int i = 0; i < uf.size(); i++)
	{
		current = abs(uf[i] - 1);
		if (max < current)
		{
			max = current;
		}
	}
	return max;
}
double relativeError()
{
	vector<double> md, td, bd, fc, sc, rf, f, x;
	setRandomSolution(x);
	setRandomData(md, td, bd, f, fc, sc);
	setRandomSolutionVector(md, td, bd, fc, sc, rf, x);

	//showMatrix(md, td, bd, rf, fc, sc);

	stepOne(md, td, bd, rf, fc, sc);
	stepTwo(md, td, bd, rf, fc, sc);
	stepThree(md, td, bd, rf, fc, sc);

	cout << "\nProgram solution" << '\t' << "Precise solution" << endl;

	/*for (int i = 0; i < N; i++)
	{
		cout << "X" << i + 1 << " = " << rf[i] << '\t' << "X*" << i + 1 << " = " << x[i] << endl;
	}*/

	double max = -1;
	double current;
	for (int i = 0; i < rf.size(); i++)
	{
		current = abs((rf[i] - x[i]) / x[i]);
		if (max < current)
		{
			max = current;
		}
	}
	return max;
}
void showResults(vector<double>& md, vector<double>& td, vector<double>& bd, vector<double>& f, vector<double>& fc, vector<double>& sc, vector<double>& uf, double evalAccuracy, double relError)
{
	if (!isSolutionCorrect(md, td, bd, f, fc, sc))
	{
		cout << "Invalid incoming data (division by zero found)" << endl;
		return;
	}
	/*for (int i = 0; i < N; i++)
	{
		cout << "X" << i + 1 << " = " << f[i] << endl;
	}*/

	cout << "Evaluation accuracy: " << evalAccuracy << endl;
	cout << "Relative error: " << relError << endl;
}

int main() {
	srand(time(0));
	//// Последняя буква d в имени вектора - сокращение от diagonal
	//// m - main, t - top, b - bottom diagonal (md, td, bd)
	//
	//// f - вектор правой части системы уравнений
	//// uf - вектор, соответствующий правой части системы с единичным решением
	//// rf - вектор, соответствующий правой части системы со случайным решением
	//// fc, sc - первая и вторая испорченные колонки соответственно

	vector<double> md, td, bd, f, fc, sc, uf, rf;

	//readDataFromFile(md, td, bd, f, fc, sc);
	setRandomData(md, td, bd, f, fc, sc);

	setUnitarySolutionVector(md, td, bd, fc, sc, uf);
	double evalAccuracy = evaluationAccuracy(md, td, bd, uf, fc, sc);

	double relError = relativeError();

	stepOne(md, td, bd, f, fc, sc);
	stepTwo(md, td, bd, f, fc, sc);
	stepThree(md, td, bd, f, fc, sc);

	//showResults(md, td, bd, f, fc, sc, uf, evalAccuracy, relError);
	cout << "\nRelative error: " << relError << endl;

	system("pause");
	return 0;
}