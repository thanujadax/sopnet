/*
 * SegmentPairDetailsWriter.cpp
 *
 *  Created on: Jun 4, 2014
 *      Author: thanuja
 */

#include <fstream>

#include <boost/lexical_cast.hpp>

#include "SegmentPairDetailsWriter.h"

static logger::LogChannel segmentpairdetailswriterlog("segmentpairdetailswriterlog", "[SegmentPairDetailsWriter] ");

SegmentPairDetailsWriter::SegmentPairDetailsWriter() {

	registerInput(_segments, "segments");
	registerInput(_problemConfiguration, "problem configuration");
	registerInput(_linearConstraints, "linear constraints");
	registerInput(_features, "features");
}

void
SegmentPairDetailsWriter::writeSegmentPairDetails(
		const std::string segmentPairPropertiesFile,
		const std::string segmentPairConstraintsFile){

	updateInputs();

	if (!_segments.isSet() || !_problemConfiguration.isSet() ) {

		LOG_DEBUG(segmentpairdetailswriterlog) << "not all required inputs are present -- skip dumping" << std::endl;
		return;
	}


	LOG_DEBUG(segmentpairdetailswriterlog) << "writing down details of segment pairs ..."  << std::endl;

	writeSegmentPairProperties(segmentPairPropertiesFile);

	LOG_DEBUG(segmentpairdetailswriterlog) << "writing down constraints for segment pairs ..."  << std::endl;

	writeSegmentPairConstraints(segmentPairConstraintsFile);


}

void
SegmentPairDetailsWriter::writeSegmentPairProperties(const std::string segmentPairPropertiesFile){
	/* for each segment pair, write down
	 * segment pair id, variable id,
	 * segment1 id, variable id, segment2 id, variable id
	 * for seg1 and 2, {center, area} of {source slice, target slice}
	 */

	std::ofstream segmentPairPropertiesOutput;
	segmentPairPropertiesOutput.open(segmentPairPropertiesFile.c_str());


	foreach(boost::shared_ptr<SegmentPair> segmentPair, _segments->getSegmentPairs() ){

		writeSegmentPairComponentDetails(segmentPair,segmentPairPropertiesOutput);

		writeSegmentPairCosts(segmentPair,segmentPairPropertiesOutput);

		writeSegmentPairFeatures(segmentPair,segmentPairPropertiesOutput);

		segmentPairPropertiesOutput << std::endl;

	}

	segmentPairPropertiesOutput.close();

}

