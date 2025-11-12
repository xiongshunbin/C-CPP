#include <iostream>
/**
 * std::cout << "The sum of " << v1;
 *           << " and " << v2;
 *           << " is " << v1 + v2 << std::endl;
 * 这段程序不合法。后两条语句打印输出缺少输出流。
 */

int main()
{
    std::cout << "Enter two numbers:" << std::endl;
    int v1 = 0, v2 = 0;
    std::cin >> v1 >> v2;
    std::cout << "The sum of " << v1;
    std::cout << " and " << v2;
    std::cout << " is " << v1 + v2 << std::endl;
    return 0;
}