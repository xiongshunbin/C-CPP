#define SDL_MAIN_HANDLED

#include "game_manager.h"

#include <ShellScalingApi.h>		// 引用头文件
#pragma comment(lib, "Shcore.lib")	// 链接库文件

int main(int argc, char** argv)
{
	// 设置该进程不受系统 DPI 设置影响
	SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

	return GameManager::instance()->run(argc, argv);
}