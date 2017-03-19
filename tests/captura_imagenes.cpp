#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <ctime>
#include <cstdio>
#include <cstdlib>

#include <unistd.h>

#include <cstring>

#include <time.h>

#include <sstream>


using namespace std;

cv::VideoCapture video;
cv::Mat bgrMap;

int main(int argc, char* argv[]){
  char key = 0;

  
  int segundos = atoi(argv[1]);
 
  

  int numSnapshot = 0;
  std::string id = "0";
  std::cout << "Press 'Esc' to exit" << std::endl;

  video.open(0);

  if (!video.isOpened()) {
    std::cerr<<"Could not open video"<<std::endl;
    return -1;
  }
  
  std::cout << segundos << std::endl;

  while (key!=27 && video.grab()) {
	
	time_t rawtime;
	struct tm * timeinfo;
	
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	
	
	
	std::string hora = "";
	  
	  
	  
    video.retrieve(bgrMap);

    cv::imshow("BGR image", bgrMap);
    
    
    

    //if (key == 27) { //CADA x (ARGV[0]) SECS SACAR FOTO
		
	  for(int i=11; i<19; i++){
			  hora += asctime(timeinfo)[i];
	  }
	  
	  std::cout << hora << std::endl;
		
      cv::imwrite(id + "-" + hora + ".png", bgrMap);
      numSnapshot++;
      id = static_cast<std::ostringstream*>(&(std::ostringstream() << numSnapshot))->str();
      
      
		  
	  
	  
     //}
     
     sleep(segundos);

  key=cv::waitKey(20);
  
  
  }
}
