#include <string.h>
#include <math.h>

#include "libgraphics/extgraph.h"
#include "libdisplay/display.h"
#include "libgraphics/imgui.h"
#include "third_party/dirent.h"
#include "libgraphics/win32Export.h"
#include "libdisplay/zip.h"

static double WinWidth, WinHeight; // 窗口尺寸
static int ShowGUI = 0;			   //切换GUI界面
static int ShowAbout = 0;
static int ShowUseWay = 0;
static int ShowPictureRegion = 1;
static int ShowMoreButtons = 0; // 显示更多按钮
static int ShowFilesList = 0;
static int ShowCropBox = 0;
static int ShowSaveAnother = 0; // 另存为
static char BasePatch[100] = "../resource";
static char FileName[20][100] = {""};
static int ChangeGUI = 0;
static double MouseX = 0;
static double MouseY = 0;
static int CurIndex = 0;	  // 当前显示图片的Index
static int ShouldDisplay = 0; // 当前是否应该显示图片
static int MaxWidth = 0;	  // 显示框宽度
static int MaxHeight = 0;	  // 显示框高度

static char *selectedLabel1 = NULL;
static char *selectedLabel2 = NULL;

// 清屏函数，provided in libgraphics
void DisplayClear(void);

// 用户的显示函数
void Display(void);

// 图片显示函数
void ShowBMP(void);

// 开始界面
void StartGUI(void);

// 帮助界面
void ShowHelp(void);

// 搜素文件夹中的文件
void SearchFiles(char *BasePath);

// 显示另存为界面
void DrawShowSaveAnother();

// 显示BMP图片显示界面
void DrawShowPictureRegion();

// 显示裁剪界面
void DrawShowCropBox();

// 显示文件列表
void DrawShowFilesList(void);

// 显示修改打开路径的文本框
void DrawEditText(void);

// 显示帮助菜单的关于界面
void DrawShowAbout(void);

// 显示帮助界面的使用方法界面
void DrawShowUseWay(void);

// 用户的字符事件响应函数
void CharEventProcess(char ch)
{
	uiGetChar(ch); // GUI字符输入
	Display();	   //刷新显示
}

// 用户的键盘事件响应函数
void KeyboardEventProcess(int key, int event)
{
	uiGetKeyboard(key, event); // GUI获取键盘
	Display();				   // 刷新显示
}

// 用户的鼠标事件响应函数
void MouseEventProcess(int x, int y, int button, int event)
{
	uiGetMouse(x, y, button, event); // GUI获取鼠标
	MouseX = ScaleXInches(x);
	MouseY = ScaleYInches(y);
	Display(); // 刷新显示
}

