#include <limits>

#include <util/ProgramOptions.h>
#include <sopnet/inference/ProblemAssembler.h>
#include <sopnet/inference/ObjectiveGenerator.h>
#include <inference/LinearSolver.h>
#include "GoldStandardExtractor.h"
#include "GoldStandardCostFunction.h"

logger::LogChannel goldstandardextractorlog("goldstandardextractorlog", "[GoldStandardExtractor] ");

GoldStandardExtractor::GoldStandardExtractor(){

	registerInput(_groundTruth, "ground truth");
	registerInput(_allSegments, "all segments");
	registerInput(_allLinearConstraints, "all linear constraints");

	registerOutput(_reconstructor->getOutput("reconstruction"), "gold standard");
	registerOutput(_linearSolver->getOutput("solution"), "gold standard solution");
	registerOutput(_reconstructor->getOutput("discarded segments"), "negative samples");
}

void
GoldStandardExtractor::updateOutputs() {

	LOG_DEBUG(goldstandardextractorlog) << "searching for best-fitting segments to ground truth" << std::endl;

	pipeline::Process<GoldStandardCostFunction> goldStandardCostFunction;
	pipeline::Process<ObjectiveGenerator>       objectiveGenerator;

	goldStandardCostFunction->setInput("ground truth", _groundTruth);

	objectiveGenerator->setInput("segments", _allSegments);
	objectiveGenerator->addInput("cost functions", goldStandardCostFunction->getOutput());

	_linearSolver->setInput("objective", objectiveGenerator->getOutput());
	_linearSolver->setInput("linear constraints", _allLinearConstraints);
	_linearSolver->setInput("parameters", boost::make_shared<LinearSolverParameters>(Binary));

	_reconstructor->setInput("solution", _linearSolver->getOutput("solution"));
	_reconstructor->setInput("segments", _allSegments);
}

