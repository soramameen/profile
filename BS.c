#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>

int binary_search(int id) {
    int left = 0;
    int right = profile_data_nitems - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

     
        if (profile_data_store[mid].id == id) {
            return mid;
        }
     
        else if (profile_data_store[mid].id < id) {
            left = mid + 1;
        }
 
        else {
            right = mid - 1;
        }
    }

    return -1;
}


int is_sorted() {
    int i = 0;
    for (i = 0; i < profile_data_nitems; i++){
        if(profile_data_store[i].id > profile_data_store[i+1].id){
            printf('このデータはソートされていません')
            return 0;
        }
    }
    return 1;
}
void binary(){
    if (is_sorted()){
        binary_search(id);
    }
}
int cmd_find(int id){
    int i = 0;
    for(i = 0; i< profile_data_items; i++){
        if(profile_data_store[i].id == id){
            return i;
        }
    }
    return -1;
}
int binary_search(int id) {
    int left = 0;
    int right = profile_data_nitems - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

     
        if (profile_data_store[mid].id == id) {
            return mid;
        }
     
        else if (profile_data_store[mid].id < id) {
            left = mid + 1;
        }
 
        else {
            right = mid - 1;
        }
    }

    return -1;
}


int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int n = sizeof(arr) / sizeof(arr[0]);

    if (is_sorted(arr, n)) {
        printf("配列は昇順に整列されています。\n");
    } else {
        printf("配列は昇順に整列されていません。\n");
    }

    return 0;
}
