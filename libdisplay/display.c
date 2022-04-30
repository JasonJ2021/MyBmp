#include "display.h"
#include "../libgraphics/win32Export.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../third_party/stb_image.h"
#include "../third_party/stb_image_resize.h"
#include "../third_party/stb_image_write.h"

static HBITMAP oldBitmap = NULL, bitmap = NULL;
static int picture_width = 0;
static int picture_height = 0;

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
    int n;
    data = stbi_load(s, &picture_width, &picture_height, &n, 4);
}

void displayPicture(void)
{
    displayViewPort(0, 0, picture_width, picture_height);
    displayData();
}

void resizePicture(int output_width, int output_height)
{
    unsigned char *temp_data = (unsigned char *)malloc(output_width * output_height * 4);
    stbir_resize_uint8(data, frameBuffer.portWidth, frameBuffer.portHeight, 0, temp_data, output_width, output_height, 0, 4);
    stbi_image_free(data);
    data = temp_data;
    picture_width = output_width;
    picture_height = output_height;
}

void displayData()
{
    // ������ÿ��һ���൱������һ��
    int stride = frameBuffer.windowsWidth * 4;
    int src_stride = frameBuffer.portWidth * 4;
    // ����ʼָ�룬ָ��ǰviewport ��N�� �� ��һ������
    unsigned char *rowPtr = frameBuffer.data + frameBuffer.portY * stride + frameBuffer.portX * 4;
    // Դ�ļ�bmp������������ʼָ��
    unsigned char *src_Row_Ptr = data;

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

void clearPicture()
{
    displayViewPort(0, 0, picture_width, picture_height);
    displayFillWithColor(255, 255, 255);
}

void left_Rotate_Picture()
{
    unsigned char *temp_data = (unsigned char *)malloc(picture_width * picture_height * 4);

    // ������ÿ��һ���൱������һ��
    int dst_stride = picture_height * 4;
    int src_stride = picture_width * 4;
    unsigned char *src_ptr;
    unsigned char *dst_ptr;

    for (int row = 0; row < picture_height; ++row)
    {
        for (int col = 0; col < picture_width; ++col)
        {
            src_ptr = data + row * src_stride + col * 4;
            dst_ptr = temp_data + dst_stride * (picture_width - col - 1) + row * 4;
            dst_ptr[0] = src_ptr[0];
            dst_ptr[1] = src_ptr[1];
            dst_ptr[2] = src_ptr[2];
            dst_ptr[3] = src_ptr[3];
        }
    }

    stbi_image_free(data);
    data = temp_data;
    int temp = picture_height;
    picture_height = picture_width;
    picture_width = temp;
}

void right_Rotate_Picture()
{
    unsigned char *temp_data = (unsigned char *)malloc(picture_width * picture_height * 4);

    // ������ÿ��һ���൱������һ��
    int dst_stride = picture_height * 4;
    int src_stride = picture_width * 4;
    unsigned char *src_ptr;
    unsigned char *dst_ptr;

    for (int row = 0; row < picture_height; ++row)
    {
        for (int col = 0; col < picture_width; ++col)
        {
            src_ptr = data + row * src_stride + col * 4;
            dst_ptr = temp_data + dst_stride * col + (picture_height - row - 1) * 4;
            dst_ptr[0] = src_ptr[0];
            dst_ptr[1] = src_ptr[1];
            dst_ptr[2] = src_ptr[2];
            dst_ptr[3] = src_ptr[3];
        }
    }

    stbi_image_free(data);
    data = temp_data;
    int temp = picture_height;
    picture_height = picture_width;
    picture_width = temp;
}

void save_Picture(string s)
{
    stbi_write_bmp(s, picture_width, picture_height, 4, data, 0);
}

void cut_Picture(int x_downLeft , int y_downLeft , int x_upRight , int y_upRight)
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
    int src_stride = picture_width * 4;
    unsigned char *src_ptr;
    unsigned char *dst_ptr;
    // ��Ҫ����һ��base_dataָ�� , ָ��src_data�м��еĿ�ʼλ��
    unsigned char *base_data = data + src_stride * (picture_height - y_downLeft - output_width - 1 ) + 4 * x_downLeft;
    for (int row = 0; row < output_width; ++row)
    {
        for (int col = 0; col < output_height; ++col)
        {
            src_ptr = base_data + row * src_stride + col * 4;
            dst_ptr = temp_data + row * dst_stride + col * 4;
            dst_ptr[0] = src_ptr[0];
            dst_ptr[1] = src_ptr[1];
            dst_ptr[2] = src_ptr[2];
            dst_ptr[3] = src_ptr[3];
        }
    }

    stbi_image_free(data);
    data = temp_data;
    picture_width = output_width;
    picture_height = output_height;
}