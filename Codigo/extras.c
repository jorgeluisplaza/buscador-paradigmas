#include "cabecera.h"

void saveRanking(Ranking *r, int *id, enum code *statusCode){
	if(r == NULL){
		*statusCode = ERR_EMPTY_VARIABLE;
		return;
	}
	FILE * archivoRanking;
	archivoRanking = fopen("archivoRanking.txt", "w");
	*id = rand()%10;
	char * fecha = (char*)malloc(sizeof(char)*25);
	time_t tiempo;
	tiempo = time(NULL);
	sprintf(fecha, "%s", asctime(localtime(&tiempo)));
	printf("Fecha al guardar el ranking: %s", fecha);
	printf("El identificador del ranking es: %i\n", *id);
	int h,j;
	fprintf(archivoRanking, "%i\n", *id);
	for(h = 0; h < r->cantidadDocumentos; h++){
		fprintf(archivoRanking, "%s\n", ".I ");
		for(j = 0; j < r->listaDocumentos[h].cantLineasTitulo; j++){
			fprintf(archivoRanking, "%s", r->listaDocumentos[h].titulo[j]);
		}
		for(j = 0; j < r->listaDocumentos[h].cantLineasAutores; j++){
			if(j == 0){
				fprintf(archivoRanking, "%s\n", ".A");
			}
			else{
				fprintf(archivoRanking, "%s", r->listaDocumentos[h].autores);
			}
		}
		for(j = 0; j < r->listaDocumentos[h].cantLineasFecha; j++){
			fprintf(archivoRanking, "%s", r->listaDocumentos[h].fecha[j]);
		}
		for(j = 0; j < r->listaDocumentos[h].cantLineasExtracto; j++){
			fprintf(archivoRanking, "%s", r->listaDocumentos[h].extracto[j]);
		}
	}
	fclose(archivoRanking);
}

Ranking* loadRanking(int id, enum code * statusCode){
	FILE * archivoRanking;
	Ranking * rankingAux;
	rankingAux = (Ranking*)malloc(sizeof(Ranking)*128);
	rankingAux->listaDocumentos = (Result*)malloc(sizeof(Result)*128);
	archivoRanking = fopen("archivoRanking.txt", "r");
	int linea;
	Result documento;
	int h = 0;
	char lineaAux[128];
	fscanf(archivoRanking, "%i", &linea);
	fgets(lineaAux, 128, archivoRanking);
	fgets(lineaAux, 128, archivoRanking);
	if(linea == id){
		while(!feof(archivoRanking)){
			rankingAux->listaDocumentos[h] = documento;
			fgets(lineaAux, 128, archivoRanking);
			while(lineaAux[1] != 'I' && !feof(archivoRanking)){
				if(lineaAux[1] == 'T'){
					int k = 0;
					while(lineaAux[1] != 'A'){
						strcpy(rankingAux->listaDocumentos[h].titulo[k], lineaAux);
						fgets(lineaAux, 128, archivoRanking);
						k++;
					}
				}
				if(lineaAux[1] == 'A'){
					int k = 0;
					while(lineaAux[1] != 'B'){
						strcpy(rankingAux->listaDocumentos[h].autores, lineaAux);
						fgets(lineaAux, 128, archivoRanking);
						k++;
					}
				}
				if(lineaAux[1] == 'B'){
					int k = 0;
					while(lineaAux[1] != 'W'){
						strcpy(rankingAux->listaDocumentos[h].fecha[k], lineaAux);
						fgets(lineaAux, 128, archivoRanking);
						k++;
					}
				}
				if(lineaAux[1] == 'W'){
					int k = 0;
					while(lineaAux[1] != 'I' && !feof(archivoRanking)){
						strcpy(rankingAux->listaDocumentos[h].extracto[k], lineaAux);
						fgets(lineaAux, 128, archivoRanking);
						k++;
					}
				}
			}
			h++;
		}
	}
	else{
		*statusCode = ERR_ID_NOT_CORRECT;
	}
	fclose(archivoRanking);
	return rankingAux;
}

/*char * didYouMean(Index * indice, char * consulta, enum code *statusCode){
	printf("%s\n", consulta);
}*/
