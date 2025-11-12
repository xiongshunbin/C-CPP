#include <iostream>

int main()
{
    std::cout << "/*";                  // valid
    std::cout << "*/";                  // valid
    // std::cout << /* "*/" */;         // invalid
    std::cout << /* "*/" */";           // invalid
    std::cout << /* "*/" /* "/*" */;    // valid
    std::cout << std::endl;
    return 0;
}