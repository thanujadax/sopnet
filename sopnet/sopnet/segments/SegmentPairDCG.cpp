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
	SegmentPairSelector::selectSegmentPairs();


}

void
SegmentPairDCG::onInputSet(const pipeline::InputSetBase&){
	if (_segmentPairsIn.isSet()) {

		_segmentPairFeatureExtractor->setInput("segments", _segmentPairsIn.getAssignedOutput());
		_segmentPairSelector->setInput("features",_segmentPairFeatureExtractor->getOutput());
		_segmentPairSelector->setInput("segment pairs all",_segmentPairsIn.getAssignedOutput());

	}
}


void
SegmentPairDCG::SegmentPairSelector::selectSegmentPairs(){

	_segmentPairsOut->clear();
	LOG_DEBUG(segmentpairdcglog) << "selecting segment pairs ..." << std::endl;
	LOG_DEBUG(segmentpairdcglog) << "number of features per each segment pair: " << _features->size() << std::endl;

	// select the segment pairs to use, based on feature values
	foreach (boost::shared_ptr<SegmentPair> segmentPair, _segmentPairsIn->getSegmentPairs()) {
		LOG_DEBUG(segmentpairdcglog) << "checking segment id: " << segmentPair->getId() << std::endl;
		const std::vector<double>& features = _features->get(segmentPair->getId());
		LOG_DEBUG(segmentpairdcglog) << "feature value: " << features[OFFSET_FEATURE_ID] << std::endl;
		if(features[OFFSET_FEATURE_ID] > OFFSET_THRESHOLD){
			_segmentPairsOut->add(segmentPair);

		}

	}
}
