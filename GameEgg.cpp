
//Import các thư viện cần thiết
#include<windows.h>
#include<stdio.h>
#include<GL/glut.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include <sys/timeb.h>
#include <sys/utime.h>
#include <time.h>



// Tỉ lệ màn hình
int screenWidth = 1080; // Chiều rộng
int screenHeight = 950; // Chiều cao

double rxo, gxo, bxo;
int droppedEggs = 0; // Đếm số quả trứng đã rơi xuống, để có thể tính được điểm 
int eggsCaught = 0; // Đếm số quả trứng đã bắt được, đồng nghĩa với điểm   
int missedEggs = 0; // Đếm số quả trứng rơi ngoài   missedEggs = droppedEggs - eggsCaught
int level = 1; // Level người chơi đạt được

double red, g, b;
int eggX, eggY; // Tọa độ để có thể vẽ được quả trứng
int sunX = 1350, sunY = 700; // Tọa độ mặt trời
int flag = 0; // Cờ. Dùng để chia trường hợp có thể xảy ra của game

int random;
// Tốc độ rơi của trứng từng level 
float speedLv1 = -2.5, speedLv2 = -4.5, speedLv3 = -8.5, speedLv4 = -10.5;

// Tọa độ của rổ
int basketX = 470;
int basketY = 50;

int FPS = 40; // Khai báo FPS

// Hàm đếm milis
int getMilliCount() {
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}


// Hàm sleep
void sleep(int sleeptime)
{
	int count = 0;
	int beginsleep = getMilliCount();
	while (getMilliCount() - beginsleep < sleeptime)
	{
		count++;
	}
}

void getScores()
{
	if (eggY <= 127 && (eggX >= basketX && eggX <= basketX + 180)) // Kiểm tra nếu Y của quả trứng đang ở 1 tọa độ sát với mặt đất và tọa độ X năm giữa 2 vị trí X bên trên đầu và cuối của cái xô
	{	
		if (flag != -100 && flag != -200)
		{
			rxo = red;
			gxo = g;
			bxo = b;
			eggsCaught++; // Tăng số trứng bắt được lên
		}
		eggY -= 20; // Giảm tọa độ Y xuống để có thể vẽ lại quả trứng 
	}
	else if (eggY <= 127 && (eggX < basketX || eggX > basketX + 180)) // Kiểm tra nếu trứng nằm bên ngoài tọa độ của X
	{
		missedEggs = droppedEggs - eggsCaught; // Công thức tính số trứng không hứng được, số trứng được tạo - số trứng bắt được
	}
}


// Hàm update
void updateWorld() {
	int beginFrame = getMilliCount();
	glutPostRedisplay(); // thông báo cho chương trình vẽ lại
	int timeDriff = getMilliCount() - beginFrame;
	if (timeDriff < FPS) // 40 khung hình / giây
	{
		sleep(FPS - timeDriff);
	}

	srand(time(0)); // Hàm lấy giá trị ngẫu nhiên trong khoảng thời gian 
	int res = rand() % (10 - (-10) + 1) - 10; // Khởi tạo giá trị ngẫu nhiên trong khoảng [-10; 10]

	if (basketX + res >= 1080) { // Kiểm tra nếu giá trị X của xô nếu cộng thêm res vượt qua rìa bên phải màn hình
		if (res > 0)
		{
			basketX -= res; // Đặt lại giá trị basketX
		}
		else
		{
			basketX += res; // Đặt lại giá trị basketX
		}

	}
	else if (basketX - res < 50) { // Kiểm tra nếu giá trị X của xô nếu trừ thêm res vượt qua rìa bên trái màn hình
		if (res > 0) {
			basketX += res; // Đặt lại giá trị basketX
		}
		else
		{
			basketX -= res; // Đặt lại giá trị basketX
		}

	}
	else // Nếu giá trị X của xô nằm giữa màn hình
		basketX = basketX + res; // Đặt lại giá trị basketX


		


	// Kiểm tra số điểm 
	if (eggsCaught >= 5 && eggsCaught < 10) // Nếu số diểm lơn hơn 5 và bé hơn 10
	{
		level = 2; // Đặt lại level
		eggY += speedLv2; // Đặt lại tốc độ rơi của trứng

	}
	else if (eggsCaught >= 10 && eggsCaught < 15) // Nếu số điểm lớn hơn 10 và bé hơn 15
	{
		level = 3; // Đặt lại level
		eggY += speedLv3; // Đặt lại tốc độ rơi của trứng

	}
	else if (eggsCaught >= 15 && eggsCaught < 20) // Nếu số điểm lớn hơn 15 và bé hơn 20
	{
		level = 4; // Đặt lại level
		eggY += speedLv4; // Đặt lại tốc độ rơi của trứng

	}
	else // Hoặc level = 1
	{
		eggY += speedLv1; // Đặt lại tốc độ rơi của trứng
	}

	getScores(); // Gọi hàm tính điểm
}


