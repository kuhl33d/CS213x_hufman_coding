//huff.h
#include<iostream>
#include<fstream>
using namespace std;
class lnode;//forward declaration for errors
class pnode{
    public:
        char *st;
        int size;
        int freq;
        pnode *p_left,*p_right;
        pnode(){
            size=0;
            freq=0;
            p_left=NULL;
            p_right=NULL;
        }
        pnode(lnode *node);
        pnode(lnode *left_n,lnode *right_n);
        void bye(pnode *t){
            if(t==NULL)
                return;
            bye(t->p_left);
            bye(t->p_right);
            delete t;
        }
        ~pnode(){
            delete st;
        }
};
class lnode{
    public:
        char *st;
        int size;
        int freq;
        lnode *next;
        pnode *down;
        lnode(){
            st=NULL;
            size=0;
            next=NULL;
            down=NULL;
        }
        lnode(char l,int freq){//intial node
            this->freq=freq;
            st = new char;
            size=1;
            st[0]=l;
            next=NULL;
            down=NULL;
        }
        lnode(pnode *down){//new node
            next=NULL;
            this->freq=down->freq;
            this->down=down;
            this->size = down->size;
            this->st = new char[this->size];
            int w=0;
            for(int i=0;i<down->size;i++){
                this->st[w++] = down->st[i];
            }
        }
        ~lnode(){
            delete st;
        }
};
pnode::pnode(lnode *node){//leaf
    p_left=NULL;
    p_right=NULL;
    this->freq = node->freq;
    this->size=1;
    this->st = new char;
    this->st[0] = node->st[0];
}
pnode::pnode(lnode *left_n,lnode *right_n){
    if(left_n->down==NULL && right_n->down==NULL){//first time
        this->p_left = new pnode(left_n);
        this->p_right = new pnode(right_n);
        this->size = left_n->size + right_n->size;
        this->freq = left_n->freq + right_n->freq;
        this->st = new char[this->size];
        int w=0;
        for(int i=0;i<left_n->size;i++){
            this->st[w++] = left_n->st[i];
        }
        for(int i=0;i<right_n->size;i++){
            this->st[w++] = right_n->st[i];
        }
    }else if(left_n->down==NULL && right_n->down!=NULL){//tree in right
        this->p_right = right_n->down;
        this->p_left = new pnode(left_n);
        this->freq = left_n->freq + right_n->freq;
        this->size = left_n->size + right_n->size;
        this->st = new char[this->size];
        int w=0;
        for(int i=0;i<left_n->size;i++){
            this->st[w++] = left_n->st[i];
        }
        for(int i=0;i<right_n->size;i++){
            this->st[w++] = right_n->st[i];
        }
    }else if(right_n->down==NULL && left_n->down!=NULL){//tree in left
        this->p_left = left_n->down;
        this->p_right = new pnode(right_n);
        this->freq = left_n->freq + right_n->freq;
        this->size = left_n->size + right_n->size;
        this->st = new char[this->size];
        int w=0;
        for(int i=0;i<right_n->size;i++){
            this->st[w++] = right_n->st[i];
        }
        for(int i=0;i<left_n->size;i++){
            this->st[w++] = left_n->st[i];
        }
    }else{
        this->p_left = left_n->down;
        this->p_right = right_n->down;
        this->freq = left_n->freq + right_n->freq;
        this->size = left_n->size + right_n->size;
        this->st = new char[this->size];
        int w=0;
        for(int i=0;i<left_n->size;i++){
            this->st[w++] = left_n->st[i];
        }
        for(int i=0;i<right_n->size;i++){
            this->st[w++] = right_n->st[i];
        }
    }
}
class binary_node{
public:
    unsigned char bits;
    unsigned int size:4;//0 to 15 only need to 8
    binary_node *next,*prev;
    binary_node(){
        bits = 0;
        next=NULL;
        prev=NULL;
        size=0;
    }
    binary_node(binary_node *S){
        this->bits=S->bits;
        this->size=S->size;
        this->next=NULL;
        this->prev=NULL;
    }
};
class binary_list{
public:
    binary_node *head,*tail;
    unsigned int size;
    binary_list(){
        head=tail= new binary_node();
        size=0;
    }
    binary_list(binary_list *S){
        this->size=S->size;
        this->head=this->tail=NULL;
        binary_node *t = S->head,*tmp;
        while(t!=NULL){
            tmp = new binary_node(t);
            if(this->head==NULL){
                this->head=this->tail=tmp;
            }else{
                this->tail->next=tmp;
                tmp->prev=this->tail;
                this->tail=tmp;
            }
            t=t->next;
        }

    }
    ~binary_list(){
        binary_node *t;
        while(head != NULL){
            t = head->next;
            delete head;
            head = t;
        }
    }
    void attach(int v){
        size++;
        if(tail->size==8){
            binary_node *tmp = new binary_node();
            tail->next = tmp;
            tmp->prev=tail;
            tail=tmp;
        }
        if(v==0){
            tail->bits &= ~(1<<(7-tail->size));
            tail->size++;
        }else{
            tail->bits |= (1<<(7-tail->size));
            tail->size++;
        }
    }
    void remove(){
        if(size!=0){
            size--;
            if(tail->size==0){
                binary_node *t = tail->prev;
                delete tail;
                t->next=NULL;
                tail=t;
            }
            tail->size--;
        }
    }
    void print(){
        binary_node *t = head;
        while(t != NULL){
            for(int i=7;i>=8-t->size;i--){
                ((1<<i)&t->bits)?(cout << 1):(cout << 0);
            }
            t = t->next;
        }
    }
};

