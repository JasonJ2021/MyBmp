#include <string.h>
#include <math.h>

#include "libgraphics/extgraph.h"
#include "libdisplay/display.h"
#include "libgraphics/imgui.h"
#include "third_party/dirent.h"
#include "libgraphics/win32Export.h"
#include "libdisplay/zip.h"

static double winwidth, winheight; // ���ڳߴ�
static int ShowGUI = 0;			   //�л�GUI����
static int ShowAbout = 0;
static int ShowUseWay = 0;
static int ShowPictureRegion = 1;
static int show_more_buttons = 0; // ��ʾ���ఴť
static int ShowFilesList = 0;
static int ShowCropBox = 0;
static int ShowSaveAnother = 0; // ���Ϊ
static char BasePatch[200] = "./resource";
static char FileName[100][200] = {""};

static double mouse_x = 0;
static double mouse_y = 0;
static int cur_index = 0;	   // ��ǰ��ʾͼƬ��Index
static int should_display = 0; // ��ǰ�Ƿ�Ӧ����ʾͼƬ
static int max_width = 0;	   // ��ʾ����
static int max_height = 0;	   // ��ʾ��߶�

// ����������provided in libgraphics
void DisplayClear(void);

// �û�����ʾ����
void Display(void);

// ͼƬ��ʾ����
void ShowBMP(void);

// ��ʼ����
void StartGUI(void);

// ��������
void ShowHelp(void);

//
void SearchFiles(char *BasePath);

void DrawShowSaveAnother();
void DrawShowPictureRegion();
void DrawShowCropBox();
void DrawShowFilesList(void);

void DrawEditText(void);

void DrawShowAbout(void);

void DrawShowUseWay(void);

// �û����ַ��¼���Ӧ����
void CharEventProcess(char ch)
{
	uiGetChar(ch); // GUI�ַ�����
	Display();	   //ˢ����ʾ
}

// �û��ļ����¼���Ӧ����
void KeyboardEventProcess(int key, int event)
{
	uiGetKeyboard(key, event); // GUI��ȡ����
	Display();				   // ˢ����ʾ
}

// �û�������¼���Ӧ����
void MouseEventProcess(int x, int y, int button, int event)
{
	uiGetMouse(x, y, button, event); // GUI��ȡ���
	mouse_x = ScaleXInches(x);
	mouse_y = ScaleYInches(y);
	Display(); // ˢ����ʾ
}

