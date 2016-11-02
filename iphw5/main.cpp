#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include<opencv2/nonfree/nonfree.hpp>
#include<opencv2/legacy/legacy.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include<vector>
using namespace cv;
const int FEATURE_POINT=255;
int main(int argc, const char * argv[]) {
    // insert code here...
    
    Mat img = imread("/Users/sovietreborn/Desktop/input.jpg"),imggray;
    if(img.empty())
    {
        fprintf(stderr, "Can not load image \n");
        return -1;
    }
    int thetaSize=180;
    float phiSize, pi=3.14159265358979;
    vector<float> cosTab, sinTab;
    vector<vector<int>> houMtx;
    
    Mat gray,thresgray;
    gray.create(img.size(),CV_RGB2GRAY);
    cvtColor(img,gray,CV_RGB2GRAY);
    Mat resultsobel;
    resultsobel.create(img.size(),gray.type());
    for(int j= 1; j<img.rows-1; ++j)
        
    {
        
        const uchar *previous = gray.ptr<const uchar>(j-1);
        
        const uchar *current = gray.ptr<const uchar>(j);
        
        const uchar *next = gray.ptr<const uchar>(j+1);
        
        uchar *output = resultsobel.ptr<uchar>(j);
        
        for(int k= 1; k<img.cols-1; k++)
            
        {
            int i=k;
            output[i] =sqrt(pow((2*current[i+1]+previous[i+1]+next[i+1]-2*current[i-1]-previous[i-1]-next[i-1]),2)+pow((-2*previous[i]-previous[i+1]-previous[i-1]+2*next[i]+next[i+1]+next[i-1]),2));
        }
    }
        imwrite("/Users/sovietreborn/Desktop/1st.jpg", resultsobel);

     imwrite("/Users/sovietreborn/Desktop/grayst.jpg", gray);
    threshold(resultsobel, thresgray, 100, 255, CV_THRESH_BINARY);
    for (int i = 0; i < resultsobel.rows; i++)
    {
        for (int j = 0; j < resultsobel.cols; j++)
        {
            if(resultsobel.at<uchar>(i, j)>180)
            thresgray.at<uchar>(i, j)=255;
            //thresgray.at<uchar>(i, j)=0;
        }
    }
    
         imwrite("/Users/sovietreborn/Desktop/graytres.jpg", thresgray);
    float theta;
    cosTab.resize(180);
    sinTab.resize(180);
    for (int i = 0; i < thetaSize; i++)
    {
        theta = i * pi / 180.;
        cosTab[i] = cos(theta);
        sinTab[i] = sin(theta);
    }
    phiSize = sqrt(pow((float)thresgray.rows, 2) + pow((float)thresgray.cols, 2));
    vector<int> _hou(thetaSize, 0);
    houMtx.resize(int(2 * phiSize), _hou);
    vector<vector<int>> binaryImgMtx(thresgray.rows,vector<int>(thresgray.cols,0));
    
    for (int i = 0; i < thresgray.rows; i++)
    {
        for (int j = 0; j < thresgray.cols; j++)
        {
            binaryImgMtx[i][j]=thresgray.at<uchar>(i, j);
        }
    }
    float phi;
    
    for (int i = 0; i < binaryImgMtx.size(); i++)
    {
        for (int j = 0; j < binaryImgMtx[i].size(); j++)
        {
            if (binaryImgMtx[i][j] == FEATURE_POINT)
            {
                for (int nTheta = 0; nTheta < thetaSize; nTheta++)
                {
                    phi = i * sinTab[nTheta] + j * cosTab[nTheta];
                    houMtx[int(phi + phiSize)][nTheta]++;
                }
            }
        }
    }
    
    Mat houImg = Mat(int(2 * phiSize), thetaSize, CV_8UC1, cv::Scalar(0));

    for (int i = 0; i < houImg.rows; i++)
    {
        for (int j = 0; j < houImg.cols; j++)
        {
            if (houMtx[i][j] > 0)
            {
                houImg.at<uchar>(i, j) = 255;
            }
        }
    }
    vector<int> buf(3, 0);
    for (int i = 0; i < (int)phiSize; i++)
    {
        for (int j = 0; j < thetaSize; j++)
        {
            if (houMtx[i][j] > buf[2])
                buf[0] = i, buf[1] = j, buf[2] = houMtx[i][j];
        }
    }
    float num;
    Mat newImg = Mat(img.rows, img.cols,img.type(), cv::Scalar(255));
    newImg=img;
            Point tmp,rightnow;
    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {

            if (binaryImgMtx[i][j] == FEATURE_POINT)
            {
                num = i * sinTab[buf[1]] + j * cosTab[buf[1]];
                
                if (int(num + phiSize) == buf[0])
                {
                    rightnow.x=j;
                    rightnow.y=i;
                    if (tmp.x!=0&&tmp.y!=0) {

                        line( newImg, tmp, rightnow, Scalar( 0, 255, 0), 5 );
                    }
                    tmp=rightnow;

                }
            }
        }
    }
    imwrite("/Users/sovietreborn/Desktop/line.jpg", newImg);

    
}