#include <iostream>
#include <iomanip> // äëÿ êðàñèâîãî âûâîäà ìàòðèö
#include <vector>
#include <ctime>
using namespace std;

int counter_add = 0, counter_mult = 0;

// çäåñü ñâåðõó ÿ áóäó ñêëàäûâàòü ïðîòîòèïû ôóíêöèé, à ñàìè äîïîëíèòåëüíûå ôóíêöèè ÿ áóäó ïèñàòü 
// â ñàìîì íèçó ïðîãðàììíîãî êîäà ïîñëå ôóíêöèè main äëÿ òîãî ÷òîáû íå çàãðîìîæäàòü ïðîãðàììíûé êîä
// íèæå ãäå áóäóò ïðîïèñàíû ñàìè ôóíêöèè ÿ îñòàâèë êîìåíòàðèè ïî ïîâîäó èõ ðàáîòû 
int output_matrix(vector <vector <int> >A, int row, int column);
int addition_zeros(int n, int k);
vector <vector <int> > matrix_subtraction(vector <vector <int> > A, vector <vector <int> > B, int n);
vector <vector <int> > addition_matrix(vector <vector <int> > A, vector <vector <int> > B, int n);
vector <vector <int> > multiplication_by_definition(vector <vector <int> > A, vector <vector <int> > B, int m, int n, int n1, int p);
vector <vector <int> > Strassen_multiplication(vector <vector <int> > A, vector <vector <int> > B, int n, int k);

// îñíîâíàÿ ôóíêöèÿ â êîòîðîé ïðîèñõîäèò îáúÿâëåíèå âñåõ 3 ìàòðèö è çàïîëíåíèå ìàòðèöû À è Â ñëó÷àéíûìè ÷èñëàìè
// ïîñëå ýòîãî ÿ âûçûâàþ ôóíêöèþ íàõîæäåíèÿ èõ ïðîèçâåäåíèÿ (ïî îïðåäåëåíèþ) à çàòåì âûçûâàåòñÿ ôóíêöèÿ Ìåòîä Øòðàññåíà
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
	vector <vector <int> > B(n1, vector <int>(p)); // âåêòîðà ýòî äèíàìè÷åñêèå ìàññèâû â ñ++ ÿ áóäó èõ èñïîëüçîâàòü äëÿ õðàíåíèÿ 2-õ ìàññèâîâ ÷òîáû íå ðàáîòàòü ñ äèíàìè÷åñêèì 
	vector <vector <int> > C(m, vector <int>(p));  // âûäåëåíèåì ïàìÿòè âðó÷íóþ

	srand(time(NULL)); // óñòàíîâêà çåðíà ãåíåðàòîðà ñëó÷àéíûõ ÷èñåë ïðè ïîìîùè ctime

	// ââîä ðàçìåðà ìàòðèöû à è á è ïðîâåðêà èõ ðàçìåðîâ íà ñîîòâåòñòèå 
	// åñëè ðàçìåðû íåïðàâèëüíûå âûâîä ñîîáùåíèÿ îá îøèáêå è ïðåêðàùåíèå ðàáîòû ïðîãðàììû
	if (n1 != n)
	{
		cout << "ERROR the matrices have incorrect dimensions";
		return 0;
	}

	// çàïîëíèëè ìàòðèö À è Â ñëó÷àéíûìè ÷èñëàìè [-9; 9] è âûâîä ìàòðèöû À è Â íà ýêðàí èñïîëüçóÿ ñàìîïèñíóþ ôóíêöèþ 
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

	//----------------------------------------íà÷àëî óìíîæåíèå ìàòðèö ïî îïðåäåëåíèþ-------------------------------
	cout << "The product of matrices by definition" << "\n";
	C = multiplication_by_definition(A, B, m, n, n1, p);
	output_matrix(C, m, p);
	cout << "The number of elementary addition operations: " << counter_add << "\n";
	cout << "The number of elementary multiplication operations: " << counter_mult << "\n";
	cout << "Total number of elementary operations: " << counter_mult + counter_add << "\n\n";
	counter_add = 0;
	counter_mult = 0;
	//----------------------------------------çàêîí÷èëîñü óìíîæåíèå ìàòðèö ïî îïðåäåëåíèþ-------------------------------

	//----------------------------------------íà÷èíàåòñÿ ïîäãîòîâëåíèå ê àëãîðèòìó øòðàññåíà----------------------------
	add_z = addition_zeros(n, k); // ñ÷èòàåì äî ñêîëüêè ñòðîê è ñòîëáöîâ íàì íóæíî äîïîëíÿòü èñõîäíóþ ìàòðèöó
	A.resize(add_z); // äîáàâëåíèå ñòðîê ñîñòîÿùèõ èç 0 ïðè ïîìîùè ñòàíäàðòíîãî ìåòîäà
	for (int i = 0; i < add_z; i++)
		A[i].resize(add_z, 0); // ðàñøèðåíèå êàæäîé ñòðîêè äî íóæíîãî ðàçìåðà ïðè ïîìîùè âñòàâêè 0
	// òîæå ñàìîå òîëüêî äëÿ Â
	B.resize(add_z);
	for (int i = 0; i < add_z; i++) 
		B[i].resize(add_z, 0);
	//----------------------------------------íà÷àëî óìíîæåíèÿ ìàòðèö ìåòîäîì Øòðàññåíà-------------------------------
	C = Strassen_multiplication(A, B, add_z, k);
	cout << "The product of matrices according to the Strassen algorithm" << "\n";
	output_matrix(C, add_z, add_z);
	cout << "The number of elementary addition operations: " << counter_add << "\n";
	cout << "The number of elementary multiplication operations: " << counter_mult << "\n";
	cout << "Total number of elementary operations: " << counter_add + counter_mult << "\n\n";
	//----------------------------------------êîíåö óìíîæåíèÿ ìàòðèö ìåòîäîì Øòðàññåíà-------------------------------
}

