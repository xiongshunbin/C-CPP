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
		std::cout << "�޷����ļ�" << std::endl;
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
		std::cout << "�޷����ļ�" << std::endl;
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

	// ��ʼ��ÿ����
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	Mix_Init(MIX_INIT_MP3);
	TTF_Init();

	// ������
	/*
	* arguments:
	*	int frequency: ��Ƶ�Ĳ�����(һ��ʹ��44100Hz, CD����, ����)
	*	Uint16 format: Ҫ�������Ƶ��ʽ����ϵͳ���ֽ����йأ�ͨ�õ�MIX_DEFAULT_FORMAT
	*	int channels: ��������2����������1��������
	*	int chunksize: ��Ƶ��������С��һ�㴫��2048
	*/
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	// �������򴰿�
	SDL_Window* window = SDL_CreateWindow(u8"��ã�����", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);

	// ��Ⱦ��, int index: ��Ҫ��ʼ��������������ͨ��Ϊ-1
	/*
	* SDL_RENDERER_ACCELERATED: Ӳ������
	* SDL_RENDERER_PRESENTVSYNC: ��ֱͬ����Ϊ��������Ļ˺�ѵ����
	* SDL_RENDERER_TARGETTEXTURE: �л���Ⱦ��Ŀ��
	*/
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// 1. ��Ӳ���е�ͼƬ���ص��ڴ���
	SDL_Surface* suf_img = IMG_Load("avatar.jpg");
	// 2. ���ڴ�ͼƬ�����ϴ���GPU�������������
	SDL_Texture* tex_img = SDL_CreateTextureFromSurface(renderer, suf_img);

	// ����TTF_Font�����������
	TTF_Font* font = TTF_OpenFont("ipix.ttf", 32);
	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Surface* suf_text = TTF_RenderUTF8_Blended(font, u8"��ã�����", color);
	SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);

	// ��������
	Mix_Music* music = Mix_LoadMUS("music.mp3");
	Mix_FadeInMusic(music, -1, 1500);

	// ��̬��ʱ��֡��Ϊ60
	int fps = 60;
	// ��Ϸ�Ƿ��˳��ı��λ
	bool is_quit = false;

	// ʱ��ṹ����������������¼�
	SDL_Event event;
	// ��¼����λ��
	SDL_Point pos_cursor = { 0, 0 };
	// ��ʾͼƬ������
	SDL_Rect rect_img, rect_text;
	// ��ȡ�����ܼ���������
	Uint64 last_counter = SDL_GetPerformanceCounter();
	Uint64 counter_freq = SDL_GetPerformanceFrequency();


	rect_img.w = suf_img->w;
	rect_img.h = suf_img->h;

	rect_text.w = suf_text->w;
	rect_text.h = suf_text->h;

	// ��Ϸ��ѭ��
	while (!is_quit)
	{
		// 1. �����¼�����Ϣ
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

		// 2. ��������
		rect_img.x = pos_cursor.x;
		rect_img.y = pos_cursor.y;

		rect_text.x = pos_cursor.x;
		rect_text.y = pos_cursor.y;

		// ��ÿ�λ�ͼ֮ǰ��Ҫ����һ֡�����ݽ������
		// ���û�ͼ��ɫ
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// 3. ��Ⱦ��ͼ
		SDL_RenderCopy(renderer, tex_img, nullptr, &rect_img);
		filledCircleRGBA(renderer, pos_cursor.x, pos_cursor.y, 50, 255, 0, 0, 125);
		SDL_RenderCopy(renderer, tex_text, nullptr, &rect_text);

		// ����Ⱦ�����Ƶ����ݸ��µ������ϣ�����������չʾ�����ڻ�������
		SDL_RenderPresent(renderer);
	}

	return 0;
}