## Karon Video Capture

用于抓取视频帧，支持 rtsp\rtmp\video file 等

作者： Karon Luo

日期：2021/03/02

依赖：

vlc3.0+、 intel media sdk、vaapi(libva)、ffmpeg、opencv

编译命令：

~~~shell
./build.sh
~~~

其中编译完成后生成：

libKaronVideoCapture.so.4.2.0 (4.2.0 根据 opencv 版本 编译）

如果需要关联新的 OpenCV 版本进行编译请修改编译脚本 build.sh 文件

test_kvc (用于测试抓取视频的功能）

测试命令：

~~~shell
./test_kvc <rtsp address|video file>
~~~
