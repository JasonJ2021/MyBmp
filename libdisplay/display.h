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
picture pictures[12]; // ������12����Ƭ
extern int picture_index;

/**
 * @brief ������Ϊ��λ��ȡ���ڿ��
 *
 * @return int ���ڿ��
 */
int displayGetWindowPixelWidth();


/**
 * @brief ������Ϊ��λ��ȡ���ڸ߶�
 *
 * @return int ���ڸ߶�
 */
int displayGetWindowPixelHeight();

/**
 * @brief ���õ�ǰport�ĳߴ�
 * ��������Ϊ��λ
 * @param x �������½ǵĺ�����
 * @param y �������½ǵ�������
 * @param width ���ڿ��
 * @param height ���ڸ߶�
 */
void displayViewPort(int x, int y, int width, int height);

/**
 * @brief ��ĳ����ɫ���ĳ��port
 *
 * @param r
 * @param g
 * @param b
 */
void displayFillWithColor(float r, float g, float b);



/**
 * @brief 
 * 
 * @param i ͼƬ������ֵ
 * @param x ͼƬ���½ǵĺ�����ֵ
 * @param y ͼƬ���½ǵ�������ֵ����������Ϊ��λ
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
 * ����ԭ�㶼ȡ����Ļ���½�!
 * @param i ͼƬ������ֵ
 * @param x_downLeft 
 * @param y_downLeft 
 * @param x_upRight 
 * @param y_upRight 
 */
void cut_Picture(int i , int x_downLeft , int y_downLeft , int x_upRight , int y_upRight);

/**
 * @brief ����ͼƬ��ʾ������λ��x
 * 
 * @param x 
 */
void set_picture_middle_x(int x);

/**
 * @brief ����ͼƬ��ʾ������λ��y
 * 
 * @param y 
 */
void set_picture_middle_y(int y);

int getNextIndex(int index);

int getPrevIndex(int index);


void expandPicture(int i );

void shrinkPicture(int i );

void retrievePicture(int i);