void
SegmentPairDetailsWriter::writeSegmentPairComponentDetails(boost::shared_ptr<SegmentPair> segmentPair,std::ofstream& propOutput){

	boost::shared_ptr<ContinuationSegment> segment1, segment2;
	boost::shared_ptr<Slice> slice1, slice2, slice3;

	unsigned int segmentPairId, segment1Id, segment2Id, segmentPairVarId, segment1VarId, segment2VarId;
	unsigned int segmentPairIsi,segmentPairSourceSection, segmentPairTargetSection,
	segment1SourceSection, segment2SourceSection, segment1TargetSection, segment2TargetSection,
	segment1Isi, segment2Isi;
	unsigned int area1, area2, area3;

	Direction segmentPairDirection, segment1Direction, segment2Direction;

	double x1, x2, y1, y2, x3, y3;

	segmentPairId = segmentPair->getId();
	segmentPairVarId = _problemConfiguration->getVariable(segmentPairId);
	segmentPairIsi = segmentPair->getInterSectionInterval();
	segmentPairDirection = segmentPair->getDirection();

	segment1 = segmentPair->getContinuationSegment1();
	segment2 = segmentPair->getContinuationSegment2();

	segment1Id = segment1->getId();
	segment1VarId = _problemConfiguration->getVariable(segment1Id);
	segment1Isi = segment1->getInterSectionInterval();
	segment2Id = segment2->getId();
	segment2VarId = _problemConfiguration->getVariable(segment2Id);
	segment2Isi = segment2->getInterSectionInterval();

	if(segment1->getDirection()==Left){
		segment1Direction = Left;
		slice1 = segment1->getTargetSlice();
		slice2 = segment1->getSourceSlice();

	} else {
		segment1Direction = Right;
		slice2 = segment1->getTargetSlice();
		slice1 = segment1->getSourceSlice();

	}

	if(segment2->getDirection()==Left) {
		segment2Direction = Left;
		slice3 = segment2->getSourceSlice();
	}
	else {
		segment2Direction = Right;
		slice3 = segment2->getTargetSlice();
	}

	area1 = slice1->getComponent()->getSize();
	area2 = slice2->getComponent()->getSize();
	area3 = slice3->getComponent()->getSize();

	x1 = slice1->getComponent()->getCenter().x;
	y1 = slice1->getComponent()->getCenter().y;

	x2 = slice2->getComponent()->getCenter().x;
	y2 = slice2->getComponent()->getCenter().y;

	x3 = slice3->getComponent()->getCenter().x;
	y3 = slice3->getComponent()->getCenter().y;

	segmentPairSourceSection = slice1->getSection();
	segmentPairTargetSection = slice3->getSection();

	segment1SourceSection = segment1->getSourceSlice()->getSection();
	segment1TargetSection = segment1->getTargetSlice()->getSection();
	segment2SourceSection = segment2->getSourceSlice()->getSection();
	segment2TargetSection = segment2->getTargetSlice()->getSection();

	propOutput << "SegPID " << segmentPairId << "; SegPVarID " << segmentPairVarId << "; ";
	propOutput << "SegPISI " << segmentPairIsi << "; SegPDirection " << segmentPairDirection << "; ";
	propOutput << "SegPSrcSection " << segmentPairSourceSection << "; " << "SegPDstSection " << segmentPairTargetSection << "; ";
	propOutput << "Seg1ID " << segment1Id << "; Seg1VarID " << segment1VarId << "; Seg1ISI " << segment1Isi << "; seg1Dir" << segment1Direction << "; ";
	propOutput << "Seg1SrcSection " << segment1SourceSection << "; Seg1DestSection " << segment1TargetSection << "; ";
	propOutput << "Seg2ID " << segment2Id << "; Seg2VarID " << segment2VarId << "; Seg2ISI " << segment2Isi << "; seg2Dir" << segment2Direction << "; ";
	propOutput << "Seg2SrcSection " << segment2SourceSection << "; Seg2DestSection " << segment2TargetSection << "; ";

	propOutput << "a1 " << area1 << "; ";
	propOutput << "a2 " << area2 << "; ";
	propOutput << "a3 " << area3 << "; ";

	propOutput << "centroids (" << x1 << "," << y1 << "), ("<< x2 << "," << y2 << "), ("<< x3 << "," << y3 << "); ";

}

void
SegmentPairDetailsWriter::writeSegmentPairCosts(boost::shared_ptr<SegmentPair> segmentPair,std::ofstream& out){
	/*
	 *
	 */

}

void

SegmentPairDetailsWriter::writeSegmentPairFeatures(boost::shared_ptr<SegmentPair> segmentPair,std::ofstream& featuresOutput){
	/* So far we only have 3 features
	 * # 60 - offset
	 * # 61 - multiple of dA/dz of seg1 and seg2
	 * # 62 - abs of 61
	 */

	const std::vector<double>& features = _features->get(segmentPair->getId());
	/*for (unsigned int j = 0; j < features.size(); j++) {
		featuresOutput << features[j] << " ";
	}
	featuresOutput << std::endl;
	*/
	featuresOutput << "offset " << features[59] << "; ";
	featuresOutput << "changeOfAreaMult " << features[60] << "; ";
	featuresOutput << "abs_changeOfAreaMult " << features[61] << "; ";
	featuresOutput << "d2A " << features[62] << "; ";
	featuresOutput << "abs_d2A " << features[63] << "; ";

}

void
SegmentPairDetailsWriter::writeSegmentPairConstraints(const std::string segmentPairConstraintsFile){
	/* From the total number of constraints, only pick the constraints relevant for segment pairs.
	 * Currently we have 2 constraints per each segment pair and they are the last block of linear constraints
	 */

	std::ofstream constraintOutput;
	constraintOutput.open(segmentPairConstraintsFile.c_str());

	unsigned int numSegPairs = _segments->getSegmentPairs().size();
	unsigned int numSegPairConstraints = numSegPairs * 2;

/*	unsigned int segPairConstraintStart, segPairConstraintStop;

	segPairConstraintStop = _linearConstraints->size(); // less than
	segPairConstraintStart = segPairConstraintStop - numSegPairConstraints;*/

	constraintOutput << "Total number constraints = " << _linearConstraints->size() << std::endl;
	constraintOutput << "Number of segment pair constraints = " << numSegPairConstraints << std::endl;

/*	for(unsigned int i=segPairConstraintStart; i<segPairConstraintStop; i++){

		LinearConstraint& constraint = *_linearConstraints[i];
		constraintOutput << constraint << std::endl;

	}*/

	foreach (const LinearConstraint& constraint, *_linearConstraints) {
	        	constraintOutput << constraint << std::endl;
	}

	constraintOutput.close();

}
