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


void setExposure(float value) {

	float exposure = value;

	video.set(CV_CAP_PROP_EXPOSURE, exposure);
	cout << "Exposicion " << video.get(CV_CAP_PROP_EXPOSURE) << endl;
}


void captura(){


    //while(key != 27){

	//sleep(segundos);

    std::cout <<  "Hora: " <<hora << std::endl;
	

    //cv::cvtColor(bgrMap, bgrMap, CV_BGR2RGB);

	//video.retrieve(bgrMap);

	//cv::cvtColor(bgrMap, bgrMap, CV_BGR2RGB);
	
	setExposure(100);
	sleep(1);
	video.grab();
	video.retrieve(bgrMap);
	imwrite("images/1.png", bgrMap);
	
	
	setExposure(99);
	sleep(1);
	video.grab();
	video.retrieve(bgrMap);
	imwrite("images/2.png", bgrMap);
	
	
	setExposure(95);
	sleep(1);
	video.grab();
	video.retrieve(bgrMap);
	imwrite("images/3.png", bgrMap);
	
	
	setExposure(91);
	sleep(1);
	video.grab();
	video.retrieve(bgrMap);
	imwrite("images/4.png", bgrMap);
	video.grab();

	setExposure(88);
	sleep(1);
	video.grab();
	video.retrieve(bgrMap);
	imwrite("images/5.png", bgrMap);
	video.grab();

	setExposure(85);
	sleep(1);
	video.grab();
	video.retrieve(bgrMap);
	imwrite("images/6.png", bgrMap);
	video.grab();

	setExposure(82);
	sleep(1);
	video.grab();
	video.retrieve(bgrMap);
	imwrite("images/7.png", bgrMap);
	video.grab();

	setExposure(79);
	sleep(1);
	video.grab();
	video.retrieve(bgrMap);
	imwrite("images/8.png", bgrMap);
	video.grab(); 
	
 
	float val;
	for (size_t i = 1; i <= 8; i++)
	{
		String img_name = std::to_string(i) + std::string(".png");
		val = pow(2, (i*(-1)));
		
		Mat img = imread("images/"+img_name);
		images.push_back(img);
		times.push_back(1 / val);
	}


	//MAT RESPONSE
	Mat response;
	Ptr<CalibrateDebevec> calibrate = createCalibrateDebevec();
	calibrate->process(images, response, times);


	//GENERAMOS LA IMAGEN HDR
	Mat hdr;
	Ptr<MergeDebevec> merge_debevec = createMergeDebevec();
	merge_debevec->process(images, hdr, times, response);

	Mat ldr;
	Ptr<TonemapDurand> tonemap = createTonemapDurand(2.2f);
	tonemap->process(hdr, ldr);
	Mat fusion;
	Ptr<MergeMertens> merge_mertens = createMergeMertens();
	merge_mertens->process(images, fusion);

	//cvtColor(fusion, fusion, CV_BGR2RGB);
	imwrite("images/" + id + "-" + hora + ".png", fusion * 255);
	//imwrite("images/ldr.png", ldr * 255);
	//imwrite("images/hdr.hdr", hdr);
        
        

        //cv::imshow("Video", bgrMap);

    //cv::imwrite("images/" + id + "-" + hora + ".png", bgrMap);
    numSnapshot++;
    id = static_cast<std::ostringstream*>(&(std::ostringstream() << numSnapshot))->str();



    //sleep(segundos);

    //}

}


int graba(){
    std::cout << "'Esc'  para salir" << std::endl;

	video.set ( CV_CAP_PROP_FRAME_WIDTH, 1920 );
    video.set ( CV_CAP_PROP_FRAME_HEIGHT, 1080 ); 

    if (!video.open()) {
        std::cerr<<"No se puede abrir la camera"<<std::endl;
        exit(-1);
    }

    

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

	if(key == 'c'){
	cout << "entra" << endl;

		captura();

	}
        
        
	
        cv::imshow("Video", bgrMap);



        key=cv::waitKey(20);



    }while(key!=27 && video.grab());



}



int main(int argc, char* argv[]){



    segundos = atoi(argv[1]);

  //  std::thread foto_(captura);
    std::thread video_(graba);



    video_.join();
    //foto_.join();
    
    


}
