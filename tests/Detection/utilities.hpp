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


void save_data(ofstream& fichero, string ruta_fichero, string nombre_img, Point2i pi, Point2i pf, float dist, float angulo, int id){



    fichero.open(ruta_fichero.c_str(), ios::app);

    int pi_x = pi.x;
    int pi_y = pi.y;
    int pf_x = pf.x;
    int pf_y = pf.y;

    fichero << id << ": ("+ to_string(pi_x) +", "+ to_string(pi_y) +")    ("+ to_string(pf_x) +", "+ to_string(pf_y) +")    "+ to_string(dist) +"    "+ to_string(angulo) +"\n";
    fichero.close();


}


void inicializa_fichero(ofstream& fichero, string ruta_fichero){
    fichero.open(ruta_fichero.c_str(), ios::app);
    fichero << "Id_img  (Pi.x, Pi.y)    (Pf.x, Pf.y)    Dist    Angle\n";
    fichero.close();
}

void save_end_img(ofstream& fichero, string ruta_fichero){
    fichero.open(ruta_fichero.c_str(), ios::app);
    fichero << "\n";
    fichero.close();
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

void detecta_sun(Mat& img1, int umbral_bajo){

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
    threshold(bilateralFilterImg, edge, umbral_bajo, 255, 0);

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

        circle(img1, centro, (int)radio, Scalar(0, 0, 255), 2, 8, 0);
        circle(img1, centro, 5, Scalar(255, 0, 0), 2, 8, 0);



    }else if(soles_radio.size() == 1){
        auto radio = soles_radio.front();
        auto centro = soles_center.front();
        soles_radio.clear();
        soles_center.clear();
        circle(img1, centro, (int)radio, Scalar(0, 0, 255), 2, 8, 0);
        circle(img1, centro, 5, Scalar(255, 0, 0), 2, 8, 0);
    }
    else
    {


        //NO SE DETECTO EL SOL MEDIANTE LA TÉCNICA UTILIZADA
        Mat hsv;
        cvtColor(img1, hsv, CV_RGB2GRAY);

        Scalar LOW(228,230,237);
        Scalar HIGH(255,255,255);


        //DETECCIÓN MEDIANTE BRILLO
        Mat mask;
        inRange(hsv, LOW, HIGH, mask);


        //ELIMINACION DE RUIDOS
        Moments m;
        m = moments(mask);
        double moment_area = m.m00;
        Point2f center;

        //Buscar el centro
        int x = int(m.m10/m.m00);
        int y = int(m.m01/m.m00);
        center.x = x;
        center.y = y;


        circle(img1, center, 70, Scalar(255,0,0),2,8,0);



        //imshow("SOLITO",hsv);

    }

    //namedWindow("sol", CV_WINDOW_NORMAL);
    //imshow("sol", img1);

}


void dibuja_CloudTracking(Mat img, const vector<Point2f>& img_ant_pts, const vector<Point2f>& img_actual_pts,
  int x, int y,
  const vector<unsigned char>& estado, int id){
    ofstream fichero;


    for(int i=0; i<img_ant_pts.size(); i++){

        if(estado[i]){
            //PUNTOS INICIAL Y FINAL
            Point2i pi, pf;
            pi = img_ant_pts[i];
            pf = img_actual_pts[i];


            //Calculamos la distancia y el angulo entre los dos puntos
            //float dist = sqrt(pow((pf.x - pi.x) , 2) + pow((pf.y - pi.y) , 2));
            //float angulo = fmod((180*atan2((pf.y - pi.y), (pf.x - pi.x))),360);

            //if(dist >= 2 && dist <= 80){
                /*cout << "Pi: (" << pi.x << ", " << pi.y << ")" << endl;
                cout << "Pf: (" << pf.x << ", " << pf.y << ")" << endl;
                cout << "Dist: " << dist << endl;
                cout << "Angulo: " << angulo << endl << endl;*/

                //dibujamos la linea
                arrowedLine(img, pi, pf, Scalar(255,0,0),1,8,0,0.2);
                rectangle(img, Point(x,y), Point(1024-x, 768-y), Scalar(0,255,255));
                //save_data(fichero, "prueba.txt", "", pi, pf, dist, angulo, id);

          //  }else
            //    continue;

        }

    }
    save_end_img(fichero, "prueba.txt");
}


