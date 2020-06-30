#include <gl/glut.h>
#include <stdio.h>
#include <Windows.h>
#include <utility>
#include <stdlib.h>
#include <GL/glext.h>
#include<ctime>
using namespace std;
#include "src/bmpfuncs.h"
#include "src/ObjParser.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include<mmsystem.h> //for sound
#pragma comment(lib,"winmm.lib");

#define HEIGHT 800
#define WIDTH 1000

//폭발
#define NUM_PARTICLES 10000
#define NUM_DEBRIS 1000
int fuel = 0, wantNormalize = 0;

/* time set variable */
clock_t start, moment, start_, moment_;
float duration, duration_ = 0.0;

double theta_ = 45.0;
double phi_ = 45.0;
int radius_ = 18;
double upVector_ = cos(phi_* M_PI / 180);
double degree = M_PI / 180;

bool goflag = false;
float go_x, go_z = 0;
float goDirection = 0;
float speed_ = 1;

/* camera location - 2 */
double eyex_, eyey_, eyez_;
float forx, forz = 0; //for all making 시각용

float get_time(void); //시간을 얻자 //for 배틀 전체 시간
float get_time_(void); //시간을 얻자 //for 때리고 나서 시간
double time1;
void printInstruction();

bool draw_all = false;

void keyboard_battle(unsigned char key, int x, int y);
void readFile(int x); //i 받아서 스위치로 파일 따로 열어주면 되지 않을까

int random1, random2 = 0; //여는 obj고르는
void loadTexture();
void drawBattle();
void do_battle(); //여기서 싸울떄 어캐 하는지,.
int fighting = 0;
bool fighting_flag = false;
int spacePushed = 0;
bool battle_done = false;
float x; //스페이스 누르면 움직이는 

double one = double(glutGet(GLUT_ELAPSED_TIME)) / 1000.0 + 1 ; //프로젝트 실행후 수가 초마다 1씩 늘어남
double one_ ;

bool preview_f = true;
bool making = true;

void shake();
void init2(void); //배틀용 
void init(void);
void draw(void);
void draw2(void); //배틀용
void draw_axis();
void draw_string(void* font, const char* str, float x_position, float y_position, float red, float green, float blue);
void draw_skyBox(float nskySize);

void light_default(void);
void setDefault();

void resize(int width, int height);
void mouse(int button, int state, int x, int y);
void activeMotion(int x, int y);
void save_menu(int option);
void load_menu(int option);
void MyMenu(); //메뉴
void main_menu_function(int option);
void keyboard(unsigned char key, int x, int y);
void specialKeyboard(int key, int x, int y);
void idle(); void idle2(); //배틀용

FILE *out; //save & load
int menu;
int save, load;

/* camera location */
double theta = 90; double phi = 40;
double radius = 35; //멀어짐 정도
double upVector = cos(phi* M_PI / 180);
double eyex, eyey, eyez;

/* object location*/
float trans_x, trans_y, trans_z = 0;
float rota_x, rota_y, rota_z = 0;//는 아래와 같은 역할을 하지 않을까

int look = 20;
int Mode = 999;
int cameraMode = 0;
int ProjectMode = 0; //making 모드의 mode는 0, zoo는 1, battle은 2

/* texture mapM_PIng set variable */
GLuint *skyboxTex = new GLuint();  // environment map - skybox
void skyboxTextureMapping(void);

/* Back Ground */
GLuint *battle = new GLuint();

/* multiple viewports */
GLint current_width, current_height;
double mini_theta = 45; double mini_phi = 45;
double mini_upV = 1;

/* object translation/rotation */
GLfloat tran_x = 0, tran_y = 0, tran_z = 0;
GLfloat obj_angle = 0;
bool keystates[5] = { false };
int mouse_x = 0, mouse_y = 0;  // eye position setting

/* lego */
void draw_obj(ObjParser *objParser);
ObjParser *Lego_1, *Lego_2, *Lego_3, *Lego_4, *Lego_5, *Lego_6, *Lego_7, *Lego_8, *Lego_9, *Lego_10, *Lego_11, *Lego_12, *Lego_13, *Lego_14, *Lego_15;

int lego_num = 0;
unsigned int color = 0, lego_type = 0;  // 프리뷰에서 보여지는 레고의 색과 타입
unsigned int selected_color, selected_type;  // 프리뷰를 통해 정해진 현재 레고의 색과 타입

//배경화면 설정 + 그 아페 obj설정 해도 보일까? dㅇㅇ 보임
GLuint g_textureID = -1;

float goX(float ang) {
	return sin(ang*degree);
}

float goZ(float ang) {
	return cos(ang*degree);
}

float get_time(void)	// 이 함수의 사용후로부터 총 시간을 잰다
{
	moment = clock();	// 현재 시각 
	duration = (float)(moment - start);	// 현재 시각빼기

	return duration;	// 발사한 후로 얼마나 지났는 지를 리턴하비다.
} //1000이 1초임미다.

float get_time_(void)	// 이 함수의 사용후로부터 총 시간을 잰다
{
	moment_ = clock();	// 현재 시각 
	duration_ = (float)(moment_ - start_);	// 현재 시각빼기

	return duration_;	// 발사한 후로 얼마나 지났는 지를 리턴하비다.
} //1000이 1초임미다.

struct Vertex {
	float tu, tv;
	float x, y, z;
};

Vertex g_quadVertices[] = {
	{ 0,0,-1,-1,0 },{ 1,0,1,-1,0 },{ 1,1,1,1,0 },{ 0,1,-1,1,0 }
};

//obj부서짐
struct particleData {
	float position[3];
	float speed[3];
	float color[3];
};

typedef struct particleData particleData;

struct debrisData {
	float position[3];
	float speed[3];
	float orientation[3];
	float orientationSpeed[3];
	float color[3];
	float scale[3];
};

typedef struct debrisData debrisData;

particleData particles[NUM_PARTICLES];
debrisData debris[NUM_DEBRIS];

