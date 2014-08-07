/*
 * SegmentPair.h
 *
 *  Created on: May 13, 2014
 *      Author: thanuja
 */

#ifndef SOPNET_SEGMENTPAIR_H_
#define SOPNET_SEGMENTPAIR_H_

#include "Segment.h"
#include "ContinuationSegment.h"

class SegmentPair: public Segment {

public:

	SegmentPair(unsigned int id, Direction direction,
			boost::shared_ptr<ContinuationSegment> continuationSegment1,
			boost::shared_ptr<ContinuationSegment> continuationSegment2,
			boost::shared_ptr<Slice> sourceSlice,
			boost::shared_ptr<Slice> midSlice,
			boost::shared_ptr<Slice> targetSlice);

	boost::shared_ptr<ContinuationSegment> getContinuationSegment1() const;

	boost::shared_ptr<ContinuationSegment> getContinuationSegment2() const;

	std::vector<boost::shared_ptr<Slice> > getSlices() const;

	boost::shared_ptr<Slice> getSourceSlice() const;

	boost::shared_ptr<Slice> getMidSlice() const;

	boost::shared_ptr<Slice> getTargetSlice() const;

private:

	boost::shared_ptr<ContinuationSegment> _continuationSegment1;

	boost::shared_ptr<ContinuationSegment> _continuationSegment2;

	boost::shared_ptr<Slice> _sourceSlice;

	boost::shared_ptr<Slice> _midSlice;

	boost::shared_ptr<Slice> _targetSlice;

};

#endif /* SOPNET_SEGMENTPAIR_H_ */
