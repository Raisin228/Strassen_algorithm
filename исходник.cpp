#include <iostream>
#include <iomanip> // для красивого вывода матриц
#include <vector>
#include <ctime>
using namespace std;

int counter_add = 0, counter_mult = 0;

// здесь сверху я буду складывать прототипы функций, а сами дополнительные функции я буду писать 
// в самом низу программного кода после функции main для того чтобы не загромождать программный код
// ниже где будут прописаны сами функции я оставил коментарии по поводу их работы 
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
	long long m, n, n1, p; 
	int k, add_z;
	cout << "Enter the size of the matrix A (m x n): ";
	cin >> m >> n;
	cout << "Enter the size of the matrix B (n x p): ";
	cin >> n1 >> p;
	cout << "Enter the value of the k parameter: ";
	cin >> k;
	vector <vector <int> > A(m, vector <int>(n));
	vector <vector <int> > B(n1, vector <int>(p)); // вектора это динамические массивы в с++ я буду их использовать для хранения 2-х массивов чтобы не работать с динамическим 
	vector <vector <int> > C(m, vector <int>(p));  // выделением памяти вручную

	srand(time(NULL)); // установка зерна генератора случайных чисел при помощи ctime

	// ввод размера матрицы а и б и проверка их размеров на соответстие 
	// если размеры неправильные вывод сообщения об ошибке и прекращение работы программы
	if (n1 != n)
	{
		cout << "ERROR the matrices have incorrect dimensions";
		return 0;
	}

	// заполнили матриц А и В случайными числами [-9; 9] и вывод матрицы А и В на экран используя самописную функцию 
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

	//----------------------------------------начало умножение матриц по определению-------------------------------
	cout << "The product of matrices by definition" << "\n";
	C = multiplication_by_definition(A, B, m, n, n1, p);
	output_matrix(C, m, p);
	cout << "The number of elementary addition operations: " << counter_add << "\n";
	cout << "The number of elementary multiplication operations: " << counter_mult << "\n";
	cout << "Total number of elementary operations: " << counter_mult + counter_add << "\n\n";
	counter_add = 0;
	counter_mult = 0;
	//----------------------------------------закончилось умножение матриц по определению-------------------------------

	//----------------------------------------начинается подготовление к алгоритму штрассена----------------------------
	add_z = addition_zeros(n, k); // считаем до скольки строк и столбцов нам нужно дополнять исходную матрицу
	A.resize(add_z); // добавление строк состоящих из 0 при помощи стандартного метода
	for (int i = 0; i < add_z; i++)
		A[i].resize(add_z, 0); // расширение каждой строки до нужного размера при помощи вставки 0
	// тоже самое только для В
	B.resize(add_z);
	for (int i = 0; i < add_z; i++) 
		B[i].resize(add_z, 0);
	//----------------------------------------начало умножения матриц методом Штрассена-------------------------------
	C = Strassen_multiplication(A, B, add_z, k);
	cout << "The product of matrices according to the Strassen algorithm" << "\n";
	output_matrix(C, add_z, add_z);
	cout << "The number of elementary addition operations: " << counter_add << "\n";
	cout << "The number of elementary multiplication operations: " << counter_mult << "\n";
	cout << "Total number of elementary operations: " << counter_add + counter_mult << "\n\n";
	//----------------------------------------конец умножения матриц методом Штрассена-------------------------------
}

