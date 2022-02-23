#include <iostream>
#include <omp.h>
#include <string>

#define MAX_THREADS 5

void task1();
void task2();
void task3();
void task4();
void task5();

int main()
{
	char key;
	bool isExit = false;

	while (!isExit) {
		std::cout << "Select task (1-5): ";
		std::cin >> key;

		switch (key)
		{
		case '1':
			task1();
			break;
		case '2':
			task2();
			break;
		case '3':
			task3();
			break;
		case '4':
			task4();
			break;
		case '5':
			task5();
			break;
		default:
			isExit = true;
			break;
		}

		std::cout << std::endl;
		std::cout << std::endl;
	}
	getchar();
}


// Вывести параллельно “Hello World, NN” 5 - 10 раз
void task1() {
	#pragma omp parallel num_threads(MAX_THREADS)
	{
		printf("Hello World! %d\n", omp_get_thread_num());
	}
}



void task2() {
	const int size = 250;
	int trash_count = 50;

	int A[size][size];
	int B[size][size];
	int C[size][size];

	// случайные матрицы A и B
	#pragma omp parallel for
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++) {
			A[i][j] = rand() % 100;
			B[j][i] = rand() % 100;
		}

	double time = omp_get_wtime();

	// static - разделение на равные чанки между потоками последовательно
	// dynamic - ... по мере освобождения выдаются чанки
	// guided - разделение на усмотрение ОС, но не более [chunk], по мере освобождения
	// runtime - размер чанков определяется переменной окружения ОС, последовательно

	// 1 - с равномерным временем выполнения
	#pragma omp parallel for schedule(static,10) 
	for (int k = 0; k < 8000; k++)
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				C[i][j] = A[i][j] + B[i][j];

	printf("\t static = %lf", omp_get_wtime() - time);
	time = omp_get_wtime();

	#pragma omp parallel for schedule(dynamic,10) 
	for (int k = 0; k < 8000; k++)
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				C[i][j] = A[i][j] + B[i][j];

	printf("\t dynamic = %lf", omp_get_wtime() - time);
	time = omp_get_wtime();

	#pragma omp parallel for schedule(runtime) 
	for (int k = 0; k < 8000; k++)
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				C[i][j] = A[i][j] + B[i][j];

	printf("\t runtime = %lf", omp_get_wtime() - time);
	time = omp_get_wtime();

	#pragma omp parallel for schedule(guided,10) 
	for (int k = 0; k < 8000; k++)
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				C[i][j] = A[i][j] + B[i][j];

	printf("\t guided = %lf \n", omp_get_wtime() - time);
	time = omp_get_wtime();

	// 2 -  с неравномерным временем выполнения
	// нечетные итерации выполняются дольше
	#pragma omp parallel for schedule(static,10) 
	for (int k = 0; k < 8000; k++)
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++) {
				if (j % 2 == 1) {
					for (int p = 0; p < trash_count; p++);
				}
				C[i][j] = A[i][j] + B[i][j];
			}

	printf("\t static = %lf", omp_get_wtime() - time);
	time = omp_get_wtime();

	#pragma omp parallel for schedule(dynamic,10) 
	for (int k = 0; k < 8000; k++)
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++) {
				if (j % 2 == 1) {
					for (int p = 0; p < trash_count; p++);
				}
				C[i][j] = A[i][j] + B[i][j];
			}

	printf("\t dynamic = %lf", omp_get_wtime() - time);
	time = omp_get_wtime();

	#pragma omp parallel for schedule(runtime) 
	for (int k = 0; k < 8000; k++)
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++) {
				if (j % 2 == 1) {
					for (int p = 0; p < trash_count; p++);
				}
				C[i][j] = A[i][j] + B[i][j];
			}

	printf("\t runtime = %lf", omp_get_wtime() - time);
	time = omp_get_wtime();


	#pragma omp parallel for schedule(guided,10) 
	for (int k = 0; k < 8000; k++)
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++) {
				if (j % 2 == 1) {
					for (int p = 0; p < trash_count; p++);
				}
				C[i][j] = A[i][j] + B[i][j];
			}

	printf("\t guided = %lf \n", omp_get_wtime() - time);
	time = omp_get_wtime();

	// 3 - с неравномерным временем выполнения 
	// итерации в первой половине цикла выполняются дольше
	#pragma omp parallel for schedule(static,10) 
	for (int k = 0; k < 8000; k++)
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++) {
				if (i < size / 2) {
					for (int p = 0; p < trash_count; p++);
				}
				C[i][j] = A[i][j] + B[i][j];
			}
	printf("\t static = %lf", omp_get_wtime() - time);
	time = omp_get_wtime();

	#pragma omp parallel for schedule(dynamic,10) 
	for (int k = 0; k < 8000; k++)
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++) {
				if (i < size / 2) {
					for (int p = 0; p < trash_count; p++);
				}
				C[i][j] = A[i][j] + B[i][j];
			}
	printf("\t dynamic = %lf", omp_get_wtime() - time);
	time = omp_get_wtime();

	#pragma omp parallel for schedule(runtime) 
	for (int k = 0; k < 8000; k++)
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++) {
				if (i < size / 2) {
					for (int p = 0; p < trash_count; p++);
				}
				C[i][j] = A[i][j] + B[i][j];
			}
	printf("\t runtime = %lf", omp_get_wtime() - time);
	time = omp_get_wtime();

	for (int k = 0; k < 8000; k++)
	#pragma omp parallel for schedule(guided,10) 
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++) {
				if (i < size / 2) {
					for (int p = 0; p < trash_count; p++);
				}
				C[i][j] = A[i][j] + B[i][j];
			}

	printf("\t guided = %lf \n", omp_get_wtime() - time);
	time = omp_get_wtime();
}

