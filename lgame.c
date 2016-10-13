#include<stdio.h>
#include"omp.h"
#include<stdlib.h>

// the strcuture of the field

typedef struct {
    int present;
    int past;
    //int life; //1 - is life 0 - is dead
} Field;
    int FieldWidth;
    int FieldHeight;
    //FieldWidth = 100;//atoi(argv[1]);
    //FieldHeight = 100;//atoi(argv[2]);
    //Field* mainField = (Field *)malloc((FieldWidth*FielHeight)*sizeof(Field));
    Field mainField[10][10];
    Field helpMassive[10][10];

void InitializeField(/*Field* mainField,*/ int width, int height){
    int i;
    int j;
    int k;
    i=0; j=0;
    srand(time(NULL));
   // #pragma omp parallel
   // {
   // #pragma omp parallel for

        for (i = 0;i < height;i++){
            for (j = 0;j < width; j++){
              //k = i*j;
              k=rand();
              //if (k%2 == 0){
                mainField[i][j].past = 1;
                mainField[i][j].present = 1;
              //}
              //else{
                mainField[i][j].past = 0;
                mainField[i][j].present = 0;
                helpMassive[i][j].past = 0;
                helpMassive[i][j].present = 0;
            //}
            }
        }
   // combination for ciclic structure
   mainField[2][2].present = 1;
   mainField[2][2].past =1;
   mainField[3][3].present = 1;
   mainField[3][3].past = 1;
   mainField[3][4].present = 1;
   mainField[3][4].past = 1;
   mainField[4][2].present = 1;
   mainField[4][2].past =1;
   mainField[4][3].present = 1;
   mainField[4][3].past = 1;
   // }
}
void showPicture(/*Field* mainField ,*/ int width, int height){
    int i;
    int j;
    for (i = 0; i < height; i++){
        for ( j = 0; j < width; j++){
            printf ("%d", mainField[i][j].present);
        }
        printf("\n");
    }
}

int LifeorDeath (Field a0,Field a1, Field a2, Field a3, Field a4, Field a5, Field a6, Field a7 , Field a8){
    int result = 0;
    int life = 0;
    int death = 0;
    if (a1.past == 1) life ++;
    //else death ++;
    if (a2.past == 1) life ++;
    //else death ++;
    if (a3.past == 1) life ++;
    //else death ++;
    if (a4.past == 1) life ++;
    //else death ++;
    if (a5.past == 1) life ++;
    //else death ++;
    if (a6.past == 1) life ++;
    //else death ++;
    if (a7.past == 1) life ++;
    //else death ++;
    if (a8.past == 1) life ++;

    if (a0.present == 0){
        if (life == 3)
            result = 1;
        return result;
    }
    if (a0.present == 1){
        if ((life == 2) || (life == 3))
            {result = 1; return result;}
        else result = 0;
    }
return result;
}

void nextStepforMiddle( /*Field* mainField ,*/ int width, int height){
    int i;
    int j;
    int k;
  #pragma omp parallel
    {
    #pragma omp parallel for

        for (i = 1; i < height - 1; i++)
            for ( j = 1; j < width - 1; j++){
            mainField[i][j].present = LifeorDeath(mainField[i][j],mainField[i-1][j-1],mainField[i-1][j], mainField[i-1][j+1],mainField[i][j-1], mainField[i][j+1],mainField[i+1][j-1], mainField[i+1][j], mainField[i+1][j+1]);
            }
    }
}

