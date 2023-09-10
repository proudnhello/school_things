// Moore Macauley
// mmacaule
// pa6

#include"List.h"
#include<iostream>
#include<string>
#include<stdexcept>


// Creates a new Node
List::Node::Node(ListElement x){
    data = x;
    next = nullptr;
    prev = nullptr;
}


// Creates a new List
List::List(void){
    pos_cursor = 0;
    num_elements = 0;

    frontDummy = new Node(-1);
    backDummy = new Node(-1);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
}


// Constructs a List that is a copy of L
List::List(const List& L){
    pos_cursor = 0;
    num_elements = 0;

    frontDummy = new Node(-1);
    backDummy = new Node(-1);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;

    Node *N = nullptr;

    for(N=L.frontDummy->next; N->next!=nullptr; N=N->next){
        insertBefore(N->data);
    }

    moveFront();

    while(L.position() != position()){
        moveNext();
    }

}


// Deconstructs the List
List::~List(void){
    clear();
    delete(frontDummy);
    delete(backDummy);
}


// Returns the length of the List
int List::length(void) const{
    return num_elements;
}


// Returns the cursor's current pos
int List::position(void) const{
    return pos_cursor;
}


// Returns the front element of the List
ListElement List::front() const{
    if(length() == 0){
        throw std::length_error("List: front(): empty List");
    }
    return frontDummy->next->data;
}


// Returns the back element in this List.
ListElement List::back() const{
    if(length() == 0){
        throw std::length_error("List: back(): empty List");
    }
    return backDummy->prev->data;
}

// Returns the element after the cursor.
ListElement List::peekNext() const{
    if(length() <= 0){
        throw std::length_error("List: peekNext(): empty List");
    }
    if(position() >= length()){
        throw std::out_of_range("List: peekNext(): called with cursor at end of list");
    }
    return afterCursor->data;
}


// Returns the element before the cursor.
ListElement List::peekPrev() const{
    if(length() <= 0){
        throw std::length_error("List: peekPrev(): empty List");
    }
    if(position() <= 0){
        throw std::out_of_range("List: peekPrev(): called with cursor at start of list");
    }
    return beforeCursor->data;
}


// Clears a List
void List::clear(){
    Node *N = nullptr;

    for(N=frontDummy->next->next; N!=nullptr; N=N->next){
        delete(N->prev);
    }
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;
}


// Moves cursor to the front
void List::moveFront(){
    beforeCursor = frontDummy;
    afterCursor = frontDummy->next;
    pos_cursor = 0;
}


// Moves cursor to the back
void List::moveBack(){
    beforeCursor = backDummy->prev;
    afterCursor = backDummy;
    pos_cursor = length();
}


// Moves cursor up by 1. Returns the List element that was passed over. 
ListElement List::moveNext(){
    if(position() >= length()){
        throw std::out_of_range("List: moveNext(): called with cursor at end of list");
    }
    pos_cursor++;
    beforeCursor = afterCursor;
    afterCursor = beforeCursor->next;
    return beforeCursor->data;
}


// Moves cursor back by 1. Returns the List element that was passed over. 
ListElement List::movePrev(){
    if(position() <= 0){
        throw std::out_of_range("List: movePrev(): called with cursor at start of list");
    }
    pos_cursor--;
    afterCursor = beforeCursor;
    beforeCursor = afterCursor->prev;
    return afterCursor->data;
}

// Inserts x after the cursor 
void List::insertAfter(ListElement x){
    Node *temp = new Node(x);
    temp->prev = beforeCursor;
    temp->next = afterCursor;

    afterCursor->prev = temp;
    beforeCursor->next = temp;

    afterCursor = temp;

    num_elements++;
}


// Inserts x before the cursor 
void List::insertBefore(ListElement x){
    Node *temp = new Node(x);
    temp->prev = beforeCursor;
    temp->next = afterCursor;

    afterCursor->prev = temp;
    beforeCursor->next = temp;

    beforeCursor = temp;

    num_elements++;
    pos_cursor++;
}


// Sets the value after cursor to be x
void List::setAfter(ListElement x){
    if(position() >= length()){
        throw std::out_of_range("List: setAfter(): called with cursor at end of list");
    }
    afterCursor->data = x;
}


// Sets the value before cursor to be x
void List::setBefore(ListElement x){
    if(position() <= 0){
        throw std::out_of_range("List: setBefore(): called with cursor at start of list");
    }
    beforeCursor->data = x;
}


