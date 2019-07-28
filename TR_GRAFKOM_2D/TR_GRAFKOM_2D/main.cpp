#include<stdio.h>
#include<stdlib.h>
#include<glut.h>
#include<math.h>
#include<Box2D\Box2D.h>

#define PI 3.14
const int size = 13;
bool gravity = true;
b2World* world;

void addTriangle(int x, int y, int w, int h, bool dynamic){
}

void drawTriangle(){
}

void addCircle(int x, int y, int radius, bool dynamic){
	b2BodyDef bodydef;
	bodydef.position.Set(x, y);
	if(dynamic){
		bodydef.type = b2_dynamicBody;
	}

	b2Body* body = world->CreateBody(&bodydef);

	b2CircleShape shape;
	shape.m_radius=radius;
	shape.m_p.Set(0,0);

	b2FixtureDef fixturedef;
	fixturedef.shape=&shape;
	fixturedef.density=1.0;
	body->CreateFixture(&fixturedef);
}

void drawCircle(b2Vec2 center, float radius, float angle){
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(center.x, center.y, 0);
	glRotatef(angle*180/PI, 0, 0, 1);

	glBegin(GL_LINE_LOOP);
		for(int a = 0; a <= 360; a++){
			glVertex2f(cos(a*PI/180.0)*radius, (sin(a*PI/180.0)*radius));
		}
	glEnd();

	glPopMatrix();
}

void addRect(int x, int y, int w, int h, bool dynamic){
	b2BodyDef bodydef;
	bodydef.position.Set(x, y);
	if(dynamic){
		bodydef.type = b2_dynamicBody;
	}
	b2Body* body = world->CreateBody(&bodydef);
	
	b2PolygonShape shape;
	shape.SetAsBox(w, h);

	b2FixtureDef fixturedef;
	fixturedef.shape=&shape;
	fixturedef.density=1.0;
	body->CreateFixture(&fixturedef);
}

void drawRect(b2Vec2 points[4], b2Vec2 center, float angle){
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(center.x, center.y, 0);
	glRotatef(angle*180.0/PI, 0, 0, 1);
	glBegin(GL_LINE_LOOP);
	for(int a = 0; a < 4; a++){
		glVertex2f(points[a].x, points[a].y);
	}
	glEnd();
	glPopMatrix();
}
void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	b2Body* node = world->GetBodyList();
	b2Vec2 points[4];

	while(node){
		if(node->GetFixtureList()->GetShape()->GetType() == 0){
			b2CircleShape* circle = (b2CircleShape* ) node->GetFixtureList()->GetShape();
			drawCircle(node->GetWorldCenter(), circle->m_radius, node->GetAngle());
		}else{
			for(int a = 0; a < 4; a++){
				points[a] = ((b2PolygonShape* ) node->GetFixtureList()->GetShape())->GetVertex(a);
			}
			drawRect(points, node->GetWorldCenter(), node->GetAngle());
		}
		node = node->GetNext();
	}

	glutSwapBuffers();
}

void loop(int v){
		world->Step(0.02, 8, 3);
		glutPostRedisplay();
		glutTimerFunc(1, loop, 0);
}

void keyFunc(unsigned char key, int x, int y){
	x = x;
	y = 600 - y;
	switch(key){
	case 'A':
	case 'a':
		addRect(x, y, size, size, true);
		break;
	case 'S':
	case 's':
		addCircle(x, y, size, true);
		break;
	case 'D':
	case 'd':
		break;
	case 'F':
	case 'f':
		if(gravity){	
			glutTimerFunc(1, loop, 0);
			gravity = false;
		}
		break;
	case 27:
		exit(1);
	}
	world->Step(0.01, 5, 5); //redisplay versi box2d
	glutPostRedisplay();
}

void init(void){
	glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 800.0, 0.0, 600.0);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0,0,0,1);
	world = new b2World(b2Vec2(0, -9.81));
	glColor3f(1, 1, 1);
	addRect(400, -5, 800, 10, false);
}

int main(int argc, char** argv){
	printf("PRESS F TO ENABLE GRAVITY\n\n");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glMatrixMode(GL_PROJECTION);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(300 ,100);
	glutCreateWindow("TR_GRAFKOM_2D");
	
	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyFunc);

	glutMainLoop();
	return 0;
}