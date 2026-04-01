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

// 标准正交坐标系 x, y, z
std::vector<vec3> _3DPointList = {
    {-100,  100, -100},
    {-100,  100,  100},
    { 100,  100,  100},
    { 100,  100, -100},
    {-100, -100, -100},
    {-100, -100,  100},
    { 100, -100, -100},
    { 100, -100,  100}
};

std::vector<SDL_FRect> _2DPointList;


bool Initlib(bool flags);
void HendleEvents();
void Update();
void Render();
float transX(float x);
float transY(float y);
void SetDrawColor(RGB rgb);
void DrawBACKGRD();
void DrawLINES();
void DrawPOINT(SDL_FRect fr);
void DrawPOINTS();
void Update2Detas();

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
    Update2Detas();
};

void Render() {
    DrawBACKGRD();
    // DrawLINES();
    DrawPOINTS();
    SDL_RenderPresent(renderer_);
}

float transX(float x)
{ 
    return x + (float)(width_ / 2);
};

float transY(float y)
{ 
    return (float)(height_ / 2) - y;
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
    SDL_FRect Temp = {fr.x - (POINT_SIZE / 2),
                      fr.y - (POINT_SIZE / 2),
                      POINT_SIZE, POINT_SIZE};
    SDL_RenderFillRect(renderer_, &Temp);
};

void DrawPOINTS() {
    for (int i = 0; i < _2DPointList.size(); i++)
        DrawPOINT(_2DPointList[i]);
};

void Update2Detas() {
    for (int i = 0; i < _3DPointList.size(); i++) {
        SDL_FRect Temp = {transX(_3DPointList[i].x),
                          transY(_3DPointList[i].y),
                          POINT_SIZE, POINT_SIZE  };
        _2DPointList.push_back(Temp);
    }
};
