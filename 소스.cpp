#pragma once
#include "src/ObjParser.h"
#include<GL/GL.h>
#include <gl/glut.h>
#include <gl/freeglut.h>  //�̰� �����ؾ� ���콺 �� ����� �� �ִ�.
#include<gl/glext.h> //skybox��#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <vector>
#include <utility>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <ctime>
#include "src/bmpfuncs.h"

#include<mmsystem.h>
#pragma comment(lib,"winmm.lib");

#define HEIGHT 1000
#define WIDTH 1600
using namespace std;

using std::strlen; //strlen�� ��� ����

void init(void); //�ʱ�ȭ�� ���� �Լ�
void resize(int wid, int hig);
void draw(void); //�� �׸� �׸��� �Լ�

				 //draw lego
void draw_obj(ObjParser* objParser);

void mouse(int button, int state, int x, int y); //������ �Է� �ν��ϴ� �Լ�
void activeMotion(int x, int y);
void keyboard(unsigned char key, int x, int y); //�ܼ� ���� Ű���� �Է¿� ���� �Լ�
void specialKeyboard(int key, int x, int y); //�����¿� Ű���� �Է¿����� �Լ�
void mouseWheel(int wheel, int direction, int x, int y); //���콺 �� �Լ�
void light_default(void);
void draw_skyBox();
void environmentMapSetting(void);

void MyMenu();
void main_menu_f(int option);

void idle();
float get_time(void);
void draw_cylinder();

double OBJspin = 0; //ȸ������ �ʱ� �� 0�� //lab5
float scale = .5f; // 1. ���� ���� scale ���� ����
double theta = 45.0; //�浵
double phi = 45.0; //����
int radius = 18; // ? 
double upVector = cos(phi* M_PI / 180); //�� ����

										/* camera location */
double eyex, eyey, eyez;

/* object location*/
float trans_x, trans_y, trans_z = 0;
float rota_x, rota_y, rota_z = 0;//�� �Ʒ��� ���� ������ ���� ������

int look = 20;
int Mode = 999;
int cameraMode = 0;
int ProjectMode = 0; //making ����� mode�� 0, zoo�� 1, battle�� 2

int color_select = 1, type_select = 0;

int height;

/* sky box */
GLuint g_nCubeTex;

/* time set variable */
clock_t start, moment; //ctime�� ���� ���� ����
float duration = 0.0;
bool Launch = false;
float launch_y = 0; //�̰ɷ� ����

int current_wid, current_hei;
int lego_num = 0;

/* lego */
ObjParser* Lego_1, *Lego_2, *Lego_3, *Lego_4, *Lego_5, *Lego_6, *Lego_7, *Lego_8, *Lego_9, *Lego_10, *Lego_11, *Lego_12;

FILE *out; //save & load

bool load = false;

class LEGO {
private:
	float loc_x, loc_y, loc_z;
	float Rot_x, Rot_y, Rot_z;
	int color;
	int type;
public:
	void LEGO_init()
	{
		loc_x = 0.0;
		loc_y = 0.0;
		loc_z = 0.0;
		Rot_x = 0.0;
		Rot_y = 0.0;
		Rot_z = 0.0;
		color = 0;
		type = 0;
	}

	void LEGO_color_select(int c)
	{
		switch (c) {
		case 1:
			glColor3f(0.7450f, 0.7450f, 0.7450f); //ȸ��
			break;
		case 2:
			glColor3f(0.947059f, 0.644706f, 0.364706f); //Ȳ���
			break;
		case 3:
			glColor3f(0, 0, 0); //������
			break;
		case 4:
			glColor3f(1, 1, 1); //�Ͼ��
			break;
		case 5:
			glColor3f(0.937255f, 0.220784f, 0.229784f);  //������
			break;
		case 6:
			glColor3f(0.603922f, 0.803922f, 0.196078f);  //���λ�
			break;
		case 7:
			glColor3f(0, 1, 0.2f);   //�ʷϻ�
			break;
		case 8:
			glColor3f(1.05f, 6.04706f, 0.424706f); //���
			break;
		case 9:
			glColor3f(1, 0.4f, 0); // ��Ȳ��
			break;
		case 10:
			glColor3f(1, 0.8f, 0.1f); // �����
			break;
		}
	}

