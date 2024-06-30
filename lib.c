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

#include "lib.h"



/** Funciones rutinarias **/

void DatosAlumno() // Muestra por pantalla el nombre y el correo del alumno. 
{
    printf("- Nombre: Joan Amoros Ramirez\n");
    printf("- Email: joan.amoros@goumh.umh.es\n");
}


char *strtok2 (char *cadena, char *sep, int comillas) // función strtok con comillas --> comillas activadas = 1; comillas desactivadas = 0
{ 
    static char *pt1 = NULL;
    static char *pt2 = NULL;
    static char *ptFinal;

    if(cadena != NULL)
	{
        pt1 = cadena;
        ptFinal = pt1 + strlen(cadena);
    }
    else
	{
        pt1 = pt2+1;
    }

    if(comillas == 1)
	{
        while(pt1[0] != '\0' && pt1 < ptFinal && strchr(sep, pt1[0]) != NULL && pt1[0] != '"')
		{
            pt1++;
        }

        if(pt1[0] == '"')
		{
            pt1++;
            pt2 = pt1+1;
			
            while(pt2[0] != '\0' && pt2[0] != '"')
			{
                
                pt2++;
            }

            if(pt2[0] == '\0')
			{
                pt1 = pt2 = ptFinal = NULL;
                return NULL;

            }
            else
			{
                pt2[0]='\0';
                return pt1;
            }
        }
    }
    else
	{
        while(pt1[0] != '\0' && pt1<ptFinal && strchr(sep, pt1[0]) != NULL)
		{
            pt1++;
        }
    }

    if(pt1>=ptFinal)
	{
        pt1 = pt2 = ptFinal = NULL;
        return NULL;
    }

    pt2 = pt1 +1;

    while(pt2[0] != '\0' && strchr(sep, pt2[0]) == NULL)
	{
        pt2++;
    }

    pt2[0]='\0';

    return pt1;

}


char *ObtenerCampos (char *lin, char *sep, int id, char *val) // Obtiene el nombre de cada campo // 
{
    int i, tamSep = 1;
    char *p1, *p2;

    if(lin == NULL || lin[0] == '\0')
	{
        return NULL;
    }

    p1 = lin;
    p2 = strstr(lin, sep);

    for(i=0; i<id; i++)
	{
        p1 = p2 + tamSep;
        p2 = strstr(p1, sep);
    }

    if(p2 == NULL)
	{
        strcpy(val, p1);
    }
    else
	{
        strncpy(val, p1, p2-p1);
        val[p2-p1] = '\0';
    }

    return val;
}


void Trim (char *cad) // Esta función quita los espacios en blanco // 
{
	int c, i, j, n=strlen(cad);
	
	if (cad==NULL || n<1)
		return;
	n--;
	while(n>=0 && ((c=cad[n])==' ' || c=='\t' || c=='\n' || c=='\r' || c=='"'))
		n--;
	cad[n+1]='\0';
	i=0;
	while (i<=n && ((c=cad[i])==' ' || c=='\t' || c=='\n' || c=='\r' || c=='"'))
		i++;
	for (j=0 ; j<=n-i+1; j++)
		cad[j]=cad[j+i];
}


int FechatoNumero (char* cad) // de fecha pasamos a numero en el formato DMA
{
	int res, dia, mes, anyo;
	
	
	dia = (cad[0]-'0')*10 +(cad[1]-'0');

	mes = (cad[3]-'0')*10 +(cad[4]-'0');
		
	anyo = (cad[6]-'0')*1000 +(cad[7]-'0')*100 +(cad[8]-'0')*10 +(cad[9]-'0');
	
	
	res = dia;

	if (mes==1)
		res=res;
	else if (mes==2)
		res=res+31;
	else if (mes==3 && (anyo%4)!=0)
		res=res+59;
	else if (mes==3 && (anyo%4)==0)
		res=res+60;
	else if (mes==4 && (anyo%4)!=0)
		res=res+90;
	else if (mes==4 && (anyo%4)==0)
		res=res+91;
	else if (mes==5 && (anyo%4)!=0)
		res=res+120;
	else if (mes==5 && (anyo%4)==0)
		res=res+121;
	else if (mes==6 && (anyo%4)!=0)
		res=res+151;
	else if (mes==6 && (anyo%4)==0)
		res=res+152;
	else if (mes==7 && (anyo%4)!=0)
		res=res+181;
	else if (mes==7 && (anyo%4)==0)
		res=res+182;
	else if (mes==8 && (anyo%4)!=0)
		res=res+212;
	else if (mes==8 && (anyo%4)==0)
		res=res+213;
	else if (mes==9 && (anyo%4)!=0)
		res=res+243;
	else if (mes==9 && (anyo%4)==0)
		res=res+244;
	else if (mes==10 && (anyo%4)!=0)
		res=res+273;
	else if (mes==10 && (anyo%4)==0)
		res=res+274;
	else if (mes==11 && (anyo%4)!=0)
		res=res+304;
	else if (mes==11 && (anyo%4)==0)
		res=res+305;
	else if (mes==12 && (anyo%4)!=0)
		res=res+334;
	else if (mes==12 && (anyo%4)==0)
		res=res+335;

	res=(res+(anyo-1)*365);
	
	res=(res+(anyo-1)/4);


	return res;
}




/** Funciones de validación **/

int ValidarCaracteres (char* nombre_bd) // Esta función se encarga de comprobar que el usuario ha escrito el nombre correctamente //
{
    char cadena[100] = "";
    int j = 0, errores = 0;

    strcpy(cadena, nombre_bd); // Aquí copiamos en cadena el parámetro

    while(cadena[j] != '\0') // Aqui vamos recorriendo la cadena caracter a caracter para comprobarlo
	{ 
        if(cadena[j] == '_')
		{
            j++;
            continue;
        }
        if(cadena[j] >= '0'  &&  cadena[j] <= '9')
		{
            j++;
            continue;
        }
        if(cadena[j] >= 'a'  &&  cadena[j] <= 'z')
		{
            j++;
            continue;
        }
        if(cadena[j] >= 'A'  &&  cadena[j] <= 'Z')
		{
            j++;
            continue;
        }

        printf("ERROR de caracteres.\n");
        errores++;
        break;
    }
	
    return errores;
}


int ValidarCaracteresBD (char* nombre_bd) // Valida la cadena de caracteres de una base de datos
{
    char copia[100] = "";
    int j = 0, errores = 0;
	
	strcpy(copia, nombre_bd);

    while(copia[j] != '\0') // Aquí vamos recorriendo la cadena caracter a caracter para comprobarlo
	{ 
        if(copia[j] != '.')
		{
            j++;
        }
        else 
		{
			errores++;
			break;
		}
    }
	
    return errores;
}


TYPE ValidarTipo(char *valor) // Esta función se encarga de comprobar que los tipos son los correctos
{
    if(strcmp(valor, "NUM") == 0) // 0 
	{
        return NUM;
    }
    else if(strcmp(valor, "DATE") == 0) // 1 
	{
        return DATE;
    }
	else if(strcmp(valor, "EMAIL") == 0) // 2 
	{
        return EMAIL;
    }
    else if(strcmp(valor, "TEXT") == 0) // 3 
	{
        return TEXT;
    }
    else // 4 
	{
        return ERROR;
    }
}


int VerDuplicados (char **cadena, int num_arg) // Comprueba que los campos de una misma tabla no se repitan 
{
    int i, j;

    for(i=0; i<num_arg; i++)
	{
        for(j=i+1; j<num_arg; j++)
		{
            if(strcmp(cadena[i], cadena[j]) == 0)
			{
                printf("ERROR: No se pueden repetir los campos.\n");
                return 1;
            }
        }
    }

    return 0;

}


