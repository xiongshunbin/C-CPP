#define SDL_MAIN_HANDLED

#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL2_rotozoom.h>

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	SDL_Window* window = SDL_CreateWindow(u8"你好，世界", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	bool is_quit = false;

	SDL_Event event;

	Uint64 last_counter = SDL_GetPerformanceCounter();
	Uint64 counter_freq = SDL_GetPerformanceFrequency();

	
	// 图片等比缩放
	SDL_Texture* tex_mushan = IMG_LoadTexture(renderer, "mushan.png");
	SDL_SetTextureBlendMode(tex_mushan, SDL_BLENDMODE_BLEND);

	SDL_Surface* suf_qr = IMG_Load("bilibili_qr.png");
	SDL_Surface* zoomed_suf = rotozoomSurface(suf_qr, 0.0, 2, SMOOTHING_ON);
	SDL_Texture* tex_qr = SDL_CreateTextureFromSurface(renderer, zoomed_suf);
	SDL_SetTextureBlendMode(tex_qr, SDL_BLENDMODE_BLEND);

	SDL_FreeSurface(suf_qr);
	SDL_FreeSurface(zoomed_suf);

	while (!is_quit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				is_quit = true;
		}

		Uint64 current_counter = SDL_GetPerformanceCounter();
		double delta = (double)(current_counter - last_counter) / counter_freq;
		last_counter = current_counter;

		if (delta * 1000 < 1000.0 / 60)
			SDL_Delay((Uint32)(1000.0 / 60 - delta * 1000));

		static Uint8 alpha = 0;
		if (alpha < 255)
			alpha += 1;
		SDL_SetTextureAlphaMod(tex_mushan, alpha);
		SDL_SetTextureAlphaMod(tex_qr, alpha);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_Rect rect_dst;
		int width_img, height_img;
		SDL_QueryTexture(tex_mushan, nullptr, nullptr, &width_img, &height_img);

		int width_screen, height_screen;
		SDL_GetWindowSizeInPixels(window, &width_screen, &height_screen);

		rect_dst =
		{
			(int)(width_screen / 3 - width_img / 2),
			(int)(height_screen - height_img) / 2,
			width_img, height_img
		};

		SDL_RenderCopy(renderer, tex_mushan, nullptr, &rect_dst);


		const int width_bar = 5, height_bar = 300;

		roundedRectangleRGBA(renderer, (width_screen - width_bar) / 2, (height_screen - height_bar) / 2,
			(width_screen + width_bar) / 2, (height_screen + height_bar) / 2, 2, 255, 255, 255, alpha);
		roundedBoxRGBA(renderer, (width_screen - width_bar) / 2, (height_screen - height_bar) / 2,
			(width_screen + width_bar) / 2, (height_screen + height_bar) / 2, 2, 255, 255, 255, alpha);


		SDL_QueryTexture(tex_qr, nullptr, nullptr, &width_img, &height_img);
		rect_dst =
		{
			(int)(width_screen * 2 / 3 - width_img / 2),
			(int)(height_screen - height_img) / 2,
			width_img, height_img
		};
		SDL_RenderCopy(renderer, tex_qr, nullptr, &rect_dst);

		SDL_RenderPresent(renderer);
	}


	return 0;
}