// 主函数
void Main()
{
	// 打开控制台，方便用printf/scanf输出/入变量信息，方便调试
	//InitConsole();
	//关闭控制台，仅在VS Code环境下使用，若为VS2019或VS2017环境，请注释掉下面这行代码
	// FreeConsole();

	// 初始化窗口
	SetWindowTitle("BMP图片显示器");
	// SetWindowSize(13.39, 7.48);  // 如果屏幕尺寸不够，则按比例缩小
	//  获取全屏窗口尺寸
	double fullwidth = GetFullScreenWidth();
	double fullheight = GetFullScreenHeight();
	// 设置窗口尺寸
	SetWindowSize(fullwidth, fullheight);
	printf("尺寸大小：%.2f %.2f\n", fullwidth, fullheight);
	// 初始化图形系统
	InitGraphics();

	// 获得窗口尺寸
	WinWidth = GetWindowWidth();
	WinHeight = GetWindowHeight();
	printf("窗口尺寸：%.2f %.2f\n", WinWidth, WinHeight);
	DisplayViewPort(0, 0, DisplayGetWindowPixelWidth(), DisplayGetWindowPixelHeight());
	DisplayFillWithColor(255, 255, 255);
	// 自定义颜色
	DefineColor("GrayBlue", 0.57, 0.71, 0.83);

	// 注册时间响应函数
	registerCharEvent(CharEventProcess);		 // 字符
	registerKeyboardEvent(KeyboardEventProcess); // 键盘
	registerMouseEvent(MouseEventProcess);		 // 鼠标

	//获取文件信息
	SearchFiles(BasePatch);

	//============================================== DEBUG DISPLAY
	// 设置全屏
	// WinWidth = GetFullScreenWidth();
	// WinHeight = GetFullScreenWidth();
	// SetWindowSize(WinWidth, WinHeight);
	// int x , y , n ;
	// unsigned char *data = stbi_load(".../resource/1.bmp" , &x , &y , &n , 4);
	// int size = sizeof(data);
	// InitConsole();
	// InitGraphics();
	// SetWindowTitle("MyBmp");
	// // ========================GUI PART==================================
	// //自定义颜色
	// DefineColor("GrayBlue", 0.57, 0.71, 0.83);

	// // 注册时间响应函数
	// registerCharEvent(CharEventProcess);         // 字符
	// registerKeyboardEvent(KeyboardEventProcess); // 键盘
	// registerMouseEvent(MouseEventProcess);       // 鼠标

	// setMenuColors("GrayBlue", "Black", "Dark Gray", "White", 1);
	// setButtonColors("GrayBlue", "Black", "Dark Gray", "White", 1);
	// setTextBoxColors("GrayBlue", "Black", "Dark Gray", "White", 1);

	// =================================================================================
	// char *s = (char *)malloc(100);
	// int width = displayGetWindowPixelWidth();
	// int height = displayGetWindowPixelHeight();
	// sprintf(s , "x = %d , y = %d , n = %d size = %d width = %d , height = %d" , x , y, n , size , width , height);
	// MovePen(1,1);
	// DrawTextString(s);
	// registerTimerEvent(timerCallback);
	// 15ms为周期，近似于60FPS
	// // startTimer(0, 15);
	// DisplayViewPort(0,0,displayGetWindowPixelWidth(),displayGetWindowPixelHeight());

	// // 初始化窗口设置背景颜色为白色
	// DisplayFillWithColor(255,255,255);
	// printf("win_width = %d , win_height = %d\n" , WinWidth , WinHeight);
	// SetPictureMiddleX(inchXToPixelX(WinWidth/2));
	// SetPictureMiddleY(inchYToPixelY(WinHeight/2));
	// ReadInPicture("../resource/2.bmp");
	// DisplayPicture(0);
	// ClearPicture(0);
	// ReadInPicture("../resource/2.bmp");
	// RightRotatePicture(0);
	// RightRotatePicture();
	// ReadInPicture("../resource/1.bmp");
	// DisplayPicture(0);
	// ClearPicture(0);
	// DisplayPicture(1);
	// ClearPicture(1);
	//==============================cur part ==========================//
	// CutPicture(0 , 0,0,200,200);
	// DisplayPicture(0);

	//==============================save part =========================//
	// SavePicture("../resource/saved.bmp");
	// DisplayPicture("../resource/2.bmp");

	//==============================resize part =======================//
	// DisplayPicture("../resource/2.bmp");
	// ClearPicture();
	// ResizePicture(500 ,500);
	// DisplayPicture();
	// ClearPicture();
	// ResizePicture(100 ,100);
	// DisplayData();
}

//显示函数
void Display()
{
	// 清屏
	DisplayClear();

	switch (ShowGUI)
	{
	case 0: //开始界面
		StartGUI();
		break;
	case 1: //图片显示界面
		ShowBMP();
		break;
	case 2: //软件帮助界面
		ShowHelp();
		break;
	default:
		break;
	}
}