	void set_lego_color(int c)
	{
		color = c;
	}
	int get_lego_color()
	{
		return color;
	}

	void LEGO_draw_con_type(int t)
	{
		switch (t) {
		case 1:
			draw_obj(Lego_1);
			break;
		case 2:
			draw_obj(Lego_2);
			break;
		case 3:
			draw_obj(Lego_3);
			break;
		case 4:
			draw_obj(Lego_4);
			break;
		case 5:
			draw_obj(Lego_5);
			break;
		case 6:
			draw_obj(Lego_6);
			break;
		case 7:
			draw_obj(Lego_7);
			break;
		case 8:
			draw_obj(Lego_8);
			break;
		case 9:
			draw_obj(Lego_9);
			break;
		case 10:
			draw_obj(Lego_10);
			break;
		case 11:
			draw_obj(Lego_11);
			break;
		}
	}

	void set_lego_type(int t)
	{
		type = t;
	}
	int get_lego_type()
	{
		return type;
	}
	void set_lego_rotate(int x, int y, int z)
	{
		Rot_x = x;
		Rot_y = y;
		Rot_z = z;
	}
	float get_lego_rotate_x()
	{
		return Rot_x;
	}
	float get_lego_rotate_y()
	{
		return Rot_y;
	}
	float get_lego_rotate_z()
	{
		return Rot_z;
	}
	void get_lego_rotated()
	{
		//rotate on x axis
		glRotatef(Rot_x, 1.0, 0.0, 0.0);

		//rotate on y axis
		glRotatef(Rot_y, 0.0, 1.0, 0.0);

		//rotate on z axis
		glRotatef(Rot_z, 0.0, 0.0, 1.0);
	}
	void set_lego_location(float x, float y, float z)
	{
		loc_x = x;
		loc_y = y;
		loc_z = z;
	}
	float get_lego_location_x()
	{
		return loc_x;
	}
	float get_lego_location_y()
	{
		return loc_y;
	}
	float get_lego_location_z()
	{
		return loc_z;
	}
	void get_lego_translated()
	{
		glTranslatef(loc_x, loc_y, loc_z);
	}

	void draw_lego()
	{
		LEGO_color_select(color);
		get_lego_translated();
		get_lego_rotated();
		LEGO_draw_con_type(type);
	}
};

LEGO lego[1000];
LEGO preview;

void draw_obj(ObjParser *objParser) {
	glBegin(GL_TRIANGLES);
	for (unsigned int n = 0; n < objParser->getFaceSize(); n += 3) { //obj parser���� 3���� face�� �޾ƿ´�
																	 //�켱 n-1����, ��
		glNormal3f(objParser->normal[objParser->normalIdx[n] - 1].x, // normal �ε����� �޾ƿ� 
			objParser->normal[objParser->normalIdx[n] - 1].y,
			objParser->normal[objParser->normalIdx[n] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n] - 1].x, //vertice �ε���
			objParser->vertices[objParser->vertexIdx[n] - 1].y,
			objParser->vertices[objParser->vertexIdx[n] - 1].z);
		//�̹��� n+1 -1 ����
		glNormal3f(objParser->normal[objParser->normalIdx[n + 1] - 1].x,
			objParser->normal[objParser->normalIdx[n + 1] - 1].y,
			objParser->normal[objParser->normalIdx[n + 1] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 1] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].z);
		//�̹��� n+2 -1, ��
		glNormal3f(objParser->normal[objParser->normalIdx[n + 2] - 1].x,
			objParser->normal[objParser->normalIdx[n + 2] - 1].y,
			objParser->normal[objParser->normalIdx[n + 2] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 2] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].z);
	}
	glEnd();
}

