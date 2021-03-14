#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <random>
#include <chrono>
#include <time.h>
#include <algorithm>



using namespace std;
using namespace std::chrono;

#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

ifstream fin("sortari.in");
//ofstream fout("sortari.out");

bool check(vector<int> v, int n)
{
    int i;
    for(i = 0; i < n-1; i++)
        if(v[i] > v[i + 1]) return 0;
    return 1;
}

///bubble sort

vector<int> bubble_sort(vector<int> v,int m)
{
    bool ok = 0;
    int i;

    while(ok == 0)
    {
        ok = 1;
        for(i = 0; i < m - 1; i++)
            if(v[i] > v[i + 1])
            {
                swap(v[i], v[i + 1]);
                ok = 0;
            }

    }
    return v;
}
/////count sort

vector<int> count_sort(vector<int> v, int n, int max_el)
{
    ///vector de frecventa
    int* f = new int[max_el + 1];
    int i;
    for(i = 0; i < max_el; i++) f[i] = 0;
    vector<int> aux;
    for(i = 0; i < n; i++)
        f[v[i]]++;
    for(i = 0; i <= max_el; i++)
        while(f[i]--)
            aux.push_back(i);

    delete[] f;
    return aux;

}

///quicksort utilizand mediana de 3

int pivotmediana(int a, int b, int c)
{
    ///verificam daca a este exclusiv mai mare decat b, c
    /// <=> este mediana
    if ((a > b) ^ (a > c))
        return a;
    ///verificam acelasi lucru pentru b
    else if ((b < a) ^ (b < c))
        return b;
    else
        return c;
}

void quick_sort3(vector<int> &v, int st, int dr)
{
    int mij = st + (dr - st)/2;
    int i = st, j = dr;
    int p = pivotmediana(v[st], v[mij], v[dr]);
    while(i < j)
    {
        while(v[i] < p) i++;
        while(v[j] > p) j--;
        if(i <= j) swap(v[i++], v[j--]);
    }
    if(j > st) quick_sort3(v, st, j);
    if(i < dr) quick_sort3(v, i, dr);


}
///// quicksort utilizand un pivot random

void quick_sort(vector<int> &v, int st, int dr)
{
    int i = st, j = dr;
    int p = v[rand()% (dr-st+1) + st];
    while(i < j)
    {
        while(v[i] < p) i++;
        while(v[j] > p) j--;
        if(i <= j) swap(v[i++], v[j--]);
    }
    if(j > st) quick_sort(v, st, j);
    if(i < dr) quick_sort(v, i, dr);
}


//
/////mergesort

void merge(vector<int> &v, int st, int mij, int dr)
{
    int i, j;
    vector<int> aux;

    i = st;
    j = mij + 1;
    while(i <= mij && j <=dr)
        if(v[i] < v[j])
            aux.push_back(v[i++]);
        else aux.push_back(v[j++]);
    while(i <= mij)
        aux.push_back(v[i++]);
    while(j <= dr)
        aux.push_back(v[j++]);
    //modificam vectorul v
    //inlocuind elementele din v cu cele din aux
    for(i = st; i <= dr; i++)
        v[i] = aux[i - st];
}

void merge_sort(vector<int> &v, int st, int dr)
{
    if(dr > st)
    {
        int mij = st + (dr - st) / 2;
        merge_sort(v, st, mij);
        merge_sort(v, mij + 1, dr);
        merge(v, st, mij, dr);
    }
}


/////radix sort baza putere a lui 2 -> 2^8 = 256

void radix_sort256(vector<int> &v, int n)
{
    queue<int> q[256];
    int k;
    for(int i = 0; i <= 24; i += 8)
    {
        for(int j = 0; j < n; ++j)
            q[(v[j]>>i)&255].push(v[j]);
        k = 0;
        for(int j = 0; j <= 255 && k < n; ++j)
            while(!q[j].empty())
            {
                v[k++] = q[j].front();
                q[j].pop();
            }
    }
}

/////radix sort dar in baza 2^4
void radix_sort16(vector<int> &v, int n)
{
    queue<int> q[16];
    int k;
    for(int i = 0; i <= 24; i += 4)
    {
        for(int j = 0; j < n; ++j)
            q[(v[j]>>i)&15].push(v[j]);
        k = 0;
        for(int j = 0; j <= 15 && k < n; ++j)
            while(!q[j].empty())
            {
                v[k++] = q[j].front();
                q[j].pop();
            }
    }
}


