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
void WstawPrzyciski(HWND hwndMainWindow);
bool isGameOn = false;
bool isFirstPlayerTurn = true;
CHAR Text[200];
CHAR szText[200];
int Array[9];// 0 puste  pole 1 pole X -1 pole O
void drawX(HDC hdc, int x, int y);
void drawO(HDC hdc, int x, int y);
void drawButton(HDC hdc);
void drawBoard(HDC hdc);
void Result(HWND hwndDig, wynikGry a);
wynikGry CheckResult();
wynikGry a = nierozstrzygniety;
HINSTANCE hInstance;
HDC Okno;
HBITMAP hBitmapGameBoard, hBitmapX, hBitmapO, hBitmapStart, hBitmapReset;
WNDPROC wpOrgButtonProc;
LRESULT CALLBACK ButtonWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_PAINT:
  {
    //paint the window's client menu
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
    HWND hwndButton1 = GetDlgItem(hwndDig, IDC_BUTTON1);
    WNDPROC wpOrgButtonProc = (WNDPROC)SetWindowLong(hwndButton1, -4, (LONG)ButtonWndProc);
    hBitmapGameBoard = LoadBitmap(hInstance, MAKEINTRESOURCE(105));
    hBitmapX = LoadBitmap(hInstance, MAKEINTRESOURCE(104));
    hBitmapO = LoadBitmap(hInstance, MAKEINTRESOURCE(103));
    hBitmapStart = LoadBitmap(hInstance, MAKEINTRESOURCE(107));
    hBitmapReset = LoadBitmap(hInstance, MAKEINTRESOURCE(106));
  }
  return TRUE;
  //GWL_WNDPROC -4
  case WM_COMMAND:
    switch (HIWORD(wParam))
    {
    case BN_CLICKED:
      switch (LOWORD(wParam))
      {
      case IDC_BUTTON1: 
      {
        isGameOn=!isGameOn;
        HDC hdc = GetDC(hwndDig);
        drawButton(hdc);
        if (isGameOn == false)
        {
          drawBoard(hdc);
          for (int i = 0; i < 9; i++)
            Array[i] = 0;
        }
        ReleaseDC(hwndDig, hdc);
        return TRUE;
      }
        return TRUE;
      }
      return FALSE;
    }
    return FALSE;
  case WM_LBUTTONDOWN:
  {
    if (isGameOn == true)
    {
      int x = LOWORD(lParam), y = HIWORD(lParam);
      if ((x >= 0 && x <= 500) && (y >= 0 && y <= 364))
      {
        int filedX = ((x - 0) / 167);
        int filedY = ((y - 0) / 122);
        if (Array[filedX * 3 + filedY] == 0)
        {
          x = filedX * 167 + 0;
          y = filedY * 122 + 0;//x=((x-iMinBoardX/iWidthBorardX)*iWidthX+iMinBoardX+iWidthBoardX)
          HDC hdc = GetDC(hwndDig);
          if (isFirstPlayerTurn == true)
          {
            drawX(hdc, x, y);
            Array[3 * filedX + filedY] = 1;
          }
          else
          {
            drawO(hdc, x, y);
            Array[3 * filedX + filedY] = -1;
          }
          ReleaseDC(hwndDig, hdc);
          isFirstPlayerTurn = !isFirstPlayerTurn;
        }
      }
    }
    a = CheckResult();
    Result(hwndDig, a);
  }
  //wsprintf(szText, "Kliknales myszka x=%d, y=%d", LOWORD(lParam), HIWORD(lParam));
 // MessageBox(hwndDig, szText, TEXT("Klikniecie"), MB_OK);
  return TRUE;
  case WM_PAINT:
  {
    UpdateWindow(hwndDig);
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwndDig, &ps);
    drawBoard(hdc);
    isGameOn = false;
    drawButton(hdc);
    //SendDlgItemMessage(hwndDig, IDC_BUTTON1, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmapStart);
    int x, y;
    for (int i = 0; i < 9; i++)
    {
      switch (i / 3)
      {
      case 0: x = 0; break;
      case 1: x = 167; break;
      case 2: x = 334; break;
      }
      switch (i % 3)
      {
      case 0: y = 0; break;
      case 1: y = 122; break;
      case 2: y = 244; break;
      }
      if (Array[i] != 0)
        Array[i] == 1 ? drawX(hdc, x, y) : drawO(hdc, x, y);
    }
    ReleaseDC(hwndDig, hdc);
    EndPaint(hwndDig, &ps);
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
  Okno = GetDC(hwndMainWindow);
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}