void newSpeed(float dest[3]) {
	float x = (2.0 * ((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;
	float y = (2.0 * ((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;
	float z = (2.0 * ((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;

	if (wantNormalize) {
		float len = sqrt(x * x + y * y + z * z);

		if (len) {
			x = x / len;
			y = y / len;
			z = z / len;
		}
	}
	dest[0] = x;
	dest[1] = y;
	dest[2] = z;
}

void newExplosion() {
	for (int i = 0; i < NUM_PARTICLES; i++) {
		particles[i].position[0] = 0.0;
		particles[i].position[1] = 0.0;
		particles[i].position[2] = 0.0;

		particles[i].color[0] = 1.0;
		particles[i].color[1] = 1.0;
		particles[i].color[2] = 0.5;

		newSpeed(particles[i].speed);
	}

	for (int i = 0; i < NUM_DEBRIS; i++) {
		debris[i].position[0] = 0.0;
		debris[i].position[1] = 0.0;
		debris[i].position[2] = 0.0;

		debris[i].orientation[0] = 0.0;
		debris[i].orientation[1] = 0.0;
		debris[i].orientation[2] = 0.0;

		debris[i].color[0] = .7;
		debris[i].color[1] = .7;
		debris[i].color[2] = .7;

		debris[i].scale[0] = (2.0 * ((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;
		debris[i].scale[1] = (2.0 * ((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;
		debris[i].scale[2] = (2.0 * ((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;

		newSpeed(debris[i].speed);
		newSpeed(debris[i].orientationSpeed);

	}
	fuel = 4000;
}

//레고 객체화
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
			glColor3f(0.7450f, 0.7450f, 0.7450f); //회색
			break;
		case 2:
			glColor3f(0.947059f, 0.644706f, 0.364706f); //황토색
			break;
		case 3:
			glColor3f(0, 0, 0); //검정색
			break;
		case 4:
			glColor3f(1, 1, 1); //하얀색
			break;
		case 5:
			glColor3f(0.937255f, 0.220784f, 0.229784f);  //빨간색
			break;
		case 6:
			glColor3f(0.603922f, 0.803922f, 0.196078f);  //연두색
			break;
		case 7:
			glColor3f(0, 1, 0.2f);   //초록색
			break;
		case 8:
			glColor3f(1.05f, 6.04706f, 0.424706f); //살색
			break;
		case 9:
			glColor3f(1, 0.4f, 0); // 주황색
			break;
		case 10:
			glColor3f(1, 0.8f, 0.1f); // 노란색
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
		case 12:
			draw_obj(Lego_12);
			break;
		case 13:
			draw_obj(Lego_13);
			break;
		case 14:
			draw_obj(Lego_14);
			break;
		case 15:
			draw_obj(Lego_15);
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
	void set_lego_rotate(float x, float y, float z)
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

//레고 객체화
class LEGO1 {
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
			glColor3f(0.7450f, 0.7450f, 0.7450f); //회색
			break;
		case 2:
			glColor3f(0.947059f, 0.644706f, 0.364706f); //황토색
			break;
		case 3:
			glColor3f(0, 0, 0); //검정색
			break;
		case 4:
			glColor3f(1, 1, 1); //하얀색
			break;
		case 5:
			glColor3f(0.937255f, 0.220784f, 0.229784f);  //빨간색
			break;
		case 6:
			glColor3f(0.603922f, 0.803922f, 0.196078f);  //연두색
			break;
		case 7:
			glColor3f(0, 1, 0.2f);   //초록색
			break;
		case 8:
			glColor3f(1.05f, 6.04706f, 0.424706f); //살색
			break;
		case 9:
			glColor3f(1, 0.4f, 0); // 주황색
			break;
		case 10:
			glColor3f(1, 0.8f, 0.1f); // 노란색
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
		case 12:
			draw_obj(Lego_12);
			break;
		case 13:
			draw_obj(Lego_13);
			break;
		case 14:
			draw_obj(Lego_14);
			break;
		case 15:
			draw_obj(Lego_15);
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
	void set_lego_rotate(float x, float y, float z)
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

LEGO lego[1000], battle_obj[1000];
LEGO1 all_obj[1000];
LEGO preview, redo;

void draw_preview(ObjParser *objParser) {
	glBegin(GL_TRIANGLES);
	for (unsigned int n = 0; n < objParser->getFaceSize(); n += 3) { //obj parser에서 3면의 face를 받아온다
																	 //우선 n-1부터, 전
		glNormal3f(objParser->normal[objParser->normalIdx[n] - 1].x, // normal 인덱스를 받아옴 
			objParser->normal[objParser->normalIdx[n] - 1].y,
			objParser->normal[objParser->normalIdx[n] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n] - 1].x, //vertice 인덱스
			objParser->vertices[objParser->vertexIdx[n] - 1].y,
			objParser->vertices[objParser->vertexIdx[n] - 1].z);

		//이번엔 n+1 -1 ㅡ중
		glNormal3f(objParser->normal[objParser->normalIdx[n + 1] - 1].x,
			objParser->normal[objParser->normalIdx[n + 1] - 1].y,
			objParser->normal[objParser->normalIdx[n + 1] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 1] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].z);

		//이번엔 n+2 -1, 후
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
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);  // 실행되는 glut 창의 크기

	glutInitWindowPosition(400, 100); //중앙에 윈도우 생성
	glutCreateWindow("Final");  // 실행되는 glut 창의 이름

	/* Default Making BGM */
	PlaySound(TEXT("audio/making.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
	init();  // -> 사용자 초기화 함수
	
	/* Callback 함수 정의 */
	/* 유휴시간에 할 일 */

	glutDisplayFunc(draw);
	glutReshapeFunc(resize);

	printInstruction(); //소개

	/* 마우스 동작처리 등록 */
	glutMouseFunc(mouse);  // 마우스 버튼 입력 처리. left/rigt/wheel click 시 활성됨.
	glutMotionFunc(activeMotion);  // drag tracking
	/* Pop up 메뉴 생성 및 추가 */
	MyMenu();

	/* 키보드 입력 처리 */
	glutKeyboardFunc(keyboard);  // 입력된 일반키에 대한 처리
	glutSpecialFunc(specialKeyboard);  // when a special key is pressed
	glutIdleFunc(idle);

	/* --------------------------------------------------------------------------------------------------------------------------------------------*/

	//second battle
	glutInitWindowPosition(0, 0); //창 처음에 뜨는 위치

	glutCreateWindow("battle");  // 실행되는 glut 창의 이름
	init2();  // -> 사용자 초기화 함수	

	glutDisplayFunc(draw2); //싸울떄 사용되는 윈도우창대한 draw
	glutKeyboardFunc(keyboard_battle);
	
	/* 유휴시간에 할 일 */
	glutIdleFunc(idle2);

    /* LooM_PIng 시작 */
	glutMainLoop(); //이거위로 겹침
	
	return 0;
}

void init(void) {
	/* Load obj files for drawing legos */
	/* height = 2, width = 1, length ~ */
	Lego_1 = new ObjParser("obje/body.obj"); //해당 메모리로 객체를 만든후 그 주소 포인터를 반환, &랑 비슷 
	Lego_2 = new ObjParser("obje/eye.obj");
	Lego_3 = new ObjParser("obje/foot.obj");
	Lego_4 = new ObjParser("obje/head.obj");
	Lego_5 = new ObjParser("obje/leg_b.obj");
	Lego_6 = new ObjParser("obje/leg_f.obj");
	Lego_7 = new ObjParser("obje/mouth.obj");
	Lego_8 = new ObjParser("obje/mouth_bird.obj");
	Lego_9 = new ObjParser("obje/part.obj");
	Lego_10 = new ObjParser("obje/tail_ball.obj");
	Lego_11 = new ObjParser("obje/tail_cir.obj");
	Lego_12 = new ObjParser("obje/tail_long.obj");
	Lego_13 = new ObjParser("obje/tail_tri.obj");
	Lego_14 = new ObjParser("obje/wing_l.obj");
	Lego_15 = new ObjParser("obje/wing_r.obj");

	// Interface Introduction
	/* 화면의 기본색 설정 */
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);

	/* light setting */
	light_default();

	/* Texture */
	glEnable(GL_TEXTURE_2D);

	skyboxTextureMapping();

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);   //polygon의 원래 색상은 무시하고 texture로 덮음
	glEnable(GL_TEXTURE_2D);

}

void init2(void) {
	/* Background color :  Black */
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/* resize callback */
	glutReshapeFunc(resize);

	/* light setting */
	light_default();
	
	/* BG texture */
	loadTexture();
	
	/* Texture */
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);   //polygon의 원래 색상은 무시하고 texture로 덮음
	glEnable(GL_TEXTURE_2D);

	/* Enable depth buffer */
	glEnable(GL_DEPTH_TEST);

	readFile(random1); //랜덤 FILE 열기
}

void draw_miniView(void) {
	glMatrixMode(GL_VIEWPORT);
	glPushMatrix();
	glViewport(current_width * 2 / 3, current_height * 2 / 3, current_width / 3, current_height / 3);  // 오른쪽 맨 위 1/3 크기
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glClearColor(0, 0, 0., 0);

	double mini_eyex, mini_eyey, mini_eyez;

	mini_eyex = 19 * sin(mini_theta*M_PI / 180)*cos(mini_phi*M_PI / 180); //곱하는 수 클수록 멀리이
	mini_eyey = 19 * sin(mini_phi*M_PI / 180);
	mini_eyez = 19 * cos(mini_theta*M_PI / 180)*cos(mini_phi*M_PI / 180);
	
	gluLookAt(mini_eyex, mini_eyey, mini_eyez, 0, 0, 0, 0, mini_upV, 0);

	glRotatef(-90, 0, 1, 0);
	glTranslatef(0, 0, -2);


	glScalef(1.2, 1.2, 1.2); //더 작게 보이게 
	preview.set_lego_color(color);
	preview.set_lego_type(lego_type);
	preview.set_lego_rotate(0, 0, 0);
	preview.set_lego_location(0, 0, 0);
	
	if (preview_f) {
		preview.draw_lego();
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, " [ P R E V I E W ] ", -1, 7, 0, 0, 0);
	}
	else {

	}

	glPopMatrix();

	glMatrixMode(GL_VIEWPORT);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();

	glFlush();
}

void draw_mainView() {
	glPushMatrix();
	glViewport(0, 0, current_width, current_height);
	glLoadIdentity();

	eyex = radius * sin(theta*M_PI / 180)*cos(phi*M_PI / 180);
	eyey = radius * sin(phi*M_PI / 180);
	eyez = radius * cos(theta*M_PI / 180)*cos(phi*M_PI / 180);
	upVector = cos(phi*M_PI / 180); // simplify upvector correspond to y-axis
	

	if (cameraMode == 0) {
		gluLookAt(eyex + go_x , eyey, eyez + go_z, go_x, 0, go_z, 0, upVector, 0); //default //all만들고 나서 trans 빼야함
	}
	else if (cameraMode == 1) { //이거 되긴 하,.?
		gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, upVector, 0);
	}

	draw_skyBox(300.0f);

	//glRotatef(-90, 0, 1, 0);
	draw_axis();

	glPushMatrix();

	all_obj[lego_num].set_lego_color(selected_color);
	all_obj[lego_num].set_lego_type(selected_type);
	all_obj[lego_num].set_lego_location(tran_x, tran_y, tran_z); // 그 후 이동시켜주고 이를 저장하게 될 것임
	all_obj[lego_num].set_lego_rotate(rota_x, rota_y, rota_z); // 회전시켜 주고 이를 저장하게 될 것임

	for (int i = 0; i <= 100; i++) { //레고 그림
		all_obj[i].draw_lego();
	}

	glPopMatrix();

	glPushMatrix();
	/* drawing legos */
	glScalef(1.2, 1.2, 1.2); //scailing to 0.8

	lego[lego_num].set_lego_color(selected_color);
	lego[lego_num].set_lego_type(selected_type);
	lego[lego_num].set_lego_location(tran_x, tran_y, tran_z); // 그 후 이동시켜주고 이를 저장하게 될 것임
	lego[lego_num].set_lego_rotate(rota_x, rota_y, rota_z); // 회전시켜 주고 이를 저장하게 될 것임
 
	//7을 누르면 obj가 앞으로 가는것을 볼 수 있드
	glTranslatef(go_x, 0, go_z); //이걸로 움직
	glRotatef(goDirection, 0, 1, 0); //이걸로 움직
	
	for (int i = 0; i <= lego_num; i++) { //레고 그림
		lego[i].draw_lego();
	}

	glPopMatrix();

	glFlush();
}



void draw(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_mainView();
	draw_miniView();

	glutSwapBuffers();
}

void shake() {
	if (0 < get_time_() < 500) {
		glTranslatef(0, 0, x); //흔들림
	}
	else if (500 < get_time_() < 1000) {
		glTranslatef(0, 0, 4); //흔들림
	}
	else {
		glTranslatef(0, 0, -2); //흔들림
	}
}

void draw2(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	eyex_ = radius_ * sin(theta_*M_PI / 180)*cos(phi_*M_PI / 180);
	eyey_ = radius_ * sin(phi_*M_PI / 180);
	eyez_ = radius_ * cos(theta_*M_PI / 180)*cos(phi_*M_PI / 180);
	upVector_ = cos(phi_*M_PI / 180); // simplify upvector correspond to y-axis

	gluLookAt(eyex_, eyey_, eyez_, 0, 0, 0, 0, upVector_, 0);
	
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f); //큐브나 좌표축 그릴 때 사용한 색의 영향을 안받을려면 필요
	glBindTexture(GL_TEXTURE_2D, *battle);
	glPushMatrix();
	glScalef(1.3, 1.3, 1.3);
	glRotatef(45, 0, 1, 0);
	glRotatef(-45, 1, 0, 0);
	glTranslatef(-0, -0, -35);
	drawBattle(); //배경그림
	glPopMatrix();

	switch (spacePushed)
	{
	case 3:
		glTranslatef(0.5, -0, -0);
		glutPostRedisplay();
		break;
	case 4:
		glTranslatef(-1, -0, 0);
		glScalef(0.8, 0.8, 0.8);
		glutPostRedisplay();
		break;
	case 5:
		glTranslatef(0.7, -0, 0);
		glScalef(1.2, 1.2, 1.2);
		glutPostRedisplay();
		break;
	case 6:
		glTranslatef(-0.8, -0, 0);
		break;
	case 7:
		glTranslatef(0.8, -0, 0);
		break;
	case 8:
		break;
	}
	if (fuel == 0) {
		//여기에오브제
		glScalef(0.8, 0.8, 0.8);
		glTranslatef(1.2, 0, -9);
		glRotatef(-30, 1, 0, 0);//고개 들어올림정도 //작을수록 높이
		glRotatef(38, 1, 0, 1); //바닥에 기울어진정도 //클수록왼쪽으로 기울
		glRotatef(-20, 0, 0, 1);
		glTranslatef(-.5, -2, 0);

		for (int i = 0; i <= 1000; i++) { //레고 그림
			battle_obj[i].draw_lego();
			//battle 마치고, battle_obj.LEGO_init 해야합니당!!!
		}

	}

	if (fuel > 0) {
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glBegin(GL_POINTS);
		for (int i = 0; i < NUM_PARTICLES; i++) {
			glColor3fv(particles[i].color);
			glVertex3fv(particles[i].position);

		}
		glEnd();
		glPopMatrix();

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_DEPTH_TEST);
		glNormal3f(0.0, 0.0, 1.0);

		for (int i = 0; i < NUM_DEBRIS; i++) {
			glColor3fv(debris[i].color);

			glPushMatrix();
			glTranslatef(debris[i].position[0], debris[i].position[1], debris[i].position[2]);
			glRotatef(debris[i].orientation[0], 1.0, 0, 0);
			glRotatef(debris[i].orientation[1], 0, 1.0, 0);
			glRotatef(debris[i].orientation[2], 0, 0, 1.0);
			glScalef(debris[i].scale[0], debris[i].scale[1], debris[i].scale[2]);

			glBegin(GL_TRIANGLES);
			glVertex3f(0, 0.5, 0);
			glVertex3f(-0.25, 0, 0);
			glVertex3f(0.25, 0, 0);
			glEnd();
			glPopMatrix();
		}
	}

switch (spacePushed)
	{
	default:
		break;
	case 1:
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, " Oops! ", -8.5, -6.5, 0, 0, 0);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, " Wild Trivial object", -8.5, -7.5, 0, 0, 0);
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, " appeared!", -8.5, -8.5, 0, 0, 0);
		break;
	case 2:
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, " U can break this with ' 7' hitting", -8.5, -7.5, 0, 0, 0);
		break;
	case 3:
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, " U can break this with ' 6' hitting", -8.5, -7.5, 0, 0, 0);
		glTranslatef(-0, -0, -10);
		glutPostRedisplay();
		break;
	case 4:
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, " U can break this with ' 5' hitting", -8.5, -7.5, 0, 0, 0);
		glTranslatef(-0, -0, 10);
		glutPostRedisplay();
		break;
	case 5:
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, " U can break this with ' 4' hitting", -8.5, -7.5, 0, 0, 0);
		break;
	case 6:
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, " U can break this with ' 3' hitting", -8.5, -7.5, 0, 0, 0);
		break;
	case 7:
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, " U can break this with ' 2' hitting", -8.5, -7.5, 0, 0, 0);
		break;
	case 8:
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, " U can break this with ' 1' hitting", -8.5, -7.5, 0, 0, 0);
		break;
	}
	
	glutSwapBuffers();
}

void draw_obj(ObjParser *objParser) {
	glBegin(GL_TRIANGLES);
	for (unsigned int n = 0; n < objParser->getFaceSize(); n += 3) { //obj parser에서 3면의 face를 받아온다
																	 //우선 n-1부터, 전
		glNormal3f(objParser->normal[objParser->normalIdx[n] - 1].x, // normal 인덱스를 받아옴 
			objParser->normal[objParser->normalIdx[n] - 1].y,
			objParser->normal[objParser->normalIdx[n] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n] - 1].x, //vertice 인덱스
			objParser->vertices[objParser->vertexIdx[n] - 1].y,
			objParser->vertices[objParser->vertexIdx[n] - 1].z);

		//이번엔 n+1 -1 ㅡ중
		glNormal3f(objParser->normal[objParser->normalIdx[n + 1] - 1].x,
			objParser->normal[objParser->normalIdx[n + 1] - 1].y,
			objParser->normal[objParser->normalIdx[n + 1] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 1] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].z);

		//이번엔 n+2 -1, 후
		glNormal3f(objParser->normal[objParser->normalIdx[n + 2] - 1].x,
			objParser->normal[objParser->normalIdx[n + 2] - 1].y,
			objParser->normal[objParser->normalIdx[n + 2] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 2] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].z);
	}
	glEnd();
}

