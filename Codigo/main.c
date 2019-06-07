#include "cabecera.h"

int main()
{
    //MENU
    printf("Presione 0 para salir\nPresione 1 para cargar StopWords\nPresione 2 para crear el indice\nPresione 3 para guardar el indice\nPresione 4 para cargar el indice\nPresione 5 para realizar la busqueda\nPresione 6 para mostrar los resultados\nPresione 7 para guardar el ranking\nPresione 8 para cargar ranking\n");
    char * directorioStopWords;
    char * directorioDocumentos;
    directorioStopWords = (char*)malloc(sizeof(char)*100);
    directorioDocumentos = (char*)malloc(sizeof(char)*100);
    directorioStopWords = "StopWords.txt"; //CAMBIAR DIRECTORIO PARA COMPILAR
    directorioDocumentos = "TestCollection.txt"; //CAMBIAR PARA COMPILAR
    int *id; //IDENTIFICADOR DEL INDICE
    int *idRanking; //IDENTIFICADOR DEL RANKING
    idRanking = (int*)malloc(sizeof(int));
    Ranking * ranking;
    ranking = (Ranking*)malloc(sizeof(Ranking));
    char * texto; //PARAMETRO PARA LA CONSULTA
    //char * textoAux;
    texto = (char*)malloc(sizeof(char)*64);
    //textoAux = (char*)malloc(sizeof(char)*64);
    id = (int*)malloc(sizeof(int));
    int i;
    *id = 0;
    enum code * statusCode; //SE CREA EL ENUM
    statusCode = malloc(sizeof(*statusCode));
    Index * indice; //INDICE
    indice = (Index*)malloc(sizeof(Index));
    StopWords * sw; //STOPWORDS
    sw = (StopWords*)malloc(sizeof(StopWords));
    int identificadorAux, identificadorRankingAux, topK; //IDENTIFICADORES Y TOPK
    int aux;
    aux = 10;
    while(aux != 0){
        stdin = freopen(NULL,"r",stdin);
        fflush(stdin);
        printf("Ingrese numero: "); //PARA PEDIR NUMERO DEL MENU
        scanf(" %d", &aux);
        stdin = freopen(NULL,"r",stdin);
        if(aux == 0){
            continue;
        }
        else if(aux == 1){ //OPCION 1 CARGAR STOPWORD
            sw = loadStopWords(directorioStopWords, statusCode);
            if(*statusCode == OK){
                printf("StopWords cargados correctamente \n");
        }
            else{
                printf("El directorio es incorrecto o el archivo no existe \n");
                return 0;
            }
        }
        else if(aux == 2){ //OPCION 2 CREAR INDICE
            indice = createIndex(directorioDocumentos, sw, statusCode);
            if(*statusCode == OK){
                printf("Indice creado correctamente \n");
            }
            else if(sw == NULL){
                printf("Porfavor cargar StopWords antes de crear el indice\n");
            }
            else{
            printf("El directorio es incorrecto o el archivo no existe \n");
            return 0;
            }
        }
        else if(aux == 3){ //OPCION 3 GUARDAR INDICE
            saveIndex(indice, id, statusCode);
            if(*statusCode == OK){
                printf("Indice guardado correctamente en el archivo Indice.txt\n");
            }
            else{
                printf("Ocurrio un error \n");
                return 0;
            }
        }
        else if(aux == 4){ //OPCION 4 CARGAR INDICE
            printf("Ingrese el identificador del indice: ");
            scanf("%d", &identificadorAux);
            fflush(stdin);
            indice = loadIndex(identificadorAux , statusCode);
            if(*statusCode == OK){
                printf("Indice cargado correctamente \n");
            }
            else{
                printf("Identificador no es correcto \n");
            }
        }
        else if(aux ==5){ //OPCION 5 INGRESAR BUSQUEDA
            printf("Introduzca su busqueda: ");
            fgets(texto, 64, stdin);
            ranking = query(indice , sw, texto, statusCode);
            if(*statusCode == ERR_NO_SEARCH){
                printf("No se encontraron resultados \n");
                return 0;
            }
            else{
                printf("busqueda realizada con exito\n");
            }
        }
        else if(aux == 6){ //OPCION 6 MOSTRAR RESULTADOS
            printf("Introduzca el numero de resultados que desea: ");
            scanf("%d", &topK);
            fflush(stdin);
            displayResults(ranking, topK, statusCode);
            if(*statusCode == ERR_NO_SEARCH){
                printf("No se encontraron resultados \n");
                return 0;
            }
            else if(*statusCode == ERR_EMPTY_VARIABLE){
                printf("Porfavor cargar StopWords y/o crear el indice\n");
            }
        }
        else if(aux == 7){ //OPCION 7 GUARDAR RANKING (OPCIONAL)
            saveRanking(ranking, idRanking, statusCode);
            if(*statusCode == OK){
                printf("Ranking guardado correctamente\n");
            }
            else{
                printf("Ha ocurrido un error, intentelo nuevamente\n");
            }
        }
        else if(aux == 8){ //OPCION 8 CARGAR RANKING (OPCIONAL)
            printf("Ingrese el identificador del ranking: ");
            scanf("%d", &identificadorRankingAux);
            fflush(stdin);
            ranking = loadRanking(*idRanking, statusCode);
            if(*statusCode == OK){
                printf("Ranking cargado correctamente\n");
            }
            else{
                printf("Identificador no es correcto\n");
            }
        }
    }
    free(texto);
    free(indice->matrizIncidencia);
    free(indice);
    free(ranking->listaDocumentos);
    free(ranking);
    free(sw);
    free(statusCode);
    free(directorioDocumentos);
    free(directorioStopWords);
    free(id);
    free(idRanking);
    return 0;
}

