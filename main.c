#include <string.h>
#include <math.h>

#include "libgraphics/extgraph.h"
#include "libdisplay/display.h"
#include "libgraphics/imgui.h"

static double windows_width;  // 窗口宽度 ， 以inch 为单位
static double windows_height; // 窗口高度 ， 以inch 为单位
static int gui_Mode = 0;      // 选择gui的模式

void Display();
void StartGUI();

// 用户的字符事件响应函数
void CharEventProcess(char ch)
{
    uiGetChar(ch);
    Display();
}

// 用户的键盘事件响应函数
void KeyboardEventProcess(int key, int event)
{
    uiGetKeyboard(key, event); // GUI获取键盘
    Display();                 // 刷新显示
}

// 用户的鼠标事件响应函数
void MouseEventProcess(int x, int y, int button, int event)
{
    uiGetMouse(x, y, button, event); // GUI获取鼠标
    Display();                       // 刷新显示
}

// 主函数
void Main()
{
    // 设置全屏
    windows_width = GetFullScreenWidth();
    windows_height = GetFullScreenWidth();
    SetWindowSize(windows_width, windows_height);
    // int x , y , n ;
    // unsigned char *data = stbi_load("./resource/1.bmp" , &x , &y , &n , 4);
    // int size = sizeof(data);
    InitGraphics();
    SetWindowTitle("MyBmp");
    // ========================GUI PART==================================
    //自定义颜色
    DefineColor("GrayBlue", 0.57, 0.71, 0.83);

    // 注册时间响应函数
    registerCharEvent(CharEventProcess);         // 字符
    registerKeyboardEvent(KeyboardEventProcess); // 键盘
    registerMouseEvent(MouseEventProcess);       // 鼠标

    setMenuColors("GrayBlue", "Black", "Dark Gray", "White", 1);
    setButtonColors("GrayBlue", "Black", "Dark Gray", "White", 1);
    setTextBoxColors("GrayBlue", "Black", "Dark Gray", "White", 1);

    // =================================================================================
    // char *s = (char *)malloc(100);
    // int width = displayGetWindowPixelWidth();
    // int height = displayGetWindowPixelHeight();
    // sprintf(s , "x = %d , y = %d , n = %d size = %d width = %d , height = %d" , x , y, n , size , width , height);
    // MovePen(1,1);
    // DrawTextString(s);
    // registerTimerEvent(timerCallback);
    // 15ms为周期，近似于60FPS
    // // startTimer(0, 15);
    // displayViewPort(0,0,displayGetWindowPixelWidth(),displayGetWindowPixelHeight());

    // 初始化窗口设置背景颜色为白色
    // displayFillWithColor(255,255,255);

    // displayPicture("./resource/2.bmp");
    // clearPicture();
    // readInPicture("./resource/2.bmp");
    // right_Rotate_Picture();
    // right_Rotate_Picture();
    // displayPicture();

    //==============================cur part ==========================//
    // cut_Picture(0,0,200,200);
    // displayPicture();

    //==============================save part =========================//
    // save_Picture("./resource/saved.bmp");
    // displayPicture("./resource/2.bmp");

    //==============================resize part =======================//
    // displayPicture("./resource/2.bmp");
    // clearPicture();
    // resizePicture(500 ,500);
    // displayPicture();
    // clearPicture();
    // resizePicture(100 ,100);
    // displayData();
}

// 显示函数
void Display()
{

    switch (gui_Mode)
    {
    case 0:
        StartGUI();
        break; //开始界面
    default:
        break;
    }
}

// 开始界面
void StartGUI()
{
    double fH = GetFontHeight();  //字高
    double w = windows_width / 4; //按钮宽度
    double h = fH * 2.5;          //按钮高度
    int selection;                //菜单选项
    double x;
    double y;
    static char *menuListHelp[] = {"help" , "about" ,"methods"};
    static char *selectedLabel1 = NULL;
    static char *selectedLabel2 = NULL;

    drawMenuBar(0, windows_height - fH * 1.5, windows_width, fH * 1.5); //菜单栏

    selection = menuList(GenUIID(0), 0, windows_height - fH * 1.5, w / 3, TextStringWidth(menuListHelp[1]) * 1.2, fH * 1.5, menuListHelp, sizeof(menuListHelp) / sizeof(menuListHelp[0]));
    if (selection > 0)
    {
        selectedLabel1 = menuListHelp[0];
        selectedLabel2 = menuListHelp[selection];
    }
    if (selection == 1)
    {
        gui_Mode = 0;
    }
    if (selection == 2)
    {
        gui_Mode = 0;
    }

    drawMenuBar(0, 0, windows_width, fH * 3); //上一步操作显示栏

    if (button(GenUIID(0), windows_width / 2 - w / 2, windows_height / 2 + 7 * fH, w, h, "登录"))
    {
        selectedLabel1 = NULL;
        selectedLabel2 = NULL;
        gui_Mode = 0;
    }
    // if (button(GenUIID(0), windows_width / 2 - w / 2, windows_height / 2 + 3.5 * fH, w, h, "用户登录"))
    // {
    //     selectedLabel1 = NULL;
    //     selectedLabel2 = NULL;
    //     gui_Mode = 2;
    // }
    // if (button(GenUIID(0), windows_width / 2 - w / 2, windows_height / 2, w, h, "管理员登录"))
    // {
    //     selectedLabel1 = NULL;
    //     selectedLabel2 = NULL;
    //     gui_Mode = 3;
    // }
    // if (button(GenUIID(0), windows_width / 2 - w / 2, windows_height / 2 - 3.5 * fH, w, h, "退出"))
    //     exit(-1);

    // SetPenColor("Red");
    // drawLabel(0, fH * 2, "上一步菜单操作为：");
    // SetPenColor("Black");
    // drawLabel(0, fH * 0.5, selectedLabel1);
    // drawLabel(w / 3, fH * 0.5, selectedLabel2);
}
