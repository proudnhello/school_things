// Moore Macauley
// mmacaule
// pa5

#include"List.h"
#include<iostream>

void shuffle(List& D){
    List front;
    List back;
    int frontSize = (D.length())/2;

    front.moveFront();
    D.moveFront();
    while(D.position() < frontSize){
        D.moveNext();
        front.insertBefore(D.peekPrev());
    }
    back.moveFront();
    while(D.position() < D.length()){
        D.moveNext();
        back.insertBefore(D.peekPrev());
    }

    D.clear();

    front.moveFront();
    back.moveFront();
    while(front.position() < front.length()){
        front.moveNext();
        back.moveNext();
        D.insertBefore(back.peekPrev());
        D.insertBefore(front.peekPrev());
    }

    if(back.position() != back.length()){
        back.moveNext();
        D.insertBefore(back.peekPrev());
    }
}

int main(int argc, char const *argv[]){

    // Ensure the correct values were passed in
    if(argc != 2){
        fprintf(stderr, "Usage: %s <max deck size>\n", argv[0]);
        exit(1);
    }

    int size = std::stoi(argv[1]);

    List start;
    List current;
    std::cout << "deck size \tshuffle count\n";
    std::cout << "------------------------------\n";

    for(int i = 1; i <= size; i++){
        start.moveBack();
        start.insertAfter(i);
        current.moveBack();
        current.insertAfter(i);

        int total = 1;
        shuffle(current);
        while(!(current == start)){
            shuffle(current);
            total++;
        }

        std::cout << " " << i << "\t \t" << total << "\n"; // This print statement provided by the tutor Simon Kwong
        }

    return 0;
}
