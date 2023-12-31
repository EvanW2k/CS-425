# Results for Assignment 01

## Improvement Iterations

Here's a table showing the improvements I did to make the application go faster.  The **Time** column in the table represents the _wall-clock time_ for a program run.

| Version | Time | Speedup | Memory (KB) | Changes |
| :-----: | ---- | :-----: | :------: | ------- |
| [01](01.cpp) | 9.90s | &mdash; | 1041076 | Initial version - no changes |
| 02 | 9.65s | 1.03x | 1041364 | used optimization options, -o1 gave best speed |


## Profiling Analysis

### Initial Review
The profile of 01 displayed `Transform::float4::dot(Vertex const&) const` as the the hottest function.
Consuming ~23% of the execution time. The function does float multiplication operations and sums them along with another float. I can't tell if there is a way to optimize this. 
Perhaps there is something to be done with the struct float4 as the function `Transform::operator*(Vertex const&)` also lies in the struct, coming with ~13% of the programs execution time.
