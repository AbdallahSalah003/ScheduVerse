rm *.out
gcc process_generator.c -o generator.out
gcc scheduler.c -o scheduler.out -lm
gcc clk.c -o clk.out
gcc process.c -o process.out
./generator.out