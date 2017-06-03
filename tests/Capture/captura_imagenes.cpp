#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <raspicam/raspicam_cv.h>
#include <ctime>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <thread>
#include <unistd.h>
#include <cstring>
#include <time.h>
#include <sstream>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>
#include "../Arduino/Temperature/temperature.h"


using namespace std;
using namespace cv;
using namespace raspicam;
namespace opt = boost::program_options;
namespace as = boost::asio;

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

//cv::VideoCapture video;
raspicam::RaspiCam_Cv video;
cv::Mat bgrMap;

int numSnapshot = 0;
  std::string id = "0";

int segundos;

time_t rawtime;
struct tm * timeinfo;
int day;
int month;
int year;

std::string hora;
char key = 0;

vector<Mat> images;
vector<float> times;

const float max_exposure = 200;
const float max_iso = 1000;
float exposure;
float iso;
string ruta_carpeta_destino;

io_service io;
string PORT;
std::string database;
sqlite3* db;
char* sqError = 0;



// Base serial settings
serial_port_base::baud_rate BAUD(115200);
serial_port_base::flow_control FLOW( serial_port_base::flow_control::none );
serial_port_base::parity PARITY( serial_port_base::parity::none );
serial_port_base::stop_bits STOP( serial_port_base::stop_bits::one );

void setExposure(float exposure) {

	video.set(CV_CAP_PROP_EXPOSURE, exposure);
	video.grab();
	cout << "Exposicion " << video.get(CV_CAP_PROP_EXPOSURE) << endl;
}

void setISO(float iso){
	video.set(CV_CAP_PROP_GAIN, iso);
	video.grab();
	cout << "ISO" << video.get(CV_CAP_PROP_GAIN) << endl;
}



void captura(){


    while(key != 27){

	sleep(segundos);

    std::cout <<  "Hora: " <<hora << std::endl;


	//setExposure(-8);
	setISO(2);
	sleep(2);
	video.grab();
	video.retrieve(bgrMap);
	imwrite(ruta_carpeta_destino.c_str() + id + "-" + hora + "_1.png", bgrMap);


	//setExposure(5);
	setISO(7);
	sleep(2);
	video.grab();
	video.retrieve(bgrMap);
	imwrite(ruta_carpeta_destino.c_str() + id + "-" + hora + "_2.png", bgrMap);


	//setExposure(10);
	setISO(10);
	sleep(2);
	video.grab();
	video.retrieve(bgrMap);
	imwrite(ruta_carpeta_destino.c_str() + id + "-" + hora + "_3.png", bgrMap);

    string fecha = to_string(year);
    fecha += "/";
    fecha += to_string(month);
    fecha += "/";
    fecha += to_string(day);

  ask_data(port, db, fecha, hora, id);


    numSnapshot++;
    id = static_cast<std::ostringstream*>(&(std::ostringstream() << numSnapshot))->str();

    }

}


int graba(){
    std::cout << "'Esc'  para salir" << std::endl;

	video.set ( CV_CAP_PROP_FRAME_WIDTH, 1024 );
    video.set ( CV_CAP_PROP_FRAME_HEIGHT, 768 );
    //video.set ( CV_CAP_PROP_FPS, 30);
    cout << video.get ( CV_CAP_PROP_GAIN) << endl;
    cout << video.get ( CV_CAP_PROP_EXPOSURE) << endl;


    if (!video.open()) {
        std::cerr<<"No se puede abrir la camera"<<std::endl;
        exit(-1);
    }



    exposure = 0;
	  iso = 0;

    sleep(1);

    do {

        time(&rawtime);
        timeinfo = localtime(&rawtime);


        year = timeinfo->tm_year + 1900;
        month = timeinfo->tm_mon + 1;
        day = timeinfo->tm_day;

        hora = "";
        for(int i=11; i<19; i++){
          hora += asctime(timeinfo)[i];
        }

	      video.grab();
        video.retrieve(bgrMap);
        cv::imshow("Video", bgrMap);



        key=cv::waitKey(20);



    }while(key!=27 && video.grab());

    video.release();



}



int main(int argc, char* argv[]){

    opt::options_description desc("Options");
    desc.add_options()
    ("help,h", "./Captura_images_rgb [[ --port | -p ] puerto] [[ --segundos | -s ] segundos] [[ --destino | -d ] carpeta_destino_imagenes] [[ --database | -b ] base_de_datos]")
    ("segundos,s", opt::value<int>(&segundos)->default_value(20), "segundos")
    ("destino,d", opt::value<std::vector<std::string>>(), "carpeta de destino de las imagenes")
    ("database,b", opt::value<std::vector<std::string>>(), "base de datos")
    ("port,p", opt::value<std::vector<std::string>>(), "puerto");

    opt::variables_map vm;
    opt::positional_options_description p;
    p.add("port", -1);
    store(opt::parse_command_line(argc, argv,desc), vm);
    notify(vm);

    if(argc == 1){
      cout << desc << endl;
      return -1;
    }

    if(vm.count("help")){
      cout << desc << endl;
      return 0;
    }
    if(vm.count("segundos")){
      segundos = vm["segundos"].as<int>();
    }
    if(vm.count("destino")){
      std::vector<std::string> v = vm["destino"].as<std::vector<std::string>>();
      ruta_carpeta_destino = v[0];
    }
    if(vm.count("database")){
      std::vector<std::string> v = vm["database"].as<std::vector<std::string>>();
      ruta_carpeta_destino = v[0];
    }
    if(vm.count("port")){
      std::vector<std::string> v = vm["port"].as<std::vector<std::string>>();
      PORT = v[0];
    }

    int db_handler = sqlite3_open(database.c_str(), &db);
    if(db_handler){
        std::cerr << "No se puede abrir la base de datos. Error: " << sqlite3_errmsg(db) <<'\n';
        return(-1);
    }else{
      cout << "Se abrió la base de datos" << endl;
    }

    string crear_tabla_data = "CREATE TABLE IF NOT EXISTS DATOS ( ID_NOMBRE TEXT PRIMARY KEY, HORA TEXT, FECHA TEXT,  TEMPERATURA INTEGER, HUMEDAD INTEGER, SOLAR REAL, ESTADO TEXT);";
    int exe = sqlite3_exec(db, crear_tabla_data.c_str(), callback, 0, &sqError);
    if(exe != SQLITE_OK){
      fprintf(stderr, "SQL error: %s\n", sqError);
      sqlite3_free(sqError);
    }



    namedWindow("Video", CV_WINDOW_NORMAL);

    serial_port port( io, PORT.c_str() );
    port.set_option( BAUD );
    port.set_option( FLOW );
    port.set_option( PARITY );
    port.set_option( STOP );


    std::thread foto_(captura);
    std::thread video_(graba);



    video_.join();
    foto_.join();

    sqlite3_close(db);
    return 0;
}


void trackbar_exposure(float, void*){
		video.set(CV_CAP_PROP_EXPOSURE, exposure);
		imshow("IMG", bgrMap);
		cout << "Exposure: " << video.get ( CV_CAP_PROP_EXPOSURE) << endl;
}

void trackbar_iso(float, void*){
		video.set(CV_CAP_PROP_GAIN, iso);
		imshow("IMG", bgrMap);
		cout << "ISO: " << video.get ( CV_CAP_PROP_GAIN) << endl;
}
