#include<Windows.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<GL/glut.h>
//#include"declarations.h"
//#include"colours.h"
#include<GL/glut.h>

GLfloat colors[48][3]={
						{1.0,1.0,1.0},
						{0.0,0.0,0.0},
						{1.0,0.0,0.0},
						{0.0,1.0,0.0},
						{0.0,0.0,1.0},
						{1.0,1.0,0.0},
						{1.0,0.0,1.0},
						{0.0,1.0,1.0},						
						{0.75,0.8,0.9},
						{0.6,0.2,0.7},
						{0.3,0.3,0.3},
						{0.1,0.55,0.1},
						{1.0,0.3,0.0},
						{0.5,0.4,1},
						{0.6,0.0,0.1},
						{0.5,0.2,0.1},
						{0.7,0.7,0.7},
						{0.67,0.0,0.0},
						{0.301961,0,0},
						{0,0.301961,0},
						{0.0,0.5,0.4},
						{0.301961,0,0},
						{0.6,0,0},
						{0,0.6,0},
						{0.898039,0,0},
						{0,0.898039,0},
						{1,0.301961,0},
						{0,0.501961,0.6},
						{0.698039,0.501961,0},
						{0,0.501961,1},
						{0.698039,0.698039,0},
						{0,0.698039,1},
						{1,0.8,0},
						{0,.63452,1},
						{.43276,1,0},
						{0.101961,0.301961,1},
						{.8,0.698039,0.898039},
						{0.101961,.4,1},
						{0.698039,0.501961,1},
						{.898039,.898039,.898039},
						{.898039,.301961,1},
						{0.698039,0.698039,0.698039},
						{0,0.698039,0.698039},
						{0.501961,0.501961,0.501961},
						{0,0.501961,0.501961},
						{0.301961,.301961,.301961},
						{0.501961,.10961,0.4}
						};
						
						
#define PENCIL 1
#define BRUSH 2
#define LINE 3
#define RECT 4
#define FILLRECT 5
#define SPRAY 6
#define CIRCLE 7
#define ERAZE 8
#define QUAD 9 
#define NEW 10
#define OPEN 11
#define SAVE 12
#define EXIT 13
#define INSIDECLIP 14
#define OUTSIDECLIP 15
#define TRANSLATE 16
#define SCALING 17
#define SPIRAL 18
#define ERAZE1 19
#define ERAZE2 20
#define ERAZE3 21
#define TRANSLATE1 22
#define LIMACON 23
#define CARDOID 24
#define LEAF 25
#define ROTATEL 26
#define ROTATER 27
#define REFX 28
#define REFY 29
#define TRI 30
#define MAXX 1018
#define MAXY 700

struct Color
{
GLfloat red;
GLfloat green;
GLfloat blue;
};

struct Tri
{
	GLfloat x,y;
};


float arr[MAXX-50][MAXY-50][3],mat[MAXX-50][MAXY-50][3],clip[MAXX-50][MAXY-40][3],mat1[MAXX-50][MAXY-40][3], arr1[MAXX-50][MAXY-50][3],pix[3];
int size=20, count=0, tricount = 0;
int refx,refy;
int linex=0,liney=0,elinex=0,eliney=0,clr=0;
int drawline,drawrect,drawcircle,drawspiral,drawleaf,drawcardoid,drawlimacon,insideclip,outsideclip,translate,scaling,trans_paste,scale_paste,translate1,trans_paste1,rot_paste,rotatel,rotater,reflectx,reflecty,triangle;
int ax,ay,bx,by,px,py,savex,savey;
int state1,ret,savec,openc;
long matsize=(MAXX-50)*(MAXY-40)*3;
FILE *fp;
int screenwidth=MAXX,screenheight=MAXY;
struct Color col[MAXX-50][MAXY-50];
struct Tri tri[4];
int pixels[MAXX-50][MAXY-50];

void drawobj()
{
int m,n;
for(m=0;m<MAXX-50;m++)
for(n=0;n<MAXY-50;n++)
{
if(pixels[m][n])
{
glBegin(GL_POINTS);
glColor3f(col[m][n].red,col[m][n].green,col[m][n].blue);
glVertex2f(m,n);
glEnd();
}
}
}

void lineloop(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
	glBegin(GL_LINE_LOOP);
	glVertex2i(x1,y1);
    glVertex2i(x2,y2);
	glVertex2i(x3,y3);
	glVertex2i(x4,y4);
	glEnd();
	glFlush();
}

void polygon(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
	glBegin(GL_POLYGON);
	glVertex2i(x1,y1);
	glVertex2i(x2,y2);
	glVertex2i(x3,y3);
	glVertex2i(x4,y4);
	glEnd();
	glFlush();
}

void spiral(GLdouble Radius,int x,int y)
{
	GLdouble Angle;
	glBegin(GL_LINE_STRIP);
	for(Angle = 0; Angle < 40.0; Angle += 0.2)
	{
		glVertex2d(x+Radius * cos(Angle), y+Radius * sin(Angle));
		Radius *= 1.005;
	}
	glEnd();
	glFlush();
}

void limacon(GLdouble Radius,int x,int y)
{
	GLdouble Angle;
	int b = Radius/2;
	int a = Radius;
	glBegin(GL_LINE_STRIP);
	for(Angle = 0; Angle < 40.0; Angle += 0.2)
	{
		Radius = a*cos(Angle)+b;
		glVertex2d(x+Radius * cos(Angle), y+Radius * sin(Angle));
		Radius *= 1.005;
	}
	glEnd();
	glFlush();
}

void cardoid(GLdouble Radius,int x,int y)
{
	GLdouble Angle;
	int b,a;
	a = Radius;
	b = a;
	glBegin(GL_LINE_STRIP);
	for(Angle = 0; Angle < 40.0; Angle += 0.2)
	{
		Radius = a*cos(Angle)+b;
		glVertex2d(x+Radius * cos(Angle), y+Radius * sin(Angle));
		Radius *= 1.005;
	}
	glEnd();
	glFlush();
}

void threeleaf(GLdouble Radius,int x,int y)
{
	GLdouble Angle;
	int a;
	a = Radius;
	glBegin(GL_LINE_STRIP);
	for(Angle = 0; Angle < 40.0; Angle += 0.2)
	{
		Radius = a*cos(3*Angle);
		glVertex2d(x+Radius * cos(Angle), y+Radius * sin(Angle));
		Radius *= 1.005;
	}
	glEnd();
	glFlush();
}

void line(int x1,int y1, int x2, int y2)
{
	glBegin(GL_LINES);
	glVertex2i(x1,y1);
	glVertex2i(x2,y2);
	glEnd();
	glFlush();
}

void draw_text(char *info,int i,int j)
{
	glRasterPos2i(i,j);
	while(*info)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*info++);
	}
}
// to colour the boxes on the left column of right side
void boxes(int x,int y,GLfloat color1[])
{
	glColor3fv(color1);
	polygon(970+5,x,970+5,y,970+45,y,970+45,x);
}
// to colour the boxes on the right column of right side
void boxes1(int x,int y,GLfloat color1[])
{
	glColor3fv(color1);
	polygon(900+5,x,900+5,y,900+45,y,900+45,x);
}

void menu(int x1, int x2, int y1, int y2, GLfloat color1[])
{
	glColor3fv(color1);
	polygon(x1,y1,x1,y2,x2,y2,x2,y1);	
}
//to test whether the given point is inside the area defined by coordinates 
int inside_area(int left,int right,int bottom,int top,int x,int y)
{
	if(x>left && x<right)
		if(y<top && y>bottom)
			return true;
	return false;
}

void draw_pixel(GLint cx,GLint cy)
{
	glBegin(GL_POINTS);
	glVertex2i(cx,cy);
	glEnd();

}

// draws points for circle based on symmetry

void plot_pixels(GLint h,GLint k,GLint x,GLint y)
{
	draw_pixel(x+h,y+k);
	draw_pixel(-x+h,y+k);
	draw_pixel(x+h,-y+k);
	draw_pixel(-x+h,-y+k);
	draw_pixel(y+h,x+k);
	draw_pixel(-y+h,x+k);
	draw_pixel(y+h,-x+k);
	draw_pixel(-y+h,-x+k);
}


void circle(GLint h,GLint k,GLint r)
{
	GLint d=1-r,x=0,y=r;
	while(y>x)
	{
		plot_pixels(h,k,x,y);
		if(d<0)	d+=2*x+3;
		else
		{
			d+=2*(x-y)+5;
			--y;
		}
		++x;
	}
	plot_pixels(h,k,x,y);
	
}
// function to spray . size defines size of spraying 
void spray(int x,int y)
{
	int randx,randy;
	int x1,y1;
	x1=x+10;
	y1=y+15;
	glBegin(GL_POINTS);
	for(int i=0;i<50;i++)
	{
		randx=x1-size+2/2+rand()%(size/2);
		randy=y1-size+2/2+rand()%(size/2);
		glVertex2i(randx,randy);
	}
	glFlush();
	glEnd();
}


