//��������³�
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

int map[M][M];//����һ��M*M�ĵ�ͼ
//��Ҹ���-1���������-2��ǽ1��·0���ƶ�ʯ��3����ʯ2
int x, y;//����
int GhostNum;//��������
int num;//�����ͷ��������
int gameover;//��Ϸ�������
int win;//�ж���Ϸͨ��
int score=0;//����
int levels=1;//�ؿ���
int lastplayerwealth=2;//��һ����ҵĲƸ�;
//��������
void Gotoxy(int x, int y);//�ƶ����
void HideCursor();//���ع��
void ColorChoose(int color);//��ɫѡ����
void BuildMap();//�����ͼ����
void ShowUI();//��ʾ��Ϸ����
void SideScreen();//����Ļ
void Gameover();//�ж���Ϸ�Ƿ����
void GameClearance();//�ж�ͨ��
void PlayGame();//��ʼ��Ϸ
void ClearRock();//�������
void UpdateSideScreen();//���¸���Ļ
void BossMove(class Ghost *p);//Boss׷��

//�����
class Player
{
    public:
        int live;//����ֵ
        int wealth;//�Ƹ�ֵ
        int x,y;//���λ��
        //�������
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
        //��Ҽ���ϰ�
        int DetectObstacle(int x,int y)
        {
            if(map[y][x]==3 || map[y][x]==1  || map[y][x]==-1)
                return 1;//���ϰ�
            return 0;
        }
        //��ʾ���
        void ShowPlayer(int x, int y, int flag)
        {
            Gotoxy(2*x, y);
            if (flag == 1)
            {
                ColorChoose(4);
                printf("��");
                map[y][x]=-1;
                
            }
            if (flag == 0)
            {
                map[y][x]=0;
                printf("  ");        
            }
        } 
        //��ñ�ʯ
		void GetRuby(int x, int y)
        {
            if(map[y][x]==2)
            {
                this->wealth++;
                UpdateSideScreen();
            }
        }  
        //���ƶ�ʯ�����
        void ProduceRock()
        {
            if(this->wealth>0 && map[17][10]!=3 && map[17][10]!=-1)
            {
                map[17][10]=3;
                Gotoxy(2*10, 17);
                ColorChoose(5);
                printf("��");   
                this->wealth--;
                UpdateSideScreen();
                
            }
        }
         //�����ƶ�ʯ��
        int Detectpushable(int x,int y)
        {
            if(map[y][x]==3)
                return 1;
            return 0;
        }
        //���ƶ�ʯ����ʾ
        void ShowRock(int x, int y, int flag)
        {
            Gotoxy(2*x, y);
            if (flag == 1)
            {
                map[y][x]=3;
                ColorChoose(5);
                printf("��"); 
                
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
                    case 72://����
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
                    case 75://����
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
                    case 77://����
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
                    case 80://����
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

//������
class Ghost
{
    public:
        int live;//����ֵ
        int x,y;//����λ��
        int direction;//�ƶ�����
        int flag;//�������
        void CreateGhost()
        {
            srand(time(NULL));
            this->x=10;
            this->y=1;
            this->direction=(rand()%4+1);
            this->live=100;
            this->flag=1;
        }
        //�����ɱ�ʯ��ʾ
        void ShowRuby(int x,int y)
        {
            if(x==0 && y==0)
                return;
            Gotoxy(2*x, y);
            ColorChoose(2);
            printf("��");  

        }
        //������ʾ
        void ShowGhost(int x,int y,int flag)
        {
            Gotoxy(2*x, y);
            if (flag == 1)
            {
                map[y][x]=-2;
                ColorChoose(1);
                printf("��");  
                
            }
            if (flag == 0)
            {
                map[y][x]=0;
                ColorChoose(4);
                printf("  ");        
            }

        }
        //�������ϰ�
        int GhostCheck(int x,int y,int direction)
        {
            switch (direction)
            {
                case UP: //��1��ǽ          ��2����ʯ       ��3�����ƶ���ʯ��
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
        //�����ƶ�
        void MoveGhost()
        {

            srand(time(NULL));
            if(this->live>0)//����������
            {
                
                if(!this->GhostCheck(this->x,this->y,this->direction))//ǰ�����ϰ�
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
                else//  //���ǰ�����ϰ�
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



//�ƶ����
void Gotoxy(int x, int y) 
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord; 
	coord.X = x;
	coord.Y = y; 
	SetConsoleCursorPosition(handle, coord);
}
//���ع��
void HideCursor() 
{                  //CONSOLE_CURSOR_INFO�ṹ���������̨������Ϣ,DWORD dwSize���ٷֱȺ�ȣ�1~100����BOOL bVisible����Ƿ�ɼ�
	CONSOLE_CURSOR_INFO cursor_info={1,0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info); //SetConsoleCursorInfo��������ָ���Ŀ���̨���Ĵ�С�Ϳɼ��ԡ�
}
//��ɫѡ����
void ColorChoose(int color)   
{
	switch(color)
	{
	   	case 1:               //����ɫ
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN|FOREGROUND_BLUE);
			break;
		case 2:               //��ɫ
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN);	
			break;
		case 3:               //��ɫ
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN);
			break;
		case 4:               //��ɫ
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED);
			break;
		case 5:               //��ɫ
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_BLUE);
			break;
		case 6:               //��ɫ
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN);
			break;
		case 7:               //����ɫ
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_BLUE);
			break;
	}
}
//�����ͼ����
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
//��ʾ��Ϸ����
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
				        printf("�x");//�ϱ߽�
                        else
                        printf("��");//�±߽�
                        }
                        
                        
                    }
                    else if(j==0 || j==M-1)
                    {
                        ColorChoose(7);
                        if(j==0)
				        printf("��");//��߽�
                        else
                        printf("��");//�ұ߽�

                    }
                    else
                    {
                        
                      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED);
				      printf("��");//��ש
                    }
                break;
                case -1:
                    Gotoxy(2*j,i);
                    ColorChoose(3);
				    printf("��");//����
                break;
            }
        }
	}
}

