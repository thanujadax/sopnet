#ifndef SOPNET_GUI_SLICE_ERRORS_VIEW_H__
#define SOPNET_GUI_SLICE_ERRORS_VIEW_H__

#include <sstream>

#include <pipeline/all.h>
#include <gui/TextPainter.h>
#include <sopnet/evaluation/SliceErrors.h>
#include <sopnet/evaluation/Errors.h>

class ErrorsView : public pipeline::SimpleProcessNode<> {

public:

	ErrorsView() :
		_painter(new gui::TextPainter()) {

		registerInput(_sliceErrors, "slice errors");
		registerInput(_variationOfInformation, "variation of information", pipeline::Optional);
		registerInput(_tedErrors, "tolerant edit distance errors", pipeline::Optional);
		registerInput(_hammingDistance, "hamming distance",pipeline::Optional);
		registerInput(_hammingDistanceLowLevel, "hamming distance low level",pipeline::Optional);
		registerOutput(_painter, "painter");

		_painter.registerSlot(_sizeChanged);
	}

private:

	void updateOutputs() {

		std::stringstream ss;

		ss
				<< "false positives: " << _sliceErrors->numFalsePositives() << ", "
				<< "false negatives: " << _sliceErrors->numFalseNegatives() << ", "
				<< "splits: " << _sliceErrors->numFalseSplits() << ", "
				<< "merges: " << _sliceErrors->numFalseMerges() << "; "
				<< "total: " <<
					(_sliceErrors->numFalsePositives() +
					 _sliceErrors->numFalseNegatives() +
					 _sliceErrors->numFalseSplits() +
					 _sliceErrors->numFalseMerges()) << std::endl;

		if (_variationOfInformation.isSet())
			ss
				<< " -- "
				<< "variation of information: " << *_variationOfInformation;

		if (_tedErrors.isSet())
			ss
				<< " -- TED "
				<< "false positives " << _tedErrors->getNumFalsePositives() << ", "
				<< "false negatives " << _tedErrors->getNumFalseNegatives() << ", "
				<< "splits: " << _tedErrors->getNumSplits() << ", "
				<< "merges: " << _tedErrors->getNumMerges() << std::endl;

		if(_hammingDistance.isSet()){
			ss << "Hamming Distance: " << *_hammingDistance << std::endl;
			ss << "Hamming Distance (low-level): " << *_hammingDistanceLowLevel << std::endl;
		}

		_painter->setText(ss.str());

		_sizeChanged(_painter->getSize());
	}

	pipeline::Input<SliceErrors>       _sliceErrors;
	pipeline::Input<double>            _variationOfInformation;
	pipeline::Input<Errors>		   _tedErrors;
	pipeline::Input<unsigned int>		_hammingDistance;
	pipeline::Input<unsigned int>		_hammingDistanceLowLevel;
	pipeline::Output<gui::TextPainter> _painter;

	signals::Slot<const gui::SizeChanged> _sizeChanged;
};

#endif // SOPNET_GUI_SLICE_ERRORS_VIEW_H__

