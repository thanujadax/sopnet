/*
 * SegmentPairExtractionPipeline.h
 *
 *  Created on: May 15, 2014
 *      Author: thanuja
 */

#ifndef SOPNET_SEGMENTS_SEGMENTPAIREXTRACTIONPIPELINE_H_
#define SOPNET_SEGMENTS_SEGMENTPAIREXTRACTIONPIPELINE_H_

#include <vector>
#include <string>

#include <imageprocessing/ImageExtractor.h>
#include <imageprocessing/ImageStack.h>
#include <sopnet/inference/LinearConstraints.h>
#include <sopnet/segments/Segments.h>
#include <sopnet/segments/SegmentPairExtractor.h>
#include <pipeline/Output.h>
#include <pipeline/Value.h>

/**
 * Contains a pipeline with a limb for every inter-section interval, providing
 * segments and constraints on it. Creates itself from a vector of strings to a
 * directory or an image stack.
 */
class SegmentPairExtractionPipeline {

public:

	SegmentPairExtractionPipeline(
			boost::shared_ptr<ImageStack> neuronSlices,
			boost::shared_ptr<Segments> continuationSegments,
			pipeline::Value<bool> forceExplanation, bool finishLastInterval =
					false);

	/**
	 * Get the extracted segments in the given interval.
	 */
	pipeline::OutputBase& getSegments(unsigned int interval);

	/**
	 * Get linear constraints on the segments in the given interval.
	 */
	pipeline::OutputBase& getConstraints(unsigned int interval);

	/**
	 * Get the number of intervals for which the pipeline was created.
	 */
	unsigned int numIntervals() const {
		return _segmentPairExtractors.size();
	}

private:

	void create();

	// neuron slices
	boost::shared_ptr<ImageStack> _neuronSlices;
	// already extracted continuation segments
	boost::shared_ptr<Segments> _continuationSegments;

	// the segment extractors for each interval
	std::vector<boost::shared_ptr<SegmentExtractor> > _segmentPairExtractors;

	pipeline::Value<bool> _forceExplanation;

	// should the last interval be treaded specially?
	bool _finishLastInterval;

};

#endif /* SOPNET_SEGMENTS_SEGMENTPAIREXTRACTIONPIPELINE_H_ */

