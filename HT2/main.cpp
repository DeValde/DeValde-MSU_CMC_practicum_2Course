#include <iostream>
#include <cstring>
using namespace std;

class mstring {
private:
    char * str;
    int l;
    char * buffer;
    int lold = 1;
    void copy(){
        for (int i = 0; i < l ; ++i) {

            buffer[i] = str[i];
        }
    }
    void paste(){
        for (int i = 0; i<lold ; ++i) {
            str[i]= buffer[i];
        }
    }
    int strlen(const char * s){
        int c = 0;
        while( s[c] != '\0')
            c++;

        return c;
    }
public:
    mstring(const char * s){
        int i = 0;
        while (s[i] != '\0')
            i++;
        l = i;
        str = new char [l+1];
        buffer = new char[l+1];
        for (int j = 0; j < i+1; ++j) {
            str[j]= s[j];
        }
    }

    mstring (){
        str = new char [1];
        str[0] = '\0';
        l = 1;
        buffer = new char[1];
    }
    mstring(const mstring &cop){
        str = new char [cop.l];
        buffer = new char [cop.l];
        l = cop.l;
        for (int i = 0; i < l; ++i) {
            str[i] = cop.str[i];
        }
    }
    ~mstring(){
        delete[] str;
    }

    int length() const;
    bool isempty() const;
    void add(char c);//, с - символ
    void add(const char* c);// с - строка
    void insert(char c,int i);// с - символ, i - позиция
    void insert(const char* c,int i);//, с - строка, i - позиция
    void del(int i); //i - позиция
    void del(int i, int j);// i, j - позиции задающие диапазон удаления с i по j включительно
    int search(const char* str);
    void replace(const char* sub_str, const char *new_str); //замена подстроки sub_str на new_str,
    void print();

};
#include <cstring>
int mstring::length() const {
    return l;
}
bool mstring::isempty() const{
    if (str[0] == '\0')
        return true;
    else return false;
}
void mstring::add(char c) {
    copy();
    lold = l;
    l+=1;
    delete[] str;
    str = new char [l];
    paste();
    str[lold] = c;
    str[lold+1] = '\0';
}
void mstring::add(const char *c) {
    int counter = 0;
    int a = 0;
    if (this->isempty()){
        l = strlen(c);
        str = new char [l];
        for (int i = 0; i < strlen(c) ; ++i) {
            str[i] = c[i];
        }
        return;
    }

    while (c[counter] != '\0')
        counter++;
    copy();
    delete[] str;
    lold = l;
    l+=counter+1;
    str = new char [l];
    paste();
    for (int i = lold; i <l ; ++i) {
        str[i] = c[a];
        a++;
    }

}

void mstring:: insert(char c,int i){
    int k = i;
    int j = 0;
    char cpy[l];
    char save[l];
    strcpy(cpy,str);
    delete[] str;
    l ++;
    str = new char [l];
    strcpy(str,cpy);
    while (str[k] !='\0'){
        save[j] = str[k];
        k++;
        j++;
    }
    str[i] = c;
    j = 0;
    for (int m = i+1; m < k+ 1+strlen(save) ; ++m) {
        str[m] = save[j];
        j++;
    }
}

void mstring::insert(const char* c,int i){
    int k = i;
    int j = 0;
    char cpy[l+1];
    strcpy(cpy,str);
    delete[] str;
    l = l+ strlen(c);
    str = new char [l];
    strcpy(str,cpy);
    buffer = new char [l];
    while (str[k] !='\0'){
        buffer[j] = str[k];
        k++;
        j++;
    }
    j=0;
    for (int m = i; m < i+ strlen(c) ; ++m) {
        str[m] = c[j];
        j++;
    }
    j=0;
    for (int m = i+ strlen(c); m < i+ strlen(c)+strlen(buffer) ; ++m) {
        str[m] = buffer[j];
        j++;
    }
}

void mstring::print(){
    for (int i = 0; i < l  ; ++i) {
        std::cout << str[i];
    }
    std::cout <<"\n";
}

void mstring::del(int i) {
    int k = 0;
    char cpy[l];
    for (int j = i+1; j <= l ; ++j) { // <= to take \0
        cpy[k] = str[j];
        k++;
    }
    k=0;
    for (int j = i; j <l ; ++j) {
        str[j] =cpy[k];
        k++;
    }
    l = l-1;

}

void mstring::del(int i, int j) { // probably a lot of copies in del, not good if string is huge.
    for (int k = i; k <=j ; ++k) {
        del(i);
    }
}

int mstring:: search(const char *s){ // could be optimized using KMP.
    int j = 0;
    for (int i = 0; i < l ; ++i) {
        int k=i;
        int ret = i;
        if (str[i] == s[0]){
            for (j; j < strlen(s) ; ++j) {
                if (str[k] == s[j])
                    k++;
                else break;
            }
            if (j == strlen(s) ){
                return ret;
            }
        }
    }
    return -1;
}

void mstring::replace(const char *sub_str, const char *new_str) {
    int idx = search(sub_str);
    if(idx == -1 )
        return;
    else(insert(new_str, idx));
}

int main()
{
    mstring s ("hello world");
    mstring s1 = s;
    mstring s2;
    cout << s.length() << endl;
    if (s2.isempty())
    {
        s.add('!');
        s2.add("HI!");
        s.print();
        s2.print();
    }
    if (!s1.isempty())
    {
        s1.insert(' ',5);
        s1.insert('m',6);
        s1.insert('y',7);
        s.insert(" my",5);
        s.print();
        s1.print();
    }
    s.del(9);
    s1.del(5,7);
    s.print();
    s1.print();
    return 0;
}
