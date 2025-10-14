#include <iostream>
#include "./utility/Logger.h"

using namespace utility;

int main()
{
    Logger::getInstance()->openFile("./test.log");
    INFO("Hi! My name is %s.", "mushan");
    return 0;
}