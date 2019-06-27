/* Inclusion de las librerias. */
#include <stdio.h>		// Libreria estandar de C.
#include <stdlib.h>   	// Libreria necesaria para la generaracion de numeros aleatorios.
#include <time.h>		// Libreria necesaria para que los numeros aleatorios cambien en cada ejecucion, usando la fecha del sistema como semilla.

/* Definicion de las constantes. */
// El tablero requerido es de 15x15, contando los bordes las filas y columnas valdran 17.
#define FILAS 17
#define COLUMNAS 17

/* Prototipos de las funciones principales (requeridas en el guion). */
int menu(void); // Menu que solo deja salir una opcion valida para jugar.
void jugar_ordenador(int tablero_partida); // Funcion principal de juego solo.
int jugar_contra_otro(void); // Funcion principal de juego multijugador.
void inicializar_tablero(int tablero[FILAS][COLUMNAS]);  // Funcion encargada de inicializar el tablero.
int selecciona_casilla(int tablero[FILAS][COLUMNAS], int fila, int columna); // Funcion que determina si una posicion de movimiento es valida.
void cargar_tablero_partida(int tablero[FILAS][COLUMNAS]); // Funcion que inicializa el tablero con uno introducido desde un archivo introducido como parametro.
void guardar_tablero_fichero(int tablero[FILAS][COLUMNAS], char nombre_fichero[25]); // Funcion encargada de guardar un tablero leible a un fichero.

/* Prototipos de las funciones extras. */
void inicializar_tablero_multijugador(int tablero[FILAS][COLUMNAS]); // Funcion encargada de inicializar el tablero si la partida es con dos usuarios.
int selecciona_casilla_multijugador(int tablero[FILAS][COLUMNAS], int fila, int columna, int serpiente); // Funcion encargada de comprobar si una casilla es valida en el modo jugar_contra_otro. 
void imprimir_tablero(int tablero[FILAS][COLUMNAS]); // Funcion que imprime el tablero para enseñar al usuario.

/* Definicion de las variables globales. */
FILE *fichero_salida;	// Fichero de salida de informacion.
FILE *fichero_entrada;  // Fichero con tablero para importar.

int main(int argc, char* argv[]){
	srand(time(NULL));
	/* Comprobacion de parametros iniciales, el numero de parametros puede ser 2 o 3 en funcion de si existe fichero de entrada. */
	int tablero_partida;
	if(argc != 2 && argc != 3 ){
		// Si el numero de argumentos no es ni dos ni tres detengo el juego.
		printf("Error en el número de argumentos (%d).\n", argc-1);
		return -1;  
	}
	else{
		// Si el numero de argumentos es dos o tres intento abrir el fichero de salida.
		fichero_salida = fopen(argv[1],"w");
        if(fichero_salida==NULL){
        	// Si no se puede abrir el fichero lanzo un mensaje de error y detengo el juego.
			printf("Error: No se pudo abrir el archivo de escritura.\n");
			return -2;
		} 
		/* Dependiendo si los argumentos eran 2 o 3 se establece la variable tablero_partida. */
		if(argc == 2){
			tablero_partida = 0;
		}
		else{
			// Si los argumentos son 3, intento abrir el fichero de entrada.
			fichero_entrada = fopen(argv[2],"r");
			if( fichero_entrada == NULL ){
				printf("Error: No se pudo abrir el fichero de entrada.\n");
				return -3;
			}
			tablero_partida = 1;
		}
	}

	/* Ejecucion de la aplicacion */
	int opcion;
	int perdedor;
	do{
		// Llamo a la funcion menu y en funcion del resultado realizo una accion.
		opcion = menu();   
		switch(opcion){
			case 1:
				// Llamo a la funcion jugar solo.
				printf("La elección del usuario ha sido jugar contra el ordenador.\n");
				fprintf(fichero_salida, "La elección del usuario ha sido jugar contra el ordenador.\n");
				jugar_ordenador(tablero_partida);	
				// Al acabar la ejecucion, cierro y vuelvo a abrir el fichero de entrada (si existia) para poder volver a leer los valores.
				if(fichero_entrada != NULL){
				 	fclose(fichero_entrada);
				 	fichero_entrada = fopen(argv[2],"r");
				}				
			break;
			case 2:
				// Llamo a la funcion jugar contra otro que devuelve el valor del perdedor.
				printf("La elección del usuario ha sido jugar contra otro usuario.\n");
				fprintf(fichero_salida, "La elección del usuario ha sido jugar contra otro usuario.\n"); 
				perdedor = jugar_contra_otro();	
				// Si el perdedor vale 1, el jugador ganador es el 2 y viceversa.
				if(perdedor == 1){
					printf("Ganó el jugador 2.\n");
					fprintf(fichero_salida, "Ganó el jugador 2.\n"); 
				}
				else{
					printf("Ganó el jugador 1.\n");
					fprintf(fichero_salida, "Ganó el jugador 1.\n"); 
				}
			break;
			case 3:
				// Salgo del programa.
				fprintf(fichero_salida, "La eleccion del usuario ha sido salir del programa\n");
				printf("El programa ha finalizado.");
				fprintf(fichero_salida, "El programa ha finalizado.");
			break;										
		}
		// Imprimo dos saltos de linea para que en la ejecucion y en el fichero haya una separacion entre ejecuciones
		printf("\n");
		fprintf(fichero_salida, "\n"); 
	}while(opcion != 3);

	/* Antes de salir del juego se cierran los ficheros */
	fclose(fichero_salida);
	fclose(fichero_entrada);
	return 0;  
}


