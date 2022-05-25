#include "display.h"
#include "../libgraphics/win32Export.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../third_party/stb_image.h"
#include "../third_party/stb_image_resize.h"
#include "../third_party/stb_image_write.h"

static HBITMAP oldBitmap = NULL, bitmap = NULL;
static int picture_middle_x = 0;
static int picture_middle_y = 0;
int picture_index = 0;


struct FrameBuffer
{
    int windowsWidth, windowsHeight; // ���ڴ�С
    int portX, portY;                // ��ͼλ�ã��������½�
    int portWidth, portHeight;       // ��ͼ��С
    unsigned char *data;
};

static struct FrameBuffer frameBuffer = {0};

int displayGetWindowPixelWidth()
{
    return GetWindowPixelWidth();
}

int displayGetWindowPixelHeight()
{
    return GetWindowPixelHeight();
}

void displayViewPort(int x, int y, int width, int height)
{
    if (frameBuffer.data == NULL)
    {
        frameBuffer.windowsWidth = displayGetWindowPixelWidth();
        frameBuffer.windowsHeight = displayGetWindowPixelHeight();
        BITMAPINFO bitmapInfo = {{sizeof(BITMAPINFOHEADER),
                                  frameBuffer.windowsWidth, frameBuffer.windowsHeight,
                                  1,
                                  32, // һ������ռ4��byte��32��bit
                                  BI_RGB,
                                  frameBuffer.windowsWidth * frameBuffer.windowsHeight * 4,
                                  0, 0, 0, 0}};
        LPVOID ptr = NULL;
        bitmap = CreateDIBSection(GetDisplayOSDC(), &bitmapInfo, DIB_RGB_COLORS, &ptr, 0, 0);
        oldBitmap = (HBITMAP)SelectObject(GetDisplayOSDC(), bitmap);
        frameBuffer.data = (unsigned char *)ptr;
        frameBuffer.portWidth = frameBuffer.windowsWidth;
        frameBuffer.portHeight = frameBuffer.windowsHeight;
    }
    // ���ͼƬ�Ƿ񳬳�����
    frameBuffer.portX = x < 0 ? 0 : x;
    frameBuffer.portX = frameBuffer.portX >= frameBuffer.windowsWidth - 1
                            ? frameBuffer.windowsWidth - 1
                            : frameBuffer.portX;
    frameBuffer.portY = y < 0 ? 0 : y;
    frameBuffer.portY = frameBuffer.portY >= frameBuffer.windowsHeight - 1
                            ? frameBuffer.windowsHeight - 1
                            : frameBuffer.portY;
    int tmp;
    tmp = frameBuffer.portX + width;
    frameBuffer.portWidth = (tmp > frameBuffer.windowsWidth ? frameBuffer.windowsWidth : tmp) - frameBuffer.portX;
    tmp = frameBuffer.portY + height;
    frameBuffer.portHeight = (tmp > frameBuffer.windowsHeight ? frameBuffer.windowsHeight : tmp) - frameBuffer.portY;
}

void displayFillWithColor(float r, float g, float b)
{
    // ������ÿ��һ���൱������һ��
    int stride = frameBuffer.windowsWidth * 4;
    // ����ʼָ�룬ָ��ǰviewport ��N�� �� ��һ������
    unsigned char *rowPtr = frameBuffer.data + frameBuffer.portY * stride + frameBuffer.portX * 4;
    for (int row = 0; row < frameBuffer.portHeight; ++row)
    {
        // ������ʼָ�룬ָ�� rowPtr������ �� ��N�� ����
        unsigned char *pixPtr = rowPtr;
        for (int col = 0; col < frameBuffer.portWidth; ++col)
        {
            // �����ظ���ɫֵ
            // ע�⣺WIN32��BITMAP����BGRA���֣�������RGBA
            //   һ������ռ4��byte��32��bit
            pixPtr[0] = b;
            pixPtr[1] = g;
            pixPtr[2] = r;
            pixPtr[3] = 255; // ͸��������

            pixPtr += 4; // ����һ��
        }
        rowPtr += stride; // ����һ��
    }

    // ������viewport���Ϊ��Ҫ���£�������Ҫ������
    {
        // ע�⣺viewportRECTʹ��Win32����ϵ��
        //   y�ᷴ��ͬʱԭ���ڴ������Ͻ�
        RECT viewportRECT = {
            frameBuffer.portX,                                                      // ��
            frameBuffer.windowsHeight - frameBuffer.portY - frameBuffer.portHeight, // ��
            frameBuffer.portX + frameBuffer.portWidth,                              // ��
            frameBuffer.windowsHeight - frameBuffer.portY                           // ��
        };
        InvalidateRect(GetDisPlayWindows(), &viewportRECT, FALSE); // FALSEע�⣡
    }
}

