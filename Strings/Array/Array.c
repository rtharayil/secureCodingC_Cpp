#include <stdio.h>

void clear(int array[]) {
    // Here, sizeof(array) gives size of int*, not the array
    size_t n = sizeof(array) / sizeof(array[0]);
    printf("Inside clear(): computed size = %zu (INCORRECT!)\n", n);

    for (size_t i = 0; i < n; ++i) {
        array[i] = 0;
    }
}

void print_array(int arr[], size_t size) {
    for (size_t i = 0; i < size; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(void) {
    int dis[12];
    
    // Initialize array with non-zero values
    for (int i = 0; i < 12; ++i) {
        dis[i] = i + 1;
    }

    printf("Before clear():\n");
    print_array(dis, 12);

    clear(dis); // Problematic function

    printf("After clear():\n");
    print_array(dis, 12);  // You will notice many values are not cleared

    return 0;
}