// 图片显示界面
void ShowBMP()
{
	static char *menuListFile[] = {"文件",
								   "打开 | Ctrl-O",
								   "关闭 | Ctrl-P",
								   "保存 | Ctrl-S",
								   "另存 | Ctrl-Q",
								   "退出 | Ctrl-E"};
	static char *menuListTool[] = {
		"工具",
		"放大 | Ctrl-A",
		"缩小 | Ctrl-B",
		"左旋 | Ctrl-X",
		"右旋 | Ctrl-Y",
		"复位 | Ctrl-R",
		"裁剪 | Ctrl-T",
		"压缩 | Ctrl-Z",
	};
	static char *menuListHelp[] = {"帮助",
								   "使用方法 | Ctrl-U",
								   "关于"};



	double fontHeight = GetFontHeight(); //字高
	double x = 0;
	double y = WinHeight;
	double h = fontHeight * 1.5;						   //按钮高度
	double w = TextStringWidth(menuListHelp[0]) * 3.8;	   //一级菜单条选项宽度
	double wlist = TextStringWidth(menuListTool[3]) * 1.5; //二级菜单选项条宽度
	double xindent = WinHeight / 40;
	static int selection = 0;

	//显示菜单条
	setMenuColors("GrayBlue", "Black", "Dark Gray", "White", 1); //设置菜单条颜色为GrayBlue
	SetPenSize(2);
	drawMenuBar(0, y - h, WinWidth, h);

	//显示文件菜单
	SetPenSize(2);
	setMenuColors("GrayBlue", "Black", "Dark Gray", "White", 1); // 设置菜单框颜色为Black，label颜色为Black
	selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListFile, sizeof(menuListFile) / sizeof(menuListFile[0]));
	if (selection > 0)
	{
		selectedLabel1 = menuListFile[0];
		selectedLabel2 = menuListFile[selection];
	}
	if (selection == 1) // 打开
	{
		ShowFilesList = 1;
		ShowCropBox = 0;

	}
	if (selection == 2) // 关闭
	{
		ShowFilesList = 0;
	}
	if (selection == 3) // 保存
	{
		if (ShouldDisplay)
		{
			SavePicture(pictures[CurIndex].name, CurIndex);
		}
	}
	if (selection == 4) // 另存为
	{
		ShowSaveAnother = 1;
		ShowCropBox = 0;

	}
	if (selection == 5) // 选择退出
	{
		ExitGraphics();
	}

	// 工具菜单
	selection = menuList(GenUIID(0), x + w, y - h, w, wlist, h, menuListTool, sizeof(menuListTool) / sizeof(menuListTool[0]));
	if (selection > 0)
	{
		selectedLabel1 = menuListTool[0];
		selectedLabel2 = menuListTool[selection];
	}

	// 放大
	if (selection == 1)
	{
		if (ShouldDisplay)
		{
			ExpandPicture(CurIndex, MaxWidth, MaxHeight);
		}
	}

	// 缩小
	if (selection == 2)
	{
		if (ShouldDisplay)
		{
			ShrinkPicture(CurIndex);
		}
	}

	// 逆时针旋转
	if (selection == 3)
	{
		if (ShouldDisplay)
		{
			LeftRotatePicture(CurIndex);
		}
	}

	// 顺时针旋转
	if (selection == 4)
	{
		if (ShouldDisplay)
		{
			RightRotatePicture(CurIndex);
		}
	}
	// 复位
	if (selection == 5)
	{
		if (ShouldDisplay)
		{
			RetrievePicture(CurIndex);
		}
	}

	// 裁剪
	if (selection == 6)
	{
		if (ShouldDisplay)
		{
			ShowCropBox = 1;
			ShowFilesList = 0;
			ShowSaveAnother = 0;

		}
	}
	// 保存并且压缩
	if (selection == 7)
	{
		if (ShouldDisplay)
		{
			SavePicture(pictures[CurIndex].name, CurIndex);
			char compressed_name[200];
			memset(compressed_name, 0, 200);
			strncpy(compressed_name, pictures[CurIndex].name, 128);
			strcat(compressed_name, ".huf");
			compress(pictures[CurIndex].name, compressed_name);
		}
	}

	// 帮助菜单
	selection = menuList(GenUIID(0), x + 2 * w, y - h, w, wlist, h, menuListHelp, sizeof(menuListHelp) / sizeof(menuListHelp[0]));
	if (selection > 0)
	{
		selectedLabel1 = menuListHelp[0];
		selectedLabel2 = menuListHelp[selection];
		ChangeGUI = 1;
		ShowGUI = 2;
	}
	if (selection == 1)
	{
		ShowUseWay = 1;
		ShowAbout = 0;
	}
	if (selection == 2)
	{
		ShowUseWay = 0;
		ShowAbout = 1;
	}

	if (ShowFilesList)
	{
		DrawShowFilesList();
	}
	if (ShowSaveAnother)
	{
		DrawShowSaveAnother();
	}
	if (ShowPictureRegion)
	{
		DrawShowPictureRegion();
	}
	if (ShowCropBox)
	{
		DrawShowCropBox();
	}

	if (ShowUseWay)
	{
		DrawShowUseWay();
	}
	if (ShowAbout)
	{
		DrawShowAbout();
	}

	// 返回按钮
	SetPenSize(1);
	if (button(GenUIID(0), 0, fontHeight * 4, WinWidth / 12, fontHeight * 2.5, "返回"))
	{
		ShowAbout = 0;
		ShowUseWay = 0;
		printf("%s %s", selectedLabel1, selectedLabel2);
		ShowGUI = 0;
		
		selectedLabel1 = "返回";
		selectedLabel2 = NULL;
	}

	// 显示菜单上一步操作
	drawMenuBar(0, 0, WinWidth, fontHeight * 3);
	SetPenColor("black");
	SetPointSize(2);
	drawLabel(0, fontHeight * 2, "上一步菜单操作为: ");
	SetPenColor("Red");
	drawLabel(0, fontHeight * 0.4, selectedLabel1);
	drawLabel(TextStringWidth(menuListHelp[0]) * 2, fontHeight * 0.4, selectedLabel2);
}

