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
int umbral_bajo = 0;
int const umbral_alto = 255;
int ratio_ = 3;
int kernel_size = 3;
int umbral;
std::vector<std::vector<Point2i>> tracking_points;
Point2i referencia_ant_i;
Point2i referencia_ant_f;

Point2i referencia_act_i;
Point2i referencia_act_f;

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

std::vector<std::vector<Point2f>> agrupa_vectores(const std::vector<Point2f>& img_ant_pts, const std::vector<Point2f>& img_actual_pts){

    std::vector<std::vector<Point2f>> v;
    std::vector<Point2f> v_act;
    std::vector<Point2f> v_ant;

    Point2f point_ant, point_act;


    point_ant = img_ant_pts[0];
    point_act = img_actual_pts[0];
    float dist = 0;

    for (size_t i = 1; i < img_ant_pts.size(); i++) {
        dist = sqrt(pow((img_ant_pts[i].x - point_ant.x), 2) + pow((img_ant_pts[i].y - point_ant.y),2));
        cout << dist << endl;
        //if (dist > 10 && dist < 200) {
          Point2f punto_medio_ant;
          punto_medio_ant.x = (point_ant.x + img_ant_pts[i].x)/2;
          punto_medio_ant.y = (point_ant.y + img_ant_pts[i].y)/2;
          v_ant.push_back(punto_medio_ant);

          Point2f punto_medio_act;
          punto_medio_act.x = (point_act.x + img_actual_pts[i].x)/2;
          punto_medio_act.y = (point_act.y + img_actual_pts[i].y)/2;
          v_act.push_back(punto_medio_act);

          point_ant = img_ant_pts[i];
          point_act = img_actual_pts[i];
      //  }
    }


    v.push_back(v_ant);
    v.push_back(v_act);
    cout << "N v_act: " << v_act.size() << endl;
    cout << "N v_ant: " << v_ant.size() << endl;
    return v;

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
                /*cout << i << endl;
                cout << "Pi: (" << pi.x << ", " << pi.y << ")" << endl;
                cout << "Pf: (" << pf.x << ", " << pf.y << ")" << endl;
                cout << "Dist: " << dist << endl;
                cout << "Angulo: " << angulo << endl << endl;*/

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

void detecta_sun(Mat img1, int umbral_bajo){

    vector<float> soles_radio;
    vector<Point2f> soles_center;

    Mat img;
    img1.copyTo(img);
    //BILATERAL FILTER
    Mat bilateralFilterImg;
    bilateralFilter(img, bilateralFilterImg, 5, 175, 175);

    //EDGE DETECTION
    Mat edge;
    cvtColor(bilateralFilterImg, bilateralFilterImg, CV_RGB2GRAY);
    threshold(bilateralFilterImg, edge, umbral_bajo, umbral_alto, 0);

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
            //double area = (3.14159265359 * pow(radius[i],2));

            if (radius[i] > 150 && radius[i] < 210) {
                    cout << radius[i] << endl;
                    soles_radio.push_back(radius[i]);
                    soles_center.push_back(center[i]);
            }
    }

    if(soles_radio.size() > 1){

        int i, proximo;
        auto centro = soles_center.front();
        auto radio = soles_radio.front();


        //BUSQUEDA DEL ELEMENTO MÁS PROXIMO A 200

        int num_aproximar = 200;

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
    else
    {


        //NO SE DETECTO EL SOL MEDIANTE LA TÉCNICA UTILIZADA
        //TODO: DETECCION MEDIANTE ILUMINACIÓN
        Mat hsv;
        cvtColor(img1, hsv, CV_RGB2GRAY);

        Scalar LOW(228,230,237);
        Scalar HIGH(255,255,255);


        //DETECCIÓN MEDIANTE BRILLO
        Mat mask;
        inRange(hsv, LOW, HIGH, mask);

        imshow("mask", mask);

        //ELIMINACION DE RUIDOS
        Moments m;
        m = moments(mask);
        double moment_area = m.m00;
        cout << "Area: " << moment_area << endl;
        Point2f center;

        //Buscar el centro
        int x = int(m.m10/m.m00);
        int y = int(m.m01/m.m00);
        center.x = x;
        center.y = y;

        //cout << center.x << "  " << center.y << endl;



        circle(img, center, 70, Scalar(255,0,0),2,8,0);



        imshow("SOLITO",hsv);

    }

    namedWindow("sol", CV_WINDOW_NORMAL);
    imshow("sol", img);

}


int main(int argc, char *argv[])
{
    umbral_bajo=250;

    string ruta_directorio = argv[1];
    referencia_act_i.x=0;
    referencia_act_i.y=0;
    referencia_act_f.x=0;
    referencia_act_f.y=0;
    referencia_ant_f=referencia_act_f;
    referencia_ant_i=referencia_ant_i;

    cout << "Umbral: " << umbral_bajo << endl << endl;

    vector<string> imgs_name = leer_imagenes(ruta_directorio);
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
        Mat im = imread(ruta_directorio+"/"+imgs_name_sort[i]);
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
        Mat img_sun = images[i];
        Mat im_ant, im_act;

        cvtColor(img_ant, im_ant, CV_RGB2GRAY);
        cvtColor(img_actual, im_act, CV_RGB2GRAY);
        //cvtColor(img_sun, img_sun, CV_RGB2GRAY);

        cout << "IMG: " << i << endl;
        detecta_sun(img_sun, umbral_bajo);

        goodFeaturesToTrack(im_ant, puntos_ant, numero_puntos, 0.01, 0);
        calcOpticalFlowPyrLK(im_ant, im_act, puntos_ant, puntos_actual, estado, error);

        cout << "Puntos ant: " << puntos_ant.size() << endl;
        cout << "Puntos actuales: " << puntos_actual.size() << endl;

        std::vector<std::vector<Point2f>> v(2);
        v = agrupa_vectores(puntos_ant, puntos_actual);
        puntos_ant = v[0];
        puntos_actual = v[1];

        dibuja_CloudTracking(img_actual, puntos_ant, puntos_actual, estado, i);


        namedWindow("nubes", CV_WINDOW_NORMAL);
        imshow("nubes", img_actual);


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
