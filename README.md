# Incomplete-Directed-Perfect-Phylogeny
This project contains the codes for solving Incomplete Directed Perfect Phylogeny problem, and some other problems studied in our research

## Introduction

  This program implements the algorithm for solving IDPP problem proposed in [1], and it is used as part of research in our experimental study. See also https://github.com/researchGit/Compatibility-Testing.

  This document will lead you to a better understanding of using the program on your local machine.

## Components

  * **IDPP** contains all source codes associated with this project. Under this directory, **cmake-build-debug** directory contains **Makefile** and a Unix executable file named as **IDPP**.  
  * Under **cmake-build-debug** folder, we include data that we produced and reported in our paper. **IDPPvsTreeCompatibility** contains IDPP instances and their transformed instances that can be used in Tree Compatibility Testing. **input** folder contains some small examples of the matrices of IDPP problem. **seed folder** contains seed matrices that were used in our research.

## Environment

  IDPP is implemented in C++ with respect to c++11 standard. For convenience, it is recommended to use a Unix-based device, such as a Mac. You can find a Unix executable file named **IDPP** under **cmake-build-debug** directory.  

  Intuitively, a simple command **./IDPP** can be used to execute the program. If this file is lost, you simply need to run **make** command in the command tool to re-compile the program and generate a unix executable file. To do this, the necessary **Makefile** can also be found under the this direectory. And this is the only prerequisite your machine needs to satisfy.  

  If there is any problem on *compiler* and *make*, please refer to the corresponding websites for help. Typically, we use *Clang* as our C++ compiler.

## Main Features

  IDPP is mainly used for solving IDPP problems by taking a matrix with 0, 1 and ? marks as an input. It will construct a tree to display the information of the matrix. The final results, reported in NEWICK format, are printed in the **result** files under **cmake-build-debug** folder. For details, please refer to paper [1].

  As part of research, our implementation of IDPP algorithm has mainly following features.
  * Generating the IDPP instances(containing 0, 1 and ?) and their transformed instances for Compatibility Testing.
  * Generating the seed matrices(containing 0 and 1) that can be used for examining IDPP algorithm.
  * Taking IDPP instances as inputs and solving IDPP problems.
  * Taking seed matrices as inputs, and converting some of the 0 and 1 entries to ? marks and then soling the IDPP problems.

## Execution

  IDPP uses command line as its interface. The Unix executable file **IDPP** can be found under the **cmake-build-debug** directory in the project. Download the project, use command console to direct to this directory on your device. The commands are as described below.

### Template

  Here are the command templates:
  * ./IDPP -g -sd --s 100 --c 20 --h/--m/--l, this command is used for generating the seed matrices with different density of 1 entries. This specific example of the command would produce a matrix with the order of 100 \* 2000(100 rows/species, 2000 columns/characters) by specifying --c is 20 times. --h is for high density, --m is for intermediate density and --l is for low density correspondingly.
  * ./IDPP -g -t --s 100 --c 20 --p 95 --l/--h --f 30, this command is used for generating the IDPP instances and their transformed instances for Compatibility Testing. --p specifies the percentage of 0 and 1 entries that are remained in the matrices. 95 means that only 5 percent of those entries will be converted to ? randomly. --f specifies how many instances will be produced.
  * ./IDPP -r -sd --p 95 --ep 0 --f 30 filepath, this command takes the seed matrix as inputs, by specifying the percentage of entries to be converted, the program would be able to solve the matrices. filepath is basically the path that includes the seed matrix file.
  * ./IDPP -r -t --ep 0 --f 30 filepath, this command takes the IDPP instances as inputs and solve the matrices. filepath is the path contains the IDPP instances. Assuming that there are 30 instances, and they are preferred to be named as filename1, filename2, ... , filename30. Then the filepath only needs to include filename. An example: /Users/instances will be used if all 30 instances are named as instances1, instances2, ... , instances30.

## Author
  This project is implemented by Lei Liu(lliu@iastate.edu)

## License
  This project is under [MIT License](LICENSE.md).

## Conclusion
  This software could be a useful tool for solving IDPP problems. Your comments, opinions and criticism are very welcome, and they may become new inspirations directing to huge improvements.  

  I would be very glad to see your citations to this work in your own work if you are going to use some ideas from this project.  

  If you have trouble executing the program, please feel free to contact me through email.

## References
  [1] Pe’er, I.; Pupko, T.; Shamir, R.; Sharan, R. Incomplete directed perfect phylogeny. SIAM J. Comput. 2004, 33, 590–607. doi:10.1137/S0097539702406510.
