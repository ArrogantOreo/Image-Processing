#include<iostream>
#include<vector>
#include<cmath>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/**
 * @brief 三通道非局部均值滤波 (Non-Local Means Filter)
 * * @param src          输入图像 (CV_8UC3)
 * @param patchRadius  邻域窗口(图像块)的半径。例如设为3，则块大小为 7x7
 * @param searchRadius 搜索窗口的半径。例如设为10，则搜索范围为 21x21
 * @param h            平滑参数，控制去噪程度
 * @return Mat         滤波后的输出图像
 */
void nonLocalMeansFilter(const Mat& src,Mat& dst,int patchRadius=3, int searchRadius=10,double h=10){
    //确保输入图像是三通道
    CV_Assert(src.type()==CV_8UC3);
    dst = Mat::zeros(src.size(), src.type());
    //图像边缘扩充，避免边界
    Mat padded;
    int pad = searchRadius + patchRadius;
    copyMakeBorder(src,padded,pad,pad,pad,pad,BORDER_REFLECT);

    double h2 = h * h;
    //计算块内像素通道的总数（用于计算距离平均值）
    int patchElements = (2*patchRadius+1)*(2*patchRadius+1)*3;
    // 遍历原始图像的每一个像素
    for(int i=0;i<src.rows;i++){
        for(int j=0;j<src.cols;j++){
            //对应在pad中的位置
            int pi=i+pad;
            int pj=j+pad;
            double sumWeights=0.0;
            Vec3d sumValues(0.0,0.0,0.0);//用于累加带权重的BGR值
            //在搜索窗口内遍历
            for(int dy= -searchRadius; dy <= searchRadius;dy++){
                for(int dx=-searchRadius; dx <= searchRadius;dx++){
                    int si=pi+dy;
                    int sj=pj+dx;
                    
                    //计算两个图像块之间的欧氏距离平方
                    double dist=0.0;
                    for(int py=-patchRadius;py<=patchRadius;py++){
                        for(int px=-patchRadius;px<=patchRadius;px++){
                            Vec3b p1=padded.at<Vec3b>(pi+py,pj+px);
                            Vec3b p2=padded.at<Vec3b>(si+py,sj+px); 
                            dist+=(p1[0]-p2[0])*(p1[0]-p2[0])+
                                  (p1[1]-p2[1])*(p1[1]-p2[1])+
                                  (p1[2]-p2[2])*(p1[2]-p2[2]);
                        }
                    }
                    //将距离除以元素个数，使其对块大小不那么敏感
                    dist/=patchElements;
                    //计算高斯权重
                    double weight=exp(-dist/h2);
                    sumWeights+=weight;
                    Vec3b neighborPixel=padded.at<Vec3b>(si,sj);
                    sumValues[0]+=weight*neighborPixel[0];
                    sumValues[1]+=weight*neighborPixel[1];
                    sumValues[2]+=weight*neighborPixel[2];
                }
            }
            // 归一化并赋值给输出图像
            dst.at<Vec3b>(i,j)[0]=saturate_cast<uchar>(sumValues[0] / sumWeights);
            dst.at<Vec3b>(i,j)[1]=saturate_cast<uchar>(sumValues[1] / sumWeights);
            dst.at<Vec3b>(i,j)[2]=saturate_cast<uchar>(sumValues[2] / sumWeights);
        }
    }
}
int main(){
    Mat src=imread("../test.jpg",IMREAD_COLOR);
    if (src.empty()) {
        cerr << "Error: 无法读取图像，请检查文件路径！" << endl;
        return -1;
    }
    Mat dst;
    cout << "开始非局部均值滤波，请稍候 (该算法时间复杂度较高)..." << endl;
    
    nonLocalMeansFilter(src,dst,10,3,10.0);
    imshow("Original Image", src);
    imshow("NL-Means Filtered", dst);
    
    cout << "处理完成！按任意键退出..." << endl;
    waitKey(0);

    return 0;
}

