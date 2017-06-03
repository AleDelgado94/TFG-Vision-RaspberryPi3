#include <iostream>
#include <vector>
#include <string>

using namespace std;


struct result_query{
  const unsigned char* id_name;
  const unsigned char* hora;
  const unsigned char* fecha;
  float temperatura;
  float humedad;
  float celula_fotoelectrica;
  const unsigned char* estado;
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
