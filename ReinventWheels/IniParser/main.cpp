#include <iostream>

/**
 * 1.解析的实际场景
 *		- 工具/桌面软件的简单配置需求
 *
 * 2.示例ini文件
 *		; this is a comment
 *		# also a comment
 *		topkey = topvalue
 *
 *		[server]
 *		host = example.com
 *		port = 8080
 *		path = "/api/v1/resource" ; inline comment
 *		escaped = "line\nnew"	# another inline comment
 * 
 *		[database]
 *		user = dbuser
 *		password = "p@ss;word" ; note semicolon inside quoted value
 *		timeout = 30
 * 
 * 3.ini语法要点
 *		- 支持行首注释 ; 或 #
 *		- 支持行尾 inline comment	(但仅在引号外部)
 *		- 空行在解析时应忽略
 *		- section 以 [name] 定义, 默认 section 使用空字符串 ""
 *		- key-value 形式 key = value (两侧可有空白)
 *		- a = b = c 以第一个 = 划分, 右侧可作为整个value
 *		- 未加引号, 从 = 到行尾(或注释符前), trim 后为value
 *		- 双引号包裹, 保留内部字符, 不受 inline 注释影响
 *		- 支持转义, 例如: \n, \", \\
 */

int main()
{
	
	return 0;
}
