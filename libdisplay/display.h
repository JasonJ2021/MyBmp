#pragma once
#include "../third_party/stb_image.h"
#include "../libgraphics/strlib.h"

unsigned char *data;
/**
 * @brief 以像素为单位获取窗口宽度
 *
 * @return int 窗口宽度
 */
int displayGetWindowPixelWidth();


/**
 * @brief 以像素为单位获取窗口高度
 *
 * @return int 窗口高度
 */
int displayGetWindowPixelHeight();

/**
 * @brief 设置当前port的尺寸
 * 均以像素为单位
 * @param x 距离左下角的横坐标
 * @param y 距离左下角的纵坐标
 * @param width 窗口宽度
 * @param height 窗口高度
 */
void displayViewPort(int x, int y, int width, int height);

/**
 * @brief 以某种颜色填充某个port
 *
 * @param r
 * @param g
 * @param b
 */
void displayFillWithColor(float r, float g, float b);



/**
 * @brief 显示一张图片
 * 
 * @param s 完整的文件名 
 */
void displayPicture(string s);


void resizePicture(int output_width , int output_height);


void displayData();


void clearPicture();

