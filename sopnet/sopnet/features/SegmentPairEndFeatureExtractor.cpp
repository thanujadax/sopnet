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

    unsigned int numSegmentPairFeatures = 5;

    _features->clear();

    _features->resize(_segments->size(), numSegmentPairEndFeatures);

    _features->addName("spe is segment pair end");
    _features->addName("spe area of end segment");
    _features->addName("spe change of area");
    _features->addName("spe change of area / area_cts");
    _features->addName("spe change of area / area_end");



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

}

void
SegmentPairFeatureExtractor::computeFeatures(const boost::shared_ptr<ContinuationSegment> continuation, std::vector<double>& features){

	features[0] = 0;
	features[1] = Features::NoFeatureValue;
	features[2] = Features::NoFeatureValue;
	features[3] = Features::NoFeatureValue;
	features[4] = Features::NoFeatureValue;

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

	features[0] = 0;
	features[1] = Features::NoFeatureValue;
	features[2] = Features::NoFeatureValue;
	features[3] = Features::NoFeatureValue;
	features[4] = Features::NoFeatureValue;

}

void
SegmentPairFeatureExtractor::computeFeatures(const boost::shared_ptr<SegmentPairEnd> segmentPairEnd, std::vector<double>& features){

	double areaChangeProduct, d2Area;
	boost::shared_ptr<ContinuationSegment> continuationSegment = segmentPairEnd->getContinuationSegment();
	boost::shared_ptr<EndSegment> endSegment = segmentPairEnd->getEndSegment();

	boost::shared_ptr<Slice> slice1, slice2, slice3;

	if(continuationSegment->getDirection()==Right) {
		slice1 = continuationSegment->getSourceSlice();
		slice2 = continuationSegment->getTargetSlice();
	}
	else {
		slice2 = continuationSegment->getSourceSlice();
		slice1 = continuationSegment->getTargetSlice();
	}


	slice3 = endSegment->getSlice();
	double area_slice1 = (double)slice1->getComponent()->getSize();
	double area_slice3 = (double)slice3->getComponent()->getSize();

	features[0] = 1;
	features[1] = area_slice3;
	features[2] = area_slice1 - area_slice3;
	features[3] = (area_slice1 - area_slice3)/area_slice1;
	features[4] = (area_slice1 - area_slice3)/area_slice3;

}

