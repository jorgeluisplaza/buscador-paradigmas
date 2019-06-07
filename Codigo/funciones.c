#include "cabecera.h"

StopWords* loadStopWords(char* pathStopWordsFile,enum code *statusCode){
    *statusCode = OK;
    FILE* listaStopWords;
    StopWords * palabrasDesechables;
    char auxiliar[64];
    palabrasDesechables = (StopWords*)malloc(sizeof(StopWords));
    listaStopWords = fopen(pathStopWordsFile,"r");
    if(listaStopWords == NULL){ //OCURRE CUANDO EL DIRECTORIO ES INCORRECTO O EL ARCHIVO NO EXISTE
        *statusCode = ERR_FILE_NOT_FOUND;
        return 0;
    }
    int i = 0;
    while(!feof(listaStopWords)){ //CICLO HASTA LLEGAR AL FINAL DEL ARCHIVO
        fgets(auxiliar, 128, listaStopWords); //SE OBTIENE UNA LINEA
        if(auxiliar[strlen(auxiliar)-1] == '\n'){ //SE QUITAN LOS SALTOS DE LINEA 
            auxiliar[strlen(auxiliar)-1] = '\0';
        }
        strcpy(palabrasDesechables->listaPalabras[i], auxiliar); //FUNCION QUE AGREGA LA PALABRA A LA LISTA
        i++;
    }
    palabrasDesechables->cantidadPalabras = i;
    fclose(listaStopWords);
    *statusCode = OK;
    return palabrasDesechables;
}

Index* createIndex(char * pathDocumentsFile, StopWords *sw, enum code * statusCode){
    FILE * listaDocumentos;
    listaDocumentos = (FILE*)malloc(sizeof(FILE));
    listaDocumentos = fopen(pathDocumentsFile, "r");
    if(listaDocumentos == NULL){ //CUANDO EL DIRECTORIO ES INCORRECTO O EL ARCHIVO NO EXISTE
        *statusCode = ERR_FILE_NOT_FOUND;
        return 0;
    }
    int ID = 0;
    Index * indice;
    indice = (Index*)malloc(sizeof(Index));
    indice->documentos = (Result*)malloc(sizeof(Result)*256);
    char linea[128];
    Result documento; 
    documento.cantLineasTitulo = 0;
    while(!feof(listaDocumentos)){
        fgets(linea, 128, listaDocumentos);
        while(linea[1] != 'I' && !feof(listaDocumentos)){ //GUARDA LAS LINEAS DEL TITULO
            if(linea[1] == 'T'){
                int k = 0;
                while(linea[1] != 'A'){
                    strcpy(documento.titulo[k], linea);
                    fgets(linea, 128, listaDocumentos);
                    k++;
                }
                documento.cantLineasTitulo = k;
            if(linea[1] == 'A'){ //GUARDA LAS LINEAS DEL AUTOR
                int h = 0;
                while(linea[1] != 'B'){
                    strcpy(documento.autores, linea);
                    fgets(linea, 128, listaDocumentos);
                    h++;
                }
                documento.cantLineasAutores = h;
            }
            if(linea[1] == 'B'){ //GUARDA LAS LINEAS DE LA FECHA O LUGAR
                int x = 0;
                while(linea[1] != 'W'){
                    strcpy(documento.fecha[x], linea);
                    fgets(linea, 128, listaDocumentos);
                    x++;
                }
                documento.cantLineasFecha = x;
            }
            if(linea[1] == 'W'){ //GUARDA LAS LINEAS DEL EXTRACTO
                int j = 0;
                while(linea[1] != 'I' && !feof(listaDocumentos)){
                    strcpy(documento.extracto[j], linea);
                    fgets(linea, 128, listaDocumentos);
                    j++;
                }
                documento.cantLineasExtracto = j;
            }
        }
        }
        if(documento.cantLineasTitulo != 0){
            documento.ID = ID;
            indice->documentos[ID] = documento;
            ID++;
        }
    }
    indice->cantidadDeDocumentos = ID;
    obtenerListaPalabras(indice, sw); //SE LLAMA A LA FUNCION OBTENERLISTAPALABRAS PARA OBTENER LAS PALABRAS QUE OBTIENE TODAS LAS PALABRAS DEL DOCUMENTO
    fclose(listaDocumentos);
    return indice;
}

