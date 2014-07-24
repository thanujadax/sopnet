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

	registerInput(registerInput(_segmentPairs, "segment pairs"););
	registerOutput(_segmentPairsOut, "segment pairs DCG");

}
