#include"math_helper.h"
#include"Timer.h"
#include<fstream>
#include<stdio.h>
#include<string>
#include<vector>
#include<cmath>
#include<iostream>

using namespace std;
using std::string;
using std::vector;

void sigmoid(Vector &Vin, Vector &Vout){
  fori(0, Vin.n) Vout[i] = tanh(Vin[i]);
}

struct NetworkParameters{
  int N;           //Number of recurrent units
  int Nplastic;    //Number of plastic units
  float Anoise;    //Noise amplitude
  float pc;        //Sparsity parameter
  float scale;     //Scaling for the recurrent matrix
  float g;         //Synaptic strength scaling factor
  float delta;
  float tau;       //Time constant
};

struct SimulationParameters{
  int nloops;     
  int learn_skip; //Train every learn_skip steps during the window
  float dt;        //Integration time step

  float input_pulse_value;
  
  int start_pulse;
  int reset_duration;
  
  float start_train;
  float end_train;

  int nsteps;
  
  bool get_innate;
  bool saveX;
  bool saveZ;
  bool perturbation;
};

enum TRAINING_MODE {READOUT, RECURRENT, NONE};

class RRN{
  Matrix Wrec; //Wrec
  Vector Win; //WIn   Nxninputs
  Vector Wout; //Wout  noutputsxN
  Vector Xv, X;
  float Z;
  vector<vector<float>> target; //chaos or neuron XY data
  Matrix ZvsT, XvsT;
  NetworkParameters np;
  SimulationParameters sp;
  RNG rng;
  int N;
  void init(){
    //****Initialize recurrent matrix****//
    Matrix Wrec_mask(N,N);
    Wrec_mask.randf(rng);
    //Random connections with random weigths
    fori(0,N*N)
      if(Wrec_mask.data[i] <= np.pc)
	Wrec_mask.data[i] = 1;
      else 
	Wrec_mask.data[i] = 0;

    Wrec.randn(rng, 0.0f, 1.0f);
    Wrec *= np.scale;
    Wrec *= Wrec_mask;
    //No self connections
    fori(0,N) Wrec[i][i] = 0.0f;
    
    //Random input weigths
    Win.randn(rng, 0.0f, 1.0f);
    //Random initial output weigths
    Wout.randn(rng, 0.0f, 1.0f);
    Wout *= 1.0f/sqrt(N);

  }
public:
  RRN(vector<vector<float>> &target,
      NetworkParameters np,
      SimulationParameters sp):
    sp(sp), np(np),
    N(np.N),
    Xv(np.N), X(np.N),
    ZvsT(sp.nsteps, 1), XvsT(sp.nsteps, np.N),
    Wrec(np.N,np.N), Win(np.N), Wout(np.N),
    target(target)
  {
    srand(time(NULL));
    rng.seed(rand(), rand());
    init();
  };

  void update_params(NetworkParameters np,
		     SimulationParameters sp){
    this->np = np;
    this->sp = sp;
  }
  
