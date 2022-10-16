#include <cdefBF533.h>
#include <sys\exception.h>
#include <stdio.h>

extern unsigned char * p_LCD_display;
extern unsigned char * p_LCD_process;
extern int fill_image;
extern unsigned char bmpSequence[20][230456];
extern unsigned char dotfonts[75][384];

//���������Ļ
void add_text(int txt, unsigned char* Buffer)
{
	unsigned char* p0, * p1, * p2, * p, * pout;
	p0 = dotfonts[0];
	p1 = dotfonts[1];
	p2 = dotfonts[2];
	

	pout = Buffer;
	pout += 960 * 18;//������Ч��
	int i, j;

	if (txt < 10)
	{// 01-09
		p = dotfonts[txt];
		for (i = 0; i < 24; i++)
		{
			pout += 960 - 16 * 3 * 2;//������ĩ��ʼд���Ŵ�
			for (j = 0; j < 16; j++)//д0
			{
				if (*p0 == 0xff)
				{
					*pout = 0xff;
					*(pout + 1) = 0xff;
					*(pout + 2) = 0xff;
				}
				pout += 3;
				p0++;
			}
			for (j = 0; j < 16; j++)
			{
				if (*p == 0xff)
				{
					*pout = 0xff;
					*(pout + 1) = 0xff;
					*(pout + 2) = 0xff;
				}
				pout += 3;
				p++;
			}
		}
	}else if(txt<20)
	{// 10-19
		p = dotfonts[txt - 10];
		for (i = 0; i < 24; i++)
		{
			pout += 960 - 16 * 3 * 2;//������ĩ��ʼд���Ŵ�
			for (j = 0; j < 16; j++)//д1
			{
				if (*p1 == 0xff)
				{
					*pout = 0xff;
					*(pout + 1) = 0xff;
					*(pout + 2) = 0xff;
				}
				pout += 3;
				p1++;
			}
			for (j = 0; j < 16; j++)
			{
				if (*p == 0xff)
				{
					*pout = 0xff;
					*(pout + 1) = 0xff;
					*(pout + 2) = 0xff;
				}
				pout += 3;
				p++;
			}
		}
	}
	else 
	{
		{// 20
			p = dotfonts[txt - 20];
			for (i = 0; i < 24; i++)
			{
				pout += 960 - 16 * 3 * 2;//������ĩ��ʼд���Ŵ�
				for (j = 0; j < 16; j++)//д2
				{
					if (*p2 == 0xff)
					{
						*pout = 0xff;
						*(pout + 1) = 0xff;
						*(pout + 2) = 0xff;
					}
					pout += 3;
					p2++;
				}
				for (j = 0; j < 16; j++)
				{
					if (*p == 0xff)
					{
						*pout = 0xff;
						*(pout + 1) = 0xff;
						*(pout + 2) = 0xff;
					}
					pout += 3;
					p++;
				}
			}
		}
	}

}

//ָ��λ�����ӵ����ַ�(��ɫ)
void add_arrow_grey(char ch, unsigned char* buffer, int x, int y) 
{
	unsigned char* pfont, * pout;
	pfont = dotfonts[ch - 48];
	pout = buffer + 960*18;//������Ч��
	int i, j, k;

	pout += y * 960 + x * 3;
	for (i = 0; i < 24; i++)
	{	
		for (j = 0; j < 16; j++)
		{
			if (*pfont == 0xff)
			{
				*pout = 0xff;
				*(pout + 1) = 0xff;
				*(pout + 2) = 0xff;
			}
			// else{
				// *pout = 128;
				// *(pout + 1) = 128;
				// *(pout + 2) = 128;
			// }
			pout += 3;
			pfont++;
		}
		pout += 960 - 16 * 3;
	}
}

//���� < O i > �ĸ���ť
void add_buttons(unsigned char* buffer)
{
	unsigned char* p=buffer;
	unsigned char ch[4]={'<','O','i','>'};
	int x=36+12;
	int y=177+8;
	int i=0;
	while(i<4)
	{		
		add_arrow_grey(ch[i],p,x,y);
		i++;
		x+=70;
	}
	
}

//���ӿ�����Ļ
void add_start(char* txt, unsigned char* buffer)
{
	int x_pos = 30;
	int y_pos = 30;
	int i;
	for (i = 0; i < 3; i++)
	{
		while ((*txt) != '\n')
		{
			add_single(*txt, buffer, x_pos, y_pos);
			x_pos += 16 ;
			txt++;
		}
		txt++;
		y_pos += 24;
		x_pos = 30;
	}
}

