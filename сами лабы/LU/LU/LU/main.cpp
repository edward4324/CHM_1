#include <complex>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

double set_precision(const complex<double> value, const int precision)
{
	const double power = pow(10, precision);
	return
		static_cast<int>(value.real() * power) / power;
}

void LU(vector <vector <complex<double>>> A, vector <vector <complex<double>>>& L,
	vector <vector <complex<double>>>& U, int n)
{
	U = A;

	for (int i = 0; i < n; i++)
		for (int j = i; j < n; j++)
			L[j][i] = U[j][i] / U[i][i];

	for (int k = 1; k < n; k++)
	{
		for (int i = k - 1; i < n; i++)
			for (int j = i; j < n; j++)
				L[j][i] = U[j][i] / U[i][i];

		for (int i = k; i < n; i++)
			for (int j = k - 1; j < n; j++)
				U[i][j] = U[i][j] - L[i][k - 1] * U[k - 1][j];
	}
}

bool compare(vector <vector <complex<double>>> A, vector <vector <complex<double>>> B)
{
	const size_t n = A.size();

	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			//if (set_precision(A[i][j], 2) != set_precision(B[i][j], 2))
			if (abs(A[i][j].real() - B[i][j].real()) > 0.01)
				return false;
		}
	}

	return true;
}

void product(
	vector <vector <complex<double>>>& A,
	vector <vector <complex<double>>>& B,
	vector <vector <complex<double>>>& R,
	int n
)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			for (int k = 0; k < n; k++)
				R[i][j] += A[i][k] * B[k][j];
}

void show(vector <vector <complex<double>>> A, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << "\t" << A[i][j].real() << "\t";
		}
		cout << endl;
	}
}

double random(double range_begin, double range_end)
{
	srand(time(nullptr));
	std::random_device rd;
	std::mt19937 gen(rd());
	const std::uniform_real_distribution<> dis(range_begin, range_end);

	return set_precision(dis(gen), 2);
}

int main()
{
	constexpr int n = 10;
	vector <vector <complex<double>>> a(n), l(n), u(n), r(n);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			a[i].emplace_back(random(-10,10));
			l[i].emplace_back(0);
			u[i].emplace_back(0);
			r[i].emplace_back(0);
		}
	}
	LU(a, l, u, n);
	/*cout << "Fisrt matrix" << endl;
	show(a, n);
	cout << "U matrix" << endl;
	show(u, n);
	cout << "L matrix" << endl;
	show(l, n);*/
	product(l, u, r, n);
	cout << "L*U matrix" << endl;
	show(r, n);

	cout << "Result ";
	cout << (compare(a, r) ? "==" : "!=");
	cout << " A\n";
	return 0;
}