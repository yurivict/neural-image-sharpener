
#include "neural-network.h"
#include "MiniDNN.h"


namespace NeuralNetwork {

using namespace MiniDNN;

typedef Eigen::MatrixXd Matrix;

Net::Net(unsigned numColors_, unsigned lowResOuterSize_, unsigned intermediateSize_, unsigned highResInnerSize_)
: network(new MiniDNN::Network)
{
	// memorize the values
	numColors = numColors_;
	lowResOuterSize = lowResOuterSize_;
	intermediateSize = intermediateSize_;
	highResInnerSize = highResInnerSize_;

	// Create layers: use 2 fully connected layers
	Layer* layer1 = new FullyConnected<Tanh>(lowResOuterSize*lowResOuterSize*numColors, intermediateSize);
	Layer* layer2 = new FullyConnected<Identity>(intermediateSize, ((highResInnerSize+1)*(highResInnerSize+1)-4)*numColors);

	// Add layers to the network object
	network->add_layer(layer1);
	network->add_layer(layer2);

	// Set output layer
	network->set_output(new RegressionMSE());

	// (Optional) set callback function object
	VerboseCallback callback;
	network->set_callback(callback);

	// Initialize parameters with N(0, 0.01^2) using random seed 123
	network->init(0, 0.01, 123);
}

Net::~Net() {
	delete network;
}

void Net::train(const TrainingData::Images &trainingData, unsigned imageMarginPercent, unsigned lowResOuterSize, unsigned highResInnerSize, unsigned batchSize, unsigned numEpochs) {
	// Create optimizer object
	RMSProp opt;
	opt.m_lrate = 0.001;

	// Set random seed and generate some data
	std::srand(123);

	// collect training data stats

	// find how much NN training data samles is available in all images
	size_t numTrainingDataSamples = TrainingData::calcNumTrainingDataSamples(trainingData, imageMarginPercent, lowResOuterSize, highResInnerSize);

	// Predictors -- each column is an observation
	Matrix inputs(lowResOuterSize*lowResOuterSize*numColors, numTrainingDataSamples);
	// Response variables -- each column is an observation
	Matrix response((highResInnerSize+1)*(highResInnerSize+1)*numColors, numTrainingDataSamples);

	// Fit the model with a batch size of 100, running numEpochs epochs with random seed 123
	network->fit(opt, inputs, response, batchSize, numEpochs, 123/*random seed*/);
}

void Net::writeFile(const std::string &folderName, const std::string &fileName) {
	network->export_net(folderName, fileName);
}

}