// ìåòîä øòðàññåíà
vector <vector <int> > Strassen_multiplication(vector <vector <int> > A, vector <vector <int> > B, int n, int k) // â ôóíêöèþ ïåðåäà¸ì 2 âåêòîðà èõ ðàçìåð n * n è ïàðàìåòð Ê äî êîòîðîãî ìû áóäåì äåëèòü ìàòðèöû
{
	// åñëè ðàçìåðû ìàòðèö ñòàëè <= ÷åì k òîãäà ìû ñîçäà¸ì âåêòîð Ñ è ðåøàåì ìàòðèöû óìíîæåíèåì ïî îïðåäåëåíèþ
	if (n <= k)
	{
		vector <vector <int> > C(n, vector <int>(n));
		C = multiplication_by_definition(A, B, n, n, n, n);
		return C;
	}
	vector <vector <int> > C(n, vector <int>(n)); // âåêòîð Ñ â êîòîðîì áóäåò õðàíèòüñÿ ðåçóëüòàò óìíîæåíèÿ 2-õ ìàòðèö À è Â
	int n_new = n / 2; // 4 ïîäìàòðèöû ðàçìåðà n_new

	vector <vector <int> > A11(n_new, vector <int>(n_new));
	vector <vector <int> > A12(n_new, vector <int>(n_new)); // ñîçäà¸ì ìàëåíüêèå ïîäìàòðèöû 
	vector <vector <int> > A21(n_new, vector <int>(n_new)); // â íèõ ìû ñåé÷àñ áóäåì áóäåì ðàñêëàäûâàòü íàøè ìàòðèöû À è Â
	vector <vector <int> > A22(n_new, vector <int>(n_new));
	vector <vector <int> > B11(n_new, vector <int>(n_new));
	vector <vector <int> > B12(n_new, vector <int>(n_new));
	vector <vector <int> > B21(n_new, vector <int>(n_new));
	vector <vector <int> > B22(n_new, vector <int>(n_new));

	// ðàçëîæåíèå ìàòðèö â ïîäìàòðèöû (çäåñü è ïðîèñõîäèò äåëåíèå)
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

	// ñîçäàíèå âåêòîðîâ P1..P7 äëÿ ôîðìóë Øòðàññåíà
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

	// ñîçäàíèå âåêòîðîâ Ñ1...Ñ4 
	vector <vector <int> > C11(n_new, vector <int>(n_new));
	vector <vector <int> > C12(n_new, vector <int>(n_new));
	vector <vector <int> > C21(n_new, vector <int>(n_new));
	vector <vector <int> > C22(n_new, vector <int>(n_new));

	C11 = addition_matrix(matrix_subtraction(addition_matrix(P5, P4, n_new), P2, n_new), P6, n_new); // C11 = P5 + P4 - P2 + P6
	C12 = addition_matrix(P1, P2, n_new); // C12 = P1 + P2
	C21 = addition_matrix(P3, P4, n_new); // C21 = P3 + P4
	C22 = matrix_subtraction(matrix_subtraction(addition_matrix(P5, P1, n_new), P3, n_new), P7, n_new); // C22 = P5 + P1 - P3 - P7

	// òåïåðü èç ìàòðèö Ñ11...C22 ìû áóäåì ñîáèðàòü ðåçóëüòèðóþùóþ ìàòðèöó Ñ òàêæå êàê êîãäà ìû äåëèëè ìàòðèöû À è Â òîëüêî â îáðàòíóþ ñòîðîíó
	for (int i = 0; i < n_new; i++)
		for (int j = 0; j < n_new; j++)
		{
			C[i][j] = C11[i][j];
			C[i][j + n_new] = C12[i][j];
			C[i + n_new][j] = C21[i][j];
			C[i + n_new][j + n_new] = C22[i][j];
		}
	return C; // âîçâðàùàåì Ñ êîãäà ìàòðèöû ïîëíîñòüþ óìíîæåíû
}

