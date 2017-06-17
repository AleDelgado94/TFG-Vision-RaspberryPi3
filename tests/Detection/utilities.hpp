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
#include <boost/program_options/options_description.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>
#include <boost/asio.hpp>

using namespace std;
using namespace cv;
namespace opt = boost::program_options;
namespace as = boost::asio;
using namespace::boost::asio;


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

Point2f detecta_sun(Mat& img1, int umbral_bajo){

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
    //imshow("threshold", edge);

    //COUNTOURS
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(edge, contours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector<vector<Point>> contours_poly(contours.size());
    vector<Point2f> center(contours.size());
    vector<float> radius(contours.size());

    cout << "Contornos encontrados: " << contours.size() << endl;

    Point2f aprox_punto_central;

    for (int i = 0; i < contours.size(); i++)
    {
            approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
            minEnclosingCircle((Mat)contours_poly[i], center[i], radius[i]);
            //double area = (3.14159265359 * pow(radius[i],2));

          //  cout << "Radio: " << radius[i] << endl;

            if (radius[i] > 75 && radius[i] < 100) {
                    soles_radio.push_back(radius[i]);
                    soles_center.push_back(center[i]);
            }
    }

    std::cout << "Radio_Sol: " << soles_radio.size() << '\n';

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
        aprox_punto_central = centro;

        std::cout << "Detección por contornos aproximacion" << '\n';

        circle(img1, centro, 70, Scalar(0, 0, 255), 2, 8, 0);
        circle(img1, centro, 5, Scalar(255, 0, 0), 2, 8, 0);



    }else if(soles_radio.size() == 1){
        auto radio = soles_radio.front();
        auto centro = soles_center.front();
        aprox_punto_central = centro;
        soles_radio.clear();
        soles_center.clear();
        std::cout << "Detección por contornos" << '\n';
        circle(img1, centro, 70, Scalar(0, 0, 255), 2, 8, 0);
        circle(img1, centro, 5, Scalar(255, 0, 0), 2, 8, 0);
    }
    else
    {

      //imshow("img1",img1);
        //NO SE DETECTO EL SOL MEDIANTE LA TÉCNICA UTILIZADA
        Mat hsv;
        cvtColor(img1, hsv, CV_RGB2GRAY);

        Scalar LOW(228,230,237);
        Scalar HIGH(255,255,255);


        //DETECCIÓN MEDIANTE BRILLO
        Mat mask;
        inRange(hsv, LOW, HIGH, mask);
        //imshow("Mascara", mask);


        //ELIMINACION DE RUIDOS
        Moments m;
        m = moments(mask);
        double moment_area = m.m00;
        Point2f center;

        cout << moment_area << endl;
        //Buscar el centro
        int x = int(m.m10/m.m00);
        int y = int(m.m01/m.m00);
        center.x = x;
        center.y = y;
        cout << center << endl;
        aprox_punto_central = center;

        std::cout << "Detección por brillo" << '\n';
        circle(img1, center, 70, Scalar(255,0,0),2,8,0);



        //imshow("SOLITO",hsv);

    }

    //namedWindow("sol", CV_WINDOW_NORMAL);
    //imshow("sol", img1);
    return aprox_punto_central;

}


void dibuja_CloudTracking(Mat& img, const vector<Point2f>& img_ant_pts, const vector<Point2f>& img_actual_pts,
  int x, int y,
  const vector<unsigned char>& estado, int id){
    ofstream fichero;


    for(int i=0; i<img_ant_pts.size(); i++){

      //  if(estado[i]){
            //PUNTOS INICIAL Y FINAL
            Point2i pi, pf;
            pi = img_ant_pts[i];
            pf = img_actual_pts[i];


            //Calculamos la distancia y el angulo entre los dos puntos
            float dist = sqrt(pow((pf.x - pi.x) , 2) + pow((pf.y - pi.y) , 2));
            float angulo = fmod((180*atan2((pf.y - pi.y), (pf.x - pi.x))),360);

            if(dist >= 5 && dist <= 80){
                /*cout << "Pi: (" << pi.x << ", " << pi.y << ")" << endl;
                cout << "Pf: (" << pf.x << ", " << pf.y << ")" << endl;
                cout << "Dist: " << dist << endl;
                cout << "Angulo: " << angulo << endl << endl;*/

                //dibujamos la linea
                arrowedLine(img, pi, pf, Scalar(255,0,0),1,8,0,0.2);
                //save_data(fichero, "prueba.txt", "", pi, pf, dist, angulo, id);

            }else
                continue;

      //  }

    }
    save_end_img(fichero, "prueba.txt");
}

