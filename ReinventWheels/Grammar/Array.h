#pragma once
#include <iostream>

void printArray(int a[], int len)
{
	std::cout << "[printArray]sizeof(a) = " << sizeof(a) << std::endl;
	int* p = a;

	for (int i = 0; i < len; ++i)
	{
		std::cout << "a[" << i << "] = " << p[i] << std::endl;
	}
}

void testArray()
{
	int a[] = { 1, 3, 5, 7 ,9 };
	std::cout << "[testArray]sizeof(a) = " << sizeof(a) << std::endl;

	int* arr_ptr = new int[5];
	std::cout << "[new int[5]]sizeof(arr_ptr) = " << sizeof(arr_ptr) << std::endl;
	delete[] arr_ptr;

	printArray(a, sizeof(a) / sizeof(int));
}