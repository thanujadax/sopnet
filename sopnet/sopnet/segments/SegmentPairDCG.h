/*
 * SegmentPairDCG.h
 *
 *  Created on: Jul 24, 2014
 *      Author: thanuja
 */

#ifndef SOPNET_SEGMENTS_SEGMENTPAIRDCG_H_
#define SOPNET_SEGMENTS_SEGMENTPAIRDCG_H_

#define OFFSET_FEATURE_ID 60
#define OFFSET_THRESHOLD 0.3

#include <pipeline/all.h>
#include <util/foreach.h>
#include <imageprocessing/ImageStack.h>
#include <sopnet/features/Features.h>
#include <sopnet/features/SegmentPairFeatureExtractor.h>

#include "Segments.h"
#include "SegmentPair.h"

class SegmentPairDCG : public pipeline::SimpleProcessNode<> {

public:
	SegmentPairDCG();

private :

	class SegmentPairSelector : public pipeline::SimpleProcessNode<> {

		public:

			SegmentPairSelector();

		private:

			void updateOutputs();

			void selectSegmentPairs();

			pipeline::Input<Segments> _segmentPairsIn;

			pipeline::Input<Features> _features;

			pipeline::Output<Segments> _segmentPairsOut;

		};


	void onInputSet(const pipeline::InputSetBase& signal);

	pipeline::Input<Segments> _segmentPairsIn;

    boost::shared_ptr<SegmentPairFeatureExtractor> _segmentPairFeatureExtractor;

    boost::shared_ptr<SegmentPairSelector> _segmentPairSelector;

};

#endif /* SOPNET_SEGMENTS_SEGMENTPAIRDCG_H_ */
