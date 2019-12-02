#include<stdio.h>
#include<stdlib.h>
#include<glut.h>
#include<math.h>
#include<Box2D\Box2D.h>


using namespace std;

#define PI 3.14
const int size = 12;
bool gravity = true;
bool isTriangle = false;
bool isSquare = false;

b2World* world;
b2MouseJointDef jointDef;
b2MouseJoint* joint;


b2Body* addTriangle(int x, int y, bool dynamic){
	b2BodyDef bodydef;

	bodydef.position.Set(x, y);
	if(dynamic){
		bodydef.type = b2_dynamicBody;
	}
	b2Body* body = world->CreateBody(&bodydef);

	b2PolygonShape shape;
	//vertex untuk membuat segitiga
	b2Vec2 vert[3];
	vert[0].Set(-15.0f, -8.659f);
	vert[1].Set(15.0f, -8.659);
	vert[2].Set(0.0f, 17.319);
	
	int32 count = 3;
	shape.Set(vert, count);

	b2FixtureDef fixturedef;
	fixturedef.shape=&shape;
	fixturedef.density=1.0;

	body->CreateFixture(&fixturedef);
	return body;
}

void drawTriangle(b2Vec2 points[3], b2Vec2 center, float angle){
	glColor3f(1, 1, 1);
	//TO DO: Modify this transformation
	glPushMatrix();
	glTranslatef(center.x, center.y, 0);
	glRotatef(angle*180/PI, 0, 0, 1);

	glBegin(GL_LINE_LOOP);
	for(int a = 0; a < 3; a++){
		glVertex2f(points[a].x, points[a].y );
	}

	glEnd();
	glPopMatrix();

	glBegin(GL_POINTS);
	glVertex2f(center.x, center.y);
	glEnd();
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

		glBegin(GL_POINTS);
	glVertex2f(center.x, center.y);
	glEnd();

}

b2Body* addRect(int x, int y, int w, int h, bool dynamic){
			b2BodyDef myBodyDef;
			if(dynamic){
			myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
			}
			myBodyDef.position.Set(x, y); //set the starting position
			myBodyDef.angle = 0; //set the starting angle

			b2Body* dynamicBody = world->CreateBody(&myBodyDef);

			b2PolygonShape boxShape;
			boxShape.SetAsBox(w,h);

			b2FixtureDef boxFixtureDef;
			boxFixtureDef.shape = &boxShape;
			boxFixtureDef.density = 1;
			dynamicBody->CreateFixture(&boxFixtureDef);
	return dynamicBody;
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

	glBegin(GL_POINTS);
	glVertex2f(center.x, center.y);
	glEnd();
}


void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();



	b2Body* node = world->GetBodyList();
	b2Vec2 points[4];
	b2Vec2 trianglePoints[3];

	b2Color color(1, 1, 1);

	while(node){
		if(node->GetFixtureList()->GetShape()->GetType() == b2Shape::e_circle){
			b2CircleShape* circle = (b2CircleShape* ) node->GetFixtureList()->GetShape();
			drawCircle(node->GetWorldCenter(), circle->m_radius, node->GetAngle());
		} else {
			int checkShape = ((b2PolygonShape*)node->GetFixtureList()->GetShape())->GetVertexCount();		

			if (checkShape == 3) {
				for (int a = 0; a < 3; a++) {
					trianglePoints[a] = ((b2PolygonShape*)node->GetFixtureList()->GetShape())->GetVertex(a);
				}
				drawTriangle(trianglePoints, node->GetWorldCenter(), node->GetAngle());
			}
			if (checkShape == 4) {
				for (int a = 0; a < 4; a++) {
					points[a] = ((b2PolygonShape*)node->GetFixtureList()->GetShape())->GetVertex(a);
				}
				drawRect(points, node->GetWorldCenter(), node->GetAngle());	
			}
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

	//dasar
	glColor3f(1, 1, 1);
	myRect = addRect(400, -5, 800, 10, false);
}


class myCallback : public b2QueryCallback{
public:
	myCallback(const b2Vec2& point){
		m_point = point;
		m_fixture = NULL;
	}

	bool ReportFixture(b2Fixture* fixture){

		bool inside = fixture->TestPoint(m_point);
		if(inside){
			m_fixture = fixture;
			return false;
		}
		return true;
	}

	b2Vec2 m_point;
	b2Fixture* m_fixture;
};

bool mouseDown = false;
b2AABB aabb;
b2Vec2 mouseVec;
b2Vec2 box;

void mouseFunc(int button, int state, int x, int y){
	x = x;
	y = 600 - y;
	mouseVec.Set(x, y);

		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			box.Set(0.001f, 0.001f);
			aabb.lowerBound = mouseVec - box;
			aabb.upperBound = mouseVec + box;
			myCallback call(mouseVec);
			world->QueryAABB(&call, aabb);

			if(call.m_fixture){
				b2Body* body = call.m_fixture->GetBody();
				b2MouseJointDef md;
				jointDef.bodyA = myRect;
				jointDef.bodyB = body;
				jointDef.target = mouseVec;
				jointDef.maxForce = 1000.0f * body->GetMass();
				joint = (b2MouseJoint*)world->CreateJoint(&jointDef);
				body->SetAwake(true);
			}

			mouseDown = true;
		}else{
			if(joint){
				world->DestroyJoint(joint);
				joint = NULL;
			}
			mouseDown = false;
		}
}

void motionFunc(int x, int y){
	x = x;
	y = 600 - y;
	if(mouseDown){
		if(joint){
			joint->SetTarget(b2Vec2(x, y));
		}
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