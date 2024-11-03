#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <atomic>
#include <windows.h>
#include <conio.h> // для _getch()

using namespace cv;
using namespace std;

const float NOISE_PERCENTAGE = 0.1;
const int FILTER_SIZE = 5;
atomic<bool> keepRunning(true);
atomic<bool> restartRequested(false);
static bool messageShown = false;

void addImpulseNoise(Mat& image) {
    int totalPixels = image.rows * image.cols;
    int noisyPixels = static_cast<int>(totalPixels * NOISE_PERCENTAGE);
    srand(static_cast<unsigned int>(time(0)));

    for (int i = 0; i < noisyPixels; ++i) {
        int x = rand() % image.cols;
        int y = rand() % image.rows;
        image.at<uchar>(y, x) = (rand() % 2) ? 0 : 255;
    }
}

void applyHorizontalMedianFilter(const Mat& noisyImage, Mat& filteredImage) {
    for (int y = 0; y < noisyImage.rows; ++y) {
        for (int x = 2; x < noisyImage.cols - 2; ++x) {
            vector<uchar> window;
            for (int k = -2; k <= 2; ++k) {
                window.push_back(noisyImage.at<uchar>(y, x + k));
            }
            sort(window.begin(), window.end());
            filteredImage.at<uchar>(y, x) = window[2];
        }
    }
}

string openFileDialog() {
    OPENFILENAMEA ofn;
    char filename[MAX_PATH];
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Image Files\0*.jpg;*.png;*.bmp\0All Files\0*.*\0\0";
    ofn.nFilterIndex = 1;

    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;

    ofn.lpstrFile = filename;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = "Select image";
    ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileNameA(&ofn) != 0) {
        return filename;
    }
    else {
        cerr << "File selection error or operation cancelled." << endl;
        return "";
    }
}

void clearKeyboardBuffer() {
    while (_kbhit()) {
        _getch(); // Очищаем буфер клавиатуры
    }
}

void displayImages(const Mat& image, const Mat& noisyImage, const Mat& filteredImage) {
    while (keepRunning) {
        imshow("Original Image", image);
        imshow("Noisy Image", noisyImage);
        imshow("Filtered Image", filteredImage);
        waitKey(30);  // Для обновления окон
        // Печать сообщения один раз после первого отображения изображений
        if (!messageShown) {
            cout << "Press Enter to select a new image, or ESC to exit." << endl;
            messageShown = true;
        }
    }
}

void keyListener() {
    while (keepRunning) {
        if (_kbhit()) {
            int key = _getch();
            if (key == 13) {  // Enter
                system("cls");
                restartRequested = true;
                keepRunning = false;
            }
            else if (key == 27) {  // ESC
                keepRunning = false;
            }
        }
        this_thread::sleep_for(chrono::milliseconds(10)); // Небольшая задержка
    }
}

int main() {
    while (true) {
        string filePath = openFileDialog();
        if (filePath.empty()) {
            cerr << "File not selected!" << endl;
            return -1;
        }

        Mat image = imread(filePath, IMREAD_GRAYSCALE);
        if (image.empty()) {
            cerr << "Unable to load the image!" << endl;
            return -1;
        }

        if (image.rows != 256 || image.cols != 256) {
            resize(image, image, Size(256, 256));
        }

        Mat noisyImage = image.clone();
        addImpulseNoise(noisyImage);

        Mat filteredImage = noisyImage.clone();
        applyHorizontalMedianFilter(noisyImage, filteredImage);

        // Очищаем буфер клавиатуры перед запуском потоков
        clearKeyboardBuffer();
        messageShown = false;

        // Устанавливаем флаг для запуска потоков
        keepRunning = true;
        restartRequested = false;

        // Запускаем поток для отображения
        thread displayThread(displayImages, image, noisyImage, filteredImage);

        // Запускаем поток для отслеживания нажатий клавиш только после загрузки изображения
        thread keyThread(keyListener);

        // Ожидаем завершения потоков
        displayThread.join();
        keyThread.join();


        // Закрываем окна после завершения потоков
        destroyAllWindows();

        if (!restartRequested) {
            break; // Выходим из главного цикла, если пользователь нажал ESC
        }
    }

    return 0;
}
