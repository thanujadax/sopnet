/*
 * HammingDistanceCalculator.cpp
 *
 *  Created on: Aug 21, 2014
 *      Author: thanuja
 */

#include <util/Logger.h>
#include "HammingDistanceCalculator.h"

logger::LogChannel hammingdistancecalculatorlog("hammingdistancecalculatorlog", "[HammingDistanceCalculator] ");

HammingDistanceCalculator::HammingDistanceCalculator() :
		_hammingDistance(new unsigned int(0)) {

	registerInput(_goldStandard, "gold standard solution");
	registerInput(_result, "solution");

	registerOutput(_hammingDistance, "hamming distance");
}

void
HammingDistanceCalculator::updateOutputs(){
	LOG_DEBUG(hammingdistancecalculatorlog) << "updating outputs..." << std::endl;
	*_hammingDistance = 0;
	calculateHammingDistance();
	LOG_DEBUG(hammingdistancecalculatorlog) << "updated outputs!" << std::endl;
}


void
HammingDistanceCalculator::calculateHammingDistance(){
	std::vector<double> goldStandardVector = _goldStandard->getVector();
	std::vector<double> resultVector = _result->getVector();

	if(goldStandardVector.size() != resultVector.size()){
		LOG_DEBUG(hammingdistancecalculatorlog) << "ERROR: result size and gold standard size mismatch" << std::endl;
	}
	else {
		std::vector<double>::iterator result_iter = resultVector.begin();
		for (std::vector<double>::iterator goldStandard_iter = goldStandardVector.begin() ; goldStandard_iter != goldStandardVector.end(); ++goldStandard_iter){
			if(*result_iter != *goldStandard_iter){
				*_hammingDistance = *_hammingDistance +1;
			}
			++result_iter;
		}
		LOG_DEBUG(hammingdistancecalculatorlog) << "Hamming Distance: " << *_hammingDistance << std::endl;
	}


}


