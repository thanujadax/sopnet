/*
 * SegmentPairDCG.cpp
 *
 *  Created on: Jul 24, 2014
 *      Author: thanuja
 */

#include "SegmentPairDCG.h"

static logger::LogChannel segmentpairdcglog("segmentpairdcglog", "[SegmentPairDCG] ");

SegmentPairDCG::SegmentPairDCG() :
	_segmentPairsOut(boost::make_shared<Segments>()),
	_segmentPairFeaturesOut(boost::make_shared<Features>()){

	registerInput(_segmentPairsIn, "segment pairs");

	registerOutput(_segmentPairsOut, "segment pairs DCG");
	registerOutput(_segmentPairFeaturesOut, "segment pair features");

}

void
SegmentPairDCG::updateOutputs(){
/*	_segmentPairsOut->clear();
	_segmentPairsOut->addSegments(_segmentPairsIn->getSegmentPairs());*/
}
