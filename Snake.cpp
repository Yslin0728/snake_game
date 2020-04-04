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

#define SCREEN_HEIGHT 500 //³]©w¹CÀ¸µøµ¡°ª«× 
#define SCREEN_WIDTH 500 //³]©w¹CÀ¸µøµ¡¼e«×
#define GRID_SIDE 40 //³]©w¹CÀ¸¤è°}¨CÃä®æ¤l¼Æ¶q 
#define LEFT_MARGINE 30 //³]©w¥ªÃä¬É 
#define TOP_MARGINE 40 //³]©w¤WÃä¬É 
#define FOOD_AMOUT 1 //³]©w¨C¦¸²£¥Í­¹ª«¼Æ¶q 
#define INIT_SPEED  80 //³]©wªì©l³g­¹³D²¾°Ê³t«× 


//«Å§i«e¶i¤è¦V¦CÁ|¨ç¼Æ 
enum Direction {
	RIGHT, 
	LEFT, 
	UP, 
	DOWN 
};

//«Å§i¹CÀ¸³õ¥X²{ª«Åé¦CÁ|¨ç¼Æ 
enum Object {
	BACKGROUND, //­I´º 
	WALL, //Àð©Î»ÙÃªª« 
	FOOD, //­¹ª«
	BACKGROUND2,//­I´º¶Â
	SPEED,//¦Y¨ìÅÜºC 
 	POINTT,//¦Y¨ì¥[¤Q¤À 
 	LENGTH//¦Y¨ì¦h¤T¸` 
 
};

//«Å§i³g­¹³D¨­Åé¸`ÂIµ²ºc 
struct Node {
	int row; //¸`ÂI¦ì¦b²Ä´X¦æ 
	int col; //¸`ÂI¦ì¦b²Ä´X¦C 
	Direction direct; //¸Ó¸`ÂIªº«e¶i¤è¦V 
	struct Node *next;	//«ü¦V¤U¤@­Ó¸`ÂI 
}; 

//©w¸q«ü¦V¸`ÂIµ²ºcªº«ü¼ÐÅÜ¼Æ 
typedef struct Node *NodePointer;

void openWindow(); //¶}±Ò´åÀ¸µøµ¡ 
void closeGame(NodePointer snake); //³B²z¹CÀ¸µ²§ôÅÞ¿è 
int playGame(int field[][GRID_SIDE], NodePointer snake); //¹CÀ¸¶i¦æÅÞ¿è 
bool IsGameOver(NodePointer snake, int field[][GRID_SIDE]); //³g­¹³D¬O§_¦º¤`(¦º¤`±ø¥ó¡G¼²Àð©M¼²¨ì¦Û¤v¨­Åé) 
int showGameOverMsg(); //¹CÀ¸µ²§ô°T®§
void showInfo(); //Åã¥Ü¹CÀ¸¬ÛÃö¸ê°T 
void drawGameField(int field[][GRID_SIDE]); //Ã¸»s¹CÀ¸°Ï°ì 
void drawSquare(int row, int col, int color); //Ã¸»s¤è¶ô 
void controlSnakeDirection(int field[][GRID_SIDE], NodePointer snake); //Åª¨úÁä½L¤è¦V¿é¤J¡A¨Ã³]©w¨ì©Ò¦³³g­¹³D¸`ÂI 
void moveNext(NodePointer snake); //Ã¸»s³g­¹³D¨C«e¶i¤@¨Bªº¨­Åé§ïÅÜ  
void createFood(int field[][GRID_SIDE], NodePointer snake); //²£¥Í­¹ª« 
bool IsAtWall(int field[][GRID_SIDE], int row, int col); //§PÂ_¬O§_¼²¨ìÀð  
bool IsAtSnake(NodePointer snake, int row, int col); //§PÂ_¬O§_¼²¨ì³g­¹³D¦Û¤vªº¨­Åé
void eatFood(int field[][GRID_SIDE], NodePointer snake); //³B²z¦Y¨ì­¹ª«ÅÞ¿è 
void addNode(NodePointer snake); //¼W¥[³g­¹³D¨­Åé¸`ÂI 
void playmusic();
void introduction(NodePointer snake);
int original();
Direction AI(int field[][GRID_SIDE], NodePointer snake);//¹ê§@¹q¸£±±¨î³g­¹³DÅÞ¿è 
void load(int x,int y) ;
void loading();
int playcombat(int field[][GRID_SIDE], NodePointer snake,NodePointer snake2);
void controlSnakeDirection2(int field[][GRID_SIDE], NodePointer snake);
void closeGame2(NodePointer snake,NodePointer snake2);

