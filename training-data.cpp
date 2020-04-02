
#include "misc.h"
#include "training-data.h"

#include <assert.h>
#include <png++/png.hpp>

#include <memory>
#include <string>
#include <vector>

namespace TrainingData {

/// local functions

static std::vector<std::string> getFilesWithExtension(const std::string &dir, const char *ext) {
	std::vector<std::string> lst;
	for (const auto &entry : std::__fs::filesystem::directory_iterator(dir)) {
		std::string path = entry.path();
		if (path.size() >= 1+std::strlen(ext) && path.substr(path.size() - std::strlen(ext)) == ext)
			lst.push_back(path);
	}
	return lst;
}


/// interface

const Images* load(const std::string &dir) {
	auto files = getFilesWithExtension(dir, ".png"); // XXX we ony support png files for now
	if (files.empty())
		FAIL("no PNG files were found in the directory " << dir)
	std::unique_ptr<Images> images(new Images);
	for (auto &file : files) {
		PRINT("TrainingData::load: " << file)
		png::image<png::rgb_pixel> image(file);
		auto &imgData = (*images)[file];
		// round to being a multiple of HiResSquareSize
		std::get<0>(imgData) = image.get_width();
		std::get<1>(imgData) = image.get_height();
		std::get<2>(imgData).reset(new float[image.get_width()*image.get_height()*3]);
		float *p = std::get<2>(imgData).get();
		for (size_t y = 0; y < image.get_height(); y++)
			for (size_t x = 0; x < image.get_width(); x++) {
				auto pixel = image.get_pixel(x, y);
				*p++ = pixel.red;
				*p++ = pixel.green;
				*p++ = pixel.blue;
			}
				
	}
	return images.release();
}

size_t calcNumTrainingDataSamples(const Images &trainingData, unsigned imageMarginPercent, unsigned lowResOuterSize, unsigned highResInnerSize) {
	size_t numTrainingDataSamples = 0;
	auto one = [&](unsigned width, unsigned height) {
		unsigned w = width  - 2*(width*imageMarginPercent)  - lowResOuterSize*highResInnerSize;
		unsigned h = height - 2*(height*imageMarginPercent) - lowResOuterSize*highResInnerSize;
		assert(w < width && h < height);
		return w*h;
	};
	for (auto &img : trainingData)
		numTrainingDataSamples += one(std::get<0>(img.second), std::get<1>(img.second));
	return numTrainingDataSamples;
}

}
