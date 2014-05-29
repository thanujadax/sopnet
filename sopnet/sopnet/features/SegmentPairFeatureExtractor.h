/*
 * SegmentPairFeatureExtractor.h
 *
 *  Created on: May 26, 2014
 *      Author: thanuja
 */

#ifndef SOPNET_SEGMENTPAIRFEATUREEXTRACTOR_H_
#define SOPNET_SEGMENTPAIRFEATUREEXTRACTOR_H_

#include <pipeline/all.h>
#include <sopnet/segments/Segments.h>
#include <sopnet/segments/SegmentPair.h>
#include "Features.h"
/*
 *
 */
class SegmentPairFeatureExtractor : public pipeline::SimpleProcessNode<>  {
public:
	SegmentPairFeatureExtractor();
	virtual ~SegmentPairFeatureExtractor();

private:

    void updateOutputs();

	void computeFeatures(const boost::shared_ptr<EndSegment> end, std::vector<double>& features);

	void computeFeatures(const boost::shared_ptr<ContinuationSegment> continuation, std::vector<double>& features);

	void computeFeatures(const boost::shared_ptr<BranchSegment> branch, std::vector<double>& features);

	void computeFeatures(const boost::shared_ptr<SegmentPair> segmentPair, std::vector<double>& features);

	double getRelativeOffset(
			const boost::shared_ptr<Slice> slice1,
			const boost::shared_ptr<Slice> slice2,
			const boost::shared_ptr<Slice> slice3);

	double getD2Area(
			const boost::shared_ptr<Slice> slice1,
			const boost::shared_ptr<Slice> slice2,
			const boost::shared_ptr<Slice> slice3);

    pipeline::Input<Segments> _segments;

    pipeline::Output<Features> _features;

};

#endif /* SOPNET_SEGMENTPAIRFEATUREEXTRACTOR_H_ */
