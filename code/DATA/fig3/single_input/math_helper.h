#ifndef MATH_HELPER_H
#define MATH_HELPER_H
#include<ostream>
#include<iostream>
#include<cmath>

#define fori(x,y) for(int i=x; i<y; i++)
#define forj(x,y) for(int j=x; j<y; j++)

#define RANDESP (rand()/(float)RAND_MAX)

#define sqrt2 1.41421356237310f

#define RANDOM_MAX 18446744073709551615u

/* Pseudorandom number generation Xorshift128plus*/
class RNG{
  uint64_t s[2]; /* PRNG state */
public:
  /* The PRNG state must be seeded so that it is not everywhere zero. */
  void seed(uint64_t s0, uint64_t s1){
  s[0] = s0;  s[1] = s1;  
  }
  RNG(){
    /* The PRNG state must be seeded so that it is not everywhere zero. */
    s[0] = 12679825035178159220u;
    s[1] = 15438657923749336752u;
  }
  /* 64-bit (pseudo)random integer */
  uint64_t nexti(void){
    uint64_t x = s[0];
    uint64_t const y = s[1];
    s[0] = y;
    x ^= x << 23; // a
    x ^= x >> 17; // b
    x ^= y ^ (y >> 26); // c
    s[1] = x;
    return x + y;
  }
  /* Random number from a uniform distribution */
  float uniform(float min, float max){
    return min + (nexti()/((double) RANDOM_MAX))*(max - min);
  }
  float nextf(){
    return uniform(0.0f, 1.0f);
  }
};

class Matrix{
public:
  float *data; //The data itself, stored aligned in memory
  int n, m; //size of the matrix
  //  Matrix(){}
  Matrix(int n, int m);
  Matrix(const Matrix &some_M);
  ~Matrix();
  void fill_with(float x);
  void randf(RNG &rng);
  void randf(RNG &rng, float a, float b);
  void randi(RNG &rng);
  void randn(RNG &rng, float mean, float var);
  void eye(float a);
  void freeCPU();
  void print();
  int size(){ return n*m;}
  
  void operator *=(float a){
    fori(0,n*m) data[i] *= a;
  }
  void operator *=(Matrix &a){
    fori(0,n*m) data[i] *= a.data[i];
  }
  void operator =(const Matrix &a){
    fori(0,n*m) data[i] = a.data[i];
  }
  void operator -=(Matrix &a){
    fori(0,n*m) data[i] -= a.data[i];
  }
  void operator +=(Matrix &a){
    fori(0,n*m) data[i] += a.data[i];
  }
	 
  float* operator [](const int &i){return &data[m*i];}
};

class Vector: public Matrix{
public:
  Vector(int N): Matrix(N,1){}
  Vector(const Vector &some_V): Matrix(some_V.n,1){}
  
  struct MVprod{
    Matrix *m;
    Vector *v;
  };
  struct VMprod{
    Matrix *m;
    Vector *v;
  };
  struct ProdSum{
    MVprod a;
    MVprod b;
  };

  struct PSVsum{
    ProdSum a;
    Vector *b;
  };
  struct PVsum{
    MVprod a;
    Vector *b;
  };
  struct VVsum{
    Vector *a;
    Vector *b;
  };

