#include "MoveSenseCamera.h"
#include "CameraMode.h"
#include "Camera.h"
#include <iostream>
#include <opencv_modules.hpp>
#include <opencv.hpp>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#define TEST_FPS 0 //帧率显示控制

using namespace std;
using namespace cv;

int main()
{
	//选择模式：D_VPU_320X240_LR_30FPS（左右图）  D_VPU_320X240_LD_30FPS（左图视差图）   D_VPU_320X240_LRD_30FPS（左右图视差图）
	int sel  = D_VPU_320X240_LD_30FPS;
	movesense::MoveSenseCamera c(sel);
	if( !(movesense::MS_SUCCESS == c.OpenCamera() ))
	{
		printf("Open Camera Faild! \n");
		return false;
	}
	else{
		printf("Open Camera Success! \n");
		c.GetCameraID();//获取设备ID
	}


	//获取相机参数-t_P2
	//MsgPkg pkg;
	//c.GetCamParas(pkg);
	//printf("t_P2 is ::\n t_P2(0,0)= %0.8f\n t_P2(1,1)= %0.8f\n t_P2(0,2)= %0.8f\n t_P2(1,2)= %0.8f\n t_P2(0,3)= %0.8f\n",pkg.fdata[0],pkg.fdata[1],pkg.fdata[2],pkg.fdata[3],pkg.fdata[4]);
	

	int width = 320;
	int height = 240;
	int len_two  = width*height*2;
	int len_three = width*height*3;
	cv::Mat left(height,width,CV_8UC1),right(height,width,CV_8UC1),dip(height,width,CV_8UC1);
	unsigned char *im_data_LR  = new unsigned char[width*height*2];
	unsigned char *im_data_LRD = new unsigned char[width*height*3];

#if TEST_FPS
	int cnt = 0;
	timeval starttime,endtime;
	gettimeofday(&starttime,0);
#endif

	while(1)
	{
#if TEST_FPS
		if( len_three > 0 || len_two >0){
			len_two =0;
			len_three =0;
			cnt ++;
			if(cnt == 50)
			{
				gettimeofday(&endtime,0);
				double timeuse = 1000000*(endtime.tv_sec - starttime.tv_sec) + endtime.tv_usec - starttime.tv_usec;
				timeuse /=1000000;
				printf("fps is %f.\r\n",cnt/timeuse);
				gettimeofday(&starttime,0);
				cnt = 0;
			}
		}
#endif

		if( (sel == D_VPU_320X240_LR_30FPS) || (sel == D_VPU_320X240_LD_30FPS))
		{
			c.GetImageData(im_data_LR ,len_two);
			if(len_two>0)
			{
				for(int i = 0 ; i < height; i++)
				{
					memcpy(left.data+width*i,im_data_LR+i*width*2,width);
					memcpy(right.data+width*i,im_data_LR+i*width*2+width,width);
				}
			}

			cv::imshow("left",left);
			if(D_VPU_320X240_LR_30FPS == sel)
				cv::imshow("right",right);  //显示左右图
			else
				cv::imshow("disp",right);  //显示左图和视差图

		}

		else if(sel == D_VPU_320X240_LRD_30FPS)
		{
			c.GetImageData(im_data_LRD ,len_three);
			if(len_three>0)
			{
				for(int i = 0 ; i < height; i++)
				{
					memcpy(left.data+width*i,im_data_LRD+i*width*3,width);
					memcpy(right.data+width*i,im_data_LRD+i*width*3+width,width);
					memcpy(dip.data+width*i,im_data_LRD+i*width*3+width*2,width);
				}
			}
			//显示左右图和视差图
			cv::imshow("left",left);
			cv::imshow("right",right);
			cv::imshow("disp",dip);

		}
		else
		{
			printf("Selected mode is not supported!! Program is exiting!\n");
			break;
		}

		char key = cv::waitKey(10);
		if(key =='Q' || key=='q')
			break;
	}
	c.CloseCamera();
	delete im_data_LR;
	delete im_data_LRD;
	return 0;
}


