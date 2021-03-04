#include "KaronVideoCapture.hpp"

using namespace cv;

/*
Author: Karon.Luo
Date: 20210301
Dependencies: vaapi\ffmpeg\opencv4.2.0\live555\vlc3.0+
*/

void *KaronVideoCapture::lock(void *opaque, void **planes)
{
	struct context *ctx = (context *)(opaque);
	ctx->mtx.lock();
	*planes = ctx->pixels;

	return NULL;
}
void KaronVideoCapture::unlock(void *opaque, void *picture, void *const *planes)
{
	// CV_UNUSED(picture);
	struct context *ctx = (context *)(opaque);
	unsigned char *data = (unsigned char *)(*planes); // (unsigned char *)*planes;
	if (data != nullptr)
	{
		try
		{
			ctx->frame = cv::Mat(cv::Size(ctx->video_width, ctx->video_height), CV_8UC4, data);

			/*
		if (ctx->mat_buffer.size() < MAT_BUFFER_SIZE)
		{
			ctx->mat_buffer.push_back(ctx->frame);
		}
		else
		{
			ctx->mat_buffer.pop_back();
		}
		*/
		}
		catch (const char *errmsg)
		{
			printf("Convert to OpenCV Mat object was error: %s\r\n", errmsg);
		}
	}

	ctx->mtx.unlock();
}
void KaronVideoCapture::show(void *opaque, void *picture)
{
	// CV_UNUSED(picture);
	// (void)opaque;
}
KaronVideoCapture::KaronVideoCapture(std::string VideoUrl, int VideoWidth, int VideoHeight)
{
	printf("Karon Video Capture Version: 1.0\r\n");
	printf("Video and system information\r\n");
	printf("========================================\r\n");
	printf("OpenCV Version: %s\r\n", cv::getVersionString().c_str());
	printf("VideoLan Version: %s\r\n", libvlc_get_version());
	ctx.video_url = VideoUrl;
	ctx.video_width = VideoWidth;
	ctx.video_height = VideoHeight;
	inst = NULL;
	player = NULL;
	media = NULL;
	/*
	const char *const vlc_args[] =
		{
			"-I",
			"dummy",
			"--ignore-config",
			"--network-caching=60000",
			"--no-audio",
			"--rtp-timeout=32767",
			"--packetizer-mpegvideo-sync-iframe",
			"--rtsp-frame-buffer-size=2147483647",
			"--no-drop-late-frames",
			"--no-skip-frames",
			"--codec=eacad32,hevc",
			"--verbose=2"};
	*/

	// std::string s_buff_length =  buffer_length
	const char *const vlc_args[] =
		{
			"--no-audio",
			"--network-caching=4000",
			"-I", "dummy",
			"--ignore-config",
			"--loop"};

	inst = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
	if (NULL != inst)
	{
		ctx.pixels = new uchar[ctx.video_width * ctx.video_height * 4];
		memset(ctx.pixels, 0, ctx.video_width * ctx.video_height * 4);
		player = libvlc_media_player_new(inst);
		if (ctx.video_url.find("rtsp://") != -1 || ctx.video_url.find("rtmp://") != -1)
		{
			media = libvlc_media_new_location(inst, ctx.video_url.c_str());
		}
		else
		{
			media = libvlc_media_new_path(inst, ctx.video_url.c_str());
		}
		if (NULL != media)
		{
			// Set media option if libvlc version >= 3.0 .

			libvlc_media_add_option(media, ":avcodec-hw=vaapi");
			libvlc_media_add_option(media, ":I");
			libvlc_media_add_option(media, ":dummy");
			libvlc_media_add_option(media, ":network-caching=4000");
			libvlc_media_add_option(media, ":ignore-config");
			libvlc_media_add_option(media, ":no-audio");
			libvlc_media_add_option(media, ":loop");
			libvlc_media_parse(media);
			libvlc_media_player_set_media(player, media);
			libvlc_media_release(media);
			libvlc_video_set_callbacks(player, lock, unlock, show, &ctx);
			libvlc_video_set_format(player, "RGBA", ctx.video_width, ctx.video_height, ctx.video_width * 4);
			libvlc_media_player_play(player);
			usleep(1000000);
			printf("Video Url: %s\r\n", VideoUrl.c_str());
			if (!IsOpened())
			{
				printf("Can not open video, please check video url.\r\n");
			}
			else
			{
				// Must call IsOpended() if want get video fps.
				printf("Video Size: %d x %d \r\n", VideoWidth, VideoHeight);
				printf("Video FPS: %d\r\n", GetVideoFps());
			}
		}
	}
	else
	{
		printf("Can not initalization!\r\n");
	}
	printf("========================================\r\n");
}
int KaronVideoCapture::GetVideoWidth()
{
	return ctx.video_width;
}
int KaronVideoCapture::GetVideoHeight()
{
	return ctx.video_height;
}
int KaronVideoCapture::GetVideoFps()
{
	return (int)libvlc_media_player_get_fps(player);
}
cv::Mat KaronVideoCapture::GetVideoMat(int RetryCount)
{
	/*
	if (ctx.mat_buffer.size() >= MAT_BUFFER_SIZE)
	{
		cur_frame = ctx.mat_buffer.front();
		ctx.mat_buffer.pop_back();
		cv::cvtColor(cur_frame, cur_frame, cv::COLOR_RGB2BGR);
	}
	*/
	int icount = 0;
	for (;;)
	{
		usleep(25000);
		if (!ctx.frame.empty())
		{
			cur_frame = ctx.frame;
			cv::cvtColor(cur_frame, cur_frame, cv::COLOR_RGB2BGR);
			ctx.frame.release();
			break;
		}
		else
		{
			icount++;
			if (icount > RetryCount)
			{
				throw "Error: Can not get video mat!\r\n";
			}
		}
	}
	return cur_frame;
	//return ctx.frame;
}
bool KaronVideoCapture::IsOpened(int RetryCount)
{
	bool result = false;
	for (int i = 0; i < RetryCount; i++)
	{
		usleep(25000);
		if (!ctx.frame.empty())
		{
			result = true;
			break;
		}
	}
	return result;
}
void KaronVideoCapture::Release()
{
	if (!ctx.is_release)
	{
		printf("KaronVideoCapture release!\r\n");
		libvlc_media_player_stop(player);
		libvlc_media_player_release(player);
		libvlc_release(inst);
		ctx.frame.release();
		/*
			if (ctx.mat_buffer.size() > 0)
			{
				ctx.mat_buffer.swap(ctx.mat_buffer);
				ctx.mat_buffer.clear();
			}
		*/
		if (NULL != ctx.pixels)
		{
			delete ctx.pixels;
			ctx.pixels = NULL;
		}
		ctx.is_release = true;
	}
}
KaronVideoCapture::~KaronVideoCapture()
{
	Release();
}
