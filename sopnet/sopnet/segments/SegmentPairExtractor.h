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
#include <sopnet/segments/SegmentPairEnd.h>
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

	void extractSegmentPairEnds(boost::shared_ptr<ContinuationSegment> segment);

	void addNextSegmentPair(
			Direction direction,
			boost::shared_ptr<ContinuationSegment> segment1,
			boost::shared_ptr<ContinuationSegment> segment2,
			boost::shared_ptr<Slice> sourceSlice,
			boost::shared_ptr<Slice> midSlice,
			boost::shared_ptr<Slice> targetSlice);

	void addNextSegmentPairEnd(
			Direction direction,
			boost::shared_ptr<ContinuationSegment> continuationSegment,
			boost::shared_ptr<EndSegment> endSegment,
			boost::shared_ptr<Slice> sourceSlice,
			boost::shared_ptr<Slice> targetSlice);

	void assembleLinearConstraints();

	void assembleLinearConstraint(boost::shared_ptr<SegmentPair> segmentPair,
			double coefSegmentPair, double coefSegment1, double coefSegment2, Relation relation, double value);

	void assembleLinearConstraint(boost::shared_ptr<SegmentPairEnd> segmentPairEnd,
			double coefSegmentPairEnd, double coefContinuation, double coefEnd, Relation relation, double value);

    // a list of neuron segments for each pair of frames
    pipeline::Inputs<Segments>         _neuronSegments;

    // a list of mitochondria segments for each pair of frames
    pipeline::Inputs<Segments>         _mitochondriaSegments;

    // a list of synapse segments for each pair of frames
    pipeline::Inputs<Segments>         _synapseSegments;

    // should segment pairs be extracted?
    pipeline::Input<bool>			_withSegmentPairs;

    // should segment pair ends be extracted?
    pipeline::Input<bool>			_withSegmentPairEnds;

    // all segment pairs in the problem
    pipeline::Output<Segments>          _segmentPairs;

    // all segment pair ends in the problem
    pipeline::Output<Segments>          _segmentPairEnds;

	// list of all segment pair linear constraints
	pipeline::Output<LinearConstraints> _linearConstraints;

    // all segments in the problem
    boost::shared_ptr<Segments>          _allSegments;
};

#endif /* SOPNET_SEGMENTS_SEGMENTPAIREXTRACTOR_H_ */
