#include "display.h"
#include "../libgraphics/win32Export.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../third_party/stb_image.h"
#include "../third_party/stb_image_resize.h"

static HBITMAP oldBitmap = NULL, bitmap = NULL;

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


void displayPicture(string s){
    int width , height , n ;
	data = stbi_load(s , &width , &height , &n , 4);
    displayViewPort(0,0 , width , height);
    displayData();
}


void resizePicture(int output_width , int output_height){
    unsigned char * temp_data = (unsigned char *)malloc(output_width * output_height * 4);
    stbir_resize_uint8(data , frameBuffer.portWidth , frameBuffer.portHeight,0,temp_data , output_width,output_height,0,4);
    stbi_image_free(data);
    data = temp_data;
    frameBuffer.portWidth = output_width;
    frameBuffer.portHeight = output_height;
}

void displayData(){
    // 步长，每跨一步相当于往上一行
    int stride = frameBuffer.windowsWidth * 4;
    int src_stride = frameBuffer.portWidth*4;
    // 行起始指针，指向当前viewport 第N行 的 第一个像素
    unsigned char *rowPtr = frameBuffer.data + frameBuffer.portY * stride + frameBuffer.portX * 4;
    // 源文件bmp的像素数据起始指针
    unsigned char *src_Row_Ptr = data;

    for (int row = 0; row < frameBuffer.portHeight; ++row)
    {
        // 像素起始指针，指向 rowPtr所在行 的 第N个 像素
        unsigned char *pixPtr = rowPtr;
        unsigned char *srcPtr = src_Row_Ptr + src_stride * (frameBuffer.portHeight - 1 );
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


void clearPicture(){
    displayFillWithColor(255,255,255);
}