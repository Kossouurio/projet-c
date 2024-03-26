#include <stdio.h>
#include <SDL.h>
#include "Demineur.h"

const char* pImgPathDark[] = { DARK_TILE, BASE_DARK, DARK1,DARK2,DARK3,DARK4,DARK5,DARK6,DARK7,DARK8 };
const char* pImgPathLight[] = { LIGHT_TILE, BASE_LIGHT, LIGHT1,LIGHT2,LIGHT3,LIGHT4,LIGHT5,LIGHT6,LIGHT7,LIGHT8 };

void PrintTile(int x, int y, int w, int h,  SDL_Texture* Texture, SDL_Renderer* renderer) {
	SDL_QueryTexture(Texture, NULL, NULL, &w, &h);
	SDL_Rect Rect; Rect.x = x; Rect.y = y; Rect.w = w; Rect.h = h;
	SDL_RenderCopy(renderer, Texture, NULL, &Rect);
}

void PrintGridSDL(int w, int h, SDL_Texture* Texture, SDL_Texture* Texture2, SDL_Texture* Bomb, SDL_Renderer* renderer, Grid* pGrid) {
	for (int i = 0; i < pGrid->size; i++) {
		for (int j = 0; j < pGrid->size; j++) {
			if (pGrid->tile[i][j].IsRevealed == FALSE) {
				if (pGrid->tile[i][j].IsMined == TRUE) {
					PrintTile(i * h, j * w, w, h, Bomb, renderer);
				}
				else if ((i + j) % 2 == 0) {
					PrintTile(i * h, j * w, w, h, Texture, renderer);
				}
				else {
					PrintTile(i * h, j * w, w, h, Texture2, renderer);
				}
			}
			else {
				
			}

		}
		
	}
}


#define WIDTH 800
#define HEIGHT 640


SDL_Texture* Createtexture(const char* path, SDL_Renderer* renderer) {
	SDL_Surface* pSurface = SDL_LoadBMP(path);
	SDL_Texture* Texture = SDL_CreateTextureFromSurface(renderer, pSurface);
	if (Texture == NULL) {
		printf("Erreur dnas la création de la texture");
	}
	SDL_FreeSurface(pSurface);
	return Texture;
}

int main(int argc, char* argv[]) {

	// variable declarations
	SDL_Window* win = NULL;
	SDL_Renderer* renderer = NULL;
	int tile_width = 32;
	int	tile_height = 32; // texture width & height


	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return 1;

	// create the window and renderer
	// note that the renderer is accelerated
	win = SDL_CreateWindow("MineSweeper", 100, 100, WIDTH, HEIGHT, 0);
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	// load our image

	SDL_Texture** DarkTexture = (SDL_Texture**) malloc(sizeof(SDL_Texture*) * 10);
	if (DarkTexture == NULL) {
		exit(1);
	}
	for (int i = 0; i < 10; i++) {
		DarkTexture[i] = Createtexture(pImgPathDark[i], renderer);
	}

	SDL_Texture** LightTexture = (SDL_Texture**)malloc(sizeof(SDL_Texture*) * 10);
	if (LightTexture == NULL) {
		exit(1);
	}
	for (int i = 0; i < 10; i++) {
		LightTexture[i] = Createtexture(pImgPathLight[i], renderer);
	}

	/*SDL_Texture* Dark_Texture = Createtexture(DARK_TILE, renderer);
	SDL_Texture* BaseDark = Createtexture(BASE_DARK, renderer);
	SDL_Texture* Dark1 = Createtexture(DARK1, renderer);
	SDL_Texture* Dark2 = Createtexture(DARK2, renderer);
	SDL_Texture* Dark3 = Createtexture(DARK3, renderer);
	SDL_Texture* Dark4 = Createtexture(DARK4, renderer);
	SDL_Texture* Dark5 = Createtexture(DARK5, renderer);
	SDL_Texture* Dark6 = Createtexture(DARK6, renderer);
	SDL_Texture* Dark7 = Createtexture(DARK7, renderer);
	SDL_Texture* Dark8 = Createtexture(DARK8, renderer);*/



	/*SDL_Texture* Light_Texture = Createtexture(LIGHT_TILE, renderer);
	SDL_Texture* Light1 = Createtexture(LIGHT1, renderer);
	SDL_Texture* Light2 = Createtexture(LIGHT2, renderer);
	SDL_Texture* Light3 = Createtexture(LIGHT3, renderer);
	SDL_Texture* Light4 = Createtexture(LIGHT4, renderer);
	SDL_Texture* Light5 = Createtexture(LIGHT5, renderer);
	SDL_Texture* Light6 = Createtexture(LIGHT6, renderer);
	SDL_Texture* Light7 = Createtexture(LIGHT7, renderer);
	SDL_Texture* Light8 = Createtexture(LIGHT8, renderer);*/


	SDL_Texture* Bomb = Createtexture(BOMB, renderer);

	Grid grid;
	InitGrid(&grid);
	GenerateBomb(&grid,8,8);

	// main loop
	while (1) {
		int MouseX, MouseY;
		


		// event handling
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				break;
			else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
				break;
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				SDL_GetMouseState(&MouseX, &MouseY);
				int Tilei = MouseX % 32;
				int Tilej = MouseY / 32;
				if (e.button.button == SDL_BUTTON_LEFT) {
					Tile* tile = GetTile(&grid, Tilei, Tilej);
				}
			}

		}

		// clear the screen
		SDL_RenderClear(renderer);
		PrintGridSDL(tile_width, tile_height, Dark_Texture, Light_Texture,Bomb, renderer, &grid);

		SDL_RenderPresent(renderer);

	}
	SDL_DestroyTexture(Light_Texture);
	SDL_DestroyTexture(Dark_Texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);

	return 0;
}