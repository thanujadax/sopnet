/*
 * SegmentPairExtractor.h
 *
 *  Created on: May 19, 2014
 *      Author: thanuja
 */

#ifndef SOPNET_SEGMENTS_SEGMENTPAIREXTRACTOR_H_
#define SOPNET_SEGMENTS_SEGMENTPAIREXTRACTOR_H_

#include <boost/function.hpp>

#include <pipeline/all.h>
#include <sopnet/segments/Segments.h>
#include <sopnet/segments/SegmentPair.h>
#include <sopnet/segments/ContinuationSegment.h>

/*
 *
 */
class SegmentPairExtractor : public pipeline::SimpleProcessNode<>{

public:

	SegmentPairExtractor();

private:

	void updateOutputs();

	void extractSegmentPairsAll();

	void extractSegmentPairs(boost::shared_ptr<ContinuationSegment> segment);

	void addNextSegmentPair(
			Direction direction,
			boost::shared_ptr<ContinuationSegment> segment1,
			boost::shared_ptr<ContinuationSegment> segment2);

	// list of all continuation segments
	pipeline::Input<Segments>          _segments;

	// list of all segment pairs
	pipeline::Output<Segments>          _segmentPairs;

};

#endif /* SOPNET_SEGMENTS_SEGMENTPAIREXTRACTOR_H_ */
