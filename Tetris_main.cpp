
/*  
 * Name:   Tetris		
 */
#include <stdio.h>
#include <winbgim.h>
#include <conio2.h>
#include <time.h>   //
#include <stdlib.h>

#define chang  30

void checkerror();            //错误检测函数 
int delay(int num);          //延时函数 
int presskey();             //按键检测函数  q 表示离开 
int drawgrap(int x,int y,int grap,int en);  //画图形函数  参数X： 横坐标 Y ：纵坐标   graph ：图形标号  key=grap
int turnover();             //  W 翻转函数
int srandom();              //产生2到20之间的随机数 
int clrhang()  ;            //消除满格子的一行 
int judgedown() ;           //判断能否下降  返回值为1表示不能下降  返回值为2，表示游戏结束了 
int inputdata();            //用来画底部的方框 
int rightmove();            //判断是否可以向右移动
int leftmove() ;            //判断是否可以左移动
int strdown() ;             //按键s后直线下降
int strdown()  ;            //按键s后直线下降
int allstart() ;            // 数据全部清零操作 
int data[20][16];           //20层  16：一层有16个格子 
int y,x1,y1,x3,y3,x4,y4,key,temp,tu,flag=-1,cs,extime,extu,score=0,looptime,yan,bkcolor,tim; //score 总的分数 
int fan;      //readfile()的返回值 
char ch,wname[20];

typedef struct fgdata          //定义结构体，用于读写文件操作 
{
	char name[20] ;            // 玩家姓名 
	int   fscore;              //分数 
	int  fgcolor;              //方格的颜色 
	int  bgcolor;              // 背景颜色 
	int  shu[20][16];          //用来存储方块 
}FGDATA;  

int readfile(FGDATA player[]) ;  //读文件操作
int writefile(FGDATA player[]) ;   //写文件操作  存储当前玩家的信息 
void drawinfo(FGDATA player[]);         //画边框 
int main()
{
	FGDATA player[100];   //申明结构体 .最多有100个玩家 
    int judge,i,q,t; 
    const char tit[10]={'T','E','T','R','I','S'}; //用在窗口标题显示英语字符 
    const char title[10]={'G','A','M','E',' ','O','V','E','R'}; //游戏结束显示结束游戏 
    readfile(player);//读文件操作 
	x1= 210 ,y1=0,cs=0,looptime=20,tim=looptime; //游戏初始化数据 
	if(yan==0)yan=14;        //如果开始颜色为黑色，改为黄色 
	if(bkcolor==0)bkcolor=11; //yan：方块的背景颜色，e按键切换颜色 
    initwindow(1024,720); //open a 640x480 graphics window
    setwindowtitle(tit);     //设置了窗口标题 
    tu=srandom();            //产生随机数 
    extu = srandom();   //产生随机数，用在下次的下落；        
    setbkcolor(bkcolor);  //设置背景颜色 3 兰色  7 淡灰色 11 淡青色 
    while(!kbhit())
      { 	         
        cs++; 
       A: if(judge==1) 
		{
		   extime++;                   //增加一个额外时间，保证不能下降的时候可以左右移动 
		   if(extime==looptime)
		   {
		   judgedown();
		   tu=extu, extu=srandom();
		   cs=0 ,y1=0,x1=210;//加入extu保证能够在方格外面显示下一个图形 
		   goto B;
	       }
	    }
	    else extime=0;
		judge=judgedown();
		delay(400);                             //消除满格子的一行 
		if(cs==looptime)
		{ 
		y1=y1+chang,cs=0;          //必须放在judge后面 
		goto B;
	    }
	    if(judge==2)
		  {
	      allstart();
		  goto C;
	      }
	  } 
	printf("",ch=getch());
	presskey();
	setviewport(0,0,1024,600,11);          //设置清屏区域 
B:	clearviewport();                       //清屏 
    clrhang();                               //消除满格子的一行 
	inputdata();                           //输入存储的方块数据                      
	drawinfo(player);                            //边框 等级，分数，游戏 su表示画的边框的长度; 480X 600 
	drawgrap(x1,y1,tu,0); 
    if(ch!=81 && ch!=113 && ch!=27) goto A;
C:  settextstyle(1,0,5);
	outtextxy(540,330,title);        //显示game over 
    writefile(player);
	getch(); 
 	closegraph();         //close graphics window
 	return 0;
}

