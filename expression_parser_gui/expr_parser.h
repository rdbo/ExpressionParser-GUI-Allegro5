#include "pch.h"
#define FONT_SIZE 18
#define MAX_DECIMALS 15
#define MAX_OPERATIONS 1000
#define DEFAULT_MAX 1024
#define DEFAULT_ID 0
#define DEFAULT_BTN 25
#define BTN_VEC ImVec2(DEFAULT_BTN, DEFAULT_BTN)

extern ALLEGRO_DISPLAY* display;
extern ALLEGRO_EVENT_QUEUE* queue;
extern ImGuiWindowFlags wf;
extern ImFont* roboto;
extern ImFont* roboto2;

struct Window
{
    int x, y, w, h;
};

extern Window wrect;

namespace ExpressionParser
{
    void Init();
    void Run();
    void Draw();
    void AdjustWindow();
    void Solve(int i);
    void Shutdown();
}