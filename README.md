# pressuresand
A falling sand game, another RZUF test. Inspired by [The Powder Toy](https://github.com/The-Powder-Toy/The-Powder-Toy) and countless similar ones. The advantage is the ability to easily create new types of cells (elements) in JSON and convert them into binary format using the included Python script in the tools folder.

![Example expression](https://maseuko.pl/res/pressuresand.png)

### RZUF3
You can find the framework repository [here](https://github.com/izawartka/rzuf3).

### Binaries
The latest binaries are available in the Releases section on the right. Just download the zip file appropriate for your system (Windows x64 or Linux x64), extract it, and run `pressuresand.exe` (or `pressuresand` if you are on Linux).

### Dependencies
RZUF3 requires the following libraries: `SDL2`, `SDL2_image`, `SDL2_ttf` (min. >=2.0.18, recommended >=2.20.0), `spdlog`.

### Windows building
1. Make sure you have [Visual Studio](https://visualstudio.microsoft.com/) (with CMake support and vcpkg) installed.

2. Install the necessary packages:  
   `vcpkg install sdl2 sdl2-image sdl2-ttf spdlog`

3. Clone the repo using the command:  
    `git clone --recurse-submodules https://github.com/izawartka/pressuresand`  
    or just download the ZIP archive from the repository page.

4. Ensure that a proper path to the vcpkg toolchain file is set in the `CMakeLists.txt` file. Default is `C:/src/vcpkg/scripts/buildsystems/vcpkg.cmake`.

5. Open the Cmake project in Visual Studio, wait for CMake to configure the project, and then build it using the `x64-Release` or `x64-Debug` configuration.

6. Done! The binary file should be located in the `bin` directory.

### Linux building
1. Install the necessary packages. For Debian-based distributions, use the command:  
   `sudo apt install git cmake g++ ninja-build libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libspdlog-dev -y`

2. Clone the repo:  
   `git clone --recurse-submodules https://github.com/izawartka/pressuresand`.

3. Navigate to the directory:  
   `cd pressuresand`.

4. Build the project  
   For the release version:  
   `cmake --preset x64-Release`  
   `cmake --build --preset x64-Release`  

   For the debug version:  
   `cmake --preset x64-Debug`  
   `cmake --build --preset x64-Debug`  

5. Done! The binary file should be located in the `bin` directory.

### Author
masuo / izawartka
