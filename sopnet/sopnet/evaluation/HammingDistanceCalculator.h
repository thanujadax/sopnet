/*
 * HammingDistanceCalculator.h
 *
 *  Created on: Aug 21, 2014
 *      Author: thanuja
 */

#ifndef HAMMINGDISTANCECALCULATOR_H_
#define HAMMINGDISTANCECALCULATOR_H_

#include <pipeline/all.h>

#include <sopnet/segments/Segments.h>
#include <sopnet/inference/Solution.h>
#include "SliceErrors.h"

/*
 *
 */
class HammingDistanceCalculator : public pipeline::SimpleProcessNode<>{

public:
	HammingDistanceCalculator();

private:

	void updateOutputs();

	void calculateHammingDistance();

	pipeline::Input<Solution> _goldStandard;
	pipeline::Input<Solution> _result;

	pipeline::Output<unsigned int> _hammingDistance;

};

#endif /* HAMMINGDISTANCECALCULATOR_H_ */