// Hàm vẽ màn hình bắt đầu
void startScreen()
{
	int k; // Biến đếm

	// Các chuỗi xuất hiện trong màn hình
	char name[18] = "Catching Egg";
	char start[20] = "Press T To Start";
	char quit[17] = "Press Q To Quit";

	// Hiện chuỗi name
	glColor3f(1, 0, 0); // Màu đỏ
	glRasterPos2i(480, 500); // Chuỗi sẽ xuất hiện tại vị trí (480, 500)
	for (k = 0; k < 18; k++) // Duyệt mảng và vẽ từng chữ cái theo font Times new Romans với cỡ chữ 24
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, name[k]);

	// Hiện chuỗi start
	glColor3f(1, 1, 0);
	glRasterPos2i(450, 460); // Chuỗi sẽ xuất hiện tại vị trí (500, 470)
	for (k = 0; k < 20; k++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, start[k]);

	// Hiện chuỗi quit
	glColor3f(0, 1, 0);
	glRasterPos2i(450, 420);// Chuỗi sẽ xuất hiện tại vị trí (500, 440)
	for (k = 0; k < 17; k++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, quit[k]);

}

// Hàm màn hình chiến thắng
void winScreen()
{

	int m; // Biến đếm

	// Các chuỗi xuất hiện trong màn hình
	char win[18] = "You win";
	char quit[17] = "Press Q To Quit";
	// Hiện chuỗi win
	glColor3f(1, 0, 0);
	glRasterPos2i(470, 500);
	for (m = 0; m < 18; m++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, win[m]);

	// Hiện chuỗi quit
	glColor3f(1, 1, 1);
	glRasterPos2i(430, 470);
	for (m = 0; m < 17; m++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, quit[m]);


}


// Hàm màn hình thất bại
void loseScreen()
{

	int l;// Biến đếm

	// Các chuỗi xuất hiện trong màn hình
	char lose[18] = "You Lose";
	char scores[18] = "Your Score:";
	char quit[17] = "Press Q To Quit";
	char retry[17] = "Press T To Retry";
	// Hiện chuỗi lose
	glColor3f(1, 0, 0);
	glRasterPos2i(490, 500);
	for (l = 0; l < 18; l++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, lose[l]);
	}

	// Hiện chuỗi scores
	glRasterPos2i(470, 470);
	for (l = 0; l < 18; l++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, scores[l]);
	}

	// Hiện điểm bạn đạt được
	// Hàm glutBitmap chỉ có thể hiện được các kế tự từ 0 -> 9
	// Để có thể hiện được các số lớn hơn 10 ý tưởng là vẽ số 1 trước sau đó vẽ số đứng sau
	// Kiếm tra nếu điểm nhỏ hơn 10
	if (eggsCaught < 10) {
		int c = eggsCaught;
		glRasterPos2i(600, 470);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 48 + c); // Để có thể chuyển hệ thập phân sang đồ họa cần cộng 48 (số 0) + số điểm đạt được
	}
	// Kiểm tra nếu điểm lớn hơn 1
	else {
		// Vẽ số 1
		glRasterPos2i(600, 470);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 49); // Thể hiện số 1 
		int d = eggsCaught;
		// Vẽ số hàng đơn vị
		glRasterPos2i(610, 470);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 48 + d - 10); //  Thể hiện số hàng đơn vị cần trừ cho 10 (ví dụ 11 => 11 - 10 = 1)
	}

	// Hiện chuỗi quit
	glColor3f(1.0, 1.0, 1.0);
	glRasterPos2i(450, 430);
	for (l = 0; l < 17; l++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, quit[l]);
	}

	glRasterPos2i(450, 380);
	for (l = 0; l < 17; l++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, retry[l]);
}

