make spotless
make

for i in {1000000..1001000}
do
	echo -n "$i " >> data.dat
	cat "test_design.tex" | ./banhammer -s -f $i | tail -n 4 | head -n 1 | tail -c +25 >> data.dat
done

gnuplot <<END

set terminal pdf
set output "figure2.pdf"
set title "Bits Examined Per Miss with Different Bloom Filter Sizes"
set xlabel "Size of Bloom Filter"
set ylabel "Bits Missed"
plot "data.dat" with lines

END

rm data.dat

make spotless