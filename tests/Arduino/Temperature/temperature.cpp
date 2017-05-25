#include "temperature.h"

Temperature::Temperature(std::string port)
{
    arduino1 = new Serial(port.c_str());
    if(!arduino1->IsConnected()){
        cout << "Error to connect with arduino. Exit..." << endl;
        exit(-1);
    }
}

Temperature::~Temperature()
{
    delete arduino1;
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

int Temperature::getTemperature(int i)
{
    fprintf(arduino1->getHandler(), "%c", 't');
    sleep(2);
    int temperatura;
    fscanf(arduino1->getHandler(), "%d", &temperatura);
    if(i != 0) cout << "La temperatura es de " << temperatura << " ºC" << endl;
    //delete this;

}

int Temperature::getHumidity(int i)
{
    fprintf(arduino1->getHandler(), "%c", 'h');
    sleep(2);
    int humedad;
    fscanf(arduino1->getHandler(), "%d", &humedad);
    if(i != 0) cout << "La humedad es de " << humedad << " %" << endl;
    //delete this;
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

    char option;
    Temperature *t = new Temperature("/dev/ttyACM0");
    t->getTemperature(0);
    t->getHumidity(0);
    delete t;

    while(option != 'q'){

        cout << endl << "Temperature (t)\nHumedad (h)" << endl;
        cout << "Introduce opcion: ";
        cin >> option;
        cout << endl;

        if(option == 'h'){
            t = new Temperature("/dev/ttyACM0");
            t->getHumidity(1);
            delete t;
        }else if(option == 't'){
            t = new Temperature("/dev/ttyACM0");
            t->getTemperature(1);
            delete t;

        }

    }

    return 0;
}



