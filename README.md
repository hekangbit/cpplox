# cpplox
this is a tree-walk interpreter project for lox-language.<br>
The original project was implemented in java, here I use CPP.<br>

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

### 2 delete env to avoid asan report issue [to-do]

### 3 static method [to-do]
