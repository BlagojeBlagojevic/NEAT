#define NUMBER_OF_SPICES 1000
#define LR 0.1
#define MUTATION_RATE 0.9
#define NUMBER_OF_LAYER 3
#define NUMBER_OF_NEURON 10
#define NINPUTS 2
#define NOUT 1
#define NEAT_IMPLEMETATION
#define MATRIX_ACTIVATION_DEFAULT_DISABLE
#define MATRIX_ACTIVATION_RELU
#include "neat.h"
#include<math.h>
int main() {

	static NEAT n[NUMBER_OF_SPICES];  //MACRO EXTENDED
	//system("pause");
	static NEAT n1[NUMBER_OF_SPICES];
	neat_alloc(n);
	neat_alloc(n1);

	srand(time(0));
	  
 float error = 0.0f;
  for(size_t counter = 0;counter < 100;counter++){
  	for(size_t j = 0; j < NUMBER_OF_SPICES;j++){
  		 error = 0.0f;
  		for(size_t x = 0;x < 10;x++)
			for(size_t i = 0;i < 10;i++){
  			float out = (float)x/3.0f+(float)i/2.0f;
  			n[j].input.elem[0] = (float)i;
  			n[j].input.elem[1] = (float)x;
  			neat_forward(&n[j],1);
  			error+=(out - n[j].out[NUMBER_OF_LAYER - 1].elem[0])*(out - n[j].out[NUMBER_OF_LAYER - 1].elem[0]);
				//error+=fabs(out - n[j].out[NUMBER_OF_LAYER - 1].elem[0]);
			}
		n[j].fitnes = sqrt(error) / 90;
		error = 0.0f;
		}
		neat_crossover(n,n1);
		printf("best fit GEN %d is %f\n",counter,n[0].fitnes);
	}
	while(1){
		printf("Input value(x, y):\t");
		float x,y;
		scanf("%f %f",&x,&y);
		//system("pause");
		for(size_t k = 0;k < NUMBER_OF_SPICES;k++){
		n[k].input.elem[0] = x;
		n[k].input.elem[1] = y;
		}
			//n[k].input.elem[0] = x;
		//system("pause");
		neat_forward(&n[0],1);
		matrix_print_out(n[0].out[NUMBER_OF_LAYER - 1],"OUT ",NOUT-1);
		printf("\ncoreect(%f/3.0f+%f/2.0f) = %f\n",x,y,x/3.0f+y/2.0f);
		//neat_print(&n[0]);
	}
}
