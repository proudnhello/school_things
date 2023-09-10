make

#all this code is heavily based on my asgn 1 code
#remember to comment out the section on default functionality and uncomment the section for graph3.sh in play-dreidel.c
for p in {1..20}
do
for a in {2..8}
do
    for i in {1..10000};
    do

        ./play-dreidel -p $a -c $p -s $i >> results.dat
        
    done

    sort -n results.dat > sorted_results.dat
    uniq -u sorted_results.dat > temp.dat
    uniq -d sorted_results.dat >> temp.dat
    sort -n temp.dat > results_that_appear.dat
    uniq -c sorted_results.dat > ocurrences.dat

    max_value=$(tail -n 1 results_that_appear.dat)

    for (( i=0; i <= $max_value; i++ ))
    do
        current_value=$(head -n 1 results_that_appear.dat | head -c -1)
        if [[ $current_value = $i ]]
        then
            head -c 7 ocurrences.dat >> graph_data.dat
            echo "" >> graph_data.dat
            tail -n +2 results_that_appear.dat > temp.dat
            cat temp.dat > results_that_appear.dat
            tail -n +2 ocurrences.dat > temp.dat
            cat temp.dat > ocurrences.dat
        else
            echo 0 >> graph_data.dat
        fi
    done

    gnuplot <<END

        set terminal pdf
        set output "test$a$p.pdf"
        set title "Player Victory Count"
        set xlabel "Player Number"
        set ylabel "Victories"
        set yrange [0:10000]
        plot "graph_data.dat" with histogram title ""
    quit
END
rm *.dat
done
done
