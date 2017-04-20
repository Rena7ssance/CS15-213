#Labs-Cache Lab
---

## Part A Implement a cache simulator
- Programming Rules
	- It uses **LRU**(least-recently used)replacement policy
	- Part A only interests in data cache performance

- Programming Reflection
	- `malloc`
	
		```
		#include <stdlib.h>
		T ** array;
		array = (T **)malloc(sizeof(T *) * RowNumber);
		if (array == NULL) exit(-1);
		for (int i = 0; i < RowNumber; i++) {
			array[i] = (T *)malloc(sizeof(T) * ColNumber);
		}
		for (int i = 0; i < RowNumber; i++) {
			free(array[i]);
		}
		free(array);
		``` 
	- How to define a set and the use of `struct`
	  
	  	```
		// one set struct definition
		struct set {
			int *valid;  		     //array
			long *tag;				 //array
			clock_t *lastAccessTime; //array
		}
		```
	- Handle the *load* operation(load)
	
		```
		load(){
			if cache hit:
				hit++;
			else:
				miss++;
				if there exists empty block:
					fill
				else:
					evict++;
					LRU-replace;
		}
		``` 
	- Handle the *store* operation(write)
		- This lab does not involved the write strategy (write-throught/write-back) 	
		- When write not hit(miss), it is prefered to use **write-allocate** in CSAPP, which refers to **load** the data from the lower cache hierarchy.

		```
		store(){
			if cache hit:
				hit++;
			else:
				load();
		}
		```
	- Handle the *modify* operation:
		- combine load and store