// ôóíêöèÿ äëÿ âû÷èòàíèÿ 2-õ âåêòîðîâ (áóäåò èñïîëüçîâàòüñÿ êîãäà áóäóò ôîðìóëû P1...P7)
vector <vector <int> > matrix_subtraction(vector <vector <int> > A, vector <vector <int> > B, int n) // ïåðåäà¸òñÿ 2 âåêòîðà è èõ ðàçìåð n * n
{
	vector <vector <int> > C(n, vector <int>(n));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			C[i][j] = A[i][j] - B[i][j]; // ïîýëåìåíòíîå âû÷èòàíèå ýëåìåíòîâ
			counter_add++; // îáíîâëÿåì ñ÷¸ò÷èê ýëåìåíòàðíûõ îïåðàöèé ñëîæåíèÿ íà ++
		}
	return C;
}

// ôóíêöèÿ äëÿ ñëîæåíèå 2-õ âåêòîðîâ (îíà áóäåò èñïîëüçîâàòüñÿ êîãäà áóäóò ôîðìóëû P1...P7)
vector <vector <int> > addition_matrix(vector <vector <int> > A, vector <vector <int> > B, int n) // ïåðåäà¸òñÿ 2 âåêòîðà è èõ ðàçìåð n * n
{
	vector <vector <int> > C(n, vector <int>(n));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			C[i][j] = A[i][j] + B[i][j]; // ïîýëåìåíòíîå ñëîæåíèå ìàòðèö
			counter_add++;
		}
	return C;
}

// âû÷èñëåíèå êîëè÷åñòâà äîáàâëåííûõ ñòðîê è ñòîëáöîâ
int addition_zeros(int n, int k) // n - ðàçìåðû îäíîé èç ñòîðîí ìàòðèö ñåé÷àñ ê - ýòî òî äî êàêîãî ðàçìåðà íóæíî äåëèòü ìàòðèöû
{
	int N = n;
	while (true)
	{
		while (N > k && N % 2 == 0) // áåð¸ì òåêóùèå ðàçìåðû è ïðîâåðÿåì ÷òîáû îíè áûëè ÷åòíûìè è äàëüøå äåëèì ïîïîëàì => åñëè ðàçìåðû ñòàíîâÿòñÿ ÷¸òíûìè è < ê òîãäà âîçâðàùàåì n
		{
			N /= 2;
			if (N <= k)
				return n;
		} // êîãäà ìû âûëåòàåì èç ýòîãî öèêëà ïðè îäíîì èç N òîãäà ìû áåð¸ì ðàçìåð íà 1 áîëüøå è íà÷èíàåì ïðîâåðÿòü åãî
		if (N <= k)
			return n;
		else
		{
			n++;
			N = n;
		}
	}
}

// óìíîæåíèå ìàòðèö (ïî îïðåäåëåíèþ)
vector <vector <int> > multiplication_by_definition(vector <vector <int> > A, vector <vector <int> > B, int m, int n, int n1, int p) // 2 âåêòîðà m è n ðàçìåðû À n1 è p ðàçìåðû B
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
	counter_mult += (m * n) * p; // ñ÷¸ò÷èêè ýëåìåíòàðíûõ îïåðàöèé ñëîæåíèÿ è *
	counter_add += (n - 1) * m * p;
	return C;
}

// ôóíêöèÿ äëÿ âûâîäà ìàññèâîâ ñ áëîêèðîâêîé âûâîäà â ïîòîê åñëè ðàçìåð ìàòðèöû áîëüøå ÷åì 30 * 30
int output_matrix(vector <vector <int> >A, int row, int column) // ìàòðèöà íà âûâîä è å¸ ñòðîêè è êîëîíêè (ðàçìåðû)
{
	// åñëè ðàçìåðû áîëüøå îäíîãî èç äîïóñòèìûõ ïàðàìåòðîâ òî âûâîäèì ñîîáùåíèå î áëîêèðîâêå âûâîäà íà ýêðàí
	if (row > 32 || column > 32) 
	{
		cout << "The dimensions of the matrices exceed (30x30).Their withdrawal is blocked" << "\n\n";
		return 0;
	}
	// èíà÷å âûâîäèì ìàòðèöû
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
			cout << setw(6) << A[i][j];
		cout << "\n";
	}
	cout << "\n";
	return 0;
}