void draw_axis() {
	glLineWidth(1);
	glBegin(GL_LINES); {
		glColor3f(1, 0, 0);  // x축은 red
		glVertex3f(0, 0, 0);
		glVertex3f(3, 0, 0);
		glColor3f(0, 1, 0);  // y축은 green
		glVertex3f(0, 0, 0);
		glVertex3f(0, 3, 0);
		glColor3f(0, 0, 1);  // z축은 blue
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 3);
	} glEnd();

	glLineWidth(1);  // 두께 다시 환원
}

void draw_string(void* font, const char* str, float x_position, float y_position, float red, float green, float blue) {
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-10, 10, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(red, green, blue);
	glRasterPos3f(x_position, y_position, 0);

	for (unsigned int i = 0; i < strlen(str); i++) {
		glutBitmapCharacter(font, str[i]);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}

void draw_skyBox(float nskySize) {
	glTranslatef(0, nskySize, 0);

	glColor3f(1.0, 1.0, 1.0);   // white로 color를 set해주어야 texture색상이 제대로 적용 됨!
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // polygon의 원래 색상은 무시하고 texture로 덮음
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // polygon의 원래 색상과 texture 색상을 곱하여 덮음, texture가 입혀진 표면에 광원 효과 설정 가능
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *skyboxTex);
	glBegin(GL_QUADS);

	// px
	glTexCoord3d(1.0, -1.0, -1.0); glVertex3f(nskySize, -nskySize, -nskySize);
	glTexCoord3d(1.0, -1.0, 1.0); glVertex3f(nskySize, -nskySize, nskySize);
	glTexCoord3d(1.0, 1.0, 1.0); glVertex3f(nskySize, nskySize, nskySize);
	glTexCoord3d(1.0, 1.0, -1.0); glVertex3f(nskySize, nskySize, -nskySize);

	// nx
	glTexCoord3d(-1.0, -1.0, -1.0); glVertex3f(-nskySize, -nskySize, -nskySize);
	glTexCoord3d(-1.0, -1.0, 1.0); glVertex3f(-nskySize, -nskySize, nskySize);
	glTexCoord3d(-1.0, 1.0, 1.0); glVertex3f(-nskySize, nskySize, nskySize);
	glTexCoord3d(-1.0, 1.0, -1.0); glVertex3f(-nskySize, nskySize, -nskySize);

	// py
	glTexCoord3d(1.0, 1.0, 1.0); glVertex3f(nskySize, nskySize, nskySize);
	glTexCoord3d(-1.0, 1.0, 1.0); glVertex3f(-nskySize, nskySize, nskySize);
	glTexCoord3d(-1.0, 1.0, -1.0); glVertex3f(-nskySize, nskySize, -nskySize);
	glTexCoord3d(1.0, 1.0, -1.0); glVertex3f(nskySize, nskySize, -nskySize);

	// ny
	glTexCoord3d(1.0, -1.0, 1.0); glVertex3f(nskySize, -nskySize, nskySize);
	glTexCoord3d(-1.0, -1.0, 1.0); glVertex3f(-nskySize, -nskySize, nskySize);
	glTexCoord3d(-1.0, -1.0, -1.0); glVertex3f(-nskySize, -nskySize, -nskySize);
	glTexCoord3d(1.0, -1.0, -1.0); glVertex3f(nskySize, -nskySize, -nskySize);

	// pz
	glTexCoord3d(1.0, -1.0, 1.0); glVertex3f(nskySize, -nskySize, nskySize);
	glTexCoord3d(-1.0, -1.0, 1.0); glVertex3f(-nskySize, -nskySize, nskySize);
	glTexCoord3d(-1.0, 1.0, 1.0); glVertex3f(-nskySize, nskySize, nskySize);
	glTexCoord3d(1.0, 1.0, 1.0); glVertex3f(nskySize, nskySize, nskySize);

	// nz
	glTexCoord3d(1.0, -1.0, -1.0); glVertex3f(nskySize, -nskySize, -nskySize);
	glTexCoord3d(-1.0, -1.0, -1.0); glVertex3f(-nskySize, -nskySize, -nskySize);
	glTexCoord3d(-1.0, 1.0, -1.0); glVertex3f(-nskySize, nskySize, -nskySize);
	glTexCoord3d(1.0, 1.0, -1.0); glVertex3f(nskySize, nskySize, -nskySize);

	glEnd();

	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_CUBE_MAP);

	glTranslatef(0, -nskySize , 0);
}

