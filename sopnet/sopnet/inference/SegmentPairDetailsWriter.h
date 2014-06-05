/*
 * SegmentPairDetailsWriter.h
 *
 *  Created on: Jun 4, 2014
 *      Author: thanuja
 */

#ifndef SEGMENTPAIRDETAILSWRITER_H_
#define SEGMENTPAIRDETAILSWRITER_H_

#include <pipeline/all.h>

#include <inference/LinearConstraints.h>
#include <inference/LinearConstraint.h>
#include <inference/LinearObjective.h>
#include <sopnet/segments/Segments.h>
#include <sopnet/features/Features.h>
#include <sopnet/inference/ProblemConfiguration.h>

/* Writes the details of segment pairs (features, var ids, segments, constraints)
 *
 */
class SegmentPairDetailsWriter : public pipeline::SimpleProcessNode<>  {

	typedef boost::function<
			void
			(const std::vector<boost::shared_ptr<EndSegment> >&          ends,
			 const std::vector<boost::shared_ptr<ContinuationSegment> >& continuations,
			 const std::vector<boost::shared_ptr<BranchSegment> >&       branches,
			 const std::vector<boost::shared_ptr<SegmentPair> >&         segmentPairs,
			 std::vector<double>& costs)>
			costs_function_type;

public :

	SegmentPairDetailsWriter();

	void writeSegmentPairDetails(
				const std::string segmentPairPropertiesFile,
				const std::string segmentPairConstraintsFile);

private :

	void updateOutputs() {}

	void writeSegmentPairProperties(const std::string segmentPairPropertiesFile);

	void writeSegmentPairConstraints(const std::string segmentPairConstraintsFile);

	void writeSegmentPairCosts(boost::shared_ptr<SegmentPair> segmentPair,std::ofstream& out);

	void writeSegmentPairFeatures(boost::shared_ptr<SegmentPair> segmentPair,std::ofstream& out);

	// all extracted segments
	pipeline::Input<Segments> _segments;

	// map from segment ids to variable numbers in the linear constraints
	pipeline::Input<std::map<unsigned int, unsigned int> > _segmentIdsToVariables;

	// the segment features
	pipeline::Input<Features> _features;

	// all linear constraints on the slices (one set per section)
	pipeline::Input<LinearConstraints> _linearConstraints;

	pipeline::Input<ProblemConfiguration> _problemConfiguration;

	std::map<unsigned int, double> _linearCostMap;




};

#endif /* SEGMENTPAIRDETAILSWRITER_H_ */
