#pragma once
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
