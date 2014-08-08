#include "Segments.h"

std::vector<boost::shared_ptr<EndSegment> >          Segments::EmptyEnds;
std::vector<boost::shared_ptr<ContinuationSegment> > Segments::EmptyContinuations;
std::vector<boost::shared_ptr<BranchSegment> >       Segments::EmptyBranches;
std::vector<boost::shared_ptr<SegmentPair> >         Segments::EmptySegmentPairs;

Segments::~Segments() {

	clear();
}

void
Segments::clear() {

	// delete all trees

	foreach (EndSegmentKdTree* endTree, _endTrees)
		if (endTree)
			delete endTree;
	_endTrees.clear();

	foreach (ContinuationSegmentKdTree* continuationTree, _continuationTrees)
		if (continuationTree)
			delete continuationTree;
	_continuationTrees.clear();

	foreach (BranchSegmentKdTree* branchTree, _branchTrees)
		if (branchTree)
			delete branchTree;
	_branchTrees.clear();

	foreach (SegmentPairKdTree* segmentPairTree, _segmentPairTrees)
		if (segmentPairTree)
			delete segmentPairTree;
	_segmentPairTrees.clear();

	// delete all adaptors

	foreach (EndSegmentVectorAdaptor* endVectorAdaptor, _endAdaptors)
		if (endVectorAdaptor)
			delete endVectorAdaptor;
	_endAdaptors.clear();

	foreach (ContinuationSegmentVectorAdaptor* continuationVectorAdaptor, _continuationAdaptors)
		if (continuationVectorAdaptor)
			delete continuationVectorAdaptor;
	_continuationAdaptors.clear();

	foreach (BranchSegmentVectorAdaptor* branchVectorAdaptor, _branchAdaptors)
		if (branchVectorAdaptor)
			delete branchVectorAdaptor;
	_branchAdaptors.clear();

	foreach (SegmentPairVectorAdaptor* segmentPairVectorAdaptor, _segmentPairAdaptors)
		if (segmentPairVectorAdaptor)
			delete segmentPairVectorAdaptor;
	_segmentPairAdaptors.clear();

	// clear all segments

	_ends.clear();
	_continuations.clear();
	_branches.clear();
	_segmentPairs.clear();
}

void
Segments::resize(int numInterSectionInterval) {

		_ends.resize(numInterSectionInterval);
		_endAdaptors.resize(numInterSectionInterval, 0);
		_endTrees.resize(numInterSectionInterval, 0);
		_endTreeDirty.resize(numInterSectionInterval, true);

		_continuations.resize(numInterSectionInterval);
		_continuationAdaptors.resize(numInterSectionInterval, 0);
		_continuationTrees.resize(numInterSectionInterval, 0);
		_continuationTreeDirty.resize(numInterSectionInterval, true);

		_branches.resize(numInterSectionInterval);
		_branchAdaptors.resize(numInterSectionInterval, 0);
		_branchTrees.resize(numInterSectionInterval, 0);
		_branchTreeDirty.resize(numInterSectionInterval, true);

		_segmentPairs.resize(numInterSectionInterval);
		_segmentPairAdaptors.resize(numInterSectionInterval, 0);
		_segmentPairTrees.resize(numInterSectionInterval, 0);
		_segmentPairTreeDirty.resize(numInterSectionInterval, true);

}

void
Segments::add(boost::shared_ptr<EndSegment> end) {

	unsigned int interSectionInterval = end->getInterSectionInterval();

	// resize all end inter-section interval vectors
	if (_ends.size() < interSectionInterval + 1) {

		resize(interSectionInterval + 1);
	}

	_endTreeDirty[interSectionInterval] = true;

	_ends[interSectionInterval].push_back(end);
}

void
Segments::add(boost::shared_ptr<ContinuationSegment> continuation) {

	unsigned int interSectionInterval = continuation->getInterSectionInterval();

	// resize the vector of trees to hold as many trees as we have inter-section
	// intervals
	if (_continuationTrees.size() < interSectionInterval + 1) {

		resize(interSectionInterval + 1);
	}

	_continuationTreeDirty[interSectionInterval] = true;

	_continuations[interSectionInterval].push_back(continuation);
}

