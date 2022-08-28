#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXIMA_LONGITUD_CADENA 30

/*
Programa escrito por Alexander Perelman
El objetivo de este programa es poder manipular una nómina de empleados
utilizando apuntadores para crear una estructura dinámica. 
*/

//Definición de estructura direccion
typedef struct DIR {
	char calle[MAXIMA_LONGITUD_CADENA];
	int num_ext;
	char pais[MAXIMA_LONGITUD_CADENA];
	int cp;
	char ciudad[MAXIMA_LONGITUD_CADENA];
};

//Definición de estructura empleado
typedef struct EMP {
	char nombre[MAXIMA_LONGITUD_CADENA];
	char genero;
	char e_civil;
	int id; // se identifica al empleado por su número de ingreso
	int num_hijos;
	float salario;
	struct DIR direccion;
	struct EMP* prox;
} empleado;

empleado* crear_empleado() {
	/*
		Función para solicitar la memoria y almacenar la info de un empleado
		no recibe ningún parametro y regresa un apuntador al empleado creado
	*/
	empleado* e;
	e = (empleado*)malloc(sizeof(empleado));
	if (e == NULL) {
		printf("Sin memoria");
		exit(0);
	}//if
	fgetc(stdin);
	printf("Agregar datos para un empleado\n");
	printf("Nombre completo: ");
	fgets(e->nombre, MAXIMA_LONGITUD_CADENA, stdin);
	e->nombre[strlen(e->nombre) - 1] = '\0';
	printf("Genero (M/F/N): ");
	scanf_s("%c", &e->genero); 
	fgetc(stdin);
	printf("Estado civil (S/C/N): ");
	scanf_s("%c", &e->e_civil);
	printf("Numero de ingreso: ");
	scanf_s("%d", &e->id);
	printf("Numero de hijos: ");
	scanf_s("%d", &e->num_hijos);
	printf("Salario mensual: ");
	scanf_s("%f", &e->salario);
	fgetc(stdin);
	printf("calle o avenida: ");
	fgets(e->direccion.calle, MAXIMA_LONGITUD_CADENA, stdin);
	e->direccion.calle[strlen(e->direccion.calle) - 1] = '\0';
	printf("Numero externo: ");
	scanf_s("%d", &e->direccion.num_ext);
	fgetc(stdin);
	printf("Ciudad: ");
	fgets(e->direccion.ciudad, MAXIMA_LONGITUD_CADENA, stdin);
	e->direccion.ciudad[strlen(e->direccion.ciudad) - 1] = '\0';
	printf("Codigo postal: ");
	scanf_s("%d", &e->direccion.cp);
	fgetc(stdin);
	printf("Pais: ");
	fgets(e->direccion.pais, MAXIMA_LONGITUD_CADENA, stdin);
	e->direccion.pais[strlen(e->direccion.pais) - 1] = '\0';
	e->prox = NULL;
	return e;
}//func

empleado* insertar_empleado(empleado* inicio) {
	/*
		Función para insertar un empleado en la lista ordenado por su id
		inicio: apuntador a la lista de empleados
		regresa el apuntador a la lista
	*/
	empleado* nuevo;
	empleado* actual;
	int id_nuevo;
	
	nuevo = crear_empleado();
	id_nuevo = nuevo->id;

	if (inicio == NULL) {//si la lista está vacía
		inicio = nuevo;
	}//if
	else if (id_nuevo < inicio->id) { //Si su id es menor que el primer empleado, se vuelve el primero de la lista 
		nuevo->prox = inicio;
		inicio = nuevo;
	}
	else{ //busca su lugar en la lista 
		actual = inicio;
		while (actual->prox != NULL) {
			if (id_nuevo < actual->prox->id) {
				break;
			}//if
			actual = actual->prox;
		}//while
		nuevo->prox = actual->prox;
		actual->prox = nuevo;
	}//else
	return inicio;
}//func

empleado* borrar_empleado(empleado* inicio, int id_elim) {
	empleado* actual;
	if (inicio == NULL) { //Si la lista esta vacía
		printf("Lista vacía");
	}//if
	else if (inicio->id == id_elim) { //Si el empleado a eliminar es el primero de la lista
		inicio = inicio->prox;
	}//else if
	else {
		actual = inicio;
		while (actual->prox != NULL) {
			if (actual->prox->id == id_elim) { //encontramos el id a eliminar
				actual->prox = actual->prox->prox;
				break;
			}//if
			if (actual->prox->id > id_elim) { //al estar ordenada la lista, si se pasa del id es que no está
				break; //esto ayuda a no tener que siempre recorrer toda la lista en caso de que no esté
			}//if
			actual = actual->prox;
		}//while
		if (actual->prox == NULL || actual->prox->id > id_elim) {
			printf("El empleado no esta en la lista");
		}//if
	}//else
	return inicio;
}

