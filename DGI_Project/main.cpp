#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <GL/glut.h>
#include <glm\glm\glm.hpp>
#include <glm\glm\gtc\matrix_transform.hpp>
#include <vector>
#include <algorithm>

class Bone {
public:
	float x;	// x position in parent space
	float y;	// y position in parent space
	float z;	// z position in parent space
	float angle; // angle in parent space
	Bone() : x(0), y(0), z(0), angle(999) {}
	Bone(float _x, float _y, float _z, float _angle) : x(_x), y(_y), z(_z), angle(_angle) {}
};

class BoneCalc {
public:
	float x;        // x position in world space
	float y;        // y position in world space
	float angle;    // angle in world space
	float cosAngle; // sine of angle
	float sinAngle; // cosine of angle
	BoneCalc() {}
	BoneCalc(Bone bone) { x = bone.x; y = bone.y; angle = bone.angle; cosAngle = cos(bone.angle); sinAngle = sin(bone.angle); }
};

enum Result {
	Success,    // the target was reached
	Processing, // still trying to reach the target
	Failure    // failed to reach the target
};

class CCDResult {
public:
	int result;
	std::vector<Bone> bones;
	CCDResult(int _result, std::vector<Bone> _bones) : result(_result), bones(_bones) {}
	CCDResult(int _result) : result(_result) { std::vector<Bone> emptybones(0); bones = emptybones; }
	CCDResult() : result(Result::Failure) { std::vector<Bone> emptybones(0); bones = emptybones; }
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
glm::vec2 mousePos(0, 0);
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
void drawMouseDot();
static CCDResult calcCCD(std::vector<Bone>, glm::vec2, float);
float simplifyAngle(float);
float degToRad(float);
float radToDeg(float);
glm::vec2 rotateVector(Bone);

int  main() {
	Bone upperLeftArm(armLength, 0.0f, 0.0f, 0.0f);
	Bone lowerLeftArm(armLength, 0.0f, 0.0f, degToRad(90.0f));

	Bone upperRightArm(-armLength, 0.0f, 0.0f, 0.0f);
	Bone lowerRightArm(-armLength, 0.0f, 0.0f, degToRad(-90.0f));
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

		CCDResult leftResult = calcCCD(leftArm, mousePos, limbSize);
		CCDResult rightResult = calcCCD(rightArm, mousePos, limbSize);
		while (true) {
			if(!leftStop)
				leftResult = calcCCD(leftResult.bones, mousePos, limbSize);
			if(!rightStop)
				rightResult = calcCCD(rightResult.bones, mousePos, limbSize);
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
	drawMouseDot();

	updateValues();
	/*
	std::cout << "Left Up Angle:" << radToDeg(leftArm[0].angle) << std::endl;
	std::cout << "Left down Angle:" << radToDeg(leftArm[1].angle) << std::endl;
	std::cout << "Right Up Angle:" << radToDeg(rightArm[0].angle) << std::endl;
	std::cout << "Right down Angle:" << radToDeg(rightArm[1].angle) << std::endl;
	*/
	gluLookAt(camera.x, camera.y, camera.z, lookat.x, lookat.y, lookat.z, 0, 1, 0);
	glPushMatrix();

	// Body
	glTranslatef(SCREEN_WIDTH / 2 + limbSize / 2, SCREEN_HEIGHT - bodyHeight + legLength, 0);

	glPushMatrix();
	glTranslatef(SCREEN_WIDTH + 100, SCREEN_HEIGHT + 160, 0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(10, 20, 2);
	glPopMatrix();

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
	glRotatef(radToDeg(leftArm[0].angle), 0, 0, 1);
	glLineWidth(limbSize);
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(leftArm[0].x, leftArm[0].y, leftArm[0].z);
	glEnd();
	glPushMatrix();
	glTranslatef(leftArm[0].x, leftArm[0].y, leftArm[0].z);
	glRotatef(radToDeg(leftArm[1].angle), 0, 0, 1);
	glLineWidth(limbSize);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(leftArm[1].x, leftArm[1].y, leftArm[1].z);
	glEnd();
	glPopMatrix();

	glRotatef(radToDeg(rightArm[0].angle), 0, 0, 1);
	glLineWidth(limbSize);
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(rightArm[0].x, rightArm[0].y, rightArm[0].z);
	glEnd();
	glPushMatrix();
	glTranslatef(rightArm[0].x, rightArm[0].y, rightArm[0].z);
	glRotatef(radToDeg(rightArm[1].angle), 0, 0, 1);
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
	mousePos.x = x;
	mousePos.y = SCREEN_HEIGHT - y;
	std::cout << mousePos.x << " " << mousePos.y << std::endl;
	changed = true;
	drawFigure();
}

Bone worldSpacePos(Bone bone) {
	bone.x = bone.x + SCREEN_WIDTH / 2 + limbSize / 2;
	bone.y = -bone.y + SCREEN_HEIGHT - bodyHeight + legLength;
	return bone;
}

void drawMouseDot() {
	glPushMatrix();
	glTranslatef(mousePos.x, mousePos.y, 0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(2, 20, 2);
	glPopMatrix();
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

float radToDeg(float rad) {
	return rad * 180 / M_PI;
}

static CCDResult calcCCD(
	std::vector<Bone> bones,	// Bone values to update
	glm::vec2 target,              // Target y position for the end effector
	float arrivalDist           // Must get within this range of the target
) {
	
	// Set an epsilon value to prevent division by small numbers.
	const float epsilon = 0.001;

	// Set max arc length a bone can move the end effector and it can be considered no motion.
	// This so that we can detect a failure state.
	const float trivialArcLength = 0.00001;

	// If there are no bones, return failure.
	int numBones = bones.size();
	if (!(numBones > 0))
		return CCDResult(Result::Failure);

	// The square area distance within we much reach for it to be considered a solution. 
	float arrivalDistSqr = arrivalDist * arrivalDist;

	// Generate the world space bone data (wsbd).
	std::vector<BoneCalc> worldBones;

	// Start with the root bone and add it to the wsbd.
	BoneCalc rootWorldBone(worldSpacePos(bones[0]));
	worldBones.push_back(rootWorldBone);

	// Convert child bones to world space.
	for (int boneIdx = 1; boneIdx < numBones; ++boneIdx) {
		BoneCalc prevWorldBone = worldBones[boneIdx - 1];
		BoneCalc curLocalBone(worldSpacePos(bones[boneIdx]));

		BoneCalc newWorldBone;
		newWorldBone.x = prevWorldBone.x + prevWorldBone.cosAngle * curLocalBone.x - prevWorldBone.sinAngle * curLocalBone.y;
		newWorldBone.y = prevWorldBone.y + prevWorldBone.sinAngle * curLocalBone.x + prevWorldBone.cosAngle * curLocalBone.y;
		newWorldBone.angle = prevWorldBone.angle + curLocalBone.angle;
		newWorldBone.cosAngle = cos(newWorldBone.angle);
		newWorldBone.sinAngle = sin(newWorldBone.angle);
		worldBones.push_back(newWorldBone);
	}

	// Track the end effector position (the final bone)
	glm::vec2 endPos(worldBones[numBones].x, worldBones[numBones].y);

	// Perform CCD on the bones by optimizing each bone in a loop
	// from the final bone to the root bone
	bool modifiedBones = false;
	for (int boneIdx = numBones - 1; boneIdx >= 0; --boneIdx) {
		// Get the vector from the current bone to the end effector position.
		glm::vec2 curToEnd(endPos.x - worldBones[boneIdx].x, endPos.y - worldBones[boneIdx].y);
		float curToEndMag = sqrt(curToEnd.x * curToEnd.x + curToEnd.y * curToEnd.y);

		// Get the vector from the current bone to the target position.
		glm::vec2 curToTarget(target.x - worldBones[boneIdx].x, target.y - worldBones[boneIdx].y);
		float curToTargetMag = sqrt(std::pow(curToTarget.x, 2) + std::pow(curToTarget.y, 2));

		// Get rotation to place the end effector on the line from the current
		// joint position to the target postion.
		float cosRotAng = 1, sinRotAng = 0, endTargetMag = (curToEndMag * curToTargetMag);
		if (endTargetMag > epsilon) {
			cosRotAng = (curToEnd.x * curToTarget.x + curToEnd.y * curToTarget.y) / endTargetMag;
			sinRotAng = (curToEnd.x * curToTarget.y - curToEnd.y * curToTarget.y) / endTargetMag;
		}

		// Clamp the cosine into range when computing the angle (might be out of range
		// due to floating point error).
		float rotAng = acos(std::max(-1.0f, std::min(1.0f, cosRotAng)));
		if (sinRotAng < 0.0)
			rotAng = -rotAng;

		// Rotate the end effector position.
		endPos.x = worldBones[boneIdx].x + cosRotAng * curToEnd.x - sinRotAng * curToEnd.y;
		endPos.y = worldBones[boneIdx].y + sinRotAng * curToEnd.x + cosRotAng * curToEnd.y;

		// Rotate the current bone in local space (this value is output to the user)
		bones[boneIdx].angle = simplifyAngle(bones[boneIdx].angle + rotAng);

		// Check for termination
		glm::vec2 endToTarget = target - endPos;
		if (std::pow(endToTarget.x, 2) + std::pow(endToTarget.y, 2) <= arrivalDistSqr) {
			return CCDResult(Result::Success, bones);
		}

		// Track if the arc length that we moved the end effector was
		// a nontrivial distance.
		if (!modifiedBones && abs(rotAng) * curToEndMag > trivialArcLength)
			modifiedBones = true;
	}

	// We failed to find a valid solution during this iteration.
	if (modifiedBones)
		return CCDResult(Result::Processing, bones);
	else
		return CCDResult(Result::Failure);
}

glm::vec2 rotateVector(Bone bone) {
	glm::vec4 rotationPoint(bone.x, bone.y, 0, 1);
	glm::mat4 trans;
	trans = glm::rotate(trans, bone.angle, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec4 result = trans * rotationPoint;
	return glm::vec2(result.x, result.y);
}