//  画图形函数   参数X： 横坐标 Y ：纵坐标   graph ：图形标号 en :1表示用在 drawinfo（）函数中，0表示用在正常的函数 
int drawgrap(int x,int y,int grap,int en)         
{
	int a,y2;  
	int st2[16]={0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0};        //田字格子 
	
	int st3[16]={0,0,1,0,0,0,1,1,0,0,0,1,0,0,0,0};        //横着  Z 字 
	int st4[16]={0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0};        //竖直  Z 字  
	
	int st5[16]={0,0,1,0,0,0,1,0,0,0,1,1,0,0,0,0};        //横着的反  7  
	int st6[16]={0,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0};        //竖直的 反 7 
	int st7[16]={0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,0};        //旋转90度后 
	int st8[16]={0,0,0,1,0,1,1,1,0,0,0,0,0,0,0,0};        //旋转90度后 
	
	int st9[16]={0,0,0,1,0,0,1,1,0,0,0,1,0,0,0,0};        //    凸 
	int st10[16]={0,0,1,0,0,1,1,1,0,0,0,0,0,0,0,0};       //旋转90度后 
	int st11[16]={0,0,1,0,0,0,1,1,0,0,1,0,0,0,0,0};       //旋转90度后  
	int st12[16]={0,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0};       //旋转90度后 
    
    int st13[16]={0,0,0,1,0,0,1,1,0,0,1,0,0,0,0,0};       //反  Z 字
    int st14[16]={0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0};       //旋转90度后 
    
    int st15[16]={0,0,0,1,0,0,0,1,0,0,1,1,0,0,0,0};       //横着的7  字 
    int st16[16]={0,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0};       // 竖直的7字 
    int st17[16]={0,0,1,1,0,0,1,0,0,0,1,0,0,0,0,0};       // 旋转90度后 
    int st18[16]={0,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0};       //旋转90度后   
    
    int st19[16]={1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0};       //竖直的长条
	int st20[16]={0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1};        //横着的长条  
    if(en==0) key=grap ;
	y2=y;                                             //翻转标志   	
	for(a=0;a<16;a++)
	{ 
	if(a==4) x=x+chang,y=y2;
	if(a==8) x=x+chang,y=y2;
	if(a==12)   x=x+chang,y=y2;
	setcolor(RED);
	setfillstyle(1,yan);
	switch(grap)
	  {
	  	case 19:
	  		if(st19[a]==1)
	  		rectangle(x,y,x+chang,y+chang),floodfill(x+10,y+10, RED);
	  		break;
	  	case 20:
	  		if(st20[a]==1)
	  		rectangle(x,y,x+chang,y+chang),floodfill(x+10,y+10, RED);
	  		break;
	  	case 2:
	  		if(st2[a]==1)
	  		rectangle(x,y,x+chang,y+chang),floodfill(x+10,y+10, RED);
	  		break;
	  	case 3:
	  		if(st3[a]==1)
	  		rectangle(x,y,x+chang,y+chang),	floodfill(x+10,y+10, RED);
	  		break;
	  	case 4:
	  		if(st4[a]==1)
	  		rectangle(x,y,x+chang,y+chang),floodfill(x+10,y+10, RED);
	  		break;
	  	case 5:
	  		if(st5[a]==1)
	  		rectangle(x,y,x+chang,y+chang),	floodfill(x+10,y+10, RED);
	  		break;
	  	case 6:
	  		if(st6[a]==1)
	  		rectangle(x,y,x+chang,y+chang),	floodfill(x+10,y+10, RED);
	  		break;
	  	case 7:
	  		if(st7[a]==1)
	  		rectangle(x,y,x+chang,y+chang),floodfill(x+10,y+10, RED);
	  		break;
	  	case 8:
	  		if(st8[a]==1)
	  		rectangle(x,y,x+chang,y+chang),	floodfill(x+10,y+10, RED);
	  		break;
	  	case 9:
	  		if(st9[a]==1)
	  		rectangle(x,y,x+chang,y+chang),floodfill(x+10,y+10, RED);
	  		break;
	  	case 10:
	  		if(st10[a]==1)
	  		rectangle(x,y,x+chang,y+chang),	floodfill(x+10,y+10, RED);
	  		break;
	  	case 11:
	  		if(st11[a]==1)
	  		rectangle(x,y,x+chang,y+chang),floodfill(x+10,y+10, RED);
	  		break;
	  	case 12:
	  		if(st12[a]==1) 
			rectangle(x,y,x+chang,y+chang),floodfill(x+10,y+10, RED);
	  		break;
	  	case 13:
	  		if(st13[a]==1) 
			rectangle(x,y,x+chang,y+chang),	floodfill(x+10,y+10, RED);
	  		break;
	  	case 14:
	  		if(st14[a]==1) 
			rectangle(x,y,x+chang,y+chang),floodfill(x+10,y+10, RED);
	  		break;
	  	case 15:
	  		if(st15[a]==1) 
			rectangle(x,y,x+chang,y+chang),	floodfill(x+10,y+10, RED);
	  		break;
	  	case 16:
	  		if(st16[a]==1) 
			rectangle(x,y,x+chang,y+chang),floodfill(x+10,y+10, RED);
	  		break;
	  	case 17:
	  		if(st17[a]==1) 
			rectangle(x,y,x+chang,y+chang),floodfill(x+10,y+10, RED);
	  		break;
	  	case 18:
	  		if(st18[a]==1) 
			rectangle(x,y,x+chang,y+chang),	floodfill(x+10,y+10, RED);
	  		break;
      }
	  y=y+chang;    
    }
	return 0;
} 