// Hàm khởi tạo
void init(void)
{
	
	glViewport(0, 0, screenWidth, screenHeight); // Set Khung nhìn

	glClearColor(0.18, 0.68, 1.0, 0.0); // Màu nền (màu xanh nước biển)

    glMatrixMode(GL_PROJECTION); // Load ma trận Projection
    glLoadIdentity(); // Thay ma trận đơn vị
    gluOrtho2D(0, (GLdouble)screenWidth, 0, (GLdouble)screenHeight); // Xác định ma trận chiếu trực quan
    glMatrixMode(GL_MODELVIEW); // Load ma trận Model View
    glutPostRedisplay(); // Hiện thị lại cửa sổ hiện tại

}

// Vẽ thanh gỗ
void drawWood(GLint x, GLint y, GLint width, GLint height)
{
	// Vẽ thanh gỗ màu nâu để gà đứng lên
	glBegin(GL_POLYGON);
	glColor3f(0.7, 0.5, 0.2);
	glVertex2i(x, y);
	glVertex2i(x + width, y);
	glVertex2i(x + width, y + height);
	glVertex2i(x, y + height);
	glEnd();

}


// Vẽ nền đất
void drawGround(GLint x, GLint y, GLint width, GLint height)
{
	// Nền đất màu xanh
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.2, 0.0);
	glVertex2i(x, y);
	glVertex2i(x + width, y);
	glVertex2i(x + width, y + height);
	glVertex2i(x, y + height);
	glEnd();
}

// Vẽ cái xô
void drawBasket(int i, int j, int random)
{
	// Cái xô màu vàng
	glColor3f(rxo, gxo, bxo);
	
	glBegin(GL_QUADS);
	glVertex2f(0.0 + i, 90.0 + j);
	glVertex2f(50.0 + i, 30.0 + j);
	glVertex2f(170.0 + i, 30.0 + j);
	glVertex2f(220.0 + i, 90.0 + j);
	glEnd();
}

//vẽ con gà 
void drawCK(int ck) { // Cần truyền vào 1 biến ck đẻ có thể tái sử dụng lại hình vẽ bằng cách dịch chuyển hình vẽ sang phải hay sang trái
	glColor3f(0.62745098039, 0.32156862745, 0.17647058823);//màu
	glBegin(GL_POLYGON);//ức  gà chiên nước mắm
	glVertex2f(600 + ck, 630);
	glVertex2f(750 + ck, 630);
	glVertex2f(710 + ck, 580);
	glVertex2f(640 + ck, 580);
	glEnd();

	glBegin(GL_POLYGON);//ức  gà trên chiên nước mắm
	glVertex2f(600 + ck, 650);
	glVertex2f(750 + ck, 650);
	glVertex2f(750 + ck, 630);
	glVertex2f(600 + ck, 630);
	glEnd();
	
	glColor3f(0.80392156862, 0.52156862745, 0.24705882352);
	glBegin(GL_POLYGON);//cổ  gà chiên nước mắm
	glVertex2f(720 + ck, 700);
	glVertex2f(750 + ck, 700);
	glVertex2f(750 + ck, 650);
	glVertex2f(700 + ck, 650);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);//focau gà chiên nước mắm tầng 1
	glVertex2f(600 + ck, 660);
	glVertex2f(630 + ck, 660);
	glVertex2f(630 + ck, 650);
	glVertex2f(600 + ck, 650);
	glEnd();

	glBegin(GL_POLYGON);//focau gà chiên nước mắm tầng 2
	glVertex2f(600 + ck, 670);
	glVertex2f(620 + ck, 670);
	glVertex2f(620 + ck, 660);
	glVertex2f(600 + ck, 660);
	glEnd();

	glBegin(GL_POLYGON);//focau gà chiên nước mắm tầng 3
	glVertex2f(600 + ck, 685);
	glVertex2f(610 + ck, 685);
	glVertex2f(610 + ck, 670);
	glVertex2f(600 + ck, 670);
	glEnd();

	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_POLYGON);//mõm gà chiên nước mắm
	glVertex2f(750 + ck, 740);
	glVertex2f(795 + ck, 725);
	glVertex2f(750 + ck, 710);
	glEnd();


	glColor3f(0.95686274509, 0.6431372549, 0.37647058823);
	glBegin(GL_POLYGON);//đầu  gà chiên nước mắm
	glVertex2f(700 + ck, 720);
	glVertex2f(770 + ck, 720);
	glVertex2f(750 + ck, 700);
	glVertex2f(720 + ck, 700);
	glEnd();

	glBegin(GL_POLYGON);//đầu  gà chiên nước mắm
	glVertex2f(700 + ck, 730);
	glVertex2f(770 + ck, 730);
	glVertex2f(770 + ck, 720);
	glVertex2f(700 + ck, 720);
	glEnd();

	glBegin(GL_POLYGON);//đầu  gà chiên nước mắm
	glVertex2f(720 + ck, 750);
	glVertex2f(750 + ck, 750);
	glVertex2f(770 + ck, 730);
	glVertex2f(700 + ck, 730);
	glEnd();


	glColor3f(0.93333333333, 0.90980392156, 0.66666666666);
	glBegin(GL_POLYGON);//canh gà chiên nước mắm
	glVertex2f(640 + ck, 630);
	glVertex2f(720 + ck, 630);
	glVertex2f(700 + ck, 610);
	glVertex2f(620 + ck, 610);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	glPointSize(10.0);
	glBegin(GL_POINTS);//canh gà chiên nước mắm
	glVertex2f(740 + ck, 740);

	glEnd();
}


