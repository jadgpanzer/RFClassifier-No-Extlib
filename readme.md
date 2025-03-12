# 无须外部库的随机森林分类器（Random-Forest Classifier WithOut External Library）
本项目为使用C/C++构建的随机森林分类器，不需要任何外部库即可实现，避免了安装和配置libtorch等繁琐的工作，适合在系统底层进行部署
## 编译过程
### 安装依赖
确保已安装以下工具：

‌CMake‌（版本 ≥ 3.12）

‌C++编译器‌（如GCC、Clang或MSVC）

‌构建工具‌（如Make、Ninja或Visual Studio）

（Linux用户可通过包管理器安装，例如`Ubuntu/Debian：sudo apt install build-essential cmake`）

### 安装过程
在根目录下创建一个`build`目录：
```
mkdir build
cd build
```
运行CMake生成构建文件：
Linux:
```
cmake .. -DCMAKE_BUILD_TYPE=Release  # 或 Debug
```
‌Windows（Visual Studio）：
```
cmake .. -G "Visual Studio 16 2019"  # 指定生成器版本
```
编译：
```
cmake --build . --config Release  # 或 Debug
```
最后生成的文件位于`build/Release/random_forest_demo`或者是`build/Debug/random_forest_demo`


### 数据集
使用`Dataset::load_dataset(string dataset_path)`方法指定数据集的路径

使用`Dataser::num_classes`指定分类数量

本方法提供了IRIS鸢尾花数据集<sup><a href="#ref1">1</a></sup>，在生成的可执行文件`random_forest_demo`中可以测试，使用以下命令调用：
```
.\Release\random_forest_demo <IRIS数据集的位置>
```
如果一切正常，程序应该输出：
```
Result = 2
```
1. <p name = "ref1">https://doi.org/10.24432/C56C76.</p>