int menu(void){
	int opcion;
	/* Se muestra una lista de opciones que solo avanza cuando la opcion elegida esta entre 1 y 3. */
	do{	
		printf("Bienvenido a Snake.\n");
		printf ("Elige una opcion:\n");
		fprintf(fichero_salida, "Elige una opcion:\n");  
		printf("1) Juego contra el ordenador.\n");
		fprintf(fichero_salida, "1) Juego contra el ordenador.\n"); 
		printf("2) Juego contra otro usuario.\n");
		fprintf(fichero_salida, "2) Juego contra otro usuario.\n"); 
		printf("3) Salir del programa.\n");
		fprintf(fichero_salida, "3) Salir del programa.\n");  
		scanf("%d", &opcion);
		fprintf(fichero_salida, "Se ha seleccionado la opcion %d\n", opcion);
		if( (opcion !=1 ) && (opcion != 2) && (opcion != 3)){
			// Si la opcion no es ni 1 ni 2 ni 3 repito el menu
			printf("Error: Valor no valido (%d).\n", opcion);
			printf("Relanzando menu\n\n");
			fprintf(fichero_salida, "Error: Valor no valido (%d).\n", opcion);
			fprintf(fichero_salida, "Relanzando menu.\n\n");
		}
	}while (opcion!=1 && opcion!=2 && opcion!=3);

	/* Se devuelve la opcion elegida (Siempre entre 1 y 3). */
	return opcion;
}

void imprimir_tablero(int tablero[FILAS][COLUMNAS]){
    int i,j;
    /* Se imprime la cabecera */
    printf("\t\t\t      ");		
	fprintf(fichero_salida, "\t\t\t      ");		
	for (j=1; j<=COLUMNAS; j++){
		// En funcion de si el valor es de uno o dos digitos imprimo o no un espacio.
		if(j<9){ 
			printf(" %d ", j+1);
			fprintf(fichero_salida, " %d ", j+1);		
		}
		else{
			printf("%d ", j+1);
			fprintf(fichero_salida, "%d ", j+1);		
		}
	}
	printf("\n");
	fprintf(fichero_salida, "\n");		

	/* Se imprime el tablero */
	for (i=0; i<FILAS; i++){
		// Primero pongo al principio de la fila un numero para indicar en que fila esta.
		if(i<9){
			printf("\t\t\t %d--->", i+1);
			fprintf(fichero_salida, "\t\t\t %d--->", i+1);		
		}	
		else{
			printf("\t\t\t%d--->", i+1);
			fprintf(fichero_salida, "\t\t\t%d--->", i+1);		
		}
		for (j=0; j<COLUMNAS; j++){
			/* En funcion del valor de la posicion se imprime un caracter */
			if(tablero[i][j] == 0){
				// Si la posicion es 0 simboliza que esta vacia.
				printf("[ ]");
				fprintf(fichero_salida, "[ ]");	
			}
			else if(tablero[i][j] < 0){
				// Si la posicion es negativa, es un borde horizontal o lateral.
				printf("[1]");
				fprintf(fichero_salida, "[1]");	
			}
			else if(tablero[i][j] == 3){
				// Si la posicion un tres, simboliza la cabeza de la primera serpiente.
				printf("[Ø]");
				fprintf(fichero_salida, "[Ø]");	
			}
			else if( (tablero[i][j] > 3) &&  (tablero[i][j] < (FILAS*COLUMNAS)+3) ){
				// Si la posicion esta entre 4 y filas * columnas + 3 (valor escogido arbitrariamente para empezar la segunda serpiente) simbolizara el cuerpo de la primera serpiente.
				printf("[#]");
				fprintf(fichero_salida, "[#]");	
			}
			else if(tablero[i][j] == (FILAS*COLUMNAS)+3){
				// Si la posicion es filas * columnas + 3, simboliza la cabeza de la segunda serpiente.
				printf("[€]");
				fprintf(fichero_salida, "[€]");	
			}
			else if(tablero[i][j] == 2){
				// Si la posicion es 2, representara una manzana.
				printf("[2]");
				fprintf(fichero_salida, "[2]");	
			}
			else{
				// Si la posicion es mayor, simbolizara el cuerpo de la segunda serpiente.
				printf("[·]");
				fprintf(fichero_salida, "[·]");	
			}
		}
		printf("\n");
		fprintf(fichero_salida, "\n");
	}
}


