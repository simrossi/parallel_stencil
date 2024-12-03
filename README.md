# Parallel stencil computations

Parallel implementation of stencil computations using OpenMPI for distributed computing.

## Description

This repository contains the project for the UniTN HPC course.

## Getting Started

### Dependencies

* OpenMPI
* OpenMP (optional)

### Installing

Clone the repository and compile with Make
```
git clone https://github.com/simrossi/parallel_stencil
cd parallel_stencil
make
```

### Executing program

The program processes an input file containing a matrix and generates an output file with the resulting matrix. The file format consists of two rows: the first row specifies the dimensions, and the second row contains the continuous data.

```
./stencil -i <input_file> -o <output_file> [-l <log_file>]
```
To change the stencil and the operation to perform modify the arguments provided to `init_stencil()` in `src/main.c`.
A list of different stencils can be found in `src/patterns.h` while all different operations are contained in `src/operations.h`.

### Python scripts

The folder `scripts/` contains some python scripts that can be used to perform various operations.
One of them is `image.py` which allows to apply the stencil computation to a provided image.

```
./image.py <stencil_binary> <image> <output_directory>
```

The command above will save the resulting image in the specified `output_directory`.

### Expand program

The program can be expanded by adding new stencils and new operations:
* To add a new stencil create a funcion declaration in `src/patterns.h` and its definition in `src/patterns.c`.
* To add a new operation create a function declaration in `src/operations.h` and its definition in `src/operations.c`.

You can guide yourself by looking at already existing stencils and operations.
Don't forget to apply your new changes by running `make clean && make`.

## Authors

Contributors names and contact info

Simone Rossi: [@simrossi](https://github.com/simrossi)
Dorijan Di Zepp: [@najirod02](https://github.com/najirod02)

## License

This project is licensed under the GPLv3 License - see the LICENSE file for details

## Acknowledgments

Inspiration, code snippets, etc.
* [example](https://)
