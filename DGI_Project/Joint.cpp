#include "Joint.h"

Joint::Joint(int _index, std::string _name, Matrix4 _bindLocalTransform) :
	index(_index),
	name(_name),
	localBindTransform(_bindLocalTransform) {}

void Joint::calcInverseBindTransform(Matrix4 parentBindTransform) {
	Matrix4 bindTransform(parentBindTransform * localBindTransform);
	inverseBindTransform = bindTransform.invert();
	for (Joint child : children) {
		child.calcInverseBindTransform(bindTransform);
	}
}

