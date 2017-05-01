#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <raspicam/raspicam_cv.h>
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


using namespace std;
using namespace cv;
//using namespace raspicam;

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)



vector<string> leer_imagenes(const string& ruta_imagenes)
{
    vector<string> imagenes;
    dirent* img;
    DIR* directorio;
    directorio = opendir(ruta_imagenes.c_str());
    if (directorio)
    {
        img = readdir(directorio);
        while (img != nullptr)
      {
      imagenes.push_back(string(img->d_name));
      img = readdir(directorio);
      }
        closedir(directorio);
        //ordenamos el vector con los nombres
        sort( imagenes.begin(), imagenes.end() );
    }
    return imagenes;
}


void genera_hdr(int i, vector<string> imgs){

    vector<Mat> images;
    vector<float> times;

    float val;
    //while(i<imgs.size()){



        for(int k=i; k<(i+3); k++){
            val = pow(2, (k*(-1)));
            Mat img = imread("images/"+imgs[k]);
            imshow("img",img);
            images.push_back(img);
            times.push_back(1/val);
        }

        //MAT RESPONSE
        Mat response;
        Ptr<CalibrateDebevec> calibrate = createCalibrateDebevec();
        calibrate->process(images, response, times);

        //GENERAMOS LA IMAGEN HDR
        Mat hdr;
        Ptr<MergeDebevec> merge_debevec = createMergeDebevec();
        merge_debevec->process(images, hdr, times, response);

        /*Mat ldr;
        Ptr<TonemapDurand> tonemap = createTonemapDurand(2.2f);
        tonemap->process(hdr, ldr);*/

        Mat fusion;
        Ptr<MergeMertens> merge_mertens = createMergeMertens();
        merge_mertens->process(images, fusion);


        imwrite("images_hdr/"+imgs[i], fusion * 255);


        //i = i+3;
        cout << i << endl;

    //}
}



int main(int argc, char *argv[])
{

    vector<string> imgs = leer_imagenes("images");
    cout << "SIZE: " << imgs.size() << endl;

    for(int i=2; i<imgs.size(); i=i+3)
        genera_hdr(i,imgs);





    //waitKey(0);
    return 0;
}
