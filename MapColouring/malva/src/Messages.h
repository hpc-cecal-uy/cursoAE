/*****************************************************************************/
/***																	   ***/
/***		Modificado por G.J.L.P.										   ***/
/***		Añadidos nuevos mensajes que indican falta de algún 	 	   ***/
/***		Fichero de Configuración (No específico para ningún	  		   ***/
/***		problema) o nuevos errores.				  					   ***/
/***																	   ***/
/*****************************************************************************/

#ifndef RLFAP_MESSAGES
#define RLFAP_MESSAGES

#ifndef MAX_BUFFER
#define MAX_BUFFER 200
#endif

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

inline void show_message(int value)
{
	switch (value)
	{
		case 1: cout << endl << "Error: number of arguments in the execution call is incorrect !!"
			     << endl; break;
		case 2: cout << endl << "Error: It's imposible find Configuration file !!" << endl;
			break;
		/* Específicos de RLFAP */
		case 3:	cout << endl << "Error: It is imposible find the Celar problem definition file (cst.txt) !!"
			     << endl; break;
		case 4: cout << endl << "Error: It is imposible find the Celar domains file (dom.txt) !!"
			     << endl; break;
		case 5:	cout << endl << "Error: It is imposible find the Celar links file (var.txt) !!"
			     << endl; break;
		case 6:	cout << endl << "Error: It is imposible find the Celar constraints file (ctr.txt) !!"
			     << endl; break;
		/* Fallos de Memoria */
		case 7:	cout << endl << "Error: No avalible memory for \"malloc\" operation !!"  << endl;
			break;
		case 8:	cout << endl << "Error: in \"free\" operation !!"  << endl;
			break;
		/* Específicos del MaxCut */
		case 9:	cout << endl << "Error: It is imposible find the Maxcut file (Maxcut.txt) !!"
			     << endl; break;
		/* Genéricos de Falta de ficheros de configuracion  adicionales al mensaje 2 */
		case 10: cout << endl << "Error: It's imposible find Configuration file (Config.cfg) !!"
			      << endl; break;
		case 11: cout << endl << "Error: It's imposible find Skeleton Configuration File (Ske.cfg) !!"
			      << endl; break;
		case 12: cout << endl << "Error: It's imposible find Instance Problem File !!" << endl;
			 break;
		case 13: cout << endl << "Error: It's imposible find Resultate File !!" << endl;
			 break;
		case 14: cout << endl << "Error: Index out of Range !!" << endl;
			 break;
		default: cout << endl << "Unkown Error !!" << endl;
	}

	cout << endl << " " << endl;
	exit(-1);
}

inline void continue_question()
{
	fflush(stdout);
	cout << endl << "Press any key to continue..." << endl;
	fflush(stdin);
	getc(stdin);
}

inline void get_path(const char *source,char *target)
{
	int last = 0;

	for(int i = 0; i < strlen(source); i++)
	{
		target[i] = source[i];
		if(target[i] == '/')
			last = i;
	}
	target[last+1] = '\0';
}

inline unsigned count_lines(char *file_name) // returns the number of lines of a file
{
	char line[MAX_BUFFER];
	FILE *file;
	int count=0;

	if ((file=fopen(file_name,"r"))==NULL)
	{
		fflush(stdout);
		printf("File not found !");
	}
  
	while (!feof(file))
	{
		if (fgets(line,MAX_BUFFER,file)) count++;    		     	
		else
		{
			fclose(file);
			break;
		}
	}	
	return count;
}

#endif
