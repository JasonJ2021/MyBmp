#include <string.h>
#include <math.h>

#include "libgraphics/extgraph.h"
#include "libdisplay/display.h"
#include "libgraphics/imgui.h"
#include "third_party/dirent.h"
#include "libgraphics/win32Export.h"
#include "libdisplay/zip.h"

static double WinWidth, WinHeight; // ���ڳߴ�
static int ShowGUI = 0;			   //�л�GUI����
static int ShowAbout = 0;
static int ShowUseWay = 0;
static int ShowPictureRegion = 1;
static int ShowMoreButtons = 0; // ��ʾ���ఴť
static int ShowFilesList = 0;
static int ShowCropBox = 0;
static int ShowSaveAnother = 0; // ���Ϊ
static char BasePatch[100] = "../resource";
static char FileName[20][100] = {""};
static int ChangeGUI = 0;
static double MouseX = 0;
static double MouseY = 0;
static int CurIndex = 0;	  // ��ǰ��ʾͼƬ��Index
static int ShouldDisplay = 0; // ��ǰ�Ƿ�Ӧ����ʾͼƬ
static int MaxWidth = 0;	  // ��ʾ����
static int MaxHeight = 0;	  // ��ʾ��߶�

static char *selectedLabel1 = NULL;
static char *selectedLabel2 = NULL;

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

// �����ļ����е��ļ�
void SearchFiles(char *BasePath);

// ��ʾ���Ϊ����
void DrawShowSaveAnother();

// ��ʾBMPͼƬ��ʾ����
void DrawShowPictureRegion();

// ��ʾ�ü�����
void DrawShowCropBox();

// ��ʾ�ļ��б�
void DrawShowFilesList(void);

// ��ʾ�޸Ĵ�·�����ı���
void DrawEditText(void);

// ��ʾ�����˵��Ĺ��ڽ���
void DrawShowAbout(void);

// ��ʾ���������ʹ�÷�������
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
	MouseX = ScaleXInches(x);
	MouseY = ScaleYInches(y);
	Display(); // ˢ����ʾ
}

// ������
void Main()
{
	// �򿪿���̨��������printf/scanf���/�������Ϣ���������
	// InitConsole();
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
	WinWidth = GetWindowWidth();
	WinHeight = GetWindowHeight();
	printf("���ڳߴ磺%.2f %.2f\n", WinWidth, WinHeight);
	DisplayViewPort(0, 0, DisplayGetWindowPixelWidth(), DisplayGetWindowPixelHeight());
	DisplayFillWithColor(255, 255, 255);
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
	// DisplayViewPort(0,0,displayGetWindowPixelWidth(),displayGetWindowPixelHeight());

	// // ��ʼ���������ñ�����ɫΪ��ɫ
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
	static char *menuListTool[] = {
		"����",
		"�Ŵ� | Ctrl-A",
		"��С | Ctrl-B",
		"���� | Ctrl-X",
		"���� | Ctrl-Y",
		"��λ | Ctrl-R",
		"�ü� | Ctrl-T",
		"ѹ�� | Ctrl-Z",
	};
	static char *menuListHelp[] = {"����",
								   "ʹ�÷��� | Ctrl-U",
								   "����"};

	double fontHeight = GetFontHeight(); //�ָ�
	double x = 0;
	double y = WinHeight;
	double h = fontHeight * 1.5;						   //��ť�߶�
	double w = TextStringWidth(menuListHelp[0]) * 3.8;	   //һ���˵���ѡ����
	double wlist = TextStringWidth(menuListTool[3]) * 1.5; //�����˵�ѡ�������
	double xindent = WinHeight / 40;
	static int selection = 0;

	//��ʾ�˵���
	setMenuColors("GrayBlue", "Black", "Dark Gray", "White", 1); //���ò˵�����ɫΪGrayBlue
	SetPenSize(2);
	drawMenuBar(0, y - h, WinWidth, h);

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
		ShowCropBox = 0;
	}
	if (selection == 2) // �ر�
	{
		ShowFilesList = 0;
	}
	if (selection == 3) // ����
	{
		if (ShouldDisplay)
		{
			SavePicture(pictures[CurIndex].name, CurIndex);
		}
	}
	if (selection == 4) // ���Ϊ
	{
		ShowSaveAnother = 1;
		ShowCropBox = 0;
	}
	if (selection == 5) // ѡ���˳�
	{
		ExitGraphics();
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
		if (ShouldDisplay)
		{
			ExpandPicture(CurIndex, MaxWidth, MaxHeight);
		}
	}

	// ��С
	if (selection == 2)
	{
		if (ShouldDisplay)
		{
			ShrinkPicture(CurIndex);
		}
	}

	// ��ʱ����ת
	if (selection == 3)
	{
		if (ShouldDisplay)
		{
			LeftRotatePicture(CurIndex);
		}
	}

	// ˳ʱ����ת
	if (selection == 4)
	{
		if (ShouldDisplay)
		{
			RightRotatePicture(CurIndex);
		}
	}
	// ��λ
	if (selection == 5)
	{
		if (ShouldDisplay)
		{
			RetrievePicture(CurIndex);
		}
	}

	// �ü�
	if (selection == 6)
	{
		if (ShouldDisplay)
		{
			ShowCropBox = 1;
			ShowFilesList = 0;
			ShowSaveAnother = 0;
		}
	}
	// ���沢��ѹ��
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

	// �����˵�
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

	// ���ذ�ť
	SetPenSize(1);
	if (button(GenUIID(0), 0, fontHeight * 4, WinWidth / 12, fontHeight * 2.5, "����"))
	{
		ShowAbout = 0;
		ShowUseWay = 0;
		printf("%s %s", selectedLabel1, selectedLabel2);
		ShowGUI = 0;

		selectedLabel1 = "����";
		selectedLabel2 = NULL;
	}

	// ��ʾ�˵���һ������
	drawMenuBar(0, 0, WinWidth, fontHeight * 3);
	SetPenColor("black");
	SetPointSize(2);
	drawLabel(0, fontHeight * 2, "��һ���˵�����Ϊ: ");
	SetPenColor("Red");
	drawLabel(0, fontHeight * 0.4, selectedLabel1);
	drawLabel(TextStringWidth(menuListHelp[0]) * 2, fontHeight * 0.4, selectedLabel2);
}