void disp()
{    // display menu on the right side with the names
	glColor3fv(colors[1]);
	draw_text("Spray",975,MAXY-100-5);
	draw_text("Eraser",975,MAXY-145-5);
	draw_text("Triangle",974,MAXY-190-5);
	draw_text("Scale",980,MAXY-190-45-5);
	draw_text("Trans-",975,MAXY-190-90-5);
	draw_text("late",975,MAXY-190-90-20);
	draw_text("Copy",980,MAXY-280-45-5);
	draw_text("Limacon",975,MAXY-330-45-5);
	draw_text("3 Leaf",975,MAXY-370-45-5);
	draw_text("Rotate",977,MAXY-413-45-5);
	draw_text("Right",980,MAXY-413-45-20);
	draw_text("Reflect",977,MAXY-457-45-5);
	draw_text("abt Y",980,MAXY-457-45-20);
	draw_text("Clip",980,MAXY-502-45-5);
	draw_text("Out",982,MAXY-502-45-20);
	draw_text("Pencil",910,MAXY-100-5);
	draw_text("Line",910,MAXY-145-5);
	draw_text("Circle",910,MAXY-190-5);
	draw_text("Rect.",910,MAXY-190-45-5);
	draw_text("Fill",910,MAXY-190-90-5);
	draw_text("Rect.",910,MAXY-190-90-20);
	draw_text("Cut",910,MAXY-280-45-5);
	draw_text("Spiral",910,MAXY-330-45-5);
	draw_text("Cardoid",903,MAXY-370-45-5);
	draw_text("Rotate",907,MAXY-413-45-5);
	draw_text("Left",910,MAXY-413-45-20);
	draw_text("Reflect",906,MAXY-457-45-5);
	draw_text("abt X",908,MAXY-457-45-20);
	draw_text("Clip",910,MAXY-502-45-5);
	draw_text("In",914,MAXY-502-45-20);
	glColor3fv(colors[clr]);
	polygon(10,10,10,40,40,40,40,10);	
	
	glFlush();
}

void menu_disp()
{   //display the menu for file operations
	glColor3f(1.0,1.0,1.0);
	draw_text("New",13,MAXY-50);
    draw_text("Open",60,MAXY-50);
    draw_text("Save",110,MAXY-50);
    draw_text("Exit",160,MAXY-50);
	glColor3fv(colors[7]);
    draw_text("By Akanksha Bubber and Niveditha Shenoy",MAXX-200,MAXY-20);
	
	glFlush();
}

