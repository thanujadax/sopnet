#ifndef SOPNET_INFERENCE_PROBLEM_GRAPH_WRITER_H__
#define SOPNET_INFERENCE_PROBLEM_GRAPH_WRITER_H__

#include <pipeline/all.h>

#include <inference/LinearConstraints.h>
#include <inference/LinearConstraint.h>
#include <inference/LinearObjective.h>
#include <sopnet/segments/Segments.h>
#include <sopnet/features/Features.h>
#include <sopnet/inference/ProblemConfiguration.h>

/**
 * A sink process node that dumps a problem (i.e., sets of Segments and
 * LinearConstraints) as a lemon graph structure.
 */
class ProblemGraphWriter : public pipeline::SimpleProcessNode<> {

	typedef boost::function<
			void
			(const std::vector<boost::shared_ptr<EndSegment> >&          ends,
			 const std::vector<boost::shared_ptr<ContinuationSegment> >& continuations,
			 const std::vector<boost::shared_ptr<BranchSegment> >&       branches,
			 const std::vector<boost::shared_ptr<SegmentPair> >&       segmentPairs,
			 std::vector<double>& costs)>
			costs_function_type;

public:

	ProblemGraphWriter();

	void write(
			const std::string& slicesFile,
			const std::string& segmentsFile,
			const std::string& constraintsFile,
			const std::string& sliceImageDirectory,
			int originSlice,
			int targetSlice);

private:

	// we produce nothing
	void updateOutputs() {};

	void writeSlices(
			const std::string& slicesFile,
			const std::string& sliceImageDirectory,
			int originSlice,
			int targetSlice);

	void writeSegments(const std::string& segmentsFile, int originSlice, int targetSlice);

	void writeConstraints(const std::string& constraintsFile);

	void writeSlice(const Slice& slice, std::ofstream& out);

	void writeSliceImage(const Slice& slice, const std::string& sliceImageDirectory, int originSection, int targetSection);

	void writeSegment(const Segment& segment, std::ofstream& out, int originSection, int targetSection);

	void writeSegmentPairDetails(
			const std::string& segmentPairPropertiesFile,
			const std::string& segmentPairConstraintsFile);

	void writeSegmentPairProperties(const std::string& segmentPairPropertiesFile);

	void writeSegmentPairConstraints(const std::string& segmentPairConstraintsFile);

	void writeSegmentPairCosts(boost::shared_ptr<SegmentPair> segmentPair,std::ofstream& out);

	void writeSegmentPairFeatures(boost::shared_ptr<SegmentPair> segmentPair,std::ofstream& out);


	// all extracted segments
	pipeline::Input<Segments> _segments;

	// map from segment ids to variable numbers in the linear constraints
	pipeline::Input<std::map<unsigned int, unsigned int> > _segmentIdsToVariables;

	// the segment features
	pipeline::Input<Features> _features;

	// all linear constraints on the slices (one set per section)
	pipeline::Inputs<LinearConstraints> _linearConstraints;

	pipeline::Input<ProblemConfiguration> _problemConfiguration;

	pipeline::Input<costs_function_type> _randomForestCostFunction;
	pipeline::Input<costs_function_type> _segmentationCostFunction;
	pipeline::Input<costs_function_type> _linearCostFunction;

	std::map<unsigned int, double> _randomForestCostMap;
	std::map<unsigned int, double> _segmentationCostMap;
	std::map<unsigned int, double> _linearCostMap;

	
};

#endif // SOPNET_INFERENCE_PROBLEM_GRAPH_WRITER_H__

