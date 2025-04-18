//#include <stdio.h>
//#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/*************************************************************************
* RISC-V Vectorized Version
* Author: Cristóbal Ramírez Lazo
* email: cristobal.ramirez@bsc.es
* Barcelona Supercomputing Center (2020)
*************************************************************************/

#ifdef USE_RISCV_VECTOR
#include "../../common/vector_defines.h"
#endif

/************************************************************************/

//Enable RESULT_PRINT in order to see the result vector, for instruction count it should be disable
#define RESULT_PRINT
//#define INPUT_PRINT
#define MAXNAMESIZE 1024 // max filename length
#define M_SEED 9
#define MAX_WEIGHT 10
#define NUM_RUNS 100

int rows, cols;
int* wall;
int* result;
string inputfilename;
string outfilename;
//#include "timer.h"

void init(int argc, char** argv);
void run();
void run_vector();
void output_printfile(int *dst, string& filename);
//void init_data(int *data, string& filename );
/*************************************************************************
*GET_TIME
*returns a long int representing the time
*************************************************************************/
long long get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000000) + tv.tv_usec;
}
// Returns the number of seconds elapsed between the two specified times
float elapsed_time(long long start_time, long long end_time) {
        return (float) (end_time - start_time) / (1000 * 1000);
}
/*************************************************************************/

void init(int argc, char** argv)
{
    if(argc!=4){
        printf("Usage: pathfiner width num_of_steps input_file output_file\n");
        exit(0);
    }

    cols = atoi(argv[1]);
    rows = atoi(argv[2]);
    //inputfilename = argv[3];
    outfilename = argv[3];
    //}else{
    //            printf("Usage: pathfiner width num_of_steps input_file output_file\n");
    //            exit(0);
    //    }
    printf("cols: %d rows: %d \n", cols , rows);

    wall = new int[rows * cols];
    result = new int[cols];
    
    //int seed = M_SEED;
    //srand(seed);
    /*
    init_data(wall, inputfilename );
    */
    
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            wall[i*cols+j] = rand() % 10;
        }
    }
    
    //for (int j = 0; j < cols; j++)
    //    result[j] = wall[j];

#ifdef INPUT_PRINT
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%d ",wall[i*cols+j]) ;
        }
        printf("\n") ;
    }
#endif
}

void fatal(char *s)
{
    fprintf(stderr, "error: %s\n", s);

}

#define IN_RANGE(x, min, max)   ((x)>=(min) && (x)<=(max))
#define CLAMP_RANGE(x, min, max) x = (x<(min)) ? min : ((x>(max)) ? max : x )
#define MIN(a, b) ((a)<=(b) ? (a) : (b))

int main(int argc, char** argv)
{

// if(argc != 1 ) {
//         cout << "Usage: " << argv[0] << " <input_file> " << endl;
//         exit(1);
//     }   
long long start_0 = get_time();
init(argc,argv);
long long end_0 = get_time();
printf("TIME TO INIT DATA: %f\n", elapsed_time(start_0, end_0));
    //unsigned long long cycles;

#ifndef USE_RISCV_VECTOR
    run();
#else
    run_vector();
#endif

return EXIT_SUCCESS;
}

#ifndef USE_RISCV_VECTOR

void run()
{
    int min;
    int *src,*dst, *temp;
    
    //src = (int *)malloc(sizeof(int)*cols);
    
    printf("NUMBER OF RUNS: %d\n",NUM_RUNS);
    long long start = get_time();

    for (int j=0; j<NUM_RUNS; j++) {
        src = new int[cols];
        for (int x = 0; x < cols; x++){
            result[x] = wall[x];
        }

        dst = result;
        for (int t = 0; t < rows-1; t++) {
            temp = src;
            src = dst;
            dst = temp;
            //#pragma omp parallel for private(min)
            for(int n = 0; n < cols; n++){
              min = src[n];
              if (n > 0)
                min = MIN(min, src[n-1]);
              if (n < cols-1)
                min = MIN(min, src[n+1]);
              dst[n] = wall[(t+1)*cols + n]+min;
            }
        }   
        //delete src;
    }

    long long end = get_time();
    printf("TIME TO FIND THE SMALLEST PATH: %f\n", elapsed_time(start, end));

#ifdef RESULT_PRINT

    output_printfile(dst, outfilename );

#endif  // RESULT_PRINT
    free(dst);
    free(wall);
    free(src);
}

#else // USE_RISCV_VECTOR

void run_vector()
{
    int *dst;

    long long start = get_time();
    printf("NUMBER OF RUNS: %d\n",NUM_RUNS);

    for (int j=0; j<NUM_RUNS; j++) {
        for (int x = 0; x < cols; x++){
            result[x] = wall[x];
        }
        dst = result;

        unsigned long int gvl = __builtin_epi_vsetvl(cols, __epi_e32, __epi_m1);

        _MMR_i32    xSrc_slideup;
        _MMR_i32    xSrc_slidedown;
        _MMR_i32    xSrc;
        _MMR_i32    xNextrow; 

        int aux,aux2;

        for (int t = 0; t < rows-1; t++) 
        {
            aux = dst[0] ;
            for(int n = 0; n < cols; n = n + gvl)
            {
                gvl = __builtin_epi_vsetvl(cols-n, __epi_e32, __epi_m1);
                xNextrow = _MM_LOAD_i32(&dst[n],gvl);
                xSrc = xNextrow;
                aux2 = (n+gvl >= cols) ?  dst[n+gvl-1] : dst[n+gvl];
                xSrc_slideup = _MM_VSLIDE1UP_i32(xSrc,aux,gvl);
                xSrc_slidedown = _MM_VSLIDE1DOWN_i32(xSrc,aux2,gvl);
                xSrc = _MM_MIN_i32(xSrc,xSrc_slideup,gvl);
                xSrc = _MM_MIN_i32(xSrc,xSrc_slidedown,gvl);
                xNextrow = _MM_LOAD_i32(&wall[(t+1)*cols + n],gvl);
                xNextrow = _MM_ADD_i32(xNextrow,xSrc,gvl);
                aux = dst[n+gvl-1];
                _MM_STORE_i32(&dst[n],xNextrow,gvl);
                FENCE();
            }
        }

        FENCE();
    }
    long long end = get_time();
    printf("TIME TO FIND THE SMALLEST PATH: %f\n", elapsed_time(start, end));

#ifdef RESULT_PRINT

    output_printfile(dst, outfilename );

#endif // RESULT_PRINT

    free(wall);
    free(dst);
}
#endif // USE_RISCV_VECTOR

/*
void init_data(int *data,  string&  inputfile ) {

    ifstream fp (inputfile.c_str());
    assert(fp.is_open()); // were there any errors on opening?

    int value;
    int i=0;
    while (!fp.eof()){
        fp >> data[i];
        i++;
    }

    if( !(i-1 == cols*rows) ) {
        printf("error: the number of elements does not match the parameters i=%d , colsxrows = %d \n",i,cols*rows);
        exit(1);
    }
    fp.close();
}
*/
void output_printfile(int *dst,  string& outfile ) {
    ofstream myfile;
    myfile.open(outfile);
    assert(myfile.is_open());

    for (int j = 0; j < cols; j++)
    {
        myfile << dst[j] <<" " ;
    }
    myfile.close();
}