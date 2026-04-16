#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <SDL3/SDL.h>


#define WHEEL_RUN_SCALE 10.00f;
#define MOUTH_RUN_SCALE 00.05f;
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

struct vec2 {
    int s;
    int e;
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
bool IsDrawBACKGRD_ = true;
bool IsDrawLINE_ = true;
bool IsDrawPOINTS_ = true;
// 窗口标题
const char* Title_ = "2r3";
// 渲染“点”的大小。点为一个正方形图像，这是图像的边长
const int FPS = 60;
const float POINT_SIZE = 10.00;
// 窗口的长
const int Width_ = 800;
// 窗口的高
const int Height_ =  800;

// 3D 投影参数
float FOCAL_LENGTH = 1000.00f;  // 焦距（决定透视强度）
float CAMERA_Z = 500.00f;      // 相机距离
float rotationY = 0.00f;       // Y轴旋转角度
float rotationX = 0.00f;       // Y轴旋转角度


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
    // x     y     z    索引
    {-100,  100, -100}, // 0
    {-100,  100,  100}, // 1
    { 100,  100, -100}, // 2
    { 100,  100,  100}, // 3
    {-100, -100, -100}, // 4
    {-100, -100,  100}, // 5
    { 100, -100, -100}, // 6
    { 100, -100,  100}  // 7
};

// SDL 渲染的 2 维图形的集合
std::vector<SDL_FRect> _2DPointList;

std::vector<vec2> LineList = {
    //  | 结束点      | 起始点
    {0, 1}, {0, 2}, {0, 4},
    {1, 3}, {1, 5}, {3, 2},
    {3, 7}, {2, 6}, {4, 5},
    {4, 6}, {5, 7}, {7, 6}
};

bool IsKeyDown[4] = { 
    false, // SDLK_W
    false, // SDLK_A
    false, // SDLK_S
    false  // SDLK_D
};


std::unordered_map<std::string, float> EventValue;

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
 * rotateY 是将 3 维点绕 Y 轴旋转的函数
 * @param point 原始 3 维点
 * @param angle 旋转角度（弧度）
 * @return 旋转后的 3 维点
 * @note 描述可能有误
 */
vec3 rotateY(vec3 point, float angle);
/**
 * rotateX 是将 3 维点绕 X 轴旋转的函数
 * @param point 原始 3 维点
 * @param angle 旋转角度（弧度）
 * @return 旋转后的 3 维点
 * @note 描述可能有误
 */
vec3 rotateX(vec3 point, float angle);
/**
 * SetDrawColor 是设置画笔颜色的函数。
 * @param rgb 画笔的颜色
 */
void SetDrawColor(RGB rgb);
// 画制背景
void DrawBACKGRD();
// 画制线段
void DrawLINE(vec2 vc2);
void DrawLINES();
/** 
 * 画制单个点
 * @param fr SDL 渲染的 2 维图形
 */
void DrawPOINT(SDL_FRect fr);
// 画制所有点
void DrawPOINTS();
void Update2Detas();

int main(int argc, char* argv) 
{
    (void)argc, (void)argv;
    SDL_Log("Hello 2r3");

    // init SDL library
    Initlib(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO));

    // create window and renderer
    window_ = SDL_CreateWindow(Title_, 
                               Width_, 
                               Height_, 
                               SDL_WINDOW_RESIZABLE);
    SDL_SetWindowRelativeMouseMode(window_, true);
    SDL_HideCursor();
    renderer_ = SDL_CreateRenderer(window_, NULL);
    

    EventValue.insert_or_assign("SDL_EVENT_MOUSE_WHEEL_Y", 0.0f);
    EventValue.insert_or_assign("SDL_EVENT_MOUSE_MOTION_X", 0.0f);
    EventValue.insert_or_assign("SDL_EVENT_MOUSE_MOTION_Y", 0.0f);


    // main loop
    auto FrameTime = (1e9 / FPS);
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
            // 窗口失去焦点
            case(SDL_EVENT_WINDOW_FOCUS_LOST): {
                SDL_SetWindowRelativeMouseMode(window_, false);
                SDL_ShowCursor();
                break;
            }
            // 窗口获得焦点
            case(SDL_EVENT_WINDOW_FOCUS_GAINED): {
                SDL_SetWindowRelativeMouseMode(window_, true);
                SDL_HideCursor();
                break;
            }
            // 退出
            case(SDL_EVENT_QUIT): {
                IsGoing_ = false;
                break;
            }
            // 滚轮事件
            case(SDL_EVENT_MOUSE_WHEEL): {
                EventValue["SDL_EVENT_MOUSE_WHEEL_Y"] = event.wheel.y;
                break;
            }
            // 鼠标移动
            case(SDL_EVENT_MOUSE_MOTION): {
                EventValue["SDL_EVENT_MOUSE_MOTION_X"] = event.motion.xrel;
                EventValue["SDL_EVENT_MOUSE_MOTION_Y"] = event.motion.yrel;
                break;
            }
            case(SDL_EVENT_KEY_DOWN): {
                switch (event.key.key) {
                    case(SDLK_ESCAPE): { IsGoing_  = !IsGoing_;      break; }
                    case(SDLK_F1): { IsDrawPOINTS_ = !IsDrawPOINTS_; break; }
                    case(SDLK_F2): { IsDrawLINE_   = !IsDrawLINE_;   break; };

                    case(SDLK_W): { IsKeyDown[0] = true; break; };
                    case(SDLK_A): { IsKeyDown[1] = true; break; };
                    case(SDLK_S): { IsKeyDown[2] = true; break; };
                    case(SDLK_D): { IsKeyDown[3] = true; break; };
                    default: break; 
                }
                break;
            };
            default: break; 
        }
    }
};

