/*
 * SegmentPairEnd.h
 *
 *  Created on: Aug 4, 2014
 *      Author: thanuja
 */

#ifndef SEGMENTPAIREND_H_
#define SEGMENTPAIREND_H_

#include "Segment.h"
#include "ContinuationSegment.h"
#include "EndSegment.h"

class SegmentPairEnd: public Segment {

public:

	SegmentPairEnd(unsigned int id, Direction direction,
			boost::shared_ptr<ContinuationSegment> continuationSegment,
			boost::shared_ptr<EndSegment> endSegment,
			boost::shared_ptr<Slice> sourceSlice,
			boost::shared_ptr<Slice> targetSlice);

	boost::shared_ptr<ContinuationSegment> getContinuationSegment() const;

	boost::shared_ptr<EndSegment> getEndSegment() const;

	std::vector<boost::shared_ptr<Slice> > getSlices() const;

	boost::shared_ptr<Slice> getSourceSlice() const;

	boost::shared_ptr<Slice> getTargetSlice() const;

private:

	boost::shared_ptr<ContinuationSegment> _continuationSegment;

	boost::shared_ptr<EndSegment> _endSegment;

	boost::shared_ptr<Slice> _sourceSlice;

	boost::shared_ptr<Slice> _targetSlice;

};


#endif /* SEGMENTPAIREND_H_ */

