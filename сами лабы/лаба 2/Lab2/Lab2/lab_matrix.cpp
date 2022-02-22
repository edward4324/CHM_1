#include "lab_matrix.h"
#include <random>
#include <iostream>
#include <fstream>

//Статический метод на получение случайного числа из диапазона
COMPLEX_DOUBLE lab_matrix::random(const double range_begin, const double range_end)
{
	/*srand(time(nullptr));
	std::random_device rd;
	std::mt19937 gen(rd());
	const std::uniform_real_distribution<> dis(range_begin, range_end);

	return set_precision(dis(gen), 2);*/

	
	return
		-range_begin + static_cast<double>(rand()) / RAND_MAX * 2 * range_end;
}

bool lab_matrix::is_result_wrong(
	const SOLUTION_VECTOR& x, 
	const SOLUTION_VECTOR& f
) const
{
	const auto result = new SOLUTION_VECTOR(n_);

	for (int i = 0; i < n_; i++)
		for (int j = 0; j < n_; j++)
		{
			(*result)[i] += get_from_matrix(b_matrix_, i, j) * x[i];
		}

	for (int i = 0; i < n_; i++)
		if ((*result)[i] != f[i]) {
			delete result;
			return true;
		}
	delete result;
	return false;
}

//Получение X и Y
void lab_matrix::find_result(SOLUTION_VECTOR* x, const SOLUTION_VECTOR* input_f) const
{
	COMPLEX_DOUBLE sum = 0;
	const auto y = new SOLUTION_VECTOR(n_);
	//Y
	for (int i = 0; i < n_; i++)
	{
		for (int k = k0(i); k < i; k++)
		{
			COMPLEX_DOUBLE b_ik = get_from_matrix(b_matrix_, i, k);
			sum += b_ik * (*y)[k];
		}
		(*y)[i] = ((*input_f)[i] - sum) / get_from_matrix(b_matrix_, i, i);
		sum = 0;
	}

	//X
	for (int i = n_ - 1; i >= 0; i--)
	{
		for (int k = i + 1; k <= kn(i); k++)
		{
			COMPLEX_DOUBLE b_ki = get_from_matrix(b_matrix_, k, i);
			sum += b_ki * (*x)[k];
		}
		COMPLEX_DOUBLE b_ii = get_from_matrix(b_matrix_, i, i);
		(*x)[i] = (*y)[i] - sum / b_ii;
		sum = 0;
	}
}

//Заполнение векторов x и f2
void lab_matrix::fill_x1_and_f2(const double range_begin, const double range_end) const
{
	for (int i = 0; i < n_; i++)
		(*x1_)[i] = random(range_begin, range_end);
	for (int i = 0; i < n_; i++)
	{
		COMPLEX_DOUBLE sum = 0;
		for (int j = 0; j < n_; j++) {
			if (j > i)
				sum += get_from_matrix(matrix_, j, i) * (*x1_)[j];
			else
				sum += get_from_matrix(matrix_, i, j) * (*x1_)[j];
		}		
		(*f2_)[i] = sum;
	}
}
//Заполнение основной матрицы случайными числами
void lab_matrix::fill_random(const double range_begin, const double range_end) const
{
	for (int i = 0; i < n_; i++)
		for (int j = 0; j < n_; j++)
			add_to_matrix(
				matrix_,
				i,
				j,
				random(range_begin, range_end)
				);
	for (int i = 0; i < n_; i++)
		(*f_)[i] = random(range_begin, range_end);
	//print_matrix(std::cout);
	*b_matrix_ = *matrix_;
}

//Заполнение значениями для "плохого" результата
void lab_matrix::fill_random_bad(
	const double range_begin,
	const double range_end,
	const int k
) const
{
	const auto matrix1 = new SOLUTION_MATRIX(n_);
	for (int i = 0; i < n_; i++)
		(*matrix1)[i].resize(n_);
	const auto matrix2 = new SOLUTION_MATRIX(n_);
	for (int i = 0; i < n_; i++)
		(*matrix2)[i].resize(n_);

	for (int i = 0; i < n_; i++)
		for (int j = 0; j <= i; j++)
			(*matrix1)[i][j] = random(range_begin, range_end);
	for (int i = 0; i < n_; i++)
		for (int j = i; j < n_; j++)
			(*matrix2)[i][j] = random(range_begin, range_end);

	const double c = 1 / pow(10, k+2);
	for (int i = 0; i < n_; i++)
	{
		(*matrix1)[i][i] *= c;
		(*matrix2)[i][i] *= c;
	}

	const auto t = new SOLUTION_MATRIX(n_);
	for (int i = 0; i < n_; i++)
	{
		(*t)[i].resize(n_);
		for (int j = 0; j < n_; j++)
		{
			COMPLEX_DOUBLE sum;
			for (int k = 0; k < n_; k++) {
				const COMPLEX_DOUBLE prod = (*matrix1)[i][k] * (*matrix2)[k][j];
				sum += prod;
			}
			(*t)[i][j] = sum;
			sum = 0;
		}
	}

	{
		for (int i = 0; i < n_; i++)
			(*t)[i][i] /= 1000000000;
	}

	for (int i = 0; i < n_; i++)
		for (int j = 0; j < n_; j++)
			add_to_matrix(matrix_ , i, j, (*t)[i][j]);
	for (int i = 0; i < n_; i++)
		(*f_)[i] = random(range_begin, range_end);

	delete t;
	delete matrix1;
	delete matrix2;
}

