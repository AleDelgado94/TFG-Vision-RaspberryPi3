#include <iostream>
#include <boost/asio.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>
#include "../../Database/sqlite3/sqlite3.h"

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


int main(int argc, char const *argv[]){
  io_service io;
  string PORT;
  sqlite3* db;

  int db_handler = sqlite3_open("../../Database/database.db", &db);
  if(db_handler){
    cout << "Se abrió la base de datos" << endl;
  }else{
    cout << "Error al abrir la base de datos" << endl;
  }


  opt::options_description desc("Options");
  desc.add_options()
    ("help,h", "./Temperature [[ --port | -p ] puerto]")
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
    if(vm.count("port")){
      std::vector<std::string> v = vm["port"].as<std::vector<std::string>>();
      PORT = v[0];
    }


    serial_port port( io, PORT.c_str() );
    // Setup port - base settings
    port.set_option( BAUD );
    port.set_option( FLOW );
    port.set_option( PARITY );
    port.set_option( STOP );





  unsigned char input[1];

  do{
      cin >> input[0];


    //opcion[0] = static_cast<unsigned char>( input );
    if(input[0] == 't' || input[0] == 'h' || input[0] == 's'){
      as::write(port, as::buffer(input, 1));
      sleep(1);
      cout << atof(read_serial(port).c_str());
    }else continue;
  }while(input[0] != 'q');

  return 0;
}
