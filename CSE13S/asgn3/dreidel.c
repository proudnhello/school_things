#include "dreidel.h"
#include <stdio.h>
#include "mtrand.h"

char spin_dreidel(void){
	uint64_t random = mtrand_rand64();
	switch(random % 4){
		case 0:
			return 'G';
			break;
		case 1:
			return 'H';
			break;
		case 2:
			return 'N';
			break;
		default:
			return 'S';
			break;
	}
}

int play_game(int n_players, int coins_per_player, int *n_rounds){
	int message = 0;
	if(*n_rounds == 1){
		message = 1;
	}
	*n_rounds = 0;
	int players_in_game = n_players;
	int players_temp = n_players;
	//All players start with coins_per_player. Smaller player numbers will leave some values untouched, but as all the loops iterate between 0 and n_players, these shouldn't affect anything
	int coins[8] = {coins_per_player, coins_per_player, coins_per_player, coins_per_player, coins_per_player, coins_per_player, coins_per_player, coins_per_player};
	int pot = 0;
	int remv;

	while (players_in_game > 1){
		*n_rounds += 1;
		//printf("\n");
		//Uses players temp instead of n players to make breaking out of the loop easier below
		for (int i = 0; i < players_temp; i+=1){
			if (coins[i] > -1){
				char result = spin_dreidel();
				//printf("%d %d %d %c %d\n", i, coins[i], *n_rounds, result, pot);
				//printf("%c\n", result);
				switch(result){
					case 'G':
						coins[i] += pot;
						pot = 0;
						break;
					case 'H':
						//Coins removed should be rounded down, which is done via truncation thanks to int division
						remv = pot/2;
						coins[i] += remv;
						pot -= remv;
						break;
					case 'S':
						coins[i] -= 1;
						if (coins[i] == -1)
						{
							players_in_game -= 1;
							//Message assigned in play-dreidel.c
							if (message)
							{
								//so is player_names[]
								printf("%s: eliminated in round %d of a %d player game.\n", player_names[i], *n_rounds, n_players);
							}
							if (players_in_game <= -1){
								//This should stop the loop from continuing after this round
								players_temp = 0;
							}
						}else{
							//only increace the pot if the player has a coin to give
							pot += 1;
						}
						break;
					default:
						break;
				}
			}
		}
	}

	for(int i = 0; i < n_players; i+=1){
		//Iterates over the coins list, looking for the for the one player that still has coins left.
		if (coins[i] > -1){
			return i;
		}
	}

	return 1;
}
