#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <fstream>
#include <iomanip>


class ArrayGenerator {
public:
    void generate_array() {
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<> dist(_min_value, _max_value);
        arr.resize(_size);
        for (int i = 0; i < _size; ++i) {
            arr[i] = dist(generator);
        }
    }
    std::vector<int> gen_1(int n) {
        return {arr.begin(), arr.begin() + n};
    }
    std::vector<int> gen_2(int n) {
        std::vector<int> temp = {arr.begin(), arr.begin() + n};
        std::sort(temp.rbegin(), temp.rend());
        return temp;
    }

    std::vector<int> gen_3(int n) {
        std::vector<int> temp = {arr.begin(), arr.begin() + n};
        std::sort(temp.begin(), temp.end());
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<> dist(0, n);
        for (int i = 0; i < 10; ++i) {
            std::swap(temp[dist(generator)], temp[dist(generator)]);
        }
        return temp;
    }
    ArrayGenerator() {
        generate_array();
    }

private:
    int _max_value = 6000;
    int _min_value = 0;
    int _size = 10000;
    std::vector<int> arr;
};

void insertion_sort(std::vector<int>& a, int s, int e) {
    for (int i = s + 1; i <= e; ++i) {
        int key = a[i];
        int j = i - 1;

        while (j >= s && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }

        a[j + 1] = key;
    }
}

void merge_sort(std::vector<int>& a, int s, int m, int e) {
    int n1 = m - s + 1;
    int n2 = e - m;
    std::vector<int> left(n1), right(n2);

    for (int i = 0; i < n1; i++) {
        left[i] = a[s + i];
    }
    for (int i = 0; i < n2; i++) {
        right[i] = a[m + 1 + i];
    }

    int l = 0, r = 0, k = s;
    while (l < n1 && r < n2) {
        if (left[l] <= right[r]) {
            a[k] = left[l];
            l++;
        } else {
            a[k] = right[r];
            r++;
        }
        k++;
    }

    while (l < n1) {
        a[k] = left[l];
        l++;
        k++;
    }

    while (r < n2) {
        a[k] = right[r];
        r++;
        k++;
    }
}

void merge(std::vector<int>& a, int s, int e) {
    if (e - s <= 15) { // <= 15
        insertion_sort(a, s, e);
        return;
    }

    int mid = s + (e - s) / 2;
    merge(a, s, mid);
    merge(a, mid + 1, e);
    merge_sort(a, s, mid, e);
}


void standart_merge(std::vector<int>& a, int l, int r) {
    if (l >= r) {
        return;
    }
    int mid = l + (r - l) / 2;
    standart_merge(a, l, mid);
    standart_merge(a, mid + 1, r);
    merge_sort(a, l, mid, r);
}


class Test{
public:
    long long measureMergeSortTime(std::vector<int> array, int repetitions = 10) {
        long long totalTime = 0;
        for (int i = 0; i < repetitions; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            standart_merge(array, 0, array.size() - 1);
            auto elapsed = std::chrono::high_resolution_clock::now() - start;
            totalTime += std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        }
        return totalTime / repetitions;
    }

    long long measureHybridSortTime(std::vector<int> array, int repetitions = 10) {
        long long totalTime = 0;
        for (int i = 0; i < repetitions; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            merge(array, 0, array.size() - 1);
            auto elapsed = std::chrono::high_resolution_clock::now() - start;
            totalTime += std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        }
        return totalTime / repetitions;
    }
};

int main() {
    ArrayGenerator generator;
    Test tester;
    std::vector<int> sizes;
    for (int size = 500; size <= 10000; size += 100) {
        sizes.push_back(size);
    }

    std::ofstream csvFileMerge("merge_results.csv");
    std::ofstream csvFileHybrid("hybrid_results.csv");

    csvFileMerge << "Size,Random,gen_2,gen_3\n";
    csvFileHybrid << "Size,Random,gen_2,gen_3\n";
    for (int size : sizes) {
        std::vector<int> randomArray = generator.gen_1(size);
        std::vector<int> gen_2Array = generator.gen_2(size);
        std::vector<int> gen_3Array = generator.gen_3(size);

        long long randomTimeMerge = tester.measureMergeSortTime(randomArray);
        long long gen_2TimeMerge = tester.measureMergeSortTime(gen_2Array);
        long long gen_3TimeMerge = tester.measureMergeSortTime(gen_3Array);

        long long randomTimeHybrid = tester.measureHybridSortTime(randomArray);
        long long gen_2TimeHybrid = tester.measureHybridSortTime(gen_2Array);
        long long gen_3TimeHybrid = tester.measureHybridSortTime(gen_3Array);

        csvFileMerge << size << "," << randomTimeMerge << "," << gen_2TimeMerge << "," << gen_3TimeMerge << "\n";
        csvFileHybrid << size << "," << randomTimeHybrid << "," << gen_2TimeHybrid << "," << gen_3TimeHybrid << "\n";

        std::cout << "Size: " << size
                  << " Random Merge: " << randomTimeMerge << " microsec"
                  << " gen_2 Merge: " << gen_2TimeMerge << " microsec"
                  << " gen_3 Merge: " << gen_3TimeMerge << " microsec\n";

        std::cout << "Size: " << size
                  << " Random Hybrid: " << randomTimeHybrid << " microsec"
                  << " gen_2 Hybrid: " << gen_2TimeHybrid << " microsec"
                  << " gen_3 Hybrid: " << gen_3TimeHybrid << " microsec\n";
    }

    csvFileMerge.close();
    csvFileHybrid.close();

    return 0;
}
