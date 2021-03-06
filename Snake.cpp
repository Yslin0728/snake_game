#include <stdio.h>
#include <stdlib.h>
#include <conio.h> 
#include <graphics.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <iostream>
#include "mmsystem.h" 
#include<pthread.h>

#define SCREEN_HEIGHT 500 //設定遊戲視窗高度 
#define SCREEN_WIDTH 500 //設定遊戲視窗寬度
#define GRID_SIDE 40 //設定遊戲方陣每邊格子數量 
#define LEFT_MARGINE 30 //設定左邊界 
#define TOP_MARGINE 40 //設定上邊界 
#define FOOD_AMOUT 1 //設定每次產生食物數量 
#define INIT_SPEED  80 //設定初始貪食蛇移動速度 


//宣告前進方向列舉函數 
enum Direction {
	RIGHT, 
	LEFT, 
	UP, 
	DOWN 
};

//宣告遊戲場出現物體列舉函數 
enum Object {
	BACKGROUND, //背景 
	WALL, //牆或障礙物 
	FOOD, //食物
	BACKGROUND2,//背景黑
	SPEED,//吃到變慢 
 	POINTT,//吃到加十分 
 	LENGTH//吃到多三節 
 
};

//宣告貪食蛇身體節點結構 
struct Node {
	int row; //節點位在第幾行 
	int col; //節點位在第幾列 
	Direction direct; //該節點的前進方向 
	struct Node *next;	//指向下一個節點 
}; 

//定義指向節點結構的指標變數 
typedef struct Node *NodePointer;

void openWindow(); //開啟游戲視窗 
void closeGame(NodePointer snake); //處理遊戲結束邏輯 
int playGame(int field[][GRID_SIDE], NodePointer snake); //遊戲進行邏輯 
bool IsGameOver(NodePointer snake, int field[][GRID_SIDE]); //貪食蛇是否死亡(死亡條件：撞牆和撞到自己身體) 
int showGameOverMsg(); //遊戲結束訊息
void showInfo(); //顯示遊戲相關資訊 
void drawGameField(int field[][GRID_SIDE]); //繪製遊戲區域 
void drawSquare(int row, int col, int color); //繪製方塊 
void controlSnakeDirection(int field[][GRID_SIDE], NodePointer snake); //讀取鍵盤方向輸入，並設定到所有貪食蛇節點 
void moveNext(NodePointer snake); //繪製貪食蛇每前進一步的身體改變  
void createFood(int field[][GRID_SIDE], NodePointer snake); //產生食物 
bool IsAtWall(int field[][GRID_SIDE], int row, int col); //判斷是否撞到牆  
bool IsAtSnake(NodePointer snake, int row, int col); //判斷是否撞到貪食蛇自己的身體
void eatFood(int field[][GRID_SIDE], NodePointer snake); //處理吃到食物邏輯 
void addNode(NodePointer snake); //增加貪食蛇身體節點 
void playmusic();
void introduction(NodePointer snake);
int original();
Direction AI(int field[][GRID_SIDE], NodePointer snake);//實作電腦控制貪食蛇邏輯 
void load(int x,int y) ;
void loading();
int playcombat(int field[][GRID_SIDE], NodePointer snake,NodePointer snake2);
void controlSnakeDirection2(int field[][GRID_SIDE], NodePointer snake);
void closeGame2(NodePointer snake,NodePointer snake2);

void createSpeed(int field[][GRID_SIDE], NodePointer snake); //產生減速食物 
void eatSpeed(int field[][GRID_SIDE], NodePointer snake); //處理吃到減速食物邏輯
void createPoint(int field[][GRID_SIDE], NodePointer snake); //產生加分食物 
void eatPoint(int field[][GRID_SIDE], NodePointer snake); //處理吃到加分食物邏輯
void createLength(int field[][GRID_SIDE], NodePointer snake); //產生增長食物 
void eatLength(int field[][GRID_SIDE], NodePointer snake); //處理吃到加長度食物邏輯
//void * pthread_func2(void * data_ptr);
//void * pthread_func1(void * data_ptr);
void combat();

