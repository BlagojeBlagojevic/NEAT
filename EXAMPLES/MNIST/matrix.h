#ifndef MATRIX_H
#define MATRIX_H
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdint.h>
#include<string.h>

#ifndef MATRIX_CALLOC
#include<stdlib.h>
#define MATRIX_CALLOC calloc
#endif

#ifndef MATRIX_ASSERT
#include<assert.h>
#define MATRIX_ASSERT assert
#endif

#define MATRIX_SHIFT(m,y,x) (m).elem[(y)*(m).cols + (x)]

#ifndef TYPE
#define TYPE float
#endif

#ifndef MATRIX_ACTIVATION_DEFAULT_DISABLE
#define MATRIX_ACTIVATION_SIGMOID
#endif

#ifndef T
#define T 1
#endif


typedef struct {
	size_t rows;    //NUM OF   ROWS
	size_t cols;    //NUM OF   COLS
	size_t stride;  //WHER TO  SPLIT
	TYPE  *__restrict__ elem;    //ELEMETS  IN
	} Mat;

//UTILITY FUNCTION
extern inline float rand_float(void);
extern inline float sigmoid(float x);

//MATRIX OPERATION
Mat  matrix_alloc(size_t rows, size_t cols);
extern inline void matrix_free(Mat m);

extern inline void matrix_dot(Mat dest, Mat a, Mat b);
extern inline void matrix_sum(Mat dest, Mat a);
extern inline void matrix_dec(Mat dest, Mat a);
extern inline void matrix_mul_scalar(Mat dest, float scalar);
extern inline void matrix_add_scalar(Mat dest, float scalar);
extern inline void matrix_rand(Mat dest,float low, float high);
extern inline void matrix_activation(Mat m);
extern inline void matrix_activation_cols(Mat m,size_t num);
extern inline void matrix_feedforward(Mat *out,Mat input, Mat *weights, Mat *bias, size_t num_of_layer);
extern inline void matrix_reproduce(Mat a, Mat b);
extern inline void matrix_softmax(Mat m);
extern inline void matrix_copy(Mat dest, Mat src);
extern inline void matrix_convolution(Mat dest,Mat a, Mat kernel);

extern inline void matrix_print(Mat m,const char *name);
extern inline void matrix_print_out(Mat m, const char *name, size_t out);
#define MATRIX_PRINT(m) matrix_print((m), #m)

#ifndef LR
#define LR 0.6
#endif

#ifndef MUTATION_RATE
#define MUTATION_RATE 0.9
#endif


extern inline void matrix_mutation(Mat m);
extern inline void matrix_mutation_enable(Mat m, float DM);
extern inline void matrix_mutation_disable(Mat m,float EM);

#endif  //MATRIX_H


#ifdef MATRIX_IMPLEMETATION
extern inline float rand_float(void) {
	return (float)rand() / (float)RAND_MAX;
	}

Mat matrix_alloc(size_t rows, size_t cols) {
	Mat m;
	m.cols = cols;
	m.rows = rows;
	m.elem = (TYPE *)MATRIX_CALLOC((rows)*(cols)+1,sizeof(*m.elem));    //(*M.elem) derefrence Changing type of element
	MATRIX_ASSERT(m.elem!=NULL);
	return m;
	}

extern inline void matrix_free(Mat m) {

	free(m.elem);
	}
#define OPTIM_DOT
#ifndef OPTIM_DOT
extern inline void matrix_dot(Mat dest, Mat a, Mat b) { //a[1x2] b[2x3] c[1x3]
	MATRIX_ASSERT(a.cols == b.rows);
	MATRIX_ASSERT(dest.rows == a.rows);
	MATRIX_ASSERT(dest.cols == b.cols);
	for(size_t y = 0; y < dest.rows; y++) {
		for(size_t x = 0; x < dest.cols; x++) {
			MATRIX_SHIFT(dest,y,x) = 0;
			for(size_t i = 0; i < a.cols; i++) {
				MATRIX_SHIFT(dest,y,x) += MATRIX_SHIFT(a,y,i) * MATRIX_SHIFT(b,i,x);
				}

			}
		}

	}
	
#endif
#ifdef OPTIM_DOT
extern inline void matrix_dot(Mat  dest, const Mat a, const Mat b) { //a[1x2] b[2x3] c[1x3]
	MATRIX_ASSERT(a.cols == b.rows);
	MATRIX_ASSERT(dest.rows == a.rows);
	MATRIX_ASSERT(dest.cols == b.cols);

	Mat b_T = matrix_alloc(b.cols, b.rows);
	//Transpose matrix b
	for(size_t y = 0; y < b_T.rows; y++) {
		for(size_t x = 0; x < b_T.cols; x++) {
			MATRIX_SHIFT(b_T, y, x) = MATRIX_SHIFT(b, x, y);
			}
		}
	//Multiplay matrix with transposed
	for(size_t y = 0; y < dest.rows; y++) {
		for(size_t x = 0; x < dest.cols; x++) {
			MATRIX_SHIFT(dest, y, x) = 0;
			for(size_t i = 0; i < a.cols; i++) {
				MATRIX_SHIFT(dest, y, x) += MATRIX_SHIFT(a, y, i) * MATRIX_SHIFT(b_T, x, i);
				}
			}
		}
	matrix_free(b_T);
}
#endif	
	
