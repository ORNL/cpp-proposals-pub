---
title: "Atomic Refs Bound to Memory Orderings & Atomic Accessors"
document: P2689R4
date: today
audience: SG1 & LEWG
author:
  - name: Christian Trott 
    email: <crtrott@sandia.gov>
  - name: Damien Lebrun-Grandie 
    email: <lebrungrandt@ornl.gov>
  - name: Mark Hoemmen 
    email: <mhoemmen@nvidia.com>
  - name: Daniel Sunderland
    email: <dansunderland@gmail.com>
  - name: Nic Morales
    email: <nmmoral@sandia.gov>
  - name: Nevin Liber
    email: <nliber@anl.gov>
toc: true
---


# Revision History

## P2689R4 LEWG Feedback and catching up to `atomic_ref` changes

- Harmonize with changes to `atomic_ref`
  - added `store_op` member functions
  - added new floating point minimum and maximum member functions
  - incorporated changes to `value_type` enabling a `const T`
  - regrouped member functions
  - make move ctor deleted
  - add converting ctor (for when `U` and `T` are similar, and `U*` is convertible to `T*`)
  - add `address` function
  - make (almost) everything constexpr
  - fix constructor wording
  - update latest working draft version from N4917 (2022) to N5054 (July 2026)
  - fix formatting of exposition-only identifiers
  - fix constraint on non-generic _`atomic-ref-bound`_
  - fix `difference_type` in non-generic _`atomic-ref-bound`_
- Improve nonwording sections
  - Explain why _`atomic-ref-bound`_ omits `compare_exchange_{weak,strong}`
    with user-specified `failure` memory order
  - Explain why _`atomic-ref-bound`_ actually _improves_ safety
    by making operations implicitly use a memory order other than
    sequential consistency
  - Expand explanation of why we prohibit memory order conversions
  - Explain why we do not attempt to define a
    generic proxy reference accessor

## LEWG reviews of R3

- LEWG reviewed P2689R3 on 2024-04-30 but did not have quorum.

- LEWG reviewed P2689R3 again on 2026-08-04.
  This time, LEWG had quorum and took two POLLS.

  - "P2689 should provide conversions from different memory orders":
    SF/F/N/A/SA, 0/0/2/8/1 (out of 16), consensus against.

  - "P2689 should make _`atomic-ref-bounded`_ / _`basic-atomic-accessor`_
    NON exposition only (user spellable)":
    SF/F/N/A/SA, 0/2/8/1/0 (out of 16), no consensus.

## P2689R3 (LEWG Feedback)

- Split definition of _`atomic-ref-bound`_ into one for arithmetic and pointer, and one for other types
  - needed because the typedef `difference_type` should only appear for arithmetic and pointer types
- Added discussion for making _`atomic-ref-bound`_ exposition only or not
- Added discussion arguing for not having converting constructors with respect to memory order
- Added missing `fetch_min` and `fetch_max` functions
- Use "west const"
- Added header synopsis changes

## P2689R2 (SG1 Issaquah 2023 discussion)

- Renamed _`atomic-ref-bounded`_ to _`atomic-ref-bound`_
- Renamed _`atomic-ref-unbounded`_ to _`atomic-ref-unbound`_
- Fixed the wording for _`basic-atomic-accessor`_`::offset`
- Fixed the wording for _`basic-atomic-accessor`_`::access`
- If P2616R3 (Making std::atomic notification/wait operations usable in more situations) is also approved, similar changes should be applied to _`atomic-ref-bound`_ as well

### Issaquah 2023 SG1 Polls

#### P2689 poll

Forward P2689R1 to LEWG, targeting C++26
Incl. wording changes:
<br>
- Atomic ref [un]bounded to [un]bound
<br>
- Basic atomic accessors `access()` effects should be
`return reference(p[i]);`
<br>
- Basic atomic accessors `offset()` effects should be
`return p+i;`
<table>
<thead>
<tr>
<th>SF</th>
<th>F</th>
<th>N</th>
<th>A</th>
<th>SA</th>
</tr>
</thead>
<tbody>
<tr>
<td>5</td>
<td>9</td>
<td>0</td>
<td>0</td>
<td>0</td>
</tr>
</tbody>
</table>
Unanimous consent

#### P2616 poll
Apply changes in P2616R3 to the additions of P2689R1
<table>
<thead>
<tr>
<th>SF</th>
<th>F</th>
<th>N</th>
<th>A</th>
<th>SA</th>
</tr>
</table>
No objection to unanimous consent

## P2689R1 2023-01 (pre-Issaquah 2023) mailing

- Added _`atomic-ref-bounded`_ exposition-only template for an `atomic_ref` like type bounded to a particular `memory_order`
- Added `atomic_ref_relaxed`, `atomic_ref_acq_rel` and `atomic_ref_seq_cst` alias templates
- Added _`basic-atomic-accessor`_ exposition-only template
- Added `atomic_accessor_relaxed`, `atomic_accessor_acq_rel` and `atomic_accessor_seq_cst` templates

## Initial Version 2022-10 Mailing

### Kona 2022 SG1 polls

#### SG1 Poll #1
We like the bound `memory_order` versions of `atomic_ref` (This says nothing about whether these are aliases of general a template that takes a `memory_order` argument.)
<br>
<br>
`atomic_ref_relaxed`
<br>
`atomic_ref_acq_rel` (acquire for load, release for store)
<br>
`atomic_ref_seq_cst`
<table>
<thead>
<tr>
<th>SF</th>
<th>F</th>
<th>N</th>
<th>A</th>
<th>SA</th>
</tr>
</thead>
<tbody>
<tr>
<td>1</td>
<td>11</td>
<td>0</td>
<td>1</td>
<td>0</td>
</tr>
</tbody>
</table>
Consensus

#### SG1 Poll #2
X\) The meaning of the bound `memory_order` is that is the default order
<br>
Y\) The meaning of the bound `memory_order` is that is the only order
<table>
<thead>
<tr>
<th>SX</th>
<th>X</th>
<th>N</th>
<th>Y</th>
<th>SY</th>
</tr>
</thead>
<tbody>
<tr>
<td>1</td>
<td>1</td>
<td>3</td>
<td>4</td>
<td>2</td>
</tr>
</tbody>
</table>
Consensus for Y
<br>
<br>
SX : we have a variant of this type in Folly and it has been useful to be able to override this
<br>
SY : it's weird for `atomic_ref_relaxed` to have a `seq_cst` atomic done to it; since you can re-form the `_ref` you can still do it

#### SG1 Poll #3
the next version of this proposal, with these changes, should target SG1 & LEWG
<br>
<br>
`atomic_accessor` -> `atomic_ref`
<br>
`atomic_accessor_relaxed` -> `atomic_ref_relaxed`
<br>
`atomic_accessor_acq_rel` -> `atomic_ref_acq_rel`
<br>
`atomic_accessor_seq_cst` -> `atomic_ref_seq_cst`
<br>
<br>
(Return to SG1 for final wording when this is in LWG.)
<table>
<thead>
<tr>
<th>SF</th>
<th>F</th>
<th>N</th>
<th>A</th>
<th>SA</th>
</tr>
</thead>
<tbody>
<tr>
<td>6</td>
<td>6</td>
<td>2</td>
<td>0</td>
<td>0</td>
</tr>
</tbody>
</table>

# Summary

This proposal adds three "bound atomic reference" types.

1. `atomic_ref_seq_cst`

2. `atomic_ref_acq_rel`

3. `atomic_ref_relaxed`

