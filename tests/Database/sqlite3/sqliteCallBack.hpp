#include <iostream>
#include <vector>
#include <string>

using namespace std;

/*
struct result_query{
  const unsigned char* id_name;
  const unsigned char* hora;
  const unsigned char* fecha;
  float temperatura;
  float humedad;
  float celula_fotoelectrica;
  const unsigned char* estado;
};*/


class result_query {
private:
  const unsigned char* id_name;
  const unsigned char* hora;
  const unsigned char* fecha;
  float temperatura;
  float humedad;
  float celula_fotoelectrica;
  const unsigned char* estado;

public:
  result_query (){};
  virtual ~result_query (){};

  const unsigned char *getId_name() const;
  void setId_name(const unsigned char *value);
  const unsigned char *getHora() const;
  void setHora(const unsigned char *value);
  const unsigned char *getFecha() const;
  void setFecha(const unsigned char *value);
  float getTemperatura() const;
  void setTemperatura(float value);
  float getHumedad() const;
  void setHumedad(float value);
  float getCelula_fotoelectrica() const;
  void setCelula_fotoelectrica(float value);
  const unsigned char *getEstado() const;
  void setEstado(const unsigned char *value);
};

static int callback(void *data, int argc, char **argv, char **azColName){
    int i;
   fprintf(stderr, "%s: ", (const char*)data);
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}
/*
static int callback_one_data(void *data, int argc, char **argv, char **azColName){
   int i;
   std::vector<result_query>* query;// = static_cast<std::vector<result_query>*>(data);
   result_query fila;

   fila.id_name = argv[0] ? argv[0] : "NULL";
   fila.hora = argv[1] ? argv[1] : "NULL";
   fila.fecha = argv[2] ? argv[2] : "NULL";
   fila.temperatura = atof(argv[3]) ? atof(argv[3]) : 0;
   fila.humedad = atof(argv[4]) ? atof(argv[4]) : 0;
   fila.celula_fotoelectrica = atof(argv[5]) ? atof(argv[5]) : 0;
   fila.estado = argv[6] ? argv[6] : "NULL";




   query->push_back(fila);

   std::vector<result_query> &data = (std::vector<result_query>)query;
   return 0;
}*/

const unsigned char *result_query::getHora() const
{
return hora;
}

void result_query::setHora(const unsigned char *value)
{
hora = value;
}

const unsigned char *result_query::getFecha() const
{
return fecha;
}

void result_query::setFecha(const unsigned char *value)
{
fecha = value;
}

float result_query::getTemperatura() const
{
return temperatura;
}

void result_query::setTemperatura(float value)
{
temperatura = value;
}

float result_query::getHumedad() const
{
return humedad;
}

void result_query::setHumedad(float value)
{
humedad = value;
}

float result_query::getCelula_fotoelectrica() const
{
return celula_fotoelectrica;
}

void result_query::setCelula_fotoelectrica(float value)
{
celula_fotoelectrica = value;
}

const unsigned char *result_query::getEstado() const
{
return estado;
}

void result_query::setEstado(const unsigned char *value)
{
estado = value;
}

const unsigned char *result_query::getId_name() const
{
return id_name;
}

void result_query::setId_name(const unsigned char *value)
{
id_name = value;
}
