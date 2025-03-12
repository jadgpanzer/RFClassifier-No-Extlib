#ifndef DSTH
#define DSTH
#include "util.h"
struct Sample {
    std::vector<float> features;
    int label;
};

struct Dataset {
    std::vector<Sample> samples;
    int num_classes;
    void load_dataset(string dataset_path);
};

#endif 