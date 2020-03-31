#pragma once

#include <map>
#include <memory>
#include <tuple>

namespace TrainingData {

typedef std::tuple<unsigned/*width*/, unsigned/*height*/, std::unique_ptr<float>/*image data*/> Image;
typedef std::map<std::string/*path*/, Image> Images;

Images* load(const std::string &dir);

};
