//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Ball.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

int x, y;     // положение мячика
int dy, g, dxHole;   // приращение координат
int r;         // радиус мячика
int **arrCor;  //Массив координат

int ctB = 0;
int yHole;
int xHole;

// Переменные для расчёта силы запуска мяча
int yBallS1, yBallS2;
int Score = 0;

bool ifMouse = false;

//Удаление мяча
void delBall(int num) {
	if(num + 1 == ctB) {
		arrCor[num][0] = 0;
		arrCor[num][1] = 0;
		arrCor[num][2] = 0;
		arrCor[num][3] = 0;
        arrCor[num][4] = 0;
	}
	else {
		for(int i = num; i < ctB-1; i++) {
			arrCor[i][0] = arrCor[i+1][0];
			arrCor[i][1] = arrCor[i+1][1];
			arrCor[i][2] = arrCor[i+1][2];
			arrCor[i][3] = arrCor[i+1][3];
			arrCor[i][4] = arrCor[i+1][4];
		}
		arrCor[ctB-1][0] = 0;
		arrCor[ctB-1][1] = 0;
		arrCor[ctB-1][2] = 0;
		arrCor[ctB-1][3] = 0;
		arrCor[ctB-1][4] = 0;
	}
	ctB--;
}

//Стирание мяча
void eraseBall(int num) {
	Form1->Canvas->Brush->Color = clBtnFace;
	Form1->Canvas->Pen->Color = clBtnFace;
	Form1->Canvas->Ellipse(arrCor[num][0] - r/2,
						   arrCor[num][1] - r/2,
						   arrCor[num][0] + r/2,
						   arrCor[num][1] + r/2);
}

//Проверка на столкновение со стеной
void floor(int number) {
	if (arrCor[number][2] > 0) {
		// мяч движется вниз
		if (arrCor[number][1] + arrCor[number][2] + r/2 > Form1->ClientHeight) {
			arrCor[number][2] = -arrCor[number][2];
			if(arrCor[number][1] - arrCor[number][3] > 13) {
				arrCor[number][3] = arrCor[number][1];
			}
			else {
				arrCor[number][4] += 1;
			}
		}
	}
	else {
		// мяч движется вверх
		if (arrCor[number][1] + arrCor[number][2] - r/2 < 60 ) {
			arrCor[number][2] = -arrCor[number][2];
		}
	}
}


//Прорисовка мяча
void drawBall(int num) {
	Form1->Canvas->Brush->Color = clRed;
    Form1->Canvas->Pen->Color = clBlack;
	Form1->Canvas->Ellipse(arrCor[num][0] - r/2,
						   arrCor[num][1] - r/2,
						   arrCor[num][0] + r/2,
						   arrCor[num][1] + r/2);
}

//Соударение двух мячей
void ballCollision(int num1, int num2) {
	int x1 = arrCor[num1][0], y1 = arrCor[num1][1], dy1 = arrCor[num1][2];
	int x2 = arrCor[num2][0], y2 = arrCor[num2][1], dy2 = arrCor[num2][2];

	if(( (y1 + dy1) - (y2 + dy2) <= r/2 ) && (abs(x1 - x2) <= r/2)) {
		if((dy1 > 0 && dy2 > 0) && (y2 > y1 || y1 > y2)) {
			if(y1 > y2) {
				arrCor[num2][2] = 1;
				arrCor[num1][2] = 7;
			}
			else {
				arrCor[num1][2] = 1;
				arrCor[num2][2] = 7;
			}
		}
		else if ((dy1 < 0 && dy2 < 0) && (y2 > y1 || y1 > y2)) {
			if(y1 > y2) {
				arrCor[num1][2] = -1;
				arrCor[num2][2] = -7;
			}
			else {
				arrCor[num2][2] = -1;
				arrCor[num1][2] = -7;
			}
		}
		else {
			arrCor[num2][2] = -arrCor[num2][2];
			arrCor[num1][2] = -arrCor[num1][2];
		}
	}
}
//---------------------------------------------------------------------------

