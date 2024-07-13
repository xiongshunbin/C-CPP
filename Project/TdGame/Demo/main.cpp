#define SDL_MAIN_HANDLED

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <cJSON.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>

void test_json()
{
	std::fstream file("test.json");
	if (!file.good())
	{
		std::cout << "无法打开文件" << std::endl;
		return;
	}
	std::stringstream str_stream;
	str_stream << file.rdbuf();
	file.close();

	cJSON* json_root = cJSON_Parse(str_stream.str().c_str());
	cJSON* json_name = cJSON_GetObjectItem(json_root, "name");
	cJSON* json_age = cJSON_GetObjectItem(json_root, "age");
	cJSON* json_pets = cJSON_GetObjectItem(json_root, "pets");

	std::cout << json_name->string << ": " << json_name->valuestring << std::endl;
	std::cout << json_age->string << ": " << json_age->valueint << std::endl;

	std::cout << json_pets->string << ":" << std::endl;
	cJSON* json_item = nullptr;
	cJSON_ArrayForEach(json_item, json_pets)
	{
		std::cout << "\t" << json_item->valuestring << std::endl;
	}
}

void test_csv()
{
	std::fstream file("test.csv");
	if (!file.good())
	{
		std::cout << "无法打开文件" << std::endl;
		return;
	}

	std::string str_line;
	while (std::getline(file, str_line))
	{
		std::string str_grid;
		std::stringstream str_stream(str_line);
		while (std::getline(str_stream, str_grid, ','))
		{
			std::cout << str_grid << " ";
		}

		std::cout << std::endl;
	}

	file.close();
}

int main()
{
	test_json();
	std::cout << "=====================" << std::endl;
	test_csv();

	// 初始化每个库
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	Mix_Init(MIX_INIT_MP3);
	TTF_Init();

	// 打开声道
	/*
	* arguments:
	*	int frequency: 音频的采样率(一般使用44100Hz, CD质量, 常用)
	*	Uint16 format: 要解码的音频格式，跟系统的字节序有关，通用的MIX_DEFAULT_FORMAT
	*	int channels: 声道数，2：立体声，1：单声道
	*	int chunksize: 音频缓冲区大小，一般传入2048
	*/
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	// 创建程序窗口
	SDL_Window* window = SDL_CreateWindow(u8"你好，世界", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);

	// 渲染器, int index: 需要初始化的驱动索引，通常为-1
	/*
	* SDL_RENDERER_ACCELERATED: 硬件加速
	* SDL_RENDERER_PRESENTVSYNC: 垂直同步，为了消除屏幕撕裂的情况
	* SDL_RENDERER_TARGETTEXTURE: 切换渲染的目标
	*/
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// 1. 把硬盘中的图片加载到内存中
	SDL_Surface* suf_img = IMG_Load("avatar.jpg");
	// 2. 把内存图片数据上传到GPU，变成纹理数据
	SDL_Texture* tex_img = SDL_CreateTextureFromSurface(renderer, suf_img);

	// 创建TTF_Font加载字体对象
	TTF_Font* font = TTF_OpenFont("ipix.ttf", 32);
	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Surface* suf_text = TTF_RenderUTF8_Blended(font, u8"你好，世界", color);
	SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);

	// 加载音乐
	Mix_Music* music = Mix_LoadMUS("music.mp3");
	Mix_FadeInMusic(music, -1, 1500);

	// 动态延时，帧率为60
	int fps = 60;
	// 游戏是否退出的标记位
	bool is_quit = false;

	// 时间结构体对象，用来处理窗口事件
	SDL_Event event;
	// 记录鼠标的位置
	SDL_Point pos_cursor = { 0, 0 };
	// 显示图片的区域
	SDL_Rect rect_img, rect_text;
	// 获取高性能计数器计数
	Uint64 last_counter = SDL_GetPerformanceCounter();
	Uint64 counter_freq = SDL_GetPerformanceFrequency();


	rect_img.w = suf_img->w;
	rect_img.h = suf_img->h;

	rect_text.w = suf_text->w;
	rect_text.h = suf_text->h;

	// 游戏主循环
	while (!is_quit)
	{
		// 1. 处理事件和消息
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				is_quit = true;
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				pos_cursor.x = event.motion.x;
				pos_cursor.y = event.motion.y;
			}
		}

		Uint64 current_counter = SDL_GetPerformanceCounter();
		double delta = (double)(current_counter - last_counter) / counter_freq;
		last_counter = current_counter;

		if (delta * 1000 < 1000.0 / 60)
			SDL_Delay((Uint32)(1000.0 / 60 - delta * 1000));

		// 2. 处理数据
		rect_img.x = pos_cursor.x;
		rect_img.y = pos_cursor.y;

		rect_text.x = pos_cursor.x;
		rect_text.y = pos_cursor.y;

		// 在每次绘图之前，要对上一帧的内容进行清空
		// 设置绘图颜色
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// 3. 渲染绘图
		SDL_RenderCopy(renderer, tex_img, nullptr, &rect_img);
		filledCircleRGBA(renderer, pos_cursor.x, pos_cursor.y, 50, 255, 0, 0, 125);
		SDL_RenderCopy(renderer, tex_text, nullptr, &rect_text);

		// 让渲染器绘制的内容更新到窗口上，把像素数据展示到窗口缓冲区上
		SDL_RenderPresent(renderer);
	}

	return 0;
}