#include <GUI.h>
#include <LCDConf.h>
#define GUI_TOPLINE     30   
#define GUI_BCOLOR      GUI_BLACK   //背景色
#define GUI_FCOLOR      GUI_YELLOW  //前景色
#define TORANG_OK       GUI_GREEN   //合格
#define TORANG_NOK      GUI_RED    //不合格
#define TORANG_HI       GUI_RED    //偏高
#define TORANG_LO       GUI_YELLOW //偏低
#define TORANG_RST      GUI_GRAY   //初始状态
typedef enum
{
	Region_Wifi=0,
	Region_Title,
	Region_Battery,
	Region_Center,
	Region_Job,
	Region_Batch,
	Region_End//表示区域个数

}RectRegion;
typedef enum
{
	Battery0 = 0,
	Battery1,
	Battery2,
	Battery3,
	Battery4
}BatteryLevel;
typedef enum
{
	Wifi0 = 0,
	Wifi1,
	Wifi2,
	Wifi3,
	Wifi4
}WifiLevel;
typedef struct
{
	int width;
	int height;
	int x0;
	int y0;
	int x1;
	int y1;
}GUI_Region;
GUI_Region lcdSize;
GUI_RECT rectRegions[Region_End];
int rectReginWidths[Region_End] = { 60,200,60,320,200,120 };
void splash()
{
	GUI_SetFont(GUI_FONT_16_ASCII);
	GUI_ClearRectEx(&rectRegions[Region_Center]);
	GUI_DispStringInRect("Version:1.83", &rectRegions[Region_Center], GUI_TA_HCENTER | GUI_TA_VCENTER);
	GUI_Delay(500);
	GUI_ClearRectEx(&rectRegions[Region_Center]);
	GUI_DispStringInRect("Serial:090815678", &rectRegions[Region_Center], GUI_TA_HCENTER | GUI_TA_VCENTER);
	GUI_Delay(500);
	GUI_ClearRectEx(&rectRegions[Region_Center]);
	GUI_DispStringInRect("Sapn:120 Nm", &rectRegions[Region_Center], GUI_TA_HCENTER | GUI_TA_VCENTER);
	GUI_Delay(500);
	GUI_ClearRectEx(&rectRegions[Region_Center]);
	GUI_DispStringInRect("Battery:100%", &rectRegions[Region_Center], GUI_TA_HCENTER | GUI_TA_VCENTER);
	GUI_Delay(500);
	GUI_ClearRectEx(&rectRegions[Region_Center]);
	GUI_DispStringInRect("Zero....", &rectRegions[Region_Center], GUI_TA_HCENTER | GUI_TA_VCENTER);
	//开始校准，校准完毕退出，进入主屏幕
}
void drawWifi(WifiLevel Wifix)
{
	GUI_ClearRectEx(&rectRegions[Region_Wifi]);
	GUI_SetPenSize(2);
	int centerx = rectReginWidths[Region_Wifi] / 2-8;
	int rGradient = 10;
	for (int index = Wifi2; index <= Wifix; index++)
	{
		GUI_DrawArc(centerx, GUI_TOPLINE-5, rGradient,  rGradient, 45, 135);
		rGradient += 7;
	}
	if (Wifix > Wifi0)
	{
		GUI_FillCircle(centerx, GUI_TOPLINE - 5, 2);
	}
	GUI_SetPenSize(1);
}
void drawBattery(BatteryLevel Batteryx)
{
	GUI_ClearRectEx(&rectRegions[Region_Battery]);
	GUI_SetPenSize(2);
	GUI_DrawRoundedFrame(257, 8, 262, 20, 1, 2);
	GUI_DrawRoundedFrame(262, 2, 317, 27, 3, 2);
	// 3 10 2
	int y0 = 304;
	if (Batteryx <= Battery1)
	{
		GUI_SetColor(GUI_RED);
	}
	else
	{
		GUI_SetColor(GUI_YELLOW);
	}
	for (int index = Battery1; index <= Batteryx; index++)
	{
		GUI_FillRoundedRect(y0, 5, y0+6, 24, 1);
		y0 -= 12;	
	}
	if (Batteryx == Battery0)
	{
		GUI_DrawLine(262, 2, 317, 27);
		GUI_DrawLine(262, 27, 317, 2);
	}
	GUI_SetPenSize(1);
	GUI_SetColor(GUI_FCOLOR);
}
void drawData()
{

}
void createRegion()
{
	//Wifi->Title->Battery
	int width = 0;
	for (int index = Region_Wifi; index <= Region_Battery; index++)
	{
		rectRegions[index].x0 = 0+width;
		rectRegions[index].y0 = 0;
		rectRegions[index].x1 = rectReginWidths[index]+ rectRegions[index].x0-1;
		rectRegions[index].y1 = GUI_TOPLINE - 1;
		width += rectReginWidths[index];
	}
	GUI_DrawHLine(GUI_TOPLINE, lcdSize.x0, lcdSize.x1);
	//center
	width = 0;
	rectRegions[Region_Center].x0 = 0 + width;
	rectRegions[Region_Center].y0 = GUI_TOPLINE+1;
	rectRegions[Region_Center].x1 = rectReginWidths[Region_Center] + rectRegions[Region_Center].x0 - 1;
	rectRegions[Region_Center].y1 = lcdSize.height-GUI_TOPLINE - 2;
	GUI_DrawHLine(lcdSize.height - GUI_TOPLINE - 1, lcdSize.x0, lcdSize.x1);
	//Job ->Batch
	width = 0;
	for (int index = Region_Job; index <= Region_Batch; index++)
	{
		rectRegions[index].x0 = 0 + width;
		rectRegions[index].y0 = lcdSize.height-GUI_TOPLINE;
		rectRegions[index].x1 = rectReginWidths[index] + rectRegions[index].x0 - 1;
		rectRegions[index].y1 = lcdSize.height - 1;
		width += rectReginWidths[index];
	}
		 
}
void init()
{ 
	int width = GUI_GetScreenSizeX();
	int height = GUI_GetScreenSizeY();
	GUI_Region region = { width,height,0,0,width - 1,height - 1 };
	lcdSize=region;
	GUI_SetBkColor(GUI_BCOLOR);
	GUI_SetColor(GUI_FCOLOR);
	createRegion();
	
	//Loading 
	//底层
	drawWifi(Wifi4);
	drawBattery(Battery4);
	splash();
}
void MainTask(void)
{
	GUI_SetAfterInitHook(init);
	GUI_Init();
	GUI_SetFont(GUI_FONT_16_ASCII);
	GUI_DispStringInRect("1:(Pset1)",&rectRegions[Region_Title], GUI_TA_HCENTER| GUI_TA_VCENTER);
	 
	
	int index = 0;
	while (1)
	{
		drawWifi(index);
		drawBattery(index);
		index++;
		if (index > Wifi4)
		{
			index = 0;
		}
		GUI_Delay(500);
	}
}