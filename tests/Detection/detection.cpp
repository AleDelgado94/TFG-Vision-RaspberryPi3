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
std::string database;
sqlite3* db;



int main(int argc, char *argv[])
{
    umbral_bajo=220;
    int filas, columnas, puntos, inicial, precision, sol;
    std::string ruta_directorio, output_prediction;
    int key;

    opt::options_description desc("Options");
    desc.add_options()
      ("help,h", "./Detection [[ --source | -s ] ruta_imagenes_originales] [[ --destination | -d ] ruta_destino_hdr]")
      ("dir,d", opt::value<std::vector<std::string>>(), "directorio_imagenes")
      ("filas,f", opt::value<int>(&filas)->default_value(12), "filas")
      ("columnas,c", opt::value<int>(&columnas)->default_value(8), "columnas")
      ("puntos,p", opt::value<int>(&puntos)->default_value(2000), "puntos")
      ("precision,u", opt::value<int>(&precision)->default_value(70), "precision")
      ("sol,s", opt::value<int>(&sol)->default_value(1), "sol")
      ("database,b", opt::value<std::vector<std::string>>(), "base de datos")
      ("inicial,i", opt::value<int>(&inicial)->default_value(1), "inicial")
      ("output,o", opt::value<std::vector<std::string>>(), "Fichero salida de la prediccion");

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
      if(vm.count("inicial")){
        inicial = vm["inicial"].as<int>();
      }
      if(vm.count("precision")){
        precision = vm["precision"].as<int>();
      }
      if(vm.count("sol")){
        sol = vm["sol"].as<int>();
      }
      if(vm.count("dir")){
        std::vector<std::string> v = vm["dir"].as<std::vector<std::string>>();
        ruta_directorio = v[0];
      }
      if(vm.count("database")){
        std::vector<std::string> v = vm["database"].as<std::vector<std::string>>();
        database = v[0];
      }
      if(vm.count("output")){
        std::vector<std::string> v = vm["output"].as<std::vector<std::string>>();
        output_prediction = v[0];
      }else{
        std::cerr << "Especifica el fichero de salida de la prediccion con -o" << '\n';
        return -1;
      }

      int db_handler = sqlite3_open(database.c_str(), &db);
      if(db_handler){
          std::cerr << "No se puede abrir la base de datos. Error: " << sqlite3_errmsg(db) <<'\n';
          return(-1);
      }

      if(!existeFichero(output_prediction)){
        std::cout << "Creando fichero: " << output_prediction << '\n';
        inicializa_fichero(fichero, output_prediction);
      }



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


    int j=sol;
    Point2f centro_sol;

    for(int i=inicial; i<images.size(); i++){

        int tipo_deteccion_sol = -1;

        Mat img_ant = images[i-1].clone();
        Mat img_actual = images[i].clone();
        Mat img_sun = images[i].clone();

        Mat im_ant, im_act;

        //PARA CADA VENTANA SE PROCESAN LOS VECTORES DE MOVIMIENTO
        //IMAGEN SE DIVIDE EN VENTANAS DE 8X12 DE TAMAÑO (128X64)

        //cvtColor(img_ant, im_ant, CV_RGB2GRAY);
        //cvtColor(img_actual, im_act, CV_RGB2GRAY);
        im_ant = img_ant;
        im_act = img_actual;

        cout << "IMG: " << i << endl;


        if(j == sol)
          centro_sol = detecta_sun(img_sun, umbral_bajo);

        tipo_deteccion_sol = sun(img_sun, umbral_bajo);
        if(tipo_deteccion_sol == 0) std::cout << "Detecta por brillo" << '\n';
        else  std::cout << "Detecta por contornos" << '\n';


      //  vectores_Window(centro_sol, img_actual, im_ant, im_act, i, filas, columnas);
        //vectores(centro_sol, img_actual, im_ant, im_act, i, filas, columnas, puntos);
        vectores_img(fichero, centro_sol, img_actual, im_ant, im_act, i, filas, columnas, precision ,puntos,
          db, tipo_deteccion_sol, output_prediction, imgs_name_sort[i]);
        std::cout << "--------------------------------------------------------------------------" << '\n';
        int fontFace = FONT_HERSHEY_PLAIN;
        double fontScale = 1.5;
        int thinkness = 1;
        putText(img_actual, imgs_name_sort[i], Point(10,25), fontFace, fontScale, CV_RGB(29,247,29), thinkness, 8);
        save_end_img(fichero, output_prediction);


        //dibuja_CloudTracking(img_actual, puntos_ant, puntos_actual, estado, i);


        namedWindow("nubes", CV_WINDOW_NORMAL);
        //namedWindow("sol", CV_WINDOW_NORMAL);
        imshow("nubes", img_actual);
        //imshow("sol", img_sun);

        j--;

        if(j == 0) j = sol;

        key = waitKey(0);
        if(key == 27)
            break;



    }

    sqlite3_close(db);
    return 0;
}
