#include <iostream>

#include "anoedgeglobal.hpp"
#include "utils.hpp"

void run_anoedge_global(int argc, char** argv) {
	if (argc != 6) {
		cout << "Expected five arguments but got " + to_string(argc) << endl;
		return;
	}

    string algorithm = argv[1];
	string dataset_name = argv[2];
	int rows = stoi(argv[3]);
	int buckets = stoi(argv[4]);
    double decay_factor = stof(argv[5]);

	AnoedgeGlobal anoedge_global(algorithm, dataset_name, rows, buckets, decay_factor);
	anoedge_global.run();
}

int main(int argc, char** argv) {
	if (argc < 2) {
		cout << "Expected at least two arguments but got " + to_string(argc) << endl;
		return 0;
	}
	string algorithm = argv[1];
	if (algorithm == "anoedge_g") {
		run_anoedge_global(argc, argv);
	} else {
		cout << "Algorithm " << algorithm << " doesn't match with any of the available algorithms (anoedge_g, anoedge_l, anograph, anograph_k)" << endl;
	}
	return 0;
}