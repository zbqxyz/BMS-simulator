 #ifndef    __PRINT_H_
 #define    __PRINT_H_
 
#include <uart.h>
extern int PrintTest(void);
extern void    Print(char* fmt, ...);
extern void    PrintCH(char ch);
extern void    PrintDEC(int dec);
extern void    PrintFLT(double flt);
extern void    PrintBIN(int bin);
extern void    PrintHEX(int hex);
extern void    PrintSTR(char* str);
 
 #define ConsolePrint(ch)    UART0SendByte(ch)
 
 #define PRINT_STRING    0     //��ӡ�ַ�������
 #define PRINT_BUFF      1     //��ӡ����������
 #define PRINT_NETBUFF   1     //��ӡ����������
 
 
 #endif    /*#ifndef __PRINT_H_*/


