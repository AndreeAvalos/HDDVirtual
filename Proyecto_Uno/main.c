#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
//libreria necesaria para poder utilizar el strok
#include <string.h>
#include <ctype.h>
//definimos los valores que usaremos como falso y verdadero
#define True 1
#define False 0



int main()
{
    printf("--------------Primer Proyecto de Manejo e Implementos de Archivos------------\n");
    Menu();
    printf("---------------------------USTED ACABA DE SALIR-------------------------------");
    return 0;
}

//variables que se guardan para crear Archivo
int sizeArchivo=0;
char *pathArchivo=NULL,*unitArchivo=NULL,*tipoArchivo=NULL,*carpetaArchivo[30];
//banderas para saber si se puede ejecutar la creacion de archivos
//{size,unidad,ruta}
int fileReady[3]= {False,False,False};
//-------------------------------------------

//Variable que pone pausa en el while
int NewLine = True;

//Arreglo que contendra la cadena
char linea[500];

void Menu()
{
    int salir=False;
    char desicion[1];
    printf("\t");

    while(salir!=True)
    {
        char separador[] ="-";
        printf("--Ingrese el Comando a Realizar--\n");
        scanf("%[^\n]", linea);
        while(getchar()!='\n');
        Parser(linea);
        char *trozo = NULL;
        //Trozo el cual recoge el modulo que se va a ejecutar
        trozo = strtok(linea, separador);
        //el primer trozo indica que comando vamos a utilizar
        if(strcmp(trozo, "mkdisk") == 0)
        {
            //Imprimimos que esta en el menu de creacion de archivos
            printf("*************************************\n");
            printf("* Ingreso a la creacion de Archivos *\n");
            printf("*************************************\n");
            //arreglo para guardar los parametros de la creacion de un archivo binario
            char *parametros[4];
            //for que recorre en busca de - para hacer split
            for(int i=0; i<4; i++)
            {
                //guardamos el trozo de cadena
                parametros[i]=trozo;
                //hacemos el proximo split
                trozo = strtok( NULL, separador);

            }
            //llamamos el metodo MKDISK para optener los valores y crear el archivo
            MKDISK(parametros[0],parametros[1],parametros[2],parametros[3]);
        }
        else if(strcmp(trozo,"rmdisk")==0)
        {
            //Imprimimos que esta en el menu de creacion de archivos
            printf("****************************************\n");
            printf("* Ingreso a la Eliminacion de Archivos *\n");
            printf("****************************************\n");
            //arreglo para guardar los parametros de la creacion de un archivo binario
            char *parametros[2];
            //for que recorre en busca de - para hacer split
            for(int i=0; i<2; i++)
            {
                //guardamos el trozo de cadena
                parametros[i]=trozo;
                //hacemos el proximo split
                trozo = strtok( NULL, separador);

            }
            //llamamos el metodo MKDISK para optener los valores y crear el archivo
            RMDISK(parametros[0],parametros[1]);


        }
        else if(strcmp(trozo,"exit")==0)
        {
            salir=True;
        }

        else
        {
            //si no existe el comando imprimimos
            printf("ERROR---No existe el comando %s\n",trozo);
            printf("\t");

        }

    }
}

void separarRuta(char *ruta)
{
//arreglo de char, tomandolo como STRING
    char palabra[200];
    //string que toma la ruta como apuntador para no tocar el original
    char *string=ruta;
    //copiamos el contenido del string de entrada y lo pasamos a variable string
    strcpy(palabra,string);

    //printf("La direccion es: %s\n",palabra);
    //variable en donde se alojara la ruta temporal, los nombres de direccion
    char *rutatemp;
    //split para separar la ruta temporal
    rutatemp = strtok(palabra,"/");
    //contador para que vaya contando cuantas carpetas existen antes del archivo
    int contador =0;
    //ciclo que recorre el string y lo separa
    while(rutatemp!=NULL)
    {
        rutatemp = strtok(NULL,"/");
        contador++;
    }
    //volvemos a copar el string para divirlo
    strcpy(palabra,string);
    //printf("Ruta a divir: %s\n",palabra);
    //lo separamos por /
    rutatemp = strtok(palabra,"/");
    //directorio donde se va a juntar la ruta completa de carpetas
    char *rutaDef[30];
    rutaDef[0]='/';
    //ciclo que concatena el nombre y un / antes de cada nombre
    for(int i=0; i<contador*2-3; i++)
    {
        if(i%2==0)
        {
            strcat(rutaDef,rutatemp);
            rutatemp = strtok(NULL,"/");
        }
        else
        {
            strcat(rutaDef,"/");
        }
    }
    //copiamos el directorio de carpetas a la variable global
    strcpy(carpetaArchivo,rutaDef);


}
//metodo para parsear sin espacios y convertirlo en minusculas
void Parser(char cadena[200])
{
//variable que guardara la palabra de manera temporal
    char temporal[200];
    //contador que se utilizara para guardar la posicion en el temporal
    int contador=0;
    for(int i=0; i<200; i++)
    {
        //Si viene espacio vacio simplemente lo ignora
        if(cadena[i]==32)
        {

        }
        else
        {
            //igualamos la posicion del contador en el temporal con la cadena en minusculas
            temporal[contador]=tolower(cadena[i]);
            contador++;
        }
    }
    //copiamos el valor temporal en linea
    memcpy(&linea,&temporal,sizeof(temporal));

}

