#include <iostream>
#include "./utility/Logger.h"

using namespace utility;

int main()
{
    Logger::getInstance()->openFile("./test.log");
    Logger::getInstance()->setLevel(Logger::ERROR);
    Logger::getInstance()->setMaxSize(200);
    debug("我是一条测试信息");
    info("Hi! My name is %s.", "mushan");
    error("注意哦!我是一条错误信息");
    return 0;
}