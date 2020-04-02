#pragma once

#include "training-data.h"

#include <string>

// forward
namespace MiniDNN {
	class Network; // use it in as an opaque class outside of the NeuralNetwork namespace
}

namespace NeuralNetwork {

class Net {
	MiniDNN::Network *network;
	unsigned numColors;
	unsigned lowResOuterSize;
	unsigned intermediateSize;
	unsigned highResInnerSize;
public:
	Net(unsigned numColors_, unsigned lowResOuterSize_, unsigned intermediateSize_, unsigned highResInnerSize_);
	~Net();

	void train(const TrainingData::Images &trainingData, unsigned imageMarginPercent, unsigned lowResOuterSize, unsigned highResInnerSize, unsigned batchSize, unsigned numEpochs);
	void writeFile(const std::string &folderName, const std::string &fileName);
};

}