int main(int argc, char **argv) {
	//for Window �ʱ�ȭ
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //GLUT_SINGLE -> DOUBLE //lab5 //�ʱ�ȭ ��, display��带 �� GLUT_DOUBLE�� ���־�� �Ѵ�.+ DEPYH_BUFFER
	glutInitWindowSize(WIDTH, HEIGHT); //1600*1000
	glutInitWindowPosition(800, 500); //ó���� â���拚 ��ġ�� �������
	glutCreateWindow("Final Proj");
	init();

	//glutIdleFunc(moving); 
	glutDisplayFunc(&draw);
	glutIdleFunc(idle); //��� 
	glutMouseFunc(&mouse); //mouseFunction ���� �Է°��� mouse�� �޾ƿ��ڳ�
	glutKeyboardFunc(&keyboard);  // �Էµ� �Ϲ�Ű�� ���� ó�� Ȱ��ȭ �����ش�
	glutSpecialFunc(&specialKeyboard);  // Ư��Ű�� ���� ó�� Ȱ��ȭ �����ش�
	glutMouseWheelFunc(&mouseWheel); //������ �����ص� ���콺 �� �Լ��� Ȱ��ȭ �����ش�
	glutMotionFunc(&activeMotion);  // drag tracking

	MyMenu();

	glutMainLoop();

	return 0;
}


void resize(int wid, int hig) {
	glViewport(0, 0, wid, hig);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //������� �ʱ�ȭ
	current_wid = wid;
	current_hei = hig;
	gluPerspective(45, (double)wid / (double)hig, 1, 500); //45�� ����, nearplane =1, farplane=500 , �ΰ��� palne���� frustum ũ�⸦ �����Ѵ�
	glMatrixMode(GL_MODELVIEW);
}

void idle(void)
{
	if (Launch == TRUE && (get_time() > 20000)) {   // ���� �߻� ���̰�, �߻����� 20001~
		launch_y += 0.3f;
	}
	else if (Launch == TRUE && (10000 <= get_time() && get_time() <= 20000)) { // ���� �߻� ���̰�, �߻����� 10000~20000
		launch_y += 0.11f;
		/*for (int i = 0; i <= lego_num; i++) {
		int temp = 0;
		temp = (lego[i].get_lego_location_y()) + 0.11;
		lego[i].set_lego_location(lego->get_lego_location_x(), temp, lego->get_lego_location_z());
		}*/
	}
	glutPostRedisplay();
}

float get_time(void)   // �߻��� �ķκ��� �ð��� ���.
{
	moment = clock();   // ���� �ð� ȹ��
	duration = (float)(moment - start);   // �߻� �ð����� ���� �ð�����

	return duration;   // �߻��� �ķ� �󸶳� ������ ��
}

