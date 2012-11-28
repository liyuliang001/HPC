echo "" > guided.out
echo "" > dynamic.out
echo "" > static.out
for mode in 1 2 3
do
	mpirun -n 3 ./guided $mode 100 100 1.1505 >> guided.out
done
for mode in 1 2 3
do
	mpirun -n 4 ./dynamic $mode 100 100 >> dynamic.out
done
for mode in 1 2 3
do
	mpirun -n 3 ./static $mode 100 >> static.out
done
