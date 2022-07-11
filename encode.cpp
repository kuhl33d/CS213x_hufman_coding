//encode.cpp
#include<iostream>
#include<stdio.h>
#include<fstream>
#include"huff.h"
using namespace std;
int main(int argc, char const *argv[]){
    int size;
    list L;
    lnode *pnn;
    char r1,r2,fname[255];
    cout << "enter file name: ";
    gets(fname);
    {
        ifstream file1(fname,ios::binary);
        if(!file1.is_open()){
            cout << "file not found" << endl;
            return 1;
        }
        file1.seekg(0,ios::end);
        size = file1.tellg();
        // cout << size << endl;
        file1.seekg(0,ios::beg);
        for(int i=0,f=1,stop=0;i<size;i++,f=1,stop=0){
            file1.seekg(i,ios::beg);
            file1.read(&r1,1);
            if(!L.find(r1)){
                for(int j=i+1;j<size;j++){
                    file1.read(&r2,1);
                    if(r1==r2)
                        f++;
                }
                pnn = new lnode(r1,f);
                L.insert(pnn);
            }
        }
        file1.close();
    }
    lnode *n;
    pnode *p;
    while(L.length > 1){
        p = new pnode(L.head,L.head->next);
        n = new lnode(p);
        L.insert(n,1);//special insert
    }
    l_code code;
    n_code *trav;
    binary_node *b;
    code.find_leaf(L.head->down);
    // code.print_codes();
    code.print_codes();
    cout << "size: " << L.head->down->freq << endl;
    cout << "size unique: " << code.length << endl;
    cout << "max bits: " << code.max << "\nmin code: " << code.min << endl;
    // code.cypher(st,st_size);
    {
        //dictionary file:<st_size><size_decoded><max bit>{<letter><size><encoded>,....}
        char tmp;
        int m=0,tmp_m;
        tmp_m=code.max;
        while(tmp_m!=0){
            tmp_m>>=8;
            m++;
        }
        ofstream file("dictionary.bin",ios::binary | ios::out);
        //size_encoded
        file.write((const char *)(&L.head->down->freq),sizeof(L.head->down->freq));
        //size_decoded
        file.write((const char*)(&code.length),sizeof(code.length));
        //max bits for size
        file.write((const char *)(&code.max),sizeof(code.max));
        trav=code.head;
        while(trav!=NULL){
            file.write(&trav->l,sizeof(trav->l));
            file.write((const char*)(&trav->bits),m);
            // file.write((const char*)(&trav->bits),sizeof(int));
            b = trav->binary->head;
            while(b != NULL){
                tmp=0;
                for(int i=7;i>=8-b->size;i--){
                    ((1<<i)&b->bits)?(tmp |= (1<<i)):(tmp &= ~(1<<i));
                }
                file.write((const char*)(&tmp),sizeof(tmp));
                // file.write((const char*)(&b->bits),sizeof(b->bits));//only cause of recursion 1110 000
                b=b->next;
            }
            trav=trav->next;
        }
        file.close();
    }
    {
        binary_list encoded;
        ifstream file1(fname,ios::binary);
        for(int i=0;i<size;i++){
            file1.read(&r1,1);
            trav = code.find(r1);
            b=trav->binary->head;
            while(b != NULL){
                for(int i=7;i>=8-b->size;i--){
                    ((1<<i)&b->bits)?(encoded.attach(1)):(encoded.attach(0));
                }
                b = b->next;
            }
        }
        file1.close();
        ofstream file("encoded.bin",ios::binary | ios::out);
        b=encoded.head;
        while(b != NULL){
            file.write((const char*)(&b->bits),sizeof(b->bits));
            b=b->next;
        }
        file.close();
    }
    L.head->down->bye(L.head->down);//proot is not saved; recursive call in delete
    cout << "program ends" << endl;
    return 0;
}
//end encode.cpp