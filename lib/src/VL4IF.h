/*
 * VL4IF.h
 *
 *  Created on: May 5, 2016
 *      Author: renyin
 */

#ifndef SRC_VL4IF_H_
#define SRC_VL4IF_H_
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>		/* getopt_long() */

#include <fcntl.h>		/* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>		/* for videodev2.h */

#include <linux/videodev2.h>
#include <linux/v4l2-controls.h>
using namespace std;


#define CLEAR(x) memset (&(x), 0, sizeof (x))

enum CAMERA_FPS
{
	CAMERA_FPS_30,
};

struct buffer
{
  void *start;
  size_t length;
};

class VL4_IF {
public:
	VL4_IF(int w,int h, CAMERA_FPS fps);
	virtual ~VL4_IF();

	//return : null : no device found not null: device name
	char* ScanCamera(const string &vid, const string &pid);

	bool OpenCamera(string dev_name);
	void CloseCamera();

	bool SetCameraParas(int w, int h, CAMERA_FPS fps);

	int GetImageData(unsigned char * &data, int &len);

	//lw add
	void SendI2CAddr(int addr);
	void SetI2CValue(int val);
	unsigned char GetI2CValue();
	//lw add end

	bool m_camera_opened;
	string m_dev_name;
	CAMERA_FPS m_fps;
	int nWidth;
	int nHeight;

	//V4L related
	int m_dev_fd;
private:

	struct buffer *buffers;
	int m_buf_cnt;

	bool open_device(string dev_name);
	bool init_uvcdev();

	int xioctl(int fd, int request, void *arg);

	bool init_mmap(void);

	bool start_capturing(void);

	bool stop_capturing(void);

	bool uninit_device(void);

	bool close_device(void);

	int read_frame(struct v4l2_buffer &buf);

	unsigned int GetFPSCount(CAMERA_FPS fps)
		{
			int val = 30;
			switch(fps)
			{
			case CAMERA_FPS_30:
				val = 30;break;
			default:
				break;
			}
			return val;
		}
};

#endif /* SRC_VL4IF_H_ */
