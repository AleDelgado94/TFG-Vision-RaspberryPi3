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
#include <algorithm>
#include <regex>


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
        //sort( imagenes.begin(), imagenes.end() );
    }
    return imagenes;
}


int main(int argc, char *argv[])
{

    vector<string> imgs_name = leer_imagenes("images_hdr/");
    vector<string> imgs_name_sort(imgs_name.size()-2);

    for(int i=2; i<imgs_name.size(); i++){
        regex rgx("^[0-9]+");
        smatch match;
        string s = imgs_name[i];


        if(regex_search(s, match, rgx)){
            string number(match[0]);
            int n = atoi(number.c_str());

            imgs_name_sort[n] = s;
        }

    }


    namedWindow("IMG", CV_WINDOW_NORMAL);
    namedWindow("diff", CV_WINDOW_NORMAL);
   // namedWindow("original", CV_WINDOW_NORMAL);
    /*if(argv[1] != ""){
        string ruta_img(argv[1]);

        Mat img = imread(ruta_img.c_str());
        imshow("IMG", img);
    }
    else{
        return -1;
    }
*/

    vector<Mat> images;
    for(int i=0; i<200; i++){
        Mat im = imread("images_hdr/"+imgs_name_sort[i]);
        images.push_back(im);
    }

    //DIFERENCIA ENTRE LAS IMAGENES
    Mat img_diff;
    absdiff(images[100],images[101],img_diff);

    //ESCALA DE GRISES
    Mat gray;
    cvtColor(img_diff, gray, CV_RGB2GRAY);

    int ratio=3;
    int umbral = 50;
    //int kernel = 3;

    //Eliminar ruido
    Mat ruido;
    Mat kernel = Mat::ones(3,3,CV_32F)/(float)(9);
    morphologyEx(gray, ruido, MORPH_OPEN, kernel);


    //Edges
    Mat edges;
    //Canny(gray, edges, umbral, umbral*ratio, kernel);
    threshold(ruido, edges, umbral, 255, 0);
    //CONTORNOS
    Mat contours;
    vector<Vec4i> hierarchy;
    //findContours(edges, contours, hierarchy,  RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );




    imshow("diff", img_diff);
    imshow("IMG", edges);

    waitKey(0);
    return 0;
}
