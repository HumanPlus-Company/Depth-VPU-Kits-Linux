#include "MoveSenseCamera.h"
#include "Camera.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
//lw add
unsigned char lsb,msb;

namespace movesense{

	Camera *g_camif = 0;

MoveSenseCamera::MoveSenseCamera(CameraMode mode)
{
	g_camif = new Camera(mode);
}


MoveSenseCamera::~MoveSenseCamera(void)
{
	delete g_camif;
}


MoveSenseErrorCode MoveSenseCamera::OpenCamera(){
	return g_camif->OpenCamera();
}


void MoveSenseCamera::CloseCamera(){
	g_camif->CloseCamera();
}
	

int MoveSenseCamera::GetImageData(unsigned char * &data, int &len){
	return g_camif->GetImageData(data,len);
}

//lw add

//--保留暂不对外提供start
void MoveSenseCamera::SetSM_Para(unsigned char val){
	g_camif->I2CWrite(0xC0,val);
}
void MoveSenseCamera::SetSM_P1(unsigned char val){
	g_camif->I2CWrite(0xC1,val);
}
void MoveSenseCamera::SetSM_P2(unsigned char val){
g_camif->I2CWrite(0xC2,val);
}
void MoveSenseCamera::SetSM_P3(unsigned char val){
	g_camif->I2CWrite(0xC3,val);
}
void MoveSenseCamera::SetDesiredBin(unsigned char val){
	g_camif->I2CWrite(0xE3,val);
}
void MoveSenseCamera::SetCamCmd(unsigned char val){
	g_camif->I2CWrite(0x08,val);
}
//--保留暂不对外提供end

//矫正开关
void MoveSenseCamera::SetUndistort(bool val){
	if(val == true){
		g_camif->I2CWrite(0xB4,0x01);
		if(g_camif->I2CRead(0xB4) == 1)
			printf("SetUndistort Success!\n");
		else
			printf("SetUndistort Fail!\n");
	}
	else
	{
		g_camif->I2CWrite(0xB4,0x00);
		if(g_camif->I2CRead(0xB4) == 0)
			printf("SetUndistort Success!\n");
		else
			printf("SetUndistort Fail!\n");
	}
}
//版本ID

void MoveSenseCamera::SetVersionID(unsigned char id){
	g_camif->I2CWrite(0x00,id);
}
unsigned char MoveSenseCamera::GetVersionID(){
	return (unsigned char)g_camif->I2CRead(0x00);
}
//设备ID的设置--不对外提供
void MoveSenseCamera::SetCameraID(int id){
	g_camif->I2CWrite(0x01,(unsigned char)(id & 0xff));
	g_camif->I2CWrite(0x02,(unsigned char)((id >>8) & 0xff));
	g_camif->I2CWrite(0x03,(unsigned char)((id >>16) & 0xff));
	g_camif->I2CWrite(0x04,(unsigned char)((id >>24) & 0xff));
	usleep(1000);//1ms
	g_camif->I2CWrite(0x08,0x20);
}
//设备ID的获取
int MoveSenseCamera::GetCameraID(){
	unsigned char id0,id1,id2,id3;
	id0 = (unsigned char)g_camif->I2CRead(0x01);
	id1 = (unsigned char)g_camif->I2CRead(0x02);
	id2 = (unsigned char)g_camif->I2CRead(0x03);
	id3 = (unsigned char)g_camif->I2CRead(0x04);
	printf("Camera ID:%x-%x-%x-%x\n",id3,id2,id1,id0);
	return (int)((id3<<24) + (id2<<16) + (id1<<8) + id0);
}
//复位相机--Reset引脚
void MoveSenseCamera::ResetCamera(bool res_val){
	if(res_val == true)
		g_camif->SendAddr(250);
	else
		g_camif->SendAddr(251);
}
//使能相机
void MoveSenseCamera::EnableCam(bool val){
	if(val == true)
		g_camif->I2CWrite(0x09,0x01);
	else
		g_camif->I2CWrite(0x09,0x00);
}

//获取相机参数t_P2
void MoveSenseCamera::GetCamParas(MsgPkg& p){
	p.data = (unsigned char*)malloc(20); //必须要
	p.fdata = (float*)malloc(5);

	for(int i = 144; i < 41*sizeof(float); i++)
	{
		g_camif->SendAddr(16 + i);
		p.data[i-144] = (unsigned char)g_camif->GetValue();
		usleep(2000);//2ms
	}

	memcpy(p.fdata, p.data, 5*sizeof(float));
	p.fdata[4] = p.fdata[4] * p.fdata[0];
	//printf("t_P2 is ::\n t_P2(0,0)= %f\n t_P2(1,1)= %f\n t_P2(0,2)= %f\n t_P2(1,2)= %f\n t_P2(0,3)= %f\n",p.fdata[0],p.fdata[1],p.fdata[2],p.fdata[3],p.fdata[4]);
}
//lw add end

void MoveSenseCamera::SetCameraMode(CameraMode cameraMode){
	g_camif->SetCameraMode(cameraMode);	
}

CameraMode MoveSenseCamera::GetCameraMode(){
	return 	g_camif->GetCameraMode();
}

}
