/*
1、根据以下的正规式，编制正规文法，画出状态图；
      标识符			<字母>(<字母>|<数字字符>)*
      十进制整数		0 | (（1|2|3|4|5|6|7|8|9）（0|1|2|3|4|5|6|7|8|9）*)
      八进制整数  	0（1|2|3|4|5|6|7）（0|1|2|3|4|5|6|7）*
      十六进制整数	0x（0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f）（0|1|2|3|4|5|6|7|8|9|a|b|c|d|e|f）*
      运算符和界符	+  -  *  /  >  <  =  (  )
      关键字			if  then  else  while  do
      
2、根据状态图，设计词法分析函数int scan( )，完成以下功能：
      1）	从文本文件中读入测试源代码，根据状态转换图，分析出一个单词，
      2）	以二元式形式输出单词<单词种类，单词属性>
      其中单词种类用整数表示：
      0：标识符
      1：十进制整数
      2：八进制整数
      3：十六进制整数
      运算符和界符，关键字采用一字一符，不编码
      其中单词属性表示如下：
      标识符，整数由于采用一类一符，属性用单词表示
      运算符和界符，关键字采用一字一符，属性为空
      
3、编写测试程序，反复调用函数scan( )，输出单词种别和属性。

4、program.txt内容：
  if data+92>0x3f then
	  data=data+01;
  else
	  data=data-01;
    
5、运行结果：
    <if , >
    <0 , data>
    <+ , >
    <1 , 92>
    <> , >
    <3 , 3f>
    <then , >
    <0 , data>
    <= , >
    <0 , data>
    <+ , >
    <2 , 1>
    <; ,>
    <else , >
    <0 , data>
    <= , >
    <0 , data>
    <- , >
    <2 , 1>
    <; , >

*/


// lexical_analysis.cpp : 定义控制台应用程序的入口点。

#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void scan() {//词法分析函数；
	char ch, token[8];
	FILE *fp;
	char tab[6][6] = { {'i','f'},{'t','h','e','n'},{'w','h','i','l','e'},{'d','o'},{'e','l','s','e'} };//定义二维字符数组来存储字符串数组；

	if ((fp = fopen("G://program.txt", "r")) == NULL) {//以只读的方式在G盘目录下打开program.txt文件，如果文件为空则退出；
		printf("Cannot open file strike any key exit!!\n");
		getchar();
		exit(1);
	}
	ch = fgetc(fp);//获取文件中的第一个字符；
	while (ch != EOF) {//只要读出的字符不是文件结束标志,就继续执行；
		if (ch == ' ') {//如果获取的字符为空格，就获取下一个字符；
			ch = fgetc(fp);
			continue;//结束本次分析，返回进行是否为文件结束符的分析；
		}

		for (int i = 0;i < 8;i++) {//清空字符数组；
			token[i] =NULL;
		}

		if ((ch >= 'a'&&ch <= 'z') || ch >= 'A'&&ch <= 'Z') {//标识符；
			int m = 0;
			while ((ch >= '0'&&ch <= '9') || (ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z')) {//如果字符符合0~9或者a~z或者A~Z就存入字符数组，并继续获取下一个进行判断；
				token[m++] = ch;
				ch = fgetc(fp);
			}
			token[m++] = '\0';//字符数组默认需要以'\0'结束
			int c = 0;
			for (int n = 0;n < 6;n++) {//与关键字相比较，相同时输出关键字；
				if (strcmp(token, tab[n]) == 0) {
					printf("<%s,>\n", tab[n]);
					c++;
					break;//匹配成功就进行下一轮分析，返回进行是否为文件结束符的分析；
				}
			}
			if (c == 0)
				printf("<0,%s>\n", token);//不匹配任意的关键字时，输出标识符；

			continue;//结束本次分析，返回进行是否为文件结束符的分析；
		}
		else if (ch >= '0'&&ch <= '9') {//数字；
			int sum = 0;
			if(ch != '0'){//当开头不为0，则必定是十进制数；
				while (ch >= '0'&&ch <= '9') {
					sum = sum * 10 + ch - '0';
					ch = fgetc(fp);
				}
				printf("<1,%d>\n", sum);//输出此次分析的十进制数；
			}
			else {
				ch = fgetc(fp);
				if (ch == 'x') {//当第一个字符为0，而第二个为x,则为十六进制数；
					ch = fgetc(fp);
					int i = 0;
					while ((ch>='0'&&ch<='9')||(ch>='a'&&ch<='f')) {
						token[i++] = ch;//将十六进制数存入字符数组；
						ch=fgetc(fp);
					}
					printf("<3,%s>\n", token);//输出十六进制整数数；
				}
				else if (ch<='0'&&ch>='9') {//如果只是独立的0，输出十进制数0；
					printf("<1,0>\n");
				}
				else {//输出八进制数；
					while (ch >= '0'&&ch <= '9') {
						sum = sum * 10 + ch - '0';
						ch = fgetc(fp);
					}
					printf("<2,%d>\n", sum);
				}
			}
			continue;//结束本次分析，返回进行是否为文件结束符的分析；
		}
		else {//输出运算符和界符；
			if (ch == ' '||ch=='\n'||ch=='r'||ch=='\t') {//如果是空格，换行符，回车，制表符则获取下一字符；
				ch = fgetc(fp);
				continue;//结束本次分析，返回进行是否为文件结束符的分析；
			}
			switch (ch) {
				case '<':token[0] = ch;break;
				case '>':token[0] = ch;break;
				case '=':token[0] = ch;break;
				case '+':token[0] = ch;break;
				case '-':token[0] = ch;break;
				case '*':token[0] = ch;break;
				case '/':token[0] = ch;break;
				case '(':token[0] = ch;break;
				case ')':token[0] = ch;break;
				case ';':token[0] = ch;break;
				default:break;
			}
			printf("<%c,>\n",token[0]);
			ch = fgetc(fp);
			continue;//结束本次分析，返回进行是否为文件结束符的分析；
		}
	}
	fclose(fp);//关闭文件；
}

int main()
{
	scan();
	system("pause");
    return 0;
}


