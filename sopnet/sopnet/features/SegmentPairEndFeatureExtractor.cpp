/*
 * SegmentPairEndFeatureExtractor.cpp
 *
 *  Created on: Sep 8, 2014
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
#include <sopnet/segments/EndSegment.h>
#include "SegmentPairEndFeatureExtractor.h"

logger::LogChannel segmentpairendfeatureextractorlog("segmentpairendfeatureextractorlog", "[SegmentPairEndFeatureExtractor] ");

SegmentPairEndFeatureExtractor::SegmentPairEndFeatureExtractor() :
	_features(new Features()),
	_dz(10.0){

    registerInput(_segments, "segments");
    registerOutput(_features, "features");

}

SegmentPairEndFeatureExtractor::~SegmentPairEndFeatureExtractor() {
	// TODO Auto-generated destructor stub
}

void
SegmentPairEndFeatureExtractor::updateOutputs(){

    LOG_DEBUG(segmentpairendfeatureextractorlog) << "extracting features" << std::endl;

    unsigned int numSegmentPairFeatures = 6;

    _features->clear();

    _features->resize(_segments->size(), numSegmentPairEndFeatures);

    _features->addName("spe is segment pair end");
    _features->addName("spe 2nd segment relative offset");
    _features->addName("spe product of change of area of two segments");
    _features->addName("spe abs of product of change of area");
    _features->addName("spe 2nd derivative of area");
    _features->addName("spe abs of 2nd derivative of area");


    foreach (boost::shared_ptr<EndSegment> segment, _segments->getEnds())
                    computeFeatures(segment, _features->get(segment->getId()));

    foreach (boost::shared_ptr<ContinuationSegment> segment, _segments->getContinuations())
                    computeFeatures(segment, _features->get(segment->getId()));

    foreach (boost::shared_ptr<BranchSegment> segment, _segments->getBranches())
                    computeFeatures(segment, _features->get(segment->getId()));

    foreach (boost::shared_ptr<SegmentPair> segment, _segments->getSegmentPairs())
                    computeFeatures(segment, _features->get(segment->getId()));

    foreach (boost::shared_ptr<SegmentPairEnd> segment, _segments->getSegmentPairEnds())
                    computeFeatures(segment, _features->get(segment->getId()));

    LOG_ALL(segmentpairendfeatureextractorlog) << "found features: " << *_features << std::endl;

    LOG_DEBUG(segmentpairendfeatureextractorlog) << "done" << std::endl;
}

void
SegmentPairFeatureExtractor::computeFeatures(const boost::shared_ptr<EndSegment> end, std::vector<double>& features){

	features[0] = 0;
	features[1] = Features::NoFeatureValue;
	features[2] = Features::NoFeatureValue;
	features[3] = Features::NoFeatureValue;
	features[4] = Features::NoFeatureValue;
	features[5] = Features::NoFeatureValue;
}

void
SegmentPairFeatureExtractor::computeFeatures(const boost::shared_ptr<ContinuationSegment> continuation, std::vector<double>& features){

	features[0] = 0;
	features[1] = Features::NoFeatureValue;
	features[2] = Features::NoFeatureValue;
	features[3] = Features::NoFeatureValue;
	features[4] = Features::NoFeatureValue;
	features[5] = Features::NoFeatureValue;
}

void
SegmentPairFeatureExtractor::computeFeatures(const boost::shared_ptr<BranchSegment> branch, std::vector<double>& features){

	features[0] = 0;
	features[1] = Features::NoFeatureValue;
	features[2] = Features::NoFeatureValue;
	features[3] = Features::NoFeatureValue;
	features[4] = Features::NoFeatureValue;
	features[5] = Features::NoFeatureValue;
}

void
SegmentPairFeatureExtractor::computeFeatures(const boost::shared_ptr<SegmentPair> segmentPair, std::vector<double>& features){

	double areaChangeProduct, d2Area;
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

	areaChangeProduct = getAreaChangeProduct(slice1,slice2,slice3);
	d2Area = getD2Area(slice1,slice2,slice3);

	features[0] = 1;
	features[1] = getRelativeOffset(slice1,slice2,slice3);
	features[2] = areaChangeProduct;
	features[3] = getAbsVal(areaChangeProduct);
	features[4] = d2Area;
	features[5] = getAbsVal(d2Area);
}