int EsDecimalValido (char *cadena) // Función que sirve para validar números, devuelve 1 si todo ok y 0 si algo mal
{
    int i;
	int p = 0;

    if(cadena[0] == '+' || cadena[0] == '-')
	{
        i = 1;
    }
    else
	{
        i = 0;
    }

    if(strlen(cadena)==1 && i == 1) // caso de que solo escribe + o -
	{ 
        return 0;
    }

    if(strlen(cadena) == 0)
	{
        return 0;
    }

    if(i == 1 && strlen(cadena) == 2 && cadena[1] == '.') // caso de +. o -.
	{ 
        return 0;
    }
	
    if(strlen(cadena) == 1 && i == 0 && cadena[0] == '.') // caso de .
	{ 
        return 0;
    }

    for(i; i<strlen(cadena); i++)
	{
        if(cadena[i] < '0' || cadena[i] > '9') // en el caso de que no sea un numero la posición i
		{ 
            if(cadena[i] == '.')
			{
                p++; // aumentamos el contador de comas
            }
            else
			{
                return 0;
            }
        }
        if(p == 2) // en el caso de que el número tenga mas de una coma
		{ 
            return 0;
        }
    }

    return 1; // si todo ha salido bien


}


int EsFechaValidaDMA (char *cadena) // Función que valida una fecha en formato DD/MM/AAAA
{ 
    int i, d, m, a;
    
    if(strlen(cadena) != 10)
	{
        return 0;
    }

    for(i = 0; i < 10; i++)
	{

        if(i != 2 && i != 5)
		{

            if(cadena[i]<'0' || cadena[i] > '9')
			{
                return 0;
            }
        }
    }
	
    if(cadena[2] != '/' && cadena[5] != '/')
	{ 
        return 0;
    }

    d=(cadena[0]-'0')*10 +(cadena[1]-'0');
    m=(cadena[3]-'0')*10 +(cadena[4]-'0');
    a=(cadena[6]-'0')*1000 +(cadena[7]-'0')*100+(cadena[8]-'0')*10+ (cadena[9]-'0'); 

    if(d<1 || m<1 || a<1 || d>31 || m>12 || a>9999){
     
        return 0;
    }
    if ((m == 4 || m == 6 || m == 9 || m == 11) && d > 30) // Los meses de abril, junio, septiembre y noviembre no tienen más de 30 días. 
	{
		return 0; // FALSO.
	}
	if (m == 2) // Caso particular para el mes de febrero. 
	{
		if (d > 29) // Ningún mes de febrero tiene más de 29 días.
		{
			return 0; 
		}
		if ((a%4 != 0) && d > 28) // Los años que no son bisiestos no tienen más de 28 días. 
		{
			return 0;
		}
		if ((a%400 == 0) && d > 29) // Sin embargo, los años múltiplos de 400 son bisiestos aunque sean divisibles entre 100. 
		{
			return 0; 
		}
	}

    return 1;

}


bool EsCorreoValido(char *cadena) // Función para determinar si un correo es válido
{
	bool emailValido = true;
	int nArrobasValido = 0, nPuntosValido = 0;

	for(int i = 0; i < strlen(cadena); i++) 
	{
		
		if(cadena[i] == '@') 
		{
			nArrobasValido++;

			if((isdigit(cadena[i-1]) || isalpha(cadena[i-1])) && (isdigit(cadena[i+1]) || isalpha(cadena[i+1]))) 
			{
				continue;
			}
			else 
			{
				emailValido = false;
			}

		} 
		else if(cadena[i] == '.') 
		{
			nPuntosValido++;
				
			if((isdigit(cadena[i-1]) || isalpha(cadena[i-1])) && (isdigit(cadena[i+1]) || isalpha(cadena[i+1]))) 
			{
				continue;
			}
			else 
			{
				emailValido = false;
			}
		
		} 
		else if (isdigit(cadena[i]) || isalpha(cadena[i])) 
		{
			continue;
		
		} 
		else
		{
			emailValido = false;
		}
	}
	
	if(cadena[strlen(cadena)-1] == '.' || cadena[strlen(cadena)-1] == '@') emailValido = false;
	if(nArrobasValido == 1 && nPuntosValido == 1);
	else emailValido = false;

	return emailValido;
}




/** Funciones de Carpetas/Directorios **/

void MostrarBasesDeDatos() // Lista el nombre de todas las bases de datos
{

    DIR *d;
    struct dirent *f;
    int j =0;
    
    d = opendir(".");

    if(d == NULL)
	{
        printf("NO hay bases de datos.\n");
        return;
    }
	else
	{
		while((f = readdir(d)) != NULL)
		{
			if ((strcmp(f->d_name, ".")!=0) && (strcmp(f->d_name, "..")!=0))
			{
				
				if(ValidarCaracteresBD(f->d_name) == 0)
				{
					printf("- %s\n", f->d_name);
					j++;
				}
				
			}
		}
		
		if(j==0)
		{
			printf("NO hay bases de datos.\n");
			return;
		}
	}

    

    if(j == 0)
	{
        printf("NO hay bases de datos.\n");
        return;
    }

    closedir(d);

}


int EntrarDirectorio (char *nombre_bd) // Esta función se encarga de comprobar si existe una carpeta
{
    DIR *dir;
	
    if(dir = opendir(nombre_bd)) // si existe la base de datos
	{
        return 0;
    }
    else // si no existe 
	{ 
        printf("La base de datos '%s' no existe.\n", nombre_bd);
        return 1;
    }
}


void CrearBD (char *nombre_bd) // Esta función nos va a crear una carpeta en el ordenador, donde vamos a ir guardando las tablas
{
    char aux1[15] = "\\meta.db";
    char aux2[50] = "";

    strcpy(aux2, nombre_bd);
    strcat(aux2, aux1);
	
    if(access(nombre_bd, F_OK) == 0)
	{
        printf("La carpeta ya estaba creada.\n");
    }
    else
	{
        mkdir(nombre_bd); // creamos la carpeta
        CrearMeta(nombre_bd, aux2); // llamamos a la función para crear el archivo meta.db
    }
}




/** Funciones del meta **/

void CrearMeta (char *nombre_bd, char *aux2) // Esta función se encarga de crear el meta.db
{
    DIR *dir;

    if(dir = opendir(nombre_bd))
	{
        FILE *fichero;
        fichero = fopen(aux2, "w");

        if(fichero != NULL)
		{
            fprintf(fichero, "TABLAS: 0\n");
            fprintf(fichero, "--------------------\n");

            fclose(fichero);
        }
        else
		{
            printf("algo salio mal");
        }
    }
}


void EditarMeta(char *nombre_bd, TABLE *tabla) // Sirve para modificar el meta.db
{
    
    DIR *dir;

    char aux1[11] = "\\meta.db";
    char aux2[100]= "";
    char aux_tipos[50]= "";
    int numero_tablas = 0, i;

    strcpy(aux2, nombre_bd);
    strcat(aux2, aux1);

    numero_tablas = ContarTablas(nombre_bd); // aqui contamos tablas

    if(dir = opendir(nombre_bd))
	{
        FILE *fichero;
        fichero = fopen(aux2,"w");

        if(fichero != NULL)
		{
            fprintf(fichero, "TABLAS: %d\n", numero_tablas);
            fprintf(fichero, "---------------------\n");

            while(tabla != NULL){

                fprintf(fichero, "%s: %d\n", tabla->nombre, tabla->numCampos);

                for(i=0; i<tabla->numCampos; i++)
				{
                    if(tabla->tipos[i] == 0)
					{
                        strcpy(aux_tipos, "NUM");
                    }
                    else if(tabla->tipos[i] == 1)
					{
                        strcpy(aux_tipos, "DATE");
                    }
                    else if(tabla->tipos[i] == 2)
					{
                        strcpy(aux_tipos, "EMAIL");
                    }
					else if(tabla->tipos[i] == 3)
					{
                        strcpy(aux_tipos, "TEXT");
                    }

                    fprintf(fichero, "%s: %s\n", tabla->campos[i], aux_tipos);

                }
                
                if(tabla->sig == NULL)
				{
                    break;
                }

                fprintf(fichero, "---------------------\n");
                tabla = tabla->sig;

            }
        }

        fclose(fichero);
    }
}




/** Funciones de tablas **/

