#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctime>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <thread>
#include <unistd.h>
#include <cstring>
#include <time.h>
#include <sstream>
#include <dirent.h>
#include <sys/stat.h>
#include <cmath>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>
#include <algorithm>
#include <regex>


using namespace std;
using namespace cv;




int main(int argc, char const *argv[]) {
  Mat mask = Mat::ones(1024,768,CV_8UC3);
  Mat img = imread(argv[1]);

  Mat roi1(img, Rect(0,0,128,64));
  imshow("ventana1", roi1);
  Mat roi2(img, Rect(128,0,128,64));
  imshow("ventana2", roi2);
  Mat roi3(img, Rect(256,0,128,64));
  imshow("ventana3", roi3);
  Mat roi4(img, Rect(384,0,128,64));
  imshow("ventana4", roi4);
  Mat roi5(img, Rect(512,0,128,64));
  imshow("ventana5", roi5);
  Mat roi6(img, Rect(640,0,128,64));
  imshow("ventana6", roi6);
  Mat roi7(img, Rect(768,0,128,64));
  imshow("ventana7", roi7);
  Mat roi8(img, Rect(896,0,128,64));
  imshow("ventana8", roi8);


  waitKey(0);
  return 0;
}