void skyboxTextureMapping() {
	glGenTextures(1, skyboxTex);
	int width, height, channels;
	uchar* img[6];

	glBindTexture(GL_TEXTURE_CUBE_MAP, *skyboxTex);
	img[0] = readImageData("BG/nx.bmp", &width, &height, &channels);  // img 변수 안에 negative x 면의 RGB 정보가 들어가게 됨
	img[1] = readImageData("BG/px.bmp", &width, &height, &channels);
	img[2] = readImageData("BG/ny.bmp", &width, &height, &channels);  // y axis
	img[3] = readImageData("BG/py.bmp", &width, &height, &channels);  //하늘
	img[4] = readImageData("BG/nz.bmp", &width, &height, &channels);  // Z axis
	img[5] = readImageData("BG/pz.bmp", &width, &height, &channels);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img[1]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img[2]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img[3]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img[4]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img[5]);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
}

void resize(int width, int height) {  // WM_SIZE 메시지 처리를 위한 callback 함수
	// 윈도우 생성 및 크기변화 시 WM_SIZE 메시지 발생
	// 크기 변화시 viewport설정을 다시 해야함
	printf("resize function is called\n");
	current_width = width; current_height = height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
}

void idle(void) {
	if (goflag) {
		go_x += goX(goDirection)*0.02*speed_;
		go_z += goZ(goDirection)*0.02*speed_;
	}
	else {

	}

	if (spacePushed != 0) {
		if (0 < get_time_() < 1000) {
			x = -2;
			glutPostRedisplay();
		}
		else if (1000 < get_time_() < 1500) {
			x = 2;
			glutPostRedisplay();
		}
		else {
			x = 0;
			glutPostRedisplay();
		}
	}
	else{}

	if (battle_done) {
		MessageBox(NULL, TEXT(" 다른 WIndow 창을 봐주세요"), TEXT("부셔버리셨어요!"), MB_OK);
	}else{}

	glutPostRedisplay();
}

