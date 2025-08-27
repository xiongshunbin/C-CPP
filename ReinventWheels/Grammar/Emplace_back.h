#pragma once

#include <iostream>
#include <vector>
#include <string>

class MyClass 
{
public:
	// 普通构造函数
	MyClass(int a, const std::string& b) : data(b) {
		std::cout << "Constructing MyClass with a = " << a << " and b = " << b << std::endl;
	}

	// 拷贝构造函数
	MyClass(const MyClass& other) : data(other.data) {
		std::cout << "Copy constructor called!" << std::endl;
	}

	// 移动构造函数
	MyClass(MyClass&& other) noexcept : data(std::move(other.data)) {
		std::cout << "Move constructor called!" << std::endl;
	}
private:
	std::string data;
};

void testEmplace_back() 
{
	std::vector<MyClass> vec;

	MyClass obj(10, "example1");
	vec.push_back(obj);                         // 复制构造
	vec.push_back(MyClass(20, "example2"));     // 临时对象 -> 移动构造

	vec.emplace_back(30, "example3");           // 直接构造对象，省略了拷贝或移动
}
