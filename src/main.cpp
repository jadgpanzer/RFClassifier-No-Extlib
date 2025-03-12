/*
这是一个示例的项目，使用的是鸢尾花IRIS数据集
数据集位于data\iris.data
*/
#include "RandomForest.h"

int main(int argc,char* argv[]) {
    Dataset sample_data;
    string path(argv[1]);
    sample_data.load_dataset(path);
    sample_data.num_classes = 4;
    RandomForest rf(100, 5, 4); // 100棵树，最大深度5，4个特征
    
    // 训练模型
    rf.train(sample_data);
    // 保存模型
    rf.save_model("forest.model");
    // 加载模型
    RandomForest rf2(100, 5, 4);
    rf2.load_model("forest.model");
    // 预测示例（使用IRIS数据集，这里预测结果应该为2）
    Sample test_sample{{5.8, 2.8, 5.1, 2.4}};
    int prediction = rf2.predict(test_sample);
    printf("Result = %d",prediction);
    return 0;
}
