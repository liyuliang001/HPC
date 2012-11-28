
echo "" > openmp.out 
echo "" > mpi.out

make clean; make;
for trape_num in 100 1000 10000 100000 1000000 10000000 100000000 1000000000
do
	mpirun -n 4 ./mpi_int 0 1.570796327 $trape_num >> mpi.out
done
for trape_num in 100 1000 10000 100000 1000000 10000000 100000000 1000000000
do
	./openmp_int 4 0 1.570796327 $trape_num >> openmp.out
done
