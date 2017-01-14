#include "Camera.h"
#include "VL4IF.h"
#include "pid_vid.h"

namespace movesense
{
	unsigned char lrd_val;

	VL4_IF *g_cds = NULL;


	Camera::Camera(CameraMode mode):m_camera_opened(false),m_usbspeed(MOVESENSE_USB20)
	{
		int w = 320,h = 240;
		CAMERA_FPS fps = CAMERA_FPS_30;
		switch(mode)
		{
			case D_VPU_320X240_LR_30FPS:
				w = 320;
				h = 240;
				fps = CAMERA_FPS_30;
				lrd_val = 0x00;
				break;

			case D_VPU_320X240_LD_30FPS:
				w = 320;
				h = 240;
				fps = CAMERA_FPS_30;
				lrd_val = 0x10;
				break;

			case D_VPU_320X240_LRD_30FPS:
				w = 320;
				h = 360;
				fps = CAMERA_FPS_30;
				lrd_val = 0x20;
				break;
			default:
				printf("--default \n");
				break;
		}
		g_cds = new VL4_IF(w,h,fps);
		m_mode = mode;
	}


	Camera::~Camera(void)
	{
		delete g_cds;
	}

	MoveSenseErrorCode Camera::OpenCamera()
	{
		char* name = g_cds->ScanCamera(g_vid_usb,g_pid_usb2);
		if(name == NULL)
		{
			name = g_cds->ScanCamera(g_vid_usb,g_pid_usb3);
			if(name == NULL)
			{
				printf("ScanCamera faild \n");
				return MS_ERROR;
			}else
			{
				m_usbspeed = MOVESENSE_USB30;
				printf("USB 3.0 interface! \n");
			}
		}else
		{
			m_usbspeed = MOVESENSE_USB20;
			printf("USB 2.0 interface! \n");
		}
		if(g_cds->OpenCamera(name)) //have test 2.0
		{
			//先判断是否有错
			g_cds->SendI2CAddr(0x05);
			if(g_cds->GetI2CValue() != 0x01)
			{
				usleep(2000); //2ms
				g_cds->SendI2CAddr(0x06);
				printf("Camera error:register value is -%d- \n",g_cds->GetI2CValue());
				return MS_ERROR;
			}
			//lw add相机模式选择
			g_cds->SendI2CAddr(0x07);
			g_cds->SetI2CValue(lrd_val);
			usleep(2000); //2ms
			if(lrd_val == g_cds->GetI2CValue())
				printf("Camera Mode set Success!\n");
			else
				printf("Camera Mode set Failed!\n");
			sleep(1);//不要取消哦
			return MS_SUCCESS;
		}

		return MS_ERROR;
	}

	//�ر����
	void Camera::CloseCamera()
	{
		g_cds->CloseCamera();
	}

	int Camera::GetImageData(unsigned char * &data, int &len)
	{
		return g_cds->GetImageData(data,len);;
	}

	//lw add
	void Camera::SendAddr(int addr){
		g_cds->SendI2CAddr(addr);
	}
	void Camera::SetValue(int val){
		g_cds->SetI2CValue(val);
	}
	unsigned char Camera::GetValue(){
		return g_cds->GetI2CValue();
	}
	void Camera::I2CWrite(int addr,int val){
		g_cds->SendI2CAddr(addr);
		usleep(100);
		g_cds->SetI2CValue(val);
	}
	unsigned char Camera::I2CRead(int addr){
		g_cds->SendI2CAddr(addr);
		usleep(100);
		return g_cds->GetI2CValue();
	}
	//lw add end

	void Camera::SetCameraMode(CameraMode cameraMode)
	{
		if(m_mode!=cameraMode)
		{
			m_mode = cameraMode;
			int w = 320,h = 240;
			CAMERA_FPS fps = CAMERA_FPS_30;
			switch(m_mode)
			{

			case D_VPU_320X240_LR_30FPS:
				w = 320;
				h = 240;
				fps = CAMERA_FPS_30;
				break;

			case D_VPU_320X240_LD_30FPS:
				w = 320;
				h = 240;
				fps = CAMERA_FPS_30;
				break;

			case D_VPU_320X240_LRD_30FPS:
				w = 320;
				h = 360;
				fps = CAMERA_FPS_30;
				break;

			default:break;
			}
			delete g_cds;
			g_cds = new VL4_IF(w,h,fps);
		}
	}
	CameraMode Camera::GetCameraMode()
	{
		return m_mode;
	}

}