int speed; //貪食蛇移動速度 
int scoreSum = 0; //紀錄分數 
int totalTime = 0; //紀錄遊戲時間 
int const scorePerFood = 1; //每一份食物可得分數 
bool meetSelfWillDie = false; //決定貪食蛇撞到自己身體是否會死掉 
bool IFPlayAI = false; //是否開啟AI模式 
bool change = false;
int food_row;
int food_col;
int field[GRID_SIDE][GRID_SIDE] = {{3,3,3,3,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3},
		   						   {3,3,3,3,3,3,3,3,1,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,1,3,3,3,3,3,3,3,3},
								   {3,3,3,3,3,3,3,1,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,1,3,3,3,3,3,3,3},
								   {3,3,3,3,3,3,1,0,0,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,0,0,1,3,3,3,3,3,3},
								   {1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							   	   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
								   {3,3,3,1,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,3,3,3,3,1,3,3,3},
								   {3,3,3,1,3,1,3,1,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,1,3,1,3,1,3,3,3},
								   {3,3,3,1,3,1,3,1,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,1,3,1,3,1,3,3,3},
								   {3,3,3,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,3,3,3}};	

int main(){
	openWindow();
	
	Node head = {10, 11, RIGHT, NULL}; //設定蛇頭初始位置和方向 
	NodePointer snake = &head;
	Node head2 = {18, 11, RIGHT, NULL}; //設定蛇頭初始位置和方向 
	NodePointer snake2 = &head2;
	
	introduction(snake);
}

void introduction(NodePointer snake){
 	cleardevice();
 	playmusic();
 	char msg1[70] = "Welcome to  Nyan cat world !!!           ";
 	char msg2[70] = "此遊戲有三種模式:	Nyan, AI , Harder    ";
 	char msg3[70] = "press ' o ' for  Nyan       ";
 	char msg5[70] = "press ' a ' for  AI       ";
	char msg6[70] = "press ' f ' for  Harder       ";
	 	
	int i = 0;
    for(;;i++){
     	setcolor(13);
     	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 3);
     	outtextxy(30, SCREEN_HEIGHT / 2, msg1);
     
     	setcolor(14);
     	moveto(5, 190);
     	lineto(225, 190);
     	moveto(335, 190);
     	lineto(550, 190);
     	moveto(5, 190);
     	lineto(5, 500);
     	moveto(550, 190);
     	lineto(550, 500);
     	moveto(5, 500);
     	lineto(550, 500);
		//畫輪廓
     	moveto(220, 150);
     	lineto(280, 160);
     	moveto(220, 150);
     	lineto(230, 190);
     	moveto(340, 150);
     	lineto(280, 160);
     	moveto(340, 150);
     	lineto(330, 190);
     	ellipse(280,190,0,360,50,30); 
		//畫眼睛 
  		moveto(250, 175);
     	lineto(260, 185);
     	moveto(260, 175);
     	lineto(250, 185);
     
     	moveto(300, 175);
     	lineto(310, 185);
     	moveto(310, 175);
     	lineto(300, 185);
		//畫嘴巴
     	arc(275, 200, 180,360, 5);
        arc(285, 200, 180,360, 5);
     
     	setcolor(WHITE);
     	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 1);
     	outtextxy(20, SCREEN_HEIGHT / 2 + 80, msg2);
     	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 2);
     	outtextxy(70, SCREEN_HEIGHT / 2 + 100, msg3);
     	outtextxy(70, SCREEN_HEIGHT / 2 + 120, msg5);
     	outtextxy(70, SCREEN_HEIGHT / 2 + 140, msg6);
     
     	delay(700);
      
     	if(kbhit()) {
      		char key;
   			key = getch(); 
   			
			if (key == 'o' || key == 'O' ) {
    			original();     
			}
			else if(key== 'f'|| key == 'F' ){
    			combat();
   			}
			else if(key== 'a'|| key == 'A' ){
    			IFPlayAI = !IFPlayAI;
    			original();
   			}
  		}
 	}
}
void combat(){
	cleardevice();
	int field[GRID_SIDE][GRID_SIDE] = {{3,3,3,3,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3},
		   						   {3,3,3,3,3,3,3,3,1,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,1,3,3,3,3,3,3,3,3},
								   {3,3,3,3,3,3,3,1,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,1,3,3,3,3,3,3,3},
								   {3,3,3,3,3,3,1,0,0,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,0,0,1,3,3,3,3,3,3},
								   {1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							   	   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
								   {3,3,3,1,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,3,3,3,3,1,3,3,3},
								   {3,3,3,1,3,1,3,1,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,1,3,1,3,1,3,3,3},
								   {3,3,3,1,3,1,3,1,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,1,3,1,3,1,3,3,3},
								   {3,3,3,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,3,3,3}};
								   
    								  
	while(TRUE){
		Node head = {10, 11, RIGHT, NULL}; //設定蛇頭初始位置和方向 
		NodePointer snake = &head;
		Node head2 = {18, 11, RIGHT, NULL}; //設定蛇頭初始位置和方向 
	    NodePointer snake2 = &head2;
		speed = INIT_SPEED;
		//設定遊戲場和障礙物
		char key;
		key = playcombat(field,snake,snake2); //進行遊戲
		if (key == 'q' || key == 'Q')
			closeGame2(snake,snake2); //如果玩家輸入'q'離開遊戲	
		else if (key == 's' || key == 'S'){ 
			speed = INIT_SPEED;
   			scoreSum = 0;
   			totalTime = 0;
			introduction(snake); //如果玩家輸入's' 繼續遊戲
		}		    
	}
//	pthread_t pth1;
//	pthread_t pth2;
// 	pthread_create(&pth1, NULL, pthread_func1, NULL);
// 	pthread_create(&pth2, NULL, pthread_func2, NULL);
// 	pthread_join(pth1,NULL);
// 	pthread_join(pth2,NULL);
}
//void * pthread_func1(void * data_ptr) {	
//	while(TRUE){
//		Node head = {10, 11, RIGHT, NULL}; //設定蛇頭初始位置和方向 
//		NodePointer snake = &head;
//		speed = INIT_SPEED;
//		//設定遊戲場和障礙物
//		char key;
//		key = playGame(field, snake); //進行遊戲
//		if (key == 'q' || key == 'Q')
//			closeGame(snake); //如果玩家輸入'q'離開遊戲	
//		else if (key == 's' || key == 'S'){ 
//			speed = INIT_SPEED;
//   			scoreSum = 0;
//   			totalTime = 0;
//			introduction(snake); //如果玩家輸入's' 繼續遊戲
//		}
//	}
//}
//void * pthread_func2(void * data_ptr) {	
//	while(TRUE){
//		Node head = {20, 11, LEFT, NULL}; //設定蛇頭初始位置和方向 
//		NodePointer snake2 = &head;
//		speed = INIT_SPEED;
//		//設定遊戲場和障礙物
//		playcombat(field, snake2); //進行遊戲	    
//	}
//}
int original( ){
	cleardevice();
	int field[GRID_SIDE][GRID_SIDE] = {{3,3,3,3,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3},
		   						   {3,3,3,3,3,3,3,3,1,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,1,3,3,3,3,3,3,3,3},
								   {3,3,3,3,3,3,3,1,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,1,3,3,3,3,3,3,3},
								   {3,3,3,3,3,3,1,0,0,0,0,0,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,0,0,0,0,0,1,3,3,3,3,3,3},
								   {1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							       {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
								   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							   	   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
								   {3,3,3,1,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,3,3,3,3,1,3,3,3},
								   {3,3,3,1,3,1,3,1,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,1,3,1,3,1,3,3,3},
								   {3,3,3,1,3,1,3,1,3,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,1,3,1,3,1,3,3,3},
								   {3,3,3,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,3,3,3}};
	while(TRUE){
		Node head = {10, 11, RIGHT, NULL}; //設定蛇頭初始位置和方向 
		NodePointer snake = &head;
		speed = INIT_SPEED;
		//設定遊戲場和障礙物
		char key;
		key = playGame(field, snake); //進行遊戲
		if (key == 'q' || key == 'Q')
			closeGame(snake); //如果玩家輸入'q'離開遊戲	
		else if (key == 's' || key == 'S'){ 
			speed = INIT_SPEED;
   			scoreSum = 0;
   			totalTime = 0;
			introduction(snake); //如果玩家輸入's' 繼續遊戲
		}		    
	}
}

void playmusic(){
	PlaySound(TEXT("Nyan.wav"), NULL, SND_ASYNC|SND_LOOP);
}

//開啟游戲視窗
void openWindow(){
	initwindow(SCREEN_WIDTH + LEFT_MARGINE * 3, SCREEN_HEIGHT + TOP_MARGINE * 3, "Hungry Snake Game");
}

//處理遊戲結束邏輯 
void closeGame(NodePointer snake){
	free(snake);
	exit(0);
}
void closeGame2(NodePointer snake,NodePointer snake2){
	free(snake);
	free(snake2);
	exit(0);
}

//遊戲進行邏輯
int playGame(int field[][GRID_SIDE], NodePointer snake){
	
	int i = 1;
	drawGameField(field); //繪製遊戲區域 
	createFood(field, snake); //產生第一份食物 
	
	while(true){
		controlSnakeDirection(field, snake); //讀取玩家輸入方向鍵，並將新方向設定到各貪食蛇節點 
		moveNext(snake); //依據節點的方向，繪製新的貪食蛇位置 
		eatFood(field, snake); //判斷是否有吃到食物，如果有新增節點到尾巴 
		eatSpeed(field, snake); //判斷是否有吃到速度，如果有速度變慢 
  		eatPoint(field, snake); //判斷是否有吃到分數，如果有分數 +5 
  		eatLength(field, snake); //判斷是否有吃到，如果有速度變慢
		showInfo(); //顯示時間和分數資訊 
		
		if(IsGameOver(snake, field)) //判斷是否符合遊戲結束條件， 
			return showGameOverMsg(); //顯示遊戲結束訊息，並等待玩家輸入選項 
		
		//除了吃到食物會產生新食物，系統也隨機產生新食物 
		srand((unsigned)time(NULL));
		if(rand()*rand() % 30 == 0)
   			createFood(field, snake);
  		if(rand()*rand() % 30 == 1)
   			createSpeed(field, snake);
  		if(rand()*rand() % 30 == 2)
   			createPoint(field, snake);
  		if(rand()*rand() % 30 == 3)
   			createLength(field, snake);	
		
		i++;
  		if(i % 50 == 4){
   			if(speed>=6)
				speed -= 5;
   			//printf("%.2f\n", speed);
  		}
		
		delay(speed); //決定貪食蛇移動速度，speed越小移動越快 
		
		//讀取非方向鍵的其他鍵盤輸入 
	   	if(kbhit()) {
	   		char key;
			key = getch();	
			
			//只有輸入大小寫的a, q 和 s，系統才有反應並將輸入結果拋到外層等待處理 
			if (key == 'q' || key == 'Q' || key == 's' || key == 'S')
				return key; 				
			else if (key == 'a') //決定是否改變模式 ，主要有玩家模式和電腦操控的AI模式 
				IFPlayAI = !IFPlayAI;
			//else if (key == 'f')
				//combat();
		}					
	}		
}
int playcombat(int field[][GRID_SIDE], NodePointer snake,NodePointer snake2){
	int i = 1;
	drawGameField(field); //繪製遊戲區域 
	createFood(field, snake); //產生第一份食物 
	
	while(true){
		controlSnakeDirection(field, snake); //讀取玩家輸入方向鍵，並將新方向設定到各貪食蛇節點 
		controlSnakeDirection(field, snake2);
		moveNext(snake);
		moveNext(snake2);  //依據節點的方向，繪製新的貪食蛇位置 
		eatFood(field, snake); //判斷是否有吃到食物，如果有新增節點到尾巴 
//		eatSpeed(field, snake); //判斷是否有吃到速度，如果有速度變慢 
//  		eatPoint(field, snake); //判斷是否有吃到分數，如果有分數 +5 
//  		eatLength(field, snake); //判斷是否有吃到，如果有速度變慢
  		eatFood(field, snake2); //判斷是否有吃到食物，如果有新增節點到尾巴 
//		eatSpeed(field, snake2); //判斷是否有吃到速度，如果有速度變慢 
//  		eatPoint(field, snake2); //判斷是否有吃到分數，如果有分數 +5 
//  		eatLength(field, snake2); //判斷是否有吃到，如果有速度變慢
		showInfo(); //顯示時間和分數資訊 
		
		if(IsGameOver(snake, field)) //判斷是否符合遊戲結束條件， 
			return showGameOverMsg(); //顯示遊戲結束訊息，並等待玩家輸入選項 
		if(IsGameOver(snake2, field)) //判斷是否符合遊戲結束條件， 
			return showGameOverMsg(); //顯示遊戲結束訊息，並等待玩家輸入選項 
		
		
		delay(speed); //決定貪食蛇移動速度，speed越小移動越快 
		
		//讀取非方向鍵的其他鍵盤輸入 
	   	if(kbhit()) {
	   		char key;
			key = getch();	
			
			//只有輸入大小寫的a, q 和 s，系統才有反應並將輸入結果拋到外層等待處理 
			if (key == 'q' || key == 'Q' || key == 's' || key == 'S')
				return key; 				
			else if (key == 'a') //決定是否改變模式 ，主要有玩家模式和電腦操控的AI模式 
				IFPlayAI = !IFPlayAI;
			else if (key == 'f')
				combat();
		}					
	}		
}

//繪製遊戲區域，依據遊戲場矩陣設定繪製物件 
void drawGameField(int field[][GRID_SIDE]){
	
   	int row = 0, col = 0;
   	
   	cleardevice(); //清理螢幕畫面 
	
	for(row = 0; row < GRID_SIDE; row ++){
   		for(col = 0; col <  GRID_SIDE; col++){
				switch(field[row][col])	{
					case WALL: //牆在矩陣中的值是1 
						drawSquare(row, col, YELLOW);
						break;
					case FOOD: //食物在矩陣中的值是2
						drawSquare(row, col, GREEN);
						break;	
					case BACKGROUND://背景在矩陣中的值是0
						drawSquare(row, col, LIGHTCYAN);
						break;
					case BACKGROUND2://背景在矩陣中的值是4
						drawSquare(row, col, BLACK);
						break;
				}	
		   }
	}
}

//繪製方塊
void drawSquare(int row, int col, int color){
	
	int squareHeight = SCREEN_HEIGHT / GRID_SIDE;
   	int SquareWidth = SCREEN_WIDTH / GRID_SIDE;
	int left = 0, right = 0, bottom = 0, top = 0;
	left = LEFT_MARGINE + col * SquareWidth + col;
   	top =  TOP_MARGINE + row * squareHeight + row;
	right = left + SquareWidth;
   	bottom = top + squareHeight;
   	 
	setfillstyle(SOLID_FILL,color); //設定繪製物件的為實心和顏色 
   	bar(left , top, right, bottom);	//設定繪製方塊的位置 
}

//繪製貪食蛇每前進一步的身體改變 
void moveNext(NodePointer snake){
	int currRow, currCol;
	int count = 1;
	while (snake != NULL){
		currRow = snake->row;
		currCol = snake->col;
		
		//依據節點的方向屬性，設定移動下一步的位置	
		switch(snake->direct){
			case RIGHT:
				snake->col++;
				break;			
			case LEFT:
				snake->col--;
				break;
			case UP:
				snake->row--;
				break;			
			case DOWN:
				snake->row++;
				break;
		}
		int color= 4;
		if(count%3==0){
			color = 6;
		}
		count++;
		
		drawSquare(snake->row, snake->col, color);
		
		if (!IsAtSnake(snake, currRow, currCol))
			drawSquare(currRow, currCol, LIGHTCYAN);
			
		snake = snake -> next;	
	}
}


//貪食蛇是否死亡(死亡條件：撞牆和撞到自己身體) 
bool IsGameOver(NodePointer snake, int field[][GRID_SIDE]){
	NodePointer head = snake;
	
	//判斷是否撞到牆 
	if (IsAtWall(field, head->row, head->col))
		return true;
	
	//判斷是否撞到貪食蛇自己 
	if(meetSelfWillDie)
		return IsAtSnake(snake->next, head->row, head->col);
	
	return false;		
}

//判斷是否撞到牆
bool IsAtWall(int field[][GRID_SIDE], int row, int col){
		if (field[row][col] == WALL || field[row][col] == BACKGROUND2)
			return true;
		return false;
}

//判斷是否撞到貪食蛇自己的身體
bool IsAtSnake(NodePointer snake, int row, int col){

	while(snake != NULL){
		if (row == snake->row && col == snake->col)
			return true;
		snake = snake -> next;		
	}
	return false;
}

//遊戲結束訊息
int showGameOverMsg(){
   	cleardevice(); //清理所有螢幕資料，如果希望只顯示訊息時，取消註解 
   	PlaySound(NULL, NULL, SND_FILENAME);
	int i = 0;
	char msg1[70] = "Game      Over!!                             \n";
	char msg2[70] = "press [q] to quit or [s] to restart!!        ";
   	for(;;i++){
	   	setcolor(13);
	   	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 6);
	   	outtextxy(0, SCREEN_HEIGHT / 2, msg1);
	   	
	   	setcolor(14);
	   	//畫輪廓 
	   	moveto(220, 250);
    	lineto(280, 260);
    	moveto(220, 250);
    	lineto(230, 290);
    	moveto(340, 250);
    	lineto(280, 260);
    	moveto(340, 250);
    	lineto(330, 290);
    	ellipse(280,290,0,360,50,30); 
    	//畫眼睛 
		moveto(250, 275);
    	lineto(260, 285);
    	moveto(260, 275);
    	lineto(250, 285);
    	
    	moveto(300, 275);
    	lineto(310, 285);
    	moveto(310, 275);
    	lineto(300, 285);
	   	//畫嘴巴 
	   	arc(275, 300, 180,
            360, 5);
        arc(285, 300, 180,
            360, 5);
	   	
	   	setcolor(WHITE);
	   	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 2);
	   	outtextxy(20, SCREEN_HEIGHT / 2 + 70, msg2);
	   	
	   	delay(700);
	   	
	   	
	   	setcolor(BLACK);
	   	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 7);
	   	outtextxy(0, SCREEN_HEIGHT / 2- 20, msg1);	   
		   	
	   	if(kbhit()) {
	   		char key;
			key = getch();	
			if (key == 'q' || key == 'Q' || key == 's' || key == 'S') {
				return key; 				
			}
		}
	}	
}

