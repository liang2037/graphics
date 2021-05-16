/*
 *        Computer Graphics Course - Shenzhen University
 *    Mid-term Assignment - Tetris implementation sample code
 * ============================================================
 *
 * - ����������ǲο����룬����Ҫ����ο���ҵ˵��������˳������ɡ�
 * - ��������OpenGL�����������������У���ο���һ��ʵ��γ�����ĵ���
 *
 * - ��ʵ�ֹ������£�
 * - 1) �������̸�͡�L���ͷ���
 * - 2) ������/��/�¼����Ʒ�����ƶ����ϼ���ת����
 *
 * - δʵ�ֹ������£�
 * - 1) ������ɷ��鲢���ϲ�ͬ����ɫ
 * - 2) ������Զ������ƶ�
 * - 3) ����֮�����ײ���
 * - 4) ���̸���ÿһ�������֮���Զ�����
 * - 5) ����
 *
 */

#include "include/Angel.h"

#pragma comment(lib, "glew32.lib")
#include <ctime>
#include <fstream>
#include <string>
#include<stdio.h>
#include <cstdlib>
#include <iostream>

using namespace std;

GLuint program;
GLuint vPosition;
GLuint vColor;

int starttime;			// ���Ʒ��������ƶ�ʱ��
int rotation = 0;		// ���Ƶ�ǰ�����еķ�����ת
vec2 tile[4];			// ��ʾ��ǰ�����еķ���
bool gameover = false;	// ��Ϸ�������Ʊ���
int xsize = 700;		// ���ڴ�С��������Ҫ�䶯���ڴ�С����
int ysize = 800;

int z;					//��ʾ���ڷ���������
int col;

vec2 ne_tile[4];			// ��ʾ��һ������
int ne_z;					// ��һ������������
int ne_col;

//�������
char gameover_t[50] = "";
char Score_t[50] = "0";
int Score = 0;
char diff_t[50] = "1";
int diff = 1;


// һ����ά�����ʾ���п��ܳ��ֵķ���ͷ���
vec2 allRotationsLshape[7][4][4] =
{ { { vec2(0, 0), vec2(-1,0), vec2(1, 0), vec2(-1,-1) },	// "L"
{ vec2(0, 1), vec2(0, 0), vec2(0,-1), vec2(1, -1) },
{ vec2(1, 1), vec2(-1,0), vec2(0, 0), vec2(1,  0) },
{ vec2(-1,1), vec2(0, 1), vec2(0, 0), vec2(0, -1) } },

{ { vec2(0, 0), vec2(-1, -1), vec2(-1, 0), vec2(0, -1) },	//"O"
{ vec2(0, 0), vec2(-1, -1), vec2(-1, 0), vec2(0, -1) },
{ vec2(0, 0), vec2(-1, -1), vec2(-1, 0), vec2(0, -1) },
{ vec2(0, 0), vec2(-1, -1), vec2(-1, 0), vec2(0, -1) } },

{ { vec2(0, 0), vec2(1, 0), vec2(-1, 0), vec2(-2, 0) },	//"I"
{ vec2(0, 0), vec2(0, -2), vec2(0, -1), vec2(0, 1) },
{ vec2(0, 0), vec2(1, 0), vec2(-1, 0), vec2(-2, 0) },
{ vec2(0, 0), vec2(0, -2), vec2(0, -1), vec2(0, 1) } },

{ { vec2(0, 0), vec2(1, 0), vec2(-1, -1), vec2(0, -1) },	//"S"
{ vec2(0, 0), vec2(0, 1), vec2(1,0), vec2(1, -1) },
{ vec2(0, 0), vec2(1, 0), vec2(-1, -1), vec2(0, -1) },
{ vec2(0, 0), vec2(0, 1), vec2(1, 0), vec2(1, -1) } },

{ { vec2(0, 0), vec2(-1, 0), vec2(1, 0), vec2(1, -1) },	//"J"
{ vec2(0, 0), vec2(0, 1), vec2(0, -1), vec2(1, 1) },
{ vec2(0, 0), vec2(1,0), vec2(-1, 0), vec2(-1, 1) },
{ vec2(0, 0), vec2(0, 1), vec2(0, -1), vec2(-1, -1) } },

{ { vec2(0, 0), vec2(-1, 0), vec2(0, -1), vec2(1, -1) },	//"Z"
{ vec2(0, -1), vec2(0, 0), vec2(1, 0), vec2(1, 1) },
{ vec2(0, 0), vec2(-1, 0), vec2(0, -1), vec2(1, -1) },
{ vec2(0, -1), vec2(0, 0), vec2(1, 0), vec2(1, 1) } },

{ { vec2(0, 0), vec2(1, 0), vec2(-1, 0), vec2(0, -1) },	//"T"
{ vec2(0, 0), vec2(0, 1), vec2(0, -1), vec2(1, 0) },
{ vec2(0, 0), vec2(0, 1), vec2(-1, 0), vec2(1, 0) },
{ vec2(-1, 0), vec2(0, 1), vec2(0, -1), vec2(0, 0) } }
};


