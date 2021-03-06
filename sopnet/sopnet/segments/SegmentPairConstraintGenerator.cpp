/*
 * SegmentPairConstraintGenerator.cpp
 *
 *  Created on: Jul 24, 2014
 *      Author: thanuja
 */

#include <util/foreach.h>
#include "SegmentPairConstraintGenerator.h"

static logger::LogChannel segmentpairconstraintgeneratorlog("segmentpairconstraintgeneratorlog", "[SegmentPairConstraintGenerator] ");

SegmentPairConstraintGenerator::SegmentPairConstraintGenerator() :
		_linearConstraints(new LinearConstraints) {

	registerInput(_segmentPairs, "segment pairs", pipeline::Optional);
	registerInput(_segmentPairEnds, "segment pair ends", pipeline::Optional);
	registerInput(_withSegmentPairs, "with segment pairs");
	registerInput(_withSegmentPairEnds, "with segment pair ends");

	registerOutput(_linearConstraints,"segment pair linear constraints");
}

void
SegmentPairConstraintGenerator::updateOutputs(){

	assembleLinearConstraints();
}

void
SegmentPairConstraintGenerator::assembleLinearConstraints(){

	_linearConstraints->clear();

	if(_segmentPairs.isSet() && *_withSegmentPairs){
		/* For each segment pair p_ij consisting of segments s_i and s_j the following 2 constraints are added
		 * 1). p_ij - s_i -s_j >= -1
		 * 2). -2*p_ij + s_i + s_j >= 0
		 */
		LOG_DEBUG(segmentpairconstraintgeneratorlog) << "assembling linear constraints for segment pairs..." << std::endl;

		foreach (boost::shared_ptr<SegmentPair> segmentPair, _segmentPairs->getSegmentPairs()){

			// constraint 1
			assembleLinearConstraint(segmentPair,1.0,-1.0,-1.0,GreaterEqual,-1.0);
			// constraint 2
			assembleLinearConstraint(segmentPair,-2.0,1.0,1.0,GreaterEqual,0.0);

		}
	}

	if(_segmentPairEnds.isSet() && *_withSegmentPairEnds){
		LOG_DEBUG(segmentpairconstraintgeneratorlog) << "assembling linear constraints for segment pair ends..." << std::endl;
		/* For each segment pair end p_ij consisting of segments s_i and s_j the following 2 constraints are added
		 * 1). p_ij - s_i -s_j >= -1
		 * 2). -2*p_ij + s_i + s_j >= 0
		 */

		foreach (boost::shared_ptr<SegmentPairEnd> segmentPairEnd, _segmentPairEnds->getSegmentPairEnds()){

			// constraint 1
			assembleLinearConstraint(segmentPairEnd,1.0,-1.0,-1.0,GreaterEqual,-1.0);
			// constraint 2
			assembleLinearConstraint(segmentPairEnd,-2.0,1.0,1.0,GreaterEqual,0.0);

		}
	}

}

void
SegmentPairConstraintGenerator::assembleLinearConstraint(boost::shared_ptr<SegmentPair> segmentPair,
		double coefSegmentPair, double coefSegment1, double coefSegment2, Relation relation, double value){

	LinearConstraint constraint;
	unsigned int segmentId1,segmentId2,segmentPairId;

	segmentPairId = segmentPair->getId();
	segmentId1 = segmentPair->getContinuationSegment1()->getId();
	segmentId2 = segmentPair->getContinuationSegment2()->getId();

	constraint.setCoefficient(segmentPairId,coefSegmentPair);
	constraint.setCoefficient(segmentId1,coefSegment1);
	constraint.setCoefficient(segmentId2,coefSegment2);

	constraint.setValue(value);

	constraint.setRelation(relation);

	_linearConstraints->add(constraint);
}

void
SegmentPairConstraintGenerator::assembleLinearConstraint(boost::shared_ptr<SegmentPairEnd> segmentPairEnd,
		double coefSegmentPairEnd, double coefContinuation, double coefEnd, Relation relation, double value){

	LinearConstraint constraint;
	unsigned int continuationSegmentId,endSegmentId,segmentPairEndId;

	segmentPairEndId = segmentPairEnd->getId();
	continuationSegmentId = segmentPairEnd->getContinuationSegment()->getId();
	endSegmentId = segmentPairEnd->getEndSegment()->getId();

	constraint.setCoefficient(segmentPairEndId,coefSegmentPairEnd);
	constraint.setCoefficient(continuationSegmentId,coefContinuation);
	constraint.setCoefficient(endSegmentId,coefEnd);

	constraint.setValue(value);

	constraint.setRelation(relation);

	_linearConstraints->add(constraint);
}
