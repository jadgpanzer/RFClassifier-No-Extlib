#include "TreeNode.h"
TreeNode* deserialize_with_null(std::istream& in) {
    std::string marker;
    in >> marker;
    if (marker == "N") {
        return nullptr;
    } else {
        in.seekg(-marker.size(), std::ios::cur); // 回退读取位置
        return TreeNode::deserialize(in);
    }
}
void TreeNode::serialize(std::ostream& out) const {
    if (is_leaf) {
        out << "L " << class_label << "\n";
        out << "N\nN\n";  // 显式标记叶子节点没有子节点
    } else {
        out << "I " << split_feature << " " << threshold << "\n";
        // 递归序列化子树
        if (left) left->serialize(out);
        else out << "N\n";
        if (right) right->serialize(out);
        else out << "N\n";
    }
}

TreeNode* TreeNode::deserialize(std::istream& in) {
    if (!in.good()) {
        throw std::runtime_error("Unexpected end of stream");
    }
    std::string type;
    in >> type;
    
    TreeNode* node = new TreeNode();
    if (type == "L") {
        node->is_leaf = true;
        in >> node->class_label;
        // 显式设置叶子节点的子节点为空
        node->left = nullptr;
        node->right = nullptr;
        
        // 消耗掉后续的两个N标记
        std::string n1, n2;
        in >> n1 >> n2;
    } else if (type == "I") {
        node->is_leaf = false;
        in >> node->split_feature >> node->threshold;
        // 递归反序列化子树
        node->left = deserialize_with_null(in);
        node->right = deserialize_with_null(in);
    } else if (type == "N") {
        delete node;
        return nullptr;
    }
    return node;
}