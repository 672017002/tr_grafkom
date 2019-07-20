#include<stdio.h>
#include<stdlib.h>
#include<glut.h>
#include<math.h>

#define PI 3.14
int size = 13;
bool check = false;

struct shapes *node = NULL, *head = NULL, *curr = NULL;
struct shapes{
	int x, y, shapeInd;
	shapes *next;
};

void init(void){
	glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluOrtho2D(0.0, 800.0, 0.0, 600.0);
}

void makeLink(int x, int y, int shapeInd){
	node = (struct shapes*) malloc(sizeof(struct shapes));
	node->x = x;
	node->y = y;
	node->shapeInd = shapeInd;
	node->next = NULL;

	if(head == NULL){
		head = node;
		curr = node;
	}else{
		curr->next = node;
		curr = node;
	}
}

void drawShapes(int x, int y, int shapeInd){
	glBegin(GL_LINE_LOOP);
	switch(shapeInd){
	case 0:
		glVertex2f(x,y+size);
		glVertex2f(x+size,y-size);
		glVertex2f(x-size,y-size);
		break;
	case 1:
		glVertex2f(x+size,y+size);
		glVertex2f(x+size,y-size);
		glVertex2f(x-size,y-size);
		glVertex2f(x-size, y+size);
		break;
	case 2:
		int i;
		int lineAmount = 100;
	
		GLfloat twicePi = 2.0f * PI;
	
			for(i = 0; i <= lineAmount;i++) { 
				glVertex2f(
					x + (size * cos(i *  twicePi / lineAmount)), 
					y + (size * sin(i * twicePi / lineAmount))
				);
			}
			break;
		}
		glEnd();
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);

	node = head;
	if(check){
		glColor3f(1, 1, 1);
		while(node != NULL){
			drawShapes(node->x, node->y, node->shapeInd);
			node = node->next;
		}
	}
	glutSwapBuffers();
}

void keyFunc(unsigned char key, int x, int y){
	check = true;
	x = x;
	y = 600 - y;
	switch(key){
	case 'a':
		printf("Segitiga\n");
		makeLink(x, y, 0);
		glutPostRedisplay();
		break;
	case 's':
		printf("Kotak\n");
		makeLink(x, y, 1);
		glutPostRedisplay();
		break;
	case 'd':
		printf("Lingkaran\n");
		makeLink(x, y, 2);
		glutPostRedisplay();
		break;
	case 'r':
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		system("cls");
		check = 0;
		head = NULL;
		printf("PRESS R TO RESET\n\n");
		glutPostRedisplay();
		break;
	case 27:
		exit(1);
	}
}

int main(int argc, char** argv){
	printf("PRESS R TO RESET\n\n");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glMatrixMode(GL_PROJECTION);
	glutInitWindowSize(800, 600);
	glutCreateWindow("TR_GRAFKOM_2D");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyFunc);
	glutMainLoop();
	return 0;
}