#include "DecisionTree.h"
// DecisionTree implementation

float gini_impurity(const std::vector<int>& labels, int num_classes) {
    std::vector<int> counts(num_classes, 0);
    for (int label : labels) {
        counts[label]++;
    }
    float impurity = 1.0;
    for (int count : counts) {
        float prob = static_cast<float>(count) / labels.size();
        impurity -= prob * prob;
    }
    return impurity;
}

DecisionTree::DecisionTree(int max_depth, int num_features) 
    : max_depth(max_depth), num_features(num_features), root(nullptr) {}

DecisionTree::~DecisionTree() {
    delete_tree(root);
}

void DecisionTree::delete_tree(TreeNode* node) {
    if (node) {
        delete_tree(node->left);
        delete_tree(node->right);
        delete node;
        node = nullptr; 
    }
}

void DecisionTree::train(const Dataset& data) {
    root = build_tree(data, 0);
}

TreeNode* DecisionTree::build_tree(const Dataset& data, int depth) {
    // Base cases
    if (data.samples.empty()) return nullptr;
    
    // Check purity
    std::vector<int> labels;
    for (const auto& sample : data.samples) {
        labels.push_back(sample.label);
    }
    
    int majority = *std::max_element(labels.begin(), labels.end(),
        [&](int a, int b) { return std::count(labels.begin(), labels.end(), a) < 
                                  std::count(labels.begin(), labels.end(), b); });
    
    if (gini_impurity(labels, data.num_classes) < 0.1 || depth >= max_depth) {
        TreeNode* leaf = new TreeNode();
        leaf->is_leaf = true;
        leaf->class_label = majority;
        return leaf;
    }
    
    // Feature selection
    std::vector<int> features(num_features);
    std::iota(features.begin(), features.end(), 0);
    std::shuffle(features.begin(), features.end(), std::mt19937{std::random_device{}()});
    int selected_features = sqrt(num_features);
    features.resize(selected_features);
    
    // Find best split
    float best_gini = INFINITY;
    int best_feature = -1;
    float best_threshold = 0;
    
    for (int feature : features) {
        // Get unique values
        std::vector<float> values;
        for (const auto& sample : data.samples) {
            values.push_back(sample.features[feature]);
        }
        std::sort(values.begin(), values.end());
        
        // Try thresholds
        for (size_t i = 1; i < values.size(); ++i) {
            float threshold = (values[i-1] + values[i]) / 2.0;
            
            std::vector<int> left_labels, right_labels;
            for (const auto& sample : data.samples) {
                if (sample.features[feature] < threshold) {
                    left_labels.push_back(sample.label);
                } else {
                    right_labels.push_back(sample.label);
                }
            }
            
            float gini = (left_labels.size() * gini_impurity(left_labels, data.num_classes) +
                        right_labels.size() * gini_impurity(right_labels, data.num_classes)) 
                        / data.samples.size();
            
            if (gini < best_gini) {
                best_gini = gini;
                best_feature = feature;
                best_threshold = threshold;
            }
        }
    }
    
    // Split data
    Dataset left_data, right_data;
    left_data.num_classes = data.num_classes;
    right_data.num_classes = data.num_classes;
    
    for (const auto& sample : data.samples) {
        if (sample.features[best_feature] < best_threshold) {
            left_data.samples.push_back(sample);
        } else {
            right_data.samples.push_back(sample);
        }
    }
    
    TreeNode* node = new TreeNode();
    node->split_feature = best_feature;
    node->threshold = best_threshold;
    node->left = build_tree(left_data, depth + 1);
    node->right = build_tree(right_data, depth + 1);
    
    return node;
}

int DecisionTree::predict(const Sample& sample) const {
    return predict_helper(root, sample);
}

int DecisionTree::predict_helper(TreeNode* node, const Sample& sample) const {
    if (node->is_leaf) {
        return node->class_label;
    }
    
    if (sample.features[node->split_feature] < node->threshold) {
        return predict_helper(node->left, sample);
    } else {
        return predict_helper(node->right, sample);
    }
}


void DecisionTree::save(std::ofstream& fout) const {
    // 写入树参数
    fout << max_depth << " " 
         << num_features << "\n";
    
    // 序列化树结构
    serialize_tree(fout, root);
}

void DecisionTree::serialize_tree(std::ostream& out, TreeNode* node) const {
    if (!node) {
        out << "N\n";  // 空节点标记
        return;
    }
    node->serialize(out);
    serialize_tree(out, node->left);
    serialize_tree(out, node->right);
}

void DecisionTree::load(std::ifstream& fin) {
    // 清除旧树
    delete_tree(root);
    // 读取参数
    fin >> max_depth >> num_features;
    // 重建树结构
    root = deserialize_tree(fin);
}

TreeNode* DecisionTree::deserialize_tree(std::istream& in) {
    char marker;
    in >> marker;
    
    if (marker == 'N') {
        return nullptr;
    }
    // 回退读取位置
    in.seekg(-1, std::ios::cur);
    TreeNode* node = TreeNode::deserialize(in);
    node->left = deserialize_tree(in);
    node->right = deserialize_tree(in);
    return node;
}