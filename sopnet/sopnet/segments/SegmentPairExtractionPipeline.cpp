/*
 * SegmentPairExtractionPipeline.cpp
 *
 *  Created on: May 16, 2014
 *      Author: thanuja
 */

#include "SegmentPairExtractionPipeline.h"

static logger::LogChannel segmentpairextractionpipelinelog("segmentpairextractionpipelinelog", "[SegmentPairExtractionPipeline] ");

SegmentPairExtractionPipeline::SegmentPairExtractionPipeline(
		boost::shared_ptr<ImageStack> neuronSlices,
		boost::shared_ptr<Segments> continuationSegments,
		pipeline::Value<bool> forceExplanation,
		bool finishLastInterval) :
	_neuronSlices(neuronSlices),
	_continuationSegments(continuationSegments),
	_forceExplanation(forceExplanation),
	_finishLastInterval(finishLastInterval) {

	create();
}

void
SegmentPairExtractionPipeline::create() {
	_segmentPairExtractors.clear();

	unsigned int numInterSectionIntervals = _continuationSegments->getNumInterSectionIntervals();

	LOG_DEBUG(segmentpairextractionpipelinelog) << "creating pipeline for " << numInterSectionIntervals << " sections" << std::endl;

}

pipeline::OutputBase&
SegmentPairExtractionPipeline::getSegments(unsigned int interval) {

        LOG_ALL(segmentpairextractionpipelinelog) << "getting segments for interval " << interval << std::endl;

        return _segmentPairExtractors[interval]->getOutput("segments");
}

pipeline::OutputBase&
SegmentPairExtractionPipeline::getConstraints(unsigned int interval) {

        LOG_ALL(segmentpairextractionpipelinelog) << "getting constraints for interval " << interval << std::endl;

        return _segmentPairExtractors[interval]->getOutput("linear constraints");
}
