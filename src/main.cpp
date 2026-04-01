#include <iostream>
#include <vector>
#include <SDL3/SDL.h>


struct RGB {
    Uint8 r;
    Uint8 g;
    Uint8 b;
};
struct vec3 {
    float x;
    float y;
    float z;
};

bool IsGoing_ = true;
const char* Title_ = "2r3";
int width_ = 1960;
int height_ =  1080;
const float POINT_SIZE = 10.00;
SDL_Window* window_ = nullptr;
SDL_Renderer* renderer_ = nullptr;

const RGB BACKGRD = {2  , 62 , 138};
const RGB POINT   = {255, 0  , 110};
const RGB lINE    = {254, 228, 64 };

std::vector<vec3> _3DPointList;

std::vector<SDL_FRect> PointList = {
    {100.00, 0.00, POINT_SIZE, POINT_SIZE} // 测试 Rect
};


bool Initlib(bool flags);
void HendleEvents();
void Update();
void Render();
int transX(int x);
int transY(int y);
void SetDrawColor(RGB rgb);



int main(int argc, char* argv) 
{
    (void)argc, (void)argv;
    SDL_Log("Hello 2r3");

    // init SDL library
    if (Initlib(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO))) {
        SDL_Log("init SDL library successfully");
    };

    // create window and renderer
    window_ = SDL_CreateWindow(Title_, 
                              width_, 
                              height_, 
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
    SetDrawColor(BACKGRD);
    SDL_RenderClear(renderer_);

    // <!-- start for test -->
    SetDrawColor(POINT);
    SDL_RenderFillRect(renderer_, &PointList[0]);
    // <!-- end for test   -->

    SDL_RenderPresent(renderer_);

}

int transX(int x)
{ 
    return x + width_ / 2;
};

int transY(int y)
{ 
    return height_ / 2 - y;
}

void SetDrawColor(RGB rgb) 
{
    SDL_RenderClear(renderer_);
    SDL_SetRenderDrawColor(renderer_, rgb.r, rgb.g, rgb.b, 255);
};