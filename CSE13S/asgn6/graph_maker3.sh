make spotless
make

for i in {1..50000}
do
	echo -n "$i " >> hits.dat
	echo -n "$i " >> misses.dat
	echo -n "$i " >> false_positives.dat
	cat "test_design.tex" | ./banhammer -s -f $i > temp.txt
	cat "temp.txt" | head -n 2 | tail -n 1 | tail -c +10 >> hits.dat
	cat "temp.txt" | head -n 3 | tail -n 1 | tail -c +12 >> misses.dat
	cat "temp.txt" | tail -n 3 | head -n 1 | tail -c +18 >> false_positives.dat
done

gnuplot <<END

set terminal pdf
set output "figure3.pdf"
set title "Hits/Misses in a Hash Table as Bloom Filter Changes Size"
set xlabel "Size of Bloom Filter"
set ylabel "Hits/Misses"
plot "hits.dat" with lines, "misses.dat" with lines

set output "figure4.pdf"
set title "Percentage of False Positives in a Bloom Filter as it Changes Size"
set xlabel "Size of Bloom Filter"
set ylabel "Percent of False Positives"
plot "false_positives.dat" with lines

END

rm *.dat
rm temp.txt

make spotless