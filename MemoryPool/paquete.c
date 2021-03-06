#include "paquete.h"

//---------------------CREAR PAQUETES

t_paquete_select* crear_paquete_select(char* nombretabla, uint16_t valor_key)
{
	uint32_t tamanio_tabla = strlen(nombretabla)+1;

	t_paquete_select* paquete = malloc(sizeof(t_paquete_select));
	paquete->nombre_tabla = malloc(tamanio_tabla);

	strcpy(paquete->nombre_tabla, nombretabla);
	paquete->valor_key = valor_key;
	paquete->nombre_tabla_long= tamanio_tabla;

	return paquete;
}

t_paquete_insert* crear_paquete_insert(char *nombre_tabla, uint16_t valor_key, char *value, long long timestamp)
{
	uint32_t tamanio_tabla = strlen(nombre_tabla)+1;
	uint32_t tamanio_value = strlen(value)+1;

	t_paquete_insert* paquete = malloc(sizeof(t_paquete_insert));

	paquete->nombre_tabla = malloc(tamanio_tabla);
	paquete->value = malloc(tamanio_value);

	strcpy(paquete->nombre_tabla,nombre_tabla);
	strcpy(paquete->value,value);

	paquete->valor_key = valor_key;
	paquete->timestamp = timestamp;
	paquete->nombre_tabla_long= tamanio_tabla;
	paquete->value_long= tamanio_value;

	return paquete;
}

t_paquete_create* crear_paquete_create(char* nombre_tabla, char* consistencia, int particiones, int tiempo_compactacion)
{
	uint32_t tamanio_tabla = strlen(nombre_tabla)+1;
	uint32_t tamanio_consistencia = strlen(consistencia)+1;

	t_paquete_create* paquete = malloc(sizeof(t_paquete_create));

	paquete->nombre_tabla = malloc(tamanio_tabla);

	strcpy(paquete->nombre_tabla, nombre_tabla);

	paquete->consistencia = consistencia;
	paquete->particiones= particiones;
	paquete->tiempo_compactacion= tiempo_compactacion;
	paquete->nombre_tabla_long = tamanio_tabla;
	paquete->consistencia_long = tamanio_consistencia;

	return paquete;
}


//---------------------ARMAR PAQUETES

t_paquete_select* paquete_select(char* parametros, t_log* logger)
{
	uint16_t valor_key;
	char* nombre_tabla;

	parametros= strtok(NULL, " ");
	if(parametros==NULL){
		log_error(logger, "Select invalido");
		return NULL;
	}
	nombre_tabla = parametros;
	parametros = strtok(NULL, " ");
	if(parametros==NULL || !validar_numero(parametros)){
		log_error(logger, "Key invalida");
		return NULL;
	}
	valor_key = atoi(parametros);

	t_paquete_select* paquete = crear_paquete_select(nombre_tabla, valor_key);

	loggear_paquete_select(paquete, logger);

	return paquete;
}

t_paquete_insert* paquete_insert(char* parametros, t_log* logger)
{
	uint16_t valor_key;
	char* nombre_tabla;
	char* value;
	long long timestamp=0;

//	parametros= strtok(NULL, " ");
//	nombre_tabla = parametros;
//	parametros = strtok(NULL, " ");
//	valor_key = atoi(parametros);
//	parametros = strtok(NULL, "\"");
//	value = parametros;
//	parametros = strtok(NULL, " ");
//	timestamp = get_timestamp(parametros);

	parametros= strtok(NULL, " ");
	if(parametros==NULL){
		log_error(logger, "Insert invalido");
		return NULL;
	}
	nombre_tabla = parametros;

	parametros = strtok(NULL, " ");
	if(parametros==NULL || !validar_numero(parametros)){
		log_error(logger,"Key invalida");
		return NULL;
	}
	valor_key = atoi(parametros);

	parametros = strtok(NULL, " ");

	if(parametros==NULL){
		log_error(logger,"Value invalido");
		return NULL;
	}
	if((string_starts_with(parametros, "\"") && string_ends_with(parametros, "\"")))
	{
		parametros = strtok(parametros, "\"");
	}
	else
	{
		log_error(logger, "El value no esta entre comillas");
		return NULL;
	}
	value = parametros;
	timestamp = get_timestamp(parametros);

	t_paquete_insert* paquete = crear_paquete_insert(nombre_tabla, valor_key, value, timestamp);

	loggear_paquete_insert(paquete, logger);

	return paquete;
}

long long get_timestamp(char * parametros){
	long long valor;
	if (parametros==NULL) {
		struct timeval te;
		gettimeofday(&te, NULL); // get current time
		valor = te.tv_sec*1000LL + te.tv_usec/1000;
	} else {valor = atoll(parametros);}

	return valor;
}

t_paquete_create* paquete_create(char* parametros, t_log* logger)
{
	char* nombre_tabla;
	char* consistencia;
	uint16_t particiones;
	uint16_t tiempo_compactacion;

	parametros= strtok(NULL, " ");
	if(parametros==NULL){
		log_error(logger, "Create invalido");
		return NULL;
	}
	nombre_tabla =parametros;
	parametros = strtok(NULL, " ");
	if(parametros==NULL || !validarConsistencia(parametros)){
		log_error(logger, "Consistencia invalida");
		return NULL;
	}
	consistencia=parametros;
	parametros = strtok(NULL, " ");
	if(parametros==NULL || !validar_numero(parametros) || !strcmp(parametros,"0")){
		log_error(logger, "Particiones invalidas");
		return NULL;
	}
	particiones = atoi(parametros);
	parametros = strtok(NULL, " ");
	if(parametros==NULL || !validar_numero(parametros)){
		log_error(logger, "Tiempo de compactacion invalido");
		return NULL;
	}
	tiempo_compactacion = atoi(parametros);

	t_paquete_create* paquete = crear_paquete_create(nombre_tabla, consistencia, particiones, tiempo_compactacion);

	loggear_paquete_create(paquete, logger);

	return paquete;
}


t_paquete_describe_lfs* paquete_describe_para_lfs(char* parametros,t_log* logger)
{
	parametros= strtok(NULL, " ");
		if(parametros == NULL){
			uint32_t tamanio_tabla= strlen("ALL")+1;
//			t_paquete_describe_lfs* paquete= malloc(tamanio_tabla + sizeof(uint32_t));
			t_paquete_describe_lfs* paquete= malloc(sizeof(t_paquete_describe_lfs));
			paquete->nombre_tabla ="ALL";
			paquete->nombre_tabla_long = tamanio_tabla;
			return paquete;
		}
		else
		{
			uint32_t tamanio_tabla= strlen(parametros)+1;
//			t_paquete_describe_lfs* paquete= malloc(tamanio_tabla + sizeof(uint32_t));
			t_paquete_describe_lfs* paquete= malloc(sizeof(t_paquete_describe_lfs));
			paquete->nombre_tabla =parametros;
			paquete->nombre_tabla_long = tamanio_tabla;
			return paquete;
		}
}

bool validarConsistencia(char* consistencia){

	if (strcmp(consistencia, "SC")==0) {
		return true;
	}
	else if (strcmp(consistencia, "SHC")==0) {
		return true;
	}
	else if (strcmp(consistencia, "EC")==0) {
		return true;

	}
	return false;
}
bool validar_numero(char* parametro){
	for(int i=0;i<string_length(parametro);i++){
		if(!isdigit(parametro[i])) return false;
	}
	return true;
}
