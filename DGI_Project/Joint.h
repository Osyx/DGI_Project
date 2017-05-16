#ifndef JOINT_H
#define JOINT_H

#include <string>
#include <vector>

#include "Matrices.h"

class Joint {
	Matrix4 animatedTransform;
	Matrix4 localBindTransform;
	Matrix4 inverseBindTransform;
public:
	int const index;
	const std::string name;
	std::vector<Joint> children;

	Joint(int, std::string, Matrix4);
	void addChild(Joint joint) { children.push_back(joint); }
	Matrix4 getAnimatedTransform() { return animatedTransform; }
	void setAnimationTransform(Matrix4 animatedTransform) { this->animatedTransform = animatedTransform; }
	Matrix4 getInverseBindTransform() {	return inverseBindTransform; }
	void calcInverseBindTransform(Matrix4);
} joint;

#endif