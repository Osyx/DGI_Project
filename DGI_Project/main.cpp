#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <GL/glut.h>
#include <glm\glm\glm.hpp>
#include <vector>
#include <algorithm>

const int SCREEN_WIDTH = 300;
const int SCREEN_HEIGHT = 340;
glm::vec3 camera(0.0f, 0.0f, 0.9f);
glm::vec3 lookat(0.0f, 0.0f, 0.0f);

float headSize = 20.0f;
float bodyHeight = 70.0f;
float limbSize = bodyHeight/100.0f * 20.0f;
float armLength = limbSize * 10.0f;
float legLength = armLength;

void init() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluOrtho2D(0, 400, 0, 500);
}

void updateValues() {
	limbSize = bodyHeight / 100.0f * 7.0f;
	armLength = limbSize * 10.0f;
	legLength = armLength;
}

void drawFigure() {

	init();
	updateValues();
	gluLookAt(camera.x, camera.y, camera.z, lookat.x, lookat.y, lookat.z, 0, 1, 0);
	glPushMatrix();
		
		// Body
		glTranslatef(SCREEN_WIDTH / 2 + limbSize / 2, SCREEN_HEIGHT - bodyHeight + legLength, 0);
		/*
		glBegin(GL_QUADS);
		
		//top
		glColor3f(0.0f, 0.0f, 0.8f);
		glVertex3f(-bodyHeight/2, bodyHeight, -10);
		glVertex3f(bodyHeight/2, bodyHeight, -10);
		glVertex3f(bodyHeight/2, bodyHeight, 10);
		glVertex3f(-bodyHeight/2, bodyHeight, 10);

		//bot
		glColor3f(0.0f, 0.0f, 0.8f);
		glVertex3f(-bodyHeight / 2, -bodyHeight, 10);
		glVertex3f(bodyHeight / 2, -bodyHeight, 10);
		glVertex3f(bodyHeight / 2, -bodyHeight, -10);
		glVertex3f(-bodyHeight / 2, -bodyHeight, -10);

		//front
		glColor3f(0.0f, 0.8f, 0.8f);
		glVertex3f(-bodyHeight / 2, bodyHeight, 10);
		glVertex3f(bodyHeight / 2, bodyHeight, 10);
		glVertex3f(bodyHeight / 2, -bodyHeight, 10);
		glVertex3f(-bodyHeight / 2, -bodyHeight, 10);

		//back
		glColor3f(0.0f, 0.8f, 0.8f);
		glVertex3f(-bodyHeight / 2, -bodyHeight, -10);
		glVertex3f(bodyHeight / 2, -bodyHeight, -10);
		glVertex3f(bodyHeight / 2, bodyHeight, -10);
		glVertex3f(-bodyHeight / 2, bodyHeight, -10);

		//left
		glColor3f(0.0f, 0.8f, 0.0f);
		glVertex3f(-bodyHeight / 2, bodyHeight, -10);
		glVertex3f(-bodyHeight / 2, bodyHeight, 10);
		glVertex3f(-bodyHeight / 2, -bodyHeight, 10);
		glVertex3f(-bodyHeight / 2, -bodyHeight, -10);

		//right
		glColor3f(0.8f, 0.0f, 0.8f);
		glVertex3f(bodyHeight / 2, bodyHeight, 10);
		glVertex3f(bodyHeight / 2, bodyHeight, -10);
		glVertex3f(bodyHeight / 2, -bodyHeight, -10);
		glVertex3f(bodyHeight / 2, -bodyHeight, 10);

		glEnd();
		*/
		/*
		glBegin(GL_POLYGON);
		glColor3f(0.0f, 0.8f, 0.8f);
		glVertex2f(-bodyHeight / 3, bodyHeight);
		glVertex2f(bodyHeight / 3, bodyHeight);
		glVertex2f(bodyHeight / 3, -bodyHeight);
		glVertex2f(-bodyHeight / 3, -bodyHeight);
		glEnd();
		*/

		glPushMatrix();
		glTranslatef(0, bodyHeight, 0);
		glLineWidth(limbSize);
		glColor3f(0.0, 0.8, 0.8);
		glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0, -bodyHeight * 2, 0);
		glEnd();
		glPopMatrix();


		// Head
		glPushMatrix();
			glTranslatef(0, bodyHeight + headSize, 0);
			glColor3f(1.0f, 0.0f, 0.0f);
			glutSolidSphere(headSize, 20, 2);
		glPopMatrix();

		// Arms
		glPushMatrix();
			glTranslatef(0, bodyHeight - limbSize, 0);
			glLineWidth(limbSize);
			glColor3f(0.0, 1.0, 0.0);
			glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(armLength, 0, 0);
			glEnd();
			glPushMatrix();
				glTranslatef(armLength, 0, 0);
				glLineWidth(limbSize);
				glColor3f(1.0, 0.0, 0.0);
				glBegin(GL_LINES);
				glVertex3f(0.0, 0.0, 0.0);
				glVertex3f(armLength, 0, 0);
				glEnd();
			glPopMatrix();

			glLineWidth(limbSize);
			glColor3f(0.0, 1.0, 0.0);
			glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(-armLength, 0, 0);
			glEnd();
			glPushMatrix();
				glTranslatef(-armLength, 0, 0);
				glLineWidth(limbSize);
				glColor3f(1.0, 0.0, 0.0);
				glBegin(GL_LINES);
				glVertex3f(0.0, 0.0, 0.0);
				glVertex3f(-armLength, 0, 0);
				glEnd();
			glPopMatrix();

		glPopMatrix();

		// Legs
		glPushMatrix();
			glTranslatef(0, -bodyHeight, 0);
			glLineWidth(limbSize);
			glColor3f(0.0, 1.0, 0.0);
			glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(10, -legLength, 0);
			glEnd();
			glPushMatrix();
				glTranslatef(10, -legLength, 0);
				glLineWidth(limbSize);
				glColor3f(1.0, 0.0, 0.0);
				glBegin(GL_LINES);
				glVertex3f(0.0, 0.0, 0.0);
				glVertex3f(10, -legLength, 0);
				glEnd();
			glPopMatrix();

			glTranslatef(0, 0, 0);
			glLineWidth(limbSize);
			glColor3f(0.0, 1.0, 0.0);
			glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(-10, -legLength, 0);
			glEnd();
			glPushMatrix();
				glTranslatef(-10, -legLength, 0);
				glLineWidth(limbSize);
				glColor3f(1.0, 0.0, 0.0);
				glBegin(GL_LINES);
				glVertex3f(0.0, 0.0, 0.0);
				glVertex3f(-10, -legLength, 0);
				glEnd();
			glPopMatrix();

		glPopMatrix();


		glFlush();

	glPopMatrix();

	glutSwapBuffers();
	
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		camera.z -= 0.1f;
		init();
		glutPostRedisplay();
		break;
	case 'd':
		camera.z += 0.1f;
		init();
		glutPostRedisplay();
		break;
	case 'w':
		bodyHeight += 1;
		glutPostRedisplay();
		break;
	case 's':
		bodyHeight -= 1;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

