/*
 * SegmentPairEnd.cpp
 *
 *  Created on: Aug 4, 2014
 *      Author: thanuja
 */


#include <imageprocessing/ConnectedComponent.h>
#include "SegmentPairEnd.h"

SegmentPairEnd::SegmentPairEnd(unsigned int id, Direction direction,
			boost::shared_ptr<ContinuationSegment> continuationSegment,
			boost::shared_ptr<EndSegment> endSegment,
			boost::shared_ptr<Slice> sourceSlice,
			boost::shared_ptr<Slice> targetSlice) :
		Segment(
				id,
				direction,
				continuationSegment->getSourceSlice()->getComponent()->getCenter(),
				 continuationSegment->getInterSectionInterval()),
		_continuationSegment(continuationSegment),
		_endSegment(endSegment),
		_sourceSlice(sourceSlice),
		_targetSlice(targetSlice){}

boost::shared_ptr<ContinuationSegment>
SegmentPairEnd::getContinuationSegment() const {

	return _continuationSegment;
}

boost::shared_ptr<EndSegment>
SegmentPairEnd::getEndSegment() const {

	return _endSegment;
}

std::vector<boost::shared_ptr<Slice> >
SegmentPairEnd::getSlices() const {

	std::vector<boost::shared_ptr<Slice> > slices;

	//slices.push_back(getSlice());

	return slices;
}

boost::shared_ptr<Slice>
SegmentPair::getSourceSlice() const {

	return _sourceSlice;
}

boost::shared_ptr<Slice>
SegmentPair::getTargetSlice() const {

	return _targetSlice;
}
