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
			boost::shared_ptr<ContinuationSegment> continuationSegment2,
			boost::shared_ptr<Slice> sourceSlice,
			boost::shared_ptr<Slice> midSlice,
			boost::shared_ptr<Slice> targetSlice):
		Segment(
				id,
				direction,
				(continuationSegment1->getSourceSlice()->getComponent()->getCenter()*continuationSegment1->getSourceSlice()->getComponent()->getSize()   +
				 continuationSegment1->getTargetSlice()->getComponent()->getCenter()*continuationSegment1->getTargetSlice()->getComponent()->getSize() +
				 continuationSegment2->getTargetSlice()->getComponent()->getCenter()*continuationSegment2->getTargetSlice()->getComponent()->getSize())/
				 (continuationSegment1->getSourceSlice()->getComponent()->getSize() + continuationSegment1->getTargetSlice()->getComponent()->getSize() + continuationSegment2->getTargetSlice()->getComponent()->getSize()),
				 continuationSegment1->getInterSectionInterval()),
		_continuationSegment1(continuationSegment1),
		_continuationSegment2(continuationSegment2),
		_sourceSlice(sourceSlice),
		_midSlice(midSlice),
		_targetSlice(targetSlice){}

boost::shared_ptr<ContinuationSegment>
SegmentPair::getContinuationSegment1() const {

	return _continuationSegment1;
}

boost::shared_ptr<ContinuationSegment>
SegmentPair::getContinuationSegment2() const {

	return _continuationSegment2;
}

std::vector<boost::shared_ptr<Slice> >
SegmentPair::getSlices() const {

	std::vector<boost::shared_ptr<Slice> > slices;

	//slices.push_back(getSlice());

	return slices;
}

boost::shared_ptr<Slice>
SegmentPair::getSourceSlice() const {

	return _sourceSlice;
}

boost::shared_ptr<Slice>
SegmentPair::getMidSlice() const {

	return _midSlice;
}

boost::shared_ptr<Slice>
SegmentPair::getTargetSlice() const {

	return _targetSlice;
}