// ������
void Main()
{
	// �򿪿���̨��������printf/scanf���/�������Ϣ���������
	InitConsole();
	//�رտ���̨������VS Code������ʹ�ã���ΪVS2019��VS2017��������ע�͵��������д���
	// FreeConsole();
	// ��ʼ������
	SetWindowTitle("BMPͼƬ��ʾ��");
	// SetWindowSize(13.39, 7.48);  // �����Ļ�ߴ粻�����򰴱�����С
	//  ��ȡȫ�����ڳߴ�
	double fullwidth = GetFullScreenWidth();
	double fullheight = GetFullScreenHeight();
	// ���ô��ڳߴ�
	SetWindowSize(fullwidth, fullheight);
	printf("�ߴ��С��%.2f %.2f\n", fullwidth, fullheight);
	// ��ʼ��ͼ��ϵͳ
	InitGraphics();

	// ��ô��ڳߴ�
	winwidth = GetWindowWidth();
	winheight = GetWindowHeight();
	printf("���ڳߴ磺%.2f %.2f\n", winwidth, winheight);
	displayViewPort(0, 0, displayGetWindowPixelWidth(), displayGetWindowPixelHeight());
	displayFillWithColor(255, 255, 255);
	// �Զ�����ɫ
	DefineColor("GrayBlue", 0.57, 0.71, 0.83);

	// ע��ʱ����Ӧ����
	registerCharEvent(CharEventProcess);		 // �ַ�
	registerKeyboardEvent(KeyboardEventProcess); // ����
	registerMouseEvent(MouseEventProcess);		 // ���

	//��ȡ�ļ���Ϣ
	SearchFiles(BasePatch);

	//============================================== DEBUG DISPLAY
	// ����ȫ��
	// winwidth = GetFullScreenWidth();
	// winheight = GetFullScreenWidth();
	// SetWindowSize(winwidth, winheight);
	// int x , y , n ;
	// unsigned char *data = stbi_load("./resource/1.bmp" , &x , &y , &n , 4);
	// int size = sizeof(data);
	// InitConsole();
	// InitGraphics();
	// SetWindowTitle("MyBmp");
	// // ========================GUI PART==================================
	// //�Զ�����ɫ
	// DefineColor("GrayBlue", 0.57, 0.71, 0.83);

	// // ע��ʱ����Ӧ����
	// registerCharEvent(CharEventProcess);         // �ַ�
	// registerKeyboardEvent(KeyboardEventProcess); // ����
	// registerMouseEvent(MouseEventProcess);       // ���

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
	// 15msΪ���ڣ�������60FPS
	// // startTimer(0, 15);
	// displayViewPort(0,0,displayGetWindowPixelWidth(),displayGetWindowPixelHeight());

	// // ��ʼ���������ñ�����ɫΪ��ɫ
	// displayFillWithColor(255,255,255);
	// printf("win_width = %d , win_height = %d\n" , winwidth , winheight);
	// set_picture_middle_x(inchXToPixelX(winwidth/2));
	// set_picture_middle_y(inchYToPixelY(winheight/2));
	// readInPicture("./resource/2.bmp");
	// displayPicture(0);
	// clearPicture(0);
	// readInPicture("./resource/2.bmp");
	// right_Rotate_Picture(0);
	// right_Rotate_Picture();
	// readInPicture("./resource/1.bmp");
	// displayPicture(0);
	// clearPicture(0);
	// displayPicture(1);
	// clearPicture(1);
	//==============================cur part ==========================//
	// cut_Picture(0 , 0,0,200,200);
	// displayPicture(0);

	//==============================save part =========================//
	// save_Picture("./resource/saved.bmp");
	// displayPicture("./resource/2.bmp");

	//==============================resize part =======================//
	// displayPicture("./resource/2.bmp");
	// clearPicture();
	// resizePicture(500 ,500);
	// displayPicture();
	// clearPicture();
	// resizePicture(100 ,100);
	// displayData();
}

//��ʾ����
void Display()
{
	// ����
	DisplayClear();

	switch (ShowGUI)
	{
	case 0: //��ʼ����
		StartGUI();
		break;
	case 1: //ͼƬ��ʾ����
		ShowBMP();
		break;
	case 2: //�����������
		ShowHelp();
		break;
	default:
		break;
	}
}

