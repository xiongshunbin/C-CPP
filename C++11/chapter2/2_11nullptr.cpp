#include <iostream>
using namespace std;

/**
 * nullptr无法隐式转换为整型，但是可以隐式匹配指针类型.在C++11中，相比NULL和0，使用nullptr初始化空指针可以令我们编写的程序更加健壮，在对指针初始化时，建议使用nullptr。
**/

void func(char *p)
{
    cout << "void func(char* p)" << endl;
}

void func(int p)
{
    cout << "void func(int p)" << endl;
}

int main(int argc, char *argv[])
{
    /**
     * 在C程序中，NULL表示的是(void *)0，而在C++中，NULL表示的是0
     * C/C++是强类型语言，从C到C++之后，语言对类型的要求更加的严格，C++不允许将一个void *类型的变量隐式转换为其他类型的指针的。
     **/
    int *ptr1 = nullptr;
    char *ptr2 = nullptr;
    double *ptr3 = nullptr;
    void *ptr4 = nullptr;

    // int *ptr5 = ptr4;    // error, a value of type "void *" cannot be used to initialize an entity of type "int *"
    int *ptr5 = 0;
    int *ptr6 = (int *)ptr4;

    // int
    func(10);

    // char *
    // func(NULL);      // error call of overloaded ‘func(NULL)’ is ambiguous
    func(nullptr);

    return 0;
}