#ifndef RECIBIR_H_
#define RECIBIR_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<commons/string.h>
#include<commons/config.h>
#include<string.h>

#include "paquete.h"
#include "API.h"
#include "utils.h"
#include "tablas.h"
//#include "envio.h"

typedef struct respuestaSELECT_FS
{
	uint32_t tamanio_key;
	uint16_t rta;
	char* keyHallada;
}respuestaSELECT_FS;

typedef struct t_metadataDescribe
{
	char* consistencia;
	int particiones;
	int tiempo_de_compactacion;
}t_metadataDescribe;

//--------------------LEVANTAR SERVIDOR
void* servidor(void * arg);

//--------------------RECIBIR PAQUETE
void recibir_paquetes(int cliente_fd, int server_fd, t_config* config, t_log* logger);

//--------------------RECIBIR OPERACION
int recibir_operacion(int);

//---------------------DESERIALIZAR PAQUETE
t_paquete_select* deserializar_paquete_select(int socket_cliente);
t_paquete_insert* deserializar_paquete_insert(int socket_cliente);
t_paquete_create* deserializar_paquete_create(int socket_cliente);
t_list* deserializar_respuesta_describe(int conexion);
SEED* deserealizar_seed(int socket_cliente);

//------------------RECIBIR MENSAJES------------------
void recibir_seed(int socket_cliente);
void* recibir_buffer(int* size, int socket_cliente);
void* recibir_mensaje_para_kernel(int socket_cliente);
void recibir_mensaje(int socket_cliente);

//--------------------ARCHIVO CONEXION.C
int esperar_cliente(t_config* config,int); //conexion.c ya incluye a "recibir.h", por lo que no puedo agregar a "conexion.h" en este header

#endif /* RECIBIR_H_ */
