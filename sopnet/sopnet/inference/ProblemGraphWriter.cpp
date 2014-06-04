#include <fstream>

#include <boost/lexical_cast.hpp>

#include <vigra/impex.hxx>
#include <vigra/impexalpha.hxx>

#include "ProblemGraphWriter.h"

static logger::LogChannel problemgraphwriterlog("problemgraphwriterlog", "[ProblemGraphWriter] ");

util::ProgramOption optionProblemGraphFile(
		util::_module           = "sopnet",
		util::_long_name        = "problemGraphFile",
		util::_description_text = "Path to the problem graph file to produce.",
		util::_default_value    = "problem.graph");

ProblemGraphWriter::ProblemGraphWriter() {

	registerInput(_segments, "segments");
	registerInput(_problemConfiguration, "problem configuration");
	registerInputs(_linearConstraints, "linear constraints");
	registerInput(_features, "features");
	registerInput(_randomForestCostFunction, "segment cost function");
	registerInput(_segmentationCostFunction, "segmentation cost function");
	registerInput(_linearCostFunction, "linear cost function");
}

void
ProblemGraphWriter::write(
		const std::string& slicesFile,
		const std::string& segmentsFile,
		const std::string& constraintsFile,
		const std::string& sliceImageDirectory,
		int originSlice,
		int targetSlice) {

	updateInputs();

	if (!_segments.isSet() || !_problemConfiguration.isSet() ) {

		LOG_DEBUG(problemgraphwriterlog) << "not all required inputs are present -- skip dumping" << std::endl;
		return;
	}

	LOG_DEBUG(problemgraphwriterlog) << "dumping problem graph..." << originSlice << std::endl;

	writeSlices(slicesFile, sliceImageDirectory, originSlice, targetSlice);

	writeSegments(segmentsFile, originSlice, targetSlice);

	writeConstraints(constraintsFile);

	LOG_DEBUG(problemgraphwriterlog) << "done" << std::endl;
}

void
ProblemGraphWriter::writeSlices(
		const std::string& slicesFile,
		const std::string& sliceImageDirectory,
		int originSlice,
		int targetSlice) {

	LOG_DEBUG(problemgraphwriterlog) << "writing slices to " << slicesFile << std::endl;

	std::ofstream out(slicesFile.c_str());

	out << "id sectionid bb.minX bb.maxX bb.minY bb.maxY value center.x center.y size" << std::endl;

	foreach (boost::shared_ptr<EndSegment> end, _segments->getEnds()) {

		if (end->getDirection() == Left)
			writeSlice(*end->getSlice(), out);
	}

	LOG_DEBUG(problemgraphwriterlog) << "writing slice images to " << sliceImageDirectory << std::endl;

	foreach (boost::shared_ptr<EndSegment> end, _segments->getEnds()) {

		if (end->getDirection() == Left)
			writeSliceImage(*end->getSlice(), sliceImageDirectory, originSlice, targetSlice);
	}
}

void
ProblemGraphWriter::writeSegments(const std::string& segmentsFile, int originSlice, int targetSlice) {

	LOG_DEBUG(problemgraphwriterlog) << "writing segments to " << segmentsFile << std::endl;

	std::ofstream out(segmentsFile.c_str());

	out << "segmentid,type,origin_section,origin_slice_id,target1_section,target1_slice_id,target2_section,target2_slice_id,cost,direction,";

	const std::vector<std::string>& names    = _features->getNames();

	for (unsigned int i = 0; i < names.size(); i++) {
		if (i < names.size()) {
			LOG_DEBUG(problemgraphwriterlog) << "write feature name " << names[i] << std::endl;
			out << names[i] << ",";
		}
	}

	out << std::endl;

	std::vector<double> randomForestCosts(_segments->size(), 0);
	std::vector<double> segmentationCosts(_segments->size(), 0);

	(*_randomForestCostFunction)( _segments->getEnds(), _segments->getContinuations(), _segments->getBranches(), _segments->getSegmentPairs(), randomForestCosts );
	(*_segmentationCostFunction)( _segments->getEnds(), _segments->getContinuations(), _segments->getBranches(), _segments->getSegmentPairs(), segmentationCosts );

	unsigned int counter = 0;
	foreach (boost::shared_ptr<Segment> segment, _segments->getSegments()) {
		_randomForestCostMap[ segment->getId() ] = randomForestCosts[counter];
		_segmentationCostMap[ segment->getId() ] = segmentationCosts[counter];
		counter++;
	
}
	foreach (boost::shared_ptr<EndSegment> end, _segments->getEnds())
		writeSegment(*end, out, originSlice, targetSlice);

	foreach (boost::shared_ptr<ContinuationSegment> continuation, _segments->getContinuations())
		writeSegment(*continuation, out, originSlice, targetSlice);

	foreach (boost::shared_ptr<BranchSegment> branch, _segments->getBranches())
		writeSegment(*branch, out, originSlice, targetSlice);

	out.close();
}

