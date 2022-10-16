#include <cdefBF533.h>
//#include "sys_res.h"
//#include "tsc2200_Marco.h"

segment("sdram0_bank0")unsigned char Buffer01[320*3*262]; 
segment("sdram0_bank1")unsigned char Buffer02[320*3*262]; 
segment("sdram0_bank2")unsigned char bmpSequence[20][230456];//230456=320x240x3+56
segment("sdram0_bank2")unsigned char dotfonts[75][384];//字库数组

extern void add_text(int txt,unsigned char * Buffer);//增加字幕
extern void add_start(char* txt, unsigned char* buffer);//增加开机字幕
extern void add_buttons(unsigned char* buffer);//增加按钮
extern void ReadFilesAll( );
extern void Fill_bmp_to_Buffer(unsigned char * bmp,unsigned char * Buffer); 
extern void Init_Interrupt_DMA0(void);
extern void Init_PLL();
extern void Init_EBIU();
extern void Init_SDRAM();
extern void LCD_Init_CPLD();
extern void LCD_Init_PPI();
extern void LCD_Init_DMA(unsigned char* buffer);
extern void Fill_Dada_Buffer(unsigned char* buffer);
extern void LCD_Init_Timer();
extern void DMA0_Enable();
extern void PPI_Enable();
extern void Timer_Enable();

extern void home_page_into(int txt,unsigned char * Buffer);
unsigned char * p_LCD_display = Buffer01; 
unsigned char * p_LCD_process = Buffer02; 