//延时函数   
int delay(int num)
{
	unsigned int n,u;
	for(n=20000;n>0;n--)
	for(u=num;u>0;u--);	
}  

//按键检测函数：a,s,d,w左右移动  q 表示离开  space 暂停  c从新开始游戏 
int presskey()
{
//	delay(200); 
	switch(ch)
	{ 
	case 87:      //键盘大小写的 a 和 A 
	case 119:  
	case 72:     //键盘  上 
	    tu=turnover();
		break;
	case 65:
	case 97:
	case 75:
		leftmove();   //向左移动 
		break;
	case 77:
	case 100:
	case 68:
		rightmove();  //向右移动 
		break;
	case 83:
	case 115:
	case 80:
	    strdown();   //竖直向下移动 
	    score=score+20;
		break;
	case 32:         //如果按下space ---halt 
		outtextxy(600,500,"游戏暂停");
		while(!kbhit())
        {
        delay(4000);   
        }
        break;
    case  69:
    case 101:
    	yan=yan+1;
    	if(yan>15)  yan=0;
    	break;
    case 102:
    case 70:
    	bkcolor+=1;
    	if(bkcolor>15) bkcolor=0;
    	setbkcolor(bkcolor);
    	break;
    case 99:
    case 67:          //重新开始游戏 
    	allstart();
    	score=0 ;
    	break;
	default :
	    break ;
    }
   // if(flag==0) flag=key;               //判断进入循环之前flag为0出现bug ；若flag为0 ，不能画图！ 
    checkerror();
    cs=0,extime=0;                        //下落时间和额外时间都要置为零 
 } 

//错误检测函数，判断每次移动是否出错  
void checkerror()
{
	if(x1<0)    x1=0;
    switch(key)
    {
    	case 19:
    		if(x1>450) x1=450;
    		break;
    	case 18: 
    	case 16:  
    	case 14:  
    	case 12:  
    	case 10:  
    	case 8:  
    	case 6:  
    	case 4:  
    	case 2:  
    	    if(x1>420)  x1=420;
			break;
    	case 17:  
    	case 15:  
    	case 13:  
    	case 11:  
    	case 9:  
    	case 7: 
    	case 5:  
    	case 3: 
    	     if(x1>390)  x1=390;
    	     break;
    	case 20:   
		     if(x1>360)  x1=360; 
			 break; 
		default :  break;   	   
    }

} 

//  W 翻转函数
int turnover()
{
	int x0,y0;   //x0 ======xp 表示一层有多少的格子 y0 ======= yp  表示有多少层 
	x0=x1/chang,y0= 18-y1/chang; 
	switch(key)
	{
	 //19和20是长条翻转 
		case 19:
			if(x1>360 || data[y0-2][x0+1]==1 || data[y0-2][x0+2]==1 || data[y0-2][x0+3]==1) return 19;
			else return 20;
			break;
		case 20:
			if(data[y0][x0]==1 ||data[y0-1][x0]==1 ||data[y0+1][x0]==1 ) return 20;
			else return 19;
			break;
	//	田	
		case  2:
		    return 2;
			break;
	//3,4为  Z		
		case  3:
			if(data[y0-2][x0]==1 || data[y0][x0+1]==1) return 3;
		    else return 4;
			break;
		case 4:
			if(x1==420 || data[y0-2][x0+1]==1 || data[y0-2][x0+2]==1) return 4;
		    else return 3;
			break;
	//5,6,7,8为反  7		  
		case 5:
			if(data[y0-2][x0]==1 ||data[y0][x0]==1 || data[y0][x0+1]==1) return 5;
		    else return  6;
		    break;
		case 6:
			if(x1==420||data[y0-2][x0+1]==1||data[y0-2][x0+2]==1) return 6;
		    else return  7;
		    break;
		case 7:
			if(data[y0-1][x0+1]==1||data[y0][x0+1]==1) return 7;
		    else return  8;
		    break;
		case 8:
			if(x1==420||data[y0-1][x0]==1||data[y0-1][x0+2]==1||data[y0-2][x0+2]==1) return 8;
		    else return  5;
		    break;
	//9,10,11,12为  凸  
		case 9:
			if(data[y0-1][x0]==1||data[y0][x0+1]==1) return 9;
		    else return  10;
		    break;
		case 10:
			if(x1==420||data[y0-1][x0+2]==1) return 10;
		    else return  11;
		    break;			
		case 11:
			if(data[y0][x0]==1||data[y0-2][x0]==1) return 11;
		    else return  12;
		    break;
	    case 12:
	    	if(x1==420||data[y0-2][x0+1]==1||data[y0-2][x0+2]==1) return 12;
		    else return  9;
		    break;
	//13,14为反  Z 
		case 13:
			if(data[y0][x0]==1||data[y0-1][x0]==1) return 13;
		    else return  14;
		    break;
		case 14:
			if(x1==420||data[y0-2][x0]==1||data[y0-1][x0+2]==1) return 14;
		    else return  13;
		    break;
	//15,16,17,18为  7 
		case 15:
			if(data[y0][x0]==1||data[y0-1][x0+1]==1||data[y0][x0+1]==1) return 15;
		    else return  16;
		    break;
		case 16:
			if(x1==420||data[y0-1][x0]==1||data[y0-2][x0]==1||data[y0-1][x0+2]==1) return 16;
		    else return  17;
		    break;
		case 17:
			if(data[y0][x0]==1||data[y0-2][x0+1]==1) return 17;
		    else return  18;
		    break;
		case 18:
			if(x1==420||data[y0-2][x0+2]==1||data[y0-1][x0+2]==1) return 18;
		    else return  15;
		    break;		
	}
} 