// ͼƬ��ʾ����
void ShowBMP()
{
	static char *menuListFile[] = {"�ļ�",
								   "�� | Ctrl-O",
								   "�ر� | Ctrl-P",
								   "���� | Ctrl-S",
								   "��� | Ctrl-Q",
								   "�˳� | Ctrl-E"};
	static char *menuListTool[] = {"����",
								   "�Ŵ� | Ctrl-A",
								   "��С | Ctrl-B",
								   "��ת | Ctrl-X",
								   "��λ | Ctrl-R",
								   "�ü� | Ctrl-T",
								   "ѹ�� | Ctrl-Z",
								   "��ѹ | Ctrl-U"};
	static char *menuListHelp[] = {"����",
								   "ʹ�÷��� | Ctrl-U",
								   "����"};

	static char *selectedLabel1 = NULL;
	static char *selectedLabel2 = NULL;

	double fontHeight = GetFontHeight(); //�ָ�
	double x = 0;
	double y = winheight;
	double h = fontHeight * 1.5;						   //��ť�߶�
	double w = TextStringWidth(menuListHelp[0]) * 3.8;	   //һ���˵���ѡ����
	double wlist = TextStringWidth(menuListTool[3]) * 1.5; //�����˵�ѡ�������
	double xindent = winheight / 40;
	static int selection = 0;

	//��ʾ�˵���
	setMenuColors("GrayBlue", "Black", "Dark Gray", "White", 1); //���ò˵�����ɫΪGrayBlue
	SetPenSize(2);
	drawMenuBar(0, y - h, winwidth, h);

	//��ʾ�ļ��˵�
	SetPenSize(2);
	setMenuColors("GrayBlue", "Black", "Dark Gray", "White", 1); // ���ò˵�����ɫΪBlack��label��ɫΪBlack
	selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListFile, sizeof(menuListFile) / sizeof(menuListFile[0]));
	if (selection > 0)
	{
		selectedLabel1 = menuListFile[0];
		selectedLabel2 = menuListFile[selection];
	}
	if (selection == 1) // ��
	{
		ShowFilesList = 1;
	}
	if (selection == 2) // �ر�
	{
		ShowFilesList = 0;
	}
	if (selection == 3) // ����
	{
		if (should_display)
		{
			save_Picture(pictures[cur_index].name, cur_index);
		}
	}
	if (selection == 4) // ���Ϊ
	{
		ShowSaveAnother = 1;
	}
	if (selection == 5) // ѡ���˳�
	{
		ExitGraphics(-1);
	}

	// ���߲˵�
	selection = menuList(GenUIID(0), x + w, y - h, w, wlist, h, menuListTool, sizeof(menuListTool) / sizeof(menuListTool[0]));
	if (selection > 0)
	{
		selectedLabel1 = menuListTool[0];
		selectedLabel2 = menuListTool[selection];
	}

	// �Ŵ�
	if (selection == 1)
	{
		if (should_display)
		{
			expandPicture(cur_index, max_width, max_height);
		}
	}

	// ��С
	if (selection == 2)
	{
		if (should_display)
		{
			shrinkPicture(cur_index);
		}
	}

	// ��ת
	if (selection == 3)
	{
		if (should_display)
		{
			right_Rotate_Picture(cur_index);
		}
	}

	// ��λ
	if (selection == 4)
	{
		if (should_display)
		{
			retrievePicture(cur_index);
		}
	}

	// �ü�
	if (selection == 5)
	{
		if (should_display)
		{
			ShowCropBox = 1;
			ShowFilesList = 0;
		}
	}
	// ���沢��ѹ��
	if (selection == 6)
	{
		if (should_display)
		{
			save_Picture(pictures[cur_index].name, cur_index);
			char compressed_name[200];
			memset(compressed_name, 0, 200);
			strncpy(compressed_name, pictures[cur_index].name, 128);
			strcat(compressed_name, ".huf");
			compress(pictures[cur_index].name, compressed_name);
		}
	}

	// �����˵�
	selection = menuList(GenUIID(0), x + 2 * w, y - h, w, wlist, h, menuListHelp, sizeof(menuListHelp) / sizeof(menuListHelp[0]));
	if (selection > 0)
	{
		selectedLabel1 = menuListHelp[0];
		selectedLabel2 = menuListHelp[selection];
	}
	if (selection == 1)
	{

		ShowPictureRegion = 0;
		ShowUseWay = 1;
		ShowAbout = 0;
	}
	if (selection == 2)
	{
		ShowPictureRegion = 0;
		ShowAbout = 1;
		ShowUseWay = 0;
	}
	if (ShowFilesList)
	{
		DrawShowFilesList();
	}
	if (ShowSaveAnother)
	{
		DrawShowSaveAnother();
	}
	if (ShowUseWay)
	{
		DrawShowUseWay();
	}
	if (ShowAbout)
	{
		DrawShowAbout();
	}
	if (ShowPictureRegion)
	{
		DrawShowPictureRegion();
	}
	if (ShowCropBox)
	{
		DrawShowCropBox();
	}
	// ���ذ�ť
	SetPenSize(1);
	if (button(GenUIID(0), 0, fontHeight * 4, winwidth / 12, fontHeight * 2.5, "����"))
	{
		ShowAbout = 0;
		ShowUseWay = 0;
		printf("%s %s", selectedLabel1, selectedLabel2);
		if (selectedLabel1 == menuListHelp[0])
		{
			// ��ʾͼƬ��ʾ����
			ShowPictureRegion = 1;
			//
		}
		else
		{
			ShowGUI = 0;
		}
		selectedLabel1 = NULL;
		selectedLabel2 = NULL;
	}

	// ��ʾ�˵���һ������
	drawMenuBar(0, 0, winwidth, fontHeight * 3);
	SetPenColor("black");
	SetPointSize(2);
	drawLabel(0, fontHeight * 2, "��һ���˵�����Ϊ: ");
	SetPenColor("Red");
	drawLabel(0, fontHeight * 0.4, selectedLabel1);
	drawLabel(TextStringWidth(menuListHelp[0]) * 2, fontHeight * 0.4, selectedLabel2);
}