void createSpeed(int field[][GRID_SIDE], NodePointer snake); //²£¥Í´î³t­¹ª« 
void eatSpeed(int field[][GRID_SIDE], NodePointer snake); //³B²z¦Y¨ì´î³t­¹ª«ÅÞ¿è
void createPoint(int field[][GRID_SIDE], NodePointer snake); //²£¥Í¥[¤À­¹ª« 
void eatPoint(int field[][GRID_SIDE], NodePointer snake); //³B²z¦Y¨ì¥[¤À­¹ª«ÅÞ¿è
void createLength(int field[][GRID_SIDE], NodePointer snake); //²£¥Í¼Wªø­¹ª« 
void eatLength(int field[][GRID_SIDE], NodePointer snake); //³B²z¦Y¨ì¥[ªø«×­¹ª«ÅÞ¿è
//void * pthread_func2(void * data_ptr);
//void * pthread_func1(void * data_ptr);
void combat();

int speed; //³g­¹³D²¾°Ê³t«× 
int scoreSum = 0; //¬ö¿ý¤À¼Æ 
int totalTime = 0; //¬ö¿ý¹CÀ¸®É¶¡ 
int const scorePerFood = 1; //¨C¤@¥÷­¹ª«¥i±o¤À¼Æ 
bool meetSelfWillDie = false; //¨M©w³g­¹³D¼²¨ì¦Û¤v¨­Åé¬O§_·|¦º±¼ 
bool IFPlayAI = false; //¬O§_¶}±ÒAI¼Ò¦¡ 
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
	
	Node head = {10, 11, RIGHT, NULL}; //³]©w³DÀYªì©l¦ì¸m©M¤è¦V 
	NodePointer snake = &head;
	Node head2 = {18, 11, RIGHT, NULL}; //³]©w³DÀYªì©l¦ì¸m©M¤è¦V 
	NodePointer snake2 = &head2;
	
	introduction(snake);
}

