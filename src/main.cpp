#include <iostream>
#include <vector>
#include <SDL3/SDL.h>


/**
 * RGB 是一个自定义的结构体用 RGB 格式来存放一个色值。大小范围 [0, 255]
 * @param r 红色分量
 * @param g 绿色分量
 * @param b 蓝色分量
 */
struct RGB {
    Uint8 r;
    Uint8 g;
    Uint8 b;
};
/**
 * vec3 是一个三位变量，它存放在三维正交坐标系中的坐标值。
 * @param x 三维正交坐标系的 x 坐标
 * @param y 三维正交坐标系的 y 坐标
 * @param z 三维正交坐标系的 z 坐标
 */
struct vec3 {
    float x;
    float y;
    float z;
};
// 主循环的开关 true 代表运行主循环；false 代表停止主循环
bool IsGoing_ = true;
// 是否打印当前的帧率
bool IsLogFPS_ = false;
// 窗口标题
const char* Title_ = "2r3";
// 渲染“点”的大小。点为一个正方形图像，这是图像的边长
const float POINT_SIZE = 10.00;
// 窗口的长
const int width_ = 1960;
// 窗口的高
const int height_ =  1080;


SDL_Window* window_ = nullptr;
SDL_Renderer* renderer_ = nullptr;

// 背景的颜色
const RGB BACKGRD = {2  , 62 , 138}; // #023E8A
// 点的颜色
const RGB POINT   = {255, 0  , 110}; // #FF016E
// 线段的颜色
const RGB LINE    = {254, 228, 64 }; // #FEE440

// 在标准正交坐标系中的 3 维点的集合
std::vector<vec3> _3DPointList = {
    {-100,  100, -100},
    {-100,  100,  100},
    { 100,  100, -100},
    { 100,  100,  100},
    {-100, -100, -100},
    {-100, -100,  100},
    { 100, -100, -100},
    { 100, -100,  100}
};

// SDL 渲染的 2 维图形的集合
std::vector<SDL_FRect> _2DPointList;

/**
 * Initlib 是初始化的时候，判断是否失败的函数
 * @param flags 传入的一个逻辑表达式。正常工作应传入 true
 * @return true 代表初始化成功；false 代表失败
 */
bool Initlib(bool flags);
void HendleEvents();
void Update();
void Render();
/**
 * transX 是将传入的 3 维坐标的 x 值转换为 SDL 坐标系下的 x 值
 * @param x 传入的 3 维坐标
 * @return SDL 坐标系下的 x 值
 */
float transX(float x);
/**
 * transY 是将传入的 3 维坐标的 y 值转换为 SDL 坐标系下的 y 值
 * @param y 传入的 3 维坐标
 * @return SDL 坐标系下的 y 值
 */
float transY(float y);
/**
 * SetDrawColor 是设置画笔颜色的函数。
 * @param rgb 画笔的颜色
 */
void SetDrawColor(RGB rgb);
// 画制背景
void DrawBACKGRD();
// 画制线段
void DrawLINES();
/** 
 * 画制单个点
 * @param fr SDL 渲染的 2 维图形
 */
void DrawPOINT(SDL_FRect fr);
// 画制所有点
void DrawPOINTS();
void Update3Detas();
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

void Update3Detas() {
    for (int i = 0; i < _3DPointList.size(); i++) {
        _3DPointList[i].x = _3DPointList[i].x / _3DPointList[i].z;
        _3DPointList[i].y = _3DPointList[i].y / _3DPointList[i].z;
    }
}
void Update2Detas() {
    for (int i = 0; i < _3DPointList.size(); i++) {
        SDL_FRect Temp = {transX(_3DPointList[i].x),
                          transY(_3DPointList[i].y),
                          POINT_SIZE, POINT_SIZE  };
        _2DPointList.push_back(Temp);
    }
};

