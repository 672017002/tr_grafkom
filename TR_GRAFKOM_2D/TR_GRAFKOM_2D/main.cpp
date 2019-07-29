#include<stdio.h>
#include<stdlib.h>
#include<glut.h>
#include<math.h>
#include<Box2D\Box2D.h>

#define PI 3.14
const int size = 13;
bool gravity = true;
bool isTriangle = false;
b2World* world;

b2Body* addTriangle(int x, int y, bool dynamic){
	b2BodyDef bodydef;
	bodydef.position.Set(x, y);
	if(dynamic){
		bodydef.type = b2_dynamicBody;
	}
	b2Body* body = world->CreateBody(&bodydef);
	b2FixtureDef fixturedef;
	b2PolygonShape shape;
	fixturedef.shape=&shape;
	fixturedef.density=1.0;

	//vertex untuk membuat segitiga
	b2Vec2 vert[3];
	vert[0] = b2Vec2(x-size, y-size);
	vert[1] = b2Vec2(x+size, y-size);
	vert[2] = b2Vec2(x, y+size);

	int32 count = 3;
	shape.Set(vert, count);

	body->CreateFixture(&fixturedef);
	return body;
}

//drawTriangle belum dicoba, error masih di addTriangle shape.set assertion fail
void drawTriangle(b2Vec2 points[3], b2Vec2 center, float angle){
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(center.x, center.y, 0);
	glRotatef(angle*180.0/PI, 0, 0, 1);
	glBegin(GL_LINE_LOOP);
	for(int a = 0; a < 3; a++){
		glVertex2f(points[a].x, points[a].y);
	}
	glEnd();
	glPopMatrix();
}

b2Body* addCircle(int x, int y, int radius, bool dynamic){
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
	return body;
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

b2Body* addRect(int x, int y, int w, int h, bool dynamic){
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
	return body;
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
	b2Vec2 trianglePoints[3];

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
		if(isTriangle){
			for(int a = 0; a < 3; a++){
			}
			isTriangle = false;
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

b2Body* myRect;
void init(void){
	glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 800.0, 0.0, 600.0);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0,0,0,1);
	world = new b2World(b2Vec2(0, -9.81));
	glColor3f(1, 1, 1);
	addRect(400, -5, 800, 10, false);
	//myRect = addRect(300, 300, size, size, true);

}
bool mouseDown = false;
void mouseFunc(int button, int state, int x, int y){
	x = x;
	y = 600 - y;
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			mouseDown = true;
		}else{
			mouseDown = false;
		}
}

void motionFunc(int x, int y){
	if(mouseDown){
		//printf("asdasd");
		b2Vec2 mouse(x, 600-y);
		myRect->ApplyForceToCenter(mouse, true);
	}
}

void keyboardFunc(unsigned char key, int x, int y){
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
		isTriangle = true;
		addTriangle(x, y, true);
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
	glutPostRedisplay();
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
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);
    glutKeyboardFunc(keyboardFunc);

	glutMainLoop();
	return 0;
}