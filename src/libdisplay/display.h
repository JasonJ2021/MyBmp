#pragma once
#include "../third_party/stb_image.h"
#include "../libgraphics/strlib.h"

typedef struct
{
    int picture_width;
    int picture_height;
    unsigned char *data;
    char name[128];
} picture;
picture pictures[12]; // 最多读入12张照片
extern int picture_index;

/**
 * @brief 以像素为单位获取窗口高度
 *
 * @return int
 */
int DisplayGetWindowPixelWidth();

/**
 * @brief 以像素为单位获取窗口高度
 *
 * @return int 窗口高度
 */
int DisplayGetWindowPixelHeight();

/**
 * @brief 设置当前port的尺寸
 * 均以像素为单位
 * @param x 距离左下角的横坐标
 * @param y 距离左下角的纵坐标
 * @param width 窗口宽度
 * @param height 窗口高度
 */
void DisplayViewPort(int x, int y, int width, int height);

/**
 * @brief 以某种颜色填充某个port
 *
 * @param r
 * @param g
 * @param b
 */
void DisplayFillWithColor(float r, float g, float b);

/**
 * @brief 显示索引值为i的图片
 *
 * @param i 图片的索引值
 * @param x 图片左下角的横坐标值
 * @param y 图片左下角的纵坐标值，都以像素为单位
 */
void DisplayPicture(int i);

/**
 * @brief 更改图片大小
 *
 * @param output_width 更改后图片的宽
 * @param output_height 更改后图片的长
 */
void ResizePicture(int i, int output_width, int output_height);

/**
 * @brief 显示索引值为i的图片
 *
 * @param i 图片的索引值
 */
void DisplayData(int i);

/**
 * @brief 清空当前显示图片内容
 *
 * @param i 图片的索引值
 */
void ClearPicture(int i);

/**
 * @brief 读取相对路径为s的图片
 *
 * @param s 图片相对路径
 * @return int 返回图片的索引值
 */
int ReadInPicture(string s);

/**
 * @brief 顺时针旋转索引值为i的图片
 *
 * @param i 图片的索引值
 */
void RightRotatePicture(int i);

/**
 * @brief 逆时针旋转索引值为i的图片
 *
 * @param i 图片的索引值
 */
void LeftRotatePicture(int i);

/**
 * @brief 保存图片
 *
 * @param s 保存图片的相对路径
 * @param i 要保存图片的索引值
 */
void SavePicture(string s, int i);

/**
 * @brief
 * 坐标原点都取在屏幕左下角!
 * @param i 图片的索引值
 * @param x_downLeft
 * @param y_downLeft
 * @param x_upRight
 * @param y_upRight
 */
void CutPicture(int i, int x_downLeft, int y_downLeft, int x_upRight, int y_upRight);

/**
 * @brief 设置图片显示的中心位置x
 *
 * @param x
 */
void SetPictureMiddleX(int x);

/**
 * @brief 设置图片显示的中心位置y
 *
 * @param y
 */
void SetPictureMiddleY(int y);

/**
 * @brief 获取下一个显示图片的索引值
 *
 * @param index 当前的索引值
 * @return int 下一个显示图片的索引值
 */
int GetNextIndex(int index);

/**
 * @brief 获取上一个显示图片的索引值
 *
 * @param index 当前的索引值
 * @return int 上一个显示图片的索引值
 */
int GetPrevIndex(int index);

/**
 * @brief 以1.1的比例放大索引值为i的图片
 *
 * @param i 图片索引值
 * @param max_width 放大图片的最大宽度
 * @param max_height 放大图片的最大高度
 */
void ExpandPicture(int i, int max_width, int max_height);

/**
 * @brief 以0.9的比例缩小索引值为i的图片
 *
 * @param i 图片索引值
 */
void ShrinkPicture(int i);

/**
 * @brief 重新取回索引值为i的图片
 *
 * @param i 图片索引值
 */
void RetrievePicture(int i);

/**
 * @brief 寻找相对路径为name的图片索引值
 *
 * @param name 图片相对路径
 * @return int 返回图片的索引值
 */
int FindIndex(string name);
