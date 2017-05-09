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
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include <cmath>
#include <algorithm>
#include <regex>


using namespace std;
using namespace cv;

ofstream fichero;

void save_data(string ruta_fichero, string nombre_img, Point2i pi, Point2i pf, float dist, float angulo, int id){



    fichero.open(ruta_fichero.c_str(), ios::app);

    int pi_x = pi.x;
    int pi_y = pi.y;
    int pf_x = pf.x;
    int pf_y = pf.y;

    fichero << id << ": ("+ to_string(pi_x) +", "+ to_string(pi_y) +")    ("+ to_string(pf_x) +", "+ to_string(pf_y) +")    "+ to_string(dist) +"    "+ to_string(angulo) +"\n";
    fichero.close();


}


void inicializa_fichero(string ruta_fichero){
    fichero.open(ruta_fichero.c_str(), ios::app);
    fichero << "Id_img  (Pi.x, Pi.y)    (Pf.x, Pf.y)    Dist    Angle\n";
    fichero.close();
}

void save_end_img(string ruta_fichero){
    fichero.open(ruta_fichero.c_str(), ios::app);
    fichero << "\n";
    fichero.close();
}


void dibuja_CloudTracking(Mat img, const vector<Point2f>& img_ant_pts, const vector<Point2f>& img_actual_pts, const vector<unsigned char>& estado,
                          int id){
    for(int i=0; i<img_ant_pts.size(); i++){

        if(estado[i]){
            //PUNTOS INICIAL Y FINAL
            Point2i pi, pf;
            pi = img_ant_pts[i];
            pf = img_actual_pts[i];


            //Calculamos la distancia y el angulo entre los dos puntos
            float dist = sqrt(pow((pf.x - pi.x) , 2) + pow((pf.y - pi.y) , 2));
            float angulo = atan2((pf.y - pi.y), (pf.x - pi.x));

            if(dist >= 2 && dist <= 80){
                cout << i << endl;
                cout << "Pi: (" << pi.x << ", " << pi.y << ")" << endl;
                cout << "Pf: (" << pf.x << ", " << pf.y << ")" << endl;
                cout << "Dist: " << dist << endl;
                cout << "Angulo: " << angulo << endl << endl;

                //dibujamos la linea
                arrowedLine(img, pi, pf, Scalar(255,0,0),1,8,0,0.2);
                save_data("prueba.txt", "", pi, pf, dist, angulo, id);



            }else
                continue;

        }

    }
    save_end_img("prueba.txt");


}


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


    string directorio_images = argv[1];


    vector<string> imgs_name = leer_imagenes(directorio_images);
    vector<string> imgs_name_sort(imgs_name.size()-2);
    int numero_puntos = 2000;

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



    vector<Mat> images;
    for(int i=0; i<200; i++){
        Mat im = imread(directorio_images+"/"+imgs_name_sort[i]);
        images.push_back(im);
    }

    vector<Point2f> puntos_ant(numero_puntos);
    vector<Point2f> puntos_actual(numero_puntos);
    vector<unsigned char> estado(numero_puntos);
    vector<float> error;
    int key;

    inicializa_fichero("prueba.txt");


    for(int i=1; i<200; i++){
        Mat img_ant = images[i-1];
        Mat img_actual = images[i];
        Mat im_ant, im_act;

        cvtColor(img_ant, im_ant, CV_RGB2GRAY);
        cvtColor(img_actual, im_act, CV_RGB2GRAY);

        cout << "IMG: " << i << endl;

        goodFeaturesToTrack(im_ant, puntos_ant, numero_puntos, 0.01, 0);
        calcOpticalFlowPyrLK(im_ant, im_act, puntos_ant, puntos_actual, estado, error);


        dibuja_CloudTracking(img_actual, puntos_ant, puntos_actual, estado, i);
        namedWindow("img", CV_WINDOW_NORMAL);
        imshow("img", img_actual);

        /*
         *
         * namedWindow(to_string(i), CV_WINDOW_NORMAL);
        imshow(to_string(i), img_actual);
         * */

        cout << endl;
        key = waitKey(0);
        if(key == 27)
            break;

    }








    return 0;
}