void MostrarTablas (TABLE *t, char *nombre_carpeta) // Muestra las tablas que hay en una base de datos
{

    TABLE *tablaaux = t;
    int i, j;
    char aux[10]= "";

    if(ContarTablas(nombre_carpeta)==0)
	{
        printf("NO hay tablas.\n");
        return;
    }
	

    for(i=0; i<ContarTablas(nombre_carpeta); i++)
	{
        printf("- %s.tab\n", tablaaux->nombre);
        tablaaux = tablaaux->sig;
    }
}


TABLE* InicializarTablas (int numCampos, char *nombre_tabla, char **campos, TYPE *tipos) // Inicializa una tabla
{
    TABLE *tabla = malloc(sizeof(TABLE)); // Reservamos memoria para una tabla

    if(tabla == NULL)
	{
        return NULL;
    }

    strcpy(tabla->nombre, nombre_tabla); // Copiamos en la tabla el nombre de la tabla
    
	tabla->numCampos = numCampos;
    
	tabla->campos = campos;
    tabla->tipos = tipos;
    
	tabla->ant = NULL;
    tabla->sig = NULL;


    return tabla;
}


void CrearTabla (TABLE *tabla, char *nombre_bd, char *nombre_tabla) // Esta función sirve para crear el fichero .tab (el archivo de la tabla)
{ 
    //new table clientes codigo NUM nombre TEXT fecha DATE direccion TEXT telefono TEXT//
	//new table elche umh NUM prog DATE clase TEXT practicas EMAIL //
	//new table ejemplo asd TEXT qwerty EMAIL pepito TEXT //
	//new table joan insert TEXT fecha DATE horario DATE hey NUM //
	//new table cosas id NUM denominacion TEXT peso NUM volumen NUM entrega DATE//
	//new table personas nombre TEXT edad NUM fecha DATE
	
	
	char aux1[5] = ".tab";
    char aux2[5] = "\\";
    char aux3[100] = "";

    strcpy(aux3, nombre_bd);
    strcat(aux3, aux2);
    strcat(aux3, nombre_tabla);
    strcat(aux3, aux1);

    if(access(aux3, F_OK) == 0)
	{
        printf("La tabla %s ya fue creada\n", nombre_tabla);
    }
    else
	{
        DIR *dir;
        if(dir = opendir(nombre_bd))
		{
            FILE *fichero;
            fichero = fopen(aux3, "w");

            if(fichero != NULL)
			{
                EditarMeta(nombre_bd, tabla); // Editamos el meta
            }

            fclose(fichero);
        }
        else
		{
            printf("ERROR inesperado.\n");
        }  
    }    
}


void InsertarUltimaTabla (TABLE *GranTabla, TABLE *tabla_auxliar) // Función que recorre la GranTabla y se sitúa en la última posición
{
    TABLE *GranTabla2;
	
    GranTabla2 = GranTabla;
	
    while(GranTabla2->sig != NULL)
	{
        GranTabla2 = GranTabla2->sig;
    }

    GranTabla2->sig = tabla_auxliar;
	
    tabla_auxliar->ant = GranTabla2;
}


int EntrarTabla (char *nombre_db, char *nombre_tabla) // Función que se encarga de decirnos si una tabla existe
{
    DIR *dir;
	
    char aux1[10] = ".tab";
    char aux2[5] = "\\";
    char aux3[100] = "";

    strcpy(aux3, nombre_db);
    strcat(aux3, aux2);
    strcat(aux3, nombre_tabla);
    strcat(aux3, aux1);


    if(access(aux3, F_OK) == 0) // La tabla sí existe
	{
        if(dir = opendir(nombre_db))
		{
            return 1;
        }
    }
    else // La tabla no existe
	{
        return 0;
    }
}


TABLE *BuscarTabla (char *nombre_tabla, TABLE *tabla) // Busca la tabla que necesitemos 
{
	TABLE *tabla2 = tabla;
	
    if(tabla2 != NULL)
	{
        while(tabla2 != NULL)
		{
            if(strcmp(tabla2->nombre, nombre_tabla) == 0)
			{
                return tabla2;
            }

            tabla2 = tabla2->sig;
        }
    }
    else
	{
        return tabla2;
    }

}


int ContarTablas (char *nombre_bd) // Función que se encarga de contar las tablas de una base de datos
{
    DIR *dir;
    struct dirent *f; // Habilita trabajar con los ficheros de la carpeta
    int num = 0;

    if(dir = opendir(nombre_bd))
	{
        while(f = readdir(dir))
		{
            num++;
        }
    }

    num = num - 3;
	
    return num;
}


TABLE *CargarTablas (char *nombre_bd) // Carga las tablas en memoria //
{

    TABLE *tabla = NULL, *tablanueva;
	
    FILE *meta;
	
    char aux1[11] = "\\meta.db";
    char aux2[50] = "";
    char aux3[50] = "";
	
    char linea[1000], val[100];
	
    int num_tabla, num_campos, i, j;
	
    char *nombre_tabla;
    char **nombre_campo;
	
    TYPE *tipos; 
    

    strcpy(aux2,nombre_bd);
    strcat(aux2,aux1);

    meta = fopen(aux2, "r"); // Abrimos el meta.db en modo lectura
    if(meta == NULL)
	{
        printf("ERROR: El archivo meta.db no existe.\n");
        return 0;
    }
	
    fgets(linea,1000,meta); // Leemos los primeros mil caracteres de una línea
	
    ObtenerCampos(linea, ":", 1, val); // Obtenemos el valor del campo 1
	
    Trim(val);                  // Le quitamos los espacios al valor
    num_tabla = atoi(val);      // y lo convertimos a número

    for(i=0; i<num_tabla;i++) // Vamos a hacer este proceso tantas veces como tablas tengamos en el meta.db
	{ 
        fgets(linea,1000,meta); // IGNORAMOS LOS GUIONES DEL META
        fgets(linea,1000,meta); // Cogemos la línea del nombre de la tabla y num de campos. "JOAN: 2"
        
		ObtenerCampos(linea, ":", 0,val); // Cogemos el primer valor de la línea hasta los dos puntos "JOAN"
        Trim(val);                  // Quita los espacios
        
		nombre_tabla = strdup(val);   // Apuntamos nombre_tabla al valor de val
		
        ObtenerCampos(linea, ":", 1, val); // Cogemos el número de campos "2"
        Trim(val);                  // y quitamos los espacios
        
		num_campos = atoi(val);       // Convertimos a numero el valor de numcampos "2" 

        nombre_campo = malloc(sizeof(char*)*num_campos); // Reservamos memoria tipo puntero para los parámetros
        tipos = malloc(sizeof(TYPE*)*num_campos); //Reservamos memoria para los tipos
        
        for(j=0; j<num_campos; j++) // Lo vamos a hacer por cada columna de la tabla
		{ 
            fgets(linea,1000,meta); // Cogemos la línea
            
			ObtenerCampos(linea, ":", 0, val);  // Vemos el nombre del campo
            Trim(val);
            nombre_campo[j]=strdup(val); // Lo copiamos en la caja de nombre_campo que es un puntero doble
            
			ObtenerCampos(linea,":",1,val); // Cogemos el tipo
            Trim(val);
            tipos[j]=ValidarTipo(val); // Lo metemos en la caja de tipos
            
        }

        tablanueva = InicializarTablas(num_campos, nombre_tabla, nombre_campo, tipos); // Creamos la tabla

        if(tabla == NULL) // Si es la primera tabla
		{ 
            tabla = tablanueva;
                    
        }
        else // Si no la insertamos la última
		{ 
            InsertarUltimaTabla(tabla, tablanueva);          
        }

    }
    fclose(meta); // cerramos el meta
    return tabla;
}


void LiberarMemoriaTablas (TABLE *GranTabla) // Libera la memoria de una tabla // 
{
	TABLE *tablasiguiente; 
	
	int i; 
	
	if(GranTabla == NULL)
	{
		return;
	}
	
	while(GranTabla != NULL)
	{
		tablasiguiente = GranTabla->sig; 
		
		for(i=0; i<GranTabla->numCampos; i++)
		{
			free(GranTabla->campos[i]);
		}
		
		free(GranTabla->campos);
		free(GranTabla->tipos);
		
		free(GranTabla);
		
		GranTabla = GranTabla->sig;
	}
	
	
}


