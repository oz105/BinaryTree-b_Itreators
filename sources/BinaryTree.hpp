#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <stack>
#include <queue>


namespace ariel {


    template <class T>
    class BinaryTree {
        
        struct Node {

            T data;
            Node* left;
            Node* right;
            Node(T value):data(value),left(nullptr),right(nullptr){}
        };

    private:
        Node* root = nullptr;

    public:

        BinaryTree(){}

        void copy(Node & src, Node& dst){
            if(dst.left) {
                src.left = new Node(dst.left->data);
                copy(*src.left,*dst.left);
            }
            if(dst.right) {
                src.right = new Node(dst.right->data);
                copy(*src.right, *dst.right);
            }
        }

        BinaryTree<T>(const BinaryTree<T> & other) { // copy constractor
            if(!other.root){
                root = other.root;
            }
            else{
                root = new Node(other.root->data);
                copy(*root,*(other.root));
            }
        }

        ~BinaryTree() { // distract
            if(root){
                delete root;
            }
        }


        BinaryTree<T> & operator = (const BinaryTree<T> other) { // operator "="
            if (this == &other) {return *this;}
            delete(root);
            root = new Node(other.root->data);
            copy(*root, *other.root);
            return *this;
        }

        BinaryTree<T> & operator = (BinaryTree<T> &&other) noexcept {
            delete root ; 
            root = other.root;
            other.root = nullptr;
            return *this;
        }

        BinaryTree<T> (BinaryTree<T> &&other) noexcept { // move constract
            root = other.root;
            other.root = nullptr;
        }

        BinaryTree<T> & add_root(const T &data) {
            if(!root) {
                root = new Node(data);
            }
            else{
                root->data = data;
            }
            return *this;
        }

        BinaryTree<T>& add_left(const T& parent,const T& left_child){
            if(root==nullptr){
                throw (std::string)"There is no root in this Tree";
            }
            std::stack<Node*> stack;
            Node *current = root;
            while (current != nullptr || !(stack.empty())){

                while (current !=  nullptr){
                    stack.push(current);
                    current = current->left;
                }

                current = stack.top();
                stack.pop();
                if(current->data==parent){
                    if(current->left!=nullptr){
                        current->left->data=left_child;
                    }
                    else{
                        current->left=new Node(left_child);
                    }
                    return *this;
                }
                current = current->right;
            }
            throw std::invalid_argument{"The parent was not found"};
        }

        BinaryTree<T>& add_right(const T& parent,const T& right_child){
            if(root==nullptr){
                throw (std::string)"There is no root in this Tree";
            }

            std::stack<Node*> stack;
            Node *current = root;
            while (current != nullptr || !stack.empty())
            {
                while (current !=  nullptr)
                {
                    stack.push(current);
                    current = current->left;
                }
                current = stack.top();
                stack.pop();
                if(current->data==parent){
                    if(current->right!=nullptr){
                        current->right->data=right_child;
                    }
                    else{
                        current->right=new Node(right_child);
                    }
                    return *this;
                }
                current = current->right;
            }
            throw std::invalid_argument{"The parent was not found"};
        }


        class inorder {

        public:
            std::stack<Node*> path;
            std::queue<Node*> order;
            Node* curr;

            inorder(Node* n) {
                bool flag = true;
                if (n) {
                    curr = n;
                    while(flag) {
                        if(curr) {
                            path.push(curr);
                            curr = curr->left;
                        }
                        else {
                            if(path.empty()) {
                                break;
                            }
                            curr = path.top();
                            order.push(curr);
                            curr = curr->right;
                            path.pop();
                        }
                    }
                    curr = order.front();
                }
                else {
                    curr = nullptr;
                }

            }

            inorder & operator ++ () { // without int flag its ++i
                order.pop();
                if (order.empty()) {curr = nullptr; return *this;}
                curr = order.front();
                return *this;
            }
            inorder operator ++ (int) { //with int flag its i++
                inorder tmp = *this;
                order.pop();
                if (order.empty()) {curr = nullptr; return tmp;}
                curr = order.front();
                return tmp;
            }

