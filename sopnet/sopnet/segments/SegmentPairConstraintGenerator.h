/*
 * SegmentPairConstraintGenerator.h
 *
 *  Created on: Jul 24, 2014
 *      Author: thanuja
 */

#ifndef SOPNET_SEGMENTS_SEGMENTPAIRCONSTRAINTGENERATOR_H_
#define SOPNET_SEGMENTS_SEGMENTPAIRCONSTRAINTGENERATOR_H_

#include <boost/function.hpp>

#include <pipeline/all.h>
#include <inference/LinearConstraints.h>
#include "Segments.h"
#include "SegmentPair.h"
#include "SegmentPairEnd.h"
#include "ContinuationSegment.h"


class SegmentPairConstraintGenerator : public pipeline::SimpleProcessNode<>  {

public:
	SegmentPairConstraintGenerator();

private:
	void updateOutputs();
	void assembleLinearConstraints();
	void assembleLinearConstraint(boost::shared_ptr<SegmentPair> segmentPair,
			double coefSegmentPair, double coefSegment1, double coefSegment2, Relation relation, double value);
	void assembleLinearConstraint(boost::shared_ptr<SegmentPairEnd> segmentPairEnd,
			double coefSegmentPairEnd, double coefContinuation, double coefEnd, Relation relation, double value);

    // a list of segment pairs for each pair of frames
    pipeline::Input<Segments>         _segmentPairs;

    // a list of segment pair ends for each pair of frames
    pipeline::Input<Segments>         _segmentPairEnds;

    // should segment pairs be extracted?
    pipeline::Input<bool>			_withSegmentPairs;

    // should segment pair ends be extracted?
    pipeline::Input<bool>			_withSegmentPairEnds;

	// list of all segment pair and segment pair end linear constraints
	pipeline::Output<LinearConstraints> _linearConstraints;

};

#endif /* SOPNET_SEGMENTS_SEGMENTPAIRCONSTRAINTGENERATOR_H_ */
