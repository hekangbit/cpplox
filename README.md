# cpplox
this is a lox interpreter project, it is a tree-walk interpreter.<br>
The original author implemented it in java, but this code has implemented it in CPP<br>

## build & run
mkdir build<br>
cd build<br>
cmake ..<br>
cmake .. -DUSE_ASAN # link libasan to detect memory leak etc issue<br>
make -j8<br>
./lox ../xxx.lox<br>

## ebook
https://craftinginterpreters.com/contents.html
Chapter 4-13

## Extended functions

### 1 break [done]
support break in loop stmt

### 2 static method [to-do]
