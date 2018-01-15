
#include<stdlib.h>
#include<fstream>
#include<cmath>
#include<iostream>
#include<string.h>

#define fori(x,y)   for(int i = x; i<y; i++)
#define forj(x,y)   for(int j = x; j<y; j++)

using namespace std;

int main(int argc, char *argv[]){
  //  ifstream in(argv[1]);

  int Nt  =10;

  ifstream in[Nt];

  if(argc>1){
    fori(0,Nt){
      in[i].open( (string("non-trained/trial/X")+to_string(i+1)+string(".dat")).c_str());
    }
  }
  else{
    fori(0,Nt){
      in[i].open( (string("trained/trial/X")+to_string(i+1)+string(".dat")).c_str());
    }

  }


  int N = 800;
  float X[Nt][N];
  float mean[N];
  float var[N];
  float meanvar;
  int step;
  while(in[0]){

    fori(0,N) var[i] = mean[i] = 0.0f;

    fori(0,Nt) in[i]>>step;

    forj(0,N){
      fori(0,Nt){
	in[i]>>X[i][j];
	mean[j] += X[i][j];
      }

    }

    fori(0,N) mean[i]/=(float)Nt;
  

    fori(0,Nt){
      forj(0,N) var[j] += pow(X[i][j]-mean[j],2);
    }

    fori(0,N) var[i]/=(float)Nt;
  
    meanvar = 0.0f;
    fori(0,N) meanvar += var[i];
    cout<<step<<" "<<meanvar/(float)N<<endl;
  }  
  return 0;
}
