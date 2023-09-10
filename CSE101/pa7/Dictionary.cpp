// Moore Macauley
// mmacaule
// pa7

#include"Dictionary.h"
#include<iostream>
#include<string>
#include<stdexcept>

// Constructs a new tree node
Dictionary::Node::Node(keyType k, valType v){
    key = k;
    val = v;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
}

// Constructs a new Dictionary
Dictionary::Dictionary(){
    // -123456789 was picked as a value unlikely to appear in normal use for debugging purposes
    nil = new Node("THIS IS NILL", -123456789);
    root = nil;
    current = nil;
    num_pairs = 0;
}

// Copies the subtree of R into the dictionary, stopping when R == N
void Dictionary::preOrderCopy(Node* R, Node* N){
    if(R != N){
        setValue(R->key, R->val);
        preOrderCopy(R->left, N);
        preOrderCopy(R->right, N);
    }
}

// Constructs a new Dictioary that is equal to D
Dictionary::Dictionary(const Dictionary& D){
    nil = new Node("THIS IS NILL", -123456789);
    root = nil;
    current = nil;
    num_pairs = 0;
    preOrderCopy(D.root, D.nil);
}

// Deletes the subtree at R
void Dictionary::postOrderDelete(Node* R){
    if(R != nil){
        postOrderDelete(R->left);
        postOrderDelete(R->right);
        delete(R);
        R = nil;
    }
}

// Removes all pairs from the dictionary
void Dictionary::clear(){
    postOrderDelete(root);
    num_pairs = 0;
    root = nil;
    current = nil;
}

// Deconstructs a Dictionary
Dictionary::~Dictionary(){
    postOrderDelete(root);
    delete nil;
}

// Returns the number of key value pairs
int Dictionary::size() const{
    return num_pairs;
}

// Returns a pointer to the occurance of K, starting at R
Dictionary::Node *Dictionary::search(Node* R, keyType k) const{
    if(R == nil){
        return nil;
    }
    if(k == R->key){
        return R;
    }

    if(k < R->key){
        return search(R->left, k);
    }
    if(k > R->key){
        return search(R->right, k);
    }

    return nil; // This should literally never get here
}

// Returns true if k is in the dictionary, false otherwise
bool Dictionary::contains(keyType k) const{
    if(search(root, k) != nil){
        return true;
    }
    return false;
}

// Returns the value that corrisponds to k
valType& Dictionary::getValue(keyType k) const{
    Node *temp = search(root, k);
    if(temp == nil){
        throw std::out_of_range("Dictionary: getValue(): key does not exist");
    }
    return temp->val;
}

// Returns true if current exists, false otherwise
bool Dictionary::hasCurrent() const{
    if(current != nil){
        return true;
    }
    return false;
}

// Returns the current key if it exists
keyType Dictionary::currentKey() const{
    if(current == nil){
        throw std::out_of_range("Dictionary: currentKey(): current is not defined");
    }
    return current->key;
}

// Returns the value corrisponding to the current key
valType& Dictionary::currentVal() const{
    if(current == nil){
        throw std::out_of_range("Dictionary: currentVal(): current is not defined");
    }
    return current->val;
}

// Sets the value at k to v if it exists, adds a new value if it does not
void Dictionary::setValue(keyType k, valType v){
    if(root == nil){
        root = new Node(k, v);
        root->parent = nil;
        root->left = nil;
        root->right = nil;
        num_pairs++;
        return;
    }

    Node* iter = root;

    // While there are still keys left to check
    while(iter != nil){
        int comp = iter->key.compare(k);
        // If the current key is equal to k, change the corrisponding value to v
        if(comp == 0){
            iter->val = v;
            return;
        // If k is less than the current key, we need to check left
        }else if(comp > 0){
            // If the left node is nil, then add a new node to the left of the current node
            if(iter->left == nil){
                Node *temp = new Node(k, v);
                temp->parent = iter;
                temp->left = nil;
                temp->right = nil;
                iter->left = temp;
                num_pairs++;
                return;
            }else{
            // Otherwise, step iter down to the left
                iter = iter->left;
            }
        // Otherwise, we check right
        }else{
            // If the right node is nil, then add a new node to the right of the current node
            if(iter->right == nil){
                Node *temp = new Node(k, v);
                temp->parent = iter;
                temp->left = nil;
                temp->right = nil;
                iter->right = temp;
                num_pairs++;
                return;
            }else{
            // Otherwise, step iter down to the right
                iter = iter->right;
            }
        }
    }
}

