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
    int filas, columnas, puntos;
    std::string ruta_directorio;
    int key;

    opt::options_description desc("Options");
    desc.add_options()
      ("help,h", "./Detection [[ --source | -s ] ruta_imagenes_originales] [[ --destination | -d ] ruta_destino_hdr]")
      ("dir,d", opt::value<std::vector<std::string>>(), "directorio_imagenes")
      ("filas,f", opt::value<int>(&filas)->default_value(12), "filas")
      ("puntos,p", opt::value<int>(&puntos)->default_value(2000), "puntos")
      ("columnas,c", opt::value<int>(&columnas)->default_value(8), "columnas");

      opt::variables_map vm;
      store(opt::parse_command_line(argc, argv,desc), vm);
      notify(vm);


      if(vm.count("help") || argc == 1){
        cout << desc << endl;
        return 0;
      }
      if(vm.count("filas")){
        filas = vm["filas"].as<int>();
      }
      if(vm.count("columnas")){
        columnas = vm["columnas"].as<int>();
      }
      if(vm.count("puntos")){
        puntos = vm["puntos"].as<int>();
      }
      if(vm.count("dir")){
        std::vector<std::string> v = vm["dir"].as<std::vector<std::string>>();
        ruta_directorio = v[0];
      }



    //string ruta_directorio = argv[1];


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

            imgs_name_sort[n] = s;
        }

    }




    vector<Mat> images;
    for(int i=0; i<imgs_name_sort.size(); i++){
        Mat im = imread(ruta_directorio+"/"+imgs_name_sort[i]);
        images.push_back(im);
    }

    inicializa_fichero(fichero, "prueba.txt");

    for(int i=1; i<imgs_name_sort.size(); i++){
        Mat img_ant = images[i-1].clone();
        Mat img_actual = images[i].clone();
        Mat img_sun = images[i].clone();
        Mat im_ant, im_act;
        Point2f centro_sol;
        //PARA CADA VENTANA SE PROCESAN LOS VECTORES DE MOVIMIENTO
        //IMAGEN SE DIVIDE EN VENTANAS DE 8X12 DE TAMAÑO (128X64)

        cvtColor(img_ant, im_ant, CV_RGB2GRAY);
        cvtColor(img_actual, im_act, CV_RGB2GRAY);

        cout << "IMG: " << i << endl;
        centro_sol = detecta_sun(img_sun, umbral_bajo);


      //  vectores_Window(centro_sol, img_actual, im_ant, im_act, i, filas, columnas);
        vectores(centro_sol, img_actual, im_ant, im_act, i, filas, columnas, puntos);



        //dibuja_CloudTracking(img_actual, puntos_ant, puntos_actual, estado, i);


        namedWindow("nubes", CV_WINDOW_NORMAL);
        //namedWindow("sol", CV_WINDOW_NORMAL);
        imshow("nubes", img_actual);
        //imshow("sol", img_sun);



        key = waitKey(0);
        if(key == 27)
            break;

    }


    return 0;
}