  void operator =(MVprod p){
    if(n!=p.m->n){
      std::cerr<<"Vector Matrix sizes dont match!"<<std::endl;
      std::cerr<<"Trying "<<n<<"x1 = "<<p.m->n<<"x"<<p.m->m<<" · 1x"<<p.v->n<<std::endl;
      exit(1);
    }
    float temp = 0.0;
    fori(0,n){
      temp = 0.0f;
      forj(0,p.v->n) temp += (*p.m)[i][j]*(*p.v)[j];
      data[i] = temp;
      }
  }
  void operator +=(MVprod p){
    if(n!=p.m->n){
      std::cerr<<"Vector Matrix sizes dont match!"<<std::endl;
      std::cerr<<"Trying "<<n<<"x1 = "<<p.m->n<<"x"<<p.m->m<<" · 1x"<<p.v->n<<std::endl;
      exit(1);
    }
    float temp;
    fori(0,n){
      temp = data[i];
      forj(0,p.v->n) temp += (*p.m)[i][j]*(*p.v)[j];
      data[i] = temp;
    }
  }
  void operator =(VMprod p){
    if(n!=p.m->m){
      std::cerr<<"Vector Matrix sizes dont match!"<<std::endl;
      std::cerr<<"Trying "<<n<<"x1 = "<<p.m->n<<"x"<<p.m->m<<" · 1x"<<p.v->n<<std::endl;
      exit(1);
    }
    float temp = 0.0;
    fori(0,n){
      temp = 0.0f;
      forj(0,p.v->n) temp += (*p.m)[j][i]*(*p.v)[j];
      data[i] = temp;
    }
  }
  void operator +=(VMprod p){
    if(n!=p.m->m){
      std::cerr<<"Vector Matrix sizes dont match!"<<std::endl;
      std::cerr<<"Trying "<<n<<"x1 = "<<p.m->n<<"x"<<p.m->m<<" · 1x"<<p.v->n<<std::endl;
      exit(1);
    }
    float temp;
    fori(0,n){
      temp = data[i];
      forj(0,p.v->n) temp += (*p.m)[j][i]*(*p.v)[j];
      data[i] = temp;
    }
  }

  void operator  =(PVsum p){*this = p.a;*this += *p.b;}
  void operator +=(PVsum p){*this += p.a;*this += *p.b;}

  void operator  =(ProdSum p){*this =p.a; *this += p.b;}
  void operator +=(ProdSum p){*this +=p.a; *this += p.b;}

  void operator  =(PSVsum p){*this =p.a; *this += *p.b;}
  void operator +=(PSVsum p){*this +=p.a; *this += *p.b;}

  void operator  =(Vector &p){
    if(n != p.n){std::cerr<<"Vector sizes dont match!"<<std::endl;exit(1);}
    fori(0,n) data[i] = p[i];
  }
  void operator +=(Vector &p){
    if(n != p.n){std::cerr<<"Vector sizes dont match!"<<std::endl;exit(1);}
    fori(0,n) data[i] += p[i];
  }

  void operator  =(VVsum p){*this =*p.a; *this += *p.b;}
  void operator +=(VVsum p){*this +=*p.a; *this += *p.b;}
  
  float operator *(Vector &b){
    float res = 0.0f;
    fori(0,n) res += data[i]*b[i];
    return res;
  }
  float& operator [](const int &i){return data[i];}
};



Vector::VVsum operator+(Vector &a, Vector &b);
Vector::PVsum operator+(Vector::MVprod a, Vector &b);
Vector::ProdSum operator+(Vector::MVprod a, Vector::MVprod b);
Vector::PSVsum operator+(Vector::ProdSum a, Vector &b);
Vector::MVprod  operator *(Matrix &a, Vector &b);
Vector::VMprod  operator *(Vector &b, Matrix &a);







struct float3{
  float x,y,z;
  float3 operator+ (float3 b){ return {x+b.x, y+b.y, z+b.z};}
  void operator+= (float3 b){ x+=b.x; y+=b.y; z+=b.z;}
  float3 operator- (float3 b){ return {x-b.x, y-b.y, z-b.z};}
  void operator-= (float3 b){ x-=b.x; y-=b.y; z-=b.z;}
  float3 operator* (float3 b){ return {x*b.x, y*b.y, z*b.z};}
  float3 operator/ (float3 b){ return {x/b.x, y/b.y, z/b.z};}

  bool   operator< (float b ){ return x<b && y<b && z<b; }

  template <typename T>
           float3 operator* (T b){ return {b*x, b*y, b*z};}
  template <typename T>
           float3 operator/ (T b){ return {x/b, y/b, z/b};}
  template <typename T>
           float3 operator+ (T b){ return {b+x, b+y, b+z};}
  template <typename T>
           float3 operator- (T b){ return {x-b, y-b, z-b};}

  float dot(float3 b){return x*b.x+y*b.y+z*b.z;}
  float norm(){ return x*x+y*y+z*z; }
  float max(){ return x>y?(x>z?x:z):(y>z?y:z);}
};

std::ostream &operator<<(std::ostream &os, float3 const &a);

