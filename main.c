#include <string.h>
#include <math.h>

#include "libgraphics/extgraph.h"
#include "libdisplay/display.h"
#include "libgraphics/imgui.h"

static double windows_width;  // ���ڿ�� �� ��inch Ϊ��λ
static double windows_height; // ���ڸ߶� �� ��inch Ϊ��λ
static int gui_Mode = 0;      // ѡ��gui��ģʽ

void Display();
void StartGUI();

// �û����ַ��¼���Ӧ����
void CharEventProcess(char ch)
{
    uiGetChar(ch);
    Display();
}

// �û��ļ����¼���Ӧ����
void KeyboardEventProcess(int key, int event)
{
    uiGetKeyboard(key, event); // GUI��ȡ����
    Display();                 // ˢ����ʾ
}

// �û�������¼���Ӧ����
void MouseEventProcess(int x, int y, int button, int event)
{
    uiGetMouse(x, y, button, event); // GUI��ȡ���
    Display();                       // ˢ����ʾ
}

// ������
void Main()
{
    // ����ȫ��
    windows_width = GetFullScreenWidth();
    windows_height = GetFullScreenWidth();
    SetWindowSize(windows_width, windows_height);
    // int x , y , n ;
    // unsigned char *data = stbi_load("./resource/1.bmp" , &x , &y , &n , 4);
    // int size = sizeof(data);
    InitGraphics();
    SetWindowTitle("MyBmp");
    // ========================GUI PART==================================
    //�Զ�����ɫ
    DefineColor("GrayBlue", 0.57, 0.71, 0.83);

    // ע��ʱ����Ӧ����
    registerCharEvent(CharEventProcess);         // �ַ�
    registerKeyboardEvent(KeyboardEventProcess); // ����
    registerMouseEvent(MouseEventProcess);       // ���

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
    // 15msΪ���ڣ�������60FPS
    // // startTimer(0, 15);
    // displayViewPort(0,0,displayGetWindowPixelWidth(),displayGetWindowPixelHeight());

    // ��ʼ���������ñ�����ɫΪ��ɫ
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

// ��ʾ����
void Display()
{

    switch (gui_Mode)
    {
    case 0:
        StartGUI();
        break; //��ʼ����
    default:
        break;
    }
}

// ��ʼ����
void StartGUI()
{
    double fH = GetFontHeight();  //�ָ�
    double w = windows_width / 4; //��ť���
    double h = fH * 2.5;          //��ť�߶�
    int selection;                //�˵�ѡ��
    double x;
    double y;
    static char *menuListHelp[] = {"help" , "about" ,"methods"};
    static char *selectedLabel1 = NULL;
    static char *selectedLabel2 = NULL;

    drawMenuBar(0, windows_height - fH * 1.5, windows_width, fH * 1.5); //�˵���

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

    drawMenuBar(0, 0, windows_width, fH * 3); //��һ��������ʾ��

    if (button(GenUIID(0), windows_width / 2 - w / 2, windows_height / 2 + 7 * fH, w, h, "��¼"))
    {
        selectedLabel1 = NULL;
        selectedLabel2 = NULL;
        gui_Mode = 0;
    }
    // if (button(GenUIID(0), windows_width / 2 - w / 2, windows_height / 2 + 3.5 * fH, w, h, "�û���¼"))
    // {
    //     selectedLabel1 = NULL;
    //     selectedLabel2 = NULL;
    //     gui_Mode = 2;
    // }
    // if (button(GenUIID(0), windows_width / 2 - w / 2, windows_height / 2, w, h, "����Ա��¼"))
    // {
    //     selectedLabel1 = NULL;
    //     selectedLabel2 = NULL;
    //     gui_Mode = 3;
    // }
    // if (button(GenUIID(0), windows_width / 2 - w / 2, windows_height / 2 - 3.5 * fH, w, h, "�˳�"))
    //     exit(-1);

    // SetPenColor("Red");
    // drawLabel(0, fH * 2, "��һ���˵�����Ϊ��");
    // SetPenColor("Black");
    // drawLabel(0, fH * 0.5, selectedLabel1);
    // drawLabel(w / 3, fH * 0.5, selectedLabel2);
}
