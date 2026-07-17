/*
Medrano Solano Enrique y Frida Regina Gutierrez Benavides
EDA II | Proyecto Final

Ecualización de Histogramas en Imagenes con lenguaje C.
*/

//Librería a utilizar
#include <stdio.h>
#include <stdlib.h>
#include <omp.h> // Paralelo
#include <math.h>

//Librería para crear y leer imagenes
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb-master/stb_image_write.h"

//Variables globales
#define L 256 //Definicón de caracteres
int width, height, channels, numThreads, widthP, heightP, channelsP, sizeImage;
int minimo = INT_MAX; //Inicia con un valor maximo para evitar errores.
double startTime, finishTime, loadImaST, loadImaFT, loadImaPST, loadImaPFT, generationImaST, generationImaFT, CSVgenerationST, CSVgenerationFT;
double loadSec, loadPl, genImaSec, genImaPl, CSVgenSec, CSVgenPl, secuencialTime, parallelTime, speedUp;

//Definición de funciones
double equalizedSec(unsigned char *image);
double equalizedParallel(unsigned char *imageP); 

int main(int argc, char *argv[]){
    //Método principal
    // Regresa si no se le pasan dos argumentos en la terminal
    if(argc != 2){
        printf("Ingrese: %s [Nombre del archivo de la imagen.jpg]\n", argv[0]);
        exit(0);
    }
    char *imageFileName = argv[1];
    //Se carga la imagen
    loadImaST = omp_get_wtime(); //Inico sec
    unsigned char *image = stbi_load(imageFileName, &width, &height, &channels, 0);
    loadImaFT = omp_get_wtime(); //Fin sec
    loadSec = loadImaFT - loadImaST; //Total tiempo load secuencial
    //Paralelo
    loadImaPST = omp_get_wtime(); //Inico parallel
    unsigned char *imageP = stbi_load(imageFileName, &widthP, &heightP, &channelsP, 0);
    loadImaPFT = omp_get_wtime(); //Fin parallel
    loadPl = loadImaPFT - loadImaPST; //Total timepo load Parallel

    printf("\n---------PROYECTO FINAL--------\n");
    printf("------Ecualizacion de imagenes------\n");

    //Casos de errores
    if(imageFileName == NULL){
        printf("Error al cargar imagen: %s \n", argv[1]);
        return 0;
    } else{
        printf("\n¡Imagen: %s subida con exito!\n", argv[1]);
        printf("\nParametros de la imagen\n");
        printf("Ancho: %d\nAltura: %d\nCanales: %d\nTamaño; %d\n", width, height, channels, width*height);

        printf("\nEcualizacion Secuencial\n");
        equalizedSec(image);
        printf("\nCDF minimo: %d\n", minimo);
        printf("Tiempo de ejecucion: %f [s]\n", secuencialTime);
        printf("Timepo de carga de la imagen: %f [s]\n", loadSec);
        printf("Tiempo generacion de la imagen: %f [s]\n", genImaSec);
        printf("Tiempo generacion del CSV: %f [s]\n", CSVgenSec);
        
        printf("\nEcualizacion Paralela\n");
        equalizedParallel(imageP);
        printf("\nCDF minimo: %d\n", minimo);
        printf("Tiempo de ejecucion: %f [s]\n", parallelTime);
        printf("Timepo de carga de la imagen: %f [s]\n", loadPl);
        printf("Tiempo generacion de la imagen: %f [s]\n", genImaPl);
        printf("Tiempo generacion del CSV: %f [s]\n", CSVgenPl);

        printf("\nMetricas\n");
        speedUp = secuencialTime / parallelTime;
        printf("Numero de procesadores: %d\n", numThreads);
        printf("SpeedUp: %f [s]\n", speedUp);
        printf("Eficiencia: %f [s]\n", speedUp / numThreads);
        printf("Overhead: %f [s]\n", (parallelTime - (secuencialTime / numThreads)));

    }
    return 0;
}

//Funciones
double equalizedSec(unsigned char *image){
    //Método para la ecualización de imagenes de modo secuencial
    //Tiempos de la ecualización
    startTime = omp_get_wtime(); //Inicia

    sizeImage = width * height; //Tamaño de la imagen

    //Se crear los arreglo llenos de "0"
    int histograma[L] = {0}; //Histograma original
    int histoEqualized[L] = {0}; //Histograma equalizado
    int CDF[L] = {0}; // Función distributiva acumulativa
    int equalizedCDF[L]; // CDF equalizada

    //Calcular histograma original
    for(int i = 0; i < sizeImage; i++){
        histograma[image[i * channels]]++;
    }

    //Calcular CDF
    CDF[0] = histograma[0];
    for(int i = 1; i < L; i++){
        CDF[i] = histograma[i] + CDF[i - 1];
    }

    //Encontrar valor minimo de CDF != 0
    for (int i = 0; i < L; i++)
    {
        if (CDF[i] < minimo && CDF[i] != 0)
        {
            minimo = CDF[i];
            break; //Parar forzado
        }
    }

    //Calcular CDF equalizado
    for (int i = 0; i < L; i++)
    {
        equalizedCDF[i] = (int)(((double)(CDF[i] - minimo) / (sizeImage - minimo)) * 255.0);
        equalizedCDF[i] = round(equalizedCDF[i]);
    }

    //Se crea el canal de la imagen en rojo
    unsigned char *imageRed = malloc(sizeImage);
    //Aplicar equalización a la imagen
    for (int i = 0; i < sizeImage; i++)
    {
        imageRed[i] = equalizedCDF[image[i * channels]];
    }

    //Calcular el histograma equalizado
    for (int i = 0; i < sizeImage; i++)
    {
        histoEqualized[imageRed[i]]++;
    }

    finishTime = omp_get_wtime(); //FInaliza tiempo
    secuencialTime = finishTime - startTime; //Tiempo en total secuencial;
    printf("¡Se generó el histograma equalizado!\n");

    //Guarda y crea la imagen ecualizada
    generationImaST = omp_get_wtime(); //Inicio
    stbi_write_jpg("ImageES.jpg", width, height, 1, imageRed, 100);
    generationImaFT = omp_get_wtime(); //Fin
    genImaSec = generationImaFT - generationImaST; //Total generación de la imagen
    printf("¡Se creó la imagen ecualizada!!\n");

    //Libera memoria de la imagen
    free(imageRed);
    stbi_image_free(image);

    //Se llama a la función SaveCSV
    CSVgenerationST = omp_get_wtime(); //Inicio
    FILE *csv = fopen("SecHistogram.csv", "w");
    fprintf(csv, "%s, %s, %s\n", "Valor", "Histograma Original", "Histograma Equalizado");
    for (int i = 0; i < L; i++)
    {
        fprintf(csv, "%d, %d, %d\n", i, histograma[i], histoEqualized[i]);
    }
    fclose(csv); //Cerrar archivo
    CSVgenerationFT = omp_get_wtime(); //Fin
    CSVgenSec = CSVgenerationFT - CSVgenerationST;

    return minimo, secuencialTime, genImaSec, CSVgenSec;
}