void nextStepforBorder( /*Field* mainField ,*/ int width, int height){
    int i;
    int j;
    int k;
    // up and down borders
    #pragma omp parallel
    {
    #pragma omp parallel for

    for (i = 1; i < height - 1; i++){
        mainField[0][i].present = LifeorDeath(mainField[0][i],mainField[0][i-1],mainField[0][i+1],mainField[1][i],mainField[1][i-1],mainField[1][i+1],mainField[width-1][i],mainField[width-1][i-1],mainField[width - 1][i+1]);
        mainField[width - 1][i].present = LifeorDeath(mainField[width-1][i],mainField[width-1][i-1],mainField[width-1][i+1],mainField[width-2][i],mainField[width-2][i-1],mainField[width-2][i+1],mainField[0][i],mainField[0][i-1],mainField[0][i+1]);
    }
    }
    // left and right borders
    #pragma omp parallel
    {
    #pragma omp parallel for

    for (i = 1; i < width - 1; i++){
        mainField[i][0].present = LifeorDeath(mainField[i][0],mainField[i-1][0],mainField[i+1][0],mainField[i][1],mainField[i-1][1],mainField[i+1][1],mainField[i][height-1],mainField[i-1][height-1],mainField[i+1][height-1]);
        mainField[i][height - 1].present = LifeorDeath(mainField[i][height - 1],mainField[i-1][height - 1],mainField[i+1][height - 1],mainField[i][height-2],mainField[i-1][height-2],mainField[i+1][height-2],mainField[i][0],mainField[i-1][0],mainField[i+1][0]);
    }
    }
    // corners
    // left up
    mainField[0][0].present = LifeorDeath(mainField[0][0],mainField[0][1],mainField[0][height-1],mainField[1][0],mainField[1][1],mainField[1][height-1],mainField[width-1][0],mainField[width-1][1],mainField[width-1][height-1]);
    // right up
    mainField[0][height-1].present = LifeorDeath(mainField[0][height-1],mainField[0][height-2],mainField[0][0],mainField[1][0],mainField[1][height-1],mainField[1][height-2],mainField[width-1][height-1],mainField[width-1][height-2],mainField[width-1][0]);
    // left down
    mainField[width-1][0].present = LifeorDeath(mainField[width-1][0],mainField[width-1][1],mainField[width-1][height-1],mainField[width-2][0],mainField[width-2][1],mainField[width-2][height-1],mainField[0][0],mainField[0][1],mainField[0][height-1]);
    // right down
    mainField[width-1][height-1].present = LifeorDeath(mainField[width-1][height-1],mainField[width-1][height-2],mainField[width-1][0],mainField[width-2][0],mainField[width-2][height-1],mainField[width-2][height-2],mainField[0][height-1],mainField[0][height-2],mainField[0][0]);
}
void ForEqualPastAndPresent(/*Field* mainField,*/ int width, int height){
  int i;
  int j;
 // #pragma omp parallel
  //{
  //#pragma  omp parallel for

    for(i=0; i<width;i++)
        for(j=0;j<height;j++)
            mainField[i][j].past = mainField[i][j].present;
 //}
}

int ForCyclicResearch (int width, int height){
  int i;
  int j;
  int result = 1;
  for (i=0; i<width ; i++)
        for(j=0;j<height; j++){
            if(mainField[i][j].present != helpMassive[i][j].present)
                result = 0;
        }
  return result;
  }

void addCicle (int width, int height){
  int i;
  int j;
  for (i=0; i<width; i++)
        for(j=0;j<height; j++){
        helpMassive[i][j].present = mainField[i][j].past;
        }
  }

int isTheEnd (/*Field* mainField,*/ int width, int height){
  int i;
  int j;
  int result = 1;
    for (i=0; i<width ; i++)
        for(j=0;j<height; j++){
            if(mainField[i][j].past != mainField[i][j].present)
                result = 0;
        }
   return result;
  //}
}
void ForStart(/*Field* mainField,*/ int width, int height){
  nextStepforBorder(width,height);
  nextStepforMiddle(width,height);
  int flag = 0;
  int cicle = 0;
  addCicle(width,height);
  while ((isTheEnd(width,height) == 0) && (ForCyclicResearch(width,height) == 0)){
    printf("\n");
    showPicture(FieldWidth, FieldHeight);
    printf("\n");
    printf("\n");
    ForEqualPastAndPresent(width,height);
    nextStepforMiddle(width,height);
    nextStepforBorder(width,height);
  }
  showPicture(width,height);
}

int main(int argc, char ** argv){
    FieldWidth = 10;//atoi(argv[1]);
    FieldHeight = 10;//atoi(argv[2]);
    InitializeField(FieldWidth, FieldHeight);
    showPicture(FieldWidth, FieldHeight);
    printf("\n");
    printf("\n");
    ForStart(FieldWidth, FieldHeight);
return 0;
}