//产生并且返回2到20之间的随机数  
int srandom()
{
	int res; 
	srand(time(0));
	res=rand()%19+2;
	return res; 
}

//二维数组用来画所有的方块  19层 X 16格 
int inputdata()
{
	int yp,xp;          //yp ：层数  xp ：每层的格子数目 
	setfillstyle(1,yan);
	for(yp=0;yp<20;yp++)    
	{
		x3=0,y3=600-(yp+1)*chang;
		for(xp=0;xp<16;xp++)
		{
			if(data[yp][xp]==1) setcolor(RED),rectangle(x3,y3,x3+chang,y3+chang),floodfill(x3+10,y3+10, RED);
	        x3=x3+chang;         
		}
	}
}

//边框 等级，分数，游戏 su表示画的边框的长度; 480X 600  显示当前玩家名称 
void drawinfo(FGDATA player[])
{
	 char fen[50],wname[20];
	 int su;
	 x4=483,y4=0,su=5;
	 sprintf(fen,"游戏总得分：%d",score);
	 drawgrap(610,120,extu,1);     //画下一个图形 
     setcolor(GREEN);
	 for(;y4<600;)
	 {
	 rectangle(x4,y4,x4+su,y4+su);
	 y4+=su;
     }
     x4=0,y4=603;
	 for(;x4<1200;)
	 {
	 	rectangle(x4,y4,x4+su,y4+su);
	    x4+=su;
	 }
	setcolor(5);
	settextstyle(1,0,4);
	outtextxy(600,30," BY   AMEE"); 
	settextstyle(0,0,2);
	outtextxy(600,100,"下一个");
	outtextxy(600,300,fen);
	if(score<=1000) outtextxy(600,400,"游戏等级：1"),looptime=20; 
    else if(score<=2000&&score>1000) outtextxy(600,400,"游戏等级：2"),looptime=16,tim=16; 
    else if(score<=5000&&score>2000) outtextxy(600,400,"游戏等级：3"),looptime=13,tim=13; 
    else if(score<=10000&&score>5000) outtextxy(600,400,"游戏等级：4"),looptime=9,tim=9; 
    else if(score>10000) outtextxy(600,400,"游戏等级：5"),looptime=5,tim=5; 
    outtextxy(600,440,"当前玩家：");
    outtextxy(700,620,"重新开始 C or c"); 
    outtextxy(750,440,player[flag].name);
	outtextxy(10,620,"提示：");
	outtextxy(90,620,"翻转：↑or W");  
	outtextxy(90,640,"左移：←or A");  
	outtextxy(400,620,"右移：→or D");  
	outtextxy(400,640,"下移：↓or S");  
	outtextxy(90,660,"退出游戏：ESC or Q");  		
	outtextxy(90,680,"空格暂停游戏，任意键退出暂停");
	outtextxy(700,660,"切换方块颜色 E");
	outtextxy(700,680,"切换背景颜色 F");
	outtextxy(700,640,"切换颜色");
}

