make clean
make collatz

#iterates over all the numbers that will be plotted to create the data that will be used in the graphs

for i in {2..10000};
do
    ./collatz -n $i > sequence.dat;        #Creates collatz sequence for number i

    #Creates the data on collatz sequence lengths in the format of "(starthing number i) (sequence length)" to length_data, and saves all sequence lengths in order to raw_length_data

    echo -n "$i " >> length_data.dat
    cat sequence.dat | wc -l >> raw_length_data.dat;
    tail raw_length_data.dat -n 1 >> length_data.dat;

    #Creates data on maximum collatz sequence values in the format "(starting number i) (max value)" to value_data

    echo -n "$i " >> value_data.dat ;
    sort -n sequence.dat > ordered_sequence.dat;
    tail ordered_sequence.dat -n 1 >> value_data.dat;

    #Creates data on maximum collatz values and collatz sequence lengths in the format "(length) (maximum value)"

    tail raw_length_data.dat -n 1 | head -c -1 >> comparison_data.dat;
    echo -n " " >> comparison_data.dat
    tail ordered_sequence.dat -n 1 >> comparison_data.dat;
    
done

#Produces a list of all the lengths that appear ordered from least to greatest, the maximum length that appears, and a list of the number of times all lengths occur, also ordered from least to greatest

sort -n raw_length_data.dat > sorted_length_data.dat;
uniq -u sorted_length_data.dat > temp.dat;
uniq -d sorted_length_data.dat >> temp.dat
sort -n temp.dat > lengths_that_appear.dat;
#I googled how to save the output of a command to a variable, specifically from the first few paragraphs of https://linuxhint.com/bash_command_output_variable/
max_value=$(tail -n 1 lengths_that_appear.dat);    
uniq -c sorted_length_data.dat > ocurrences.dat;

#Iterates from 0 to the maximum sequence length

for (( i=0; i <= $max_value; i++ ))
do
    #We take the next largest sequence length that apears

    current_value=$(head -n 1 lengths_that_appear.dat | head -c -1)        #Same note as last time this appeared
    if [[ $current_value = $i ]]
    then
        #If it is equal to the current iterative, then that length does appear, so we get the number of occurences from our list of the number of occurences for all lengths
        #Before removing that sequence length and the number of occurences from their respective lists

        head -c 7 ocurrences.dat >> historgram_data.dat
        echo "" >> historgram_data.dat
        tail -n +2 lengths_that_appear.dat > temp.dat
        cat temp.dat > lengths_that_appear.dat
        tail -n +2 ocurrences.dat > temp.dat
        cat temp.dat > ocurrences.dat
    else
        #If this is not equal, then the length does not appear, so we add a 0 to the data to be plotted

        echo 0 >> historgram_data.dat
    fi
done

gnuplot <<END
    #plots length data into figure 1.pdf

    set terminal pdf
    set output "figure 1.pdf"
    set title "Collatz Sequence Lengths"
    set xlabel "n"
    set ylabel "length"
    set zeroaxis
    plot "length_data.dat" with dots title ""

    #plots comparison data into figure 3.pdf

    set output "figure 3.pdf"
    set title "Collatz Sequence Lengths VS Maximum Collatz Sequence Values"
    set xlabel "length"
    set ylabel "value"
    set yrange [0:500000]
    plot "comparison_data.dat" with dots title ""

    #plots value data into figure 2.pdf

    set output "figure 2.pdf"
    set title "Maximum Collatz Sequence Value"
    set xlabel "n"
    set ylabel "value"
    set yrange [0:100000]
    plot "value_data.dat" with dots title ""

    #plots histogram data into figure 4.pdf

    set output "figure 4.pdf"
    set title "Collatz Sequence Length Histogram"
    set xlabel "length"
    set ylabel "frequency"
    set xrange [0:225]
    set yrange [0:200]
    plot "historgram_data.dat" with histogram title ""

    quit
END

#Remove all txt files created during runtime, because some of the files don't get overwritten, just added on to, so having them around would break things, and clutter is bad

rm raw_length_data.dat
rm comparison_data.dat
rm sequence.dat
rm ordered_sequence.dat
rm value_data.dat
rm temp.dat
rm lengths_that_appear.dat
rm ocurrences.dat
rm historgram_data.dat
rm length_data.dat
rm sorted_length_data.dat
