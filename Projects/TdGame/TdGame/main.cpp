#undef main

#include "game_manager.h"

#ifdef _WIN32
#include <ShellScalingApi.h>		// ����ͷ�ļ�
#pragma comment(lib, "Shcore.lib")	// ���ӿ��ļ�
#endif


int main(int argc, char** argv)
{
#ifdef _WIN32
	// ���øý��̲���ϵͳ DPI ����Ӱ��
	SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
#endif

	return GameManager::instance()->run(argc, argv);
}