// Vẽ mặt trời 
void drawSun()
{

	float a, b, c;

	int p;

	glColor3f(1.0, 1.0, 0.0); // Màu đỏ 

	glBegin(GL_POLYGON);
	for (p = 0; p <= 360; p += 1)
	{
		a = sunX + 60 * (cos(p));
		b = sunY + 60 * (sin(p));
		c = 0;

		glVertex3f(a, b, c);
	}
	glEnd();
}

// Vẽ bụi cỏ 
void drawGrass(GLint g, GLint k)
{
	glBegin(GL_LINES);
	glColor3f(0.0, 0.39215686274, 0.0);
	glVertex2i(g, k);
	glVertex2i(g, k + 55);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0.0, 0.39215686274, 0.0);
	glVertex2i(g, k);
	glVertex2i(g + 45, k + 45);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0.0, 0.39215686274, 0.0);
	glVertex2i(g, k);
	glVertex2i(g - 45, k + 45);
	glEnd();
}


// Vẽ cây
void drawTree(int x, int y)
{
	glColor3f(0.95686274509, 0.6431372549, 0.37647058823);
	glBegin(GL_POLYGON);
	glVertex2f(1300 + x, 80 + y);
	glVertex2f(1325 + x, 80 + y);
	glVertex2f(1325 + x, 390 + y);
	glVertex2f(1300 + x, 390 + y);
	glEnd();

	glColor3f(0.62745098039, 0.32156862745, 0.17647058823);
	glBegin(GL_POLYGON);
	glVertex2f(1325 + x, 80 + y);
	glVertex2f(1350 + x, 80 + y);
	glVertex2f(1350 + x, 390 + y);
	glVertex2f(1325 + x, 390 + y);
	glEnd();


	glColor3f(0.19607843137, 0.80392156862, 0.19607843137);
	glBegin(GL_TRIANGLES);
	glVertex2f(1220 + x, 280 + y);
	glVertex2f(1430 + x, 280 + y);
	glVertex2f(1325 + x, 430 + y);
	glEnd();

	glColor3f(0.19607843137, 0.80392156862, 0.19607843137);
	glBegin(GL_TRIANGLES);
	glVertex2f(1240 + x, 350 + y);
	glVertex2f(1410 + x, 350 + y);
	glVertex2f(1325 + x, 470 + y);
	glEnd();

	glColor3f(0.19607843137, 0.80392156862, 0.19607843137);
	glBegin(GL_TRIANGLES);
	glVertex2f(1260 + x, 420 + y);
	glVertex2f(1390 + x, 420 + y);
	glVertex2f(1325 + x, 520 + y);
	glEnd();
}


