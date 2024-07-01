// 简单工厂模式

#include <iostream>

using namespace std;

enum class Type :char {Chair, Desk, Bed};

class Factory
{
public:
    void generate(Type type);
};

int main()
{
    
    return 0;
}

void Factory::generate(Type type)
{
    switch (type)
    {
    case Type::Chair:

        break;
    case Type::Desk:

        break;
    case Type::Bed:

        break;
    default:
        break;
    }
}
