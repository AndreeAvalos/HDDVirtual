#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
//libreria necesaria para poder utilizar el strok
#include <string.h>
#include <ctype.h>
#include <time.h>
//definimos los valores que usaremos como falso y verdadero
#define True 1
#define False 0


typedef struct
{
    int part_status;
    int part_type;
    int part_fit;
    int part_start;
    int part_size;
    char part_name[16];
} Particion;

typedef struct
{
    int mbr_size;
    int mbr_corrupt;
    char mrb_fecha_creacion[128];
    int mbr_disk_signature;
    Particion particion1,particion2,particion3,particion4;
    int part1,part2,part3,part4;
} MBR;

typedef struct
{
    int part_status;
    char part_fit;
    int part_next;
    int part_start;
    int part_size;
    char part_name[16];

} EBR;
int main()
{
    printf("--------------Primer Proyecto de Manejo e Implementos de Archivos------------\n");
    Menu();
    printf("---------------------------USTED ACABA DE SALIR-------------------------------");
    return 0;
}

//variables que se guardan para crear,remover,particionar
int sizeArchivo=0;
char *pathArchivo=NULL,*unitArchivo=NULL,*tipoArchivo=NULL,*carpetaArchivo[30];
char *nameParticion=NULL;
int typeParticion=-1,fitParticion=-1,delParticion=-1;
int sizeParticion=0,addsizeParticion=0;
int size_MBR=512;//size de mbr en bytes
int numPartExtends=0;
int valido=False;
int partValida=False;
//banderas para saber si se puede ejecutar la creacion de archivos
//{size,unidad,ruta,tipo,fit,delete,name,add}
int fileReady[8]= {False,False,False,False,False,False,False,False};
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
        else if(strcmp(trozo,"fdisk")==0)
        {
            //Imprimimos que esta en el menu de creacion de archivos
            printf("****************************************\n");
            printf("*  Ingreso a Creacion de Particiones   *\n");
            printf("****************************************\n");
            //arreglo para guardar los parametros de la creacion de un archivo binario
            char *parametros[9];
            char aux[50];
            char *paraux;
            for(int i =0; i<9; i++)
            {

                parametros[i]="NOHAY";

            }

            parametros[0]=trozo;
            for(int i=1; i<9; i++)
            {
                trozo=strtok(NULL,"-");

                if(i==8)
                {
                    if(trozo!=0x0)
                    {
                        if(strcmp(trozo,"add=")==0)
                        {
                            trozo=strtok(NULL,"-");
                            strcpy(aux,"add=-");
                            strcat(aux,trozo);
                            paraux=strtok(aux,"");
                            parametros[i]=paraux;
                        }
                        else
                        {
                            parametros[i]=trozo;
                        }
                    }

                    else
                    {
                        parametros[i]=trozo;
                    }
                }
                else
                {
                    parametros[i]=trozo;
                }
            }
            if(parametros[3]!=0x0)
            {
                if(existeMBR(parametros[3])==1)
                {
                    printf("MBR encontrado....se procedera a crear particion\n");
                }
                else
                {
                    printf("No se encontro MBR, se procedera a crear uno\n");
                    crearMBR(parametros[3],500);
                    printf("MBR creado con exito, se procedera a crear particion");
                }
            }
            else
            {
                printf("No existe ruta para crear MBR\n");
            }

            FDISK(parametros[0],parametros[1],parametros[2],parametros[3],parametros[4],parametros[5],parametros[6],parametros[7],parametros[8]);
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
int existeMBR(char *ruta)
{

    char rutat[300];
    if(ruta!=0x0)
        strcpy(rutat,ruta);
    split(rutat);
    int valor=0;

    FILE *archivo = fopen(pathArchivo,"rb");
    if(archivo)
    {
        fseek(archivo,0,SEEK_CUR);
        MBR mbrPueba;
        fread(&mbrPueba,sizeof(MBR),1,archivo);
        if(mbrPueba.mbr_corrupt!=0)
        {
            valor =1;
        }
        fclose(archivo);
    }

    return valor;
}
void crearMBR(char *ruta, int size_mbr)
{
    char rutat[300];
    if(ruta!=0x0)
        strcpy(rutat,ruta);
    split(rutat);
    FILE *archivo = fopen(pathArchivo,"wb");
    if(archivo)
    {
        MBR mbr;
        mbr.mbr_corrupt=4;
        mbr.mbr_size=size_mbr;
        time_t tiempo = time(0);
        struct tm *tlocal = localtime(&tiempo);
        strftime(mbr.mrb_fecha_creacion,128,"%d/%m/%y %H:%M:%S",tlocal);
        fseek(archivo,0,SEEK_CUR);
        //printf("tamano de MBR %d\n",sizeof(MBR));
        fwrite(&mbr,sizeof(MBR),1,archivo);
        printf("Se escribio el mbr correctamente en los primeros 512 bytes del disco\n");
        fclose(archivo);

    }
    else
    {
        printf("No existe disco para crear MBR\n");
    }

//    FILE *temp =fopen(pathArchivo,"rb");
//    if(archivo)
//    {
//        MBR siescribio;
//        fseek(temp,0,SEEK_CUR);
//        fread(&siescribio,sizeof(MBR),1,temp);
//        printf("el archivo esta corrupto si es 0: %d\n",siescribio.mbr_corrupt);
//        fclose(temp);
//    }
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

    if(fileReady[0]==True && fileReady[1]==True && fileReady[2]==True)
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

//usar TRUNCATE para crear archivos
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


    FILE *archivo = fopen(path,"wb+");//creamos un archivo tipo solo para abrir
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
        else if(strcmp(temporal,"type")==0)
        {
            temporal = strtok(NULL,separador);
            //Parseamos el valor a int
            fileReady[3]=True;
            if(strcmp(temporal,"p")==0)
                typeParticion=1;
            else if(strcmp(temporal,"e")==0)
                typeParticion=2;
            else  if(strcmp(temporal,"l")==0)
            {
                typeParticion=3;
            }
            else  if(strcmp(temporal,"null")==0)
                typeParticion=0;
            else
                typeParticion=-1;

        }
        else if(strcmp(temporal,"fit")==0)
        {
            temporal = strtok(NULL,separador);
            fileReady[4]=True;
            if(strcmp(temporal,"bf")==0)
                fitParticion=1;
            else if(strcmp(temporal,"ff")==0)
                fitParticion=2;
            else  if(strcmp(temporal,"wf")==0)
                fitParticion=3;
            else  if(strcmp(temporal,"null")==0)
                fitParticion=0;
            else
                fitParticion=-1;

        }
        else if(strcmp(temporal,"delete")==0)
        {
            temporal = strtok(NULL,separador);
            //Parseamos el valor a int
            fileReady[5]=True;
            if(strcmp(temporal,"null")==0)
                delParticion=0;
            else if(strcmp(temporal,"fast")==0)
                delParticion=1;
            else  if(strcmp(temporal,"full")==0)
                delParticion=2;
            else
                delParticion=-1;

        }
        else if(strcmp(temporal,"name")==0)
        {
            temporal = strtok(NULL,separador);
            //Parseamos el valor a int
            fileReady[6]=True;
            nameParticion = temporal;

        }
        else if(strcmp(temporal,"add")==0)
        {
            temporal = strtok(NULL,separador);
            //Parseamos el valor a int
            fileReady[7]=True;
            addsizeParticion = atoi(temporal);

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
MBR mbrActual;//mbr del disco actual para crear
Particion partActual;//particion a montar

//Metodo publico para particionar un disco
void FDISK(char *cmd, char *sizet, char *unit, char *path, char *type, char *fit, char *del,char *name, char *add)
{
    //printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n ",sizet,unit,path,type,fit,del,name,add);

    split(sizet,cmd);
    split(unit,cmd);
    char rutat[300];//donde se deposita la ruta ejemplo path=/home/andree/disco1.dsk
    if(path!=0x0)
        strcpy(rutat,path);
    split(rutat,cmd);//separamos la ruta
    split(type,cmd);
    split(fit,cmd);
    split(del,cmd);
    split(name,cmd);
    split(add,cmd);

    /*
    tipo:{ null=0,p=1,e=2,l=3,cualquierotra=-1}
    ajuse:{null=0,bf=1,ff=2,wf=3,cualquierotra=-1}
    eliminacion:{null=0,fast=1,full=2,cualquierotra=-1}
    */
    if(fileReady[0]==True && fileReady[1]==True && fileReady[2]==True&& fileReady[3]==True&& fileReady[4]==True&& fileReady[5]==True&& fileReady[6]==True&& fileReady[7]==True)
    {


        int tamano=0;
        if(strcmp(unitArchivo,"b")==0)
        {
            tamano=sizeArchivo;
        }
        else if(strcmp(unitArchivo,"k")==0)
        {
            tamano=sizeArchivo*1024;
        }
        else if(strcmp(unitArchivo,"m")==0)
        {
            tamano=sizeArchivo*1024*1024;
        }
        printf("El tamano es : %d\n",tamano);
        printf("La ruta donde se encuentra el archivo es: %s\n",pathArchivo);
        if(typeParticion==0)
        {
            printf("El tipo a crear es: Nula\n");
        }
        else if (typeParticion==1)
        {
            printf("El tipo a crear es: Primaria\n");
        }
        else if (typeParticion==2)
        {
            printf("El tipo a crear es: Extendida\n");
        }
        else if (typeParticion==3)
        {
            printf("El tipo a crear es: Logica\n");
        }

        if(fitParticion==0)
        {
            printf("Ajuste a realizar es: Nula\n");
        }
        else if (fitParticion==1)
        {
            printf("Ajuste a realizar es: BestFit\n");
        }
        else if (fitParticion==2)
        {
            printf("Ajuste a realizar es: FirstFit\n");
        }
        else if (fitParticion==3)
        {
            printf("Ajuste a realizar es: WorstFit\n");
        }

        if(delParticion==0)
        {
            printf("Tipo de eliminacion es: Nula\n");
        }
        else if (delParticion==1)
        {
            printf("Tipo de eliminacion es: Fast\n");
        }
        else if (delParticion==2)
        {
            printf("Tipo de eliminacion es: Full\n");
        }
        printf("Nombre de la particion: %s\n",nameParticion);
        printf("Tamano a agregar o disminuir particion: %d\n",addsizeParticion);

        if(typeParticion!=-1)
        {
            if(fitParticion!=-1)
            {
                if(delParticion!=-1)
                {
                    FILE *archivo = fopen(pathArchivo,"rb");
                    int comparasion=0;//comparasion para saber si existe la particion


                    if(archivo)//saber si existe el archivo
                    {
                        fseek(archivo,0,SEEK_CUR);//posicionamos el puntero al principio del archivo
                        fread(&mbrActual,sizeof(MBR),1,archivo);//leemos el mbr
                        if(mbrActual.part1==0)
                        {
                            fseek(archivo,size_MBR,SEEK_CUR);
                            //CrearParticion(estado{0=no activo, 1=activo},tipo{p=primario,e=extendida},tamano{int tamanoParticion},nombreparticion);
                            crearParticion(0,typeParticion,fitParticion,tamano,nameParticion,delParticion,addsizeParticion,pathArchivo);
                            if(partValida==True)
                            {
                                mbrActual.particion1=partActual;
                                mbrActual.part1=1;
                                printf("Particion creada con exito\n");
                            }
                            else
                            {
                                printf("No se pudo crear la particion");
                            }
                        }
                        else if(mbrActual.part2==0)
                        {
                            fseek(archivo,size_MBR,SEEK_CUR);
                            //CrearParticion(estado{0=no activo, 1=activo},tipo{p=primario,e=extendida},tamano{int tamanoParticion},nombreparticion);
                            crearParticion(0,typeParticion,fitParticion,tamano,nameParticion,delParticion,addsizeParticion,pathArchivo);
                            if(partValida==True)
                            {
                                mbrActual.particion2=partActual;
                                mbrActual.part2=1;
                                printf("Particion creada con exito\n");
                            }
                            else
                            {
                                printf("No se pudo crear la particion");
                            }
                        }
                        else if(mbrActual.part3==0)
                        {
                            fseek(archivo,size_MBR,SEEK_CUR);
                            //CrearParticion(estado{0=no activo, 1=activo},tipo{p=primario,e=extendida},tamano{int tamanoParticion},nombreparticion);
                            crearParticion(0,typeParticion,fitParticion,tamano,nameParticion,delParticion,addsizeParticion,pathArchivo);
                            if(partValida==True)
                            {
                                mbrActual.particion3=partActual;
                                mbrActual.part3=1;
                                printf("Particion creada con exito\n");
                            }
                            else
                            {
                                printf("No se pudo crear la particion");
                            }

                        }
                        else if(mbrActual.part4==0)
                        {
                            fseek(archivo,size_MBR,SEEK_CUR);
                            //CrearParticion(estado{0=no activo, 1=activo},tipo{p=primario,e=extendida},tamano{int tamanoParticion},nombreparticion);
                            crearParticion(0,typeParticion,fitParticion,tamano,nameParticion,delParticion,addsizeParticion,pathArchivo);
                            if(partValida==True)
                            {
                                mbrActual.particion4=partActual;
                                mbrActual.part4=1;
                                printf("Particion creada con exito\n");

                            }
                            else
                            {
                                printf("No se pudo crear la particion");
                            }
                        }
                        else
                        {
                            printf("No se pueden crear mas particiones en este disco, ya alcanzo las 4 posibles");
                        }

                        partValida=False;
                        //cerramos el archivo;
                        fclose(archivo);
                    }
                    else
                    {
                        printf("No se encontro disco en este directorio");
                    }

                    FILE *mbrTemp = fopen(pathArchivo,"wb");
                    if(archivo)
                    {
                        fseek(archivo,0,SEEK_CUR);
                        fwrite(&mbrActual,sizeof(MBR),1,mbrTemp);
                        fclose(mbrTemp);
                        printf("Se actualizo el MBR\n");
                    }
                    else
                    {
                        printf("No se puedo actualizar el MBR\n");
                    }

                    fileReady[0]=False;
                    fileReady[1]=False;
                    fileReady[2]=False;
                    fileReady[3]=False;
                    fileReady[4]=False;
                    fileReady[5]=False;
                    fileReady[6]=False;
                    fileReady[7]=False;
                }
                else
                {
                    printf("No existe el tipo de eliminacion\n");
                }

            }
            else
            {
                printf("No existe el tipo de ajuste\n");
            }
        }
        else
        {
            printf("No existe el tipo de particion\n");
        }

    }
    else
    {
        printf("No se puede crear Particion por falta de parametros\n");
    }



//
//    FILE *archivo = fopen(pathArchivo,"rb");//archivo de disco
//    int comparasion=0;//comparasion para saber si existe la particion
//
//
//    if(archivo)//saber si existe el archivo
//    {
//        fseek(archivo,0,SEEK_CUR);//posicionamos el puntero al principio del archivo
//        fread(&mbrActual,sizeof(MBR),1,archivo);//leemos el mbr
//
////        if(mbrActual.part1==1&&comparasion==0)//si existe la particion 1 y no esta encontrada la particion selecionada
////        {
////            if(strcmp(mbrActual.particion1.part_name,name)==0)//si la particion es la que buscamos
////            {
////                comparasion=1;//cambiamos el valor de comparacion para saber que lo encontramos
////                partActual=mbrActual.particion1;//ponemos la particion actual;
////                fseek(archivo,size_MBR,SEEK_CUR);
////                //CrearParticion(estado{0=no activo, 1=activo},tipo{p=primario,e=extendida},tamano{int tamanoParticion},nombreparticion);
////                crearParticion(0,typeParticion,fitParticion,sizeParticion,nameParticion);
////            }
////            else
////            {
////                comparasion=0;//seguimos con el valor 0
////            }
////        }
////
////        if(mbrActual.part2==1&&comparasion==0)
////        {
////            if(strcmp(mbrActual.particion2.part_name,name)==0)//si la particion es la que buscamos
////            {
////                comparasion=1;//cambiamos el valor de comparacion para saber que lo encontramos
////                partActual=mbrActual.particion2;//ponemos la particion actual;
////            }
////            else
////            {
////                comparasion=0;//seguimos con el valor 0
////            }
////        }
////
////        if(mbrActual.part3==1&&comparasion==0)
////        {
////            if(strcmp(mbrActual.particion3.part_name,name)==0)//si la particion es la que buscamos
////            {
////                comparasion=1;//cambiamos el valor de comparacion para saber que lo encontramos
////                partActual=mbrActual.particion3;//ponemos la particion actual;
////            }
////            else
////            {
////                comparasion=0;//seguimos con el valor 0
////            }
////        }
////
////        if(mbrActual.part4==1&&comparasion==0)
////        {
////            if(strcmp(mbrActual.particion4.part_name,name)==0)//si la particion es la que buscamos
////            {
////                comparasion=1;//cambiamos el valor de comparacion para saber que lo encontramos
////                partActual=mbrActual.particion4;//ponemos la particion actual;
////            }
////            else
////            {
////                comparasion=0;//seguimos con el valor 0
////            }
////        }

//
    printf("********************* Proceso Terminado ************************\n");
    printf("\t");
}

void crearParticion(int activo, int tipo, int fit, int size, char *nombre, int eliminar,int addsizeParticion,char *ruta)
{
    if(mbrActual.part1==0&&mbrActual.part2==0&&mbrActual.part3==0&&mbrActual.part4==0)
    {
        partActual.part_fit=fit;
        strcpy(partActual.part_name,nombre);
        partActual.part_type=tipo;
        partActual.part_status=activo;
        partActual.part_size=size;
        partActual.part_start=size_MBR;
        if(tipo==2)
        {
            numPartExtends=1;
        }
        return;
    }
    else
    {
        Particion temporal;
        //aqui realizamos el primer ajuste para posicionar la particion;
        int inicio = FirstFitDisco(size,ruta);
        partValida=True;

        if(valido==True)
        {
            temporal.part_fit=fit;
            strcpy(temporal.part_name,nombre);
            temporal.part_type=tipo;
            temporal.part_status=activo;
            temporal.part_size=size;
            temporal.part_start=inicio;
            if(tipo==2&&numPartExtends==0)
            {
                numPartExtends=1;
                partActual=temporal;
                partValida=True;
                return;
            }
            else
            {
                printf("Ya existe una particion extendida");
                partValida=False;
            }
            partActual=temporal;
            return;

        }
        else
        {
            printf("No existe espacio disponible para crear una particion del tamano %d\n",size);
        }
    }
}
//primer AJUSTE
int FirstFitDisco(int tamano,char *ruta)
{
    valido = 0;
    int contador_Espacios=1;
    int inicio = 512;
    char *tt='t';

    FILE *archivo = fopen(ruta,"rb");
    int salir = 0;
    if(archivo)
    {
        fseek(archivo,512,SEEK_CUR);
        int inicioArchivo = ftell(archivo);
        fseek(archivo,0,SEEK_END);
        int finArchivo= ftell(archivo);
        printf("Incio de archivo: %d\n",inicioArchivo);
        printf("fin de archivo %d\n",finArchivo);
        int iterador = finArchivo-inicioArchivo;
        int c;
        int contador=0;

        if(tamano<=iterador)
        {
            rewind(archivo);
            fseek(archivo,512,SEEK_CUR);
            printf("puntero en %d\n",ftell(archivo));
            printf("Nos desplazamos despues de la memoria reservada del mbr\n");
            while(contador<=iterador&&contador_Espacios<=tamano)
            {
                contador_Espacios++;
                contador++;
                c=fgetc(archivo);
                if(c=='\0')
                {
                    valido=True;
                }
                else
                {
                    valido=False;
                    inicio+=contador_Espacios;
                    contador_Espacios=1;
                }
            }
        }
        else
        {
            printf("El tamano de la particion es mayor al espacio disponible\n");
            valido = False;
        }
    }
    else
    {
        printf("No existe disco para la particion\n");
    }
    printf("El inicio para colocar la posicion es: %d\n",inicio);

    return inicio;
}
