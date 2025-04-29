#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "graphics.h"
#include "open.h"

using namespace std;

struct Hole {
    int x, y;
    bool Rat;
};

void displayFinalScore(int score) {
    cout << "Game Over! Your final score is: " << score;
}

const SDL_Color BLACK = { 0, 0, 0, 255 };  

int main(int argc, char* argv[])
{ 
    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
	Graphics graphics;
    graphics.window = window;
    graphics.renderer = renderer;
    
    vector<Hole> holes = { {200, 200, false}, {400, 200, false}, {600, 200, false}, {300, 300, false}, {500, 300, false} };
    int score = 0;
    srand(static_cast<unsigned int>(time(0)));

    int ratIndex = rand() % holes.size();
    holes[ratIndex].Rat = true;

    bool process = true;
    SDL_Event event;
    
    SDL_Texture* background = graphics.loadTexture("ground.jpg");
    graphics.prepareScene(background);

    graphics.presentScene();
    SDL_Texture* image = graphics.loadTexture("rat.png");
	SDL_Texture* final = graphics.loadTexture("Gameover.png");

    while (process) {
        

        for (auto& hole : holes) {
            SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
            SDL_Rect rect = { hole.x, hole.y, 90, 90 };
            SDL_RenderFillRect(renderer, &rect);

            if (hole.Rat) {
                int ratX = hole.x + (100 - 80) / 2;  
                int ratY = hole.y + (100 - 80) / 2;
                graphics.renderTexture(image, ratX, ratY);                
            }
        }

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                displayFinalScore(score);
                graphics.prepareScene(final);
                graphics.presentScene();
                SDL_Delay(3000);
                process = false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);

                for (int i = 0; i < holes.size(); i++) {
                    if (x >= holes[i].x && x <= holes[i].x + 100 &&
                        y >= holes[i].y && y <= holes[i].y + 100) {
                        if (holes[i].Rat) {
                            holes[i].Rat = false;
                            score++;
                            holes[rand() % holes.size()].Rat = true;
                            break;
                        }
                        else {
                            displayFinalScore(score);
                            graphics.prepareScene(final);
                            graphics.presentScene();
                            SDL_Delay(3000);
                            process = false;
                        }
                        break;
                    }
                }
            }
        }
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(image);
    SDL_RenderClear(graphics.renderer);
    SDL_RenderPresent(graphics.renderer);
    SDL_Delay(3000);


    SDL_DestroyRenderer(graphics.renderer);
    SDL_DestroyWindow(graphics.window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}




