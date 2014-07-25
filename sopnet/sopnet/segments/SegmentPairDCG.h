/*
 * SegmentPairDCG.h
 *
 *  Created on: Jul 24, 2014
 *      Author: thanuja
 */

#ifndef SOPNET_SEGMENTS_SEGMENTPAIRDCG_H_
#define SOPNET_SEGMENTS_SEGMENTPAIRDCG_H_

#include <pipeline/all.h>
#include <sopnet/features/Features.h>

#include "Segments.h"


class SegmentPairDCG : public pipeline::SimpleProcessNode<> {

public:
	SegmentPairDCG();

private :

	void updateOutputs();

    // all segment pairs in the problem
    pipeline::Input<Segments>          _segmentPairsIn;

    // selected segment pairs
    pipeline::Output<Segments>          _segmentPairsOut;

    // all segments in the problem
    pipeline::Output<Features>          _segmentPairFeaturesOut;

};

#endif /* SOPNET_SEGMENTS_SEGMENTPAIRDCG_H_ */