// Hàm vẽ lại cái xô
void redrawBasket(int a, int b)
{
	// Gán các giá trị mới vào tọa độ xô
	basketX = a; // Gán vào tọa độ X
	basketY = b; // Gán vào tọa độ Y
	
	glutPostRedisplay();// Vẽ lại cửa sổ
}

// Vẽ quả trứng
void drawEgg()
{

	float x, y, z;
	int t;
	
	
	glColor3f(red, g, b); // Màu trắng

	glBegin(GL_POLYGON);
	for (t = 0; t <= 360; t += 1)
	{
		x = eggX + 16 * (cos(t));
		y = eggY + 25 * (sin(t));
		z = 0;

		glVertex3f(x, y, z);
	}
	glEnd();
}

// Hàm lấy quả trứng ngẫu nhiên
void getRandomEggs() {
	// Quả trứng sẽ rớt ngẫu nhiên tại 4 vị trí
	// Chỉ cần thay đổi tọa độ X để trứng có thể xuất hiện tại cùng 1 Y
	switch (rand() % 3)
	{
		
		case 0:eggX = 190; break;
		case 1:eggX = 590; break;
		case 2:eggX = 960; break;
	}
}



// Hàm để khởi tạo lại vị trí bắt đầu của quả trứng
void eggStart()
{
	
	//if (missedEggs >= 5)
	//{
	//	printf("\n\n\t\t\t\tGAME OVER\n\n");
	//	printScores();
	//}
	red = rand() % 10 * 0.1;
	g = rand() % 10 * 0.1;
	b = rand() % 10 * 0.1;
	eggY = 540; // Vị trí Y của quả trứng, sẽ nằm sát dưới thanh gỗ
	droppedEggs++; // Đếm số quả trứng được tạo
	getRandomEggs(); // Lấy ngẫu nhiên X của quả trứng 
	

}


// Hàm tính điểm
//void getScores() 
//{
//	if (eggY <= 127 && (eggX >= basketX && eggX <= basketX + 180)) // Kiểm tra nếu Y của quả trứng đang ở 1 tọa độ sát với mặt đất và tọa độ X năm giữa 2 vị trí X bên trên đầu và cuối của cái xô
//	{
//		eggsCaught++; // Tăng số trứng bắt được lên
//		eggY -= 20; // Giảm tọa độ Y xuống để có thể vẽ lại quả trứng 
//	}
//	else if (eggY <= 127 && (eggX < basketX || eggX > basketX + 180)) // Kiểm tra nếu trứng nằm bên ngoài tọa độ của X
//	{
//		missedEggs = droppedEggs - eggsCaught; // Công thức tính số trứng không hứng được, số trứng được tạo - số trứng bắt được
//	}
//}