//判断能否下降  返回值为1表示不能下降并且存储当前的方块 返回值为2，表示游戏结束了 到达底部时停止 tim值表示存储的条件 
int judgedown()
{
	 int x0,y0;
	 x0=x1/chang,y0= 18-y1/chang;          //x0 ======xp 表示一层有多少的格子 y0 ======= yp  表示有多少层   data[yp][xp]
	 switch(key)
	 {
	 	case 2:          //    田 
		    if(data[y0-2][x0]==1 || data[y0-2][x0+1]==1 || y1==510) 
			{ 
			if(y0==18) return 2;
			else 
			  {
			  if(extime==tim) data[y0-1][x0]=data[y0][x0]=data[y0-1][x0+1]=data[y0][x0+1]=1;  //额外时间到期后才能存储数据 
			  return 1;
			  } 
	     	}
	     	else return 0;
	 	    break;
	 	case 3:
		     if(data[y0-2][x0]==1 || data[y0-3][x0+1]==1 || data[y0-3][x0+2]==1 || y1==480) 
			 {
			 if(y0==18) return 2;
			 else 
			  {
			  if(extime==tim) data[y0-1][x0]=data[y0-1][x0+1]=data[y0-2][x0+1]=data[y0-2][x0+2]=1; 
			  return 1;
			  } 
		     }
		     else return 0;
		     break;
		case 4:
			 if(data[y0-3][x0]==1 || data[y0-2][x0+1]==1 || y1==480) 
			 {
			 if(y0==18) return 2;
			 else 
			  {
			  if(extime==tim) data[y0-1][x0]=data[y0-2][x0]=data[y0-1][x0+1]=data[y0][x0+1]=1; 
			  return 1;
			  } 
		     }
		     else return 0;
			 break;
		case 5:
			 if(data[y0-2][x0]==1 || data[y0-2][x0+1]==1 || data[y0-3][x0+2]==1 || y1==480)  
			 {
			 if(y0==18) return 2;
			 else
			  {
			  if(extime==tim) data[y0-1][x0]=data[y0-1][x0+1]=data[y0-1][x0+2]=data[y0-2][x0+2]=1;
			  return 1;
			  }  
			 } 
			 else return 0;
			 break;
	    case 6:
	    	 if(data[y0-3][x0]==1 || data[y0-1][x0+1]==1 || y1==480) 
	    	 {
	    	 if(y0==18) return 2;
	    	 else
			  {
	    	  if(extime==tim) data[y0][x0]=data[y0-1][x0]=data[y0-2][x0]=data[y0][x0+1]=1;
			  return 1;		
	    	  }
	    	 }
	    	 else return 0;
	    	 break;
	    case 7:
	    	if(data[y0-3][x0]==1 || data[y0-3][x0+1]==1 || data[y0-3][x0+2]==1 || y1==480)
	    	 {
	    	 	if(y0==18) return 2;
	         	else 
	         	{
	         	 if(extime==tim) data[y0-2][x0]=data[y0-2][x0+1]=data[y0-1][x0]=data[y0-2][x0+2]=1;
				 return 1;	
	         	}
	    	 }
	    	 else return 0;
	    	 break;
	    
	    case 8:
	         if(data[y0-3][x0]==1 || data[y0-3][x0+1]==1 ||y1==480) 
	         {
	         	if(y0==18) return 2;
	         	else 
	         	{
	         	 if(extime==tim) data[y0-2][x0]=data[y0][x0+1]=data[y0-1][x0+1]=data[y0-2][x0+1]=1;
				 return 1;	
	         	}
	         }
	         else return 0;
	         break;
	    case 9:
	    	if(data[y0-3][x0]==1 || data[y0-3][x0+1]==1 || data[y0-3][x0+2]==1 ||y1==480)
	    	 {
	    	 	if(y0==18) return 2;
	         	else 
	         	{
	         	 if(extime==tim) data[y0-2][x0]=data[y0-2][x0+1]=data[y0-1][x0+1]=data[y0-2][x0+2]=1;
				 return 1;	
	         	}
	    	 }
	    	 else return 0;
	    	 break;
	    case 10:
	    	if(data[y0-2][x0]==1 || data[y0-3][x0+1]==1 || y1==480) 
	    	 {
	    	 	if(y0==18) return 2;
	    	 	else 
	    	 	{
	    	 	 if(extime==tim) data[y0-1][x0]=data[y0][x0+1]=data[y0-1][x0+1]=data[y0-2][x0+1]=1;
	    	 	 return 1;
	    	 	}
	    	 }
	    	 else return 0;
	    	 break; 	 
	    case 11:
	    	 if(data[y0-2][x0]==1 || data[y0-3][x0+1]==1 || data[y0-2][x0+2]==1 || y1==480) 
	    	 {
	    	 	if(y0==18) return 2;
	    	 	else 
	    	 	{
	    	 	 if(extime==tim) data[y0-1][x0]=data[y0-1][x0+1]=data[y0-2][x0+1]=data[y0-1][x0+2]=1;
	    	 	 return 1;
	    	 	}
	    	 }
	    	 else return 0;
	    	 break; 
	    case 12:
	    	 if(data[y0-3][x0]==1 || data[y0-2][x0+1]==1 || y1==480) 
	    	 {
	    	 if(y0==18) return 2;
	    	 else 
	    	  {
	    	  if(extime==tim) data[y0][x0]=data[y0-1][x0]=data[y0-2][x0]=data[y0-1][x0+1]=1;
	    	  return 1;
	    	  }
	    	 }
	    	 else return 0;
	    	 break;
	    case 13:
	    	if(data[y0-3][x0]==1 || data[y0-3][x0+1]==1 || data[y0-2][x0+2]==1 || y1==480) 
	    	{
	    	 if(y0==18) return 2;
	    	 else 
	    	 {
	    	 if(extime==tim) data[y0-2][x0]=data[y0-1][x0+1]=data[y0-2][x0+1]=data[y0-1][x0+2]=1;
	    	 return 1;
	    	 }
	    	}
	    	else return 0;
	    	break;
	    case 14:
	    	 if(data[y0-2][x0]==1 || data[y0-3][x0+1]==1 || y1==480) 
	    	 {
	    	 	if(y0==18) return 2;
	    	 	else 
	    	 	{
	    	 	 if(extime==tim) data[y0][x0]=data[y0-1][x0]=data[y0-1][x0+1]=data[y0-2][x0+1]=1;
	    	 	 return 1;
	    	 	}
	    	 }
	    	 else return 0;
	    	 break;
	    case 15:
	    	 if(data[y0-3][x0]==1 || data[y0-3][x0+1]==1 || data[y0-3][x0+2]==1 || y1==480)
	    	 {
	    	 	if(y0==18) return 2;
	         	else 
	         	{
	         	 if(extime==tim) data[y0-2][x0]=data[y0-2][x0+1]=data[y0-1][x0+2]=data[y0-2][x0+2]=1;
				 return 1;	
	         	}
	    	 }
	    	 else return 0;
	    	 break;
        case 16:
        	 if(data[y0-1][x0]==1 || data[y0-3][x0+1]==1 || y1==480) 
        	 {
        	 	if(y0==18) return 2;
        	 	else 
        	 	{
        	 	 if(extime==tim) data[y0][x0]=data[y0][x0+1]=data[y0-1][x0+1]=data[y0-2][x0+1]=1;
        	 	 return 1;
        	 	}
        	 }
        	 else return 0;
        	 break;
        case 17:
        	 if(data[y0-3][x0]==1 || data[y0-2][x0+1]==1 || data[y0-2][x0+2]==1 || y1==480) 
        	 {
        	 	if(y0==18) return 2;
        	 	else 
        	 	{
        	 	 if(extime==tim) data[y0-1][x0]=data[y0-2][x0]=data[y0-1][x0+1]=data[y0-1][x0+2]=1;
				 return 1;
        	 	}
         	 }
         	 else return 0;
        	 break;
        case 18:
        	if(data[y0-3][x0]==1 || data[y0-3][x0+1]==1 || y1==480) 
        	 {
        	 	if(y0==18) return 2;
        	 	else 
        	 	{
        	 	 if(extime==tim) data[y0][x0]=data[y0-1][x0]=data[y0-2][x0]=data[y0-2][x0+1]=1;
				 return 1;
        	 	}
         	 }
         	 else return 0;
        	 break;
        case 19:
        	 if(data[y0-3][x0]==1 || y1==480) 
        	 {
        	 	if(y0==18) return 2;
        	 	else 
				{
				if(extime==tim) data[y0+1][x0]=data[y0][x0]=data[y0-1][x0]=data[y0-2][x0]=1;
				return 1;
        	 	}
        	 }
        	 else return 0;
        	 break;
        case 20:
        	 if(data[y0-3][x0]==1 || data[y0-3][x0+1]==1 ||data[y0-3][x0+2]==1 || data[y0-3][x0+3]==1 || y1==480) 
        	 {
        	 if(y0==18) return 2;
        	 else 
        	  {
        	  if(extime==tim) data[y0-2][x0]=data[y0-2][x0+1]=data[y0-2][x0+2]=data[y0-2][x0+3]=1;
			  return 1;	
        	  } 
        	 }
        	 else return 0;
        	 break;
	 }

}

