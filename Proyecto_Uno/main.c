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
int sizeArchivo;
char *pathArchivo,*unitArchivo,*tipoArchivo,*carpetaArchivo[30];
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
            MKDISK(parametros[1],parametros[2],parametros[3]);
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
            RMDISK(parametros[1]);


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

    printf("La direccion es: %s\n",palabra);
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
    printf("Ruta a divir: %s\n",palabra);
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
void MKDISK(char *size, char *unit, char *path)
{

    split(size);//separamos la parte de tamano y obtenemos su valor
    split(unit);//obtenemos el valor de la unidad
    split(path);//obtenemos el valor de la ruta
    printf("Tamano de la unidad a crear: %d\n",sizeArchivo);
    printf("Tipo de unidad: %s\n",tipoArchivo);
    printf("Ruta de archivo a crear: %s\n",pathArchivo);
    separarRuta(pathArchivo);//separamos la ruta para poder crear el arbol de carpetas donde se creeara el archivo
    CrearArchivo(sizeArchivo,unitArchivo,pathArchivo);//creamos el archivo binario que sera nuestro disco duro virtual
    printf("********************* Proceso Terminado ************************\n");
    printf("\t");
    NewLine=False;
}
//metodo interno para creacion de archivos
void CrearArchivo(char *size, char *unit,char *path)
{
//string donde se guardara la instruccion
    char comando[500];
    strcpy(comando,"sudo mkdir -p ");//comando para crear arbol de carpetas
    strcat(comando,carpetaArchivo);//concatenamos el comando con la ruta de carpetas
    printf("Comando a ejecutarse %s\n",comando);
    system(comando);//ejecutamos el comando
    strcpy(comando,"sudo chmod 777 ");//comando para dar permisos de escritura lectura y ejecucion
    strcat(comando,carpetaArchivo);//concatenamos el comando con la carpeta a dar permisos
    system(comando);//ejecutamos el comando
    char cadena [] = "0";
    int bytes = sizeArchivo; //bytes a poner en el archivo
    int kilobytes = sizeArchivo*1024;//kilobytes en el archivo
    int megabytes = kilobytes*1024;//megabytes en el archivo
    int numLineas = 0;
    int contador=0;

    FILE *archivo = fopen(path,"wb");//creamos un archivo tipo solo para abrir
    if(archivo)//comprobamos si existe el archivo
    {
        int i =0;
        int j =0;

        if(strcmp(unitArchivo,"b")==0)//comparamos si es bytes
        {
            //ciclo que llena el archivo de 0 byte por byte
            for(i=0; i<bytes; i++)
            {
                fputs(cadena, archivo);
            }

        }
        else if(strcmp(unitArchivo,"k")==0)//comparacion si es kilobytes
        {
            numLineas=kilobytes/100;
            //ciclo que llena el archivo de 0 byte por byte
            for(i=0; i<numLineas; i++)
            {
                for(j=0; j<100; j++)
                {
                    if(contador<=kilobytes)
                    {
                        fputs(cadena, archivo);
                        contador++;
                    }
                    else
                    {
                        break;
                    }
                }
                fputs("\n", archivo);
            }


        }
        else if (strcasecmp(unitArchivo,"m")==0)//comparamos si es megabyte
        {
            numLineas=megabytes/1000;
            //ciclo que llena el archivo de 0 byte por byte
            for(i=0; i<numLineas; i++)
            {
                for(j=0; j<1000; j++)
                {
                    if(contador<=megabytes)
                    {
                        fputs(cadena, archivo);
                        contador++;
                    }
                    else
                    {
                        break;
                    }
                }
                fputs("\n", archivo);
            }
        }
        else
        {
            //error no ejecutar

        }
        printf("Se creo el archivo en el directorio\n");
    }
    else
    {
        printf("No se pudo crear el Archivo\n");

    }

    fclose(archivo);

}

void RMDISK(char *path)
{
    split(path);//separamos el ruta para obtener el valor
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
    free(archivo);
    printf("********************* Proceso Terminado ************************\n");
    printf("\t");

}

//para separar por split
void split(char *valor)
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
        }
        else if(strcmp(temporal,"unit")==0)
        {
            temporal = strtok(NULL,separador);
            //Especificamos que tipo es
            unitArchivo = temporal;
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
        }
        else
        {
            printf("ERROR---No existe %s en linea de comando MKDISK\n",temporal);
            return;
        }
    }
    else
    {
        printf("ERROR---Falta de argumentos en linea de comando MKDISK\n");
    }
}

