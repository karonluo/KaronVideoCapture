## Karon Video Capture API

### 描述

用于抓取视频帧，支持 rtsp\rtmp\video file 等协议格式

支持 hevc(h265)\h264 硬件解码

### 硬件解码支持 

Intel 核心显卡\VAAPI(libva) 

### 作者

成都云图睿视科技有限公司

Karon Luo

### 编译命令

~~~shell
./build.sh
~~~

编译完成后生成：libKaronVideoCapture.so.4.2.0 (4.2.0 根据 opencv 版本 编译）和 test_kvc (用于测试抓取视频的功能）

如果需要关联其他 OpenCV 版本进行编译请修改编译脚本 build.sh 文件

### 测试命令

~~~shell
./test_kvc <rtsp address|video file>
~~~

### 测试环境信息

#### 操作系统

Ubuntu 16.04 

#### SDK 及软件依赖项：

1. vlc3.0+(libvlc 3.0+)
2. intel media sdk（msdk）
3. vaapi(libva2.6.0)
4. ffmpeg
5. opencv（opencv4.2.0 测试通过，也可以使用其他版本的 OpenCV 但需要重新编译）

#### 硬件参数：

> CPU: Intel(R) Celeron(R) CPU N3450 @ 1.10GHz

> GPU: Intel(R) HD Graphics 500

#### 执行 vainfo 的结果

~~~shell
libva info: VA-API version 1.6.0
libva info: User environment variable requested driver 'iHD'
libva info: Trying to open /usr/lib/x86_64-linux-gnu/dri/iHD_drv_video.so
libva info: Found init function __vaDriverInit_1_6
libva info: va_openDriver() returns 0
vainfo: VA-API version: 1.6 (libva 2.6.0)
vainfo: Driver version: Intel iHD driver - 19.4.0
vainfo: Supported profile and entrypoints
      VAProfileNone                   :	VAEntrypointVideoProc
      VAProfileNone                   :	VAEntrypointStats
      VAProfileMPEG2Simple            :	VAEntrypointVLD
      VAProfileMPEG2Main              :	VAEntrypointVLD
      VAProfileH264Main               :	VAEntrypointVLD
      VAProfileH264Main               :	VAEntrypointEncSlice
      VAProfileH264Main               :	VAEntrypointFEI
      VAProfileH264Main               :	VAEntrypointEncSliceLP
      VAProfileH264High               :	VAEntrypointVLD
      VAProfileH264High               :	VAEntrypointEncSlice
      VAProfileH264High               :	VAEntrypointFEI
      VAProfileH264High               :	VAEntrypointEncSliceLP
      VAProfileVC1Simple              :	VAEntrypointVLD
      VAProfileVC1Main                :	VAEntrypointVLD
      VAProfileVC1Advanced            :	VAEntrypointVLD
      VAProfileJPEGBaseline           :	VAEntrypointVLD
      VAProfileJPEGBaseline           :	VAEntrypointEncPicture
      VAProfileH264ConstrainedBaseline:	VAEntrypointVLD
      VAProfileH264ConstrainedBaseline:	VAEntrypointEncSlice
      VAProfileH264ConstrainedBaseline:	VAEntrypointFEI
      VAProfileH264ConstrainedBaseline:	VAEntrypointEncSliceLP
      VAProfileVP8Version0_3          :	VAEntrypointVLD
      VAProfileHEVCMain               :	VAEntrypointVLD
      VAProfileHEVCMain               :	VAEntrypointEncSlice
      VAProfileHEVCMain               :	VAEntrypointFEI
      VAProfileHEVCMain10             :	VAEntrypointVLD
      VAProfileVP9Profile0            :	VAEntrypointVLD
~~~
