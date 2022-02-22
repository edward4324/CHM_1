#ifndef LAB_MATRIX
#define LAB_MATRIX
#include <string>
#include <vector>
#include <complex>
#define COMPLEX_DOUBLE std::complex <double>
#define SOLUTION_MATRIX std::vector<std::vector< COMPLEX_DOUBLE > >
#define SOLUTION_VECTOR std::vector< COMPLEX_DOUBLE >

class lab_matrix
{
	SOLUTION_MATRIX* matrix_;//матрица с исходными данными
	SOLUTION_MATRIX* b_matrix_;//матрица с разложением
	SOLUTION_VECTOR
		*f_,//вектор с исходным значением для иксов
		*f2_,//вектор с рандомным значением для иксов (используется при поиске погрешности)
		*x_,//вектор с исходным решением системы
		*x1_;//вектор с решением системы (используется при поиске погрешности)
	const int
		n_,//размерности матрицы
		l_;//половина ширины ленты

public:
	lab_matrix(const int n, const int l)
		:
		n_(n),
		l_(l)
	{
		matrix_ = new SOLUTION_MATRIX(n_);
		for (int i = 0; i < n; i++)
			(*matrix_)[i].resize(l_);
		b_matrix_ = new SOLUTION_MATRIX(n_);
		for (int i = 0; i < n; i++)
			(*b_matrix_)[i].resize(l_);

		f_ = new SOLUTION_VECTOR(n);
		f2_ = new SOLUTION_VECTOR(n);
		x_ = new SOLUTION_VECTOR(n);
		x1_ = new SOLUTION_VECTOR(n);
	}
	~lab_matrix()
	{
		delete matrix_;
		delete b_matrix_;

		delete f_;
		delete f2_;

		delete x_;
		delete x1_;
	}

	int get_n() const { return n_; }
	int get_l() const { return l_; }

	void fill_x1_and_f2(double range_begin, double range_end) const;
	void fill_random(double range_begin, double range_end) const;
	void fill_random_bad(double range_begin, double range_end, int k) const;

	void solve() const;

	//void find_result() const;
	COMPLEX_DOUBLE find_errors(const double range_begin, const double range_end) const;
	void normalize() const
	{
		normalize(x_, f_);
	}
	bool is_result_wrong(
		const SOLUTION_VECTOR& x,
		const SOLUTION_VECTOR& f
	) const;
	//void get_b_c() const;
	std::istream& read_data_from_file(std::istream& input_stream) const;
	std::ostream& print_result(std::ostream& out) const;
	std::ostream& print_matrix(std::ostream& out) const;
	std::ostream& print_matrix_and_f(std::ostream& out) const;

	static void normalize(SOLUTION_VECTOR* input_x, SOLUTION_VECTOR* input_f)
	{
		for (int i = 0; i < input_x->size(); i++)
		{
			const std::complex<double> k = static_cast<std::complex<double>>(1) / (*input_f)[i];
			(*input_x)[i] = (*input_x)[i] * k;
			(*input_f)[i] = (*input_f)[i] * k;
		}
	}

private:
	static COMPLEX_DOUBLE max_complex(COMPLEX_DOUBLE lhs, COMPLEX_DOUBLE rhs)
	{
		if (lhs.real() > rhs.real())
		{
			if (lhs.imag() > rhs.real())
				return lhs;
		}
		return rhs;
	}
	static COMPLEX_DOUBLE set_precision(const double value, const int precision)
	{
		const double power = pow(10, precision);
		return
			{static_cast<int>(value * power) / power, 0};
	}
	void find_result(SOLUTION_VECTOR* x, const SOLUTION_VECTOR* input_f) const;
	//Добавление новых значений в векторы данных
	void add_to_matrix(
		SOLUTION_MATRIX* matrix,
		const int i,
		const int j,
		const COMPLEX_DOUBLE value
	) const
	{
		if (
			!(i < 0 || i >= n_ || j - i + l_ - 1 < 0 || j - i + l_ - 1 >= l_)
			)
			(*matrix)[i][j - i + l_ - 1] = value;
	}
	//Получение значения из основной матрицы
	COMPLEX_DOUBLE get_from_matrix(
		const SOLUTION_MATRIX* matrix,
		const int i,
		const int j
	             ) const
	{
		if (i < 0 || i >= n_ || j - i + l_ - 1 < 0 || j - i + l_ - 1 >= l_)
			return 0;
		return (*matrix)[i][j - i + l_ - 1];
	}
	int k0(const int i) const
	{
		return
			(i <= l_ ? 1 : i - l_ + 1) - 1;
	}
	int kn(const int i) const
	{
		return
			(i <= n_ - l_ ? i + l_ - 1 : n_ - 1);
	}
	static COMPLEX_DOUBLE random(double range_begin, double range_end);
};
#endif 