void vectores_Window(Mat& img_original, const Mat& img_ant, const Mat& img_act, int id, int alto=64,
  int ancho=128, int num_puntos=5){
    vector<vector<vector<Point2f>>> v_tracking;


    for (size_t i = ancho ; i < 1024-ancho; i=i+ancho) {
      for (size_t j = alto; j < 768 - alto; j=j+alto) {
          std::vector<Point2f> p_img_anterior(num_puntos);
          std::vector<Point2f> p_img_actual(num_puntos);
          std::vector<unsigned char> estado(num_puntos);
          std::vector<float> error;

          Mat ventana_img_actual(img_act, Rect(i,j,ancho,alto));
          Mat ventana_img_ant(img_ant, Rect(i,j,ancho,alto));
          std::string im(to_string(i));
          im += "_";
          im += to_string(j);


          //imshow(im, ventana_img_actual);

          goodFeaturesToTrack(ventana_img_ant, p_img_anterior, num_puntos, 0.01, 0);
          if(p_img_anterior.size() < num_puntos)
            continue;
          else
            calcOpticalFlowPyrLK(ventana_img_ant, ventana_img_actual, p_img_anterior, p_img_actual, estado, error);
          for (size_t k = 0; k < p_img_anterior.size(); k++) {
              p_img_anterior[k].x += i;
              p_img_anterior[k].y += j;
          }
          for (size_t k = 0; k < p_img_actual.size(); k++) {
              p_img_actual[k].x += i;
              p_img_actual[k].y += j;
          }

          int media_x_ant = 0;
          int media_y_ant = 0;
          int media_x_act = 0;
          int media_y_act = 0;
          std::vector<Point2f> v_ant(1);
          std::vector<Point2f> v_act(1);

          //PUNTO MEDIO PARA EL VECTOR DE LA IMAGEN ANTERIOR EN ESTA VENTANA
          for (size_t k = 0; k < p_img_anterior.size(); k++) {
              media_x_ant += p_img_anterior[k].x;
              media_y_ant += p_img_anterior[k].y;
          }
          Point2f p_ini(media_x_ant/p_img_anterior.size(), media_y_ant/p_img_anterior.size());
          /*for (size_t i = 0; i < p_img_anterior.size(); i++) {
            v_ant.push_back(p_ini);
          }*/
          v_ant[0] = p_ini;


          //PUNTO MEDIO PARA EL VECTOR ACTUAL EN ESTA VENTANA
          for (size_t k = 0; k < p_img_actual.size(); k++) {
            //  media_x_act += p_img_actual[k].x;
            //  media_y_act += p_img_actual[k].y;

              while(p_img_anterior[k].x != p_ini.x && p_img_anterior[k].y != p_ini.y){
                if(p_img_anterior[k].x < p_ini.x){
                  p_img_anterior[k].x++;
                  p_img_actual[k].x++;
                }else if(p_img_anterior[k].x > p_ini.x){
                  p_img_anterior[k].x--;
                  p_img_actual[k].x--;
                }

                if(p_img_anterior[k].y < p_ini.y){
                  p_img_anterior[k].y++;
                  p_img_actual[k].y++;
                }else if(p_img_anterior[k].y > p_ini.y){
                  p_img_anterior[k].y--;
                  p_img_actual[k].y--;
                }
              }


          }

          float CONSTANTE_X = 0;
          float CONSTANTE_Y = 0;

          for (size_t i = 0; i < p_img_actual.size(); i++) {
            Point2f pi, pf;
            pi = v_ant[0];
            pf = p_img_actual[i];


            //Calculamos la distancia y el angulo entre los dos puntos
            float dist = sqrt(pow((pf.x - pi.x) , 2) + pow((pf.y - pi.y) , 2));
            float angulo = fmod((180*atan2((pf.y - pi.y), (pf.x - pi.x))),360);

            if(dist >= 2 && dist<=30){
              CONSTANTE_X += dist*cos(angulo);
              CONSTANTE_Y += dist*sin(angulo);
            }
          }

          Point2f p_final = Point2f(p_ini.x+(int)CONSTANTE_X, p_ini.y+(int)CONSTANTE_Y);

          /*for (size_t i = 0; i < p_img_actual.size(); i++) {
            v_act.push_back(p_final);
          }*/
          v_act[0] = p_final;

        //  v_act.push_back(Point2f((int)(media_x_act/p_img_actual.size()), (int)(media_y_act/p_img_actual.size())));
          //CALCULAMOS LA SUMA DE VECTORES PARA VER LA MEDIA DEL VECTOR RESULTANTE PARTIENDO DEL PUNTO
          //CALCULADO ANTERIORMENTE




          if(v_ant.size() == 1 && v_act.size() == 1)
            dibuja_CloudTracking(img_original, v_ant, v_act, i, j ,estado, id);
            rectangle(img_original, Point(i,j), Point(1024-i, 768-j), Scalar(0,255,255));


      }
    }
}