void readInPicture(string s)
{
    for(int i = 0 ; i < picture_index;i++){
        if(strcmp(s,pictures[i].name) == 0){
            return;
        }
    }
    printf("read in a picture !\n");
    int n;
    pictures[picture_index].data = stbi_load(s, &pictures[picture_index].picture_width, &pictures[picture_index].picture_height, &n, 4);
    strcpy(pictures[picture_index].name , s);
    picture_index++;
}

void displayPicture(int i)
{
    int x = picture_middle_x - pictures[i].picture_width/2;
    int y = picture_middle_y - pictures[i].picture_height/2;
    displayViewPort(x, y, pictures[i].picture_width,pictures[i].picture_height);
    displayData(i);
}

void resizePicture(int i ,int output_width, int output_height)
{
    unsigned char *temp_data = (unsigned char *)malloc(output_width * output_height * 4);
    stbir_resize_uint8(pictures[i].data, frameBuffer.portWidth, frameBuffer.portHeight, 0, temp_data, output_width, output_height, 0, 4);
    stbi_image_free(pictures[i].data);
    pictures[i].data = temp_data;
    pictures[i].picture_width = output_width;
    pictures[i].picture_height = output_height;
}

void displayData(int i )
{
    // ������ÿ��һ���൱������һ��
    int stride = frameBuffer.windowsWidth * 4;
    int src_stride = frameBuffer.portWidth * 4;
    // ����ʼָ�룬ָ��ǰviewport ��N�� �� ��һ������
    unsigned char *rowPtr = frameBuffer.data + frameBuffer.portY * stride + frameBuffer.portX * 4;
    // Դ�ļ�bmp������������ʼָ��
    unsigned char *src_Row_Ptr = pictures[i].data;

    for (int row = 0; row < frameBuffer.portHeight; ++row)
    {
        // ������ʼָ�룬ָ�� rowPtr������ �� ��N�� ����
        unsigned char *pixPtr = rowPtr;
        unsigned char *srcPtr = src_Row_Ptr + src_stride * (frameBuffer.portHeight - 1);
        for (int col = 0; col < frameBuffer.portWidth; ++col)
        {
            // �����ظ���ɫֵ
            // ע�⣺WIN32��BITMAP����BGRA���֣�������RGBA
            //   һ������ռ4��byte��32��bit
            // pixPtr[0] = b;
            // pixPtr[1] = g;
            // pixPtr[2] = r;
            // pixPtr[3] = 255; // ͸��������
            pixPtr[0] = srcPtr[2];
            pixPtr[1] = srcPtr[1];
            pixPtr[2] = srcPtr[0];
            pixPtr[3] = 255;
            srcPtr += 4;
            pixPtr += 4; // ����һ��
        }
        rowPtr += stride; // ����һ��
        src_Row_Ptr -= src_stride;
    }

    // ������viewport���Ϊ��Ҫ���£�������Ҫ������
    {
        // ע�⣺viewportRECTʹ��Win32����ϵ��
        //   y�ᷴ��ͬʱԭ���ڴ������Ͻ�
        RECT viewportRECT = {
            frameBuffer.portX,                                                      // ��
            frameBuffer.windowsHeight - frameBuffer.portY - frameBuffer.portHeight, // ��
            frameBuffer.portX + frameBuffer.portWidth,                              // ��
            frameBuffer.windowsHeight - frameBuffer.portY                           // ��
        };
        InvalidateRect(GetDisPlayWindows(), &viewportRECT, FALSE); // FALSEע�⣡
    }
}

void clearPicture(int i )
{
    int x = picture_middle_x - pictures[i].picture_width/2;
    int y = picture_middle_y - pictures[i].picture_height/2;
    displayViewPort(x, y, pictures[i].picture_width, pictures[i].picture_height);
    displayFillWithColor(154, 154, 154);
}

void left_Rotate_Picture(int i )
{
    unsigned char *temp_data = (unsigned char *)malloc(pictures[i].picture_width * pictures[i].picture_height * 4);

    // ������ÿ��һ���൱������һ��
    int dst_stride = pictures[i].picture_height * 4;
    int src_stride = pictures[i].picture_width * 4;
    unsigned char *src_ptr;
    unsigned char *dst_ptr;

    for (int row = 0; row < pictures[i].picture_height; ++row)
    {
        for (int col = 0; col < pictures[i].picture_width; ++col)
        {
            src_ptr = data + row * src_stride + col * 4;
            dst_ptr = temp_data + dst_stride * (pictures[i].picture_width - col - 1) + row * 4;
            dst_ptr[0] = src_ptr[0];
            dst_ptr[1] = src_ptr[1];
            dst_ptr[2] = src_ptr[2];
            dst_ptr[3] = src_ptr[3];
        }
    }

    stbi_image_free(pictures[i].data);
    pictures[i].data = temp_data;
    int temp = pictures[i].picture_height;
    pictures[i].picture_height = pictures[i].picture_width;
    pictures[i].picture_width = temp;
}

