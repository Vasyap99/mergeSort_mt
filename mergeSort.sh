#!/bin/bash
g++ -pthread -fsanitize=thread mergeSort.cpp -o ms && ./ms