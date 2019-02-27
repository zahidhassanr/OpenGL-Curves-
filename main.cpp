#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

using namespace std;
struct point2d
{
    double x, y;
};

struct point2d cp[200];

struct point2d movePoint[100000000];
int cpidx;
int pos=0;
int poinIndex=0;
int CurrentState=0;
bool show=true;

bool curve=false;
bool selected=false;
int SelectIndex;



void drawSphere(double radius,int slices,int stacks)
{
	struct point2d points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		//h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			//points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,0);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,0);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,0);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,0);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,0);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,0);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,0);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,0);
			}glEnd();
		}
	}
}

int** MatMult(int Matrix1[][4],int Matrix2[][1])
{
        int sum=0;
		int** result;
        result = new int*[4];
        for(int i=0;i<4;i++)
            result[i]=new int[4];

		for(int i=0;i<1;i++)
		{
			for(int j=0;j<4;j++)
			{
				for(int k=0;k<4;k++)
				{
					sum=sum+(Matrix2[k][i]*Matrix1[j][k]);
				}
				result[j][i]=sum;
				sum=0;
			}
		}
		return result;
}

void drawSquare()
{
    glBegin(GL_QUADS);
    {
        glVertex3d( 3,  3, 0);
        glVertex3d( 3, -3, 0);
        glVertex3d(-3, -3, 0);
        glVertex3d(-3,  3, 0);
    }
    glEnd();
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

        case 'u':
            if(curve==true){
            CurrentState=1;
            pos=0;
            cout<<"U pressed"<<endl;
            }
			break;
        case 'a':
            if(CurrentState==4)
                CurrentState=-1;
            else{
                CurrentState=4;
                selected=false;
            }
            poinIndex=0;
            cout<<"A pressed"<<endl;
			break;

			case 'g':
            if(show==false)
                show=true;
            else
                show=false;
			break;
		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			break;
		case GLUT_KEY_UP:		// up arrow key
			break;

		case GLUT_KEY_RIGHT:
			break;
		case GLUT_KEY_LEFT:
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void drawSelected()
{
    if(selected==true)
    {
        glPushMatrix();{
        glTranslatef(cp[SelectIndex].x,cp[SelectIndex].y,0);

        drawSphere(8,20,50);
        }glPopMatrix();
    }
}


void drawCurve(){

    if(curve==true  ){

                for(int i=0;i<cpidx;i=i+2)
                {
                    if(i==0)
                    {
                        pos=0;
                    }
                    int Hermit[4][4]={

                                        2, -2, 1, 1,
                                       -3, 3, -2,-1,
                                        0, 0, 1, 0,
                                        1, 0, 0, 0
                                    };

                    int Vertex_mat_x[4][1],Vertex_mat_y[4][1];

                    if(i==cpidx-2)
                    {
                        Vertex_mat_x[0][0]=cp[i].x;
                        Vertex_mat_x[1][0]=cp[0].x;
                        Vertex_mat_x[2][0]=cp[i+1].x-cp[i].x;
                        Vertex_mat_x[3][0]=cp[1].x-cp[0].x;

                        Vertex_mat_y[0][0]=cp[i].y;
                        Vertex_mat_y[1][0]=cp[0].y;
                        Vertex_mat_y[2][0]=cp[i+1].y-cp[i].y;
                        Vertex_mat_y[3][0]=cp[1].y-cp[0].y;
                    }
                    else{
                    Vertex_mat_x[0][0]=cp[i].x;
                    Vertex_mat_x[1][0]=cp[i+2].x;
                    Vertex_mat_x[2][0]=cp[i+1].x-cp[i].x;
                    Vertex_mat_x[3][0]=cp[i+3].x-cp[i+2].x;

                    Vertex_mat_y[0][0]=cp[i].y;
                    Vertex_mat_y[1][0]=cp[i+2].y;
                    Vertex_mat_y[2][0]=cp[i+1].y-cp[i].y;
                    Vertex_mat_y[3][0]=cp[i+3].y-cp[i+2].y;

                    }
                    int** result_x;
                    result_x = new int*[4];
                    for(int j=0;j<4;j++)
                        result_x[j]=new int[4];


                        int** result_y;
                    result_y = new int*[4];
                    for(int j=0;j<4;j++)
                        result_y[j]=new int[4];




                    result_x=MatMult(Hermit,Vertex_mat_x);
                    result_y=MatMult(Hermit,Vertex_mat_y);




                    int Ax,Bx,Cx,Dx,Ay,By,Cy,Dy;
                    Ax=result_x[0][0];
                    Bx=result_x[1][0];
                    Cx=result_x[2][0];
                    Dx=result_x[3][0];

                    //cout<<Ax<<" "<<Bx<<" "<<Cx<<" "<<Dx<<endl;

                    Ay=result_y[0][0];
                    By=result_y[1][0];
                    Cy=result_y[2][0];
                    Dy=result_y[3][0];


                    double t=0.001;
                    double slice=0.01;
                    double x1_value,x2_value,y1_value,y2_value;
                    int temp=cpidx/2;
//                    for(int k=0;k<100;k++)
//                    {
//                        x1_value=Ax*pow(t,3)+Bx*t*t+Cx*t+Dx;
//                        y1_value=Ay*pow(t,3)+By*t*t+Cy*t+Dy;
//
//                        t=t+slice;
//
//                        x2_value=Ax*pow(t,3)+Bx*pow(t,2)+Cx*t+Dx;
//                        y2_value=Ay*pow(t,3)+By*pow(t,2)+Cy*t+Dy;
//
//                        if(pos<=(temp*100))
//                        {
//                            movePoint[pos].x=x2_value;
//                            movePoint[pos++].y=y2_value;
//                        }
//
//                        glColor3f(0,1,0);
//                        glBegin(GL_LINES);{
//                            glVertex3f(x1_value,y1_value,0);
//                            glVertex3f(x2_value,y2_value,0);
//                        }glEnd();
//
//
//                    }



                // Forward Method

                       double f_x=Dx;
                       double f_y=Dy;
                       double del_fx=Ax*pow(t,3)+Bx*t*t+Cx*t;
                       double del_fy=Ay*pow(t,3)+By*pow(t,2)+Cy*t;

                       double del_f_2x=6*Ax*pow(t,3)+2*Bx*pow(t,2);
                       double del_f_2y=6*Ay*pow(t,3)+2*By*pow(t,2);

                       double del_f_3x=6*Ax*pow(t,3);
                       double del_f_3y=6*Ay*pow(t,3);

                       glBegin(GL_POINTS);{
                            glVertex3f(f_x,f_y,0);

                        }glEnd();

                        for(int k=0;k<=1000;k++)
                        {
                            f_x+=del_fx;
                            f_y+=del_fy;

                            del_fx+=del_f_2x;
                            del_fy+=del_f_2y;

                            del_f_2x+=del_f_3x;
                            del_f_2y+=del_f_3y;
                            glColor3f(1,1,0);
                            glBegin(GL_POINTS);{
                            glVertex3f(f_x,f_y,0);

                        }glEnd();

                        if(pos<=(temp*1000))
                        {
                            movePoint[pos].x=f_x;
                            movePoint[pos++].y=f_y;
                        }

                        }


            }
}

}
void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN && CurrentState==0){		// 2 times?? in ONE click? -- solution is checking DOWN or UP

                std::cout << x << " " << y << std::endl;

                cp[cpidx].x = (double)x;
                cp[cpidx].y = (double)(600 - y);
                cpidx++;
			}

			if(state == GLUT_DOWN  && CurrentState==2)
            {
                double x_pos=(double)x;
                double y_pos=(double)(600-y);

                cp[SelectIndex].x=x_pos;
                cp[SelectIndex].y=y_pos;
                CurrentState=-1;
            }

			if(state == GLUT_DOWN  && CurrentState==1)
            {

                double x_pos=(double)x;
                double y_pos=(double)(600-y);
                int Index=0;
                double Min_dist=sqrt(pow(x_pos-cp[0].x,2)+pow(y_pos-cp[0].y,2));
                for(int i=1;i<cpidx;i++)
                {
                    double curr_dist=sqrt(pow(x_pos-cp[i].x,2)+pow(y_pos-cp[i].y,2));
                    if(curr_dist < Min_dist)
                    {
                        Min_dist=curr_dist;
                        Index=i;
                    }
                }
                SelectIndex=Index;
                selected=true;
                CurrentState=2;

            }



			break;

		case GLUT_RIGHT_BUTTON:
			//........
