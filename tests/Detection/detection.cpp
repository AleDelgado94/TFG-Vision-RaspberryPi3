#include "utilities.hpp"


using namespace std;
using namespace cv;

ofstream fichero;
int umbral_bajo = 0;
int const umbral_alto = 255;
int ratio_ = 3;
int kernel_size = 3;
int umbral;
std::vector<std::vector<Point2i>> tracking_points;



int main(int argc, char *argv[])
{
    umbral_bajo=250;
    int key;

    string ruta_directorio = argv[1];


    cout << "Umbral: " << umbral_bajo << endl << endl;

    vector<string> imgs_name = leer_imagenes(ruta_directorio);
    vector<string> imgs_name_sort(imgs_name.size()-2);

    for(int i=2; i<imgs_name.size(); i++){
        regex rgx("^[0-9]+");
        smatch match;
        string s = imgs_name[i];


        if(regex_search(s, match, rgx)){
            string number(match[0]);
            int n = atoi(number.c_str());
            cout << n << endl;

            imgs_name_sort[n] = s;
        }

    }




    vector<Mat> images;
    for(int i=0; i<10; i++){
        Mat im = imread(ruta_directorio+"/"+imgs_name_sort[i]);
        images.push_back(im);
    }

    /*vector<Point2f> puntos_ant(numero_puntos);
    vector<Point2f> puntos_actual(numero_puntos);
    vector<unsigned char> estado(numero_puntos);
    vector<float> error;
    int key;*/

    inicializa_fichero(fichero, "prueba.txt");

    for(int i=1; i<10; i++){
        Mat img_ant = images[i-1];
        Mat img_actual = images[i];
        Mat img_sun = images[i];
        Mat im_ant, im_act;
        //PARA CADA VENTANA SE PROCESAN LOS VECTORES DE MOVIMIENTO
        //IMAGEN SE DIVIDE EN VENTANAS DE 8X12 DE TAMAÑO (128X64)

        cvtColor(img_ant, im_ant, CV_RGB2GRAY);
        cvtColor(img_actual, im_act, CV_RGB2GRAY);

        cout << "IMG: " << i << endl;
        detecta_sun(img_sun, umbral_bajo);


        /*goodFeaturesToTrack(im_ant, puntos_ant, numero_puntos, 0.01, 0);
        calcOpticalFlowPyrLK(im_ant, im_act, puntos_ant, puntos_actual, estado, error);

        cout << "Puntos ant: " << puntos_ant.size() << endl;
        cout << "Puntos actuales: " << puntos_actual.size() << endl;*/

        vectores_Window(img_actual, im_ant, im_act, i);



        //dibuja_CloudTracking(img_actual, puntos_ant, puntos_actual, estado, i);


        namedWindow("nubes", CV_WINDOW_NORMAL);
        imshow("nubes", img_actual);



        cout << endl;
        key = waitKey(0);
        if(key == 27)
            break;

    }




    return 0;
}
