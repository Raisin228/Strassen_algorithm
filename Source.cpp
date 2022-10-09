#include <iostream>
#include <iomanip> // для красивого вывода матриц
#include <vector>
#include <ctime>
using namespace std;

// здесь сверху я буду складывать прототипы функций, а сами дополнительные функции я буду писать 
// в самом низу программного кода после функции main для того чтобы не загромождать программный код
// ниже где будет прописана сама функция я оставил коментарии по поводу её работы 
int output_matrix(vector <vector <int> >A, int row, int column);
int addition_zeros(int n, int k);
int multiplication_by_definition(vector <vector <int> > A, vector <vector <int> > B, int m, int n, int n1, int p, vector <vector <int> > C);
//int addition_zeros(int n, int k);

// основная функция в которой происходит объявление всех 3 матриц и заполнение матрицы А и В случайными числами
// после этого я вызываю функцию нахождения их произведения (по определению) а затем вызывается функция Метод Штрассена
int main()
{
	long long m, n, n1, p;
	int k, add_z;
	cout << "Enter the size of the matrix A (m x n): ";
	cin >> m >> n;
	cout << "Enter the size of the matrix B (n x p): ";
	cin >> n1 >> p;
	cout << "Enter the value of the k parameter: ";
	cin >> k;
	vector <vector <int> > A(m, vector <int>(n));
	vector <vector <int> > B(n1, vector <int>(p)); // вектора это динамические массивы в с++
	vector <vector <int> > C(m, vector <int>(p));

	srand(time(NULL)); // установка зерна генератора случайных чисел при помощи ctime

	// ввод размера матрицы а и б и проверка их размеров на соответстие 
	// если размеры неправильные вывод сообщения об ошибке и прекращение работы программы
	if (n1 != n)
	{
		cout << "ERROR the matrices have incorrect dimensions";
		return 0;
	}

	// заполнили матриц случайными числами и вывод матрицы А и В на экран используя самописную функцию 
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			A[i][j] = rand() % 19 - 9;
	for (int i = 0; i < n1; i++)
		for (int j = 0; j < p; j++)
			B[i][j] = rand() % 19 - 9;
	cout << "Matrix A" << "\n";
	output_matrix(A, m, n);
	cout << "Matrix B" << "\n";
	output_matrix(B, n1, p);

	multiplication_by_definition(A, B, m, n, n1, p, C);
	//----------------------------------------закончилось умножение матриц по определению-------------------------------

	//----------------------------------------начинается подготовление к алгоритму штрассена----------------------------
	// добавление в матрицы 0 строк и столбцов
	add_z = addition_zeros(n, k);
	A.resize(add_z);
	for (int i = 0; i < add_z; i++)
		A[i].resize(add_z, 0);
	B.resize(add_z);
	for (int i = 0; i < add_z; i++)
		B[i].resize(add_z, 0);
	// sdfasdfas

}








// вычисление количества добавленных строк и столбцов
int addition_zeros(int n, int k)
{
	int N = n;
	while (true)
	{
		while (N > k && N % 2 == 0)
		{
			N /= 2;
			if (N <= k)
				return n;
		}
		if (N <= k)
			return n;
		else
		{
			n++;
			N = n;
		}
	}
}

// умножение матриц и вывод результирующей матрицы на экран (умножение по определению)
int multiplication_by_definition(vector <vector <int> > A, vector <vector <int> > B, int m, int n, int n1, int p, vector <vector <int> > C)
{
	long long counter_addition = 0, counter_multiplication = 0;
	counter_addition = (n1 - 1) * m * p;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
		{
			for (int q = 0; q < p; q++)
			{
				C[i][q] += A[i][j] * B[j][q];
			}
			counter_multiplication += n;
		}

	cout << "The product of matrices by definition" << "\n";
	output_matrix(C, m, p);
	cout << "The number of elementary multiplication operations: " << counter_multiplication << "\n";
	cout << "The number of elementary addition operations: " << counter_addition << "\n";
	cout << "Total number of elementary operations: " << counter_multiplication + counter_addition << "\n";
	return 0;
}

// функция для вывода массивов с блокировкой вывода в поток если размер матрицы больше чем 30 * 30
int output_matrix(vector <vector <int> >A, int row, int column)
{
	if (row > 30 || column > 30)
	{
		cout << "The dimensions of the matrices exceed (30x30).Their withdrawal is blocked" << "\n\n";
		return 0;
	}
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
			cout << setw(6) << A[i][j];
		cout << "\n";
	}
	cout << "\n";
	return 0;
}



