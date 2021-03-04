#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <csignal>
#include <unistd.h>
#include <mutex>
#include <thread>
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/core.hpp"
#include "vlc/vlc.h"
#include <vector>

#define MAT_BUFFER_SIZE 2

/*
Karon Video Capture
Version: 1.0
Author: Karon Luo
Date: 2021-03-01
Dependencies: vaapi\ffmpeg\opencv\live555\vlc3.0+
*/

class KaronVideoCapture
{
public:
	/*
		VideoUrl, example: rtsp://username:password@ip or /home/download/video.mp4
		Width, example: 1920
		Height, example: 1080
	*/
	KaronVideoCapture(std::string VideoUrl, int Width, int Heigth);
	int GetVideoWidth();
	int GetVideoHeight();
	int GetVideoFps();
	/*
			Get OpenCV Mat object 
			defalut retry times is 1000
	*/
	cv::Mat GetVideoMat(int RetryCount = 1000);
	bool IsOpened(int RetryCount = 1000);
	void Release();
	~KaronVideoCapture();

private:
	static void *lock(void *opaque, void **planes);
	static void unlock(void *opaque, void *picture, void *const *planes);
	static void show(void *opaque, void *picture);
	libvlc_instance_t *inst = NULL;
	libvlc_media_player_t *player = NULL;
	libvlc_media_t *media = NULL;
	cv::Mat cur_frame;
	struct context
	{
		uchar *pixels;
		std::mutex mtx;
		cv::Mat frame;
		int video_width, video_height;
		float video_fps;
		std::string video_url;
		std::vector<cv::Mat> mat_buffer;
		bool is_release = false;
	} ctx;
};