//��ָ��λ�����ӵ����ַ�����ɫ��
void add_single(char ch, unsigned char* buffer, int x, int y) 
{
	unsigned char* pfont, * pout;
	pfont = dotfonts[ch - 48];
	pout = buffer + 960*18;//������Ч��
	int i, j, k;

	pout += y * 960 + x * 3;
	for (i = 0; i < 24; i++)
	{	
		for (j = 0; j < 16; j++)
		{
			if (*pfont == 0xff)
			{
				*pout = 0xff;
				*(pout + 1) = 0xff;
				*(pout + 2) = 0xff;
			}
			pout += 3;
			pfont++;
		}
		pout += 960 - 16 * 3;
	}
}

//��D���ж�ȡbmp�ļ����ֿ��ļ�//
void ReadFilesAll()
{
	FILE *fp;
	if( (fp=fopen("D:\\320x240bmpWithHead20files.dat","rb")) == NULL ){
	    printf("Fail to open file!\n");
	    exit(0);  //�˳����򣨽�������
	}
	
	int i;
	for(i=0;i<20;i++)
	{
		if (fread(bmpSequence[i], 230456, 1, fp) != 1)
	    {
	        printf("read info header error!");
	        exit(0);
	    }
	}
	
	FILE *fp2;
	if( (fp2=fopen("D:\\zk_0-z.dat","rb")) == NULL ){
	    printf("Fail to open file!\n");
	    exit(0);  //�˳����򣨽�������
	}
	if (fread(dotfonts, 75*384, 1, fp2) != 1)
	    {
	        printf("read info header error!");
	        exit(0);
	    }
}

//�����������bmpͼƬ ����������  �ײ������ĸ���ť
void Fill_bmp_to_Buffer(unsigned char * bmp,unsigned char * Buffer)
{//bmp 54+320x240x3+2
 //buffer 320*3*262
	unsigned char* p_in,*p_out;
	p_in=bmp;
	p_out=Buffer;
	int i,j;
	
	p_in+=54;//����bmpͷ��Ϣ
	p_out+=257*960;//������Ч����,�������һ��
	
	for(i=0;i<240;i++)
	{
		for(j=0;j<320;j++)
		{
			*(p_out+2)=*p_in;
			*(p_out+1)=*(p_in+1);
			*(p_out)=*(p_in+2);
			p_in+=3;
			p_out+=3;
		}
		p_out-=2*960;
		
	}
	//�����ĸ���ť
	p_out=Buffer;
	add_buttons(p_out);
}

void DMA0_Enable()
{
	*pDMA0_CONFIG |= 0x0001;
}

EX_INTERRUPT_HANDLER( myPPI_DMA0_ISR )
{ 
	*pDMA0_IRQ_STATUS |= 0x0001;
	 
	*pDMA0_START_ADDR = p_LCD_display;
	DMA0_Enable( );
	fill_image = 1;
}

void Init_Interrupt_DMA0(void)
{
	*pSIC_IAR0 = 0xFFFFFFFF; 
	*pSIC_IAR1 = 0xFFFFFFF1; 
	*pSIC_IAR2 = 0xFFFFFFFF;
	register_handler( ik_ivg8 , myPPI_DMA0_ISR);
	*pSIC_IMASK |= 0x00000100;
}

void Init_PLL()
{
 	unsigned short* p_ppl_ctl,* p_ppl_div;
	p_ppl_ctl = PLL_CTL;
	*p_ppl_ctl = 0x2600;//�ں�ʱ�� 513MHz
	p_ppl_div = PLL_DIV;
	*p_ppl_div = 0x0004;//ϵͳʱ��128.25MHz
	
}
void Init_EBIU()
{
	*pEBIU_AMBCTL0 = 0x7bb07bb0;//�첽�ڴ�����ƼĴ���
	*pEBIU_AMBCTL1 = 0xa4c77bb0;//�������á�ѡͨ�ͱ���ʱ��ļ�����λ
	*pEBIU_AMGCTL = 0x00ff;//�첽�ڴ�ȫ�ֿ��ƼĴ���
	
}
void Init_SDRAM()
{
	*pEBIU_SDRRC = 0x00000817;//SDRAMˢ�����ʿ��ƼĴ���
	*pEBIU_SDBCTL = 0x00000013;//SDRAM�ڴ�����ƼĴ���
	*pEBIU_SDGCTL = 0x0091998d;//SDRAM�ڴ�ȫ�ֿ��ƼĴ���
}