They work like `atomic_ref`, except that each of them
is bound to a particular `memory_order` value at compile time.
Wherever an atomic `atomic_ref` member function would have
an optional `memory_order` parameter with a default value,
the corresponding `atomic_ref_*` member function
just uses its class' bound `memory_order` value.

The following table shows the memory order
used by each of the three types for loads and stores.

<table>
<thead><tr><th>Atomic Ref</th><th>`memory_order`</th><th>Loads</th><th>Stores</th></tr></thead>
<tbody>
<tr>
  <td>`atomic_ref_relaxed`</td>
  <td>`memory_order_relaxed`</td>
  <td>`memory_order_relaxed`</td>
  <td>`memory_order_relaxed`</td>
</tr>
<tr>
  <td>`atomic_ref_acq_rel`</td>
  <td>`memory_order_acq_rel`</td>
  <td>`memory_order_acquire`</td>
  <td>`memory_order_release`</td>
</tr>
<tr>
  <td>`atomic_ref_seq_cst`</td>
  <td>`memory_order_seq_cst`</td>
  <td>`memory_order_seq_cst`</td>
  <td>`memory_order_seq_cst`</td>
</tr>
</tbody>
</table>

This proposal also adds four `mdspan` "atomic accessors."
Each of them performs element accesses atomically
by using an atomic reference type.
They differ only by their `reference` types.

<table>
<thead><tr><th>Accessor</th><th>`reference`</th></tr></thead>
<tbody>
<tr><td>`atomic_accessor`</td><td>`atomic_ref`</td></tr>
<tr><td>`atomic_accessor_relaxed`</td><td>`atomic_ref_relaxed`</td></tr>
<tr><td>`atomic_accessor_acq_rel`</td><td>`atomic_ref_acq_rel`</td></tr>
<tr><td>`atomic_accessor_seq_cst`</td><td>`atomic_ref_seq_cst`</td></tr>
</tbody>
</table>

# Rationale

`atomic_accessor` was part of the rationale provided in P0009 for `mdspan`'s *accessor policy* template parameter.

One of the primary use cases for these accessors is the ability to write algorithms with somewhat generic `mdspan` outputs,
which can be called in sequential and parallel contexts.
When called in parallel contexts users would simply pass an `mdspan` with an atomic accessor.
The algorithm implementation itself could be agnostic to the calling context.

A variation on this use case is an implementation of an algorithm taking an execution policy,
which adds the atomic accessor to its output argument if called with a parallel policy,
while using the `default_accessor` when called with a sequential policy.
The following demonstrates this with a function computing a histogram:

```c++
template<class T, class Extents, class LayoutPolicy>
constexpr auto
add_atomic_accessor_if_needed(std::execution::sequenced_policy,
  std::mdspan<T, Extents, LayoutPolicy> m)
{
  return m;
}

template<class ExecutionPolicy, class T, class Extents, class LayoutPolicy>
constexpr auto add_atomic_accessor_if_needed(ExecutionPolicy,
  std::mdspan<T, Extents, LayoutPolicy> m)
{
  return std::mdspan(m.data_handle(), m.mapping(), std::atomic_accessor<T>());
}

template<class ExecT>
void compute_histogram(ExecT exec, float bin_size,
  std::mdspan<int, std::dims<1, int>> output,
  std::mdspan<float, std::dims<1, int>> data)
{
  static_assert(std::is_execution_policy_v<ExecT>);
  auto accumulator = add_atomic_accessor_if_needed(exec, output);

  std::for_each(exec,
    data.data_handle(), data.data_handle() + data.extent(0),
    [=] (float val) {
      int bin = std::abs(val)/bin_size;
      if (bin > int(output.extent(0))) {
        bin = output.extent(0) - 1;
      }
      accumulator[bin]++;
    }
   );
}
```

