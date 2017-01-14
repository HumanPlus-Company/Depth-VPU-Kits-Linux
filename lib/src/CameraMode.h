#pragma once

namespace movesense
{
	typedef unsigned char CameraMode;

	#define	D_VPU_320X240_LR_30FPS 220
	#define	D_VPU_320X240_LD_30FPS 219
	#define	D_VPU_320X240_LRD_30FPS 218
}

struct MsgPkg{
	unsigned char type;
	unsigned char len;
	unsigned char *data;
	float *fdata;
};
