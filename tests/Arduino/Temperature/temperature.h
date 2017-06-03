#include <iostream>
#include <boost/asio.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>
#include <sqlite3.h>
#include "../../Database/sqlite3/sqliteCallBack.hpp"
#include <list>

using namespace::boost::asio;
using namespace std;
namespace opt = boost::program_options;
namespace as = boost::asio;


// Base serial settings
serial_port_base::baud_rate BAUD(115200);
serial_port_base::flow_control FLOW( serial_port_base::flow_control::none );
serial_port_base::parity PARITY( serial_port_base::parity::none );
serial_port_base::stop_bits STOP( serial_port_base::stop_bits::one );


string read_serial(serial_port& port){
  boost::asio::streambuf sb;
  size_t n = as::read_until(port, sb, '\n');
  boost::asio::streambuf::const_buffers_type bufs = sb.data();
  std::string temp(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + n);

  return temp;
}


std::list<result_query>* consulta_datos(sqlite3* db, char* consulta){
  int handler;
  sqlite3_stmt* ppStmt;
  std::list<result_query>* resultado = new std::list<result_query>();
  handler = sqlite3_prepare_v2(db, consulta, -1, &ppStmt, NULL);
  if(handler != SQLITE_OK){
    cerr << "Error al ejecutar la consulta" << endl;
  }else{
    int i=0;
    const unsigned char* vacio = reinterpret_cast<const unsigned char*>("");

    while(SQLITE_ROW == sqlite3_step(ppStmt)){
      result_query fila;

      if(sqlite3_column_text(ppStmt, 0) == NULL){
        fila.setId_name(vacio);
      }else{
        fila.setId_name(sqlite3_column_text(ppStmt, 0));
      }


      if(sqlite3_column_text(ppStmt, 1) == NULL){
        fila.setHora(vacio);
      }else{
        fila.setHora(sqlite3_column_text(ppStmt, 1));
      }


      if(sqlite3_column_text(ppStmt, 2) == NULL){
        fila.setFecha(vacio);
      }else{
        fila.setFecha(sqlite3_column_text(ppStmt, 2));
      }


      if(sqlite3_column_text(ppStmt, 6) == NULL){
        fila.setEstado(vacio);
      }else{
        fila.setEstado(sqlite3_column_text(ppStmt, 6));
      }


      fila.setTemperatura(sqlite3_column_double(ppStmt, 3));
      fila.setHumedad(sqlite3_column_double(ppStmt, 4));
      fila.setCelula_fotoelectrica(sqlite3_column_double(ppStmt, 5));

      resultado->push_back(fila);
      i++;

    }
  }
  return resultado;
}


std::list<result_query>* consulta_datos(sqlite3* db, std::string consulta){
  int handler;
  sqlite3_stmt* ppStmt;
  std::list<result_query>* resultado = new std::list<result_query>();
  handler = sqlite3_prepare_v2(db, consulta.c_str(), -1, &ppStmt, NULL);
  if(handler != SQLITE_OK){
    cerr << "Error al ejecutar la consulta" << endl;
  }else{
    int i=0;
    const unsigned char* vacio = reinterpret_cast<const unsigned char*>("");

    while(SQLITE_ROW == sqlite3_step(ppStmt)){
      result_query fila;

      if(sqlite3_column_text(ppStmt, 0) == NULL){
        fila.setId_name(vacio);
      }else{
        fila.setId_name(sqlite3_column_text(ppStmt, 0));
      }


      if(sqlite3_column_text(ppStmt, 1) == NULL){
        fila.setHora(vacio);
      }else{
        fila.setHora(sqlite3_column_text(ppStmt, 1));
      }


      if(sqlite3_column_text(ppStmt, 2) == NULL){
        fila.setFecha(vacio);
      }else{
        fila.setFecha(sqlite3_column_text(ppStmt, 2));
      }


      if(sqlite3_column_text(ppStmt, 6) == NULL){
        fila.setEstado(vacio);
      }else{
        fila.setEstado(sqlite3_column_text(ppStmt, 6));
      }


      fila.setTemperatura(sqlite3_column_double(ppStmt, 3));
      fila.setHumedad(sqlite3_column_double(ppStmt, 4));
      fila.setCelula_fotoelectrica(sqlite3_column_double(ppStmt, 5));

      resultado->push_back(fila);
      i++;

    }
  }
  return resultado;
}