int num_empleados(empleado* inicio) {
	/*
		Función que cuenta el número de empleados en la lista
		recibe el apuntador al inicio de la lista
		regresa el número de empleados
	*/
	int i = 0;
	empleado* actual;
	if (inicio != NULL) {
		actual = inicio;
		i = 1;
		while (actual->prox != NULL) {
			i++;
			actual = actual->prox;
		}//while
	}//if
	return i;
}//func

void imprime_empleado(empleado* e) {
	/*
		Imprime la información de un empleado
		recibe el apuntador al empleado a imprimir.
	*/
	printf("\n\nInformación del empleado: \n");
	printf("Nombre del empleado: %s\n", e->nombre);
	printf("Numero de ingreso: %d\n", e->id);
	printf("Genero del empleado: %c\n", e->genero);
	printf("Estado civil: %c\n", e->e_civil);
	printf("Numero de hijos del empleado: %d\n", e->num_hijos);
	printf("Salario mensual del empleado: %f\n", e->salario);
	printf("Calle o avenida: %s\n", e->direccion.calle);
	printf("Numero externo: %d\n", e->direccion.num_ext);
	printf("Codigo Postal: %d\n", e->direccion.cp);
	printf("Ciudad: %s\n", e->direccion.ciudad);
	printf("Pais: %s\n", e->direccion.pais);
}

void mostrar_empleado(empleado* inicio, int id_mostrar) {
	/*
		Imprime la información de un empleado en particular
		recibe el apuntador al inicio y el id del empleado a imprimir
	*/
	empleado* actual;
	if (inicio == NULL) { //Si la lista esta vacía
		printf("Lista vacía");
	}//if
	else if (inicio->id == id_mostrar) { //Si el empleado a imprimir es el primero de la lista
		imprime_empleado(inicio);
	}//else if
	else {
		actual = inicio;
		while (actual->prox != NULL) {
			if (actual->prox->id == id_mostrar) { //encontramos el id a imprimir
				imprime_empleado(actual->prox);
				break;
			}//if
			if (actual->prox->id > id_mostrar) { //al estar ordenada la lista, si se pasa del id es que no está
				break; //esto ayuda a no tener que siempre recorrer toda la lista en caso de que no esté
			}//if
			actual = actual->prox;
		}//while
		if (actual->prox == NULL || actual->prox->id > id_mostrar) {
			printf("El empleado no esta en la lista");
		}//if
	}//else
}


void los_empleados(empleado* inicio) {
	/*
		Muestra por pantalla la información de todos los empleados
		inicio: apuntador a la lista de empleados
	*/
	if (inicio == NULL) {
		printf("\n No hay empleados en la lista \n");
	}//if
	else {
		empleado* e = inicio;
		while (e != NULL) {
			imprime_empleado(e);
			e = e->prox;
		}//while
		delete e;
	}//else
}//func

void imprime_opciones() {
	/* funcion auxiliar para imprimir las opciones del menu
	*/
	printf("\n Menu de acciones: \n");
	printf("\n 1. insertar empleado \n");
	printf("\n 2. eliminar empleado \n");
	printf("\n 3. contar empleados \n");
	printf("\n 4. mostrar un empleado \n");
	printf("\n 5. mostrar a todos los empleados \n");
	printf("\n 6. salir \n");
	
}

int main() {
	empleado* inicio = NULL;
	int accion = 0;
	int id = 0;
	int count = 0;
	int val;
	printf("\n Bienvenido a la nomina de empleados \n");
	imprime_opciones();
	
	while (1) {
		printf("\n escribe el numero de la accion que quieres: ");

		while (scanf_s("%d", &accion) == 0) {
			printf("\nError: Input invalido, asegurate que sea un numero: ");
			fgetc(stdin);
		}
			
		switch (accion) {
		case 1:
			inicio = insertar_empleado(inicio);
			break;
		case 2: 
			printf("\n ID del empleado a eliminar: ");
			scanf_s("%d", &id);
			inicio = borrar_empleado(inicio, id);
			break;
		case 3:
			count = num_empleados(inicio);
			printf("\n El numero de empleados en la nomina es %d", count);
			break;
		case 4:
			printf("\n ID del empleado a imprimir: ");
			scanf_s("%d", &id);
			mostrar_empleado(inicio, id);
			break;
		case 5: 
			los_empleados(inicio);
			break;
		case 6:
			exit(0);
			break;
		default:
			printf("Opcion equivocada");

		}//switch
	}//while
	return 0;
}//main