void introduction(NodePointer snake){
 	cleardevice();
 	playmusic();
 	char msg1[70] = "Welcome to  Nyan cat world !!!           ";
 	char msg2[70] = "¦¹¹CÀ¸¦³¤TºØ¼Ò¦¡:	Nyan, AI , Harder    ";
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
		//µe½ü¹ø
     	moveto(220, 150);
     	lineto(280, 160);
     	moveto(220, 150);
     	lineto(230, 190);
     	moveto(340, 150);
     	lineto(280, 160);
     	moveto(340, 150);
     	lineto(330, 190);
     	ellipse(280,190,0,360,50,30); 
		//µe²´·ú 
  		moveto(250, 175);
     	lineto(260, 185);
     	moveto(260, 175);
     	lineto(250, 185);
     
     	moveto(300, 175);
     	lineto(310, 185);
     	moveto(310, 175);
     	lineto(300, 185);
		//µe¼L¤Ú
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
		Node head = {10, 11, RIGHT, NULL}; //³]©w³DÀYªì©l¦ì¸m©M¤è¦V 
		NodePointer snake = &head;
		Node head2 = {18, 11, RIGHT, NULL}; //³]©w³DÀYªì©l¦ì¸m©M¤è¦V 
	    NodePointer snake2 = &head2;
		speed = INIT_SPEED;
		//³]©w¹CÀ¸³õ©M»ÙÃªª«
		char key;
		key = playcombat(field,snake,snake2); //¶i¦æ¹CÀ¸
		if (key == 'q' || key == 'Q')
			closeGame2(snake,snake2); //¦pªGª±®a¿é¤J'q'Â÷¶}¹CÀ¸	
		else if (key == 's' || key == 'S'){ 
			speed = INIT_SPEED;
   			scoreSum = 0;
   			totalTime = 0;
			introduction(snake); //¦pªGª±®a¿é¤J's' Ä~Äò¹CÀ¸
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
//		Node head = {10, 11, RIGHT, NULL}; //³]©w³DÀYªì©l¦ì¸m©M¤è¦V 
//		NodePointer snake = &head;
//		speed = INIT_SPEED;
//		//³]©w¹CÀ¸³õ©M»ÙÃªª«
//		char key;
//		key = playGame(field, snake); //¶i¦æ¹CÀ¸
//		if (key == 'q' || key == 'Q')
//			closeGame(snake); //¦pªGª±®a¿é¤J'q'Â÷¶}¹CÀ¸	
//		else if (key == 's' || key == 'S'){ 
//			speed = INIT_SPEED;
//   			scoreSum = 0;
//   			totalTime = 0;
//			introduction(snake); //¦pªGª±®a¿é¤J's' Ä~Äò¹CÀ¸
//		}
//	}
//}
//void * pthread_func2(void * data_ptr) {	
//	while(TRUE){
//		Node head = {20, 11, LEFT, NULL}; //³]©w³DÀYªì©l¦ì¸m©M¤è¦V 
//		NodePointer snake2 = &head;
//		speed = INIT_SPEED;
//		//³]©w¹CÀ¸³õ©M»ÙÃªª«
//		playcombat(field, snake2); //¶i¦æ¹CÀ¸	    
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
		Node head = {10, 11, RIGHT, NULL}; //³]©w³DÀYªì©l¦ì¸m©M¤è¦V 
		NodePointer snake = &head;
		speed = INIT_SPEED;
		//³]©w¹CÀ¸³õ©M»ÙÃªª«
		char key;
		key = playGame(field, snake); //¶i¦æ¹CÀ¸
		if (key == 'q' || key == 'Q')
			closeGame(snake); //¦pªGª±®a¿é¤J'q'Â÷¶}¹CÀ¸	
		else if (key == 's' || key == 'S'){ 
			speed = INIT_SPEED;
   			scoreSum = 0;
   			totalTime = 0;
			introduction(snake); //¦pªGª±®a¿é¤J's' Ä~Äò¹CÀ¸
		}		    
	}
}

void playmusic(){
	PlaySound(TEXT("Nyan.wav"), NULL, SND_ASYNC|SND_LOOP);
}

//¶}±Ò´åÀ¸µøµ¡
void openWindow(){
	initwindow(SCREEN_WIDTH + LEFT_MARGINE * 3, SCREEN_HEIGHT + TOP_MARGINE * 3, "Hungry Snake Game");
}

//³B²z¹CÀ¸µ²§ôÅÞ¿è 
void closeGame(NodePointer snake){
	free(snake);
	exit(0);
}
void closeGame2(NodePointer snake,NodePointer snake2){
	free(snake);
	free(snake2);
	exit(0);
}

