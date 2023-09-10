make clean
make

#Used to create figure 4 and 5

for i in {1000000..1001000}
do
	./sorting -s -q -h -r $i -n $i -p 0 > temp.dat
	head -n 2 temp.dat | head -n 1 >> heap.dat
	head -n 2 temp.dat | tail -n 1 >> quick.dat
	tail -n 1 temp.dat >> shell.dat
done

gnuplot <<END

set terminal pdf
set output "figure5.pdf"
set title "Sorting Algorithms and Large Array Sizes"
set xlabel "Number of Array Elements"
set ylabel "Moves Made"
plot "heap.dat" with lines, "quick.dat" with lines, "shell.dat" with lines
END

# make clean