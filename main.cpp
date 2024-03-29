#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>
#include <string>
#include <sstream>
#include <cassert>

using namespace std;

ifstream in("input.txt");
ofstream out("output.txt");

class Vector
{
        int dimensiune;
        int *data;
        public:
            int get_dim();
            int* get_data();
            int get_data(int i);
            void set_dim(int x);
            void set_data(int i,int p);
            Vector();
            Vector(int dim);
            ~Vector();
            Vector operator =(const Vector &ob);
            friend ifstream & operator >> (ifstream &in, Vector &L)
            {
                in>>L.dimensiune;
                for(int i=0;i<L.dimensiune;i++)
                    in>>L.data[i];
                return in;
            }
            friend ofstream & operator << (ofstream &out, Vector &L)
            {
                for(int i=0;i<L.dimensiune;i++)
                    out<<L.data[i]<<" ";
                return out;
            }
            int operator[](int val);

};

int Vector:: get_data(int i)
{
    assert(i < dimensiune);
    return data[i];
}

void Vector:: set_data(int i,int p)
{
    assert(i < dimensiune);
    data[i]=p;
}

void Vector:: set_dim(int x)
{
    dimensiune=x;
    delete []data;
    data = new int[x];
}

int Vector:: get_dim()
{
    return dimensiune;
}

int* Vector::  get_data()
{
    return data;
}

Vector::Vector()
{
    dimensiune=2;
    data=new int[2];
    data[0]=0;
    data[1]=0;
}

Vector::Vector(int dim)
{
    dimensiune=dim;
    data= new int[dim];
    for(int i=0;i<dimensiune;i++)
        data[i]=i;
}

Vector::~Vector()
{
    delete []data;
}

Vector::operator[](int val)
{
        return data[val];
}

Vector Vector::operator=(const Vector &ob)
{
    if(this!=&ob)
    {
        dimensiune=ob.dimensiune;
        delete []data;
        data= new int[dimensiune];
        for(int i=0;i<dimensiune;i++)
            data[i]=ob.data[i];
    }
    return *this;

}

class Matrice{
        Vector *v;
    public:
        Matrice();
        Vector *get_v() const;
        void set_v_dim(int dim);
        void delete_v();
        virtual string get_type()=0;
};

void Matrice::delete_v()
{
    if(v!=NULL)
    {
         delete []v;
         v=NULL;
    }
}

Matrice::Matrice()
{
    v=NULL;
}

void Matrice:: set_v_dim(int dim)
{
    delete_v();
    v=new Vector[dim];
    for(int i = 0; i < dim; ++i)
        v[i].set_dim(dim);
}

Vector* Matrice::get_v()const
{
     return v;
}

class Matrice_patratica:public Matrice
{
    int dim;
    public:
        Matrice_patratica(int d);
        Matrice_patratica();
        Matrice_patratica(const Matrice_patratica &M);
        ~Matrice_patratica();
        Matrice_patratica operator=(const Matrice_patratica &M);
        string get_type();
        friend ifstream & operator >> (ifstream &in, Matrice_patratica &M)
        {
            in>>M.dim;

            for(int i=0;i<M.dim;i++)
            {
                Vector p=M.get_v()[i];
                for(int j=0;j<M.dim;j++)
                {
                    int q;
                    in>>q;
                    p.set_data(j,q);
                }
            }
            return in;
        }
        friend ofstream & operator << (ofstream &out, Matrice_patratica &M)
        {
            for(int i=0;i<M.dim;i++)
            {
                for(int j=0;j<M.dim;j++)
                    out<<M.get_v()[i].get_data()[j]<<" ";
                out<<"\n";
            }
            out<<M.determinant()<<"\n";
            M.patratica();
            M.afis_diagonala();
            M.afis_triunghiulara();
            return out;
        }
        int get_dimm();
        Vector & operator[](const int &val) const;
        string triunghiulara();
        void afis_triunghiulara();
        bool diagonala();
        void afis_diagonala();
        void patratica();
        int determinant();
        Matrice_patratica matrice_rez( int l, int c);
};

