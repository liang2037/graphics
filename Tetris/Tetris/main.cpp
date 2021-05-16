/*
 *        Computer Graphics Course - Shenzhen University
 *    Mid-term Assignment - Tetris implementation sample code
 * ============================================================
 *
 * - 本代码仅仅是参考代码，具体要求请参考作业说明，按照顺序逐步完成。
 * - 关于配置OpenGL开发环境、编译运行，请参考第一周实验课程相关文档。
 *
 * - 已实现功能如下：
 * - 1) 绘制棋盘格和‘L’型方块
 * - 2) 键盘左/右/下键控制方块的移动，上键旋转方块
 *
 * - 未实现功能如下：
 * - 1) 随机生成方块并赋上不同的颜色
 * - 2) 方块的自动向下移动
 * - 3) 方块之间的碰撞检测
 * - 4) 棋盘格中每一行填充满之后自动消除
 * - 5) 其他
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

int starttime;			// 控制方块向下移动时间
int rotation = 0;		// 控制当前窗口中的方块旋转
vec2 tile[4];			// 表示当前窗口中的方块
bool gameover = false;	// 游戏结束控制变量
int xsize = 700;		// 窗口大小（尽量不要变动窗口大小！）
int ysize = 800;

int z;					//表示现在方块的随机数
int col;

vec2 ne_tile[4];			// 表示下一个方块
int ne_z;					// 下一个方块的随机数
int ne_col;

//输出内容
char gameover_t[50] = "";
char Score_t[50] = "0";
int Score = 0;
char diff_t[50] = "1";
int diff = 1;


// 一个二维数组表示所有可能出现的方块和方向。
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


// 绘制窗口的颜色变量
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

// 当前方块的位置（以棋盘格的左下角为原点的坐标系）
vec2 tilepos = vec2(5, 19);

vec2 ne_tilepos = vec2(12.5,17);

// 布尔数组表示棋盘格的某位置是否被方块填充，即board[x][y] = true表示(x,y)处格子被填充。
// （以棋盘格的左下角为原点的坐标系）
bool board[10][20];


// 当棋盘格某些位置被方块填充之后，记录这些位置上被填充的颜色
vec4 boardcolours[1200];

GLuint locxsize;
GLuint locysize;

GLuint vaoIDs[3];
GLuint vboIDs[6];

//下一个方块的vao，vbo
GLuint ne_vaoIDs[1];
GLuint ne_vboIDs[2];

//网格线的vao，vbo
GLuint nexk_vaoIDs[1];
GLuint nexk_vboIDs[2];



//////////////////////////////////////////////////////////////////////////
// 修改棋盘格在pos位置的颜色为colour，并且更新对应的VBO

void changecellcolour(vec2 pos, vec4 colour)
{
	// 每个格子是个正方形，包含两个三角形，总共6个定点，并在特定的位置赋上适当的颜色
	for (int i = 0; i < 6; i++)
		boardcolours[(int)(6 * (10 * pos.y + pos.x) + i)] = colour;

	vec4 newcolours[6] = { colour, colour, colour, colour, colour, colour };

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);

	// 计算偏移量，在适当的位置赋上颜色
	int offset = 6 * sizeof(vec4) * (int)(10 * pos.y + pos.x);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(newcolours), newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//////////////////////////////////////////////////////////////////////////
// 当前方块移动或者旋转时，更新VBO

void updatetile()
{
	
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);

	// 每个方块包含四个格子
	for (int i = 0; i < 4; i++)
	{
		// 计算格子的坐标值
		GLfloat x = tilepos.x + tile[i].x;
		GLfloat y = tilepos.y + tile[i].y;

		vec4 p1 = vec4(33.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
		vec4 p2 = vec4(33.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);
		vec4 p3 = vec4(66.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
		vec4 p4 = vec4(66.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);

		
		// 每个格子包含两个三角形，所以有6个顶点坐标
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

	// 每个方块包含四个格子
	for (int i = 0; i < 4; i++)
	{
		// 计算格子的坐标值
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

		// 每个格子包含两个三角形，所以有6个顶点坐标
		vec4 newpoints[6] = { p1, p2, p3, p2, p3, p4 };
		glBufferSubData(GL_ARRAY_BUFFER, i * 6 * sizeof(vec4), 6 * sizeof(vec4), newpoints);
	}
	glGenVertexArrays(1, &nexk_vaoIDs[0]);
	glBindVertexArray(nexk_vaoIDs[0]);
	glGenBuffers(2, nexk_vboIDs);
	// 棋盘格顶点位置
	glBindBuffer(GL_ARRAY_BUFFER, nexk_vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(vec4), gridpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// 棋盘格顶点颜色
	glBindBuffer(GL_ARRAY_BUFFER, nexk_vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(vec4), gridcolours, GL_STATIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);
	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////
// 设置当前方块为下一个即将出现的方块。在游戏开始的时候调用来创建一个初始的方块，
// 在游戏结束的时候判断，没有足够的空间来生成新的方块。
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
		//实现如果第20行还空并且下一个可掉落，掉落下一个的下面一行
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

	// 将新方块放于棋盘格的最上行中间位置并设置默认的旋转方向
	tilepos = vec2(5, 19);
	rotation = 0;
	//新方块生成
	for (int i = 0; i < 4; i++)
	{
		tile[i] = allRotationsLshape[z][0][i];
	}
	updatetile();
	// 给新方块赋上颜色
	vec4 newcolours[24];
	for (int i = 0; i < 24; i++)
		newcolours[i] = tilecolor[col];

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newcolours), newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//下一方块生成
	for (int i = 0; i < 4; i++)
	{
		ne_tile[i] = allRotationsLshape[ne_z][0][i];
	}
	ne_updatetile();
	// 给下一方块赋上颜色
	vec4 ne_newcolours[24];
	for (int i = 0; i < 24; i++)
		ne_newcolours[i] = tilecolor[ne_col];

	glBindBuffer(GL_ARRAY_BUFFER, ne_vboIDs[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ne_newcolours), ne_newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

//////////////////////////////////////////////////////////////////////////
// 游戏和OpenGL初始化

void init()
{
	// 初始化棋盘格，包含64个顶点坐标（总共32条线），并且每个顶点一个颜色值
	vec4 gridpoints[64];
	vec4 gridcolours[64];

	// 纵向线
	for (int i = 0; i < 11; i++)
	{
		gridpoints[2 * i] = vec4((33.0 + (33.0 * i)), 33.0, 0, 1);
		gridpoints[2 * i + 1] = vec4((33.0 + (33.0 * i)), 693.0, 0, 1);

	}

	// 水平线
	for (int i = 0; i < 21; i++)
	{
		gridpoints[22 + 2 * i] = vec4(33.0, (33.0 + (33.0 * i)), 0, 1);
		gridpoints[22 + 2 * i + 1] = vec4(363.0, (33.0 + (33.0 * i)), 0, 1);
	}

	// 将所有线赋成白色
	for (int i = 0; i < 64; i++)
		gridcolours[i] = white;

	// 初始化棋盘格，并将没有被填充的格子设置成黑色
	vec4 boardpoints[1200];
	for (int i = 0; i < 1200; i++)
		boardcolours[i] = black;

	// 对每个格子，初始化6个顶点，表示两个三角形，绘制一个正方形格子
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

	// 将棋盘格所有位置的填充与否都设置为false（没有被填充）
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 20; j++)
			board[i][j] = false;

	// 载入着色器
	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	locxsize = glGetUniformLocation(program, "xsize");
	locysize = glGetUniformLocation(program, "ysize");

	vPosition = glGetAttribLocation(program, "vPosition");
	vColor = glGetAttribLocation(program, "vColor");

	glGenVertexArrays(3, &vaoIDs[0]);

	// 棋盘格顶点
	glBindVertexArray(vaoIDs[0]);
	glGenBuffers(2, vboIDs);

	// 棋盘格顶点位置
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 64 * sizeof(vec4), gridpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// 棋盘格顶点颜色
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 64 * sizeof(vec4), gridcolours, GL_STATIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	// 棋盘格每个格子
	glBindVertexArray(vaoIDs[1]);
	glGenBuffers(2, &vboIDs[2]);

	// 棋盘格每个格子顶点位置
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, 1200 * sizeof(vec4), boardpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// 棋盘格每个格子顶点颜色
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200 * sizeof(vec4), boardcolours, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	

	// 当前方块
	glBindVertexArray(vaoIDs[2]);
	glGenBuffers(2, &vboIDs[4]);

	// 当前方块顶点位置
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// 当前方块顶点颜色
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);


	// 下一个方块
	glBindVertexArray(ne_vaoIDs[0]);
	glGenBuffers(2, &ne_vboIDs[0]);

	// 当前方块顶点位置
	glBindBuffer(GL_ARRAY_BUFFER, ne_vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// 当前方块顶点颜色
	glBindBuffer(GL_ARRAY_BUFFER, ne_vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	glBindVertexArray(0);

	glClearColor(0, 0, 0, 0);

	// 游戏初始化
	newtile();
	starttime = glutGet(GLUT_ELAPSED_TIME);
}



//////////////////////////////////////////////////////////////////////////
// 检查在cellpos位置的格子是否被填充或者是否在棋盘格的边界范围内。

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
// 在棋盘上有足够空间的情况下旋转当前方块

void rotate()
{
	// 计算得到下一个旋转方向
	int nextrotation = (rotation + 1) % 4;

	// 检查当前旋转之后的位置的有效性
	if (checkvalid((allRotationsLshape[z][nextrotation][0]) + tilepos)
		&& checkvalid((allRotationsLshape[z][nextrotation][1]) + tilepos)
		&& checkvalid((allRotationsLshape[z][nextrotation][2]) + tilepos)
		&& checkvalid((allRotationsLshape[z][nextrotation][3]) + tilepos))
	{
		// 更新旋转，将当前方块设置为旋转之后的方块
		rotation = nextrotation;
		for (int i = 0; i < 4; i++)
			tile[i] = allRotationsLshape[z][rotation][i];

		updatetile();
	}
}

//////////////////////////////////////////////////////////////////////////
// 检查棋盘格在row行有没有被填充满
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
// 放置当前方块，并且更新棋盘格对应位置顶点的颜色VBO


void settile()
{
	// 每个格子
	for (int i = 0; i < 4; i++)
	{
		// 获取格子在棋盘格上的坐标
		int x = (tile[i] + tilepos).x;
		int y = (tile[i] + tilepos).y;
		// 将格子对应在棋盘格上的位置设置为填充
		board[x][y] = true;
		// 并将相应位置的颜色修改
		changecellcolour(vec2(x, y), tilecolor[col]);
	}

	//消除并计算得分
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
// 给定位置(x,y)，移动方块。有效的移动值为(-1,0)，(1,0)，(0,-1)，分别对应于向
// 左，向下和向右移动。如果移动成功，返回值为true，反之为false。

bool movetile(vec2 direction)
{
	// 计算移动之后的方块的位置坐标
	vec2 newtilepos[4];
	for (int i = 0; i < 4; i++)
		newtilepos[i] = tile[i] + tilepos + direction;

	// 检查移动之后的有效性
	if (checkvalid(newtilepos[0])
		&& checkvalid(newtilepos[1])
		&& checkvalid(newtilepos[2])
		&& checkvalid(newtilepos[3]))
	{
		// 有效：移动该方块
		tilepos.x = tilepos.x + direction.x;
		tilepos.y = tilepos.y + direction.y;

		updatetile();

		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
// 重新启动游戏

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
// 游戏渲染部分

void sc_Text(vec2 vz, vec3 ccol, const char* str)
{
	//更改渲染模式
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
	glDrawArrays(GL_TRIANGLES, 0, 1200); // 绘制棋盘格 (10*20*2 = 400 个三角形)

	glBindVertexArray(vaoIDs[2]);
	glDrawArrays(GL_TRIANGLES, 0, 24);	 // 绘制当前方块 (8 个三角形)

	glBindVertexArray(ne_vaoIDs[0]);
	glDrawArrays(GL_TRIANGLES, 0, 24);	 // 绘制当前方块 (8 个三角形)

	glBindVertexArray(vaoIDs[0]);
	glDrawArrays(GL_LINES, 0, 64);		 // 绘制棋盘格的线

	glBindVertexArray(nexk_vaoIDs[0]);
	glDrawArrays(GL_LINES, 0, 32);		 // 绘制棋盘格的线

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
// 在窗口被拉伸的时候，控制棋盘格的大小，使之保持固定的比例。

void reshape(GLsizei w, GLsizei h)
{
	xsize = w;
	ysize = h;
	glViewport(0, 0, w, h);
}

//////////////////////////////////////////////////////////////////////////
// 键盘响应事件中的特殊按键响应

void special(int key, int x, int y)
{
	if (!gameover)
	{
		switch (key)
		{
		case GLUT_KEY_UP:	// 向上按键旋转方块
			rotate();
			break;
		case GLUT_KEY_DOWN: // 向下按键移动方块
			if (!movetile(vec2(0, -1)))
			{
				settile();
				newtile();
			}
			break;
		case GLUT_KEY_LEFT:  // 向左按键移动方块
			movetile(vec2(-1, 0));
			break;
		case GLUT_KEY_RIGHT: // 向右按键移动方块
			movetile(vec2(1, 0));
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 键盘响应时间中的普通按键响应
int Sc = 2000;		//路程
int speed = 100;	//速度
int jtime = 0;		//掉落时间

void idle(void)
{
	//模拟实际运动来控制实际掉落速度
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
	case 033: // ESC键 和 'q' 键退出游戏
		exit(EXIT_SUCCESS);
		break;
	case 'q':
		exit(EXIT_SUCCESS);
		break;
	case 'r': // 'r' 键重启游戏
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
	glutCreateWindow("俄罗斯方块_梁贵皓_2018152037");
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
