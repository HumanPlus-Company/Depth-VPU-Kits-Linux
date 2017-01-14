#pragma once
#include "CameraMode.h"
#include "MoveSenseErrorCode.h"
namespace movesense
{

using namespace movesense;

	typedef enum{
		MOVESENSE_USB20,
		MOVESENSE_USB30,
	}MoveSenseUSBSpeed;

class Camera
{

public:
	Camera(CameraMode mode = D_VPU_320X240_LR_30FPS);
	~Camera();

public:
	MoveSenseErrorCode OpenCamera();


	void CloseCamera();
	

	int GetImageData(unsigned char * &data, int &len);

	//lw add
	void SendAddr(int addr);

	void SetValue(int val);

	unsigned char GetValue();

	void I2CWrite(int addr,int val);

	unsigned char I2CRead(int addr);
	//lw add end

	void SetCameraMode(CameraMode cameraMode);
	CameraMode GetCameraMode();

private:
	bool m_camera_opened;
	MoveSenseUSBSpeed  m_usbspeed;
	CameraMode m_mode;
};

}