int Matrice_patratica:: get_dimm()
{
    return dim;
}

string Matrice_patratica:: get_type()
{
    string B="Matrice patratica de dimensiune ";
    B=B+to_string(dim);
    return B;
}

int verif (int k, int s[])
{
    for (int i=0; i<k; i++)
        if  (s[i]==s[k])
            return 0;
    return 1;
}

int prelucrare(int s[], Matrice_patratica &A)
{
    int nr_inv=0;
    for (int i=1; i<A.get_dimm(); i++)
    {
        for (int j=0; j<i; j++)
            if (s[i]<s[j])  nr_inv++;
    }
    int semnul_permutarii=pow(-1,nr_inv);
    int PRODUS=semnul_permutarii;
    for (int i=0;i<A.get_dimm();i++)
        PRODUS=PRODUS*A.get_v()[i].get_data(s[i]);
    return PRODUS;
}

Matrice_patratica::Matrice_patratica()
{
    dim=1;
    set_v_dim(1);
    int *p=get_v()[0].get_data();
    p=new int[1];
    p[0]=0;
}

void backtracking(int k, int s[], Matrice_patratica &A , int *REZ)
{
    if (k==A.get_dimm())
        (*REZ)=(*REZ)+prelucrare(s, A);
    else
    {
        for (int i=0; i<A.get_dimm(); i++)
        {
            s[k]=i;
            if (verif(k,s)!=0)
                backtracking(k+1, s, A, REZ);
        }
    }
}

int Matrice_patratica:: determinant()
{
    int n=get_dimm();
    int s[n];
    for (int i=0;i<n;i++)
        s[i]=0;
    int REZ=0;
    backtracking(0, s, *this, &REZ);
    return REZ;
}

void Matrice_patratica:: afis_triunghiulara()
{
    string s;
    s=triunghiulara();
    out<<s<<"\n";
}

void Matrice_patratica:: patratica()
{
    out<<"true"<<"\n";
}

bool Matrice_patratica:: diagonala()
{
    for(int i=0;i<dim;i++)
    {
        int *p=get_v()[i].get_data();
        for(int j=0;j<dim;j++)
            if(p[j]!=0 && i!=j)
                return false;
    }
    return true;
}

void Matrice_patratica:: afis_diagonala()
{
    if(diagonala()==1)
        out<<"true"<<"\n";
    else
        out<<"false"<<"\n";
}

string Matrice_patratica:: triunghiulara()
{
    int superior=1,inferior=1;
    for(int i=0;i<dim;i++)
    {
        int *p=get_v()[i].get_data();
        for(int j=i+1;j<dim;j++)
        {
            if(p[j]!=0)
                superior=0;
        }
    }
    for(int i=0;i<dim;i++)
    {
        int *p=get_v()[i].get_data();
        for(int j=0;j<i;j++)
        {
            if(p[j]!=0)
                inferior=0;
        }
    }
    if(superior==1 && inferior==1)
        return "triunghiulara superior si inferior";
    else
    {
        if(superior==1)
            return "triunghiulara superior";
        if(inferior==1)
            return "triunghiulara inferior ";
        if(inferior==0 && superior==0)
            return "netriunghiulara";
    }

}

Matrice_patratica::Matrice_patratica(int d){
    dim=d;
    set_v_dim(d);
    for(int i=0;i<d;i++)
    {
        for(int j=0;j<d;j++)
        {
            get_v()[i].set_data(j, j);
        }
    }
}

Matrice_patratica::~Matrice_patratica()
{
    delete_v();
}

Matrice_patratica::Matrice_patratica(const Matrice_patratica &M){
    dim=M.dim;
    set_v_dim(dim);
    for(int i=0;i<dim;i++)
    {
        int *p=get_v()[i].get_data();
        int *q=M.get_v()[i].get_data();
        for(int j=0;j<dim;j++)
        {
            p[j]=q[j];
        }
    }
}

