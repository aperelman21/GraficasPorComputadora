#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXIMA_LONGITUD_CADENA 30

//Definicion de estructura direccion
typedef struct DIR {
	char calle[MAXIMA_LONGITUD_CADENA];
	int num_ext;
	char pais[MAXIMA_LONGITUD_CADENA];
	int cp;
	char ciudad[MAXIMA_LONGITUD_CADENA];
};

//Definicion de estructura empleado
typedef struct EMP {
	char nombre[MAXIMA_LONGITUD_CADENA];
	char genero;
	char e_civil;
	int id;
	int num_hijos;
	float salario;
	struct DIR direccion;
	struct EMP* prox;
} empleado;

empleado* crear_empleado() {
	/*
		Funcion para solicitar la memoria y almacenar la info de un empleado
		no recibe ningun parametro y regresa un apuntador al empleado creado
	*/
	empleado* e;
	e = (empleado*)malloc(sizeof(empleado));
	if (e == NULL) {
		printf("Sin memoria");
		exit(0);
	}//if
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
	empleado* nuevo;
	empleado* actual;
	int id_nuevo;
	
	nuevo = crear_empleado();
	id_nuevo = nuevo->id;

	if (inicio == NULL) {//si la lista esta vacia
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

void los_empleados(empleado* inicio) {
	if (inicio == NULL) {
		printf("\n No hay empleados en la lista \n");
	}//if
	else {
		empleado* e = inicio;
		while (e != NULL) {
			printf("\n\nInformacion del empleado\n");
			printf("Nombre del empleado %s\n", e->nombre);
			printf("Numero de ingreso %d\n", e->id);
			printf("Genero del empleado %c\n", e->genero);
			printf("Estado civil: %c\n", e->e_civil);
			printf("Numero de hijos del empleado %d\n", e->num_hijos);
			printf("Salario mensual del empleado %f\n", e->salario);
			printf("Calle o avenida %s\n", e->direccion.calle);
			printf("Numero externo %d\n", e->direccion.num_ext);
			printf("Codigo Postal %d\n", e->direccion.cp);
			printf("Ciudad %s\n", e->direccion.ciudad);
			printf("Pais %s\n", e->direccion.pais);
			e = e->prox;
		}//while
		delete e;
	}//else
}//func

int main() {
	empleado* inicio = NULL;
	inicio = insertar_empleado(inicio);
	inicio = insertar_empleado(inicio);
	inicio = insertar_empleado(inicio);
	//insertar_empleado(inicio);
	los_empleados(inicio);
}
