#!/bin/bash
opencv_flags=`pkg-config /home/yt/opencv4.2/opencv.pc --libs --cflags`

echo "Build KaronVideoCapture ..."
g++ ./src/KaronVideoCapture.cpp -o ./libKaronVideoCapture.so.4.2.0 -O3 ${opencv_flags} -I./include \
-lvlc -fPIC -std=c++11 -shared
rm -rf ./libKaronVideoCapture.so
ln -s ./libKaronVideoCapture.so.4.2.0 ./libKaronVideoCapture.so
sleep 2
echo "Build Test Main ..."
g++ ./src/test_vc.cpp  -o ./test_vc -O3 -std=c++11 ${opencv_flags} -I./include -L./ \
-lvlc -lKaronVideoCapture \
-D USEOPENCV=0