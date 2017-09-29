# README #

This README would normally document whatever steps are necessary to get your application up and running.

### What is this repository for? ###

* AF-Stream: a distributed stream processing system with **approximate fault tolerance**
* Version: 0.2

### Example: Count-Min Sketch ###

Step 1: build AF-Stream library 

* Create a directory (here we select release in the root directory)
```
mkdir release && cd release
```

* Generate Makefile via Cmake
```
cmake ..
```

* Compile
```
make
```

* Copy it to system lib directory (Or add current directory to LD\_LIBRARY\_PATH
```
sudo cp libafstream.so /usr/lib
```

Step 2: compile applications

* preparation
    * application source code (see apps/count-min)
    * a configuration file (see conf/count-min/count-min.ini)
    * input trace (see apps/count-min/input.txt

```
cd apps/count-min
make
```

Step 3: execute (e.g. 1 local sketch worker and 1 global sketch worker)

* In terminal 1: 
```
./cm_worker ../conf/count-min/count-min.ini Worker1 0
```

* In terminal 2:
```
./cm_merger ../conf/count-min/count-min.ini Worker2 0
```

### How do I get set up? ###

* Summary of set up
* Configuration
* Dependencies
* Database configuration
* How to run tests
* Deployment instructions

### Contribution guidelines ###

* Writing tests
* Code review
* Other guidelines

### Who do I talk to? ###

* Repo owner or admin
* Other community or team contact