void mouse(int button,int state,int x,int y)
{		
	if(state==GLUT_DOWN && button==GLUT_LEFT_BUTTON)
	{
		y=MAXY-y;

		// For colour pallette to set/choose the colour
		if(inside_area(50,75,5,25,x,y))
		{
			clr=0;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[0]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		
		if(inside_area(75,100,5,25,x,y))
		{
			clr=1;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[1]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(100,125,5,25,x,y))
		{
			clr=2;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[2]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(125,150,5,25,x,y))
		{
			clr=3;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[3]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(150,175,5,25,x,y))
		{
			clr=4;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[4]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(175,200,5,25,x,y))
		{
			clr=5;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[5]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(200,225,5,25,x,y))
		{
			clr=6;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[6]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(225,250,5,25,x,y))
		{
			clr=7;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[7]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(250,275,5,25,x,y))
		{
			clr=8;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[8]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(275,300,5,25,x,y))
		{
			clr=9;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[9]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}

			if(inside_area(50,75,30,50,x,y))
		{
			clr=10;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[10]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		
		if(inside_area(75,100,30,50,x,y))
		{
			clr=11;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[11]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(100,125,30,50,x,y))
		{
			clr=12;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[12]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(125,150,30,50,x,y))
		{
			clr=13;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[13]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(150,175,30,50,x,y))
		{
			clr=14;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[14]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(175,200,30,50,x,y))
		{
			clr=15;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[15]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(200,225,30,50,x,y))
		{
			clr=16;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[16]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		/*if(inside_area(225,250,30,50,x,y))
		{
			clr=17;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[17]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}*/
		if(inside_area(250,275,30,50,x,y))
		{
			clr=18;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[18]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(275,300,30,50,x,y))
		{
			clr=19;


			//filling the big polygon on left with selected colour
			glColor3fv(colors[19]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(300,325,30,50,x,y))
		{
			clr=20;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[20]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}

		if(inside_area(325,350,30,50,x,y))
		{
			clr=21;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[21]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(350,375,30,50,x,y))
		{
			clr=22;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[22]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(375,400,30,50,x,y))
		{
			clr=23;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[23]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(400,425,30,50,x,y))
		{
			clr=24;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[24]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(425,450,30,50,x,y))
		{
			clr=25;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[25]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(450,475,30,50,x,y))
		{
			clr=26;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[26]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(475,500,30,50,x,y))
		{
			clr=27;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[27]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(500,525,30,50,x,y))
		{
			clr=28;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[28]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(525,550,30,50,x,y))
		{
			clr=29;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[29]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(550,575,30,50,x,y))
		{
			clr=30;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[30]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(575,600,30,50,x,y))
		{
			clr=31;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[31]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(600,625,30,50,x,y))
		{
			clr=32;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[32]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(625,650,30,50,x,y))
		{
			clr=33;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[33]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(650,675,30,50,x,y))
		{
			clr=34;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[34]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		// When an option is chosen on the left side bar, its highlighted and all the others are made white
		if(inside_area(675,700,30,50,x,y))
		{
			clr=35;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[35]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(700,725,30,50,x,y))
		{
			clr=36;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[36]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(725,750,30,50,x,y))
		{
			clr=37;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[37]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(750,775,30,50,x,y))
		{
			clr=38;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[38]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(275,300,30,50,x,y))
		{
			clr=19;

			//filling the big polygon on left with selected colour
			glColor3fv(colors[19]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}


		// When an option is chosen on the left side bar, its highlighted and all the others are made white

		int i=125,j=90;
		if(inside_area(970+5,970+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[20]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes1(575,610,colors[0]);  
			boxes1(530,565,colors[0]);
			boxes1(485,520,colors[0]);
			boxes1(440,475,colors[0]);
			boxes1(395,430,colors[0]);
			boxes1(350,385,colors[0]);
			boxes1(305,340,colors[0]);
			boxes1(260,295,colors[0]);
			boxes1(215,250,colors[0]);
			boxes1(170,205,colors[0]);
			boxes1(125,160,colors[0]);

			boxes(575,610,colors[7]);  
			boxes(530,565,colors[0]);
			boxes(485,520,colors[0]);
			boxes(440,475,colors[0]);
			boxes(395,430,colors[0]);
			boxes(350,385,colors[0]);
			boxes(305,340,colors[0]);	
			boxes(260,295,colors[0]);
			boxes(215,250,colors[0]);
			boxes(170,205,colors[0]);
			boxes(125,160,colors[0]);

			glColor3f(1,1,1);
			draw_text("SPRAY - press + to increase size , press - to decrease size",625,30);
			disp();
			glFlush();
			state1=SPRAY;return;
		}
		i+=45;j+=45;
		if(inside_area(970+5,970+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[20]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
				boxes1(575,610,colors[0]);  
			boxes1(530,565,colors[0]);
			boxes1(485,520,colors[0]);
			boxes1(440,475,colors[0]);
			boxes1(395,430,colors[0]);
			boxes1(350,385,colors[0]);
			boxes1(305,340,colors[0]);
			boxes1(260,295,colors[0]);
			boxes1(215,250,colors[0]);
			boxes1(170,205,colors[0]);
			boxes1(125,160,colors[0]);

			boxes(575,610,colors[0]);  
			boxes(530,565,colors[7]);
			boxes(485,520,colors[0]);
			boxes(440,475,colors[0]);
			boxes(395,430,colors[0]);
			boxes(350,385,colors[0]);
			boxes(305,340,colors[0]);
			boxes(260,295,colors[0]);
			boxes(215,250,colors[0]);
			boxes(170,205,colors[0]);
			boxes(125,160,colors[0]);

			glColor3f(1,1,1);
			draw_text("ERASER '+' to increase and '-' to decrease erazer size",625,30);
			disp();
			glFlush();
			state1=ERAZE2;return;
		}
		i+=45;j+=45;
        if(inside_area(970+5,970+45,MAXY-i,MAXY-j,x,y))
		{
		glColor3fv(colors[20]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes1(575,610,colors[0]);  
			boxes1(530,565,colors[0]);
			boxes1(485,520,colors[0]);
			boxes1(440,475,colors[0]);
			boxes1(395,430,colors[0]);
			boxes1(350,385,colors[0]);
			boxes1(305,340,colors[0]);
			boxes1(260,295,colors[0]);
			boxes1(215,250,colors[0]);
			boxes1(170,205,colors[0]);
			boxes1(125,160,colors[0]);

			boxes(575,610,colors[0]);  
			boxes(530,565,colors[0]);
			boxes(485,520,colors[7]);
			boxes(440,475,colors[0]);
			boxes(395,430,colors[0]);
			boxes(350,385,colors[0]);
			boxes(305,340,colors[0]);
			boxes(260,295,colors[0]);
			boxes(215,250,colors[0]);
			boxes(170,205,colors[0]);
			boxes(125,160,colors[0]);

			glColor3f(1,1,1);
			draw_text("TRIANGLE - To draw a triangle",625,30);
			disp();
			glFlush();
			triangle=1;
			tricount= 0;
			state1=TRI;return;
		}		
		i+=45;j+=45;
		if(inside_area(970+5,970+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[20]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
				boxes1(575,610,colors[0]);  
			boxes1(530,565,colors[0]);
			boxes1(485,520,colors[0]);
			boxes1(440,475,colors[0]);
			boxes1(395,430,colors[0]);
			boxes1(350,385,colors[0]);
			boxes1(305,340,colors[0]);
			boxes1(260,295,colors[0]);
			boxes1(215,250,colors[0]);
			boxes1(170,205,colors[0]);
			boxes1(125,160,colors[0]);

			boxes(575,610,colors[0]);  
			boxes(530,565,colors[0]);
			boxes(485,520,colors[0]);
			boxes(440,475,colors[7]);
			boxes(395,430,colors[0]);
			boxes(350,385,colors[0]);
			boxes(305,340,colors[0]);
			boxes(260,295,colors[0]);
			boxes(215,250,colors[0]);
			boxes(170,205,colors[0]);
			boxes(125,160,colors[0]);

			glColor3f(1,1,1);
			draw_text("SCALING - to zoom the selected area to twice its size",625,30);
			disp();
			glFlush();
			state1=SCALING;return;
		}
		i+=45;j+=45;
		if(inside_area(970+5,970+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[20]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
				boxes1(575,610,colors[0]);  
			boxes1(530,565,colors[0]);
			boxes1(485,520,colors[0]);
			boxes1(440,475,colors[0]);
			boxes1(395,430,colors[0]);
			boxes1(350,385,colors[0]);
			boxes1(305,340,colors[0]);
			boxes1(260,295,colors[0]);
			boxes1(215,250,colors[0]);
			boxes1(170,205,colors[0]);
			boxes1(125,160,colors[0]);

			boxes(575,610,colors[0]);  
			boxes(530,565,colors[0]);
			boxes(485,520,colors[0]);
			boxes(440,475,colors[0]);
			boxes(395,430,colors[7]);
			boxes(350,385,colors[0]);
			boxes(305,340,colors[0]);
			boxes(260,295,colors[0]);
			boxes(215,250,colors[0]);
			boxes(170,205,colors[0]);
			boxes(125,160,colors[0]);

			glColor3f(1,1,1);
			draw_text("TRANSLATE - to shift selected area to desired location ",625,30);
			disp();
			glFlush();
			state1=TRANSLATE;return;
		}
		i+=45;j+=45;
		if(inside_area(970+5,970+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[20]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
	boxes1(575,610,colors[0]);  
			boxes1(530,565,colors[0]);
			boxes1(485,520,colors[0]);
			boxes1(440,475,colors[0]);
			boxes1(395,430,colors[0]);
			boxes1(350,385,colors[0]);
			boxes1(305,340,colors[0]);
			boxes1(260,295,colors[0]);
			boxes1(215,250,colors[0]);
			boxes1(170,205,colors[0]);
			boxes1(125,160,colors[0]);

			boxes(575,610,colors[0]);  
			boxes(530,565,colors[0]);
			boxes(485,520,colors[0]);
			boxes(440,475,colors[0]);
			boxes(395,430,colors[0]);
			boxes(350,385,colors[7]);
			boxes(305,340,colors[0]);
			boxes(260,295,colors[0]);
			boxes(215,250,colors[0]);
			boxes(170,205,colors[0]);
			boxes(125,160,colors[0]);

			glColor3f(1,1,1);
			draw_text("COPY - To copy and paste",625,30);
			disp();
			glFlush();
			state1=TRANSLATE1;return;
		}		

		i+=45;j+=45;
		if(inside_area(970+5,970+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[20]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes1(575,610,colors[0]);  
			boxes1(530,565,colors[0]);
			boxes1(485,520,colors[0]);
			boxes1(440,475,colors[0]);
			boxes1(395,430,colors[0]);
			boxes1(350,385,colors[0]);
			boxes1(305,340,colors[0]);
			boxes1(260,295,colors[0]);
			boxes1(215,250,colors[0]);
			boxes1(170,205,colors[0]);
			boxes1(125,160,colors[0]);

			boxes(575,610,colors[0]);  
			boxes(530,565,colors[0]);
			boxes(485,520,colors[0]);
			boxes(440,475,colors[0]);
			boxes(395,430,colors[0]);
			boxes(350,385,colors[0]);
			boxes(305,340,colors[7]);
			boxes(260,295,colors[0]);
			boxes(215,250,colors[0]);
			boxes(170,205,colors[0]);
			boxes(125,160,colors[0]);

			glColor3f(1,1,1);
			draw_text("LIMACON - To draw a Limacon",625,30);
			disp();
			glFlush();
			state1 = LIMACON; return;
		}		

		i+=45;j+=45;
		if(inside_area(970+5,970+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[20]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes1(575,610,colors[0]);  
			boxes1(530,565,colors[0]);
			boxes1(485,520,colors[0]);
			boxes1(440,475,colors[0]);
			boxes1(395,430,colors[0]);
			boxes1(350,385,colors[0]);
			boxes1(305,340,colors[0]);
			boxes1(260,295,colors[0]);
			boxes1(215,250,colors[0]);
			boxes1(170,205,colors[0]);
			boxes1(125,160,colors[0]);

			boxes(575,610,colors[0]);  
			boxes(530,565,colors[0]);
			boxes(485,520,colors[0]);
			boxes(440,475,colors[0]);
			boxes(395,430,colors[0]);
			boxes(350,385,colors[0]);
			boxes(305,340,colors[0]);
			boxes(260,295,colors[7]);
			boxes(215,250,colors[0]);
			boxes(170,205,colors[0]);
			boxes(125,160,colors[0]);

			glColor3f(1,1,1);
			draw_text("THREELEAF - To draw three leaves",625,30);
			disp();
			glFlush();
			state1=LEAF;return;
		}	
		i+=45;j+=45;
		if(inside_area(970+5,970+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[20]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes1(575,610,colors[0]);  
			boxes1(530,565,colors[0]);
			boxes1(485,520,colors[0]);
			boxes1(440,475,colors[0]);
			boxes1(395,430,colors[0]);
			boxes1(350,385,colors[0]);
			boxes1(305,340,colors[0]);
			boxes1(260,295,colors[0]);
			boxes1(215,250,colors[0]);
			boxes1(170,205,colors[0]);
			boxes1(125,160,colors[0]);

			boxes(575,610,colors[0]);  
			boxes(530,565,colors[0]);
			boxes(485,520,colors[0]);
			boxes(440,475,colors[0]);
			boxes(395,430,colors[0]);
			boxes(350,385,colors[0]);
			boxes(305,340,colors[0]);
			boxes(260,295,colors[0]);
			boxes(215,250,colors[7]);
			boxes(170,205,colors[0]);
			boxes(125,160,colors[0]);

			glColor3f(1,1,1);
			draw_text("To rotate clockwise by 20 degrees",625,30);
			disp();
			glFlush();
			state1=ROTATER;return;
		}

		i+=45;j+=45;
		if(inside_area(970+5,970+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[20]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes1(575,610,colors[0]);  
			boxes1(530,565,colors[0]);
			boxes1(485,520,colors[0]);
			boxes1(440,475,colors[0]);
			boxes1(395,430,colors[0]);
			boxes1(350,385,colors[0]);
			boxes1(305,340,colors[0]);
			boxes1(260,295,colors[0]);
			boxes1(215,250,colors[0]);
			boxes1(170,205,colors[0]);
			boxes1(125,160,colors[0]);

			boxes(575,610,colors[0]);  
			boxes(530,565,colors[0]);
			boxes(485,520,colors[0]);
			boxes(440,475,colors[0]);
			boxes(395,430,colors[0]);
			boxes(350,385,colors[0]);
			boxes(305,340,colors[0]);
			boxes(260,295,colors[0]);
			boxes(215,250,colors[0]);
			boxes(170,205,colors[7]);
			boxes(125,160,colors[0]);

			glColor3f(1,1,1);
			draw_text("Reflect about Y Axis",625,30);
			disp();
			glFlush();
			state1=REFY;return;
		}
		i+=45;j+=45;
		if(inside_area(970+5,970+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[20]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes1(575,610,colors[0]);  
			boxes1(530,565,colors[0]);
			boxes1(485,520,colors[0]);
			boxes1(440,475,colors[0]);
			boxes1(395,430,colors[0]);
			boxes1(350,385,colors[0]);
			boxes1(305,340,colors[0]);
			boxes1(260,295,colors[0]);
			boxes1(215,250,colors[0]);
			boxes1(170,205,colors[0]);
			boxes1(125,160,colors[0]);

			boxes(575,610,colors[0]);  
			boxes(530,565,colors[0]);
			boxes(485,520,colors[0]);
			boxes(440,475,colors[0]);
			boxes(395,430,colors[0]);
			boxes(350,385,colors[0]);
			boxes(305,340,colors[0]);
			boxes(260,295,colors[0]);
			boxes(215,250,colors[0]);
			boxes(170,205,colors[0]);
			boxes(125,160,colors[7]);

			glColor3f(1,1,1);
			draw_text("Clip everything outside the clip window",625,30);
			disp();
			glFlush();
			state1=OUTSIDECLIP;return;
		}

			 i=125;j=90;
		if(inside_area(900+5,900+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[20]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			
			boxes1(575,610,colors[7]);  
			boxes1(530,565,colors[0]);
			boxes1(485,520,colors[0]);
			boxes1(440,475,colors[0]);
			boxes1(395,430,colors[0]);
			boxes1(350,385,colors[0]);
			boxes1(305,340,colors[0]);
			boxes1(260,295,colors[0]);
			boxes1(215,250,colors[0]);
			boxes1(170,205,colors[0]);
			boxes1(125,160,colors[0]);

			boxes(575,610,colors[0]);  
			boxes(530,565,colors[0]);
			boxes(485,520,colors[0]);
			boxes(440,475,colors[0]);
			boxes(395,430,colors[0]);
			boxes(350,385,colors[0]);
			boxes(305,340,colors[0]);
			boxes(260,295,colors[0]);
			boxes(215,250,colors[0]);
			boxes(170,205,colors[0]);
			boxes(125,160,colors[0]);

			glColor3f(1,1,1);
			draw_text("PENCIL - used for free hand drawing",625,30);
			disp();
			glFlush();
			state1=PENCIL;return;
		}
		i+=45;j+=45;
		if(inside_area(900+5,900+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[20]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
				boxes1(575,610,colors[0]);  
			boxes1(530,565,colors[7]);
			boxes1(485,520,colors[0]);
			boxes1(440,475,colors[0]);
			boxes1(395,430,colors[0]);
			boxes1(350,385,colors[0]);
			boxes1(305,340,colors[0]);
			boxes1(260,295,colors[0]);
			boxes1(215,250,colors[0]);
			boxes1(170,205,colors[0]);
			boxes1(125,160,colors[0]);

			boxes(575,610,colors[0]);  
			boxes(530,565,colors[0]);
			boxes(485,520,colors[0]);
			boxes(440,475,colors[0]);
			boxes(395,430,colors[0]);
			boxes(350,385,colors[0]);
			boxes(305,340,colors[0]);
			boxes(260,295,colors[0]);
			boxes(215,250,colors[0]);
			boxes(170,205,colors[0]);
			boxes(125,160,colors[0]);

			glColor3f(1,1,1);
			draw_text("LINE - used to draw a line of any length between two desired points",625,30);
			disp();
			glFlush();
			state1=LINE;return;
		}
		i+=45;j+=45;
        if(inside_area(900+5,900+45,MAXY-i,MAXY-j,x,y))
		{
		glColor3fv(colors[20]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes1(575,610,colors[0]);  
			boxes1(530,565,colors[0]);
			boxes1(485,520,colors[7]);
			boxes1(440,475,colors[0]);
			boxes1(395,430,colors[0]);
			boxes1(350,385,colors[0]);
			boxes1(305,340,colors[0]);
			boxes1(260,295,colors[0]);
			boxes1(215,250,colors[0]);
			boxes1(170,205,colors[0]);
			boxes1(125,160,colors[0]);

			boxes(575,610,colors[0]);  
			boxes(530,565,colors[0]);
			boxes(485,520,colors[0]);
			boxes(440,475,colors[0]);
			boxes(395,430,colors[0]);
			boxes(350,385,colors[0]);
			boxes(305,340,colors[0]);
			boxes(260,295,colors[0]);
			boxes(215,250,colors[0]);
			boxes(170,205,colors[0]);
			boxes(125,160,colors[0]);

			glColor3f(1,1,1);
			draw_text("CIRCLE - used to draw circle of desired radius",625,30);
			disp();
			glFlush();
			state1=CIRCLE;return;
		}		
		i+=45;j+=45;
		if(inside_area(900+5,900+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[20]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
				boxes1(575,610,colors[0]);  
			boxes1(530,565,colors[0]);
			boxes1(485,520,colors[0]);
			boxes1(440,475,colors[7]);
			boxes1(395,430,colors[0]);
			boxes1(350,385,colors[0]);
			boxes1(305,340,colors[0]);
			boxes1(260,295,colors[0]);
			boxes1(215,250,colors[0]);
			boxes1(170,205,colors[0]);
			boxes1(125,160,colors[0]);

			boxes(575,610,colors[0]);  
			boxes(530,565,colors[0]);
			boxes(485,520,colors[0]);
			boxes(440,475,colors[0]);
			boxes(395,430,colors[0]);
			boxes(350,385,colors[0]);
			boxes(305,340,colors[0]);
			boxes(260,295,colors[0]);
			boxes(215,250,colors[0]);
			boxes(170,205,colors[0]);
			boxes(125,160,colors[0]);

			glColor3f(1,1,1);
			draw_text("RECTANGLE - used to draw empty rectangle of desired size",625,30);
			disp();
			glFlush();
			state1=RECT;return;
		}
		i+=45;j+=45;
		if(inside_area(900+5,900+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[20]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
				boxes1(575,610,colors[0]);  
			boxes1(530,565,colors[0]);
			boxes1(485,520,colors[0]);
			boxes1(440,475,colors[0]);
			boxes1(395,430,colors[7]);
			boxes1(350,385,colors[0]);
			boxes1(305,340,colors[0]);
			boxes1(260,295,colors[0]);
			boxes1(215,250,colors[0]);
			boxes1(170,205,colors[0]);
			boxes1(125,160,colors[0]);

			boxes(575,610,colors[0]);  
			boxes(530,565,colors[0]);
			boxes(485,520,colors[0]);
			boxes(440,475,colors[0]);
			boxes(395,430,colors[0]);
			boxes(350,385,colors[0]);
			boxes(305,340,colors[0]);
			boxes(260,295,colors[0]);
			boxes(215,250,colors[0]);
			boxes(170,205,colors[0]);
			boxes(125,160,colors[0]);

			glColor3f(1,1,1);
			draw_text("FILLED RECTANGLE - draws rectangle filled with selected colour ",625,30);
			disp();
			glFlush();
			state1=FILLRECT;return;
		}
		i+=45;j+=45;
		if(inside_area(900+5,900+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[20]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes1(575,610,colors[0]);  
			boxes1(530,565,colors[0]);
			boxes1(485,520,colors[0]);
			boxes1(440,475,colors[0]);
			boxes1(395,430,colors[0]);
			boxes1(350,385,colors[7]);
			boxes1(305,340,colors[0]);
			boxes1(260,295,colors[0]);
			boxes1(215,250,colors[0]);
			boxes1(170,205,colors[0]);
			boxes1(125,160,colors[0]);

			boxes(575,610,colors[0]);  
			boxes(530,565,colors[0]);
			boxes(485,520,colors[0]);
			boxes(440,475,colors[0]);
			boxes(395,430,colors[0]);
			boxes(350,385,colors[0]);
			boxes(305,340,colors[0]);
			boxes(260,295,colors[0]);
			boxes(215,250,colors[0]);
			boxes(170,205,colors[0]);
			boxes(125,160,colors[0]);

			glColor3f(1,1,1);
			draw_text("CUT - To cut and paste",625,30);
			disp();
			glFlush();
			state1=TRANSLATE;return;
		}		
		i+=45;j+=45;
		if(inside_area(900+5,900+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[20]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes1(575,610,colors[0]);  
			boxes1(530,565,colors[0]);
			boxes1(485,520,colors[0]);
			boxes1(440,475,colors[0]);
			boxes1(395,430,colors[0]);
			boxes1(350,385,colors[0]);
			boxes1(305,340,colors[7]);
			boxes1(260,295,colors[0]);
			boxes1(215,250,colors[0]);
			boxes1(170,205,colors[0]);
			boxes1(125,160,colors[0]);

			boxes(575,610,colors[0]);  
			boxes(530,565,colors[0]);
			boxes(485,520,colors[0]);
			boxes(440,475,colors[0]);
			boxes(395,430,colors[0]);
			boxes(350,385,colors[0]);
			boxes(305,340,colors[0]);
			boxes(260,295,colors[0]);
			boxes(215,250,colors[0]);
			boxes(170,205,colors[0]);
			boxes(125,160,colors[0]);

			glColor3f(1,1,1);
			draw_text("SPIRAL - To draw a spiral",625,30);
			disp();
			glFlush();
			state1=SPIRAL;return;
		}
		i+=45;j+=45;
		if(inside_area(900+5,900+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[20]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes1(575,610,colors[0]);  
			boxes1(530,565,colors[0]);
			boxes1(485,520,colors[0]);
			boxes1(440,475,colors[0]);
			boxes1(395,430,colors[0]);
			boxes1(350,385,colors[0]);
			boxes1(305,340,colors[0]);
			boxes1(260,295,colors[7]);
			boxes1(215,250,colors[0]);
			boxes1(170,205,colors[0]);
			boxes1(125,160,colors[0]);

			boxes(575,610,colors[0]);  
			boxes(530,565,colors[0]);
			boxes(485,520,colors[0]);
			boxes(440,475,colors[0]);
			boxes(395,430,colors[0]);
			boxes(350,385,colors[0]);
			boxes(305,340,colors[0]);
			boxes(260,295,colors[0]);
			boxes(215,250,colors[0]);
			boxes(170,205,colors[0]);
			boxes(125,160,colors[0]);

			glColor3f(1,1,1);
			draw_text("CARDOID - To draw a cardoid",625,30);
			disp();
			glFlush();
			state1=CARDOID;return;
		}
		i+=45;j+=45;
		if(inside_area(900+5,900+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[20]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes1(575,610,colors[0]);  
			boxes1(530,565,colors[0]);
			boxes1(485,520,colors[0]);
			boxes1(440,475,colors[0]);
			boxes1(395,430,colors[0]);
			boxes1(350,385,colors[0]);
			boxes1(305,340,colors[0]);
			boxes1(260,295,colors[0]);
			boxes1(215,250,colors[7]);
			boxes1(170,205,colors[0]);
			boxes1(125,160,colors[0]);

			boxes(575,610,colors[0]);  
			boxes(530,565,colors[0]);
			boxes(485,520,colors[0]);
			boxes(440,475,colors[0]);
			boxes(395,430,colors[0]);
			boxes(350,385,colors[0]);
			boxes(305,340,colors[0]);
			boxes(260,295,colors[0]);
			boxes(215,250,colors[0]);
			boxes(170,205,colors[0]);
			boxes(125,160,colors[0]);

			glColor3f(1,1,1);
			draw_text("To rotate anticlockwise by 20 degrees",625,30);
			disp();
			glFlush();
			state1=ROTATEL;return;
		}
		i+=45;j+=45;
		if(inside_area(900+5,900+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[20]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes1(575,610,colors[0]);  
			boxes1(530,565,colors[0]);
			boxes1(485,520,colors[0]);
			boxes1(440,475,colors[0]);
			boxes1(395,430,colors[0]);
			boxes1(350,385,colors[0]);
			boxes1(305,340,colors[0]);
			boxes1(260,295,colors[0]);
			boxes1(215,250,colors[0]);
			boxes1(170,205,colors[7]);
			boxes1(125,160,colors[0]);

			boxes(575,610,colors[0]);  
			boxes(530,565,colors[0]);
			boxes(485,520,colors[0]);
			boxes(440,475,colors[0]);
			boxes(395,430,colors[0]);
			boxes(350,385,colors[0]);
			boxes(305,340,colors[0]);
			boxes(260,295,colors[0]);
			boxes(215,250,colors[0]);
			boxes(170,205,colors[0]);
			boxes(125,160,colors[0]);

			glColor3f(1,1,1);
			draw_text("Reflect about X Axis",625,30);
			disp();
			glFlush();
			state1=REFX;return;
		}
		i+=45;j+=45;
		if(inside_area(900+5,900+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[20]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes1(575,610,colors[0]);  
			boxes1(530,565,colors[0]);
			boxes1(485,520,colors[0]);
			boxes1(440,475,colors[0]);
			boxes1(395,430,colors[0]);
			boxes1(350,385,colors[0]);
			boxes1(305,340,colors[0]);
			boxes1(260,295,colors[0]);
			boxes1(215,250,colors[0]);
			boxes1(170,205,colors[0]);
			boxes1(125,160,colors[7]);

			boxes(575,610,colors[0]);  
			boxes(530,565,colors[0]);
			boxes(485,520,colors[0]);
			boxes(440,475,colors[0]);
			boxes(395,430,colors[0]);
			boxes(350,385,colors[0]);
			boxes(305,340,colors[0]);
			boxes(260,295,colors[0]);
			boxes(215,250,colors[0]);
			boxes(170,205,colors[0]);
			boxes(125,160,colors[0]);

			glColor3f(1,1,1);
			draw_text("Clip everything inside the clip window",625,30);
			disp();
			glFlush();
			state1=INSIDECLIP;return;
		}
		//if the options on the top which are related to files are selected
		if(inside_area(5,45,MAXY-60,MAXY-35,x,y))
		{
			menu(5,45,MAXY-60,MAXY-35,colors[7]);
			menu(55,95,MAXY-60,MAXY-35,colors[20]);
			menu(105,145,MAXY-60,MAXY-35,colors[20]);
			menu(155,195,MAXY-60,MAXY-35,colors[20]);
			
			menu_disp();
			state1=NEW;
			return;		
		}
		if(inside_area(55,95,MAXY-60,MAXY-35,x,y))
		{
			menu(5,45,MAXY-60,MAXY-35,colors[20]);
			menu(55,95,MAXY-60,MAXY-35,colors[7]);
			menu(105,145,MAXY-60,MAXY-35,colors[20]);
			menu(155,195,MAXY-60,MAXY-35,colors[20]);
			
			menu_disp();
			state1=OPEN;
			openc=0;
			return;		
		}
		if(inside_area(105,145,MAXY-60,MAXY-35,x,y))
		{
			menu(5,45,MAXY-60,MAXY-35,colors[20]);
			menu(55,95,MAXY-60,MAXY-35,colors[20]);
			menu(105,145,MAXY-60,MAXY-35,colors[7]);
			menu(155,195,MAXY-60,MAXY-35,colors[20]);
			menu_disp();
			state1=SAVE;
			savec=0;
			return;		
		}
		if(inside_area(155,195,MAXY-60,MAXY-35,x,y))
		{
			menu(5,45,MAXY-60,MAXY-35,colors[20]);
			menu(55,95,MAXY-60,MAXY-35,colors[20]);
			menu(105,145,MAXY-60,MAXY-35,colors[20]);
			menu(155,195,MAXY-60,MAXY-35,colors[7]);
			
			menu_disp();
			state1=EXIT;
			return;		
		}
		
	}
	if(state1==NEW)
	{
		
	glColor3fv(colors[0]);
	polygon(50,80,MAXX-150,80,MAXX-150,MAXY-90,50,MAXY-90);

	//Outer boundary
	glColor3f(0.25,0.45,0.3);	//background colour - borders
	polygon(0,MAXY-90,0,MAXY,MAXX,MAXY,MAXX,MAXY-90); //top
	menu_disp();
    glColor3f(0.25,0.45,0.3);
	polygon(MAXX-150,80,MAXX,80,MAXX,MAXY-90,MAXX-150,MAXY-90); //right
	polygon(0,0,0,MAXY-90,50,MAXY-90,50,0);  //left
	polygon(0,0,0,80,MAXX,80,MAXX,0); //bottom

	//Colour pallette display
	glColor3fv(colors[0]);
	polygon(50,5,50,25,75,25,75,5);
	glColor3fv(colors[1]);
	polygon(50+25,5,50+25,25,75+25,25,75+25,5);
	glColor3fv(colors[2]);
	polygon(50+50,5,50+50,25,75+50,25,75+50,5);
	glColor3fv(colors[3]);
	polygon(50+75,5,50+75,25,75+75,25,75+75,5);
	glColor3fv(colors[4]);
	polygon(50+100,5,50+100,25,75+100,25,75+100,5);
	glColor3fv(colors[5]);
	polygon(50+125,5,50+125,25,75+125,25,75+125,5);
	glColor3fv(colors[6]);
	polygon(50+150,5,50+150,25,75+150,25,75+150,5);
	glColor3fv(colors[8]);
	polygon(50+200,5,50+200,25,75+200,25,75+200,5);
	glColor3fv(colors[7]);
	polygon(50+175,5,50+175,25,75+175,25,75+175,5);
	glColor3fv(colors[9]);
	polygon(50+225,5,50+225,25,75+225,25,75+225,5);
	glColor3fv(colors[10]);
	polygon(50,5+25,50,25+25,75,25+25,75,5+25);
	glColor3fv(colors[11]);
	polygon(50+25,5+25,50+25,25+25,75+25,25+25,75+25,5+25);
	glColor3fv(colors[12]);
	polygon(50+50,5+25,50+50,25+25,75+50,25+25,75+50,5+25);
	glColor3fv(colors[13]);
	polygon(50+75,5+25,50+75,25+25,75+75,25+25,75+75,5+25);
	glColor3fv(colors[14]);
	polygon(50+100,5+25,50+100,25+25,75+100,25+25,75+100,5+25);
	glColor3fv(colors[15]);
	polygon(50+125,5+25,50+125,25+25,75+125,25+25,75+125,5+25);
	glColor3fv(colors[16]);
	polygon(50+150,5+25,50+150,25+25,75+150,25+25,75+150,5+25);
	/*glColor3fv(colors[17]);
	polygon(50+200,5+25,50+200,25+25,75+200,25+25,75+200,5+25);*/
	glColor3fv(colors[17]);
	polygon(50+175,5+25,50+175,25+25,75+175,25+25,75+175,5+25);
	glColor3fv(colors[18]);
	polygon(50+200,5+25,50+200,25+25,75+200,25+25,75+200,5+25);
	glColor3fv(colors[19]);
	polygon(50+225,5+25,50+225,25+25,75+225,25+25,75+225,5+25);
	glColor3fv(colors[20]);
	polygon(50+250,5+25,50+250,25+25,75+250,25+25,75+250,5+25);
	glColor3fv(colors[21]);
	polygon(50+275,5+25,50+275,25+25,75+275,25+25,75+275,5+25);
	glColor3fv(colors[22]);
	polygon(50+300,5+25,50+300,25+25,75+300,25+25,75+300,5+25);
	glColor3fv(colors[23]);
	polygon(50+325,5+25,50+325,25+25,75+325,25+25,75+325,5+25);
	glColor3fv(colors[24]);
	polygon(50+350,5+25,50+350,25+25,75+350,25+25,75+350,5+25);
	glColor3fv(colors[25]);
	polygon(50+375,5+25,50+375,25+25,75+375,25+25,75+375,5+25);
	glColor3fv(colors[26]);
	polygon(50+400,5+25,50+400,25+25,75+400,25+25,75+400,5+25);
	glColor3fv(colors[27]);
	polygon(50+425,5+25,50+425,25+25,75+425,25+25,75+425,5+25);
	glColor3fv(colors[28]);
	polygon(50+450,5+25,50+450,25+25,75+450,25+25,75+450,5+25);
	glColor3fv(colors[29]);
	polygon(50+475,5+25,50+475,25+25,75+475,25+25,75+475,5+25);
	glColor3fv(colors[30]);
	polygon(50+500,5+25,50+500,25+25,75+500,25+25,75+500,5+25);
	glColor3fv(colors[31]);
	polygon(50+525,5+25,50+525,25+25,75+525,25+25,75+525,5+25);
	glColor3fv(colors[32]);
	polygon(50+550,5+25,50+550,25+25,75+550,25+25,75+550,5+25);
	glColor3fv(colors[33]);
	polygon(50+575,5+25,50+575,25+25,75+575,25+25,75+575,5+25);
	glColor3fv(colors[34]);
    polygon(50+600,5+25,50+600,25+25,75+600,25+25,75+600,5+25);
	glColor3fv(colors[35]);
	polygon(50+625,5+25,50+625,25+25,75+625,25+25,75+625,5+25);
	glColor3fv(colors[36]);
	polygon(50+650,5+25,50+650,25+25,75+650,25+25,75+650,5+25);
	glColor3fv(colors[37]);
	polygon(50+675,5+25,50+675,25+25,75+675,25+25,75+675,5+25);
	glColor3fv(colors[38]);
	polygon(50+700,5+25,50+700,25+25,75+700,25+25,75+700,5+25);
	glColor3fv(colors[39]);
	polygon(50+725,5+25,50+725,25+25,75+725,25+25,75+725,5+25);
	glColor3fv(colors[40]);


	//White boxes 
	glColor3f(1,1,1);
	int x=90,y=125,i;
	for(i=0;i<11;i++) //White box border for right side tools
	{    
		polygon(970+5,MAXY-x,970+5,MAXY-y,970+45,MAXY-y,970+45,MAXY-x);
		x+=45;
		y+=45;
	}

	x=90; y=125;

		for(i=0;i<11;i++) //White box border for left side tools
	{    
		polygon(900+5,MAXY-x,900+5,MAXY-y,900+45,MAXY-y,900+45,MAXY-x);
		x+=45;
		y+=45;
	}
	
	
    //displays options in the white polygons
	disp();

	

	
	
	
	}
	//for save option
	if(state1==SAVE && savec==0)
	{
		char filename[30];
		savec=1;
		glReadPixels(50,50,MAXX-50,MAXY-40,GL_RGB,GL_FLOAT,mat);
		printf("Enter the filename to save : ");
		scanf("%s",filename);
		fp=fopen(filename,"w");			
		fwrite(mat,matsize,sizeof(GLubyte),fp);
		fclose(fp);
		printf("\nFILE SAVED\n");	
		 //to retain the option selected as highlighted 
		  menu(5,45,MAXY-60,MAXY-35,colors[20]);
	      menu(55,95,MAXY-60,MAXY-35,colors[20]);
		  menu(105,145,MAXY-60,MAXY-35,colors[7]);
		  menu(155,195,MAXY-60,MAXY-35,colors[20]);
		  menu_disp();
		glFlush();
	}
	if(state1==OPEN && openc==0)
	{
		char filename[30];
		openc=1;
		printf("\nEnter filename to open : ");
		scanf("%s",filename);
		fp=fopen(filename,"r");	
		if(fp==NULL)
		{
			printf("\nFile does not exist");
			return;
		}
		fread(mat,matsize,sizeof(GLubyte),fp);
		fclose(fp);			
		
	glColor3fv(colors[0]);
	polygon(50,80,MAXX-150,80,MAXX-150,MAXY-90,50,MAXY-90);
		glFlush();
		glRasterPos2i(50,50);
		glDrawPixels(MAXX-50,MAXY-40,GL_RGB,GL_FLOAT,mat);
		printf("\nFILE OPENED\n");
		    menu(5,45,MAXY-60,MAXY-35,colors[20]);
			menu(55,95,MAXY-60,MAXY-35,colors[20]);
			menu(105,145,MAXY-60,MAXY-35,colors[7]);
			menu(155,195,MAXY-60,MAXY-35,colors[20]);
		menu_disp();
		glFlush();
	}
	if(trans_paste==1)
	{ 
		glRasterPos2i(50,50);
		glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
		if(x>=50)			
			if(y>=50)			
				if((x+px-1)<=(MAXX-50))
					if((y+py-1)<=(MAXY-40))
					{
						glColor3fv(colors[0]);
						polygon(ax,by,ax,by+py,ax+px,by+py,ax+px,by);
						glRasterPos2i(x,y);
						glDrawPixels(px-1,py-1,GL_RGB,GL_FLOAT,clip);		
					}
		glFlush();
		trans_paste=0;
	}

	if(trans_paste1==1)
	{ 
		glRasterPos2i(50,50);
		glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
		if(x>=50)			
			if(y>=50)			
				if((x+px-1)<=(MAXX-50))
					if((y+py-1)<=(MAXY-40))
					{
						//glColor3fv(colors[7]);
						//olygon(ax,by,ax,by+py,ax+px,by+py,ax+px,by);
						glRasterPos2i(x,y);
						glDrawPixels(px-1,py-1,GL_RGB,GL_FLOAT,clip);		
					}
		glFlush();
		trans_paste1=0;
	}
	
	if(scale_paste==1)
	{ 
		glRasterPos2i(50,50);
		glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
		if(x>=50)			
			if(y>=50)			
				if((x+px-1)<=(MAXX-50))
					if((y+py-1)<=(MAXY-40))
					{
						glColor3fv(colors[0]);
						polygon(ax,by,ax,by+py,ax+px,by+py,ax+px,by);
						glPixelZoom(2,2);
						glRasterPos2i(x,y);
						glDrawPixels(px-1,py-1,GL_RGB,GL_FLOAT,clip);		
					}
		glFlush();
		glPixelZoom(1,1);
		scale_paste=0;
	}
	if(state1==EXIT)
	{
		exit(0);
	}	
	if(state==GLUT_UP)
	{
		if(outsideclip)
		{
			px=elinex-linex; py=liney-eliney;
			glReadPixels(linex,eliney,px,py,GL_RGB,GL_FLOAT,clip);
			glColor3fv(colors[0]);
			polygon(50,80,MAXX-150,80,MAXX-150,MAXY-90,50,MAXY-90);
			glFlush();
			glRasterPos2i(linex,eliney);
			glDrawPixels(px,py,GL_RGB,GL_FLOAT,clip);
			outsideclip=0;
		}	
		if(insideclip)
		{
			glColor3fv(colors[0]);
			printf("%d %d %d %d",linex,liney,elinex,eliney);
			polygon(linex,liney,linex,eliney,elinex,eliney,elinex,liney);
			lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
			glFlush();
			insideclip=0;
		}	
		if(rotatel)
		{
			int i,j;
			glColor3f(1,1,1);
			lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
			ax=linex;ay=liney;bx=elinex;by=eliney;
			px=elinex-linex; py=liney-eliney;
			glReadPixels(linex,eliney+1,px-1,py-1,GL_RGB,GL_FLOAT,clip);
			refx = (elinex+linex)/2;
			refy = (eliney+liney)/2;
			for(i=linex;i<elinex-1;i++)
				for(j=liney-1;j>eliney+1;j--)
				{
					glReadPixels(i,j,1,1,GL_RGB,GL_FLOAT,pix);
					pixels[i][j]=1;
					col[i][j].red=pix[0];
					col[i][j].green=pix[1];
					col[i][j].blue=pix[2];
				}
			rot_paste = 1;
			glColor3fv(colors[0]);
			polygon(ax,by,ax,by+py,ax+px,by+py,ax+px,by);
			glPushMatrix();
			glTranslated(refx,refy,0);
			glRotated(20,0,0,1);
			glTranslated(-(refx),-(refy),0);
			drawobj();
			glPopMatrix();
			//glFlush();
			for(i=0;i<MAXX-50;i++)
				for(j=0;j<MAXY-50;j++)
				{
					if(pixels[i][j])
						pixels[i][j] = 0;
				}	
			elinex = linex = eliney = liney = rotatel = 0;
		}
		if(rotater)
		{
			int i,j;
			glColor3f(1,1,1);
			lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
			ax=linex;ay=liney;bx=elinex;by=eliney;
			px=elinex-linex; py=liney-eliney;
			glReadPixels(linex,eliney+1,px-1,py-1,GL_RGB,GL_FLOAT,clip);
			refx = (elinex+linex)/2;
			refy = (eliney+liney)/2;
			for(i=linex;i<elinex-1;i++)
				for(j=liney-1;j>eliney+1;j--)
				{
					glReadPixels(i,j,1,1,GL_RGB,GL_FLOAT,pix);
					pixels[i][j]=1;
					col[i][j].red=pix[0];
					col[i][j].green=pix[1];
					col[i][j].blue=pix[2];
				}
			rot_paste = 1;
			glColor3fv(colors[0]);
			polygon(ax,by,ax,by+py,ax+px,by+py,ax+px,by);
			glPushMatrix();
			glTranslated(refx,refy,0);
			glRotated(-20,0,0,1);
			glTranslated(-(refx),-(refy),0);
			drawobj();
			glPopMatrix();
			//glFlush();
			for(i=0;i<MAXX-50;i++)
				for(j=0;j<MAXY-50;j++)
				{
					if(pixels[i][j])
						pixels[i][j] = 0;
				}	
			elinex = linex = eliney = liney = rotater = 0;
		}
		if(reflectx)
		{
			int i,j;
			glColor3f(1,1,1);
			lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
			ax=linex;ay=liney;bx=elinex;by=eliney;
			px=elinex-linex; py=liney-eliney;
			glReadPixels(linex,eliney+1,px-1,py-1,GL_RGB,GL_FLOAT,clip);
			refx = (elinex+linex)/2;
			refy = (eliney+liney)/2;
			for(i=linex;i<elinex-1;i++)
				for(j=liney-1;j>eliney+1;j--)
				{
					glReadPixels(i,j,1,1,GL_RGB,GL_FLOAT,pix);
					pixels[i][j]=1;
					col[i][j].red=pix[0];
					col[i][j].green=pix[1];
					col[i][j].blue=pix[2];
				}
			rot_paste = 1;
			glColor3fv(colors[0]);
			polygon(ax,by,ax,by+py,ax+px,by+py,ax+px,by);
			glPushMatrix();
			glTranslated(refx,refy,0);
			glScaled(1,-1,0);
			glTranslated(-(refx),-(refy),0);
			drawobj();
			glPopMatrix();
			//glFlush();
			for(i=0;i<MAXX-50;i++)
				for(j=0;j<MAXY-50;j++)
				{
					if(pixels[i][j])
						pixels[i][j] = 0;
				}	
			elinex = linex = eliney = liney = reflectx = 0;
		}
		if(reflecty)
		{
			int i,j;
			glColor3f(1,1,1);
			lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
			ax=linex;ay=liney;bx=elinex;by=eliney;
			px=elinex-linex; py=liney-eliney;
			glReadPixels(linex,eliney+1,px-1,py-1,GL_RGB,GL_FLOAT,clip);
			refx = (elinex+linex)/2;
			refy = (eliney+liney)/2;
			for(i=linex;i<elinex-1;i++)
				for(j=liney-1;j>eliney+1;j--)
				{
					glReadPixels(i,j,1,1,GL_RGB,GL_FLOAT,pix);
					pixels[i][j]=1;
					col[i][j].red=pix[0];
					col[i][j].green=pix[1];
					col[i][j].blue=pix[2];
				}
			rot_paste = 1;
			glColor3fv(colors[0]);
			polygon(ax,by,ax,by+py,ax+px,by+py,ax+px,by);
			glPushMatrix();
			glTranslated(refx,refy,0);
			glScaled(-1,1,0);
			glTranslated(-(refx),-(refy),0);
			drawobj();
			glPopMatrix();
			//glFlush();
			for(i=0;i<MAXX-50;i++)
				for(j=0;j<MAXY-50;j++)
				{
					if(pixels[i][j])
						pixels[i][j] = 0;
				}	
			elinex = linex = eliney = liney = reflecty = 0;
		}
		if(triangle==1)
		{
			tricount=3;
			glBegin(GL_LINES);
			glVertex2i(linex,liney);
			glVertex2i(elinex,eliney);
			glEnd();
			glFlush();
		}
		if(triangle==2)
		{
			tricount=0;
			glBegin(GL_LINE_LOOP);
			glVertex2i(tri[0].x,tri[0].y);
			glVertex2i(tri[1].x,tri[1].y);
			glVertex2i(elinex,eliney);
			glEnd();
			glFlush();
			tri[0].x = 0.0; tri[0].y = 0.0;
			tri[1].x = 0.0; tri[1].y = 0.0;
			tri[2].x = 0.0; tri[2].y = 0.0;
			triangle = 0;
			linex = 0;
			liney = 0;
			elinex = 0;
			eliney = 0;
		}
		if(translate)
		{
			ax=linex;ay=liney;bx=elinex;by=eliney;
			px=elinex-linex; py=liney-eliney;
			glReadPixels(linex,eliney+1,px-1,py-1,GL_RGB,GL_FLOAT,clip);			
			trans_paste=1;
			translate=0;
		}		
		if(translate1)
		{
			ax=linex;ay=liney;bx=elinex;by=eliney;
			px=elinex-linex; py=liney-eliney;
			glReadPixels(linex,eliney+1,px-1,py-1,GL_RGB,GL_FLOAT,clip);			
			trans_paste1=1;
			translate1=0;
		}		
		if(scaling)
		{
			ax=linex;ay=liney;bx=elinex;by=eliney;
			px=elinex-linex; py=liney-eliney;
			glReadPixels(linex,eliney+1,px-1,py-1,GL_RGB,GL_FLOAT,clip);			
			scale_paste=1;
			scaling=0;
		}	
		
		if(drawline)
		{
			glBegin(GL_LINES);
			glVertex2i(linex,liney);
			glVertex2i(elinex,eliney);
			glEnd();
			glFlush();
			linex=0;
			liney=0;
			elinex=0;
			eliney=0;
			drawline=0;
		}
		if(drawrect)
		{
			lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
			linex=0;
			liney=0;
			elinex=0;
			eliney=0;
			drawrect=0;
		}
		if(drawspiral)
		{
			spiral(elinex-linex,linex,liney);
			linex=0;
			liney=0;
			elinex=0;
			eliney=0;
			drawspiral=0;
		}
		if(drawlimacon)
		{
			limacon(elinex-linex,linex,liney);
			linex=0;
			liney=0;
			elinex=0;
			eliney=0;
			drawlimacon=0;
		}
		if(drawcardoid)
		{
			cardoid(elinex-linex,linex,liney);
			linex=0;
			liney=0;
			elinex=0;
			eliney=0;
			drawcardoid=0;
		}
		if(drawleaf)
		{
			threeleaf(elinex-linex,linex,liney);
			linex=0;
			liney=0;
			elinex=0;
			eliney=0;
			drawleaf=0;
		}
		if(drawcircle)
		{
			double r=sqrt(pow((double)(elinex-linex),2)+(pow((double)(eliney-liney),2)));
			if((linex-r)<50)
				r=linex-50;
			if((liney-r)<50)
				r=liney-50;
			if((linex+r)>(MAXX-51))
				r=MAXX-51-linex;
			if((liney+r)>(MAXY-41))
				r=MAXY-41-liney;
			circle(linex,liney,r);
			glFlush();
			linex=0;
			liney=0;
			elinex=0;
			eliney=0;
			drawcircle=0;	
		}
		glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr1);
	}
	
}

void mymove(int mx,int my)
{
	GLint x=mx;
	GLint y=MAXY-my;
	//checks state and implements respective functions
	if(state1==INSIDECLIP)
	{
		if(x<MAXX-49 && x>49 && y>50 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				drawrect=1;
				insideclip=1;
				elinex=x;
				eliney=y;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
				glColor3fv(colors[0]);
			}						
		}
	}
	if(state1==ROTATEL)
	{
		if(x<MAXX-49 && x>49 && y>50 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				elinex=x;
				eliney=y;
				rotatel=1;
				drawrect=1;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
				glColor3f(0,0,0);
			}						
		}
	}
	if(state1==ROTATER)
	{
		if(x<MAXX-49 && x>49 && y>50 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				elinex=x;
				eliney=y;
				rotater=1;
				drawrect=1;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
				glColor3f(0,0,0);
			}						
		}
	}
	if(state1==REFX)
	{
		if(x<MAXX-49 && x>49 && y>50 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				elinex=x;
				eliney=y;
				reflectx=1;
				drawrect=1;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
				glColor3f(0,0,0);
			}						
		}
	}
	if(state1==REFY)
	{
		if(x<MAXX-49 && x>49 && y>50 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				elinex=x;
				eliney=y;
				reflecty=1;
				drawrect=1;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
				glColor3f(0,0,0);
			}						
		}
	}
	if(state1==SPIRAL)
	{
		if(x<MAXX-49 && x>49 && y>50 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				elinex=x;
				eliney=y;
				drawspiral=1;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				spiral(elinex-linex,linex,liney);
			}
		}
	}

	if(state1==LIMACON)
	{
		if(x<MAXX-49 && x>49 && y>50 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				elinex=x;
				eliney=y;
				drawlimacon=1;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				limacon(elinex-linex,linex,liney);
			}
		}
	}

	if(state1==CARDOID)
	{
		if(x<MAXX-49 && x>49 && y>50 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				elinex=x;
				eliney=y;
				drawcardoid=1;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				cardoid(elinex-linex,linex,liney);
			}
		}
	}
	if(state1==LEAF)
	{
		if(x<MAXX-49 && x>49 && y>50 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				elinex=x;
				eliney=y;
				drawleaf=1;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				threeleaf(elinex-linex,linex,liney);
			}
		}
	}
	if(state1==OUTSIDECLIP)
	{
		if(x<MAXX-49 && x>49 && y>50 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				drawrect=1;
				outsideclip=1;
				elinex=x;
				eliney=y;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
				glColor3fv(colors[1]);
			}						
		}
	}
	if(state1==TRANSLATE)
	{
		if(x<MAXX-49 && x>49 && y>50 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				drawrect=1;
				translate=1;
				elinex=x;
				eliney=y;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
				glColor3fv(colors[1]);
			}						
		}
	}

	if(state1==TRANSLATE1)
	{
		if(x<MAXX-49 && x>49 && y>50 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				drawrect=1;
				translate1=1;
				elinex=x;
				eliney=y;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
				glColor3fv(colors[1]);
			}						
		}
	}
	if(state1==SCALING)
	{
		if(x<MAXX-49 && x>49 && y>50 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				drawrect=1;
				scaling=1;
				elinex=x;
				eliney=y;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
				glColor3fv(colors[1]);
			}						
		}
	}
	if(state1==BRUSH)
	{
		if(x<(MAXX-57) && x>49 && y>49 && y<(MAXY-47))
		{
			GLint brushsize=8;
			glRecti(x,y,x+brushsize,y+brushsize);
		}
		glFlush();
	}
	if(state1==PENCIL)
	{
		if(x<(MAXX-49) && x>49 && y>49 && y<(MAXY-39))
		{
			glBegin(GL_POLYGON);
			glVertex2f(x,y);
			glVertex2f(x,y+1.8);
			glVertex2f(x+1.8,y+1.8);
			glVertex2f(x+1.8,y);
			glEnd();			
			glFlush();
		}
	}
	if(state1==ERAZE1)
	{

		if(x<(MAXX-57) && x>49 && y>49 && y<(MAXY-47))
		{
			glColor3f(1.0,1.0,1.0);
			GLint brushsize=8;
			glRecti(x,y,x+brushsize,y+brushsize);
		}
		glFlush();
	}
	if(state1==ERAZE2)
	{

		if(x<(MAXX-57) && x>49 && y>49 && y<(MAXY-47))
		{
			glColor3f(1.0,1.0,1.0);
			GLint brushsize=16;
			glRecti(x,y,x+brushsize,y+brushsize);
		}
		glFlush();
	}
	if(state1==ERAZE3)
	{

		if(x<(MAXX-57) && x>49 && y>49 && y<(MAXY-47))
		{
			glColor3f(1.0,1.0,1.0);
			GLint brushsize=24;
			glRecti(x,y,x+brushsize,y+brushsize);
		}
		glFlush();
	}
	if(state1==SPRAY)
	{
		if(x<(MAXX-50) && x>59 && y>54 && y<(MAXY-45))
		{
			glPointSize(1.0);
			spray(x,y);
			glFlush();
		}
	}
	if(state1==LINE)
	{
		if(x<MAXX-49 && x>49 && y>49 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				drawline=1;
				elinex=x;
				eliney=y;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				glBegin(GL_LINES);
				glVertex2i(linex,liney);
				glVertex2i(x,y);
				glEnd();
				glFlush();
			}
		}
	}
	if(state1==TRI)
	{
		if(x<MAXX-49 && x>49 && y>49 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
				tri[0].x = x;
				tri[0].y = y;
				tricount = 1;
			}
			else if(tricount!=3)
			{
				triangle=1;
				elinex=x;
				tri[1].x = x;
				eliney=y;
				tri[1].y = y;
				tricount = 2;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				glBegin(GL_LINES);
				glVertex2i(linex,liney);
				glVertex2i(x,y);
				glEnd();
				glFlush();
			}
			else if(tricount==3)
			{
				triangle=2;
				elinex=x;
				tri[2].x = x;
				eliney=y;
				tri[2].y = y;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				glBegin(GL_LINE_LOOP);
				glVertex2i(tri[0].x,tri[0].y);
				glVertex2i(tri[1].x,tri[1].y);
				glVertex2i(x,y);
				glEnd();
				glFlush();
			}
		}
	}
	if(state1==RECT)
	{
		if(x<MAXX-49 && x>49 && y>50 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				drawrect=1;
				elinex=x;
				eliney=y;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
			}
		}
	}
	if(state1==FILLRECT)
	{
		if(x<MAXX-49 && x>49 && y>49 && y<MAXY-38)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				drawline=1;
				elinex=x;
				eliney=y;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				glBegin(GL_POLYGON);
				glVertex2i(linex,liney);
				glVertex2i(linex,eliney);
				glVertex2i(elinex,eliney);
				glVertex2i(elinex,liney);
				glEnd();
     			glFlush();
			}
		}
	}
	if(state1==CIRCLE)
	{
		if(x<MAXX-49 && x>49 && y>49 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}	
			else
			{
				drawcircle=1;
				elinex=x;
				eliney=y;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
		   		double r=sqrt(pow((double)(elinex-linex),2)+(pow((double)(eliney-liney),2)));
				if((linex-r)<50)
					r=linex-50;
				if((liney-r)<50)
					r=liney-50;
				if((linex+r)>(MAXX-51))
					r=MAXX-51-linex;
				if((liney+r)>(MAXY-41))
					r=MAXY-41-liney;
				circle(linex,liney,r);				
				glFlush();			
			}
		}
	}	
}



void display()
{
	glReadPixels(100,100,MAXX-100,MAXY-100,GL_RGB,GL_FLOAT,arr1);
	glClear(GL_COLOR_BUFFER_BIT);	

	glColor3fv(colors[0]);
	polygon(50,80,MAXX-150,80,MAXX-150,MAXY-90,50,MAXY-90);

	//Outer boundary
	glColor3f(0.69,0.25,0.33);	//background colour - borders
	polygon(0,MAXY-90,0,MAXY,MAXX,MAXY,MAXX,MAXY-90); //top
	menu_disp();
    glColor3f(0.69,0.25,0.33);
	polygon(MAXX-150,80,MAXX,80,MAXX,MAXY-90,MAXX-150,MAXY-90); //right
	polygon(0,0,0,MAXY-90,50,MAXY-90,50,0);  //left
	polygon(0,0,0,80,MAXX,80,MAXX,0); //bottom

	//Colour pallette 
	glColor3fv(colors[0]);
	polygon(50,5,50,25,75,25,75,5);
	glColor3fv(colors[1]);
	polygon(50+25,5,50+25,25,75+25,25,75+25,5);
	glColor3fv(colors[2]);
	polygon(50+50,5,50+50,25,75+50,25,75+50,5);
	glColor3fv(colors[3]);
	polygon(50+75,5,50+75,25,75+75,25,75+75,5);
	glColor3fv(colors[4]);
	polygon(50+100,5,50+100,25,75+100,25,75+100,5);
	glColor3fv(colors[5]);
	polygon(50+125,5,50+125,25,75+125,25,75+125,5);
	glColor3fv(colors[6]);
	polygon(50+150,5,50+150,25,75+150,25,75+150,5);
	glColor3fv(colors[8]);
	polygon(50+200,5,50+200,25,75+200,25,75+200,5);
	glColor3fv(colors[7]);
	polygon(50+175,5,50+175,25,75+175,25,75+175,5);
	glColor3fv(colors[9]);
	polygon(50+225,5,50+225,25,75+225,25,75+225,5);
	glColor3fv(colors[10]);
	polygon(50,5+25,50,25+25,75,25+25,75,5+25);
	glColor3fv(colors[11]);
	polygon(50+25,5+25,50+25,25+25,75+25,25+25,75+25,5+25);
	glColor3fv(colors[12]);
	polygon(50+50,5+25,50+50,25+25,75+50,25+25,75+50,5+25);
	glColor3fv(colors[13]);
	polygon(50+75,5+25,50+75,25+25,75+75,25+25,75+75,5+25);
	glColor3fv(colors[14]);
	polygon(50+100,5+25,50+100,25+25,75+100,25+25,75+100,5+25);
	glColor3fv(colors[15]);
	polygon(50+125,5+25,50+125,25+25,75+125,25+25,75+125,5+25);
	glColor3fv(colors[16]);
	polygon(50+150,5+25,50+150,25+25,75+150,25+25,75+150,5+25);
	/*glColor3fv(colors[17]);
	polygon(50+200,5+25,50+200,25+25,75+200,25+25,75+200,5+25);*/
	glColor3fv(colors[17]);
	polygon(50+175,5+25,50+175,25+25,75+175,25+25,75+175,5+25);
	glColor3fv(colors[18]);
	polygon(50+200,5+25,50+200,25+25,75+200,25+25,75+200,5+25);
	glColor3fv(colors[19]);
	polygon(50+225,5+25,50+225,25+25,75+225,25+25,75+225,5+25);
	glColor3fv(colors[20]);
	polygon(50+250,5+25,50+250,25+25,75+250,25+25,75+250,5+25);
	glColor3fv(colors[21]);
	polygon(50+275,5+25,50+275,25+25,75+275,25+25,75+275,5+25);
	glColor3fv(colors[22]);
	polygon(50+300,5+25,50+300,25+25,75+300,25+25,75+300,5+25);
	glColor3fv(colors[23]);
	polygon(50+325,5+25,50+325,25+25,75+325,25+25,75+325,5+25);
	glColor3fv(colors[24]);
	polygon(50+350,5+25,50+350,25+25,75+350,25+25,75+350,5+25);
	glColor3fv(colors[25]);
	polygon(50+375,5+25,50+375,25+25,75+375,25+25,75+375,5+25);
	glColor3fv(colors[26]);
	polygon(50+400,5+25,50+400,25+25,75+400,25+25,75+400,5+25);
	glColor3fv(colors[27]);
	polygon(50+425,5+25,50+425,25+25,75+425,25+25,75+425,5+25);
	glColor3fv(colors[28]);
	polygon(50+450,5+25,50+450,25+25,75+450,25+25,75+450,5+25);
	glColor3fv(colors[29]);
	polygon(50+475,5+25,50+475,25+25,75+475,25+25,75+475,5+25);
	glColor3fv(colors[30]);
	polygon(50+500,5+25,50+500,25+25,75+500,25+25,75+500,5+25);
	glColor3fv(colors[31]);
	polygon(50+525,5+25,50+525,25+25,75+525,25+25,75+525,5+25);
	glColor3fv(colors[32]);
	polygon(50+550,5+25,50+550,25+25,75+550,25+25,75+550,5+25);
	glColor3fv(colors[33]);
	polygon(50+575,5+25,50+575,25+25,75+575,25+25,75+575,5+25);
	glColor3fv(colors[34]);
    polygon(50+600,5+25,50+600,25+25,75+600,25+25,75+600,5+25);
	glColor3fv(colors[35]);
	polygon(50+625,5+25,50+625,25+25,75+625,25+25,75+625,5+25);
	glColor3fv(colors[36]);
	polygon(50+650,5+25,50+650,25+25,75+650,25+25,75+650,5+25);
	glColor3fv(colors[37]);
	polygon(50+675,5+25,50+675,25+25,75+675,25+25,75+675,5+25);
	glColor3fv(colors[38]);
	polygon(50+700,5+25,50+700,25+25,75+700,25+25,75+700,5+25);
	glColor3fv(colors[39]);
	polygon(50+725,5+25,50+725,25+25,75+725,25+25,75+725,5+25);
	glColor3fv(colors[40]);



	//White boxes 
	glColor3fv(colors[0]);
	int x=90,y=125,i;
	for(i=0;i<11;i++) //White box border for right side tools
	{    
		polygon(970+5,MAXY-x,970+5,MAXY-y,970+45,MAXY-y,970+45,MAXY-x);
		x+=45;
		y+=45;
	}

	x=90; y=125;

		for(i=0;i<11;i++) //White box border for left side tools
	{    
		polygon(900+5,MAXY-x,900+5,MAXY-y,900+45,MAXY-y,900+45,MAXY-x);
		x+=45;
		y+=45;
	}
	
	

	disp();
	glFlush();
	
	glColor3fv(colors[1]);

	
	
	//To retain the original screen when another window is opened
	if(count!=0)
	{
		glRasterPos2i(50,50);
		glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr1);
	}
	else count++;
	draw_text(" ",MAXX-500,MAXY-10);
	

	
	glFlush();
}

void keyb(unsigned char key,int x,int y) //to change size of SPRAY
{	
	if(key=='+')
		{
			if(state1==SPRAY)size+=3;
			else if(state1==ERAZE1) state1=ERAZE2;
			else if(state1==ERAZE2) state1=ERAZE3;
		}
	if(key=='-')
		{
			if(state1==SPRAY) size-=3;
			else if(state1==ERAZE2) state1=ERAZE1;
			else if(state1==ERAZE3) state1=ERAZE2;
	}
	
}

void toggleGlutWindowMaximizeBox(char *szWindowTitle)
{
    long dwStyle;
    HWND hwndGlut;
    hwndGlut = FindWindow(NULL, (LPCWSTR)"Graphics Editor");
    dwStyle = GetWindowLong(hwndGlut, GWL_STYLE);
    dwStyle &=~WS_MAXIMIZEBOX;
    SetWindowLong(hwndGlut, GWL_STYLE, dwStyle);
}

void myReshape(int screenwidth,int screenheight)
{
	glutReshapeWindow(MAXX,MAXY);
	toggleGlutWindowMaximizeBox("Graphics Editor");
	glViewport(0, 0, MAXX, MAXY);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,MAXX,0.0,MAXY);
	glColor3f(0.0,0.0,0.0);
	
}

void main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(MAXX,MAXY);
	glutInitWindowPosition(0,0);

	glutCreateWindow("Graphics Editor");
	toggleGlutWindowMaximizeBox("Graphics Editor");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyb);
	glutMotionFunc(mymove);
	glutMainLoop();
}
