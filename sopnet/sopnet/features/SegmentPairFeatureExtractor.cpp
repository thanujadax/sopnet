/*
 * SegmentPairFeatureExtractor.cpp
 *
 *  Created on: May 26, 2014
 *      Author: thanuja
 */

#include <fstream>
#include <math.h>

#include <boost/filesystem.hpp>

#include <util/helpers.hpp>
#include <imageprocessing/ConnectedComponent.h>
#include <sopnet/exceptions.h>
#include <sopnet/segments/ContinuationSegment.h>
#include "SegmentPairFeatureExtractor.h"

logger::LogChannel segmentpairfeatureextractorlog("segmentpairfeatureextractorlog", "[SegmentPairFeatureExtractor] ");

SegmentPairFeatureExtractor::SegmentPairFeatureExtractor() :
	_features(new Features()){

    registerInput(_segments, "segments");
    registerOutput(_features, "features");

}

SegmentPairFeatureExtractor::~SegmentPairFeatureExtractor() {
	// TODO Auto-generated destructor stub
}

void
SegmentPairFeatureExtractor::updateOutputs(){

    LOG_DEBUG(segmentpairfeatureextractorlog) << "extracting features" << std::endl;

    _features->clear();

    _features->addName("sp 2nd segment relative offset");
    _features->addName("sp 2nd derivative of size");

    foreach (boost::shared_ptr<SegmentPair> segment, _segments->getSegmentPairs())
                    computeFeatures(*segment, _features->get(segment->getId()));

    LOG_ALL(segmentpairfeatureextractorlog) << "found features: " << *_features << std::endl;

    LOG_DEBUG(segmentpairfeatureextractorlog) << "done" << std::endl;
}

void
SegmentPairFeatureExtractor::computeFeatures(const SegmentPair& segmentPair, std::vector<double>& features){

	boost::shared_ptr<ContinuationSegment> segment1 = segmentPair.getContinuationSegment1();
	boost::shared_ptr<ContinuationSegment> segment2 = segmentPair.getContinuationSegment2();

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

	features[0] = getRelativeOffset(slice1,slice2,slice3);
	features[1] = getD2Area(slice1,slice2,slice3);
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
			((x2-x1)*(x3-x2)+(y2-y1)*(y3-y2)) /
			(sqrt(pow((x2-x1),2)+pow((y2-y1),2))*sqrt(pow((x3-x2),2)+pow((y3-y2),2)))
			);

	return offset;
}

double
SegmentPairFeatureExtractor::getD2Area(
		const boost::shared_ptr<Slice> slice1,
		const boost::shared_ptr<Slice> slice2,
		const boost::shared_ptr<Slice> slice3){

	// returns the rate of change of area across the 3 slices of the segment pair

	unsigned int a1, a2, a3;

	a1 = slice1->getComponent()->getSize();
	a2 = slice2->getComponent()->getSize();
	a3 = slice3->getComponent()->getSize();

	return (a2-a1)*(a3-a2)/(a1*a2);

}
