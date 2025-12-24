## Single-Threaded HTTP Linux Server in C

A minimal single-threaded HTTP server designed for microcontrollers with limited resources (e.g., ESP32). The server serves static files stored in memory.

## Building the Project
```bash
git clone git@github.com:kenka1/http_server.git
cd ./http_server/
cmake -B build
cmake --build build
./build/http_server
```

## License
This project is licensed under the MIT License.
See the LICENSE file for more details.
