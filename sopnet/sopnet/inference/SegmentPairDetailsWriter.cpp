/*
 * SegmentPairDetailsWriter.cpp
 *
 *  Created on: Jun 4, 2014
 *      Author: thanuja
 */

#include <fstream>

#include <boost/lexical_cast.hpp>

#include "SegmentPairDetailsWriter.h"

SegmentPairDetailsWriter::SegmentPairDetailsWriter() {

	registerInput(_segments, "segments");
	registerInput(_problemConfiguration, "problem configuration");
	registerInputs(_linearConstraints, "linear constraints");
	registerInput(_features, "features");
	registerInput(_linearCostFunction, "linear cost function");
}

void
ProblemGraphWriter::writeSegmentPairDetails(
		const std::string segmentPairPropertiesFile,
		const std::string segmentPairConstraintsFile){

	updateInputs();

	if (!_segments.isSet() || !_problemConfiguration.isSet() ) {

		LOG_DEBUG(problemgraphwriterlog) << "not all required inputs are present -- skip dumping" << std::endl;
		return;
	}


	// LOG_DEBUG(problemgraphwriterlog) << "writing down details of segment pairs ..."  << std::endl;

	// writeSegmentPairProperties(segmentPairPropertiesFile);

	LOG_DEBUG(problemgraphwriterlog) << "writing down constraints for segment pairs ..."  << std::endl;

	writeSegmentPairConstraints(segmentPairConstraintsFile);


}

void
ProblemGraphWriter::writeSegmentPairProperties(const std::string segmentPairPropertiesFile){
	/* for each segment pair, write down
	 * segment pair id, variable id,
	 * segment1 id, variable id, segment2 id, variable id
	 * for seg1 and 2, {center, area} of {source slice, target slice}
	 */
	boost::shared_ptr<ContinuationSegment> segment1, segment2;
	boost::shared_ptr<Slice> slice1, slice2, slice3;

	unsigned int segmentPairId, segment1Id, segment2Id, segmentPairVarID, segment1VarID, segment2VarID;

	std::ofstream segmentPairPropertiesOutput(segmentPairPropertiesFile.c_str());
	segmentPairPropertiesOutput.open(segmentPairPropertiesFile.c_str());


	foreach(boost::shared_ptr<SegmentPair> segmentPair, _segments->getSegmentPairs() ){

		segment1 = segmentPair->getContinuationSegment1();
		segment2 = segmentPair->getContinuationSegment2();

		if(segment1->getDirection()==Left){

			slice1 = segment1->getTargetSlice();
			slice2 = segment1->getSourceSlice();

		} else {

			slice2 = segment1->getTargetSlice();
			slice1 = segment1->getSourceSlice();

		}

		if(segment2->getDirection()==Left)
			slice3 = segment2->getSourceSlice();
		else
			slice3 = segment2->getTargetSlice();


		writeSegmentPairCosts(segmentPair,segmentPairPropertiesOutput);

		writeSegmentPairFeatures(segmentPair,segmentPairPropertiesOutput);

		segmentPairPropertiesOutput.close();
	}

}

void
ProblemGraphWriter::writeSegmentPairCosts(boost::shared_ptr<SegmentPair> segmentPair,std::ofstream& out){
	/*
	 *
	 */

}

void
ProblemGraphWriter::writeSegmentPairFeatures(boost::shared_ptr<SegmentPair> segmentPair,std::ofstream& out){
	/*
	 *
	 */

}

void
ProblemGraphWriter::writeSegmentPairConstraints(const std::string segmentPairConstraintsFile){
	/* From the total number of constraints, only pick the constraints relevant for segment pairs.
	 * Currently we have 2 constraints per each segment pair and they are the last block of linear constraints
	 */

	std::ofstream constraintOutput;
	constraintOutput.open(segmentPairConstraintsFile.c_str());

	unsigned int numSegPairs = _segments->getSegmentPairs().size();
	unsigned int numSegPairConstraints = numSegPairs * 2;

	unsigned int segPairConstraintStart, segPairConstraintStop;

	segPairConstraintStop = _linearConstraints.size(); // less than
	segPairConstraintStart = segPairConstraintStop - numSegPairConstraints;

	for(unsigned int i=segPairConstraintStart; i<segPairConstraintStop; i++){

		constraintOutput << _linearConstraints[i] << std::endl;

	}

	constraintOutput.close();

}
