# Data Structures III Course Repostory

Welcome to my repostory for the Data Structures III course! This repostory contains all my assignments. The course focuses on advanced data structures, emphasizing binary file storage and graph theory. Below you will find detailed information about each of the two main assignments included in this course.

## Assignment 1: Binary File Data Management

### Objective

The first assignment aims to demonstrate proficiency in storing data within binary files. Additionally, it involves developing functionalities for efficient manipulation of this data, ensuring fast access and modification capabilities.
This project is structured with a distinction between header files (`.h`) located in the `headers` directory and source files (`.c`) in the `src` directory. It includes a `makefile` for straightforward compilation and execution.

## Structure

- **Headers**: The `headers` directory contains all the header files with declarations needed for the project.

  - `funcAux.h`: Declarations for auxiliary functions.
  - `funcGiven.h`: Declarations for given functions, which are possibly provided as pre-defined project components.
  - `funcMain.h`: Declarations for the main functionality.
  - `functionalities.h`: Declarations for various functionalities.
  - `head.h`: Potentially a central header for common includes and definitions.

- **Source**: The `src` directory holds the implementation files corresponding to the headers.

  - `funcAux.c`: Implementation of auxiliary functions.
  - `funcGiven.c`: Functions given by the teacher.
  - `funcMain.c`: Core implementation of the main functionality.
  - `functionalities.c`: Implementation of the program functionalities.
  - `main.c`: The entry point of the program containing the `main` function.

- **Makefile**: A makefile is provided to automate the compilation process.

  - To compile the project, run `make all`.
  - To start the execution of the compiled program, use `make run`.

Ensure you have C compiler and `make` installed on your system to utilize the makefile for building and running the project.

## Assignment 2: Graph Theory Concepts

### Objective

The second assignment delves deeper into graph theory, focusing on the implementation and analysis of various graph data structures and algorithms. It aims to solidify understanding of graph concepts and their application in solving complex problems.

The Graphs project is a C++ codebase organized into header and source files, aimed at providing functionality related to graph data structures and algorithms. The project makes use of a makefile for compilation and execution, ensuring a streamlined build process.
The project is split into two main directories: `headers` and `src`.

- **Headers**: The `headers` directory contains all the `.h` files which declare the functions and classes used within the source files.

  - `funcCheck.h`: Header for functions responsible for validation checks.
  - `funcGiven.h`: Header for predefined functions, which may be a part of the initial project template.
  - `funcOpen.h`: Header for functions that handle opening and managing files or data streams.
  - `funcRW.h`: Header for functions dedicated to reading from and writing to data structures or files.
  - `functionalities.h`: Header for miscellaneous functionalities that don't fall into the other categories.
  - `graph.h`: Header for the graph class or related graph functions.
  - `struct.h`: Header for additional structures, possibly supporting the graph data structures.

- **Source**: The `src` directory includes the `.cpp` files which contain the actual implementation of the declared functions and classes.

  - `funcCheck.cpp`: Implements validation check functions.
  - `funcGiven.cpp`: Implements the given functions.
  - `funcOpen.cpp`: Implements file or data stream opening functions.
  - `funcRW.cpp`: Implements read/write functionalities.
  - `functionalities.cpp`: Implements various functionalities.
  - `graph.cpp`: Implements graph-related functions or classes.
  - `main.cpp`: Contains the `main` function, serving as the entry point for the program.
  - `struct.cpp`: Implements the additional structures defined in `struct.h`.

- **Makefile**: The project includes a makefile to facilitate the build process.

  - Compile the project using `make all`.
  - Execute the compiled binary with `make run`.

## Instructions

To get started with this project:

1. Clone or download the project to your local machine.
2. Navigate to the project directory in your terminal.
3. Run `make all` to compile the project.
4. Execute the program with `make run`.

Make sure you have a C++ compiler and `make` installed on your system.

## Installation and Usage

Please follow the instructions below to clone the repostory and run the assignments on your local machine.

```bash
git clone [Repostory URL]
cd [Repostory Directory]
```

## Contributing

This repostory is primarily for academic evaluation. However, constructive feedback and suggestions are welcome. Please feel free to fork the repostory and submit pull requests for any enhancements.


Thank you for visiting my Data Structures III course repostory!
