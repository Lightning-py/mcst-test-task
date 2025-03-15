#include <string.h>

// функция сортировки пузырьком
void bubbleSort(char **arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (strcmp(arr[j], arr[j + 1]) > 0) {
                char *temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// функция для слияния в сортировке слиянием
void merge(char **arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    char **L = (char **)calloc(n1, sizeof(char *));
    char **R = (char **)calloc(n2, sizeof(char *));

    memcpy(L, arr + left, n1 * sizeof(char *));
    memcpy(R, arr + (mid + 1), n2 * sizeof(char *));

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (strcmp(L[i], R[j]) <= 0) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

// сортировка слиянием
void mergeSort(char **arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

// функция разбиения для быстрой сортировки
int partition(char **arr, int low, int high) {
    char *pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j < high; j++) {
        if (strcmp(arr[j], pivot) < 0) {
            i++;
            char *temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    char *temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i + 1;
}

// быстрая сортировка
void quickSort(char **arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}