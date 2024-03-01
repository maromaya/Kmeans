#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
 
 /*functions:*/
int checktypeClus(int K);
int checktypeIter(int iteration_number);
double ECDis(double* arr1, double* arr2, int d);
void printAll(double** centroids, int K, int d);
int checkEPS(double **centroids,double **prevCen,int K,int d);



int checktypeClus(int K) {
    if (K < 2) {
        printf("Invalid number of clusters!\n");
        return 1;
    }
    return 0; 
}

int checktypeIter(int iteration_number) {
    if (iteration_number < 1 || iteration_number > 1000) {
        printf("Invalid maximum iteration!\n");
        return 1;
    }
    return 0; 
}

double ECDis(double* arr1, double* arr2, int d) {
    double sum = 0.0;
    double sub;
    int i;
    for (i = 0; i < d; i++) {
        sub = arr1[i] - arr2[i];
        sum += sub * sub;
    }
    return sqrt(sum);
}

void printAll(double** centroids, int K, int d) {
    int i;
    int j;
    for (i = 0; i < K; i++) {
        for (j = 0; j < d; j++) {
            if(j<d-1){
            printf("%.4f,", centroids[i][j]); 
            }
            else{
                printf("%.4f", centroids[i][j]); 
            }
        }
        printf("\n");
    }
}


int checkEPS(double **centroids,double **prevCen,int K,int d){
    int i;
    double res;
    for(i=0;i<K;i++){
        res=ECDis(centroids[i],prevCen[i],d);
        if (res>0.001){
            return 0;
        }

    }
    return 1;
}



int main(int argc, char** argv) {
    int K;
    int iteration_number;
    double** vectors = NULL;
    double** centroids = NULL;
    double** prevCen = NULL;
    int iter=0;
    int d=1;
    int i;
    int j;
    int rows=0;
    char* line = NULL;
    long len;
    int strlen1;
    char* token1; 
    int* pos = NULL;


    if (argc < 2 || argc > 3) {
        printf("Invalid number of arguments!\n");
        return 1;
    }
    K = atoi(argv[1]);
    


    if(checktypeClus(K) == 1){
        return 1;
    }
    if (argc == 4) {
        iteration_number = atoi(argv[2]);
        if(checktypeIter(iteration_number)==1){
            return 1;
        }
    } else {
        iteration_number = 200;
    }


    fseek(stdin, 0, SEEK_END);
    len = ftell(stdin);
    fseek(stdin, 0, SEEK_SET);
    line = malloc(sizeof(char) * (len+ 1));
    if (line == NULL) {
        printf("An Error Has Occurred");
        return 1;
    }

    line[len] = '\0';
    fread(line, sizeof(char), len, stdin);
    fclose(stdin);

    for (token1 = line; *token1 != '\n'; ++token1) 
    {
        if (*token1 == ',')
        {
             ++d;
        }
    }
    
    for ( ; *token1 != '\0'; ++token1) {
        if (*token1 == '\n') { 
            ++rows; 
            *token1 = '\0';
            }
    }

    if (K>rows){
        printf("Invalid number of clusters!");
        return 1;
    }
    
    vectors = calloc(rows, sizeof(double*));
    if (vectors == NULL) {
        printf("An Error Has Occurred");
        return 1;
    }
    for(i=0; i<rows; i++){
        vectors[i] = calloc(d, sizeof(double));
        if (vectors[i] == NULL) {
        printf("An Error Has Occurred");
        return 1;
    }
    }

    for(i=0; i< rows; i++){
        for(token1 = line; *token1 != '\0' ; ++token1)
        {
            if(*token1 == ','){
                *token1 = '\0';
            }
    }
        for (j=0; j<d; j++){
            vectors[i][j] = atof(line);
            strlen1 = strlen(line);
            line = line + strlen1 +1;
    }
    }

    centroids = malloc(K*sizeof(double*));
    prevCen = malloc(K*sizeof(double*));
    if((prevCen==NULL)||(centroids==NULL)){
        printf("An Error Has Occurred");
        return 1;
    }
    for (i=0; i<K; i++){
        centroids[i] = malloc(d * sizeof(double));
        prevCen[i] = malloc(d * sizeof(double));
    if((centroids[i]==NULL)||(prevCen[i]==NULL)){
            printf("An Error Has Occurred");
            return 1;
        }
        for(j=0; j<d; j++){
            centroids[i][j] = vectors[i][j];
            prevCen[i][j] = vectors[i][j];
        }
    }

    pos = malloc(rows*sizeof(int));
    if(pos==NULL){
        printf("An Error Has Occured");
        return 1;
    };


for (i = 0; i < rows; i++) {
        double min = 1e20;
        int indexJ = 0;
        for (j = 0; j < K; j++) {
            double cur = ECDis(centroids[j], vectors[i], d);
            if (cur < min) {
                min = cur;
                indexJ = j;
            }
        }
        pos[i] = indexJ;
}

for(i=0 ; i<K ; i++){
        int count =0;
        int k=0;
        for(j=0 ;j<d ;j++){
            centroids[i][j]=0.0;
        }
        for(j =0; j< rows ; j++){
            if(i==pos[j]){
                count+=1;
                for (k=0;k<d;k++){
                    centroids[i][k]+=vectors[j][k];
                }                    
            }
        }
        if (count==0){
            printf("An Error Has Occurred");
            return 1;
        }
        for (k=0;k<d;k++){
            centroids[i][k]=centroids[i][k] / count;
        }
    }

while ((checkEPS(centroids, prevCen, K, d) != 1) && (iter < iteration_number-1)) {
    iter+=1;


for (i = 0; i < rows; i++) {
        double min = 1e20;
        int indexJ = 0;
        for (j = 0; j < K; j++) {
            double cur = ECDis(centroids[j], vectors[i], d);
            if (cur < min) {
                min = cur;
                indexJ = j;
            }
        }
        pos[i] = indexJ;
}

for(i=0 ; i<K ; i++){
            for(j=0;j<d;j++){
                prevCen[i][j]=centroids[i][j];
                centroids[i][j]=0.0;
            }
        }

for(i=0 ; i<K ; i++){
        int count =0;
        int k=0;
        for(j=0 ;j<d ;j++){
            centroids[i][j]=0.0;
        }
        for(j =0; j< rows ; j++){
            if(i==pos[j]){
                count+=1;
                for (k=0;k<d;k++){
                    centroids[i][k]+=vectors[j][k];
                }                    
            }
        }
        if (count==0){
            printf("An Error Has Occurred");
            return 1;
        }
        for (k=0;k<d;k++){
            centroids[i][k]=centroids[i][k] / count;
        }
    }

}

printAll(centroids, K, d);


free(vectors);
free(pos);
free(centroids);
free(prevCen);

return 0;
}