Matrice_patratica Matrice_patratica:: operator=(const Matrice_patratica &M){
    dim=M.dim;
    for(int i=0;i<dim;i++)
    {
        int *p=get_v()[i].get_data();
        int *q=M.get_v()[i].get_data();
        for(int j=0;j<dim;j++)
            p[j]=q[j];
    }
    return *this;
}

Vector & Matrice_patratica:: operator[](const int &val) const
{
    Vector p=get_v()[val];
    return p;
}

class Matrice_oarecare:public Matrice
{
    int l;
    int c;
    public:
        Matrice_oarecare(int l,int c);
        Matrice_oarecare();
        Matrice_oarecare(const Matrice_oarecare &M);
        ~Matrice_oarecare();
        Matrice_oarecare operator=(const Matrice_oarecare &M);
        string get_type();
        friend ifstream & operator >> (ifstream &in, Matrice_oarecare &M)
        {
            in>>M.l;
            in>>M.c;
            for(int i=0;i<M.l;i++)
            {
                for(int j=0;j<M.c;j++)
                {
                    in>>M.get_v()[i].get_data()[j];
                }
            }
            return in;
        }
        friend ofstream & operator << (ofstream &out, Matrice_oarecare &M)
        {
            for(int i=0;i<M.l;i++)
            {
                for(int j=0;j<M.c;j++)
                    out<<M.get_v()[i].get_data(j)<<" ";
                out<<"\n";
            }

            if(M.c==M.l)
            {
                Matrice_patratica A(M.c);
                out<<A.determinant()<<"\n";
                A.patratica();
                A.afis_diagonala();
                A.afis_triunghiulara();
            }
            else
                M.patratica();

            return out;
        }
        int* & operator[](const int &val) const;
        void patratica();

};

string Matrice_oarecare:: get_type()
{
    string A="Matrice ";
    A=A+to_string(l);
    A=A+" x ";
    A=A+to_string(c);

    return A;
}

void Matrice_oarecare::patratica()
{
    if(c!=l)
        out<<"false"<<"\n";
    else
        out<<"true"<<"\n";
}

Matrice_oarecare::Matrice_oarecare(){
    set_v_dim(1);
    for(int i=0;i<1;i++)
    {
        int *p=get_v()[i].get_data();
        for(int j=0;j<1;j++)
        {
            p[j]=j;
        }
    }
}

Matrice_oarecare::Matrice_oarecare(int l, int c){
    l=l;
    c=c;
    set_v_dim(l);
    for(int i=0;i<l;i++)
    {
        int *p=get_v()[i].get_data();
        p=new int[c];
    }
    for(int i=0;i<l;i++)
    {
        int *p=get_v()[i].get_data();
        for(int j=0;j<c;j++)
        {
            p[j]=j;
        }
    }
}

Matrice_oarecare::~Matrice_oarecare(){
    for(int i=0;i<c;i++)
    {
        int *p=get_v()[i].get_data();
        delete []p;
    }
    delete_v();
}

Matrice_oarecare::Matrice_oarecare(const Matrice_oarecare &M){
    l=M.l;
    c=M.c;
    set_v_dim(l);
    for(int i=0;i<l;i++)
    {
         int *p=get_v()[i].get_data();
         p=new int[c];
    }
    for(int i=0;i<l;i++)
    {
        int *p=get_v()[i].get_data();
        int *q=M.get_v()[i].get_data();
        for(int j=0;j<c;j++)
        {
            p[j]=q[j];
        }
    }
}

Matrice_oarecare Matrice_oarecare:: operator=(const Matrice_oarecare &M){
    l=M.l;
    c=M.c;
    set_v_dim(l);
    for(int i=0;i<l;i++)
    {
        int *p=get_v()[i].get_data();
        int *q=M.get_v()[i].get_data();
        for(int j=0;j<c;j++)
            p[j]=q[j];
    }
    return *this;
}

int* & Matrice_oarecare:: operator[](const int &val) const
{
    int *p=get_v()[val].get_data();
    return p;
}

int main()
{
    Matrice_oarecare A(5,5);
    in>>A;
    out<<A;
    Matrice *bptr;
    bptr= &A;
    out<<bptr->get_type();
    return 0;
}
