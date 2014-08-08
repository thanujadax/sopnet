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
#include "ContinuationSegment.h"


class SegmentPairConstraintGenerator : public pipeline::SimpleProcessNode<>  {

public:
	SegmentPairConstraintGenerator();

private:
	void updateOutputs();
	void assembleLinearConstraints();
	void assembleLinearConstraint(boost::shared_ptr<SegmentPair> segmentPair,
			double coefSegmentPair, double coefSegment1, double coefSegment2, Relation relation, double value);

    // a list of segment pairs for each pair of frames
    pipeline::Input<Segments>         _segmentPairs;

	// list of all segment pair linear constraints
	pipeline::Output<LinearConstraints> _linearConstraints;

};

#endif /* SOPNET_SEGMENTS_SEGMENTPAIRCONSTRAINTGENERATOR_H_ */
