#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;
#include <winsock2.h>
#include <string>

// Merge Sort Algorithm
void merge(vector<int>& arr, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> leftArr(n1);
    vector<int> rightArr(n2);

    for (int i = 0; i < n1; ++i) {
        leftArr[i] = arr[left + i];
    }
    for (int j = 0; j < n2; ++j) {
        rightArr[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            ++i;
        }
        else {
            arr[k] = rightArr[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        arr[k] = leftArr[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        arr[k] = rightArr[j];
        ++j;
        ++k;
    }
}

void mergeSort(vector<int>& arr, int left, int right)
{
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

int main()
{
    cout << "Hello, I am creating a web server" << endl;

    SOCKET wsocket;
    SOCKET new_wsocket;
    WSADATA wsaData;
    struct sockaddr_in server;
    int server_len;
    int BUFFER_SIZE = 30720;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cout << "Could not initialize Winsock" << endl;
        return 1;
    }

    // Create a socket
    wsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (wsocket == INVALID_SOCKET)
    {
        cout << "Could not create a socket" << endl;
        WSACleanup();
        return 1;
    }

    // Bind socket to address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(8080);
    server_len = sizeof(server);

    if (bind(wsocket, (SOCKADDR*)&server, server_len) != 0)
    {
        cout << "Could not bind socket" << endl;
        closesocket(wsocket);
        WSACleanup();
        return 1;
    }

    // Listen to address
    if (listen(wsocket, 20) != 0)
    {
        cout << "Could not start listening" << endl;
        closesocket(wsocket);
        WSACleanup();
        return 1;
    }

    cout << "Listening on 127.0.0.1:8080" << endl;

    while (true)
    {
        // Accept client request
        new_wsocket = accept(wsocket, (SOCKADDR*)&server, &server_len);

        if (new_wsocket == INVALID_SOCKET)
        {
            cout << "Could not accept client request" << endl;
            closesocket(wsocket);
            WSACleanup();
            return 1;
        }

        // Read request
        char buff[30720] = { 0 };
        int bytes = recv(new_wsocket, buff, BUFFER_SIZE, 0);
        if (bytes < 0)
        {
            cout << "Could not read client request" << endl;
            closesocket(new_wsocket);
            closesocket(wsocket);
            WSACleanup();
            return 1;
        }

        // Sorting algorithm (Merge Sort)
        vector<int> numbers = { 9, 2, 5, 1, 7, /*...add more numbers...*/ };

        // Prepare input numbers for display
        string inputNumbers;
        for (int number : numbers) {
            inputNumbers += to_string(number) + ", ";
        }
        inputNumbers = inputNumbers.substr(0, inputNumbers.size() - 2);  // Remove the trailing comma and space

        mergeSort(numbers, 0, numbers.size() - 1);

        // Prepare response
        string serverMessage = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n";
        serverMessage += "<html><head><style>";
        serverMessage += "body { font-family: Arial, sans-serif; background-color: #f5f5f5; }";
        serverMessage += "h1 { color: #333333; text-align: center; }";
        serverMessage += "ul { list-style-type: none; padding-left: 0; }";
        serverMessage += "li { margin-bottom: 5px; padding: 10px; background-color: #ffffff; }";
        serverMessage += "</style></head><body>";
        serverMessage += "<h1>Sorted Numbers</h1>";
        serverMessage += "<p><strong>Input Numbers:</strong> " + inputNumbers + "</p>";
        serverMessage += "<ul>";

        for (int number : numbers) {
            serverMessage += "<li>" + to_string(number) + "</li>";
        }

        serverMessage += "</ul></body></html>";

        // Send response
        int bytesSent = send(new_wsocket, serverMessage.c_str(), serverMessage.size(), 0);
        if (bytesSent < 0)
        {
            cout << "Could not send response" << endl;
        }
        else
        {
            cout << "Sent response to client" << endl;
        }

        closesocket(new_wsocket);
    }

    closesocket(wsocket);
    WSACleanup();
    return 0;
}
