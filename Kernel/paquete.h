#ifndef PAQUETE_H_
#define PAQUETE_H_

#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<string.h>
#include<stdint.h>

//---------------------ESTRUCTURA PAQUETES

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef enum
{
	CREATE, //Agregado
	DROP, //Agregado
	DESCRIBE, //Agregado
	SELECT, //Agregado
	INSERT, //Agregado
	JOURNAL, //Agregado
	RUN, //Agregado
	ADD //Agregado
}op_code; //TIPO DE PAQUETE

typedef struct t_paquete_select
{
	uint32_t nombre_tabla_long; //Longitud del nombre de la tabla
	char* nombre_tabla;
	int valor_key;

}__attribute__((packed)) t_paquete_select;

typedef struct t_paquete_insert
{
	uint32_t nombre_tabla_long; //Longitud del nombre de la tabla
	uint32_t value_long; 		//Longitud del value
	char* nombre_tabla;
	int valor_key;
	char* value;
	int timestamp;
}__attribute__((packed)) t_paquete_insert;

t_log* iniciar_logger(void);
t_config* leer_config(void);

//---------------------CREAR PAQUETE

t_paquete_select* crear_paquete_select(char *nombretabla,int valor_key);
t_paquete_insert* crear_paquete_insert(char *nombretabla,int valor_key, char *value, int timestamp);

//---------------------ARMAR PAQUETE

t_paquete_select* selectf(char* parametros);
t_paquete_insert* insert(char* parametros);

//---------------------LOGGEAR PAQUETE

void loggear_paquete_select(t_paquete_select* paquete);
void loggear_paquete_insert(t_paquete_insert* paquete);

//----------------------------------------------------ELIMINAR PAQUETE

//void eliminar_paquete_select(t_paquete_select* paquete);
//void eliminar_paquete_insert(t_paquete_insert* paquete);

#endif /* PAQUETE_H_ */