//顯示遊戲相關資訊
void showInfo(){
	totalTime += speed;
	char timeMsg[45] = " Time:";
	char scoreMsg[45] = "Score:";
	char modeMsg[20] = "";
	char optMsg1[50] = "press [q] to quit, [s] to restart or";
	char optMsg2[50] = "press [a] to change mode.";
	
	char time[10];
	char score[10];
	
	sprintf(time, "%5d", totalTime/1000);
	strcat(timeMsg, time);
	strcat(timeMsg, " sec.");
	
	setcolor(WHITE); //設定文字顏色 
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1); //設定字型，水平或垂直和字型大小 
   	outtextxy(0, 0, timeMsg); //依據坐標輸出文字到螢幕	
	
	sprintf(score, "%5d", scoreSum);
	strcat(scoreMsg, score);
	strcat(scoreMsg, " point.");

	setcolor(WHITE);
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1);
   	outtextxy(0, 19, scoreMsg);	
   	
   	if(IFPlayAI){
   		strcat(modeMsg, "AI Mode    ");  		
	}else{
   		strcat(modeMsg, "Player Mode");  		
	}
			
   	setcolor(LIGHTMAGENTA);
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1);
   	outtextxy(SCREEN_HEIGHT - LEFT_MARGINE * 2, 0, modeMsg);
   	
   	setcolor(LIGHTGREEN);
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1);
   	outtextxy(0, TOP_MARGINE + (GRID_SIDE + 2) * SCREEN_HEIGHT / GRID_SIDE, optMsg1);
	outtextxy(0, TOP_MARGINE + (GRID_SIDE + 2) * SCREEN_HEIGHT / GRID_SIDE + 20, optMsg2);	      	
}

