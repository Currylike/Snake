}/*------------------------------------------------------------
    this program design by Bug 
   ------------------------------------------------------------*/
#include "StdAfx.h" 
 #include <WINDOWS.H>
 #include <STDIO.H>
 #include <STRING.H>
 #include <TIME.H> 
 #include <STDLIB.H>

 
  
 
 #define DRAFRAMENUM 62                //Drawn framework of rect ' number           
 #define ID_TIME 1					 //Time ID
 #define RECTDISTANCE 10             //Every rectangular size
 #define MAXSNAKELEN 60             //Limit snake's length
 #define BOSS 92
 
  
 
 typedef struct POSITION
 {
  int xLeft ;
  int yTop ;
  int xRight ;
  int yBottom ;
 
 }BODYPOS,BEANPOS, * PBODYPOS;
 
 BODYPOS bodyPos[60] = {
  {300,300,310,310},
  {310,300,320,310},    
  {320,300,330,310},   
  {330,300,340,310}
 };        //initialize body of snake
 BEANPOS beanPos;
 
 static int snakeLen = 4,Pause = 0,over = 0,beanMiss = 0;
 
 void bean(void);
 int feed(void);
 void moveJudge(int,int,int,int,PBODYPOS);
 int gameOver();
 LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
 
 int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR szCmdLine, int iCmdShow)
 {
      static TCHAR szAppName[] = TEXT ("snake") ;
      HWND         hwnd ;
      MSG          msg ;
      WNDCLASS     wndclass ;
   int    temp;
   temp = feed();
   beanPos.xLeft = temp;
   beanPos.xRight = temp + RECTDISTANCE;
   beanPos.yTop = temp;
   beanPos.yBottom= temp + RECTDISTANCE; 
 
      wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
      wndclass.lpfnWndProc   = WndProc ;
      wndclass.cbClsExtra    = 0 ;
      wndclass.cbWndExtra    = 0 ;
      wndclass.hInstance     = hInstance ;
      wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
      wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
      wndclass.hbrBackground = NULL ;
      wndclass.lpszMenuName  = NULL ;
      wndclass.lpszClassName = szAppName ;
 
      if (!RegisterClass (&wndclass))
      {
           MessageBox (NULL, TEXT ("This program requires Windows NT!"), 
                       szAppName, MB_ICONERROR) ;
           return 0 ;
      }
      
      hwnd = CreateWindow (szAppName,                // window class name
                           TEXT ("Snake"),               // window caption
                           WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX& ~WS_THICKFRAME,           // window style
                           350,                  // initial x position
                           50,                     // initial y position
                           736,                  // initial x size
                           648,                  // initial y size
                           NULL,                  // parent window handle
                           NULL,                  // window menu handle
                           hInstance,                // program instance handle
                           NULL) ;                 // creation parameters
      
      ShowWindow (hwnd, iCmdShow) ;
      UpdateWindow (hwnd) ;
      
      while (GetMessage (&msg, NULL, 0, 0))
      {
           TranslateMessage (&msg) ;
           DispatchMessage (&msg) ;
      }
      return msg.wParam ;
 }
 
 LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
 {
   HDC         hdc,hMemDC;
   HBITMAP hBitmap ;
   PAINTSTRUCT ps ;
   RECT   rect ;
   HBRUSH   hBrush;
   HFONT   hFont;
   LOGFONT  lf;
   static int  upButton,downButton,leftButton,rightButton;
   int   i,k,xRight,xLeft,yTop,yBottom;
   char   szBuff[10][6];
   UINT ClientWidth,ClientHeight ;
 
 
   xRight  = 0 ;
   yTop  = 0 ;
   xLeft  = 10 ;
   yBottom = 10 ;
   rect.left = 10 ;
   rect.top = 10 ;
   rect.right = 610 ;
   rect.bottom= 610 ;
 
 
   lf.lfHeight  = 60;
   lf.lfWidth   = 60;
   lf.lfWeight  = 1000;
   lf.lfEscapement = 0;
   lf.lfOrientation = 0;
   lf.lfItalic  = 0;
   lf.lfUnderline  = 0;
   lf.lfStrikeOut  = 0;
   lf.lfCharSet  = 1;
 
   snakeLen=min(snakeLen,MAXSNAKELEN);          //insure the snake's length litter than MAXSNAKELEN
 
   switch (message)
   {
   case WM_CREATE:
		SetTimer(hwnd,ID_TIME,500,NULL);          //send WM_TIME message every 0.5 minutes
           break;
   case WM_TIMER:
		InvalidateRect(hwnd,NULL,TRUE);
		break;
   case WM_KEYDOWN:
    switch (wParam)              //remember the state of key
    {
    case VK_UP:
     upButton  = 1 ;
     downButton  = 0 ;
     leftButton  = 0 ;
     rightButton = 0 ;
     break;
    case VK_DOWN:
     upButton  = 0 ;
     downButton  = 1 ;
     leftButton  = 0 ;  
     rightButton = 0 ;
     break;
    case VK_LEFT:
     upButton  = 0 ;
     downButton  = 0 ;
     leftButton  = 1 ;
     rightButton = 0 ;
     break;
    case VK_RIGHT:
     upButton  = 0 ;
     downButton  = 0 ;
     leftButton  = 0 ;
     rightButton = 1 ;
     break;
    case VK_ESCAPE:                   //pause function
     Pause =  1;
	 InvalidateRect(hwnd,&rect,TRUE);
     KillTimer(hwnd,ID_TIME);
     break;
    case 'R':
     if (Pause && !over)
     {
     SendMessage(hwnd,WM_CREATE,wParam,lParam);
     Pause = 0;
     }
     break;
    case VK_SPACE:                    //snake accelerate
     if (!Pause && !over)
     {
      InvalidateRect(hwnd,&rect,TRUE);
     }
     break;
    case VK_RETURN:
      if (over)                  //reset
      {
      SendMessage(hwnd,WM_CREATE,wParam,lParam);
      snakeLen = 4;
      over  = 0;
      bodyPos[0].xLeft = 300;bodyPos[0].yTop = 300;bodyPos[0].xRight = 310;bodyPos[0].yBottom = 310;
      bodyPos[1].xLeft = 310;bodyPos[1].yTop = 300;bodyPos[1].xRight = 320;bodyPos[1].yBottom = 310;
      bodyPos[2].xLeft = 320;bodyPos[2].yTop = 300;bodyPos[2].xRight = 330;bodyPos[2].yBottom = 310;
      bodyPos[3].xLeft = 330;bodyPos[3].yTop = 300;bodyPos[3].xRight = 340;bodyPos[3].yBottom = 310;
      }
    default:
     break;
    }
    break;
      case WM_PAINT:
     hdc = BeginPaint (hwnd, &ps) ;
	 RECT ClientRect;
	 GetClientRect(hwnd,&ClientRect);
	 ClientWidth=ClientRect.right-ClientRect.left;
	 ClientHeight=ClientRect.bottom-ClientRect.top;

	 hdc		= GetDC(hwnd);					//创建屏幕DC
	 hMemDC		= CreateCompatibleDC(hdc);		//创建内存DC
	 hBitmap	= CreateCompatibleBitmap(hdc,ClientWidth,ClientHeight);//创建兼容位图内存块,存放图片数据
	 SelectObject(hMemDC,hBitmap);	
     
	 
	 
	 SetBkMode(hMemDC,TRANSPARENT);                //background attribute
    SetTextColor(hMemDC,RGB(250,2,2));               //text color
     hBrush = CreateSolidBrush(RGB(0,0,200));
     SelectObject(hMemDC,hBrush);
     
     TextOut(hMemDC,650,60,szBuff[0],wsprintf(szBuff[0],"%s","function:"));
     TextOut(hMemDC,640,80,szBuff[1],wsprintf(szBuff[1],"%s","ESC to pause"));
     TextOut(hMemDC,640,100,szBuff[2],wsprintf(szBuff[2],"%s","R to resume"));
     TextOut(hMemDC,630,120,szBuff[3],wsprintf(szBuff[3],"%s","Enter to revive"));
     TextOut(hMemDC,635,200,szBuff[4],wsprintf(szBuff[4],"%s","direction key"));
     TextOut(hMemDC,640,220,szBuff[5],wsprintf(szBuff[5],"%s","↑↓←→"));
     TextOut(hMemDC,635,400,szBuff[7],wsprintf(szBuff[7],"%s  %d","Score :",snakeLen * RECTDISTANCE-40));
     TextOut(hMemDC,640,480,szBuff[2],wsprintf(szBuff[2],"%s","By :Bug"));
 
      for (k=0;k<DRAFRAMENUM;k++)                 //drawn framework
      {
       Rectangle(hMemDC,xLeft,yTop+(k*RECTDISTANCE),xRight,yBottom+(k*RECTDISTANCE));
      }
      for (k=0;k<DRAFRAMENUM;k++)
      {
       Rectangle(hMemDC,xLeft+(k*RECTDISTANCE),yTop,xRight+(k*RECTDISTANCE),yBottom);
      }
      xLeft  = 610 ;							//rotate painting dirction
      xRight = 620 ; 
      yTop  = 610 ;
      yBottom = 620 ;
      for (k=0;k<DRAFRAMENUM;k++)
      {
       Rectangle(hMemDC,xLeft,yTop-(k*RECTDISTANCE),xRight,yBottom-(k*RECTDISTANCE));
      }
      for (k=0;k<DRAFRAMENUM;k++)
      {
       Rectangle(hMemDC,xLeft-(k*RECTDISTANCE),yTop,xRight-(k*RECTDISTANCE),yBottom);
      }
	      DeleteObject(hBrush);
 
     if(!Pause)                     //drawn snake
     {
      hBrush = CreateSolidBrush(RGB(255,0,0));
      SelectObject(hMemDC,hBrush); 
 
      moveJudge(upButton,downButton,leftButton,rightButton,bodyPos);
      for (i=0;i<snakeLen;i++)
      {            
       Rectangle(hMemDC,bodyPos[i].xLeft,bodyPos[i].yTop,bodyPos[i].xRight,bodyPos[i].yBottom);
      }
      DeleteObject(hBrush);
 
 
      hBrush = CreateSolidBrush(RGB(255,140,0));
      SelectObject(hMemDC,hBrush);
      if (BOSS == snakeLen)
      {
       hBrush = CreateSolidBrush(RGB(0,0,0));
       SelectObject(hMemDC,hBrush);
      } 
      Rectangle(hMemDC,beanPos.xLeft,beanPos.yTop,beanPos.xRight,beanPos.yBottom);   //drawn bean
      DeleteObject(hBrush);
 
      bean();
     }else{
		hFont = CreateFontIndirect(&lf);
		SelectObject(hMemDC,hFont);
		TextOut(hMemDC,200,230,szBuff [9],wsprintf(szBuff[9],"%s","Pause"));
		DeleteObject(hFont);
	 }
     if(gameOver())
     {
      hFont = CreateFontIndirect(&lf);
      SelectObject(hMemDC,hFont);
      TextOut(hMemDC,200,180,szBuff [6],wsprintf(szBuff[6],"%s","Game"));
      TextOut(hMemDC,200,250,szBuff [7],wsprintf(szBuff[7],"%s","Over"));
      KillTimer(hwnd,ID_TIME);
      over = 1;
      DeleteObject(hFont);
     }
	 BitBlt(hdc,0,0,ClientWidth,ClientHeight,hMemDC,0,0,SRCCOPY);
     EndPaint (hwnd, &ps) ;
           return 0 ;
      case WM_DESTROY:
           PostQuitMessage (0) ;
           return 0 ; 
      }
      return DefWindowProc (hwnd, message, wParam, lParam) ;
 }
 
 void moveJudge(int upButton,int downButton,int leftButton,int rightButton,PBODYPOS pBodyPos)
 {
  BODYPOS temp[60];
 
  if(0 == pBodyPos[0].xLeft - pBodyPos[1].xLeft )          //judge the snake ' moveing way
  {
   if(rightButton)                 
   {
    memcpy(&pBodyPos[1],pBodyPos,sizeof(bodyPos)-sizeof(BODYPOS));
    pBodyPos[0].xLeft = pBodyPos[0].xLeft + RECTDISTANCE;
    pBodyPos[0].xRight = pBodyPos[0].xRight + RECTDISTANCE;
   }else if (leftButton)
   {
    memcpy(&pBodyPos[1],pBodyPos,sizeof(bodyPos)-sizeof(BODYPOS));
    pBodyPos[0].xLeft = pBodyPos[0].xLeft - RECTDISTANCE;
    pBodyPos[0].xRight = pBodyPos[0].xRight - RECTDISTANCE;
   }else if(pBodyPos[0].yTop - pBodyPos[1].yTop <0)
   {
    memcpy(&pBodyPos[1],pBodyPos,sizeof(bodyPos)-sizeof(BODYPOS));
    pBodyPos[0].yTop = pBodyPos[0].yTop - RECTDISTANCE;
    pBodyPos[0].yBottom = pBodyPos[0].yBottom - RECTDISTANCE;
   }else
   {
    memcpy(&pBodyPos[1],pBodyPos,sizeof(bodyPos)-sizeof(BODYPOS));
    pBodyPos[0].yTop = pBodyPos[0].yTop + RECTDISTANCE;
    pBodyPos[0].yBottom = pBodyPos[0].yBottom + RECTDISTANCE;
   }
  }else
  {
 
   if(upButton)
   {
    memcpy(&pBodyPos[1],pBodyPos,sizeof(bodyPos)-sizeof(BODYPOS));
    pBodyPos[0].yTop = pBodyPos[0].yTop - RECTDISTANCE;
    pBodyPos[0].yBottom = pBodyPos[0].yBottom - RECTDISTANCE;    
   }else if (downButton)
   {
    memcpy(&pBodyPos[1],pBodyPos,sizeof(bodyPos)-sizeof(BODYPOS));
    pBodyPos[0].yTop = pBodyPos[0].yTop + RECTDISTANCE;
    pBodyPos[0].yBottom = pBodyPos[0].yBottom + RECTDISTANCE;
   }else if(pBodyPos[0].xLeft - pBodyPos[1].xLeft <0)
   {
    memcpy(&pBodyPos[1],pBodyPos,sizeof(bodyPos)-sizeof(BODYPOS));
    pBodyPos[0].xLeft = pBodyPos[0].xLeft - RECTDISTANCE;
    pBodyPos[0].xRight = pBodyPos[0].xRight - RECTDISTANCE;
   }else
   {
    memcpy(&pBodyPos[1],pBodyPos,sizeof(bodyPos)-sizeof(BODYPOS));
    pBodyPos[0].xLeft = pBodyPos[0].xLeft + RECTDISTANCE;
    pBodyPos[0].xRight = pBodyPos[0].xRight + RECTDISTANCE;
   }
  }
 }
 int gameOver(void)
 { 
  int i,xLeft,xRight,yTop,yBottom;
  xLeft  = 0 ;
  yTop  = 0 ;                    
  xRight  = 620 ; 
  yBottom  = 620 ;
 
 
  for(i=1;i<snakeLen;i++)                     //idioctonia
  {
   if ((bodyPos[0].xLeft == bodyPos[i].xLeft) && (bodyPos[0].yTop == bodyPos[i].yTop))
    return 1;
  }
  if (xLeft == bodyPos[0].xLeft || xRight == bodyPos[0].xRight)           //knock snake' s head against a brick wall
  {
   return 1;
  }else if(yTop == bodyPos[0].yTop || yBottom == bodyPos[0].yBottom)
   return 1;
 
  return 0;
 
 }
 void bean()
 {
  int i;
 
  if (beanPos.xLeft == bodyPos[0].xLeft && beanPos.yTop == bodyPos[0].yTop)
   beanMiss = 1;
  if(beanMiss)
  {
   while(beanMiss)
   {
    beanPos.xLeft   = feed();
    beanPos.xRight  = beanPos.xLeft + RECTDISTANCE;
    beanPos.yTop    = beanPos.xLeft;
    beanPos.yBottom  = beanPos.xRight;
    for (i=0;i<snakeLen;i++)
    {
     if (beanPos.xLeft != bodyPos[i].xLeft && beanPos.yTop != bodyPos[i].yTop )
     {
      beanMiss = 0;
      break;
     }
    }
   }
  snakeLen++;
  }
 }
 
 int feed(void)
 {
  int temp,i;
 
  srand((int)time(0));
 
  for (i=0;i<10;i++)
   temp =rand()%611;
  if (temp<10)
  {
   temp+=10;
  }
  if (0 != temp % 10)
  {
   while(0 != temp % 10)
   {
    temp--;
    if (0 == temp % 10)
     break;
   }
  }
  return temp;
 }