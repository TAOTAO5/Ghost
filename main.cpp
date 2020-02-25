//幽灵与地下城
#include<iostream>
#include<stdio.h>
#include<Windows.h>
#include<conio.h>
#include<time.h> 
#include<math.h>
#include<process.h>
#include<stdlib.h>

#define M 20
#define UP    1
#define DOWN  2
#define LEFT  3
#define RIGHT 4

int map[M][M];//构造一个M*M的地图
//玩家格子-1，幽灵格子-2，墙1，路0，推动石块3，宝石2
int x, y;//坐标
int GhostNum;//幽灵数量
int num;//用于释放幽灵计数
int gameover;//游戏结束标记
int win;//判断游戏通过
int score=0;//分数
int levels=1;//关卡数
int lastplayerwealth=2;//上一局玩家的财富;
//辅助函数
void Gotoxy(int x, int y);//移动光标
void HideCursor();//隐藏光标
void ColorChoose(int color);//颜色选择函数
void BuildMap();//随机地图创建
void ShowUI();//显示游戏界面
void SideScreen();//副屏幕
void Gameover();//判断游戏是否结束
void GameClearance();//判断通过
void PlayGame();//开始游戏
void ClearRock();//清除方块
void UpdateSideScreen();//更新副屏幕
void BossMove(class Ghost *p);//Boss追踪

//玩家类
class Player
{
    public:
        int live;//生命值
        int wealth;//财富值
        int x,y;//玩家位置
        //创建玩家
        void CreatePlayer()
        {
            this->wealth=lastplayerwealth;
            this->live=100;
            this->x=M-M/2-1;
            this->y=M-2;
            win=0;
            gameover=0;
            GhostNum=5;
            num=1;
        }
        //玩家检测障碍
        int DetectObstacle(int x,int y)
        {
            if(map[y][x]==3 || map[y][x]==1  || map[y][x]==-1)
                return 1;//有障碍
            return 0;
        }
        //显示玩家
        void ShowPlayer(int x, int y, int flag)
        {
            Gotoxy(2*x, y);
            if (flag == 1)
            {
                ColorChoose(4);
                printf("♀");
                map[y][x]=-1;
                
            }
            if (flag == 0)
            {
                map[y][x]=0;
                printf("  ");        
            }
        } 
        //获得宝石
		void GetRuby(int x, int y)
        {
            if(map[y][x]==2)
            {
                this->wealth++;
                UpdateSideScreen();
            }
        }  
        //可推动石块产生
        void ProduceRock()
        {
            if(this->wealth>0 && map[17][10]!=3 && map[17][10]!=-1)
            {
                map[17][10]=3;
                Gotoxy(2*10, 17);
                ColorChoose(5);
                printf("■");   
                this->wealth--;
                UpdateSideScreen();
                
            }
        }
         //检测可推动石块
        int Detectpushable(int x,int y)
        {
            if(map[y][x]==3)
                return 1;
            return 0;
        }
        //可推动石块显示
        void ShowRock(int x, int y, int flag)
        {
            Gotoxy(2*x, y);
            if (flag == 1)
            {
                map[y][x]=3;
                ColorChoose(5);
                printf("■"); 
                
            }
            if (flag == 0)
            {
                map[y][x]=0;
                printf("  ");

            }
        }
        void MovePlayer()
        {
            
            if (kbhit())
            {
                char m;
                m = getch();
                switch (m)
                {   
                    case 72://向上
                        if(!this->DetectObstacle(this->x,this->y-1))
                        {
                            this->ShowPlayer(this->x,this->y,0);
                            this->y--;;
                            this->GetRuby(this->x,this->y);
                            this->ShowPlayer(this->x,this->y,1);
                        }
                        else 
                        {
                            if(this->Detectpushable(this->x,this->y-1) && (!this->DetectObstacle(this->x,this->y-2)))
                            {
                                this->ShowRock(this->x,this->y-1,0);
                                this->ShowPlayer(this->x,this->y,0);
                                this->y--;;
                                this->ShowRock(this->x,this->y-1,1);
                                this->ShowPlayer(this->x,this->y,1);
                            }
                        }
                                
                    break;
                    case 75://向左
                        if(!this->DetectObstacle(this->x-1,this->y))
                        {
                            this->ShowPlayer(this->x,this->y,0);
                            this->x--;
                            this->GetRuby(this->x,this->y);
                            this->ShowPlayer(this->x,this->y,1);
                        }
                        else
                        { 
                            if(this->Detectpushable(this->x-1,this->y) && (!this->DetectObstacle(this->x-2,this->y)))
                            {
                                this->ShowRock(this->x-1,this->y,0);
                                this->ShowPlayer(this->x,this->y,0);
                                this->x--;
                                this->ShowRock(this->x-1,this->y,1);
                                this->ShowPlayer(this->x,this->y,1);
                            }
                        }
                    break;
                    case 77://向右
                        if(!this->DetectObstacle(this->x+1,this->y))
                        {
                            this->ShowPlayer(this->x,this->y,0);
                            this->x++; 
                            this->GetRuby(this->x,this->y);
                            this->ShowPlayer(this->x,this->y,1);
                        }
                        else
                        {
                            if(this->Detectpushable(this->x+1,this->y) && (!this->DetectObstacle(this->x+2,this->y)))
                            {
                                this->ShowRock(this->x+1,this->y,0);
                                this->ShowPlayer(this->x,this->y,0);
                                this->x++; 
                                this->ShowRock(this->x+1,this->y,1);
                                this->ShowPlayer(this->x,this->y,1);
                            }
                        }
                    break;
                    case 80://向下
                        if(!this->DetectObstacle(this->x,this->y+1))
                        {
                            this->ShowPlayer(this->x,this->y,0);
                            this->y++;
                            this->GetRuby(this->x,this->y);
                            this->ShowPlayer(this->x,this->y,1);
                        }
                        else
                        {
                            if(this->Detectpushable(this->x,this->y+1) && (!this->DetectObstacle(this->x,this->y+2)))
                            {
                                this->ShowRock(this->x,this->y+1,0);
                                this->ShowPlayer(this->x,this->y,0);
                                this->y++;
                                this->ShowRock(this->x,this->y+1,1);
                                this->ShowPlayer(this->x,this->y,1);
                            }
                        }
                    break;
                    case 'x':
                        this->ProduceRock();
                    break;

                }
            }
                    
        }

}Player; 

