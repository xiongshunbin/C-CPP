#include <iostream>
using namespace std;

/**
 * 1.原始字面量
 * 字面量：表示固定的值，分为：数字类型、字符串类型
 * 作用：修饰字符串使字符串按照原意表达，不进行转义
 * 语法：R"xxx(原始字符串)xxx"，其中xxx只是起到描述的作用，相当于备注，在实际解析的过程中会被忽略掉，如果在使用过程中加上了xxx，那么前后的xxx必须相同，可不写
 * 使用：
 *      a. 字符串含有特殊字符（如\）
 *      b. 长字符串多行展示
 * 如果字符串里面都是普通字符，它本身就表示最原始的含义，就不需要使用原始字面量
**/ 

int main(int argc, char* argv[])
{
#if 0
    /**
     * 转义字符('\')的使用方式:
     *      a.把普通字符转换为有特殊含义的字符，eg: t -> \t(制表符，4个空格)、\r(回车)、\n(换行)
     *      b.把有特殊含义的字符转换为普通字符，eg: \\ -> '\'
     * 转义字符在转义的时候，如果转义失败，会按原样输出对应的字符
     * 在windows系统中常用\来描述路径，就需要使用转义字符，而在Linux系统中使用/描述路径，就不需要转义
     **/
    string str = "D:\hello\world\test.text";        // D:helloworld    est.text
    cout << str << endl;
    string str1 = "D:\\hello\\world\\test.text";    // D:\hello\world\test.text
    cout << str1 << endl;

    /**
     * 对于比较长的字符串，在编码过程中，一行如果盛不下，都写到一行中，代码可读性较差，需要写到多行中，需要使用连接符\，把每一行的子字符串进行关联
     **/
    string str2 = "<html>\
            <head>\
            <title>\
            海贼王\
            </title>\
            </head>\
            <body>\
            <p>\
            我是要成为海贼王的男人!!!\
            </p>\
            </body>\
            </html>";
    cout << str2 << endl;
#else
    // string str1 = R"hello(D:\hello\world\test.text)world";   // not allow
    string str1 = R"hello(D:\hello\world\test.text)hello";  // D:\hello\world\test.text)hello
    cout << str1 << endl;

    string str2 = R"(<html>
            <head>
            <title>
            海贼王
            </title>
            </head>
            <body>
            <p>
            我是要成为海贼王的男人!!!
            </p>
            </body>
            </html>)";
    cout << str2 << endl;
#endif
    return 0;
}