float3 abs(float3 a);
float3 floor(float3 a);

float3 make_float3(float x, float y, float z);
float3 make_float3(float x);


//////////////FLOAT2//////////////////////////////////////////////////
struct float2{
  float x,y;
  float2 operator+ (float2 b){ return {x+b.x, y+b.y};}
  void operator+= (float2 b){ x+=b.x; y+=b.y;}
  float2 operator- (float2 b){ return {x-b.x, y-b.y};}
  void operator-= (float2 b){ x-=b.x; y-=b.y;}
  float2 operator* (float2 b){ return {x*b.x, y*b.y};}
  float2 operator/ (float2 b){ return {x/b.x, y/b.y};}

  bool   operator< (float b ){ return x<b && y<b ; }

  template <typename T>
           float2 operator* (T b){ return {b*x, b*y};}
  template <typename T>
           float2 operator/ (T b){ return {x/b, y/b};}
  template <typename T>
           float2 operator+ (T b){ return {b+x, b+y};}
  template <typename T>
           float2 operator- (T b){ return {x-b, y-b};}
  

  float dot(float2 b){return x*b.x+y*b.y;}
  float norm(){ return x*x+y*y; }
};

std::ostream &operator<<(std::ostream &os, float2 const &a);

float2 abs(float2 a);
float2 floor(float2 a);

float2 make_float2(float x, float y);
float2 make_float2(float x);

//////////////INT3//////////////////////////////////////////////////
struct int3{
  int x,y,z;
  int3 operator+ (int3 b){ return {x+b.x, y+b.y, z+b.z};}
  void operator+= (int3 b){ x+=b.x; y+=b.y; z+=b.z;}
  int3 operator- (int3 b){ return {x-b.x, y-b.y, z-b.z};}
  void operator-= (int3 b){ x-=b.x; y-=b.y; z-=b.z;}
  int3 operator* (int3 b){ return {x*b.x, y*b.y, z*b.z};}
  int3 operator/ (int3 b){ return {x/b.x, y/b.y, z/b.z};}

  bool   operator< (int b ){ return x<b && y<b && z<b; }

  template <typename T>
           int3 operator* (T b){ return {b*x, b*y, b*z};}
  template <typename T>
           int3 operator/ (T b){ return {x/b, y/b, z/b};}
  template <typename T>
           int3 operator+ (T b){ return {b+x, b+y, b+z};}
  template <typename T>
           int3 operator- (T b){ return {x-b, y-b, z-b};}

  int dot(int3 b){return x*b.x+y*b.y+z*b.z;}
  int norm(){ return x*x+y*y+z*z; }
  int max(){ return x>y?(x>z?x:z):(y>z?y:z);}
  operator float3(){return {(float)x, (float)y, (float)z};}
};

std::ostream &operator<<(std::ostream &os, int3 const &a);

int3 abs(int3 a);

int3 make_int3(int x, int y, int z);
int3 make_int3(int x);

//////////////INT2//////////////////////////////////////////////////
struct int2{
  int x,y;
  int2 operator+ (int2 b){ return {x+b.x, y+b.y};}
  void operator+= (int2 b){ x+=b.x; y+=b.y;}
  int2 operator- (int2 b){ return {x-b.x, y-b.y};}
  void operator-= (int2 b){ x-=b.x; y-=b.y;}
  int2 operator* (int2 b){ return {x*b.x, y*b.y};}
  int2 operator/ (int2 b){ return {x/b.x, y/b.y};}

  bool   operator< (int b ){ return x<b && y<b ; }

  template <typename T>
           int2 operator* (T b){ return {b*x, b*y};}
  template <typename T>
           int2 operator/ (T b){ return {x/b, y/b};}
  template <typename T>
           int2 operator+ (T b){ return {b+x, b+y};}
  template <typename T>
           int2 operator- (T b){ return {x-b, y-b};}

  int dot(int2 b){return x*b.x+y*b.y;}
  int norm(){ return x*x+y*y; }
};

std::ostream &operator<<(std::ostream &os, int2 const &a);

int2 abs(int2 a);

int2 make_int2(int x, int y);
int2 make_int2(int x);



#endif
