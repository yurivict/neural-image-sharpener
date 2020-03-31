
#include "misc.h"
#include "training-data.h"

#include <exception>
#include <cstdlib>
#include <string>


static int main_guarded(int argc, char *argv[]) {

	/// parameters
	unsigned paramSampleCountOuter      = 6;     // how many values to sample in the lo-res image around the sample square
	unsigned paramSampleCountInner      = 5;     // how many pixels to add in a low-res image between pixels to build a hi-res image
	unsigned paramNnIntermediateSize    = 100;   // the intermediate NN size between FullyConnected

	/// paths
	std::string nnTrainingSetPath = ".";         // where the training set is located (a directory with images)
	std::string nnPath = "./neural-sharpener";   // base path where the neural network is created (suffixes will be added to create several files)

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
			!argUInt("--sample-count-outer",   paramSampleCountOuter) &&
			!argUInt("--sample-count-inner",   paramSampleCountInner) &&
			!argUInt("--nn-intermediate-size", paramNnIntermediateSize) &&
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
	if (paramSampleCountOuter%2 != 0 || paramSampleCountOuter == 0)
		FAIL("sample-count-outer has to be a positive even number")
	if (paramSampleCountInner == 0)
		FAIL("sample-count-inner has to be a positive number")
	if (paramNnIntermediateSize < 3) // smaller intermediate numbers don't make sense because they can't represent enough information
		FAIL("sample-count-inner has to be >= 3")
	if (nnPath.empty())
		FAIL("nn-path can't be empty")

	/// load the training set
	std::unique_ptr<TrainingData::Images> trainingData(TrainingData::load(nnTrainingSetPath));
	PRINT("loaded TrainingData with " << trainingData->size() << " images")

	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) try {
	return main_guarded(argc, argv);
} catch (std::exception &e) {
	std::cout << "EXCEPTION: " << e.what() << std::endl;
	return 1;
}