TABLE* borrarTabla (TABLE *tabla, char *tablaborrar) // Función que borra una tabla de una base de datos
{
	int i;
	
	TABLE *tAux = NULL;
	TABLE *tSig = NULL;
	TABLE *tAnt = NULL; 
	
	tAux = BuscarTabla(tablaborrar, tabla); 
	
	tSig = tAux->sig; 
	tAnt = tAux->ant;
	
	if(tAux == tabla && tAux->sig == NULL) // Solo una tabla
	{
		tabla = NULL;
	}
	else if(tAux == tabla && tAux->ant == NULL) // Primera tabla
	{
		tabla = tabla->sig; 
		tSig->ant = NULL;
		tAux->sig = NULL;
	}
	else if(tAux->sig == NULL) // Última tabla
	{
		tAnt->sig = NULL;
		tAux->ant = NULL;
	}
	else // Tabla de en medio
	{
		tAnt->sig = tAux->sig; 
		tSig->ant = tAux->ant; 
	}
	
	// FREE
	
	for(i=0; i<tAux->numCampos; i++)
	{
		free(tAux->campos[i]);
	}
	
	free(tAux->campos);
	free(tAux->tipos);
	
	free(tAux);
	
	return tabla; 
    
}


TABLE *CopiarTabla (char *nombre_tabla, char *nombre_bd_origen, char *nombre_bd_destino, TABLE *GranTabla) // Función que copia una tabla de una carpeta a otra carpeta // 
{

    TABLE *GranTablaAux = NULL, *tabla_aux = NULL, *tabla_aux2 = NULL;
    ROW *GranLinea = NULL;
	
	GranTablaAux = CargarTablas(nombre_bd_origen);
	
	tabla_aux = BuscarTabla(nombre_tabla, GranTablaAux);
	tabla_aux2 = InicializarTablas(tabla_aux->numCampos, tabla_aux->nombre, tabla_aux->campos, tabla_aux->tipos);
	
	
	// Cargar las lineas correspondientes de esta tabla //
	GranLinea = CargarLineas(nombre_bd_origen, nombre_tabla, tabla_aux->numCampos, tabla_aux->tipos);
	
	
	if(GranTabla == NULL)
	{
		GranTabla = tabla_aux2; 
	}
	else
	{
		InsertarUltimaTabla(GranTabla, tabla_aux2);
	}
	
	CrearTabla(GranTabla, nombre_bd_destino, nombre_tabla);
	
	
	ActualizarLineas(nombre_bd_destino, GranLinea, GranTabla);
	
	
	
	return GranTabla;
}




/** Funciones de líneas **/

ROW *InicializarLinea (char **valores) // Inicializa una línea
{
    ROW *linea = malloc(sizeof(ROW)); // Reserva memoria para el tamaño de una línea

    if(linea == NULL)
	{
        return NULL;
    }

    linea->valores = valores;
    linea->sig = NULL;
    linea->ant = NULL;

    return linea;
}


void InsertarUltimaLinea(ROW *LaGranLinea, ROW *linea_auxiliar) // Inicializa una línea
{

    ROW *LaGranLinea2 = LaGranLinea;

    while(LaGranLinea2->sig != NULL)
	{
        LaGranLinea2 = LaGranLinea2->sig;
    }

    LaGranLinea2->sig = linea_auxiliar;
    linea_auxiliar->ant = LaGranLinea2;
    linea_auxiliar->sig = NULL;
}


void ActualizarLineas (char *nombre_db, ROW *linea, TABLE *tabla) // Esta función se encarga realmente de modificar el fichero .tab cuando se le hagan modificaciones
{

    DIR *dir;

    char aux1[11] = ".tab";
    char aux2[50] = "\\";
    char aux3[50] = "";

    strcpy(aux3, nombre_db);
    strcat(aux3, aux2);
    strcat(aux3, tabla->nombre);
    strcat(aux3, aux1);


    if(dir = opendir(nombre_db))
	{
        FILE *fichero;
        fichero = fopen(aux3, "w");

        if(fichero != NULL)
		{
            //escribir todas las lineas
            while(linea != NULL)
			{
                for(int i = 0; i<tabla->numCampos ; i++)
				{ 
                    if(tabla->tipos[i] == TEXT && i == tabla->numCampos-1)
					{ 
                        fprintf(fichero, "\"%s\"\n", linea->valores[i]);
                    }
                    else if(tabla->tipos[i] == TEXT)
					{
                        fprintf(fichero, "\"%s\", ", linea->valores[i]);
                    }
                    else if(tabla->numCampos-1 == i)
					{
                        fprintf(fichero, "%s\n", linea->valores[i]);
                    }
                    else
					{
                        fprintf(fichero, "%s, ", linea->valores[i]);
                    }
                }

                if(linea->sig == NULL)
				{
                    break;
                }

                linea = linea->sig;
            }
            
            fclose(fichero);
        }
        else
		{
            printf("ERROR inesperado.\n");
        }

    }
    else
	{
        printf("ERROR: No nos deja entrar a la carpeta.\n");
    }

}


ROW *CargarLineas (char *nombre_db, char *nombre_tabla ,int num_campos,  TYPE* tipos) // Carga las líneas de una tabla en memoria // 
{

    char aux1[5] = ".tab";
    char aux2[5] = "\\";
    char aux3[100] = "";
    
	FILE *fichero;
    
	char linea[1000];
    
	int i,comillas;
    
	long final;
    
	char *token, *lineaaux;
    char **valores;
    
	ROW *fila = NULL, *filanueva;

    strcpy(aux3,nombre_db);
    strcat(aux3,aux2);
    strcat(aux3,nombre_tabla);
    strcat(aux3,aux1);

    if(access(aux3,F_OK) == 0) // Si podemos acceder a la ruta entramos
	{ 
        fichero = fopen(aux3, "r"); // Abrimos el fichero en modo lectura

        fseek(fichero, 0, SEEK_END); // Sitúa el cursor en la última posición

        if (ftell(fichero) == 0) // Dime donde está el cursor en el fichero, y si está en la posición 0 entramos
        {
            fclose(fichero);
            return fila;
        }

        final = ftell(fichero); // Dime donde está la posición del fichero
        fseek(fichero, 0, SEEK_SET); // Mueve el cursor al principio del fichero, es decir, a la posición 0


        while(final != ftell(fichero)) // Mientras el cursor no llegue al final del fichero
		{ 
            fgets(linea,1000,fichero); // Cogemos la línea

            valores = malloc(sizeof(char*)*num_campos); // Reservamos memoria del tamaño de los num_campos

            for(i=0; i<num_campos; i++)
			{
                comillas = (tipos[i] == TEXT); // Si hay un campo de tipo texto comillas valdrá 1
            
                if(i == 0)
				{
                    lineaaux = linea;
                }
                else
				{
                    lineaaux = NULL;
                }
				
                token = strtok2(lineaaux, ",\n", comillas); // Dividimos la línea en tokens con las comillas activadas
                valores[i] = strdup(token);
				Trim(valores[i]); // Muy importante, para que no deje espacios en blanco entre los valores de tipo texto 
            }
			
            filanueva = InicializarLinea(valores); // Creamos la estructura de la línea

            if(fila == NULL) // Si es la primera línea
			{ 
                fila = filanueva;       
            }
            else
			{
                InsertarUltimaLinea(fila, filanueva); // Si no, insertamos la línea la última  
            }
        }                 
    }
    else
	{
        printf("ERROR inesperado.\n");
        return 0;
    }

    fclose(fichero);
    return fila;

}


