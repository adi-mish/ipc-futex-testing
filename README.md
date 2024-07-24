# ipc-futex-testing
Simple reader-writer ipc using a futex syscall

# How to build
Navigate to project root directory and run `make all` to get the executables in the bin directory. Optionally, you can change compiler in the Makefile from the CC variable. Check if compiler is installed and in the path directory of your project. Run `make clean` to clean all binaries and object files.<br>
On some systems, the binaries might require sudo permissions to run since they set thread affinities and bind to a specific core.
