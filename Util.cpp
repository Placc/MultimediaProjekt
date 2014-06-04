#include "Util.h"

namespace Util {
	bool compareRects(WeightedRect &rect1, WeightedRect &rect2){
		return (rect1.weight < rect2.weight);
	}

	bool compareEvals(Eval &eval1, Eval &eval2){
		return (eval1.classification < eval2.classification);
	}

	bool comparePlotValues(Point2d &p1, Point2d &p2){
		return (p1.y < p2.y);
	}
}