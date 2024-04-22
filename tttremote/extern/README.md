# Внешние зависимости для игры по сети

Этот подпроект использует следующие зависимости:

- [libzmq](https://github.com/zeromq/libzmq) - библиотека, упрощающая
  использование средств операционной системы для сетевой (и не только
  коммуникации);
- [cppzmq](https://github.com/zeromq/cppzmq) - библиотека, предоставляющая
  интерфейс в стиле C++ для libzmq.

Эти подпроекты подключены как *подмодули* git. При клонировании репозитория
необходимо убедиться, что они добавлены корректно. Если папки `cppzmq` и
`libzmq` пустые, то нужно сделать выполнить следующие команды git:

```shell
git submodule init
git submodule update
```

## Установка зависимостей

### libzmq

#### Установка для Windows

Откройте командную строку (powershell) в этой папке, после чего выполните
следующие команды:

```powershell
mkdir build build/libzmq_out
cd build/libzmq_out
cmake -G"NMake Makefiles" -DWITH_PERF_TOOL=OFF -DZMQ_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release ../../libzmq
nmake
cp lib/* ..
```

После этого нужно расположить файл `bin/libzmq-mt-4_3_6.dll` так, чтобы его
можно было найти при выполнении программы.  Самый простой способ - это
добавить этот файл в папку с исполняемыми файлами. Например, для проекта
`tttremote` по умолчанию исполняемые файлы генерируются в `out/build/<имя
решения>/tttremote` относительно корня репозитория.

Обратите внимание, что по умолчанию эта инструкция собирает библиотеку под
32-битную архитектуру, поэтому в настройках CMake (CMakeSettings.json) нужно 
установить компилятор `msvc_x86` для того, чтобы все собралось.

#### Установка для Unix/MAC

Откройте командную строку в этой папке, после чего выполните следующие команды:

```sh
mkdir -p build/libzmq_out
cd build/libzmq_out
cmake -DWITH_PERF_TOOL=OFF -DZMQ_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release ../../libzmq
make
cp lib/libzmq.a ..
```
