#pragma once

#include <map>
#include <memory>
#include <tuple>

namespace TrainingData {

// types
typedef std::tuple<unsigned/*width*/, unsigned/*height*/, std::unique_ptr<float>/*image data*/> Image;
typedef std::map<std::string/*path*/, Image> Images;

// interface
const Images* load(const std::string &dir);
size_t calcNumTrainingDataSamples(const Images &trainingData, unsigned imageMarginPercent, unsigned lowResOuterSize, unsigned highResInnerSize); // how many samples does this image provide

}