// Hàm vẽ
void render(void)
{

	glClear(GL_COLOR_BUFFER_BIT); // Xóa bộ đệm và cho biết các bộ đệm để hiện màu

	// Khởi tạo các biến để vẽ thanh gỗ và mặt đất
	int w = 3000, h = 30;
	GLint x = 0;
	GLint y = 550;

	drawWood(x, y, w, h); // Vẽ thanh gỗ
	drawGround(x, 0, w, h + 50); // Vẽ mặt đất

	int g = 100, k = 77; // vẽ cỏ 
	drawGrass(g, k);
	drawGrass(g + 100, k);
	drawGrass(g + 500, k);
	drawGrass(g + 900, k);
	drawTree(100, 0);
	drawTree(-100, 0);

	int sunX = 2000, sunY = 1500; // Tọa độ của mặt trời 

	drawSun(); // Vẽ mặt trời

	// Vẽ 3 con gà
	drawCK(-100);
	drawCK(-500);
	drawCK(280);

	int i; // Khởi tạo biến đếm để có thể vẽ ra các chuỗi

	// Khởi tạo các chuỗi sẽ được hiển thị lên màn hình
	char level1[12] = "LEVEL 1";
	char level2[12] = "LEVEL 2";
	char level3[12] = "LEVEL 3";
	char level4[12] = "LEVEL 4";
	char score[12] = "SCORES: ";
	char miss[12] = "MISSED: ";
	
	
	
	if (flag >= 1 && (missedEggs < 5 || eggsCaught < 20 )) // Kiếm tra nếu cờ có đang được bật và các giá trị có đang nằm trong luật của trò chơi hay không
	{

		// Hiện chuỗi score
		glColor3f(1, 1, 1);
		glRasterPos2i(50, 730);
		for (i = 0; i < 12; i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, score[i]);

		// Hiện chuỗi miss
		glRasterPos2i(50, 690);
		for (i = 0; i < 12; i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, miss[i]);

		// Kiểm tra level
		if (level == 1) // Kiểm tra nếu level = 1 
		{
			// Hiện level 1
			glColor3f(1, 1, 1);
			glRasterPos2i(50, 770);
			for (i = 0; i < 12; i++)
			{
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, level1[i]);
				
			}

			// Hiện số điểm 
			glRasterPos2i(150, 730);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 48+eggsCaught);

			// Hiện số trứng rơi ra ngoài
			glRasterPos2i(150, 690);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 48 + missedEggs);

			// Kiếm tra nếu số trứng rơi ra ngoài = 5, kết thúc trò chơi, thay đổi giá trị cờ để có thể hiện lên màn hình thất bại
			if (missedEggs == 5) {
				flag = -200; // Thay đổi giá trị cờ
			}

		}

		else if (level == 2) // Kiểm tra nếu level băng 2
		{
			// Hiện level 2
			glColor3f(1, 0, 0);
			glRasterPos2i(50, 770);
			for (i = 0; i < 12; i++)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, level2[i]);

			// Hiện số điểm
			glColor3f(1, 1, 1);
			glRasterPos2i(150, 730);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 48 + eggsCaught);
			
			// Hiện số trứng rơi ra ngoài
			glRasterPos2i(150, 690);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 48 + missedEggs);

			// Kiếm tra nếu số trứng rơi ra ngoài = 5, kết thúc trò chơi, thay đổi giá trị cờ để có thể hiện lên màn hình thất bại
			if (missedEggs == 5) {
				flag = -200; // Thay đổi giá trị cờ
			}
		}

		else if (level == 3) // Kiểm tra nếu level bằng 3
		{
			//Hiện level 3
			glColor3f(0, 1, 1);
			glRasterPos2i(50, 770);
			for (i = 0; i < 12; i++)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, level3[i]);

			// Do đang lv 3 nên số điểm sẽ lớn hơn 10 nên phải tách thành 2 phần là 1 và phần đơn vị để có thể vẽ lên được màn hình

			// Hiển thị số 1
			glColor3f(1, 1, 1);
			glRasterPos2i(150, 730);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 49); // 49 = 1

			// Hiển thị số điểm phần đơn vị
			glRasterPos2i(160, 730);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 48 + eggsCaught-10);
			
			// Hiển thị số trứng rơi ra bên ngoài
			glRasterPos2i(150, 690);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 48 + missedEggs);

			// Kiếm tra nếu số trứng rơi ra ngoài = 5, kết thúc trò chơi, thay đổi giá trị cờ để có thể hiện lên màn hình thất bại
			if (missedEggs == 5) {
				flag = -200; // Thay đổi giá trị cờ
			}
		}
		else if (level == 4) // Kiểm tra level bằng 4
		{
			// Hiện level 4
			glColor3f(1, 0, 1);
			glRasterPos2i(50, 770);
			for (i = 0; i < 12; i++)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, level4[i]);

			// Hiển thị số 1
			glColor3f(1, 1, 1);
			glRasterPos2i(150, 730);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 49);

			// Hiển thị số điểm phần đơn vị
			glRasterPos2i(160, 730);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 48 + eggsCaught - 10);
			
			// Hiển thị số trứng rơi ra ngoài
			glRasterPos2i(150, 690);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 48 + missedEggs);

			// Kiếm tra nếu số trứng rơi ra ngoài = 5, kết thúc trò chơi, thay đổi giá trị cờ để có thể hiện lên màn hình thất bại
			if (missedEggs == 5) {
				flag = -200; // Thay đổi giá trị cờ
			}

			// Kiểm tra nếu số trứng bắt được = 20, hiện lên màn hình thắng cuộc 
			if (eggsCaught == 20) {
				flag = -100; // Thay đổi giá trị cờ
			}
		}


		// Kiểm tra nếu vị trí Y của trứng, nếu Y của trứng sát với mặt đất  
		if (eggY <= 110)
			eggStart(); // Đặt lại tọa độ của quả trứng

		

		drawEgg(); // Vẽ lại quả trứng

		/*sleep(40);*/ // Hàm ngủ

		//srand(time(0)); // Hàm lấy giá trị ngẫu nhiên trong khoảng thời gian 
		//int res = rand() % (10 - (-10) + 1) - 10; // Khởi tạo giá trị ngẫu nhiên trong khoảng [-10; 10]

		//if (basketX + res >= 1080) { // Kiểm tra nếu giá trị X của xô nếu cộng thêm res vượt qua rìa bên phải màn hình
		//	if (res > 0) 
		//	{
		//		basketX -= res; // Đặt lại giá trị basketX
		//	}
		//	else 
		//	{
		//		basketX += res; // Đặt lại giá trị basketX
		//	}
		//	
		//}
		//else if (basketX - res < 50) { // Kiểm tra nếu giá trị X của xô nếu trừ thêm res vượt qua rìa bên trái màn hình
		//	if (res > 0) {
		//		basketX += res; // Đặt lại giá trị basketX
		//	} 
		//	else
		//	{
		//		basketX -= res; // Đặt lại giá trị basketX
		//	}
		//	
		//}
		//else // Nếu giá trị X của xô nằm giữa màn hình
		//	basketX = basketX + res; // Đặt lại giá trị basketX


		drawBasket(basketX, basketY, random);// Vẽ lại xỗ với giá trị mới
		

		//// Kiểm tra số điểm 
		//if (eggsCaught >= 5 && eggsCaught < 10) // Nếu số diểm lơn hơn 5 và bé hơn 10
		//{
		//	level = 2; // Đặt lại level
		//	eggY += speedLv2; // Đặt lại tốc độ rơi của trứng
		//	
		//}
		//else if (eggsCaught >= 10 && eggsCaught < 15) // Nếu số điểm lớn hơn 10 và bé hơn 15
		//{
		//	level = 3; // Đặt lại level
		//	eggY += speedLv3; // Đặt lại tốc độ rơi của trứng
		//	
		//}
		//else if (eggsCaught >= 15 && eggsCaught < 20) // Nếu số điểm lớn hơn 15 và bé hơn 20
		//{
		//	level = 4; // Đặt lại level
		//	eggY += speedLv4; // Đặt lại tốc độ rơi của trứng
		//	
		//}
		//else // Hoặc level = 1
		//{
		//	eggY += speedLv1; // Đặt lại tốc độ rơi của trứng
		//}

		//getScores(); // Gọi hàm tính điểm

	}
	else if (flag == -100) // Màn hình thắng
	{
		winScreen(); // Hàm vẽ màn hình thắng
		glutPostRedisplay(); // Hiển thị lại cửa sổ
	}
	else if (flag == -200) // Màn hình thua
	{
		loseScreen(); // Hàm vẽ màn hình thua
		glutPostRedisplay(); // Hiển thị lại cửa sổ
	}
	else // Màn hình bắt đầu
	{
		startScreen(); // Hàm vẽ màn hình bắt đầu
	}

	glFlush(); // Thực hiện các hàm gl
	glutSwapBuffers(); // Hoán đổi bộ đệm của cửa sổ hiện tại
}

