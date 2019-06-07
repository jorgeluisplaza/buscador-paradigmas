#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//SE RESPETA LA VARIABLE ENUM, SOLO SE LE AGREGARON ALGUNOS VALORES
enum code{OK, ERR_FILE_NOT_FOUND, ERR_FILE_NOT_PERM, ERR_NO_SEARCH, ERR_ID_NOT_CORRECT, ERR_EMPTY_VARIABLE};

//ESTRUCTURAS
typedef struct Result{ 
    int ID;
    char titulo[128][128]; //GUARDA LAS LINEAS DEL TITULO
    char autores[128];  //GUARDA LA LINEA DEL AUTOR
    char extracto[128][128]; //GUARDA LAS LINEAS DEL EXTRACTO
    char fecha[128][128]; //GUARDA LAS LINEAS DE LA FECHA O LUGAR
    int ranking;
    int cantLineasTitulo;
    int cantLineasExtracto;
    int cantLineasAutores;
    int cantLineasFecha;
    char listaPalabras[4096][64]; //LISTA DE LAS PALABRAS EN EL DOCUMENTO
    int cantidadPalabras; 
    int repeticionesPalabras; //SE USA PARA SABER LA CANTIDAD DE VECES QUE SE REPITEN LAS PALABRAS DE LA CONSULTA
} Result;

typedef struct Index{
    Result * documentos; //PUNTERO A DOCUMENTOS, CADA POSICION DE LA MEMORIA GUARDA UN DOCUMENTO
    int cantidadDeDocumentos;
    int ** matrizIncidencia; //MATRIZ DE INCIDENCIA, FILAS SON PALABRAS, COLUMNAS DOCUMENTOS
    char listaFinalPalabras[16384][64]; //LISTA CON LAS PALABRAS SIN STOPWORD NI DUPLICADOS
    int largoLista;
    int ID;
} Index;

typedef struct Ranking{
    Result * listaDocumentos; //LISTA CON LOS DOCUMENTOS ORDENADOS DE MAYOR A MENOS DEPENDIENDO DE LA CONSULTA
    int cantidadDocumentos;
} Ranking;

typedef struct StopWords{
    char listaPalabras[512][512]; //LISTA DE STOPWORDS
    int cantidadPalabras;
} StopWords;

//DECLARACION DE LAS FUNCIONES DEL CODIGO
void obtenerListaPalabras(Index *in, StopWords *sw);
int eliminarPalabra(char * palabra, StopWords *sw);
void eliminarDuplicados(char lista[16384][64], int largo, Index * indiceFinal);
void crearMatriz(char lista [5000][64], int largo, Index * indice);
StopWords* loadStopWords(char* pathStopWordsFile,enum code *statusCode);
Index* createIndex(char * pathDocumentsFile, StopWords *sw, enum code * statusCode);
void saveIndex(Index *i, int *id , enum code *statusCode);
Index* loadIndex(int id , enum code * statusCode);
Ranking * query(Index *i, StopWords *sw, char * text, enum code * StatusCode);
void displayResults(Ranking *r, int TopK, enum code *statusCode);
void saveRanking(Ranking *r, int *id, enum code *statusCode);
Ranking* loadRanking(int id, enum code * statusCode);
char * didYouMean(Index * indice, char * consulta, enum code *statusCode);