void TFTLCD_Delay(unsigned int Value)
{ 
	unsigned int i=0;
	for(i=0;i<Value;i++)
	{
		asm("nop;");
	}
}
#define pCtrlClrReg1 (volatile unsigned char *)0x203F8000
#define pCtrlClrReg2 (volatile unsigned char *)0x203FC000
#define pCtrlSetReg1 (volatile unsigned char *)0x203C8000
#define pCtrlSetReg2 (volatile unsigned char *)0x203CC000

void LCD_Init_CPLD(void)
{ 	
	*pCtrlSetReg1 = 0xff;
	TFTLCD_Delay(0x100); *pCtrlClrReg1 = 0xff;
	TFTLCD_Delay(0x100); *pCtrlSetReg2 = 0xff;
	TFTLCD_Delay(0x100); *pCtrlClrReg2 = 0xff;
	TFTLCD_Delay(0x100); *pCtrlSetReg2 = 0x03;
	TFTLCD_Delay(0x100); return ;
}

void LCD_Init_PPI()
{
	*pPPI_CONTROL = 0x801E;
	*pPPI_COUNT = 960-1;
	*pPPI_DELAY = 204-1;//204+960+60
}

void LCD_Init_DMA(unsigned char* buffer)
{
	*pDMA0_CONFIG = 0x0090;//����Ϊstopģʽ
	*pDMA0_START_ADDR = buffer;
	*pDMA0_X_COUNT = 960;
	*pDMA0_Y_COUNT = 262;
	*pDMA0_X_MODIFY = 1;
	*pDMA0_Y_MODIFY = 1;
}

void LCD_Init_Timer()
{// TIMER1 ������ͬ���źţ�TIMER2 ����֡ͬ���ź�
	*pTIMER1_CONFIG = 0x02A9; 
	*pTIMER1_PERIOD = 1224;
	*pTIMER1_WIDTH = 90;
	
	*pTIMER2_CONFIG = 0x02A9; 
	*pTIMER2_PERIOD = 262*1224;
	*pTIMER2_WIDTH = 3*1224;
}

void PPI_Enable()
{
	*pPPI_CONTROL |= 0x0001;
}


void Timer_Enable()
{
	*pTIMER_ENABLE = 0x0006;//timer1 timer2 enable
}

void home_page_into(int txt,unsigned char * Buffer){
	unsigned char * p_in;
	unsigned char * p_out;
	int i,j,k,l;
	p_out=Buffer;
	for(i=0;i<251252;i++){
		*(p_out+i)=0;
	}
	add_buttons(p_out);
	if(txt==0){
		for(i=0;i<3;i++){
			for(j=0;j<4;j++){
				p_in=(bmpSequence[i*4+j]+54);
				for(k=0;k<60;k++){
					for(l=0;l<80;l++){
						*(p_out+(79-k)*320*3+l*3+i*60*320*3+j*80*3)=*(p_in+k*320*3*4+l*3*4+2);
						*(p_out+(79-k)*320*3+l*3+1+i*60*320*3+j*80*3)=*(p_in+k*320*3*4+l*3*4+1);
						*(p_out+(79-k)*320*3+l*3+2+i*60*320*3+j*80*3)=*(p_in+k*320*3*4+l*3*4);
					}
				}
			}
		}	
	}
	else if(txt==1){
		for(i=0;i<2;i++){
			for(j=0;j<4;j++){
				p_in=(bmpSequence[i*4+j+12]+54);
				for(k=0;k<60;k++){
					for(l=0;l<80;l++){
						*(p_out+(79-k)*320*3+l*3+i*60*320*3+j*80*3)=*(p_in+k*320*3*4+l*3*4+2);
						*(p_out+(79-k)*320*3+l*3+1+i*60*320*3+j*80*3)=*(p_in+k*320*3*4+l*3*4+1);
						*(p_out+(79-k)*320*3+l*3+2+i*60*320*3+j*80*3)=*(p_in+k*320*3*4+l*3*4);
					}
				}
			}
		}
	}
}