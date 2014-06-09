/*
 * SegmentPairFeatureExtractor.cpp
 *
 *  Created on: May 26, 2014
 *      Author: thanuja
 */

#include <fstream>
#include <math.h>
#include <cmath>

#include <boost/filesystem.hpp>

#include <util/helpers.hpp>
#include <imageprocessing/ConnectedComponent.h>
#include <sopnet/exceptions.h>
#include <sopnet/segments/ContinuationSegment.h>
#include "SegmentPairFeatureExtractor.h"

logger::LogChannel segmentpairfeatureextractorlog("segmentpairfeatureextractorlog", "[SegmentPairFeatureExtractor] ");

SegmentPairFeatureExtractor::SegmentPairFeatureExtractor() :
	_features(new Features()),
	_dz(10.0){

    registerInput(_segments, "segments");
    registerOutput(_features, "features");

}

SegmentPairFeatureExtractor::~SegmentPairFeatureExtractor() {
	// TODO Auto-generated destructor stub
}

void
SegmentPairFeatureExtractor::updateOutputs(){

    LOG_DEBUG(segmentpairfeatureextractorlog) << "extracting features" << std::endl;

    unsigned int numSegmentPairFeatures = 5;

    _features->clear();

    _features->resize(_segments->size(), numSegmentPairFeatures);

    _features->addName("sp 2nd segment relative offset");
    _features->addName("sp product of change of area of two segments");
    _features->addName("sp abs of product of change of area");
    _features->addName("sp 2nd derivative of area");
    _features->addName("sp abs of 2nd derivative of area");


    foreach (boost::shared_ptr<EndSegment> segment, _segments->getEnds())
                    computeFeatures(segment, _features->get(segment->getId()));

    foreach (boost::shared_ptr<ContinuationSegment> segment, _segments->getContinuations())
                    computeFeatures(segment, _features->get(segment->getId()));

    foreach (boost::shared_ptr<BranchSegment> segment, _segments->getBranches())
                    computeFeatures(segment, _features->get(segment->getId()));

    foreach (boost::shared_ptr<SegmentPair> segment, _segments->getSegmentPairs())
                    computeFeatures(segment, _features->get(segment->getId()));

    LOG_ALL(segmentpairfeatureextractorlog) << "found features: " << *_features << std::endl;

    LOG_DEBUG(segmentpairfeatureextractorlog) << "done" << std::endl;
}

void
SegmentPairFeatureExtractor::computeFeatures(const boost::shared_ptr<EndSegment> end, std::vector<double>& features){

	features[0] = Features::NoFeatureValue;
	features[1] = Features::NoFeatureValue;
	features[2] = Features::NoFeatureValue;
	features[3] = Features::NoFeatureValue;
	features[4] = Features::NoFeatureValue;
}

void
SegmentPairFeatureExtractor::computeFeatures(const boost::shared_ptr<ContinuationSegment> continuation, std::vector<double>& features){

	features[0] = Features::NoFeatureValue;
	features[1] = Features::NoFeatureValue;
	features[2] = Features::NoFeatureValue;
	features[3] = Features::NoFeatureValue;
	features[4] = Features::NoFeatureValue;
}

void
SegmentPairFeatureExtractor::computeFeatures(const boost::shared_ptr<BranchSegment> branch, std::vector<double>& features){

	features[0] = Features::NoFeatureValue;
	features[1] = Features::NoFeatureValue;
	features[2] = Features::NoFeatureValue;
	features[3] = Features::NoFeatureValue;
	features[4] = Features::NoFeatureValue;
}

void
SegmentPairFeatureExtractor::computeFeatures(const boost::shared_ptr<SegmentPair> segmentPair, std::vector<double>& features){

	boost::shared_ptr<ContinuationSegment> segment1 = segmentPair->getContinuationSegment1();
	boost::shared_ptr<ContinuationSegment> segment2 = segmentPair->getContinuationSegment2();

	boost::shared_ptr<Slice> slice1, slice2, slice3;

	if(segment1->getDirection()==Right) {
		slice1 = segment1->getSourceSlice();
		slice2 = segment1->getTargetSlice();
	}
	else {
		slice2 = segment1->getSourceSlice();
		slice1 = segment1->getTargetSlice();
	}

	if(segment2->getDirection()==Right)
		slice3 = segment2->getTargetSlice();
	else
		slice3 = segment2->getSourceSlice();

	// TODO:
	// fill zeros for feature[0] to feature[numEnds+numContinuations+numBranches -1]
	// start with feature[numEnds+numContinuations+numBranches] instead of features[0]
	features[0] = getRelativeOffset(slice1,slice2,slice3);
	features[1] = getAreaChangeProduct(slice1,slice2,slice3);
	features[2] = getAbsVal(features[1]);
	features[3] = getD2Area(slice1,slice2,slice3);
	features[4] = getAbsVal(features[3]);
}

double
SegmentPairFeatureExtractor::getRelativeOffset(
		const boost::shared_ptr<Slice> slice1,
		const boost::shared_ptr<Slice> slice2,
		const boost::shared_ptr<Slice> slice3){

	double offset, x1, x2, y1, y2, x3, y3;


	x1 = slice1->getComponent()->getCenter().x;
	y1 = slice1->getComponent()->getCenter().y;

	x2 = slice2->getComponent()->getCenter().x;
	y2 = slice2->getComponent()->getCenter().y;

	x3 = slice3->getComponent()->getCenter().x;
	y3 = slice3->getComponent()->getCenter().y;

	offset = acos(
			((x2-x1)*(x3-x2)+(y2-y1)*(y3-y2)+_dz*_dz) /
			(sqrt(pow((x2-x1),2)+pow((y2-y1),2)+pow(_dz,2))*sqrt(pow((x3-x2),2)+pow((y3-y2),2)+pow(_dz,2)))
			);

	return offset;
}

double
SegmentPairFeatureExtractor::getAreaChangeProduct(
		const boost::shared_ptr<Slice> slice1,
		const boost::shared_ptr<Slice> slice2,
		const boost::shared_ptr<Slice> slice3){

	// returns the product of change of area of the two segments in the segmentPair

	double a1, a2, a3;

	a1 = (double)slice1->getComponent()->getSize();
	a2 = (double)slice2->getComponent()->getSize();
	a3 = (double)slice3->getComponent()->getSize();

	return (a2-a1)*(a3-a2)/(a1*a2);

}

double
SegmentPairFeatureExtractor::getAbsVal(double input){

	if(input<0)
		return input * -1.0;
	else
		return input;

}

double
SegmentPairFeatureExtractor::getD2Area(
		const boost::shared_ptr<Slice> slice1,
		const boost::shared_ptr<Slice> slice2,
		const boost::shared_ptr<Slice> slice3){

	double a1, a2, a3;

	a1 = (double)slice1->getComponent()->getSize();
	a2 = (double)slice2->getComponent()->getSize();
	a3 = (double)slice3->getComponent()->getSize();

	return (a3-2*a2+a1)/(_dz*_dz);
}
