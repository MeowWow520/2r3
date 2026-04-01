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
bool IsLogFPS_ = false;
const char* Title_ = "2r3";
const float POINT_SIZE = 10.00;
int width_ = 1960;
int height_ =  1080;
SDL_Window* window_ = nullptr;
SDL_Renderer* renderer_ = nullptr;

const RGB BACKGRD = {2  , 62 , 138}; // #023E8A
const RGB POINT   = {255, 0  , 110}; // #FF016E
const RGB LINE    = {254, 228, 64 }; // #FEE440

std::vector<vec3> _3DPointList;

std::vector<SDL_FRect> _2DPointList = {
    {100.00, 0.00  , POINT_SIZE, POINT_SIZE},
    {200.00, 50.00 , POINT_SIZE, POINT_SIZE},
    {300.00, 100.00, POINT_SIZE, POINT_SIZE},
    {400.00, 150.00, POINT_SIZE, POINT_SIZE},
    {500.00, 200.00, POINT_SIZE, POINT_SIZE},
    {600.00, 250.00, POINT_SIZE, POINT_SIZE},
    {700.00, 300.00, POINT_SIZE, POINT_SIZE}
};


bool Initlib(bool flags);
void HendleEvents();
void Update();
void Render();
int transX(int x);
int transY(int y);
void SetDrawColor(RGB rgb);
void DrawBACKGRD();
void DrawLINES();
void DrawPOINT(SDL_FRect fr);
void DrawPOINTS();


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
    auto FrameTime = (1e9 / 60);
    auto deltaTime = 0.0f;
    while(IsGoing_) {
        auto StartTicks= SDL_GetTicksNS();

        HendleEvents();
        Update();
        Render();

        auto EndTicks = SDL_GetTicksNS();  
        auto Diffent = EndTicks - StartTicks;
        if (Diffent <  FrameTime) {
            SDL_Delay((Uint32)((FrameTime - Diffent) / 1e6));
            deltaTime = (float)(FrameTime / 1e9);
        } else { deltaTime = (float)(Diffent / 1e9); }
        if (IsLogFPS_) SDL_Log("Current FPS: %f", 1 / deltaTime);
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
    // DrawBACKGRD();
    // DrawLINES();
    // DrawPOINTS();
    // SDL_RenderPresent(renderer_);
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
    SDL_SetRenderDrawColor(renderer_, rgb.r, rgb.g, rgb.b, 255);
}

void DrawBACKGRD() {
    SDL_FRect BackGRD_FR = {0.00, 0.00, (float)width_, (float)height_};
    SetDrawColor(BACKGRD);
    SDL_RenderFillRect(renderer_, &BackGRD_FR);
};

void DrawLINES() {
    SetDrawColor(LINE);
};

void DrawPOINT(SDL_FRect fr) {
    SetDrawColor(POINT);
    SDL_RenderFillRect(renderer_, &fr);
};

void DrawPOINTS() {
    for (int i = 0; i < _2DPointList.size(); i++)
        DrawPOINT(_2DPointList[i]);
};