void LiberarMemoriaLineas (ROW *GranLinea, int numCampos) // Libera la memoria de una línea // 
{
	ROW *lineasiguiente; 
	
	int i; 
	
	if(GranLinea == NULL)
	{
		return;
	}
	
	while(GranLinea != NULL)
	{
		lineasiguiente = GranLinea->sig; 
		
		for(i=0; i<numCampos; i++)
		{
			free(GranLinea->valores[i]);
		}
		
		free(GranLinea->valores);
		free(GranLinea);
		
		GranLinea = lineasiguiente;
	}
	
}


ROW *BorrarLineas (int numCampos, ROW *Big_Row, TYPE tipo, char *valor, char *operador, int pos) // Función que borra líneas según una condición específica // 
{
	int i, j, k;
	j=1; 
	
	ROW *linea_auxiliar = Big_Row;
	ROW *linea_anterior = NULL; 
	ROW *linea_siguiente = NULL;

    if(Big_Row == NULL)
	{
        printf("ERROR: No se han encontrado datos.\n");
    }
	else
	{
		if(tipo == NUM)
		{
			if(strcmp(operador,"=")==0)
			{
				while(linea_auxiliar != NULL)
				{
					linea_anterior = linea_auxiliar->ant; 
					linea_siguiente = linea_auxiliar->sig;
					
					if(atof(linea_auxiliar->valores[pos]) == atof(valor))
					{
						if(linea_auxiliar==Big_Row && linea_auxiliar->sig == NULL) // solo si hay 1 fila
						{
							Big_Row = NULL; 
						}
						else if(linea_auxiliar == Big_Row && linea_auxiliar->ant == NULL) // si quiero borrar la primera fila
						{
							Big_Row = Big_Row->sig; 
							linea_siguiente->ant = NULL;
							linea_auxiliar->sig = NULL; // para comprobar
						}
						else if (linea_auxiliar->sig ==NULL) // si quiero borrar la ultima fila
						{
							linea_anterior->sig = NULL; 
							linea_auxiliar->ant = NULL;
						}
						else // si quiero borrar una fila del medio
						{
							linea_anterior->sig = linea_auxiliar->sig; 
							linea_siguiente->ant = linea_auxiliar->ant; 
						}
						
						for(i=0; i<numCampos; i++)
						{
							free(linea_auxiliar->valores[i]);
						}
						
						free(linea_auxiliar->valores);
						free(linea_auxiliar);
					}
					
					linea_auxiliar = linea_siguiente; 
					
				}
				
				return Big_Row; 
			}
			else if(strcmp(operador,"<")==0)
			{
				while(linea_auxiliar != NULL)
				{
					linea_anterior = linea_auxiliar->ant; 
					linea_siguiente = linea_auxiliar->sig;
					
					if(atof(linea_auxiliar->valores[pos]) < atof(valor))
					{
						if(linea_auxiliar==Big_Row && linea_auxiliar->sig == NULL) // solo si hay 1 fila
						{
							Big_Row = NULL; 
						}
						else if(linea_auxiliar == Big_Row && linea_auxiliar->ant == NULL) // si quiero borrar la primera fila
						{
							Big_Row = Big_Row->sig; 
							linea_siguiente->ant = NULL;
							linea_auxiliar->sig = NULL; // para comprobar
						}
						else if (linea_auxiliar->sig ==NULL) // si quiero borrar la ultima fila
						{
							linea_anterior->sig = NULL; 
							linea_auxiliar->ant = NULL;
						}
						else // si quiero borrar una fila del medio
						{
							linea_anterior->sig = linea_auxiliar->sig; 
							linea_siguiente->ant = linea_auxiliar->ant; 
						}
						
						for(i=0; i<numCampos; i++)
						{
							free(linea_auxiliar->valores[i]);
						}
						
						free(linea_auxiliar->valores);
						free(linea_auxiliar);
					}
					
					linea_auxiliar = linea_siguiente; 
					
				}
				
				return Big_Row; 
			}
			else if(strcmp(operador,">")==0)
			{
				while(linea_auxiliar != NULL)
				{
					linea_anterior = linea_auxiliar->ant; 
					linea_siguiente = linea_auxiliar->sig;
					
					if(atof(linea_auxiliar->valores[pos]) > atof(valor))
					{
						if(linea_auxiliar==Big_Row && linea_auxiliar->sig == NULL) // solo si hay 1 fila
						{
							Big_Row = NULL; 
						}
						else if(linea_auxiliar == Big_Row && linea_auxiliar->ant == NULL) // si quiero borrar la primera fila
						{
							Big_Row = Big_Row->sig; 
							linea_siguiente->ant = NULL;
							linea_auxiliar->sig = NULL; // para comprobar
						}
						else if (linea_auxiliar->sig ==NULL) // si quiero borrar la ultima fila
						{
							linea_anterior->sig = NULL; 
							linea_auxiliar->ant = NULL;
						}
						else // si quiero borrar una fila del medio
						{
							linea_anterior->sig = linea_auxiliar->sig; 
							linea_siguiente->ant = linea_auxiliar->ant; 
						}
						
						for(i=0; i<numCampos; i++)
						{
							free(linea_auxiliar->valores[i]);
						}
						
						free(linea_auxiliar->valores);
						free(linea_auxiliar);
					}
					
					linea_auxiliar = linea_siguiente; 
					
				}
				
				return Big_Row; 
			}
			else
			{
                printf("ERROR: El tercer parametro ha de ser un relacionador operacional [< / > / =]\n");
            }
		}
		else if(tipo == DATE)
		{
			if(strcmp(operador,"=")==0)
			{
				while(linea_auxiliar != NULL)
				{
					linea_anterior = linea_auxiliar->ant; 
					linea_siguiente = linea_auxiliar->sig;
					
					if((FechatoNumero(linea_auxiliar->valores[pos]) == FechatoNumero(valor))) 
					{
						if(linea_auxiliar==Big_Row && linea_auxiliar->sig == NULL) // solo si hay 1 fila
						{
							Big_Row = NULL; 
						}
						else if(linea_auxiliar == Big_Row && linea_auxiliar->ant == NULL) // si quiero borrar la primera fila
						{
							Big_Row = Big_Row->sig; 
							linea_siguiente->ant = NULL;
							linea_auxiliar->sig = NULL; // para comprobar
						}
						else if (linea_auxiliar->sig ==NULL) // si quiero borrar la ultima fila
						{
							linea_anterior->sig = NULL; 
							linea_auxiliar->ant = NULL;
						}
						else // si quiero borrar una fila del medio
						{
							linea_anterior->sig = linea_auxiliar->sig; 
							linea_siguiente->ant = linea_auxiliar->ant; 
						}
						
						for(i=0; i<numCampos; i++)
						{
							free(linea_auxiliar->valores[i]);
						}
						
						free(linea_auxiliar->valores);
						free(linea_auxiliar);
					}
					
					linea_auxiliar = linea_siguiente; 
					
				}
				
				return Big_Row; 
			}
			if(strcmp(operador,"<")==0)
			{
				while(linea_auxiliar != NULL)
				{
					linea_anterior = linea_auxiliar->ant; 
					linea_siguiente = linea_auxiliar->sig;
					
					if((FechatoNumero(linea_auxiliar->valores[pos]) < FechatoNumero(valor))) 
					{
						if(linea_auxiliar==Big_Row && linea_auxiliar->sig == NULL) // solo si hay 1 fila
						{
							Big_Row = NULL; 
						}
						else if(linea_auxiliar == Big_Row && linea_auxiliar->ant == NULL) // si quiero borrar la primera fila
						{
							Big_Row = Big_Row->sig; 
							linea_siguiente->ant = NULL;
							linea_auxiliar->sig = NULL; // para comprobar
						}
						else if (linea_auxiliar->sig ==NULL) // si quiero borrar la ultima fila
						{
							linea_anterior->sig = NULL; 
							linea_auxiliar->ant = NULL;
						}
						else // si quiero borrar una fila del medio
						{
							linea_anterior->sig = linea_auxiliar->sig; 
							linea_siguiente->ant = linea_auxiliar->ant; 
						}
						
						for(i=0; i<numCampos; i++)
						{
							free(linea_auxiliar->valores[i]);
						}
						
						free(linea_auxiliar->valores);
						free(linea_auxiliar);
					}
					
					linea_auxiliar = linea_siguiente; 
					
				}
				
				return Big_Row; 
			}
			if(strcmp(operador,">")==0)
			{
				while(linea_auxiliar != NULL)
				{
					linea_anterior = linea_auxiliar->ant; 
					linea_siguiente = linea_auxiliar->sig;
					
					if((FechatoNumero(linea_auxiliar->valores[pos]) > FechatoNumero(valor))) 
					{
						if(linea_auxiliar==Big_Row && linea_auxiliar->sig == NULL) // solo si hay 1 fila
						{
							Big_Row = NULL; 
						}
						else if(linea_auxiliar == Big_Row && linea_auxiliar->ant == NULL) // si quiero borrar la primera fila
						{
							Big_Row = Big_Row->sig; 
							linea_siguiente->ant = NULL;
							linea_auxiliar->sig = NULL; // para comprobar
						}
						else if (linea_auxiliar->sig ==NULL) // si quiero borrar la ultima fila
						{
							linea_anterior->sig = NULL; 
							linea_auxiliar->ant = NULL;
						}
						else // si quiero borrar una fila del medio
						{
							linea_anterior->sig = linea_auxiliar->sig; 
							linea_siguiente->ant = linea_auxiliar->ant; 
						}
						
						for(i=0; i<numCampos; i++)
						{
							free(linea_auxiliar->valores[i]);
						}
						
						free(linea_auxiliar->valores);
						free(linea_auxiliar);
					}
					
					linea_auxiliar = linea_siguiente; 
					
				}
				
				return Big_Row; 
			}
			else
			{
                printf("ERROR: El tercer parametro ha de ser un relacionador operacional [< / > / =]\n");
            }
		}
		else if(tipo == EMAIL)
		{
			if(strcmp(operador,"=")==0)
			{
				while(linea_auxiliar != NULL)
				{
					linea_anterior = linea_auxiliar->ant; 
					linea_siguiente = linea_auxiliar->sig;
					
					if(strcmp(linea_auxiliar->valores[pos], valor) == 0)  
					{
						if(linea_auxiliar==Big_Row && linea_auxiliar->sig == NULL) // solo si hay 1 fila
						{
							Big_Row = NULL; 
						}
						else if(linea_auxiliar == Big_Row && linea_auxiliar->ant == NULL) // si quiero borrar la primera fila
						{
							Big_Row = Big_Row->sig; 
							linea_siguiente->ant = NULL;
							linea_auxiliar->sig = NULL; // para comprobar
						}
						else if (linea_auxiliar->sig ==NULL) // si quiero borrar la ultima fila
						{
							linea_anterior->sig = NULL; 
							linea_auxiliar->ant = NULL;
						}
						else // si quiero borrar una fila del medio
						{
							linea_anterior->sig = linea_auxiliar->sig; 
							linea_siguiente->ant = linea_auxiliar->ant; 
						}
						
						for(i=0; i<numCampos; i++)
						{
							free(linea_auxiliar->valores[i]);
						}
						
						free(linea_auxiliar->valores);
						free(linea_auxiliar);
					}
					
					linea_auxiliar = linea_siguiente; 
					
				}
				
				return Big_Row; 
			}
			else if(strcmp(operador,"<")==0)
			{
				while(linea_auxiliar != NULL)
				{
					linea_anterior = linea_auxiliar->ant; 
					linea_siguiente = linea_auxiliar->sig;
					
					if(strcmp(linea_auxiliar->valores[pos], valor) < 0)
					{
						if(linea_auxiliar==Big_Row && linea_auxiliar->sig == NULL) // solo si hay 1 fila
						{
							Big_Row = NULL; 
						}
						else if(linea_auxiliar == Big_Row && linea_auxiliar->ant == NULL) // si quiero borrar la primera fila
						{
							Big_Row = Big_Row->sig; 
							linea_siguiente->ant = NULL;
							linea_auxiliar->sig = NULL; // para comprobar
						}
						else if (linea_auxiliar->sig ==NULL) // si quiero borrar la ultima fila
						{
							linea_anterior->sig = NULL; 
							linea_auxiliar->ant = NULL;
						}
						else // si quiero borrar una fila del medio
						{
							linea_anterior->sig = linea_auxiliar->sig; 
							linea_siguiente->ant = linea_auxiliar->ant; 
						}
						
						for(i=0; i<numCampos; i++)
						{
							free(linea_auxiliar->valores[i]);
						}
						
						free(linea_auxiliar->valores);
						free(linea_auxiliar);
					}
					
					linea_auxiliar = linea_siguiente; 
					
				}
				
				return Big_Row; 
			}
			else if(strcmp(operador,">")==0)
			{
				while(linea_auxiliar != NULL)
				{
					linea_anterior = linea_auxiliar->ant; 
					linea_siguiente = linea_auxiliar->sig;
					
					if(strcmp(linea_auxiliar->valores[pos], valor) > 0)  
					{
						if(linea_auxiliar==Big_Row && linea_auxiliar->sig == NULL) // solo si hay 1 fila
						{
							Big_Row = NULL; 
						}
						else if(linea_auxiliar == Big_Row && linea_auxiliar->ant == NULL) // si quiero borrar la primera fila
						{
							Big_Row = Big_Row->sig; 
							linea_siguiente->ant = NULL;
							linea_auxiliar->sig = NULL; // para comprobar
						}
						else if (linea_auxiliar->sig ==NULL) // si quiero borrar la ultima fila
						{
							linea_anterior->sig = NULL; 
							linea_auxiliar->ant = NULL;
						}
						else // si quiero borrar una fila del medio
						{
							linea_anterior->sig = linea_auxiliar->sig; 
							linea_siguiente->ant = linea_auxiliar->ant; 
						}
						
						for(i=0; i<numCampos; i++)
						{
							free(linea_auxiliar->valores[i]);
						}
						
						free(linea_auxiliar->valores);
						free(linea_auxiliar);
					}
					
					linea_auxiliar = linea_siguiente; 
					
				}
				
				return Big_Row; 
			}
			else
			{
                printf("ERROR: El tercer parametro ha de ser un relacionador operacional [< / > / =]\n");
            }
		}
		
		else if(tipo == TEXT)
		{
			if(strcmp(operador,"=")==0)
			{
				while(linea_auxiliar != NULL)
				{
					linea_anterior = linea_auxiliar->ant; 
					linea_siguiente = linea_auxiliar->sig;
					
					if(strcmp(linea_auxiliar->valores[pos], valor) == 0)  
					{
						if(linea_auxiliar==Big_Row && linea_auxiliar->sig == NULL) // solo si hay 1 fila
						{
							Big_Row = NULL; 
						}
						else if(linea_auxiliar == Big_Row && linea_auxiliar->ant == NULL) // si quiero borrar la primera fila
						{
							Big_Row = Big_Row->sig; 
							linea_siguiente->ant = NULL;
							linea_auxiliar->sig = NULL; // para comprobar
						}
						else if (linea_auxiliar->sig ==NULL) // si quiero borrar la ultima fila
						{
							linea_anterior->sig = NULL; 
							linea_auxiliar->ant = NULL;
						}
						else // si quiero borrar una fila del medio
						{
							linea_anterior->sig = linea_auxiliar->sig; 
							linea_siguiente->ant = linea_auxiliar->ant; 
						}
						
						for(i=0; i<numCampos; i++)
						{
							free(linea_auxiliar->valores[i]);
						}
						
						free(linea_auxiliar->valores);
						free(linea_auxiliar);
					}
					
					linea_auxiliar = linea_siguiente; 
					
				}
				
				return Big_Row; 
			}
			else if(strcmp(operador,"<")==0)
			{
				while(linea_auxiliar != NULL)
				{
					linea_anterior = linea_auxiliar->ant; 
					linea_siguiente = linea_auxiliar->sig;
					
					if(strcmp(linea_auxiliar->valores[pos], valor) < 0)  
					{
						if(linea_auxiliar==Big_Row && linea_auxiliar->sig == NULL) // solo si hay 1 fila
						{
							Big_Row = NULL; 
						}
						else if(linea_auxiliar == Big_Row && linea_auxiliar->ant == NULL) // si quiero borrar la primera fila
						{
							Big_Row = Big_Row->sig; 
							linea_siguiente->ant = NULL;
							linea_auxiliar->sig = NULL; // para comprobar
						}
						else if (linea_auxiliar->sig ==NULL) // si quiero borrar la ultima fila
						{
							linea_anterior->sig = NULL; 
							linea_auxiliar->ant = NULL;
						}
						else // si quiero borrar una fila del medio
						{
							linea_anterior->sig = linea_auxiliar->sig; 
							linea_siguiente->ant = linea_auxiliar->ant; 
						}
						
						for(i=0; i<numCampos; i++)
						{
							free(linea_auxiliar->valores[i]);
						}
						
						free(linea_auxiliar->valores);
						free(linea_auxiliar);
					}
					
					linea_auxiliar = linea_siguiente; 
					
				}
				
				return Big_Row; 
			}
			else if(strcmp(operador,">")==0)
			{
				while(linea_auxiliar != NULL)
				{
					linea_anterior = linea_auxiliar->ant; 
					linea_siguiente = linea_auxiliar->sig;
					
					if(strcmp(linea_auxiliar->valores[pos], valor) > 0)   
					{
						if(linea_auxiliar==Big_Row && linea_auxiliar->sig == NULL) // solo si hay 1 fila
						{
							Big_Row = NULL; 
						}
						else if(linea_auxiliar == Big_Row && linea_auxiliar->ant == NULL) // si quiero borrar la primera fila
						{
							Big_Row = Big_Row->sig; 
							linea_siguiente->ant = NULL;
							linea_auxiliar->sig = NULL; // para comprobar
						}
						else if (linea_auxiliar->sig ==NULL) // si quiero borrar la ultima fila
						{
							linea_anterior->sig = NULL; 
							linea_auxiliar->ant = NULL;
						}
						else // si quiero borrar una fila del medio
						{
							linea_anterior->sig = linea_auxiliar->sig; 
							linea_siguiente->ant = linea_auxiliar->ant; 
						}
						
						for(i=0; i<numCampos; i++)
						{
							free(linea_auxiliar->valores[i]);
						}
						
						free(linea_auxiliar->valores);
						free(linea_auxiliar);
					}
					
					linea_auxiliar = linea_siguiente; 
					
				}
				
				return Big_Row; 
			}
			else
			{
                printf("ERROR: El tercer parametro ha de ser un relacionador operacional [< / > / =]\n");
            }
		}
		else
		{
			printf("ERROR de tipos.\n");
			return NULL;
		}

	}
	
}




