
#include "main.h"
#include "stm32f4xx_hal.h"
#include "ashuzuchaozuo.h"
// C program for implementation of ftoa()
#include <math.h>
#include <stdio.h>

//#ifndef ASHUZUCHAOZUO_H
//#define ASHUZUCHAOZUO_H





 //寻找数组
//xunzaoshuzu
//*bei 被寻找的数组
//beichang 被寻找的数组的长度
//*xun 寻找数组
//xunchang寻找数组的长度
//wei 起始寻找的位置
//返回找到的数组首次出现的位置（从0开始）
int32_t xunzhaoshuzuuint8_t(uint8_t *bei,int32_t beichang,uint8_t *xun,int32_t xunchang,int32_t wei)
{
	uint32_t i,xuni,zhen;
	if(beichang<0||xunchang<0||wei<0||beichang<(xunchang+wei))return -2;//输入错误
	for(i=wei;i<(beichang - xunchang + 1);i++)
	{
		zhen=0;
		for(xuni=0;xuni<xunchang;xuni++)
		{					
			if(bei[i+xuni]!=xun[xuni])break;		
			else
			{
				zhen++;
				if(zhen==xunchang)	
					{
						return i;
					}
			}		
		}
	}
	return -1;//返回错误
}

int32_t xunzhaoshuzuuint16_t(uint16_t *bei,int32_t beichang,uint16_t *xun,int32_t xunchang,int32_t wei)
{
	uint32_t i,xuni,zhen;
	if(beichang<0||xunchang<0||wei<0||beichang<(xunchang+wei))return -2;//输入错误
	for(i=wei;i<beichang;i++)
	{
		zhen=0;
		for(xuni=0;xuni<xunchang;xuni++)
		{
			if(bei[i+xuni]!=xun[xuni])break;
			else
			{
				zhen++;
				if(zhen==xunchang)
					{
						return i;
					}
			}
		}
	}
	return -1;//返回错误
}


int32_t xunzhaoshuzuuint32_t(uint32_t *bei,int32_t beichang,uint32_t *xun,int32_t xunchang,int32_t wei)
{

	uint32_t i,xuni,zhen;
	if(beichang<0||xunchang<0||wei<0||beichang<(xunchang+wei))return -2;//输入错误
	for(i=wei;i<beichang;i++)
	{
		zhen=0;
		for(xuni=0;xuni<xunchang;xuni++)
		{
			if(bei[i+xuni]!=xun[xuni])break;
			else
			{
				zhen++;
				if(zhen==xunchang)
					{
						return i;
					}
			}
		}
	}
	return -1;//返回错误
}

//取数组右边
//*bei 被截取的数组
//beichang 被截取的数组长度
//wei 需要截取的起始位置
//返回截取之后数组的长度
int32_t qushuzuyoubianuint8_t(uint8_t *bei,int32_t beichang,int32_t wei)
{
	uint32_t i;
	if(wei>=beichang||wei<0||beichang<0)return -1;
	for(i=0;i<beichang-wei;i++)
	{
		bei[i]=bei[i+wei];
	}
	return beichang-wei;
}

int32_t qushuzuyoubianuint16_t(uint16_t *bei,int32_t beichang,int32_t wei)
{
	uint32_t i;
	if(wei>=beichang||wei<0||beichang<0)return -1;

	for(i=0;i<beichang-wei;i++)
	{
		bei[i]=bei[i+wei];
	}
	return beichang-wei;
}

int32_t qushuzuyoubianuint32_t(uint32_t *bei,int32_t beichang,int32_t wei)
{
	uint32_t i;
	if(wei>=beichang||wei<0||beichang<0)return -1;

	for(i=0;i<beichang-wei;i++)
	{
		bei[i]=bei[i+wei];
	}
	return beichang-wei;
}
 
//文本到数字
//*wenben 被转换的文本
//返回该文本的数字
int32_t wenbendaoshuzi(uint8_t* wenben,uint8_t len)
{
	uint8_t i;
	int32_t fanhui = 0,fuhao = 0;
	
	for(i = 0;i < len;i++)
	{
		if(wenben[ i ] == '-')//判断文本中有没有“-”号20200322
		{
			fuhao = 1;
		}
		
		if(wenben[ i ] <= 58 && wenben[ i ] >= 48)
		{
			fanhui *= 10;
			fanhui += wenben[ i ] - 48;
			
		}else{
			fanhui *= 10;
		}
		
	}
	if(fuhao == 1)
	{
		fanhui = -fanhui;
	}
return fanhui;	
}