void StartGUI()
{

	static char *menuListHelp[] = {
		"帮助",
		"使用方法",
		"关于",
	};

	double fontHeight = GetFontHeight();				   //字高
	double h = fontHeight * 2.5;						   //按钮高度
	double w = WinWidth / 6;							   //按钮宽度
	double x = WinWidth / 2 - w;						   // 按钮基准坐标
	double y = WinHeight / 2 - fontHeight;				   // 按钮基准坐标
	double w1 = TextStringWidth(menuListHelp[0]) * 1.5;	   //一级菜单条选项宽度
	double wlist = TextStringWidth(menuListHelp[1]) * 1.5; //二级菜单选项条宽度
	double h1 = fontHeight * 1.5;						   //菜单选项高度
	static int selection = 0;							   //菜单选项

	setMenuColors("GrayBlue", "Black", "Dark Gray", "White", 1); //设置菜单条颜色为GrayBlue
	SetPenSize(1);
	drawMenuBar(0, WinHeight - h1, WinWidth, h1); //菜单栏

	selection = menuList(GenUIID(0), 0, WinHeight - h1, w1, wlist, h1, menuListHelp, sizeof(menuListHelp) / sizeof(menuListHelp[0]));

	if (selection > 0)
	{
		selectedLabel1 = menuListHelp[0];
		selectedLabel2 = menuListHelp[selection];
		ChangeGUI = 0;
	}
	if (selection == 1)
	{
		ShowUseWay = 1;
		ShowAbout = 0;
		ShowGUI = 2;
	}
	if (selection == 2)
	{
		ShowUseWay = 0;
		ShowAbout = 1;
		ShowGUI = 2;
	}
	if (ShowUseWay)
	{
		DrawShowUseWay();
	}
	if (ShowAbout)
	{
		DrawShowAbout();
	}

	SetPenSize(1);
	setButtonColors("GrayBlue", "Black", "Dark Gray", "White", 1); // 设置按钮颜色

	DrawEditText();
	if (button(GenUIID(0), x, y + h * 1.5, w, h, ShowMoreButtons ? "收起文件" : "展示文件"))
	{
		if (ShowMoreButtons = !ShowMoreButtons)
		{
			selectedLabel1 = "展示文件";
			selectedLabel2 = NULL;
		}
		else
		{
			selectedLabel1 = "收起文件";
			selectedLabel2 = NULL;
		}

	}

	if (ShowMoreButtons)
	{
		int k = 0;
	
		for (k = 0; k < 12; k++)
		{
			char name[128];
			strcpy(name, FileName[k]);
			if (button(GenUIID(k), x + w * 1.1, (y + h * 1.8) - k * h * 0.6, w * 0.8, h * 0.5, name))
			{
				selectedLabel1 = FileName[k];
				selectedLabel2 = NULL;
				ShowGUI = 1;

				char name_full[156];
				strcpy(name_full, BasePatch);
				strcat(name_full, "/");
				strcat(name_full, name);
				char *temp;
				// 需要判断是否含有后缀.huf
				if ((temp = strstr(name_full, ".huf")) != NULL)
				{
					// 含有后缀.huf
					// 直接解压
					char uncompressed_name[156];
					memset(uncompressed_name, 0, 156);
					strncpy(uncompressed_name, name_full, temp - name_full);
					printf("file name = %s\n", uncompressed_name);
					uncompress(name_full, uncompressed_name);
				}
				else
				{
					ReadInPicture(name_full);
					int i = -1;
					if ((i = FindIndex(name_full)) >= 0)
					{
						CurIndex = i;
					}
					ShouldDisplay = 1;
				}
				// ../resource/2.bmp

				printf("button %s %s\n", name, FileName[k]);
			}
		}
	}
	if (button(GenUIID(0), x, y, w, h, "进入显示图片界面"))
	{
		selectedLabel1 = "进入显示图片界面";
		selectedLabel2 = NULL;
		ShowGUI = 1;
	}
	if (button(GenUIID(0), x, y - h * 1.5, w, h, "退出"))
		ExitGraphics();

	// 显示菜单上一步操作
	drawMenuBar(0, 0, WinWidth, fontHeight * 3);
	SetPenColor("black");
	SetPointSize(2);
	drawLabel(0, fontHeight * 2, "上一步菜单操作为: ");
	SetPenColor("Red");
	drawLabel(0, fontHeight * 0.4, selectedLabel1);
	drawLabel(TextStringWidth(menuListHelp[0]) * 2, fontHeight * 0.4, selectedLabel2);
}

