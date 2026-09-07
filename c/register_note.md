## instruction 
+ ip: cpu instruction 
+ sp: stack pointer 
+ bp: base pointer 

## arguments
di, so. dx, cx, 8, 9, stack... 

## counter 
cx 

## mul/div 
dx, ax 

## string copy 
si, di 

## save the following register before you run a new function, as the function might overwrite it 
ax, cx, dx, si, di, 8, 9, 10, 11 

## save and restore the following register if you want to use it 
bx, bp, 12, 13, 14, 15, sp 


## variable pointer 
`char* pointer = malloc(sizeof(char));`

| left hand side | right hand side | 
| :--- | :--- | 
| at stack | at heap | 
| has address of stack | has address of heap | 
| store address of heap it points to | store binary, which is the value it holds | 

( ): contain 
address01(address02)
address02(value)

--- 

`char** pointer = malloc(num * sizeof(char*));`
`for i in range num: pointer[i] = malloc(sizeof(char));`

| left hand side | right hand side | 
| :--- | :--- | 
| at stack | at heap | 
| has address of stack | has address of heap | 
| store address of heap it points to | store other heap address it points to | 
| - | for each address, has store value it holds | 

address01(address02)
address02(address03, 04...based on lengh and pointer type)
address03(value) ... 

