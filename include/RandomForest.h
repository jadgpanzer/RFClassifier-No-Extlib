#ifndef RFH
#define RFH

#include "util.h"
#include "Dataset.h"
#include "TreeNode.h"
#include "DecisionTree.h"




class RandomForest {
    public:
        RandomForest(int num_trees, int max_depth, int num_features);
        
        void train(const Dataset& data);
        int predict(const Sample& sample) const;
        void save_model(const std::string& path) const;
        void load_model(const std::string& path);
        
    private:
        std::vector<DecisionTree> trees;
        int num_trees;      // 树的数量
        int max_depth;      // 深度
        int num_features;   //特征数量，和数据集相匹配
        
        Dataset bootstrap_sample(const Dataset& data) const;
};


#endif