//讀取鍵盤方向輸入，並設定到所有貪食蛇節點 
void controlSnakeDirection(int field[][GRID_SIDE], NodePointer snake){
	
	Direction snakeDirect = snake -> direct;
	
	//get key code by pressing keybord
	int key;
	if(kbhit()) 
		key = getch();
		
	//decide snake's moving direction	
	switch(key){
		case KEY_RIGHT:
			snakeDirect = RIGHT;
			break;			
		case KEY_LEFT:
			snakeDirect = LEFT;
			break;		
		case KEY_UP:
			snakeDirect = UP;
			break;				
		case KEY_DOWN:
			snakeDirect = DOWN;
			break;				
	}
	
	if (IFPlayAI) 
		snakeDirect = AI(field, snake);

	//為了達到像蛇一樣移動方式，當蛇改變方向時，並不是整個身體一起改變，
	//而是延遲性地，各個節點逐一改變，當一個節點收到新的前進方向時，
	//會將自己目前的方向傳給下一個。 
	Direction temp;
	while(snake != NULL){
		temp = snake -> direct;
		snake -> direct = snakeDirect;
		snakeDirect = temp;
		snake = snake -> next;
	}
}
//讀取鍵盤方向輸入，並設定到所有貪食蛇節點 
void controlSnakeDirection2(int field[][GRID_SIDE], NodePointer snake){
	
	Direction snakeDirect = snake -> direct;
	
	//get key code by pressing keybord
	int key;
	if(kbhit()) 
		key = _getch();
		
	//decide snake's moving direction	
	switch(key){
		case KEY_RIGHT:
			snakeDirect = RIGHT;
			break;			
		case KEY_LEFT:
			snakeDirect = LEFT;
			break;		
		case KEY_UP:
			snakeDirect = UP;
			break;				
		case KEY_DOWN:
			snakeDirect = DOWN;
			break;				
	}
	
	Direction temp;
	while(snake != NULL){
		temp = snake -> direct;
		snake -> direct = snakeDirect;
		snakeDirect = temp;
		snake = snake -> next;
	}
}
//產生食物
void createFood(int field[][GRID_SIDE], NodePointer snake){
	int row, col, i, foodAmount = FOOD_AMOUT;
	srand((unsigned)time(NULL)); //取目前系統時間作為亂數種子 

	for(i=0; i< foodAmount; i++){
		//如果亂數產生的位置是在牆和蛇身體重疊，則重新產生，直到符合條件為止 
		do{
			row = rand() % GRID_SIDE;
			col = rand() % GRID_SIDE;	
		}while(IsAtWall(field, row, col) || IsAtSnake(snake, row, col));
	
		field[row][col] = FOOD;
		food_row= row;
		food_col= col;
		drawSquare(row,col, GREEN);			
	}
}

