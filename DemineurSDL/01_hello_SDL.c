#include <stdio.h>
#include <SDL.h>
#include "Demineur.h"

const char* pImgPathDark[] = { BASE_DARK, DARK1,DARK2,DARK3,DARK4,DARK5,DARK6,DARK7,DARK8,  DARK_TILE, DARK_FLAG };
const char* pImgPathLight[] = {  BASE_LIGHT, LIGHT1,LIGHT2,LIGHT3,LIGHT4,LIGHT5,LIGHT6,LIGHT7,LIGHT8, LIGHT_TILE, LIGHT_FLAG };

void PrintTile(int x, int y, int w, int h,  SDL_Texture* Texture, SDL_Renderer* renderer) {
	SDL_QueryTexture(Texture, NULL, NULL, &w, &h);
	SDL_Rect Rect; Rect.x = x; Rect.y = y; Rect.w = w; Rect.h = h;
	SDL_RenderCopy(renderer, Texture, NULL, &Rect);
}

int EasterEgg() {

	// variable declarations
	SDL_Window* win2 = NULL;
	SDL_Renderer* renderer = NULL;
	int tile_width = 32;
	int	tile_height = 32; // texture width & height

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return 1;

	win2 = SDL_CreateWindow("EasterEgg de fou", 100, 100, 720, 720, 0);
	renderer = SDL_CreateRenderer(win2, -1, SDL_RENDERER_ACCELERATED);
	SDL_Texture* Win_Texture = Createtexture(EASTER_EGG, renderer);
	SDL_RenderClear(renderer);
	SDL_Rect dstrect = { 0, 0, WIDTH, HEIGHT };
	SDL_RenderCopy(renderer, Win_Texture, NULL, &dstrect);
	SDL_RenderPresent(renderer);
	BOOL2 running = TRUE;
	SDL_Event e;
	while (running == TRUE) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				SDL_DestroyWindow(win2);
				running = FALSE;
			}
		}
	}

	return 0;
}


void PrintGridSDL(int w, int h, SDL_Texture** LightTexture, SDL_Texture** Darktexture, SDL_Texture* Bomb, SDL_Renderer* renderer, Grid* pGrid, BOOL2 ShowBomb) {
	for (int i = 0; i < pGrid->size; i++) {
		for (int j = 0; j < pGrid->size; j++) {
			Tile* tile = GetTile(pGrid, i, j);
			if (tile->IsRevealed == FALSE) {
				if (tile->IsMined == TRUE && ShowBomb == TRUE) {
					tile->Texture = Bomb;
					PrintTile(i * h, j * w, w, h, tile->Texture, renderer);
				}
				else if (tile->IsFlag == TRUE) {
					if ((i + j) % 2 == 0) //light tile
					{
						tile->Texture = LightTexture[10];
					}
					else {
						tile->Texture = Darktexture[10];
					}
				}
				else {
					if ((i + j) % 2 == 0)
					{
						tile->Texture = LightTexture[9];
					}
					else {
						tile->Texture = Darktexture[9];
					}
				}

				PrintTile(i * h, j * w, w, h, tile->Texture, renderer);

			}
			else {
				if ((i + j) % 2 == 0) //light tile
				{
					tile->Texture = LightTexture[tile->AdjacentMines];
				}
				else {
					tile->Texture = Darktexture[tile->AdjacentMines];
				}
				PrintTile(i * h, j * w, w, h, tile->Texture, renderer);
			}

		}

	}
	
}



SDL_Texture* Createtexture(const char* path, SDL_Renderer* renderer) {
	SDL_Surface* pSurface = SDL_LoadBMP(path);
	SDL_Texture* Texture = SDL_CreateTextureFromSurface(renderer, pSurface);
	if (Texture == NULL) {
		printf("Erreur dnas la création de la texture");
	}
	SDL_FreeSurface(pSurface);
	return Texture;
}

void DestroyTexture(SDL_Texture** TextureTab) {
	for (int i = 0; i < 11; i++) {
		SDL_DestroyTexture(TextureTab[i]);
	}
	free(TextureTab);
}

Grid Restart(Grid* pGrid, SDL_Texture** LightTab, SDL_Texture** DarkTab) {
	Grid NewGrid;
	NewGrid.size = pGrid->size;
	NewGrid.difficulty = pGrid->difficulty;
	for (int i = 0; i < pGrid->size; i++)
	{
		for (int j = 0; j < pGrid->size; j++)
		{
			Tile* tile = GetTile(pGrid, i, j);

			tile->x = i;
			tile->y = j;
			tile->IsRevealed = FALSE;
			tile->Value = i * pGrid->size + j;
			tile->IsMined = FALSE;
			tile->IsFlag = FALSE;
			tile->AdjacentMines = 0;
			if ((i + j) % 2 == 0)
			{
				tile->Texture = LightTab[9];
			}
			else {
				tile->Texture = DarkTab[9];
			}
		}
	}
}

