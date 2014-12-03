#include "Utility.h"

USING_NS_CC;

Point gStart = Point::ZERO;
Point gTarget = Point::ZERO;

auto heuristic = [](Point a, Point b)
{
	// First Try
//	return a.getDistanceSq(gTarget) > b.getDistanceSq(gTarget);

	// Second Try
//	return abs(a.x - gTarget.x) + abs(a.y - gTarget.y) > abs(b.x - gTarget.x) + abs(b.y - gTarget.y);

	// Third Try
// 	float tmpa = a.getDistanceSq(gStart), tmpb = b.getDistanceSq(gStart);
// 	if (floatEquals(tmpa, tmpb)) {
// 		return a.getDistanceSq(gTarget) > b.getDistanceSq(gTarget);
// 	}
// 	return tmpa > tmpb;

	// Fourth Try
	float starta = a.getDistanceSq(gStart), startb = b.getDistanceSq(gStart);
	float targeta = a.getDistanceSq(gTarget), targetb = b.getDistanceSq(gTarget);

	return min(starta, targeta) > min(startb, targetb);
};

typedef priority_queue<Point, vector<Point>, decltype(heuristic) >  PointPriorityQueue;

Vector<FiniteTimeAction *> breadthFirstSearch(
	Point start, Point target, Point gridIndexSize, Point gridPixelSize, 
	const unordered_set<Point, PointHash, PointCmp> &occupiedGrids, float durationEach)
{
	Vector<FiniteTimeAction *> actions;
	if (floatEquals(start.x, target.x) && floatEquals(start.y, target.y)) {
		return actions;
	}
	// record what the preview grid of the current grid is
	unordered_map<Point, Point, PointHash, PointCmp> prev;
	// record how many minimum "steps" required to get here
	unordered_map<Point, int, PointHash, PointCmp> step;
	PointPriorityQueue searchQueue(heuristic);
	bool found = false;

	// For heuristic comparison
	gStart = start;
	gTarget = target;
	searchQueue.push(start);
	prev[start] = Point(-1, -1);
	step[start] = 0;

	while (!searchQueue.empty()) {
		Point current = searchQueue.top();
		searchQueue.pop();
//		cout << "(" << current.x << ' ' << current.y << "): " << step[current] << endl;

		static const int dirX[] = { 0, 1, 0, -1, 1, 1, -1, -1 };
		static const int dirY[] = { 1, 0, -1, 0, 1, -1, -1, 1 };

		for (int i = 0; i < sizeof(dirX) / sizeof(int); i++) {
			if (dirX[i] != 0 && dirY[i] != 0 &&
				(occupiedGrids.count(Point(current.x + dirX[i], current.y)) ||
				occupiedGrids.count(Point(current.x, current.y + dirY[i])))) {
				continue;
			}
			Point next(current.x + dirX[i], current.y + dirY[i]);
			if (next.x >= 0 && next.x < gridIndexSize.x && next.y >= 0 && next.y < gridIndexSize.y && !occupiedGrids.count(next)) {
				// if moving vertical or horizontal, "step" is two
				// if diagonal, "step" is three
				int newStep = (dirX[i] != 0 && dirY[i] != 0) ? step[current] + 3 : step[current] + 2;
				if (!prev.count(next) || newStep < step[next]) {
					prev[next] = current;
					step[next] = newStep;
					searchQueue.push(next);
					if (next == target) {
						// due to it's a A* search, once we found it, it's already the shortest path, no need to continue search
						found = true;
						break;
					}
				}
			}
		}
		if (found) {
			break;
		}
	}

//	cout << "------------------------------------\n";
	Point pointer = target;
	while (pointer != start) {
//		cout << "(" << pointer.x << ' ' << pointer.y << "): " << step[pointer] << endl;
		auto movement = MoveTo::create(durationEach, Point((pointer.x + 0.5f) * gridPixelSize.x, (pointer.y + 0.5f) * gridPixelSize.y));
		actions.pushBack(movement);
		pointer = prev[pointer];
	}
//	cout << "------------------------------------\n";

// 	for (auto &i : actions) {
// 		i->setDuration(totalTime / (float)actions.size());
// 	}
	reverse(begin(actions), end(actions));

	return actions;
}

