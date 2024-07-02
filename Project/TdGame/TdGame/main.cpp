#define SDL_MAIN_HANDLED

#include "game_manager.h"

#ifdef _WIN32
#include <windows.h>
#include <ShellScalingApi.h>		// ����ͷ�ļ�
#pragma comment(lib, "Shcore.lib")	// ���ӿ��ļ�
#endif

int main(int argc, char** argv)
{
#ifdef _WIN32
	// ���øý��̲���ϵͳ DPI ����Ӱ��
	SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

	// ���ؿ���̨����
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
#endif

	return GameManager::instance()->run(argc, argv);
}