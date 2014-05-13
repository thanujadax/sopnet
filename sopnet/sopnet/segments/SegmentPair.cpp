/*
 * SegmentPair.cpp
 *
 *  Created on: May 13, 2014
 *      Author: thanuja
 */

#include <imageprocessing/ConnectedComponent.h>
#include "SegmentPair.h"

SegmentPair::SegmentPair(unsigned int id, Direction direction,
			boost::shared_ptr<ContinuationSegment> continuationSegment1,
			boost::shared_ptr<ContinuationSegment> continuationSegment2) :
		Segment(
				id,
				direction,
				(continuationSegment1->getSourceSlice()->getComponent()->getCenter()*continuationSegment1->getSourceSlice()->getComponent()->getSize()   +
				 continuationSegment1->getTargetSlice()->getComponent()->getCenter()*continuationSegment1->getTargetSlice()->getComponent()->getSize() +
				 continuationSegment2->getTargetSlice()->getComponent()->getCenter()*continuationSegment2->getTargetSlice()->getComponent()->getSize())/
				 (continuationSegment1->getSourceSlice()->getComponent()->getSize() + continuationSegment1->getTargetSlice()->getComponent()->getSize() + continuationSegment2->getTargetSlice()->getComponent()->getSize()),
				 continuationSegment1->getSourceSlice()->getSection() + (direction == Left ? 0 : 1)),
		_continuationSegment1(continuationSegment1),
		_continuationSegment2(continuationSegment2) {}

boost::shared_ptr<ContinuationSegment>
SegmentPair::getContinuationSegment1() const {

	return _continuationSegment1;
}

boost::shared_ptr<ContinuationSegment>
SegmentPair::getContinuationSegment2() const {

	return _continuationSegment2;
}

