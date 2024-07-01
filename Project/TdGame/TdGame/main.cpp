#define SDL_MAIN_HANDLED

#include "game_manager.h"

#include <ShellScalingApi.h>		// ����ͷ�ļ�
#pragma comment(lib, "Shcore.lib")	// ���ӿ��ļ�

int main(int argc, char** argv)
{
	// ���øý��̲���ϵͳ DPI ����Ӱ��
	SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

	return GameManager::instance()->run(argc, argv);
}