// Remove the node after the cursor
void List::eraseAfter(){
    if(position() >= length()){
        throw std::out_of_range("List: eraseAfter(): called with cursor at end of list");
    }
    Node *del = afterCursor;
    afterCursor = del->next;
    beforeCursor->next = afterCursor;
    afterCursor->prev = beforeCursor;
    
    num_elements--;
    delete(del);
}


// Deletes element before cursor.
void List::eraseBefore(){
    if(position() <= 0){
        throw std::out_of_range("List: eraseBefore(): called with cursor at end of list");
    }
    Node *del = beforeCursor;
    beforeCursor = del->prev;
    afterCursor->prev = beforeCursor;
    beforeCursor->next = afterCursor;
    
    num_elements--;
    pos_cursor--;
    delete(del);
}


// Finds the first occurrence of x in List, returns -1 if none are found
int List::findNext(ListElement x){
    if(length() == 0){
        return -1;
    }

    while(position() < length()){
        moveNext();
        if(peekPrev() == x){
            return position();
        }
    }

    return -1;

}


// Finds the last occurence of x in the List, returns -1 if none are found
int List::findPrev(ListElement x){
    if(length() == 0){
        return -1;
    }

    while(position() > 0){
        movePrev();
        if(peekNext() == x){
            return position();
        }
    }

    return -1;
}


// Removes all duplicates 
void List::cleanup(){
    // If the length is 0 or 1, then there are obviously no dupes, so return immediately
    if(length() == 0 || length() == 1){
        return;
    }

    // We iterate over all the values in the List
    Node *current = frontDummy->next;
    while(current != backDummy){
        // Then we iterate over all the values not already iterated over
        Node *iter = current->next;
        while(iter != backDummy){
            // If there is another value in the list equal to current, remove that value
            if(iter->data == current->data){
                // If the value to be removed is part of the cursor, move it before removing the value
                if(beforeCursor == iter){
                    beforeCursor = iter->prev;
                }else if (afterCursor == iter){
                    afterCursor = iter->next;
                }
                iter->prev->next = iter->next;
                iter->next->prev = iter->prev;
                // Save a pointer to iter to a temp value, and delete it
                // Nessisary b/c all other pointers to iter have been deleted
                Node *temp = iter;
                iter = iter->next;
                delete(temp);
                num_elements--;
            }else{
                iter = iter->next;
            }
        }
        current = current->next;
    }

    // Finally, we need to find the position the cursor has ended up in
    current = frontDummy;
    int currPos = 0;
    // Increment every time current moves up by 1 until we find the cursor
    while(current != beforeCursor){
        currPos++;
        current = current->next;
    }
    // then set the cursor position to that
    pos_cursor = currPos;
}


// Returns a new List that consists of L added to the end of the current List
List List::concat(const List& L) const{
    if(length() == 0){
        List copy = L;
        return L;
    }
    if(L.length() == 0){
        List copy = *this;
        return copy;
    }
    List cat = *this;
    cat.moveBack();

    Node *N = nullptr;

    for(N=L.frontDummy->next; N->next!=nullptr; N=N->next){
        cat.insertBefore(N->data);
    }

    cat.moveFront();

    return cat;
}

// Checks to see if R and the List are equal
bool List::equals(const List& R) const{
    if(length() != R.length()){
        return false;
    }
    if(length() == 0){
        return true;
    }
    
    Node *l = frontDummy->next;
    Node *r = R.frontDummy->next;

    while(l->next != nullptr){
        if(l->data != r->data){
            return false;
        }
        l = l->next;
        r = r->next;
    }
    return true;
}


// Converts the List to a string and returns it
std::string List::to_string(void) const{
    Node *N = nullptr;
    std::string s = "";

    for(N=frontDummy->next; N!=backDummy; N=N->next){
        s += std::to_string(N->data)+" ";
    }

    return s;
}

// I feel like the adeptus mechanicus coding these
// I have no idea what it's doing, I just copy what prof did pray it works

// Overwrites <<
std::ostream& operator<< (std::ostream& stream, const List& L){
    return stream << L.to_string();
}

// Overwrites ==
bool operator== (const List& A, const List& B){
    return A.List::equals(B);
}

List& List::operator=(const List& L){
    if(this != &L){
        List temp = L;

        std::swap(frontDummy, temp.frontDummy);
        std::swap(backDummy, temp.backDummy);
        std::swap(beforeCursor, temp.beforeCursor);
        std::swap(afterCursor, temp.afterCursor);
        pos_cursor = temp.pos_cursor;
        num_elements = temp.num_elements;
    }

    return *this;
}
