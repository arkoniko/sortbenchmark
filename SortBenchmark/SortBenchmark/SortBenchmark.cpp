#include <iostream>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <vector>
#include <algorithm>
using namespace std;

class Heap {
public:
    Heap(int* array, int size, bool top_down = true) : array_(array), size_(size) {
        if (top_down) {
            for (int i = size / 2 - 1; i >= 0; i--) {
                heap_down(i);
            }
        }
        else {
            for (int i = 1; i < size; i++) {
                heap_up(i);
            }
        }
    }

    void sort() {
        for (int i = size_ - 1; i >= 0; i--) {
            int temp = array_[0];
            array_[0] = array_[i];
            array_[i] = temp;
            size_--;
            heap_down(0);
        }
    }

private:
    void heap_down(int parent) {
        while (parent < size_) {
            int child = 2 * parent + 1;
            if (child + 1 < size_ && array_[child] < array_[child + 1]) {
                child++;
            }
            if (child < size_ && array_[parent] < array_[child]) {
                int temp = array_[parent];
                array_[parent] = array_[child];
                array_[child] = temp;
                parent = child;
            }
            else {
                break;
            }
        }
    }

    void heap_up(int child) {
        int parent = (child - 1) / 2;
        while (child > 0 && array_[child] > array_[parent]) {
            int temp = array_[child];
            array_[child] = array_[parent];
            array_[parent] = temp;
            child = parent;
            parent = (child - 1) / 2;
        }
    }

    int* array_;
    int size_;
};

void CountingSort(int* array, int size, int m) {
    int* count = new int[m];
    for (int i = 0; i < m; i++) {
        count[i] = 0;
    }
        for (int i = 0; i < size; i++) {
            count[array[i]]++;
        }
    int index = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < count[i]; j++) {
            array[index++] = i;
        }
    }

    delete[]count;
}

void BucketSortInts(int* array, int size, int m) {
    std::vector<std::vector<int>> buckets(m); ///tworzenie wektora z m pustych wektorów (kubełków)
    //std::vector<int>* buckets = new std::vector<int>[size];

    for (int i = 0; i < size; i++) {
        buckets[array[i]].push_back(array[i]); //umieszcza każdy element w tablicy w odpowiednim kubełku
    }

    for (int i = 0; i < m; i++) {
        if (!buckets[i].empty()) {
            int* bucket_array = &buckets[i][0]; //przekształcenie wektora kubełków na tablicę
            Heap heap(bucket_array, buckets[i].size()); //heapsort z wyżej
            heap.sort();
        }
    }

    int index = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < buckets[i].size(); j++) {
            array[index++] = buckets[i][j];
        }
    }
}

int main(){
    for (int i = 10; i <= 10000000; i *= 10) { //mnozy prawy i wynik przypisuje lewemu
        int* array = (int*)malloc(i * sizeof(int));

        for (int j = 0; j < i; j++) {
            array[j] = ((rand() << 15) + rand()) % 10000000;
        }

        auto start = std::chrono::high_resolution_clock::now();
        int* array1 = (int*)malloc(i * sizeof(int));
        memcpy(array1, array, i * sizeof(int));
        Heap heap(array1, i, false);
        heap.sort();
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "HeapSort na tablicy o rozmiarze: " << i << " zajal " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " mikrosekund" << std::endl;

        auto start1 = std::chrono::high_resolution_clock::now();
        int* array2 = (int*)malloc(i * sizeof(int));
        memcpy(array2, array, i * sizeof(int));
        CountingSort(array2, i, 10000000);
        auto end1 = std::chrono::high_resolution_clock::now();
        std::cout << "CountingSort na tablicy o rozmiarze: " << i << " zajal " << std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count() << " mikrosekund" << std::endl;
        
        auto start2 = std::chrono::high_resolution_clock::now();
        int* array3 = (int*)malloc(i * sizeof(int));
        memcpy(array3, array, i * sizeof(int));
        BucketSortInts(array3, i, 10000000);
        auto end2 = std::chrono::high_resolution_clock::now();
        std::cout << "BucketSort na tablicy o rozmiarze: " << i << " zajal " << std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2).count() << " mikrosekund" << std::endl;
        
        

        bool is_sort1 = true;
        bool is_sort2 = true;
        bool is_sort3 = true;
        for (int x = 1; x < i; x++) {
            if (array1[x] < array1[x - 1]) {
                is_sort1 = false;
                break;
            }
            if (array2[x] < array2[x - 1]) {
                is_sort2 = false;
                break;
            }
            if (array3[x] < array3[x - 1]) {
                is_sort3 = false;
                break;
            }
        }
        if (is_sort1 && is_sort2 && is_sort3) {
            std::cout << "Tablice sa posortowane poprawnie\n" << std::endl;
        }
        else {
            std::cout << "BLAD: tablice nie sa poprawnie posortowane\n" << std::endl;
        }

        free(array);
        free(array1);
        free(array2);
        //free(array3);
    }

    return 0;
}
