#ifndef SOPNET_GOLD_STANDARD_EXTRACTOR_H__
#define SOPNET_GOLD_STANDARD_EXTRACTOR_H__

#include <pipeline/SimpleProcessNode.h>
#include <pipeline/Process.h>
#include <sopnet/segments/Segments.h>
#include <sopnet/inference/Reconstructor.h>
#include <sopnet/inference/Solution.h>
#include <inference/LinearConstraints.h>
#include <util/point.hpp>

class GoldStandardExtractor : public pipeline::SimpleProcessNode<> {

public:

	GoldStandardExtractor();

private:

	void updateOutputs();

	pipeline::Input<Segments>          _groundTruth;
	pipeline::Input<Segments>          _allSegments;
	pipeline::Input<LinearConstraints> _allLinearConstraints;

	pipeline::Process<Reconstructor> _reconstructor;
	pipeline::Process<LinearSolver> _linearSolver;
};

#endif // SOPNET_GOLD_STANDARD_EXTRACTOR_H__

