/////////////////////////////////////////////////////////////////////////////
//
//  lychrel.cpp
//
//  A program that searches for the largest (in terms of number of
//    iterations without exceeding a specified maximum) palindrome value.
//
//  The program reports a list of numbers that share the maximum number of
//    iterations, along with the size and final palindrome number
//

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include <thread>   // for threading
#include <barrier>

#include "LychrelData.h"

// A structure recording the starting number, and its final palindrome.
//   An vector of these records (typedefed to "Records") is built during
//   program execution, and output before the program terminates.
struct Record {
    Number n;
    Number palindrome;
};
using Records = std::vector<Record>;

// A structure for keeping each threads version of maxIter and records
struct Result {
    size_t maxIter = 0;  // Records the current maximum number of iterations
    Records records; // list of values that took maxIter iterations
};

// Application specific constants
const size_t MaxIterations = 7500;
const size_t MaxThreads = 10;
const size_t LastID = MaxThreads - 1;   // id of last thread

//
// --- main ---
//
int main() {
    LychrelData data;

    std::cerr << "Processing " << data.size() << " values ...\n";

    const size_t chunk_size = data.size() / MaxThreads + 1;

    std::barrier barrier(MaxThreads);
    Result results[MaxThreads];

    // Iterate across all available data values, processing them using the 
    //   reverse-digits and sum technique described in class.
    for (auto id = 0; id < MaxThreads; ++id) {

       /* const size_t begin = id * size;
        const size_t end = std::min(data.size(), begin + size);*/

        std::thread t{ [=, &results, &data, &barrier]() {


            Number number;
            while (data.getNext(number)) {

                size_t iter = 0;

                Number n = number;

                // The Lychrel loop - for any iteration, take the number, reverse
                //   its digits, and sum those values together.  If that sum
                //   is a palindrome, stop processing
                while (!n.is_palindrome() && ++iter < MaxIterations) {
                    Number sum(n.size());   // Value used to store current sum of digits
                    Number r = n.reverse(); // reverse the digits of the value

                    // An iterator pointing to the first digit of the reversed
                    //   value.  This iterator will be incremented to basically
                    //   traverse the digits of the main number in reverse
                    auto rd = n.begin();

                    bool carry = false;  // flag to indicate if we had a carry

                    // Sum the digits using the "transform" algorithm.  This
                    //   algorithm traverses a range of values (in our case,
                    //   starting with the least-siginificant [i.e., right most]
                    //   digit) of the original number, adding each digit to its
                    //   matching digit (by position) in the reversed number.
                    //
                    // The result is stored in the sum variable, which is
                    //   built up one digit at a time, respecting if a carry
                    //   digit was necessary for any iteration. 
                    std::transform(n.rbegin(), n.rend(), sum.rbegin(),
                        [&](auto d) {
                            auto v = d + *rd++ + carry;

                            carry = v > 9;
                            if (carry) { v -= 10; }

                            return v;
                        }
                    );

                    // If there's a final carry value, prepend that to the sum
                    if (carry) { sum.push_front(1); }

                    // Transfer the sum making it the next number to be processed
                    //   (i.e., reversed, summed, and checked if it's a
                    //   palindrome)
                    n = sum;
                }

                // Update our records.  First, determine if we have a new
                //   maximum number of iterations that isn't the control limit
                //   (MaxIterations) for a particular number.  If we're less
                //   tha the current maximum (maxIter) or we've exceeded the number
                //   of permissible iterations, ignore the current result and move
                //   onto the next number.
                if ((iter < results[id].maxIter || iter == MaxIterations)) { continue; }

                // Otherwise update our records, which possibly means discarding
                //   our current maximum and rebuilding our records list.
                Record record{ number, n };
                if (iter > results[id].maxIter) {
                    results[id].records.clear();
                    results[id].maxIter = iter;
                }

                results[id].records.push_back(record);

                
            }

            // sync threads
            barrier.arrive_and_wait();
        } };


        (id < LastID) ? t.detach() : t.join();

    }


    // Find result among results array gathered by the threads
    auto result_index = results[0].maxIter;
    for (auto i = 1; i < MaxThreads; ++i) {
        if (results[i].maxIter > results[result_index].maxIter) {
            result_index = i;
        }
    }

    // Output our final results
    std::cout << "\nmaximum number of iterations = " << results[result_index].maxIter << "\n";
    for (auto& [number, palindrome] : results[result_index].records) {
        std::cout 
            << "\t" << number 
            << " : [" << palindrome.size() << "] "
            << palindrome << "\n";
    }
}