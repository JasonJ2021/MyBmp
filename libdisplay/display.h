#pragma once
#include "../third_party/stb_image.h"
#include "../libgraphics/strlib.h"

unsigned char *data;
typedef struct {
    int picture_width;
    int picture_height;
    unsigned char *data;
    char name[128];
}picture;
picture pictures[12]; // 最多读入12张照片
extern int picture_index;

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
 * @brief 
 * 
 * @param i 图片的索引值
 * @param x 图片左下角的横坐标值
 * @param y 图片左下角的纵坐标值，都以像素为单位
 */
void displayPicture(int i);


void resizePicture(int output_width , int output_height);


void displayData(int i );


void clearPicture(int i );


void readInPicture(string s);

void right_Rotate_Picture(int i );

void left_Rotate_Picture(int i );

void save_Picture(string s , int i);



/**
 * @brief 
 * 坐标原点都取在屏幕左下角!
 * @param i 图片的索引值
 * @param x_downLeft 
 * @param y_downLeft 
 * @param x_upRight 
 * @param y_upRight 
 */
void cut_Picture(int i , int x_downLeft , int y_downLeft , int x_upRight , int y_upRight);

/**
 * @brief 设置图片显示的中心位置x
 * 
 * @param x 
 */
void set_picture_middle_x(int x);

/**
 * @brief 设置图片显示的中心位置y
 * 
 * @param y 
 */
void set_picture_middle_y(int y);

int getNextIndex(int index);

int getPrevIndex(int index);


void expandPicture(int i );

void shrinkPicture(int i );

void retrievePicture(int i);