//
//  main.cpp
//  MergeSort
//
//  Created by WENXI LU on 1/26/19.
//  Copyright © 2019 WENXI LU. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <cstdlib>

using namespace std;
int garray[] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8};
typedef struct Array{
    int low;
    int high;
} Array_t;

void * mergeSort(void *arr);
void merge(int lo, int hi);

void * mergeSort(void *arr) {
    Array_t *a = (Array_t *)arr;
    int mid = (a->low + a->high)/2;
    Array_t subarray[2];
    pthread_t thread[2];
    subarray[0].low = a->low;
    subarray[0].high = mid;
    subarray[1].low = mid + 1;
    subarray[1].high = a->high;
    
    if(a->low >= a->high){
        return 0;
    }
    for(int i = 0; i < 2; ++i) {
        pthread_create(&thread[i], NULL, mergeSort, &subarray[i]);
    }
    for(int i = 0; i < 2; ++i) {
        pthread_join(thread[i], NULL);
    }
    merge(a->low, a->high);
    return 0;
}


void merge (int lo, int hi) {
//    int array[] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8};
    int mid = (lo + hi) / 2;
    int i = lo;
    int j = mid + 1;
    int temp[hi - lo + 1];
    int k = 0;
    while(i <= mid && j <= hi) {
        if(garray[i] > garray[j]) {
            temp[k++] = garray[j++];
        } else {
            temp[k++] = garray[i++];
        }
    }
    while(i <= mid) {
        temp[k++] = garray[i++];
    }
    while(j <= hi) {
        temp[k++] = garray[j++];
    }
//    for(int k = i; k <= hi; ++k) {
//        if(i > mid) {
//            garray[k] = temp[j];
//            j++;
//        }
//        else if(j > hi) {
//            garray[k] = temp[i];
//            i++;
//        }
//        else if(garray[i] < garray[j]) {
//            garray[k] = temp[i];
//            i++;
//        }
//        else {
//            garray[k] = temp[j];
//            j++;
//        }
//    }
    for (int p = 0; p < (hi-lo+1) ; p++)
        garray[lo + p] = temp[p];
}


int main(int argc, const char * argv[]) {
    // insert code here...
//    int array[] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8};
    Array_t arr;
    arr.low = 0;
    arr.high = 9;
    pthread_t thread;
    pthread_create(&thread, NULL, mergeSort, &arr);
    pthread_join(thread, NULL);
    for(int i = 0; i < 10; ++i) {
        cout << garray[i] << endl;
    }
    return 0;
}
