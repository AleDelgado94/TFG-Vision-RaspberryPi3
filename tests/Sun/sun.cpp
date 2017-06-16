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


using namespace std;
using namespace cv;
//using namespace raspicam;

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

int umbral_bajo = 0;
int const umbral_alto = 255;
int ratio_ = 3;
int kernel_size = 3;
int umbral;


int menor_vector(vector<float> soles_radio){
    float menor = soles_radio[0];
    int retorno;
    for(int k=0; k<soles_radio.size(); k++){
        if(soles_radio[k] < menor){
            menor = soles_radio[k];
            retorno = k;
        }
    }
    return retorno;
}

void detecta_sun(Mat img, int umbral_bajo){

    vector<float> soles_radio;
    vector<Point2f> soles_center;

    //BILATERAL FILTER
    Mat bilateralFilterImg;
    bilateralFilter(img, bilateralFilterImg, 5, 175, 175);
    imshow("bilateralFilter", bilateralFilterImg);
    //EDGE DETECTION
    Mat edge;
    cvtColor(bilateralFilterImg, bilateralFilterImg, CV_RGB2GRAY);
    threshold(bilateralFilterImg, edge, umbral_bajo, umbral_alto, 0);
    imshow("threshold", edge);

    //COUNTOURS
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(edge, contours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector<vector<Point>> contours_poly(contours.size());
    vector<Point2f> center(contours.size());
    vector<float> radius(contours.size());

    for (int i = 0; i < contours.size(); i++)
    {
            
            approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
            minEnclosingCircle((Mat)contours_poly[i], center[i], radius[i]);
            double area = (3.14159265359 * pow(radius[i],2));
  cout << radius[i] << endl;
            if (radius[i] > 40 && radius[i] < 100) {
              //      cout << radius[i] << endl;
                    soles_radio.push_back(radius[i]);
                    soles_center.push_back(center[i]);
            }
    }

    if(soles_radio.size() > 1){

        int i, proximo;
        auto centro = soles_center.front();
        auto radio = soles_radio.front();


        //BUSQUEDA DEL ELEMENTO MÁS PROXIMO A 200

        int num_aproximar = 80;

        vector<float> diferencia;
        for(int j=0; j<soles_radio.size(); j++){
            diferencia.push_back(abs(num_aproximar-soles_radio[j]));
        }

        int index = menor_vector(diferencia);

        centro = soles_center[index];
        radio = soles_radio[index];

        circle(img, centro, (int)radio, Scalar(0, 0, 255), 2, 8, 0);
        circle(img, centro, 5, Scalar(255, 0, 0), 2, 8, 0);



    }else if(soles_radio.size() == 1){
        auto radio = soles_radio.front();
        auto centro = soles_center.front();
        soles_radio.clear();
        soles_center.clear();
        circle(img, centro, (int)radio, Scalar(0, 0, 255), 2, 8, 0);
        circle(img, centro, 5, Scalar(255, 0, 0), 2, 8, 0);
    }

    imshow("IMG", img);
}

int main(int argc, char *argv[])
{
    namedWindow("IMG", CV_WINDOW_NORMAL);
    umbral_bajo=235;

    cout << "Umbral: " << umbral_bajo << endl << endl;


    if(argv[1] != ""){
        string ruta_img(argv[1]);

        Mat img = imread(ruta_img.c_str());
        detecta_sun(img, umbral_bajo);
    }
    else{
        return -1;
    }

    waitKey(0);
    return 0;
}