/** Funciones de impresión **/

void ImprimirEncabezado(TABLE *tabla_actual) // Imprime la cabecera de una tabla // 
{
    int i;
	
	for(i=0; i<tabla_actual->numCampos; i++)
	{
		printf("%s   ", tabla_actual-> campos[i]);
	}
	
	printf("\n");
    

}


void ImprimirTablaEntera (int numCampos, ROW *Big_Row, TABLE *t) // Imprime toda la tabla //
{

    int i;
	int j;
	
	j=1;
	
    ROW *linea_auxiliar = Big_Row;

    if(Big_Row == NULL)
	{
        printf("No se han encontrado datos\n");
    }
    else
	{
        while(linea_auxiliar != NULL)
		{
            printf("%d- ", j);
			
			for(i=0; i< numCampos-1; i++)
			{
				printf("%s, ",linea_auxiliar->valores[i]);
				
			}
			
			
			printf("%s\n", linea_auxiliar->valores[numCampos-1]);
			
            j++;

            linea_auxiliar = linea_auxiliar->sig;
        }
    }
}


void ImprimirTablaAscendentemente(int numCampos, ROW *Big_Row, TABLE *tabla_actual) // Imprime por pantalla una tabla ascendentemente // 
{
    int i, j=1;
    ROW *linea_auxiliar = Big_Row;

    if(Big_Row == NULL)
	{
        printf("No se han encontrado datos\n");
    }
    else
	{
        OrdenarAscendentemente(linea_auxiliar, tabla_actual, 0); // En el caso de que en el examen me pida ordenar una columna concreta simplemnte cambiar el 0 a la columna deseada

        while(linea_auxiliar != NULL)
		{
            printf("%d- ",j);

            for(i=0; i<numCampos-1; i++)
			{
                printf("%s, ", linea_auxiliar->valores[i]);
            }

            printf("%s\n", linea_auxiliar->valores[numCampos-1]);
			
            j++;
			
            linea_auxiliar = linea_auxiliar->sig;
        }
    }
}