if(state == GLUT_DOWN && CurrentState==0){

    if(cpidx >=4 && cpidx%2==0)
    {
        curve=true;
        CurrentState=-1;
    }
}


			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(150*cos(cameraAngle), 150*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(0,0,0,	0,0,-1,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects


	int i;
if(show==true){
    for (i = 0; i < cpidx; i++)
    {
        glColor3f(1, 1, 0);
        glPushMatrix();
        {
            glTranslatef(cp[i].x, cp[i].y, 0);
            drawSquare();
        }
        glPopMatrix();

        if(i%2==1)
        {
            glColor3f(1, 0, 0);

            glBegin(GL_LINES);{
                glVertex3f(cp[i].x,cp[i].y,0);
                glVertex3f(cp[i-1].x,cp[i-1].y,0);
            }glEnd();


            int x_co=cp[i].x-cp[i-1].x;
            int y_co=cp[i].y-cp[i-1].y;

            double cos=(x_co)/sqrt(x_co*x_co+y_co*y_co);
            double theta=acos(cos)*180/3.1416;

            glPushMatrix();{
            glColor3f(0,1,0);
            glTranslatef(cp[i].x, cp[i].y-5, 0);
            if(cp[i].y > cp[i-1].y)
                glRotatef(theta,0,0,1);
            else
                glRotatef(-theta,0,0,1);
            glBegin(GL_TRIANGLES);{glVertex3f(0,8,0);
                glVertex3f(8,0,0);

                glVertex3f(0,-8,0);

            }glEnd();
            }glPopMatrix();


        }
    }
}
    drawCurve();

    drawSelected();

    if(CurrentState==4)
    {   glTranslatef(movePoint[poinIndex].x,movePoint[poinIndex].y,0);
        drawSphere(5,20,30);
    }



	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){

    int temp=cpidx/2;
    if(poinIndex<temp*1000)
        poinIndex++;
    else
        poinIndex=0;
//    for(int i=0;i<2000;i++)
//        for(int j=0;j<2000;j++);
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization

	cpidx = 0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluOrtho2D(0, 800, 0, 600);
	//gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
