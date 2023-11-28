# Results for Assignment 02

## Improvement Iterations

Here's a table showing the improvements I did to make the application go faster.  The **Time** column in the table represents the dev tool time to load all the requrests afters a page refresh.

| Version | Time | Speedup | Changes |
| :-----: | ---- | :-----: | ------- |
| [01] | 1.1 min | &mdash; | Initial version |
| [02] | 1.1 min | 0x | encapsulated in async task |  
| [03] | 25.8 sec | 2.71x | ring buffer |


### Initial Review

After finally getting the pictures to load it took 1.1 minutes according ot the dev tools network tab.

### Encapsulated in Async Task

No time change. 

### Producer/Consumer ring buffer

Initally attempted with no succsess, added Async task instead.

Eventually got ring buffer to work, version [03] of the program uses a ring buffer with 10 producers, 30 consumers, and a ring buffer size of 15. 
I mainly based my ring buffer class on the in class example; although I simplified it as I was getting errors that I didn't qutie understand. I believe version [02] has my original ring buffer commented out.

