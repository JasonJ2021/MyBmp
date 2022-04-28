# MyBmp

一些窗口的metadata :

Width : 10 inches 

Height : 7 inches

每个像素在 x y 方向上大小大约为0.0122 inch

## BMP 文件解析







## 图形库文档

**Descrption**

​	Main()函数首先调用的函数，调用后会创建一个图形窗口

**Parameters** 

​	void

**return**

​	void

**初始化**

第三方图形库基于windows 系统，程序的入口为**Main()**函数

Libgraphics有两个库比较重要

- extgraph.h
- graphics.h

### graphics

#### InitGraphics 

> void InitGraphics(void);

**Descrption**

​	Main()函数首先调用的函数，调用后会创建一个图形窗口

**Parameters** 

​	void

**return**

​	void

#### MovePen

> void MovePen(double x, double y);

**Descrption**

​	将画笔移动到x , y 的位置（单位为英寸），只是移动不划线

**Parameters** 

​	double x : 横坐标

​	double y：纵坐标

**return**

​	void



#### DrawLine

> void DrawLine(double dx, double dy);

**Descrption**

​	绘画函数，从当前的(x , y )位置画到(x + dx , y +dy)

**Parameters** 

​	double dx : 移动的横坐标

​	double dy：移动的纵坐标

**return**

​	void



#### DrawArc

> void DrawArc(double r, double start, double sweep);

**Descrption**

​	以画笔当前位置作为圆弧所在圆x轴的右起点，画一段圆弧

**Parameters** 

​	double r : 圆弧半径

​	double start : 起始的弧度（相对x轴方向逆时针为正）

​	double sweep：画的弧度

**return**

​	void



#### GetWindowsWidth & GetWindowsHeight

> double GetWindowWidth(void);
>
> double GetWindowHeight(void);

**Descrption**

​	返回当前窗口的宽度和高度，单位为inch

**Parameters** 

​	void

**return**

​	宽度/高度



#### GetCurrentX(void) & GetCurrentY(void);

> double GetCurrentX(void);
>
> double GetCurrentY(void);

**Descrption**

​	返回当前画笔的位置，单位为inch

**Parameters** 

​	void

**return**

​	当前所在的宽度/高度

### extgraph

#### DrawTextString

> void DrawTextString(string text);

**Descrption**

​	在当前的位置显示string 文本，以目前的字体和大小显示。当前位置会根据函数执行而变动，以便于下一次调用显示在上一次的结尾位置

**Parameters** 

​	string text : 显示的文本

**return**

​	void



#### TextStringWidth

> double TextStringWidth(string text);

**Descrption**

​	计算text如果以当前的字体与大小显示会占用多少宽度

**Parameters** 

​	string text : 要显示的文本

**return**

​	double : 占用的宽度



#### HasColor

> bool HasColor(void);

**Descrption**

​	检测当前的窗口是否能支持显示有颜色的图片

**Parameters** 

​	void

**return**

​	bool( int )



#### SetPenColor

> void SetPenColor(string color);
>
> string GetPenColor(void);

**Descrption**

​	设置当前画笔的颜色，已经设置好的颜色有:

 \*   Black, Dark Gray, Gray, Light Gray, White,

 \*   Red, Yellow, Green, Cyan, Blue, Magenta

​	如果需要自己定义颜色，可以调用**DefineColor**

**Parameters** 

​	string color : 颜色的名字，例如Black 

**return**

​	void



#### SetPenSize 

> void SetPenSize(int size);
>
> int GetPenSize(void);

**Descrption**

​	设置当前画笔的大小，单位为像素

**Parameters** 

​	string color : 颜色的名字，例如Black 

**return**

​	void



#### DefineColor

> void DefineColor(string name, double red, double green, double blue);

**Descrption**

​	自定义颜色，最多可以定义256种（包括初始设置的十几种颜色） ， 颜色的参数范围在[0 , 1 ]之间

**Parameters** 

​	string name : 定义颜色的名字

​	double red , green , blue : 定义颜色的强度

**return**

​	void



#### SetWindowsTitle

> void SetWindowTitle(string title);

**Descrption**

​	设置窗口的名称

**Parameters** 

​	string title : 定义窗口的名字

**return**

​	void



#### ExitGraphics

**Description :** 

无条件退出当前窗口



####  SetWindowSize

> void SetWindowSize(double width, double height);

**Description :** 

设置当前窗口的大小



#### GetXResolution

> double GetXResolution(void);
>
> double GetYResolution(void);

这两个函数很重要 , 返回x ,  y 方向每英尺包含的像素

注意在graphics中线条总是比我们预期的多一个像素的长度

> DrawLine(2 / GetXResolution(), 0); 画一条长度为3像素的线



#### ScaleXInches

> double ScaleXInches(int x);
>
> double ScaleYInches(int y);

像素数量转换为英尺