double equalizedParallel(unsigned char *imageP){
    //Método para la ecualización de imagenes de modo paralelo

    //Se establecer la varibales internas
    sizeImage = widthP * heightP; //Tamaño de pixeles de la imagen
    int histograma[L];
    int histoEqualized[L];
    int CDF[L];
    int equalizedCDF[L];
    int i;

    //Se crea el canal rojo de la imagen
    unsigned char *imageRedP = malloc(sizeImage);

    //Numero de procesadores y de hilos
    numThreads = omp_get_num_procs();
    omp_set_num_threads(numThreads); //Numeros de hilos

    startTime = omp_get_wtime(); //Inicio paralelo
    //Inicia paralelización
    #pragma omp parallel private(i)
    {
        //llenar de 0 los arreglos
        #pragma omp for
        for (i = 0; i < L; i++)
        {
            histograma[i] = 0;
            histoEqualized[i] = 0;
            CDF[i] = 0;
            equalizedCDF[i] = 0;
        }
        #pragma omp barrier

        #pragma omp for reduction(+:histograma)
        //Calcular el histograma original
        for (i = 0; i < sizeImage; i++)
            #pragma omp atomic
            histograma[imageP[ i* channelsP]]++;
    }

    //Calcular CDF
    #pragma omp single //Solo se le encarga a un hilo exclusivo
    {
        CDF[0] = histograma[0];
        for (i = 1; i < L; i++)
        {
            CDF[i] = histograma[i] + CDF[i - 1];
        }
    }

    //Calcular el CDF Minimo != 0, con la clausula reduction
    #pragma omp parallel for reduction(min:minimo)
    for (i = 0; i < L; i++)
    {
        if (CDF[i] < minimo && CDF[i] != 0)
        {
            minimo = CDF[i];
        }
    }

    //Calcular CDF equalizado
    #pragma omp parallel for 
    for (i = 0; i < L; i++)
    {
        equalizedCDF[i] = (int)(((double)(CDF[i] - minimo) / (sizeImage - minimo)) * 255.0);
        equalizedCDF[i] = round(equalizedCDF[i]);
    }

    //Aplicar la ecualización a la imagen en el canal rojo
    #pragma omp parallel for
    for (i = 0; i < sizeImage; i++)
    {
        imageRedP[i] = equalizedCDF[imageP[i * channelsP]];

        //Calcular el histograma equalizado
        #pragma omp atomic
        histoEqualized[imageRedP[i]]++;
    }

    finishTime = omp_get_wtime(); //Fin paralelo
    parallelTime = finishTime - startTime; //Total del tiempo de método paralelo

    printf("\n¡Se generó el histograma ecualizado!\n");

    //Aquí termina la paralelización
    #pragma omp single
    {
        //Crear y guardar la imagen ecualizada
        generationImaST = omp_get_wtime(); //INicio
        stbi_write_jpg("ImageEP.jpg", widthP, heightP, 1, imageRedP, 100);
        generationImaFT = omp_get_wtime(); //Fin
        genImaPl = generationImaFT - generationImaST; //Total generacion de imagen
        printf("¡Se creó la imagen ecualizada!!\n");

        //Se crea el archivo .csv
        CSVgenerationST = omp_get_wtime(); //Inicio
        FILE *csv = fopen("ParallelHistogram.csv", "w");
        fprintf(csv, "%s, %s, %s\n", "Valor", "Histograma Original", "Histograma Equalizado");
        for (i = 0; i < L; i++)
        {
            fprintf(csv, "%d, %d, %d\n", i, histograma[i], histoEqualized[i]);
        }
        fclose(csv); //Cerrar archivo
        CSVgenerationFT = omp_get_wtime(); //FIn
        CSVgenPl = CSVgenerationFT - CSVgenerationST; //Total de generacion CSV
    }
    //LIberar memoria de la imagen
    free(imageRedP);
    
    return minimo, parallelTime, genImaPl, CSVgenPl, numThreads;
}
