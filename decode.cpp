//decode.cpp
#include<iostream>
#include<fstream>
#include"huff.h"
using namespace std;
int main(){
    int size_decoded,size_encoded,max,max_bytes=0,binary_bytes;
    binary_list *b;
    tree_decode tree;
    {
        char l,byte;
        int bits,tmp_max;
        ifstream file("dictionary.bin",ios::binary);
        if(!file.is_open()){
            cout << "dictionary.bin not found" << endl;
            return 1;
        }
        file.read((char*)(&size_decoded),sizeof(int));
        file.read((char*)(&size_encoded),sizeof(int));
        file.read((char*)(&max),sizeof(int));
        cout << "size: " << size_decoded << " unique: " << size_encoded << endl;
        tmp_max = max;
        while (tmp_max!=0){
            tmp_max >>= 8;
            max_bytes++;
        }
        for(int i=0;i<size_encoded;i++){
            file.read(&l,sizeof(char));
            file.read((char*)(&bits),max_bytes);
            // file.read((char*)(&bits),sizeof(int));
            bits<<=8*(4-max_bytes);
            bits>>=8*(4-max_bytes);
            if(bits>8){
                binary_bytes = bits/8;
                if(bits%8!=0){
                    binary_bytes++;
                }
            }else{
                binary_bytes = 1;
            }
            b = new binary_list;
            for(int j=0,k=bits;j<binary_bytes;j++,k-=8){
                file.read(&byte,sizeof(char));
                if(k>=8){
                    for(int l=7;l>=0;l--){
                        ((1<<l)&byte)?(b->attach(1)):(b->attach(0));
                    }
                }else{
                    for(int j=7;j>=8-(k%8);j--){
                        ((1<<j)&byte)?(b->attach(1)):(b->attach(0));
                    }
                }
            }
            cout << '\'';
            printf("%x",l);
            cout << "\' " << b->size << " "; 
            b->print();
            cout << endl;
            tree.make(l,b);
            delete b;
        }
        file.close();
    }
    char byte,l;
    int decoded=0;
    b= new binary_list;
    ifstream file("encoded.bin",ios::binary);
    if(!file.is_open()){
        cout << "encoded.bin not found" << endl;
        return 1;
    }
    file.seekg(0,ios::end);
    int size = file.tellg();
    file.seekg(0,ios::beg);
    for(int i=0;i<size;i++){
        file.read(&byte,sizeof(byte));
        for(int j=7;j>=0;j--){
            ((1<<j)&byte)?(b->attach(1)):(b->attach(0));
        }
    }
    int c;
    file.close();
    char dname[255];
    cout << "enter decoded file name: ";
    gets(dname);
    cout << "show bytes 0 or 1? ";
    cin >> c;
    ofstream out(dname,ios::binary);
    binary_node *t = b->head;
    while(t != NULL && decoded < size_decoded){
        for(int i=7;i>=8-t->size && decoded < size_decoded;i--){
            if((1<<i)&(t->bits)){
                if(tree.find(1,l)){
                    decoded++;
                    out.write(&l,sizeof(l));
                    if(c)
                        printf("%x ",l);
                }
            }else{
                if(tree.find(0,l)){
                    decoded++;
                    out.write(&l,sizeof(l));
                    if(c)
                        printf("%x ",l);
                }
            }
        }
        t=t->next;
    }
    out.close();
    delete b;
    return 0;
}
//end decode.cpp