void Update() {
    if (IsKeyDown[0]) CAMERA_Z -= 10.00f;
    if (IsKeyDown[2]) CAMERA_Z += 10.00f;

    // 更新滚轮
    float Wheel_Y = EventValue["SDL_EVENT_MOUSE_WHEEL_Y"];
    if (Wheel_Y != 0.00f) CAMERA_Z += Wheel_Y * WHEEL_RUN_SCALE;
    EventValue["SDL_EVENT_MOUSE_WHEEL_Y"] = 0.00f;

    float Mouth_Mov_X = EventValue["SDL_EVENT_MOUSE_MOTION_X"];
    if (Mouth_Mov_X != 0.00f) rotationX += Mouth_Mov_X * MOUTH_RUN_SCALE;
    EventValue["SDL_EVENT_MOUSE_MOTION_X"] = 0.00f;

    float Mouth_Mov_Y = EventValue["SDL_EVENT_MOUSE_MOTION_Y"];
    if (Mouth_Mov_Y != 0.00f) rotationY += Mouth_Mov_Y * MOUTH_RUN_SCALE;
    EventValue["SDL_EVENT_MOUSE_MOTION_Y"] = 0.00f;


    Update2Detas();
};

void Render() {
    if (IsDrawBACKGRD_) DrawBACKGRD();
    if (IsDrawLINE_) DrawLINES();
    if (IsDrawPOINTS_) DrawPOINTS();
    SDL_RenderPresent(renderer_);
    for (int i = 0 ; i < sizeof(IsKeyDown)/sizeof(IsKeyDown[0]); i++) IsKeyDown[i] = false;
}

float transX(float x)
{ 
    return x + (float)(Width_ / 2);
};

float transY(float y)
{
    return (float)(Height_ / 2) - y;
}

vec3 rotateY(vec3 point, float angle) {

    float cosA = cos(angle);
    float sinA = sin(angle);
    return {
        point.x,
        point.y * cosA - point.z * sinA,
        point.y * sinA + point.z * cosA
    };
}

vec3 rotateX(vec3 point, float angle) {
    float cosA = cos(angle);
    float sinA = sin(angle);
    return {
        point.x * cosA - point.z * sinA,
        point.y,
        point.x * sinA + point.z * cosA
    };
}

void SetDrawColor(RGB rgb)
{
    SDL_SetRenderDrawColor(renderer_, rgb.r, rgb.g, rgb.b, 255);
}

void DrawBACKGRD() {
    SDL_FRect BackGRD_FR = {0.00, 0.00, (float)Width_, (float)Height_};
    SetDrawColor(BACKGRD);
    SDL_RenderFillRect(renderer_, &BackGRD_FR);
};

void DrawLINE(vec2 vc2) {
    SetDrawColor(LINE);
    int start = vc2.s;
    int end   = vc2.e;
    float x1, y1, x2, y2;
    x1 = _2DPointList[start].x;
    y1 = _2DPointList[start].y;
    x2 = _2DPointList[end].x;
    y2 = _2DPointList[end].y;
    SDL_RenderLine(renderer_, x1, y1, x2, y2);
};

void DrawLINES() {
    for (const auto& Line : LineList)
        DrawLINE(Line);
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
    _2DPointList.clear();  // 清空旧数据

    for (const auto& point : _3DPointList) {
        // 先旋转点
        vec3 rotated = rotateX(rotateY(point, rotationY), rotationX);
        // 计算点到相机的距离
        float depth = rotated.z + CAMERA_Z;
        // 避免除零错误
        if (depth <= 0.001f) depth = 0.001f;
        // 透视投影公式
        float projectedX = (rotated.x * FOCAL_LENGTH) / depth;
        float projectedY = (rotated.y * FOCAL_LENGTH) / depth;
        // 根据深度调整点的大小
        float size = POINT_SIZE * (FOCAL_LENGTH / depth);
        SDL_FRect Temp = {
            transX(projectedX),
            transY(projectedY),
            size, size };
        _2DPointList.push_back(Temp);
    }
};