// Định hình lại cửa sổ 
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);// Set Khung nhìn
	glMatrixMode(GL_PROJECTION);// Load ma trận Projection
	glLoadIdentity();// Thay ma trận đơn vị
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);// Xác định ma trận chiếu trực quan
	//gluLookAt(0, 0, 0, 0, 0, 0 ,1, 1, 0);
	glMatrixMode(GL_MODELVIEW);// Load ma trận Model View

}

// Hàm nhận bàn phím
void processInput(unsigned char key, int x, int y)
{
	// Kiểm tra key
	switch (key)
	{	
		
		case 's': // Nếu bấm s thì giảm tọa độ Y của trứng để trứng rơi nhanh hơn  
			eggY -= 10; // Giảm tọa độ Y của trứng đi 10
			break;
		case 'w':
			eggY += 10;
			break;
		case 'a': // Nếu bấm a thì giảm tọa độ X để dịch chuyển quả trứng sang bên trái
			eggX -= 10; // Giảm tọa độ X của trứng đi 10
			if (eggX <= 0) // Nếu giá trị X của trứng bé hơn 0
			{
				eggX = 20; // Đặt lại giá trị X của trứng
			}
			break;

		case 'd': // Nếu bấm a thì tăng tọa độ X để dịch chuyển quả trứng sang bên phải
			eggX += 10; // Tăng tọa độ X của trứng đi 10
			if (eggX >= 1060) // Nếu giá trị X của trứng lớn hơn 1060
			{
				eggX = 1040; // Đặt lại giá trị X của trứng
			}
			break;

		case 'q': // Nếu bấm q thì thoát ra
			exit(0);
			break;

		case 't': // Nếu bấm t thì thay đổi flag để bắt đầu game
			flag = 2;
			droppedEggs = 0; // Đếm số quả trứng đã rơi xuống, để có thể tính được điểm 
			eggsCaught = 0; // Đếm số quả trứng đã bắt được, đồng nghĩa với điểm   
			missedEggs = 0; // Đếm số quả trứng rơi ngoài   missedEggs = droppedEggs - eggsCaught
			level = 1; // Level người chơi đạt được
			/*eggY += speedLv1;*/
			break;
		//case 'r':
		//	flag = 0;
		//	break;
			
		default:
			break;
	}
}


