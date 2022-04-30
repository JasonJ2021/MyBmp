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
    int windowsWidth, windowsHeight; // 窗口大小
    int portX, portY;                // 视图位置，距离左下角
    int portWidth, portHeight;       // 视图大小
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
                                  32, // 一个像素占4个byte，32个bit
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
    // 检查图片是否超出窗口
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
    // 步长，每跨一步相当于往上一行
    int stride = frameBuffer.windowsWidth * 4;
    // 行起始指针，指向当前viewport 第N行 的 第一个像素
    unsigned char *rowPtr = frameBuffer.data + frameBuffer.portY * stride + frameBuffer.portX * 4;
    for (int row = 0; row < frameBuffer.portHeight; ++row)
    {
        // 像素起始指针，指向 rowPtr所在行 的 第N个 像素
        unsigned char *pixPtr = rowPtr;
        for (int col = 0; col < frameBuffer.portWidth; ++col)
        {
            // 逐像素赋颜色值
            // 注意：WIN32的BITMAP采用BGRA布局，而不是RGBA
            //   一个像素占4个byte，32个bit
            pixPtr[0] = b;
            pixPtr[1] = g;
            pixPtr[2] = r;
            pixPtr[3] = 255; // 透明度满格

            pixPtr += 4; // 往右一列
        }
        rowPtr += stride; // 往上一行
    }

    // 将整个viewport标记为需要更新（不是需要擦除）
    {
        // 注意：viewportRECT使用Win32坐标系，
        //   y轴反向，同时原点在窗口左上角
        RECT viewportRECT = {
            frameBuffer.portX,                                                      // 左
            frameBuffer.windowsHeight - frameBuffer.portY - frameBuffer.portHeight, // 上
            frameBuffer.portX + frameBuffer.portWidth,                              // 右
            frameBuffer.windowsHeight - frameBuffer.portY                           // 下
        };
        InvalidateRect(GetDisPlayWindows(), &viewportRECT, FALSE); // FALSE注意！
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
    // 步长，每跨一步相当于往上一行
    int stride = frameBuffer.windowsWidth * 4;
    int src_stride = frameBuffer.portWidth * 4;
    // 行起始指针，指向当前viewport 第N行 的 第一个像素
    unsigned char *rowPtr = frameBuffer.data + frameBuffer.portY * stride + frameBuffer.portX * 4;
    // 源文件bmp的像素数据起始指针
    unsigned char *src_Row_Ptr = data;

    for (int row = 0; row < frameBuffer.portHeight; ++row)
    {
        // 像素起始指针，指向 rowPtr所在行 的 第N个 像素
        unsigned char *pixPtr = rowPtr;
        unsigned char *srcPtr = src_Row_Ptr + src_stride * (frameBuffer.portHeight - 1);
        for (int col = 0; col < frameBuffer.portWidth; ++col)
        {
            // 逐像素赋颜色值
            // 注意：WIN32的BITMAP采用BGRA布局，而不是RGBA
            //   一个像素占4个byte，32个bit
            // pixPtr[0] = b;
            // pixPtr[1] = g;
            // pixPtr[2] = r;
            // pixPtr[3] = 255; // 透明度满格
            pixPtr[0] = srcPtr[2];
            pixPtr[1] = srcPtr[1];
            pixPtr[2] = srcPtr[0];
            pixPtr[3] = 255;
            srcPtr += 4;
            pixPtr += 4; // 往右一列
        }
        rowPtr += stride; // 往上一行
        src_Row_Ptr -= src_stride;
    }

    // 将整个viewport标记为需要更新（不是需要擦除）
    {
        // 注意：viewportRECT使用Win32坐标系，
        //   y轴反向，同时原点在窗口左上角
        RECT viewportRECT = {
            frameBuffer.portX,                                                      // 左
            frameBuffer.windowsHeight - frameBuffer.portY - frameBuffer.portHeight, // 上
            frameBuffer.portX + frameBuffer.portWidth,                              // 右
            frameBuffer.windowsHeight - frameBuffer.portY                           // 下
        };
        InvalidateRect(GetDisPlayWindows(), &viewportRECT, FALSE); // FALSE注意！
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

    // 步长，每跨一步相当于往上一行
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

    // 步长，每跨一步相当于往上一行
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

    // 步长，每跨一步相当于往上一行
    int dst_stride = output_width * 4;
    int src_stride = picture_width * 4;
    unsigned char *src_ptr;
    unsigned char *dst_ptr;
    // 需要创建一个base_data指针 , 指向src_data中剪切的开始位置
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