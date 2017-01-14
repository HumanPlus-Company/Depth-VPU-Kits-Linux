#ifndef MoveSenseCamera_H_
#define MoveSenseCamera_H_

#include "CameraMode.h"
#include "MoveSenseErrorCode.h"

namespace movesense{

class MoveSenseCamera
{
public:
	MoveSenseCamera(CameraMode mode = D_VPU_320X240_LR_30FPS);
	~MoveSenseCamera(void);


	//打开相机，启动采集
	MoveSenseErrorCode OpenCamera();

	//关闭相机
	void CloseCamera();

	//相机的使能开关
	void EnableCam(bool val);

	//矫正开关
	void SetUndistort(bool val);

	//获取设备ID
	int GetCameraID();

	//获取相机参数-t_p2
	void GetCamParas(MsgPkg& p);

	//读取图片
	int GetImageData(unsigned char * &data, int &len);


//**************************************************
	//----保留暂不对外提供start
	void SetSM_Para(unsigned char val);
	void SetSM_P1(unsigned char val);
	void SetSM_P2(unsigned char val);
	void SetSM_P3(unsigned char val);
	void SetDesiredBin(unsigned char val);
	//命令控制寄存器
	void SetCamCmd(unsigned char val);
	//----保留暂不对外提供end

	//******暂时用不到-start
	//模式相关
	void SetCameraMode(CameraMode cameraMode);
	CameraMode GetCameraMode();

	//版本ID
	void SetVersionID(unsigned char id);
	unsigned char GetVersionID();
	//设备ID
	void SetCameraID(int id);

	//相机复位开关--Reset引脚
	void ResetCamera(bool res_val);
	//*****暂时用不到-end
//**************************************************
};
}
#endif
