#include <util/Logger.h>
#include <util/foreach.h>
#include "Reconstructor.h"

static logger::LogChannel reconstructorlog("reconstructorlog", "[Reconstructor] ");

Reconstructor::Reconstructor() :
	_reconstruction(new Segments()),
	_discardedSegments(new Segments()) {

	registerInput(_solution, "solution");
	registerInput(_segments, "segments");
	registerOutput(_reconstruction, "reconstruction");
	registerOutput(_discardedSegments, "discarded segments");
}

void
Reconstructor::updateOutputs() {

	LOG_DEBUG(reconstructorlog) << "reconstructing segments from solution" << std::endl;

	updateReconstruction();
}

void
Reconstructor::updateReconstruction() {

	// remove all previous segment in the reconstruction
	_reconstruction->clear();
	_discardedSegments->clear();

	LOG_ALL(reconstructorlog) << "Solution consists of segments: ";

	_currentSegmentNum = 0;

	foreach (boost::shared_ptr<EndSegment> segment, _segments->getEnds())
		probe(segment);

	foreach (boost::shared_ptr<ContinuationSegment> segment, _segments->getContinuations())
		probe(segment);

	foreach (boost::shared_ptr<BranchSegment> segment, _segments->getBranches())
		probe(segment);

	foreach (boost::shared_ptr<SegmentPair> segment, _segments->getSegmentPairs())
			probe(segment);

	foreach (boost::shared_ptr<SegmentPairEnd> segment, _segments->getSegmentPairEnds())
			probe(segment);

	LOG_ALL(reconstructorlog) << std::endl;

}

template <typename SegmentType>
void
Reconstructor::probe(boost::shared_ptr<SegmentType> segment) {

	if (_currentSegmentNum < _solution->size() && (*_solution)[_currentSegmentNum] == 1.0) {

		_reconstruction->add(segment);

		LOG_ALL(reconstructorlog) << segment->getId() << " ";

	} else  {

		_discardedSegments->add(segment);
	}

	_currentSegmentNum++;
}