//幽灵类
class Ghost
{
    public:
        int live;//生命值
        int x,y;//幽灵位置
        int direction;//移动方向
        int flag;//死亡标记
        void CreateGhost()
        {
            srand(time(NULL));
            this->x=10;
            this->y=1;
            this->direction=(rand()%4+1);
            this->live=100;
            this->flag=1;
        }
        //幽灵变成宝石显示
        void ShowRuby(int x,int y)
        {
            if(x==0 && y==0)
                return;
            Gotoxy(2*x, y);
            ColorChoose(2);
            printf("◆");  

        }
        //幽灵显示
        void ShowGhost(int x,int y,int flag)
        {
            Gotoxy(2*x, y);
            if (flag == 1)
            {
                map[y][x]=-2;
                ColorChoose(1);
                printf("※");  
                
            }
            if (flag == 0)
            {
                map[y][x]=0;
                ColorChoose(4);
                printf("  ");        
            }

        }
        //幽灵检测障碍
        int GhostCheck(int x,int y,int direction)
        {
            switch (direction)
            {
                case UP: //“1”墙          “2”宝石       “3”可推动的石块
                    if(map[y-1][x]==1 || map[y-1][x]==2 || map[y-1][x]==3)
                        return 1;
                    else
                        return 0;
                break;  
                case DOWN: 
                    if(map[y+1][x]==1 || map[y+1][x]==2 || map[y+1][x]==3)
                        return 1;
                    else
                        return 0;
                break;  
                case LEFT:
                    if(map[y][x-1]==1 || map[y][x-1]==2 || map[y][x-1]==3)
                        return 1;
                    else
                        return 0;
                break;  
                case RIGHT:
                    if(map[y][x+1]==1 || map[y][x+1]==2 || map[y][x+1]==3)
                        return 1;
                    else
                        return 0;
                break;  
            }
        }
        //幽灵移动
        void MoveGhost()
        {

            srand(time(NULL));
            if(this->live>0)//如果幽灵活着
            {
                
                if(!this->GhostCheck(this->x,this->y,this->direction))//前方无障碍
                {
                    switch (this->direction)
                    {
                        case UP: 
                            this->ShowGhost(this->x,this->y,0);
                            this->y--;
                            this->ShowGhost(this->x,this->y,1);
                        break;  
                        case DOWN: 
                            this->ShowGhost(this->x,this->y,0);
                            this->y++;
                            this->ShowGhost(this->x,this->y,1);
                        break;  
                        case LEFT:
                            this->ShowGhost(this->x,this->y,0);
                            this->x--;
                            ShowGhost(this->x,this->y,1);
                        break;  
                            case RIGHT:
                            this->ShowGhost(this->x,this->y,0);
                            this->x++;
                            this->ShowGhost(this->x,this->y,1);
                        break;  
                    }
                }
                else//  //如果前方有障碍
                {
                    while(this->GhostCheck(this->x,this->y,this->direction))
                    {                
                        this->direction=(rand()%4+1);
                    }

                }
                this->live--;
                
            }
            else
            {
                if(this->flag)
                {  
                    map[this->y][this->x]=2;
                    this->flag=0;
                    this->ShowRuby(this->x,this->y);
                    score+=5;
                    UpdateSideScreen();
                }
            }
        }
        

}Ghost[100];



