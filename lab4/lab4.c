// Demo of heavily simplified sprite engine
// by Ingemar Ragnemalm 2009
// used as base for lab 4 in TSBK03.
// OpenGL 3 conversion 2013.

//MAC
//gcc lab4.c -o lab4 ../common/*.c ../common/Mac/MicroGlut.m -I../common SpriteLight.c -framework OpenGL -framework Cocoa -Wno-deprecated-declarations

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "Mac/MicroGlut.h"
	// uses framework Cocoa
#else
	#include <GL/gl.h>
	#include "MicroGlut.h"
#endif

#include <stdlib.h>
#include "LoadTGA.h"
#include "SpriteLight.h"
#include "GL_utilities.h"
#include <math.h>

// L�gg till egna globaler h�r efter behov.
float kMaxDistance = 110.0f;
float kCohesionWeight = 0.0015f;
float kAvoidanceWeight = 0.7f;
float kAlignmentWeight = 0.08f;
float kRandom = 2.0f;
float kfoodWeight = 0.9f;
float kMaxSpeed = 2.0f;

TextureData *sheepFace, *blackSheepFace, *foodFace;


SpriteRec *blackSheep;
SpriteRec *food;

float CalDist(SpritePtr p1, SpritePtr p2){
	float d1 = pow(p2->position.h - p1->position.h,2);
	float d2 = pow(p2->position.v - p1->position.v,2);

	return sqrt(d1+d2);

}

FPoint CalcAvoidance(SpritePtr p1, SpritePtr p2){
	
	FPoint a = (FPoint){
		p1->position.h - p2->position.h,
		p1->position.v - p2->position.v
	};
	float length = sqrt(pow(a.h, 2.0f) + pow(a.v, 2.0f));

	a.h *= (1.0f - length/100)/length;
	a.v *= (1.0f - length/100)/length;

	return a;
}

void SpriteBehavior() // Din kod!
{
// L�gg till din labbkod h�r. Det g�r bra att �ndra var som helst i
// koden i �vrigt, men mycket kan samlas h�r. Du kan utg� fr�n den
// globala listroten, gSpriteRoot, f�r att kontrollera alla sprites
// hastigheter och positioner, eller arbeta fr�n egna globaler.

SpritePtr p1 = gSpriteRoot;
SpritePtr p2;

while(p1 != NULL){
	
	if(p1->face == foodFace){
		p1 = p1->next;
	}

	int count = 0;	
	// For Cohesion
	p1->averagePosition.h = 0;
	p1->averagePosition.v = 0;

	// For seperation
	p1->avoidanceVector.h = 0;
	p1->avoidanceVector.v = 0;

	// For Alignment
	p1->speedDiff.h = 0;
	p1->speedDiff.v = 0;

	// for Food 
	p1->foodPullVector.h = 0;
	p1->foodPullVector.v = 0;	

	p2 = gSpriteRoot;

	while(p2 != NULL){
		
		if(p2 != p1 && CalDist(p1,p2) < kMaxDistance ){
			// FOOOOOD!!
			if(p2->face == foodFace){
				float diff_h = p1->position.h - p2->position.h;
				float diff_v = p1->position.v - p2->position.v;
	
				float distance = sqrt(diff_h*diff_h + diff_v*diff_v);
				// New position when eaten
				if(distance < 12){
					p2->position.h = random() % 600;
					p2->position.v = random() % 800;
				}

				p1->foodPullVector.h += 1/(distance) * 0.2 * (p2->position.h - p1->position.h);
				p1->foodPullVector.v += 1/(distance) * 0.2 * (p2->position.v - p1->position.v);
				
				p2 = p2->next;
				continue;
			}
			
			// For Cohesion
			p1->averagePosition.h += p2->position.h;
			p1->averagePosition.v += p2->position.v;

			// For seperation
			p1-> avoidanceVector.h += CalcAvoidance(p1,p2).h;
			p1-> avoidanceVector.v += CalcAvoidance(p1,p2).v;
			
			// For Alignment
			p1->speedDiff.h += p2->speed.h - p1->speed.h;
			p1->speedDiff.v += p2->speed.v - p1->speed.v;

			count += 1;
		}
		
		p2 = p2->next;
	}

	if(count > 0){

		if(p1->face == foodFace){
			p1 = p1->next;
			continue;
		}
		// For Cohesion
		p1->averagePosition.h /= count;
		p1->averagePosition.v /= count;

		//For Seperation
		p1->avoidanceVector.h /= count;
		p1->avoidanceVector.v /= count;

		//For Alignment
		p1->speedDiff.h /= count;
		p1->speedDiff.v /= count;

	}
	p1 = p1->next;
}

// Second loop for adding the resulting contribution
SpritePtr p = gSpriteRoot;
while(p != NULL){
	
	if(p->face == foodFace){
		p = p->next;
		continue;
	}
		// Cohesion
		p->speed.h += (p->averagePosition.h - p->position.h) * kCohesionWeight;
		p->speed.v += (p->averagePosition.v - p->position.v) * kCohesionWeight;

		// For Seperation
		p->speed.h += p->avoidanceVector.h * kAvoidanceWeight;
		p->speed.v += p->avoidanceVector.v * kAvoidanceWeight;

		// For Alignment
		p->speed.h += p->speedDiff.h * kAlignmentWeight;
		p->speed.v += p->speedDiff.v * kAlignmentWeight;

		// For Food
		p->speed.h += p->foodPullVector.h * kfoodWeight;
		p->speed.v += p->foodPullVector.v * kfoodWeight;

		p->position.h += p->speed.h;
		p->position.v += p->speed.v;
	
	if (p == blackSheep)
	{
		
		p->speed.h += kRandom * (random() / (float)RAND_MAX) - kRandom/2.0f;
		p->speed.v += kRandom * (random() / (float)RAND_MAX) - kRandom/2.0f;

		
	}
	// To prevent too high speed
	if(p->speed.h > kMaxSpeed){
		p->speed.h = kMaxSpeed;

	}
	if(p->speed.v > kMaxSpeed){
		p->speed.v = kMaxSpeed;

	}
	

	p = p->next;
}


}