//����Ļ
void SideScreen ()  
{                  
	Gotoxy(45,2);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN|FOREGROUND_RED);
	printf("��     ��");
	Gotoxy(45,4);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_BLUE);
	printf("��  ����");
    Gotoxy(45,6);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN);
	printf("�� Ǯ��");
	Gotoxy(51,9);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN|FOREGROUND_RED);
	printf("��  ��");
	Gotoxy(45,11);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN);
	printf("�����  ��������  �ƶ�");
	Gotoxy(45,13);
	printf("x�� ��������");
}
//���¸���Ļ����
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

        
//�ж���Ϸ����
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
        printf("��Ϸ����"); 
        Gotoxy(12,11);
        printf("��Enter�����¿�ʼ"); 
        Gotoxy(15,12);
        printf("��Esc���˳���Ϸ"); 
       
        while(1)//�ȴ�ѡ��
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
                        PlayGame();//��������ʼ��Ϸ
                    
                    break;
                    case 27:
                        exit(0);//esc���˳�
                    
                    break;
                }
            }
        }
    }
    
}
//�ж���Ϸͨ��
void GameClearance()
{
    if(Ghost[GhostNum-1].live==0 && map[Ghost[GhostNum-1].y][Ghost[GhostNum-1].x]==2)
    {
        win=1;
        Gotoxy(17,10);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED);
        printf("��ϲͨ��!"); 
        Gotoxy(12,11);
        printf("��Enter��������һ��"); 
        Gotoxy(15,12);
        printf("��Esc���˳���Ϸ"); 
       
        while(1)//�ȴ�ѡ��
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
                        PlayGame();//��������ʼ��Ϸ
                    
                    break;
                    case 27:
                    
                        exit(0);//esc���˳�
                    
                    break;
                }
            }
        }
    }

}
//************���߳�*************

VOID GhostThread(PVOID p)//�����ƶ��߳�
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
            Ghost[i].MoveGhost();//�����ƶ�
  			if(win)
                break;
           
    	}
		Sleep(200);
    }
}
VOID PlayerThread(PVOID p)//����ƶ��߳�
{
	while(1)
	{
        Player.MovePlayer();
		Sleep(10);
	}
}
VOID ClrRock(PVOID p)//ÿʮ������ʯ�齫��ʧ
{
    
    while(1)
    {
        Sleep(50000);
        ClearRock();   
    }
}

//���̺߳ϲ�
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
        Sleep(5000);//ÿ��5���ͷ�һ������
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
                angle = atan(fabs(deltay/deltax)); // ��һ����
            else if(deltax<0 && deltay>0 )
                angle = Pi-atan(fabs(deltay/deltax)); // �ڶ�����
            else if( deltax<0 && deltay<0 )
                angle = Pi+atan(fabs(deltay/deltax)); // ��������
            else
                angle = 2*Pi-atan(fabs(deltay/deltax)); // ��������
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
