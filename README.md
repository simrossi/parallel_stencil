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
./stencil input output
```
To change the stencil modify the stencil variable in `src/main.c`.

### Expand program

The program can be expanded by adding new stencil operations and neighborhoods.
To add a new stencil operation:
* Add a new value to the `StencilOperation` enum in `src/types.h`.
* Implement the operation in `src/operations/`.
* Insert a new case to the `stencil.operation` switch case to address the new operation in `src/sequential.c` and `src/parallel.c`.

To add a new stencil neighborhood type:
* Add a new value to the `StencilType` enum in `src/types.h`.
* Implement the function to retrieve the neighborhood in `src/stencil.c`.
* Insert a new case to the `stencil.type` switch case to address the new type in `src/sequential.c` and `src/parallel.c`.

Apply the new changes with `make`.

## Authors

Contributors names and contact info

Simone Rossi: [@simrossi](https://github.com/simrossi)
Dorijan Di Zepp: [@najirod02](https://github.com/najirod02)

## License

This project is licensed under the GPLv3 License - see the LICENSE file for details

## Acknowledgments

Inspiration, code snippets, etc.
* [example](https://)