void
ProblemGraphWriter::writeConstraints(const std::string& constraintsFile) {

	LOG_DEBUG(problemgraphwriterlog) << "writing constraints to " << constraintsFile << std::endl;

	std::ofstream out(constraintsFile.c_str());

	// because we assume to use only two sections, no constraints that ensure that
	// the left segments minus right segments equals zero, exists. the set of constraints
	// is limited to a variable number of coefficients as a constraint on a set of
	// segments, where only one segment can be picked (corresponding to the paths on
	// the component tree). later, these lists have to be defined pair-wise. redundant
	// constraints are not inserted into the database by means of checking by a paired
	// foreign key consisting of the segment ids

	foreach (boost::shared_ptr<LinearConstraints> linearConstraints, _linearConstraints) {

		foreach (const LinearConstraint& linearConstraint, *linearConstraints) {


		//foreach (LinearConstraint& linearConstraint, *_linearConstraints) {
	
		// out << "relation,value,coefficients (variable length)";

		if (linearConstraint.getRelation() == LessEqual ) {
			out << "0" << ",";
		} else if (linearConstraint.getRelation() == GreaterEqual ) {
			out << "1" << ",";
		} else { // it is equal
			out << "2" << ",";
		};

		out << linearConstraint.getValue();

		unsigned int variable;
		double coef;
		foreach (boost::tie(variable, coef), linearConstraint.getCoefficients()) {
			unsigned int segmentId = _problemConfiguration->getSegmentId(variable);
			out << "," << segmentId << "," << coef;
		}

		out << std::endl;

	}
	}

	out.close();

}

void
ProblemGraphWriter::writeSlice(const Slice& slice, std::ofstream& out) {

	out << slice.getId() << " ";
	out << slice.getSection() << " ";
	out << slice.getComponent()->getBoundingBox().minX << " ";
	out << slice.getComponent()->getBoundingBox().maxX << " ";
	out << slice.getComponent()->getBoundingBox().minY << " ";
	out << slice.getComponent()->getBoundingBox().maxY << " ";
	out << slice.getComponent()->getValue() << " ";
	out << slice.getComponent()->getCenter().x << " ";
	out << slice.getComponent()->getCenter().y << " ";
	out << slice.getComponent()->getSize() << " ";
	out << std::endl;
}

void
ProblemGraphWriter::writeSliceImage(const Slice& slice, const std::string& sliceImageDirectory, int originSection, int targetSection) {

	unsigned int section = slice.getSection();
	unsigned int id      = slice.getId();
	int section_to_process = 0;
	if( section == 0 ) {
		section_to_process = originSection;
	} else {
		section_to_process = targetSection;
	};

	std::string filename = sliceImageDirectory + "/" + boost::lexical_cast<std::string>(section_to_process) + "_" + boost::lexical_cast<std::string>(id) + ".png";

 // invert https://github.com/ukoethe/vigra/blob/master/src/examples/invert.cxx

	vigra::exportImageAlpha(
	 vigra::srcImageRange(slice.getComponent()->getBitmap()),
	 vigra::srcImage(slice.getComponent()->getBitmap()),
	 vigra::ImageExportInfo(filename.c_str()));
}

void
ProblemGraphWriter::writeSegment(const Segment& segment, std::ofstream& out, int originSection, int targetSection) {

	LOG_ALL(problemgraphwriterlog) << "writing segment " << segment.getId() << std::endl;

	out << segment.getId() << " ";

	int slice_size = segment.getSlices().size();

	out << slice_size;


	// FIXME: get section and check which to set.
	foreach (boost::shared_ptr<Slice> slice, segment.getSlices()) {
		if( slice->getSection() == 0 ) {
			out << " " << originSection;
		} else {
			out << " " << targetSection;
		};
        //out << " " << slice->getSection();
		out << " " << slice->getId();
	}

	switch( slice_size )
  	{
		case 1:
			out << " " << "-1" << " " << "-1";
			out << " " << "-1" << " " << "-1";
			break;
		case 2:
			out << " " << "-1" << " " << "-1";
			break;
		case 3:
			break;
	}

	out << " " << _randomForestCostMap[ segment.getId() ];
    out << " " << segment.getDirection() << " ";

	const std::vector<double>&      features = _features->get(segment.getId());
	// const std::vector<std::string>& names    = _features->getNames();

	for (unsigned int i = 0; i < features.size(); i++) {
		out << features[i] << " ";
	}

	out << " " << _randomForestCostMap[ segment.getId() ];

	out << " " << _segmentationCostMap[ segment.getId() ];

	out << std::endl;

}