void right_Rotate_Picture(int i )
{
    unsigned char *temp_data = (unsigned char *)malloc(pictures[i].picture_width * pictures[i].picture_height * 4);

    // ������ÿ��һ���൱������һ��
    int dst_stride = pictures[i].picture_height * 4;
    int src_stride = pictures[i].picture_width * 4;
    unsigned char *src_ptr;
    unsigned char *dst_ptr;

    for (int row = 0; row < pictures[i].picture_height; ++row)
    {
        for (int col = 0; col < pictures[i].picture_width; ++col)
        {
            src_ptr = pictures[i].data + row * src_stride + col * 4;
            dst_ptr = temp_data + dst_stride * col + (pictures[i].picture_height - row - 1) * 4;
            dst_ptr[0] = src_ptr[0];
            dst_ptr[1] = src_ptr[1];
            dst_ptr[2] = src_ptr[2];
            dst_ptr[3] = src_ptr[3];
        }
    }

    stbi_image_free(pictures[i].data);
    pictures[i].data = temp_data;
    int temp = pictures[i].picture_height;
    pictures[i].picture_height = pictures[i].picture_width;
    pictures[i].picture_width = temp;
}

void save_Picture(string s , int i)
{
    stbi_write_bmp(s, pictures[i].picture_width, pictures[i].picture_height, 4, pictures[i].data, 0);
}

void cut_Picture(int i , int x_downLeft , int y_downLeft , int x_upRight , int y_upRight)
{
    if (x_downLeft > x_upRight)
    {
        int temp = x_downLeft;
        x_downLeft = x_upRight;
        x_upRight = temp;
    }
    if (y_downLeft > y_upRight)
    {
        int temp = y_downLeft;
        y_downLeft = y_upRight;
        y_upRight = temp;
    }
    int output_width = x_upRight - x_downLeft;
    int output_height = y_upRight - y_downLeft;
    unsigned char *temp_data = (unsigned char *)malloc(output_width * output_height * 4);

    // ������ÿ��һ���൱������һ��
    int dst_stride = output_width * 4;
    int src_stride = pictures[i].picture_width * 4;
    unsigned char *src_ptr;
    unsigned char *dst_ptr;
    // ��Ҫ����һ��base_dataָ�� , ָ��src_data�м��еĿ�ʼλ��
    unsigned char *base_data = pictures[i].data + src_stride * (pictures[i].picture_height - y_downLeft - output_height - 1) + 4 * x_downLeft;
    for (int row = 0; row < output_height; ++row)
    {
        for (int col = 0; col < output_width; ++col)
        {
            src_ptr = base_data + row * src_stride + col * 4;
            dst_ptr = temp_data + row * dst_stride + col * 4;
            dst_ptr[0] = src_ptr[0];
            dst_ptr[1] = src_ptr[1];
            dst_ptr[2] = src_ptr[2];
            dst_ptr[3] = src_ptr[3];
        }
    }

    stbi_image_free(pictures[i].data);
    pictures[i].data = temp_data;
    pictures[i].picture_width = output_width;
    pictures[i].picture_height = output_height;
}

/**
 * @brief ����ͼƬ��ʾ������λ��x
 * 
 * @param x 
 */
void set_picture_middle_x(int x){
    picture_middle_x = x;
    printf("%d\n" , picture_middle_x);
}

/**
 * @brief ����ͼƬ��ʾ������λ��y
 * 
 * @param y 
 */
void set_picture_middle_y(int y){
    picture_middle_y = y;
    printf("%d\n" , picture_middle_x);
}


int getNextIndex(int index){
    if(index + 1 >= picture_index){
        return 0;
    }
    return index + 1;
}

int getPrevIndex(int index){
    if(index - 1 < 0){
        return picture_index - 1;
    }
    return index - 1;
}


void expandPicture(int i ,int max_width , int max_height){
    if((int)(pictures[i].picture_height*1.1) > max_height)return;
    if((int)(pictures[i].picture_width*1.1) > max_width)return;
    resizePicture(i,pictures[i].picture_width*1.1 , pictures[i].picture_height*1.1);
}

void shrinkPicture(int i ){
    resizePicture(i,pictures[i].picture_width*0.9 , pictures[i].picture_height*0.9);
}

void retrievePicture(int i){
    int n ;
    stbi_image_free(pictures[i].data);
    pictures[i].data = stbi_load(pictures[i].name, &pictures[i].picture_width, &pictures[i].picture_height, &n, 4);
}

