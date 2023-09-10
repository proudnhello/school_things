#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mtrand.h"
#include "dreidel.h"

#define OPTIONS "p:c:s:v"
//Values declared in dreidel.h

char * player_names[8] = {"Aharon", "Batsheva", "Chanah", "David", "Ephraim", "Faige", "Gamaliel", "Hannah"};

int main(int argc, char **argv)
{
	//Default values for # of players, starting coins, and seed.
	int players = 4;
	int coins = 3;
	uint64_t my_seed = 613;
	int y = 0;
	int *n_rounds = &y;
	int opt = 0;
	int temp;
	uint64_t temp2;
	while ((opt = getopt(argc, argv, OPTIONS)) != -1){
		switch (opt){
			case 'p': 
				temp = atoi(optarg);
				//Players must be between 2 and 8
				if ((2 <= temp && temp <= 8))
				{
					players = temp;
				}else{
					return 1;
				}
				break;
			case 'c':
				temp = atoi(optarg);
				//Starting coins must be between 1 and 20
				if ((1 <= temp && temp <= 20))
				{
					coins = temp;
				}else{
					return 1;
				}
				break;
			case 's':
				temp2 = strtoul(optarg, NULL, 10);
				//seed must be at most 10 decimal characters. 10000000000 is 11 digits long, so any value less than it will be 10 characters long at most
				//it also cannot be 0, or negative, but as uint64_t is unsigned, negatives shouldn't be a problem
				if ((temp2 < 10000000000) && temp2 > 0){
					my_seed = temp2;
				}else{
					return 1;
				}
				break;
			case 'v':
				*n_rounds = 1;
				break;
			default:
				return 1;
		}
	}
	mtrand_seed(my_seed);

		//Segment used with default functionality
	printf("%s %d %d %d %ld\n", player_names[play_game(players, coins, n_rounds)], players, coins, *n_rounds, my_seed);
		//Segment used with graph1.sh and graph2.sh
	//play_game(players, coins, n_rounds);
	//printf("%d\n", *n_rounds);
		//Segment used with graph3.sh
	//printf("%d\n", play_game(players, coins, n_rounds));


	return 0; 
}
