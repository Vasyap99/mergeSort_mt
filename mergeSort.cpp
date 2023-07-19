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

void mergeSort(int* arr, int l, int r){
   if(l>=r){
      return;
   }
   int m=(l+r-1)/2;

   if(NUM_THREADS<MAX_THREADS){

      bool b1=true,b2=true;

      future<void> f1,f2;

      try{
	      f1 = async(launch::async,mergeSort,  arr,l,m);
          NUM_THREADS++;
	  }catch(...){
          mergeSort( arr,l,m);
		  b1=false;
	  }

      try{
          f2 = async(launch::async,mergeSort,  arr,m+1,r);
          NUM_THREADS++;
	  }catch(...){
          mergeSort( arr,m+1,r);
		  b2=false;	  
      }


      if(b1)
      try {
          f1.get();
          NUM_THREADS--;
      } catch (...) {
          cout << "Some error in thread" << endl;
      }

      if(b2)
      try {
          f2.get();
          NUM_THREADS--;
      } catch (...) {
          cout << "Some error in thread" << endl;
      }

   }else{
      cout <<  "::2" <<endl;
      mergeSort( arr,l,m);
      mergeSort( arr,m+1,r);
   }

   merge(arr,l,m,r);   

}



void out_arr(int* a,int sz){
   for(int i=0;i<sz;i++){
      cout << a[i] << " ";
   }
   cout << endl;
}

int main(){
   vector<int> v1={1,45,2,-100,6,13,474,-23,-89,-100,8,4000,5,-4,2,6,2,-8,8,0};
   mergeSort(v1.data(), 0, v1.size()-1);
   out_arr(v1.data(),v1.size());
   return 0;
}
