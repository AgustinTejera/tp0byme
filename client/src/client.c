#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	/* ---------------- LOGGING ---------------- */

	/*Creado nuestro logger, usemos log_info() para loggear
	el string "Soy un Log" y cerremos el logger al
	final del programa con log_destroy().
	*/
	t_log* logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger, "Hola! Soy un Log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	t_config* config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	char* ip = config_get_string_value(config, "IP");
	char* puerto = config_get_string_value(config, "PUERTO");
	char* valor = config_get_string_value(config, "CLAVE");

	// Loggeamos el valor de config
	log_info(logger,valor);
	log_info(logger,ip);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	int conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log", "Log:", 1, LOG_LEVEL_INFO);
	if(nuevo_logger==NULL){
		printf("No se creo correctamente el log :(\n");
		exit(1);
	}
	else{
		return nuevo_logger;
	}
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");
	if(nuevo_config == NULL){
		printf("No se pudo crear la config\n");
		//perror(" No se pudo crear la config x2/n");
		exit(2);
	}
	else{
		return nuevo_config;
	}
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío

	while(strcmp(leido, "")){
	//while(strncmp(leido,"",(char)sizeof(leido))){
		log_info(logger,leido);
		free(leido);
		leido = readline("> ");
	}


	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido= readline("> ");
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete

	while(strcmp(leido,"")){
		agregar_a_paquete(paquete, leido, strlen(leido)+1);
		enviar_paquete(paquete, conexion);
		free(leido);
		leido  = readline("> ");
		}
		// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
		free(leido);
		eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	if(logger != NULL){
		log_destroy(logger);
	}
	if(config != NULL){
			config_destroy(config);
	}
	if(conexion != -1){
			liberar_conexion(conexion);
		}

}
