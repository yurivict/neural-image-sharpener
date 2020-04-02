
#include "misc.h"
#include "neural-network.h"
#include "training-data.h"

#include <exception>
#include <cstdlib>
#include <string>


static int main_guarded(int argc, char *argv[]) {

	/// parameters
	unsigned paramSampleCountOuter      = 7;     // how many values to sample in the lo-res image around the sample square
	unsigned paramSampleCountInner      = 5;     // how many pixels to add in a low-res image between pixels to build a hi-res image
	unsigned paramNnIntermediateSize    = 100;   // the intermediate NN size between FullyConnected
	unsigned paramTrainingBatchSize     = 1000;  // size of batch for training
	unsigned paramEpochsNumber          = 1000;  // number of epochs to run training for
	unsigned paramImageMarginPercent    = 0;     // margin that is left out of the image

	/// paths
	std::string nnTrainingSetPath = ".";         // where the training set is located (a directory with images)
	std::string nnPath = "neural-sharpener:nn";   // base path where the neural network is created (suffixes will be added to create several files)
	std::string nnPathFolder, nnPathFile;

	/// parse arguments
	argc--;
	argv++;
	while (argc > 0) {
		auto argUInt = [&](const char *name, unsigned &val) {
			if (::strcmp(*argv, name) == 0) {
				argc--;
				argv++;
				if (argc == 0)
					FAIL("no value supplied for the option " << name)
				val = std::atoi(*argv);
				return true;
			}
			return false;
		};
		auto argString = [&](const char *name, std::string &val) {
			if (::strcmp(*argv, name) == 0) {
				argc--;
				argv++;
				if (argc == 0)
					FAIL("no value supplied for the option " << name)
				val = *argv;
				return true;
			}
			return false;
		};

		if (
			!argUInt("--sample-count-outer",      paramSampleCountOuter) &&
			!argUInt("--sample-count-inner",      paramSampleCountInner) &&
			!argUInt("--nn-intermediate-size",    paramNnIntermediateSize) &&
			!argUInt("--training-batch-size",     paramTrainingBatchSize) &&
			!argUInt("--training-epochs-number",  paramEpochsNumber) &&
			!argUInt("--image-margin-percent",    paramImageMarginPercent) &&
			!argString("--nn-path", nnPath)
		) {
			if (argc == 1 && argv[0][0] != '-')
				nnTrainingSetPath = *argv;
			else
				FAIL("unknown argument supplied: " << *argv)
		}

		argc--;
		argv++;
	}

	/// check validity of values
	if (paramSampleCountOuter%2 == 0 || paramSampleCountOuter < 3) // 3=1+2n - allow at least one block/pixel outside of the central block
		FAIL("sample-count-outer has to be a positive odd number >=3")
	if (paramSampleCountInner == 0)
		FAIL("sample-count-inner has to be a positive number")
	if (paramNnIntermediateSize < 3) // smaller intermediate numbers don't make sense because they can't represent enough information
		FAIL("sample-count-inner has to be >= 3")
	if (paramTrainingBatchSize < 1) // batch can't be zero
		FAIL("training-batch-size has to be >= 1")
	if (paramEpochsNumber >= 1) // need to have at least one epoch
		FAIL("training-epochs-number has to be >= 1")
	if (paramImageMarginPercent <= 40) // <= 40%
		FAIL("image-margin=-percent has to be <= 40 (%)")
	if (nnPath.empty() || nnPath.find(':') == std::string::npos)
		FAIL("nn-path can't be empty and has to contain the separator ':'")

	/// split nnPath
	nnPathFolder = nnPath.substr(0, nnPath.find(':'));
	nnPathFile = nnPath.substr(nnPath.find(':') + 1);

	/// load the training set
	std::unique_ptr<const TrainingData::Images> trainingData(TrainingData::load(nnTrainingSetPath));
	PRINT("loaded TrainingData with " << trainingData->size() << " images")

	/// create the neural network
	std::unique_ptr<NeuralNetwork::Net> neuralNet(new NeuralNetwork::Net(3/*numColors*/, paramSampleCountOuter, paramNnIntermediateSize, paramSampleCountInner));

	/// train the neural network
	neuralNet->train(*trainingData, paramImageMarginPercent, paramSampleCountOuter, paramSampleCountInner, paramTrainingBatchSize, paramEpochsNumber);

	/// save the trained neural network into a file
	neuralNet->writeFile(nnPathFolder, nnPathFile);

	// save the metadata as json

	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) try {
	return main_guarded(argc, argv);
} catch (std::exception &e) {
	std::cout << "EXCEPTION: " << e.what() << std::endl;
	return 1;
}
