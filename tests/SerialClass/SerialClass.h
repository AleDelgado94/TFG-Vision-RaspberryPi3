
#define ARDUINO_WAIT_TIME 2000

#include <stdio.h>
#include <stdlib.h>

class Serial
{
    private:
        std::string port;
        FILE *handler;
        std::string last_data;
        bool connected;

    public:
        //Initialize Serial communication with the given COM port
        Serial(std::string portName);
        //Close the connection
        //NOTA: for some reason you can't connect again before exiting
        //the program and running it again
        ~Serial();



        std::string getLast_data() const;
        void setLast_data(const std::string &value);
        bool getConnected() const;
        void setConnected(bool value);
        std::string getPort() const;
        void setPort(const std::string &value);
        bool IsConnected();
};