void dibuja_CloudTracking_red(Mat& img, const vector<Point2f>& img_ant_pts, const vector<Point2f>& img_actual_pts,
  int x, int y,
  const vector<unsigned char>& estado, int id){
    ofstream fichero;


    for(int i=0; i<img_ant_pts.size(); i++){

      //  if(estado[i]){
            //PUNTOS INICIAL Y FINAL
            Point2i pi, pf;
            pi = img_ant_pts[i];
            pf = img_actual_pts[i];


            //Calculamos la distancia y el angulo entre los dos puntos
            float dist = sqrt(pow((pf.x - pi.x) , 2) + pow((pf.y - pi.y) , 2));
            float angulo = fmod((180*atan2((pf.y - pi.y), (pf.x - pi.x))),360);

            if(dist >= 5 && dist <= 80){
                /*cout << "Pi: (" << pi.x << ", " << pi.y << ")" << endl;
                cout << "Pf: (" << pf.x << ", " << pf.y << ")" << endl;
                cout << "Dist: " << dist << endl;
                cout << "Angulo: " << angulo << endl << endl;*/

                //dibujamos la linea
                arrowedLine(img, pi, pf, Scalar(0,0,255),1,8,0,0.2);
              //  rectangle(img, Point(x,y), Point(1024-x, 768-y), Scalar(0,255,255));
                //save_data(fichero, "prueba.txt", "", pi, pf, dist, angulo, id);

            }else
                continue;

      //  }

    }
    save_end_img(fichero, "prueba.txt");
}

