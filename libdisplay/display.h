#pragma once
#include "../third_party/stb_image.h"
#include "../libgraphics/strlib.h"

unsigned char *data;
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
 * @brief ��ʾһ��ͼƬ
 * 
 * @param s �������ļ��� 
 */
void displayPicture();


void resizePicture(int output_width , int output_height);


void displayData();


void clearPicture();


void readInPicture(string s);

void right_Rotate_Picture();

void left_Rotate_Picture();

void save_Picture(string s);



/**
 * @brief 
 * ����ԭ�㶼ȡ����Ļ���½�!
 * @param x_downLeft 
 * @param y_downLeft 
 * @param x_upRight 
 * @param y_upRight 
 */
void cut_Picture(int x_downLeft , int y_downLeft , int x_upRight , int y_upRight);

