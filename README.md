# pressuresand
A falling sand game, another RZUF test. Inspired by [The Powder Toy](https://github.com/The-Powder-Toy/The-Powder-Toy) and countless similar ones. The advantage is the ability to easily create new types of cells (elements) in JSON and compile them into binary format using the included Python script in the tools folder.

### RZUF3
You can find the framework repository [here](https://github.com/izawartka/rzuf3).

### Binaries
The latest binaries are available in the Releases section on the right. Just download the zip file, extract it, and run `pressuresand.exe`.

### Dependencies
RZUF3 requires the following libraries: `SDL2`, `SDL2_image`, `SDL2_ttf`, `spdlog`. For installation on Windows, it's best to use vcpkg. If CMake has trouble finding them, ensure the appropriate path is set in the second line of `CMakeLists.txt`.

### Author
masuo / izawartka