void vectores(Point2f centro_sol, Mat& img_original, const Mat& img_ant, const Mat& img_act, int id, int filas=12,
  int columnas=8, int num_puntos=2000){
    vector<vector<vector<Point2f>>> v_tracking;
    int ancho = 1024/columnas;
    int alto = 768/filas;

    cout << ancho << "  " << alto << endl;
    cout << "El centro del sol es: " << centro_sol << endl;


    int i=0;
    int j=0;


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

          goodFeaturesToTrack(img_ant, p_img_anterior, num_puntos, 0.01, 0);
          calcOpticalFlowPyrLK(img_ant, img_act, p_img_anterior, p_img_actual, estado, error);



            dibuja_CloudTracking(img_original, p_img_anterior, p_img_actual, i, j ,estado, id);

            for (size_t i = ancho ; i < 1024 - ancho; i=i+ancho) {
              for (size_t j = 0; j < 768 - alto; j=j+alto) {
                rectangle(img_original, Point(i,j), Point(i+ancho, j+alto), Scalar(0,255,255));

                if((centro_sol.x-70 >= i && centro_sol.y+70 >= j) && (centro_sol.x-70 <= i+ancho && centro_sol.y+70 <= j+alto)
                || (centro_sol.x+70 >= i && centro_sol.y-70 >= j) && (centro_sol.x+70 <= i+ancho && centro_sol.y-70 <= j+alto)
                || (centro_sol.x+70 >= i && centro_sol.y+70 >= j) && (centro_sol.x+70 <= i+ancho && centro_sol.y+70 <= j+alto)
                || (centro_sol.x-70 >= i && centro_sol.y-70 >= j) && (centro_sol.x-70 <= i+ancho && centro_sol.y-70 <= j+alto)
                || (centro_sol.x-70 >= i && centro_sol.y >= j) && (centro_sol.x-70 <= i+ancho && centro_sol.y <= j+alto)
                || (centro_sol.x+70 >= i && centro_sol.y >= j) && (centro_sol.x+70 <= i+ancho && centro_sol.y <= j+alto)
                || (centro_sol.x >= i && centro_sol.y+70 >= j) && (centro_sol.x <= i+ancho && centro_sol.y+70 <= j+alto)
                || (centro_sol.x >= i && centro_sol.y-70 >= j) && (centro_sol.x <= i+ancho && centro_sol.y-70 <= j+alto)
                || (centro_sol.x-70/2 >= i && centro_sol.y >= j) && (centro_sol.x-70/2 <= i+ancho && centro_sol.y <= j+alto)
                || (centro_sol.x+70/2 >= i && centro_sol.y >= j) && (centro_sol.x+70/2 <= i+ancho && centro_sol.y <= j+alto)
                || (centro_sol.x >= i && centro_sol.y+70/2 >= j) && (centro_sol.x <= i+ancho && centro_sol.y+70/2 <= j+alto)
                || (centro_sol.x >= i && centro_sol.y-70/2 >= j) && (centro_sol.x <= i+ancho && centro_sol.y-70/2 <= j+alto)){
                  Point ini(centro_sol.x-70,centro_sol.y+70);
                  Point fin(centro_sol.x+70, centro_sol.y-70);
                  rectangle(img_original, ini, fin, Scalar(0,0,255));
                }
              }
            }

  }

  bool entre(float num_procesar, float value_max, float value_min){
    return (num_procesar <= value_max && num_procesar >= value_min) ? true : false;
  }



  bool entre(Point punto_procesar, Point vertice_superior, Point vertice_inferior){
    if(entre(punto_procesar.x, vertice_superior.x, vertice_inferior.x) && entre(punto_procesar.y, vertice_superior.y, vertice_inferior.y))
      return true;
    else
      return false;
  }


  void predice(Mat& img_original, Point2f centro_sol, Point2f base_vector, Point2f final_vector){
    double dist = sqrt(pow((final_vector.x - base_vector.x) , 2) + pow((final_vector.y - base_vector.y) , 2));
    //cvFlip(img_original);

    //angle = angle
    Point ini_sol(centro_sol.x-70,centro_sol.y+70);
    Point fin_sol(centro_sol.x+70, centro_sol.y-70);
    if(dist >= 15){


      //CALCULAMOS LAS COMPONENTES DEL VECTOR
      int COMPONENTE_X = final_vector.x - base_vector.x;
      int COMPONENTE_Y = -(final_vector.y - base_vector.y);

      double angle = ((atan2((COMPONENTE_Y - 0), (COMPONENTE_X - 0)))*360) / (2*M_PI);
      std::cout << "angulo: " << angle << '\n';


      Point final = Point((base_vector.x+(100*COMPONENTE_X)), (base_vector.y+100*(-COMPONENTE_Y)));
      Point actual = final_vector;

      int i=0;
      bool intercepta = false;
      while(((actual.x < 1024 && actual.x > 0) && (actual.y < 768 && actual.y > 0))){
        if(angle == 0){
          actual.x = actual.x + COMPONENTE_X;
        }else if(angle == 90){
          actual.y = actual.y - -COMPONENTE_Y;
        }else if(angle == 180){
          actual.x = actual.x + COMPONENTE_X;
        }else if(angle == -90){
          actual.y = actual.y + -COMPONENTE_Y;
        }else if(angle > 90){
          //SEGUNDO CUADRANTE +-
          actual.x = actual.x + COMPONENTE_X;
          actual.y = actual.y - COMPONENTE_Y;
        }else if(angle > 0){
          //PRIMER CUADRANTE ++
          actual.x = actual.x + COMPONENTE_X;
          actual.y = actual.y - COMPONENTE_Y; 
        }else if(angle < -90){
          //CUARTO CUADRANTE --
          actual.x = actual.x + COMPONENTE_X;
          actual.y = actual.y + -COMPONENTE_Y;
        }else if(angle < 0){
          //TERCER CUADRANTE -+
          actual.x = actual.x + COMPONENTE_X;
          actual.y = actual.y + -COMPONENTE_Y;
        }
        circle(img_original, actual, 1, Scalar(255,0,0));

        if(entre(actual, ini_sol, fin_sol)){
         intercepta = true;

         break;
        }
      }

      std::cout << "Direccion al Sol??: " << intercepta << '\n';
      std::cout << "COMPONENTE_X: " << COMPONENTE_X << '\n';
      std::cout << "COMPONENTE_Y: " << COMPONENTE_Y << '\n';
      //line(img_original, base_vector, final, CV_RGB(0,255,0), 2 ,CV_AA);
    }

  }


  void vectores_img(Point2f centro_sol, Mat& img_original, const Mat& img_ant, const Mat& img_act, int id, int filas=12,
    int columnas=8, int num_puntos=5000){
      vector<vector<vector<Point2f>>> v_tracking;
      int ancho = 1024/columnas;
      int alto = 768/filas;


            std::vector<Point2f> p_img_anterior(num_puntos);
            std::vector<Point2f> p_img_actual(num_puntos);
            std::vector<unsigned char> estado(num_puntos);
            std::vector<float> error;

            Mat bilateralFilterImg;
            bilateralFilter(img_ant, bilateralFilterImg, 5, 175, 175);

            //EDGE DETECTION
            Mat edge;
            cvtColor(bilateralFilterImg, bilateralFilterImg, CV_RGB2GRAY);
            threshold(bilateralFilterImg, edge, 200, 255, 0);
            imshow("edge", edge);

            Mat bilateralFilterImg2;
            bilateralFilter(img_act, bilateralFilterImg2, 5, 175, 175);

            //EDGE DETECTION
            Mat edge2;
            cvtColor(bilateralFilterImg2, bilateralFilterImg2, CV_RGB2GRAY);
            threshold(bilateralFilterImg2, edge2, 200, 255, 0);
            imshow("edge", edge2);


            //imshow(im, ventana_img_actual);

            goodFeaturesToTrack(edge, p_img_anterior, num_puntos, 0.01, 0);
            calcOpticalFlowPyrLK(edge, edge2, p_img_anterior, p_img_actual, estado, error);

            int index = 0;
            for (size_t i = 0; i < 1024 ; i=i+ancho) {
              for (size_t j = 0; j < 768  ; j=j+alto) {
                cout << "Index: " << index << endl;
                index++;
                std::vector<Point2f> v_p_ant;
                std::vector<Point2f> v_p_act;
                for (size_t k = 0; k < p_img_anterior.size(); k++) {
                  Point2f p_procesado_actual = p_img_anterior[k];
                  Point2f p_procesado_actual_act = p_img_actual[k];
                  //Si el punto se encuentra dentro del área a procesar, entonces los reducimos a un
                  //único vector por área
                  if(entre(p_procesado_actual.x, i+ancho, i) && entre(p_procesado_actual.y, j+alto, j)){
                    //cout << p_procesado_actual << endl;
                    v_p_ant.push_back(p_procesado_actual);
                    v_p_act.push_back(p_procesado_actual_act);
                  }
                }

                float media_x=0, media_y=0;
                int size = v_p_ant.size();
                if(size >= 1){
                //  std::cout << "Hay " << size << " vectores" << '\n';
                //  std::cout << "Hay " << v_p_act.size() << " vectores actuales" << '\n';
                  for (size_t k = 0; k < size; k++) {
                    media_x += v_p_ant[k].x;
                    media_y += v_p_ant[k].y;
                  }
                  Point2f p_medio((int)(media_x/size), (int)(media_y/size));
                  cout << "Punto medio: " << p_medio << endl;


                  for (size_t k = 0; k < size; k++) {
                      while(v_p_ant[k].x != p_medio.x){
                        if(v_p_ant[k].x < p_medio.x){
                          v_p_ant[k].x++;
                          v_p_act[k].x++;
                        }else if(v_p_ant[k].x > p_medio.x){
                          v_p_ant[k].x--;
                          v_p_act[k].x--;
                        }
                      }

                      while(v_p_ant[k].y != p_medio.y){
                        if(v_p_ant[k].y < p_medio.y){
                          v_p_ant[k].y++;
                          v_p_act[k].y++;
                        }else if(v_p_ant[k].y > p_medio.y){
                          v_p_ant[k].y--;
                          v_p_act[k].y--;
                        }
                      }
                  }

                  std::vector<Point2f> v_ini(1), v_fin(1);
                  v_ini.push_back(v_p_ant[0]);

                  float CONSTANTE_X = 0;
                  float CONSTANTE_Y = 0;


                  int media_x_act=0, media_y_act=0;
                  int size_act = v_p_act.size();
                  for (size_t k = 0; k < v_p_act.size(); k++) {
                    media_x_act += v_p_act[k].x;
                    media_y_act += v_p_act[k].y;
                  }
                  Point2f p_final = Point2f((int)(media_x_act/size_act), (int)(media_y_act/size_act));


                  v_fin.push_back(p_final);

                  //std::cout << "Distancia final: " << sqrt(pow((p_final.x - p_medio.x) , 2) + pow((p_final.y - p_medio.y) , 2)) << '\n';

                  //arrowedLine(img_original, p_medio, p_final, Scalar(0,0,255),1,8,0,0.2);
                  dibuja_CloudTracking_red(img_original, v_ini, v_fin,i,j,estado,id);
                  predice(img_original, centro_sol,p_medio, p_final);
               }


              }
            }


            //  dibuja_CloudTracking(img_original, p_img_anterior, p_img_actual, i, j ,estado, id);

              for (size_t i = 0 ; i < 1024 ; i=i+ancho) {
                for (size_t j = 0; j < 768 ; j=j+alto) {
                  rectangle(img_original, Point(i,j), Point(i+ancho, j+alto), Scalar(0,255,255));

                  if((centro_sol.x-70 >= i && centro_sol.y+70 >= j) && (centro_sol.x-70 <= i+ancho && centro_sol.y+70 <= j+alto)
                  || (centro_sol.x+70 >= i && centro_sol.y-70 >= j) && (centro_sol.x+70 <= i+ancho && centro_sol.y-70 <= j+alto)
                  || (centro_sol.x+70 >= i && centro_sol.y+70 >= j) && (centro_sol.x+70 <= i+ancho && centro_sol.y+70 <= j+alto)
                  || (centro_sol.x-70 >= i && centro_sol.y-70 >= j) && (centro_sol.x-70 <= i+ancho && centro_sol.y-70 <= j+alto)
                  || (centro_sol.x-70 >= i && centro_sol.y >= j) && (centro_sol.x-70 <= i+ancho && centro_sol.y <= j+alto)
                  || (centro_sol.x+70 >= i && centro_sol.y >= j) && (centro_sol.x+70 <= i+ancho && centro_sol.y <= j+alto)
                  || (centro_sol.x >= i && centro_sol.y+70 >= j) && (centro_sol.x <= i+ancho && centro_sol.y+70 <= j+alto)
                  || (centro_sol.x >= i && centro_sol.y-70 >= j) && (centro_sol.x <= i+ancho && centro_sol.y-70 <= j+alto)
                  || (centro_sol.x-70/2 >= i && centro_sol.y >= j) && (centro_sol.x-70/2 <= i+ancho && centro_sol.y <= j+alto)
                  || (centro_sol.x+70/2 >= i && centro_sol.y >= j) && (centro_sol.x+70/2 <= i+ancho && centro_sol.y <= j+alto)
                  || (centro_sol.x >= i && centro_sol.y+70/2 >= j) && (centro_sol.x <= i+ancho && centro_sol.y+70/2 <= j+alto)
                  || (centro_sol.x >= i && centro_sol.y-70/2 >= j) && (centro_sol.x <= i+ancho && centro_sol.y-70/2 <= j+alto)){
                    Point ini(centro_sol.x-70,centro_sol.y+70);
                    Point fin(centro_sol.x+70, centro_sol.y-70);
                    rectangle(img_original, ini, fin, Scalar(0,0,255));
                  }
                }
              }

    }


