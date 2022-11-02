#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<stdlib.h>
#include<list>

using namespace std;

#define up 		72
#define left 	75
#define right 	77
#define down 	80

// Intento de creación de videojuego //

// gotoxy //
void gotoxy(int x, int y){
	
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	SetConsoleCursorPosition(hCon, dwPos);
	
}

// hide cursor //
void  hide_cursor(){
	
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize 		= 	2;
	cci.bVisible 	= 	FALSE;
	
	SetConsoleCursorInfo(hCon, &cci);
}

void draw_limits(){
	for(int i = 2; i <78; i++){
		gotoxy(i,3);	printf("%c", 205);
		gotoxy(i,33); 	printf("%c", 205);
	}
	
	for(int i = 4; i<33; i++){
		gotoxy(2,i);	printf("%c",186);
		gotoxy(77,i);	printf("%c",186);
	}
	
	gotoxy(2,3);	printf("%c",201);
	gotoxy(2,33);	printf("%c",200);
	gotoxy(77,3);	printf("%c",187);
	gotoxy(77,33);	printf("%c",188);
}

// class //

class mc{
	int x,y;
	int hearts;
	int lives;
	public:
		mc(int _x,int _y, int _hearts, int _lives): x(_x), y(_y), hearts(_hearts), lives(_lives){}
		int X() { return x; }
		int Y() { return y; }
		int LIVES() { return lives; }
		void heart() { hearts--; }
		void drawmc();
		void deletemc();
		void movemc();
		void drawhearts();
		void death();
};

void mc::drawmc(){
	gotoxy(x,y); 	printf("  %c",30);
	gotoxy(x,y+1); 	printf(" %c%c%c",40,207,41);
	gotoxy(x,y+2); 	printf("%c%c %c%c",30,190,190,30);
}

void mc::deletemc(){
	gotoxy(x,y); 	printf("        ");
	gotoxy(x,y+1); 	printf("        ");
	gotoxy(x,y+2); 	printf("        ");
	
}

void mc::movemc(){
	
	if(kbhit()){
		
		char keyword = getch();
		deletemc();
		if(keyword == left  && x	>	3){ x--; }
		if(keyword == right && x+6 	<	77){ x++; }
		
		if(keyword == up	&& y	>	4){ y--; }
		if(keyword == down	&& y+3	<	33){ y++; }
		if(keyword == 'e') { hearts--; }
		if(keyword == 'f') { hearts++; }
		drawmc();
		drawhearts();
	}
	
}

void mc::drawhearts(){
	
	gotoxy(50,2);	printf("lives: %d", lives);
	
	gotoxy(64,2);	printf("health: ");
	gotoxy(70,2);	printf("        ");
	for(int i = 0; i < hearts; i++){
		gotoxy(70+i,2);	printf("%c", 3);
	}
	
}

void mc::death(){
	
	if(hearts == 0){
		deletemc();
		gotoxy(x,y);	printf("  **   ");
		gotoxy(x,y+1);	printf(" ****  ");
		gotoxy(x,y+2);	printf("  **   ");
		Sleep(200);
		
		deletemc();
		gotoxy(x,y);	printf("* ** * ");
		gotoxy(x,y+1);	printf(" ****  ");
		gotoxy(x,y+2);	printf("* ** * ");
		Sleep(200);
		
		deletemc();
		lives--;
		hearts = 3;
		drawhearts();
		drawmc();
	}
	
}


class ast{
	int x,y;
	
	public:
		ast(int _x, int _y): x(_x), y(_y){ }
		int X() { return x; }
		int Y() { return y; }
		void drawast();
		void moveast();
		void hit(class mc &p);
};

void ast::drawast(){
	
	gotoxy(x,y);	printf("%c", 184);
	
}

void ast::moveast(){
	
	gotoxy(x,y);	printf(" ");
	y++;
	if(y > 32){
	
	x = rand()%71 + 4;
	y=4;
	}
	drawast();
	
}

void ast::hit(class mc &p){
	if(x >= p.X() && x < p.X()+6 && y >= p.Y() && y <= p.Y()+2){
		p.heart();
		p.drawmc();
		p.drawhearts();
		x = rand()%71 + 4;
		y=4;
	}
}

// class //
class bullet{
	int x, y;
	public:
		bullet(int _x, int _y): x(_x), y(_y){ }
		int X() { return x; }
		int Y() { return y; }
		void movebullet();
		bool out();
};

void bullet::movebullet(){
	gotoxy(x,y);	printf(" ");
	y--;
	gotoxy(x,y); printf("%c", 12);
	
}

bool bullet::out(){
	if(y == 4) { return true; }
	else { return false; }
}

// main //
int main(){

	hide_cursor();
	draw_limits();
	mc p(37,30,3,3);
	p.drawmc();
	p.drawhearts();
	
	list<ast*> a;
	list<ast*>::iterator ita;
	for(int i = 0; i < 10; i++){
		a.push_back(new ast(rand()%75 + 3, rand()%5 + 4));
	}
	
	list<bullet*> b;
	list<bullet*>::iterator it;
	
	int points = 0;
	
	bool game_over = false;
	while(!game_over){
		
		gotoxy(4,2); printf("Points: %d", points);
		
		if(kbhit()){
			char kb = getch();
			if(kb == 'a'){
				b.push_back(new bullet(p.X() + 2, p.Y() - 1));
			}
		}
		
		for(it = b.begin(); it != b.end(); it++){
			(*it)->movebullet();
			if((*it)->out()){
				gotoxy((*it)->X(), (*it)->Y()); printf(" ");
				delete(*it);
				it = b.erase(it);
			}
		}
		
		
		for(ita = a.begin(); ita != a.end(); ita++){
			(*ita)->moveast();
			(*ita)->hit(p);
		}
		
		for(ita = a.begin();  ita != a.end(); ita++){
			
			for(it = b.begin(); it != b.end(); it++){
				if((*ita)->X() == (*it)->X() && ( (*ita)->Y()+1 == (*it)->Y() || (*ita)->Y() == (*it)->Y())	){
			
					gotoxy((*it)->X(), (*it)->Y());	printf(" ");
					delete(*it);
					it = b.erase(it);
					
					a.push_back(new ast(rand()%74+3, 4));
					gotoxy((*ita)->X(), (*ita)->Y());	printf(" ");
					delete(*ita);
					ita = a.erase(ita);
					
					points++;
				}
			}
		}
		
		if(p.LIVES() == 0){ game_over = true; }
		p.death();
		p.movemc();
		Sleep(30);
	}
	
	system("cls");
	printf("GAME OVER \n");
	system("pause");
	
	return 0;
}
