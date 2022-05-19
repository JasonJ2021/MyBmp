#pragma once

#include <Windows.h>

/**
 * @brief 返回窗口句柄
 * 
 * @return HWND 
 */
HWND GetDisPlayWindows();

/**
 * @brief 返回osdc句柄
 * 
 * @return HDC 
 */
HDC GetDisplayOSDC();

int GetWindowPixelWidth();
int GetWindowPixelHeight();

int inchXToPixelX(double x);
int inchYToPixelY(double y);