void vectores_Window(Point2f centro_sol, Mat& img_original, const Mat& img_ant, const Mat& img_act, int id, int filas=12,
  int columnas=8, int num_puntos=15){
    vector<vector<vector<Point2f>>> v_tracking;
    int ancho = 1024/columnas;
    int alto = 768/filas;

    cout << ancho << "  " << alto << endl;
    cout << "El centro del sol es: " << centro_sol << endl;


    int i=0;
    int j=0;
    for (size_t i = ancho ; i < 1024 - ancho; i=i+ancho) {
      for (size_t j = 0; j < 768 - alto; j=j+alto) {


        circle(img_original, Point(i,j), 5, Scalar(255,0,0),2,8,0);
        circle(img_original, centro_sol, 65, Scalar(0,0,255),2,8,0);

        rectangle(img_original, Point(i,j), Point(i+ancho, j+alto), Scalar(0,255,255));



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

          //goodFeaturesToTrack(img_ant, p_img_anterior, num_puntos, 0.01, 0);
          goodFeaturesToTrack(ventana_img_ant, p_img_anterior, num_puntos, 0.01, 0);
          /*if(p_img_anterior.size() < num_puntos)
            continue;
          else*/
          calcOpticalFlowPyrLK(ventana_img_ant, ventana_img_actual, p_img_anterior, p_img_actual, estado, error);
            //calcOpticalFlowPyrLK(img_ant, img_act, p_img_anterior, p_img_actual, estado, error);



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
          //v_ant[0] = p_ini;
          for (size_t l = 0; l < p_img_anterior.size(); l++) {
            v_ant.push_back(p_ini);
          }


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
            pi = v_ant[i];
            pf = p_img_actual[i];


            //Calculamos la distancia y el angulo entre los dos puntos
            float dist = sqrt(pow((pf.x - pi.x) , 2) + pow((pf.y - pi.y) , 2));
            float angulo = fmod((180*atan2((pf.y - pi.y), (pf.x - pi.x))),360);

            //if(dist >= 2 && dist<=30){
              CONSTANTE_X += dist*cos(angulo);
              CONSTANTE_Y += dist*sin(angulo);
            //  cout << dist << endl;
            //}
          }

          Point2f p_final = Point2f(p_ini.x+(int)CONSTANTE_X, p_ini.y+(int)CONSTANTE_Y);
          for (size_t i = 0; i < p_img_actual.size(); i++) {
            v_act.push_back(p_final);
          }

          //v_act[0] = p_final;

        //  v_act.push_back(Point2f((int)(media_x_act/p_img_actual.size()), (int)(media_y_act/p_img_actual.size())));
          //CALCULAMOS LA SUMA DE VECTORES PARA VER LA MEDIA DEL VECTOR RESULTANTE PARTIENDO DEL PUNTO
          //CALCULADO ANTERIORMENTE



  //        if(v_ant.size() == 1 && v_act.size() == 1)
            //dibuja_CloudTracking(img_original, p_img_anterior, p_img_actual, i, j ,estado, id);
            dibuja_CloudTracking(img_original, v_ant, p_img_actual, i, j ,estado, id);





      }
    }




        for (size_t i = ancho ; i < 1024 - ancho; i=i+ancho) {
          for (size_t j = 0; j < 768 - alto; j=j+alto) {
            if((centro_sol.x-70 >= i && centro_sol.y+70 >= j) && (centro_sol.x-70 <= i+ancho && centro_sol.y+70 <= j+alto)
            || (centro_sol.x+70 >= i && centro_sol.y-70 >= j) && (centro_sol.x+70 <= i+ancho && centro_sol.y-70 <= j+alto)
            || (centro_sol.x+70 >= i && centro_sol.y+70 >= j) && (centro_sol.x+70 <= i+ancho && centro_sol.y+70 <= j+alto)
            || (centro_sol.x-70 >= i && centro_sol.y-70 >= j) && (centro_sol.x-70 <= i+ancho && centro_sol.y-70 <= j+alto)
            || (centro_sol.x-70 >= i && centro_sol.y >= j) && (centro_sol.x-70 <= i+ancho && centro_sol.y <= j+alto)
            || (centro_sol.x+70 >= i && centro_sol.y >= j) && (centro_sol.x+70 <= i+ancho && centro_sol.y <= j+alto)
            || (centro_sol.x >= i && centro_sol.y+70 >= j) && (centro_sol.x <= i+ancho && centro_sol.y+70 <= j+alto)
            || (centro_sol.x >= i && centro_sol.y-70 >= j) && (centro_sol.x <= i+ancho && centro_sol.y-70 <= j+alto)
            || (centro_sol.x-70/2 >= i && centro_sol.y >= j) && (centro_sol.x-70/2 <= i+ancho && centro_sol.y <= j+alto)
            || (centro_sol.x+70/2 >= i && centro_sol.y >= j) && (centro_sol.x+70/2 <= i+ancho && centro_sol.y <= j+alto)
            || (centro_sol.x >= i && centro_sol.y+70/2 >= j) && (centro_sol.x <= i+ancho && centro_sol.y+70/2 <= j+alto)
            || (centro_sol.x >= i && centro_sol.y-70/2 >= j) && (centro_sol.x <= i+ancho && centro_sol.y-70/2 <= j+alto)){
              Point ini(centro_sol.x-70,centro_sol.y+70);
              Point fin(centro_sol.x+70, centro_sol.y-70);
              rectangle(img_original, ini, fin, Scalar(0,0,255));
              rectangle(img_original, Point(i,j), Point(i+ancho, j+alto), Scalar(0,0,255));
            }
          }
        }

}
