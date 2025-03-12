#ifndef TNDH
#define TNDH
#include "util.h"
struct TreeNode {
    bool is_leaf;
    int split_feature;
    float threshold;
    int class_label;
    TreeNode* left;
    TreeNode* right;

    // 序列化方法
    void serialize(std::ostream& out) const;
    static TreeNode* deserialize(std::istream& in);

    TreeNode() : is_leaf(false), split_feature(-1), threshold(0), 
                class_label(-1), left(nullptr), right(nullptr) {}
};

#endif 