//處理吃到食物邏輯
void eatFood(int field[][GRID_SIDE], NodePointer snake){
	//如果蛇頭與食物位置重疊，就是吃到食物 
	if(field[snake->row][snake->col] == FOOD){
		field[snake->row][snake->col] = BACKGROUND; //將該食物清空 
		//printf("The snake has eaten food at row: %d, col: %d\n", snake->row, snake->col);
		addNode(snake); //增加節點 
		scoreSum = scoreSum + scorePerFood; //紀錄分數 
		createFood(field, snake); //產生新的食物 
	}
}

//增加貪食蛇身體節點
void addNode(NodePointer snake){
	NodePointer tail = NULL;
	NodePointer newNode = (NodePointer) malloc(sizeof(Node)); //初始化一個新節點 
	//尋找蛇的尾巴 
	while(snake != NULL){
		tail = snake;
		snake = snake -> next;
	}
	//將蛇尾巴的方向屬性給新節點 
	newNode -> direct = tail -> direct;
	
	//決定新節點的位置，依據尾巴的移動方向來掛上新節點 
	switch(tail->direct){
		case RIGHT:
			newNode -> row = tail -> row;
			newNode -> col = tail -> col - 1;
			break;			
		case LEFT:
			newNode -> row = tail -> row;
			newNode -> col = tail -> col + 1;			
			break;
		case UP:
			newNode -> row = tail -> row + 1;
			newNode -> col = tail -> col;			
			break;			
		case DOWN:
			newNode -> row = tail -> row - 1;
			newNode -> col = tail -> col;			
			break;
	}
	
	tail -> next = newNode; //將尾巴節點連接到新節點 
	newNode -> next = NULL;		
}