// Removes the key value pair for k
void Dictionary::remove(keyType k){
    Node *gone = search(root, k);
    if(gone == nil){
        throw std::out_of_range("Dictionary: remove(): provided key does not exist");
    }

    if(current == gone){
        current = nil;
    }

    // If the resulting value is a leaf, delete the leaf and return
    if(gone->left == nil && gone->right == nil){
        if(gone->parent != nil && gone->parent->left == gone){
            gone->parent->left = nil;
        }
        if(gone->parent != nil && gone->parent->right == gone){
            gone->parent->right = nil;
        }
        if(root == gone){
            root = nil;
        }
        delete gone;
        num_pairs--;
        return;
    }

    // If it has one child, simply move the child up to replace it
    if(gone->left != nil && gone->right == nil){
        if(gone->parent != nil && gone->parent->left == gone){
            gone->parent->left = gone->left;
        }
        if(gone->parent != nil && gone->parent->right == gone){
            gone->parent->right = gone->left;
        }
        gone->left->parent = gone->parent;
        if(root == gone){
            root = gone->left;
        }
        delete gone;
        num_pairs--;
        return;
    }

    // Same as above, but moving up from the right
    if(gone->right != nil && gone->left == nil){
        if(gone->parent != nil && gone->parent->left == gone){
            gone->parent->left = gone->right;
        }
        if(gone->parent != nil && gone->parent->right == gone){
            gone->parent->right = gone->right;
        }
        gone->right->parent = gone->parent;
        if(root == gone){
            root = gone->right;
        }
        delete gone;
        num_pairs--;
        return;
    }

    // Finally, if it has two childern, replace it with it's sucessor
    Node *replacement = findNext(gone);
    Node *rParent = replacement->parent;
    Node *rRight = replacement->right;

    // First, we replace the replacement with its right subtree (we know it cannot have a left child b/c if it did, it wouldn't be the sucessor, something in that tree would be)
    if(rParent->right == replacement){
        rParent->right = rRight;
    }
    if(rParent->left == replacement){
        rParent->left = rRight;
    }
    rRight->parent = rParent;

    // Then, we set all of the pointer in replacement to those in gone
    replacement->parent = gone->parent;
    replacement->left = gone->left;
    replacement->right = gone->right;
    if(root == gone){
        root = replacement;
    }
    delete gone;
    num_pairs--;
    
}

// Return a pointer to the leftmost, or smallest, node
Dictionary::Node *Dictionary::findMin(Node* R){
    if(R == nil){
        return nil;
    }
    Node *curr = R;
    while (curr->left != nil)
    {
        curr = curr->left;
    }
    return curr;
}

// Sets current to be the smallest Node
void Dictionary::begin(){
    if(num_pairs == 0){
        return;
    }
    current = findMin(root);
}

// Return a pointer to the rightmost, or largest, node
Dictionary::Node *Dictionary::findMax(Node* R){
    if(R == nil){
        return nil;
    }
    Node *curr = R;
    while (curr->right != nil)
    {
        curr = curr->right;
    }
    return curr;
}

// Sets current to be the smallest Node
void Dictionary::end(){
    if(num_pairs == 0){
        return;
    }
    current = findMax(root);
}

// Finds the node's sucessor, provided that the node is not the rightmost node
Dictionary::Node *Dictionary::findNext(Node *N){
    if(findMax(root) == N){
        return nil;
    }
    // Looks for the smallest value in the right subtree
    Node* temp = findMin(N->right);
    // If there isn't one, start at N, and move up the tree
    if(temp == nil){
        temp = N;
        // Checks to see if a left turn was made down the subtree, which signals the sucessor
        while(temp->parent->left != temp){
            temp = temp->parent;
        }
        temp = temp->parent;
    }
    return temp;
}

// Moves current to the current node's sucssessor
void Dictionary::next(){
    if(current == nil){
        throw std::out_of_range("Dictionary: next(): current is not defined");
    }

    current = findNext(current);

}

// Finds the node's predecessor 
Dictionary::Node *Dictionary::findPrev(Node *N){
    if(findMin(root) == N){
        return nil;
    }
    // Looks for the largest value in the left subtree
    Node* temp = findMax(N->left);
    // If there isn't one, start at N, and move up the tree
    if(temp == nil){
        temp = N;
        // Checks to see if a right turn was made down the subtree, which signals the predicesor 
        while(temp->parent->right != temp){
            temp = temp->parent;
        }
        temp = temp->parent;
    }
    return temp;
}

// Sets current to its predecessor
void Dictionary::prev(){
    if(current == nil){
        throw std::out_of_range("Dictionary: prev(): current is not defined");
    }

    current = findPrev(current);
}

// Creates a string of key value pairs with an in order ordering, starting at node R
void Dictionary::inOrderString(std::string& s, Node* R) const{
    if(R != nil){
        inOrderString(s, R->left);
        s += R->key + " : " + std::to_string(R->val) + "\n";
        inOrderString(s, R->right);
    }
}

// Returns a string of key value pairs seperated by '/n's 
std::string Dictionary::to_string() const{
    if(root == nil){
        return "";
    }
    std::string s = "";
    inOrderString(s, root);
    return s;
}

// Creates a string of keys seperate with '\n's in pre order
void Dictionary::preOrderString(std::string& s, Node* R) const{
    if(R != nil){
        s += R->key + "\n";
        preOrderString(s, R->left);
        preOrderString(s, R->right);
    }
}

// Returns a string of keys in pre order
std::string Dictionary::pre_string() const{
    if(root == nil){
        return "";
    }
    std::string s = "";
    preOrderString(s, root);
    return s;
}

// Returns if this equals d
bool Dictionary::equals(const Dictionary& D) const{
    if(to_string() == D.to_string()){
        return true;
    }
    return false;
}

// Overloads << with to_string
std::ostream& operator<<( std::ostream& stream, Dictionary& D ){
    return stream << D.to_string();
}

// Overloades == with .equals
bool operator==( const Dictionary& A, const Dictionary& B ){
    return A.equals(B);
}

// Copies D into this
Dictionary& Dictionary::operator=( const Dictionary& D ){
    clear();
    preOrderCopy(D.root, D.nil);
    return *this;
}