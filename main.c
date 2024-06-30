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

//gcc lib.c main.c -o main.exe


int main()
{
    char prompt[100] = "*";
	
    char comando[100];
    char comando_auxiliar[100];
	
    char nombre_bd[100] = "*";
    char nombre_tabla[100] = "*";

    int num_arg, num_arg_aux;
	int i, j;
	int auxiliar, auxiliar_enteros, auxiliar_comprobante;
	
	int numCampos;
	
    char sep[8] = " \n\t\r";

    char *comtok[100];
	
    char *comillas;
	char *comillasaux;
	
    char **nombre;
	char **valores;
	
    TYPE *tipos;
	
    TABLE *GranTabla = NULL;
	TABLE *TablaAuxiliar = NULL;
	TABLE *TablaAuxiliar2 = NULL;
	
    ROW *GranLinea = NULL; 
	ROW *LineaAuxiliar;


    DatosAlumno();


    while(1){

		printf("[%s]:>", prompt);
        gets(comando);
        strcpy(comando_auxiliar, comando);
        num_arg = 0;
        comtok[num_arg] = strtok(comando, sep);

        num_arg++;

        while((comtok[num_arg] = strtok(NULL, sep)) != NULL)
		{
            num_arg++;
        }

        if(comtok[0] == NULL) //si en el prompt no ponen nada vuelve a ejecutarse
		{ 
            continue;
        }
		if(num_arg == 1 && (strcmp(comtok[0], "exit") != 0) && (strcmp(comtok[0], "select") != 0))
		{
            printf("ERROR: Comando erroneo.\n");
			continue;
        }
		else if(strcmp(comtok[0], "exit") == 0) // exit
		{
            if(num_arg != 1)
			{
                printf("ERROR: Numero de parametros incorrecto.\n");
				printf("Posible comando: 'exit'.\n");
                continue;
            }
			
            if(GranTabla != NULL)
			{
                if(GranLinea != NULL) // Aquí liberamos la memoria de las líneas
				{
                    LiberarMemoriaLineas(GranLinea, TablaAuxiliar->numCampos);

                    GranLinea = NULL;
                }

                LiberarMemoriaTablas(GranTabla);

                GranTabla = NULL;
				
                return 0;

            }
            else if(GranTabla == NULL)
			{
				return 0;
            }
            else
			{
				printf("ERROR inesperado.\n");
            }
			
        }
		else if(strcmp(comtok[0], "list") == 0) // list
		{
			if(num_arg != 2)
			{
                printf("ERROR: Numero incorrecto de parametros.\n");
				printf("Posibles comandos: 'list db' y 'list table'.\n");
                continue;
            }
			
			if(strcmp(comtok[1], "db") == 0) // list db
			{
				MostrarBasesDeDatos();
			}
			
			else if(strcmp(comtok[1], "table") == 0) // list table
			{
				if(strcmp(prompt, "*") == 0)
				{
					printf("ERROR: La base de datos ha de estar activa.\n");
					continue;
				}
				
				MostrarTablas(GranTabla, nombre_bd);
			}
			else
			{
				printf("ERROR: El comando list esta mal escrito.\n");
				printf("Posibles comandos: 'list db' y 'list table'.\n");
				continue;
			}
			
		}
        else if(strcmp(comtok[0], "new") == 0 && strcmp(comtok[1], "db") == 0) // new db
		{
            if(num_arg != 3)
			{
                printf("ERROR: Numero de parametros incorrecto.\n");
				printf("Posible comando: 'new db <nombre_bd>'.\n");
                continue;
            }

            auxiliar_enteros = ValidarCaracteres(comtok[2]);

            if(auxiliar_enteros == 0)
			{
                CrearBD(comtok[2]);
            }
            else
			{
                printf("ERROR: El nombre de la carpeta esta mal escrito.\n");
				continue; 
            }

        }
        else if(strcmp(comtok[0], "use") == 0 && strcmp(comtok[1], "db") == 0) // use db
		{
            if(num_arg != 3)
			{
                printf("ERROR: Numero de parametros incorrecto.\n");
				printf("Posible comando: 'use db <nombre_bd>'.\n");
                continue;
            }

            if(strcmp(comtok[2], ".") == 0 || strcmp(comtok[2], "..") == 0)
			{
                printf("ERROR: Estas carpetas ocultas no se tocan.\n");
                continue;
            }

            auxiliar_enteros = EntrarDirectorio(comtok[2]); // Comprobamos si la ruta existe, sí = 0 y no = 1

            if(auxiliar_enteros == 0)
			{
                strcpy(prompt, comtok[2]);
                strcpy(nombre_bd, comtok[2]);
                strcpy(nombre_tabla, "*"); // Por si es la segunda vez que la llamamos y ya tiene una lista activa

            }
            else
			{
                continue;
            }


            if(GranTabla != NULL) // Aquí liberamos memoria
			{
                if(GranLinea != NULL)
				{
                    LiberarMemoriaLineas(GranLinea, TablaAuxiliar->numCampos);
                    GranLinea = NULL;
                }

                LiberarMemoriaTablas(GranTabla);
                GranTabla = NULL;

            }


            GranTabla = CargarTablas(nombre_bd);

            MostrarTablas(GranTabla, nombre_bd);

        }
        else if(strcmp(comtok[0], "new") == 0 && strcmp(comtok[1], "table") == 0) // new table
		{
            if(strcmp(prompt, "*") == 0)
			{
                printf("ERROR: La base de datos ha de estar activa.\n");
                continue;
            }
			else if(num_arg < 5)
			{
                printf("ERROR: Numero de parametros incorrecto.\n");
				printf("Posible comando: 'new table <nombre_tabla> <campo> <tipo> [<campo> <tipo> ...]'.\n");
                continue;
            }
            else if(num_arg % 2 == 0)
			{
                printf("ERROR: Numero de parametros incorrecto --> ha de ser numero impar.\n");
                continue;
            }
            

            for(i=2; i<num_arg; i++)
			{
                auxiliar_enteros = ValidarCaracteres(comtok[i]); // Comprueba que toda la cadena está bien escrita.
				
                if(auxiliar_enteros>0)
				{
                    printf("ERROR: Hay algo mal escrito.\n");
                    break;
                }
            }

            if(auxiliar_enteros>0)
			{
                continue; // si algo salió mal nos salimos del comando
            }

            numCampos = (num_arg-3)/2; // número de campos que posee la tabla

            nombre = malloc(sizeof(char*)*(numCampos)); // Reservamos memoria para el array de columnas de la tabla
            tipos = malloc(sizeof(TYPE*)*(numCampos)); // Reservamos memoria para los tipos de la tabla
            
			auxiliar = 0;
            auxiliar_comprobante = 0;

            for(i=3; i<num_arg; i++)
			{
                if(i % 2 == 1)
				{
                    nombre[auxiliar] = strdup(comtok[i]);
                }
                else
				{
                    auxiliar_enteros = ValidarTipo(comtok[i]);

                    if(auxiliar_enteros == 4)
					{
                        printf("ERROR DE TIPOS\n");
                        auxiliar_comprobante = 1;
                        break;
                    }

                    tipos[auxiliar] = ValidarTipo(comtok[i]);
                    auxiliar++;
                }
            }

            i = VerDuplicados(nombre, auxiliar);

            j = EntrarTabla(nombre_bd, comtok[2]);

            if(j != 0)
			{
                printf("ERROR\n");
                continue;
            }

            if(auxiliar_comprobante == 0 && i == 0 && j == 0)
			{
                TablaAuxiliar2 = InicializarTablas(numCampos, comtok[2], nombre, tipos);
            
                if(TablaAuxiliar2 != NULL)
				{
                    if(GranTabla == NULL)
					{
                        GranTabla = TablaAuxiliar2;

                        //Crear la tabla en .tab
                        CrearTabla(GranTabla, nombre_bd, comtok[2]);

                    }
                    else
					{
                        //Recorrer LaGranTabla hasta llegar a la última e insertarla ahí
                        InsertarUltimaTabla(GranTabla, TablaAuxiliar2);

                        //Crear la tabla en .tab
                        CrearTabla(GranTabla, nombre_bd, comtok[2]);
                    }
                }
            }
        }
        else if(strcmp(comtok[0], "use") == 0 && strcmp(comtok[1], "table") == 0) // use table
		{
            if(num_arg != 3)
			{
                printf("ERROR: Numero de parametros incorrecto.\n");
				printf("Posible comando: 'use table <nombre_tabla>'.\n");
                continue;
            }
            if(strcmp(prompt, "*") == 0)
			{
                printf("ERROR: La base de datos ha de estar activa.\n");
                continue;
            }

            auxiliar_comprobante = EntrarTabla(nombre_bd, comtok[2]);

            if(auxiliar_comprobante == 1)
			{
                strcpy(prompt, nombre_bd);
                strcpy(nombre_tabla, comtok[2]);
                strcat(prompt, "/");
                strcat(prompt, nombre_tabla);

                if(GranLinea != NULL)
				{
                    LiberarMemoriaLineas(GranLinea, TablaAuxiliar->numCampos);
                }

                GranLinea = NULL;

            }
            else
			{
                printf("ERROR: La tabla '%s' no existe.\n", comtok[2]);
				continue;
            }

            TablaAuxiliar = BuscarTabla(nombre_tabla, GranTabla);

            GranLinea = CargarLineas(nombre_bd, nombre_tabla, TablaAuxiliar->numCampos, TablaAuxiliar->tipos);

        }
        else if(strcmp(comtok[0], "insert") == 0) // insert
		{
			if(strcmp(prompt, "*") == 0)
			{
				printf("ERROR: La base de datos ha de estar activa.\n");
				continue; 
			}
			
			if(strcmp(prompt, nombre_bd) == 0)
			{
				printf("ERROR: La tabla ha de estar activa.\n");
				continue; 
			}
			
            auxiliar_comprobante = 0;
            comillas = strchr(comando_auxiliar, '"');

            if(comillas != NULL)
			{
                comillasaux = strchr(comillas+1, '"');

                if(comillasaux == NULL)
				{
                    //ERROR
                    printf("ERROR DE COMILLAS\n");
                    auxiliar_comprobante++;
                    continue;
                }
                else
				{
                    //Llamamos a la funcion strtok2 con las comillas activadas
                    num_arg_aux = 0;
                    comtok[num_arg_aux] = strtok2(comando_auxiliar, sep, 0);
                    num_arg_aux++;
                    
                    while((comtok[num_arg_aux] = strtok2(NULL, sep, 1)) != NULL) // strtok2 con comillas activas
					{ 
                        num_arg_aux++;
                    }
                }
            }
            else
			{
                //Llamamos a la funcion strtok2 con las comillas desactivadas

                num_arg_aux = 0;
                comtok[num_arg_aux] = strtok2(comando_auxiliar, sep, 0);
                num_arg_aux++;

                while((comtok[num_arg_aux] = strtok2(NULL, sep, 0)) != NULL) // strtok2 con las comillas desactivadas
				{ 
                    num_arg_aux++;
                }
            }

            if(num_arg_aux != (TablaAuxiliar->numCampos+1))
			{
                printf("ERROR: Numero de campos incorrecto.\n");
                continue;
            }

            for(i=0; i<TablaAuxiliar->numCampos; i++)
			{
                if(TablaAuxiliar->tipos[i] == NUM)
				{
                    if(EsDecimalValido(comtok[i+1]) == 0)
					{
                        printf("ERROR al insertar un numero\n");
                        auxiliar_comprobante++;
                        break;
                    }
                }
				else if(TablaAuxiliar->tipos[i] == DATE)
				{
					if(EsFechaValidaDMA(comtok[i+1]) == 0)
					{
						printf("ERROR al insertar una fecha\n");
						auxiliar_comprobante++;
						break;
					}
				}
                else if(TablaAuxiliar->tipos[i] == EMAIL)
				{
					if(EsCorreoValido(comtok[i+1]) == false)
					{
						printf("ERROR al insertar un correo\n");
						auxiliar_comprobante++;
						break;
					}
				}
				
                else if(TablaAuxiliar->tipos[i] == TEXT)
				{
                    if(EsDecimalValido(comtok[i+1]) == 1)
					{
                        printf("ERROR\n");
                        auxiliar_comprobante++;
                        break;
                    }
					if(EsFechaValidaDMA(comtok[i+1]) == 1)
					{
						printf("ERROR\n");
						auxiliar_comprobante++;
						break;
					}
					if(EsCorreoValido(comtok[i+1]) == true)
					{
						printf("ERROR\n");
						auxiliar_comprobante++;
						break;
					}
                }
            }

            if(auxiliar_comprobante != 0)
			{
                continue;
            }

            valores = malloc(sizeof(char*)*TablaAuxiliar->numCampos); // Reservamos memoria para las líneas

            for(i = 0; i < TablaAuxiliar->numCampos; i++)
			{
                valores[i] = strdup(comtok[i+1]);
            }

            //inicializamos la línea
            LineaAuxiliar = InicializarLinea(valores);

            //vemos si hay mas líneas
            if(LineaAuxiliar != NULL)
			{
                //no, pues la ponemos la primera
                if(GranLinea == NULL)
				{
                    GranLinea = LineaAuxiliar;
                    ActualizarLineas(nombre_bd, GranLinea, TablaAuxiliar);
                }
                else
				{
                    //si, pues recorremos la gran linea y la insertamos la ultima
                    InsertarUltimaLinea(GranLinea, LineaAuxiliar);

                    //Actualizar el .tab
                    ActualizarLineas(nombre_bd, GranLinea, TablaAuxiliar);
					
                }
            }
        }
        else if(strcmp(comtok[0], "select") == 0) // select
		{
			if(strcmp(prompt, "*") == 0)
			{
				printf("ERROR: No hay una base de datos activa.\n");
				continue; 
			}
			if(strcmp(prompt, nombre_bd) == 0)
			{
				printf("ERROR: Todavia no hay una tabla activa.\n");
				continue;
			}
			
			comillas = strchr(comando_auxiliar, '"');

            if(comillas != NULL)
			{
                comillasaux = strchr(comillas+1, '"');

                if(comillasaux == NULL)
				{
                    //ERROR
                    printf("ERROR DE COMILLAS\n");
                    continue;
                }
                else
				{
                    //Llamamos a la funcion strtok2 con las comillas activadas
                    num_arg_aux = 0;
                    comtok[num_arg_aux] = strtok2(comando_auxiliar, sep, 0);
                    num_arg_aux++;
                    
                    while((comtok[num_arg_aux] = strtok2(NULL, sep, 1)) != NULL) // strtok2 con comillas activas
					{ 
                        num_arg_aux++;
                    }
                }
            }
            else
			{
                //Llamamos a la funcion strtok2 con las comillas desactivadas

                num_arg_aux = 0;
                comtok[num_arg_aux] = strtok2(comando_auxiliar, sep, 0);
                num_arg_aux++;

                while((comtok[num_arg_aux] = strtok2(NULL, sep, 0)) != NULL) // strtok2 con las comillas desactivadas
				{
                    num_arg_aux++;
                }
            }
			
			
			if(num_arg_aux == 1) // select
			{
				ImprimirEncabezado(TablaAuxiliar);
				ImprimirTablaEntera(TablaAuxiliar->numCampos, GranLinea, TablaAuxiliar);
			}
			else if(num_arg_aux == 2) 
			{	
				if(strcmp(comtok[1], "ASC") == 0) // select ASC 
				{
					ImprimirEncabezado(TablaAuxiliar); 
					ImprimirTablaAscendentemente(TablaAuxiliar->numCampos, GranLinea, TablaAuxiliar);
				}
				else if(strcmp(comtok[1], "DES") == 0)  // select DES 
				{
					ImprimirEncabezado(TablaAuxiliar); 
					ImprimirTablaDescendentemente(TablaAuxiliar->numCampos, GranLinea, TablaAuxiliar);
				}
				else
				{
					printf("ERROR: El segundo par%cmetro ha de ser 'ASC' o 'DES'.\n", 160); 
					continue; 
				}
			
			}
			else if(num_arg_aux == 4) // select campo x valor
			{
				for(int i=0; i<TablaAuxiliar->numCampos; i++)
				{
					if(strcmp(TablaAuxiliar->campos[i], comtok[1]) == 0)
					{
						if(TablaAuxiliar->tipos[i] == NUM && EsDecimalValido(comtok[3]) == 1)
						{
							ImprimirEncabezado(TablaAuxiliar);
							ImprimirCampos(TablaAuxiliar->numCampos, GranLinea, TablaAuxiliar->tipos[i], comtok[3], comtok[2], i);
							
							break;
						}
						else if(TablaAuxiliar->tipos[i] == DATE && (EsFechaValidaDMA(comtok[3]) == 1))
						{
							ImprimirEncabezado(TablaAuxiliar);
							ImprimirCampos(TablaAuxiliar->numCampos, GranLinea, TablaAuxiliar->tipos[i], comtok[3], comtok[2], i);
							
							break;
						}
						else if(TablaAuxiliar->tipos[i] == EMAIL && EsCorreoValido(comtok[3]) == true)
						{
							ImprimirEncabezado(TablaAuxiliar);
							ImprimirCampos(TablaAuxiliar->numCampos, GranLinea, TablaAuxiliar->tipos[i], comtok[3], comtok[2], i);
							
							break;
						}
						else if(TablaAuxiliar->tipos[i] == TEXT)
						{
							ImprimirEncabezado(TablaAuxiliar);
							ImprimirCampos(TablaAuxiliar->numCampos, GranLinea, TablaAuxiliar->tipos[i], comtok[3], comtok[2], i);
							
							break;
						}
						else
						{
							printf("ERROR de tipos.\n");
							i++;
							continue;
						}
					}
					
					if(i+1 == TablaAuxiliar->numCampos)
					{
						printf("ERROR: Campo no encontrado.\n");
						continue; 
					}
				}
			}
			else if(num_arg_aux == 5) 
			{
				if(strcmp(comtok[4], "ASC") == 0) // select campo x valor ASC 
				{
					for(i=0; i<TablaAuxiliar->numCampos; i++)
					{
						if(strcmp(TablaAuxiliar->campos[i], comtok[1]) == 0)
						{
							if(TablaAuxiliar->tipos[i] == NUM && EsDecimalValido(comtok[3]) == 1)
							{
								ImprimirEncabezado(TablaAuxiliar);
								OrdenarAscendentemente(GranLinea, TablaAuxiliar, 0);
								ImprimirCampos(TablaAuxiliar->numCampos, GranLinea, TablaAuxiliar->tipos[i], comtok[3], comtok[2], i);
								
								break;
							}
							else if(TablaAuxiliar->tipos[i] == DATE && (EsFechaValidaDMA(comtok[3]) == 1))
							{
								ImprimirEncabezado(TablaAuxiliar);
								OrdenarAscendentemente(GranLinea, TablaAuxiliar, 0);
								ImprimirCampos(TablaAuxiliar->numCampos, GranLinea, TablaAuxiliar->tipos[i], comtok[3], comtok[2], i);
								
								break;
							}
							else if(TablaAuxiliar->tipos[i] == EMAIL && EsCorreoValido(comtok[3]) == true)
							{
								ImprimirEncabezado(TablaAuxiliar);
								OrdenarAscendentemente(GranLinea, TablaAuxiliar, 0);
								ImprimirCampos(TablaAuxiliar->numCampos, GranLinea, TablaAuxiliar->tipos[i], comtok[3], comtok[2], i);
								
								break;
							}
							else if(TablaAuxiliar->tipos[i] == TEXT)
							{
								ImprimirEncabezado(TablaAuxiliar);
								OrdenarAscendentemente(GranLinea, TablaAuxiliar, 0);
								ImprimirCampos(TablaAuxiliar->numCampos, GranLinea, TablaAuxiliar->tipos[i], comtok[3], comtok[2], i);
								
								break;
							}
							else
							{
								printf("ERROR de tipos.\n");
								i++;
								continue;
							}
						}
						
						if(i+1 == TablaAuxiliar->numCampos)
						{
							printf("ERROR: Campo no encontrado.\n");
							continue;
						}
					}
				}
				else if(strcmp(comtok[4], "DES") == 0) // select campo x valor DES 
				{
					for(i=0; i<TablaAuxiliar->numCampos; i++)
					{
						if(strcmp(TablaAuxiliar->campos[i], comtok[1]) == 0)
						{
							if(TablaAuxiliar->tipos[i] == NUM && EsDecimalValido(comtok[3]) == 1)
							{
								ImprimirEncabezado(TablaAuxiliar);
								OrdenarDescendentemente(GranLinea, TablaAuxiliar, 0);
								ImprimirCampos(TablaAuxiliar->numCampos, GranLinea, TablaAuxiliar->tipos[i], comtok[3], comtok[2], i);
								
								break;
							}
							else if(TablaAuxiliar->tipos[i] == DATE && (EsFechaValidaDMA(comtok[3]) == 1))
							{
								ImprimirEncabezado(TablaAuxiliar);
								OrdenarDescendentemente(GranLinea, TablaAuxiliar, 0);
								ImprimirCampos(TablaAuxiliar->numCampos, GranLinea, TablaAuxiliar->tipos[i], comtok[3], comtok[2], i);
								
								break;
							}
							else if(TablaAuxiliar->tipos[i] == EMAIL && EsCorreoValido(comtok[3]) == true)
							{
								ImprimirEncabezado(TablaAuxiliar);
								OrdenarDescendentemente(GranLinea, TablaAuxiliar, 0);
								ImprimirCampos(TablaAuxiliar->numCampos, GranLinea, TablaAuxiliar->tipos[i], comtok[3], comtok[2], i);
								
								break;
							}
							else if(TablaAuxiliar->tipos[i] == TEXT)
							{
								ImprimirEncabezado(TablaAuxiliar);
								OrdenarDescendentemente(GranLinea, TablaAuxiliar, 0);
								ImprimirCampos(TablaAuxiliar->numCampos, GranLinea, TablaAuxiliar->tipos[i], comtok[3], comtok[2], i);
								
								break;
							}
							else
							{
								printf("ERROR de tipos.\n");
								i++;
								continue;
							}
						}
						
						if(i+1 == TablaAuxiliar->numCampos)
						{
							printf("ERROR: Campo no encontrado\n");
							continue;
						}
					}
				}
			}
			else
			{
				printf("ERROR: Numero de parametros incorrecto.\n");
				printf("Posibles comandos:\n");
				printf("- select\n");
				printf("- select ASC\n");
				printf("- select DES\n");
				printf("- select <campo> = <valor>\n");
				printf("- select <campo> < <valor>\n");
				printf("- select <campo> > <valor>\n");
				printf("- select <campo> = <valor> ASC\n");
				printf("- select <campo> < <valor> ASC\n");
				printf("- select <campo> > <valor> ASC\n");
				printf("- select <campo> = <valor> DES\n");
				printf("- select <campo> < <valor> DES\n");
				printf("- select <campo> > <valor> DES\n");
			}
			
        }
		else if(strcmp(comtok[0], "delete") == 0) // delete
		{
			if(strcmp(prompt, "*") == 0)
			{
				printf("ERROR: No hay una base de datos activa.\n");
				continue; 
			}
			
			if(strcmp(prompt, nombre_bd) == 0)
			{
				printf("ERROR: No hay una tabla activa.\n");
				continue; 
			}
			
			comillas = strchr(comando_auxiliar, '"');

            if(comillas != NULL)
			{
                comillasaux = strchr(comillas+1, '"');

                if(comillasaux == NULL)
				{
                    //ERROR
                    printf("ERROR DE COMILLAS\n");
                    continue;
                }
                else
				{
                    //Llamamos a la funcion strtok2 con las comillas activadas
                    num_arg_aux = 0;
                    comtok[num_arg_aux] = strtok2(comando_auxiliar, sep, 0);
                    num_arg_aux++;
                    
                    while((comtok[num_arg_aux] = strtok2(NULL, sep, 1)) != NULL) // strtok2 con comillas activas
					{ 
                        num_arg_aux++;
                    }
                }
            }
            else
			{
                //Llamamos a la funcion strtok2 con las comillas desactivadas

                num_arg_aux = 0;
                comtok[num_arg_aux] = strtok2(comando_auxiliar, sep, 0);
                num_arg_aux++;

                while((comtok[num_arg_aux] = strtok2(NULL, sep, 0)) != NULL) // strtok2 con las comillas desactivadas
				{ 
                    num_arg_aux++;
                }

            }
			
	
			if(num_arg_aux == 4) // delete campo x valor //
			{
				for(int k=0; k<TablaAuxiliar->numCampos; k++)
				{
					if(strcmp(TablaAuxiliar->campos[k], comtok[1])==0)
					{
						if(TablaAuxiliar->tipos[k]==NUM && EsDecimalValido(comtok[3]) == 1)
						{
							GranLinea = BorrarLineas(TablaAuxiliar->numCampos, GranLinea, TablaAuxiliar->tipos[k], comtok[3], comtok[2], k);
							ActualizarLineas(nombre_bd, GranLinea, TablaAuxiliar);
							
							break; 
						}
						else if(TablaAuxiliar->tipos[k] == DATE && (EsFechaValidaDMA(comtok[3]) == 1))
						{
							GranLinea = BorrarLineas(TablaAuxiliar->numCampos, GranLinea, TablaAuxiliar->tipos[k], comtok[3], comtok[2], k);
							ActualizarLineas(nombre_bd, GranLinea, TablaAuxiliar);
							
							break;
						}
						else if(TablaAuxiliar->tipos[k] == EMAIL && EsCorreoValido(comtok[3]) == true)
						{
							GranLinea = BorrarLineas(TablaAuxiliar->numCampos, GranLinea, TablaAuxiliar->tipos[k], comtok[3], comtok[2], k);
							ActualizarLineas(nombre_bd, GranLinea, TablaAuxiliar);
							
							break;
						}
						else if(TablaAuxiliar->tipos[k] == TEXT)
						{
							GranLinea = BorrarLineas(TablaAuxiliar->numCampos, GranLinea, TablaAuxiliar->tipos[k], comtok[3], comtok[2], k);
							ActualizarLineas(nombre_bd, GranLinea, TablaAuxiliar);
							
							break;
						}
						else
						{
							printf("ERROR: Los tipos son incorrectos.\n");
							k++;
							continue;
						}
					}
					
					if(k+1 == TablaAuxiliar->numCampos)
					{
						printf("ERROR: Campo no encontrado.\n");
						continue; 
					}
				}
			}
			else
			{
				printf("ERROR: Numero incorrecto de parametros.\n");
				printf("Posibles comandos:\n");
				printf("delete <campo> = <valor>\n");
				printf("delete <campo> < <valor>\n");
				printf("delete <campo> > <valor>\n");
				continue;
			}
			
        }
		else if(strcmp(comtok[0], "order") == 0) // order 
		{
			if(strcmp(prompt, "*") == 0)
			{
				printf("ERROR: Necesitamos una carpeta activa.\n");
				continue;
			}
			if(strcmp(prompt, nombre_bd) == 0)
			{
				printf("ERROR: Necesitamos una tabla activa.\n");
				continue;
			}
			
			if(num_arg != 3)
			{
				printf("ERROR: Numero de parametros incorrecto\n");
				continue; 
			}
			
			comillas = strchr(comando_auxiliar, '"');

            if(comillas != NULL)
			{
                comillasaux = strchr(comillas+1, '"');

                if(comillasaux == NULL)
				{
                    //ERROR
                    printf("ERROR DE COMILLAS\n");
                    continue;
                }
                else
				{
                    //Llamamos a la funcion strtok2 con las comillas activadas
                    num_arg_aux = 0;
                    comtok[num_arg_aux] = strtok2(comando_auxiliar, sep, 0);
                    num_arg_aux++;
                    
                    while((comtok[num_arg_aux] = strtok2(NULL, sep, 1)) != NULL) // strtok2 con comillas activas
					{ 
                        num_arg_aux++;
                    }
                }
            }
            else
			{
                //Llamamos a la funcion strtok2 con las comillas desactivadas

                num_arg_aux = 0;
                comtok[num_arg_aux] = strtok2(comando_auxiliar, sep, 0);
                num_arg_aux++;

                while((comtok[num_arg_aux] = strtok2(NULL, sep, 0)) != NULL) //strtok2 con las comillas desactivadas
				{ 
                    num_arg_aux++;
                }
            }
			
			
			if(strcmp(comtok[2], "ASC") == 0) // order campo ASC
			{
				for(int i=0; i<TablaAuxiliar->numCampos; i++)
				{
					if(strcmp(TablaAuxiliar->campos[i], comtok[1]) == 0)
					{
						OrdenarAscendentemente(GranLinea, TablaAuxiliar, i); 
						ActualizarLineas(nombre_bd, GranLinea, TablaAuxiliar);
						break; 
					}
					if(i+1 == TablaAuxiliar->numCampos)
					{
						printf("ERROR: Campo no encontrado.\n");
						continue; 
					}
				}
			}
			else if(strcmp(comtok[2], "DES") == 0) // order campo DES
			{
				for(int i=0; i<TablaAuxiliar->numCampos; i++)
				{
					if(strcmp(TablaAuxiliar->campos[i], comtok[1]) == 0)
					{
						OrdenarDescendentemente(GranLinea, TablaAuxiliar, i); 
						ActualizarLineas(nombre_bd, GranLinea, TablaAuxiliar);
						break;
					}
					if(i+1 == TablaAuxiliar->numCampos)
					{
						printf("ERROR: Campo no encontrado.\n");
						continue; 
					}
				}
			}
			else
			{
				printf("ERROR: El comando order <campo> solo acepta como tercer parametro (ASC/DES)\n");
				continue; 
			}
		}
		else if(strcmp(comtok[0], "remove") == 0) // remove <nombre_tabla> 
		{
			if(num_arg == 2) 
			{
				if(strcmp(prompt, "*") == 0)
				{
					printf("ERROR: No hay una base de datos activa.\n");
					continue; 
				}
				
				auxiliar_comprobante = EntrarTabla(nombre_bd, comtok[1]);
				
				if(auxiliar_comprobante == 0)
				{
					printf("ERROR: La tabla no existe.\n");
					continue; 
				}
				
				TablaAuxiliar = BuscarTabla(comtok[1], GranTabla);
				
				if(strcmp(nombre_tabla, comtok[1]) == 0)
				{
					strcpy(prompt, nombre_bd);
					strcpy(nombre_tabla, "*");
				}
				
				// borramos la tabla
				GranTabla = borrarTabla(GranTabla, comtok[1]);
				
				
				// borrar el fichero
				char path[100] = "";
				
				strcpy(path, nombre_bd);
				strcat(path, "\\");
				strcat(path, comtok[1]);
				strcat(path, ".tab");
				
				remove(path);
				
				
				// actualizamos el meta
				EditarMeta(nombre_bd, GranTabla);
				
			}
			else
			{
				printf("ERROR: Numero incorrecto de parametros.\n");
				printf("Posible comando: 'remove <nombre_tabla>'\n");
				continue;
			}
			
        }
		else if(strcmp(comtok[0], "import") == 0 && strcmp(comtok[1], "table") == 0 && strcmp(comtok[3], "from") == 0) // import table <nombre_tabla> from <nombre_bd> 
		{
            if(num_arg != 5)
			{
                printf("ERROR: Numero de parametros incorrecto.\n");
				printf("Posible comando: 'import table <nombre_tabla> from <nombre_bd>'.\n");
                continue;
            }
			
			if(strcmp(prompt, "*") == 0)
			{
				printf("ERROR: Necesitamos una carpeta activa.\n");
				continue;
			}
			
			if(EntrarDirectorio(comtok[4]) == 1) // si no existe la bd de origen //
			{
				printf("ERROR: La base de datos de origen no existe.\n");
				continue; 
			}
			
			if(strcmp(nombre_bd, comtok[4]) == 0) // Si la base de datos origen se llama igual que la activa //
			{
				printf("ERROR: La base de datos de origen no se puede llamar igual que la activa.\n");
				continue; 
			}
			
			if(EntrarTabla(comtok[4], comtok[2]) == 0) // Si la tabla de origen no existe //
			{
				printf("ERROR: La tabla de origen no existe.\n");
				continue; 
			}
			
			if(EntrarTabla(nombre_bd, comtok[2]) == 1) // Si la tabla ya existe en el destino
			{
				printf("ERROR: La tabla a copiar ya existe en la base de datos de destino.\n");
				continue; 
			}
			
			GranTabla = CopiarTabla(comtok[2], comtok[4], nombre_bd, GranTabla);
			
			MostrarTablas(GranTabla, nombre_bd);
			
		}
        else 
		{
            printf("ERROR: Comando erroneo.\n");
			continue;
        }
    }
}