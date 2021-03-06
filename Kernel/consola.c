#include "consola.h"
void consola()
{
	while(1)
	{
		char* lineaRequest = ingresar_request();
		string_to_upper(lineaRequest);
		agregar_a_cola(lineaRequest);
	}
}

//------------------------------INGRESO DE PAQUETES------------------------------------
int request(void * arg) //RECIBIR LOGGER
{
	struct parametros * parametro;
	parametro = ( struct parametros *) arg;

	pthread_mutex_lock(&mutex_handshake);
//	handshake(arg);
	pthread_mutex_unlock(&mutex_handshake);

	retardo_ejecucion(parametro->config);

	char** vector_request = string_split(parametro->parametros," ");
	int cod_ingresado = codigo_ingresado(vector_request[0]);

	bool verificacion = verificar_request(cod_ingresado, vector_request);
	if(!verificacion){return 0;}

	int conexion_nueva = conectarse_a_memoria(vector_request, parametro->logger); //ELIGE UNA MEMORIA, SEGUN EL CRITERIO BASADO EN LA TABLA
	if(conexion_nueva==-1){return 0;}

		switch(cod_ingresado){
			case 0:;
				if(conexion_nueva==0){break;}

				t_paquete_create* paquete_create = create(vector_request);

				if(paquete_create==NULL){break;}

				if (send(conexion_nueva, &cod_ingresado, sizeof(int), 0) <= 0) puts("Error en envio de CODIGO DE OPERACION.");
				else
				{
					enviar_paquete_create(paquete_create,conexion_nueva);
				}

				sleep(1);
				describe(conexion_nueva,paquete_create->nombre_tabla);

				free(paquete_create);

				break;
			case 1:;
				drop(conexion_nueva,vector_request[1]);
				break;
			case 2:;
				describe(conexion_nueva,vector_request[1]);
				break;

			case 3:;
				if(existe_tabla(vector_request[1]))
				{
					struct timeval tiempo_inicial, tiempo_final;
					gettimeofday(&tiempo_inicial,NULL);

					t_paquete_select* paquete_select = selectf(vector_request);

					if(paquete_select==NULL){return 0;}

					if(conexion_nueva==-1)
					{
						return 0;
					}


					if (send(conexion_nueva, &cod_ingresado, sizeof(int), 0) <= 0) puts("Error en envio de CODIGO DE OPERACION.");
					else
					{
						enviar_paquete_select(paquete_select, conexion_nueva);
						recibir_mensaje(conexion_nueva);
					}
					free(paquete_select);

					gettimeofday(&tiempo_final,NULL);
					sem_wait(&semaforo_metrica);
					calcular_tiempo(tiempo_inicial,tiempo_final,0);
					sem_post(&semaforo_metrica);

				}

				break;

			case 4:;
				if(existe_tabla(vector_request[1])) //HACER DESCRIBE CON CREATE
				{
					struct timeval tiempo_inicial, tiempo_final;
					gettimeofday(&tiempo_inicial,NULL);

					t_paquete_insert* paquete_insert = insert(vector_request);
					if (send(conexion_nueva, &cod_ingresado, sizeof(int), 0) <= 0) puts("Error en envio de CODIGO DE OPERACION.");
					else{enviar_paquete_insert(paquete_insert, conexion_nueva);}
					free(paquete_insert);

					gettimeofday(&tiempo_final,NULL);
					sem_wait(&semaforo_metrica);
					calcular_tiempo(tiempo_inicial,tiempo_final,1);
					sem_post(&semaforo_metrica);
				}
				break;
			case 5:;
				journal_memorias(parametro->logger);
				//FIJARSE SI LOGEA
				break;
			case 7:
				agregar_consistencia(atoi(vector_request[2]),vector_request[4]);
				break;
			case 8:
				sem_wait(&semaforo_metrica);
				logear_metrica_consola(parametro->logger);
				sem_post(&semaforo_metrica);
				break;
			default:
				break;
		}

		if(conexion_nueva!=0){
			close(conexion_nueva);
		}

		string_iterate_lines(vector_request,free);
		free(vector_request);

		return 1;
}

char* ingresar_request()
{
	printf("\n\nCREATE    NOMBRETABLA CONSISTENCIA PARTICIONES T_COMPACTACION\nDROP\nDESCRIBE\nSELECT    NOMBRETABLA KEY\nINSERT    NOMBRETABLA KEY VALUE \nJOURNAL\nRUN    PATH\nADD MEMORY [#MEMORIA] TO [CONSISTENCY]\nEXIT\n");
	printf("\nIngrese REQUEST ");
	char * linea;

	while(1)
	{
		linea = readline(">");
		if(linea)add_history(linea);
		return linea;
		free(linea);
	}
}

void loggearListaMetadatas(t_list * metadatas){
	list_iterate(metadatas,loggearMetadataTablas);
}

void loggearMetadataTablas(void* elemento){

	t_log* logger= iniciar_logger();
	t_metadata* metadataDeTablaPedida= (t_metadata*)elemento;
	log_info(logger,"%s %s",metadataDeTablaPedida->nombre_tabla,metadataDeTablaPedida->consistencia);
	log_destroy(logger);

}