#if 0
// ��ť��ʾ����
void drawButtons()
{
	double fontHeight = GetFontHeight();//�ָ�
	double h = fontHeight * 2;
	double x = 0;
	double y = winheight / 2 - h;
	double w = winwidth / 8;



	if (button(GenUIID(0), x, y - 1.5*h, w, h, "Use Predefined Colors"))
	{
		static int times = 0;
		usePredefinedColors(++times);
	}

	if (button(GenUIID(0), x, y - 3 * h, w, h, "Random Button Colors"))
	{
		char *c[] = { "Black","Dark Gray","Gray","Light Gray","White", "Cyan",\
		"Brown", "Red", "Orange", "Yellow", "Green","Blue","Violet", "Magenta" };
		int N = sizeof(c) / sizeof(c[0]);
		static int times = 0;
		times ++;
		setButtonColors(c[times%N], c[(times + 1) % N], \
			c[(times + 2) % N] , c[(times + 3) % N], 1);
	}

	if (button(GenUIID(0), x, y - 4.5 * h, w, h, "Random EditBox Colors"))
	{
		char * c[] = { "Black","Dark Gray","Gray","Light Gray","White", "Cyan",\
		"Brown", "Red", "Orange", "Yellow", "Green","Blue","Violet", "Magenta" };
		int N = sizeof(c) / sizeof(c[0]);
		static int times = 0;
		times++;
		setTextBoxColors(c[times%N], c[(times + 1) % N], \
			c[(times + 2) % N], c[(times + 3) % N], 1);
	}
}
#endif

#if 0
// �ı��༭��ʾ����
void drawEditText()
{
	static char firstName[80] = "��";
	static char lastName[80] = "��";
	static char results[200] = "";
	static char memo[80] = "Hello World! Edit me";
	static char numstr[80] = "3.1415926";
	static char newnum[80] = "";

	double fontHeight = GetFontHeight();
	double h = fontHeight * 2;
	double w = winwidth / 4;
	double x = winwidth / 4;
	double y = winheight / 2 - h;


	textbox(GenUIID(0), 0, winheight - h * 10, w, h, memo, sizeof(memo));
	textbox(GenUIID(0), x, y, w, h, memo, sizeof(memo));

	SetPenColor("Brown");
	drawLabel(x - fontHeight / 2 - TextStringWidth("��"), (y -= h * 1.2) + fontHeight * 0.7, "��");
	if (textbox(GenUIID(0), x, y, w, h, firstName, sizeof(firstName)))
	{
		sprintf(results, "Text edit result is %s+%s", firstName, lastName);
	}
	SetPenColor("Brown");
	drawLabel(x - fontHeight / 2 - TextStringWidth("��"), (y -= h * 1.2) + fontHeight * 0.7, "��");
	if (textbox(GenUIID(0), x, y, w, h, lastName, sizeof(lastName)))
	{
		sprintf(results, "Text edit result is %s+%s", firstName, lastName);
	}

	SetPenColor("Gray");
	drawLabel(x, y -= fontHeight * 1.2, results);

	if (textbox(GenUIID(0), x, y -= h * 1.8, w, h, numstr, sizeof(numstr)))
	{
		double value;
		sscanf(numstr, "%lf", &value);
		sprintf(newnum, "Represented value is %.15f", value);
	}
	SetPenColor("Gray");
	drawLabel(x, y -= fontHeight * 1.2, newnum);

}
#endif

