# Lake Shell

**Attention: Project is in it's early stages! Bugs are common, features are missing! Feel free to report and submit pr's**

Lake shell is a hobby project of mine, with the goal of developing a nice shell for linux based systems.

## How to build and run
Make sure you have CMake 3.16+ installed, also a c++ compiler and ninja (you can also use make).

```bash
mkdir build
cd build
cmake .. -GNinja && ninja && bin/lsh
```

## What's in the box
At the moment lake shell is far away from ready. It's capable of executing commands (internal and external), the pipe works and
it can detect if the user is inside a git directory.

More will come soon ...
