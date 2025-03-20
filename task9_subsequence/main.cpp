#include <iostream>
#include <vector>

std::pair<int, std::pair<int, int>> find_subsequence(std::vector<int>& arr) {
    // переменные для текущей суммы, длины последовательности, индекса начала
    // последовательности и всех перечисленных характеристик для
    // максимальной последовательности
    int maxSum = 0;
    int currentSum = 0;
    int startIndex = 0;
    int maxStartIndex = 0;
    int maxLength = 0;
    int currentLength = 0;

    // проходимся по последовательности и пока она возрастает - суммируем и
    // сравниваем с максимальной, при необходимости обновляем характеристики
    // максимальной последовательности как только последовательность перестает
    // увеличиваться обнуляем все характеристики последовательности и идем
    // дальше
    for (size_t i = 0; i < arr.size(); ++i) {
        if (i == 0 || arr[i] > arr[i - 1]) {
            currentSum += arr[i];
            currentLength++;

            if ((currentLength > maxLength) ||
                ((currentSum > maxSum) && (currentLength == maxLength))) {
                maxSum = currentSum;
                maxStartIndex = startIndex;
                maxLength = currentLength;
            }
        } else {
            currentSum = arr[i];
            currentLength = 1;
            startIndex = i;
        }
    }

    // возвращаем максимальную сумму, индекс начала последовательности и длину
    // последовательности
    return std::make_pair(maxSum, std::make_pair(maxStartIndex, maxLength));
}

int main() {
    // вводим длину массива и сам массив
    int lenght;
    std::cin >> lenght;

    std::vector<int> arr(lenght, 0);

    for (int i = 0; i < lenght; ++i) {
        std::cin >> arr[i];
    }

    // находим последовательность
    auto result = find_subsequence(arr);

    // выводим результаты
    std::cout << result.first << std::endl;

    for (auto i = 0; i < result.second.second; ++i) {
        std::cout << arr[result.second.first + i] << " ";
    }

    std::cout << std::endl;

    return 0;
}