  void train_network(TRAINING_MODE mode){
    //Input is always null, except during a brief window
    Vector input_pattern = Vector(sp.nsteps);
    float Input;
    
    input_pattern.fill_with(0.0f);
    
    fori(sp.start_pulse, sp.start_pulse+sp.reset_duration)
      input_pattern[i] = sp.input_pulse_value;
    
    if(mode==NONE && sp.perturbation){
      fori(sp.start_pulse+500, sp.start_pulse+500+10)
	input_pattern[i] = sp.input_pulse_value/10.0f;
    }
    
    bool training = false;
    
    //Auxiliar vectors
    Vector Xv_current(N);
    
    //    Matrix chaos(sp.nsteps, 2); //output storage
    
    ofstream out;
    ofstream out2;
    ofstream out3;
    ofstream out4;
    
    Timer tim; tim.tic();
    cerr<<sp.end_train<<" "<<sp.nsteps<<endl;
    float Xv_amp = np.Anoise*sqrt(sp.dt);
    cerr<<"Starting "<<(mode==READOUT?"readout":"recurrent")<<" training: "<<endl;
    for(int loop=0; loop<sp.nloops; loop++){
      if(sp.saveX){
	if(sp.get_innate) out3.open("Xtarget.dat");
	out2.open("X.dat");
      }

      cerr<<"\rLoop: "<<1+loop<<"/"<<sp.nloops;
      Xv.randf(rng, -1.0f, 1.0f);
      sigmoid(Xv, X);
      for(int step=0; step<sp.nsteps; step++){
	
	Input = input_pattern[step];

	//******INTEGRATION**********//
	if(Xv_amp!=0) Xv_current.randn(rng, 0.0f, Xv_amp);
	else          Xv_current.fill_with(0.0f);
	
	Xv_current += Wrec*X;

	if(Input != 0){
	  fori(0,N) Xv_current[i] += Win[i]*Input;
	}
	
	fori(0,N) Xv[i] += (sp.dt/np.tau)*(Xv_current[i]-Xv[i]);

	//***Rates****//
	sigmoid(Xv, X);
	
	if(sp.saveX){
	  out2<<step<<" ";
	  fori(0,N) out2<<X[i]<<" ";
	  out2<<endl;
	}

	Z = 0.0;
	//*****Output***//
	if(mode!=RECURRENT)
	  fori(0,N) Z += Wout[i]*X[i];
	
	//***Training window***//
	if(step==sp.start_train) training = true;
	else if(step==sp.end_train) training = false;

	//*************TRAINING*************//
	if(training && step%sp.learn_skip==0 ){
	  switch(mode){
	  case READOUT:   train_readout_step(step);   break;
	  case RECURRENT: train_recurrent_step(step); break;
	  case NONE:                                  break;
	  }
	}//ENDIF TRAIN
	if(sp.get_innate)
	  std::copy(X.data, X.data+N, XvsT[step]); 
	if(sp.saveX){
	  out3<<step<<" ";
	  fori(0,N) out3<<X[i]<<" ";
	  out3<<endl;
	}
	  //	}

	if(sp.saveZ){
	  ZvsT[step][0] = Z;
	}

      }//ENDFOR STEPS 
      if(sp.saveZ){
	out.open("output.xy");
	fori(0, sp.nsteps){
	  out<<i<<" ";
	  out<<ZvsT[i][0]<<"\n ";
	  
	}
	out.close();
      }
      if(sp.saveX){
	out2.close();
	out3.close();
      }
    }//ENDFOR NLOOPS
    cerr<<" Elapsed time: "<<tim.toc()<<"s"<<endl;
  }


  void save(){
    ofstream out("SaveFile.dat");
    out<<N<<"\n";

    fori(0,N){
      forj(0,N) out<<Wrec[i][j]<<" ";
      out<<"\n";
    }
    
    fori(0,N) out<<Wout[i]<<"\n";
    fori(0,N) out<<Win[i]<<"\n";
    fori(0,target[0].size()) out<<target[0][i]<<"\n";
  }
  void load(const char *fileName){
    ifstream in(fileName);
    int Nnew;
    in>>Nnew;
    if(Nnew!=N){
      cerr<<"Please create a network with the same number of neurons as in "<<fileName<<"!!";
      exit(1);
    }
    fori(0,N) forj(0,N) in>>Wrec[i][j];
    
    fori(0,N) in>>Wout[i];
    fori(0,N) in>>Win[i];
    target[0].clear();
    float t1;
    while(in){
      in>>t1;
      target[0].push_back(t1);
    }
    

  }
private:
  void train_readout_step(int step){
    static bool initialized = false;
    static Matrix Pij(N,N);
    static Vector temp(N);
    if(!initialized){
      //Correlation matrix
      Pij.eye(1.0f/np.delta);
      initialized = true;
    }
    
    //**For the two outputs***//
    // P = P- (P·X (P·X)^T)/(1+X'·P·X)
    // P·X (P·X)^T -->> NxN matrix from two Nx1 vectors
    float den = 0.0f; //denominator
    
    temp = Pij*X; //P·X	    

    den = 1.0f + temp*X; //1+X'·P·X
	    
    //***Update Pij****//
    fori(0,N) 
      forj(0,N)
      Pij[i][j] -= temp[i]*temp[j]/den;

    //***Estimate error****//
    float error = Z - target[0][step-sp.start_train];
    //***Apply rule**// //Wout_i -= e*Pij*r
    fori(0,N)
      Wout[i] -= temp[i]*error/den; 

  }
  