void DrawEditText(void)
{
	double fontHeight = GetFontHeight();
	double h = fontHeight * 2.5; // 文本编辑框高度
	double w = WinWidth / 6;	 // 文本编辑框宽度
	double x = WinWidth / 2 - w; // x坐标基准
	double y = WinHeight / 2 - fontHeight + h * 3;

	static char temp[100] = "../resource";

	static char results[200] = "";

	SetPenColor("Brown");
	drawLabel(x - fontHeight / 2 - TextStringWidth("bmp文件路径: "), y + fontHeight * 0.7, "bmp文件路径: ");

	if (button(GenUIID(0), x + w * 1.3, y, w * 0.5, h, "确定修改"))
	{
		for (int i = 0; i < 12; i++)
		{
			strcpy(FileName[i], " ");
		}
		strcpy(BasePatch, temp);
		sprintf(results, "路径被更改为： %s", BasePatch);
		SearchFiles(BasePatch);
		selectedLabel1 = "确定修改";
		selectedLabel2 = NULL;
	}

	setTextBoxColors("GrayBlue", "Black", "Dark Gray", "White", 1); // 设置文本编辑框颜色
	if (textbox(GenUIID(0), x, y, w, h, temp, sizeof(temp)))
	{

		// printf("%s\n", BasePatch);
	}

	SetPenColor("Black");
	drawLabel(x - TextStringWidth("路径被更改为："), y + h * 1.2, results);
}

void ShowHelp()
{
	double fontHeight = GetFontHeight(); //字高
	double w = WinWidth / 12;			 //返回按钮宽度
	double h = fontHeight * 2.5;		 //返回按钮高度
	int selection;						 //菜单选项
	static char *menuListHelp[] = {
		"帮助",
		"使用方法",
		"关于",
	};

	drawMenuBar(0, WinHeight - fontHeight * 1.5, WinWidth, fontHeight * 1.5); //帮助菜单条
	selection = menuList(GenUIID(0), 0, WinHeight - fontHeight * 1.5, TextStringWidth(menuListHelp[0]) * 1.5,
						 TextStringWidth(menuListHelp[1]) * 1.2, fontHeight * 1.5, menuListHelp, sizeof(menuListHelp) / sizeof(menuListHelp[0]));
	if (selection > 0)
	{
		selectedLabel1 = menuListHelp[0];
		selectedLabel2 = menuListHelp[selection];
	}
	if (selection == 1)
	{
		ShowUseWay = 1;
		ShowAbout = 0;
		ShowGUI = 2;
	}
	if (selection == 2)
	{
		ShowUseWay = 0;
		ShowAbout = 1;
		ShowGUI = 2;
	}
	if (ShowUseWay)
	{
		DrawShowUseWay();
	}
	if (ShowAbout)
	{
		DrawShowAbout();
	}

	// 返回按钮
	if (button(GenUIID(0), 0, fontHeight * 4, w, h, "返回"))
	{
		ShowAbout = 0;
		ShowUseWay = 0;
		ShowGUI = ChangeGUI;
		selectedLabel1 = "返回";
		selectedLabel2 = NULL;
	}

	// 显示菜单上一步操作
	drawMenuBar(0, 0, WinWidth, fontHeight * 3);
	SetPenColor("black");
	SetPointSize(2);
	drawLabel(0, fontHeight * 2, "上一步菜单操作为: ");
	SetPenColor("Red");
	drawLabel(0, fontHeight * 0.4, selectedLabel1);
	drawLabel(TextStringWidth(menuListHelp[0]) * 2, fontHeight * 0.4, selectedLabel2);
}