int main()
{
    vector<int> v;
    int i, j, t;
    int T, max, n;
    fin >> T;
    for(t = 1; t <= T; t++)
    {
        v.clear();
        fin >> n >> max;
        for(i = 0; i < n; i++)
            v.push_back(rand() % max + 1);
        vector<int> aux;

//        cout << "auoleu " << "test no." << t <<":" <<"\n";
//        ///bubblesort test
//        //cout << "TESTUL " << t << " (n = " << n << " maxValue = " << max << ")" << '\n';
//        if(n > 10000) cout << "BubbleSort.....Failed(n is too large)";
//        else
//        {
//            auto Start = high_resolution_clock::now();
//            aux = bubble_sort(v, n);
//            auto Stop = high_resolution_clock::now();
//            if(check(aux, n))
//            {
//                auto Duration = duration_cast<milliseconds>(Stop-Start);
//                cout << "BubbleSort - SUCCEEDED - " << Duration.count() << " ms\n";
//
//            }
//            else cout << "BubbleSort - FAILED (sorted incorrectly)\n";
//        }
        //cout << n << " " << v.size() <<"\n";
        clock_t start;
        clock_t end;


        double elapsed;



        /// Bubble sort - n <= 10^6
        if(n < 100000 )
        {
            start = clock();

            aux = bubble_sort(v, n);

            end = clock();
            elapsed = double(end - start) / CLOCKS_PER_SEC;
            if(check(aux, n) == 1)
                cout<<"SUCCESS   "<<"Timpul de executare Bubble Sort este "<<elapsed<<" secunde\n";
            else
                cout<<"Bubble Sort FAILED:(\n";
        }
        else
            cout<<"Bubble Sort FAILED.......n prea mare!\n";

        ///pt countsort

        if(max >= 1000000) cout << "Count Sort: FAILED ... max prea mare!\n";
        else if(n >= 100000000) cout << "Count Sort: FAILED ... n prea mare!\n";
        else
        {
            start = clock();

            aux = count_sort(v, n, max);

            end = clock();
            elapsed = double(end - start) / CLOCKS_PER_SEC;
            if(check(aux, n))
                cout<<"SUCCESS   "<<"Timpul de executare CountSort este "<<elapsed<<" secunde\n";
            else
                cout<<"CountSort FAILED:(\n";

        }

        ///pt quicksort cu mediana de 3

        if(n > 100000000) cout << "Quick Sort3 - FAILED (n is too large)\n";
        else
        {
            aux = v;
            start = clock();

            quick_sort3(aux, 0, n - 1);
            end = clock();
            elapsed = double(end - start) / CLOCKS_PER_SEC;
            if(check(aux, n))
                cout<<"SUCCESS   "<<"Timpul de executare QuickSort3 este "<<elapsed<<" secunde\n";
            else
                cout<<"QuickSort3 FAILED:(\n";
        }

        ///pt quicksort cu pivot random

        if(n > 100000000) cout << "Quick Sort - FAILED (n is too large)\n";
        else
        {
            aux = v;
            start = clock();
            quick_sort(aux, 0, n - 1);
            end = clock();
            elapsed = double(end - start) / CLOCKS_PER_SEC;
            if(check(aux, n))
                cout<<"SUCCESS   "<<"Timpul de executare QuickSort este "<<elapsed<<" secunde\n";
            else
                cout<<"QuickSort FAILED:(\n";
        }
        ///pt mergesort

        if(n > 10000000) cout << "MergeSort - FAILED (n is too large)\n";
        else
        {
            aux = v;
            start = clock();
            merge_sort(aux, 0, n - 1);
            end = clock();
            elapsed = double(end - start) / CLOCKS_PER_SEC;
            if(check(aux, n))
                cout<<"SUCCESS   "<<"Timpul de executare MergeSort este "<<elapsed<<" secunde\n";
            else
                cout<<"MergeSort FAILED:(\n";

        }

        ///radix sort cu 256
        if(n > 100000000) cout << "RadixSort - FAILED (n is too large)\n";
        else
        {
            aux = v;
            start = clock();
            radix_sort256(aux, n);
            end = clock();
            elapsed = double(end - start) / CLOCKS_PER_SEC;
            if(check(aux, n))
                cout<<"SUCCESS   "<<"Timpul de executare RadixSort este "<<elapsed<<" secunde\n";
            else
                cout<<"RadixSort FAILED:(\n";
        }

        ///radix sort cu 16
        if(n > 100000000) cout << "RadixSort - FAILED (n is too large)\n";
        else
        {
            aux = v;
            start = clock();
            radix_sort16(aux, n);
            end = clock();
            elapsed = double(end - start) / CLOCKS_PER_SEC;
            if(check(aux, n))
                cout<<"SUCCESS   "<<"Timpul de executare RadixSort este "<<elapsed<<" secunde\n";
            else
                cout<<"RadixSort FAILED:(\n";
        }

        ///sortul nativ
        if(n > 100000000) cout << "NativeSort - FAILED (n is too large)\n";
        else
        {
            aux = v;
            start = clock();
            sort(aux.begin(), aux.end());

            end = clock();
            elapsed = double(end - start) / CLOCKS_PER_SEC;
            cout<<"SUCCESS   "<<"Timpul de executare NativeSort este "<<elapsed<<" secunde\n";

        }
        cout << "/-----------------------------/\n";
    }



    return 0;
}