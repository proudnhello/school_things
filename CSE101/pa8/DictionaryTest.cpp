// Moore Macauley
// mmacaule
// pa8

#include"Dictionary.h"
#include<iostream>
#include<string>

int main()
{

    std::cout << "Basic adding values, copying Dictionaries, and printing\n";

    Dictionary test;
    test.setValue("1", 0);
    std::cout << test.to_string();
    std::cout << "\n";

    test.setValue("1", 1);
    test.setValue("2", 2);
    test.setValue("3", 3);
    test.setValue("4", 4);
    test.setValue("5", 5);
    test.setValue("6", 6);
    test.setValue("7", 7);
    test.setValue("8", 8);
    test.setValue("9", 9);
    std::cout << test.to_string();
    std::cout << "\n";
    std::cout << test.size();
    std::cout << "\n";
    
    test.setValue("3", 9999);
    test.setValue("8", 72176);
    std::cout << test.to_string();
    std::cout << "\n";
    std::cout << test.pre_string();
    std::cout << "\n";
    std::cout << test.size();
    std::cout << "\n";

    Dictionary copy(test);
    std::cout << copy.to_string();
    std::cout << "\n";
    std::cout << copy.pre_string();
    std::cout << "\n";
    std::cout << copy.size();
    std::cout << "\n";
    std::cout << copy.equals(test);
    std::cout << "\n";
    test.clear();
    std::cout << "If clear works properly, there should be nothing here :";
    std::cout << test.to_string();
    std::cout << test.pre_string();
    std::cout << "\n";
    std::cout << test.size();
    std::cout << "\n";
    std::cout << copy.equals(test);
    std::cout << "\n";

    std::cout << "-------------------------------------------------------\n";
    std::cout << "Seaching for values\n";

    std::cout << copy.contains("9") << "\n";
    std::cout << copy.contains("b") << "\n";
    std::cout << copy.contains("3") << "\n";
    std::cout << copy.contains("10") << "\n";
    std::cout << copy.getValue("8") << "\n";
    std::cout << copy.getValue("3") << "\n";
    copy.begin();
    std::cout << copy.currentKey() << " : " << copy.currentVal() << "\n";
    copy.end();
    std::cout << copy.currentKey() << " : " << copy.currentVal() << "\n";

    std::cout << "-------------------------------------------------------\n";
    std::cout << "Iterating over the tree\n";
    copy.begin();
    while(copy.hasCurrent()){
        std::cout << copy.currentKey() << " : " << copy.currentVal() << "\n";
        copy.next();
    }
    std::cout << "\n";
    copy.end();
    while(copy.hasCurrent()){
        std::cout << copy.currentKey() << " : " << copy.currentVal() << "\n";
        copy.prev();
    }

    
    std::cout << "-------------------------------------------------------\n";
    std::cout << "Delete fun\n";
    std::cout << copy.to_string();
    std::cout << "\n";
    std::cout << copy.pre_string();
    std::cout << "\n";

    copy.remove("7");
    copy.remove("6");
    copy.remove("5");
    //copy.remove("4");
    
    std::cout << copy.to_string();
    std::cout << "\n";
    std::cout << copy.pre_string();
    std::cout << "\n";

    return 0;
}