void DrawShowSaveAnother() // 另存为文件名编辑框
{
	static char temp[100] = "相对路径";
	double fontHeight = GetFontHeight();
	double x = TextStringWidth("另存 | Ctrl-Q") * 1.6;
	double w = TextStringWidth("确定") * 1.2;
	double y = WinHeight - fontHeight * 14; // 坐标y基准
	SetPenSize(1);
	SetPenColor("Blue");
	drawLabel(x, y, "另存为:  ");

	textbox(GenUIID(0), x, y -= fontHeight * 2, x * 0.8, fontHeight * 1.2, temp, sizeof(temp));

	SetPenSize(1);
	if (button(GenUIID(0), x * 1.3, y -= fontHeight * 2, w, fontHeight * 1.5, "确定"))
	{
		// 新建的文件名 = temp;
		ShowSaveAnother = 0;
		if (ShouldDisplay)
		{
			SavePicture(temp, CurIndex);
		}
	}
}

void DrawShowCropBox()
{
	SetPenSize(1);
	double fontHeight = GetFontHeight();	//字高
	double h = fontHeight * 1.2;			//文字框高度
	double w = TextStringWidth("0.55") * 3; //文本框宽度
	double x = ScaleXInches(3);				//基准坐标x
	double y = WinHeight - fontHeight * 10; //基准坐标y
	double dx = TextStringWidth("x1:") * 1.2;
	static char x1[10] = "0.0";
	static char x2[10] = "0.5";
	static char y1[10] = "0.0";
	static char y2[10] = "0.5";

	drawLabel(x, y, "请输入裁剪尺寸: ");

	drawLabel(x, y -= h * 1.5, "x1:");
	drawLabel(x, y -= h * 1.5, "x2:");
	drawLabel(x, y -= h * 1.5, "y1:");
	drawLabel(x, y -= h * 1.5, "y2:");
	if (button(GenUIID(0), x + TextStringWidth("请输入裁剪尺寸: "), y, w / 2, fontHeight * 1.5, "确定"))
	{
		// 赋值
		// printf("x1=%s x2=%s y1=%s y2=%s\n", x1, x2, y1, y2);
		printf("width = %d , height = %d\n", pictures[CurIndex].picture_width, pictures[CurIndex].picture_height);
		int x1_d = atof(x1) * pictures[CurIndex].picture_width;
		int y1_d = atof(y1) * pictures[CurIndex].picture_height;
		int x2_d = atof(x2) * pictures[CurIndex].picture_width;
		int y2_d = atof(y2) * pictures[CurIndex].picture_height;

		// 复位
		strcpy(x1, "0.0");
		strcpy(x2, "0.5");
		strcpy(y1, "0.0");
		strcpy(y2, "0.5");

		printf("x1=%d y1=%d x2=%d y2=%d\n", x1_d, y1_d, x2_d, y2_d);
		if (ShouldDisplay)
		{
			CutPicture(CurIndex, x1_d, y1_d, x2_d, y2_d);
		}
		ShowCropBox = 0;
	}

	y = WinHeight - fontHeight * 10;
	textbox(GenUIID(0), x + dx, y -= h * 1.5, w, h, x1, sizeof(x1));
	textbox(GenUIID(0), x + dx, y -= h * 1.5, w, h, x2, sizeof(x2));
	textbox(GenUIID(0), x + dx, y -= h * 1.5, w, h, y1, sizeof(y1));
	textbox(GenUIID(0), x + dx, y -= h * 1.5, w, h, y2, sizeof(y2));
}

