#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef struct pair{
	float distance;
	int validation_index;
	int train_index;
}pair;
int nCols_fold1, nRows_fold1;
int nCols_fold2, nRows_fold2;
int nCols_fold3, nRows_fold3;
pair distance_fold1[2225][4449], distance_fold2[2225][4449], distance_fold3[2225][4449];
void store(int x_train[][192], int y_train[], int x_test[][192], int y_test[]){
	FILE *fp = fopen("pp_tra.dat", "r");
	int i = 0, j = 0, element;
	char character;
	while((fscanf(fp, "%d", &element)) == 1){
		if(j!=193){
			if(j==192){
				y_train[i] = element;
				++i;
				j = 0;
			} else{
				x_train[i][j] = element;
				j++;
			}
		} 
	}
	fclose(fp);
	fp = fopen("pp_tes.dat", "r");
	i = 0, j = 0;
	while((fscanf(fp, "%d", &element)) == 1){
		if(j!=193){
			if(j==192){
				y_test[i] = element;
				++i;
				j = 0;
			} else{
				x_test[i][j] = element;
				j++;
			}
		} 
	}
	fclose(fp);
	//printf("%d\n", i);
}

float calcDistances(int x_train[][192], int i, int j){
	int sumSquare = 0;
	for(int k=0;k<192;++k){
		//printf("%d %d\n", x_train[i][k], x_train[j][k]);
		sumSquare += pow((x_train[i][k] - x_train[j][k]), 2);
	}
	return sqrt(sumSquare);
}
void merge(pair arr[], int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    /* create temp arrays */
    pair L[n1], R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2){
        if (L[i].distance <= R[j].distance){
            arr[k] = L[i];
            i++;
        }
        else{
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1){
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2){
        arr[k] = R[j];
        j++;
        k++;
    }
}
 