void cargar_tablero_partida(int tablero[FILAS][COLUMNAS]){
	int i,j;
	/* Se recorre la matriz leyendo los valores y almacenandolos en la posicion correspondiente. */
	for(i=0; i<FILAS; i++){
		for(j=0; j<COLUMNAS; j++){
			// Se leen uno a uno los valores de la matriz.
			fscanf(fichero_entrada, "%d", &tablero[i][j]);
		}
	}
}


void guardar_tablero_fichero(int tablero[FILAS][COLUMNAS], char nombre_fichero[25]){
	int i,j;
	FILE *fichero_salida_tablero;
	/* Se intenta abrir el fichero */
	fichero_salida_tablero = fopen(nombre_fichero,"w");
	if( fichero_salida_tablero == NULL ){
		// En caso de no poder se lanza un error y se sigue con el juego
		printf("Error: No se pudo abrir el fichero de entrada\n");
	}
	else{
		// En caso de poder abrir el fichero se escribe en el la matriz separada por tabulaciones (\t)
		for(i=0; i<FILAS; i++){
			for(j=0; j<COLUMNAS; j++){
				fprintf(fichero_salida_tablero, "%d\t", tablero[i][j]);
			}
			fprintf(fichero_salida_tablero, "\n");
		}
		// Y se cierra el fichero
		fclose(fichero_salida_tablero);
	}
}


void inicializar_tablero(int tablero[FILAS][COLUMNAS]){
    /* Se inicializa el tablero, colocando -1's en los bordes laterales y -2's en los bordes superior e inferior. */
    int i,j;
	for (i = 0; i < FILAS; i++){
        for (j = 0; j < COLUMNAS; j++){
            if ( (i==0) || (i==16) ) {	
            		// Si es un borde superior o inferior lo inicializo a -2.
                    tablero[i][j] = -2;
            }
            else{
                if ( (j==0) || (j==16) ){
                		// Si es un borde lateral lo inicializo a -1.
                        tablero[i][j] = -1;
                }
                else{
                		// En el resto del tablero coloco 0's.
                        tablero[i][j] = 0;
                }
            }
        }
    }

    /* Se colocan en el tablero las serpientes y la manzana. */
    tablero[8][12] = 2; // Manzana.
	tablero[8][8] = 3;  // Cabeza de la serpiente.
	tablero[8][7] = 5;  // Cola de la serpiente.
}


int selecciona_casilla(int tablero[FILAS][COLUMNAS], int fila, int columna){
	int i, j, valido = 1;
	/* Compruebo que la fila y la columna introducida este entre valores posibles en el tablero. */
	if( (fila < 0) || (fila > 16) ){
		printf("Fila no valida: Debe estar entre 1 y 16.\n");
		fprintf(fichero_salida, "Fila no valida: Debe estar entre 1 y 16.\n");
		valido = 0;
	}
	else if( (columna < 0) || (columna > 16) ){
		printf("Columna no valida: Debe estar entre 1 y 16.\n");
		fprintf(fichero_salida, "Columna no valida: Debe estar entre 1 y 16.\n");
		valido = 0;
	}
	else{
		/* Se busca la cabeza de la serpiente en el tablero. */
		for(i=0; i<FILAS; i++){	
			for(j=0; j<COLUMNAS; j++){		
				if(tablero[i][j] == 3){
					if( (fila == i) && (columna == j) ){
						// Compruebo que la posicion elegida coincida con la actual posicion de la serpiente.
						printf("Posicion no valida: Esa es su posicion actual.\n");
						fprintf(fichero_salida, "Posicion no valida: Esa es su posicion actual.\n");
						valido = 0;
					}
					/* Compruebo que la posicion elegida concuerda con una de los posibles movimientos de la serpiente. */
					if( (fila == i+1 || fila == i-1 || fila == i) && (columna == j+1 || columna == j-1 || columna == j) ){
						if( tablero[fila][columna] == 5){
							// Tambien compruebo que la posicion elegida coincida con el recorrido que llevaba la serpiente.
							printf("Posicion no valida: La serpiente no se puede mover hacia atras.\n");
							fprintf(fichero_salida, "Posicion no valida: La serpiente no se puede mover hacia atras.\n");
							valido = 0;
						}
						else if( ( fila == i+1 && (columna == j+1 || columna == j-1) ) || ( fila == i-1 && (columna == j+1 || columna == j-1) ) ){
							// Tambien compruebo que la posicion no sea una diagonal, dejando solo la posibilidad de que sea una opcion valida.
							printf("Posicion no valida: El movimiento no puede ser diagonal.\n");
							fprintf(fichero_salida, "Posicion no valida: El movimiento no puede ser diagonal.\n");
							valido = 0;
						}

					}
					else{
						// Si no es ninguna de las nueve posiciones anteriores, la serpiente no se puede mover ahi.
						printf("Posicion no valida: La posicion debe ser una a la que la serpiente se pueda mover.\n");
						fprintf(fichero_salida, "Posicion no valida: La posicion debe ser una a la que la serpiente se pueda mover.\n");
						valido = 0;

					}
				}
			}
		}
	}
	return valido;
}


