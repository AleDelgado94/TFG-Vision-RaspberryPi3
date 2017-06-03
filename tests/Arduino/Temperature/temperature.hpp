/*#include <iostream>
#include <boost/asio.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>
#include <sqlite3.h>
#include "../../Database/sqlite3/sqliteCallBack.hpp"
#include <list>*/

using namespace::boost::asio;
using namespace std;
namespace opt = boost::program_options;
namespace as = boost::asio;


// Base serial settings
/*serial_port_base::baud_rate BAUD(115200);
serial_port_base::flow_control FLOW( serial_port_base::flow_control::none );
serial_port_base::parity PARITY( serial_port_base::parity::none );
serial_port_base::stop_bits STOP( serial_port_base::stop_bits::one );*/


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

void ask_data(serial_port& port, sqlite3* db, std::string fecha, std::string hora, std::string id){

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
  std::string sql("INSERT INTO DATOS (ID_NOMBRE, HORA, FECHA, TEMPERATURA, HUMEDAD, SOLAR) VALUES (");
  sql += id;
  sql += ",";
  sql += hora;
  sql += ",";
  sql += fecha;
  sql += ",";
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
