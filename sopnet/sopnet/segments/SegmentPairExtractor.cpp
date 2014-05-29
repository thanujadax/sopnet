/*
 * SegmentPairExtractor.cpp
 *
 *  Created on: May 19, 2014
 *      Author: thanuja
 */

#include <util/foreach.h>
#include "SegmentPairExtractor.h"

static logger::LogChannel segmentpairextractorlog("segmentpairextractorlog", "[SegmentPairExtractor] ");

SegmentPairExtractor::SegmentPairExtractor() :
	_segmentPairs(new Segments()),
	_linearConstraints(new LinearConstraints),
	_allSegments(boost::make_shared<Segments>()){

    registerInputs(_neuronSegments, "neuron segments");
    registerInputs(_mitochondriaSegments, "mitochondria segments");
    registerInputs(_synapseSegments, "synapse segments");

	registerOutput(_segmentPairs, "segment pairs");
	registerOutput(_linearConstraints,"segment pair linear constraints");

}

void
SegmentPairExtractor::updateOutputs(){

	collectSegments();
	extractSegmentPairsAll();
	assembleLinearConstraints();


}

void
SegmentPairExtractor::collectSegments() {

        LOG_DEBUG(segmentpairextractorlog) << "collecting segments..." << std::endl;

        _allSegments->clear();

        foreach (boost::shared_ptr<Segments> segments, _neuronSegments) {

                _allSegments->addAll(segments);

        }

        foreach (boost::shared_ptr<Segments> segments, _mitochondriaSegments) {

                _allSegments->addAll(segments);

        }

        foreach (boost::shared_ptr<Segments> segments, _synapseSegments) {

                _allSegments->addAll(segments);

        }

        LOG_DEBUG(segmentpairextractorlog) << "collected " << _allSegments->size() << " segments" << std::endl;
}

void
SegmentPairExtractor::extractSegmentPairsAll(){

	foreach (boost::shared_ptr<ContinuationSegment> segment, _allSegments->getContinuations() ) {
		extractSegmentPairs(segment);
		}
	LOG_DEBUG(segmentpairextractorlog) << "collected " << _segmentPairs->size() << " segmentPairs" << std::endl;
}

void
SegmentPairExtractor::extractSegmentPairs(boost::shared_ptr<ContinuationSegment> segment){

	// for this segment, extract all possible segment pairs that continue from its target slice

	// get interSectionInterval
	unsigned int nextInterSectionInterval = segment->getInterSectionInterval() + 1;

	// get all segments in the next interSectionInterval, continuing from this segment
	std::vector<boost::shared_ptr<ContinuationSegment> > nextIntervalContinuationSegments = _allSegments->getContinuations(nextInterSectionInterval);

	foreach(boost::shared_ptr<ContinuationSegment> nextSegment, nextIntervalContinuationSegments){
		if (segment->getDirection()==Left){
			if(nextSegment->getDirection() == Left){
				if(segment->getSourceSlice() == nextSegment->getTargetSlice()) {
					// this segment goes left, next segment goes left
					addNextSegmentPair(Left,segment,nextSegment);
				}
			} else {
				if(segment->getSourceSlice() == nextSegment->getSourceSlice()) {
					// this segment goes left, next segment goes right
					addNextSegmentPair(Left,segment,nextSegment);
				}
			}
		} else {
			// this segment goes right
			if(nextSegment->getDirection() == Left){
				if(segment->getTargetSlice() == nextSegment->getTargetSlice()) {
					// this segment goes right, next segment goes left
					addNextSegmentPair(Right,segment,nextSegment);
				}
			} else {
				if(segment->getTargetSlice() == nextSegment->getSourceSlice()) {
					// this segment goes right, next segment goes right
					addNextSegmentPair(Right,segment,nextSegment);
				}
			}

		}

	}
}

void
SegmentPairExtractor::addNextSegmentPair(
		Direction direction,
		boost::shared_ptr<ContinuationSegment> segment1,
		boost::shared_ptr<ContinuationSegment> segment2){

	boost::shared_ptr<SegmentPair> segmentPair = boost::make_shared<SegmentPair>(
			Segment::getNextSegmentId(),
			direction ,
			segment1,
			segment2);

	_segmentPairs->add(segmentPair);
}

void
SegmentPairExtractor::assembleLinearConstraints(){
	LOG_DEBUG(segmentpairextractorlog) << "assembling linear constraints..." << std::endl;

	_linearConstraints->clear();

	/* For each segment pair p_ij consisting of segments s_i and s_j the following 2 constraints are added
	 * 1). p_ij - s_i -s_j >= -1
	 * 2). -2*p_ij + s_i + s_j >= 0
	 */


	foreach (boost::shared_ptr<SegmentPair> segmentPair, _segmentPairs->getSegmentPairs()){

		// constraint 1
		assembleLinearConstraint(segmentPair,1.0,-1.0,-1.0,GreaterEqual,-1.0);
		// constraint 2
		assembleLinearConstraint(segmentPair,-2.0,1.0,1.0,GreaterEqual,0.0);

	}
}

void
SegmentPairExtractor::assembleLinearConstraint(boost::shared_ptr<SegmentPair> segmentPair,
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