//数字到文本
//wenben 被转换的文本
//返回该文本的数字
uint8_t daowenbenwenben[20];
uint8_t* shuzidaowenben(uint32_t shuzi,uint8_t len ,uint8_t* d)
{
	
	uint8_t shuzidaowenbenlinshi[20];
	int8_t i = 0;
	for(i = 0;i < len;i++)
	{
		shuzidaowenbenlinshi[i] = 0;
		shuzidaowenbenlinshi[i] +=  (shuzi%10);
		shuzi /= 10;
	}
	
	for(i = 0;i < len;i++)
	{
		daowenbenwenben[ i ] = shuzidaowenbenlinshi[ len-i-1 ]+48;
	}
	
	for(i = 0;i < len;i++)
	{
		d[ i ] = shuzidaowenbenlinshi[ len-i-1 ]+48;
	}

	return d;
}

/*参数1 输入数字
参数2 返回数字长度
参数3 返回数据地址
*/

uint8_t* shuzidaowenben2(int64_t shuzi,uint32_t* len ,uint8_t* d)
{
	uint8_t shuzidaowenbenlinshi[20],xxxx=0;
	int8_t i = 0,ii;
	if(shuzi == 0)
	{
		i = 1;
		shuzidaowenbenlinshi[0]=48;
	}
	if(shuzi < 0)
	{
		xxxx = 1;
		shuzi=-shuzi;
	}
	while(shuzi > 0)
	{
		shuzidaowenbenlinshi[i] = 0;
		shuzidaowenbenlinshi[i] +=  (shuzi%10)+48;
		shuzi /= 10;
		i++;
	}

	if ( xxxx == 1)
	{
		shuzidaowenbenlinshi[i] = '-'; 
		i++;
	}
	if(i > 40)i = 40;//如果文本长度大于40
	*len = i;
	for(ii = 0;ii < i;ii++)
	{
		d[ ii ] = shuzidaowenbenlinshi[ i-ii-1 ];
	}
	return d;
}

//合并数组
uint8_t* hebingshuzu(uint8_t* a,uint32_t alen,uint8_t* b,uint32_t blen,uint8_t* c)
{
	uint32_t i;
	for(i = 0; i < alen + blen;i++)
	{
		if(i < alen)
		{
			c[ i ] = a[ i ];
		}else{
			c[ i ] = b[ i - alen ];
		}
	}
	return c;
}
//取数字位数
//zhengshu 被取得位数的数字
//返回该整数的位数
uint8_t qushuziweishu(int32_t zhengshu)
{
	uint8_t i = 0, x = 0;
	if(zhengshu < 0)
	{
		zhengshu = -zhengshu;
		i++;
	}
	while(i < 20 && x == 0)
	{
		if(zhengshu < 10 ) x=1;
		zhengshu /= 10;
		i++;
	}
	return i;
}


// Reverses a string 'str' of length 'len'
void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

// Converts a given integer x to string str[].
// d is the number of digits required in the output.
// If d is more than the number of digits in x,
// then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    //str[i] =200;// '\0';
    return i;
}
// Converts a floating-point/double number to a string.

//st[0]=210;
//l = ftoa((double)-123.456, (char*)&st[1], 2);
//HAL_UART_Transmit(&huart1, st ,l+2,100);//可以直接这样发送数字

uint32_t ftoa(double n, char* res, int afterpoint)//会在末尾添加结束符号，所以不能越界-20230409
{
    // Extract integer part
	int i=0;
	if(n < 0)
	{
		n = -n;
		i = 1;
		res[0] = '-';
	}
    int ipart = (int)n;

    // Extract floating part
    double fpart = n - (double)ipart;

    // convert integer part to string
    if(ipart == 0)
    {
        i += intToStr(ipart, res+i, 1)  ;
    }else
    {
        i += intToStr(ipart, res+i, 0) ;
    }


    // check for display option after point
    if (afterpoint != 0) {
        res[i] = '.'; // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);

        i += intToStr((int)fpart, res + i + 1, afterpoint)+1;

    }

    return i;
}

//#endif