void
ProblemGraphWriter::writeSegmentPairDetails(
		const std::string& segmentPairPropertiesFile,
		const std::string& segmentPairConstraintsFile){

	updateInputs();

	if (!_segments.isSet() || !_problemConfiguration.isSet() ) {

		LOG_DEBUG(problemgraphwriterlog) << "not all required inputs are present -- skip dumping" << std::endl;
		return;
	}


	// LOG_DEBUG(problemgraphwriterlog) << "writing down details of segment pairs ..."  << std::endl;

	// writeSegmentPairProperties(segmentPairPropertiesFile);

	LOG_DEBUG(problemgraphwriterlog) << "writing down constraints for segment pairs ..."  << std::endl;

	writeSegmentPairConstraints(segmentPairConstraintsFile);


}

void
ProblemGraphWriter::writeSegmentPairProperties(const std::string& segmentPairPropertiesFile){
	/* for each segment pair, write down
	 * segment pair id, variable id,
	 * segment1 id, variable id, segment2 id, variable id
	 * for seg1 and 2, {center, area} of {source slice, target slice}
	 */
	boost::shared_ptr<ContinuationSegment> segment1, segment2;
	boost::shared_ptr<Slice> slice1, slice2, slice3;

	unsigned int segmentPairId, segment1Id, segment2Id, segmentPairVarID, segment1VarID, segment2VarID;

	std::ofstream segmentPairPropertiesOutput(segmentPairPropertiesFile.c_str());
	segmentPairPropertiesOutput.open(segmentPairPropertiesFile.c_str());


	foreach(boost::shared_ptr<SegmentPair> segmentPair, _segments->getSegmentPairs() ){

		segment1 = segmentPair->getContinuationSegment1();
		segment2 = segmentPair->getContinuationSegment2();

		if(segment1->getDirection()==Left){

			slice1 = segment1->getTargetSlice();
			slice2 = segment1->getSourceSlice();

		} else {

			slice2 = segment1->getTargetSlice();
			slice1 = segment1->getSourceSlice();

		}

		if(segment2->getDirection()==Left)
			slice3 = segment2->getSourceSlice();
		else
			slice3 = segment2->getTargetSlice();


		writeSegmentPairCosts(segmentPair,segmentPairPropertiesOutput);

		writeSegmentPairFeatures(segmentPair,segmentPairPropertiesOutput);

		segmentPairPropertiesOutput.close();
	}

}

void
ProblemGraphWriter::writeSegmentPairCosts(boost::shared_ptr<SegmentPair> segmentPair,std::ofstream& out){
	/*
	 *
	 */

}

void
ProblemGraphWriter::writeSegmentPairFeatures(boost::shared_ptr<SegmentPair> segmentPair,std::ofstream& out){
	/*
	 *
	 */

}

void
ProblemGraphWriter::writeSegmentPairConstraints(const std::string& segmentPairConstraintsFile){
	/* From the total number of constraints, only pick the constraints relevant for segment pairs.
	 * Currently we have 2 constraints per each segment pair and they are the last block of linear constraints
	 */

	std::ofstream constraintOutput;
	constraintOutput.open(segmentPairConstraintsFile.c_str());

	unsigned int numSegPairs = _segments->getSegmentPairs().size();
	unsigned int numSegPairConstraints = numSegPairs * 2;

	unsigned int segPairConstraintStart, segPairConstraintStop;

	segPairConstraintStop = _linearConstraints.size(); // less than
	segPairConstraintStart = segPairConstraintStop - numSegPairConstraints;

	for(unsigned int i=segPairConstraintStart; i<segPairConstraintStop; i++){

		constraintOutput << _linearConstraints[i] << std::endl;

	}

	constraintOutput.close();

}