void ImprimirTablaDescendentemente (int numCampos, ROW *Big_Row, TABLE *tabla_actual)  // Imprime (por pantalla) descendentemente una tabla //
{
    int i, j=1;
	
    ROW *linea_auxiliar = Big_Row;

    if(Big_Row == NULL)
	{
        printf("No hay datos disponibles\n");
    }
    else
	{
        OrdenarDescendentemente(linea_auxiliar, tabla_actual, 0);

        while(linea_auxiliar != NULL)
		{
			printf("%d- ", j);

            for(i=0; i<numCampos-1; i++){

               printf("%s, ", linea_auxiliar->valores[i]);
            } 

            printf("%s\n", linea_auxiliar->valores[numCampos-1]);

            j++;
			
            linea_auxiliar = linea_auxiliar->sig;
        }

    }  
}


void ImprimirCampos(int numCampos, ROW *Big_Row, TYPE tipo, char *valor, char *operador, int pos) // Imprime por pantalla los valores de un campo que cumpla con una cierta condición 
{
	
    int i, j = 1;
    ROW *linea_auxiliar = Big_Row;

    if(Big_Row == NULL)
	{
        printf("No se han encontrado datos\n");
    }
    else
	{
        if(tipo == NUM)
		{
            if(strcmp("=", operador) == 0)
			{
                while(linea_auxiliar != NULL)
				{
                    if(atof(linea_auxiliar->valores[pos]) == atof(valor))
					{
                        printf("%d- ", j);

                        for(i=0; i< numCampos-1; i++)
						{
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numCampos-1]);
						
                        j++;
                    }

                    linea_auxiliar = linea_auxiliar->sig;
                }
            }
            else if(strcmp(">", operador) == 0)
			{
                while(linea_auxiliar != NULL)
				{
                    if(atof(linea_auxiliar->valores[pos]) > atof(valor))
					{
                        printf("%d- ", j);

                        for(i=0; i<numCampos-1; i++)
						{
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numCampos-1]);
						
                        j++;
                    }

                    linea_auxiliar = linea_auxiliar->sig;
                }
            }
            else if(strcmp("<", operador) == 0)
			{
                while(linea_auxiliar != NULL)
				{
                    if(atof(linea_auxiliar->valores[pos]) < atof(valor))
					{
                        printf("%d- ", j);

                        for(i=0; i<numCampos-1; i++)
						{
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numCampos-1]);
                        j++;
                    }

                    linea_auxiliar = linea_auxiliar->sig;
                }
            }
            else
			{
                printf("ERROR: El tercer parametro ha de ser un relacionador operacional [< / > / =]\n");
            }
        }
        else if(tipo == DATE)
		{
            if(strcmp("=", operador) == 0)
			{
                while(linea_auxiliar != NULL)
				{
                    if((FechatoNumero(linea_auxiliar->valores[pos])) == (FechatoNumero(valor)))
					{
                        printf("%d- ", j);

                        for(i=0; i<numCampos-1; i++)
						{
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numCampos-1]);
						
                        j++;
                    }

                    linea_auxiliar = linea_auxiliar->sig;
                }
            }
            else if(strcmp(">", operador) == 0)
			{
                while(linea_auxiliar != NULL)
				{
                    if((FechatoNumero(linea_auxiliar->valores[pos])) > (FechatoNumero(valor)))
					{
                        printf("%d- ", j);

                        for(i=0; i<numCampos-1; i++)
						{
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numCampos-1]);
						
                        j++;
                    }

                    linea_auxiliar = linea_auxiliar->sig;
                }
            }
            else if(strcmp("<", operador) == 0)
			{
                while(linea_auxiliar != NULL)
				{
                    if((FechatoNumero(linea_auxiliar->valores[pos])) < (FechatoNumero(valor)))
					{
                        printf("%d- ", j);

                        for(i=0; i<numCampos-1; i++)
						{
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numCampos-1]);
						
                        j++;

                    }

                    linea_auxiliar = linea_auxiliar->sig;
                }
            }
            else
			{
                printf("ERROR: El tercer parametro ha de ser un relacionador operacional [< / > / =]\n");
            }
        }
		else if(tipo == EMAIL)
		{
            if(strcmp("=", operador) == 0)
			{
                while(linea_auxiliar != NULL)
				{
                    if(strcmp(linea_auxiliar->valores[pos], valor) == 0)
					{
                        printf("%d- ", j);

                        for(i=0; i<numCampos-1; i++)
						{
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numCampos-1]);
						
                        j++;
                    }
                    linea_auxiliar = linea_auxiliar->sig;
                }
            }
            else if(strcmp("<", operador) == 0)
			{
                while(linea_auxiliar != NULL)
				{
                    if(strcmp(linea_auxiliar->valores[pos], valor) < 0)
					{
                        printf("%d- ", j);

                        for(i=0; i<numCampos-1; i++)
						{
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numCampos-1]);
						
                        j++;
                    }

                    linea_auxiliar = linea_auxiliar->sig;
                }
            }
            else if(strcmp(">", operador) == 0)
			{
                while(linea_auxiliar != NULL)
				{
                    if(strcmp(linea_auxiliar->valores[pos], valor) > 0)
					{
                        printf("%d- ", j);

                        for(i=0; i<numCampos-1; i++)
						{
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numCampos-1]);
						
                        j++;
                    }

                    linea_auxiliar = linea_auxiliar->sig;
                }
            }
            else
			{
                printf("ERROR: El tercer parametro ha de ser un relacionador operacional [< / > / =]\n");
            }
        }
        else if(tipo == TEXT)
		{
            if(strcmp("=", operador) == 0)
			{
                while(linea_auxiliar != NULL)
				{
                    if(strcmp(linea_auxiliar->valores[pos], valor) == 0)
					{
                        printf("%d- ", j);

                        for(i=0; i<numCampos-1; i++)
						{
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numCampos-1]);
						
                        j++;
                    }

                    linea_auxiliar = linea_auxiliar->sig;
                }
            }
            else if(strcmp("<", operador) == 0)
			{
                while(linea_auxiliar != NULL)
				{
                    if(strcmp(linea_auxiliar->valores[pos], valor) < 0)
					{
                        printf("%d- ", j);

                        for(i=0; i<numCampos-1; i++)
						{
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numCampos-1]);
						
                        j++;
                    }

                    linea_auxiliar = linea_auxiliar->sig;
                }
            }
            else if(strcmp(">", operador) == 0)
			{
                while(linea_auxiliar != NULL)
				{
                    if(strcmp(linea_auxiliar->valores[pos], valor) > 0)
					{
                        printf("%d- ", j);

                        for(i=0; i<numCampos-1; i++)
						{
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numCampos-1]);
						
                        j++;
                    }

                    linea_auxiliar = linea_auxiliar->sig;
                }
            }
            else
			{
                printf("ERROR: El tercer parametro ha de ser un relacionador operacional [< / > / =]\n");
            }
        }
    }
}




