#include <iostream>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "TcpServer.h"

int main(int argc, char* argv[])
{
	// spdlog日志库的使用
	try
	{
		auto logger_console = spdlog::stdout_color_mt("logger_console");
		auto logger_file = spdlog::basic_logger_mt("basic_logger", "logs/basic-log.txt");

		logger_console->set_level(spdlog::level::debug);
		logger_file->set_level(spdlog::level::info);
		spdlog::flush_every(std::chrono::seconds(5)); // 定期刷新日志缓冲区
	}
	catch (const spdlog::spdlog_ex& ex)
	{
		std::cout << "Log init failed: " << ex.what() << std::endl;
	}

#if 0
	if (argc != 3)
	{
		printf("Usage : %s <port> <path>\n", argv[0]);
		return -1;
	}

	unsigned short port = atoi(argv[1]);
	// 切换服务器的工作目录
	chdir(argv[2]);
#else
	unsigned short port = 10000;
	chdir("/home/wsl-ubuntu/Test/luffy/");
	//chdir("/home/mushan/Test");
#endif

	// 启动服务器
	TcpServer* server = new TcpServer(port, 4);
	server->run();

	return 0;
}