#pragma once

#include <iostream>
#include <vector>
#include <string>

class MyClass 
{
public:
	// ��ͨ���캯��
	MyClass(int a, const std::string& b) : data(b) {
		std::cout << "Constructing MyClass with a = " << a << " and b = " << b << std::endl;
	}

	// �������캯��
	MyClass(const MyClass& other) : data(other.data) {
		std::cout << "Copy constructor called!" << std::endl;
	}

	// �ƶ����캯��
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
	vec.push_back(obj);                         // ���ƹ���
	vec.push_back(MyClass(20, "example2"));     // ��ʱ���� -> �ƶ�����

	vec.emplace_back(30, "example3");           // ֱ�ӹ������ʡ���˿������ƶ�
}
