# Labs-Bomb Lab
---

## Solution
1. disassemble the executable `bomb` file 
	- `objdump -d bomb > result`
	
2. Defuse the bomb
	- Operations:
		- `break *location` set the specified break point the function stop at 
	- **phase1**:
		- Understanding the *%esi* meaning
		- **key** `p/s (char*)location` print the char content at the specified location
	- **phase2** 
		-  	
	- **phase3**
		- `mov $0x4025cf,%esi` get the format of 2nd parameter
		- `jmpq *0x402470(,%rax,8)` indirect jump needs computation
		- `x/nx location` gets n bytes contents at the location 
	- **phase4** 
		- key point is to understand the *%edi, %esi, %edx* respectively represents the 1st, 2nd and 3rd parameters to be passed in a function
		- rewrite the prototype of `func4` and **then** put the preset values into the function.
	- **phase5** 
		- **key** `p/s (char*)location` print the char content at the specified location
		- `while` loop
	- **phase6** (Difficult)
		- Note:
			- `*0x40116f`: the six numbers stored on the stack replaced by `7-each one`
			- `0x40116f-0x4011a9` stores the corresponding  node address of each value onto the remaining blank part of the stack.
			- `0x4011ab` checks the tags value of corresponding node and keep the tag value non-increasing.
	
3. Answer:
	- Border relations with Canada have never been better.
	- 1 2 4 8 16 32
	- 0 207(one possible answer)
	- 7/3/1/0 0 all works
	- ionefg(0x49,0x4f,0x4E,0x45,0x46,0x47) one possible answer
	- 3 4 5 6 1 2


---
