#include <iostream>
#include <random>
#include <Windows.h>
#include <fstream>

#include "lab_matrix.h"

void print_results(
	const lab_matrix* solution,
	std::ostream& out,
    const double l_n,
	double range_begin,
	double range_end
);
COMPLEX_DOUBLE find_error(
    int count, 
    int n,
    double l_n,
    double range_begin, 
    double range_end
);
COMPLEX_DOUBLE find_error_bad(
    int count,
    int n,
    double l_n,
    double range_begin,
    double range_end,
    int k
);

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::string filename = "matrix.txt";
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        std::cout << "Не получилось открыть файл: matrix.txt";
        return 0;
    }
    int range_begin, range_end;
    double l_n;
    int
        choice_for_input,
        choice_for_output,
		n,
		l;

    ifs >> choice_for_input >> choice_for_output >> n >> l_n >> range_begin >> range_end;
    l = static_cast<int>(static_cast<double>(n) * l_n);
    lab_matrix* solution = nullptr;

    switch (choice_for_input)
    {
        //Ввод данных из файла
	    case 1:
		    {
				if (solution == nullptr)
	    			solution = new lab_matrix(n, static_cast<int>(l_n * n));
				solution->read_data_from_file(ifs);
		    }
    		break;

    	//Получение данных "случайным" способом
        case 2:
	        {
                if (choice_for_output != 3)
                {
                    solution = new lab_matrix(n, static_cast<int>(l_n * n));
	                solution->fill_random(range_begin, range_end);
                }
	        }
            break;

        default:
        {
            std::cout << "Incorrect data for choice, try again";
            return -1;
        }
    }

    switch (choice_for_output)
    {
        //Вывод данных о результате работы программы в консоль
	    default:
	    {
            /// <summary>
            /// вывод
            /// </summary>
            /// <returns></returns>
            solution->solve();
            solution->fill_x1_and_f2(range_begin, range_end);
	        print_results(
                solution,
                std::cout,
                l_n,
                range_begin,
                range_end
            );
	    }
	    break;
        //Вывод данных о результате работы программы в файл output.txt
		case 2:
        {
            /// <summary>
            /// вывод
            /// </summary>
            /// <returns></returns>
            if (solution == nullptr)
				solution = new lab_matrix(n, static_cast<int>(l_n * n));
            std::ofstream ofs("output.txt", std::ios::trunc);
            if (!ofs.is_open()) {
                std::cout << "Не получилось открыть файл: output.txt";
                return 0;
            }
            solution->solve();
            solution->fill_x1_and_f2(range_begin, range_end);
            ofs << "-------------------------------------\n" << "Матрица" << std::endl;
            print_results(
                solution,
                ofs,
                l_n,
                range_begin,
                range_end
            );
            ofs << "-------------------------------------" << std::endl;
            ofs.close();
        }
    	break;

            //Вывод данных о результате работы программы в файл output.txt, но k раз,
    		//сработает только если данные записываются в файл рандомно
        case 3:
        {
            if (choice_for_input != 2) {
                break;
            }
        	int k;
            ifs >> k;
            std::ofstream ofs("output.txt", std::ios::trunc);
            if (!ofs.is_open()) {
                std::cout << "Не получилось открыть файл: output.txt";
                return 0;
            }
            /// <summary>
            /// вывод
            /// </summary>
            /// <returns></returns>
            for (int i = 0; i < k; i++)
            {
                delete solution;
                solution = new lab_matrix(n, static_cast<int>(l_n * n));
                solution->fill_random(range_begin, range_end);
                solution->solve();
                solution->fill_x1_and_f2(range_begin, range_end);
                ofs << "-------------------------------------\n" << "\a Матрица №" << i + 1 << std::endl;
                print_results(
                    solution,
                    ofs,
                    l_n,
                    range_begin,
                    range_end
                );
                ofs << "-------------------------------------\n" << std::endl;
            }
        }
        break;
    }

    return 0;

}

void print_results(
	const lab_matrix* solution,
	std::ostream& out,
    const double l_n,
	const double range_begin,
	const double range_end
)
{
    //
    const int n = solution->get_n();
    const int l = solution->get_l();

    //Вывод результата работы
    //solution->print_matrix_and_f(out);
	out	<< "Результат" << std::endl;
    solution->print_result(out);
    //Погрешность для матрицы пользователя
    out << "Погрешность: ";
        const auto main_error = solution->find_errors(range_begin, range_end);
	out << main_error.real()
		<< std::endl;
    //Погрешность для хорошо обусловленной матрицы
    out << "\n\a Погрешность. Хорошо обусловленных матриц: \nМатрица порядка 1: ";
    const auto error1 = find_error(
            10,
            n,
            l_n,
            range_begin,
            range_end
        );
    out << error1.real() << std::endl;
    /*out << "Матрица порядка 2: ";
    const auto error2 = find_error(
        10,
        n * n,
        l_n,
        range_begin,
        range_end
    );
	out << error2.real() << std::endl;*/
    //Вывод погрешности для плохо обусловленной матрицы
    out << "\a Погрешность. Плохо обусловленных матриц:\nk = 2, Погрешность = ";
        const auto error_bad2 = find_error_bad(
            10,
            n,
            l_n,
            range_begin,
            range_end,
            2
        );
        out << error_bad2.real();
        out << "\nk = 4, Погрешность = ";
        const auto error_bad4 = find_error_bad(
            10,
            n,
            l_n,
            range_begin,
            range_end,
            4
        );
    out << error_bad4.real();
    out << "\nk = 6, Погрешность = ";
        const auto error_bad6 = find_error_bad(
            10,
            n,
            l_n,
            range_begin,
            range_end,
            4
        );
    out << error_bad6.real();
    out << std::endl;
}

COMPLEX_DOUBLE find_error(
    const int count,
   const int n,
   const double l_n,
   const double range_begin,
   const double range_end
)
{
    COMPLEX_DOUBLE sum_error = 0;
    for (int t = 0; t < count; t++)
    {
        const auto solution = new lab_matrix(n, static_cast<int>(l_n * n));
		solution->fill_random(range_begin, range_end);
        solution->solve();
        solution->fill_x1_and_f2(range_begin, range_end);
        sum_error += solution->find_errors(range_begin, range_end);
        delete solution;
    }

    return sum_error / static_cast<COMPLEX_DOUBLE>(count);
}

COMPLEX_DOUBLE find_error_bad(
	const int count,
	const int n,
	const double l_n,
	const double range_begin,
	const double range_end,
	const int k
)
{
    COMPLEX_DOUBLE sum_error = 0;
    for (int t = 0; t < count; t++)
    {
        const auto solution = new lab_matrix(n, static_cast<int>(l_n * n));
        solution->fill_random_bad(
            range_begin, 
            range_end, 
            k
        );
        solution->solve();
        solution->fill_x1_and_f2(range_begin, range_end);
        sum_error += solution->find_errors(range_begin, range_end);
        delete solution;
    }
    return sum_error / static_cast<COMPLEX_DOUBLE>(count);
}