//¹CÀ¸¶i¦æÅÞ¿è
int playGame(int field[][GRID_SIDE], NodePointer snake){
	
	int i = 1;
	drawGameField(field); //Ã¸»s¹CÀ¸°Ï°ì 
	createFood(field, snake); //²£¥Í²Ä¤@¥÷­¹ª« 
	
	while(true){
		controlSnakeDirection(field, snake); //Åª¨úª±®a¿é¤J¤è¦VÁä¡A¨Ã±N·s¤è¦V³]©w¨ì¦U³g­¹³D¸`ÂI 
		moveNext(snake); //¨Ì¾Ú¸`ÂIªº¤è¦V¡AÃ¸»s·sªº³g­¹³D¦ì¸m 
		eatFood(field, snake); //§PÂ_¬O§_¦³¦Y¨ì­¹ª«¡A¦pªG¦³·s¼W¸`ÂI¨ì§À¤Ú 
		eatSpeed(field, snake); //§PÂ_¬O§_¦³¦Y¨ì³t«×¡A¦pªG¦³³t«×ÅÜºC 
  		eatPoint(field, snake); //§PÂ_¬O§_¦³¦Y¨ì¤À¼Æ¡A¦pªG¦³¤À¼Æ +5 
  		eatLength(field, snake); //§PÂ_¬O§_¦³¦Y¨ì¡A¦pªG¦³³t«×ÅÜºC
		showInfo(); //Åã¥Ü®É¶¡©M¤À¼Æ¸ê°T 
		
		if(IsGameOver(snake, field)) //§PÂ_¬O§_²Å¦X¹CÀ¸µ²§ô±ø¥ó¡A 
			return showGameOverMsg(); //Åã¥Ü¹CÀ¸µ²§ô°T®§¡A¨Ãµ¥«Ýª±®a¿é¤J¿ï¶µ 
		
		//°£¤F¦Y¨ì­¹ª«·|²£¥Í·s­¹ª«¡A¨t²Î¤]ÀH¾÷²£¥Í·s­¹ª« 
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
		
		delay(speed); //¨M©w³g­¹³D²¾°Ê³t«×¡Aspeed¶V¤p²¾°Ê¶V§Ö 
		
		//Åª¨ú«D¤è¦VÁäªº¨ä¥LÁä½L¿é¤J 
	   	if(kbhit()) {
	   		char key;
			key = getch();	
			
			//¥u¦³¿é¤J¤j¤p¼gªºa, q ©M s¡A¨t²Î¤~¦³¤ÏÀ³¨Ã±N¿é¤Jµ²ªG©ß¨ì¥~¼hµ¥«Ý³B²z 
			if (key == 'q' || key == 'Q' || key == 's' || key == 'S')
				return key; 				
			else if (key == 'a') //¨M©w¬O§_§ïÅÜ¼Ò¦¡ ¡A¥D­n¦³ª±®a¼Ò¦¡©M¹q¸£¾Þ±±ªºAI¼Ò¦¡ 
				IFPlayAI = !IFPlayAI;
			//else if (key == 'f')
				//combat();
		}					
	}		
}
int playcombat(int field[][GRID_SIDE], NodePointer snake,NodePointer snake2){
	int i = 1;
	drawGameField(field); //Ã¸»s¹CÀ¸°Ï°ì 
	createFood(field, snake); //²£¥Í²Ä¤@¥÷­¹ª« 
	
	while(true){
		controlSnakeDirection(field, snake); //Åª¨úª±®a¿é¤J¤è¦VÁä¡A¨Ã±N·s¤è¦V³]©w¨ì¦U³g­¹³D¸`ÂI 
		controlSnakeDirection(field, snake2);
		moveNext(snake);
		moveNext(snake2);  //¨Ì¾Ú¸`ÂIªº¤è¦V¡AÃ¸»s·sªº³g­¹³D¦ì¸m 
		eatFood(field, snake); //§PÂ_¬O§_¦³¦Y¨ì­¹ª«¡A¦pªG¦³·s¼W¸`ÂI¨ì§À¤Ú 
//		eatSpeed(field, snake); //§PÂ_¬O§_¦³¦Y¨ì³t«×¡A¦pªG¦³³t«×ÅÜºC 
//  		eatPoint(field, snake); //§PÂ_¬O§_¦³¦Y¨ì¤À¼Æ¡A¦pªG¦³¤À¼Æ +5 
//  		eatLength(field, snake); //§PÂ_¬O§_¦³¦Y¨ì¡A¦pªG¦³³t«×ÅÜºC
  		eatFood(field, snake2); //§PÂ_¬O§_¦³¦Y¨ì­¹ª«¡A¦pªG¦³·s¼W¸`ÂI¨ì§À¤Ú 
//		eatSpeed(field, snake2); //§PÂ_¬O§_¦³¦Y¨ì³t«×¡A¦pªG¦³³t«×ÅÜºC 
//  		eatPoint(field, snake2); //§PÂ_¬O§_¦³¦Y¨ì¤À¼Æ¡A¦pªG¦³¤À¼Æ +5 
//  		eatLength(field, snake2); //§PÂ_¬O§_¦³¦Y¨ì¡A¦pªG¦³³t«×ÅÜºC
		showInfo(); //Åã¥Ü®É¶¡©M¤À¼Æ¸ê°T 
		
		if(IsGameOver(snake, field)) //§PÂ_¬O§_²Å¦X¹CÀ¸µ²§ô±ø¥ó¡A 
			return showGameOverMsg(); //Åã¥Ü¹CÀ¸µ²§ô°T®§¡A¨Ãµ¥«Ýª±®a¿é¤J¿ï¶µ 
		if(IsGameOver(snake2, field)) //§PÂ_¬O§_²Å¦X¹CÀ¸µ²§ô±ø¥ó¡A 
			return showGameOverMsg(); //Åã¥Ü¹CÀ¸µ²§ô°T®§¡A¨Ãµ¥«Ýª±®a¿é¤J¿ï¶µ 
		
		
		delay(speed); //¨M©w³g­¹³D²¾°Ê³t«×¡Aspeed¶V¤p²¾°Ê¶V§Ö 
		
		//Åª¨ú«D¤è¦VÁäªº¨ä¥LÁä½L¿é¤J 
	   	if(kbhit()) {
	   		char key;
			key = getch();	
			
			//¥u¦³¿é¤J¤j¤p¼gªºa, q ©M s¡A¨t²Î¤~¦³¤ÏÀ³¨Ã±N¿é¤Jµ²ªG©ß¨ì¥~¼hµ¥«Ý³B²z 
			if (key == 'q' || key == 'Q' || key == 's' || key == 'S')
				return key; 				
			else if (key == 'a') //¨M©w¬O§_§ïÅÜ¼Ò¦¡ ¡A¥D­n¦³ª±®a¼Ò¦¡©M¹q¸£¾Þ±±ªºAI¼Ò¦¡ 
				IFPlayAI = !IFPlayAI;
			else if (key == 'f')
				combat();
		}					
	}		
}

