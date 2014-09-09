/*
 * SegmentPairEndFeatureExtractor.h
 *
 *  Created on: Sep 8, 2014
 *      Author: thanuja
 */

#ifndef SEGMENTPAIRENDFEATUREEXTRACTOR_H_
#define SEGMENTPAIRENDFEATUREEXTRACTOR_H_

#include <pipeline/all.h>
#include <sopnet/segments/Segments.h>
#include <sopnet/segments/SegmentPair.h>
#include "Features.h"
/*
 *
 */
class SegmentPairEndFeatureExtractor : public pipeline::SimpleProcessNode<>  {
public:
	SegmentPairEndFeatureExtractor();
	virtual ~SegmentPairEndFeatureExtractor();

private:

    void updateOutputs();

	void computeFeatures(const boost::shared_ptr<EndSegment> end, std::vector<double>& features);

	void computeFeatures(const boost::shared_ptr<ContinuationSegment> continuation, std::vector<double>& features);

	void computeFeatures(const boost::shared_ptr<BranchSegment> branch, std::vector<double>& features);

	void computeFeatures(const boost::shared_ptr<SegmentPair> segmentPair, std::vector<double>& features);

	void computeFeatures(const boost::shared_ptr<SegmentPairEnd> segmentPairEnd, std::vector<double>& features);

	double orientationOfSegmentPairEnd(	const boost::shared_ptr<Slice> slice1,
										   const boost::shared_ptr<Slice> slice2);

    pipeline::Input<Segments> _segments;

    pipeline::Output<Features> _features;

    double _dz; // relative resolution in z direction. value (10.0) initialized in constructor

};

#endif /* SEGMENTPAIRENDFEATUREEXTRACTOR_H_ */
