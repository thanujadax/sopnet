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
#include <inference/LinearConstraints.h>
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

	void collectSegments();

	void updateOutputs();

	void extractSegmentPairsAll();

	void extractSegmentPairs(boost::shared_ptr<ContinuationSegment> segment);

	void addNextSegmentPair(
			Direction direction,
			boost::shared_ptr<ContinuationSegment> segment1,
			boost::shared_ptr<ContinuationSegment> segment2);

	void assembleLinearConstraints();
	void assembleLinearConstraint(boost::shared_ptr<SegmentPair> segmentPair,
			double coefSegmentPair, double coefSegment1, double coefSegment2, Relation relation, double value);

    // a list of neuron segments for each pair of frames
    pipeline::Inputs<Segments>         _neuronSegments;

    // a list of mitochondria segments for each pair of frames
    pipeline::Inputs<Segments>         _mitochondriaSegments;

    // a list of synapse segments for each pair of frames
    pipeline::Inputs<Segments>         _synapseSegments;

    // all segments in the problem
    pipeline::Output<Segments>          _segmentPairs;

	// list of all segment pair linear constraints
	pipeline::Output<LinearConstraints> _linearConstraints;

    // all segments in the problem
    boost::shared_ptr<Segments>          _allSegments;
};

#endif /* SOPNET_SEGMENTS_SEGMENTPAIREXTRACTOR_H_ */