//Ã¸»s¹CÀ¸°Ï°ì¡A¨Ì¾Ú¹CÀ¸³õ¯x°}³]©wÃ¸»sª«¥ó 
void drawGameField(int field[][GRID_SIDE]){
	
   	int row = 0, col = 0;
   	
   	cleardevice(); //²M²z¿Ã¹õµe­± 
	
	for(row = 0; row < GRID_SIDE; row ++){
   		for(col = 0; col <  GRID_SIDE; col++){
				switch(field[row][col])	{
					case WALL: //Àð¦b¯x°}¤¤ªº­È¬O1 
						drawSquare(row, col, YELLOW);
						break;
					case FOOD: //­¹ª«¦b¯x°}¤¤ªº­È¬O2
						drawSquare(row, col, GREEN);
						break;	
					case BACKGROUND://­I´º¦b¯x°}¤¤ªº­È¬O0
						drawSquare(row, col, LIGHTCYAN);
						break;
					case BACKGROUND2://­I´º¦b¯x°}¤¤ªº­È¬O4
						drawSquare(row, col, BLACK);
						break;
				}	
		   }
	}
}

//Ã¸»s¤è¶ô
void drawSquare(int row, int col, int color){
	
	int squareHeight = SCREEN_HEIGHT / GRID_SIDE;
   	int SquareWidth = SCREEN_WIDTH / GRID_SIDE;
	int left = 0, right = 0, bottom = 0, top = 0;
	left = LEFT_MARGINE + col * SquareWidth + col;
   	top =  TOP_MARGINE + row * squareHeight + row;
	right = left + SquareWidth;
   	bottom = top + squareHeight;
   	 
	setfillstyle(SOLID_FILL,color); //³]©wÃ¸»sª«¥óªº¬°¹ê¤ß©MÃC¦â 
   	bar(left , top, right, bottom);	//³]©wÃ¸»s¤è¶ôªº¦ì¸m 
}