//AI實作 
Direction AI(int field[][GRID_SIDE], NodePointer snake){
 //��芸楛��暸����拐蒂��輸����� 
 NodePointer temp = snake;
 Direction snakeDirect = temp -> direct;
 Direction currentDirect = snakeDirect;

 
 
 if(food_row > temp -> row){
   snakeDirect = DOWN; 
   if((IsAtWall(field, temp -> row+1, temp -> col)) && food_col == temp -> col){
    snakeDirect = Direction(rand()%2);
    change = true; 
    if(currentDirect == RIGHT || currentDirect == LEFT){
     snakeDirect = currentDirect;
      
    }
   
   return snakeDirect;
   }
   else if((!(IsAtWall(field, temp -> row+1, temp -> col)))&& change == true){
    snakeDirect = DOWN;
    change = false;
    goto HERE;
   }
   else if(change == true){
    
    return currentDirect;
   }
  
  }
  else if(food_row < temp -> row){
   snakeDirect = UP;
   if((IsAtWall(field, temp -> row-1, temp -> col)) && food_col == temp -> col){
    snakeDirect = Direction(rand()%2);
    change = true; 
    if(currentDirect == RIGHT || currentDirect == LEFT){
     snakeDirect = currentDirect;
      
    }
   
   return snakeDirect;
   }
   
   else if((!(IsAtWall(field, temp -> row-1, temp -> col))) && change == true){
    snakeDirect = UP;
    change = false;
    goto HERE;
   }
   else if(change == true){
    
    return currentDirect;
   }

  }
  else{;}

  
   
 
  if(food_col > temp -> col){
   snakeDirect = RIGHT;

  }
  else if(food_col < temp -> col){
   snakeDirect = LEFT;

  }
  else{;}
  
 

 
 HERE:
 
 
 switch(snakeDirect){
  case RIGHT:
   if(IsAtWall(field, temp -> row, temp -> col+1)){
    snakeDirect = Direction(rand()%2+2);
    if(currentDirect == UP || currentDirect == DOWN){
     snakeDirect = currentDirect; 
    }
    if(IsAtWall(field, temp -> row-1, temp -> col)) snakeDirect = DOWN;
    else if(IsAtWall(field, temp -> row+1, temp -> col)) snakeDirect = UP;
    
   }
   break;   
   
  case LEFT:
   if(IsAtWall(field, temp -> row, temp -> col-1)){ 
    snakeDirect = Direction(rand()%2+2);
    if(currentDirect == UP || currentDirect == DOWN){
     snakeDirect = currentDirect; 
    }
    if(IsAtWall(field, temp -> row-1, temp -> col)) snakeDirect = DOWN;
    else if(IsAtWall(field, temp -> row+1, temp -> col)) snakeDirect = UP;
    
   }
   break;
    
   
  case UP:
   break;   
   
  case DOWN:
   break; 
 }
 
 
 
 return snakeDirect;
}
//產生減速食物 
void createSpeed(int field[][GRID_SIDE], NodePointer snake){
 	int row, col, i, foodAmount = FOOD_AMOUT;
 	srand((unsigned)time(NULL));

 	for(i = 0; i < foodAmount; i++){
  		do{
   			row = rand() % GRID_SIDE;
   			col = rand() % GRID_SIDE; 
  		}while(IsAtWall(field, row, col) || IsAtSnake(snake, row, col));
 
  		field[row][col] = SPEED;
  		food_row= row;
  		food_col= col;
  		drawSquare(row, col, BLUE);
 	}
}