  void train_recurrent_step(int step){
    static bool initialized = false;
    int Nplastic = np.Nplastic;
    static vector<vector<int>> pre_plastic_units(Nplastic);
    static vector<Matrix> Pij;
    static vector<int> Npre_plastic(Nplastic,0);
    //    static vector<float> temp(N);
    if(!initialized){
      fori(0,Nplastic){
	forj(0,N) if(Wrec[i][j]!=0) pre_plastic_units[i].push_back(j);
	Npre_plastic[i] = pre_plastic_units[i].size();
	Pij.push_back(Matrix(Npre_plastic[i], Npre_plastic[i]));
	Pij[i].eye(1.0/np.delta);
	
      }
      initialized = true;
    }
    int Npp;
    float temp[N];
    std::fill(temp, temp+N, 0.0f);
    //#pragma omp parallel for schedule(auto)
    for(int unit=0; unit<Nplastic; unit++){
      Npp = Npre_plastic[unit];
      
      
      float accum = 0.0f;
      fori(0, Npp){
	accum = 0.0f;
	forj(0, Npp){
	  accum += Pij[unit][i][j]*X[pre_plastic_units[unit][j]];
	}
	temp[i] = accum;
      }

      float den = 0.0f;
      fori(0,Npp) den += X[pre_plastic_units[unit][i]]*temp[i];
      den += 1.0f;

      fori(0,Npp) 
	forj(0,Npp)
	Pij[unit][i][j] -= temp[i]*temp[j]/den;
      
      float error;
      //      int ppunit;
      error = X[unit] - XvsT[step][unit];
      //      if(unit==0) cout<<error<<endl;
      fori(0,Npp){
	//	ppunit = pre_plastic_units[unit][i];
	Wrec[unit][pre_plastic_units[unit][i]] -= error*temp[i]/den;
      }
      
    }
    

    
  }

};

int main(int argc, char* argv[]){
  NetworkParameters np;
  SimulationParameters sp;
  sp.perturbation = false;
  np.Anoise = 0.0f;

  np.pc   =       0.1f;
  np.g          = 1.5f;
  np.delta      = 1.0f;
  np.tau        = 10.0f;
  np.N = 800;
  np.Nplastic = 480;


  if(argc==1){
    cerr<<"Missing target input!!"<<endl;
    exit(1);
  }
  bool train= true;
  bool save = true;
  char *loadFile = NULL;
  fori(0,argc){
    if(string(argv[i])==string("--notrain")) train = false;   
    if(string(argv[i])==string("--nosave")) save = false;
    if(string(argv[i])==string("--load")) loadFile = argv[i+1];
    if(string(argv[i])==string("--perturbation")) sp.perturbation = true;
    if(string(argv[i])==string("--noiseAmp")) np.Anoise = stod(argv[i+1],NULL);
    if(string(argv[i])==string("--pconnect")) np.pc = stod(argv[i+1],NULL);
  }

  np.scale      = np.g /sqrt(np.pc*np.N);

  int nlines = 0;
  vector<vector<float>> target(2);
  string line;
  std::ifstream in(argv[1]);
  while(std::getline(in, line)) ++nlines;
  in.close();

  target[0].resize(nlines);
  in.open(argv[1]);

  fori(0,nlines)  in>>target[0][i];

  



  sp.learn_skip = 2;
  sp.dt         = 1.0f;
  
  float start_pulse = 200;
  float reset_duration = 50;

  sp.input_pulse_value = 2.0f;
  
  sp.start_pulse = int(start_pulse/sp.dt+0.5);
  sp.reset_duration = int(reset_duration/sp.dt+0.5);
  
  sp.start_train = sp.start_pulse + sp.reset_duration;
  sp.end_train = sp.start_train + target[0].size();
  
  sp.nsteps = int((sp.end_train+1000)/sp.dt);
  sp.nloops    = 1;
  
  sp.get_innate = true;
  sp.saveX = true;
  sp.saveZ = true;


  RRN rrn(target, np, sp);
  if(loadFile){
    rrn.load(loadFile);
    cerr<<"Using network in "<<loadFile<<endl;
  }
  else   cerr<<"Using random network"<<endl;

  if(train){

    rrn.train_network(NONE);
  
    np.Anoise = 0.001f;
    sp.nloops = 20;
    sp.get_innate = false;
    sp.saveZ = false;
  
    rrn.update_params(np, sp);
  
    rrn.train_network(RECURRENT);
  
    np.Anoise = 0.0f;
    sp.nloops = 10;
    sp.saveX = true;
    sp.saveZ = true;  
    rrn.update_params(np, sp);

    rrn.train_network(READOUT);
    // rrn.load("SaveFile.dat");
    // rrn.train_network(NONE);
    sp.saveX = true;
    sp.nloops = 1;
    rrn.update_params(np, sp);
    rrn.train_network(NONE);

  }
  else{
    sp.nloops = 1;
    rrn.update_params(np, sp);
    rrn.train_network(NONE);
  }
  if(save)rrn.save();
  return 0;
}