            T* operator ->() const {return &(order.front()->data);}
            T & operator *() const {return curr-> data;}
            bool operator == (const inorder & other) const {return (this->curr == other.curr);}
            bool operator != (const inorder & other) const {return this->curr != other.curr;}

        };

        inorder begin_inorder() {
            return inorder(root);
        }

        inorder end_inorder() {
            return (nullptr);
        }

        inorder begin() {
            return inorder(root);
        }

        inorder end() {
            return (nullptr);
        }

        class preorder {
        public:
            std::stack<Node*> path;
            std::queue<Node*> order;
            Node* curr;

            preorder(Node* n) {
                if (n) {
                    curr = n;
                    path.push(curr);
                    while((!path.empty())) {
                        curr = path.top();
                        order.push(curr);
                        path.pop();
                        curr = order.back();
                        if(curr->right){path.push(curr->right);}
                        if(curr->left){ path.push(curr->left);}
                    }
                    curr = order.front();
                } else {curr = nullptr;}

            }

            preorder & operator ++ () { //without int flag its ++i
                order.pop();
                if (order.empty()) {curr = nullptr; return *this;}
                curr = order.front();
                return *this;
            }
            preorder operator ++ (int) { //with int flag its i++
                preorder tmp = *this;
                order.pop();
                if (order.empty()) {curr = nullptr; return tmp;}
                curr = order.front();
                return tmp;
            }
            T* operator ->() const {return &(order.front()->data);}
            T & operator *() const {return curr->data;}
            bool operator == (const preorder & other) const {return curr == other.curr;}
            bool operator != (const preorder & other) const {return curr != other.curr;}
        };

        preorder begin_preorder() {
            return preorder(root);
        }

        preorder end_preorder() {
            return (nullptr);
        }

     class postorder {

        public:
            std::queue<Node*> order;
            Node* curr;

            postorder(Node* n){
                if(n) {
                    std::stack<Node*> stack_1;
                    std::stack<Node*> stack_2;
                    curr = n;
                    stack_1.push(curr);
                    while(!stack_1.empty()) {
                        stack_2.push(stack_1.top());
                        curr = stack_2.top();
                        stack_1.pop();
                        if(!curr){break;}
                        if(curr->left){stack_1.push(curr->left);}
                        if(curr->right){stack_1.push(curr->right);}
                    }
                    while(!stack_2.empty()) {
                        curr = stack_2.top();
                        order.push(curr);
                        curr = order.back();
                        stack_2.pop();
                    }
                    curr = order.front();
                } else {curr = nullptr;}
            }

            postorder & operator ++ () {
                order.pop();
                if (order.empty()) {curr = nullptr; return *this;}
                curr = order.front();
                return *this;
            }
            postorder operator ++ (int) {
                postorder tmp = *this;
                order.pop();
                if (order.empty()) {curr = nullptr; return tmp;}
                curr = order.front();
                return tmp;
            }
            T* operator ->() const {return &(order.front()->data);}
            T & operator *() const {return curr->data;}
            bool operator == (const postorder & other) const {return curr == other.curr;}
            bool operator != (const postorder & other) const {return curr != other.curr;}

        };

        postorder begin_postorder() {
            return postorder(root);
        }

        postorder end_postorder() {
            return (nullptr);
        }


        Node* get_root() const {
            return this->root;
        }

         // https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
        void print(std::ostream& os, const std::string &prefix, const Node *node, bool isLeft) const
        {
            if (node != nullptr)
            {
                std::cout << prefix;
                if (!isLeft)
                {
                    std::cout << "├──";
                }
                else
                {
                    std::cout << "└──";
                }
                std::cout << node->data << std::endl;
                // enter the next tree level - left and right branch
                printBT(prefix + (isLeft ? "│   " : "    "), node->right, false);
                printBT(prefix + (isLeft ? "│   " : "    "), node->left, true);
            }
        }

        static void print(std::ostream& os, const Node* node){
            print(os, "", node, false);    
        }


        friend std::ostream & operator << (std::ostream & os , const BinaryTree<T> & bt) {
            if (bt.root == nullptr){
                return os;
            }
            os<<"\n\n";

            print(os, bt.root);

            os<<"\n\n";
            return os;
   
        }
    };

}