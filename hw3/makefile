all:  prob1_i_ijk_static prob1_j_ijk_static prob1_k_ijk_static prob1_j_jik_static prob1_k_kji_static prob2_static prob2_dynamic prob2_guided prob3_new

prob1_i_ijk_static: ./code/prob1/prob1_i_ijk_static.c
	gcc -O2 -fopenmp -w -o ./bin/prob1_i_ijk_static ./code/prob1/prob1_i_ijk_static.c -lm -lrt 
prob1_j_ijk_static: ./code/prob1/prob1_j_ijk_static.c
	gcc -O2 -fopenmp -w -o ./bin/prob1_j_ijk_static ./code/prob1/prob1_j_ijk_static.c -lm -lrt 
prob1_k_ijk_static: ./code/prob1/prob1_k_ijk_static.c
	gcc -O2 -fopenmp -w -o ./bin/prob1_k_ijk_static ./code/prob1/prob1_k_ijk_static.c -lm -lrt 
prob1_j_jik_static: ./code/prob1/prob1_j_jik_static.c
	gcc -O2 -fopenmp -w -o ./bin/prob1_j_jik_static ./code/prob1/prob1_j_jik_static.c -lm -lrt 
prob1_k_kji_static: ./code/prob1/prob1_k_kji_static.c
	gcc -O2 -fopenmp -w -o ./bin/prob1_k_kji_static ./code/prob1/prob1_k_kji_static.c -lm -lrt 
prob2_static: ./code/prob2/prob2_static.c
	gcc -O2 -fopenmp -w -o ./bin/prob2_static ./code/prob2/prob2_static.c -lm -lrt 
prob2_dynamic: ./code/prob2/prob2_dynamic.c
	gcc -O2 -fopenmp -w -o ./bin/prob2_dynamic ./code/prob2/prob2_dynamic.c -lm -lrt 
prob2_guided: ./code/prob2/prob2_guided.c
	gcc -O2 -fopenmp -w -o ./bin/prob2_guided ./code/prob2/prob2_guided.c -lm -lrt 
prob3_new: ./code/prob3/pizza_new.cpp
	g++ -O2 -fopenmp -w -o ./bin/pizza_new ./code/prob3/pizza_new.cpp -lm -lrt

clean:
	rm ./bin/*