void obtenerListaPalabras(Index *in, StopWords *sw){
    char listaPalabras[16384][64];
    int i,j;
    Index * indiceAux;
    indiceAux = (Index*)malloc(sizeof(Index));
    indiceAux->documentos = (Result*)malloc(sizeof(Result)*256);
    int auxPalabras = 0;
    //char * aux;
    //aux = (char*)malloc(sizeof(char)*100);
    for(i = 0; i < in->cantidadDeDocumentos; i++){
        Result documentoAux;
        Result documentoAux2;
        int k = 0;
        documentoAux = in->documentos[i];
        for(j = 0; j < documentoAux.cantLineasTitulo; j++){
            if(documentoAux.titulo[j][1] != 'T'){ //REVISA CADA TITULO
                char * aux;
                aux = (char*)malloc(sizeof(char)*100);
                aux = strtok(documentoAux.titulo[j], " :/.,-\n");
                if(eliminarPalabra(aux, sw) == 0){ //ELIMINAR PALABRA SE ENCARGA DE VALIDAR SI ES STOPWORD
                    //Hace nada
                }
                else{
                    strcpy(listaPalabras[auxPalabras], aux);
                    strcpy(documentoAux2.listaPalabras[k], aux);
                    strcpy(in->documentos[i].listaPalabras[k], aux); //SE AGREGA A LA LISTA DE PALABRAS DE DOCUMENTO
                    strcpy(in->listaFinalPalabras[auxPalabras],aux); //SE AGREGA A LA LISTA TOTAL
                    auxPalabras++;
                    k++;
                }
                while(*aux != '\0'){
                    //printf("%s \n", aux);
                    aux = strtok(NULL, " /:,.-\n");
                    if(aux == NULL){
                        break;
                    }
                    else if(eliminarPalabra(aux, sw) == 0){
                            //No hace nada, solo se salta la palabra por ser StopWord.
                            }
                    else{
                        strcpy(listaPalabras[auxPalabras], aux);
                        strcpy(documentoAux2.listaPalabras[k], aux);
                        strcpy(in->documentos[i].listaPalabras[k], aux);
                        strcpy(in->listaFinalPalabras[auxPalabras],aux);
                        auxPalabras++;
                        k++;
                    }
                }
            }
        }
        documentoAux2.cantLineasTitulo = k;
        for(j = 0; j < documentoAux.cantLineasAutores-1; j++){ //REVISA CADA AUTOR
            if(documentoAux.autores[1] != 'A'){
                char * auxAutor;
                auxAutor = (char*)malloc(sizeof(char)*256);
                auxAutor = strtok(documentoAux.autores, " \n.");
                strcpy(listaPalabras[auxPalabras], auxAutor);
                strcpy(documentoAux2.autores, auxAutor);
                strcpy(in->listaFinalPalabras[auxPalabras],auxAutor);
                auxPalabras++;
            }
        }
        for(j = 0; j < documentoAux.cantLineasExtracto; j++){
            if(documentoAux.extracto[j][1] != 'W'){ //REVISA EL EXTRACTO
                char * auxExtracto;
                auxExtracto = (char*)malloc(sizeof(char)*1024);
                auxExtracto = strtok(documentoAux.extracto[j], " ():/.,-\n");
                if(eliminarPalabra(auxExtracto, sw) == 0){

                }
                else{
                    strcpy(listaPalabras[auxPalabras], auxExtracto);
                    strcpy(documentoAux2.listaPalabras[k], auxExtracto);
                    strcpy(in->documentos[i].listaPalabras[k], auxExtracto);
                    strcpy(in->listaFinalPalabras[auxPalabras],auxExtracto);
                    auxPalabras++;
                    k++;
                }
                while(auxExtracto != NULL){
                    //printf("%s \n");
                    auxExtracto = strtok(NULL, " ()/:,.-\n");
                    if(auxExtracto == NULL){
                        break;
                    }
                    else if(eliminarPalabra(auxExtracto, sw) == 0){
                        //No hace nada, solo evita que se agrege la palabra.
                    }
                    else{
                        strcpy(listaPalabras[auxPalabras], auxExtracto);
                        strcpy(documentoAux2.listaPalabras[k], auxExtracto);
                        strcpy(in->documentos[i].listaPalabras[k], auxExtracto);
                        strcpy(in->listaFinalPalabras[auxPalabras],auxExtracto);
                        auxPalabras++;
                        k++;
                    }
                }
                //printf("%i \n", auxPalabras);
            }
        }
        documentoAux2.cantidadPalabras = k;
        in->documentos[i].cantidadPalabras = k;
        documentoAux2.ID = i;
        indiceAux->documentos[i] = documentoAux2;
    }
    in->largoLista = auxPalabras;
    indiceAux->cantidadDeDocumentos = i;
    eliminarDuplicados(listaPalabras, auxPalabras, in); //ELIMINAR DUPLICADOS QUITA LOS ELEMENTOS DUPLICADOS
}

