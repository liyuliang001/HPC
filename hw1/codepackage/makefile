all: vv vv_d mv mv_d mm mm_d mm2 mm2_d

vv: vv.c
	gcc -w -o vv -O2 vv.c -lm -lrt -D SILENT
vv_d: vv_d.c
	gcc -w -o vv_d -O2 vv_d.c -lm -lrt -D SILENT
mv: mv.c
	gcc -w -o mv -O2 mv.c -lm -lrt -D SILENT
mv_d: mv_d.c
	gcc -w -o mv_d -O2 mv_d.c -lm -lrt -D SILENT
mm: mm.c
	gcc -w -o mm -O2 mm.c -lm -lrt -D SILENT
mm_d: mm_d.c
	gcc -w -o mm_d -O2 mm_d.c -lm -lrt -D SILENT
mm2: mm2.c
	gcc -w -o mm2 -O2 mm2.c -lm -lrt -D SILENT
mm2_d: mm2_d.c
	gcc -w -o mm2_d -O2 mm2_d.c -lm -lrt -D SILENT

clean:
	rm vv vv_d mv mv_d mm mm_d mm2 mm2_d