The above example is [available on Compiler Explorer here](https://godbolt.org/z/WKn46fPev).

# Design decisions

Three options for atomic refs were discussed in SG1 in Kona 2022:  add new types for `memory_order` bound atomic refs, add a new `memory_order` template parameter to the existing `atomic_ref`, or add a constructor to the existing `atomic_ref` that takes a `memory_order` and stores it.  Given that the last two are ABI breaks, the first option was polled and chosen.
It was also decided that the new bound atomic refs would not support overriding the specified `memory_order` at run time.

This proposal has chosen to make a general exposition-only template _`atomic-ref-bound`_ that takes a `memory_order` as a template argument
and alias templates for the three specific bound atomic refs.
Also, the various member functions are constrained by integral types not including `bool`, floating point types and pointer types, as opposed to the different template specializations specified for `atomic_ref`.
Other than not being able to specifiy the `memory_order` at run time, the intention is that the bound atomic ref types have the same functionality and API as `atomic_ref`.

Similarly for the atomic accessors, it was decided in SG1 in Kona 2022 to add four new types.
This proposal has chosen to make
a general exposition-only template _`basic-atomic-accessor`_ which takes the `reference` type as a template parameter, and four alias templates for the specific atomic accessors.

Assuming both papers are approved, SG1 voted that similar changes to `atomic_ref` in P2616R3 (Making std::atomic notification/wait operations usable in more situations) should also be applied to _`atomic-ref-bound`_.  They are not yet in the wording of either paper, as we do not know what order LWG will apply them to the working draft.

## Generic reference and accessor are exposition only

As mentioned above, during SG1 review we introduced explicitly named type aliases
instead of making _`basic-atomic-accessor`_ and _`atomic-ref-bound`_ part of the public interface.
This is because algorithmic considerations generally dictate memory order.
It would be unusual to make an algorithm generic on the memory order.
The only generic thing one may want to decide is whether to use atomics at all,
e.g., as a function of the execution policy, in the example above.
We could not find a use case where the generic types are useful enough to expose.

## No memory order conversions

The design does *not* permit conversions between references
or accessors with different memory orders.
This is because functions with an `atomic_ref_${MEMORY_ORDER} ref` parameter
strongly suggest that they only access `ref` with the given `${MEMORY_ORDER}`.
Allowing conversions would make it easy to violate that expectation.

For instance, consider a function `void foo_seq_cst(atomic_ref_seq_cst<T>)`.
If `foo_seq_cst` silently converts its input to `atomic_ref_relaxed<T>`,
that might break callers' code, because correct use of relaxed atomics
might require fences or other synchronization.

Permitting the opposite conversion would not harm correctness,
but it might harm performance.  Consider the following example.

```c++
void foo_relaxed(atomic_ref_relaxed<int> counter);

void bar(int& counter) {
  atomic_ref_relaxed atomic_counter(counter);
  atomic_counter++;
  ...
  foo(atomic_counter);
  ...
  atomic_counter++;
  atomic_thread_fence(memory_order::acq_rel);
}
```

If users call `foo_relaxed`, they would reasonably expect
to need to use fences or other synchronization for correctness.
If the function actually uses sequentially consistent operations,
then users would pay for that synchronization unnecessarily.
This is why we do not permit conversions
in the so-called "safer" direction.

Functions that might need to perform atomic operations
with different memory orders on the same memory location
can use `atomic_ref<T>` instead.

One argument _for_ permitting conversions is that
C++26 lets users get the pointer out of an `atomic_ref` anyway,
due to the `address` member function
(see [P2835R7](https://wg21.link/p2835r7)).
That would let users write the conversion by hand anyway,
as the example below shows.

```c++
float x = 1.25f;
atomic_ref_seq_cst<float> x_ref(x);
// ...
atomic_ref_relaxed<float> x_ref2(
  *reinterpret_cast<float*>(x_ref.address()));
```

Our counterargument is that C++ lets users do all sorts of things
with `reinterpret_cast`.  Some of those things are undefined behavior.
We don't aim to prevent all possible ways users could abuse the
Standard Library; we just want to make it harder.
Getting the object address via `address` and calling `reinterpret_cast`
on it is a clear sign that the code is doing something special
and that readers need to pay close attention to it.
We would want such code to be explicit.

## Omit `compare_exchange_{weak,strong}` with user-specified `failure` memory order

The `atomic_ref` class template includes both three-parameter and
four-parameter versions of `compare_exchange_weak` and `compare_exchange_strong`.
The four-parameter versions have two separate `memory_order` parameters:
`success`, to use if the comparison is true,
and `failure`, to use if the comparison is false.

The three-parameter versions let users set the memory order on success,
but bind the memory order on failure as a function of the success memory order,
as specified in
[[atomics.ref.ops] 25](https://eel.is/c++draft/atomics.ref.generic#atomics.ref.ops-25).

> When only one `memory_order` argument is supplied, the value of `success` is `order`,
> and the value of `failure` is `order` except that a value of `memory_rder::acq_rel`
> shall be replaced by the value `memory_order::acquire` and a value of
> `memory_order::release` shall be replaced by the value `memory_order::relaxed`.

Given that the `success` memory order is the least constrained,
we define these functions in _`atomic-ref-bound`_ to use
the class' bound memory order `memory_ordering` as the `success` memory order,
and to set the `failure` memory order `load_ordering`
based on the rule from [atomics.ref.ops] 25.
For example, we define two-parameter `compare_exchange_weak`
in _`atomic-ref-bound`_ as follows.

```c++
constexpr bool compare_exchange_weak(
  value_type& expected, value_type desired) const noexcept;
```

*Effects*: Equivalent to:
`ref.compare_exchange_weak(expected, desired, memory_ordering, load_ordering);`

We do *not* define the following three-parameter analog
that would let users set the `failure` memory order separately.

```c++
constexpr bool compare_exchange_weak(value_type& expected,
  value_type desired, memory_order failure) const noexcept;
```

This is because it would be too confusing for the third parameter
to be the `success` memory order for `atomic_ref`,
but to be the opposite `failure` memory order for _`atomic-ref-bound`_.
Users who want the functionality of `atomic_ref`'s four-parameter
overloads should just use `atomic_ref`.

## No generic "proxy reference" accessor

The _`basic-atomic-accessor`_ class template looks generic enough
that it should work with many different kinds of proxy references,
not just `atomic_ref` and the bound atomic references proposed here.
Previous reviews suggested that we could rename this class and
make it available as a generic "proxy reference accessor."

We do not attempt to do this, because the Standard currently
has no precedent for defining a "proxy reference" concept
outside the context of iterators and Ranges.
That would make a proxy reference accessor design original research,
rather than a standardization of existing practice.

Our _`basic-atomic-accessor`_ design is not as generic
as an arbitrary proxy reference accessor could be.
First, `atomic_ref<T>` and the bound atomic references
proposed here all wrap a core language reference `T&`.
Second, _`basic-atomic-accessor<T, Reference>`_
uses a raw pointer `T*` as its `data_handle_type`,
and gets the core language reference to wrap
by accessing the pointer at a given index.
A fully generic proxy reference accessor would need to

1. wrap an existing arbitrary accessor `NestedAccessor`
    (that might have arbitrary `data_handle_type`,
    `reference`, and `offset_policy` types);

2. dispatch to `NestedAccessor::access` (that might have
    possibly arbitrary effects) in its `access` function;

3. have a proxy reference that can wrap the possibly arbitrary
    `NestedAccessor::reference`; and

4. dispatch to `NestedAccessor::offset` in its `offset` function,
    and produce a data handle for a proxy reference accessor
    that wraps the offset policy.

In order to design a generic "wrapping proxy reference,"
we would need a proxy reference concept.
It's not obvious to us how to write that concept.
Proxy references come in different forms.
Sometimes they are just values.  For example,

* `vector<bool>::const_reference` is just `bool`; and

* `linalg::scaled_accessor<SF, NA>::reference` is the result of
    multiplying two linear algebra value types
    (which could either be another linear algebra value type,
    or an expression template of one).

Sometimes a proxy reference is actually a `tuple`
(as with `ranges_{enumerate,zip}_view`) or
`pair` (as with `flat_map`).

Standard Algorithms concepts like `indirectly_readable` and
`indirectly_writeable` exist for iterators and Ranges.
The concepts always deal with iterators and reference types together.
For example, `indirectly_readable` constrains an iterator type.
The Standard does not describe proxy references by themselves.

The `mdspan` design does not depend on iterators or
Ranges machinery.  That has trade-offs.
For instance, the `mdspan` authors wanted it to be easy
for users who are not C++ experts to write a custom accessor.
Contrast that with writing a custom iterator,
a tricky enough task to motivate a Boost library
and a proposal in flight, [P2727](https://wg21.link/p2727).
It also avoids the correctness and performance complications
of attempting to define a multidimensional iterator.
However, it means that `mdspan`-based algorithms
need to build up their own concepts and traits machinery.

As an example of such machinery, how would one write
the analog of `views::as_const` for `mdspan` algorithms?
(This question motivates [P4311](https://wg21.link/p4311).)
As a part of that task, given a proxy reference type `P`,
how would one get a type `P_c` such that `P` is convertible to `P_c`,
and `P_c` is both syntactically and semantically read-only?
The "const version" of `atomic_ref<T>` is just `atomic_ref<const T>`.
Not all proxy reference types work like that, though.
For `vector<bool>::reference`, the const version is `bool`.

Even existing non-Ranges traits might not help us.
For example, `common_reference_t` of `P` and `P_c` should be `P_c`.
This works for `vector<bool>::reference`.
But for `atomic_ref`, GCC 16.1 (`-std=c++26 -Wall`) and
MSVC (v19.51 VS18.6, `/std::c++latest /W4`) disagree with
Clang 22.1.0 (`-std=c++26 -Wall`) and nvc++ 26.5 (`-std=c++23 -Wall`).
With GCC and MSVC,
`common_reference_t<atomic_ref<float>, atomic_ref<const float>>` is
`atomic_ref<const float>`, but with Clang and nvc++, it is `float`.
Please see the example below, which is available
[in Compiler Explorer](https://godbolt.org/z/TraMhKfYT).

```c++
#include <atomic>
#include <ranges>
#include <type_traits>
#include <vector>

#if defined(__clang__) || defined(__NVCOMPILER)
#  define CUSTOMIZE_BASIC_COMMON_REFERENCE 1
#endif

#if defined(CUSTOMIZE_BASIC_COMMON_REFERENCE)
template <
  class T,
  class U,
  template <class> class TQual,
  template <class> class UQual
> requires std::same_as<U, std::remove_const_t<T>>
struct std::basic_common_reference<
  std::atomic_ref<T>, std::atomic_ref<U>, TQual, UQual>
{
  using type = std::atomic_ref<const T>;
};

template <
  class T,
  class U,
  template <class> class TQual,
  template <class> class UQual
> requires std::same_as<U, std::remove_const_t<T>>
struct std::basic_common_reference<
  std::atomic_ref<U>, std::atomic_ref<T>, TQual, UQual>
{
  using type = std::atomic_ref<const T>;
};
#endif

int main() {
  // GCC and MSVC disagree with Clang and nvc++.
#if ! defined(__clang__) && ! defined(__NVCOMPILER)
  // GCC says it's atomic_ref<const float>.
  static_assert(std::is_same_v<
    std::common_reference_t<std::atomic_ref<float>, std::atomic_ref<const float>>,
    std::atomic_ref<const float>
  >);
#endif

#if defined(__clang__) || defined(__NVCOMPILER)
#  if defined(CUSTOMIZE_BASIC_COMMON_REFERENCE)
  static_assert(std::is_same_v<
    std::common_reference_t<std::atomic_ref<float>, std::atomic_ref<const float>>,
    std::atomic_ref<const float>
  >);
#  else
  static_assert(std::is_same_v<
    std::common_reference_t<std::atomic_ref<float>, std::atomic_ref<const float>>,
    float
  >);
#  endif
#endif

  static_assert(std::is_same_v<
    std::common_reference_t<
      std::vector<bool>::reference, bool
    >,
    bool
  >);
  static_assert(std::is_same_v<
    std::common_reference_t<
      bool, std::vector<bool>::reference
    >,
    bool
  >);

  return 0;
}
```

## Why this proposal improves safety

In LEWG's review of R3 on 2026-08-04, one reviewer brought up a concern
that our bound atomic reference types introduce a "sharp edge" that reduces safety,
because they expose users to memory orders other than sequential consistency.
If users of `atomic` and `atomic_ref` want a nondefault memory order,
they must ask for it explicitly on every atomic operation.
Users of `atomic_ref_relaxed` and `atomic_ref_acq_rel` would get
relaxed resp. acquire-release order on every atomic operation,
but the only time they would spell out the memory order
would be in the class name.  The reviewer perceives this as less safe.

We disagree with the reviewer for the following reasons.

1. Our proposed bound atomic reference types are actually more safe
    than `atomic_ref` when used as function parameters,
    because `atomic_ref` lets the function's implementation
    use any memory order, while our bound reference types
    declare and constrain the intended memory order at compile time.

2. Our proposed bound atomic accessor types
    make algorithms over arrays safer.

3. Atomic access itself is the sharpest edge.

### Bound atomic reference types are more safe than `atomic_ref`

Our proposed bound reference types are actually more safe
than the existing `atomic_ref` when used as function parameters.
This is because our bound reference types declare and constrain
the intended memory order at compile time,
while `atomic_ref` lets users supply any memory order they like.
This makes it harder to compose functions with `atomic_ref` parameters.
Consider the following example.

```c++
void perform_update_0(atomic_ref<float>& ref, float input) {
  ref.store(noncommutative_function(float(ref), input));
}

void perform_update_1(atomic_ref<float>& ref, float input) {
  ref.store_add(input);
}

void apply_updates(atomic_ref<float>& ref, float in0, float in1) {
  perform_update_0(ref, in0);
  perform_update_1(ref, in1);
}
```

The author of `apply_updates` likely expects
the two updates to happen in order.
If we let `x` be the "original" value of `float(ref)`,
then the expected outcome would be for `ref` to hold
`noncommutative_function(x, in0) + in1`.
However, if the (possibly different) author of `perform_update_0`
and `perform_update_1` changes the memory order to relaxed,
then an execution of `apply_updates` might instead store
`noncommutative_function(x + in1, in0)`.
The author of `apply_updates` would have no way to know
by inspection of the function declarations
what memory order the functions use.
If we change the example to use `atomic_ref_seq_cst<float>`
instead of `atomic_ref<float>` as parameter types throughout,
then `apply_updates` can declare its required memory order.
Since we do not permit conversions between _`atomic-ref-bound`_
of different memory orders, users would get a compilation order
if this requirement were not satisfied.

```c++
void perform_update_0(atomic_ref_seq_cst<float> ref, float input) {
  ref.store(noncommutative_function(input));
}

void perform_update_1(atomic_ref_seq_cst<float> ref, float input) {
  ref.store_add(input);
}

void apply_updates(atomic_ref_seq_cst<float> ref, float in0, float in1) {
  perform_update_0(ref, in0);
  perform_update_1(ref, in1);
}
```

### Bound accessor types make array-based algorithms safer

The [Kokkos project](https://github.com/kokkos/kokkos) has
over a decade of practical experience with `atomic_ref_*` analogs.
Users almost exclusively access them through Kokkos' analog of `mdspan`
in parallel algorithms.  They almost always do so through
overloaded arithmetic operators in computations
that are mathematically associative and commutative.
This fits the intended use case of `atomic_ref` as explained in
[P0019](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0019r8.html).
Users aren't building complicated lock-free algorithms
with these reference types.
They hardly ever create or return one explicitly
or spell out their names.

Idiomatic `mdspan`-based algorithm interfaces
help lead to safe use of our bound atomic reference types.
Algorithms declare their access intent
through the template arguments of their `mdspan` parameters.
If an `mdspan` parameter has accessor `atomic_accessor_seq_cst`,
for example, then the algorithm will only perform sequentially consistent
atomic updates to the elements of that `mdspan`.
This is the array analog of a function
with an `atomic_ref_seq_cst` parameter.

Our atomic accessor types thus work together
with our bound reference types to improve safety
of array-based algorithm interfaces.
[P0019](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0019r8.html)
explains that the intent of `atomic_ref`
is to support "phases" of computation that alternate
between parallel algorithms with atomic accesses,
and parallel algorithms with nonatomic accesses.
Our atomic accessor types let the atomic phases
declare their array access intent.
Each phase's implementation then does not need to create
`atomic_ref` or _`atomic-ref-bound`_ objects explicitly.

### Atomic access itself is the sharpest edge

The C++ Standard includes parallel algorithms like `std::ranges::for_each`.
For `parallel_policy` and `parallel_unsequenced_policy`,
the Standard parallel algorithms promise at most
parallel forward progress across function invocations.
This means that if users might experience deadlock
if they try to use blocking synchronization.
The same might happen with explicitly user-created
`thread` or `jthread` threads, because implementations
are not required to promise concurrent forward progress.
Both `atomic` and `atomic_ref` make it easy
for users to implement synchronization by hand,
perhaps without even meaning to.
For example, users can create a barrier without even knowing
what a barrier is, by incrementing a counter atomically
and spinning until the counter reaches some value.
Software developers need tools like thread sanitizers to find such cases.
Thus, we do not think of sequential consistency as necessarily "safer."
Users still need to understand forward progress guarantees and deadlock
in order to use atomic operations correctly.

An `atomic_ref` instance wraps an existing object.
This already makes it possible for users to invoke undefined behavior
by accessing the object nonatomically
while atomic accesses are still in flight.
It's still undefined behavior even if all those atomic accesses
use sequentially consistent order.
[P0019](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0019r8.html)
explains why we accept this sharper interface.
The main intended use case of `atomic_ref` is to support
atomic operations on elements of existing array allocations.
Without `atomic_ref`, those applications would need to use
arrays of `atomic<T>` instead of arrays of `T`.
That would result in a serious performance penalty
for phases of computation where atomic access is unnecessary.

# Open questions

This proposal uses alias templates to exposition-only types for `atomic_ref_relaxed`, `atomic_accessor`, etc.
However, we do not want to prescribe a particular implementation.
For instance, if an implementer wished to derive from a type like _`atomic-ref-bound`_ (to get more
user-friendly name mangling, which is something not normally covered by the Standard), would
they be allowed to do so?  We believe an alias template to an exposition-only type is not observable
from the point of view of the Standard and such an implementation would be allowed, but we request clarification on this.
Potentially this is something we can leave to LWG review to suggest wording that would achieve that goal.

<!--

 /$$      /$$                           /$$ /$$
| $$  /$ | $$                          | $$|__/
| $$ /$$$| $$  /$$$$$$   /$$$$$$   /$$$$$$$ /$$ /$$$$$$$   /$$$$$$
| $$/$$ $$ $$ /$$__  $$ /$$__  $$ /$$__  $$| $$| $$__  $$ /$$__  $$
| $$$$_  $$$$| $$  \ $$| $$  \__/| $$  | $$| $$| $$  \ $$| $$  \ $$
| $$$/ \  $$$| $$  | $$| $$      | $$  | $$| $$| $$  | $$| $$  | $$
| $$/   \  $$|  $$$$$$/| $$      |  $$$$$$$| $$| $$  | $$|  $$$$$$$
|__/     \__/ \______/ |__/       \_______/|__/|__/  |__/ \____  $$
                                                          /$$  \ $$
                                                         |  $$$$$$/
                                                          \______/
-->

# Wording

The proposed changes are relative to [N5054](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2026/n5054.pdf):

## Bound atomic ref
### Change the header synopsis for atomic in [atomics.syn]:

[Intent is for `atomic_ref_*` to be freestanding.  We don't need to mark this
because [atomics.syn] is already marked "mostly freestanding."]{.ednote}

```
// [atomics.ref.pointer], @_class template_@ atomic_ref
template<class T> struct atomic_ref<T>;

```
::: add
```
// [atomics.ref.bounded], class template @_atomic-ref-bound_@]
template<class T, class MemoryOrder> @_atomic-ref-bound_@; // @_exposition only_@
template<class T> using atomic_ref_relaxed = @_atomic-ref-bound_@<T, memory_order_relaxed>;
template<class T> using atomic_ref_acq_rel = @_atomic-ref-bound_@<T, memory_order_acq_rel>;
template<class T> using atomic_ref_seq_cst = @_atomic-ref-bound_@<T, memory_order_seq_cst>;
```
:::
```

// [atomics.types.generic], class template atomic
template<class T> struct atomic<T>;
// [atomics.types.pointer], partial specialization for pointers
template<class T> struct atomic<T*>;
```

### Add the following just before [atomics.types.generic]:

<b>Class template _`atomic-ref-bound`_ [atomics.refbound]</b>

<b>Exposition only helper [atomics.refbound.helpers]</b>

```c++
template<class T>
constexpr bool @_atomic-ref-non-generic-type()_@;
```

Returns: `true` if the class `atomic_ref<T>` has a member type definition `difference_type`, otherwise `false`.

[Note: This is true for a set of defined integral types, floating point types, and pointers.]

<b>Generic _`atomic-ref-bound`_ [atomics.refbound.generic]</b>

<b>Overview [atomics.refbound.generic.overview]</b>

[The friend declaration may be unnecessary.
Mark didn't need it in an implementation.]{.ednote}

```c++
template <class T, memory_order MemoryOrder>
struct @_atomic-ref-bound_@ {                   // exposition only
private:
  using @_atomic-ref-unbound_@ = atomic_ref<T>; // exposition only
  @_atomic-ref-unbound_@ @_ref_@;                   // exposition only

  static constexpr memory_order store_ordering =
    MemoryOrder == memory_order_acq_rel ? memory_order_release
                                        : MemoryOrder; // exposition only

  static constexpr memory_order load_ordering =
    MemoryOrder == memory_order_acq_rel ? memory_order_acquire
                                        : MemoryOrder; // exposition only

  template<class T> friend class @_atomic-ref-bound_@<T, MemoryOrder>;

public:
  using value_type = remove_cv_t<T>;
  static constexpr memory_order memory_ordering = MemoryOrder;
  static constexpr size_t required_alignment = @_atomic-ref-unbound_@::required_alignment;

  static constexpr bool is_always_lock_free = @_atomic-ref-unbound_@::is_always_lock_free;
  bool is_lock_free() const noexcept;

  constexpr explicit @_atomic-ref-bound_@(T&);
  explicit @_atomic-ref-bound_@(T&&) = delete;
  constexpr @_atomic-ref-bound_@(const @_atomic-ref-bound_@&) noexcept;
  template<class U>
    constexpr @_atomic-ref-bound_@(const @_atomic-ref-bound_@<U, memory_ordering>&) noexcept;
  @_atomic-ref-bound_@& operator=(const @_atomic-ref-bound_@&) = delete;

  constexpr void store(value_type desired) const noexcept;
  constexpr value_type operator=(value_type desired) const noexcept;
  constexpr value_type load() const noexcept;
  constexpr operator value_type() const noexcept;

  constexpr value_type exchange(value_type desired) const noexcept;
  constexpr bool compare_exchange_weak(value_type& expected, value_type desired) const noexcept;
  constexpr bool compare_exchange_strong(value_type& expected, value_type desired) const noexcept;

  constexpr void wait(value_type old) const noexcept;
  constexpr void notify_one() const noexcept;
  constexpr void notify_all() const noexcept;

  constexpr auto address() const noexcept;
};
```
[1]{.pnum} Class _`atomic-ref-bound`_ is for exposition only.

[2]{.pnum} *Mandates:*

   * [2.1]{.pnum} `is_trivially_copyable_v<T>` is `true`.

**Operations [atomics.refbound.generic.ops]**
```c++
bool is_lock_free() const noexcept;
```
[1]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.is_lock_free();`

```c++
constexpr explicit @_atomic-ref-bound_@(T& t);
```

[2]{.pnum} *Preconditions:* The referenced object is aligned to `required_alignment`.

[3]{.pnum} *Postconditions:* _`ref`_ references the object referenced by `t`.

[4]{.pnum} *Throws:* Nothing.

```c++
constexpr @_atomic-ref-bound_@(const @_atomic-ref-bound_@& other) noexcept;
```
[5]{.pnum} *Effects:* Direct-non-list-initializes _`ref`_ with `other.`_`ref`_.

```c++
template<class U>
  constexpr @_atomic-ref-bound_@(const @_atomic-ref-bound_@<U>& other) noexcept;
```

[6]{.pnum} *Constraints:*

   * [6.1]{.pnum} `T` and `U` are similar types ([conv.qual]), and

   * [6.2]{.pnum} `is_convertible_v<U*, T*>` is `true`.

[7]{.pnum} *Effects:* Direct-non-list-initializes _`ref`_ with `other.`_`ref`_.

```c++
constexpr void store(value_type desired) const noexcept;
```
[8]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[9]{.pnum} *Effects:* Equivalent to: _`ref`_`.store(desired, store_ordering);`

```c++
constexpr value_type operator=(value_type desired) const noexcept;
```
[10]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[11]{.pnum} *Effects:* Equivalent to: `store(desired); return desired;`

```c++
constexpr value_type load() const noexcept;
```
[12]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.load(load_ordering);`

```c++
constexpr operator value_type() const noexcept;
```
[13]{.pnum} *Effects:* Equivalent to: `return load();`

```c++
constexpr value_type exchange(value_type desired) const noexcept;
```
[14]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[15]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.exchange(desired, memory_ordering);`


```c++
constexpr bool compare_exchange_weak(value_type& expected, value_type desired) const noexcept;
```
[16]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[17]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.compare_exchange_weak(expected, desired, memory_ordering, load_ordering);`

```c++
constexpr bool compare_exchange_strong(value_type& expected, value_type desired) const noexcept;
```
[18]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[19]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.compare_exchange_strong(expected, desired, memory_ordering, load_ordering);`

```c++
constexpr void wait(value_type old) const noexcept;
```
[20]{.pnum} *Effects:* Equivalent to: _`ref`_`.wait(old, load_ordering);`

```c++
constexpr void notify_one() const noexcept;
```
[21]{.pnum} *Effects:* Equivalent to: _`ref`_`.notify_one();`

```c++
constexpr void notify_all() const noexcept;
```
[22]{.pnum} *Effects:* Equivalent to: _`ref`_`.notify_all();`

```c++
constexpr auto address() const noexcept;
```
[23]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.address();`

<b>Non-generic _`atomic-ref-bound`_ [atomics.refbound.nongeneric]</b>

<b>Overview [atomics.refbound.nongeneric.overview]</b>

```c++
template <class T, memory_order MemoryOrder>
requires(@_atomic-ref-non-generic-type_@<T>())
struct @_atomic-ref-bound_@<T, MemoryOrder> {  // exposition only
private:
  using @_atomic-ref-unbound_@ = atomic_ref<T>;  // exposition only
  @_atomic-ref-unbound_@ @_ref_@;                    // exposition only

  static constexpr memory_order store_ordering =
    MemoryOrder == memory_order_acq_rel ? memory_order_release
                                        : MemoryOrder;  // exposition only

  static constexpr memory_order load_ordering =
    MemoryOrder == memory_order_acq_rel ? memory_order_acquire
                                        : MemoryOrder;  // exposition only

  template<class T> friend class @_atomic-ref-bound_@<T, MemoryOrder>;

public:
  using value_type = remove_cv_t<T>;

private:
  static constexpr bool @_is-integral-value_@ =       // exposition only
    is_integral_v<value_type> && !is_same_v<value_type, bool>;
  static constexpr bool @_is-floating-point-value_@ = // exposition only
    is_floating_point_v<value_type>;
  static constexpr bool @_is-pointer-value_@ =        // exposition only
    is_pointer_v<value_type>;

public:
  using difference_type = @_atomic-ref-unbound_@::difference_type;
  static constexpr memory_order memory_ordering = MemoryOrder;
  static constexpr size_t required_alignment =
    @_atomic-ref-unbound_@::required_alignment;

  static constexpr bool is_always_lock_free =
    @_atomic-ref-unbound_@::is_always_lock_free;

  // [atomics.refbound.nongeneric.ops] Operations common with generic version
  bool is_lock_free() const noexcept;

  constexpr explicit @_atomic-ref-bound_@(T& t);
  explicit @_atomic-ref-bound_@(T&&) = delete;
  template<class U>
    constexpr @_atomic-ref-bound_@(const @_atomic-ref-bound_@<U, memory_ordering>&) noexcept;
  constexpr @_atomic-ref-bound_@(const @_atomic-ref-bound_@&) noexcept;
  constexpr @_atomic-ref-bound_@& operator=(const @_atomic-ref-bound_@&) = delete;

  constexpr void store(value_type desired) const noexcept;
  constexpr value_type operator=(value_type desired) const noexcept;
  constexpr value_type load() const noexcept;
  constexpr operator value_type() const noexcept;

  constexpr value_type exchange(value_type desired) const noexcept;
  constexpr bool compare_exchange_weak(value_type& expected, value_type desired) const noexcept;
  constexpr bool compare_exchange_strong(value_type& expected, value_type desired) const noexcept;

  constexpr void wait(value_type old) const noexcept;
  constexpr void notify_one() const noexcept;
  constexpr void notify_all() const noexcept;

  constexpr auto address() const noexcept;

  // [atomics.refbound.nongeneric.common] Common operations
  constexpr value_type fetch_add(difference_type operand) const noexcept
  constexpr value_type fetch_sub(difference_type operand) const noexcept;
  constexpr value_type fetch_max(value_type operand) const noexcept;
  constexpr value_type fetch_min(value_type operand) const noexcept;

  constexpr void store_add(difference_type operand) const noexcept;
  constexpr void store_sub(difference_type operand) const noexcept;
  constexpr void store_max(value_type operand) const noexcept;
  constexpr void store_min(value_type operand) const noexcept;

  constexpr value_type operator+=(difference_type operand) const noexcept;
  constexpr value_type operator-=(difference_type operand) const noexcept;

  // [atomics.refbound.nongeneric.integral] Integral only operations
  constexpr value_type fetch_and(value_type operand) const noexcept;
  constexpr value_type fetch_or (value_type operand) const noexcept;
  constexpr value_type fetch_xor(value_type operand) const noexcept;

  constexpr void store_and(value_type operand) const noexcept;
  constexpr void store_or (value_type operand) const noexcept;
  constexpr void store_xor(value_type operand) const noexcept;

  constexpr value_type operator&=(value_type operand) const noexcept;
  constexpr value_type operator|=(value_type operand) const noexcept;
  constexpr value_type operator^=(value_type operand) const noexcept;

  // [atomics.refbound.nongeneric.floatingpoint] Floating point only operations
  constexpr value_type fetch_fmaximum(value_type operand) const noexcept;
  constexpr value_type fetch_fminimum(value_type operand) const noexcept;
  constexpr value_type fetch_fmaximum_num(value_type operand) const noexcept;
  constexpr value_type fetch_fminimum_num(value_type operand) const noexcept;

  constexpr void store_fmaximum(value_type operand) const noexcept;
  constexpr void store_fminimum(value_type operand) const noexcept;
  constexpr void store_fmaximum_num(value_type operand) const noexcept;
  constexpr void store_fminimum_num(value_type operand) const noexcept;

  // [atomics.refbound.nongeneric.integralpointer] Integral and pointer type operations
  constexpr value_type operator++(int) const noexcept;
  constexpr value_type operator++() const noexcept;
  constexpr value_type operator--(int) const noexcept;
  constexpr value_type operator--() const noexcept;
};
```

[1]{.pnum} Class _`atomic-ref-bound`_ is for exposition only.

[2]{.pnum} *Mandates:*

   * [2.1]{.pnum} `is_trivially_copyable_v<T>` is `true`.

<i>[Note:</i> 
Unlike `atomic_ref`, the memory ordering for the arithmetic operators is `MemoryOrder`, which is not
necessarily `memory_order_seq_cst`.
<i>-- end note]</i>

**Generic Operations [atomics.refbound.nongeneric.ops]**
```c++
bool is_lock_free() const noexcept;
```
[1]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.is_lock_free();`

```c++
constexpr explicit @_atomic-ref-bound_@(T& t);
```

[2]{.pnum} *Preconditions:* The referenced object is aligned to `required_alignment`.

[3]{.pnum} *Effects:* Direct-non-list-initializes _`ref`_ with `t`.

[4]{.pnum} *Throws:* Nothing.

```c++
constexpr @_atomic-ref-bound_@(const @_atomic-ref-bound_@& other) noexcept;
```
[7]{.pnum} *Effects:* Direct-non-list-initializes `ref` with `other.ref`.

```c++
template<class U>
  constexpr @_atomic-ref-bound_@(const @_atomic-ref-bound_@<U>& other) noexcept;
```

[6]{.pnum} *Constraints:*

   * [6.1]{.pnum} `T` and `U` are similar types ([conv.qual]), and

   * [6.2]{.pnum} `is_convertible_v<U*, T*>` is `true`.

[7]{.pnum} *Effects:* Direct-non-list-initializes _`ref`_ with `other.`_`ref`_.

```c++
constexpr void store(value_type desired) const noexcept;
```
[8]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[9]{.pnum} *Effects:* Equivalent to: _`ref`_`.store(desired, store_ordering);`

```c++
constexpr value_type operator=(value_type desired) const noexcept;
```
[10]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[11]{.pnum} *Effects:* Equivalent to: `store(desired); return desired;`

```c++
constexpr value_type load() const noexcept;
```
[12]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.load(load_ordering);`

```c++
constexpr operator value_type() const noexcept;
```
[13]{.pnum} *Effects:* Equivalent to: `return load();`

```c++
constexpr value_type exchange(value_type desired) const noexcept;
```
[14]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[15]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.exchange(desired, memory_ordering);`


```c++
constexpr bool compare_exchange_weak(value_type& expected, value_type desired) const noexcept;
```
[16]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[17]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.compare_exchange_weak(expected, desired, memory_ordering, load_ordering);`

```c++
constexpr bool compare_exchange_strong(value_type& expected, value_type desired) const noexcept;
```
[18]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[19]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.compare_exchange_strong(expected, desired, memory_ordering, load_ordering);`

```c++
constexpr void wait(T old) const noexcept;
```
[20]{.pnum} *Effects:* Equivalent to: _`ref`_`.wait(old, load_ordering);`

```c++
constexpr void notify_one() const noexcept;
```
[21]{.pnum} *Effects:* Equivalent to: _`ref`_`.notify_one();`

```c++
constexpr void notify_all() const noexcept;
```
[22]{.pnum} *Effects:* Equivalent to: _`ref`_`.notify_all();`

```c++
constexpr auto address() const noexcept;
```
[23]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.address();`

**Common Operations [atomics.refbound.nongeneric.common]**
```c++
constexpr value_type fetch_add(difference_type operand) const noexcept;
```
[1]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[2]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.fetch_add(operand, memory_ordering);`

```c++
constexpr value_type fetch_sub(difference_type operand) const noexcept;
```

[3]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[4]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.fetch_sub(operand, memory_ordering);`

```c++
constexpr value_type fetch_max(value_type operand) const noexcept;
```
[5]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[6]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.fetch_max(operand, memory_ordering);`

```c++
constexpr value_type fetch_min(value_type operand) const noexcept;
```

[7]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[8]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.fetch_min(operand, memory_ordering);`

```c++
constexpr void store_add(difference_type operand) const noexcept;
```
[9]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[10]{.pnum} *Effects:* Equivalent to: _`ref`_`.store_add(operand, memory_ordering);`

```c++
constexpr void store_sub(difference_type operand) const noexcept;
```
[11]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[12]{.pnum} *Effects:* Equivalent to: _`ref`_`.store_sub(operand, memory_ordering);`

```c++
constexpr void store_max(value_type operand) const noexcept;
```
[13]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[14]{.pnum} *Effects:* Equivalent to: _`ref`_`.store_max(operand, memory_ordering);`

```c++
constexpr void store_min(value_type operand) const noexcept;
```
[15]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[16]{.pnum} *Effects:* Equivalent to: _`ref`_`.store_min(operand, memory_ordering);`

```c++
constexpr value_type operator+=(difference_type operand) const noexcept;
```
[17]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[18]{.pnum} *Effects:* Equivalent to: `return fetch_add(operand) + operand;`

```c++
constexpr value_type operator-=(difference_type operand) const noexcept;
```
[19]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[20]{.pnum} *Effects:* Equivalent to: `return fetch_sub(operand) - operand;`

**Integral Operations [atomics.refbound.nongeneric.integral]**
```c++
constexpr value_type fetch_and(value_type operand) const noexcept;
```
[1]{.pnum} *Constraints:* _`is-integral-value`_ `&& !is_const_v<T>` is `true`.

[2]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.fetch_and(operand, memory_ordering);`

```c++
constexpr value_type fetch_or(value_type operand) const noexcept;
```
[3]{.pnum} *Constraints:* _`is-integral-value`_ `&& !is_const_v<T>` is `true`.

[4]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.fetch_or(operand, memory_ordering);`

```c++
constexpr value_type fetch_xor(value_type operand) const noexcept;
```
[5]{.pnum} *Constraints:* _`is-integral-value`_ `&& !is_const_v<T>` is `true`.

[6]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.fetch_xor(operand, memory_ordering);`

```c++
constexpr void store_and(value_type operand) const noexcept;
```
[7]{.pnum} *Constraints:* _`is-integral-value`_ `&& !is_const_v<T>` is `true`.

[8]{.pnum} *Effects:* Equivalent to: _`ref`_`.store_and(operand, memory_ordering);`

```c++
constexpr void store_or(value_type operand) const noexcept;
```
[9]{.pnum} *Constraints:* _`is-integral-value`_ `&& !is_const_v<T>` is `true`.

[10]{.pnum} *Effects:* Equivalent to: _`ref`_`.store_or(operand, memory_ordering);`

```c++
constexpr void store_xor(value_type operand) const noexcept;
```
[11]{.pnum} *Constraints:* _`is-integral-value`_ `&& !is_const_v<T>` is `true`.

[12]{.pnum} *Effects:* Equivalent to: _`ref`_`.store_xor(operand, memory_ordering);`

```c++
constexpr value_type operator&=(value_type operand) const noexcept;
```
[13]{.pnum} *Constraints:* _`is-integral-value`_ `&& !is_const_v<T>` is `true`.

[14]{.pnum} *Effects:* Equivalent to: `return fetch_and(operand) & operand;`

```c++
constexpr value_type operator|=(value_type operand) const noexcept;
```
[15]{.pnum} *Constraints:* _`is-integral-value`_ `&& !is_const_v<T>` is `true`.

[16]{.pnum} *Effects:* Equivalent to: `return fetch_or(operand) | operand;`

```c++
constexpr value_type operator^=(value_type operand) const noexcept;
```
[17]{.pnum} *Constraints:* _`is-integral-value`_ `&& !is_const_v<T>` is `true`.

[18]{.pnum} *Effects:* Equivalent to: `return fetch_xor(operand) ^ operand;`

**Floating Point Operations [atomics.refbound.nongeneric.floatingpoint]**
```c++
constexpr value_type fetch_fmaximum(value_type operand) const noexcept;
```
[1]{.pnum} *Constraints:* _`is-floating-point-value`_ `&& !is_const_v<T>` is `true`.

[2]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.fetch_fmaximum(operand, memory_ordering);`

```c++
constexpr value_type fetch_fminimum(value_type operand) const noexcept;
```
[3]{.pnum} *Constraints:* _`is-floating-point-value`_ `&& !is_const_v<T>` is `true`.

[4]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.fetch_fminimum(operand, memory_ordering);`

```c++
constexpr value_type fetch_fmaximum_num(value_type operand) const noexcept;
```
[5]{.pnum} *Constraints:* _`is-floating-point-value`_ `&& !is_const_v<T>` is `true`.

[6]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.fetch_fmaximum_num(operand, memory_ordering);`

```c++
constexpr value_type fetch_fminimum_num(value_type operand) const noexcept;
```
[7]{.pnum} *Constraints:* _`is-floating-point-value`_ `&& !is_const_v<T>` is `true`.

[8]{.pnum} *Effects:* Equivalent to: `return` _`ref`_`.fetch_fminimum_num(operand, memory_ordering);`

```c++
constexpr void store_fmaximum(value_type operand) const noexcept;
```
[9]{.pnum} *Constraints:* _`is-floating-point-value`_ `&& !is_const_v<T>` is `true`.

[10]{.pnum} *Effects:* Equivalent to: _`ref`_`.store_fmaximum(operand, memory_ordering);`

```c++
constexpr void store_fminimum(value_type operand) const noexcept;
```
[11]{.pnum} *Constraints:* _`is-floating-point-value`_ `&& !is_const_v<T>` is `true`.

[12]{.pnum} *Effects:* Equivalent to: _`ref`_`.store_fminimum(operand, memory_ordering);`

```c++
constexpr void store_fmaximum_num(value_type operand) const noexcept;
```
[13]{.pnum} *Constraints:* _`is-floating-point-value`_ `&& !is_const_v<T>` is `true`.

[14]{.pnum} *Effects:* Equivalent to: _`ref`_`.store_fmaximum_num(operand, memory_ordering);`

```c++
constexpr void store_fminimum_num(value_type operand) const noexcept;
```
[15]{.pnum} *Constraints:* _`is-floating-point-value`_ `&& !is_const_v<T>` is `true`.

[16]{.pnum} *Effects:* Equivalent to: _`ref`_`.store_fminimum_num(operand, memory_ordering);`

**Integral and Pointer Operations [atomics.refbound.nongeneric.integralpointer]**
```c++
constexpr value_type operator++(int) const noexcept;
```
[1]{.pnum} *Constraints:*

   * _`is-integral-value`_ `||` _`is-pointer-value`_ is `true`, and

   * `is_const_v<T>` is `false`.

[2]{.pnum} *Effects:* Equivalent to: `return fetch_add(1);`

```c++
constexpr value_type operator++() const noexcept;
```
[3]{.pnum} *Constraints:*

   * _`is-integral-value`_ `||` _`is-pointer-value`_ is `true`, and

   * `is_const_v<T>` is `false`.

[4]{.pnum} *Effects:* Equivalent to: `return fetch_add(1) + 1;`

```c++
constexpr value_type operator--(int) const noexcept;
```
[5]{.pnum} *Constraints:*

   * _`is-integral-value`_ `||` _`is-pointer-value`_ is `true`, and

   * `is_const_v<T>` is `false`.

[6]{.pnum} *Effects:* Equivalent to: `return fetch_sub(1);`

```c++
constexpr value_type operator--() const noexcept;
```
[7]{.pnum} *Constraints:*

   * _`is-integral-value`_ `||` _`is-pointer-value`_ is `true`, and

   * `is_const_v<T>` is `false`.

[8]{.pnum} *Effects:* Equivalent to: `return fetch_sub(1) - 1;`



<b>Memory Order Specific Atomic Refs [atomics.refbound.order]</b>

```c++
// all freestanding
namespace std {
template<class T>
using atomic_ref_relaxed = @_atomic-ref-bound_@<T, memory_order_relaxed>;

template<class T>
using atomic_ref_acq_rel = @_atomic-ref-bound_@<T, memory_order_acq_rel>;

template<class T>
using atomic_ref_seq_cst = @_atomic-ref-bound_@<T, memory_order_seq_cst>;
}
```

### In [version.syn]:

Update the feature test macro `__cpp_lib_atomic_ref`.

## Atomic Accessors

### Add to the mdspan header synopsis in [mdspan.syn]

```
// [mdspan.accessor.default], class template default_accessor
template<class ElementType>
  class default_accessor;

```
::: add
```
// [atomics.accessor.atomic], class template @_basic-atomic-accessor_@
template<class T, class ReferenceType> class @_basic-atomic-accessor_@; // exposition only
template<class T> using atomic_accessor = @_basic-atomic-accessor_@<T, atomic_ref<T>>;
template<class T> using atomic_accessor_relaxed = @_basic-atomic-accessor_@<T, atomic_ref_relaxed<T>>;
template<class T> using atomic_accessor_acq_rel = @_basic-atomic-accessor_@<T, atomic_ref_acq_rel<T>>;
template<class T> using atomic_accessor_seq_cst = @_basic-atomic-accessor_@<T, atomic_ref_seq_cst<T>>;

```
:::
```
// [mdspan.mdspan], class template mdspan
template<class ElementType, class Extents, class LayoutPolicy = layout_right,
         class AccessorPolicy = default_accessor<ElementType>>
  class mdspan;
```

### Put the following before [mdspan.mdspan]:

<b>Class template _`basic-atomic-accessor`_ [mdspan.accessor.atomic.basic]</b>

<b>General [mdspan.accessor.atomic.basic.overview]</b>

```c++
template <class ElementType, class ReferenceType>
struct @_basic-atomic-accessor_@ {  // exposition only
  using offset_policy = @_basic-atomic-accessor_@;
  using element_type = ElementType;
  using reference = ReferenceType;
  using data_handle_type = ElementType*;

  constexpr @_basic-atomic-accessor_@() noexcept = default;

  template <class OtherElementType>
  constexpr @_basic-atomic-accessor_@(default_accessor<OtherElementType>) noexcept;

  template <class OtherElementType>
  constexpr @_basic-atomic-accessor_@(@_basic-atomic-accessor<OtherElementType, ReferenceType>_@) noexcept;

  constexpr reference access(data_handle_type p, size_t i) const noexcept;
  constexpr data_handle_type offset(data_handle_type p, size_t i) const noexcept;
};
```

[1]{.pnum} Class _`basic-atomic-accessor`_ is for exposition only.

[2]{.pnum} _`basic-atomic-accessor`_ meets the accessor policy requirements.

[3]{.pnum} `ElementType` is required to be a complete object type that is neither an abstract class type nor an array type.

[4]{.pnum} Each specialization of _`basic-atomic-accessor`_ is a trivially copyable type that models `semiregular`.

[5]{.pnum} `[0, n)` is an accessible range for an object `p` of type `data_handle_type` and an object of type _`basic-atomic-accessor`_ if and only if `[p, p+n)` is a valid range.

<b>Members [mdspan.accessor.atomic.basic.members]</b>

```c++
template <class OtherElementType>
constexpr @_basic-atomic-accessor_@(default_accessor<OtherElementType>) noexcept {}

template <class OtherElementType>
constexpr @_basic-atomic-accessor_@(@_basic-atomic-accessor_@<OtherElementType, ReferenceType>) noexcept {}
```

[1]{.pnum} *Constraints:* `is_convertible_v<OtherElementType (*)[], element_type (*)[]>` is `true`.

```c++
constexpr reference access(data_handle_type p, size_t i) const noexcept;
```

[2]{.pnum} *Effects:* Equivalent to `return reference(p[i]);`

```c++
constexpr data_handle_type offset(data_handle_type p, size_t i) const noexcept;
```

[3]{.pnum} *Effects:* Equivalent to `return p + i;`

<b>Atomic accessors [mdspan.accessor.atomic.bounded]</b>
```c++
namespace std {
template <class ElementType>
using atomic_accessor = @_basic-atomic-accessor_@<ElementType, atomic_ref<ElementType>>;

template <class ElementType>
using atomic_accessor_relaxed = @_basic-atomic-accessor_@<ElementType, atomic_ref_relaxed<ElementType>>;

template <class ElementType>
using atomic_accessor_acq_rel = @_basic-atomic-accessor_@<ElementType, atomic_ref_acq_rel<ElementType>>;

template <class ElementType>
using atomic_accessor_seq_cst = @_basic-atomic-accessor_@<ElementType, atomic_ref_seq_cst<ElementType>>;
}
```

## In [version.syn]

Add the following feature test macro:

`#define __cpp_lib_atomic_accessors YYYYMML // also in <mdspan>`

# Acknowledgments

Sandia National Laboratories is a multimission laboratory managed and operated by National Technology and
Engineering Solutions of Sandia, LLC., a wholly owned subsidiary of Honeywell International, Inc., for the U.S. Department of Energy’s National Nuclear Security Administration under Grant DE-NA-0003525. 

This manuscript has been authored by UTBattelle, LLC, under Grant DE-AC05-00OR22725 with the
U.S. Department of Energy (DOE). 

This work was supported
by Exascale Computing Project 17-SC-20-SC, a joint project of
the U.S. Department of Energy’s Office of Science and
National Nuclear Security Administration, responsible for
delivering a capable exascale ecosystem, including software,
applications, and hardware technology, to support the
nation’s exascale computing imperative.

This research used resources of the Argonne Leadership Computing Facility, which is a DOE Office of Science User Facility supported under Contract DE-AC02-06CH11357.