extern inline void matrix_sum(Mat dest, Mat a) {
	MATRIX_ASSERT(dest.cols == a.cols);
	MATRIX_ASSERT(dest.rows == a.rows);
	for(size_t y = 0; y < dest.rows; y++) {
		for(size_t x = 0; x < dest.cols; x++) {

			MATRIX_SHIFT(dest,y,x) += MATRIX_SHIFT(a,y,x);
			}
		}

	}
extern inline void matrix_dec(Mat dest, Mat a) {
	MATRIX_ASSERT(dest.cols == a.cols);
	MATRIX_ASSERT(dest.rows == a.rows);
	for(size_t y = 0; y < dest.rows; y++) {
		for(size_t x = 0; x < dest.cols; x++) {

			MATRIX_SHIFT(dest,y,x) -= MATRIX_SHIFT(a,y,x);
			}
		}

	}
extern inline void matrix_add_scalar(Mat dest, float scalar) {
	for(size_t y = 0; y < dest.rows; y++) {
		for(size_t x = 0; x < dest.cols; x++) {

			MATRIX_SHIFT(dest,y,x) += scalar;
			}
		}
	}

extern inline void matrix_mul_scalar(Mat dest, float scalar) {
	for(size_t y = 0; y < dest.rows; y++) {
		for(size_t x = 0; x < dest.cols; x++) {

			MATRIX_SHIFT(dest,y,x) *= scalar;
			}
		}

	}


extern inline void matrix_rand(Mat dest,float low, float high) {
	for(size_t y = 0; y < dest.rows; y++) {
		for(size_t x = 0; x < dest.cols; x++) {
			MATRIX_SHIFT(dest,y,x) = (TYPE)((rand_float()*(high - low)) + low);   //IN RANGE
			}
		}
	}

extern inline void matrix_print(Mat m, const char *name) {
	//system("pause");
	printf("\n\n______________________________________________________________________________\n");
	printf("%s",name);
	printf(" = \n[\n");

	for(size_t y = 0; y < m.rows; y++) {
		for(size_t x = 0; x < m.cols; x++) {
			printf(" %.2f ", (float)MATRIX_SHIFT(m,y,x));
			}
		printf("\n\n");
		}
	printf("]\n");
	printf("______________________________________________________________________________\n");

	}
extern inline void matrix_print_out(Mat m, const char *name, size_t out) {
	//system("pause");
	printf("\n\n______________________________________________________________________________\n");
	printf("%s",name);
	printf(" = \n[\n");

	for(size_t y = 0; y < m.rows; y++) {
		for(size_t x = 0; x < m.cols; x++) {
			printf(" %.2f ", (float)MATRIX_SHIFT(m,y,x));
			if(x==out) {
				break;
				}
			}
		printf("\n\n");
		}
	printf("]\n");
	printf("______________________________________________________________________________\n");

	}



extern inline float sigmoid(float x) {
	return 1.0f/(1 + expf(-x));
	}

extern inline void matrix_activation_cols(Mat m,size_t num) {

	for(size_t y = 0; y < m.rows; y++) {
		MATRIX_SHIFT(m,y,num) = sigmoid(MATRIX_SHIFT(m,y,num));
		}


	}

extern inline void matrix_activation(Mat m) {

	for(size_t y = 0; y < m.rows; y++)
		for(size_t x = 0; x < m.cols; x++) {
#ifdef MATRIX_ACTIVATION_SIGMOID
			MATRIX_SHIFT(m,y,x) = sigmoid(MATRIX_SHIFT(m,y,x));
#endif
#ifdef MATRIX_ACTIVATION_TANH
			MATRIX_SHIFT(m,y,x) = tanh(MATRIX_SHIFT(m,y,x));
#endif
#ifdef MATRIX_ACTIVATION_RELU
			if(MATRIX_SHIFT(m,y,x) < 0)  MATRIX_SHIFT(m,y,x)  = 0;
#endif
#ifdef MATRIX_ACTIVATION_LRELU
			if(MATRIX_SHIFT(m,y,x) < 0)  MATRIX_SHIFT(m,y,x)  = -0.01*MATRIX_SHIFT(m,y,x);
#endif
#ifdef MATRIX_ACTIVATION_BINARY
			if(MATRIX_SHIFT(m,y,x) > 0)
				MATRIX_SHIFT(m,y,x) = 1.0f;
			else
				MATRIX_SHIFT(m,y,x) = 0 ;
#endif

#ifdef MATRIX_ACTIVATION_SWISH
			MATRIX_SHIFT(m,y,x) *= sigmoid(MATRIX_SHIFT(m,y,x));
#endif

#ifdef MATRIX_ACTIVATION_RAND_VALUE
#ifdef MATRIX_ACTIVATION_RAND_RATE
			if(rand_float() < MATRIX_ACTIVATION_RAND_RATE)
				MATRIX_SHIFT(m,y,x) += 2*rand_float() * (float)MATRIX_ACTIVATION_RAND_VALUE - (float)MATRIX_ACTIVATION_RAND_VALUE;
#endif
#endif
			}


	}

