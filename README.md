# jit

Git inspired minimal version control system.

## Dependencies

- C++17 or later (for `std::filesystem`)
- CMake
- OpenSSL
- ZLIB

## Building the Project

Ensure you have the required dependencies installed on your system.

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

After building, run the executable with desired commands. For ease of use, you may optionally create an alias for `jit` or add the executable to your `PATH`, or just use `./jit` instead of `jit` in the following commands.

### Initialization
Initialize a new local repository:
```bash
jit init
```

### Hashing and Objects
Hashes an object (file) and optionally writes it to the object database using `-w`:
```bash
jit hash-object [-w] <filepath>
```

Prints the contents of an object given its hash:
```bash
jit cat-file <hash>
```

### Trees
Creates a tree object for the given directory and optionally writes it to the object database using `-w`:
```bash
jit make-tree [-w] <directory-path>
```

Lists the contents of a tree object:
```bash
jit ls-tree <tree-hash>
```

### Commits and History
Configure user details (e.g. `user` and `email`):
```bash
jit config <key> <value>
```

Create a commit object mapping to a tree object natively:
```bash
jit commit-tree <tree-hash> "<message>" [<parent-hash>]
```

Create a full commit mapping to the current repository working directory:
```bash
jit commit -m "<message>"
```

View the commit history:
```bash
jit log
```

### Checking Out States
Switch the repository working directory to the exact state of a given commit hash:
```bash
jit checkout <commit-hash>
```