void idle2() {
	if (goflag) {
		go_x += goX(goDirection)*0.02*speed_;
		go_z += goZ(goDirection)*0.02*speed_;
	}
	else {

	}

	if (spacePushed != 0) {
		if (0 < get_time_() < 1000) {
			x = -2;
			glutPostRedisplay();
		}
		else if (1000 < get_time_() < 1500) {
			x = 2;
			glutPostRedisplay();
		}
		else {
			x = 0;
			glutPostRedisplay();
		}
	}
	else {}

	if (fuel > 0) {
		for (int i = 0; i < NUM_PARTICLES; i++) {
			particles[i].position[0] += particles[i].speed[0] * 0.2;
			particles[i].position[1] += particles[i].speed[1] * 0.2;
			particles[i].position[2] += particles[i].speed[2] * 0.2;

			particles[i].color[0] -= 1 / 500;
			if (particles[i].color[0] < 0) {
				particles[i].color[0] = 0;
			}

			particles[i].color[1] -= 1 / 100;
			if (particles[i].color[1] < 0) {
				particles[i].color[1] = 0;
			}

			particles[i].color[2] -= 1 / 50;
			if (particles[i].color[2] < 0) {
				particles[i].color[2] = 0;
			}
		}

		for (int i = 0; i < NUM_DEBRIS; i++) {
			debris[i].position[0] += debris[i].speed[0] * 0.1;
			debris[i].position[1] += debris[i].speed[1] * 0.1;
			debris[i].position[2] += debris[i].speed[2] * 0.1;

			debris[i].orientation[0] += debris[i].orientationSpeed[0] * 10;
			debris[i].orientation[1] += debris[i].orientationSpeed[1] * 10;
			debris[i].orientation[2] += debris[i].orientationSpeed[2] * 10;

		}
		--fuel;
	}
    
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	/* 인자들을 해석해서 원하는 기능 구현 */
	/* state값: 0은 버튼이 눌리는 상태변화(=클릭)를 의미하고, 1은 버튼이 (눌렸다가) 떨어지는 상태변화(=언클릭)를 의미 */
	if (state) {
		if (button == 3) {  // Zoom in with 
			radius = radius * 0.9;
			printf("The camera is getting closer to the object\n");
		}
		else if (button == 4) {
			radius = radius * 1.1;
			printf("The camera is getting further from the object\n");
		}
	}
}

void activeMotion(int x, int y) {  // motion tracking when mouse button is being clicked
								   //printf("x: %d, y: %d\n", x, y);
	if (x < mouse_x) {
		theta--;
		if (theta <= 0) theta += 360;
		printf("theta: %.0f, phi: %.0f\n", theta, phi);
	}
	else if (x > mouse_x) {
		theta++;
		if (theta > 360) theta -= 360;
		printf("theta: %.0f, phi: %.0f\n", theta, phi);
	}

	if (y < mouse_y) {
		phi -= 1;
		if (phi < 0) {
			phi += 360;
			upVector = -1;
		}
		else if (phi == 180) upVector = 1;
		printf("theta: %.0f, phi: %.0f\n", theta, phi);
	}
	else if (y > mouse_y) {
		phi += 1;  // y axis starts at top of the window
		if (phi >= 360) {
			phi -= 360;
			upVector = 1;
		}
		else if (phi == 181) upVector = -1;
		printf("theta: %.0f, phi: %.0f\n", theta, phi);
	}
	mouse_x = x; mouse_y = y;
}

