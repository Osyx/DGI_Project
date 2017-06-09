#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <GL/glut.h>
#include <glm\glm\glm.hpp>
#include <vector>
#include <algorithm>

class Bone_Calc {
public:
	float x;        // x position in world space
	float y;        // y position in world space
	float angle;    // angle in world space
	float cosAngle; // sine of angle
	float sinAngle; // cosine of angle
	Bone_Calc() {}
	Bone_Calc(float _x, float _y, float _angle, float _cosAngle, float _sinAngle) { x = _x; y = _y; angle = _angle; cosAngle = _cosAngle; sinAngle = _sinAngle; }
};

enum Result {
	Success,    // the target was reached
	Processing, // still trying to reach the target
	Failure    // failed to reach the target
};

class Bone {
public:
	float x;	// x position in parent space
	float y;	// y position in parent space
	float z;	// z position in parent space
	float angle; // angle in parent space
	Bone() : x(0), y(0), z(0), angle(999) {}
	Bone(float _x, float _y, float _z, float _angle) : x(_x), y(_y), z(_z), angle(_angle) {}
};

class VectorResult {
public:
	int result;
	std::vector<Bone> bones;
	VectorResult(int _result, std::vector<Bone> _bones) : result(_result), bones(_bones) {}
	VectorResult(int _result) : result(_result) { std::vector<Bone> emptybones(0); bones = emptybones; }
	VectorResult() : result(Result::Failure) { std::vector<Bone> emptybones(0); bones = emptybones; }
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
bool changed = false;

std::vector<Bone> rightArm;
std::vector<Bone> leftArm;
std::vector<Bone> rightLeg;
std::vector<Bone> leftLeg;

void init();
void updateValues();
void drawFigure();
void keyboard(unsigned char, int, int);
void mouse(int, int);
static VectorResult calcCCD(std::vector<Bone>, float, float, float);
float simplifyAngle(float);
float degToRad(float);

VectorResult testing(VectorResult hej) {
	std::cout << "Before inside: " << hej.bones[0].angle << std::endl;
	hej.bones[0].angle = 222;
	std::cout << "After inside: " << hej.bones[0].angle << std::endl;
	return hej;
}

int  main() {

	Bone upperLeftArm(armLength, 0.0f, 0.0f, 0.0f);
	Bone lowerLeftArm(armLength, 0.0f, 0.0f, 0.0f);

	Bone upperRightArm(-armLength, 0.0f, 0.0f, 0.0f);
	Bone lowerRightArm(-armLength, 0.0f, 0.0f, 0.0f);
	leftArm.push_back(upperLeftArm);
	leftArm.push_back(lowerLeftArm);
	rightArm.push_back(upperRightArm);
	rightArm.push_back(lowerRightArm);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(500, 500);
	glutCreateWindow("Figure");
	glutMotionFunc(mouse);
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
	if (changed) {
		limbSize = bodyHeight / 100.0f * 7.0f;
		armLength = limbSize * 10.0f;
		legLength = armLength;
		int loops = 0;
		bool rightStop = false;
		bool leftStop = false;
		bool success = false;

		VectorResult leftResult = calcCCD(leftArm, mouseX, mouseY, limbSize);
		VectorResult rightResult = calcCCD(rightArm, mouseX, mouseY, limbSize);
		while (true) {
			std::cout << mouseX << " " << mouseY << std::endl;
			if(!leftStop)
				leftResult = calcCCD(leftResult.bones, mouseX, mouseY, limbSize);
			if(!rightStop)
				rightResult = calcCCD(rightResult.bones, mouseX, mouseY, limbSize);
			if (leftResult.result == Result::Failure)
				leftStop = true;
			if (rightResult.result == Result::Failure)
				rightStop = true;
			if (leftResult.result == Result::Success && !leftStop) {
				leftArm = leftResult.bones;
				std::cout << "SUCCESS left!" << std::endl;
				if (success)
					break;
				success = true;
				leftStop = true;

			}
			if (rightResult.result == Result::Success && !rightStop) {
				rightArm = rightResult.bones;
				std::cout << "SUCCESS right!" << std::endl;
				if(success)
					break;
				success = true;
				rightStop = true;
			}
			glutPostRedisplay();
			if (loops++ == 5000 || (leftStop && rightStop))
				break;
		}
		changed = false;
		if (rightResult.result != Result::Failure)
			rightArm = rightResult.bones;
		if (leftResult.result != Result::Failure)
			leftArm = leftResult.bones;
	}
}

void drawFigure() {

	init();

	glPushMatrix();
	glTranslatef(mouseX, -mouseY, 0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(2, 20, 2);
	glPopMatrix();

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
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
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
	glColor3f(1.0, 1.0, 0.0);
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
	std::cout << "x: " << x << " y: " << y << std::endl;
	mouseX = x;
	mouseY = y;
	changed = true;
	drawFigure();
}

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

static VectorResult calcCCD(
	std::vector<Bone> bones,	// Bone values to update
	float targetX,              // Target x position for the end effector
	float targetY,              // Target y position for the end effector
	float arrivalDist           // Must get within this range of the target
) {
	
	// Set an epsilon value to prevent division by small numbers.
	const float epsilon = 0.001;

	// Set max arc length a bone can move the end effector and it can be considered no motion.
	// So that we can detect a failure state.
	const float trivialArcLength = 0.00001;

	int numBones = bones.size();
	if (!(numBones > 0))
		return VectorResult(Result::Failure);
	float arrivalDistSqr = arrivalDist * arrivalDist;

	// Generate the world space bone data.
	std::vector<Bone_Calc> worldBones;

	// Start with the root bone.
	Bone_Calc rootWorldBone(bones[0].x, bones[0].y, bones[0].angle, cos(bones[0].angle), sin(bones[0].angle));
	worldBones.push_back(rootWorldBone);

	// Convert child bones to world space.
	for (int boneIdx = 1; boneIdx < numBones; ++boneIdx) {
		Bone_Calc prevWorldBone = worldBones[boneIdx - 1];
		Bone_Calc curLocalBone(bones[boneIdx].x, bones[boneIdx].y, bones[boneIdx].angle, cos(bones[boneIdx].angle), sin(bones[boneIdx].angle));

		Bone_Calc newWorldBone;
		newWorldBone.x = prevWorldBone.x + prevWorldBone.cosAngle * curLocalBone.x - prevWorldBone.sinAngle * curLocalBone.y;
		newWorldBone.y = prevWorldBone.y + prevWorldBone.sinAngle * curLocalBone.x + prevWorldBone.cosAngle * curLocalBone.y;
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
	for (int boneIdx = numBones - 1; boneIdx >= 0; --boneIdx) {
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

		// Check for termination
		float endToTargetX = (targetX - endX);
		float endToTargetY = (targetY - endY);
		if (endToTargetX * endToTargetX + endToTargetY * endToTargetY <= arrivalDistSqr) {
			// We found a valid solution.
			glm::vec2 newLeft = glm::mat2(glm::vec2(cos(bones[boneIdx].angle), -sin(bones[boneIdx].angle)), glm::vec2(sin(bones[boneIdx].angle), cos(bones[boneIdx].angle))) * glm::vec2(bones[boneIdx].x, bones[boneIdx].y);
			bones[boneIdx].x = newLeft.x;
			bones[boneIdx].y = newLeft.y;
			bones[boneIdx].angle = 0;
			return VectorResult(Result::Success, bones);
		}

		// Track if the arc length that we moved the end effector was
		// a nontrivial distance.
		if (!modifiedBones && abs(rotAng) * curToEndMag > trivialArcLength) {

			glm::vec2 newLeft = glm::mat2(glm::vec2(cos(bones[boneIdx].angle), -sin(bones[boneIdx].angle)), glm::vec2(sin(bones[boneIdx].angle), cos(bones[boneIdx].angle))) * glm::vec2(bones[boneIdx].x, bones[boneIdx].y);
			bones[boneIdx].x = newLeft.x;
			bones[boneIdx].y = newLeft.y;
			bones[boneIdx].angle = 0;

			modifiedBones = true;
		}
	}

	// We failed to find a valid solution during this iteration.
	if (modifiedBones) {
		return VectorResult(Result::Processing, bones);
	} else
		return VectorResult(Result::Failure);
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