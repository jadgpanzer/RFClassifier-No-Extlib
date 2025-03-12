#include "RandomForest.h"
// RandomForest implementation
RandomForest::RandomForest(int num_trees, int max_depth, int num_features)
    : num_trees(num_trees), max_depth(max_depth), num_features(num_features) {}

Dataset RandomForest::bootstrap_sample(const Dataset& data) const {
    Dataset sample;
    sample.num_classes = data.num_classes;
    
    if (data.samples.empty()) {
        throw std::runtime_error("Cannot bootstrap from empty dataset");
    }
    
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<size_t> dist(0, data.samples.size() - 1);
    
    for (size_t i = 0; i < data.samples.size(); ++i) {
        sample.samples.push_back(data.samples[dist(rng)]);
    }
    return sample;
}

void RandomForest::train(const Dataset& data) {
    trees.clear();
    for (int i = 0; i < num_trees; ++i) {
        Dataset sample_data = bootstrap_sample(data);
        DecisionTree tree(max_depth, num_features);
        tree.train(sample_data);
        trees.push_back(std::move(tree));
    }
}

int RandomForest::predict(const Sample& sample) const {
    std::map<int, int> votes;
    for (const auto& tree : trees) {
        int pred = tree.predict(sample);
        votes[pred]++;
    }
    
    int max_votes = -1;
    int result = -1;
    for (const auto& pair : votes) {
        if (pair.second > max_votes) {
            max_votes = pair.second;
            result = pair.first;
        }
    }
    return result;
}

void RandomForest::save_model(const std::string& path) const {
    std::ofstream fout(path);
    if (!fout) throw std::runtime_error("Failed to open file: " + path);
    
    // 写入元数据
    fout << num_trees << " "
         << max_depth << " "
         << num_features << "\n";
    
    // 保存每棵树
    for (const auto& tree : trees) {
        tree.save(fout);
    }
}

void RandomForest::load_model(const std::string& path) {
    std::ifstream fin(path);
    if (!fin) throw std::runtime_error("Failed to open file: " + path);
    
    // 读取元数据
    int loaded_trees, loaded_depth, loaded_features;
    fin >> loaded_trees >> loaded_depth >> loaded_features;
    
    // 验证参数一致性
    if (loaded_depth != max_depth || loaded_features != num_features) {
        throw std::runtime_error("Unmatched parameters!\n");
    }
    
    // 加载每棵树
    trees.clear();
    for (int i = 0; i < loaded_trees; ++i) {
        DecisionTree tree(0, 0);  // 参数会被加载覆盖
        tree.load(fin);
        trees.push_back(std::move(tree));
    }
}

