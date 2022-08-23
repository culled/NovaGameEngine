# NovaGameEngine

We'll see what this becomes...

## Supported Platforms
* Windows x64

## Building
1. Clone NovaEngine to your machine: `git clone https://github.com/culled/NovaGameEngine.git NovaGameEngine`
2. Enter the NovaGameEngine project directory `cd NovaGameEngine`
3. Clone dependent submodules: `git submodule update --init --recursive`
4. Run the configure script for your environment located in the "scripts" directory. This will generate a Visual Studio solution and projects
  1. For Windows using Visual Studio 2022, double-click the "configure-win-x64-vs2022.bat" script
5. Open the Visual Studio solution and build like any other project!

## Building Documentation Using Doxygen
NOTE: This is experimental! If you want to try building documentation for the engine, you can use the Doxyfile found in the "docs" directory.
