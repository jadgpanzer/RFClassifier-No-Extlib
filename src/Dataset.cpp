#include "Dataset.h"


using namespace std;

void Dataset::load_dataset(string dataset_path) {
    ifstream file(dataset_path);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << dataset_path << endl;
        return;
    }

    samples.clear(); // 清空现有数据
    string line;
    int expected_columns = -1;

    while (getline(file, line)) {
        // 跳过空行
        if (line.find_first_not_of(" \t\n") == string::npos) {
            continue;
        }

        vector<string> tokens;
        istringstream iss(line);
        string token;

        // 分割行内容
        while (iss >> token) {
            tokens.push_back(token);
        }

        // 忽略空行和列数不匹配的行
        if (tokens.empty()) continue;

        // 确定列数（基于第一行）
        if (expected_columns == -1) {
            expected_columns = tokens.size();
            if (expected_columns < 2) { // 至少1个特征+1个标签
                cerr << "Error: Invalid column count in first line" << endl;
                expected_columns = -1;
                continue;
            }
        } else if (tokens.size() != expected_columns) {
            cerr << "Warning: Skipping line with mismatched columns" << endl;
            continue;
        }

        // 解析特征和标签
        Sample sample;
        try {
            // 解析前 N-1 列为浮点数特征
            for (size_t i = 0; i < tokens.size() - 1; ++i) {
                sample.features.push_back(stof(tokens[i]));
            }

            // 解析最后一列为整数标签
            sample.label = stoi(tokens.back());
        } catch (const invalid_argument& e) {
            cerr << "Error: Invalid number format in line: " << line << endl;
            continue;
        } catch (const out_of_range& e) {
            cerr << "Error: Number out of range in line: " << line << endl;
            continue;
        }

        samples.push_back(sample);
    }

    // 注意：num_classes 需要手动设置，此处不处理
}
