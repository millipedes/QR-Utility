# :hammer: Quick Response (QR) Code Version 1 Utility
This is an implementation of a QR code utility used for generating QR codes
given some ascii text input by the user.

# :floppy_disk: Making the Project
This project uses `cmake` as its build infrastructure. To get `cmake` via the
`apt` package manager:
```
sudo apt update && sudo apt upgrade
sudo apt install cmake
```

Building this project uses the following libraries:
  - gl1-mesa
  - glfw3
  - sdl2
  - gtest

To install these with the `apt` package manager:
```
sudo apt update && sudo apt upgrade
sudo apt install libgl1-mesa-dev libglfw3-dev libsdl2-dev libgtest-dev
```
