#ifndef _LIB_H_
#define _LIB_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<io.h>
#include<errno.h>
#include<dirent.h>
#include<stdbool.h>
#include<sys/stat.h>
#include<ctype.h>
#include<sys/types.h>
#include<math.h>


typedef enum {NUM, DATE, EMAIL, TEXT, ERROR} TYPE;


typedef struct tab
{
    char nombre[100];
    int numCampos;
    char **campos;
    TYPE *tipos;
    struct tab *sig, *ant;

}TABLE;

typedef struct line
{
    char **valores;
    struct line *sig, *ant;
}ROW;



// Funciones Rutinarias // 

void DatosAlumno();
char *strtok2 (char *cadena, char *sep, int comillas);
char *ObtenerCampos (char *lin, char *sep, int id, char *val);
void Trim(char *cad);
int FechatoNumero (char* cad);



// Funciones de Validación // 

int ValidarCaracteres (char* nombre_bd);
int ValidarCaracteresBD (char* nombre_bd);
TYPE ValidarTipo (char *valor); 
int VerDuplicados(char **cadena, int num_arg); 
int EsDecimalValido (char *cadena);
int EsFechaValidaDMA (char *cadena);
bool EsCorreoValido (char *cadena); 



// Funciones de Carpetas / Directorios //

void MostrarBasesDeDatos(); 
int EntrarDirectorio (char *nombre_bd); 
void CrearBD (char *nombre_bd); 



// Funciones del Meta //

void CrearMeta (char *nombre_bd, char *aux2);
void EditarMeta (char *nombre_bd, TABLE *tabla);



// Funciones de Tablas //

void MostrarTablas (TABLE *t, char *nombre_carpeta);
TABLE* InicializarTablas (int num_campos, char *nombre_tabla, char **campos, TYPE *tipos); 
void CrearTabla (TABLE *tabla, char *nombre_bd, char *nombre_tabla);
void InsertarUltimaTabla (TABLE *LaGranTabla, TABLE *tabla_auxliar); 
int EntrarTabla (char *nombre_db, char *nombre_tabla);
TABLE *BuscarTabla (char *nombre_tabla, TABLE *tabla);
int ContarTablas (char *nombre_bd); 
TABLE *CargarTablas (char *nombre_bd);
void LiberarMemoriaTablas (TABLE *GranTabla); 
TABLE* borrarTabla (TABLE *tabla, char *tablaborrar); 
TABLE *CopiarTabla (char *nombre_tabla, char *nombre_bd_origen, char *nombre_bd_destino, TABLE *LaGranTabla); 


// Funciones de Líneas //

ROW *InicializarLinea (char **valores);
void InsertarUltimaLinea(ROW *LaGranLinea, ROW *linea_auxiliar);
void ActualizarLineas (char *nombre_db, ROW *linea, TABLE *tabla);
ROW *CargarLineas(char *nombre_db, char *nombre_tabla ,int num_campos, TYPE* tipos);
void LiberarMemoriaLineas (ROW *GranLinea, int numCampos); 
ROW *BorrarLineas(int numCampos, ROW *Big_Row, TYPE tipo, char *valor, char *operador, int pos); 



// Funciones de impresión //

void ImprimirEncabezado (TABLE *tabla_actual); 
void ImprimirTablaEntera (int numCampos, ROW *Big_Row, TABLE *t);
void ImprimirTablaAscendentemente (int numCampos, ROW *Big_Row, TABLE *tabla_actual); 
void ImprimirTablaDescendentemente(int numCampos, ROW *Big_Row, TABLE *tabla_actual); 
void ImprimirCampos(int numCampos, ROW *Big_Row, TYPE tipo, char *valor, char *operador, int pos); 



// Funciones de ordenar //

void OrdenarAscendentemente(ROW *Big_Row, TABLE *tabla_actual, int campo); 
void OrdenarDescendentemente(ROW *Big_Row, TABLE *tabla_actual, int campo); 
void intercambio_valores(ROW *linea1, ROW *linea2); 







#endif