/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(pair arr[], int l, int r){
    if (l < r){
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;
 
        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
 
        merge(arr, l, m, r);
    }
}
void init(int label[]){
	for(int i=0;i<10;++i){
		label[i] = 0;
	}
}
int findMax(int label[]){
	int max = label[0], j=0;
	for(int i=1;i<10;++i){
		if(max < label[i]){
			max = label[i];
			j = i;
		}
	}
	return j;
}
//y_train[dist[l].train_index] != y_train[dist[l].validation_index]
//ist_fold = 2223
//2nd_fold = 2225
//3rd_fold = 2222
void calculateDistances(pair distance_fold1[][4449], pair distance_fold2[][4449], pair distance_fold3[][4449], int x_train[][192]){
	//calculating distances using ist fold as validation set
	int l=0, i;
	for(i=0;i<2223;++i){
		l = 0;
		for(int j=2223;j<6671;++j){
			distance_fold1[i][l].distance = calcDistances(x_train, i, j);
			distance_fold1[i][l].validation_index = i;
			distance_fold1[i][l++].train_index = j; 
		}
		mergeSort(distance_fold1[i], 0, l);
		printf("%d\n",i);
	}
	nRows_fold1 = i;
	nCols_fold1 = l;
	int k=0;
	//calculating distances using 2nd fold as validation set
	for(i=2223;i<4448;++i, ++k){ 	
			l=0;
			for(int j=0;j<2223;++j){
				distance_fold2[k][l].distance = calcDistances(x_train, i, j);
				distance_fold2[k][l].validation_index = i;
				distance_fold2[k][l++].train_index = j;
			}
			for(int j=4448;j<6671;++j){
				distance_fold2[k][l].distance = calcDistances(x_train, i, j);
				distance_fold2[k][l].validation_index = i;
				distance_fold2[k][l++].train_index = j;
			}
			mergeSort(distance_fold2[k], 0, l);
		printf("%d\n", k);
		}
		nRows_fold2 = k;
		nCols_fold2 = l;
	//calculating distances using 3rd fold as validation set
	k=0;
	for(i=4448;i<6670;++i, ++k){	//fold_3 2222 entries
			l=0;
			for(int j=0;j<2223;++j){
				distance_fold3[k][l].distance = calcDistances(x_train, i, j);
				distance_fold3[k][l].validation_index = i;
				distance_fold3[k][l++].train_index = j;
			}
			for(int j=2223;j<4448;++j){
				distance_fold3[k][l].distance = calcDistances(x_train, i, j);
				distance_fold3[k][l].validation_index = i;
				distance_fold3[k][l++].train_index = j;
			}
			mergeSort(distance_fold3[k], 0, l);
			printf("%d\n", k);
		}
		//printf("hello\n");
		nRows_fold3 = k;
		nCols_fold3 = l;
}
int findError(pair dist[], int y_train[], int k, int i){
	int index = 0, label[10], error = 0;
	init(label);
	//printf("\n\n");
	for(int l=0;l<k;++l){
		label[y_train[dist[l].train_index]] += 1;
	}
	index = findMax(label);
	if(index!=y_train[i]){
		error = 1;
	} 
	/*for(int l=0;l<10;++l){
		printf("%d ", label[l]);
	}
	printf("\n\n");*/
	return error;
}
int min(float error[]){
	float minimum = error[0];
	int j=0;
	for(int i=1;i<25;++i){
		if(error[i]<minimum){
			minimum = error[i];
			j = i;
		}
	}
	return j;
}
int findK(int x_train[][192], int y_train[], int nRows){
	pair dist[4449];
	int k, error_fold1 = 0, err, error_fold2 = 0, error_fold3 = 0, l = 0, err_index = 0, j, count;
	float error[25];
	for(int k=1;k<=25;++k){
		count = 0, err = 0;
		error_fold1 = 0, error_fold2 = 0, error_fold3 = 0;
		for(int i=0;i<nRows_fold1;++i){
			err = findError(distance_fold1[i], y_train, k, i);
			if(err == 0)
				count++;
			error_fold1 += err;//errork=1, fold=1
			err = 0;
			//printf("%d\n", j);
		}
		//printf("%d----------------------------------------------------\n\n", error_fold1);
		j=0;
		for(int i=nRows_fold1;i<4448;++i, ++j){ 	//fold_2 as validation set 2224 entries
			err = findError(distance_fold2[j], y_train, k, i);
			if(err == 0)
				count++;
			error_fold2 += err;//errork=1, fold=1if(error_fold2 == 0)
			err = 0;
			//printf("%d\n", j);
			
		}
		j=0;
		//printf("%d-----------------------------------------------------\n\n", error_fold2);
		for(int i=4448;i<6670;++i, ++j){	//fold_3 2222 entries
			err = findError(distance_fold3[j], y_train, k, i);
			if(err == 0)
				count++;
			error_fold3 += err;//errork=1, fold=1//printf("%d\n", j);
			err = 0;
		}
		//printf("%d--------------------------------------------------------\n\n", error_fold3);
		error[err_index++] = (float)(error_fold1+error_fold2+error_fold3)/3;
		//printf("%d\n", err_index);
		//printf("accuracy with k = %d is %f\n", k, ((count/6670.0) * 100));
	}
	printf("The errors with different values of k are as:\n");
	for(int i=0;i<err_index;++i){
		printf("Error with k = %d is: %f\n", i+1, error[i]);
	}
	printf("\n\n");
	k = min(error);
	return k+1;
}
//2223
//last will have 2224
pair test_distance[3333][6670];
int nRows_test = 0, nCols_test = 0;
float calcDistances_test(int x_test[][192], int x_train[][192], int i, int j){
	float sumSquare = 0;
	for(int k=0;k<192;++k){
		//printf("%d %d\n", x_train[i][k], x_train[j][k]);
		sumSquare += pow((x_train[j][k] - x_test[i][k]), 2);
	}
	return sqrt(sumSquare);
}
void calculate_test_distances(int x_test[][192], int x_train[][192], int nTest, int nTrain){
	int i, j;
	for(i=0;i<nTest;++i){
		for(j=0;j<nTrain;++j){
			test_distance[i][j].distance = calcDistances_test(x_test, x_train, i, j);
			test_distance[i][j].validation_index = i;
			test_distance[i][j].train_index = j;
		}
		mergeSort(test_distance[i], 0, j-1);
	}
	nRows_test = i;
	nCols_test = j;
}
int findError_test(pair dist[], int y_test[], int y_train[], int k, int i){
	int index = 0, label[10], error = 0;
	init(label);
	//printf("\n\n");
	for(int l=0;l<k;++l){
		label[y_train[dist[l].train_index]] += 1;
	}
	index = findMax(label);
	if(index!=y_test[i]){
		error = 1;
	} 
	/*for(int l=0;l<10;++l){
		printf("%d ", label[l]);
	}
	printf("\n\n");*/
	return error;
}
 void predict(int y_test[3333], int y_train[], int k, int n){
 	int err = 0, error = 0, count = 0;
 	for(int i=0;i<n;++i){
 		err = findError_test(test_distance[i], y_test, y_train, k, i);
			if(err == 0)
				count++;
			else
				error += 1;
			err = 0;
 	}
 	//printf("%d\n", count);
 	printf("Accuracy with the test set is: %f\n", (count/3333.0)*100);
 	printf("The number of misclassified examples are: %d\n", error);
}
int main(){
	int x_train[6670][192], y_train[6670], x_test[3333][192], y_test[3333];
	store(x_train, y_train, x_test, y_test); 
	printf("finding value of k:\n");
	calculateDistances(distance_fold1, distance_fold2, distance_fold3, x_train);
	/*for(int i=0;i<nRows_fold1;++i){
		for(int j=0;j<nCols_fold1;++j){
			printf("%f ", distance_fold1[i][j].distance);
		}
		printf("\n");
	}
	printf("-----------------------\n");
	for(int i=0;i<nRows_fold2;++i){
		for(int j=0;j<nCols_fold2;++j){
			printf("%f ", distance_fold2[i][j].distance);
		}
		printf("\n");
	}
	printf("-----------------------\n");
	for(int i=0;i<nRows_fold3;++i){
		for(int j=0;j<nCols_fold3;++j){
			printf("%f ", distance_fold3[i][j].distance);
		}
		printf("\n");
	}*/
	int k = findK(x_train, y_train, 6670);
	printf("The best value of k is: %d\n", k);
	printf("calculating test distances:\n");
	calculate_test_distances(x_test, x_train, 3333, 6670);
	printf("%d %d\n", nRows_test, nCols_test);
	/*for(int i=0;i<2;++i){
			for(int j=0;j<3;++j){
				printf("|||%f %d %d|||", test_distance[i][j].distance, test_distance[i][j].validation_index, test_distance[i][j].train_index);
			}
			printf("\n");
	}*/
	//for(int k=1;k<=25;++k)
	printf("predicting the test set labels:\n");
		predict(y_test, y_train, k, 3333);
	/*printf("\n\n%d\n", k);*/
	return 0;
}