__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
    Form1->BorderStyle = bsSingle;
	r = 25; // радиус мячика

	//Приращение координат
	g = 1;

	//Координаты для лунки
	xHole = r * 1.5;
	yHole = 400;
	dxHole = 1;

	//настройка таймеров
	Timer1->Interval = 10;
	TimerHole->Interval = 1;
	TimerHole->Enabled = true;

	//Если игра только начата, то создать массив
	if(ctB == 0) {
		arrCor = new int*[10];
		for(int i = 0; i < 10; i++) {
			arrCor[i] = new int[5];
		}
		for(int i = 0; i < 10; i++) {
			arrCor[i][0] = 0;
			arrCor[i][1] = 0;
			arrCor[i][2] = 0;
			arrCor[i][3] = 0;
            arrCor[i][4] = 0;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	Refresh();
	for(int i = 0; i < ctB; i++) {
		if(i == 0) {
			for(int j = 0; j < ctB; j++) {
				if(i != j) {
					ballCollision(i, j);
				}
			}
		}
		if(i == 1) {
			for(int j = 1; j < ctB; j++) {
				if(i != j) {
					ballCollision(i, j);
				}
			}
		}
		if(i == 2) {
			for(int j = 2; j < ctB; j++) {
				if(i != j) {
					ballCollision(i, j);
				}
			}
		}
		if(i == 3) {
			for(int j = 3; j < ctB; j++) {
				if(i != j) {
					ballCollision(i, j);
				}
			}
		}
		if(i == 4) {
			for(int j = 4; j < ctB; j++) {
				if(i != j) {
					ballCollision(i, j);
				}
			}
		}
		if(i == 5) {
			for(int j = 5; j < ctB; j++) {
				if(i != j) {
					ballCollision(i, j);
				}
			}
		}
		if(i == 6) {
			for(int j = 6; j < ctB; j++) {
				if(i != j) {
					ballCollision(i, j);
				}
			}
		}
		if(i == 7) {
			for(int j = 7; j < ctB; j++) {
				if(i != j) {
					ballCollision(i, j);
				}
			}
		}
		if(i == 8) {
			for(int j = 8; j < ctB; j++) {
				if(i != j) {
					ballCollision(i, j);
				}
			}
		}
		eraseBall(i);
		floor(i);
		arrCor[i][1] += arrCor[i][2];
		drawBall(i);
        //Проверка на попадание
		if((arrCor[i][1] - arrCor[i][2]*0.5 < yHole && arrCor[i][1] + arrCor[i][2]*0.5 > yHole) &&
		   (arrCor[i][0] <= xHole + r*0.7 && xHole - r*0.7 <= arrCor[i][0])) {
			Score++;        //Добавить очки
			eraseBall(i);	//Стереть изображение мяча
			delBall(i);		//Удалить мяч
		}
	}
}

//----------------------------------------------------------------------------
void __fastcall TForm1::FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{
	if(ctB < 10) {
		ctB += 1;
		yBallS1 = Y;

		//флаг на нажатие мыши
		ifMouse = true;
		Canvas->MoveTo(X, Y);

		//Останавливаем прорисовку
		Timer1->Enabled = false;
		Canvas->Pen->Color = clRed;
		Canvas->Ellipse(X - r/2, Y - r/2, X + r/2, Y + r/2);

		arrCor[ctB-1][0] = X;
		arrCor[ctB-1][1] = Y;
		arrCor[ctB-1][2] = dy;
		arrCor[ctB-1][3] = Y;
        arrCor[ctB-1][4] = 0;
	}
	else {
		ShowMessage("You can play with 10 balls in one time");
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{
    Refresh();
	ifMouse = false;
	int power = 0;
	yBallS2 = Y;
	power = yBallS1 - yBallS2;
	if(power < 40) {
		power = 40;
	}
	else if(power >= 200) {
		power = 200;
	}

	if(ctB == 1) 		arrCor[0][2] = power / 20;
	else if(ctB == 2)	arrCor[1][2] = power / 20;
	else if(ctB == 3)	arrCor[2][2] = power / 20;
	else if(ctB == 4)   arrCor[3][2] = power / 20;
	else if(ctB == 5) 	arrCor[4][2] = power / 20;
	else if(ctB == 6) 	arrCor[5][2] = power / 20;
	else if(ctB == 7)	arrCor[6][2] = power / 20;
	else if(ctB == 8)	arrCor[7][2] = power / 20;
	else if(ctB == 9)	arrCor[8][2] = power / 20;
	else if(ctB == 10)	arrCor[9][2] = power / 20;

	//Возобновляем прорисовку
	Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TimerHoleTimer(TObject *Sender)
{
	LabelScore->Caption = "Score: " + IntToStr(Score);
	//Стереть изображение лунки
	Canvas->Pen->Color = clBtnFace;
	Canvas->Ellipse(xHole - r*0.7, yHole - r/4, xHole + r*0.7, yHole + r/4);

    //Вычисляем новое положение лунки
	if(dxHole > 0) {
		//Лунка движется вправо
		if((xHole + dxHole + r*0.7) > Form1->ClientWidth) {
			dxHole = -dxHole;
			xHole += dxHole;
		}
	}
	else {
		//Лунка движется влево
		if((xHole + dxHole - r*0.7) < 0) {
			dxHole = -dxHole;
			xHole += dxHole;
		}
	}

	xHole += dxHole;
    //Нарисовать лунку в новой точке
	Canvas->Pen->Color = clBlack;
	Canvas->Brush->Color = clBlue;
	Canvas->Ellipse(xHole - r*0.7, yHole - r/4, xHole + r*0.7, yHole + r/4);
	Canvas->Brush->Color = clBtnFace;
}
//---------------------------------------------------------------------------
  /*
	for(int i = 0; i < ctB; i++) {
		if(arrCor[i][2] > 0) {
			arrCor[i][2] += g;
		}
		else if(arrCor[i][2] <= 0) {
			arrCor[i][2] += 2*g;
		}
		if(arrCor[i][4] >= 3) {
			arrCor[i][1] = Form1->ClientHeight - r/2;
			arrCor[i][2] = 0;
		}
	}
*/
