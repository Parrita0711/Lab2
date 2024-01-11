#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

// Funcion para imprimir el contenido del vector
void printVector(const std::vector<int>& vec) {
    for (const auto& num : vec) {
        std::cout << num << ' ';
    }
    std::cout << '\n';
}

// Funcion para preguntar los valores de la matriz
void fillVector(std::vector<int>& vec) {
    for (auto& num : vec) {
        std::cout << "Ingrese el valor de la matriz: ";
        std::cin >> num;
    }
}

// Funcion para guardar el contenido del vector en el archivo binario
void saveVectorToBinaryFile(const std::vector<int>& vec, const std::string& fileName) {
    std::ofstream outputFile(fileName, std::ios::binary);
    if (!outputFile) {
        std::cerr << "No se puede abrir el archivo de salida.\n";
        return;
    }
    for (const auto& num : vec) {
        outputFile.write(reinterpret_cast<const char*>(&num), sizeof(num));
    }
    outputFile.close();
}

// Funcion para cargar el contenido del vector desde el archivo binario
void loadVectorFromBinaryFile(std::vector<int>& vec, const std::string& fileName) {
    std::ifstream inputFile(fileName, std::ios::binary);
    if (!inputFile) {
        std::cerr << "No se puede abrir el archivo de entrada.\n";
        return;
    }
    vec.clear();
    int num;
    while (inputFile.read(reinterpret_cast<char*>(&num), sizeof(num))) {
        vec.push_back(num);
    }
    inputFile.close();
}

//Funcion para medir el tiempo de ejecucion 
template<typename F, typename... Args>
void measureExecutionTime(F function, const std::string& description, Args&&... args) {
    auto start = std::chrono::high_resolution_clock::now();
    function(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << description << duration << " microseconds.\n";

    std::ofstream file("executionTime.txt", std::ios::app);
    if (file) {
        file << description << duration << " microseconds.\n";
        file.close();
    }
}

int main() {
    int size;
    std::cout << "Ingrese el tamaño de la matriz: ";
    std::cin >> size;

    std::vector<int> vec(size);

    measureExecutionTime(fillVector, "Tiempo en cargar la matriz: ", vec);

    measureExecutionTime(printVector, "Tiempo en imprimir la matriz: ", vec);

    std::string fileName = "vectorData.bin";
    measureExecutionTime(saveVectorToBinaryFile, "Tiempo en guardar la matriz en el archivo binario: ", vec, fileName);

    vec.clear();
    measureExecutionTime(loadVectorFromBinaryFile, "Tiempo en cargar la matriz desde el archivo binario: ", vec, fileName);

    measureExecutionTime(printVector, "Tiempo en imprimir la matriz desde el archivo binario: ", vec);

    return 0;
}