void
Segments::add(boost::shared_ptr<BranchSegment> branch) {

	unsigned int interSectionInterval = branch->getInterSectionInterval();

	// resize the vector of trees to hold as many trees as we have inter-section
	// intervals
	if (_branchTrees.size() < interSectionInterval + 1) {

		resize(interSectionInterval + 1);
	}

	_branchTreeDirty[interSectionInterval] = true;

	_branches[interSectionInterval].push_back(branch);
}

void
Segments::add(boost::shared_ptr<SegmentPair> segmentPair) {

	unsigned int interSectionInterval = segmentPair->getInterSectionInterval();

	// resize the vector of trees to hold as many trees as we have inter-section
	// intervals
	if (_segmentPairTrees.size() < interSectionInterval + 1) {

		resize(interSectionInterval + 1);
	}

	_segmentPairTreeDirty[interSectionInterval] = true;

	_segmentPairs[interSectionInterval].push_back(segmentPair);
}

void
Segments::addAll(boost::shared_ptr<Segments> segments) {

	addAll(segments->getEnds());
	addAll(segments->getContinuations());
	addAll(segments->getBranches());
	addAll(segments->getSegmentPairs());
}

std::vector<boost::shared_ptr<EndSegment> >&
Segments::getEnds(unsigned int interval) {

	if (interval >= _ends.size())
		return EmptyEnds;

	return _ends[interval];
}

std::vector<boost::shared_ptr<ContinuationSegment> >&
Segments::getContinuations(unsigned int interval) {

	if (interval >= _continuations.size())
		return EmptyContinuations;

	return _continuations[interval];
}

std::vector<boost::shared_ptr<BranchSegment> >&
Segments::getBranches(unsigned int interval) {

	if (interval >= _branches.size())
		return EmptyBranches;

	return _branches[interval];

}

std::vector<boost::shared_ptr<SegmentPair> >&
Segments::getSegmentPairs(unsigned int interval) {

	if (interval >= _segmentPairs.size())
		return EmptySegmentPairs;

	return _segmentPairs[interval];
}

std::vector<boost::shared_ptr<EndSegment> >
Segments::getEnds() const {

	return get(_ends);
}

std::vector<boost::shared_ptr<ContinuationSegment> >
Segments::getContinuations() const {

	return get(_continuations);
}

std::vector<boost::shared_ptr<BranchSegment> >
Segments::getBranches() const {

	return get(_branches);
}

std::vector<boost::shared_ptr<SegmentPair> >
Segments::getSegmentPairs() const {

	return get(_segmentPairs);
}

std::vector<boost::shared_ptr<Segment> >
Segments::getSegments() const {

	std::vector<boost::shared_ptr<Segment> > allSegments;

	std::vector<boost::shared_ptr<EndSegment> >          ends          = get(_ends);
	std::vector<boost::shared_ptr<ContinuationSegment> > continuations = get(_continuations);
	std::vector<boost::shared_ptr<BranchSegment> >       branches      = get(_branches);


	std::copy(ends.begin(), ends.end(), std::back_inserter(allSegments));
	std::copy(continuations.begin(), continuations.end(), std::back_inserter(allSegments));
	std::copy(branches.begin(), branches.end(), std::back_inserter(allSegments));

	return allSegments;
}

std::vector<boost::shared_ptr<Segment> >
Segments::getSegments(unsigned int interval) {

	std::vector<boost::shared_ptr<Segment> > allSegments;

	std::vector<boost::shared_ptr<EndSegment> >          ends          = getEnds(interval);
	std::vector<boost::shared_ptr<ContinuationSegment> > continuations = getContinuations(interval);
	std::vector<boost::shared_ptr<BranchSegment> >       branches      = getBranches(interval);


	std::copy(ends.begin(), ends.end(), std::back_inserter(allSegments));
	std::copy(continuations.begin(), continuations.end(), std::back_inserter(allSegments));
	std::copy(branches.begin(), branches.end(), std::back_inserter(allSegments));

	return allSegments;
}

