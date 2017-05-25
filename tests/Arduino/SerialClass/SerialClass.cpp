#include "SerialClass.h"


FILE *Serial::getHandler() const
{
    return handler;
}

void Serial::setHandler(FILE *value)
{
    handler = value;
}

Serial::Serial(std::string portName)
{
    setConnected(false);
    setPort(portName);

    if(handler = fopen(getPort().c_str(), "w+")){
        setConnected(true);
    }
}


Serial::~Serial()
{
    fclose(handler);
}
std::string Serial::getLast_data() const
{
    return last_data;
}

void Serial::setLast_data(const std::string &value)
{
    last_data = value;
}

bool Serial::getConnected() const
{
    return connected;
}

void Serial::setConnected(bool value)
{
    connected = value;
}


std::string Serial::getPort() const
{
    return port;
}

void Serial::setPort(const std::string &value)
{
    port = value;
}

bool Serial::IsConnected()
{
    return getConnected();
}
