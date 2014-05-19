/*
 * SegmentPairExtractor.cpp
 *
 *  Created on: May 19, 2014
 *      Author: thanuja
 */

#include "SegmentPairExtractor.h"


SegmentPairExtractor::SegmentPairExtractor() :
	_segmentPairs(new Segments()){

	registerInputs(_allSegments, "segments");

	registerOutput(_segmentPairs, "segment pairs");

}

void
SegmentPairExtractor::updateOutputs(){

	extractSegmentPairs();

}

void
SegmentPairExtractor::extractSegmentPairs(){
	unsigned int numContinuationSegments = (_allSegments->getContinuations()).size();
	// vector to hold all source slice ids of continuationSegments
	std::vector<unsigned int> sources (numContinuationSegments);
	// vector to hold all target slice ids of continuationSegments
	std::vector<unsigned int> targets (numContinuationSegments);
	// vector to hold directions of all continuationSegments
	std::vector<Direction> directions (numContinuationSegments);

	int i = 0;
	foreach (boost::shared_ptr<ContinuationSegment> segment, _allSegments->getContinuations()) {

		sources.push_back(i) = segment->getSourceSlice();
		targets.push_back(i) = segment->getTargetSlice();
		directions.push_back(i) = segment->getDirection();
		i++;

		}

}


