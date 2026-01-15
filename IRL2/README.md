# IRL 2 OpenMP Multi-threading In-class codes

We have a couple of exercises for this session.

## Preparing your machine for the session
The bare minimum code that your system should be able to compile and run is
the following.

```
#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        printf("This is thread %d\n", omp_get_thread_num());
    }
}
```

You should be able to compile the code using gcc or clang with `-fopenmp` (on
Linux), or check the `OpenMP_on_Mac.md` file for flags to compile on MacOS.

If you are on MacOS, you can overwrite the Makefile with Makefile.mac, and
modify the paths if you need to.

## Task 1 - Dot product of two large vectors

Let us assume that there are two large arrays `A[N]` and `B[N]` where N is the
number of elements and it is very large (e.g. 512 $\times$ 1024 $\times$ 1024).
If you are running on crocodile, you may need to shrink this size to
128 $\times$ 1024 $\times$ 1024 (or even 120 $\times$ 1024 $\times$ 1024. Your memory usage is limited to 1GBs).

Initialize the two arrays with some value. (Fill them with some random value
using the rand() function).

Now, loop through the arrays and do element wise multiplication and sum up all
the products. Finally, print out the value you get.

### Task 1 - Part 1 Serial implementation

* Allocate the two large arrays
* Fill the arrays with rand() values, or with some other values you specify.
* Iterate through the arrays and compute the dot-product of the two arrays
* You can compile your code using `make serial`, and run the generated code
  using `./serial`.

### Task 1 - Part 2 OpenMP
Let's try quickly parallelizing this using OpenMP.

* Copy your `serial.c` to `openmp.c`. (e.g. `cp serial.c openmp.c`).
* Now add the OpenMP compiler directives (the `#pragma omp` commands in your
  code.
  * As good practice I suggest you use the `default(none)` in the directive
    and specify the `private()` and `shared()` variables.
* Compile using `make openmp`
* Does the code run correctly? What is wrong? (Hint: synchronization
  required!)
* Fix and rerun. do you get the same output as the serial version?

### Task 1 - Part 3 Pthread
Now, let's try implementing this using pthreads. (You will learn to appreciate
OpenMP even more!)

* Copy your `serial.c` to `pthread.c`.
* Now call the appropriate pthread function calls such as `pthread_create()`
  and `pthread_join()`.
  * Oh by the way, you'll need to move your dot product calculation code to
    another function to pass onto the `pthread_create()` call.
  * Oh and you also need to think about how to split up the work each thread
    does.
  * `man pthreads`, `man pthread_create` on Ubuntu or `man pthread` on MacOS
    will provide you with the pthread function information.
* How are you going to collect the results of each thread? (Another
  challenge).
* Compile your code using `make pthread`.

(Isn't OpenMP so simple to use?!)

## Task 2 - Finding Prime Numbers
Now let's write a program that computes prime numbers between 1 and N.
N is defined as 50,000,000  in the code, but feel free to adjust this.

Iterate from 1 to N and count the number of prime numbers in this range.

Your method of finding the prime number can be more efficient, but I propose
going with the simplest/inefficient approach.

Given a number `n`, search from `f=2` up to `f=sqrt(n)` and find whether `n` is
divisible by `f`. If divisible, `n` is not a prime number.
If not divisible by any value of `f`, then `n` is a prime number.

Hint: You can use the math library's `sqrt()` function. You can read more about it using `man sqrt`.

* First write your code in a serial manner. (isPrime.c)
* Then add the OpenMP directives to parallelize the code.
  * Again, try to use the `default(none)` directive and specify the `private`
    and the `shared` variables.
* Compile your code with `make isPrime`.
* Do you have the same output as your serial version? You should.
* How much quicker was your approach with 2, 4, 8 threads?
  * You can change the number of threads through environment variables
  * e.g. `OMP_NUM_THREADS=4 ./isPrime`
* Will all threads get the same amount of work? Later threads might get larger
  numbers to factor! Will cause load-imbalance. 
  * How can you fix this? hint: `schedule` directive.
  * Does this improve your performance?

## Task 3 - Compute π
We can compute Pi in a couple of ways.
Whichever approach you take, you can use the pi.c and `make pi` to compile
your code.

### Task 3 Option 1 - the elegant integaration approach
```math
\int_{0}^{1} \frac{4.0}{1+x^2} \, dx = \pi
```

If you split up the integration into small enough bars, then you can sum up
the areas of the bars to get Pi.

Then you can split up the different bars among different threads to get your
answers.

### Task 3 Option 2 - the Monte Carlo approach

* Implement Monte-Carlo method of approximating value of π
* Draw a circle $x^2 + y^2 = 1$
* In the first quadrant randomly plot x, y values in the range of [0, 1]
* Count number of points that is within the circle ($distance to origin <= 1$)
* Ratio of inside circle to total sample becomes $\frac{\pi}{4}$

![Monte Carlo method animation(Kmhkmh, CC BY 4.0, via
Wikimedia Commons)](https://upload.wikimedia.org/wikipedia/commons/d/d4/Pi_monte_carlo_all.gif)

You can see an animated image on the [Wikipedia Monte Carlo Method page](https://en.wikipedia.org/wiki/Monte_Carlo_method#Overview).


And because the random plotting is all independent of other threads, you can
make this work in parallel!
