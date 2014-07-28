/*
 * SegmentPairDCG.cpp
 *
 *  Created on: Jul 24, 2014
 *      Author: thanuja
 */

#include "SegmentPairDCG.h"

static logger::LogChannel segmentpairdcglog("segmentpairdcglog", "[SegmentPairDCG] ");

SegmentPairDCG::SegmentPairDCG() :
	_segmentPairFeatureExtractor(boost::make_shared<SegmentPairFeatureExtractor>()),
	_segmentPairSelector(boost::make_shared<SegmentPairSelector>()){

	registerInput(_segmentPairsIn, "segment pairs all");

	registerOutput(_segmentPairSelector->getOutput("segment pairs select"), "segment pairs select");

	_segmentPairsIn.registerCallback(&SegmentPairDCG::onInputSet, this);

	_segmentPairSelector->setInput("features",_segmentPairFeatureExtractor->getOutput());
	_segmentPairSelector->setInput("segment pairs all",_segmentPairsIn);

}


SegmentPairDCG::SegmentPairSelector::SegmentPairSelector() :
		_segmentPairsOut(boost::make_shared<Segments>()) {

	registerInput(_features, "features");
	registerInput(_segmentPairsIn,"segment pairs all");

	registerOutput(_segmentPairsOut, "segment pairs select");

}

void
SegmentPairDCG::SegmentPairSelector::updateOutputs(){
/*	_segmentPairsOut->clear();
	_segmentPairsOut->addSegments(_segmentPairsIn->getSegmentPairs());*/
	selectSegmentPairs();


}

void
SegmentPairDCG::onInputSet(const pipeline::InputSetBase&){
	if (_segmentPairsIn.isSet()) {

		_segmentPairFeatureExtractor->setInput("segments", _segmentPairsIn.getAssignedOutput());
	}
}


void
SegmentPairDCG::SegmentPairSelector::selectSegmentPairs(){

	_segmentPairsOut->clear();

	// select the segment pairs to use, based on feature values
	foreach (boost::shared_ptr<SegmentPair> segmentPair, _segmentPairsIn->getSegmentPairs()) {

		const std::vector<double>& features = _features->get(segmentPair->getId());

		if(features[OFFSET_FEATURE_ID] < OFFSET_THRESHOLD){
			_segmentPairsOut->add(segmentPair);
		}

	}
}
