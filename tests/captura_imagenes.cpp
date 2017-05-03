#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <raspicam/raspicam_cv.h>
#include <ctime>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <thread>
#include <unistd.h>
#include <cstring>
#include <time.h>
#include <sstream>


using namespace std;
using namespace cv;
using namespace raspicam;

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

//cv::VideoCapture video;
raspicam::RaspiCam_Cv video;
cv::Mat bgrMap;

int numSnapshot = 0;
  std::string id = "0";

int segundos;

time_t rawtime;
struct tm * timeinfo;

std::string hora;
char key = 0;

vector<Mat> images;
vector<float> times;

const int max_exposure = 100;
const int max_iso = 100;
int exposure;
int iso;

void trackbar_exposure(int, void*);
void trackbar_iso(int, void*);

void setExposure(float exposure) {

	video.set(CV_CAP_PROP_EXPOSURE, exposure);
	video.grab();
	cout << "Exposicion " << video.get(CV_CAP_PROP_EXPOSURE) << endl;
}

void setISO(float iso){
	video.set(CV_CAP_PROP_GAIN, iso);
	video.grab();
	cout << "ISO" << video.get(CV_CAP_PROP_GAIN) << endl;
}



void captura(){


    while(key != 27){

	sleep(segundos);

    std::cout <<  "Hora: " <<hora << std::endl;



	setExposure(0);
	setISO(2);
	sleep(2);
	video.grab();
	video.retrieve(bgrMap);
	imwrite("images/" + id + "-" + hora + "_1.png", bgrMap);


	setExposure(0);
	setISO(7);
	sleep(2);
	video.grab();
	video.retrieve(bgrMap);
	imwrite("images/" + id + "-" + hora + "_2.png", bgrMap);


	setExposure(0);
	setISO(10);
	sleep(2);
	video.grab();
	video.retrieve(bgrMap);
	imwrite("images/" + id + "-" + hora + "_3.png", bgrMap);

    numSnapshot++;
    id = static_cast<std::ostringstream*>(&(std::ostringstream() << numSnapshot))->str();



    //sleep(segundos);

    }

}


int graba(){
    std::cout << "'Esc'  para salir" << std::endl;

	video.set ( CV_CAP_PROP_FRAME_WIDTH, 1920 );
    video.set ( CV_CAP_PROP_FRAME_HEIGHT, 1080 );
    video.set ( CV_CAP_PROP_FPS, 30);
    cout << video.get ( CV_CAP_PROP_GAIN) << endl;
    cout << video.get ( CV_CAP_PROP_EXPOSURE) << endl;


    if (!video.open()) {
        std::cerr<<"No se puede abrir la camera"<<std::endl;
        exit(-1);
    }


    createTrackbar("Exposure","IMG",&exposure, max_exposure, trackbar_exposure);
    createTrackbar("iso","IMG",&iso, max_iso, trackbar_iso);

    exposure = 0;
	iso = 0;

    sleep(1);

    do {

        time(&rawtime);
        timeinfo = localtime(&rawtime);

        hora = "";
        for(int i=11; i<19; i++){
          hora += asctime(timeinfo)[i];
        }


	//cv::cvtColor(bgrMap, bgrMap, CV_BGR2RGB);

	video.grab();


        video.retrieve(bgrMap);



	//cv::cvtColor(bgrMap, bgrMap, CV_BGR2RGB);

	//if(key == 'c'){
	//cout << "entra" << endl;

		//captura();

	//}



        //trackbar_iso(iso,0);
		//trackbar_exposure(exposure,0);
        cv::imshow("Video", bgrMap);



        key=cv::waitKey(20);



    }while(key!=27 && video.grab());

    video.release();



}



int main(int argc, char* argv[]){



    segundos = atoi(argv[1]);
    //namedWindow("IMG", CV_WINDOW_NORMAL);


    std::thread foto_(captura);
    std::thread video_(graba);



    video_.join();
    foto_.join();




}


void trackbar_exposure(int, void*){
		video.set(CV_CAP_PROP_EXPOSURE, exposure);
		imshow("IMG", bgrMap);
		//cout << "Exposure: " << video.get ( CV_CAP_PROP_EXPOSURE) << endl;
}

void trackbar_iso(int, void*){
		video.set(CV_CAP_PROP_GAIN, iso);
		imshow("IMG", bgrMap);
		//cout << "ISO: " << video.get ( CV_CAP_PROP_GAIN) << endl;
}