//處理吃到減速食物的邏輯 
void eatSpeed(int field[][GRID_SIDE], NodePointer snake){ 
 	if(field[snake->row][snake->col] == SPEED){
		field[snake->row][snake->col] = BACKGROUND;
  		//printf("The snake has eaten speed at row: %d, col: %d\n", snake->row, snake->col);
		speed+=10;
  		createSpeed(field, snake);
 	}
}

//產生加分食物 
void createPoint(int field[][GRID_SIDE], NodePointer snake){
 	int row, col, i, foodAmount = FOOD_AMOUT;
 	srand((unsigned)time(NULL));
 	
 	for(i = 0; i < foodAmount; i++){
  		do{
   			row = rand() % GRID_SIDE;
   			col = rand() % GRID_SIDE; 
  		}while(IsAtWall(field, row, col) || IsAtSnake(snake, row, col));
 
  		field[row][col] = POINTT;
  		food_row= row;
		food_col= col;
  		drawSquare(row, col, BLACK);
 	}
}

//處理吃到減分食物的邏輯 
void eatPoint(int field[][GRID_SIDE], NodePointer snake){ 
 	
	if(field[snake->row][snake->col] == POINTT){
  		field[snake->row][snake->col] = BACKGROUND;
  		//printf("The snake has eaten point at row: %d, col: %d\n", snake->row, snake->col);
  		scoreSum+=5;
		createPoint(field, snake);
 	}
}

//產生增加長度的食物 
void createLength(int field[][GRID_SIDE], NodePointer snake){
 	int row, col, i, foodAmount = FOOD_AMOUT;
 	srand((unsigned)time(NULL));

 	for(i=0; i< foodAmount; i++){
 		
  	do{
   		row = rand() % GRID_SIDE;
   		col = rand() % GRID_SIDE; 
  	}while(IsAtWall(field, row, col) || IsAtSnake(snake, row, col));
 
  	field[row][col] = LENGTH;
  	food_row= row;
	food_col= col;
  	drawSquare(row,col, 12); 
 	}
}

//處理吃到長度增加食物的邏輯 
void eatLength(int field[][GRID_SIDE], NodePointer snake){
 
 	if(field[snake->row][snake->col] == LENGTH){
  		field[snake->row][snake->col] = BACKGROUND; 
  		//printf("The snake has eaten food at row: %d, col: %d\n", snake->row, snake->col);
  		for(int i = 0; i < 3; i++)
   			addNode(snake);
  		createFood(field, snake);
  	}
}
