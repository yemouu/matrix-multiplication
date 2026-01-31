# CS 4522 Project 1: Matrix
Implements the General Matrix Multiplication algorithm, Strassen's algorithm, and Coppersmith and Winograd's Algorithm.

[Link to this repo](https://github.com/yemouu/matrix-multiplication)

# Compile
```sh
make
```

# Run
```
usage: matrix-multiplication [options]

	-a <algorithm> - select which algorithm to use (gemm [default], strassen, or coppersmith_winograd)
	-d             - enable debug prints
	-h             - display this message
	-s <integer>   - set a value for m n and k (creates a square matrix)
	-k <integer>   - set a value for k (determines the size of the matrix)
	-m <integer>   - set a value for m (determines the size of the matrix)
	-n <integer>   - set a value for n (determines the size of the matrix)
```

```
$ ./matrix-multiplication -a gemm -s 2048
Elapsed Time: 2.987361s

$ ./matrix-multiplication -a strassen -s 2048
Elapsed Time: 4.499131s

$ ./matrix-multiplication -a coppersmith_winograd -s 2048
Elapsed Time: 4.449780s
```

```
$ ./matrix-multiplication -a coppersmith_winograd -s 4 -d
Algorithm: coppersmith_winograd
debug: 1
M: 4, N: 4, K: 4

Matrix A:
17	55	2	6
43	94	4	94
79	33	48	85
20	20	98	34

Matrix B:
16	93	31	36
25	56	65	52
75	89	51	92
5	48	77	89

Elapsed Time: 0.000003s
Matrix C:
1827	5127	4666	4190
3808	14131	14885	15170
6114	17547	13587	16541
8340	13334	9536	13802

Matrix D:
1827	5127	4666	4190
3808	14131	14885	15170
6114	17547	13587	16541
8340	13334	9536	13802
```

In the above example, `Matrix C` is the result using `gemm` while `Matrix D` is the result using `coppersmith_winograd`.
With the `-d` flag, these matricies are automatically compared for equality and if the results aren't the same an error
is printed to the terminal.