//REWRITE IN ONE LOOP
extern inline void matrix_feedforward(Mat *out,Mat input, Mat *weights, Mat *bias,size_t num_of_layer) {
	//system("pause");
	matrix_dot(out[0],input,weights[0]);
	matrix_sum(out[0],bias[0]);
	matrix_activation(out[0]);
	//system("pause");
	for(size_t i = 1; i < num_of_layer; i++) {
		//system("pause");
		matrix_dot(out[i],out[i-1],weights[i]);
		//system("pause");
		matrix_sum(out[i],bias[i]);
		//system("pause");
		matrix_activation(out[i]);
		}
	}


//*/
extern inline void matrix_softmax(Mat m) {
	double sum = 0.0f;
	for(size_t y = 0; y < m.rows; y++) {
		for(size_t x = 0; x < m.cols; x++) {
			sum+=exp((double)MATRIX_SHIFT(m,y,x) / (double) T);
			}
		}
	//printf("sum = %f",sum);
	//system("pause");
	for(size_t y = 0; y < m.rows; y++) {
		for(size_t x = 0; x < m.cols; x++) {
			MATRIX_SHIFT(m,y,x) = (double)exp((double)MATRIX_SHIFT(m,y,x) / (double)T)/(double)sum;
			}
		}
	}
extern inline void matrix_copy(Mat dest, Mat src) {
	//MATRIX_ASSERT();
	for(size_t y = 0; y < dest.rows; y++) {
		for(size_t x = 0; x < dest.cols; x++) {
			MATRIX_SHIFT(dest,y,x) = MATRIX_SHIFT(src,y,x);
			}
		}
	}
extern inline void matrix_convolution(Mat dest, Mat a, Mat kernel){
    MATRIX_ASSERT(dest.cols <= a.cols);
    MATRIX_ASSERT(dest.rows <= a.rows);
    
    for (size_t y = 0; y < dest.cols; y++){
        for (size_t x = 0; x < dest.rows; x++){

            float sum = 0.0f;
            for (size_t i = 0; i < kernel.rows; i++)
            {
                for (size_t j = 0; j < kernel.cols; j++)
                {
                    sum+=MATRIX_SHIFT(a,y+i,x+j)*MATRIX_SHIFT(kernel,i,j);
                }
                
            }
            MATRIX_SHIFT(dest,y, x) = sum;
        }
        
    }

}

extern inline void matrix_mutation(Mat m) {
	for(size_t y = 0; y < m.rows; y++) {
		for(size_t x = 0; x < m.cols; x++) {
			if(rand_float() < MUTATION_RATE && MATRIX_SHIFT(m,y,x)!=0) {
				if(rand()%2==0) {
					MATRIX_SHIFT(m,y,x)+=LR*rand_float();
					}
				else {
					MATRIX_SHIFT(m,y,x)-=LR*rand_float();
					}

				}
			}
		}
	}


extern inline void matrix_mutation_disable(Mat m,float DM) {
	for(size_t y = 0; y < m.rows; y++) {
		for(size_t x = 0; x < m.cols; x++) {
			if(rand_float() < MUTATION_RATE) {
				if(rand_float() < DM) {

					MATRIX_SHIFT(m,y,x) = 0.0f;
					}
				}
			}
		}
	}


extern inline void matrix_mutation_enable(Mat m, float EM) {
	for(size_t y = 0; y < m.rows; y++) {
		for(size_t x = 0; x < m.cols; x++) {
			if(rand_float() < MUTATION_RATE) {
				if(rand_float() < EM && 	MATRIX_SHIFT(m,y,x) == 0) {
					MATRIX_SHIFT(m,y,x) = rand_float()*2.0f - 1.0f;
					}
				}
			}
		}
	}



extern inline void matrix_reproduce(Mat a, Mat b) {
	MATRIX_ASSERT(a.rows == b.rows);
	MATRIX_ASSERT(a.cols == b.cols);
	for(size_t y = 0; y < a.rows; y++) {
		for(size_t x = 0; x < a.cols; x++) {
			int desi = rand()%2;
			if(desi == 0) {
				MATRIX_SHIFT(a,y,x) = MATRIX_SHIFT(b,y,x);

				}
			}
		}
	matrix_mutation(a);
#ifdef DISABLE_MUTATION
	matrix_mutation_disable(a, DISABLE_MUTATION);
#endif
#ifdef ENABLE_MUTATION
	matrix_mutation_enable(a, ENABLE_MUTATION);
#endif

	}

#endif  //MATRIX_IMPLEMENTATION