int eliminarPalabra(char * palabra, StopWords *sw){
    int i;
    for(i = 0; i < sw->cantidadPalabras; i++){
        if(strcmp(sw->listaPalabras[i], palabra) == 0){
            return 0;
        }
    }
    return 1;
}

void eliminarDuplicados(char lista[16384][64], int largo, Index * indiceFinal){
    int i, j ,k;
    for(i = 0; i < largo; i++){
        for(j = i + 1; j < largo; j++){ //ALGORITMO DE BUSQUEDA
            if(strcmp(indiceFinal->listaFinalPalabras[i], indiceFinal->listaFinalPalabras[j]) == 0){
                for(k = j; k < largo; k++){
                    strcpy(indiceFinal->listaFinalPalabras[k], indiceFinal->listaFinalPalabras[k+1]);
                }
                largo--;
            }
        }
    }
    indiceFinal->largoLista = largo;
    crearMatriz(lista, largo, indiceFinal); //SE LLAMA CREAR MATRIZ PARA CREAR LA MATRIZ
}

void crearMatriz(char lista [5000][64], int largo, Index * indice){
    indice->matrizIncidencia = (int**)malloc(sizeof(int*)*largo);
    int i,j, k;
    for(i = 0; i < largo ; i++){
        indice->matrizIncidencia[i] = (int*)malloc(sizeof(int)*largo);
    }
    for(i = 0; i < indice->cantidadDeDocumentos; i++){
        for(j = 0; j < largo; j++){
            indice->matrizIncidencia[j][i] = 0; //SE INICIA CON CEROS
        }
    }
    for(i = 0; i < indice->cantidadDeDocumentos; i++){
        for(j = 0; j < largo; j++){
            for(k = 0; k < indice->documentos[i].cantidadPalabras; k++){
                if(strcmp(lista[j], indice->documentos[i].listaPalabras[k]) == 0){
                    indice->matrizIncidencia[j][i]++; //SUMA EL VALOR DE LA MATRIZ
                }
            }
        }
    }
}

void saveIndex(Index *i, int *id , enum code *statusCode){
    FILE * guardar;
    srand(time(NULL));
    *id = rand() % 10;
    printf("El identificador del indice es %i \n", *id);
    char * fecha = (char*)malloc(sizeof(char)*25);
    time_t tiempo;
    tiempo = time(NULL);
    sprintf(fecha, "%s", asctime(localtime(&tiempo)));
    printf("fecha en que se guardo el indice: %s", fecha);
    guardar = fopen("Indice.txt", "w");
    int h,j;
    for(h = 0; h < i->cantidadDeDocumentos; h++){ //FOR PRINCIPAL QUE DEPENDE DEL NUMERO DE DOCUMENTOS
        fprintf(guardar, "%s", ".I "); //SE COLOCA EL ".I"
        if(h == 0){
            fprintf(guardar, "%i ", *id); //SE GUARDA EL ID A CONTINUACION DEL .I
        }
        fprintf(guardar, "%i", h+1); //SE COLOCAR  EL NUMERO DE DOCUMENTO
        fprintf(guardar, "%s", "\n");
        for(j = 0; j < i->documentos[h].cantLineasTitulo; j++){ //SE COLOCA EL TITULO
            fprintf(guardar, "%s", i->documentos[h].titulo[j]);
        }
        for(j = 0; j < i->documentos[h].cantLineasAutores; j++){ //SE COLOCA EL AUTOR
            if(j == 0){
                fprintf(guardar, "%s", ".A\n");
            }
            else{
            fprintf(guardar, "%s", i->documentos[h].autores);
            }
        }
        for(j = 0; j < i->documentos[h].cantLineasFecha; j++){//SE COLOCA LA FECHA
            fprintf(guardar, "%s", i->documentos[h].fecha[j]);
        }
        for(j = 0; j < i->documentos[h].cantLineasExtracto; j++){//SE COLOCA EL EXTRACTO
            fprintf(guardar, "%s", i->documentos[h].extracto[j]);
        }
    }
    fclose(guardar);
    guardar = fopen("matrizIncidencia.txt", "w");
    for(h = 0; h < i->largoLista; h++){
        for(j = 0; j < i->cantidadDeDocumentos; j++){
            fprintf(guardar, "%i ", i->matrizIncidencia[h][j]);
        }
        fprintf(guardar, "%s", "\n");
    }
    fclose(guardar);
    guardar = fopen("ListaPalabras.txt", "w");
    for(h = 0; h < i->largoLista; h++){
        fprintf(guardar, "%s\n", i->listaFinalPalabras[h]);
    }
    fclose(guardar);
}

