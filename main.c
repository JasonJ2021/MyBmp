#include <string.h>

#include "libgraphics/extgraph.h"
#include "libdisplay/display.h"

#include <math.h>

void timerCallback(int timerID)
{
	static unsigned char color = 0;
	static char increase = 1;

	// 划出一个小的范围（视口）用于显示3D内容
	int windowW = displayGetWindowPixelWidth();
	int windowH = displayGetWindowPixelHeight();
	int viewportW = windowW / 4;
	int viewportH = windowH / 4;
	// 左下 四分之一，显示红色
	displayViewPort(windowW / 4, windowH / 4, viewportW, viewportH);
	displayFillWithColor(color, 0, 0);
	// 右下 四分之一，显示绿色
	displayViewPort(windowW / 2, windowH / 4, viewportW, viewportH);
	displayFillWithColor(0, color, 0);
	// 左上 四分之一，显示蓝色
	displayViewPort(windowW / 4, windowH / 2, viewportW, viewportH);
	displayFillWithColor(0, 0, color);
	// 右上 四分之一，显示白色（灰色）
	displayViewPort(windowW / 2, windowH / 2, viewportW, viewportH);
	displayFillWithColor(color, color, color);

	// 参数时变
	if (increase == 1) ++color;
	else color--;

	if (color == 255) increase = 0;
	else if (color == 0) increase = 1;
}

void Main()
{
	// 设置全屏
	double w = GetFullScreenWidth();
	double h = GetFullScreenHeight();
	SetWindowSize(w, h);
	int x , y , n ;
	// unsigned char *data = stbi_load("./resource/1.bmp" , &x , &y , &n , 4);
	// int size = sizeof(data);
	InitGraphics();
	SetWindowTitle("MyBmp");
	// char *s = (char *)malloc(100);
	// int width = displayGetWindowPixelWidth();
	// int height = displayGetWindowPixelHeight();
	// sprintf(s , "x = %d , y = %d , n = %d size = %d width = %d , height = %d" , x , y, n , size , width , height);
	// MovePen(1,1);
	// DrawTextString(s);
	// registerTimerEvent(timerCallback);
	// 15ms为周期，近似于60FPS
	// // startTimer(0, 15);
	displayViewPort(0,0,displayGetWindowPixelWidth(),displayGetWindowPixelHeight());

	// 初始化窗口设置背景颜色为白色
	displayFillWithColor(255,255,255);
	
	// displayPicture("./resource/2.bmp");
	// clearPicture();
	displayPicture("./resource/2.bmp");
	// displayPicture("./resource/2.bmp");
	// displayPicture("./resource/2.bmp");
	clearPicture();
	resizePicture(500 ,500);
	displayData();
	// clearPicture();
	// resizePicture(100 ,100);
	// displayData();
}