void MyMenu() {	
	save = glutCreateMenu(save_menu);
	glutAddMenuEntry("1", 1);
	glutAddMenuEntry("2", 2);
	glutAddMenuEntry("3", 3);
	glutAddMenuEntry("4", 4);
	glutAddMenuEntry("5", 5);
	glutAddMenuEntry("6", 6);

	load = glutCreateMenu(load_menu);
	glutAddMenuEntry("1", 1);
	glutAddMenuEntry("2", 2);
	glutAddMenuEntry("3", 3);
	glutAddMenuEntry("4", 4);
	glutAddMenuEntry("5", 5);
	glutAddMenuEntry("6", 6);
	glutAddMenuEntry("for many", 7);


	menu = glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 1);
	glutAddMenuEntry("Zoo", 2);
	glutAddSubMenu("Save", save);
	glutAddSubMenu("Load", load);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void save_menu(int option) {
	switch (option)
	{
	default:
		break;
	case 1: //save  
		printf("Save model has selected\n");
		fopen_s(&out, "file1.txt", "w");
		for (int i = 0; i <= lego_num; i++) {
			fprintf(out, "%d, %d, %d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f \n",
				i, lego[i].get_lego_type(), lego[i].get_lego_color(),
				lego[i].get_lego_location_x(), lego[i].get_lego_location_y(), lego[i].get_lego_location_z(),
				lego[i].get_lego_rotate_x(), lego[i].get_lego_rotate_y(), lego[i].get_lego_rotate_z());
		}
		fclose(out);
		break;
	case 2: //save  
		printf("Save model has selected\n");
		fopen_s(&out, "file2.txt", "w");
		for (int i = 0; i <= lego_num; i++) {
			fprintf(out, "%d, %d, %d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f \n",
				i, lego[i].get_lego_type(), lego[i].get_lego_color(),
				lego[i].get_lego_location_x(), lego[i].get_lego_location_y(), lego[i].get_lego_location_z(),
				lego[i].get_lego_rotate_x(), lego[i].get_lego_rotate_y(), lego[i].get_lego_rotate_z());
		}
		fclose(out);
		break;
	case 3: //save  
		printf("Save model has selected\n");
		fopen_s(&out, "file3.txt", "w");
		for (int i = 0; i <= lego_num; i++) {
			fprintf(out, "%d, %d, %d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f \n",
				i, lego[i].get_lego_type(), lego[i].get_lego_color(),
				lego[i].get_lego_location_x(), lego[i].get_lego_location_y(), lego[i].get_lego_location_z(),
				lego[i].get_lego_rotate_x(), lego[i].get_lego_rotate_y(), lego[i].get_lego_rotate_z());
		}
		fclose(out);
		break;
	case 4: //save  
		printf("Save model has selected\n");
		fopen_s(&out, "file4.txt", "w");
		for (int i = 0; i <= lego_num; i++) {
			fprintf(out, "%d, %d, %d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f \n",
				i, lego[i].get_lego_type(), lego[i].get_lego_color(),
				lego[i].get_lego_location_x(), lego[i].get_lego_location_y(), lego[i].get_lego_location_z(),
				lego[i].get_lego_rotate_x(), lego[i].get_lego_rotate_y(), lego[i].get_lego_rotate_z());
		}
		fclose(out);
		break;
	case 5: //save  
		printf("Save model has selected\n");
		fopen_s(&out, "file5.txt", "w");
		for (int i = 0; i <= lego_num; i++) {
			fprintf(out, "%d, %d, %d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f \n",
				i, lego[i].get_lego_type(), lego[i].get_lego_color(),
				lego[i].get_lego_location_x(), lego[i].get_lego_location_y(), lego[i].get_lego_location_z(),
				lego[i].get_lego_rotate_x(), lego[i].get_lego_rotate_y(), lego[i].get_lego_rotate_z());
		}
		fclose(out);
		break;
	case 6: //save  
		printf("Save model has selected\n");
		fopen_s(&out, "file6.txt", "w");
		for (int i = 0; i <= lego_num; i++) {
			fprintf(out, "%d, %d, %d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f \n",
				i, lego[i].get_lego_type(), lego[i].get_lego_color(),
				lego[i].get_lego_location_x(), lego[i].get_lego_location_y(), lego[i].get_lego_location_z(),
				lego[i].get_lego_rotate_x(), lego[i].get_lego_rotate_y(), lego[i].get_lego_rotate_z());
		}
		fclose(out);
		break;
	}
}

void load_menu(int option) {
	switch (option)
	{
	default:
		break;
	case 1: //load  
		printf("Load model has selected\n");
		fopen_s(&out, "file1.txt", "r");
		while (!feof(out)) {
			fscanf_s(out, "%d, %d, %d, %f, %f, %f, %f, %f, %f \n",
				&lego_num, &selected_type, &selected_color,
				&tran_x, &tran_y, &tran_z, &rota_x, &rota_y, &rota_z);
			lego[lego_num].set_lego_color(selected_color);
			lego[lego_num].set_lego_type(selected_type);
			lego[lego_num].set_lego_rotate(rota_x, rota_y, rota_z);
			lego[lego_num].set_lego_location(tran_x, tran_y, tran_z);
		}
		fclose(out);
		break; // load model
	case 2: //load  
		printf("Load model has selected\n");
		fopen_s(&out, "file2.txt", "r");
		while (!feof(out)) {
			fscanf_s(out, "%d, %d, %d, %f, %f, %f, %f, %f, %f \n",
				&lego_num, &selected_type, &selected_color,
				&tran_x, &tran_y, &tran_z, &rota_x, &rota_y, &rota_z);
			lego[lego_num].set_lego_color(selected_color);
			lego[lego_num].set_lego_type(selected_type);
			lego[lego_num].set_lego_rotate(rota_x, rota_y, rota_z);
			lego[lego_num].set_lego_location(tran_x, tran_y, tran_z);
		}
		fclose(out);
		break; // load model
	case 3: //load  
		printf("Load model has selected\n");
		fopen_s(&out, "file3.txt", "r");
		while (!feof(out)) {
			fscanf_s(out, "%d, %d, %d, %f, %f, %f, %f, %f, %f \n",
				&lego_num, &selected_type, &selected_color,
				&tran_x, &tran_y, &tran_z, &rota_x, &rota_y, &rota_z);
			lego[lego_num].set_lego_color(selected_color);
			lego[lego_num].set_lego_type(selected_type);
			lego[lego_num].set_lego_rotate(rota_x, rota_y, rota_z);
			lego[lego_num].set_lego_location(tran_x, tran_y, tran_z);
		}
		fclose(out);
		break; // load model
	case 4: //load  
		printf("Load model has selected\n");
		fopen_s(&out, "file4.txt", "r");
		while (!feof(out)) {
			fscanf_s(out, "%d, %d, %d, %f, %f, %f, %f, %f, %f \n",
				&lego_num, &selected_type, &selected_color,
				&tran_x, &tran_y, &tran_z, &rota_x, &rota_y, &rota_z);
			lego[lego_num].set_lego_color(selected_color);
			lego[lego_num].set_lego_type(selected_type);
			lego[lego_num].set_lego_rotate(rota_x, rota_y, rota_z);
			lego[lego_num].set_lego_location(tran_x, tran_y, tran_z);
		}
		fclose(out);
		break; // load model
	case 5: //load  
		printf("Load model has selected\n");
		fopen_s(&out, "file5.txt", "r");
		while (!feof(out)) {
			fscanf_s(out, "%d, %d, %d, %f, %f, %f, %f, %f, %f \n",
				&lego_num, &selected_type, &selected_color,
				&tran_x, &tran_y, &tran_z, &rota_x, &rota_y, &rota_z);
			lego[lego_num].set_lego_color(selected_color);
			lego[lego_num].set_lego_type(selected_type);
			lego[lego_num].set_lego_rotate(rota_x, rota_y, rota_z);
			lego[lego_num].set_lego_location(tran_x, tran_y, tran_z);
		}
		fclose(out);
		break; // load model
	case 6: //load  
		printf("Load model has selected\n");
		fopen_s(&out, "file6.txt", "r");
		while (!feof(out)) {
			fscanf_s(out, "%d, %d, %d, %f, %f, %f, %f, %f, %f \n",
				&lego_num, &selected_type, &selected_color,
				&tran_x, &tran_y, &tran_z, &rota_x, &rota_y, &rota_z);
			lego[lego_num].set_lego_color(selected_color);
			lego[lego_num].set_lego_type(selected_type);
			lego[lego_num].set_lego_rotate(rota_x, rota_y, rota_z);
			lego[lego_num].set_lego_location(tran_x, tran_y, tran_z);
		}
		fclose(out);
		break; // load model
	case 7:
		fopen_s(&out, "file7.txt", "r");
		while (!feof(out)) {
			fscanf_s(out, "%d, %d, %d, %f, %f, %f, %f, %f, %f \n",
				&lego_num, &selected_type, &selected_color,
				&tran_x, &tran_y, &tran_z, &rota_x, &rota_y, &rota_z);
			all_obj[lego_num].set_lego_color(selected_color);
			all_obj[lego_num].set_lego_type(selected_type);
			all_obj[lego_num].set_lego_rotate(rota_x, rota_y, rota_z);
			all_obj[lego_num].set_lego_location(tran_x, tran_y, tran_z);
		}
		fclose(out);
		break;
	}
	glutPostRedisplay();
}

