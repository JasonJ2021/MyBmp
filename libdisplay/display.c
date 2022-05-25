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
    // 步长，每跨一步相当于往上一行
    int stride = frameBuffer.windowsWidth * 4;
    int src_stride = frameBuffer.portWidth * 4;
    // 行起始指针，指向当前viewport 第N行 的 第一个像素
    unsigned char *rowPtr = frameBuffer.data + frameBuffer.portY * stride + frameBuffer.portX * 4;
    // 源文件bmp的像素数据起始指针
    unsigned char *src_Row_Ptr = pictures[i].data;

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

    // 步长，每跨一步相当于往上一行
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

    // 步长，每跨一步相当于往上一行
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

    // 步长，每跨一步相当于往上一行
    int dst_stride = output_width * 4;
    int src_stride = pictures[i].picture_width * 4;
    unsigned char *src_ptr;
    unsigned char *dst_ptr;
    // 需要创建一个base_data指针 , 指向src_data中剪切的开始位置
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
 * @brief 设置图片显示的中心位置x
 * 
 * @param x 
 */
void set_picture_middle_x(int x){
    picture_middle_x = x;
    printf("%d\n" , picture_middle_x);
}

/**
 * @brief 设置图片显示的中心位置y
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

