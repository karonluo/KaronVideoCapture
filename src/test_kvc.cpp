#include "KaronVideoCapture.hpp"
//#define USEOPENCV
int main(int args, char **argv)
{
	// std::string rtsp1 = "rtsp://14.29.111.101:558/82828937-1023-4557-8867-1DABBAE13D79.sdp";
	// std::string rtsp2 = "rtsp://admin:123456@192.168.1.211";
	std::string video_url = std::string(argv[1]);
#if USEOPENCV
	cv::VideoCapture vc(video_url, cv::CAP_FFMPEG);
	if (vc.isOpened())
	{

		cv::Mat frame;
		for (;;)
		{
			vc >> frame;
			if (!frame.empty())
			{
				cv::imshow("test", frame);
			}
			int key = cv::waitKey(25);
			if (key == 27)
			{
				break;
			}
		}
	}
#else

	KaronVideoCapture kvc(video_url, 1920, 1080);
	int fps = 0;
	cv::Mat mat;
	if (kvc.IsOpened())
	{
		for (;;)
		{
			try
			{

				mat = kvc.GetVideoMat(50);
				if (!mat.empty())
				{
					cv::imshow("test", mat);
					// cv::imwrite("./test.jpg", mat);
					// printf("Video OK>>\r\n");
					// printf("fps: %d\r\n", kvc.GetVideoFps());
				}
				int key = cv::waitKey(25);
				if (key == 27)
				{
					break;
				}
			}
			catch (const char *msg)
			{
				printf("%s\r\n", msg);
			}
		}
	}
#endif
	return 0;
}
