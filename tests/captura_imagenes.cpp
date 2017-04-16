#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <ctime>
#include <cstdio>
#include <cstdlib>

#include <thread>

#include <unistd.h>

#include <cstring>

#include <time.h>

#include <sstream>


using namespace std;
using namespace cv;

cv::VideoCapture video;
cv::Mat bgrMap(1080,1920,CV_8UC4);

int numSnapshot = 0;
  std::string id = "0";

int segundos;

time_t rawtime;
struct tm * timeinfo;

std::string hora;
char key = 0;




void captura(){


    while(key != 27){

    std::cout << hora << std::endl;

    //cv::cvtColor(bgrMap, bgrMap, CV_BGR2RGB);

	//video >> bgrMap;

	//cv::cvtColor(bgrMap, bgrMap, CV_BGR2RGB);
        
        

        //cv::imshow("Video", bgrMap);

    cv::imwrite("images/" + id + "-" + hora + ".png", bgrMap);
    numSnapshot++;
    id = static_cast<std::ostringstream*>(&(std::ostringstream() << numSnapshot))->str();

    sleep(segundos);

    }

}


int graba(){
    std::cout << "'Esc' para salir" << std::endl;

    video.open(0);

    if (!video.isOpened()) {
        std::cerr<<"No se puede abrir la camera"<<std::endl;
        exit(-1);
    }

    while (key!=27 && video.grab()) {

        time(&rawtime);
        timeinfo = localtime(&rawtime);

        hora = "";
        for(int i=11; i<19; i++){
          hora += asctime(timeinfo)[i];
        }
        
        
	//cv::cvtColor(bgrMap, bgrMap, CV_BGR2RGB);

	video.grab();
        video.retrieve(bgrMap);

	//video >> bgrMap;

	cv::cvtColor(bgrMap, bgrMap, CV_BGR2RGB);
        
        

        cv::imshow("Video", bgrMap);


        key=cv::waitKey(20);


    }



}



int main(int argc, char* argv[]){



    segundos = atoi(argv[1]);

    std::thread foto(captura);
    std::thread video(graba);



    video.join();
    foto.join();
    
    


}
