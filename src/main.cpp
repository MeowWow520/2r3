#include <iostream>
#include <SDL3/SDL.h>


bool IsGoing_ = true;
struct WSize_ {
    char* title;
    int wight; 
    int height;
};
bool Initlib(bool flags);
void HendleEvents();
void Update();
void Render();


int main(int argc, char* argv) 
{
    (void)argc, (void)argv;
    // std::cout << u8"hello world";
    SDL_Log("Hello 2r3");

    // init SDL library
    if (Initlib(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO))) {
        SDL_Log("init SDL library successfully");
    };

    WSize_ WindowSize = {"2r3", 1960, 1080};
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    // create window and renderer
    window = SDL_CreateWindow(WindowSize.title, 
                             WindowSize.wight, 
                             WindowSize.height, 
                             SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, NULL);

    // main loop
    while(IsGoing_) {
        HendleEvents();
        Update();
        Render();
    }
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
    return;
};