void DrawShowPictureRegion()
{
	// 图片显示区域
	double fontHeight = GetFontHeight();
	double bmpx = TextStringWidth("帮助") * 7.5;
	double bmpy = fontHeight * 5.5;
	double bmpw = WinWidth - bmpx;
	double bmph = WinHeight - fontHeight * 10;
	double buttonw = WinWidth / 15;
	double buttonh = fontHeight * 2.3;
	SetPenSize(2);

	if (pictures[CurIndex].name)
	{
		SetPenColor("Black");
		drawLabel(WinWidth - bmpw / 1.5, bmpy + bmph + fontHeight * 0.8, "正在显示的图片是: ");
		SetPenColor("Blue");
		drawLabel(WinWidth - bmpw / 1.5 + TextStringWidth("正在显示的图片是: "), bmpy + bmph + fontHeight * 0.8, pictures[CurIndex].name);
	}
	SetPenColor("Gray");
	drawBox(bmpx, bmpy, bmpw, bmph, 1, "bmp图片显示区域", 'C', "Red");
	printf("%d\n", GetPenSize());
	// 设置图片最大高度、宽度
	MaxHeight = inchXToPixelX(bmph);
	MaxWidth = inchYToPixelY(bmpw);
	// 设置图片显示的中心位置
	SetPictureMiddleX(inchXToPixelX(bmpx + bmpw / 2));
	SetPictureMiddleY(inchYToPixelY(bmpy + bmph / 2));
	// 如果需要显示图片，在这里显示图片！
	if (ShouldDisplay)
	{
		DisplayPicture(CurIndex);
	}
	// 快捷键小图标
	// 上一张
	SetPenSize(1);
	if (button(GenUIID(0), bmpx, fontHeight * 3.1, buttonw, buttonh, ""))
	{
		// 切换显示上一张图片
		CurIndex = GetPrevIndex(CurIndex);
		selectedLabel1 = "切换上一张";
		selectedLabel2 = NULL;
	}
	SetPenColor("Black");
	MovePen(bmpx + buttonw * 0.2, fontHeight * 3 + buttonh / 2);
	DrawLine(buttonw * 0.5, -buttonh * 0.2);
	DrawLine(0, buttonh * 0.5);
	DrawLine(-buttonw * 0.5, -buttonh * 0.3);

	// 下一张
	if (button(GenUIID(0), WinWidth - buttonw, fontHeight * 3.1, buttonw, buttonh, ""))
	{
		// 切换显示下一张图片
		CurIndex = GetNextIndex(CurIndex);
		selectedLabel1 = "切换下一张";
		selectedLabel2 = NULL;
	}
	MovePen(WinWidth - buttonw * 0.2, fontHeight * 3 + buttonh / 2);
	DrawLine(-buttonw * 0.5, -buttonh * 0.2);
	DrawLine(0, buttonh * 0.5);
	DrawLine(buttonw * 0.5, -buttonh * 0.3);

	// 缩小图片
	if (button(GenUIID(0), WinWidth - buttonw * 4, fontHeight * 3.1, buttonw, buttonh, ""))
	{
		// 缩小图片
		ShrinkPicture(CurIndex);
		selectedLabel1 = "缩小";
		selectedLabel2 = NULL;
	}
	MovePen(WinWidth - buttonw * 4 + buttonw * 0.4, fontHeight * 3.1 + buttonh / 2);
	SetPenSize(2);
	DrawLine(buttonw * 0.2, 0);

	// 放大图片
	SetPenSize(1);
	if (button(GenUIID(0), bmpx + buttonw * 4, fontHeight * 3.1, buttonw, buttonh, ""))
	{
		// 放大图片
		ExpandPicture(CurIndex, MaxWidth, MaxHeight);
		selectedLabel1 = "放大";
		selectedLabel2 = NULL;
	}
	MovePen(bmpx + buttonw * 4 + buttonw * 0.35, fontHeight * 3.1 + buttonh / 2);
	SetPenSize(2);
	DrawLine(buttonw * 0.3, 0);
	MovePen(bmpx + buttonw * 4 + buttonw / 2, fontHeight * 3.1 + buttonh * 0.3);
	DrawLine(0, buttonh * 0.4);
}

