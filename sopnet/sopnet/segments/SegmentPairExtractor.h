/*
 * SegmentPairExtractor.h
 *
 *  Created on: May 19, 2014
 *      Author: thanuja
 */

#ifndef SOPNET_SEGMENTS_SEGMENTPAIREXTRACTOR_H_
#define SOPNET_SEGMENTS_SEGMENTPAIREXTRACTOR_H_

#include <pipeline/all.h>
#include <sopnet/segments/Segments.h>

/*
 *
 */
class SegmentPairExtractor : public pipeline::SimpleProcessNode<>{

public:

	SegmentPairExtractor();



private:

	void updateOutputs();

	void extractSegmentPairs();

	// list of all continuation segments
	pipeline::Inputs<Segments>          _allSegments;

	// list of all segment pairs
	pipeline::Output<Segments>          _segmentPairs;

};

#endif /* SOPNET_SEGMENTS_SEGMENTPAIREXTRACTOR_H_ */
