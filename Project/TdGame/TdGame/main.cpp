#define SDL_MAIN_HANDLED

#include "game_manager.h"

#ifdef _WIN32
#include <windows.h>
#include <ShellScalingApi.h>		// 引用头文件
#pragma comment(lib, "Shcore.lib")	// 链接库文件
#endif

int main(int argc, char** argv)
{
#ifdef _WIN32
	// 设置该进程不受系统 DPI 设置影响
	SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

	// 隐藏控制台窗口
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
#endif

	return GameManager::instance()->run(argc, argv);
}