void main_menu_function(int option) {
	switch (option) {
	case 1:
		exit(0);  // 프로그램 종료
		break;
	case 2: //Zoo mode //clear 되고, 시점 바뀌면서, picking가능하게 해야함
		setDefault();
		preview_f = false;
		glutPostRedisplay();
		PlaySound(TEXT("audio/farm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
		glutChangeToMenuEntry(2, "Making", 4); //2번째 메뉴가 option 4의 making이 된다
		glutAddMenuEntry("battle", 100); //2번째 메뉴가 option 4의 making이 된다
		glutSetMenu(menu);
		break;
	case 4: //making  mode
		setDefault();
		preview_f = true;
		glutPostRedisplay();
		PlaySound(TEXT("audio/making.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
		glutChangeToMenuEntry(2, "zoo", 2);
		glutRemoveMenuItem(5);
		glutSetMenu(menu);
		break;
	case 100: //배틀시작 하면, 랜덤숫자 두개 정해주고, 그에맞는 obj띄워주기
		goflag = false; //만약 움직이고 있었따면 멈추
		MessageBox(NULL, TEXT(" 다른 WIndow 창을 봐주세요"), TEXT("때릴 obj가 선정 되었습니다!"), MB_OK);
		start = clock();	// 싸웠을 떄 의 시각입력 start에.

		PlaySound(TEXT("audio/battle.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

		srand(time(NULL));
		int num, random;
		num = rand();
		random = num % 6 + 1;
		printf(" 1~6 에서 내가 얻은 랜덤 수는? %d \n", random);
		readFile(random); //이 랜덤에 해당하는 obj를 불러줍니다.

		fighting_flag = true;

		//do_battle(); ??
		break;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	/* 인자들을 해석해서 원하는 기능을 구현 */
	switch (key) {
	case '1':
		if (lego_type > 15) lego_type = 0;
		lego_type++;
		printf("type is  %d\n", lego_type);
		break;
	case 32:  // space bar //obj움직임
		if (goflag == false) {
			radius = 60;
			goflag = true;
		}
		else {
			goflag = false;
		}
		break;
		break;
	case 13:  // enter key //프리뷰의 레고를 메인뷰로 가져온다
		selected_color = color;
		selected_type = lego_type;
		// miniView에 관련된 항목들 초기화
		color = 0; lego_type = 0;
	
		break;
	case '3': //현재 레고상태를 홀드(저장하고) 다음에 잃어올 
		lego_num += 1;
		printf("lego num = %d\n", lego_num);
		selected_type = 0; selected_color = 0;
		tran_x = 0; tran_y = 0; tran_z = 0; //=trans_x,y,z...
		rota_y = 0;
		break;
	case '2':
		if (color > 10) color = 0;
		else color++;
		break;
	case 8:  // backspace
			 /* Undo, erase the latest fixed lego */
		if (lego_num >= 1) {
			redo = lego[lego_num-1];  // redo에 지금 그려놓은 레고 복사
			lego[lego_num].LEGO_init(); // 마지막 레고(lego_num번째 레고) 내용 비워줌
			lego_num--; //legonum 줄여줘
		}
		else printf("caution: No object is fixed\n");
		break;
	case 'z':
		/* re-do. refix the erased lego */
			lego[lego_num] = redo;
			lego_num += 1;

			printf("lego num = %d\n", lego_num);
			selected_type = 0; selected_color = 0;
			tran_x = 0; tran_y = 0; tran_z = 0; //그려줘료 fiz

			redo.LEGO_init(); //redo사용해으니 비워줘요
		break;	
	case'x':
		printf(" 배틀 누르고 얼마나지났냐 ? %lf \n", get_time());
		break;		
	case'o':
		goDirection += 1;
		break;
	case 'p':
		goDirection -= 1;
		break;
	case'j':// 순서대로 좌우상하
		tran_z += 0.1;
		break;
	case'l':
		tran_z -= 0.1;
		break;
	case'i':
		tran_x -= 0.1;
		break;
	case'k':
		tran_x += 0.1;
		break;
	case'0':
		speed_ += 0.5;
		if (speed_ > 3) {
			speed_ = 1;
		}
	}
	glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y) {
	int mo = glutGetModifiers(); //shift누르고 있으면 확인 가능
	
	switch (key) {
	case GLUT_KEY_UP:
		tran_y += 0.1;
		break;
	case GLUT_KEY_DOWN:
		tran_y -= 0.1;
		break;
	case GLUT_KEY_RIGHT:
		rota_y += 5;
		break;
	case GLUT_KEY_LEFT:
		rota_y -= 5;
		break;
	case GLUT_KEY_F1: //f1누르면 화면초기화
		setDefault();
		break;
	}
	glutPostRedisplay();
}

void printInstruction() {
	// Interface Introduction
	printf("--------------------------------------------------------------------------\n");
	printf("[ 12163291 강현지 -- 작고 하찮고 소중한 나의 동물농장,.,,... ]\n\n");
	printf("[ 프리뷰에서 ] \n1: 로 레고를 고를 수 있고, \n2: 로 레고색을 정한다음 \n");
	printf("ENTER: 로 mainVIew에 가져옵니다. \n\n");
	printf("j: 좌 l: 우 i: 상 k: 하 UP키보드: 위 DOWN키보드: 아래\n");
	printf("회전: y축으로만 회전하고, SHIFT를 누름과 동시에 좌,우 키보드로 조절 가능\n\n");
	printf("BACK SPACE: undo , z: redo 입니다\n\n");
	printf("F2는 초기화 함수입니다. 이는 Making에서 사용합니다.\n\n");
	printf("Zoo 모드에서 '스페이스바'을 누르면, 움직이게 되고 obj는 \n");
	printf("O: 왼쪽, P:오른쪽 으로 움직입니다. \n\n");
	printf("--------------------------------------------------------------------------\n");
}

void light_default() {
	glClearColor(0, 0, 0, 1.0f);

	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.3f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specularLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

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

void setDefault() {
	// initialize camera position(= view)
	theta = 90; phi = 45;
	radius = 35;
	upVector = 1;
	rota_x, rota_y, rota_z = 0;
	tran_x, tran_y, tran_z = 0;
	goDirection = 0;
	go_x, go_z = 0;

	for (int i = 0; i < 1000; i++) {
		lego[i].LEGO_init(); //레고 모든 내용 비워줌
	}
	for (int i = 0; i < 1000; i++) {
		battle_obj[i].LEGO_init(); //레고 모든 내용 비워줌
	}
	for (int i = 0; i < 1000; i++) {
		all_obj[i].LEGO_init(); //레고 모든 내용 비워줌
	}
}

void loadTexture() {
	glGenTextures(1, battle);
	uchar* img;
	int width, height, channels;

	glBindTexture(GL_TEXTURE_2D, *battle);
	img = readImageData("BG/battleBG.bmp", &width, &height, &channels);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void drawBattle() {
	int x = WIDTH / 40.0; //WIDTH //나누기 숫자 작을수록 텍스처 커짐
	int y = HEIGHT / 40.0; //HEIGHT

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex3f(-x, -y, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(x, -y, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(x, y, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-x, y, 0.0);
	glEnd();
}

void readFile(int x) { //x 따라 몇번째 파일을 불러와서 그리는지 결정됨
	switch (x)
	{
	case 1:
		fopen_s(&out, "file1.txt", "r");
		while (!feof(out)) {
			fscanf_s(out, "%d, %d, %d, %f, %f, %f, %f, %f, %f \n",
				&lego_num, &selected_type, &selected_color,
				&tran_x, &tran_y, &tran_z, &rota_x, &rota_y, &rota_z);
			battle_obj[lego_num].set_lego_color(selected_color);
			battle_obj[lego_num].set_lego_type(selected_type);
			battle_obj[lego_num].set_lego_rotate(rota_x, rota_y, rota_z);
			battle_obj[lego_num].set_lego_location(tran_x, tran_y, tran_z);
		}
		fclose(out);
		break;
	case 2:
		fopen_s(&out, "file2.txt", "r");
		while (!feof(out)) {
			fscanf_s(out, "%d, %d, %d, %f, %f, %f, %f, %f, %f \n",
				&lego_num, &selected_type, &selected_color,
				&tran_x, &tran_y, &tran_z, &rota_x, &rota_y, &rota_z);
			battle_obj[lego_num].set_lego_color(selected_color);
			battle_obj[lego_num].set_lego_type(selected_type);
			battle_obj[lego_num].set_lego_rotate(rota_x, rota_y, rota_z);
			battle_obj[lego_num].set_lego_location(tran_x, tran_y, tran_z);
		}
		fclose(out);
		break;
	case 3:
		fopen_s(&out, "file3.txt", "r");
		while (!feof(out)) {
			fscanf_s(out, "%d, %d, %d, %f, %f, %f, %f, %f, %f \n",
				&lego_num, &selected_type, &selected_color,
				&tran_x, &tran_y, &tran_z, &rota_x, &rota_y, &rota_z);
			battle_obj[lego_num].set_lego_color(selected_color);
			battle_obj[lego_num].set_lego_type(selected_type);
			battle_obj[lego_num].set_lego_rotate(rota_x, rota_y, rota_z);
			battle_obj[lego_num].set_lego_location(tran_x, tran_y, tran_z);
		}
		fclose(out);
		break;
	case 4:
		fopen_s(&out, "file4.txt", "r");
		while (!feof(out)) {
			fscanf_s(out, "%d, %d, %d, %f, %f, %f, %f, %f, %f \n",
				&lego_num, &selected_type, &selected_color,
				&tran_x, &tran_y, &tran_z, &rota_x, &rota_y, &rota_z);
			battle_obj[lego_num].set_lego_color(selected_color);
			battle_obj[lego_num].set_lego_type(selected_type);
			battle_obj[lego_num].set_lego_rotate(rota_x, rota_y, rota_z);
			battle_obj[lego_num].set_lego_location(tran_x, tran_y, tran_z);
		}
		fclose(out);
		break;
	case 5:
		fopen_s(&out, "file5.txt", "r");
		while (!feof(out)) {
			fscanf_s(out, "%d, %d, %d, %f, %f, %f, %f, %f, %f \n",
				&lego_num, &selected_type, &selected_color,
				&tran_x, &tran_y, &tran_z, &rota_x, &rota_y, &rota_z);
			battle_obj[lego_num].set_lego_color(selected_color);
			battle_obj[lego_num].set_lego_type(selected_type);
			battle_obj[lego_num].set_lego_rotate(rota_x, rota_y, rota_z);
			battle_obj[lego_num].set_lego_location(tran_x, tran_y, tran_z);
		}
		fclose(out);
		break;
	case 6:
		fopen_s(&out, "file6.txt", "r");
		while (!feof(out)) {
			fscanf_s(out, "%d, %d, %d, %f, %f, %f, %f, %f, %f \n",
				&lego_num, &selected_type, &selected_color,
				&tran_x, &tran_y, &tran_z, &rota_x, &rota_y, &rota_z);
			battle_obj[lego_num].set_lego_color(selected_color);
			battle_obj[lego_num].set_lego_type(selected_type);
			battle_obj[lego_num].set_lego_rotate(rota_x, rota_y, rota_z);
			battle_obj[lego_num].set_lego_location(tran_x, tran_y, tran_z);
		}
		fclose(out);
		break;
	}
	glutPostRedisplay();
}

void keyboard_battle(unsigned char key, int x, int y) {
	switch (key) {
	case'e':
		newExplosion();
		break;
	case 32:  // space bar /obj흔들리고, 9번뒤에는 터져야함 //그리고 끝나야함
		start_ = clock();
		int done = 0;
	
		spacePushed++;
		if (spacePushed == 9) {
			newExplosion();
		}
		else if (spacePushed == 10) {
				for (int i = 0; i <= 1000; i++) { //레고 그림
					battle_obj[i].LEGO_init();
					//battle 마치고, battle_obj.LEGO_init 해야합니당!!!
				}
			MessageBox(NULL, TEXT(" 원 WIndow 창으로 돌아가 주세요"), TEXT("부수셨어요!"), MB_OK);
			PlaySound(TEXT("audio/farm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
			spacePushed = 0;
		}

		break;
	}
	glutPostRedisplay();
}

void do_battle() {
	
}