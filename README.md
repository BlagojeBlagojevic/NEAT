# NEAT - NeuroEvolution of Augmenting Topologies

NEAT (NeuroEvolution of Augmenting Topologies) is a method for evolving artificial neural networks with a focus on controlling the topologies of the networks developed by Kenneth O. Stanley and Risto Miikkulainen in 2002. This repository contains the implementation of NEAT along with matrix operations necessary for neural network computation.

## Matrix Library

The `matrix.h` header file provides a set of functions for matrix operations. It includes functionalities for matrix allocation, freeing, dot product, element-wise sum, random initialization, activation functions (including sigmoid, tanh, ReLU, and Leaky ReLU), softmax function, matrix printing, and mutation.

### Utility Functions
- `float rand_float(void)`: Generates a random float value between 0 and 1.
- `float sigmoid(float x)`: Calculates the sigmoid activation function.

### Matrix Operations
- `Mat matrix_alloc(size_t rows, size_t cols)`: Allocates memory for a matrix of specified size.
- `void matrix_free(Mat m)`: Frees memory allocated for a matrix.
- `void matrix_dot(Mat dest, Mat a, Mat b)`: Computes the dot product of two matrices.
- `void matrix_sum(Mat dest, Mat a)`: Computes the element-wise sum of two matrices.
- `void matrix_rand(Mat dest, float low, float high)`: Initializes a matrix with random values within a specified range.
- `void matrix_activation(Mat m)`: Applies activation functions to all elements of a matrix.
- `void matrix_activation_cols(Mat m, size_t num)`: Applies activation function to a specific column of a matrix.
- `void matrix_feedforward(Mat *out, Mat input, Mat *weights, Mat *bias, size_t num_of_layer)`: Performs feedforward computation for a neural network with multiple layers.
- `void matrix_softmax(Mat m)`: Applies softmax function to a matrix.
- `void matrix_copy(Mat dest, Mat src)`: Copies the contents of one matrix to another.
- `void matrix_print(Mat m, const char *name)`: Prints the contents of a matrix.
- `void matrix_print_out(Mat m, const char *name, size_t out)`: Prints the contents of a matrix with a specified number of output columns.
- `void matrix_mutation(Mat m)`: Applies mutation to a matrix for evolutionary algorithms.
- `void matrix_reproduce(Mat a, Mat b)`: Performs reproduction between two matrices for evolutionary algorithms.

## NEAT Library

The `neat.h` header file contains the definition and implementation of the NEAT algorithm using the matrix operations provided in `matrix.h`. NEAT is a method for evolving artificial neural networks with the ability to control the topology of the networks.

### NEAT Struct
- `NEAT`: Represents an individual neural network in the NEAT algorithm. It contains fields for fitness, input, weights, biases, output, and softmax output.

### NEAT Functions
- `void neat_alloc(NEAT *n)`: Allocates memory for NEAT structures.
- `void neat_free(NEAT *n)`: Frees memory allocated for NEAT structures.
- `void neat_rand(NEAT *n, float low, float high)`: Initializes NEAT structures with random weights and biases.
- `void neat_forward(NEAT *n, size_t Niter)`: Performs feedforward computation for NEAT structures.
- `void neat_print(NEAT *n)`: Prints the contents of NEAT structures.
- `void neat_mutation(NEAT *n)`: Applies mutation to NEAT structures.
- `void neat_reproduce(NEAT *n1, NEAT *n2)`: Performs reproduction between two NEAT structures.
- `void neat_crossover(NEAT *n)`: Performs crossover between NEAT structures.
- `void neat_save(NEAT *n,const char *name)`: Save neat.
- `void neat_load(NEAT *n,const char *name)`: Load neat.

## Configuration Constants
- `NUMBER_OF_SPICES`: Number of individual neural networks in the population.
- `NUMBER_OF_LAYER`: Number of layers in each neural network.
- `NUMBER_OF_NEURON`: Number of neurons in each hidden layer.
- `NINPUTS`: Number of input neurons.
- `NOUT`: Number of output neurons.
- `MUTATION_RATE`: Mutation rate for evolutionary algorithms.
- `NUMBER_OF_SPICES` How many agent are there  
- `LR`  Max change in mutation
- `MATRIX_ACTIVATION_DEFAULT_DISABLE` Disabling default activation function
- `MATRIX_ACTIVATION_SIGMOID`    Sets activation function to be sigmoid
- `MATRIX_ACTIVATION_RELU`    Sets activation function to be RELU
- `MATRIX_ACTIVATION_LRELU`    Sets activation function to be LRELU
- `MATRIX_ACTIVATION_TANH`    Sets activation function to be tanh
- `MATRIX_ACTIVATION_SIN`    Sets activation function to be sin
## Usage
1. Include the necessary header files (`neat.h`) in your project.
2. Configure the constants according to your requirements.
3. Utilize the provided functions to perform matrix operations and implement the NEAT algorithm for evolving neural networks.

## Example
```c
#define NUMBER_OF_SPICES 5000
#define LR 0.1
#define MUTATION_RATE 0.9
#define NUMBER_OF_LAYER 3
#define NUMBER_OF_NEURON 10
#define NINPUTS 2
#define NOUT 1
#define MATRIX_ACTIVATION_DEFAULT_DISABLE
#define MATRIX_ACTIVATION_SIGMOID
#define NEAT_IMPLEMETATION
#include "neat.h"


int main() {
    static NEAT population[NUMBER_OF_SPICES];
    
    neat_alloc(population);
    neat_rand(population, -1.0, 1.0);
    neat_forward(population, NUMBER_OF_SPICES);
    neat_print(population);
    
    // Further operations...
    
    neat_free(population);
    
    return 0;
}

```
## Example APP
![ScreenRecorderProject1-ezgif com-video-to-gif-converter](https://github.com/BlagojeBlagojevic/NEAT/assets/100707842/26657d0b-90ca-4eed-90c7-0e0ad93d0f3e)






## Useful links
1. https://en.wikipedia.org/wiki/Neuroevolution_of_augmenting_topologies
2. https://neat-python.readthedocs.io/en/latest/neat_overview.html
3. https://nn.cs.utexas.edu/downloads/papers/stanley.ec02.pdf

## Credits
This implementation is based on the NEAT algorithm developed by Kenneth O. Stanley and Risto Miikkulainen.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