std::vector<boost::shared_ptr<Segment> >
Segments::getSegmentsComplex() const {

	std::vector<boost::shared_ptr<Segment> > allSegments;

	std::vector<boost::shared_ptr<EndSegment> >          ends          = get(_ends);
	std::vector<boost::shared_ptr<ContinuationSegment> > continuations = get(_continuations);
	std::vector<boost::shared_ptr<BranchSegment> >       branches      = get(_branches);
	std::vector<boost::shared_ptr<SegmentPair> >         segmentPairs  = get(_segmentPairs);


	std::copy(ends.begin(), ends.end(), std::back_inserter(allSegments));
	std::copy(continuations.begin(), continuations.end(), std::back_inserter(allSegments));
	std::copy(branches.begin(), branches.end(), std::back_inserter(allSegments));
	std::copy(segmentPairs.begin(), segmentPairs.end(), std::back_inserter(allSegments));

	return allSegments;
}

std::vector<boost::shared_ptr<EndSegment> >
Segments::findEnds(
		boost::shared_ptr<EndSegment> reference,
		double distance) {

	return find(reference->getCenter(), reference->getInterSectionInterval(), distance, _ends, _endAdaptors, _endTrees, _endTreeDirty);
}

std::vector<boost::shared_ptr<ContinuationSegment> >
Segments::findContinuations(
		boost::shared_ptr<ContinuationSegment> reference,
		double distance) {

	return find(reference->getCenter(), reference->getInterSectionInterval(), distance, _continuations, _continuationAdaptors, _continuationTrees, _continuationTreeDirty);
}

std::vector<boost::shared_ptr<BranchSegment> >
Segments::findBranches(
		boost::shared_ptr<BranchSegment> reference,
		double distance) {

	return find(reference->getCenter(), reference->getInterSectionInterval(), distance, _branches, _branchAdaptors, _branchTrees, _branchTreeDirty);
}

std::vector<boost::shared_ptr<SegmentPair> >
Segments::findSegmentPairs(
		boost::shared_ptr<SegmentPair> reference,
		double distance) {

	return find(reference->getCenter(), reference->getInterSectionInterval(), distance, _segmentPairs, _segmentPairAdaptors, _segmentPairTrees, _segmentPairTreeDirty);
}

std::vector<boost::shared_ptr<EndSegment> >
Segments::findEnds(
		const util::point<double>& center,
		unsigned int interSectionInterval,
		double distance) {

	return find(center, interSectionInterval, distance, _ends, _endAdaptors, _endTrees, _endTreeDirty);
}

std::vector<boost::shared_ptr<ContinuationSegment> >
Segments::findContinuations(
		const util::point<double>& center,
		unsigned int interSectionInterval,
		double distance) {

	return find(center, interSectionInterval, distance, _continuations, _continuationAdaptors, _continuationTrees, _continuationTreeDirty);
}

std::vector<boost::shared_ptr<BranchSegment> >
Segments::findBranches(
		const util::point<double>& center,
		unsigned int interSectionInterval,
		double distance) {

	return find(center, interSectionInterval, distance, _branches, _branchAdaptors, _branchTrees, _branchTreeDirty);
}

std::vector<boost::shared_ptr<SegmentPair> >
Segments::findSegmentPairs(
		const util::point<double>& center,
		unsigned int interSectionInterval,
		double distance) {

	return find(center, interSectionInterval, distance, _segmentPairs, _segmentPairAdaptors, _segmentPairTrees, _segmentPairTreeDirty);
}


unsigned int
Segments::getNumInterSectionIntervals() {

	return std::max(_ends.size(), std::max(_continuations.size(), _branches.size()));
}

unsigned int
Segments::size() {

	unsigned int size = 0;

	foreach (std::vector<boost::shared_ptr<EndSegment> > ends, _ends)
		size += ends.size();
	foreach (std::vector<boost::shared_ptr<ContinuationSegment> > continuations, _continuations)
		size += continuations.size();
	foreach (std::vector<boost::shared_ptr<BranchSegment> > branches, _branches)
		size += branches.size();

	foreach (std::vector<boost::shared_ptr<SegmentPair> > segmentPairs, _segmentPairs)
			size += segmentPairs.size();

	return size;
}

