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
picture pictures[12]; // ������12����Ƭ
extern int picture_index;

/**
 * @brief ������Ϊ��λ��ȡ���ڸ߶�
 *
 * @return int
 */
int DisplayGetWindowPixelWidth();

/**
 * @brief ������Ϊ��λ��ȡ���ڸ߶�
 *
 * @return int ���ڸ߶�
 */
int DisplayGetWindowPixelHeight();

/**
 * @brief ���õ�ǰport�ĳߴ�
 * ��������Ϊ��λ
 * @param x �������½ǵĺ�����
 * @param y �������½ǵ�������
 * @param width ���ڿ��
 * @param height ���ڸ߶�
 */
void DisplayViewPort(int x, int y, int width, int height);

/**
 * @brief ��ĳ����ɫ���ĳ��port
 *
 * @param r
 * @param g
 * @param b
 */
void DisplayFillWithColor(float r, float g, float b);

/**
 * @brief ��ʾ����ֵΪi��ͼƬ
 *
 * @param i ͼƬ������ֵ
 * @param x ͼƬ���½ǵĺ�����ֵ
 * @param y ͼƬ���½ǵ�������ֵ����������Ϊ��λ
 */
void DisplayPicture(int i);

/**
 * @brief ����ͼƬ��С
 *
 * @param output_width ���ĺ�ͼƬ�Ŀ�
 * @param output_height ���ĺ�ͼƬ�ĳ�
 */
void ResizePicture(int i, int output_width, int output_height);

/**
 * @brief ��ʾ����ֵΪi��ͼƬ
 *
 * @param i ͼƬ������ֵ
 */
void DisplayData(int i);

/**
 * @brief ��յ�ǰ��ʾͼƬ����
 *
 * @param i ͼƬ������ֵ
 */
void ClearPicture(int i);

/**
 * @brief ��ȡ���·��Ϊs��ͼƬ
 *
 * @param s ͼƬ���·��
 * @return int ����ͼƬ������ֵ
 */
int ReadInPicture(string s);

/**
 * @brief ˳ʱ����ת����ֵΪi��ͼƬ
 *
 * @param i ͼƬ������ֵ
 */
void RightRotatePicture(int i);

/**
 * @brief ��ʱ����ת����ֵΪi��ͼƬ
 *
 * @param i ͼƬ������ֵ
 */
void LeftRotatePicture(int i);

/**
 * @brief ����ͼƬ
 *
 * @param s ����ͼƬ�����·��
 * @param i Ҫ����ͼƬ������ֵ
 */
void SavePicture(string s, int i);

/**
 * @brief
 * ����ԭ�㶼ȡ����Ļ���½�!
 * @param i ͼƬ������ֵ
 * @param x_downLeft
 * @param y_downLeft
 * @param x_upRight
 * @param y_upRight
 */
void CutPicture(int i, int x_downLeft, int y_downLeft, int x_upRight, int y_upRight);

/**
 * @brief ����ͼƬ��ʾ������λ��x
 *
 * @param x
 */
void SetPictureMiddleX(int x);

/**
 * @brief ����ͼƬ��ʾ������λ��y
 *
 * @param y
 */
void SetPictureMiddleY(int y);

/**
 * @brief ��ȡ��һ����ʾͼƬ������ֵ
 *
 * @param index ��ǰ������ֵ
 * @return int ��һ����ʾͼƬ������ֵ
 */
int GetNextIndex(int index);

/**
 * @brief ��ȡ��һ����ʾͼƬ������ֵ
 *
 * @param index ��ǰ������ֵ
 * @return int ��һ����ʾͼƬ������ֵ
 */
int GetPrevIndex(int index);

/**
 * @brief ��1.1�ı����Ŵ�����ֵΪi��ͼƬ
 *
 * @param i ͼƬ����ֵ
 * @param max_width �Ŵ�ͼƬ�������
 * @param max_height �Ŵ�ͼƬ�����߶�
 */
void ExpandPicture(int i, int max_width, int max_height);

/**
 * @brief ��0.9�ı�����С����ֵΪi��ͼƬ
 *
 * @param i ͼƬ����ֵ
 */
void ShrinkPicture(int i);

/**
 * @brief ����ȡ������ֵΪi��ͼƬ
 *
 * @param i ͼƬ����ֵ
 */
void RetrievePicture(int i);

/**
 * @brief Ѱ�����·��Ϊname��ͼƬ����ֵ
 *
 * @param name ͼƬ���·��
 * @return int ����ͼƬ������ֵ
 */
int FindIndex(string name);
