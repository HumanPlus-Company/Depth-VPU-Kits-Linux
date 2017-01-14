Linux系统：


1、首先安装以下依赖库：
sudo apt-get install libusb-1.0-0-dev libusb-dev libudev-dev libv4l-dev



2、然后终端进入当前文件夹(lib)下，键入以下命令：
g++ -I/usr/local/include -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib -o"MoveSense_Dvpu" ./src/Camera.cpp ./src/MoveSenseCamera.cpp ./src/VL4IF.cpp ./src/Sample_Cap_OpenCV.cpp -lusb-1.0 -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_calib3d -lopencv_contrib -lopencv_features2d -lopencv_flann -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_video -ludev -lusb


3、执行完后会在lib文件夹下，生成一个名为"MoveSense_Dvpu"的可执行程序，运行即可。
