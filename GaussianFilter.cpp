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
    cv::Mat img = cv::imread("D:\\Collision Distance\\Raw_data\\Police_View\\images\\school_2.jpg", cv::IMREAD_COLOR);
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
    double sigma = 1.0;
    cout << "正在生成 " << kernelSize << "x" << kernelSize << " 的高斯卷积核..." << endl;
    vector<vector<double>> kernel = generateGaussianKernel(kernelSize, sigma);


    // 创建3个通道的矩阵
    //Opencv的顺序BGR(蓝、绿、红)
    vector<vector<vector<double>>> channels(3,vector<vector<double>>(rows,vector<double>(cols,0.0)));
    vector<vector<vector<double>>> blurredChannels(3,vector<vector<double>>(rows,vector<double>(cols,0.0)));
    

    // 4. 【核心调用】运行你自己手写的底层高斯滤波算法！
    cout << "正在拆分通道并执行卷积计算..." << endl;
    for(int c=0;c<3;c++){
        //数据桥接：提取单通道
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                channels[c][i][j]=static_cast<double>(img.at<cv::Vec3b>(i,j)[c]);
            }
        }
        //调用手写高斯滤波算法
        cout << "正在处理通道 " << c << " (共3通道)..." << endl;
        blurredChannels[c]=applyGaussianFilterWithPadding(channels[c],kernel);
    }

    // 5. 合并结果回 OpenCV 格式 (CV_8UC3 代表彩色)
    cv::Mat outImg(rows, cols, CV_8UC3);
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            cv::Vec3b bgr;
            for(int c=0;c<3;c++){
                double val=blurredChannels[c][i][j];
                bgr[c]=static_cast<uchar>(std::max(0.0, std::min(255.0,val)));
            }
            outImg.at<cv::Vec3b>(i,j)=bgr;
        }
    }
    // 6. 见证奇迹：弹出窗口，并排显示原图和你处理后的图像！
    cout << "彩色处理完成！请查看弹出的图像窗口。" << endl;
    cv::imshow("Original Image", img);
    cv::imshow("My Custom Gaussian Blur", outImg);
    
    // 程序在此暂停，等待你在弹出的图像窗口上按键盘任意键关闭
    cv::waitKey(0); 

    return 0;
}