int frame_cnt=0;
int fill_image=1;
char start[32]={'T','E','A','M','7','\n','2','0','2','2','\\','7','\\','1','5','\n','g','o','o','d','\\','g','o','o','d','\\','s','t','u','d','y','\n'};
bool start_script = true;//开机字幕显示状态
bool info_state = false;//序号显示状态
bool zoom_state = false;//缩略图显示状态
int home_page_cnt=0;
unsigned short x,y;
void main(void)
{
	Init_PLL();
	Init_EBIU(); 
	Init_SDRAM();
	
	ReadFilesAll(); 
	Fill_bmp_to_Buffer(bmpSequence[ 0 ],Buffer01);
	add_start(start,Buffer01);//增加开机字幕
	add_buttons(Buffer01);
	home_page_into(0,Buffer02);
	
	Init_Interrupt_DMA0();
	
	LCD_Init_CPLD(); 
	LCD_Init_PPI(); 
	LCD_Init_DMA(Buffer01); 
	LCD_Init_Timer();
	TuchScreen_Init(); //初始化touchscreen 
	
	DMA0_Enable(); 
	PPI_Enable(); 
	Timer_Enable();
	
	
	while(1) {
		
		if (TSC2200DataAvailable()) //触摸屏的xy坐标数据转换好了吗(is data available )?
		{
			int i=0;
			x = 0; y = 0;
			TSC2200ReadXY(&x, &y); //读取触摸屏的x,y坐标位置
			TSC2200WriteCFGReg(0); //寄存器清0
			if(fill_image)
			{
				if ((x > 500) && (x < 980) && (y > (2874)) && (y < (3360))) //判断按键<的坐标范围
				{
					if(start_script)//去掉开机字幕
					{
						Fill_bmp_to_Buffer( bmpSequence[ 0 ], p_LCD_process );
						start_script=false;
					}else if(zoom_state)//缩放图的前翻页
					{
						if(home_page_cnt==1){
						home_page_into(1,p_LCD_process);
						home_page_cnt=0;
						
					}
					else if(home_page_cnt==0){
						home_page_into(0,p_LCD_process);
						home_page_cnt=1;
					}
					
					}else{//大图浏览时的前翻页
						if(frame_cnt==0)
						{
							frame_cnt=19;
						}else{
							frame_cnt--;
						}
						Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
						if(info_state)
							add_text(frame_cnt+1,p_LCD_process);
					}
					
					if (p_LCD_display == Buffer01)
					{
						p_LCD_display = Buffer02;
						p_LCD_process = Buffer01;
					}
					else
					{
						p_LCD_display = Buffer01;
						p_LCD_process = Buffer02;
					}

					TuchScreen_Delay(10000000);
				}
				else if ((x > 1340) && (x < 1820) && (y > (2874)) && (y < (3360))) //判断按键home的坐标范围
				{
					if(start_script)//去掉开机字幕
					{
						Fill_bmp_to_Buffer( bmpSequence[ 0 ], p_LCD_process );
						start_script=false;
					}else if(zoom_state)//关闭缩放图
					{
						Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
						if(info_state)
							add_text(frame_cnt+1,p_LCD_process);
					}
					else if(!zoom_state)//打开缩放
					{
						home_page_into(0,p_LCD_process);
						home_page_cnt=1;
					}
					zoom_state=!zoom_state;
					if (p_LCD_display == Buffer01)
					{
						p_LCD_display = Buffer02;
						p_LCD_process = Buffer01;
					}
					else
					{
						p_LCD_display = Buffer01;
						p_LCD_process = Buffer02;
					}

					TuchScreen_Delay(10000000);
				}
				else if ((x > 2180) && (x < 2660) && (y > (2874)) && (y < (3360))) //判断按键i的坐标范围
				{
					if(start_script)//去掉开机字幕
					{
						Fill_bmp_to_Buffer( bmpSequence[ 0 ], p_LCD_process );
						start_script=false;
					}else if(zoom_state)
					{
						
					}else{
						info_state=!info_state;
						Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
						if(info_state)
							add_text(frame_cnt+1,p_LCD_process);
					}
					
					
					if (p_LCD_display == Buffer01)
					{
						p_LCD_display = Buffer02;
						p_LCD_process = Buffer01;
					}
					else
					{
						p_LCD_display = Buffer01;
						p_LCD_process = Buffer02;
					}
					TuchScreen_Delay(10000000);
				}
				else if ((x > 3020) && (x < 3500) && (y > (2874)) && (y < (3360))) //判断按键>的坐标范围
				{
					if(start_script)//去掉开机字幕
					{
						Fill_bmp_to_Buffer( bmpSequence[ 0 ], p_LCD_process );
						start_script=false;
					}else if(zoom_state)
					{
						if(home_page_cnt==1){
							home_page_into(1,p_LCD_process);
							home_page_cnt=0;
						}
						else if(home_page_cnt==0){
							home_page_into(0,p_LCD_process);
							home_page_cnt=1;
						}
					}else{
						//切换到下一帧
						if(frame_cnt==19)
						{
							frame_cnt=0;
						}else{
							frame_cnt++;
						}
						//填充下一帧
						Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
						if(info_state)
							add_text(frame_cnt+1,p_LCD_process);
					}
					
					
					if (p_LCD_display == Buffer01)
					{
						p_LCD_display = Buffer02;
						p_LCD_process = Buffer01;
					}
					else
					{
						p_LCD_display = Buffer01;
						p_LCD_process = Buffer02;
					}
					TuchScreen_Delay(10000000);
				}
				
				else if ((x > 0) && (x < 980) && (y > (0)) && (y < (973))){
					if(zoom_state){
						if(home_page_cnt==1){
							frame_cnt=0;
							Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
							if(info_state)
								add_text(frame_cnt+1,p_LCD_process);
							if (p_LCD_display == Buffer01)
							{
								p_LCD_display = Buffer02;
								p_LCD_process = Buffer01;
							}
							else
							{
								p_LCD_display = Buffer01;
								p_LCD_process = Buffer02;
							}
							zoom_state=!zoom_state;
						}
						else if(home_page_cnt==0){
							frame_cnt=12;
							Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
							if(info_state)
								add_text(frame_cnt+1,p_LCD_process);
							if (p_LCD_display == Buffer01)
							{
								p_LCD_display = Buffer02;
								p_LCD_process = Buffer01;
							}
							else
							{
								p_LCD_display = Buffer01;
								p_LCD_process = Buffer02;
							}
							zoom_state=!zoom_state;
						}
					}
				
					TuchScreen_Delay(10000000);
				}
				else if ((x > 982) && (x < 1964) && (y > (0)) && (y < (973))){
					if(zoom_state){
						if(home_page_cnt==1){
							frame_cnt=1;
							Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
							if(info_state)
								add_text(frame_cnt+1,p_LCD_process);
							if (p_LCD_display == Buffer01)
							{
								p_LCD_display = Buffer02;
								p_LCD_process = Buffer01;
							}
							else
							{
								p_LCD_display = Buffer01;
								p_LCD_process = Buffer02;
							}
							zoom_state=!zoom_state;
						}
						else if(home_page_cnt==0){
							frame_cnt=13;
							Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
							if(info_state)
								add_text(frame_cnt+1,p_LCD_process);
							if (p_LCD_display == Buffer01)
							{
								p_LCD_display = Buffer02;
								p_LCD_process = Buffer01;
							}
							else
							{
								p_LCD_display = Buffer01;
								p_LCD_process = Buffer02;
							}
							zoom_state=!zoom_state;
						}
					}
				
					TuchScreen_Delay(10000000);
				}
				else if ((x > 1964) && (x < 2946) && (y > (0)) && (y < (973))){
					if(zoom_state){
						if(home_page_cnt==1){
							frame_cnt=2;
							Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
							if(info_state)
								add_text(frame_cnt+1,p_LCD_process);
							if (p_LCD_display == Buffer01)
							{
								p_LCD_display = Buffer02;
								p_LCD_process = Buffer01;
							}
							else
							{
								p_LCD_display = Buffer01;
								p_LCD_process = Buffer02;
							}
							zoom_state=!zoom_state;
						}
						else if(home_page_cnt==0){
							frame_cnt=14;
							Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
							if(info_state)
								add_text(frame_cnt+1,p_LCD_process);
							if (p_LCD_display == Buffer01)
							{
								p_LCD_display = Buffer02;
								p_LCD_process = Buffer01;
							}
							else
							{
								p_LCD_display = Buffer01;
								p_LCD_process = Buffer02;
							}
							zoom_state=!zoom_state;
						}
					}
				
					TuchScreen_Delay(10000000);
				}
				else if ((x > 2946) && (x < 4096) && (y > (0)) && (y < (973))){
					if(zoom_state){
						if(home_page_cnt==1){
							frame_cnt=3;
							Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
							if(info_state)
								add_text(frame_cnt+1,p_LCD_process);
							if (p_LCD_display == Buffer01)
							{
								p_LCD_display = Buffer02;
								p_LCD_process = Buffer01;
							}
							else
							{
								p_LCD_display = Buffer01;
								p_LCD_process = Buffer02;
							}
							zoom_state=!zoom_state;
						}
						else if(home_page_cnt==0){
							frame_cnt=15;
							Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
							if(info_state)
								add_text(frame_cnt+1,p_LCD_process);
							if (p_LCD_display == Buffer01)
							{
								p_LCD_display = Buffer02;
								p_LCD_process = Buffer01;
							}
							else
							{
								p_LCD_display = Buffer01;
								p_LCD_process = Buffer02;
							}
							zoom_state=!zoom_state;
						}
					}
				
					TuchScreen_Delay(10000000);
				}
				else if ((x > 0) && (x < 982) && (y > (973)) && (y < (1916))){
					if(zoom_state){
						if(home_page_cnt==1){
							frame_cnt=4;
							Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
							if(info_state)
								add_text(frame_cnt+1,p_LCD_process);
							if (p_LCD_display == Buffer01)
							{
								p_LCD_display = Buffer02;
								p_LCD_process = Buffer01;
							}
							else
							{
								p_LCD_display = Buffer01;
								p_LCD_process = Buffer02;
							}
							zoom_state=!zoom_state;
						}
						else if(home_page_cnt==0){
							frame_cnt=16;
							Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
							if(info_state)
								add_text(frame_cnt+1,p_LCD_process);
							if (p_LCD_display == Buffer01)
							{
								p_LCD_display = Buffer02;
								p_LCD_process = Buffer01;
							}
							else
							{
								p_LCD_display = Buffer01;
								p_LCD_process = Buffer02;
							}
							zoom_state=!zoom_state;
						}
					}
				
					TuchScreen_Delay(10000000);
				}
				else if ((x > 982) && (x < 1964) && (y > (973)) && (y < (1916))){
					if(zoom_state){
						if(home_page_cnt==1){
							frame_cnt=5;
							Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
							if(info_state)
								add_text(frame_cnt+1,p_LCD_process);
							if (p_LCD_display == Buffer01)
							{
								p_LCD_display = Buffer02;
								p_LCD_process = Buffer01;
							}
							else
							{
								p_LCD_display = Buffer01;
								p_LCD_process = Buffer02;
							}
							zoom_state=!zoom_state;
						}
						else if(home_page_cnt==0){
							frame_cnt=17;
							Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
							if(info_state)
								add_text(frame_cnt+1,p_LCD_process);
							if (p_LCD_display == Buffer01)
							{
								p_LCD_display = Buffer02;
								p_LCD_process = Buffer01;
							}
							else
							{
								p_LCD_display = Buffer01;
								p_LCD_process = Buffer02;
							}
							zoom_state=!zoom_state;
						}
					}
				
					TuchScreen_Delay(10000000);
				}
				else if ((x > 1964) && (x < 2946) && (y > (973)) && (y < (1916))){
					if(zoom_state){
						if(home_page_cnt==1){
							frame_cnt=6;
							Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
							if(info_state)
								add_text(frame_cnt+1,p_LCD_process);
							if (p_LCD_display == Buffer01)
							{
								p_LCD_display = Buffer02;
								p_LCD_process = Buffer01;
							}
							else
							{
								p_LCD_display = Buffer01;
								p_LCD_process = Buffer02;
							}
							zoom_state=!zoom_state;
						}
						else if(home_page_cnt==0){
							frame_cnt=18;
							Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
							if(info_state)
								add_text(frame_cnt+1,p_LCD_process);
							if (p_LCD_display == Buffer01)
							{
								p_LCD_display = Buffer02;
								p_LCD_process = Buffer01;
							}
							else
							{
								p_LCD_display = Buffer01;
								p_LCD_process = Buffer02;
							}
							zoom_state=!zoom_state;
						}
					}
				
					TuchScreen_Delay(10000000);
				}
				else if ((x > 2946) && (x < 4096) && (y > (973)) && (y < (1916))){
					if(zoom_state){
						if(home_page_cnt==1){
							frame_cnt=7;
							Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
							if(info_state)
								add_text(frame_cnt+1,p_LCD_process);
							if (p_LCD_display == Buffer01)
							{
								p_LCD_display = Buffer02;
								p_LCD_process = Buffer01;
							}
							else
							{
								p_LCD_display = Buffer01;
								p_LCD_process = Buffer02;
							}
							zoom_state=!zoom_state;
						}
						else if(home_page_cnt==0){
							frame_cnt=19;
							Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
							if(info_state)
								add_text(frame_cnt+1,p_LCD_process);
							if (p_LCD_display == Buffer01)
							{
								p_LCD_display = Buffer02;
								p_LCD_process = Buffer01;
							}
							else
							{
								p_LCD_display = Buffer01;
								p_LCD_process = Buffer02;
							}
							zoom_state=!zoom_state;
						}
					}
				
					TuchScreen_Delay(10000000);
				}
				else if ((x > 0) && (x < 982) && (y > (1916)) && (y < (2858))){
					if(zoom_state){
						if(home_page_cnt==1){
							frame_cnt=8;
							Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
							if(info_state)
								add_text(frame_cnt+1,p_LCD_process);
							if (p_LCD_display == Buffer01)
							{
								p_LCD_display = Buffer02;
								p_LCD_process = Buffer01;
							}
							else
							{
								p_LCD_display = Buffer01;
								p_LCD_process = Buffer02;
							}
							zoom_state=!zoom_state;
						}
					}
				
					TuchScreen_Delay(10000000);
				}
				else if ((x > 982) && (x < 1964) && (y > (1916)) && (y < (2858))){
					if(zoom_state){
						if(home_page_cnt==1){
							frame_cnt=9;
							Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
							if(info_state)
								add_text(frame_cnt+1,p_LCD_process);
							if (p_LCD_display == Buffer01)
							{
								p_LCD_display = Buffer02;
								p_LCD_process = Buffer01;
							}
							else
							{
								p_LCD_display = Buffer01;
								p_LCD_process = Buffer02;
							}
							zoom_state=!zoom_state;
						}
					}
				
					TuchScreen_Delay(10000000);
				}
				else if ((x > 1964) && (x < 2946) && (y > (1916)) && (y < (2858))){
					if(zoom_state){
						if(home_page_cnt==1){
							frame_cnt=10;
							Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
							if(info_state)
								add_text(frame_cnt+1,p_LCD_process);
							if (p_LCD_display == Buffer01)
							{
								p_LCD_display = Buffer02;
								p_LCD_process = Buffer01;
							}
							else
							{
								p_LCD_display = Buffer01;
								p_LCD_process = Buffer02;
							}
							zoom_state=!zoom_state;
						}
					}
				
					TuchScreen_Delay(10000000);
				}
				else if ((x > 2946) && (x < 4096) && (y > (1916)) && (y < (2858))){
					if(zoom_state){
						if(home_page_cnt==1){
							frame_cnt=11;
							Fill_bmp_to_Buffer( bmpSequence[ frame_cnt ], p_LCD_process );
							if(info_state)
								add_text(frame_cnt+1,p_LCD_process);
							if (p_LCD_display == Buffer01)
							{
								p_LCD_display = Buffer02;
								p_LCD_process = Buffer01;
							}
							else
							{
								p_LCD_display = Buffer01;
								p_LCD_process = Buffer02;
							}
							zoom_state=!zoom_state;
						}
					}
					
				
					TuchScreen_Delay(10000000);
				}
				fill_image=0;
			}	
		}	
	}
}