//移动光标
void Gotoxy(int x, int y) 
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord; 
	coord.X = x;
	coord.Y = y; 
	SetConsoleCursorPosition(handle, coord);
}
//隐藏光标
void HideCursor() 
{                  //CONSOLE_CURSOR_INFO结构体包含控制台光标的信息,DWORD dwSize光标百分比厚度（1~100）和BOOL bVisible光标是否可见
	CONSOLE_CURSOR_INFO cursor_info={1,0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info); //SetConsoleCursorInfo用来设置指定的控制台光标的大小和可见性。
}
//颜色选择函数
void ColorChoose(int color)   
{
	switch(color)
	{
	   	case 1:               //天蓝色
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN|FOREGROUND_BLUE);
			break;
		case 2:               //绿色
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN);	
			break;
		case 3:               //黄色
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN);
			break;
		case 4:               //红色
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED);
			break;
		case 5:               //紫色
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_BLUE);
			break;
		case 6:               //白色
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN);
			break;
		case 7:               //深蓝色
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_BLUE);
			break;
	}
}
//随机地图创建
void BuildMap()
{
    srand(time(NULL));
    for(int i=0;i<M;i++)
    {
        map[0][i]=1;
        map[M-1][i]=1;
    }
    for(int i=0;i<M;i++)
    {
        map[i][0]=1;
        map[i][M-1]=1;
    }
    for(int i=1; i<M-1; i++)
    {
        for(int j=1; j<M-1; j++)
        {
            if(i<=14)
            {
                if(!(rand()%10))
                    map[i][j]=1;
                else
                {
                    map[i][j]=0;
                }
            }
            else
            {

                if(j<=4 || j>=16)
                {
                    if(!(rand()%7))
                        map[i][j]=1;
                    else
                    {
                        map[i][j]=0;
                    }
                }
                else
                {
                    map[i][j]=0;
                }
                
            }
            
            
        }
    }
    map[M-2][M-M/2]=-1;
    map[M-2][M-M/2-1]=-1;
}
void ClearRock()
{
    for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < M; j++)
		{
			
            if(map[i][j]==3)
                Player.ShowRock(j, i, 0);
            
        }
	}
}
//显示游戏界面
void ShowUI()
{
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < M; j++)
		{
			switch (map[i][j])
            {
                case 0:
                    Gotoxy(2 * j, i);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
				    printf("  ");
                break;
                case 1:
                    Gotoxy(2 * j, i);
                    if((i==0 || i==M-1) )
                    {
                        ColorChoose(7);
                        if(j!=0 && j!=M-1)
                        {
                        if(i==0)
				        printf("▁");//上边界
                        else
                        printf("▔");//下边界
                        }
                        
                        
                    }
                    else if(j==0 || j==M-1)
                    {
                        ColorChoose(7);
                        if(j==0)
				        printf("▕");//左边界
                        else
                        printf("▏");//右边界

                    }
                    else
                    {
                        
                      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED);
				      printf("▓");//红砖
                    }
                break;
                case -1:
                    Gotoxy(2*j,i);
                    ColorChoose(3);
				    printf("★");//基地
                break;
            }
        }
	}
}

