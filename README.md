# Gamepad Values Project

This project is designed to retrieve and display gamepad input values using C++ on Windows.
It provides a simple interface for initializing the gamepad, reading input values, and processing button states and axis movements.


## Project Structure

```
gamepad-values
├── src
│   ├── main.cpp        # Entry point of the application
│   └── gamepad.cpp     # Implementation of gamepad handling functions
├── include
│   └── gamepad.h       # Declaration of the Gamepad class
├── CMakeLists.txt      # CMake configuration file
└── README.md           # Project documentation
```

## Setup Instructions

1. Clone the repository:
   ```
   git clone https://github.com/nbigot/gamepad-values.git
   ```

2. Compile:
   With vscode:
   use the "CMake" vscode extension (as well as the "C/C++ Extension Pack")
   
   or manually:
   ```
   cd gamepad-values
   ./build.sh
   ```


## Usage

### Run with vscode

Click on the "CMake" icon (extention), then click "Build", the click "Debug".

Note: when running inside vscode the results are shown inside the "DEBUG CONSOLE" tab.


### Run manually

After building the project, you can run the application to see the gamepad values:

```sh
./gamepad-values/build/Debug/gamepad-values.exe
```

To stop the program, press Ctrl+C or simultaneously press the "back" and "start" buttons on the gamepad.


## Dependencies

- C++20 or later
- Gamepad input library (XInput) (ensure to install any required libraries for gamepad support)

## Contributing

Feel free to submit issues or pull requests if you have suggestions or improvements for the project.



## Visual studio code

### CMake

[CMake tutorial](https://www.youtube.com/watch?v=_BWU5mWqVA4)

Steps to Build
1. Run the configure task to generate the build files:

- Open the Command Palette (Ctrl + Shift + P).
- Type Run Task and select it.
- Choose the configure task.
2. Run the build task to compile the project:

- Open the Command Palette (Ctrl + Shift + P).
- Type Run Task and select it.
- Choose the build task.

This should configure and build your project using Visual Studio as the generator.
