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
vector <vector <int> > matrix_subtraction(vector <vector <int> > A, vector <vector <int> > B, int n);
vector <vector <int> > addition_matrix(vector <vector <int> > A, vector <vector <int> > B, int n);
vector <vector <int> > multiplication_by_definition(vector <vector <int> > A, vector <vector <int> > B, int m, int n, int n1, int p);
vector <vector <int> > Strassen_multiplication(vector <vector <int> > A, vector <vector <int> > B, int n, int k);

// основная функция в которой происходит объявление всех 3 матриц и заполнение матрицы А и В случайными числами
// после этого я вызываю функцию нахождения их произведения (по определению) а затем вызывается функция Метод Штрассена
int main()
{
	long long m, n, n1, p, counter_addition = 0, counter_multiplication = 0;
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

	cout << "The product of matrices by definition" << "\n";
	counter_addition = (n1 - 1) * m * p;
	counter_multiplication = (m * n) * p;
	C = multiplication_by_definition(A, B, m, n, n1, p);
	output_matrix(C, m, p);
	cout << "The number of elementary addition operations: " << counter_addition << "\n";
	cout << "The number of elementary multiplication operations: " << counter_multiplication << "\n";
	cout << "Total number of elementary operations: " << counter_multiplication + counter_addition << "\n\n";
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
	counter_addition = 0;
	counter_multiplication = 0;
	C = Strassen_multiplication(A, B, add_z, k);
	cout << "The product of matrices according to the Strassen algorithm" << "\n";
	output_matrix(C, add_z, add_z);

}

vector <vector <int> > Strassen_multiplication(vector <vector <int> > A, vector <vector <int> > B, int n, int k)
{
	// если размеры матриц стали <= чем k 
	if (n <= k)
	{
		vector <vector <int> > C(n, vector <int>(n));
		C = multiplication_by_definition(A, B, n, n, n, n);
		return C;
	}
	vector <vector <int> > C(n, vector <int>(n));
	int n_new = n / 2;

	vector <vector <int> > A11(n_new, vector <int>(n_new));
	vector <vector <int> > A12(n_new, vector <int>(n_new)); // создаём маленькие подматрицы которые сейчас будут рекурсивно делиться
	vector <vector <int> > A21(n_new, vector <int>(n_new));
	vector <vector <int> > A22(n_new, vector <int>(n_new));
	vector <vector <int> > B11(n_new, vector <int>(n_new));
	vector <vector <int> > B12(n_new, vector <int>(n_new));
	vector <vector <int> > B21(n_new, vector <int>(n_new));
	vector <vector <int> > B22(n_new, vector <int>(n_new));

	for (int q = 0; q < n_new; q++)
	{
		for (int w = 0; w < n_new; w++)
		{
			A11[q][w] = A[q][w];
			A12[q][w] = A[q][w + n_new];
			A21[q][w] = A[q + n_new][w];
			A22[q][w] = A[q + n_new][w + n_new];
			B11[q][w] = B[q][w];
			B12[q][w] = B[q][w + n_new];
			B21[q][w] = B[q + n_new][w];
			B22[q][w] = B[q + n_new][w + n_new];
		}
	}

	vector <vector <int> > P1(n_new, vector <int>(n_new));
	vector <vector <int> > P2(n_new, vector <int>(n_new));
	vector <vector <int> > P3(n_new, vector <int>(n_new));
	vector <vector <int> > P4(n_new, vector <int>(n_new));
	vector <vector <int> > P5(n_new, vector <int>(n_new));
	vector <vector <int> > P6(n_new, vector <int>(n_new));
	vector <vector <int> > P7(n_new, vector <int>(n_new));

	P1 = Strassen_multiplication(A11, matrix_subtraction(B12, B22, n_new), n_new, k);
	P2 = Strassen_multiplication(addition_matrix(A11, A12, n_new), B22, n_new, k);
	P3 = Strassen_multiplication(addition_matrix(A21, A22, n_new), B11, n_new, k);
	P4 = Strassen_multiplication(A22, matrix_subtraction(B21, B11, n_new), n_new, k);
	P5 = Strassen_multiplication(addition_matrix(A11, A22, n_new), addition_matrix(B11, B22, n_new), n_new, k);
	P6 = Strassen_multiplication(matrix_subtraction(A12, A22, n_new), addition_matrix(B21, B22, n_new), n_new, k);
	P7 = Strassen_multiplication(matrix_subtraction(A11, A21, n_new), addition_matrix(B11, B12, n_new), n_new, k);

	vector <vector <int> > C11(n_new, vector <int>(n_new));
	vector <vector <int> > C12(n_new, vector <int>(n_new));
	vector <vector <int> > C21(n_new, vector <int>(n_new));
	vector <vector <int> > C22(n_new, vector <int>(n_new));

	C11 = addition_matrix(matrix_subtraction(addition_matrix(P5, P4, n_new), P2, n_new), P6, n_new);
	C12 = addition_matrix(P1, P2, n_new);
	C21 = addition_matrix(P3, P4, n_new);
	C22 = matrix_subtraction(matrix_subtraction(addition_matrix(P5, P1, n_new), P3, n_new), P7, n_new);

	for (int i = 0; i < n_new; i++)
		for (int j = 0; j < n_new; j++)
		{
			C[i][j] = C11[i][j];
			C[i][j + n_new] = C12[i][j];
			C[i + n_new][j] = C21[i][j];
			C[i + n_new][j + n_new] = C22[i][j];
		}
	return C;
}

// функция для вычитание 2-х векторов (будет использоваться когда будут формулы P1...P7)
vector <vector <int> > matrix_subtraction(vector <vector <int> > A, vector <vector <int> > B, int n)
{
	vector <vector <int> > C(n, vector <int>(n));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			C[i][j] = A[i][j] - B[i][j];
	return C;
}

// функция для сложение 2-х векторов (она будет использоваться когда будут формулы P1...P7)
vector <vector <int> > addition_matrix(vector <vector <int> > A, vector <vector <int> > B, int n)
{
	vector <vector <int> > C(n, vector <int>(n));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			C[i][j] = A[i][j] + B[i][j];
	return C;
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

// умножение матриц (по определению)
vector <vector <int> > multiplication_by_definition(vector <vector <int> > A, vector <vector <int> > B, int m, int n, int n1, int p)
{
	vector <vector <int> > C(m, vector <int>(p));
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
		{
			for (int q = 0; q < p; q++)
			{
				C[i][q] += A[i][j] * B[j][q];
			}
		}
	return C;
}

// функция для вывода массивов с блокировкой вывода в поток если размер матрицы больше чем 30 * 30
int output_matrix(vector <vector <int> >A, int row, int column)
{
	if (row > 32 || column > 32)
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



