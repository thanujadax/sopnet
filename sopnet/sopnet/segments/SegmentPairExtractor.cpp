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
	_segmentPairEnds(new Segments()),
	_linearConstraints(new LinearConstraints),
	_allSegments(boost::make_shared<Segments>()){

    registerInputs(_neuronSegments, "neuron segments");
    registerInputs(_mitochondriaSegments, "mitochondria segments");
    registerInputs(_synapseSegments, "synapse segments");
    registerInput(_withSegmentPairs, "with segment pairs");
    registerInput(_withSegmentPairEnds, "with segment pair ends");

	registerOutput(_segmentPairs, "segment pairs");
	registerOutput(_segmentPairEnds, "segment pair ends");
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

	if(*_withSegmentPairs){
		foreach (boost::shared_ptr<ContinuationSegment> segment, _allSegments->getContinuations() ) {
			extractSegmentPairs(segment);
			}
		LOG_DEBUG(segmentpairextractorlog) << "collected " << _segmentPairs->size() << " segmentPairs" << std::endl;
	}

	if(*_withSegmentPairEnds){
		foreach (boost::shared_ptr<ContinuationSegment> segment, _allSegments->getContinuations() ) {
			extractSegmentPairEnds(segment);
			}
		LOG_DEBUG(segmentpairextractorlog) << "collected " << _segmentPairEnds->size() << " segmentPairEnds" << std::endl;
	}
}

void
SegmentPairExtractor::extractSegmentPairs(boost::shared_ptr<ContinuationSegment> segment){

	// for this segment, extract all possible segment pairs that continue from its target slice

	// get interSectionInterval
	unsigned int thisInterSectionInterval = segment->getInterSectionInterval();
	unsigned int nextInterSectionInterval = thisInterSectionInterval +1;

	// get all segments in the next interSectionInterval, continuing from this segment
	std::vector<boost::shared_ptr<ContinuationSegment> > nextIntervalContinuationSegments = _allSegments->getContinuations(nextInterSectionInterval);

	foreach(boost::shared_ptr<ContinuationSegment> nextSegment, nextIntervalContinuationSegments){
		if (segment->getDirection()==Left){
			if(nextSegment->getDirection() == Left){
				if(segment->getSourceSlice() == nextSegment->getTargetSlice()) {
					// this segment goes left, next segment goes left
					addNextSegmentPair(Left,segment,nextSegment,
							segment->getTargetSlice(),
							segment->getSourceSlice(),
							nextSegment->getSourceSlice());
				}
			} else {
				if(segment->getSourceSlice() == nextSegment->getSourceSlice()) {
					// this segment goes left, next segment goes right
					addNextSegmentPair(Left,segment,nextSegment,
							segment->getTargetSlice(),
							segment->getSourceSlice(),
							nextSegment->getTargetSlice());
				}
			}
		} else {
			// this segment goes right
			if(nextSegment->getDirection() == Left){
				if(segment->getTargetSlice() == nextSegment->getTargetSlice()) {
					// this segment goes right, next segment goes left
					addNextSegmentPair(Right,segment,nextSegment,
							segment->getSourceSlice(),
							segment->getTargetSlice(),
							nextSegment->getSourceSlice());
				}
			} else {
				if(segment->getTargetSlice() == nextSegment->getSourceSlice()) {
					// this segment goes right, next segment goes right
					addNextSegmentPair(Right,segment,nextSegment,
							segment->getSourceSlice(),
							segment->getTargetSlice(),
							nextSegment->getTargetSlice());
				}
			}

		}
	}

}