// Drawing routine
void Display()
{
	SpritePtr sp;
	
	glClearColor(0, 0, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	DrawBackground();
	
	SpriteBehavior(); // Din kod!
	
// Loop though all sprites. (Several loops in real engine.)
	sp = gSpriteRoot;
	do
	{
		HandleSprite(sp); // Callback in a real engine
		DrawSprite(sp);
		sp = sp->next;
	} while (sp != NULL);
	
	glutSwapBuffers();
}

void Reshape(int h, int v)
{
	glViewport(0, 0, h, v);
	gWidth = h;
	gHeight = v;
}

void Timer(int value)
{
	glutTimerFunc(20, Timer, 0);
	glutPostRedisplay();
}

void spawnFood(){
	foodFace = GetFace("bilder/mat.tga");
	NewSprite(foodFace, 200, 200, 0, 0);
	
}
// Example of user controllable parameter
float someValue = 0.0;

void Key(unsigned char key,
         __attribute__((unused)) int x,
         __attribute__((unused)) int y)
{
  switch (key)
  {
    case '+':
    	someValue += 0.1;
    	printf("someValue = %f\n", someValue);
    	break;
    case '-':
    	someValue -= 0.1;
    	printf("someValue = %f\n", someValue);
    	break;
	case 'm':
		spawnFood();
		
		
		break;
	
    case 0x1b:
      exit(0);
  }
}

void Init()
{
	TextureData *sheepFace, *blackSheepFace, *dogFace, *foodFace;
	
	LoadTGATextureSimple("bilder/leaves.tga", &backgroundTexID); // Bakgrund
	
	sheepFace = GetFace("bilder/sheep.tga"); // Ett f�r
	blackSheepFace = GetFace("bilder/blackie.tga"); // Ett svart f�r
	dogFace = GetFace("bilder/dog.tga"); // En hund
	foodFace = GetFace("bilder/mat.tga"); // Mat
	
	NewSprite(sheepFace, 100, 200, 1, 1);
	NewSprite(sheepFace, 200, 100, 1.5, -1);
	NewSprite(sheepFace, 150, 200, -1, 1.5);
	NewSprite(sheepFace, 150, 100, 1, 1);
	NewSprite(sheepFace, 200, 200, 1.5, -1);
	NewSprite(sheepFace, 50, 100, -1, 1.5);
	//blackSheep = NewSprite(blackSheepFace, 150, 50, -1, 1.5);
	//NewSprite(foodFace, 20, 200, 0, 0);
}



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 2);
	glutCreateWindow("SpriteLight demo / Flocking");
	
	glutDisplayFunc(Display);
	glutTimerFunc(20, Timer, 0); // Should match the screen synch
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);
	
	InitSpriteLight();
	Init();
	
	glutMainLoop();
	return 0;
}
