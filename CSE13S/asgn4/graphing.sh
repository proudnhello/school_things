make clean
make

#Used to create figure 1, 2, 3, 6, 7, 8, 9, and 10

for i in {1..1000}
do
	./sorting -a -r $i -n $i -p 0 > temp.dat
	head -n 2 temp.dat | head -n 1 >> bubble.dat
	head -n 2 temp.dat | tail -n 1 >> heap.dat
	tail -n 2 temp.dat | head -n 1 >> quick.dat
	tail -n 2 temp.dat | tail -n 1 >> shell.dat
done

gnuplot <<END

set terminal pdf
set output "figure2.pdf"
set title "Sorting Algorithms and Small Array Sizes"
set xlabel "Number of Array Elements"
set ylabel "Comparisons Made"
set yrange [0:20000]
plot "bubble.dat" with lines, "heap.dat" with lines, "quick.dat" with lines, "shell.dat" with lines
END

# make clean