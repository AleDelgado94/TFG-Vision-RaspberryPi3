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


void genera_hdr(int i, vector<string> imgs, string dir_origen,  string dir_destino){

    vector<Mat> images;
    vector<float> times;

    float val;
    //while(i<imgs.size()){



        for(int k=i; k<(i+3); k++){
            val = pow(2, (k*(-1)));
            Mat img = imread(dir_origen+"/"+imgs[k]);
            imshow("img",img);
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
      ("help,h", "./Meger [[ --source | -s ] ruta_imagenes_originales] [[ --destination | -d ] ruta_destino_hdr]")
      ("source,s", opt::value<std::vector<std::string>>(), "ruta_origen")
      ("destination,d", opt::value<std::vector<std::string>>(), "ruta_destino");

      opt::variables_map vm;
      opt::positional_options_description p;
      p.add("ruta_origen", -1);
      store(opt::parse_command_line(argc, argv,desc), vm);
      notify(vm);

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

                   vector<string> imgs = leer_imagenes(directorio_origen);
                   cout << imgs.size() << endl;
                   for(int i=2; i<imgs.size(); i=i+3)
                       genera_hdr(i,imgs, directorio_origen, directorio_destino);



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