//消除满格子的一行 
int clrhang()
{
	int a1=0,b1,flg,cs;
	for(;a1<18;)
	{
		flg=0,b1=0;
		while(flg==0)
		{
		if(data[a1][b1]==1 && b1<16)  b1=b1+1;      //判断 滴N层是不是全部为1 
		else flg=1;
	    }
	    if(b1==16) 
	    {
	    	score=score+40;
	    	while(a1<19)
	    	{
	        for(cs=0;cs<16;cs++)
	          {
	          data[a1][cs]=data[a1+1][cs];         //将n+1层的数据赋值给第N层 
	          }
	        a1=a1+1;
	        }
	        a1=0;
	    } 
	    else a1=a1+1;
	}
} 

//判断是否可以左移动
int leftmove()
{
	int x0,y0; //x0 ======xp 表示一层有多少的格子 y0 ======= yp  表示有多少层   data[yp][xp]
	x0=x1/chang,y0= 18-y1/chang; 
	if(x1!=0)
   {
	  switch(key)
	  {
		case 2:
			if(data[y0][x0-1]!=1 && data[y0-1][x0-1]!=1) x1=x1-chang;
			break;
		case 3:
		case 11:
			if(data[y0-1][x0-1]!=1 && data[y0-2][x0]!=1) x1=x1-chang;
			break;
		case 4:
			if(data[y0-1][x0-1]!=1 && data[y0][x0]!=1 && data[y0-2][x0-1]!=1) x1=x1-chang;
			break;
		case 5:
			if(data[y0-1][x0-1]!=1 && data[y0-2][x0+1]!=1 ) x1=x1-chang;
			break;
		case 6:
		case 12:
		case 18:
			if(data[y0][x0-1]!=1 && data[y0-2][x0-1]!=1 &&data[y0-1][x0-1]!=1) x1=x1-chang;
			break;
		case 7:
		case 17:
			if(data[y0-1][x0-1]!=1 && data[y0-2][x0-1]!=1 ) x1=x1-chang;
			break;	
		case 8:
			if(data[y0][x0]!=1 && data[y0-1][x0]!=1 &&data[y0-2][x0-1]!=1) x1=x1-chang;
			break;
		case 9:
		case 13:
			if(data[y0-1][x0]!=1 && data[y0-2][x0-1]!=1 ) x1=x1-chang;
			break;
		case 10:
			if(data[y0][x0]!=1 && data[y0-2][x0]!=1 &&data[y0-1][x0-1]!=1) x1=x1-chang;
			break;
		case 14:
			if(data[y0][x0-1]!=1 && data[y0-1][x0-1]!=1 &&data[y0-2][x0]!=1) x1=x1-chang;
			break;	
		case 15:
			if(data[y0-1][x0+1]!=1 && data[y0-2][x0-1]!=1) x1=x1-chang;
			break;	
		case 16:
			if(data[y0][x0-1]!=1 && data[y0-2][x0]!=1 &&data[y0-1][x0]!=1) x1=x1-chang;
			break;	
		case 19:
			if(data[y0+1][x0-1]!=1 && data[y0][x0-1]!=1 &&data[y0-1][x0-1]!=1 &&data[y0-2][x0-1]!=1) x1=x1-chang;
			break;	
		case 20:
			if(data[y0-2][x0-1]!=1 ) x1=x1-chang;
			break;		
	  }
   }
} 