void ask_data(serial_port& port, sqlite3* db){

  unsigned char opt[1];
  char* sqlError = 0;

  //PEDIMOS DATOS DE TEMPERATURA
  opt[0] = 't';
  float temperature;
  as::write(port, as::buffer(opt, 1));
  sleep(1);
  temperature = atof(read_serial(port).c_str());

  //PEDIMOS DATOS DE HUMEDAD
  opt[0] = 'h';
  float humedad;
  as::write(port, as::buffer(opt, 1));
  sleep(1);
  humedad = atof(read_serial(port).c_str());

  //PEDIMOS DATOS DE CELULA FOTOVOLTÁICA
  opt[0] = 's';
  float cel;
  as::write(port, as::buffer(opt, 1));
  sleep(1);
  cel = atof(read_serial(port).c_str());

  //INTRODUCIMOS LOS DATOS EN LA BASE DE DATOS SEGUN EL ID DE LA FOTO
  std::string sql("INSERT INTO DATOS (TEMPERATURA, HUMEDAD, SOLAR) VALUES (");
  sql += to_string(temperature);
  sql += ",";
  sql += to_string(humedad);
  sql += ",";
  sql += to_string(cel);
  sql += ");";
  int correct = sqlite3_exec(db, sql.c_str(), callback, 0, &sqlError);
  if(correct != SQLITE_OK)
    std::cerr << "Error: " << sqlError  << '\n';
}

/*
int main(int argc, char const *argv[]){
  io_service io;
  string PORT;
  std::string database;
  sqlite3* db;
  char* sqError = 0;


  opt::options_description desc("Options");
  desc.add_options()
    ("help,h", "./Temperature [[ --port | -p ] puerto]")
    ("port,p", opt::value<std::vector<std::string>>(), "puerto")
    ("database,d", opt::value<std::vector<std::string>>(), "base de datos");

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
    if(vm.count("port")){
      std::vector<std::string> v = vm["port"].as<std::vector<std::string>>();
      PORT = v[0];
    }
    if(vm.count("database")){
      std::vector<std::string> v = vm["database"].as<std::vector<std::string>>();
      database = v[0];
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
    }else{
      std::cout << "Tabla creada con exito" << '\n';
    }



    serial_port port( io, PORT.c_str() );
    port.set_option( BAUD );
    port.set_option( FLOW );
    port.set_option( PARITY );
    port.set_option( STOP );

    ask_data(port, db);
    string consult = "SELECT * FROM DATOS;";
    std::list<result_query>* l_original = consulta_datos(db, consult);
    std::list<result_query>* l = l_original;

    std::cout << "Size lista original: " << l_original->size() << '\n';
    std::cout << "Size lista copia: " << l->size() << '\n';


    while(!l->empty()){

      result_query fila = l->front();

      std::cout << "------------------------------" << '\n';
      cout << fila.getId_name() << endl;
      cout << fila.getHora() << endl;
      cout << fila.getFecha() << endl;
      cout << fila.getTemperatura() << endl;
      cout << fila.getHumedad() << endl;
      cout << fila.getCelula_fotoelectrica() << endl;
      cout << fila.getEstado() << endl;
      std::cout << "------------------------------" << '\n';

      l->pop_front();

    }

  unsigned char input[1];

  do{
      cin >> input[0];


    if(input[0] == 't' || input[0] == 'h' || input[0] == 's'){
      as::write(port, as::buffer(input, 1));
      sleep(1);
      cout << atof(read_serial(port).c_str());
    }else continue;
  }while(input[0] != 'q');

  sqlite3_close(db);
  return 0;
}*/
