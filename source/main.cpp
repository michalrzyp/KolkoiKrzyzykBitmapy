#include<Windows.h>
#include<stdio.h>
#include "res.h"

enum wynikGry
{
  wygrywaKolko,
  wygrywaKrzyzyk,
  remis,
  nierozstrzygniety
};
bool isGameOn = false;
bool isFirstPlayerTurn = true;
CHAR Text[200];
int Array[9];// 0 puste  pole 1 pole X -1 pole O
void drawX(HDC hdc, int x, int y);
void drawO(HDC hdc, int x, int y);
void drawBoard(HDC hdc);
wynikGry a = nierozstrzygniety;
HINSTANCE hInstance;
HBITMAP hBitmapGameBoard, hBitmapX, hBitmapO, hBitmapStart, hBitmapReset;
WNDPROC wpOrgButtonProc;
LRESULT CALLBACK ButtonWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_PAINT:
  {
    /*//paint the window's client menu
    HDC hDCBitmap;
    HDC hdc = GetDC(hwnd);
    hDCBitmap = CreateCompatibleDC(hdc);
    SelectObject(hDCBitmap, hBitmapStart);
    //StretchBlt(hdc, 0, 0, 400, 264, hDCBitmap, 0, 0, 500, 364, SRCCOPY);
    BitBlt(hdc, 0, 0, 84, 75, hDCBitmap, 0, 0, SRCCOPY);
    DeleteDC(hDCBitmap);
    ReleaseDC(hwnd, hdc);*/
  }
  return 0;
  default:
  {
    //return CallWindowProc(wpOrgButtonProc, hwnd, uMsg, wParam, lParam);
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
  }
}
INT_PTR CALLBACK DialogProc(HWND hwndDig, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_INITDIALOG:
  {
    HWND hwndButton1 = GetDlgItem(hwndDig,IDC_BUTTON1);
    WNDPROC wpOrgButtonProc = (WNDPROC)SetWindowLong(hwndButton1, -4, (LONG)ButtonWndProc);
    hBitmapGameBoard = LoadBitmap(hInstance, MAKEINTRESOURCE(105));
    hBitmapX = LoadBitmap(hInstance, MAKEINTRESOURCE(104));
    hBitmapO = LoadBitmap(hInstance, MAKEINTRESOURCE(103));
    hBitmapStart = LoadBitmap(hInstance, MAKEINTRESOURCE(107));
    hBitmapReset = LoadBitmap(hInstance, MAKEINTRESOURCE(106));
  }
    return TRUE;
    //GWL_WNDPROC -4
  case WM_LBUTTONDOWN:
  {
    int x = LOWORD(lParam), y = HIWORD(lParam);
    if ((x >= 200 && x <= 410) && (y >= 50 && y <= 200))
    {
      int filedX = ((x - 200) / 70);
      int filedY = ((y - 50) / 50);
      if (Array[filedX * 3 + filedY] == 0)
      {
        x = filedX * 70 + 200;
        y = filedY * 50 + 50;//x=((x-iMinBoardX/iWidthBorardX)*iWidthX+iMinBoardX+iWidthBoardX)
        HDC hdc = GetDC(hwndDig);
        // HWND hCtl1 = GetDlgItem(hwndDig, IDC_RADIO1);
        // HWND hCtl2 = GetDlgItem(hwndDig, IDC_RADIO2);
       // HPEN hMyPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
       // SelectObject(hdc, hMyPen);
        if (isFirstPlayerTurn == true)
        {
          drawX(hdc, x, y);
          Array[3 * filedX + filedY] = 1;
          // CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
          // EnableWindow(hCtl1, FALSE);
          // EnableWindow(hCtl2, TRUE);
        }
        else
        {
          drawO(hdc, x, y);
          Array[3 * filedX + filedY] = -1;
          //  CheckRadioButton(hwndDig, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
           // EnableWindow(hCtl2, FALSE);
           // EnableWindow(hCtl1, TRUE);
        }
        ReleaseDC(hwndDig, hdc);
        isFirstPlayerTurn = !isFirstPlayerTurn;
      }
    }
  }
    return TRUE;
  case WM_PAINT:
  {
    HDC hdc = GetDC(hwndDig);
    //BitBlt(hdc, 0, 0, 485, 242, GetDC(0), 0, 0, SRCCOPY);
    drawBoard(hdc);
  }
  return 0;
  case WM_CLOSE:
    DestroyWindow(hwndDig);
    DeleteObject(hBitmapGameBoard);
    DeleteObject(hBitmapX);
    DeleteObject(hBitmapO);
    DeleteObject(hBitmapStart);
    DeleteObject(hBitmapReset);
    PostQuitMessage(0);
    return TRUE;
  }
  return FALSE;
}
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevinstance, PSTR szCmdLIne, int iCmdShow)
{
  hInstance = hinstance;
  HWND hwndMainWindow = CreateDialog(hinstance, MAKEINTRESOURCE(IDD_MAINVIEW), NULL, DialogProc);
  ShowWindow(hwndMainWindow, iCmdShow);
  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}

