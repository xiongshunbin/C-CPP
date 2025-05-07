#pragma once
#include <iostream>

class A
{
public:
	virtual void foo() { }
};

class B : public A
{
public:
	virtual void foo() { }
	void bar() { }
};

void testVirtual()
{
	std::cout << "sizeof(A): " << sizeof(A) << std::endl;
	std::cout << "Sizeof(B): " << sizeof(B) << std::endl;
}