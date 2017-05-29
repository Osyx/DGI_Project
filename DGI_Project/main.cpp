#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <GL/glut.h>
#include <glm\glm\glm.hpp>
#include <vector>
#include <algorithm>

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

class Bone {
public:
	float x;	// x position in parent space
	float y;	// y position in parent space
	float z;	// z position in parent space
	float angle; // angle in parent space
	Bone() {}
	Bone(float _x, float _y, float _z, float _angle) : x(_x), y(_y), z(_z), angle(_angle) {}
};

const int SCREEN_WIDTH = 300;
const int SCREEN_HEIGHT = 340;
glm::vec3 camera(0.0f, 0.0f, 0.9f);
glm::vec3 lookat(0.0f, 0.0f, 0.0f);

float headSize = 20.0f;
float bodyHeight = 70.0f;
float limbSize = bodyHeight/100.0f * 7.0f;
float armLength = limbSize * 10.0f;
float legLength = armLength;
float mouseX = 0;
float mouseY = 0;

std::vector<Bone> rightArm(1);
std::vector<Bone> leftArm(1);
std::vector<Bone> rightLeg(1);
std::vector<Bone> leftLeg(1);

void init();
void updateValues();
void drawFigure();
void keyboard(unsigned char, int, int);
void mouse(int, int);
static Result calcCCD(std::vector<Bone>, float, float, float);
float simplifyAngle(float);
float degToRad(float);

int  main() {

	Bone upperLeftArm(armLength, 0.0f, 0.0f, degToRad(90.0f));
	Bone lowerLeftArm(armLength, 0.0f, 0.0f, degToRad(0.0f));

	Bone upperRightArm(-armLength, 0.0f, 0.0f, degToRad(90.0f));
	Bone lowerRightArm(-armLength, 0.0f, 0.0f, degToRad(0.0f));

	leftArm.push_back(upperLeftArm);
	leftArm.push_back(lowerLeftArm);
	rightArm.push_back(upperRightArm);
	rightArm.push_back(lowerRightArm);

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

void init() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
	glutMotionFunc(mouse);
	std::cout << "x: " << mouseX << "y: " << mouseY << std::endl;
	int loops = 0;
	while (calcCCD(leftArm, mouseX, mouseY, 3) == Result::Processing) {
		glutPostRedisplay();
		if (loops++ == 3)
			break;
	}
	for (int i = 0; i < leftArm.size() - 1; ++i) {
		float angle = leftArm[i].angle;
		if (i == 1)
			std::cout << angle << std::endl;
		glm::vec2 newLeft = glm::mat2(glm::vec2(cos(angle), -sin(angle)), glm::vec2(sin(angle), cos(angle))) * glm::vec2(leftArm[i].x, leftArm[i].y);
		leftArm[i].x = newLeft.x;
		leftArm[i].y = newLeft.y;
		std::cout << "Loop" << std::endl;
	}
	
}

void drawFigure() {

	init();
	updateValues();
	gluLookAt(camera.x, camera.y, camera.z, lookat.x, lookat.y, lookat.z, 0, 1, 0);
	glPushMatrix();

	// Body
	glTranslatef(SCREEN_WIDTH / 2 + limbSize / 2, SCREEN_HEIGHT - bodyHeight + legLength, 0);

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
	std::cout << "x: " << leftArm[0].x << " y: " << leftArm[0].y << " z: " << leftArm[0].z << std::endl;
	glVertex3f(leftArm[0].x, leftArm[0].y, leftArm[0].z);
	glEnd();
	glPushMatrix();
	glTranslatef(leftArm[0].x, leftArm[0].y, leftArm[0].z);
	glLineWidth(limbSize);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(leftArm[1].x, leftArm[1].y, leftArm[1].z);
	glEnd();
	glPopMatrix();

	glLineWidth(limbSize);
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(rightArm[0].x, rightArm[0].y, rightArm[0].z);
	glEnd();
	glPushMatrix();
	glTranslatef(rightArm[0].x, rightArm[0].y, rightArm[0].z);
	glLineWidth(limbSize);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(rightArm[1].x, rightArm[1].y, rightArm[1].z);
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

void mouse(int x, int y) {
	mouseX = x;
	mouseY = y;
}

///***************************************************************************************
/// SimplifyAngle
/// This function will convert an angle to the equivalent rotation in the range [-pi,pi]
///***************************************************************************************
float simplifyAngle(float angle) {
	angle = fmod(angle, (2.0 * M_PI));
	if (angle < -M_PI)
		angle += (2.0 * M_PI);
	else if (angle > M_PI)
		angle -= (2.0 * M_PI);
	return angle;
}

float degToRad(float deg) {
	return deg * M_PI / 180;
}

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

static Result calcCCD(
	std::vector<Bone> bones,	// Bone values to update
	float targetX,              // Target x position for the end effector
	float targetY,              // Target y position for the end effector
	float arrivalDist           // Must get within this range of the target
) {
	
	// Set an epsilon value to prevent division by small numbers.
	const float epsilon = 0.0001;

	// Set max arc length a bone can move the end effector an be considered no motion
	// so that we can detect a failure state.
	const float trivialArcLength = 0.00001;

	int numBones = bones.size();
	if(!(numBones > 0))
		return Result::Failure;
	float arrivalDistSqr = arrivalDist * arrivalDist;

	//===
	// Generate the world space bone data.
	std::vector<Bone_Calc> worldBones;

	// Start with the root bone.
	Bone_Calc rootWorldBone;
	rootWorldBone.x = bones[0].x;
	rootWorldBone.y = bones[0].y;
	rootWorldBone.angle = bones[0].angle;
	rootWorldBone.cosAngle = cos(rootWorldBone.angle);
	rootWorldBone.sinAngle = sin(rootWorldBone.angle);
	worldBones.push_back(rootWorldBone);

	// Convert child bones to world space.
	for (int boneIdx = 1; boneIdx < numBones; ++boneIdx) {
		Bone_Calc prevWorldBone = worldBones[boneIdx - 1];
		Bone_Calc curLocalBone; 
		curLocalBone.x = bones[boneIdx].x;
		curLocalBone.y = bones[boneIdx].y;
		curLocalBone.angle = bones[boneIdx].angle;
		curLocalBone.cosAngle = cos(bones[boneIdx].angle);
		curLocalBone.sinAngle = sin(bones[boneIdx].angle);

		Bone_Calc newWorldBone;
		newWorldBone.x = prevWorldBone.x + prevWorldBone.cosAngle*curLocalBone.x
			- prevWorldBone.sinAngle*curLocalBone.y;
		newWorldBone.y = prevWorldBone.y + prevWorldBone.sinAngle*curLocalBone.x
			+ prevWorldBone.cosAngle*curLocalBone.y;
		newWorldBone.angle = prevWorldBone.angle + curLocalBone.angle;
		newWorldBone.cosAngle = cos(newWorldBone.angle);
		newWorldBone.sinAngle = sin(newWorldBone.angle);
		worldBones.push_back(newWorldBone);
	}

	//===
	// Track the end effector position (the final bone)
	double endX = worldBones[numBones - 1].x;
	double endY = worldBones[numBones - 1].y;

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
		bones[boneIdx].angle = simplifyAngle(bones[boneIdx].angle + rotAng);

		std::cout << bones[boneIdx].angle << " " << boneIdx << std::endl;

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
	
}

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