void jugar_ordenador(int tablero_partida){
	int tablero[FILAS][COLUMNAS]; // Tablero de juego.
	int muerte = 0, esValido, come, opcion; // Variables de juego.
	int i, j, k, h, v, movido; // Variables usadas para bucles for.
	int nuevafila, nuevacolumna; // Posicion de movimiento de la serpiente.
	int nuevamanzanafila, nuevamanzanacolumna; // Generacion de manzanas nuevas.
	char nombre_fichero[25]; // Fichero de exportacion de tablero.
	int tamMax = (FILAS*COLUMNAS)+2, tam = 0; // Longitud maxima de la serpiente y longitud actual (depende de si existe o no fichero).

	/* Se comprueba si existe un fichero de entrada de tablero. */
	if(tablero_partida == 1){
		// Si existe el tablero pregunto si se desea importar.
		/* Bucle de menu. */
		do{
			printf("Se ha encontrado un tablero de entrada en un fichero.\n");
			printf("¿Desea importarlo?\n");
			printf("[1] - Si.\n");
			printf("[2] - No.\n");
			printf("Advertencia: Si el contenido del fichero no es un tablero con las reglas del juego se pueden producir errrores.\n");
			scanf("%d", &opcion);
			switch(opcion){
				case 1:
					cargar_tablero_partida(tablero);
					printf("Importado tablero desde el fichero.\n");
					// Se busca el valor mas grande en la matriz (sera el tamaño).
					for(i=0; i<FILAS; i++){
						for(j=0 ; j<COLUMNAS; j++){
							if(tablero[i][j] > tam){
								tam = tablero[i][j];
							}
						}				
					}
				break;
				case 2:
					printf("Generando un tablero estandar.\n");
					inicializar_tablero(tablero);
					tam = 5;
				break;
				default:
				// Si la opcion no es ninguna de las dos muestro un error y vuelvo al menu.
				printf("Error: Valor no valido (%d).\n", opcion);
				printf("Relanzando menu\n\n");
				fprintf(fichero_salida, "Error: Valor no valido (%d).\n", opcion);
				fprintf(fichero_salida, "Relanzando menu.\n\n");
				break;
			}
		}while( (opcion != 1) && (opcion != 2) );
	}
	else{
		// Si no se genera un tablero de forma estandar.
		printf("Fichero de entrada no encontrado, generando tablero estandar.\n");
		inicializar_tablero(tablero);
		tam = 5;	
	}

	/* Bucle de juego */
	do{
		come = 0;
		imprimir_tablero(tablero);
		/* Bucle de menu */
		do{
			printf("Opciones:\n");
			printf("[1] - Elegir posicion.\n");
			printf("[2] - Tablero a fichero.\n");
			fprintf(fichero_salida,"Opciones:\n");
			fprintf(fichero_salida,"[1] - Elegir posicion.\n");
			fprintf(fichero_salida,"[2] - Tablero a fichero.\n");
			scanf("%d", &opcion);
			fprintf(fichero_salida, "Se ha seleccionado la opcion %d.\n", opcion);
			if( (opcion != 1) && (opcion !=2) ){
				printf("Error: Valor no valido (%d).\n", opcion);
				printf("Relanzando menu\n\n");
				fprintf(fichero_salida, "Error: Valor no valido (%d).\n", opcion);
				fprintf(fichero_salida, "Relanzando menu.\n\n");
			}
		}while( (opcion != 1) && (opcion != 2) );
		switch(opcion){
			case 1:
				fprintf(fichero_salida, "Se ha elegido la opcion de elegir posicion.\n");
				/* Bucle de posicion */
				do{
					/* Se piden una fila y una columna por teclado. */
					printf("Seleccione la nueva fila: ");
					scanf("%d",&nuevafila);
					nuevafila--;
					fprintf(fichero_salida, "Se ha seleccionado la fila %d.\n", nuevafila);
					printf("Seleccione la nueva columna: ");
					scanf("%d",&nuevacolumna);
					nuevacolumna--;
					fprintf(fichero_salida, "Se ha seleccionado la columna %d.\n", nuevacolumna);
					/* Se comprueba si la posicion seleccionada es valida con la funcion selecciona_casilla. */		
					esValido = selecciona_casilla(tablero, nuevafila, nuevacolumna);
					if(esValido == 1){  
						/* Si la fila es valida se comprueba cual es la casilla de destino. */
						if(tablero[nuevafila][nuevacolumna] == -1){
							// Si la posicion de destino es un borde lateral se comprueba si esta al principio o al final y se mueve la nueva columna al contrario.
							if(nuevacolumna == 16){
								nuevacolumna = 1;
							}
							else if(nuevacolumna == 0){
								nuevacolumna = 15;
							}
						}

						// Se comprueba antes si es un borde lateral ya que en la nueva columna puede estar una manzana.
						if(tablero[nuevafila][nuevacolumna]==2){	
							// Si la posicion de destino es una manzana se activa la condicicion de come.
							come = 1;
						}
						else if( (tablero[nuevafila][nuevacolumna] > 3) && (tablero[nuevafila][nuevacolumna] < tamMax) ){
							// Si la posicion de destino es una de las posiciones de la serpiente se acaba el juego.
							printf ("Te has chocado contra ti mismo, fin del juego.\n");
							fprintf(fichero_salida, "Te has chocado contra ti mismo, fin del juego.\n"); 
							muerte = 1;
						}
						else if(tablero[nuevafila][nuevacolumna]== -2){
							// Si la posicion de destino es un borde superior se acaba el juego.
							printf ("Te has chocado contra el borde, fin del juego.\n");
							fprintf(fichero_salida, "Te has chocado contra el borde, fin del juego.\n");
							muerte = 1;
						}

						/* Se busca en el tablero la cabeza de la serpiente (valor 3) y se reemplaza por un -3. */
						for(i=0; i<FILAS; i++){
							for(j=0 ; j<COLUMNAS; j++){
								if(tablero[i][j] == 3){
									tablero[i][j] = -3;
									// Inicializo a 3 el nuevo valor de la cabeza.
									tablero[nuevafila][nuevacolumna] = 3;
								}
							}					
						}		

						/* Se busca cada posición de la serpiente y se mueve. */
						for(k=5; k<=tam; k++){	
							//Bucle que se ejecuta tantas veces como posiciones a mover, ya que el tam 0 es 5 (primer numero de la cola).
							movido = 0;
							for(i=0; i<FILAS; i++){
								for(j=0; j<COLUMNAS; j++){
									// Busco el -3 dejado antes.
									if(tablero[i][j]==-3 && movido == 0){
										for(h=0; h<FILAS; h++){
											for(v=0; v<COLUMNAS; v++){
												/* Se busca en la matriz una posicion igual al valor del primer bucle para igualarla a -3.
												   Si ya es la ultima iteraccion (k = tam) se comprueba si se ha comido manzana para extender o no la cola. */
												if(tablero[h][v]==k && k==tam){
													if(come == 1){
														tablero[h][v] = ++tam;
													}
													else{
														tablero[h][v] = 0;
														
													}
												}
												else if(tablero[h][v]==k){
													tablero[h][v] = -3;
												}
											}
										}
										// Activo la condicion movido para que no se vuelva a ejecutar con el -3 marcado antes.
										movido = 1;
										// Igualo el -3 anterior a la k.
										tablero[i][j]=k;
									}
								}
							}
						}

						/* Si el tamaño es el mismo valor que el tablero sin los bordes, se acaba el juego por victoria */
						if(tam = ((FILAS-2)*(COLUMNAS-2)) ){
							printf("¡Has ganado!\n");
							fprintf(fichero_salida, "¡Has ganado!\n");
							muerte = 1;
						}
						
						/* Si la condicion come se ha activado se genera una nueva manzana aleatoria. */
						if(come == 1){ 
							do{
								// Genero valores entre 1 y 15 y se compruebo que esa posicion esta vacia.
								nuevamanzanafila = 1 + (rand()%15);
								nuevamanzanacolumna = 1 + (rand()%15);
							}while(tablero[nuevamanzanafila][nuevamanzanacolumna] != 0);
							// Igualo a 2 el valor valido.
							tablero[nuevamanzanafila][nuevamanzanacolumna] = 2;
						}
					}
				}while (esValido != 1);
			break;
			case 2:
				fprintf(fichero_salida, "Se ha elegido la opcion tablero a fichero\n");
				printf("Introduzca el nombre del fichero para exportar el tablero: ");
				/* Se limpia el buffer de entrada y se usa la funcion gets para obtener el nombre del fichero. */
				getchar();
				gets(nombre_fichero);
				guardar_tablero_fichero(tablero, nombre_fichero);
				printf("Se ha guardado el tablero en el archivo %s.\n", nombre_fichero);
				fprintf(fichero_salida, "Se ha guardado el tablero en el archivo %s.\n", nombre_fichero);
			break;
		}
	}while(muerte != 1);
	printf("Puntuacion: %d.\n", tam-3);
	fprintf(fichero_salida, "Puntuacion: %d.\n", tam-3);
}


