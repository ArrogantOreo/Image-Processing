# 🖼️ Image-Processing Algorithm Workspace

Welcome to my Image Processing repository! 

本仓库用于记录和沉淀**图像处理、计算机视觉与视频编解码**领域的经典算法实现。坚持“造轮子”精神，从底层数学原理出发，手撕核心算法，探索像素级操作与画质优化方案。

本项目不仅是代码的堆砌，更是对算法底层逻辑的深度解剖，主要服务于车载视觉、流媒体开发及相关领域的探索与实践。

## 🛠️ 技术栈 (Tech Stack)
* **核心语言:** C++ / Python
* **关键技术:** 空间域/频率域滤波、形态学处理、画质客观评价指标 (PSNR/SSIM)、色彩空间转换 (YUV/RGB)
* **辅助工具:** OpenCV, FFmpeg, CMake

## 🗂️ 算法实现清单 (Algorithm Implementation List)

| 模块分类 (Category) | 算法名称 (Algorithm)         | 语言 (Lang) | 核心文件 (File)      | 状态 (Status) | 复盘博客 (Blog) |
| :------------------ | :--------------------------- | :---------: | :------------------- | :-----------: | :-------------- |
| **空间域滤波**      | 高斯滤波 (Gaussian Filter)   |     C++     | `GaussianFilter.cpp` |    ✅ 完成     | [待补充]()      |
| **空间域滤波**      | 均值滤波 / 中值滤波          |     C++     | -                    |    ⏳ 待办     | -               |
| **画质评价**        | PSNR & SSIM 计算脚本         |   Python    | -                    |    ⏳ 待办     | -               |
| **边缘检测**        | Sobel / Laplacian 算子       |     C++     | -                    |    ⏳ 待办     | -               |
| **色彩空间**        | RGB ↔ YUV420P 转换           |     C++     | -                    |    ⏳ 待办     | -               |
| **视频处理**        | 基于 FFmpeg 的码流抽帧与解析 |   Python    | -                    |    ⏳ 待办     | -               |

## 🚀 编译与运行指南 (How to run)
对于单一的 C++ 算法实现文件，可以直接使用 g++ 编译运行：
```bash
# 以高斯滤波为例
g++ GaussianFilter.cpp -o GaussianFilter
./GaussianFilter
