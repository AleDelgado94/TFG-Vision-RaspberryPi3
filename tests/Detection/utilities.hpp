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
#include <sqlite3.h>
#include "../Database/sqlite3/sqliteCallBack.hpp"
#include "typeinfo"



using namespace std;
using namespace cv;
namespace opt = boost::program_options;
namespace as = boost::asio;
using namespace::boost::asio;

/********************************
Tipo de prediccion del sol
 0 - Prediccion por brillo
 1 - Prediccion por contornos
 ********************************/


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

int sun(Mat& img1, int umbral_bajo){
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

          if (radius[i] > 50 && radius[i] < 210) {
                  soles_radio.push_back(radius[i]);
                  soles_center.push_back(center[i]);
          }
  }


  if(soles_radio.size() > 1){

      int i, proximo;
      auto centro = soles_center.front();
      auto radio = soles_radio.front();


      //BUSQUEDA DEL ELEMENTO MÁS PROXIMO A 200

      int num_aproximar = 50;

      vector<float> diferencia;
      for(int j=0; j<soles_radio.size(); j++){
          diferencia.push_back(abs(num_aproximar-soles_radio[j]));
      }

      int index = menor_vector(diferencia);

      centro = soles_center[index];
      radio = soles_radio[index];

      return 1;

  }else if(soles_radio.size() == 1){
      auto radio = soles_radio.front();
      auto centro = soles_center.front();
      soles_radio.clear();
      soles_center.clear();
      return 1;
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

      return 0;

  }
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
    imshow("threshold", edge);


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



            if (radius[i] > 50 && radius[i] < 210) {
              cout << "Radio: " << radius[i] << endl;
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

        int num_aproximar = 50;

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
    if(entre(punto_procesar.x, vertice_inferior.x, vertice_superior.x) && entre(punto_procesar.y, vertice_inferior.y, vertice_superior.y))
      return true;
    else
      return false;
  }


  void predice(Mat& img_original, Point2f centro_sol, Point2f base_vector, Point2f final_vector, int precision, int id,
    sqlite3* db, std::string fecha, std::string hora, double temperatura, double humedad, double solar, int region_imagen,
    std::string hora_img_ant, int tipo_deteccion_sol){

    double dist = sqrt(pow((final_vector.x - base_vector.x) , 2) + pow((final_vector.y - base_vector.y) , 2));
    string prediccion;
    //cvFlip(img_original);

    //angle = angle
    Point ini_sol(centro_sol.x-precision,centro_sol.y-precision);
    Point fin_sol(centro_sol.x+precision, centro_sol.y+precision);
    if(dist >= 15){

      std::cout << endl << endl << "Región: " << region_imagen << '\n';

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

        if(entre(actual, ini_sol, fin_sol)){
         intercepta = true;
         break;
        }

        if(angle == 0){
          actual.x = actual.x + COMPONENTE_X/2;
        }else if(angle == 90){
          actual.y = actual.y - -COMPONENTE_Y/2;
        }else if(angle == 180){
          actual.x = actual.x + COMPONENTE_X/2;
        }else if(angle == -90){
          actual.y = actual.y + -COMPONENTE_Y/2;
        }else if(angle > 90){
          //SEGUNDO CUADRANTE +-
          actual.x = actual.x + COMPONENTE_X/2;
          actual.y = actual.y - COMPONENTE_Y/2;
        }else if(angle > 0){
          //PRIMER CUADRANTE ++
          actual.x = actual.x + COMPONENTE_X/2;
          actual.y = actual.y - COMPONENTE_Y/2;
        }else if(angle < -90){
          //CUARTO CUADRANTE --
          actual.x = actual.x + COMPONENTE_X/2;
          actual.y = actual.y + -COMPONENTE_Y/2;
        }else if(angle < 0){
          //TERCER CUADRANTE -+
          actual.x = actual.x + COMPONENTE_X/2;
          actual.y = actual.y + -COMPONENTE_Y/2;
        }

        circle(img_original, actual, 2, Scalar(255,0,0));
      }


      std::cout << "Direccion al Sol??: " << intercepta << '\n';
      std::cout << "COMPONENTE_X: " << COMPONENTE_X << '\n';
      std::cout << "COMPONENTE_Y: " << COMPONENTE_Y << '\n';

      if(intercepta){
        //CALCULAMOS LA DISTANCIA ENTRE EL VECTOR Y LA REGION DONDE SE ENCUENTRA EL SOL
        double distancia_Sol = sqrt(pow((actual.x - base_vector.x) , 2) + pow((actual.y - base_vector.y) , 2));
        std::cout << "La distancia hasta la región de Sol es de: " << distancia_Sol << '\n';

        //DIFERENCIA EN SEGUNDOS ENTRE LAS IMAGENES
        std::tm t_act;
        std::istringstream h_act(hora);
        std::cout << "hora actual: " << hora << '\n';
        h_act >> std::get_time(&t_act, "%H:%M:%S");
        int secs_act = (t_act.tm_hour * 3600) + (t_act.tm_min * 60) + t_act.tm_sec;

        std::tm t_ant;
        std::istringstream h_ant(hora_img_ant);
        std::cout << "hora anterior: " << hora_img_ant << '\n';
        h_ant >> std::get_time(&t_ant, "%H:%M:%S");
        int secs_ant = t_ant.tm_hour * 3600 + t_ant.tm_min * 60 + t_ant.tm_sec;

        int paso_tiempo = secs_act - secs_ant;

        // PUNTO SE MUEVE "paso_tiempo" SEGUNDOS ENTRE EL PUNTO "base_vector" Y "final_vector"
        // CON UNA DISTANCIA "dist"
        double velocida_pixel = dist/paso_tiempo;
        std::cout << "Velocidad: " << velocida_pixel << '\n';

        //TENEMOS DISTANCIA HASTA LA REGION DEL SOL "distancia_Sol"
        // SI VECTOR SE DESPLAZA A LA VELOCIDAD "velocida_pixel" LLEGARÁ
        // SI ALL VA BIEN AL SOL EN UN TIEMPO ?
        std::cout << "distancia_Sol: " << distancia_Sol << '\n';
        std::cout << "paso_tiempo: " << paso_tiempo << '\n';
        std::cout << "dist: " << dist << '\n';
        double secs_llegada_a_region_solar = (distancia_Sol * paso_tiempo) / dist;

        double secs_llegada = secs_act + secs_llegada_a_region_solar;

        int hour = (int)secs_llegada_a_region_solar/3600;
        secs_llegada_a_region_solar = (int)secs_llegada_a_region_solar%3600;
        int min = (int)secs_llegada_a_region_solar/60;
        secs_llegada_a_region_solar = (int)secs_llegada_a_region_solar%60;
        int sec = (int)secs_llegada_a_region_solar;


        int hour_llegada = (int)secs_llegada/3600;
        secs_llegada = (int)secs_llegada%3600;
        int min_llegada = (int)secs_llegada/60;
        secs_llegada = (int)secs_llegada%60;
        int sec_llegada = (int)secs_llegada;



        std::string tiempo_prediccion = to_string(hour) + " horas " + to_string(min) + " minutos y " + to_string(sec) + " segundos";
        std::string tiempo_llegada_total = to_string(hour_llegada) + ":" + to_string(min_llegada) + ":" + to_string(sec_llegada);

        std::cout << "Tardará aproximadamente " << tiempo_prediccion << " en llegar a la region solar sobre las "<< tiempo_llegada_total <<" horas." << '\n';



      }

    }



  }


  void vectores_img(Point2f centro_sol, Mat& img_original, const Mat& img_ant, const Mat& img_act, int id, int filas=12,
    int columnas=8, int precision=70,int num_puntos=5000, sqlite3* db=nullptr, int tipo_deteccion_sol=-1){
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

            ///////////////////////////////
            ///////////////////////////////
            ///// CONSULTA A LA BASE DE ///
            ////// DATOS LOS PARAMETROS////
            ///////////////////////////////
            ///////////////////////////////

            std::string id_str = to_string(id);
            //EXTRAEMOS LA INFORMACIÓN ASOCIADA A LA IMAGEN ACTUAL DE LA BASE DE DATOS
            std::string query_sql("SELECT FECHA, HORA, TEMPERATURA, HUMEDAD, SOLAR FROM DATOS WHERE ID_NOMBRE = ");
            query_sql += id_str;
            std::string fecha, hora;
            double temperatura, humedad, solar;


            //ejecutamos las consultas
            sqlite3_stmt* ppStmt;
            if(int exe = sqlite3_prepare_v2(db, query_sql.c_str(), -1, &ppStmt, NULL) == SQLITE_OK ){
              while (SQLITE_ROW == sqlite3_step(ppStmt)) {
                fecha =  (char*)(sqlite3_column_text(ppStmt, 0));
                hora = (char*)(sqlite3_column_text(ppStmt, 1));
                temperatura = (sqlite3_column_double(ppStmt, 2));
                humedad = (sqlite3_column_double(ppStmt, 3));
                solar = (sqlite3_column_double(ppStmt, 4));
              }
            }else{
              std::cerr << "Error en la consulta " << query_sql << ": " << sqlite3_errmsg(db) << '\n';
            }


            std::string id_anterior = to_string(id-1);
            std::string hora_ant_sql("SELECT HORA FROM DATOS WHERE ID_NOMBRE = ");
            hora_ant_sql += id_anterior;
            std::string hora_ant;

            if(int exe = sqlite3_prepare_v2(db, hora_ant_sql.c_str(), -1, &ppStmt, NULL) == SQLITE_OK ){
              while (SQLITE_ROW == sqlite3_step(ppStmt)) {
                hora_ant = (char*)(sqlite3_column_text(ppStmt, 0));
              }
            }else{
              std::cerr << "Error en la consulta " << hora_ant_sql << ": " << sqlite3_errmsg(db) << '\n';
            }


            int index = 0;
            for (size_t i = 0; i < 1024 ; i=i+ancho) {
              for (size_t j = 0; j < 768  ; j=j+alto) {
                //cout << "Index: " << index << endl;

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
                  predice(img_original, centro_sol,p_medio, p_final, precision, id, db, fecha, hora, temperatura,
                     humedad, solar, index, hora_ant, tipo_deteccion_sol);
                  index++;
               }


              }
            }


            //  dibuja_CloudTracking(img_original, p_img_anterior, p_img_actual, i, j ,estado, id);

              for (size_t i = 0 ; i < 1024 ; i=i+ancho) {
                for (size_t j = 0; j < 768 ; j=j+alto) {
                  rectangle(img_original, Point(i,j), Point(i+ancho, j+alto), Scalar(0,255,255));

                  if((centro_sol.x-precision >= i && centro_sol.y+precision >= j) && (centro_sol.x-precision <= i+ancho && centro_sol.y+precision <= j+alto)
                  || (centro_sol.x+precision >= i && centro_sol.y-precision >= j) && (centro_sol.x+precision <= i+ancho && centro_sol.y-precision <= j+alto)
                  || (centro_sol.x+precision >= i && centro_sol.y+precision >= j) && (centro_sol.x+precision <= i+ancho && centro_sol.y+precision <= j+alto)
                  || (centro_sol.x-precision >= i && centro_sol.y-precision >= j) && (centro_sol.x-precision <= i+ancho && centro_sol.y-precision <= j+alto)
                  || (centro_sol.x-precision >= i && centro_sol.y >= j) && (centro_sol.x-precision <= i+ancho && centro_sol.y <= j+alto)
                  || (centro_sol.x+precision >= i && centro_sol.y >= j) && (centro_sol.x+precision <= i+ancho && centro_sol.y <= j+alto)
                  || (centro_sol.x >= i && centro_sol.y+precision >= j) && (centro_sol.x <= i+ancho && centro_sol.y+precision <= j+alto)
                  || (centro_sol.x >= i && centro_sol.y-precision >= j) && (centro_sol.x <= i+ancho && centro_sol.y-precision <= j+alto)
                  || (centro_sol.x-precision/2 >= i && centro_sol.y >= j) && (centro_sol.x-precision/2 <= i+ancho && centro_sol.y <= j+alto)
                  || (centro_sol.x+precision/2 >= i && centro_sol.y >= j) && (centro_sol.x+precision/2 <= i+ancho && centro_sol.y <= j+alto)
                  || (centro_sol.x >= i && centro_sol.y+precision/2 >= j) && (centro_sol.x <= i+ancho && centro_sol.y+precision/2 <= j+alto)
                  || (centro_sol.x >= i && centro_sol.y-precision/2 >= j) && (centro_sol.x <= i+ancho && centro_sol.y-precision/2 <= j+alto)){
                    Point ini(centro_sol.x-precision,centro_sol.y+precision);
                    Point fin(centro_sol.x+precision, centro_sol.y-precision);
                    rectangle(img_original, ini, fin, Scalar(0,0,255));
                  }
                }
              }

    }


