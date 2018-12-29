 /****************************************Copyright (c)****************************************************
**                                     Tomorn  Co.,LTD.
**                                     
**                                    http://www.tomorn.cn
**                                       wujikun
**--------------File Info---------------------------------------------------------------------------------
** File Name:               Print.c
** Last modified Date:      2018.09.12
** Last Version:            1.0
** Description:             
*********************************************************************************************************/
 #include <stdio.h>
 #include <stdarg.h>
 #include "print.h"
/*********************************************************************************************************
** Function name:	      Print
** Descriptions:	      打印测试函数
** input parameters:    
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
 int PrintTest(void)
 {
     Print("print: %c\n", 'c');
     Print("print  %d\n", 1234567);
     Print("print: %f\n", 1234567.1234567);
     Print("print: %s\n", "string test");
     Print("print: %b\n", 0x12345ff);
     Print("print: %x\n", 0xabcdef);
     Print("print: %%\n");
    return 0;
 }
 /*********************************************************************************************************
** Function name:	      Print
** Descriptions:	      打印调用入口
** input parameters:    
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
 void Print(char *fmt, ...)
 {
     double vargflt = 0;
     int    vargint = 0;
     char  *vargpch = NULL;
     char    vargch = 0;
     char     *pfmt = NULL;
	 
     va_list vp;
     va_start(vp, fmt);
     pfmt = fmt;
	 
     while(*pfmt)
     {
         if(*pfmt == '%')
         {
             switch(*(++pfmt))
             {                 
                case 'c':
                     vargch = va_arg(vp, int); 
                     /*    va_arg(ap, type), if type is narrow type (char, short, float) an error is given in strict ANSI
                         mode, or a warning otherwise.In non-strict ANSI mode, 'type' is allowed to be any expression. */
                     PrintCH(vargch);
                     break;
                case 'd':
                case 'i':
                     vargint = va_arg(vp, int);
                     PrintDEC(vargint);
                     break;
                 case 'f':
                     vargflt = va_arg(vp, double);
                     /*    va_arg(ap, type), if type is narrow type (char, short, float) an error is given in strict ANSI
                         mode, or a warning otherwise.In non-strict ANSI mode, 'type' is allowed to be any expression. */
                     PrintFLT(vargflt);
                    break;
                  case 's':
                     vargpch = va_arg(vp, char*);
                     PrintSTR(vargpch);
                    break;
                 case 'b':
                 case 'B':
                     vargint = va_arg(vp, int);
                    PrintBIN(vargint);
                     break;
                 case 'x':
                 case 'X':
                    vargint = va_arg(vp, int);
                     PrintHEX(vargint);
                     break;
                 case '%':
                    PrintCH('%');
                    break;
                 default:
                     break;
             }
             pfmt++;
         }
        else
        {
             PrintCH(*pfmt++);
       }
     }
     va_end(vp);
 }
 /*********************************************************************************************************
** Function name:	      Print
** Descriptions:	      打印出口
** input parameters:    
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
 void  PrintCH(char ch)
 {
     ConsolePrint(ch);
 }
/*********************************************************************************************************
** Function name:	      Print
** Descriptions:	      打印十进制
** input parameters:    
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
 void  PrintDEC(int dec)
 {
     if(dec==0)
      {
         return;
     }
      PrintDEC(dec/10);
      PrintCH( (char)(dec%10 + '0'));
 }
/*********************************************************************************************************
** Function name:	      Print
** Descriptions:	      打印浮点型数据
** input parameters:    
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
 void PrintFLT(double flt)
 {
     int icnt = 0;
     int tmpint = 0;
     
     tmpint = (int)flt;
     PrintDEC(tmpint);
     PrintCH('.');
     flt = flt - tmpint;
     tmpint = (int)(flt * 1000000);
     PrintDEC(tmpint);
 }
/*********************************************************************************************************
** Function name:	      Print
** Descriptions:	      打印字符串
** input parameters:    
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
 void  PrintSTR(char* str)
 {
     while(*str)
     {
         PrintCH(*str++);
     }
 }
/*********************************************************************************************************
** Function name:	      Print
** Descriptions:	      打印二进制
** input parameters:    
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/ 
 void    PrintBIN(int bin)
 {
     if(bin == 0)
     {
         PrintSTR("0b");
        return;
     }
     PrintBIN(bin/2);
     PrintCH( (char)(bin%2 + '0'));
 }
/*********************************************************************************************************
** Function name:	      Print
** Descriptions:	      打印十六进制
** input parameters:    
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
 void  PrintHEX(int hex)
 {
     if(hex==0)
     {
         PrintSTR("0x");
        return;
     }
     PrintHEX(hex/16);
    if(hex < 10)
     {
         PrintCH((char)(hex%16 + '0'));
     }
     else
     {
         PrintCH((char)(hex%16 - 10 + 'a' ));
     }
 }
	
	
	
	
	
	