void init(void) {
	Lego_1 = new ObjParser("obj/body.obj"); //�ش� �޸𸮷� ��ü�� ������ �� �ּ� �����͸� ��ȯ, &�� ��� 
	Lego_2 = new ObjParser("obj/eye.obj");
	Lego_3 = new ObjParser("obj/foot.obj");
	Lego_4 = new ObjParser("obj/head.obj");
	Lego_5 = new ObjParser("obj/leg_b.obj");
	Lego_6 = new ObjParser("obj/leg_f.obj");
	Lego_7 = new ObjParser("obj/mouth.obj");
	Lego_8 = new ObjParser("obj/mouth_bird.obj");
	Lego_9 = new ObjParser("obj/part.obj");
	Lego_10 = new ObjParser("obj/tail_ball.obj");
	Lego_11 = new ObjParser("obj/tail_cir.obj");
	Lego_12 = new ObjParser("obj/tail_long.obj");

	/* Background color :  Black */
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/* Default Object Color Set : Black */
	glColor3f(1.0f, 1.0f, 1.0f);

	/* resize callback */
	glutReshapeFunc(resize);

	/* light setting */
	light_default();

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);   //polygon�� ���� ������ �����ϰ� texture�� ����
	glEnable(GL_TEXTURE_2D);

	/* Environment Mapping */
	environmentMapSetting(); //!!!!!!!!!!!

							 /* Front face */
	glFrontFace(GL_CCW);

	/* Enable depth buffer */
	glEnable(GL_DEPTH_TEST);

	/* default BGM */
	PlaySound(TEXT("audio/making.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT); //ó���� �׳� ����� �뷡
}

void light_default() {
	glClearColor(0, 0, 0, 1.0f);

	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.3f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specularLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	/************* spot position setting *************/
	//GLfloat spot_direction[] = { 0.0, 0.0, -2.0, 1.0 };
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60.0);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	//glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 5);

	GLfloat ambientMaterial[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularMaterial[] = { 0.9f, 0.9f, 0.9f, 1.0f };

	/************* Material  setting *************/
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	/* global light setting */
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void draw_axis(void) { //�� �׸���
	glLineWidth(3); //��(��ǥ����)�� �β�
	glBegin(GL_LINES);

	glColor3f(1, 0, 0); //x�� ����
	glVertex3f(0, 0, 0);
	glVertex3f(4, 0, 0);

	glColor3f(0, 1, 0); //y�� �ʷ�
	glVertex3f(0, 0, 0);
	glVertex3f(0, 4, 0);

	glColor3f(0, 0, 1); //z�� �Ķ�
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 4);

	glEnd();
	glLineWidth(1);
}

void draw(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //buffer clear
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); //�ʱ�ȭ - �����·�

					  //�Ʒ� ���� multiViewport�׷���� ���� â�� �� �տ� �� ���ְ� ��
	glViewport(0, 0, current_wid, current_hei); //����Ʈ Ȯ��

	eyex = radius * sin(theta* M_PI / 180)*cos(phi* M_PI / 180);
	eyey = radius * sin(phi* M_PI / 180);
	eyez = radius * cos(theta* M_PI / 180)*cos(phi* M_PI / 180);
	upVector = cos(phi* M_PI / 180); // simplify upvector correspond to y-axis

	switch (cameraMode) {
	case 0:
		gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, upVector, 0); //default
		break;
	case 1:
		gluLookAt(eyex + trans_x, eyey + trans_y, eyez + trans_z, trans_x, trans_y, trans_z, 0, upVector, 0);
		break;
	}

	draw_skyBox();

	lego[lego_num].set_lego_color(color_select);
	lego[lego_num].set_lego_type(type_select);
	lego[lego_num].set_lego_rotate(rota_x, rota_y, rota_z);
	lego[lego_num].set_lego_location(trans_x, trans_y, trans_z);

	/* g������ launch_y�� idle�� ���� �ٲ��, ��� ���� ���� �����̸� �׷��� */
	glTranslatef(0, launch_y, 0);

	for (int i = 0; i <= lego_num; i++) { //file �ȿ� �ִ°� ������ ������
		glPushMatrix();
		lego[i].draw_lego();
		glPopMatrix();
	}


	PlaySound(TEXT("audio/battle.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

	glFlush();
	glutSwapBuffers();
}

int eye_x = 0;  // ��������. ���콺 �巡�뿡�� ���� ��ǥ �����ϱ� ���� ����
int eye_y = 0;

void activeMotion(int x, int y) {  // ���콺 ��ư�� ������,
	printf("x: %d, y: %d \n", x, y);
	if (x < eye_x) {
		theta += 1.5;
		if (theta >= 360) theta = 0.0;
	}
	else if (x > eye_x) {
		theta -= 1.5;
		if (theta <= -360) theta = 0.0;
	}
	eye_x = x;
	if (y < eye_y) {
		phi -= 1.5;
		if (abs((int)phi) % 360 == 0) phi = 0.0;
	}
	else if (y > eye_y) {
		phi += 1.5;
		if (abs((int)phi) % 360 == 0) phi = 0.0;
	}
	eye_y = y;
}

void mouse(int button, int state, int x, int y) {
	if (state) {//Ű���� ���� ��
	}
	glutPostRedisplay(); // ������� ���ݹ�
}

void keyboard(unsigned char key, int x, int y) {
	if (key == '1') {
		type_select = 1;
	}
	if (key == '2') {
		type_select = 2;
	}
	if (key == '3') {
		type_select = 3;
	}
	if (key == '4') {
		type_select = 4;
	}
	if (key == '5') {
		type_select = 5;
	}
	if (key == '6') {
		type_select = 6;
	}
	if (key == '7') {
		type_select = 7;
	}
	if (key == '8') {
		type_select = 8;
	}
	if (key == '9') {
		type_select = 9;
	}
	if (key == '10') {
		type_select = 10;
	}
	if (key == '11') {
		type_select = 11;
	}
	if (key == '12') {
		type_select = 12;
	}
	if (key == 'h') {
		lego_num += 1;
		printf("lego num = %d\n", lego_num);
		trans_x = 0; trans_y = 0; trans_z = 0;
		rota_x = 0; rota_y = 0; rota_z = 0;
		type_select = 0;
	}
	if (key == 'c') {
		color_select += 1;
		if (color_select == 10) color_select = 0;
	}
	if (key == 'g') {
		Launch = true;
		start = clock();
	}
	if (key == 8) {   // backspace
		if (lego_num >= 1) lego_num -= 1;
	}

	switch (key) {
	case't': //obj ��ġ�����ϴ¸���.
		Mode = 999;
		break;
	case'r': //obj ȸ�������ϴ¸���.
		Mode = 998;
		break;
	case'z': //obj z ��ġ �����ϴ¸���.(��, �� Ű) & z ���� ȸ����� (��, �� Ű) 
		Mode = 997;
		break;
	case 'l':
		if (cameraMode == 0) {
			cameraMode = 1;
		}
		else if (cameraMode == 1) {
			cameraMode = 0;
		}
		break;
	case 'p': //popupâ Ȯ�ο�
		MessageBox(NULL, TEXT("�� ���̿��� ���� �����."), TEXT("�����Դϴ�!"), MB_OK);
		break;
	}
	glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		switch (Mode) {
		case 999:
			trans_y += 1; //-x�� obj�� translate
			break;
		case 998:
			if (rota_x == 0) rota_x = 355;
			else rota_x -= 5;
			break;
		case 997:
			trans_z -= 1; //-z�� obj�� translate // ü�� �־���
			break;
		}
		break;
	case GLUT_KEY_DOWN:
		switch (Mode) {
		case 999:
			trans_y -= 1; //-x�� obj�� translate
			break;
		case 998:
			if (rota_x == 355) rota_x = 0;
			else rota_x += 5;
			break;
		case 997:
			trans_z += 1; //+z�� obj�� translate //����� ��
			break;
		}
		break;

	case GLUT_KEY_LEFT:
		switch (Mode)
		{
		case 999:
			trans_x -= 1; //-x�� obj�� translate
			break;
		case 998:
			if (rota_y == 0) rota_y = 355;
			else rota_y -= 5;
			break;
		case 997:
			if (rota_z == 0) rota_z = 355;
			else rota_z -= 5;
			break;
		}
		break;

	case GLUT_KEY_RIGHT:
		switch (Mode)
		{
		case 999:
			trans_x += 1; //-x�� obj�� translate
			break;
		case 998:
			if (rota_y == 355) rota_y = 0;
			else rota_y += 5;
			break;
		case 997:
			if (rota_z == 355) rota_z = 0;
			else rota_z += 5;
			break;
		}
	}
	glutPostRedisplay();
}

