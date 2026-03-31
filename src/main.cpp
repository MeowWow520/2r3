#include <iostream>
#include <vector>
#include <SDL3/SDL.h>


struct RGB {
    Uint8 r;
    Uint8 g;
    Uint8 b;
};
struct vec2 {
    int x;
    int y;
};

bool IsGoing_ = true;
const char* Title = "2r3";
int width = 1960;
int height =  1080;
SDL_Window* window_ = nullptr;
SDL_Renderer* renderer_ = nullptr;

const RGB backgrd = {2, 62, 138};
const RGB obj = {255, 0, 110};
const RGB line = {254, 228, 64};

std::vector<vec2> vec2list;
const SDL_FRect frect = {100.00, 0.00, 100.00, 100.00};


bool Initlib(bool flags);
void HendleEvents();
void Update();
void Render();
int transX(int x);
int transY(int y);



int main(int argc, char* argv) 
{
    (void)argc, (void)argv;
    SDL_Log("Hello 2r3");

    // init SDL library
    if (Initlib(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO))) {
        SDL_Log("init SDL library successfully");
    };

    // create window and renderer
    window_ = SDL_CreateWindow(Title, 
                              width, 
                              height, 
                              SDL_WINDOW_RESIZABLE);
    renderer_ = SDL_CreateRenderer(window_, NULL);

    // main loop
    while(IsGoing_) {
        HendleEvents();
        Update();
        Render();
    }

    // destroy window and renderer
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}



bool Initlib(bool flags) {
    if (flags == true) return true;
    SDL_Log(SDL_GetError());
    return false;
};

void HendleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case(SDL_EVENT_QUIT): {
                IsGoing_ = false;
                SDL_Log("Recieving SDL_EVENT_QUIT, main loop quit");
                break;
            }
            default: break; 
        }
    }
};

void Update() {
    return;
};

void Render() {
    SDL_SetRenderDrawColor(renderer_, backgrd.r, backgrd.g, backgrd.b, 255);
    SDL_RenderClear(renderer_);


    SDL_SetRenderDrawColor(renderer_, line.r, line.g, line.b, 255);
    SDL_RenderFillRect(renderer_, &frect);


    SDL_RenderPresent(renderer_);

}

int transX(int x)
{ 
    return x + width / 2;
};

int transY(int y)
{ 
    return height / 2 - y;
};