/** Funciones de ordenar **/

void OrdenarAscendentemente (ROW *GranLinea, TABLE *tabla_aux, int orden_campo) // Ordena descendentemente una tabla //
{
	ROW *i, *j;

    if(tabla_aux->tipos[orden_campo] == NUM)
	{
        for(i = GranLinea; i != NULL; i = i->sig)
		{
            for(j = i; j != NULL; j = j->sig)
			{
                if(atof(i->valores[orden_campo]) > atof(j->valores[orden_campo]))
				{
                    //printf("%s > %s \n", i->valores[orden_campo], j->valores[orden_campo]);

                    intercambio_valores(i, j);
                }
            }
        }
    }
    else if(tabla_aux->tipos[orden_campo] == DATE)
	{
        for(i = GranLinea; i != NULL; i = i->sig)
		{
            for(j = i; j != NULL; j = j->sig)
			{
                if(FechatoNumero(i->valores[orden_campo]) > FechatoNumero(j->valores[orden_campo]))
				{
                    //printf("%s > %s\n", i->valores[orden_campo], j->valores[orden_campo]);

                    intercambio_valores(i, j);
                }    
            }
        }
    }
	else if(tabla_aux->tipos[orden_campo] == EMAIL)
	{
        for(i = GranLinea; i != NULL; i = i->sig)
		{
            for(j = i; j != NULL ; j = j->sig)
			{
                if(strcmp(i->valores[orden_campo], j->valores[orden_campo])>0)
				{
                    //printf("%s > %s\n", i->valores[orden_campo], j->valores[orden_campo]);

                    intercambio_valores(i, j);
                }
            }
        }
    }
    else if(tabla_aux->tipos[orden_campo] == TEXT)
	{
        for(i = GranLinea; i != NULL; i = i->sig)
		{
            for(j = i; j != NULL ; j = j->sig)
			{
                if(strcmp(i->valores[orden_campo], j->valores[orden_campo])>0)
				{
                    //printf("%s > %s\n", i->valores[orden_campo], j->valores[orden_campo]);

                    intercambio_valores(i, j);
                }
            }
        }
    }	
}


void OrdenarDescendentemente (ROW *GranLinea, TABLE *tabla_aux, int orden_campo) // ordena descendentemente una tabla.
{
	ROW *i, *j;

    if(tabla_aux->tipos[orden_campo] == NUM)
	{
        for(i=GranLinea; i != NULL; i = i->sig)
		{
            for(j=i; j != NULL; j = j->sig)
			{
                if(atof(i->valores[orden_campo]) < atof(j->valores[orden_campo]))
				{
					//printf("%s < %s \n", i->valores[orden_campo], j->valores[orden_campo]);

                    intercambio_valores(i, j);
                }
            }
        }
    }
    else if(tabla_aux->tipos[orden_campo] == DATE)
	{
        for(i = GranLinea; i != NULL; i = i->sig)
		{
            for(j = i; j != NULL; j = j->sig)
			{
                if(FechatoNumero(i->valores[orden_campo]) < FechatoNumero(j->valores[orden_campo]))
				{
                    //printf("%s < %s\n", i->valores[orden_campo], j->valores[orden_campo]);

                    intercambio_valores(i, j);
                }    
            }
        }
    }
	else if(tabla_aux->tipos[orden_campo] == EMAIL)
	{
        for(i = GranLinea; i != NULL; i = i->sig)
		{
            for(j = i; j != NULL ; j = j->sig)
			{
                if(strcmp(i->valores[orden_campo], j->valores[orden_campo])<0)
				{
                    //printf("%s < %s\n", i->valores[orden_campo], j->valores[orden_campo]);

                    intercambio_valores(i, j);
                }
            }
        }
    }
    else if(tabla_aux->tipos[orden_campo] == TEXT)
	{
        for(i = GranLinea; i != NULL; i = i->sig)
		{
            for(j = i; j != NULL ; j = j->sig)
			{
                if(strcmp(i->valores[orden_campo], j->valores[orden_campo])<0)
				{
                    //printf("%s < %s\n", i->valores[orden_campo], j->valores[orden_campo]);

                    intercambio_valores(i, j);
                }
            }
        }
    }	
}


void intercambio_valores (ROW *linea1, ROW *linea2) // swapping
{
    char **linea_auxiliar;

    linea_auxiliar = linea1->valores;
    linea1->valores = linea2->valores;
    linea2->valores = linea_auxiliar;
}