// ���ƴ��ڵ���ɫ����
vec4 orange = vec4(1.0, 0.5, 0.0, 1.0);
vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
vec4 tilecolor[7] = {
	vec4(1.0, 0.5, 0.0,1.0),
	vec4(1.0, 0.0, 0.0,1.0),
	vec4(0.0, 1.0, 0.0,1.0),
	vec4(0.0, 0.0, 1.0,1.0),
	vec4(1.0, 1.0, 0.0,1.0),
	vec4(1.0, 0.0, 1.0,1.0),
	vec4(0.0, 1.0, 1.0,1.0)
};

// ��ǰ�����λ�ã������̸�����½�Ϊԭ�������ϵ��
vec2 tilepos = vec2(5, 19);

vec2 ne_tilepos = vec2(12.5,17);

// ���������ʾ���̸��ĳλ���Ƿ񱻷�����䣬��board[x][y] = true��ʾ(x,y)�����ӱ���䡣
// �������̸�����½�Ϊԭ�������ϵ��
bool board[10][20];


// �����̸�ĳЩλ�ñ��������֮�󣬼�¼��Щλ���ϱ�������ɫ
vec4 boardcolours[1200];

GLuint locxsize;
GLuint locysize;

GLuint vaoIDs[3];
GLuint vboIDs[6];

//��һ�������vao��vbo
GLuint ne_vaoIDs[1];
GLuint ne_vboIDs[2];

//�����ߵ�vao��vbo
GLuint nexk_vaoIDs[1];
GLuint nexk_vboIDs[2];



//////////////////////////////////////////////////////////////////////////
// �޸����̸���posλ�õ���ɫΪcolour�����Ҹ��¶�Ӧ��VBO

void changecellcolour(vec2 pos, vec4 colour)
{
	// ÿ�������Ǹ������Σ��������������Σ��ܹ�6�����㣬�����ض���λ�ø����ʵ�����ɫ
	for (int i = 0; i < 6; i++)
		boardcolours[(int)(6 * (10 * pos.y + pos.x) + i)] = colour;

	vec4 newcolours[6] = { colour, colour, colour, colour, colour, colour };

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);

	// ����ƫ���������ʵ���λ�ø�����ɫ
	int offset = 6 * sizeof(vec4) * (int)(10 * pos.y + pos.x);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(newcolours), newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//////////////////////////////////////////////////////////////////////////
// ��ǰ�����ƶ�������תʱ������VBO

