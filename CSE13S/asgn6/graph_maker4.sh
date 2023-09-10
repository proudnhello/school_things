make spotless
make

for i in {1..50000}
do
	echo -n "$i " >> mtf_off.dat
	echo -n "$i " >> mtf_on.dat
	cat "test_design.tex" | ./banhammer -s -t $i | head -n 4 | tail -n 1 | tail -c +12 >> mtf_off.dat
	cat "test_design.tex" | ./banhammer -s -t $i -m | head -n 4 | tail -n 1 | tail -c +12 >> mtf_on.dat

done

gnuplot <<END

set terminal pdf
set output "figure6.pdf"
set title "Links examined in a Hash Table Based on Table Size"
set xlabel "Size of Hash Table"
set ylabel "Links Examined"
set xrange [0:100]
plot "mtf_on.dat" with lines, "mtf_off.dat" with lines

set terminal pdf
set output "figure7.pdf"
set title "Links examined in a Hash Table Based on Table Size"
set xlabel "Size of Hash Table"
set ylabel "Links Examined"
set xrange [0:50000]
set yrange [0:10000]
plot "mtf_on.dat" with lines, "mtf_off.dat" with lines

END

rm *.dat
rm temp.txt

make spotless