class n_code{
    public:
        char l;
        binary_list *binary;
        int bits;
        int freq;
        n_code *next;
        n_code(){
            next=NULL;
            binary= new binary_list;
            bits=0;
            freq=0;
            l='\0';
        }
        n_code(char l,binary_list *binary,int bits,int freq){
            next=NULL;
            this->binary = new binary_list(binary);
            this->bits=bits;
            this->l=l;
            this->freq=freq;
        }
        ~n_code(){
            delete binary;
        }

};
class l_code{
    public:
        int length,max,min;
        n_code *head;
        n_code *tail;
        n_code *pnn;
        binary_list *tmp_code;
        int i;
        l_code(){
            i=0;
            length=0;
            head=tail=NULL;
            tmp_code= new binary_list();
            max=-1;
            min=128;
        }
        void insert(n_code *pnn){
            //
            if(pnn->bits > max)
                max=pnn->bits;
            if(pnn->bits < min)
                min=pnn->bits;
            //
            length++;
            if(head==NULL)
                head=tail=pnn;
            else if(pnn->bits >= tail->bits){
                tail->next=pnn;
                tail=pnn;
            }else if(pnn->bits <= head->bits){
                pnn->next=head;
                head=pnn;
            }else{
                n_code *t = head;
                while(t->next->bits<pnn->bits){
                    t=t->next;
                }
                pnn->next=t->next;
                t->next=pnn;
            }
        }
        n_code* find(char s){
            if(length==0)
                return NULL;
            n_code *t = head;
            while(t != NULL){
                if(t->l == s)
                    return t;
                t = t->next;
            }
            return NULL;
        }
        void cypher(char *st,int size){
            cout << "cypher: " << endl;
            for(int i=0;i<=size;i++){
                pnn=find(st[i]);
                if(pnn!=NULL){
                    pnn->binary->print();
                }
            }
            cout << endl << "end cypher" << endl;
        }
        
        void print_codes() {
            cout << "letter<hex>::freq::b_size::bits<bin>" << endl;
            n_code *trav = head;
            while(trav!=NULL){
                cout << '\'' ;
                printf ("%2x",trav->l);
                cout << '\'' << "::" << trav->freq << "::" << trav->bits <<  "::";
                trav->binary->print();
                cout << endl;
                trav = trav->next;
            }
            cout << endl;
        }
        void find_leaf(pnode *t){
            if(t->p_left==NULL && t->p_right==NULL){
                pnn = new n_code(t->st[0],tmp_code,i,t->freq);
                this->insert(pnn);
                // cout << '\'' << t->st[0] << "': f:" << t->freq << ' ';
                // tmp_code->print();
                // cout << endl;
                tmp_code->remove();
                i--;
                return;
            }
            tmp_code->attach(0);
            i++;
            find_leaf(t->p_left);
            tmp_code->attach(1);
            i++;
            find_leaf(t->p_right);
            tmp_code->remove();
            i--;
        }
        ~l_code(){
            n_code *t;
            while(head!=NULL){
                t=head->next;
                delete head;
                head=t;
            }
        }
};

class list{
    public:
        int length;
        lnode *head;
        lnode *tail;
        list(){
            length=0;
            head=tail=NULL;
        }
        int find(char s){
            if(length==0)
                return 0;
            lnode *t = head;
            while(t != NULL){
                if(t->st[0] == s)
                    return 1;
                t = t->next;
            }
            return 0;
        }
        void insert(lnode *pnn){
            length++;
            if(head==NULL)
                head=tail=pnn;
            else if(pnn->freq >= tail->freq){
                tail->next=pnn;
                tail=pnn;
            }else if(pnn->freq <= head->freq){
                pnn->next=head;
                head=pnn;
            }else{
                lnode *t = head;
                while(t->next->freq<pnn->freq){
                    t=t->next;
                }
                pnn->next=t->next;
                t->next=pnn;
            }
        }
        void insert(lnode *pnn,int i){
            length-=2;
            lnode *tmp=head->next->next;
            delete head->next;
            delete head;
            head = tmp;
            insert(pnn);
        }
        ~list(){
            lnode *t;
            while(head!=NULL){
                t=head->next;
                delete head;
                head=t;
            }
        }
};
class tree_decode_node{
public:
    char l;
    int size;
    tree_decode_node *left;
    tree_decode_node *right;
    tree_decode_node(){
        l=0;
        size=0;
        left=right=NULL;
    }
};
class tree_decode{
public:
    tree_decode_node *root;
    tree_decode_node *curr;
    tree_decode(){
        root= new tree_decode_node;
        curr=root;
    }
    void make(char l,binary_list *b){
        binary_node *t = b->head;
        tree_decode_node *pnn;
        while(t != NULL){
            for(int i=7;i>=8-t->size;i--){
                if((1<<i)&t->bits){
                    if(curr->right == NULL){
                        pnn=new tree_decode_node;
                        curr->right=pnn;
                    }
                    curr=curr->right;    
                }else{
                    if(curr->left == NULL){
                        pnn=new tree_decode_node;
                        curr->left=pnn;
                    }
                    curr=curr->left;
                }
            }
            t = t->next;
        }
        curr->l=l;
        curr=root;
    }
    int find(int i,char &l){
        if(i == 1){
            curr=curr->right;
        }else{
            curr=curr->left;
        }
        if(curr->left==NULL && curr->right==NULL){
            l=curr->l;
            curr=root;
            return 1;
        }
        return 0;
    }
    ~tree_decode(){
        bye(root);
    }
    void bye(tree_decode_node *t){
        if(t==NULL)
            return;
        bye(t->left);
        bye(t->right);
        delete t;
    }
};
//end huff.h