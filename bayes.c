#include<stdio.h>
#include<math.h>
/*GLOBAL*/
int trainX[6670][192];
int trainY[6670][1];
int testX[3333][192];
int testY[3333][1];
int countTrain[5][192];//Count of each value for each feature
float classProb[10];
float probability[192][5][10];
int occur[192][5][10];

float testclassP[10];
///////////////////////////////////////////////
void readTrain(){
  int i,j;
  FILE *f=fopen("pp_tra.dat","r");
  for(i=0;i<6670;i++){
    for(j=0;j<193;j++){
      if(j==192){
        fscanf(f,"%d",&trainY[i][0]);
      //  printf("%d ",trainY[i][0]);
      }
      else{
        fscanf(f,"%d",&trainX[i][j]);
      }
    }
  }
  fclose(f);
}
void readTest(){
  int i,j;
  FILE *f=fopen("pp_tes.dat","r");
  for(i=0;i<3333;i++){
    for(j=0;j<193;j++){
      if(j==192){
        fscanf(f,"%d",&testY[i][0]);
      }
      else{
        fscanf(f,"%d",&testX[i][j]);
      }
    }
  }
  fclose(f);
}
///Precompute using 3d array 192,i,j  for each of 192 features
void count(){
  int j,k;
    for(j=0;j<192;j++){
      int c0=0,c1=0,c2=0,c3=0,c4=0;
      for(k=0;k<6670;k++){
        if(trainX[k][j]==0){c0++;}
        else if(trainX[k][j]==1){c1++;}
        else if(trainX[k][j]==2){c2++;}
        else if(trainX[k][j]==3){c3++;}
        else if(trainX[k][j]==4){c4++;}
      }
      countTrain[0][j]=c0;
      countTrain[1][j]=c1;
      countTrain[2][j]=c2;
      countTrain[3][j]=c3;
      countTrain[4][j]=c4;
    }
}
void precompute(){
  int i,j,k;
  for(i=0;i<192;i++){
    int c[10];

    for(j=0;j<5;j++){
        for(int y=0;y<10;y++){c[y]=0;}
      //choose a particular number to count.
      for(k=0;k<6670;k++){
        if(trainX[k][i]==j){
          c[trainY[k][0]]+=1;
        }
      }
      for(k=0;k<10;k++){
        occur[i][j][k]=c[k];
      }
    }
  }


  for(i=0;i<192;i++){
    for(j=0;j<5;j++){
      for(k=0;k<10;k++){
        if(countTrain[j][i]!=0){
        probability[i][j][k]=(float)((float)occur[i][j][k]*1.0/countTrain[j][i]);
      }
      else{
        probability[i][j][k]=0;
      }
        //printf("%d %d %f\n",occur[i][j][k],countTrain[j][i],probability[i][j][k]);
      }
    }
  }
}

void classProbability(){
  int c[10];
  for(int i=0;i<6670;i++){
    c[trainY[i][0]]++;
  }
  for(int i=0;i<10;i++){
    classProb[i]=(float)c[i]*1.0/6670;
  }
}

void test(){
  int i,j,k,l;
  int cun=0;
  for(i=0;i<3333;i++){

    for(j=0;j<10;j++){
      //choose a class
      float pro=0;
      for(k=0;k<192;k++){
        for(l=0;l<5;l++){
          if(testX[i][k]==l){
            //if(probability[k][l][j]!=0){
              pro=pro+log(probability[k][l][j]);
              //printf("*%d %f %f\n",i,probability[k][l][j],pro);
            //}
          }
        }
        //break;//Values becoming too small here
      }
      testclassP[j]=pro;
    }

    /*float sum=0;
    for(j=0;j<10;j++){sum+=testclassP[j];}
    //printf("sum =%f\n",sum);
    for(j=0;j<10;j++){testclassP[j]=testclassP[j]/sum;}

     sum=0;
    for(j=0;j<10;j++){sum+=testclassP[j];}
    //printf("sum =%f\n",sum);*/
    float max=-9999999;
    int m=0;
    for(j=0;j<10;j++){
      if(testclassP[j]>max){
        max=testclassP[j];
        m=j;
      }
    }
    printf("%d, %d\n",m,testY[i][0]);
    if(m==testY[i][0]){cun++;}

  }
  printf("%d\n",cun);
printf("Accuracy: %f \n",(float)(cun*100.0/3333));

}
int main(){
  readTrain();
  count();
  precompute();
 classProbability();
  printf("test");
  /*for(int i=0;i<192;i++){
    for(int j=0;j<5;j++){
      for(int k=0;k<10;k++){
        printf("%f ",probability[i][j][k]);
      }
      printf("\n");
    }
  }*/
  /*for(int i=0;i<5;i++){
    for(int j=0;j<192;j++){
      printf("%d ",countTrain[i][j]);
    }
    printf("\n\n");
  }*/
////////////////////////////////////////////////////////////////////
//test

readTest();
test();

  return 0;
}
