#include <iostream>
#include <string>
using namespace std;

/**
 * 头文件 <string>
 * string to_string (int val);
 * string to_string (long val);
 * string to_string (long long val);
 * string to_string (unsigned val);
 * string to_string (unsigned long val);
 * string to_string (unsigned long long val);
 * string to_string (float val);
 * string to_string (double val);
 * string to_string (long double val);
 **/

// 数值转字符串类型
void numberToString()
{
    long double dd = 3.1415926789;
    string pi = "pi is " + to_string(dd);
    string love = "love is " + to_string(13.14);
    cout << pi << endl;
    cout << love << endl
         << endl;
}

/**
 * 定义于头文件 <string>
 * int                  stoi( const std::string& str, std::size_t* pos = 0, int base = 10 );
 * long                 stol( const std::string& str, std::size_t* pos = 0, int base = 10 );
 * long long            stoll( const std::string& str, std::size_t* pos = 0, int base = 10 );
 * 
 * unsigned long        stoul( const std::string& str, std::size_t* pos = 0, int base = 10 );
 * unsigned long long   stoull( const std::string& str, std::size_t* pos = 0, int base = 10 );
 * 
 * float                stof( const std::string& str, std::size_t* pos = 0 );
 * double               stod( const std::string& str, std::size_t* pos = 0 );
 * long double          stold( const std::string& str, std::size_t* pos = 0 );
 * 
 * str：要转换的字符串
 * pos：传出参数, 记录从哪个字符开始无法继续进行解析。
 * 若base为0，则自动检测数值进制：若前缀为0，则为八进制，若前缀为 0x 或 0X，则为十六进制，否则为十进制。这里的base指的是待转换的字符串按几进制转换到十进制
 **/

// 字符串转数值类型
void stringToNumber()
{
    string str_dec = "2022.02.04, Beijing Winter Olympics";
    string str_hex = "40c3";
    string str_bin = "-10010110001";
    string str_auto = "0x7f";
    // string str_test = "test";

    size_t sz;
    int i_dec = stoi(str_dec, &sz);
    int i_hex = stoi(str_hex, nullptr, 16);
    int i_bin = stoi(str_bin, nullptr, 2);
    int i_auto = stoi(str_auto, nullptr, 0); // 0： 让编译器自动推导
    // int i_test = stoi(str_test);
    // 0x...        -- 十六进制
    // 0...         -- 八进制

    cout << "..... sz = " << sz << endl;
    cout << str_dec << ": " << i_dec << endl;
    cout << str_hex << ": " << i_hex << endl;
    cout << str_bin << ": " << i_bin << endl;
    cout << str_auto << ": " << i_auto << endl;
    // cout << str_test << ": " << i_test << endl;
}

/**
 * 如果字符串中所有字符都是数值类型，整个字符串会被转换为对应的数值，并通过返回值返回
 * 如果字符串的前半部分字符是数值类型，后半部不是，那么前半部分会被转换为对应的数值，并通过返回值返回
 * 如果字符第一个字符不是数值类型转换失败
 **/

int main()
{
    numberToString();
    stringToNumber();
    return 0;
}