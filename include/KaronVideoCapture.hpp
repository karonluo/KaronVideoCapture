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

/**
 * @brief Karon Video Capture 用于获取视频帧数据，基于 VideoLan\OpenCV\FFMpeg\Live555 开源项目
 * @author Karon.Luo kluo@edgevision.ai
 * @date 2021-03-01
 * @version 1.0
 * @note Dependencies: vaapi\ffmpeg\opencv\live555\vlc3.0+
 */
class KaronVideoCapture
{
public:

	/**
	 * @brief 构造一个新的 Karon Video Capture 对象
	 * @param VideoUrl 视频源地址 文件(video file address)或者地址（rtsp) 
	 * @param VideoWidth 视频源宽度
	 * @param VideoHeigth 视频源高度
	 */
	KaronVideoCapture(std::string VideoUrl, int VideoWidth, int VideoHeigth);
	
	/**
	 * @brief 构造一个新的 Karon Video Capture 对象
	 * 
	 */
	KaronVideoCapture();

	
	/**
	 * @brief 获取视频宽度
	 * 
	 * @return int
	 */
	int GetVideoWidth();
	
	/**
	 * @brief 获取视频高度
	 * @return int
	 */
	int GetVideoHeight();
	
	/** 
	* @brief 获取视频源的帧数
	* @return int
	*/
	int GetVideoFps();
	
	/**
	 * @brief 获取视频 OpenCV Mat 对象
	 * 
	 * @param RetryCount 重试次数 
	 * @return cv::Mat OpenCV Mat 对象
	 */
	cv::Mat GetVideoMat(int RetryCount = 1000);


	/**
	 * @brief 判断视频源是否正常打开
	 * 
	 * @param RetryCount 重试次数
	 * @return true 
	 * @return false 
	 */
	bool IsOpened(int RetryCount = 1000);
	/**
	 * @brief 释放对象，手动调用析构函数 
	 * 
	 */
	void Release();
	/**
	 * @brief 重新打开视频源
	 * 
	 */
	void Reopen();

	/**
	 * @brief 打开指定视频源
	 * 
	 * @param VideoUrl 视频源地址，可以是文件路径也可以是 RTSP 协议路径
	 * @param VideoWidth 指定视频源宽度
	 * @param VideoHeight 指定视频源高度
	 * @param IsReopen 本次打开是否是重新打开
	 * @note 如果 IsReopen 为 true, 则相当于执行 Reopen 方法, 视频源的高宽设置决定内存分配的大小一定需要根据视频源的高宽进行指定。
	 * 
	 */
	void Open(std::string VideoUrl, int VideoWidth, int VideoHeight,bool IsReopen = false);

	/**
	 * @brief 析构函数，销毁 Karon Video Capture 对象
	 * 
	 */
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
