Compilation and execution:
    - On Windows:
        > mingw32-make.exe
        > ./Test<filename>
    - On Linux:
        > make
        > ./Test<filename>

For dynamic analyse:
    - > valgrind --leak-check=full ./TestStack
