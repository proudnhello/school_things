// Moore Macauley
// mmacaule
// pa6

#include"List.h"

int main(int argc, char const *argv[])
{
    List L;

    std::cout << "Basic adding items\n";
    std::cout << L.length();
    std::cout << "\n";
    std::cout << L.position();
    std::cout << "\n";

    L.insertBefore(1);
    L.insertBefore(2);
    L.insertBefore(3);

    L.insertAfter(4);
    L.insertAfter(5);
    L.insertAfter(6);

    std::cout << L.to_string();
    std::cout << "\n";
    std::cout << L.length();
    std::cout << "\n";
    std::cout << L.position();
    std::cout << "\n";
    std::cout << L.front();
    std::cout << "\n";
    std::cout << L.back();
    std::cout << "\n";
    std::cout << L.peekNext();
    std::cout << "\n";
    std::cout << L.peekPrev();
    std::cout << "\n";

    std::cout << "---------------------------------------------------------------------\n";
    std::cout << "Moving cursor and adding\n";

    std::cout << L.to_string();
    std::cout << "\n";
    L.moveFront();
    L.moveNext();
    std::cout << L.position();
    std::cout << "\n";
    std::cout << L.peekNext();
    std::cout << "\n";
    std::cout << L.peekPrev();
    std::cout << "\n";
    L.insertBefore(8);
    L.insertAfter(9);
    std::cout << L.to_string();
    std::cout << "\n";
    std::cout << L.position();
    std::cout << "\n";
    std::cout << L.peekNext();
    std::cout << "\n";
    std::cout << L.peekPrev();
    std::cout << "\n";

    std::cout << "---------------------------------------------------------------------\n";

    std::cout << "More cursor moving and adding\n";
    std::cout << L.to_string();
    std::cout << "\n";
    L.moveBack();
    L.movePrev();
    std::cout << L.position();
    std::cout << "\n";
    std::cout << L.peekNext();
    std::cout << "\n";
    std::cout << L.peekPrev();
    std::cout << "\n";
    L.insertBefore(8);
    L.insertAfter(9);
    std::cout << L.to_string();
    std::cout << "\n";
    std::cout << L.position();
    std::cout << "\n";
    std::cout << L.peekNext();
    std::cout << "\n";
    std::cout << L.peekPrev();
    std::cout << "\n";

    std::cout << "---------------------------------------------------------------------\n";

    std::cout << "Set before and after\n";
    L.setAfter(-100);
    L.setBefore(-999);
    std::cout << L.to_string();
    std::cout << "\n";

    std::cout << "---------------------------------------------------------------------\n";

    std::cout << "Erase before and after\n";
    L.eraseAfter();
    L.eraseAfter();
    std::cout << L.to_string();
    std::cout << "\n";
    std::cout << L.length();
    std::cout << "\n";
    std::cout << L.position();
    std::cout << "\n";

    L.eraseBefore();
    L.eraseBefore();
    std::cout << L.to_string();
    std::cout << "\n";
    std::cout << L.length();
    std::cout << "\n";
    std::cout << L.position();
    std::cout << "\n";

    std::cout << "---------------------------------------------------------------------\n";

    std::cout << "Copy, .equals, and ==\n";
    List copy = L;
    std::cout << L.to_string();
    std::cout << "\n";
    std::cout << copy.to_string();
    std::cout << "\n";
    std::cout << (L == copy);
    std::cout << "\n";
    copy.setBefore(68);
    std::cout << L.to_string();
    std::cout << "\n";
    std::cout << copy.to_string();
    std::cout << "\n";
    std::cout << (L == copy);
    std::cout << "\n";

    std::cout << "---------------------------------------------------------------------\n";

    std::cout << "concat\n";
    List cat = L.concat(copy);
    std::cout << L.to_string();
    std::cout << "\n";
    std::cout << copy.to_string();
    std::cout << "\n";
    std::cout << cat.to_string();
    std::cout << "\n";

    std::cout << "---------------------------------------------------------------------\n";

    std::cout << "Find next\n";
    std::cout << cat.findNext(68);
    std::cout << "\n";
    std::cout << cat.position();
    std::cout << "\n";
    std::cout << "\n";
    std::cout << cat.findNext(2);
    std::cout << "\n";
    std::cout << cat.position();
    std::cout << "\n";
    std::cout << "\n";
    std::cout << cat.findNext(5789);
    std::cout << "\n";
    std::cout << cat.position();
    std::cout << "\n";

    std::cout << "---------------------------------------------------------------------\n";

    std::cout << "Find prev\n";
    std::cout << cat.findPrev(68);
    std::cout << "\n";
    std::cout << cat.position();
    std::cout << "\n";
    std::cout << "\n";
    std::cout << cat.findPrev(2);
    std::cout << "\n";
    std::cout << cat.position();
    std::cout << "\n";
    std::cout << "\n";
    std::cout << cat.findPrev(5789);
    std::cout << "\n";
    std::cout << cat.position();
    std::cout << "\n";

    std::cout << "---------------------------------------------------------------------\n";

    std::cout << "pre cleanup\n";
    List cat2 = cat.concat(cat);
    std::cout << cat2.to_string();
    std::cout << "\n";
    std::cout << cat2.position();
    std::cout << "\n";
    for(int i = 0; i < 11; i++){
        // move into pos 11
        cat2.moveNext();
    }
    std::cout << cat2.position();
    std::cout << "\n";
    cat2.cleanup();

    std::cout << "---------------------------------------------------------------------\n";

    std::cout << "post cleanup.\n";
    std::cout << cat2.to_string();
    std::cout << "\n";
    std::cout << cat2.length();
    std::cout << "\n";
    std::cout << cat2.position();
    std::cout << "\n";
    std::cout << cat2.peekPrev();
    std::cout << "\n";
    std::cout << cat2.peekNext();
    std::cout << "\n";

    std::cout << "---------------------------------------------------------------------\n";

    std::cout << "printing with <<\n";
    std::cout << cat2;
    std::cout << "\n";

    std::cout << "---------------------------------------------------------------------\n";

    std::cout << "=\n";

    L = cat2;
    std::cout << L;
    std::cout << "\n";
    std::cout << cat2;
    std::cout << "\n";
    cat2.clear();
    std::cout << L;
    std::cout << "\n";
    std::cout << cat2;
    std::cout << "\n";

    std::cout << "---------------------------------------------------------------------\n";

    std::cout << "Clear\n";
    std::cout << L.to_string();
    std::cout << "\n";
    std::cout << L.length();
    std::cout << "\n";

    L.clear();
    std::cout << "If clear works, nothing should be here:";
    std::cout << L.to_string();
    std::cout << "\n";

    return 0;
}
