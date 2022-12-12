#include "Mypq.h"
#include<vector>
#include <stdlib.h>
#include <time.h>
#include <mutex>
#include <thread>
using namespace std;

template<class T>
struct ADD
{

	Heap<T>* heap_;
	int min_, max_;
	ADD(Heap<T>& heap, int min, int max)
	{
		heap_ = &heap;
		min_ = min;
		max_ = max;
	}

	void operator()(int n_operations)
	{
		for (int x = 0; x < n_operations; x++)
		{
			int number = min_ + rand() % (max_);
			int number2 = min_ + rand() % (max_);
			cout << "voy a añadir: " << number << endl;
			cout << endl;
			heap_->Push(number,number2);
		}
	}
};

template<class T>
struct BORRAR
{

	Heap<T>* heap_;
	int min_, max_;
	BORRAR(Heap<T>& heap, int min, int max)
	{
		heap_ = &heap;
		min_ = min;
		max_ = max;
	}

	void operator()(int n_operations)
	{
		for (int x = 0; x < n_operations; x++)
		{
			cout << "voy a borrar: " <<endl;
			cout << endl;
			heap_->PopTop();
		}
	}
};

int main()
{
	srand(time(NULL));
	Heap<int> A(100);
	int num;
	int num2;



	ADD<int> Add1(A, 1, 10000);
	BORRAR<int> Borrar1(A, 1, 10000);

	thread* threads[2];

	thread first(Add1, 20);
	thread second(Borrar1, 12);
	
	threads[0] = &first;
	threads[1] = &second;

	threads[0]->join();
	threads[1]->join();
	A.print();

/* 

	for (int x = 0; x < 15; x++)
	{
		num = rand() % 1001;
		num2 = rand() % 1001;
		A.Push(num, num2);
	}
	A.print();
	cout << endl;

	A.PopTop();
	A.print();
	cout << endl;

	A.PopTop();
	A.print();
	cout << endl;

	A.PopTop();
	A.print();
	cout << endl;

	A.PopTop();
	A.print();
	cout << endl;

	A.PopTop();
	A.print();
	cout << endl;

	*/

}