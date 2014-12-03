#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "cocos2d.h"
USING_NS_CC;

#include <iostream>
#include <cmath>
#include <unordered_set>
using namespace std;

inline bool floatEquals(float x, float y, float eps = 1e-5) {
	return fabs(x - y) < eps;
}

namespace cocos2d {
	struct PointHash {
		size_t operator()(const cocos2d::Point &p) const {
			return p.x * 9999 + p.y;
		}
	};

	struct PointCmp {
		bool operator()(const cocos2d::Point &p1, const cocos2d::Point &p2) const {
			return (floatEquals(p1.x, p2.x) && floatEquals(p1.y, p2.y));
		}
	};
}

Vector<FiniteTimeAction *> breadthFirstSearch(
	Point start, Point target, Point gridIndexSize, Point gridPixelSize,
	const unordered_set<Point, PointHash, PointCmp> &occupiedGrids, float durationEach);


#endif ///> end of __UTILITY_H__