void StartGUI()
{

	static char *menuListHelp[] = {
		"����",
		"ʹ�÷���",
		"����",
	};

	double fontHeight = GetFontHeight();				   //�ָ�
	double h = fontHeight * 2.5;						   //��ť�߶�
	double w = WinWidth / 6;							   //��ť���
	double x = WinWidth / 2 - w;						   // ��ť��׼����
	double y = WinHeight / 2 - fontHeight;				   // ��ť��׼����
	double w1 = TextStringWidth(menuListHelp[0]) * 1.5;	   //һ���˵���ѡ����
	double wlist = TextStringWidth(menuListHelp[1]) * 1.5; //�����˵�ѡ�������
	double h1 = fontHeight * 1.5;						   //�˵�ѡ��߶�
	static int selection = 0;							   //�˵�ѡ��

	setMenuColors("GrayBlue", "Black", "Dark Gray", "White", 1); //���ò˵�����ɫΪGrayBlue
	SetPenSize(1);
	drawMenuBar(0, WinHeight - h1, WinWidth, h1); //�˵���

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
	setButtonColors("GrayBlue", "Black", "Dark Gray", "White", 1); // ���ð�ť��ɫ

	DrawEditText();
	if (button(GenUIID(0), x, y + h * 1.5, w, h, ShowMoreButtons ? "�����ļ�" : "չʾ�ļ�"))
	{
		if (ShowMoreButtons = !ShowMoreButtons)
		{
			selectedLabel1 = "չʾ�ļ�";
			selectedLabel2 = NULL;
		}
		else
		{
			selectedLabel1 = "�����ļ�";
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
	if (button(GenUIID(0), x, y, w, h, "������ʾͼƬ����"))
	{
		selectedLabel1 = "������ʾͼƬ����";
		selectedLabel2 = NULL;
		ShowGUI = 1;
	}
	if (button(GenUIID(0), x, y - h * 1.5, w, h, "�˳�"))
		ExitGraphics();

	// ��ʾ�˵���һ������
	drawMenuBar(0, 0, WinWidth, fontHeight * 3);
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
	double w = WinWidth / 6;	 // �ı��༭����
	double x = WinWidth / 2 - w; // x�����׼
	double y = WinHeight / 2 - fontHeight + h * 3;

	static char temp[100] = "../resource";

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
		selectedLabel1 = "ȷ���޸�";
		selectedLabel2 = NULL;
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
	double w = WinWidth / 12;			 //���ذ�ť���
	double h = fontHeight * 2.5;		 //���ذ�ť�߶�
	int selection;						 //�˵�ѡ��
	static char *menuListHelp[] = {
		"����",
		"ʹ�÷���",
		"����",
	};

	drawMenuBar(0, WinHeight - fontHeight * 1.5, WinWidth, fontHeight * 1.5); //�����˵���
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

	// ���ذ�ť
	if (button(GenUIID(0), 0, fontHeight * 4, w, h, "����"))
	{
		ShowAbout = 0;
		ShowUseWay = 0;
		ShowGUI = ChangeGUI;
		selectedLabel1 = "����";
		selectedLabel2 = NULL;
	}

	// ��ʾ�˵���һ������
	drawMenuBar(0, 0, WinWidth, fontHeight * 3);
	SetPenColor("black");
	SetPointSize(2);
	drawLabel(0, fontHeight * 2, "��һ���˵�����Ϊ: ");
	SetPenColor("Red");
	drawLabel(0, fontHeight * 0.4, selectedLabel1);
	drawLabel(TextStringWidth(menuListHelp[0]) * 2, fontHeight * 0.4, selectedLabel2);
}

void DrawShowSaveAnother() // ���Ϊ�ļ����༭��
{
	static char temp[100] = "���·��";
	double fontHeight = GetFontHeight();
	double x = TextStringWidth("��� | Ctrl-Q") * 1.6;
	double w = TextStringWidth("ȷ��") * 1.2;
	double y = WinHeight - fontHeight * 14; // ����y��׼
	SetPenSize(1);
	SetPenColor("Blue");
	drawLabel(x, y, "���Ϊ:  ");

	textbox(GenUIID(0), x, y -= fontHeight * 2, x * 0.8, fontHeight * 1.2, temp, sizeof(temp));

	SetPenSize(1);
	if (button(GenUIID(0), x * 1.3, y -= fontHeight * 2, w, fontHeight * 1.5, "ȷ��"))
	{
		// �½����ļ��� = temp;
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
	double fontHeight = GetFontHeight();	//�ָ�
	double h = fontHeight * 1.2;			//���ֿ�߶�
	double w = TextStringWidth("0.55") * 3; //�ı�����
	double x = ScaleXInches(3);				//��׼����x
	double y = WinHeight - fontHeight * 10; //��׼����y
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
		printf("width = %d , height = %d\n", pictures[CurIndex].picture_width, pictures[CurIndex].picture_height);
		int x1_d = atof(x1) * pictures[CurIndex].picture_width;
		int y1_d = atof(y1) * pictures[CurIndex].picture_height;
		int x2_d = atof(x2) * pictures[CurIndex].picture_width;
		int y2_d = atof(y2) * pictures[CurIndex].picture_height;

		// ��λ
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
	// ͼƬ��ʾ����
	double fontHeight = GetFontHeight();
	double bmpx = TextStringWidth("����") * 7.5;
	double bmpy = fontHeight * 5.5;
	double bmpw = WinWidth - bmpx;
	double bmph = WinHeight - fontHeight * 10;
	double buttonw = WinWidth / 15;
	double buttonh = fontHeight * 2.3;
	SetPenSize(2);

	if (pictures[CurIndex].name)
	{
		SetPenColor("Black");
		drawLabel(WinWidth - bmpw / 1.5, bmpy + bmph + fontHeight * 0.8, "������ʾ��ͼƬ��: ");
		SetPenColor("Blue");
		drawLabel(WinWidth - bmpw / 1.5 + TextStringWidth("������ʾ��ͼƬ��: "), bmpy + bmph + fontHeight * 0.8, pictures[CurIndex].name);
	}
	SetPenColor("Gray");
	drawBox(bmpx, bmpy, bmpw, bmph, 1, "bmpͼƬ��ʾ����", 'C', "Red");
	printf("%d\n", GetPenSize());
	// ����ͼƬ���߶ȡ����
	MaxHeight = inchXToPixelX(bmph);
	MaxWidth = inchYToPixelY(bmpw);
	// ����ͼƬ��ʾ������λ��
	SetPictureMiddleX(inchXToPixelX(bmpx + bmpw / 2));
	SetPictureMiddleY(inchYToPixelY(bmpy + bmph / 2));
	// �����Ҫ��ʾͼƬ����������ʾͼƬ��
	if (ShouldDisplay)
	{
		DisplayPicture(CurIndex);
	}
	// ��ݼ�Сͼ��
	// ��һ��
	SetPenSize(1);
	if (button(GenUIID(0), bmpx, fontHeight * 3.1, buttonw, buttonh, ""))
	{
		// �л���ʾ��һ��ͼƬ
		CurIndex = GetPrevIndex(CurIndex);
		selectedLabel1 = "�л���һ��";
		selectedLabel2 = NULL;
	}
	SetPenColor("Black");
	MovePen(bmpx + buttonw * 0.2, fontHeight * 3 + buttonh / 2);
	DrawLine(buttonw * 0.5, -buttonh * 0.2);
	DrawLine(0, buttonh * 0.5);
	DrawLine(-buttonw * 0.5, -buttonh * 0.3);

	// ��һ��
	if (button(GenUIID(0), WinWidth - buttonw, fontHeight * 3.1, buttonw, buttonh, ""))
	{
		// �л���ʾ��һ��ͼƬ
		CurIndex = GetNextIndex(CurIndex);
		selectedLabel1 = "�л���һ��";
		selectedLabel2 = NULL;
	}
	MovePen(WinWidth - buttonw * 0.2, fontHeight * 3 + buttonh / 2);
	DrawLine(-buttonw * 0.5, -buttonh * 0.2);
	DrawLine(0, buttonh * 0.5);
	DrawLine(buttonw * 0.5, -buttonh * 0.3);

	// ��СͼƬ
	if (button(GenUIID(0), WinWidth - buttonw * 4, fontHeight * 3.1, buttonw, buttonh, ""))
	{
		// ��СͼƬ
		ShrinkPicture(CurIndex);
		selectedLabel1 = "��С";
		selectedLabel2 = NULL;
	}
	MovePen(WinWidth - buttonw * 4 + buttonw * 0.4, fontHeight * 3.1 + buttonh / 2);
	SetPenSize(1);
	DrawLine(buttonw * 0.2, 0);

	// �Ŵ�ͼƬ
	SetPenSize(1);
	if (button(GenUIID(0), bmpx + buttonw * 4, fontHeight * 3.1, buttonw, buttonh, ""))
	{
		// �Ŵ�ͼƬ
		ExpandPicture(CurIndex, MaxWidth, MaxHeight);
		selectedLabel1 = "�Ŵ�";
		selectedLabel2 = NULL;
	}
	SetPenSize(1);
	MovePen(bmpx + buttonw * 4 + buttonw * 0.35, fontHeight * 3.1 + buttonh / 2);
	SetPenSize(1);
	DrawLine(buttonw * 0.3, 0);
	MovePen(bmpx + buttonw * 4 + buttonw / 2, fontHeight * 3.1 + buttonh * 0.3);
	DrawLine(0, buttonh * 0.4);

	// ��λͼƬ
	SetPenSize(1);
	if (button(GenUIID(0), bmpx + bmpw / 2, fontHeight * 3.1, buttonw, buttonh, ""))
	{
		// ��λͼƬ
		if (ShouldDisplay)
		{
			RetrievePicture(CurIndex);
		}
		selectedLabel1 = "��λ";
		selectedLabel2 = NULL;
	}
	MovePen(bmpx + bmpw / 2 + buttonw / 1.5, fontHeight * 3.1 + buttonh / 2);
	DrawArc(buttonh / 3, 0, 360);
}

void DrawShowFilesList()
{
	SearchFiles(BasePatch);
	SetPenSize(1);
	double fontHeight = GetFontHeight();	//�ָ�
	double h = fontHeight * 1.2;			//��ť�߶�
	double w = WinWidth / 11;				//��ť���
	double x = ScaleXInches(3);				//��׼����x
	double y = WinHeight - fontHeight * 12; //��׼����y
	SetPenColor("Blue");
	drawLabel(x, y, "�ļ��б� ");

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
				ReadInPicture(uncompressed_name);
				int i = -1;
				if ((i = FindIndex(uncompressed_name)) >= 0)
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
	double fontHeight = GetFontHeight() * 1.2; //�ָ�
	double x = WinWidth / 10;
	double y = WinHeight - WinHeight / 6;

	SetPenColor("Black");
	drawLabel(x, y, "ע������1��Ĭ��bmp�ļ�·��Ϊ../resource/�������޸ģ����ı������޸ĺ󣬵���ұߡ�ȷ���޸ġ���ť��");
	drawLabel(x, y -= fontHeight * 1.5, "ע������2�� ������ʾҳ�����ƣ��ļ�����bmp�ļ���Ŀ���ܴ���10��");
	drawLabel(x, y -= fontHeight * 1.5, "ע������3���ļ������·����/�ļ�������Ӣ���ַ��� ");
	drawLabel(x, y -= fontHeight * 1.5, "ע������4�� ");
}

void DrawShowAbout()
{
	double fontHeight = GetFontHeight() * 1.2; //�ָ�
	double x = WinWidth / 10;
	double y = WinHeight - WinHeight / 6;
	SetPenColor("Black");
	drawLabel(x, y -= fontHeight * 1.5, "���������ΪBMPͼƬ��ʾ����Ϊ2022���㽭��ѧ�������ר��γ̴���ҵ��");
	drawLabel(x, y -= fontHeight * 1.5, "���������ʵ�ֶ�BMP��ʽͼƬ�ķŴ�/��С/����/����/��λ/�ü�/ѹ��/��ѹ���ȹ��ܡ�");
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
