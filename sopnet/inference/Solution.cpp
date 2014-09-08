#include "Solution.h"

Solution::Solution(unsigned int size) {

	resize(size);
}

void
Solution::resize(unsigned int size) {

	_solution.resize(size);
}

void
Solution::assignSolution(std::vector<double> newSolution) {
	_solution.swap(newSolution);
}

