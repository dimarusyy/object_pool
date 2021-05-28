#### Description
The implementation of object pool based on std::unique_ptr

#### Requirements 
* C++17

#### Dependencies
* gtest for unit-tests

#### Usage
Add to cmake
```target_link_libraries(<target> object-pool)```

Simple use case:
```
#include <pool.h>

pool_t<int> p;
p.add(10);
p.add(20);
p.add(30);

auto v1 = p.aquire();
std::cout << "first added : " << *v1 << "\n";
```
