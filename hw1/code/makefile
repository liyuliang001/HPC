all: vv vv_d mv mv_d mm mm_d mm2 mm2_d

vv: vv.c
	gcc -w -o vv -O2 vv.c -lm -lrt
vv_d: vv_d.c
	gcc -w -o vv_d -O2 vv_d.c -lm -lrt
mv: mv.c
	gcc -w -o mv -O2 mv.c -lm -lrt
mv_d: mv_d.c
	gcc -w -o mv_d -O2 mv_d.c -lm -lrt
mm: mm.c
	gcc -w -o mm -O2 mm.c -lm -lrt
mm_d: mm_d.c
	gcc -w -o mm_d -O2 mm_d.c -lm -lrt
mm2: mm2.c
	gcc -w -o mm2 -O2 mm2.c -lm -lrt
mm2_d: mm2_d.c
	gcc -w -o mm2_d -O2 mm2_d.c -lm -lrt

clean:
	rm vv vv_d mv mv_d mm mm_d mm2 mm2_d

