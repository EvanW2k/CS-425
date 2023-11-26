# Results for Assignment 02

## Improvement Iterations

Here's a table showing the improvements I did to make the application go faster.  The **Time** column in the table represents the _wall-clock time_ for a program run.
All tests are done with the Final data set and -O3 optimization.

| Version | Time | Speedup | Changes |
| :-----: | ---- | :-----: | ------- |
| [01] | 658.43s | &mdash; | Initial version - but with -O3 |
| [02] | 68.03s | 9.68x | Included threading on main for loop |
| [03] | 68.65s | 9.59x | Included dynamic scheduling |



## Profiling Analysis

### Initial Review

Initial run with Final data with no profiling and -O3 ran in ~658 seconds. The output was correct with Final/output.txt.

### Threading main for loop

Split the iteration of data among 10 threads, using a lamda function and having each thread store sperate results.
Each thread fed into an array of Result structs (a Result struct has a maxIter and a records vector). 
After all threads are done the results are compared and the index with the best result is used for output.

### Dynamic Scheduling

Used the getNext function with a while loop to replace the chunk iteration for loop. This didn't really change the speed, 
though it is required. 

### Multiple Value Retrieval

Attempted using this but it slowed it down quite a bit as I just iterated over the chunk of data I retrieved. Not sure what 
to do differently with data now that I've retrieved multiple numbers at once. 
