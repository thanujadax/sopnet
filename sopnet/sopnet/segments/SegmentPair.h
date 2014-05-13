/*
 * SegmentPair.h
 *
 *  Created on: May 13, 2014
 *      Author: thanuja
 */

#ifndef SOPNET_SEGMENTPAIR_H_
#define SOPNET_SEGMENTPAIR_H_

#include "Segment.h"

// forward declarations
class ContinuationSegment;

class SegmentPair: public Segment {

public:

	SegmentPair(unsigned int id, Direction direction,
			boost::shared_ptr<ContinuationSegment> continuationSegment1,
			boost::shared_ptr<ContinuationSegment> continuationSegment2);

	boost::shared_ptr<ContinuationSegment> getContinuationSegment1() const;

	boost::shared_ptr<ContinuationSegment> getContinuationSegment2() const;

private:

	boost::shared_ptr<ContinuationSegment> _continuationSegment1;

	boost::shared_ptr<ContinuationSegment> _continuationSegment2;

};

#endif /* SOPNET_SEGMENTPAIR_H_ */

