#include <iostream>
#include <climits>
using namespace std;

/** 
 * 超长整型
 * 取值范围：占内存大小最少是 8 字节，也可能是16或32字节，跟平台相关。
 * 整数类型：               字节数              取值范围(unsigned)
 *      char:               1                   0 ~ 2^8 - 1
 *      short int:          2                   0 ~ 2^16 - 1
 *      int:                4                   0 ~ 2^32 - 1
 *      long int:     4(32位系统),8(64位系统)     0 ~ 2^32 - 1, 0 ~ 2^64 - 1
 *      long long int:    (最少)8                0 ~ 2^64 - 1
**/

int main()
{
    // 有符号
    long int n = 100L;
    // signed long long 
    // long long int
    // signed long long int
    // 取值范围：-2^63 ~ 2^63 - 1
    long long num1 = 1111LL;    // ll或者LL都可以，但建议使用大写LL

    // 无符号
    // unsigned long long int
    // 取值范围：0 ~ 2^64 - 1
    unsigned long long num2 = 1111ULL;

    cout << "long long max: " << LLONG_MAX                      // long long max: 9223372036854775807
         << ", long long min: " << LLONG_MIN                    // long long min: -9223372036854775808
         <<", unsigned long long max: " << ULLONG_MAX << endl;  // unsigned long long max: 18446744073709551615

    char c = 6;
    long long d = c;    // 隐式的类型转换 等价于： long long d = (long long)c;
    return 0;
}