#include <iostream>
#include "endpoint.h"

/**
 * proactor 和 reactor 模式
 *	1) reactor 模式是同步模型, proactor 模式是异步模型
 * 
 * 网络编程基本流程:
 *	服务端
 *		1) socket - 创建用于监听的 socket 套接字
 *		2) bind	- 绑定本机IP + PORT
 *		3) listen - 设置监听, 等待连接请求状态
 *		4) accept - 建立连接, 创建用于通信的 socket 对象用于跟客户端进行通信
 *		5) read / write - 数据交换
 *		6) close - 断开连接
 * 
 *	客户端
 *		1) socket - 创建用于通信的 socket 套接字
 *		2) connect - 根据服务器的 IP + PORT 发起连接请求
 *		3) read / write - 数据交换
 *		4) close - 断开连接
 */

int main()
{
	client_end_point();

	return 0;
}