int main(int argc, char* argv[]) {

	// variable declarations
	SDL_Window* win = NULL;
	SDL_Renderer* renderer = NULL;
	int tile_width = 32;
	int	tile_height = 32; // texture width & height

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return 1;

	win = SDL_CreateWindow("MineSweeper", 100, 100, WIDTH, HEIGHT, 0);
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	SDL_Texture** DarkTexture = (SDL_Texture**)malloc(sizeof(SDL_Texture*) * 11);
	if (DarkTexture == NULL) {
		exit(1);
	}
	for (int i = 0; i < 11; i++) {
		DarkTexture[i] = Createtexture(pImgPathDark[i], renderer);
	}

	SDL_Texture** LightTexture = (SDL_Texture**)malloc(sizeof(SDL_Texture*) * 11);
	if (LightTexture == NULL) {
		exit(1);
	}
	for (int i = 0; i < 11; i++) {
		LightTexture[i] = Createtexture(pImgPathLight[i], renderer);
	}

	int tilesDebug[][7] =
	{
		{0,0,0,1,1,1,0},
		{0,0,0,1,0,1,0},
		{0,0,0,1,1,1,0},
		{0,0,0,1,0,0,0},
		{0,0,0,1,0,0,0},
		{0,0,0,1,0,0,0},
		{0,0,0,1,0,0,0},
	};


	SDL_Texture* Bomb = Createtexture(BOMB, renderer);
	Grid grid;
	InitGrid(&grid, LightTexture, DarkTexture);
	//Init8Grid(&grid, LightTexture, DarkTexture);
	//InitDebugGrid(&grid, LightTexture, DarkTexture, tilesDebug);

	// main loop
	BOOL2 FirstInput = TRUE;
	int end = 0;
	while (end == 0) {
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
				int Tilei = MouseX / tile_height;
				int Tilej = MouseY / tile_width;
				if (InGrid(Tilei, Tilej, &grid) == FALSE)
					continue;
				Tile* tile = GetTile(&grid, Tilei, Tilej);
				if (e.button.button == SDL_BUTTON_LEFT) {
					if (FirstInput == TRUE) {
						int bomb_amount = GenerateBomb(&grid, Tilei, Tilej);
						grid.remainingtiles = pow(grid.size, 2) - bomb_amount;
						//GenerateBombDebug(&grid, tilesDebug);
						FirstInput = FALSE;
					}
					if (tile->IsMined == TRUE) {
						end = -1;
					}
					else {
						UpdateGrid(&grid, Tilei, Tilej);
						if (CheckWin(&grid) == TRUE) {
							end = 1;
						}
					}


					
				}
				else if (e.button.button == SDL_BUTTON_RIGHT) {
					if (tile->IsFlag == TRUE) {
						tile->IsFlag = FALSE;
					}
					else {
						tile->IsFlag = TRUE;
					}
				}
			}

		}

	while (end == -1) {
		SDL_Texture* Death_Texture = Createtexture(DEATH_SCREEN, renderer);
		SDL_RenderClear(renderer);
		SDL_Rect dstrect = { 0, 0, WIDTH, HEIGHT };
		SDL_RenderCopy(renderer, Death_Texture, NULL, &dstrect);
		SDL_RenderPresent(renderer);
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				break;
			else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
				break;
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				Restart(&grid, LightTexture, DarkTexture);
				FirstInput = TRUE;
				end = 0;
			}
		}
	}

	while (end == 1) {
		SDL_Texture* Win_Texture = Createtexture(VICTORY_SCREEN, renderer);
		SDL_RenderClear(renderer);
		SDL_Rect dstrect = { 0, 0, WIDTH, HEIGHT };	
		SDL_RenderCopy(renderer, Win_Texture, NULL, &dstrect);
		SDL_RenderPresent(renderer);
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				break;
			else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
				break;
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				Restart(&grid, LightTexture, DarkTexture);
				FirstInput = TRUE;
				end = 0;
			}
		}
	}

		// clear the screen
		SDL_RenderClear(renderer);

		PrintGridSDL(tile_width, tile_height, LightTexture, DarkTexture, Bomb, renderer, &grid,FALSE);

		SDL_RenderPresent(renderer);

	}

	DestroyTexture(LightTexture);
	DestroyTexture(DarkTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);

	return 0;
}