void StartGUI()
{

	static char *menuListHelp[] = {
		"����",
		"ʹ�÷���",
		"����",
	};

	double fontHeight = GetFontHeight();				   //�ָ�
	double h = fontHeight * 2.5;						   //��ť�߶�
	double w = winwidth / 6;							   //��ť���
	double x = winwidth / 2 - w;						   // ��ť��׼����
	double y = winheight / 2 - fontHeight;				   // ��ť��׼����
	double w1 = TextStringWidth(menuListHelp[0]) * 1.5;	   //һ���˵���ѡ����
	double wlist = TextStringWidth(menuListHelp[1]) * 1.5; //�����˵�ѡ�������
	double h1 = fontHeight * 1.5;						   //�˵�ѡ��߶�
	static int selection = 0;							   //�˵�ѡ��

	static char *selectedLabel1 = NULL;
	static char *selectedLabel2 = NULL;

	setMenuColors("GrayBlue", "Black", "Dark Gray", "White", 1); //���ò˵�����ɫΪGrayBlue
	SetPenSize(1);
	drawMenuBar(0, winheight - h1, winwidth, h1); //�˵���

	selection = menuList(GenUIID(0), 0, winheight - h1, w1, wlist, h1, menuListHelp, sizeof(menuListHelp) / sizeof(menuListHelp[0]));

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
		ShowUseWay = 1;
		ShowAbout = 0;
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
	setButtonColors("GrayBlue", "Black", "Dark Gray", "White", 1); // ���ð�ť��ɫ

	DrawEditText();
	if (button(GenUIID(0), x, y + h * 1.5, w, h, show_more_buttons ? "�����ļ�" : "չʾ�ļ�"))
	{
		show_more_buttons = !show_more_buttons;
	}

	if (show_more_buttons)
	{
		int k = 0;
		// for (k = 0; k < 12; k++)
		//{
		//	char name[128];
		//	strcpy(name, FileName[k]);
		//
		//	//printf("button %d %s %s\n", Uid[0]+k, name, FileName[k]);
		// }

		for (k = 0; k < 12; k++)
		{
			char name[128];
			strcpy(name, FileName[k]);
			button(GenUIID(k), x + w * 1.1, (y + h * 1.8) - k * h * 0.8, w, h * 0.7, name);
			printf("button %s %s\n", name, FileName[k]);
		}
	}

	if (button(GenUIID(0), x, y, w, h, "������ʾͼƬ����"))
	{
		selectedLabel1 = NULL;
		selectedLabel2 = NULL;
		ShowGUI = 1;
	}
	if (button(GenUIID(0), x, y - h * 1.5, w, h, "�˳�"))
		ExitGraphics();

	// ��ʾ�˵���һ������
	drawMenuBar(0, 0, winwidth, fontHeight * 3);
	SetPenColor("black");
	SetPointSize(2);
	drawLabel(0, fontHeight * 2, "��һ���˵�����Ϊ: ");
	SetPenColor("Red");
	drawLabel(0, fontHeight * 0.4, selectedLabel1);
	drawLabel(TextStringWidth(menuListHelp[0]) * 2, fontHeight * 0.4, selectedLabel2);
}

void DrawEditText(void)
{
	double fontHeight = GetFontHeight();
	double h = fontHeight * 2.5; // �ı��༭��߶�
	double w = winwidth / 6;	 // �ı��༭����
	double x = winwidth / 2 - w; // x�����׼
	double y = winheight / 2 - fontHeight + h * 3;

	static char temp[100] = "./resource";

	static char results[200] = "";

	SetPenColor("Brown");
	drawLabel(x - fontHeight / 2 - TextStringWidth("bmp�ļ�·��: "), y + fontHeight * 0.7, "bmp�ļ�·��: ");

	if (button(GenUIID(0), x + w * 1.3, y, w * 0.5, h, "ȷ���޸�"))
	{
		for (int i = 0; i < 12; i++)
		{
			strcpy(FileName[i], " ");
		}
		strcpy(BasePatch, temp);
		sprintf(results, "·��������Ϊ�� %s", BasePatch);
		SearchFiles(BasePatch);
	}

	setTextBoxColors("GrayBlue", "Black", "Dark Gray", "White", 1); // �����ı��༭����ɫ
	if (textbox(GenUIID(0), x, y, w, h, temp, sizeof(temp)))
	{

		// printf("%s\n", BasePatch);
	}

	SetPenColor("Black");
	drawLabel(x - TextStringWidth("·��������Ϊ��"), y + h * 1.2, results);
}

