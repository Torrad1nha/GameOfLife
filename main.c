#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>

#define CELL_SIZE 4
#define GRID_WIDTH 128
#define GRID_HEIGHT 128

typedef int *grid;

grid genGrid(){
    srand(time(NULL));

    const int GRID_SIZE = GRID_WIDTH * GRID_HEIGHT;

    grid final = malloc(sizeof(int) * GRID_SIZE);
    for(int i = 0; i < GRID_SIZE; i++){
        int value = rand() % 2;
        if(value > 0)
            final[i] = 0;
        else
            final[i] = 1;
    }
    return final;
}

int checkNeighbours(grid g, int xi, int yi){
    int final = 0;
    for(int y = -1; y < 2; y++){
        for(int x = -1; x < 2; x++){

            if(g[(yi + y) * GRID_HEIGHT + (xi + x)] == 1 && x != 0 && y != 0)
                final++;
        }
    }
    return final;
}

void updateGrid(grid g){
    for(int y = 1; y < GRID_HEIGHT - 1; y++){
        for(int x = 1; x < GRID_WIDTH - 1; x++){
            int neighbour_count = checkNeighbours(g, x, y);
            int index = y * GRID_HEIGHT + x;

            if(g[index] == 1 && (neighbour_count >= 4 || neighbour_count <= 1))
                g[index] = 0;

            else if(g[index] == 0 && neighbour_count == 3)
                g[index] = 1;

            else
                g[index] = g[index];
            
        }
    }
}

void drawGrid(grid g, SDL_Renderer *renderer){
    for(int y = 1; y < GRID_HEIGHT - 1; y++){
        for(int x = 1; x < GRID_WIDTH - 1; x++){

            const SDL_Rect tmp = {.w = CELL_SIZE - 1, .h = CELL_SIZE - 1,
                                 .x = x * CELL_SIZE, .y = y * CELL_SIZE};

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            if(g[y * GRID_HEIGHT + x] == 1)
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            
            SDL_RenderFillRect(renderer, &tmp);
        }
    }
}

int main(int argc, char const *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("", 
                         SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         800, 600, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    grid test = genGrid();
    int running = 1;

    while(running){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT)
                running = 0;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        drawGrid(test, renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(1000);

        updateGrid(test);
    }

    SDL_Quit();
    return 0;
}
