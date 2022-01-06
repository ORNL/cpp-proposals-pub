# Controling size_type of mdspan

## Problem

P0009 explicitly sets the `size_type` of `extents` to `size_t`, which is then inherited by *layout mappings* and `mdspan`.
While this matches `span` whose `extent` function returns `size_t`, this behavior has significant performance impact on
various architectures where 64bit integer throughput is significantly lower than 32bit integer computation throughput.

While that problem is present for `span` it is a much bigger issue for `mdspan`, since every data access into a multi-dimensional
array is accompanied by an offset calculation which typically costs two integer operations per dimension. 

On current GPUs, which are an essential hardware component in machines used for high performance computing, machine learning and 
graphics (arguably the core constitutiency for `mdspan`), the ratio between 32bit and 64bit integer throughput is often 2x-4x.

### Example

To experiment with the impact we were investigating a simple benchmark code, which is hosted in the mdspan reference implementation.
That benchmark is using CUDA and compares a variant with raw pointers and explicit index calculation with a version which uses mdspan.

Running the raw pointer variant with `int` vs `size_t` as the loop indecies, results in a timing of 31ms vs 56ms. 
The same is observed for the `mdspan` variant when switching in the `mdspan` implementation the `size_type` from `size_t` to `int`.
The 31ms result can also be obtained when leaving `size_type` as `size_t` but casting `extents.extent(r)` to the user provided
index type inside the *layout mappings* index calculation `operator` while using `int` as the loop index type in the algorithm.