void lab_matrix::solve() const
{
	//B
	COMPLEX_DOUBLE sum = 0;
	*b_matrix_ = *matrix_;
	for (int j = 0; j < n_; j++)
	{
		for (int i = j; i <= kn(j) + 1; i++)
		{
			for (int k = k0(i); k < j; k++)
			{
				COMPLEX_DOUBLE b_ik = get_from_matrix(b_matrix_, i, k);
				COMPLEX_DOUBLE b_jk = get_from_matrix(b_matrix_, j, k);
				COMPLEX_DOUBLE b_kk = get_from_matrix(b_matrix_, k, k);
				sum += b_ik * b_jk / b_kk;
			}
			COMPLEX_DOUBLE a_ik = get_from_matrix(b_matrix_, i, j);
			//Добавление в матрицу В нового элемента
			add_to_matrix(
				b_matrix_,
				i,
				j,
				a_ik - sum
			);
			sum = 0;
		}
	}

	const auto y = new SOLUTION_VECTOR(n_);
	//Y
	for (int i = 0; i < n_; i++)
	{
		for (int k = k0(i);	k < i; k++)
		{
			COMPLEX_DOUBLE b_ik = get_from_matrix(b_matrix_, i, k);
			sum += b_ik * (*y)[k];
		}
		(*y)[i] = ((*f_)[i] - sum) / get_from_matrix(b_matrix_, i, i);
		sum = 0;
	}

	//X
	for (int i = n_ - 1; i >= 0; i--)
	{
		for (int k = i + 1; k <= kn(i); k++)
		{
			COMPLEX_DOUBLE b_ki = get_from_matrix(b_matrix_, k, i);
			sum += b_ki * (*x_)[k];
		}
		COMPLEX_DOUBLE b_ii = get_from_matrix(b_matrix_, i, i);
		(*x_)[i] = (*y)[i] - sum / b_ii;
		sum = 0;
	}

}

//Поиск ошибок и т.д.
COMPLEX_DOUBLE lab_matrix::find_errors(const double range_begin, const double range_end) const
{
	const auto n_x = new SOLUTION_VECTOR(n_);
	find_result(n_x, f2_);
	constexpr double q = 10;
	COMPLEX_DOUBLE x_precise = (*x1_)[0];
	COMPLEX_DOUBLE difference = abs((*n_x)[0] - x_precise);
	COMPLEX_DOUBLE max = abs(x_precise) > q ? difference / x_precise : difference;

	for (int i = 1; i < n_; i++)
	{
		x_precise = (*x1_)[i];
		difference = abs((*n_x)[i] - x_precise);
		const COMPLEX_DOUBLE t = difference / x_precise;
		max = abs(x_precise) > q ? difference / x_precise : difference;
	}

	delete n_x;
	return max;
}
//Чтение данных из файла
std::istream& lab_matrix::read_data_from_file(std::istream& input_stream) const
{

	const auto temp = new SOLUTION_MATRIX(n_);
	for (int i = 0; i < n_; i++)
		temp->at(i).resize(n_);
	for (int i = 0; i < n_; i++)
		for (int j = 0; j < n_; j++)
		{
			input_stream >> (*temp)[i][j];
		}

	for (int i = 0; i < n_; i++)
		for (int j = 0; j < n_; j++)
		{
			add_to_matrix(matrix_, i, j, (*temp)[i][j]);
		}
	for (int i = 0; i < n_; i++)
	{
		input_stream >> (*f_)[i];
	}

	for (int i = 0; i < n_; i++)
		for (int j = 0; j < n_; j++)
		{
			add_to_matrix(
				b_matrix_,
				i,
				j,
				get_from_matrix(matrix_, i, j)
			);
		}

	delete temp;
	return input_stream;
}
//Вывод матрицы
std::ostream& lab_matrix::print_matrix(std::ostream& out) const
{
	for (int i = 0; i < n_; i++)
	{
		for (int j = 0; j < n_; j++)
			out << get_from_matrix(matrix_, i, j).real() << '\t';
		out << std::endl;
	}
	return out;
}
//Вывод матрицы и столбца f
std::ostream& lab_matrix::print_matrix_and_f(std::ostream& out) const
{
	for (int i = 0; i < n_; i++)
	{
		for (int j = 0; j < n_; j++)
			out << get_from_matrix(matrix_, i,j).real() << '\t';
		out << "||\t" << (*f_)[i].real() << std::endl;
	}
	return out;
}

//Вывод результата (x)
std::ostream& lab_matrix::print_result(std::ostream& out) const
{
	for (int i = 0; i < n_; i++)
	{
		out << 'x' << i + 1 << " = " << (*x_)[i].real() << std::endl;
	}
	return out;
}