//判断是否可以向右移动
int rightmove()
{
	int x0,y0; //x0 ======xp 表示一层有多少的格子 y0 ======= yp  表示有多少层   data[yp][xp]
	x0=x1/chang,y0= 18-y1/chang; 
    switch(key)
    {
     case 2:
      	if(data[y0][x0+2]!=1&& data[y0-1][x0+2]!=1 ) x1=x1+chang;
      	break;
     case 3:
     case 9:
     	if(data[y0-1][x0+2]!=1&& data[y0-2][x0+3]!=1 )  x1=x1+chang;
     	break;
      case 4:
     	if(data[y0][x0+2]!=1&& data[y0-1][x0+2]!=1 &&data[y0-2][x0+1]!=1)  x1=x1+chang;
     	break;		
      case 5:
      case 15:
     	if(data[y0-1][x0+3]!=1&& data[y0-2][x0+3]!=1 )  x1=x1+chang;
     	break;		
      case 6:
     	if(data[y0][x0+2]!=1&& data[y0-1][x0+1]!=1 &&data[y0-2][x0+1]!=1)  x1=x1+chang;
     	break;		
      case 7:
     	if(data[y0-1][x0+1]!=1&& data[y0-2][x0+3]!=1 )  x1=x1+chang;
     	break;		
      case 8:
      case 10:
      case 16:
     	if(data[y0][x0+2]!=1&& data[y0-1][x0+2]!=1 &&data[y0-2][x0+2]!=1)  x1=x1+chang;
     	break;	
      case 11:
      case 13:
     	if(data[y0-1][x0+3]!=1&& data[y0-2][x0+2]!=1 )  x1=x1+chang;
     	break;	
     case 12:
     	if(data[y0][x0+1]!=1&& data[y0-1][x0+2]!=1 &&data[y0-2][x0+1]!=1)  x1=x1+chang;
     	break;	
     case 14:
     	if(data[y0][x0+1]!=1&& data[y0-1][x0+2]!=1 && data[y0-2][x0+2]!=1)  x1=x1+chang;
     	break;	
     case 17:
     	if(data[y0-1][x0+3]!=1&& data[y0-2][x0+1]!=1 )  x1=x1+chang;
     	break;
     case 18:
     	if(data[y0][x0+1]!=1&& data[y0-1][x0+1]!=1 && data[y0-2][x0+2]!=1)  x1=x1+chang;
     	break;	
     case 19:
     	if(data[y0+1][x0+1]!=1&& data[y0][x0+1]!=1 && data[y0-1][x0+1]!=1&& data[y0-2][x0+1]!=1)  x1=x1+chang;
     	break;	
     case 20:
     	if(data[y0-2][x0+4]!=1)  x1=x1+chang;
     	break;	
    }
    checkerror();          //最后检查是否有出界限的情况 
} 

