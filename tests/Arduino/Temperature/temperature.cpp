#include <iostream>
#include <boost/asio.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>

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





  unsigned char input;

  while(input != 'q'){
      cin >> input;
    unsigned char command[1] = {0};

    // Convert and send
    command[0] = static_cast<unsigned char>( input );
    if(command[0] == 't' || command[0] == 'h'){
      as::write(port, as::buffer(command, 1));
      sleep(0.5);
      // Receive response —---------------------------------------------------—
      //read(port,as::buffer(&data,1));
      cout << read_serial(port);
    }else continue;
  }

  return 0;
}
