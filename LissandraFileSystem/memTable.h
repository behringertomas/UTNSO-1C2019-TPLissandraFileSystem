/*
 * memTable.h
 *
 *  Created on: 8 may. 2019
 *      Author: utnso
 */

#ifndef MEMTABLE_H_
#define MEMTABLE_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<curses.h>
#include<sys/socket.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/temporal.h>
#include <sys/time.h>
#include"recibir.h"
#include"APIS.h"
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include<commons/collections/list.h>
#include <stdbool.h>
#include<commons/collections/dictionary.h>
#include "Lissandra.h"
#include <commons/collections/node.h>


typedef struct nodoRegistroMemTable{
	char* value;
	uint16_t key;
	long long timestamp;

}nodoRegistroMemTable;

typedef struct nodoTablaMemTable{
	char* nombreTabla;
	t_list* registros;
}nodoTablaMemTable;

void mostrarRegistros(void* elemento);
void mostrarTablas(char*key,void* elemento);
t_list* crearMemTable();


bool mayorTimeStamp(void*elemento1,void*elemento2);
bool filtrarPorKey(void* elemento,int key);
t_registro* buscarMemTable(char* nombreTabla,int key);
bool _mismoNombre(void* elemento);
bool igualNombre(void* elemento,char* nombreBuscado);
void agregarTabla(t_paquete_insert* paquete_insert);
bool existeDuplicado(nodoTablaMemTable* nodoTabla);
nodoTablaMemTable* crearNodoTabla(char* nombreTabla);
void imprimirListaTablas();
void eliminarNodoTabla();
t_list* agregarRegistro(t_list* listaTabla, nodoRegistroMemTable* nodoRegistro);
void imprimirRegistrosTabla();
void eliminarUltimoRegistro(nodoTablaMemTable* nodoTabla);
void dump();
nodoTablaMemTable* crearNodoTabla(char* nombreTabla);
nodoRegistroMemTable* crearNodoRegistro(char*value,uint16_t key,long long timestamp);
#endif /* MEMTABLE_H_ */
