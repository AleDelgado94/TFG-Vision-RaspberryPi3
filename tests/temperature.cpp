#include "temperature.h"

Temperature::Temperature(std::string port)
{
    arduino1 = new Serial(port.c_str());
    if(!arduino1->IsConnected()){
        cout << "Error to connect with arduino. Exit..." << endl;
        exit(-1);
    }
}


std::string Temperature::getData() const
{
    return data;
}

void Temperature::setData(const std::string &value)
{
    data = value;
}

int Temperature::getTime_capture() const
{
    return time_capture;
}

void Temperature::setTime_capture(int value)
{
    time_capture = value;
}

std::string Temperature::getPort() const
{
    return port;
}

void Temperature::setPort(const std::string &value)
{
    port = value;
}


int main(int argc, char *argv[])
{
    Temperature *t = new Temperature("/dev/ttyACM0");
    return 0;
}



