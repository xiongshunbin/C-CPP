#include <iostream>
#include <thread>
#include <mutex>
#include <future>
#include <list>
#include "thread_pool.h"

// C++ 版本的快速排序算法
template<typename T>
void quick_sort_recursive(T arr[], int start, int end)
{
	if (start >= end)
		return;

	T key = arr[start + end >> 1];
	int left = start - 1, right = end + 1;

	while (left < right)
	{
		do left++;	while (arr[left] < key);
		do right--;	while (arr[right] > key);
		if (left < right)
			std::swap(arr[left], arr[right]);
	}

	quick_sort_recursive(arr, start, right);
	quick_sort_recursive(arr, right + 1, end);
}

template<typename T>
void quick_sort(T arr[], int length)
{
	quick_sort_recursive(arr, 0, length - 1);
}

void test_quick_sort()
{
	int num_array[] = { 6, 1, 0, 7, 5, 2, 9, -1 };
	int length = sizeof(num_array) / sizeof(int);
	quick_sort(num_array, length);
	std::cout << "Sorted result:";
	for (int i = 0; i < length; i++)
	{
		std::cout << " " << num_array[i];
	}
	std::cout << std::endl;
}

template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input)
{
	if (input.empty())
	{
		return input;
	}

	std::list<T> result;
	result.splice(result.begin(), input, input.begin());

	const T& pivot = *(result.begin());
	auto divide_point = std::partition(input.begin(), input.end(), [&](const T& t) {
		return t < pivot;
	});

	std::list<T> lower_part;
	lower_part.splice(lower_part.end(), input, input.begin(), divide_point);

	auto new_lower(sequential_quick_sort(std::move(lower_part)));
	auto new_higher(sequential_quick_sort(std::move(input)));

	result.splice(result.end(), new_higher);
	result.splice(result.begin(), new_lower);
	return result;
}

void test_sequential_quick()
{
	std::list<int> numlists = { 6, 1, 0, 7, 5, 2, 9, -1 };
	auto sort_result = sequential_quick_sort(numlists);
	std::cout << "Sorted result:";
	for (auto iter = sort_result.begin(); iter != sort_result.end(); iter++)
	{
		std::cout << " " << (*iter);
	}
	std::cout << std::endl;
}

#if 1

int main()
{
	// test_quick_sort();

	test_sequential_quick();

	return 0;
}

#endif
