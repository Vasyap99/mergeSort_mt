#include <vector>
#include <iostream>

#include <future>

using namespace std;



int NUM_THREADS(0);

#define MAX_THREADS 100


void merge(int* arr, int l, int m, int r)
{
   int nl = m - l + 1;
   int nr = r - m;

   // создаем временные массивы
   int left[nl], right[nr];

   // копируем данные во временные массивы
   for (int i = 0; i < nl; i++)
       left[i] = arr[l + i];
   for (int j = 0; j < nr; j++)
       right[j] = arr[m + 1 + j];

   int i = 0, j = 0;
   int k = l;  // начало левой части

   while (i < nl && j < nr) {
       // записываем минимальные элементы обратно во входной массив
       if (left[i] <= right[j]) {
           arr[k] = left[i];
           i++;
       }
       else {
           arr[k] = right[j];
           j++;
       }
       k++;
   }
   // записываем оставшиеся элементы левой части
   while (i < nl) {
       arr[k] = left[i];
       i++;
       k++;
   }
   // записываем оставшиеся элементы правой части
   while (j < nr) {
       arr[k] = right[j];
       j++;
       k++;
   }
}

void mergeSort(promise<int>& p,int* arr, int l, int r){
   if(l>=r){
      p.set_value(false);
      return;
   }
   int m=(l+r-1)/2;

   promise<int> p1,p2;

   if(NUM_THREADS<MAX_THREADS){

      NUM_THREADS+=2;

      thread t1(mergeSort, std::ref(p1), arr,l,m);
      thread t2(mergeSort, std::ref(p2), arr,m+1,r);

      future<int> f1 = p1.get_future(),
                  f2 = p2.get_future(); 

      //mergeSort(arr,l,m);
      //mergeSort(arr,m+1,r);

      try {
          f1.get();
      } catch (...) {
          cout << "Some error in thread" << endl;
      }

      try {
          f2.get();
      } catch (...) {
          cout << "Some error in thread" << endl;
      }

      t1.join();
      t2.join();

      NUM_THREADS-=2;

   }else{
      cout <<  "::2" <<endl;
      mergeSort(std::ref(p1), arr,l,m);
      mergeSort(std::ref(p2), arr,m+1,r);
   }

   merge(arr,l,m,r);   

   p.set_value(true);
}

void mergeSort1(int* arr, int l, int r){
   promise<int> p1;

   NUM_THREADS++;
   thread t1(mergeSort, std::ref(p1), arr,l,r);

   future<int> f1 = p1.get_future(); 

   try {
       f1.get();
   } catch (...) {
       cout << "Some error in thread" << endl;
   }

   t1.join();

   NUM_THREADS--;
}

void out_arr(int* a,int sz){
   for(int i=0;i<sz;i++){
      cout << a[i] << " ";
   }
   cout << endl;
}

int main(){
   vector<int> v1={1,45,2,-100,6,13,474,-23,-89,-100,8,4000,5,-4,2,6,2,-8,8,0};
   mergeSort1(v1.data(), 0, v1.size()-1);
   out_arr(v1.data(),v1.size());
   return 0;
}
