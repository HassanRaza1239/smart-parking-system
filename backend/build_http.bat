@echo off
echo Building NexusPark HTTP Server...
echo Compile: g++ -std=c++17 http_server.cpp src/*.cpp -I. -Iinclude -o http_server.exe
pause
