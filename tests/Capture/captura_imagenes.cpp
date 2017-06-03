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
#include <boost/program_options/options_description.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>
#include "../Arduino/Temperature/temperature.h"


using namespace std;
using namespace cv;
using namespace raspicam;
namespace opt = boost::program_options;

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

const float max_exposure = 200;
const float max_iso = 1000;
float exposure;
float iso;
string ruta_carpeta_destino;

void trackbar_exposure(float, void*);
void trackbar_iso(float, void*);

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

	
	//setExposure(-8);
	setISO(2);
	sleep(2); 
	video.grab();
	video.retrieve(bgrMap);
	imwrite(ruta_carpeta_destino.c_str() + id + "-" + hora + "_1.png", bgrMap);
	
	
	//setExposure(5);
	setISO(7);
	sleep(2);
	video.grab();
	video.retrieve(bgrMap);
	imwrite(ruta_carpeta_destino.c_str() + id + "-" + hora + "_2.png", bgrMap);
	
	
	//setExposure(10);
	setISO(10); 
	sleep(2);
	video.grab();
	video.retrieve(bgrMap);
	imwrite(ruta_carpeta_destino.c_str() + id + "-" + hora + "_3.png", bgrMap);
        
        


    //cv::imwrite(ruta_carpeta_destino.c_str() + id + "-" + hora + ".png", bgrMap);
    numSnapshot++;
    id = static_cast<std::ostringstream*>(&(std::ostringstream() << numSnapshot))->str();



    //sleep(segundos);

    }

}


int graba(){
    std::cout << "'Esc'  para salir" << std::endl;

	video.set ( CV_CAP_PROP_FRAME_WIDTH, 1024 );
    video.set ( CV_CAP_PROP_FRAME_HEIGHT, 768 ); 
    //video.set ( CV_CAP_PROP_FPS, 30);
    cout << video.get ( CV_CAP_PROP_GAIN) << endl;
    cout << video.get ( CV_CAP_PROP_EXPOSURE) << endl;
    

    if (!video.open()) {
        std::cerr<<"No se puede abrir la camera"<<std::endl;
        exit(-1);
    }

	
    //createTrackbar("Exposure","IMG",&exposure, max_exposure, trackbar_exposure);
    //createTrackbar("iso","IMG",&iso, max_iso, trackbar_iso);
    
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

	/*if(key == 'c'){
		float ex;
		cout << "Introduce exposure" << endl;
		cin >> ex;
		setExposure(ex);

		//captura();

	}*/
		
		
        
        //trackbar_iso(iso,0);	
		//trackbar_exposure(exposure,0);
        cv::imshow("Video", bgrMap);



        key=cv::waitKey(20);



    }while(key!=27 && video.grab());
    
    video.release();


 
}



int main(int argc, char* argv[]){



   // segundos = atoi(argv[1]);
    

    opt::options_description desc("Options");
    desc.add_options()
    ("help,h", "./Temperature [[ --port | -p ] puerto]")
    ("segundos,s", opt::value<int>(&segundos)->default_value(20), "segundos")
    ("destino,d", opt::value<std::vector<std::string>>(), "carpeta de destino de las imagenes");

    opt::variables_map vm;
    opt::positional_options_description p;
    p.add("port", -1);
    store(opt::parse_command_line(argc, argv,desc), vm);
    notify(vm);

    if(argc == 1){
      cout << desc << endl;
      return -1;
    }

    if(vm.count("help")){
      cout << desc << endl;
      return 0;
    }
    if(vm.count("segundos")){
      segundos = vm["segundos"].as<int>();
    }
    if(vm.count("destino")){
      std::vector<std::string> v = vm["destino"].as<std::vector<std::string>>();
      ruta_carpeta_destino = v[0];
    }
    namedWindow("Video", CV_WINDOW_NORMAL);

    


    std::thread foto_(captura);
    std::thread video_(graba);

	
	
    video_.join();
    foto_.join();
    
    


}


void trackbar_exposure(float, void*){
		video.set(CV_CAP_PROP_EXPOSURE, exposure);
		imshow("IMG", bgrMap);
		cout << "Exposure: " << video.get ( CV_CAP_PROP_EXPOSURE) << endl;
}

void trackbar_iso(float, void*){
		video.set(CV_CAP_PROP_GAIN, iso);
		imshow("IMG", bgrMap);
		cout << "ISO: " << video.get ( CV_CAP_PROP_GAIN) << endl;
}
