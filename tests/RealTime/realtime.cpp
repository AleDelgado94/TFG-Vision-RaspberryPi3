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
#include <boost/program_options/options_description.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>
#include <algorithm>
#include <regex>



using namespace std;
using namespace cv;
//using namespace raspicam;
namespace opt = boost::program_options;

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
        sort( imagenes.begin(), imagenes.end() );
    }

    return imagenes;
}

int num_photos(const string& ruta_imagenes){
  dirent* img;
  int num=-2;
  DIR* directorio;
  directorio = opendir(ruta_imagenes.c_str());
  if (directorio)
  {
      img = readdir(directorio);
    while (img != nullptr)
    {
    num++;
    img = readdir(directorio);
    }
      closedir(directorio);
  }

  return num;
}


void genera_hdr(int i, vector<string> imgs, string dir_origen,  string dir_destino){

    vector<Mat> images;
    vector<float> times;

    float val;
    //while(i<imgs.size()){



        for(int k=i; k<(i+3); k++){
            val = pow(2, (k*(-1)));
            Mat img = imread(dir_origen+"/"+imgs[k]);
            //imshow("img",img);
            images.push_back(img);
            times.push_back(1/val);
        }

        //MAT RESPONSE
        Mat response;
        Ptr<CalibrateDebevec> calibrate = createCalibrateDebevec();
        calibrate->process(images, response, times);

        //GENERAMOS LA IMAGEN HDR
        Mat hdr;
        Ptr<MergeDebevec> merge_debevec = createMergeDebevec();
        merge_debevec->process(images, hdr, times, response);

        /*Mat ldr;
        Ptr<TonemapDurand> tonemap = createTonemapDurand(2.2f);
        tonemap->process(hdr, ldr);*/

        Mat fusion;
        Ptr<MergeMertens> merge_mertens = createMergeMertens();
        merge_mertens->process(images, fusion);


        imwrite(dir_destino+"/"+imgs[i], fusion * 255);


        //i = i+3;
        cout << i << endl;

    //}
}

std::vector<std::string> ordena_3imagenes(string directorio_origen){

  vector<string> imgs_name = leer_imagenes(directorio_origen);

  vector<string> imgs_name_sort(imgs_name.size()-2);

    for(int i=2; i<imgs_name.size(); i=i+3){
        regex rgx("^[0-9]+");
        smatch match;
        string s = imgs_name[i];
        string s2 = imgs_name[i+1];
        string s3 = imgs_name[i+2];

        if(regex_search(s, match, rgx)){
            string number(match[0]);
            int n = atoi(number.c_str());
            n = n * 3;

            imgs_name_sort[n] = s;
            imgs_name_sort[n+1] = s2;
            imgs_name_sort[n+2] = s3;

        }
  }
  for (size_t i = 0; i < imgs_name_sort.size(); i++) {
    std::cout << imgs_name_sort[i] << '\n';
  }
  return imgs_name_sort;
}



int main(int argc, char *argv[])
{

    /*if(argc != 3){
        cout << "Ayuda: " << endl;
        cout << "./Meger [ruta_imagenes_originales] [ruta_destino_hdr]" << endl;
        exit(-1);
    }*/

    string directorio_origen;
    string directorio_destino;

    opt::options_description desc("Options");
    desc.add_options()
      ("help,h", "./Realtime [[ --source | -s ] ruta_imagenes_originales] [[ --destination | -d ] ruta_destino_hdr]")
      ("source,s", opt::value<std::vector<std::string>>(), "ruta_origen")
      ("destination,d", opt::value<std::vector<std::string>>(), "ruta_destino");

      opt::variables_map vm;
      opt::positional_options_description p;
      p.add("ruta_origen", -1);
      store(opt::parse_command_line(argc, argv,desc), vm);
      notify(vm);

      if(argc == 1){
        cout << desc << endl;
        return 1;
      }

      if(vm.count("help")){
        cout << desc << endl;
      }
      if(vm.count("source")){
          std::vector<std::string> files = vm["source"].as<std::vector<string>>();
           directorio_origen = files[0];
           //COMPROBAMOS EL DIRECTORIO 1

           if(access(directorio_origen.c_str(), 0) == 0){
               struct stat estado;
               stat(directorio_origen.c_str(), &estado);

               if(estado.st_mode && S_IFDIR){
                   //Existe el directorio
                   cout << "Existe el directorio " << directorio_origen << endl;

                   //COMPROBAMOS EL TAMAÑO DEL DIRECTORIO DE origen
                   //DIRECTORIO DE DESTINO
                   if(vm.count("destination")){
                     std::vector<std::string> files_dest = vm["destination"].as<std::vector<string>>();
                       directorio_destino = files_dest[0];
                   }

                   if(access(directorio_destino.c_str(), 0) == 0){
                       struct stat estado;
                       stat(directorio_destino.c_str(), &estado);

                       if(estado.st_mode && S_IFDIR){
                           //Existe el directorio
                           cout << "Existe el directorio " << directorio_destino << endl;
                       }
                   }
                   else{
                       cout << "Creando directorio " << directorio_destino << endl;
                       mkdir(directorio_destino.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                   }


                   int num_photos_ = num_photos(directorio_origen);
                   cout << num_photos_ << endl;

                   while(1){
                     int num_photos_actual = num_photos(directorio_origen);
                     if(num_photos_actual-3 == num_photos_ ){
                       std::cout << "Sleep" << '\n';
                       sleep(7);
                       std::cout << "Wake up!" << '\n';
                       std::cout << "Generando el merge de las nuevas imagenes" << '\n';
                       std::vector<std::string> v = ordena_3imagenes(directorio_origen);
                       for (size_t i = 0; i < v.size(); i++) {
                        std::cout << v[i] << '\n';
                       }
                       genera_hdr(num_photos_actual-3, v, directorio_origen, directorio_destino);

                       num_photos_ = num_photos_actual;
                     }
                   }

                   //for(int i=2; i<imgs.size(); i=i+3)
                    //   genera_hdr(i,imgs, directorio_origen, directorio_destino);



               }
           }
           else{
               //NO Existe el directorio
               cout << "El directorio de origen no existe" << endl;
               exit(-1);
           }
      }



    //waitKey(0);
    return 0;
}
