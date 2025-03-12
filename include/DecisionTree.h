#ifndef DCSTH
#define DCSTH
#include "util.h"
#include "Dataset.h"
#include "TreeNode.h"

class DecisionTree {
    public:
        DecisionTree(int max_depth, int num_features);
        ~DecisionTree();
    
        void train(const Dataset& data);
        int predict(const Sample& sample) const;
        void save(std::ofstream& fout) const;
        void load(std::ifstream& fin);
    
        // 禁用拷贝
        DecisionTree(const DecisionTree&) = delete;
        DecisionTree& operator=(const DecisionTree&) = delete;
        
        // 启用移动，确保原对象不再持有指针
        DecisionTree(DecisionTree&& other) noexcept 
            : root(other.root), max_depth(other.max_depth), num_features(other.num_features) {
            other.root = nullptr;
        }
        
        DecisionTree& operator=(DecisionTree&& other) noexcept {
            if (this != &other) {
                delete_tree(root);
                root = other.root;
                max_depth = other.max_depth;
                num_features = other.num_features;
                other.root = nullptr;
            }
            return *this;
        }
    private:
        TreeNode* root;
        int max_depth;
        int num_features;
        
        TreeNode* build_tree(const Dataset& data, int depth);
        void delete_tree(TreeNode* node);
        int predict_helper(TreeNode* node, const Sample& sample) const;
        // 序列化方法
        void serialize_tree(std::ostream& out, TreeNode* node) const;
        TreeNode* deserialize_tree(std::istream& in);
    };

#endif