void task3() {
	const int size = 10000;
	const int iters = 10000;
	int a[size];

	#pragma omp parallel for
	for (int i = 0; i < size; i++)
		a[i] = rand() % 100;

	// без распараллеливания
	long int sum = 0;
	double time = omp_get_wtime();
	for (int j = 0; j < iters; j++) {
		sum = 0;
		for (int i = 0; i < size; i++)
			sum += a[i];
	}
	printf("\tseq: %d (time=%lf)", sum, (omp_get_wtime() - time));

	// reduction
	sum = 0;
	time = omp_get_wtime();
	for (int j = 0; j < iters; j++) {
		sum = 0;

		#pragma omp parallel for reduction (+:sum)
		for (int i = 0; i < size; i++)
			sum += a[i];

	}
	printf("\treduction: %d (time=%lf)", sum, (omp_get_wtime() - time));

	// критическая секция
	sum = 0;
	time = omp_get_wtime();
	for (int j = 0; j < iters; j++) {
		sum = 0;

		#pragma omp parallel for
		for (int i = 0; i < size; i++)
		#pragma omp critical
		{
			sum += a[i];
		}
	}
	printf("\tcritical: %d (time=%lf)", sum, omp_get_wtime() - time);

}

void task4() {
	int a[MAX_THREADS];

	for (int i = 0; i < MAX_THREADS; i++)
	{
		a[i] = 0;
	}

	int sum = 0;

	// с барьером
	std::cout << "with barrier:" << std::endl;
	#pragma omp parallel num_threads(MAX_THREADS)
	{
		int id = omp_get_thread_num();
		printf("\n %d", id + 1);
		sum += id + 1;

		#pragma omp barrier
		a[id] = sum + id + 1;
		printf("\na[%d] = %d", id+1, a[id]);
	}
	std::cout << std::endl;
	std::cout << std::endl;

	// без
	sum = 0;
	std::cout << "without:" << std::endl;
	#pragma omp parallel num_threads(MAX_THREADS)
	{
		int id = omp_get_thread_num();
		printf("\n %d", id + 1);
		sum += id + 1;

		a[id] = sum + id + 1;
		printf("\na[%d] = %d", id+1, a[id]);
	}
}


void task5() {
	const unsigned int size = 16000000;

	int* A = new int[size];
	int* B = new int[size];

	B[0] = 0;
	B[size - 1] = 0;

	// значение элемента массива равно его порядковому номеру
	#pragma omp parallel for
	for (int i = 0; i < size; i++)
		A[i] = i;

	double time1, time2;
	double time = omp_get_wtime();

	// последовательно
	for (int i = 1; i < size - 1; i++) {
		B[i] = (A[i - 1] + A[i] + A[i + 1]) / 3;
	}
	time1 = omp_get_wtime() - time;
	printf("\t seq = %lf", time1);

	time = omp_get_wtime();

	// параллельно
	#pragma omp parallel for
	for (int i = 1; i < size - 1; i++) {
		B[i] = (A[i - 1] + A[i] + A[i + 1]) / 3;
	}
	time2 = omp_get_wtime() - time;
	printf("\t paral = %lf", time2);

	printf("\nin %lf times", time1 / time2);

	delete[] A;
	delete[] B;
}