void updatetile()
{
	
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);

	// ÿ����������ĸ�����
	for (int i = 0; i < 4; i++)
	{
		// ������ӵ�����ֵ
		GLfloat x = tilepos.x + tile[i].x;
		GLfloat y = tilepos.y + tile[i].y;

		vec4 p1 = vec4(33.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
		vec4 p2 = vec4(33.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);
		vec4 p3 = vec4(66.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
		vec4 p4 = vec4(66.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);

		
		// ÿ�����Ӱ������������Σ�������6����������
		vec4 newpoints[6] = { p1, p2, p3, p2, p3, p4 };
		glBufferSubData(GL_ARRAY_BUFFER, i * 6 * sizeof(vec4), 6 * sizeof(vec4), newpoints);
	}

	
	glBindVertexArray(0);
}

void ne_updatetile()
{
	vec4 gridpoints[32];
	vec4 gridcolours[32];
	glBindBuffer(GL_ARRAY_BUFFER, ne_vboIDs[0]);

	// ÿ����������ĸ�����
	for (int i = 0; i < 4; i++)
	{
		// ������ӵ�����ֵ
		GLfloat x = ne_tilepos.x + ne_tile[i].x;
		GLfloat y = ne_tilepos.y + ne_tile[i].y;

		vec4 p1 = vec4(33.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
		vec4 p2 = vec4(33.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);
		vec4 p3 = vec4(66.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
		vec4 p4 = vec4(66.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);
		gridpoints[i * 8 + 0] = p1;
		gridpoints[i * 8 + 1] = p2;
		gridpoints[i * 8 + 2] = p2;
		gridpoints[i * 8 + 3] = p4;
		gridpoints[i * 8 + 4] = p4;
		gridpoints[i * 8 + 5] = p3;
		gridpoints[i * 8 + 6] = p3;
		gridpoints[i * 8 + 7] = p1;
		for (int j = 0; j < 8; j++)
		{
			gridcolours[i * 8 + j] = white;
			gridpoints[i * 8 + j][2] = 0;
		}

		// ÿ�����Ӱ������������Σ�������6����������
		vec4 newpoints[6] = { p1, p2, p3, p2, p3, p4 };
		glBufferSubData(GL_ARRAY_BUFFER, i * 6 * sizeof(vec4), 6 * sizeof(vec4), newpoints);
	}
	glGenVertexArrays(1, &nexk_vaoIDs[0]);
	glBindVertexArray(nexk_vaoIDs[0]);
	glGenBuffers(2, nexk_vboIDs);
	// ���̸񶥵�λ��
	glBindBuffer(GL_ARRAY_BUFFER, nexk_vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(vec4), gridpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// ���̸񶥵���ɫ
	glBindBuffer(GL_ARRAY_BUFFER, nexk_vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(vec4), gridcolours, GL_STATIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);
	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////
// ���õ�ǰ����Ϊ��һ���������ֵķ��顣����Ϸ��ʼ��ʱ�����������һ����ʼ�ķ��飬
// ����Ϸ������ʱ���жϣ�û���㹻�Ŀռ��������µķ��顣
bool fir = true;

void newtile()
{
	if (!fir)
	{
		int flag = 1;
		int nn = 0;
		for (int i = 0; i < 4; i++)
		{
			int x = (int)allRotationsLshape[ne_z][0][i].x + 5;
			int y = (int)allRotationsLshape[ne_z][0][i].y + 19;
			if ((y == 18 && board[x][y + 1]) || (ne_z == 2 && board[x][y]))
			{
				gameover = true;
				strcpy_s(gameover_t, "GAME OVER");
				glutSwapBuffers();
				return;
			}
			if (y == 18 && board[x][y] == true && board[x][y + 1] == false)
			{
				flag = 0;
			}
		}
		//ʵ�������20�л��ղ�����һ���ɵ��䣬������һ��������һ��
		if (flag == 0)
		{
			for (int i = 3;i < 7; i++)
			{
				board[i][19] = true;
			}
			vec4 newcolours[24];
			for (int i = 0; i < 24; i++)
				newcolours[i] = tilecolor[ne_col];
			tilepos = vec2(5, 20);
			for (int i = 0; i < 4; i++)
			{
				int x = (int)allRotationsLshape[ne_z][0][i].x + 5;
				int y = (int)allRotationsLshape[ne_z][0][i].y + 20;
				tile[i] = allRotationsLshape[ne_z][0][i];
				if (y == 20)
				{
					for (int ii = 0; ii < 6; ii++)
					{
						newcolours[i * 6 + ii] = black;
					}
				}
			}
			updatetile();

			glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newcolours), newcolours);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			gameover = true;
			strcpy_s(gameover_t, "GAME OVER");
			glutSwapBuffers();
			return;
		}
	}
	
	if (fir)
	{
		fir = false;
		z = rand() % 7;
		ne_z = rand() % 7;
		col = rand() % 7;
		ne_col = rand() % 7;
	}
	else
	{
		z = ne_z;
		ne_z = rand() % 7;
		col = ne_col;
		ne_col = rand() % 7;
	}

	// ���·���������̸���������м�λ�ò�����Ĭ�ϵ���ת����
	tilepos = vec2(5, 19);
	rotation = 0;
	//�·�������
	for (int i = 0; i < 4; i++)
	{
		tile[i] = allRotationsLshape[z][0][i];
	}
	updatetile();
	// ���·��鸳����ɫ
	vec4 newcolours[24];
	for (int i = 0; i < 24; i++)
		newcolours[i] = tilecolor[col];

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newcolours), newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//��һ��������
	for (int i = 0; i < 4; i++)
	{
		ne_tile[i] = allRotationsLshape[ne_z][0][i];
	}
	ne_updatetile();
	// ����һ���鸳����ɫ
	vec4 ne_newcolours[24];
	for (int i = 0; i < 24; i++)
		ne_newcolours[i] = tilecolor[ne_col];

	glBindBuffer(GL_ARRAY_BUFFER, ne_vboIDs[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ne_newcolours), ne_newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

//////////////////////////////////////////////////////////////////////////
// ��Ϸ��OpenGL��ʼ��

void init()
{
	// ��ʼ�����̸񣬰���64���������꣨�ܹ�32���ߣ�������ÿ������һ����ɫֵ
	vec4 gridpoints[64];
	vec4 gridcolours[64];

	// ������
	for (int i = 0; i < 11; i++)
	{
		gridpoints[2 * i] = vec4((33.0 + (33.0 * i)), 33.0, 0, 1);
		gridpoints[2 * i + 1] = vec4((33.0 + (33.0 * i)), 693.0, 0, 1);

	}

	// ˮƽ��
	for (int i = 0; i < 21; i++)
	{
		gridpoints[22 + 2 * i] = vec4(33.0, (33.0 + (33.0 * i)), 0, 1);
		gridpoints[22 + 2 * i + 1] = vec4(363.0, (33.0 + (33.0 * i)), 0, 1);
	}

	// �������߸��ɰ�ɫ
	for (int i = 0; i < 64; i++)
		gridcolours[i] = white;

	// ��ʼ�����̸񣬲���û�б����ĸ������óɺ�ɫ
	vec4 boardpoints[1200];
	for (int i = 0; i < 1200; i++)
		boardcolours[i] = black;

	// ��ÿ�����ӣ���ʼ��6�����㣬��ʾ���������Σ�����һ�������θ���
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 10; j++)
		{
			vec4 p1 = vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), .5, 1);
			vec4 p2 = vec4(33.0 + (j * 33.0), 66.0 + (i * 33.0), .5, 1);
			vec4 p3 = vec4(66.0 + (j * 33.0), 33.0 + (i * 33.0), .5, 1);
			vec4 p4 = vec4(66.0 + (j * 33.0), 66.0 + (i * 33.0), .5, 1);

			boardpoints[6 * (10 * i + j)] = p1;
			boardpoints[6 * (10 * i + j) + 1] = p2;
			boardpoints[6 * (10 * i + j) + 2] = p3;
			boardpoints[6 * (10 * i + j) + 3] = p2;
			boardpoints[6 * (10 * i + j) + 4] = p3;
			boardpoints[6 * (10 * i + j) + 5] = p4;
		}

	// �����̸�����λ�õ�����������Ϊfalse��û�б���䣩
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 20; j++)
			board[i][j] = false;

	// ������ɫ��
	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	locxsize = glGetUniformLocation(program, "xsize");
	locysize = glGetUniformLocation(program, "ysize");

	vPosition = glGetAttribLocation(program, "vPosition");
	vColor = glGetAttribLocation(program, "vColor");

	glGenVertexArrays(3, &vaoIDs[0]);

	// ���̸񶥵�
	glBindVertexArray(vaoIDs[0]);
	glGenBuffers(2, vboIDs);

	// ���̸񶥵�λ��
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 64 * sizeof(vec4), gridpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// ���̸񶥵���ɫ
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 64 * sizeof(vec4), gridcolours, GL_STATIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	// ���̸�ÿ������
	glBindVertexArray(vaoIDs[1]);
	glGenBuffers(2, &vboIDs[2]);

	// ���̸�ÿ�����Ӷ���λ��
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, 1200 * sizeof(vec4), boardpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// ���̸�ÿ�����Ӷ�����ɫ
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200 * sizeof(vec4), boardcolours, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	

	// ��ǰ����
	glBindVertexArray(vaoIDs[2]);
	glGenBuffers(2, &vboIDs[4]);

	// ��ǰ���鶥��λ��
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// ��ǰ���鶥����ɫ
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);


	// ��һ������
	glBindVertexArray(ne_vaoIDs[0]);
	glGenBuffers(2, &ne_vboIDs[0]);

	// ��ǰ���鶥��λ��
	glBindBuffer(GL_ARRAY_BUFFER, ne_vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// ��ǰ���鶥����ɫ
	glBindBuffer(GL_ARRAY_BUFFER, ne_vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	glBindVertexArray(0);

	glClearColor(0, 0, 0, 0);

	// ��Ϸ��ʼ��
	newtile();
	starttime = glutGet(GLUT_ELAPSED_TIME);
}



//////////////////////////////////////////////////////////////////////////
// �����cellposλ�õĸ����Ƿ��������Ƿ������̸�ı߽緶Χ�ڡ�

bool checkvalid(vec2 cellpos)
{
	if ((cellpos.x >= 0) && (cellpos.x < 10) && (cellpos.y >= 0) && (cellpos.y < 20))
	{
		if (board[(int)cellpos.x][(int)cellpos.y])
			return false;
		else
			return true;
	}
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////
// �����������㹻�ռ���������ת��ǰ����

void rotate()
{
	// ����õ���һ����ת����
	int nextrotation = (rotation + 1) % 4;

	// ��鵱ǰ��ת֮���λ�õ���Ч��
	if (checkvalid((allRotationsLshape[z][nextrotation][0]) + tilepos)
		&& checkvalid((allRotationsLshape[z][nextrotation][1]) + tilepos)
		&& checkvalid((allRotationsLshape[z][nextrotation][2]) + tilepos)
		&& checkvalid((allRotationsLshape[z][nextrotation][3]) + tilepos))
	{
		// ������ת������ǰ��������Ϊ��ת֮��ķ���
		rotation = nextrotation;
		for (int i = 0; i < 4; i++)
			tile[i] = allRotationsLshape[z][rotation][i];

		updatetile();
	}
}

//////////////////////////////////////////////////////////////////////////
// ������̸���row����û�б������
int SCORE[4] = { 5,15,30,50 };

bool checkfullrow(int row)
{
	int flag = 0;
	for (int i = 0; i < 10; i++)
	{
		if (board[i][row] == false)
		{
			flag = 1;
			break;
		}
	}
	if (flag == 1)
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ���õ�ǰ���飬���Ҹ������̸��Ӧλ�ö������ɫVBO


void settile()
{
	// ÿ������
	for (int i = 0; i < 4; i++)
	{
		// ��ȡ���������̸��ϵ�����
		int x = (tile[i] + tilepos).x;
		int y = (tile[i] + tilepos).y;
		// �����Ӷ�Ӧ�����̸��ϵ�λ������Ϊ���
		board[x][y] = true;
		// ������Ӧλ�õ���ɫ�޸�
		changecellcolour(vec2(x, y), tilecolor[col]);
	}

	//����������÷�
	int num = 0;
	for (int r = 0; r < 20 && num < 4; r++)
	{
		if (checkfullrow(r))
		{
			for (int i = r; i < 19; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					board[j][i] = board[j][i + 1];
					changecellcolour(vec2(j, i), boardcolours[6 * (10 * (i + 1) + j)]);
				}
			}
			for (int j = 0; j < 10; j++)
			{
				board[j][19] = false;
				changecellcolour(vec2(j, 19), black);
			}
			num++;
			r--;
		}
	}
	if (num > 0)
	{
		Score += SCORE[num - 1];
		sprintf_s(Score_t, "%d", Score);
		glutSwapBuffers();
	}
}

//////////////////////////////////////////////////////////////////////////
// ����λ��(x,y)���ƶ����顣��Ч���ƶ�ֵΪ(-1,0)��(1,0)��(0,-1)���ֱ��Ӧ����
// �����º������ƶ�������ƶ��ɹ�������ֵΪtrue����֮Ϊfalse��

bool movetile(vec2 direction)
{
	// �����ƶ�֮��ķ����λ������
	vec2 newtilepos[4];
	for (int i = 0; i < 4; i++)
		newtilepos[i] = tile[i] + tilepos + direction;

	// ����ƶ�֮�����Ч��
	if (checkvalid(newtilepos[0])
		&& checkvalid(newtilepos[1])
		&& checkvalid(newtilepos[2])
		&& checkvalid(newtilepos[3]))
	{
		// ��Ч���ƶ��÷���
		tilepos.x = tilepos.x + direction.x;
		tilepos.y = tilepos.y + direction.y;

		updatetile();

		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
// ����������Ϸ

void restart()
{
	srand((unsigned)time(0));
	fir = true;
	gameover = false;
	Score = 0;
	sprintf_s(Score_t, "%d", Score);
	strcpy_s(gameover_t, "");
	glutSwapBuffers();
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			board[i][j] = false;
			changecellcolour(vec2(i, j), black);
		}
	}
	newtile();
}

//////////////////////////////////////////////////////////////////////////
// ��Ϸ��Ⱦ����

void sc_Text(vec2 vz, vec3 ccol, const char* str)
{
	//������Ⱦģʽ
	glUseProgram(0);
	glColor3f(ccol[0], ccol[1], ccol[2]);
	glRasterPos2f(vz[0], vz[1]);
	int Len = strlen(str);
	for (int i = 0; i < Len; i++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
	}
	glFlush();
	glUseProgram(program);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUniform1i(locxsize, xsize);
	glUniform1i(locysize, ysize);

	glBindVertexArray(vaoIDs[1]);
	glDrawArrays(GL_TRIANGLES, 0, 1200); // �������̸� (10*20*2 = 400 ��������)

	glBindVertexArray(vaoIDs[2]);
	glDrawArrays(GL_TRIANGLES, 0, 24);	 // ���Ƶ�ǰ���� (8 ��������)

	glBindVertexArray(ne_vaoIDs[0]);
	glDrawArrays(GL_TRIANGLES, 0, 24);	 // ���Ƶ�ǰ���� (8 ��������)

	glBindVertexArray(vaoIDs[0]);
	glDrawArrays(GL_LINES, 0, 64);		 // �������̸����

	glBindVertexArray(nexk_vaoIDs[0]);
	glDrawArrays(GL_LINES, 0, 32);		 // �������̸����

	sc_Text(vec2(0.62, 0.7), vec3(1.0, 1.0, 1.0), "Next:");
	sc_Text(vec2(0.62, 0.1), vec3(1.0, 1.0, 1.0), "Score:");
	sc_Text(vec2(0.58, -0.3), vec3(1.0, 1.0, 1.0), "Difficulty:");
	sc_Text(vec2(-0.92, 0.23), vec3(1.0, 1.0, 1.0), "Operation:");
	sc_Text(vec2(-0.96, 0.1), vec3(1.0, 1.0, 1.0), "'w' Difficulty up");
	sc_Text(vec2(-1, 0), vec3(1.0, 1.0, 1.0), "'s' Difficulty down");
	sc_Text(vec2(-0.92, -0.1), vec3(1.0, 1.0, 1.0), "'q' sign out");
	sc_Text(vec2(-0.90, -0.2), vec3(1.0, 1.0, 1.0), "'r' restart");
	sc_Text(vec2(-0.90, -0.3), vec3(1.0, 1.0, 1.0), "'x' suspend");
	sc_Text(vec2(-0.90, -0.4), vec3(1.0, 1.0, 1.0), "'z' continue");

	sc_Text(vec2(0.69, 0.0), vec3(0.3, 1.0, 1.0), Score_t);
	sc_Text(vec2(0.69, -0.4), vec3(0.9, 0.5,0.5), diff_t);
	sc_Text(vec2(-0.1,0.89), vec3(1.0, 0.3, 0.3), gameover_t);

	glutSwapBuffers();
}

//////////////////////////////////////////////////////////////////////////
// �ڴ��ڱ������ʱ�򣬿������̸�Ĵ�С��ʹ֮���̶ֹ��ı�����

void reshape(GLsizei w, GLsizei h)
{
	xsize = w;
	ysize = h;
	glViewport(0, 0, w, h);
}

//////////////////////////////////////////////////////////////////////////
// ������Ӧ�¼��е����ⰴ����Ӧ

void special(int key, int x, int y)
{
	if (!gameover)
	{
		switch (key)
		{
		case GLUT_KEY_UP:	// ���ϰ�����ת����
			rotate();
			break;
		case GLUT_KEY_DOWN: // ���°����ƶ�����
			if (!movetile(vec2(0, -1)))
			{
				settile();
				newtile();
			}
			break;
		case GLUT_KEY_LEFT:  // ���󰴼��ƶ�����
			movetile(vec2(-1, 0));
			break;
		case GLUT_KEY_RIGHT: // ���Ұ����ƶ�����
			movetile(vec2(1, 0));
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// ������Ӧʱ���е���ͨ������Ӧ
int Sc = 2000;		//·��
int speed = 100;	//�ٶ�
int jtime = 0;		//����ʱ��

void idle(void)
{
	//ģ��ʵ���˶�������ʵ�ʵ����ٶ�
	if (jtime++ >= int(Sc / speed))
	{
		if (!movetile(vec2(0, -1)))
		{
			settile();
			newtile();
		}
		jtime = 0;
	}
	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		if (diff >= 25)
			break;
		speed += 20;
		diff++;
		sprintf_s(diff_t, "%d", diff);
		glutSwapBuffers();
		break;
	case 's':
		if (diff <= 1)
			break;
		diff--;
		sprintf_s(diff_t, "%d", diff);
		glutSwapBuffers();
		speed -= 20;
		break;
	case 'z':
		glutIdleFunc(idle);
		break;
	case 'x':
		glutIdleFunc(NULL);
		break;
	case 033: // ESC�� �� 'q' ���˳���Ϸ
		exit(EXIT_SUCCESS);
		break;
	case 'q':
		exit(EXIT_SUCCESS);
		break;
	case 'r': // 'r' ��������Ϸ
		restart();
		break;
	}
	glutPostRedisplay();
}


//////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(xsize, ysize);
	glutInitWindowPosition(680, 00);
	glutCreateWindow("����˹����_�����_2018152037");
	glewInit();
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}