void inicializar_tablero_multijugador(int tablero[FILAS][COLUMNAS]){
    /* Se inicializa el tablero, colocando -1's en los bordes laterales y -2's en los bordes superior e inferior. */
    int i,j;
	for (i = 0; i < FILAS; i++){
        for (j = 0; j < COLUMNAS; j++){
            if ( (i==0) || (i==16) ) {	
            		// Si es un borde superior o inferior lo inicializo a -2.
                    tablero[i][j] = -2;
            }
            else{
                if ( (j==0) || (j==16) ){
                		// Si es un borde lateral lo inicializo a -1.
                        tablero[i][j] = -1;
                }
                else{
                		// En el resto del tablero coloco 0's.
                        tablero[i][j] = 0;
                }
            }
        }
    }
    /* Se colocan en el tablero la serpiente y la manzana. */
    tablero[8][8] = 2; // Manzana.
	tablero[8][5] = 3;  // Cabeza de la serpiente.
	tablero[8][4] = 5;  // Cola de la serpiente.
	tablero[11][8] = (FILAS*COLUMNAS) + 3; // Cabeza de la segunda serpiente.
	tablero[12][8] = (FILAS*COLUMNAS) + 5; // Cola de la segunda serpiente.
}


int selecciona_casilla_multijugador(int tablero[FILAS][COLUMNAS], int fila, int columna, int serpiente){ 
	/* En funcion de que serpiente sea, se da un valor a la cabeza */
	int cabeza;
	if(serpiente==1){
		cabeza = 3;
	}
	else{
		cabeza = FILAS*COLUMNAS+3;
	}
	int i, j, valido = 1;
	/* Se comrpueba que la fila y la columna introducida este entre valores posibles en el tablero. */
	if( (fila < 0) || (fila > 16) ){
		printf("Fila no valida: Debe estar entre 1 y 16.\n");
		fprintf(fichero_salida, "Fila no valida: Debe estar entre 1 y 16.\n");
		valido = 0;
	}
	else if( (columna < 0) || (columna > 16) ){
		printf("Columna no valida: Debe estar entre 1 y 16.\n");
		fprintf(fichero_salida, "Columna no valida: Debe estar entre 1 y 16.\n");
		valido = 0;
	}
	else{
		/* Se busca la cabeza de la serpiente en el tablero. */
		for(i=0; i<FILAS; i++){	
			for(j=0; j<COLUMNAS; j++){		
				if(tablero[i][j] == cabeza){
					if( (fila == i) && (columna == j) ){
						// Compruebo que la posicion elegida coincida con la actual posicion de la serpiente.
						printf("Posicion no valida: Esa es su posicion actual.\n");
						fprintf(fichero_salida, "Posicion no valida: Esa es su posicion actual.\n");
						valido = 0;
					}
					/* Se comprueba que la posicion sea una de las 9 posibles que rodean a la cabeza. */
					if( (fila == i+1 || fila == i-1 || fila == i) && (columna == j+1 || columna == j-1 || columna == j) ){
						if( tablero[fila][columna] == 5){
							// Tambien compruebo que la posicion elegida coincida con el recorrido que llevaba la serpiente.
							printf("Posicion no valida: La serpiente no se puede mover hacia atras.\n");
							fprintf(fichero_salida, "Posicion no valida: La serpiente no se puede mover hacia atras.\n");
							valido = 0;
						}
						else if( ( fila == i+1 && (columna == j+1 || columna == j-1) ) || ( fila == i-1 && (columna == j+1 || columna == j-1) ) ){
							// Tambien compruebo que la posicion no sea una diagonal, dejando solo la posibilidad de que sea una opcion valida.
							printf("Posicion no valida: El movimiento no puede ser diagonal.\n");
							fprintf(fichero_salida, "Posicion no valida: El movimiento no puede ser diagonal.\n");
							valido = 0;
						}

					}
					else{
						// Si no es ninguna de las nueve posiciones anteriores, la serpiente no se puede mover ahi.
						printf("Posicion no valida: La posicion debe ser una a la que la serpiente se pueda mover.\n");
						fprintf(fichero_salida, "Posicion no valida: La posicion debe ser una a la que la serpiente se pueda mover.\n");
						valido = 0;

					}
				}
			}
		}
	}
	return valido;
}


