#include"math_helper.h"

#include<stdlib.h>

using std::abs;
using std::floor;



///////////////MATRIX///////////////////

Matrix::Matrix(int n, int m){
  this->n = n;
  this->m = m;
  data = new float[n*m];//(float *)malloc(sizeof(float)*n*m);
}

Matrix::Matrix(const Matrix &some_m){
  this->n = some_m.n;
  this->m = some_m.m;
  data = new float[n*m];//(float *)malloc(sizeof(float)*n*m);
  *this = some_m;
}

Matrix::~Matrix(){
  this->freeCPU();
}
  
void Matrix::fill_with(float x){
  std::fill(data, data+n*m, x);
}
void Matrix::randf(RNG &rng){
  fori(0,n*m) data[i] = RANDESP;
}
void Matrix::randf(RNG &rng, float a, float b){
  fori(0,n*m) data[i] = a + RANDESP*(b - a);
}
void Matrix::randi(RNG &rng){
  fori(0,n*m) data[i] = rand();
}
void Matrix::randn(RNG &rng, float mean, float var){
  fori(0,n*m){
      float u1, u2, s0 = 2;
      while(s0 >= 1)
	{
	  u1 = RANDESP*2.0f-1.0f;
	  u2 = RANDESP*2.0f-1.0f;
	  s0 = u1*u1 + u2*u2;
	}
      data[i] = mean + var*u1*sqrt(-2*logf(s0)/s0);
  }
}
void Matrix::eye(float a = 1.0f){
  fill_with(0.0f);
  fori(0,n) data[m*i+i] = a;
}
void Matrix::freeCPU(){delete[] data;}
void Matrix::print(){
  fori(0,n){
    forj(0,m) 
      printf("%9.2e ",data[m*i+j]);
      printf("\n");
  }
  printf("\n");
}
///////////////////////////VECTOR///////////////////////////////////////








////////////////////////MATRIX/VECTOR OPERATIONS////////////////////////




Vector::VVsum operator+(Vector &a, Vector &b){return {&a, &b};}
Vector::PVsum operator+(Vector::MVprod a, Vector &b){return {a, &b};}
Vector::ProdSum operator+(Vector::MVprod a, Vector::MVprod b){return {a,b};}
Vector::PSVsum operator+(Vector::ProdSum a, Vector &b){return{a,&b};}
Vector::MVprod  operator *(Matrix &a, Vector &b){return {&a, &b};}
Vector::VMprod  operator *(Vector &b, Matrix &a){return {&a, &b};}






/////////////////////FLOAT3/////////////////////////////////
std::ostream &operator<<(std::ostream &os, float3 const &a){
  return os<<a.x<<" "<<a.y<<" "<<a.z;
}
float3 abs(float3 a){  return {abs(a.x), abs(a.y), abs(a.z)};}
float3 floor(float3 a){ return {floor(a.x), floor(a.y), floor(a.z)};}
float3 make_float3(float x, float y, float z){ return {x,y,z};}
float3 make_float3(float x){ return {x,x,x};}
//////////////FLOAT2//////////////////////////////////////////////////
std::ostream &operator<<(std::ostream &os, float2 const &a){
  return os<<a.x<<" "<<a.y;
}
float2 abs(float2 a){  return {abs(a.x), abs(a.y)};}
float2 floor(float2 a){ return {floor(a.x), floor(a.y)};}
float2 make_float2(float x, float y){ return {x,y};}
float2 make_float2(float x){ return {x,x};}

//////////////INT3//////////////////////////////////////////////////
std::ostream &operator<<(std::ostream &os, int3 const &a){
  return os<<a.x<<" "<<a.y<<" "<<a.z;
}
int3 abs(int3 a){ return {abs(a.x), abs(a.y), abs(a.z)};}
int3 make_int3(int x, int y, int z){ return {x,y,z};}
int3 make_int3(int x){return {x, x, x};}
//////////////INT2//////////////////////////////////////////////////
std::ostream &operator<<(std::ostream &os, int2 const &a){
  return os<<a.x<<" "<<a.y;
}
int2 abs(int2 a){  return {abs(a.x), abs(a.y)};}
int2 make_int2(int x, int y){ return {x,y};}
int2 make_int2(int x){ return {x,x};}