// Hàm menu, dùng để click phải
void menu(int id)
{
	switch (id)
	{
		
		case 1: 
			
			flag=2;
			startScreen();
			flag = -100;
			droppedEggs = 0; // Đếm số quả trứng đã rơi xuống, để có thể tính được điểm 
			eggsCaught = 0; // Đếm số quả trứng đã bắt được, đồng nghĩa với điểm   
			missedEggs = 0; // Đếm số quả trứng rơi ngoài   missedEggs = droppedEggs - eggsCaught
			level = 1; // Level người chơi đạt được
			glutPostRedisplay();
			break;

		case 2: // Thoát 
			exit(0);
			break;

		default:
			break;
	}

	glutPostRedisplay(); // Hiển thị lại cửa sổ
}

void OnSpecialKeyDown(int key, int xx, int yy)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		eggX -= 10; // Giảm tọa độ X của trứng đi 10
		if (eggX <= 0) // Nếu giá trị X của trứng bé hơn 0
		{
			eggX = 20; // Đặt lại giá trị X của trứng
		}
		//Todo
		break;
	case GLUT_KEY_RIGHT:
		eggX += 10; // Tăng tọa độ X của trứng đi 10
		if (eggX >= 1060) // Nếu giá trị X của trứng lớn hơn 1060
		{
			eggX = 1040; // Đặt lại giá trị X của trứng
		}
		//Todo
		break;
	case GLUT_KEY_UP:

		//Todo
		break;
	case GLUT_KEY_DOWN:
		eggY -= 10;
		//Todo
		break;
	}
}


// Hàm main
int main(int argc, char** argv)
{
	glutInit(&argc, argv); // Khởi tạo thư viện GLUT
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); // Đặt cửa sổ qua chế độ Single và RGBA


	glutInitWindowSize(1500, 800); // Kích thước cửa sổ hiện lên
	glutInitWindowPosition(0, 0); // Vị trí hiện lên của cửa sổ

	glutCreateWindow("Catching Egg"); // Tên cửa sổ
	init();// Hàm khởi tạo

	glutCreateMenu(menu); // Tạo ra menu khi bấm chuột phải
	glutAddMenuEntry("Start game", 1); // Option 1: Start game
	glutAddMenuEntry("Quit", 2); // Option 2: Quit
	glutAttachMenu(GLUT_RIGHT_BUTTON); // Bấm chuột phải
	glutReshapeFunc(reshape); // Đặt lệnh định hình lại cửa sổ cho màn hình hiện tại
	glutDisplayFunc(render); // Đặt lệnh hiển thị lại cho cửa sổ hiện tại
	glutKeyboardFunc(processInput); // Nhận các giá trị từ bàn phím
	//glutPassiveMotionFunc(redrawBasket); // Hàm vẽ lại basket khi di chuyển xô
	glutIdleFunc(updateWorld); // Đặt lệnh gọi lại không hoạt động chung
	glutSpecialFunc(OnSpecialKeyDown);
	glutMainLoop(); // Đưa vào vòng xử lý sự kiến GLUT
	return 0;
}