int jugar_contra_otro(void){
	int tablero[FILAS][COLUMNAS]; // Tablero de juego.
	int serpiente = 2, tam = 5, tam1 = 5, tam2 = (FILAS*COLUMNAS)+5, cabeza = 3, cuerpo = 5; // Variables de multijugador (Para marcar en el bucle el valor de la cabeza y el cuerpo de cada serpiente).
	int h, v, k, i, j, movido; // Variables iteradoras para bucles for.
	int nuevafila, nuevacolumna, nuevamanzanafila, nuevamanzanacolumna; // Variables para preguntar la posicion de movimiento y generar nuevas manzanas.
	int opcion, esValido, come, muerte = 0; // Variables de juego.
	int perdedor; // Es mas facil determinar el perdedor que el ganador ya que en la mayoria de ocasiones la serpiente coincide con el perdedor porque se choca sola.
	char nombre_fichero[25]; // Variable para exportar el tablero a fichero.

	/* Se inicializa el tablero de forma estandar */
	inicializar_tablero_multijugador(tablero);
	
	/* Bucle de juego (se repite mientras no muera ninguna serpiente). */
	do{	
		come = 0;
		imprimir_tablero(tablero);
		/* Se cambia la serpiente que debe jugar. */
		if(serpiente == 1){
			// Si la serpiente era la uno, se da a las variables valor de la serpiente contraria y viceversa.
			serpiente = 2;
			cabeza = (FILAS*COLUMNAS)+3;
			cuerpo = (FILAS*COLUMNAS)+5;
			tam = tam2;
		}
		else{
			serpiente = 1;
			cabeza = 3;
			cuerpo = 5;
			tam = tam1;
		}	
		/* Bucle de menu (se repite mientras la opcion no sea valida). */
		do{
			printf("Juega la serpiente %d.\n",serpiente);
			fprintf(fichero_salida, "Juega la serpiente %d.\n",serpiente); 
			printf("Opciones:\n");
			printf("[1] - Elegir posicion.\n");
			printf("[2] - Tablero a fichero.\n");
			fprintf(fichero_salida,"Opciones:\n");
			fprintf(fichero_salida,"[1] - Elegir posicion.\n");
			fprintf(fichero_salida,"[2] - Tablero a fichero.\n");
			scanf("%d", &opcion);
			fprintf(fichero_salida, "Se ha seleccionado la opcion %d.\n", opcion);
			if( (opcion != 1) && (opcion !=2) ){
				printf("Error: Valor no valido (%d).\n", opcion);
				printf("Relanzando menu\n\n");
				fprintf(fichero_salida, "Error: Valor no valido (%d).\n", opcion);
				fprintf(fichero_salida, "Relanzando menu.\n\n");
			}
		}while( (opcion != 1) && (opcion != 2) );
		switch(opcion){
			case 1:
				/* Bucle de movimiento (se repite hasta que la posicion introducida sea valida). */
				do{
					printf("Seleccione la nueva fila: ");
					scanf("%d",&nuevafila);
					nuevafila--;
					fprintf(fichero_salida, "Se ha seleccionado la fila %d.\n", nuevafila); 
					printf("Seleccione la nueva columna\n");
					scanf("%d",&nuevacolumna);
					nuevacolumna--;
					fprintf(fichero_salida, "Se ha seleccionado la columna %d.\n", nuevacolumna); 
						
					// Se comprueba si la casilla es valida.
					esValido = selecciona_casilla_multijugador(tablero, nuevafila, nuevacolumna, serpiente);
					if(esValido == 1){  
						/* Si la fila es valida se comprueba cual es la casilla de destino. */
						if(tablero[nuevafila][nuevacolumna] == -1){
							// Si la posicion de destino es un borde lateral se comprueba si esta al principio o al final y se mueve la nueva columna al contrario.
							if(nuevacolumna == 16){
								nuevacolumna = 1;
							}
							else if(nuevacolumna == 0){
								nuevacolumna = 15;
							}
						}
						// Se comprueba antes si es un borde lateral ya que en la nueva columna puede estar una manzana.
						if(tablero[nuevafila][nuevacolumna] == 2){	
							// Si la posicion de destino es una manzana se activa la condicicion de come.
							come = 1;
						}
						else if( (serpiente == 1 && (tablero[nuevafila][nuevacolumna] > 5) && (tablero[nuevafila][nuevacolumna] < (FILAS*COLUMNAS)+3)) || (serpiente == 2 && (tablero[nuevafila][nuevacolumna] > (FILAS*COLUMNAS)+5)) ){
							// Si la posicion de destino es una de las posiciones de la serpiente se acaba el juego y gana la otra.
							printf("Te has comido a ti mismo serpiente %d, fin del juego.\n", serpiente);
							fprintf(fichero_salida, "Te has comido a ti mismo serpiente %d, fin del juego.\n", serpiente);
							muerte = 1;
							perdedor = serpiente;
						}	
						else if(tablero[nuevafila][nuevacolumna] == -2) { 
							// Si la posicion de destino es un borde superior se acaba el juego y ganala otra.
							printf ("Te has chocado contra el borde serpiente %d, fin del juego.\n", serpiente);
							fprintf(fichero_salida, "Te has chocado contra el borde serpiente %d, fin del juego.\n", serpiente);
							muerte = 1;
							perdedor = serpiente;
						}
						else if( (serpiente == 1 && (tablero[nuevafila][nuevacolumna] == (FILAS*COLUMNAS)+3)) || (serpiente == 2 && (tablero[nuevafila][nuevacolumna] == 3)) ){
							// Si se produce un choque de cabezas se comprueba que serpiente tiene un mayor tamaño, si son iguales la serpiente que ataco muere.
							printf("Ambas serpientes han chocado, fin del juego.\n");
							fprintf(fichero_salida, "Ambas serpientes han chocado, fin del juego.\n");
							muerte = 1;
							if(tam1 > (tam2-(FILAS*COLUMNAS)) ){
								printf("La serpiente 1 es mayor.\n");
								fprintf(fichero_salida, "La serpiente 1 es mayor.\n");
								perdedor = 2;
							}
							else if(tam1 < (tam2-(FILAS*COLUMNAS))){
								printf("La serpiente 2 es mayor.\n");
								fprintf(fichero_salida, "La serpiente 2 es mayor.\n");
								perdedor = 1;
							}
							else{
								printf("Ambas serpientes son iguales.\n");
								fprintf(fichero_salida, "Ambas serpientes son iguales.\n");
								perdedor = serpiente;
							}
						}
						else if( (serpiente == 2 && (tablero[nuevafila][nuevacolumna] > 3) && (tablero[nuevafila][nuevacolumna] < (FILAS*COLUMNAS)+3))  || (serpiente == 1 && (tablero[nuevafila][nuevacolumna] > (FILAS*COLUMNAS)+3)) ){
							// Si la posicion de destino es una de las posiciones de la otra serpiente pierde automaticamente.
							printf("Te has chocado contra la serpiente %d, fin del juego.\n", serpiente);
							fprintf(fichero_salida, "Te has chocado contra la serpiente %d, fin del juego.\n", serpiente); 
							muerte = 1;
							perdedor = serpiente;
						}

						/* Se busca donde estaba la cabeza y se deja un -3, moviendo el valor de la cabeza al la posicion elegida. */
						for(i=0; i<FILAS; i++){
							for(j=0; j<COLUMNAS; j++){
								if(tablero[i][j] == cabeza){
									tablero[i][j] = -3;
									tablero[nuevafila][nuevacolumna] = cabeza;
								}
							}					
						}

						/* Se busca cada posición de la serpiente y se mueve. */
						for(k = cuerpo; k<=tam; k++){	
							//Bucle que se ejecuta tantas veces como posiciones a mover, ya que el tam 0 es 5 o filas*columnas+5 (primer numero de la cola).
							movido = 0;
							for(i=0; i<FILAS; i++){
								for(j=0; j<COLUMNAS; j++){
									// Busco el -3 dejado antes.
									if(tablero[i][j]==-3 && movido == 0){
										for(h=0; h<FILAS; h++){
											for(v=0; v<COLUMNAS; v++){
												/* Se busca en la matriz una posicion igual al valor del primer bucle para igualarla a -3.
												   Si ya es la ultima iteraccion (k = tam) se comprueba si se ha comido manzana para extender o no la cola. */
												if(tablero[h][v]==k && k==tam){
													if(come == 1){
														// En funcion de que serpiente sea se incrementa un tamaño o el otro.
														if(serpiente == 1){
															tablero[h][v] = ++tam1;
														}
														else{
															tablero[h][v] = ++tam2;
														}
													}
													else{
														tablero[h][v] = 0;
													}
												}
												else if(tablero[h][v]==k){
													tablero[h][v] = -3;
												}
											}
										}
										// Activo la condicion movido para que no se vuelva a ejecutar con el -3 marcado antes.
										movido = 1;
										// Igualo el -3 anterior a la k.
										tablero[i][j]=k;
									}
								}
							}
						}

						/* Si la condicion come se ha activado se genera una nueva manzana aleatoria. */
						if(come == 1){ 
							do{
								// Genero valores entre 1 y 15 y se compruebo que esa posicion esta vacia.
								nuevamanzanafila = 1 + (rand()%15);
								nuevamanzanacolumna = 1 + (rand()%15);
							}while(tablero[nuevamanzanafila][nuevamanzanacolumna] != 0);
							// Igualo a 2 el valor valido.
							tablero[nuevamanzanafila][nuevamanzanacolumna] = 2;
						}
					}
				}while(esValido != 1);
			break;
			case 2:
				fprintf(fichero_salida,"Se ha elegido la opcion tablero a fichero\n");
				printf("Introduzca el nombre del fichero para exportar el tablero: ");
				/* Se limpia el buffer de entrada y se usa la funcion gets para obtener el nombre del fichero. */
				getchar();
				gets(nombre_fichero);
				guardar_tablero_fichero(tablero, nombre_fichero);
				printf("Se ha guardado el tablero en el archivo %s\n", nombre_fichero);
				fprintf(fichero_salida, "Se ha guardado el tablero en el archivo %s\n", nombre_fichero);
			break;	
		}
	}while (muerte != 1);
	// Devuelve 1 si el perdedor es la serpiente 1 y viceversa.
	return perdedor;
}