void drawX(HDC hdc, int x, int y)
{
  HDC hDCBitmap;
  hDCBitmap = CreateCompatibleDC(hdc);
  SelectObject(hDCBitmap, hBitmapX);
  BitBlt(hdc, x + 36, y + 5, 95, 110, hDCBitmap, 0, 0, SRCCOPY);
  DeleteDC(hDCBitmap);
}
void drawO(HDC hdc, int x, int y)
{
  HDC hDCBitmap;
  hDCBitmap = CreateCompatibleDC(hdc);
  SelectObject(hDCBitmap, hBitmapO);
  BitBlt(hdc, x + 27, y + 5, 112, 110, hDCBitmap, 0, 0, SRCCOPY);
  DeleteDC(hDCBitmap);
}
void drawBoard(HDC hdc)
{
  HDC hDCBitmap;
  hDCBitmap = CreateCompatibleDC(hdc);
  SelectObject(hDCBitmap, hBitmapGameBoard);
  StretchBlt(hdc, 0, 0, 500, 364, hDCBitmap, 0, 0, 500, 364, SRCCOPY);
  drawButton(hdc);
  DeleteDC(hDCBitmap);
}
void drawButton(HDC hdc)
{
  HDC hDCBitmap;
  hDCBitmap = CreateCompatibleDC(hdc);
  if (isGameOn == false)
  {
    SelectObject(hDCBitmap, hBitmapStart);
    BitBlt(hdc, 535, 135, 144, 107, hDCBitmap, 0, 0, SRCCOPY);
  }
  else
  {
    SelectObject(hDCBitmap, hBitmapReset);
    BitBlt(hdc, 535, 135, 144, 107, hDCBitmap, 0, 0, SRCCOPY);
  }
  DeleteDC(hDCBitmap);
}

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
  switch (a)
  {
  case wygrywaKolko:
    wsprintf(szText, "Wygrywa kolko");
    //isGameOn = false;
    MessageBox(hwndDig, szText, TEXT("Rezultat"), MB_OK);
    break;
  case wygrywaKrzyzyk:
    wsprintf(szText, "Wygrywa krzyzyk");
    //isGameOn = false;
    MessageBox(hwndDig, szText, TEXT("Rezultat"), MB_OK);
    break;
  case remis:
    wsprintf(szText, "Remis");
    //isGameOn = false;
    MessageBox(hwndDig, szText, TEXT("Rezultat"), MB_OK);
    break;
  case nierozstrzygniety:;
  default:;
  }
}

void WstawPrzyciski(HWND hwndMainWindow)
{
  HDC hdcNowy = CreateCompatibleDC(Okno);
  HDC hdc = GetDC(hwndMainWindow);
  HBITMAP hbmOld = (HBITMAP)SelectObject(hdcNowy, hBitmapStart);
  BITMAP bmInfo;
  hdcNowy = CreateCompatibleDC(Okno);
  hdc = GetDC(hwndMainWindow);
  SelectObject(hdcNowy, hBitmapStart);
  hbmOld = (HBITMAP)SelectObject(hdcNowy, hBitmapStart);
  GetObject(hBitmapStart, sizeof(bmInfo), &bmInfo);
  BitBlt(hdc, 535, 135, 144, 107, hdcNowy, 0, 0, SRCCOPY);
  ReleaseDC(hwndMainWindow, hdc);
  SelectObject(hdcNowy, hbmOld);
  DeleteDC(hdcNowy);
}