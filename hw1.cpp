#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#include <GL\glut.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include "bmp.h"

float ballX = 0.0;  // ballX: 공의 x좌표
float ballY = 0.0;  // ballY: 공의 y좌표
float ballmoveX = 0.1;  // ballmoveX: x값만큼 얼마나 움직이는지를 의미
float ballmoveY = -0.1;  // ballmoveY: y값만큼 얼마나 움직이는지를 의미
float ballrotate = 0.0;  // ballrotate: 공의 회전 값을 의미
double radius = 0.1;  // radius: 원의 반지름을 의미

float BarX = 0.0;  // BarX: 막대 바 왼쪽 상단 꼭지점의 x좌표
float BarRangeA = BarX;  // BarRangeA: 막대 바 왼쪽 모서리를 의미
float BarRangeB = BarX + 0.8;  // BarRangeB: 막대 바 오른쪽 모서리를 의미

GLfloat xrot = 0.9f, yrot = 0.8f, zrot = 1.0f;
unsigned int MyTextureObject[1];
AUX_RGBImageRec* pTextureImage[1];  //텍스쳐 저장 공간을 가리키는 포인터

void checking() {

	if (ballY <= -1.0)  // 공이 바닥으로 떨어지면 게임 종료
		exit(0);

	if ((ballY <= -0.7 && (ballX >= BarRangeA && ballX <= BarRangeB))) {  // 공이 막대 바에 부딫치면 튕기고 회전시킴
		ballmoveY = ballmoveY * -1;

		if(ballmoveX < 0) ballrotate = ballrotate + 90;  // 이 때 왼쪽에서 왔는지 오른쪽에서 왔는지에 따라 회전 방향을 달리함
		else if (ballmoveX > 0) ballrotate = ballrotate - 90;

		if (ballrotate >= 360 || ballrotate <= -360) ballrotate = 0;  // 360도 회전은 회전을 안한 것이나 마찬가지이므로 0도로 지정
	}

	if (ballX <= -0.9) {  // 공이 왼쪽 벽에 부딫치면 튕기고 회전시킴
		ballmoveX = ballmoveX * -1;
		
		if (ballmoveY < 0) ballrotate = ballrotate - 90;
		else if (ballmoveY > 0) ballrotate = ballrotate + 90;

		if (ballrotate >= 360 || ballrotate <= -360) ballrotate = 0;
	}

	if (ballX >= 0.9) {  // 공이 오른쪽 벽에 부딫치면 튕기고 회전시킴
		ballmoveX = ballmoveX * -1;

		if (ballmoveY < 0) ballrotate = ballrotate + 90;
		else if (ballmoveY > 0) ballrotate = ballrotate - 90;

		if (ballrotate >= 360 || ballrotate <= -360) ballrotate = 0;
	}

	if (ballY >= 0.9) {  // 공이 위쪽 벽에 부딫치면 튕기고 회전시킴
		ballmoveY = ballmoveY * -1;
		
		if (ballmoveX < 0) ballrotate = ballrotate - 90;
		else if (ballmoveX > 0) ballrotate = ballrotate + 90;

		if (ballrotate >= 360 || ballrotate <= -360) ballrotate = 0;
	}
}

void MyReshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

void drawCircle()
{
	float x2, y2;
	float angle;

	glBegin(GL_TRIANGLE_FAN);

	for (angle = 1.0f; angle<361.0f; angle += 0.2)
	{
		if (angle >= 2) glColor3f(1.0, 0.0, 0.0);

		x2 = sin(angle)*radius;
		y2 = cos(angle)*radius;
		glVertex3f(x2, y2, 0.0);
	}
	glEnd();
}

void MyDisplay() {
	glViewport(0, 0, 500, 500);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);  // 막대바 생성
	glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(BarX, -0.8, 0.0);  //앞면
	glTexCoord2f(1.0f, 0.0f); glVertex3f(BarX + 0.8, -0.8, 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(BarX + 0.8, -1.0, 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(BarX, -1.0, 0.0);
	glEnd();

	glColor3f(1.0, 1.0, 0.0);  // 공 생성
	glPushMatrix();
	glTranslatef(ballX, ballY, 0.0);
	glRotatef(ballrotate, 0, 0, 1.0);

	drawCircle();  // glutSolidSphere() 함수 대신 사용
	
	glPopMatrix();

	glFlush();

	glutPostRedisplay();
}

void MyKeyboard(unsigned char KeyPressed, int X, int Y) {
	switch (KeyPressed) {
	case 'a':  // 'a' 키를 누르면 막대바가 왼쪽으로 이동
		if(BarRangeA >= -1.0) BarX = BarX - 0.1;  // 단, 막대바가 왼쪽이나 오른쪽 끝에 있는 경우 이동 안함
		break;
	case 'd':  // 'd' 키를 누르면 막대바가 오른쪽으로 이동
		if(BarRangeB <= 1.0) BarX = BarX + 0.1;  
		break;
	case 27:  // 'Esc' 키를 누르면 바로 게임 종료
		exit(0);
		break;
	}

	BarRangeA = BarX;  // 막대 바 두 모서리 값을 항상 지정해줌
	BarRangeB = BarX + 0.8;
}

void MyTimer(int Value) {  // 애니메이션처럼 보이게 하기 위해 타이머를 사용

	checking();

	ballX = ballX + ballmoveX;
	ballY = ballY + ballmoveY;  // 공의 위치를 변경

	glutPostRedisplay();  // 화면을 항상 리셋
	glutTimerFunc(50, MyTimer, 1);
}

AUX_RGBImageRec* LoadBMP(char* szFilename) {
	FILE* pFile = NULL;
	if (!szFilename) {
		return NULL;
	}
	pFile = fopen(szFilename, "r");
	if (pFile) {
		fclose(pFile);
		return auxDIBImageLoad(szFilename);     //파일로부터 메모리로  (!)
	}
	return NULL;
}

int LoadGLTextures(char* szFilePath) {       //파일을 로드하고 텍스쳐로 변환
	int Status = FALSE;
	glClearColor(0.0, 0.0, 0.0, 0.0);
	memset(pTextureImage, 0, sizeof(void*) * 1);    //포인터를 널로

	if (pTextureImage[0] = LoadBMP(szFilePath)) {   //비트맵을 로드하고 오류확인
		Status = TRUE;                              //상태 플랙을 True로
		glGenTextures(1, &MyTextureObject[0]);      //텍스쳐 생성
		glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3,
			pTextureImage[0]->sizeX, pTextureImage[0]->sizeY,
			0, GL_RGB, GL_UNSIGNED_BYTE, pTextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glEnable(GL_TEXTURE_2D);
	}
	if (pTextureImage[0]) {                 //텍스쳐가 존재하면
		if (pTextureImage[0]->data) {       //텍스쳐 영상이 존재하면
			free(pTextureImage[0]->data);   //텍스쳐 영상공간 반납
		}
		free(pTextureImage[0]);             //텍스쳐 반납
	}
	return Status;
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Ball Game !!");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	if (LoadGLTextures("kon.bmp")) {  // 이미지 로드에 성공해야만 게임 창이 뜸
		glutDisplayFunc(MyDisplay);
		glutKeyboardFunc(MyKeyboard);
		glutTimerFunc(50, MyTimer, 1);
		glutMainLoop();
	}
	else {
		printf("cannot find an image file...\n");
	}
}