#ifndef CELLTRACKER_GUI_TRACKLETS_PAINTER_H__
#define CELLTRACKER_GUI_TRACKLETS_PAINTER_H__

#include <gui/RecordablePainter.h>
#include <gui/Texture.h>
#include <imageprocessing/ImageStack.h>
#include <sopnet/evaluation/SliceErrors.h>
#include <sopnet/segments/Segments.h>
#include "SliceTextures.h"

class SegmentsPainter : public gui::RecordablePainter {

public:

	SegmentsPainter(std::string name = "");

	/**
	 * Set the image stack that should be used to texture the segments.
	 */
	void setImageStack(boost::shared_ptr<ImageStack> imageStack);

	/**
	 * Set the segments to draw.
	 */
	void setSegments(boost::shared_ptr<Segments> segments);

	/**
	 * Set the errors to highlight.
	 */
	void setSliceErrors(boost::shared_ptr<SliceErrors> sliceErrors);

	void showEnds(bool show);

	void showContinuations(bool show);

	void showBranches(bool show);

	void showSegmentPairs(bool show);

private:

	void loadTextures();

	void loadTextures(const EndSegment& end);

	void loadTextures(const ContinuationSegment& continuation);

	void loadTextures(const BranchSegment& branch);

	void loadTextures(const SegmentPair& segmentPair);

	void loadTexture(const Slice& slice);

	void updateRecording();

	void draw(const EndSegment& end);

	void draw(const ContinuationSegment& continuation);

	void draw(const BranchSegment& branch);

	void draw(const SegmentPair& segmentPair);

	void drawSlice(boost::shared_ptr<Slice> slice, double red, double green, double blue);

	void drawLink(const Slice& slice1, const Slice& slice2, double red, double green, double blue);

	boost::shared_ptr<ImageStack> _imageStack;

	boost::shared_ptr<Segments> _segments;

	boost::shared_ptr<SliceErrors> _sliceErrors;

	util::rect<double> _size;

	// the distance between two sections
	double _zScale;

	// slice textures from slice ids
	SliceTextures _textures;

	// which side of the faces to draw
	bool _leftSide;
	bool _rightSide;

	bool _showEnds;
	bool _showContinuations;
	bool _showBranches;
	bool _showSegmentPairs;
};

#endif // CELLTRACKER_GUI_TRACKLETS_PAINTER_H__

