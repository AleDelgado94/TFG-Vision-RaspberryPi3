#include <iostream>

#include "SerialClass/SerialClass.h"

using namespace std;


class Temperature {
private:
  /* data */
  Serial* arduino1;
  std::string port; // dev/ttyACM0
  std::string data;
  int time_capture;
public:
  Temperature (std::string port);
  ~Temperature(void);



  std::string getPort() const;
  void setPort(const std::string &value);
  std::string getData() const;
  void setData(const std::string &value);
  int getTime_capture() const;
  void setTime_capture(int value);
};