//副屏幕
void SideScreen ()  
{                  
	Gotoxy(45,2);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN|FOREGROUND_RED);
	printf("第     关");
	Gotoxy(45,4);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_BLUE);
	printf("分  数：");
    Gotoxy(45,6);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN);
	printf("金 钱：");
	Gotoxy(51,9);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN|FOREGROUND_RED);
	printf("帮  助");
	Gotoxy(45,11);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN);
	printf("方向键  ←↑→↓  移动");
	Gotoxy(45,13);
	printf("x键 产生方块");
}
//更新副屏幕数据
void UpdateSideScreen()
{
    Gotoxy(49,2);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN|FOREGROUND_RED);
	printf("%d",levels);
	Gotoxy(54,4);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_BLUE);
	printf("%d",score);
    Gotoxy(54,6);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN);
	printf("%d$",Player.wealth);
	
}

        
//判断游戏结束
void Gameover()
{
    int flag=0;
    for(int i=0; i<20; i++)
    {
        for(int j=0; j<20; j++)
        {
           
            if(map[i][j]==-1)
                flag++;
            
        }
    }
    if(flag<2)
    {    
        gameover=1;
        Gotoxy(17,10);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED);
        printf("游戏结束"); 
        Gotoxy(12,11);
        printf("按Enter键重新开始"); 
        Gotoxy(15,12);
        printf("按Esc键退出游戏"); 
       
        while(1)//等待选择
	    {
            if (kbhit())
            {
                char m;
                m = getch();
                switch (m)
                {
                    case 13:
                    
                        system("cls");
                        levels=1;
                        GhostNum=3;
                        lastplayerwealth=2;
                        PlayGame();//清屏，开始游戏
                    
                    break;
                    case 27:
                        exit(0);//esc，退出
                    
                    break;
                }
            }
        }
    }
    
}
//判断游戏通关
void GameClearance()
{
    if(Ghost[GhostNum-1].live==0 && map[Ghost[GhostNum-1].y][Ghost[GhostNum-1].x]==2)
    {
        win=1;
        Gotoxy(17,10);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED);
        printf("恭喜通关!"); 
        Gotoxy(12,11);
        printf("按Enter键进入下一关"); 
        Gotoxy(15,12);
        printf("按Esc键退出游戏"); 
       
        while(1)//等待选择
	    {
            if (kbhit())
            {
                char m;
                m = getch();
                switch (m)
                {
                    case 13:
                    
                        system("cls");
                        levels++;
                        GhostNum+=3;
                        lastplayerwealth=Player.wealth;
                        PlayGame();//清屏，开始游戏
                    
                    break;
                    case 27:
                    
                        exit(0);//esc，退出
                    
                    break;
                }
            }
        }
    }

}
//************多线程*************

VOID GhostThread(PVOID p)//幽灵移动线程
{
	while(1)
	{
        Gameover();
        GameClearance();
	    for(int i=0;i<num;i++){
            if(gameover)
                break;
            if(i==GhostNum-1)
                BossMove(&Ghost[i]);
            Ghost[i].MoveGhost();//依次移动
  			if(win)
                break;
           
    	}
		Sleep(200);
    }
}
VOID PlayerThread(PVOID p)//玩家移动线程
{
	while(1)
	{
        Player.MovePlayer();
		Sleep(10);
	}
}
VOID ClrRock(PVOID p)//每十秒所有石块将消失
{
    
    while(1)
    {
        Sleep(50000);
        ClearRock();   
    }
}

//总线程合并
void AllThreads()
{
	_beginthread(GhostThread,0,0);
    _beginthread(PlayerThread,0,0);
    _beginthread(ClrRock,0,0);

}

void PlayGame()
{
    Player.CreatePlayer();
    BuildMap();
    ShowUI();
    srand(time(NULL));
    SideScreen();
    UpdateSideScreen();
    for(int i=0;i<GhostNum;i++)
    {
         Ghost[i].CreateGhost();
    }
    Player.ShowPlayer(Player.x,Player.y,1);
    AllThreads();
    while(1)
    {
        Sleep(5000);//每隔5秒释放一个幽灵
        if(num<=GhostNum)
            num++;
        UpdateSideScreen();
    }
}
void BossMove(class Ghost *p)
{
            int a=0,b=0;
            double angle=0;
            double Pi=3.14159265358;
            double deltax = 0;
            double deltay = 0;
            deltax=Player.x - p->x; 
            deltay=Player.y - p->y;
            if(deltax==0)
            {
            if(Player.y >= p->y)
            deltax = 0.0000001;
            else
            deltax = -0.0000001;
            }
            if(deltay == 0)
            {
            if( Player.x >= p->x ) 
            deltay = 0.0000001;
            else 
            deltay = -0.0000001;
            }
            if(deltax>0 && deltay>0)
                angle = atan(fabs(deltay/deltax)); // 第一项限
            else if(deltax<0 && deltay>0 )
                angle = Pi-atan(fabs(deltay/deltax)); // 第二项限
            else if( deltax<0 && deltay<0 )
                angle = Pi+atan(fabs(deltay/deltax)); // 第三项限
            else
                angle = 2*Pi-atan(fabs(deltay/deltax)); // 第四项限
            if(a>=0)
                a=(int)(cos(angle)+0.5);
            else
                a=(int)(cos(angle)-0.5);
            if(b>=0)
                b=(int)(sin(angle)+0.5);
            else
                b=(int)(sin(angle)-0.5);
            if(a==1  && b==0)
                p->direction=RIGHT;
            if(a==0 && b==1)
                p->direction=DOWN;
            if(a==0 && b==0)
                p->direction=UP;
                p->direction=LEFT;

                 
}
int main()
{
    HideCursor();
    PlayGame();
    return 0;
}
