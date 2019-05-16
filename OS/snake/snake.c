#include<stdio.h>
#include <pthread.h> //posix執行緒程式庫
#include <sys/time.h>
#include <curses.h>//額外的UI程式庫
#include <stdlib.h>
#include <unistd.h> // 解决usleep报错问题
 
#define MAX_X 55   //寬度
#define MAX_Y 25  //長度
#define Xposition 4  
#define Yposition 2   
 
int Length = 4;  //貪吃蛇初始長度
int life = 1;   
int input = 0;  
pthread_t id; //聲明Linux執行緒
void FoodCheck();
void FoodProduce();
void Initializition();//初始化執行緒，生成食物和蛇
void SnakeMoveDirect();
void SnakeMoveFollow();
void DeathCheck();
void DrawArea();
void * waitForKey(void *);//定義另一個執行緒
void drawDot(int x, int y,char s);
void clearDot(int x, int y);
void end();
 
struct point {
 int x;
 int y;
};
struct point SnakeMain[50];
struct point food;
 
 
int main(int argc, char** argv) {
 //
//
 Initializition();
 	 //system("date");
 refresh();//刷新
 while (life) {
 DrawArea();
 usleep(200000);//頻率0.2s
 SnakeMoveDirect();
 DeathCheck();
 }
 end();
 return 0;
}
 
void * waitForKey(void *para) {
 while (1) {
usleep(1000);
 input = getch();
 }
}
 
void end() {
	//test
 move(1, 0);
 addstr("GAME OVER use any key to esc");
 refresh();
 getch();
 endwin();
}
 
//生成食物
 
void FoodProduce() {
 int fposition = 0;
 int i;
 srand(time(NULL));
 do {
 food.x = (rand() % ((MAX_X-2) / 2))*2+2;  
 food.y = rand() % (MAX_Y-1)+1;   
 for (i = 0; i < Length; i++) {
 if (food.x == SnakeMain[i].x && food.y == SnakeMain[i].y)
 fposition = 1;
 }
 } while (fposition);
  //
}
 
//初始化，身體4長度
 
void Initializition() {
 initscr();//curses初始化
 noecho();//不顯示輸入的字符
 int i;
 //蛇初始化
 for (i = 3; i <= 6; i++) {
 SnakeMain[6 - i].x = 4;
 SnakeMain[6 - i].y = i;
 }
 FoodProduce();
 int ret;
 ret = pthread_create(&id, NULL, waitForKey, NULL);//建立執行緒
 if (ret != 0) {
 exit(1);
 }
 
 for ( i = 0; i <= MAX_X; i+=2) {  //繪制邊界
 drawDot(i, 0,'*');
 drawDot(i, MAX_Y,'*');
 }
 
 for (i = 0; i <= MAX_Y; i++) {
 drawDot(0, i,'*');
 drawDot(MAX_X, i,'*');
 }
 
}
 
//移動
void SnakeMoveFollow() {
 int i;
 for (i = Length - 1; i > 0; i--) {
 SnakeMain[i].x = SnakeMain[i - 1].x;
 SnakeMain[i].y = SnakeMain[i - 1].y;
 }
 
}
 
void SnakeMoveDirect() {
 
 clearDot(SnakeMain[Length - 1].x, SnakeMain[Length - 1].y);
 int directionX, directionY; 
 int newX, newY;
 newX = SnakeMain[0].x;
 newY = SnakeMain[0].y;
 directionX = SnakeMain[0].x - SnakeMain[1].x;
 directionY = SnakeMain[0].y - SnakeMain[1].y;
 if (input == 'h' && directionY<=0) 
 newY--;
 else if (input == 'n' && directionY>=0 )
 newY++;
 else if (input == 'b' && directionX<=0)
 newX -= 2; 
 else if (input == 'm' && directionX>=0)
 newX += 2;
 else {
 newX += directionX;
 newY += directionY;
 }
 FoodCheck();
 SnakeMoveFollow();
 SnakeMain[0].x = newX;
 SnakeMain[0].y = newY;
 
}
//監測食物並增加長度
 
void FoodCheck() {
 if (food.x == SnakeMain[0].x && food.y == SnakeMain[0].y) {
 Length = Length + 1;
 FoodProduce();
 }
}
//監測死亡
 
void DeathCheck() {
 int i;
 if (SnakeMain[0].x <=1 || SnakeMain[0].x >= MAX_X  || SnakeMain[0].y <= 0 || SnakeMain[0].y >=MAX_Y)
 life = 0;
 for (i = 4; i < Length; i++)
 if (SnakeMain[0].x == SnakeMain[i].x && SnakeMain[0].y == SnakeMain[i].y)
 life = 0;
}
 
//繪制
 
void DrawArea() {
 int i = 0;
 drawDot(SnakeMain[i].x, SnakeMain[i].y,'H');
 for (i=1; i < Length; i++) {
 drawDot(SnakeMain[i].x, SnakeMain[i].y,'@');
 }
 drawDot(food.x, food.y,'+');
 move(0,0);
 refresh();
//刷新
}
 
void drawDot(int x, int y,char s) {
 move(y+Yposition, x+Xposition);
 addch(s);
}
 
void clearDot(int x, int y) {
 move(y+Yposition, x+Xposition);
 addch(' ');
}
