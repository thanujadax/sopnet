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

	extractSegmentPairsAll();

}

void
SegmentPairExtractor::extractSegmentPairsAll(){

	foreach (boost::shared_ptr<ContinuationSegment> segment, _allSegments->getContinuations()) {
		extractSegmentPairs(segment);
		}
}

void
SegmentPairExtractor::extractSegmentPairs(boost::shared_ptr<ContinuationSegment> segment){

	// for this segment, extract all possible segment pairs that continue from its target slice
	// in the same direction

	// get interSectionInterval
	unsigned int nextInterSectionInterval = segment->getInterSectionInterval() + 1;

	// get all segments in the next interSectionInterval, continuing from this segment
	boost::shared_ptr<Segments> nextIntervalContinuationSegments = _allSegments->getContinuations(nextInterSectionInterval);

	foreach(boost::shared_ptr<ContinuationSegment> nextSegment, nextIntervalContinuationSegments){
		if (segment->getDirection()==Left){
			if(nextSegment->getDirection() == Left){
				if(segment->getSourceSlice() == nextSegment->getTargetSlice()) {
					// this segment goes left, next segment goes left
					addNextSegmentPair(Left,segment,nextSegment);
				}
			} else {
				if(segment->getSourceSlice() == nextSegment->getSourceSlice()) {
					// this segment goes left, next segment goes right
					addNextSegmentPair(Left,segment,nextSegment);
				}
			}
		} else {
			// this segment goes right
			if(nextSegment->getDirection() == Left){
				if(segment->getTargetSlice() == nextSegment->getTargetSlice()) {
					// this segment goes right, next segment goes left
					addNextSegmentPair(Right,segment,nextSegment);
				}
			} else {
				if(segment->getTargetSlice() == nextSegment->getSourceSlice()) {
					// this segment goes right, next segment goes right
					addNextSegmentPair(Right,segment,nextSegment);
				}
			}

		}

	}
}

void
SegmentPairExtractor::addNextSegmentPair(
		Direction direction,
		boost::shared_ptr<ContinuationSegment> segment1,
		boost::shared_ptr<ContinuationSegment> segment2){

	boost::shared_ptr<SegmentPair> segmentPair = boost::make_shared<SegmentPair>(
			Segment::getNextSegmentId(),
			direction ,
			segment1,
			segment2);

	_segmentPairs->add(segmentPair);
}