//metodo publico de Creacion de Archivos binarios
void MKDISK(char *cmd,char *size, char *unit, char *path)
{
    split(size,cmd);//separamos la parte de tamano y obtenemos su valor
    split(unit,cmd);//obtenemos el valor de la unidad
    split(path,cmd);//obtenemos el valor de la ruta

    if(fileReady[0]==True && fileReady[0]==True && fileReady[0]==True)
    {
        printf("Tamano de la unidad a crear: %d\n",sizeArchivo);
        printf("Tipo de unidad: %s\n",tipoArchivo);
        printf("Ruta de archivo a crear: %s\n",pathArchivo);
        separarRuta(pathArchivo);//separamos la ruta para poder crear el arbol de carpetas donde se creeara el archivo
        if(sizeArchivo>0)
            CrearArchivo(sizeArchivo,unitArchivo,pathArchivo);//creamos el archivo binario que sera nuestro disco duro virtualo
        else
            printf("No se puede crear archivo por que es 0 o numero negativo");

        fileReady[0]=False;
        fileReady[1]=False;
        fileReady[2]=False;
    }
    else
        printf("No se puede crear archivo por falta de parametros\n");
    printf("********************* Proceso Terminado ************************\n");
    printf("\t");
    NewLine=False;
}
//metodo interno para creacion de archivos
void CrearArchivo(int size, char *unit,char *path)
{
//string donde se guardara la instruccion

    char comando[500];
    strcpy(comando,"sudo mkdir -p ");//comando para crear arbol de carpetas
    strcat(comando,carpetaArchivo);//concatenamos el comando con la ruta de carpetas
    //printf("Comando a ejecutarse %s\n",comando);
    system(comando);//ejecutamos el comando
    strcpy(comando,"sudo chmod 777 ");//comando para dar permisos de escritura lectura y ejecucion
    strcat(comando,carpetaArchivo);//concatenamos el comando con la carpeta a dar permisos
    system(comando);//ejecutamos el comando
    int bt=size;
    int bytes = bt*8; //bytes a poner en el archivo
    int kilobytes = bt*1024;//kilobytes en el archivo
    int megabytes =kilobytes*1024;//megabytes en el archivo
    char *bite='\0';


    FILE *archivo = fopen(path,"ab+");//creamos un archivo tipo solo para abrir
    if(archivo)//comprobamos si existe el archivo
    {

        if(strcmp(unitArchivo,"b")==0)//comparamos si es bytes
        {
            //ciclo que llena el archivo de 0 byte por byte

            for(int i=0; i<bytes; i++)
            {
                fwrite(&bite,1,1,archivo);
            }


        }
        else if(strcmp(unitArchivo,"k")==0)//comparacion si es kilobytes
        {

            for(int i=0; i<kilobytes; i++)
            {
                fwrite(&bite,1,1,archivo);
            }


        }
        else if (strcasecmp(unitArchivo,"m")==0)//comparamos si es megabyte
        {
            for(int i=0; i<megabytes; i++)
            {
                fwrite(&bite,1,1,archivo);
            }

        }
        else
        {
            //error no ejecutar
            printf("=====NO SE PUDO CREAR EL DISCO===");

        }
        printf("Se creo el archivo en el directorio\n");


    }
    else
    {
        printf("No se pudo crear el Archivo\n");

    }


    fclose(archivo);

}

void RMDISK(char *cmd,char *path)
{
    split(path,cmd);//separamos el ruta para obtener el valor
    if(fileReady[2]==True)
    {
        printf("Ruta donde se va a eliminar: %s\n",pathArchivo);
        char comando[500];
        FILE *archivo = fopen(pathArchivo,"rb");
        if(archivo)
        {
            fclose(archivo);

            strcpy(comando,"sudo rm ");
            strcat(comando,pathArchivo);
            system(comando);



            archivo=fopen(pathArchivo,"rb");
            if(archivo)
            {
                printf("No se pudo eliminar el Disco\n");
            }
            else
            {

                printf("Disco Eliminado con exito\n");
            }

        }

        else
        {
            printf("No existe Disco en la direccion %s\n",pathArchivo);
        }
        fileReady[2]=False;
        free(archivo);
        pathArchivo=NULL;
    }
    else
    {
        printf("No ha ingresado una ruta\n");
    }

    printf("********************* Proceso Terminado ************************\n");
    printf("\t");

}

//para separar por split
void split(char *valor,char *cmd)
{

    char *temporal;
    char separador[]="=";

    //Separamos los valores de entrada
    //Aqui podemos asegurarnos si lo que viene es SIZE,UNIT o PATH
    temporal = strtok(valor,separador);
    if(temporal!=NULL)
    {
        //temporal = toLowerCase(temporal);
        if(strcmp(temporal,"size")==0)
        {
            //separamos los valores size del valor numerico
            temporal = strtok(NULL,separador);
            //Parseamos el valor a int
            sizeArchivo = atoi(temporal);
            fileReady[0]=True;
        }
        else if(strcmp(temporal,"unit")==0)
        {
            temporal = strtok(NULL,separador);
            //Especificamos que tipo es
            unitArchivo = temporal;
            fileReady[1]=True;
            if(strcmp(unitArchivo,"k")==0)
                tipoArchivo="KiloByte";
            else if(strcmp(unitArchivo,"m")==0)
                tipoArchivo="MegaByte";
            else
                tipoArchivo="Byte";
        }
        else if(strcmp(temporal,"path")==0)
        {
            temporal = strtok(NULL,separador);
            //Parseamos el valor a int
            pathArchivo = temporal;
            fileReady[2]=True;
        }
        else
        {
            printf("ERROR---No existe parametro %s en linea de comando %s\n",temporal,cmd);
            return;
        }
    }
    else
    {
        printf("ERROR---Falta de argumentos en linea de comando %s\n",cmd);
    }
}