//Ã¸»s³g­¹³D¨C«e¶i¤@¨Bªº¨­Åé§ïÅÜ 
void moveNext(NodePointer snake){
	int currRow, currCol;
	int count = 1;
	while (snake != NULL){
		currRow = snake->row;
		currCol = snake->col;
		
		//¨Ì¾Ú¸`ÂIªº¤è¦VÄÝ©Ê¡A³]©w²¾°Ê¤U¤@¨Bªº¦ì¸m	
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


//³g­¹³D¬O§_¦º¤`(¦º¤`±ø¥ó¡G¼²Àð©M¼²¨ì¦Û¤v¨­Åé) 
bool IsGameOver(NodePointer snake, int field[][GRID_SIDE]){
	NodePointer head = snake;
	
	//§PÂ_¬O§_¼²¨ìÀð 
	if (IsAtWall(field, head->row, head->col))
		return true;
	
	//§PÂ_¬O§_¼²¨ì³g­¹³D¦Û¤v 
	if(meetSelfWillDie)
		return IsAtSnake(snake->next, head->row, head->col);
	
	return false;		
}

//§PÂ_¬O§_¼²¨ìÀð
bool IsAtWall(int field[][GRID_SIDE], int row, int col){
		if (field[row][col] == WALL || field[row][col] == BACKGROUND2)
			return true;
		return false;
}

//§PÂ_¬O§_¼²¨ì³g­¹³D¦Û¤vªº¨­Åé
bool IsAtSnake(NodePointer snake, int row, int col){

	while(snake != NULL){
		if (row == snake->row && col == snake->col)
			return true;
		snake = snake -> next;		
	}
	return false;
}

//¹CÀ¸µ²§ô°T®§
int showGameOverMsg(){
   	cleardevice(); //²M²z©Ò¦³¿Ã¹õ¸ê®Æ¡A¦pªG§Æ±æ¥uÅã¥Ü°T®§®É¡A¨ú®øµù¸Ñ 
   	PlaySound(NULL, NULL, SND_FILENAME);
	int i = 0;
	char msg1[70] = "Game      Over!!                             \n";
	char msg2[70] = "press [q] to quit or [s] to restart!!        ";
   	for(;;i++){
	   	setcolor(13);
	   	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 6);
	   	outtextxy(0, SCREEN_HEIGHT / 2, msg1);
	   	
	   	setcolor(14);
	   	//µe½ü¹ø 
	   	moveto(220, 250);
    	lineto(280, 260);
    	moveto(220, 250);
    	lineto(230, 290);
    	moveto(340, 250);
    	lineto(280, 260);
    	moveto(340, 250);
    	lineto(330, 290);
    	ellipse(280,290,0,360,50,30); 
    	//µe²´·ú 
		moveto(250, 275);
    	lineto(260, 285);
    	moveto(260, 275);
    	lineto(250, 285);
    	
    	moveto(300, 275);
    	lineto(310, 285);
    	moveto(310, 275);
    	lineto(300, 285);
	   	//µe¼L¤Ú 
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

//Åã¥Ü¹CÀ¸¬ÛÃö¸ê°T
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
	
	setcolor(WHITE); //³]©w¤å¦rÃC¦â 
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1); //³]©w¦r«¬¡A¤ô¥­©Î««ª½©M¦r«¬¤j¤p 
   	outtextxy(0, 0, timeMsg); //¨Ì¾Ú§¤¼Ð¿é¥X¤å¦r¨ì¿Ã¹õ	
	
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

//Åª¨úÁä½L¤è¦V¿é¤J¡A¨Ã³]©w¨ì©Ò¦³³g­¹³D¸`ÂI 
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

	//¬°¤F¹F¨ì¹³³D¤@¼Ë²¾°Ê¤è¦¡¡A·í³D§ïÅÜ¤è¦V®É¡A¨Ã¤£¬O¾ã­Ó¨­Åé¤@°_§ïÅÜ¡A
	//¦Ó¬O©µ¿ð©Ê¦a¡A¦U­Ó¸`ÂI³v¤@§ïÅÜ¡A·í¤@­Ó¸`ÂI¦¬¨ì·sªº«e¶i¤è¦V®É¡A
	//·|±N¦Û¤v¥Ø«eªº¤è¦V¶Çµ¹¤U¤@­Ó¡C 
	Direction temp;
	while(snake != NULL){
		temp = snake -> direct;
		snake -> direct = snakeDirect;
		snakeDirect = temp;
		snake = snake -> next;
	}
}
//Åª¨úÁä½L¤è¦V¿é¤J¡A¨Ã³]©w¨ì©Ò¦³³g­¹³D¸`ÂI 
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
//²£¥Í­¹ª«
void createFood(int field[][GRID_SIDE], NodePointer snake){
	int row, col, i, foodAmount = FOOD_AMOUT;
	srand((unsigned)time(NULL)); //¨ú¥Ø«e¨t²Î®É¶¡§@¬°¶Ã¼ÆºØ¤l 

	for(i=0; i< foodAmount; i++){
		//¦pªG¶Ã¼Æ²£¥Íªº¦ì¸m¬O¦bÀð©M³D¨­Åé­«Å|¡A«h­«·s²£¥Í¡Aª½¨ì²Å¦X±ø¥ó¬°¤î 
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

//³B²z¦Y¨ì­¹ª«ÅÞ¿è
void eatFood(int field[][GRID_SIDE], NodePointer snake){
	//¦pªG³DÀY»P­¹ª«¦ì¸m­«Å|¡A´N¬O¦Y¨ì­¹ª« 
	if(field[snake->row][snake->col] == FOOD){
		field[snake->row][snake->col] = BACKGROUND; //±N¸Ó­¹ª«²MªÅ 
		//printf("The snake has eaten food at row: %d, col: %d\n", snake->row, snake->col);
		addNode(snake); //¼W¥[¸`ÂI 
		scoreSum = scoreSum + scorePerFood; //¬ö¿ý¤À¼Æ 
		createFood(field, snake); //²£¥Í·sªº­¹ª« 
	}
}

//¼W¥[³g­¹³D¨­Åé¸`ÂI
void addNode(NodePointer snake){
	NodePointer tail = NULL;
	NodePointer newNode = (NodePointer) malloc(sizeof(Node)); //ªì©l¤Æ¤@­Ó·s¸`ÂI 
	//´M§ä³Dªº§À¤Ú 
	while(snake != NULL){
		tail = snake;
		snake = snake -> next;
	}
	//±N³D§À¤Úªº¤è¦VÄÝ©Êµ¹·s¸`ÂI 
	newNode -> direct = tail -> direct;
	
	//¨M©w·s¸`ÂIªº¦ì¸m¡A¨Ì¾Ú§À¤Úªº²¾°Ê¤è¦V¨Ó±¾¤W·s¸`ÂI 
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
	
	tail -> next = newNode; //±N§À¤Ú¸`ÂI³s±µ¨ì·s¸`ÂI 
	newNode -> next = NULL;		
}

//AI¹ê§@ 
Direction AI(int field[][GRID_SIDE], NodePointer snake){
 //è‡ªå·±æ‰¾é£Ÿç‰©ä¸¦é¿é–‹ç‰† 
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
//²£¥Í´î³t­¹ª« 
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

//³B²z¦Y¨ì´î³t­¹ª«ªºÅÞ¿è 
void eatSpeed(int field[][GRID_SIDE], NodePointer snake){ 
 	if(field[snake->row][snake->col] == SPEED){
		field[snake->row][snake->col] = BACKGROUND;
  		//printf("The snake has eaten speed at row: %d, col: %d\n", snake->row, snake->col);
		speed+=10;
  		createSpeed(field, snake);
 	}
}

//²£¥Í¥[¤À­¹ª« 
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

//³B²z¦Y¨ì´î¤À­¹ª«ªºÅÞ¿è 
void eatPoint(int field[][GRID_SIDE], NodePointer snake){ 
 	
	if(field[snake->row][snake->col] == POINTT){
  		field[snake->row][snake->col] = BACKGROUND;
  		//printf("The snake has eaten point at row: %d, col: %d\n", snake->row, snake->col);
  		scoreSum+=5;
		createPoint(field, snake);
 	}
}

//²£¥Í¼W¥[ªø«×ªº­¹ª« 
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

//³B²z¦Y¨ìªø«×¼W¥[­¹ª«ªºÅÞ¿è 
void eatLength(int field[][GRID_SIDE], NodePointer snake){
 
 	if(field[snake->row][snake->col] == LENGTH){
  		field[snake->row][snake->col] = BACKGROUND; 
  		//printf("The snake has eaten food at row: %d, col: %d\n", snake->row, snake->col);
  		for(int i = 0; i < 3; i++)
   			addNode(snake);
  		createFood(field, snake);
  	}
}