void
SegmentPairExtractor::extractSegmentPairEnds(boost::shared_ptr<ContinuationSegment> segment){

	// for this segment, extract all possible segment pair ends

	// get interSectionInterval
	unsigned int thisInterSectionInterval = segment->getInterSectionInterval();
	unsigned int nextInterSectionInterval = thisInterSectionInterval +1;
	LOG_ALL(segmentpairextractorlog) << "nextInterSectionInterval: " << nextInterSectionInterval << std::endl;
	// get all segments in the next interSectionInterval, continuing from this segment
	std::vector<boost::shared_ptr<EndSegment> > nextIntervalEndSegments = _allSegments->getEnds(nextInterSectionInterval);


	if(segment->getDirection() == Right) {

		// from the end segment of the previous slice
		if(thisInterSectionInterval>1){
			unsigned int prevInterSectionInterval = thisInterSectionInterval - 1;
			// get all end segments in the prev interSectionInterval, continuing from this segment
			std::vector<boost::shared_ptr<EndSegment> > prevIntervalEndSegments
						= _allSegments->getEnds(prevInterSectionInterval);
			foreach(boost::shared_ptr<EndSegment> prevEndSegment, prevIntervalEndSegments){

				if(prevEndSegment->getDirection() == Left && (segment->getSourceSlice() == prevEndSegment->getSlice())){

					addNextSegmentPairEnd(Left,segment,prevEndSegment);
					LOG_ALL(segmentpairextractorlog) << "segment pair end added" << std::endl;
				}
			}

		}

		if(nextInterSectionInterval < _allSegments->getNumInterSectionIntervals()){
			// from the end segment of the next slice
			std::vector<boost::shared_ptr<EndSegment> > nextIntervalEndSegments
						= _allSegments->getEnds(nextInterSectionInterval);
			foreach(boost::shared_ptr<EndSegment> nextEndSegment, nextIntervalEndSegments){

				if(nextEndSegment->getDirection() == Right && (segment->getTargetSlice() == nextEndSegment->getSlice())){

					addNextSegmentPairEnd(Right,segment,nextEndSegment);
					LOG_ALL(segmentpairextractorlog) << "segment pair end added" << std::endl;
				}
			}
		}

	} else {
		// continuation segment goes left
		// from the end segment of the previous slice
		if(thisInterSectionInterval>1){
			unsigned int prevInterSectionInterval = thisInterSectionInterval - 1;
			// get all end segments in the prev interSectionInterval, continuing from this segment
			std::vector<boost::shared_ptr<EndSegment> > prevIntervalEndSegments
						= _allSegments->getEnds(prevInterSectionInterval);
			foreach(boost::shared_ptr<EndSegment> prevEndSegment, prevIntervalEndSegments){
				if(prevEndSegment->getDirection() == Left && (segment->getTargetSlice() == prevEndSegment->getSlice())){
					addNextSegmentPairEnd(Left,segment,prevEndSegment);
					LOG_ALL(segmentpairextractorlog) << "segment pair end added" << std::endl;
				}
			}

		}


		if(nextInterSectionInterval < _allSegments->getNumInterSectionIntervals()){
			// from the end segment of the next slice
			std::vector<boost::shared_ptr<EndSegment> > nextIntervalEndSegments
						= _allSegments->getEnds(nextInterSectionInterval);

			foreach(boost::shared_ptr<EndSegment> nextEndSegment, nextIntervalEndSegments){

				if(nextEndSegment->getDirection() == Right && (segment->getSourceSlice() == nextEndSegment->getSlice())){

					addNextSegmentPairEnd(Right,segment,nextEndSegment);
					LOG_ALL(segmentpairextractorlog) << "segment pair end added" << std::endl;
				}
			}
		}
	}


}


void
SegmentPairExtractor::addNextSegmentPair(
		Direction direction,
		boost::shared_ptr<ContinuationSegment> segment1,
		boost::shared_ptr<ContinuationSegment> segment2,
		boost::shared_ptr<Slice> sourceSlice,
		boost::shared_ptr<Slice> midSlice,
		boost::shared_ptr<Slice> targetSlice){

	boost::shared_ptr<SegmentPair> segmentPair = boost::make_shared<SegmentPair>(
			Segment::getNextSegmentId(),
			direction ,
			segment1,
			segment2,
			sourceSlice,
			midSlice,
			targetSlice);

	_segmentPairs->add(segmentPair);
}

void
SegmentPairExtractor::addNextSegmentPairEnd(
		Direction direction,
		boost::shared_ptr<ContinuationSegment> continuationSegment,
		boost::shared_ptr<EndSegment> endSegment){

	boost::shared_ptr<SegmentPairEnd> segmentPairEnd = boost::make_shared<SegmentPairEnd>(
			Segment::getNextSegmentId(),
			direction ,
			continuationSegment,
			endSegment);

	_segmentPairEnds->add(segmentPairEnd);
}

void
SegmentPairExtractor::assembleLinearConstraints(){

	if(*_withSegmentPairs){
		LOG_DEBUG(segmentpairextractorlog) << "assembling linear constraints for segment pairs..." << std::endl;

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

	if(*_withSegmentPairEnds){
		LOG_DEBUG(segmentpairextractorlog) << "assembling linear constraints for segment pair ends..." << std::endl;
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

void
SegmentPairExtractor::assembleLinearConstraint(boost::shared_ptr<SegmentPairEnd> segmentPairEnd,
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