void mouseWheel(int wheel, int dir, int x, int y) {
	if (dir > 0) {
		if (radius > 2) radius--;
	}
	else {
		if (radius < 100) radius++;
	}
	glutPostRedisplay();
	printf("wheel: %d,Direction: %d,\n", wheel, dir);
}

int menu; //menu �� ID

void MyMenu() {
	menu = glutCreateMenu(main_menu_f);
	glutAddMenuEntry("Quit", 1);
	glutAddMenuEntry("Zoo", 2);
	glutAddMenuEntry("Save", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void main_menu_f(int option) { //�޴�
	switch (option) {
	case 1:
		exit(0);  // ���α׷� ����
		break;
	case 2: //Zoo mode
		ProjectMode = 1;
		PlaySound(TEXT("audio/farm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
		glutChangeToMenuEntry(2, "Making", 4); //2��° �޴��� option 4�� making�� �ȴ�
		glutChangeToMenuEntry(3, "Load", 5); //3���� �޴��� option 5�� Load �� �ȴ�
		glutSetMenu(menu);
		break;
	case 3: //save  
		printf("Save model has selected\n");
		fopen_s(&out, "file1.txt", "w");
		for (int i = 0; i <= lego_num; i++) {
			fprintf(out, "%d, %d, %d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f \n",
				i, lego[i].get_lego_type(), lego[i].get_lego_color(),
				lego[i].get_lego_location_x(), lego[i].get_lego_location_y(), lego[i].get_lego_location_z(),
				lego[i].get_lego_rotate_x(), lego[i].get_lego_rotate_y(), lego[i].get_lego_rotate_z());
		}
		fclose(out);
		break; // save model
	case 4: //making  mode
		ProjectMode = 0;
		PlaySound(TEXT("audio/making.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
		glutChangeToMenuEntry(2, "zoo", 2);
		glutChangeToMenuEntry(3, "Save", 3);
		glutSetMenu(menu);
		break;
	case 5: //load  
		load = true;
		printf("Load model has selected\n");
		fopen_s(&out, "file1.txt", "r");
		while (!feof(out)) {
			fscanf_s(out, "%d, %d, %d, %f, %f, %f, %f, %f, %f \n",
				&lego_num, &type_select, &color_select,
				&trans_x, &trans_y, &trans_z, &rota_x, &rota_y, &rota_z);
			lego[lego_num].set_lego_color(color_select);
			lego[lego_num].set_lego_type(type_select);
			lego[lego_num].set_lego_rotate(rota_x, rota_y, rota_z);
			lego[lego_num].set_lego_location(trans_x, trans_y, trans_z);
		}
		fclose(out);
		break; // load model

	}
}

void draw_cylinder() {
	glViewport(current_wid * 2 / 3, current_hei * 2 / 3, current_wid / 3, current_hei / 3);

	draw_axis();

	glFlush();
}

void draw_skyBox() {
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	float g_nSkySize = 50.0f;
	glBegin(GL_QUADS);
	//px
	glTexCoord3d(1.0, -1.0, -1.0); glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(1.0, -1.0, 1.0); glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(1.0, 1.0, 1.0); glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(1.0, 1.0, -1.0); glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);

	//nx
	glTexCoord3d(-1.0, -1.0, -1.0); glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(-1.0, -1.0, 1.0); glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, 1.0); glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, -1.0); glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);

	//py
	glTexCoord3d(1.0, 1.0, 1.0); glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, 1.0); glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, -1.0); glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);
	glTexCoord3d(1.0, 1.0, -1.0); glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);

	//ny
	glTexCoord3d(1.0, -1.0, 1.0); glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, -1.0, 1.0); glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, -1.0, -1.0); glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(1.0, -1.0, -1.0); glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);

	//pz
	glTexCoord3d(1.0, -1.0, 1.0); glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, -1.0, 1.0); glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, 1.0); glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(1.0, 1.0, 1.0); glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);

	//nz
	glTexCoord3d(1.0, -1.0, -1.0); glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(-1.0, -1.0, -1.0); glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(-1.0, 1.0, -1.0); glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);
	glTexCoord3d(1.0, 1.0, -1.0); glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);

	glEnd();
}

void environmentMapSetting(void) {
	glGenTextures(1, &g_nCubeTex);
	int width, height, channels;

	uchar* img0 = readImageData("BG/1.bmp", &width, &height, &channels);
	uchar* img1 = readImageData("BG/2.bmp", &width, &height, &channels);
	uchar* img2 = readImageData("BG/3.bmp", &width, &height, &channels);
	uchar* img3 = readImageData("BG/4.bmp", &width, &height, &channels);
	uchar* img4 = readImageData("BG/5.bmp", &width, &height, &channels);
	uchar* img5 = readImageData("BG/6.bmp", &width, &height, &channels);

	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img1);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img2);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img3);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img4);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img5);

	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_CUBE_MAP);
}