//按键s后直线下降
int strdown()
{
	int sy;
	sy=judgedown();
	while(sy!=1)
	{
	 y1=y1+chang;
	 sy=judgedown();
    }
}

//写文件操作  存储当前玩家的信息 
int writefile(FGDATA player[])
{
	int i,a,w=0;
	FILE *fp;
	fp=fopen("e:\\Tetris.txt","w");   //写入所有的数据 ,覆盖之前的所有的数据  
	//赋值当前玩家的资料 
	player[flag].fgcolor=yan,player[flag].bgcolor=bkcolor,player[w].fscore=score;
    for(i=0;i<20;i++)
    for(a=0;a<16;a++) player[flag].shu[i][a]=data[i][a]; 
    //存储所有玩家的资料 		
    while(w< fan)
	 {
	 fprintf(fp," %s %d %d %d",player[w].name,player[w].fgcolor,player[w].bgcolor,player[w].fscore);//存当前玩家的姓名背景颜色 
	 for(i=0;i<20;i++)   
     for(a=0;a<16;a++) fprintf(fp," %d",player[w].shu[i][a] );//存储当前玩家的方块信息 
     w++;     
    }
} 

//读文件操作  flag:有同名玩家标志； flag1；文件不为空标志 返回值：w 
int readfile(FGDATA player[])
{
	int i=0,a=0,w=0,q=0,bz=0,f,x;
	FILE *fp;
	fp=fopen("e:\\Tetris.txt","a+");     //当文件不存在时候先创建一个 
	if(fp==NULL) printf("打开文件失败"); 
	printf("注意：玩家名称,由数字或则字母构成，长度不超过20个字符!\n");
	printf("      按下enter确认，弹出游戏窗口时，请用鼠标点击一下游戏画面进行游戏!\n");  
	printf("      玩家信息存储在：E：\\Tetris.txt文件中!\n") ;
	printf("*****************************************************************************\n") ;
	printf("亲！输入玩家名称:\n");
	gets(wname);
	for(q=0;x==0;q++) if(wname[q]=='\0') f=q,x=1; //把 \0 后面的全部写成 \0 ; 
    for(q=f;q<20;q++) wname[q]='\0';
	if(fgetc(fp)==EOF) 
     {
	 strcpy(player[0].name,wname);
	 flag=0;
	 fan=1;
     }
	else
	{ 
	 while(!feof(fp)) //读到文件结尾为止 
	 {
	 fscanf(fp," %s %d %d %d",&player[w].name,&player[w].fgcolor,&player[w].bgcolor,&player[w].fscore);
	 for(q=0;q<20;)
        {
        if(wname[q]==player[w].name[q]) q++;//判断名字和之前存储的是否相同，相同时q=20 不同时，q=50 
        else q=50;
        }
	 for(i=0;i<20;i++) 
     for(a=0;a<16;a++) 
		{ 
		fscanf(fp," %d",&player[w].shu[i][a] ) ;
		if(q==20)
		  { 
		  data[i][a]=player[w].shu[i][a];
		  yan= player[w].fgcolor,bkcolor=player[w].bgcolor; //给当前的方块颜色和背景颜色变量赋值 
		  score=player[w].fscore;
		  flag=w;
		  bz=1;  //设置标志 ：玩家的名称和之前的输入有相同的 ;
	      }
        } 
     w++;     //第一轮读完后，若没有相同的名字，则继续下轮 
     }
    if(w>99) w=99;       // 确定玩家范围不会超出，如果超出，将覆盖最后一位玩家的资料 
    if(bz==1) fan=w; 
	else  fan=w+1,flag=w;
	strcpy(player[flag].name,wname);//赋值玩家名称 
	} 
}   

// 数据全部清零操作 
int allstart()
{
	int i,a;
	for(i=0;i<20;i++)
    for(a=0;a<16;a++) data[i][a]=0;
}