void vectores_Window(Point2f centro_sol, Mat& img_original, const Mat& img_ant, const Mat& img_act, int id, int filas=12,
  int columnas=8, int precision=70, int num_puntos=15){
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




    for (size_t i = 0 ; i < 1024 ; i=i+ancho) {
      for (size_t j = 0; j < 768 ; j=j+alto) {
        rectangle(img_original, Point(i,j), Point(i+ancho, j+alto), Scalar(0,255,255));

        if((centro_sol.x-precision >= i && centro_sol.y+precision >= j) && (centro_sol.x-precision <= i+ancho && centro_sol.y+precision <= j+alto)
        || (centro_sol.x+precision >= i && centro_sol.y-precision >= j) && (centro_sol.x+precision <= i+ancho && centro_sol.y-precision <= j+alto)
        || (centro_sol.x+precision >= i && centro_sol.y+precision >= j) && (centro_sol.x+precision <= i+ancho && centro_sol.y+precision <= j+alto)
        || (centro_sol.x-precision >= i && centro_sol.y-precision >= j) && (centro_sol.x-precision <= i+ancho && centro_sol.y-precision <= j+alto)
        || (centro_sol.x-precision >= i && centro_sol.y >= j) && (centro_sol.x-precision <= i+ancho && centro_sol.y <= j+alto)
        || (centro_sol.x+precision >= i && centro_sol.y >= j) && (centro_sol.x+precision <= i+ancho && centro_sol.y <= j+alto)
        || (centro_sol.x >= i && centro_sol.y+precision >= j) && (centro_sol.x <= i+ancho && centro_sol.y+precision <= j+alto)
        || (centro_sol.x >= i && centro_sol.y-precision >= j) && (centro_sol.x <= i+ancho && centro_sol.y-precision <= j+alto)
        || (centro_sol.x-precision/2 >= i && centro_sol.y >= j) && (centro_sol.x-precision/2 <= i+ancho && centro_sol.y <= j+alto)
        || (centro_sol.x+precision/2 >= i && centro_sol.y >= j) && (centro_sol.x+precision/2 <= i+ancho && centro_sol.y <= j+alto)
        || (centro_sol.x >= i && centro_sol.y+precision/2 >= j) && (centro_sol.x <= i+ancho && centro_sol.y+precision/2 <= j+alto)
        || (centro_sol.x >= i && centro_sol.y-precision/2 >= j) && (centro_sol.x <= i+ancho && centro_sol.y-precision/2 <= j+alto)){
          Point ini(centro_sol.x-precision,centro_sol.y+precision);
          Point fin(centro_sol.x+precision, centro_sol.y-precision);
          rectangle(img_original, ini, fin, Scalar(0,0,255));
        }
      }
    }


}