// метод штрассена
vector <vector <int> > Strassen_multiplication(vector <vector <int> > A, vector <vector <int> > B, int n, int k) // в функцию передаём 2 вектора их размер n * n и параметр К до которого мы будем делить матрицы
{
	// если размеры матриц стали <= чем k тогда мы создаём вектор С и решаем матрицы умножением по определению
	if (n <= k)
	{
		vector <vector <int> > C(n, vector <int>(n));
		C = multiplication_by_definition(A, B, n, n, n, n);
		return C;
	}
	vector <vector <int> > C(n, vector <int>(n)); // вектор С в котором будет храниться результат умножения 2-х матриц А и В
	int n_new = n / 2; // 4 подматрицы размера n_new

	vector <vector <int> > A11(n_new, vector <int>(n_new));
	vector <vector <int> > A12(n_new, vector <int>(n_new)); // создаём маленькие подматрицы 
	vector <vector <int> > A21(n_new, vector <int>(n_new)); // в них мы сейчас будем будем раскладывать наши матрицы А и В
	vector <vector <int> > A22(n_new, vector <int>(n_new));
	vector <vector <int> > B11(n_new, vector <int>(n_new));
	vector <vector <int> > B12(n_new, vector <int>(n_new));
	vector <vector <int> > B21(n_new, vector <int>(n_new));
	vector <vector <int> > B22(n_new, vector <int>(n_new));

	// разложение матриц в подматрицы (здесь и происходит деление)
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

	// создание векторов P1..P7 для формул Штрассена
	vector <vector <int> > P1(n_new, vector <int>(n_new)); 
	vector <vector <int> > P2(n_new, vector <int>(n_new));
	vector <vector <int> > P3(n_new, vector <int>(n_new));
	vector <vector <int> > P4(n_new, vector <int>(n_new));
	vector <vector <int> > P5(n_new, vector <int>(n_new));
	vector <vector <int> > P6(n_new, vector <int>(n_new));
	vector <vector <int> > P7(n_new, vector <int>(n_new));

	P1 = Strassen_multiplication(A11, matrix_subtraction(B12, B22, n_new), n_new, k); // P1 = A11 * (B12 - B22)
	P2 = Strassen_multiplication(addition_matrix(A11, A12, n_new), B22, n_new, k); // P2 = (A11 + A12) * B22
	P3 = Strassen_multiplication(addition_matrix(A21, A22, n_new), B11, n_new, k); // P3 = (A21 + A22) * B11
	P4 = Strassen_multiplication(A22, matrix_subtraction(B21, B11, n_new), n_new, k); // P4 = A22 * (B21 - B11)
	P5 = Strassen_multiplication(addition_matrix(A11, A22, n_new), addition_matrix(B11, B22, n_new), n_new, k); // P5 = (A11 + A22) * (B11 + B22)
	P6 = Strassen_multiplication(matrix_subtraction(A12, A22, n_new), addition_matrix(B21, B22, n_new), n_new, k); // P6 = (A12 - A22) * (B21 + B22)
	P7 = Strassen_multiplication(matrix_subtraction(A11, A21, n_new), addition_matrix(B11, B12, n_new), n_new, k); // P7 = (A11 - A21) * (B11 + B12)

	// создание векторов С1...С4 
	vector <vector <int> > C11(n_new, vector <int>(n_new));
	vector <vector <int> > C12(n_new, vector <int>(n_new));
	vector <vector <int> > C21(n_new, vector <int>(n_new));
	vector <vector <int> > C22(n_new, vector <int>(n_new));

	C11 = addition_matrix(matrix_subtraction(addition_matrix(P5, P4, n_new), P2, n_new), P6, n_new); // C11 = P5 + P4 - P2 + P6
	C12 = addition_matrix(P1, P2, n_new); // C12 = P1 + P2
	C21 = addition_matrix(P3, P4, n_new); // C21 = P3 + P4
	C22 = matrix_subtraction(matrix_subtraction(addition_matrix(P5, P1, n_new), P3, n_new), P7, n_new); // C22 = P5 + P1 - P3 - P7

	// теперь из матриц С11...C22 мы будем собирать результирующую матрицу С также как когда мы делили матрицы А и В только в обратную сторону
	for (int i = 0; i < n_new; i++)
		for (int j = 0; j < n_new; j++)
		{
			C[i][j] = C11[i][j];
			C[i][j + n_new] = C12[i][j];
			C[i + n_new][j] = C21[i][j];
			C[i + n_new][j + n_new] = C22[i][j];
		}
	return C; // возвращаем С когда матрицы полностью умножены
}

// функция для вычитания 2-х векторов (будет использоваться когда будут формулы P1...P7)
vector <vector <int> > matrix_subtraction(vector <vector <int> > A, vector <vector <int> > B, int n) // передаётся 2 вектора и их размер n * n
{
	vector <vector <int> > C(n, vector <int>(n));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			C[i][j] = A[i][j] - B[i][j]; // поэлементное вычитание элементов
			counter_add++; // обновляем счётчик элементарных операций сложения на ++
		}
	return C;
}

// функция для сложение 2-х векторов (она будет использоваться когда будут формулы P1...P7)
vector <vector <int> > addition_matrix(vector <vector <int> > A, vector <vector <int> > B, int n) // передаётся 2 вектора и их размер n * n
{
	vector <vector <int> > C(n, vector <int>(n));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			C[i][j] = A[i][j] + B[i][j]; // поэлементное сложение матриц
			counter_add++;
		}
	return C;
}

// вычисление количества добавленных строк и столбцов
int addition_zeros(int n, int k) // n - размеры одной из сторон матриц сейчас к - это то до какого размера нужно делить матрицы
{
	int N = n;
	while (true)
	{
		while (N > k && N % 2 == 0) // берём текущие размеры и проверяем чтобы они были четными и дальше делим пополам => если размеры становятся чётными и < к тогда возвращаем n
		{
			N /= 2;
			if (N <= k)
				return n;
		} // когда мы вылетаем из этого цикла при одном из N тогда мы берём размер на 1 больше и начинаем проверять его
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
vector <vector <int> > multiplication_by_definition(vector <vector <int> > A, vector <vector <int> > B, int m, int n, int n1, int p) // 2 вектора m и n размеры А n1 и p размеры B
{
	vector <vector <int> > C(m, vector <int>(p));
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			for (int q = 0; q < p; q++)
			{
				C[i][q] += A[i][j] * B[j][q];
			}
		}
	}
	counter_mult += (m * n) * p; // счётчики элементарных операций сложения и *
	counter_add += (n - 1) * m * p;
	return C;
}

// функция для вывода массивов с блокировкой вывода в поток если размер матрицы больше чем 30 * 30
int output_matrix(vector <vector <int> >A, int row, int column) // матрица на вывод и её строки и колонки (размеры)
{
	// если размеры больше одного из допустимых параметров то выводим сообщение о блокировке вывода на экран
	if (row > 32 || column > 32) 
	{
		cout << "The dimensions of the matrices exceed (30x30).Their withdrawal is blocked" << "\n\n";
		return 0;
	}
	// иначе выводим матрицы
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
			cout << setw(6) << A[i][j];
		cout << "\n";
	}
	cout << "\n";
	return 0;
}



