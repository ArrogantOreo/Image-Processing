#include<iostream>
#include<vector>
#include<cmath>
#include <opencv2/opencv.hpp>
using namespace std;
vector<vector<double>>generateGaussianKernel(int kernelSize, double sigma){
    vector<vector<double>> kernel(kernelSize, vector<double>(kernelSize ,0.0));
    double sum=0.0;
    int center=kernelSize/2;
    for(int i=0;i<kernelSize;++i){
        for(int j=0;j<kernelSize;++j){
            int x=i-center;
            int y=j-center;
            kernel[i][j]=exp(-(x*x+y*y)/(2*sigma*sigma));
            sum+=kernel[i][j];
        }
    }
    for(int i=0;i<kernelSize;i++){
        for(int j=0;j<kernelSize;j++){
            kernel[i][j]=kernel[i][j]/sum;
        }
    }
    return kernel;
}
vector<vector<double>> applyGaussianFilterWithPadding(const vector<vector<double>>& image,const vector<vector<double>>& kernel){
    int rows=image.size();
    int cols=image[0].size();
    int kSize=kernel.size();
    int pad=kSize/2;

    vector<vector<double>> result(rows, vector<double>(cols,0.0));
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            double pixelValue=0.0;

            for(int ki=0;ki<kSize;ki++){
                for(int kj=0;kj<kSize;kj++){
                    int imgX=i+ki-pad;
                    int imgY=j+kj-pad;
                    imgX=max(0,min(imgX,rows-1));
                    imgY=max(0,min(imgY,cols-1));
                    pixelValue+=image[imgX][imgY]*kernel[ki][kj];
                }
            }
            result[i][j]=pixelValue;
        }
    }
    return result;
}
int main() {
    // 1. 读取本地图片 (请确保你的工程目录下有一张 test.jpg)
    // IMREAD_GRAYSCALE 表示将彩色图强制转换为单通道灰度图
    cv::Mat img = cv::imread("D:\\Collision Distance\\Raw_data\\Police_View\\images\\school_2.jpg", cv::IMREAD_GRAYSCALE);
    if (img.empty()) {
        cout <<"图片读取失败！请检查当前目录下是否有 test.jpg 文件。" << endl;
        return -1;
    }

    int rows = img.rows;
    int cols = img.cols;
    cout << "成功读取图像，尺寸: " << cols << "x" << rows << endl;

    // 2. 【数据桥接】将 OpenCV 的 Mat 转换为我们手写的 vector 矩阵
    cout << "正在将数据加载到底层矩阵..." << endl;
    vector<vector<double>> image(rows, vector<double>(cols, 0.0));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            image[i][j] = static_cast<double>(img.at<uchar>(i, j));
        }
    }

    // 3. 生成高斯核 (为了肉眼能明显看出效果，我们把核设大一点：11x11，sigma=3.0)
    int kernelSize = 11;
    double sigma = 3.0;
    cout << "正在生成 " << kernelSize << "x" << kernelSize << " 的高斯卷积核..." << endl;
    vector<vector<double>> kernel = generateGaussianKernel(kernelSize, sigma);

    // 4. 【核心调用】运行你自己手写的底层高斯滤波算法！
    cout << "正在执行二维卷积计算，这可能需要几秒钟（体现了算法的时间复杂度）..." << endl;
    vector<vector<double>> blurredImage = applyGaussianFilterWithPadding(image, kernel);

    // 5. 【数据桥接】将结果转换回 OpenCV 的格式用于显示
    cv::Mat outImg(rows, cols, CV_8UC1);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            double val = blurredImage[i][j];
            val = max(0.0, min(255.0, val)); // 安全截断，防止溢出
            outImg.at<uchar>(i, j) = static_cast<uchar>(val);
        }
    }

    // 6. 见证奇迹：弹出窗口，并排显示原图和你处理后的图像！
    cout << "处理完成！请查看弹出的图像窗口。" << endl;
    cv::imshow("Original Image", img);
    cv::imshow("My Custom Gaussian Blur", outImg);
    
    // 程序在此暂停，等待你在弹出的图像窗口上按键盘任意键关闭
    cv::waitKey(0); 

    return 0;
}