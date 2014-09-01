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
	registerInput(_allSegments, "segments");

	registerOutput(_hammingDistance, "hamming distance");
	registerOutput(_hammingDistanceLowLevel, "hamming distance low level");
}

void
HammingDistanceCalculator::updateOutputs(){
	LOG_DEBUG(hammingdistancecalculatorlog) << "updating outputs..." << std::endl;
	*_hammingDistance = 0;
	*_hammingDistanceLowLevel = 0;
	calculateHammingDistance();
	calculateHammingDistanceLowLevel();
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

void
HammingDistanceCalculator::calculateHammingDistanceLowLevel(){
	std::vector<double> goldStandardVector = _goldStandard->getVector();
	std::vector<double> resultVector = _result->getVector();
	unsigned int numLowLevelSegments = _allSegments->getSegments().size();

	unsigned int counter = 0;

	if(goldStandardVector.size() != resultVector.size()){
		LOG_DEBUG(hammingdistancecalculatorlog) << "ERROR: result size and gold standard size mismatch" << std::endl;
	}
	else {
		std::vector<double>::iterator result_iter = resultVector.begin();
		for (std::vector<double>::iterator goldStandard_iter = goldStandardVector.begin() ; counter < numLowLevelSegments; ++goldStandard_iter){
			if(*result_iter != *goldStandard_iter){
				*_hammingDistanceLowLevel = *_hammingDistanceLowLevel +1;
			}
			++result_iter;
			++counter;
		}
		LOG_DEBUG(hammingdistancecalculatorlog) << "Hamming Distance (low-level varibles only): " << *_hammingDistanceLowLevel << std::endl;
	}
}


