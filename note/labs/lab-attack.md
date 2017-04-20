#Labs-The Attack Lab

---
##PDF
- Introduction
	- Buffer Overflow
	- Stack / Parameter-passing mechanism
- **Task**: Free the string to **CTARGET** and **RTARGET** so that they can do more interesting things.


##Solution

### Code-Injection attacks
- [x] CTARGET-level1
	- **key**: overwrite the return address in the *test()* function, by the address of *touch()*	
	- the return address of **x86-64** is 64bit, **8bytes** in stack
- [x] CTARGET-level2 
	- Simulate passing Cookie as a parameter  
	- `gcc -c test.s` & `objdump -c test.o` can be used to generate the machine-level code

	>
	```
	mov $[cookie-value], %edi  #the first passed parameter
	pushq $ specified address
	ret
	```
	>
	```
	0000000000000000 <.text>:   0:	bf fa 97 b9 59       	mov    $0x59b997fa,%edi   5:	68 ec 17 40 00       	pushq  $0x4017ec   a:	c3                   	retq  
	```
	
	- `pushq address` and `retq` can simulate your specified code
	- **key**: get the *%rsp* of stack and put your specified code there, then overwrite the retrun address in the *test()* by the address of *%rsp*
	
- [x] CTARGET-level3
	- set the register *%rdi%*	to be the address of string/char
	- **key**: as the hints say, when function `hexmatch()` and `strncmp()` are called, they push data onto the stack, overwriting the buffer used by `getbuf()`. => where to put the code？

###Return-Oriented programming
- Problems
	- **randomization**
		- In CTARGET level, the instruction address is constant as it gives. However, with the randomization, the instruction address could vary each time. 	
	- the stack as nonexecutable, failing with a segmentation fault 
- [x] RTARGET-level3
	- **key**: `popq %rdi` means pop the value on the stack into *%rdi* and stack frame point increases automatically (used equally like `movq`)
	-  The `no op` instruction is `0x90`
- [ ] RTARET-level4 (not finished)
	

## Retrospect
- stack stores the data, instructions and address 