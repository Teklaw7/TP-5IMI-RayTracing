[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/35ZXeOOO)
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-718a45dd9cf7e7f842a935f5ebbe5719a5e09af4491e668f4dbf3b35d5cca122.svg)](https://classroom.github.com/online_ide?assignment_repo_id=12119934&assignment_repo_type=AssignmentRepo)
# 5imi Raytracing

## Compilation 

### Depuis le Makefile 

```sh
cd projet
make
./pgm
```

### Depuis le CMakeLists.txt

```sh 
mkdir build
cd build
cmake ..
make
cd ../projet
../build/pgm
```

ou 

```sh 
cd projet
mkdir ../build
cmake .. -B ../build
make -C ../build
../build/pgm
```

### Depuis un IDE (VSCode, QtCreator, etc).

Le répertoire d'execution doit être dans projet/
C'est a dire que le répertoire data/ doit être accessible.