void drawX(HDC hdc, int x, int y)
{
  /*x += 35;
  y += 25;
  HPEN hMyPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
  SelectObject(hdc, hMyPen);
  MoveToEx(hdc, x - 10, y - 10, NULL);
  LineTo(hdc, x + 10, y + 10);
  MoveToEx(hdc, x - 10, y + 10, NULL);
  LineTo(hdc, x + 10, y - 10);*/
  HDC hDCBitmap;
  hDCBitmap = CreateCompatibleDC(hdc);
  SelectObject(hDCBitmap, hBitmapX);
  //StretchBlt(hdc, 0, 0, 400, 264, hDCBitmap, 0, 0, 500, 364, SRCCOPY);
  BitBlt(hdc, 0, 0, 84, 75, hDCBitmap, 0, 0, SRCCOPY);
  DeleteDC(hDCBitmap);
}
void drawO(HDC hdc, int x, int y)
{
  /*x += 35;
  y += 25;
  HPEN hMyPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
  SelectObject(hdc, hMyPen);
  MoveToEx(hdc, x - 10, y - 10, NULL);
  LineTo(hdc, x - 10, y + 10);
  LineTo(hdc, x - 10, y - 10);
  LineTo(hdc, x + 10, y - 10);
  LineTo(hdc, x + 10, y + 10);
  LineTo(hdc, x - 10, y + 10);*/
  HDC hDCBitmap;
  hDCBitmap = CreateCompatibleDC(hdc);
  SelectObject(hDCBitmap, hBitmapO);
  //StretchBlt(hdc, 0, 0, 400, 264, hDCBitmap, 0, 0, 500, 364, SRCCOPY);
  BitBlt(hdc, 0, 0, 84, 75, hDCBitmap, 0, 0, SRCCOPY);
  DeleteDC(hDCBitmap);
}
void drawBoard(HDC hdc)
{
  /*Rectangle(hdc, 200, 50, 410, 200);
  MoveToEx(hdc, 200, 100, NULL);
  LineTo(hdc, 410, 100);
  MoveToEx(hdc, 200, 150, NULL);
  LineTo(hdc, 410, 150);

  MoveToEx(hdc, 270, 50, NULL);
  LineTo(hdc, 270, 200);
  MoveToEx(hdc, 340, 50, NULL);
  LineTo(hdc, 340, 200);*/
  HDC hDCBitmap;
  hDCBitmap = CreateCompatibleDC(hdc);
  SelectObject(hDCBitmap, hBitmapGameBoard);
  StretchBlt(hdc, 0, 0, 400, 264, hDCBitmap, 0, 0, 500,364, SRCCOPY);
  DeleteDC(hDCBitmap);
}
/*
wynikGry CheckResult()
{
  for (int i = 0; i < 3; i++)
  {
    if (Array[3 * i] == Array[3 * i + 1] && Array[3 * i] == Array[3 * i + 2])
    {
      if (Array[3 * i] == 1)
        return wygrywaKrzyzyk;
      if (Array[3 * i] == -1)
        return wygrywaKolko;
    }
    if (Array[i] == Array[i + 3] && Array[i] == Array[i + 6])
    {
      if (Array[i] == 1)
        return wygrywaKrzyzyk;
      if (Array[i] == -1)
        return wygrywaKolko;
    }
  }
  if ((Array[0] == Array[4] && Array[0] == Array[8]) || (Array[2] == Array[4] && Array[6] == Array[4]))
  {
    if (Array[4] == -1)
      return wygrywaKolko;
    if (Array[4] == 1)
      return wygrywaKrzyzyk;
  }
  for (int i = 0; i < 9; i++)
    if (Array[i] == 0)
      return nierozstrzygniety;
  return remis;
}

void Result(HWND hwndDig, wynikGry a)
{
  HWND hwndStatic2 = GetDlgItem(hwndDig, IDC_STATIC2);
  switch (a)
  {
  case wygrywaKolko:
    wsprintf(Text, "Wygrywa kolko");
    SetWindowText(hwndStatic2, Text);
    isGameOn = false;
    break;
  case wygrywaKrzyzyk:
    wsprintf(Text, "Wygrywa krzyzyk");
    SetWindowText(hwndStatic2, Text);
    isGameOn = false;
    break;
  case remis:
    wsprintf(Text, "Remis");
    SetWindowText(hwndStatic2, Text);
    isGameOn = false;
    break;
  case nierozstrzygniety:;
  default:;
  }
}*/


//void drawBitmap(HBITMAP hbmp, HDC hdc, int x, int y, int width, int height);