void ShowHelp()
{
	double fontHeight = GetFontHeight(); //�ָ�
	double w = winwidth / 12;			 //���ذ�ť���
	double h = fontHeight * 2.5;		 //���ذ�ť�߶�
	int selection;						 //�˵�ѡ��
	static char *menuListHelp[] = {
		"����",
		"ʹ�÷���",
		"����",
	};
	static char *selectedLabel1 = NULL;
	static char *selectedLabel2 = NULL;

	drawMenuBar(0, winheight - fontHeight * 1.5, winwidth, fontHeight * 1.5); //�����˵���
	selection = menuList(GenUIID(0), 0, winheight - fontHeight * 1.5, TextStringWidth(menuListHelp[0]) * 1.5,
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

	// ���ذ�ť
	if (button(GenUIID(0), 0, fontHeight * 4, w, h, "����"))
	{
		ShowAbout = 0;
		ShowUseWay = 0;
		ShowGUI = 0;
		selectedLabel1 = NULL;
		selectedLabel2 = NULL;
	}

	// ��ʾ�˵���һ������
	drawMenuBar(0, 0, winwidth, fontHeight * 3);
	SetPenColor("black");
	SetPointSize(2);
	drawLabel(0, fontHeight * 2, "��һ���˵�����Ϊ: ");
	SetPenColor("Red");
	drawLabel(0, fontHeight * 0.4, selectedLabel1);
	drawLabel(TextStringWidth(menuListHelp[0]) * 2, fontHeight * 0.4, selectedLabel2);
}

void DrawShowSaveAnother() // ���Ϊ�ļ����༭��
{
	static char temp[100] = "���Ϊ���·��";
	double fontHeight = GetFontHeight();
	double x = TextStringWidth("��� | Ctrl-Q") * 1.5;
	double w = TextStringWidth("ȷ��") * 1.2;
	drawLabel(x, winheight - fontHeight * 7, "���Ϊ:  ");

	textbox(GenUIID(0), x, winheight - fontHeight * 9, x, fontHeight * 1.5, temp, sizeof(temp));

	SetPenSize(1);
	if (button(GenUIID(0), x * 1.3, winheight - fontHeight * 11, w, fontHeight * 1.2, "ȷ��"))
	{
		// �½����ļ��� = temp;
		ShowSaveAnother = 0;
		if (should_display)
		{
			save_Picture(temp, cur_index);
		}
	}
}

void DrawShowCropBox()
{
	SetPenSize(1);
	double fontHeight = GetFontHeight();	//�ָ�
	double h = fontHeight * 1.2;			//���ֿ�߶�
	double w = TextStringWidth("0.55") * 3; //�ı�����
	double x = ScaleXInches(3);				//��׼����x
	double y = winheight - fontHeight * 10; //��׼����y
	double dx = TextStringWidth("x1:") * 1.2;
	static char x1[10] = "0.0";
	static char x2[10] = "0.5";
	static char y1[10] = "0.0";
	static char y2[10] = "0.5";

	drawLabel(x, y, "������ü��ߴ�: ");

	drawLabel(x, y -= h * 1.5, "x1:");
	drawLabel(x, y -= h * 1.5, "x2:");
	drawLabel(x, y -= h * 1.5, "y1:");
	drawLabel(x, y -= h * 1.5, "y2:");
	if (button(GenUIID(0), x + TextStringWidth("������ü��ߴ�: "), y, w / 2, fontHeight * 1.5, "ȷ��"))
	{
		// ��ֵ
		// printf("x1=%s x2=%s y1=%s y2=%s\n", x1, x2, y1, y2);
		// sscanf(x1, "%lf", &double�ͱ���);
		// sscanf(x2, "%lf", &double�ͱ���);
		printf("width = %d , height = %d\n", pictures[cur_index].picture_width, pictures[cur_index].picture_height);
		int x1_d = atof(x1) * pictures[cur_index].picture_width;
		int y1_d = atof(y1) * pictures[cur_index].picture_height;
		int x2_d = atof(x2) * pictures[cur_index].picture_width;
		int y2_d = atof(y2) * pictures[cur_index].picture_height;
		printf("x1=%d y1=%d x2=%d y2=%d\n", x1_d, y1_d, x2_d, y2_d);
		if (should_display)
		{
			cut_Picture(cur_index, x1_d, y1_d, x2_d, y2_d);
		}
		ShowCropBox = 0;
	}

	y = winheight - fontHeight * 10;
	textbox(GenUIID(0), x + dx, y -= h * 1.5, w, h, x1, sizeof(x1));

	textbox(GenUIID(0), x + dx, y -= h * 1.5, w, h, x2, sizeof(x2));
	textbox(GenUIID(0), x + dx, y -= h * 1.5, w, h, y1, sizeof(y1));

	textbox(GenUIID(0), x + dx, y -= h * 1.5, w, h, y2, sizeof(y2));
}

void DrawShowPictureRegion()
{
	// ͼƬ��ʾ����
	double fontHeight = GetFontHeight();
	double bmpx = TextStringWidth("����") * 7.5;
	double bmpy = fontHeight * 5.5;
	double bmpw = winwidth - bmpx;
	double bmph = winheight - fontHeight * 10;
	double buttonw = winwidth / 15;
	double buttonh = fontHeight * 2.3;
	SetPenSize(2);

	if (pictures[cur_index].name)
	{
		SetPenColor("Black");
		drawLabel(bmpx, bmpy + bmph + fontHeight * 0.8, "������ʾ��ͼƬ��: ");
		SetPenColor("Blue");
		drawLabel(bmpx + TextStringWidth("������ʾ��ͼƬ��: "), bmpy + bmph + fontHeight * 0.8, pictures[cur_index].name);
	}
	SetPenColor("Gray");
	drawBox(bmpx, bmpy, bmpw, bmph, 1, "bmpͼƬ��ʾ����", 'C', "Red");
	printf("%d\n", GetPenSize());
	// ����ͼƬ���߶ȡ����
	max_height = inchXToPixelX(bmph);
	max_width = inchYToPixelY(bmpw);
	// ����ͼƬ��ʾ������λ��
	set_picture_middle_x(inchXToPixelX(bmpx + bmpw / 2));
	set_picture_middle_y(inchYToPixelY(bmpy + bmph / 2));
	// �����Ҫ��ʾͼƬ����������ʾͼƬ��
	if (should_display)
	{
		displayPicture(cur_index);
	}
	// ��ݼ�Сͼ��
	// ��һ��
	SetPenSize(1);
	if (button(GenUIID(0), bmpx, fontHeight * 3.1, buttonw, buttonh, ""))
	{
		// �л���ʾ��һ��ͼƬ
		cur_index = getPrevIndex(cur_index);
	}
	SetPenColor("Black");
	MovePen(bmpx + buttonw * 0.2, fontHeight * 3 + buttonh / 2);
	DrawLine(buttonw * 0.5, -buttonh * 0.2);
	DrawLine(0, buttonh * 0.5);
	DrawLine(-buttonw * 0.5, -buttonh * 0.3);

	// ��һ��
	if (button(GenUIID(0), winwidth - buttonw, fontHeight * 3.1, buttonw, buttonh, ""))
	{
		// �л���ʾ��һ��ͼƬ
		cur_index = getNextIndex(cur_index);
	}
	MovePen(winwidth - buttonw * 0.2, fontHeight * 3 + buttonh / 2);
	DrawLine(-buttonw * 0.5, -buttonh * 0.2);
	DrawLine(0, buttonh * 0.5);
	DrawLine(buttonw * 0.5, -buttonh * 0.3);

	// ��СͼƬ
	if (button(GenUIID(0), winwidth - buttonw * 4, fontHeight * 3.1, buttonw, buttonh, ""))
	{
		// ��СͼƬ
		shrinkPicture(cur_index);
	}
	MovePen(winwidth - buttonw * 4 + buttonw * 0.4, fontHeight * 3.1 + buttonh / 2);
	SetPenSize(2);
	DrawLine(buttonw * 0.2, 0);

	// �Ŵ�ͼƬ
	SetPenSize(1);
	if (button(GenUIID(0), bmpx + buttonw * 4, fontHeight * 3.1, buttonw, buttonh, ""))
	{
		// �Ŵ�ͼƬ
		expandPicture(cur_index, max_width, max_height);
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
	double fontHeight = GetFontHeight();	//�ָ�
	double h = fontHeight * 1.5;			//��ť�߶�
	double w = winwidth / 11;				//��ť���
	double x = ScaleXInches(3);				//��׼����x
	double y = winheight - fontHeight * 10; //��׼����y
	SetPenColor("Blue");
	drawLabel(x, y, "�ļ��б� ");

	for (int k = 0; k < 12; k++)
	{
		char name[128];
		strcpy(name, FileName[k]);
		if (button(GenUIID(k), x, y - fontHeight * 2 - k * h * 1.2, w, h, name))
		{
			char name_full[156] = "./resource/";
			strcat(name_full, name);
			char *temp;
			// ��Ҫ�ж��Ƿ��к�׺.huf
			if ((temp = strstr(name_full, ".huf")) != NULL)
			{
				// ���к�׺.huf
				// ֱ�ӽ�ѹ
				char uncompressed_name[156];
				memset(uncompressed_name, 0, 156);
				strncpy(uncompressed_name, name_full, temp - name_full);
				printf("file name = %s\n", uncompressed_name);
				uncompress(name_full, uncompressed_name);
			}
			else
			{
				readInPicture(name_full);
				should_display = 1;
			}
			// ./resource/2.bmp
		}
		printf("button %s %s\n", name, FileName[k]);
	}
}

void DrawShowUseWay()
{
	double fontHeight = GetFontHeight(); //�ָ�
	double x = winwidth / 5;
	double y = winheight - winheight / 5;

	SetPenColor("Black");
	drawLabel(x, y, "ע������1��Ĭ��bmp�ļ�·��Ϊ../../bmp�������޸ģ����ı������޸ĺ󣬵���ұߡ�ȷ���޸ġ���ť��");
	drawLabel(x, y -= fontHeight * 1.5, "ע������2�� ������ʾҳ�����ƣ��ļ�����bmp�ļ���Ŀ���ܴ���12��");
}

void DrawShowAbout()
{
	double fontHeight = GetFontHeight(); //�ָ�
	double x = winwidth / 4;
	double y = winheight - winheight / 4;
	SetPenColor("Black");
	drawLabel(x, y, "���������ΪBMPͼƬ��ʾ����Ϊ2022���㽭��ѧ�������ר��γ̴���ҵ��");
}

void SearchFiles(char *BasePath)
{
	char path[200]; // ·��
	struct dirent *dp;
	DIR *dir = opendir(BasePath);
	int i = 0;
	// ������ļ���ֱ�ӽ����˴ε���
	if (!dir)
	{
		return;
	}

	while ((dp = readdir(dir)) != NULL)
	{
		// ���� "." �� ��..��
		if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
		{
			// ���ļ�����·������ƴ���γ�һ������·��
			// printf("%s\n", dp->d_name);
			strcpy(FileName[i], dp->d_name);
			// printf("%s\n", FileName[i]);
			i++;
			strcpy(path, BasePath);
			strcat(path, "/");
			strcat(path, dp->d_name);
			SearchFiles(path); // �ݹ����
		}
	}
	closedir(dir);
}