Index* loadIndex(int id , enum code * statusCode){
    Index * indice;
    char * directorioIndiceAuxiliar = "Indice.txt";
    char * directorioStopWordsAuxiliar = "StopWords.txt";
    FILE * archivo;
    StopWords * sw;
    sw = loadStopWords(directorioStopWordsAuxiliar, statusCode);
    archivo = fopen("Indice.txt", "r");
    int linea;
    char basura;
    fscanf(archivo, "%c", &basura);
    fscanf(archivo, "%c", &basura);
    fscanf(archivo, "%c", &basura);
    fscanf(archivo, "%i", &linea);
    if(linea == id){
        indice = createIndex(directorioIndiceAuxiliar, sw, statusCode); //SE CREA EL INDICE CON EL NUEVO
        fclose(archivo);
        *statusCode = OK;
        return indice;
    }
    else{
        *statusCode = ERR_ID_NOT_CORRECT; //ERROR SI EL IDENTIFICADOR NO ES CORRECTO
        fclose(archivo);
    }
    fclose(archivo);
    return 0;
}

Ranking * query(Index *i, StopWords *sw, char * text, enum code * StatusCode){ 
    int h,j,k;
    Result documentoAux;
    Ranking * ranking;
    ranking = (Ranking*)malloc(sizeof(Ranking));
    ranking->cantidadDocumentos = i->cantidadDeDocumentos;
    char listaPalabras[64][64];
    int largoLista = 0;
    char * aux;
    aux = (char*)malloc(sizeof(char)*32);
    aux = strtok(text, " :/.,-\n"); //SEPARA EL TEXTO
    if(eliminarPalabra(aux, sw) == 0){ //FUNCION ELIMINARPALABRA
        //hacer nada;
    }
    else{
        strcpy(listaPalabras[largoLista], aux);
        largoLista++;
    }
    while(*aux != '\0'){
        aux = strtok(NULL, " /:,.-\n");
        if(aux == NULL){
            break;
        }
        else if(eliminarPalabra(aux, sw) == 0){
            //No hace nada, solo se salta la palabra por ser StopWord.
        }
        else{
            strcpy(listaPalabras[largoLista], aux);
            largoLista++;
        }
    }
    for(h = 0; h < i->cantidadDeDocumentos; h++){
        i->documentos[h].repeticionesPalabras = 0;
    }
    for(h = 0; h < i->cantidadDeDocumentos ; h++){
        for(j = 0; j < i->largoLista; j++){
            for(k = 0; k < largoLista; k++){
                if(strcmp(i->listaFinalPalabras[j], listaPalabras[k]) == 0){
                    i->documentos[h].repeticionesPalabras = i->documentos[h].repeticionesPalabras + i->matrizIncidencia[j][h]; //SUMA EL VALOR A REPETICIONESPALABRA DEL DOCUMENTO
                }
            }
        }
    }
    ranking->listaDocumentos = i->documentos;
    for(j = 0; j < ranking->cantidadDocumentos; ++j){
        for(h = j + 1; h < ranking->cantidadDocumentos; ++h){
            if(ranking->listaDocumentos[j].repeticionesPalabras < ranking->listaDocumentos[h].repeticionesPalabras){
            documentoAux = ranking->listaDocumentos[j];
            ranking->listaDocumentos[j] = ranking->listaDocumentos[h];
            ranking->listaDocumentos[h] = documentoAux;
            }
        }
    }
    return ranking;
}

void displayResults(Ranking *r, int TopK, enum code * statusCode){
    int i,j;
    //if(r->listaDocumentos[0].repeticionesPalabras == 0){ //ERROR SI NO ENCUENTRA RESULTADO
        //*statusCode = ERR_NO_SEARCH;
    //}
    for(i = 0; i < TopK; i++){
        printf("%s", ".I ");
        printf("%i \n", r->listaDocumentos[i].ID+1);
        for(j = 0; j < r->listaDocumentos[i].cantLineasTitulo; j++){
            printf("%s", r->listaDocumentos[i].titulo[j]);
        }
        for(j = 0; j < r->listaDocumentos[i].cantLineasAutores; j++){
            if(j == 0){
                printf("%s", ".A\n");
            }
            else{
                printf("%s", r->listaDocumentos[i].autores);
            }
        }
        for(j = 0; j < r->listaDocumentos[i].cantLineasFecha; j++){
            printf("%s", r->listaDocumentos[i].fecha[j]);
        }
        for(j = 0; j < r->listaDocumentos[i].cantLineasExtracto; j++){
            printf("%s", r->listaDocumentos[i].extracto[j]);
        }
    }
}