int  main() {
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(500, 500);
	glutCreateWindow("Figure");
	glutDisplayFunc(drawFigure);
	//glutIdleFunc(drawFigure);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}



///***************************************************************************************
/// SimplifyAngle
/// This function will convert an angle to the equivalent rotation in the range [-pi,pi]
///***************************************************************************************
float SimplifyAngle(float angle) {
	angle = fmod(angle, (2.0 * M_PI));
	if (angle < -M_PI)
		angle += (2.0 * M_PI);
	else if (angle > M_PI)
		angle -= (2.0 * M_PI);
	return angle;
}

///***************************************************************************************
/// Bone_Calc
/// This class is used internally by the calcCCD function to represent a bone in
/// world space.
///***************************************************************************************
class Bone_Calc {
public: 
	float x;        // x position in world space
	float y;        // y position in world space
	float angle;    // angle in world space
	float cosAngle; // sine of angle
	float sinAngle; // cosine of angle
};

///***************************************************************************************
/// Result
/// This enum represents the resulting state of a CCD iteration.
///***************************************************************************************
enum Result {
	Success,    // the target was reached
	Processing, // still trying to reach the target
	Failure    // failed to reach the target
};

///***************************************************************************************
/// Bone
/// This class is used to supply the CalcIK_2D_CCD function with a bone's representation
/// relative to its parent in the kinematic chain.
///***************************************************************************************
class Bone;

class Bone {
public: 
	float x;     // x position in parent space
	float y;     // y position in parent space
	float angle; // angle in parent space
	Bone* parent = NULL;
	Bone(float _x, float _y, float _angle, Bone* _parent): x(_x), y(_y), angle(_angle), parent(_parent) {}
};

///***************************************************************************************
/// CalcIK_2D_CCD
/// Given a bone chain located at the origin, this function will perform a single cyclic
/// coordinate descent (CCD) iteration. This finds a solution of bone angles that places
/// the final bone in the given chain at a target position. The supplied bone angles are
/// used to prime the CCD iteration. If a valid solution does not exist, the angles will
/// move as close to the target as possible. The user should resupply the updated angles 
/// until a valid solution is found (or until an iteration limit is met).
///  
/// returns: Result.Success when a valid solution was found.
///          Result.Processing when still searching for a valid solution.
///          Result.Failure when it can get no closer to the target.
///***************************************************************************************
/*
static Result calcCCD(
	std::vector<Bone> bones,	// Bone values to update
	float targetX,              // Target x position for the end effector
	float targetY,              // Target y position for the end effector
	float arrivalDist           // Must get within this range of the target
) {
*/
static Result calcCCD(
	Bone child,
	float targetX,              // Target x position for the end effector
	float targetY,              // Target y position for the end effector
	float arrivalDist           // Must get within this range of the target
) {

	// Set an epsilon value to prevent division by small numbers.
	const float epsilon = 0.0001;

	// Set max arc length a bone can move the end effector an be considered no motion
	// so that we can detect a failure state.
	const float trivialArcLength = 0.00001;

	/*
	int numBones = bones.size();
	if(!(numBones > 0))
		return Result::Failure;
	*/
	float arrivalDistSqr = arrivalDist * arrivalDist;

	//===
	// Generate the world space bone data.
	std::vector<Bone_Calc> worldBones;

	// Start with the root bone.
	Bone_Calc rootWorldBone;
	rootWorldBone.x = child.parent.x;
	rootWorldBone.y = child.parent.y;
	rootWorldBone.angle = child.parent.angle;
	rootWorldBone.cosAngle = cos(rootWorldBone.angle);
	rootWorldBone.sinAngle = sin(rootWorldBone.angle);
	worldBones.push_back(rootWorldBone);

	// Convert child bones to world space.
		Bone_Calc prevWorldBone = worldBones[0];
		Bone curLocalBone = child;

		Bone_Calc newWorldBone;
		newWorldBone.x = prevWorldBone.x + prevWorldBone.cosAngle*curLocalBone.x - prevWorldBone.sinAngle*curLocalBone.y;
		newWorldBone.y = prevWorldBone.y + prevWorldBone.sinAngle*curLocalBone.x + prevWorldBone.cosAngle*curLocalBone.y;
		newWorldBone.angle = prevWorldBone.angle + curLocalBone.angle;
		newWorldBone.cosAngle = cos(newWorldBone.angle);
		newWorldBone.sinAngle = sin(newWorldBone.angle);
		worldBones.push_back(newWorldBone);

	//===
	// Track the end effector position (the final bone)
	float endX = worldBones[1].x;
	float endY = worldBones[1].y;

	//===
	// Perform CCD on the bones by optimizing each bone in a loop 
	// from the final bone to the root bone
	bool modifiedBones = false;
	for (int boneIdx = numBones - 2; boneIdx >= 0; --boneIdx) {
		// Get the vector from the current bone to the end effector position.
		float curToEndX = endX - worldBones[boneIdx].x;
		float curToEndY = endY - worldBones[boneIdx].y;
		float curToEndMag = sqrt(curToEndX * curToEndX + curToEndY * curToEndY);

		// Get the vector from the current bone to the target position.
		float curToTargetX = targetX - worldBones[boneIdx].x;
		float curToTargetY = targetY - worldBones[boneIdx].y;
		float curToTargetMag = sqrt(curToTargetX * curToTargetX + curToTargetY * curToTargetY);

		// Get rotation to place the end effector on the line from the current
		// joint position to the target postion.
		float cosRotAng, sinRotAng, endTargetMag = (curToEndMag * curToTargetMag);
		if (endTargetMag <= epsilon) {
			cosRotAng = 1;
			sinRotAng = 0;
		} else {
			cosRotAng = (curToEndX * curToTargetX + curToEndY * curToTargetY) / endTargetMag;
			sinRotAng = (curToEndX * curToTargetY - curToEndY * curToTargetX) / endTargetMag;
		}

		// Clamp the cosine into range when computing the angle (might be out of range
		// due to floating point error).
		float rotAng = acos(std::max(-1.0f, std::min(1.0f, cosRotAng)));
		if (sinRotAng < 0.0)
			rotAng = -rotAng;

		// Rotate the end effector position.
		endX = worldBones[boneIdx].x + cosRotAng * curToEndX - sinRotAng * curToEndY;
		endY = worldBones[boneIdx].y + sinRotAng * curToEndX + cosRotAng * curToEndY;

		// Rotate the current bone in local space (this value is output to the user)
		bones[boneIdx].angle = SimplifyAngle(bones[boneIdx].angle + rotAng);

		// Check for termination
		float endToTargetX = (targetX - endX);
		float endToTargetY = (targetY - endY);
		if (endToTargetX * endToTargetX + endToTargetY * endToTargetY <= arrivalDistSqr) {
			// We found a valid solution.
			return Result::Success;
		}

		// Track if the arc length that we moved the end effector was
		// a nontrivial distance.
		if (!modifiedBones && abs(rotAng) * curToEndMag > trivialArcLength) {
			modifiedBones = true;
		}
	}

	// We failed to find a valid solution during this iteration.
	if (modifiedBones)
		return Result::Processing;
	else
		return Result::Failure;
}

void createBodyStructure(float bodyHeight, float armLength, float legLength) {
	Bone* spine = new Bone(0, bodyHeight, 180, NULL);
	Bone* leftArm = new Bone(armLength, -limbSize, -90, spine);
	Bone* rightArm = new Bone(armLength, -limbSize, 90, spine);
	Bone* leftForearm = new Bone(armLength * 2, -limbSize, -90, leftArm);
	Bone* rightForearm = new Bone(armLength * 2, -limbSize, 90, rightArm);

	Bone* leftThigh = new Bone(legLength, -bodyHeight + limbSize, -90, spine);
	Bone* rightThigh = new Bone(legLength, -bodyHeight + limbSize, 90, spine);
	Bone* leftLeg = new Bone(legLength * 2, -bodyHeight + limbSize, -90, leftLeg);
	Bone* rightLeg = new Bone(legLength * 2, -bodyHeight + limbSize, 90, rightLeg);
}