void DrawShowFilesList()
{
	SearchFiles(BasePatch);
	SetPenSize(1);
	double fontHeight = GetFontHeight();	//字高
	double h = fontHeight * 1.2;			//按钮高度
	double w = WinWidth / 11;				//按钮宽度
	double x = ScaleXInches(3);				//基准坐标x
	double y = WinHeight - fontHeight * 12; //基准坐标y
	SetPenColor("Blue");
	drawLabel(x, y, "文件列表： ");

	for (int k = 0; k < 12; k++)
	{
		char name[128];
		strcpy(name, FileName[k]);
		if (button(GenUIID(k), x, y - fontHeight * 2 - k * h * 1.2, w, h, name))
		{
			selectedLabel1 = FileName[k];
			selectedLabel2 = NULL;

			char name_full[156];
			strcpy(name_full, BasePatch);
			strcat(name_full, "/");
			strcat(name_full, name);
			char *temp;
			// 需要判断是否含有后缀.huf
			if ((temp = strstr(name_full, ".huf")) != NULL)
			{
				// 含有后缀.huf
				// 直接解压
				char uncompressed_name[156];
				memset(uncompressed_name, 0, 156);
				strncpy(uncompressed_name, name_full, temp - name_full);
				printf("file name = %s\n", uncompressed_name);
				uncompress(name_full, uncompressed_name);
				ReadInPicture(uncompressed_name);
				int i = -1;
				if ((i = FindIndex(name_full)) >= 0)
				{
					CurIndex = i;
				}
				ShouldDisplay = 1;
			}
			else
			{
				ReadInPicture(name_full);
				int i = -1;
				if ((i = FindIndex(name_full)) >= 0)
				{
					CurIndex = i;
				}
				ShouldDisplay = 1;
			}
			// ../resource/2.bmp
		}
		printf("button %s %s\n", name, FileName[k]);
	}
}

void DrawShowUseWay()
{
	double fontHeight = GetFontHeight() * 1.2; //字高
	double x = WinWidth / 10;
	double y = WinHeight - WinHeight / 6;

	SetPenColor("Black");
	drawLabel(x, y, "注意事项1：默认bmp文件路径为../resource/；若需修改，在文本框中修改后，点击右边【确认修改】按钮。");
	drawLabel(x, y -= fontHeight * 1.5, "注意事项2： 由于显示页面限制，文件夹中bmp文件数目不能大于10。");
	drawLabel(x, y -= fontHeight * 1.5, "注意事项3：文件夹相对路径名/文件名请用英文字符。 ");
	drawLabel(x, y -= fontHeight * 1.5, "注意事项4： ");


}

void DrawShowAbout()
{
	double fontHeight = GetFontHeight() * 1.2; //字高
	double x = WinWidth / 10;
	double y = WinHeight - WinHeight / 6;
	SetPenColor("Black");
	drawLabel(x, y -= fontHeight * 1.5, "本软件名称为BMP图片显示器，为2022年浙江大学程序设计专题课程大作业。");
	drawLabel(x, y -= fontHeight * 1.5, "本软件可以实现对BMP格式图片的放大/缩小/左旋/右旋/复位/裁剪/压缩/解压缩等功能。");

}

void SearchFiles(char *BasePath)
{
	char path[200]; // 路径
	struct dirent *dp;
	DIR *dir = opendir(BasePath);
	int i = 0;
	// 如果是文件则直接结束此次调用
	if (!dir)
	{
		return;
	}

	while ((dp = readdir(dir)) != NULL)
	{
		// 跳过 "." 和 ”..“
		if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
		{
			// 将文件名和路径进行拼接形成一个完整路径
			// printf("%s\n", dp->d_name);
			strcpy(FileName[i], dp->d_name);
			// printf("%s\n", FileName[i]);
			i++;
			strcpy(path, BasePath);
			strcat(path, "/");
			strcat(path, dp->d_name);
			SearchFiles(path); // 递归遍历
		}
	}
	closedir(dir);
}
