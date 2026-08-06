---
title: "Atomic Refs Bound to Memory Orderings & Atomic Accessors"
document: D2689R3
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
- fix constructor wording

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

- Renamed `atomic-ref-bounded` to `atomic-ref-bound`
- Renamed `atomic-ref-unbounded` to `atomic-ref-unbound`
- Fixed the wording for `basic-atomic-accessor::offset`
- Fixed the wording for `basic-atomic-accessor::access`
- If P2616R3 (Making std::atomic notification/wait operations usable in more situations) is also approved, similar changes should be applied to `atomic-ref-bound` as well

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

- Added `atomic-ref-bounded` exposition-only template for an `atomic_ref` like type bounded to a particular `memory_order`
- Added `atomic_ref_relaxed`, `atomic_ref_acq_rel` and `atomic_ref_seq_cst` alias templates
- Added `basic-atomic-accessor` exposition-only template
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

# Rational

This proposal adds three `atomic_ref` like types each bound to a particular `memory_order`.
The API differs from `atomic_ref` in that the `memory_order` cannot be specified at run time; i.e., none of its member functions
take a `memory_order` parameter.
In the specific case of `atomic_ref_acq_rel`, loads are done via `memory_order_acquire` and stores are done via `memory_order_release`.

<table>
<thead><tr><th>Atomic Ref</th><th>`memory_order`</th><th>Loads</th><th>Stores</th></tr></thead>
<tbody>
<tr><td>`atomic_ref_relaxed`</td><td>`memory_order_relaxed`</td><td>`memory_order_relaxed`</td><td>`memory_order_relaxed`</td></tr>
<tr><td>`atomic_ref_acq_rel`</td><td>`memory_order_acq_rel`</td><td>`memory_order_acquire`</td><td>`memory_order_release`</td></tr>
<tr><td>`atomic_ref_seq_cst`</td><td>`memory_order_seq_cst`</td><td>`memory_order_seq_cst`</td><td>`memory_order_seq_cst`</td></tr>
</tbody>
</table>


This proposal also adds four atomic accessors to be used with `mdspan`.
They only differ with their `reference` types and all operations are performed atomically by using that corresponding reference type.

<table>
<thead><tr><th>Accessor</th><th>`reference`</th></tr></thead>
<tbody>
<tr><td>`atomic_accessor`</td><td>`atomic_ref`</td></tr>
<tr><td>`atomic_accessor_relaxed`</td><td>`atomic_ref_relaxed`</td></tr>
<tr><td>`atomic_accessor_acq_rel`</td><td>`atomic_ref_acq_rel`</td></tr>
<tr><td>`atomic_accessor_seq_cst`</td><td>`atomic_ref_seq_cst`</td></tr>
</tbody>
</table>

`atomic_accessor` was part of the rational provided in P0009 for `mdspan`s *accessor policy* template parameter.

One of the primary use cases for these accessors is the ability to write algorithms with somewhat generic `mdspan` outputs,
which can be called in sequential and parallel contexts.
When called in parallel contexts users would simply pass an `mdspan` with an atomic accessor - the algorithm implementation itself
could be agnostic to the calling context.

A variation on this use case is an implementation of an algorithm taking an execution policy,
which adds the atomic accessor to its output argument if called with a parallel policy,
while using the `default_accessor` when called with a sequential policy.
The following demonstrates this with a function computing a histogram:

```c++
template<class T, class Extents, class LayoutPolicy>
auto add_atomic_accessor_if_needed(
    std::execution::sequenced_policy,
    mdspan<T, Extents, LayoutPolicy> m) {
        return m;
    }

template<class ExecutionPolicy, class T, class Extents, class LayoutPolicy>
auto add_atomic_accessor_if_needed(
    ExecutionPolicy,
    mdspan<T, Extents, LayoutPolicy> m) {
        return mdspan(m.data_handle(), m.mapping(), atomic_accessor<T>());
    }

template<class ExecT>
void compute_histogram(ExecT exec, float bin_size,
               mdspan<int, stdex::dextents<int,1>> output,
               mdspan<float, stdex::dextents<int,1>> data) {

  static_assert(is_execution_policy_v<ExecT>);

  auto accumulator = add_atomic_accessor_if_needed(exec, output);

  for_each(exec, data.data_handle(), data.data_handle()+data.extent(0), [=](float val) {
    int bin = std::abs(val)/bin_size;
    if(bin > output.extent(0)) bin = output.extent(0)-1;
    accumulator[bin]++;
  });
}
```

The above example is available on godbolt: [https://godbolt.org/z/cWa6MG5dj](https://godbolt.org/z/cWa6MG5dj)

# Design decisions

Three options for atomic refs were discussed in SG1 in Kona 2022:  add new types for `memory_order` bound atomic refs, add a new `memory_order` template parameter to the existing `atomic_ref`, or add a constructor to the existing `atomic_ref` that takes a `memory_order` and stores it.  Given that the last two are ABI breaks, the first option was polled and chosen.
It was also decided that the new bound atomic refs would not support overriding the specified `memory_order` at run time.

This proposal has chosen to make a general exposition-only template `atomic-ref-bound` that takes a `memory_order` as a template argument
and alias templates for the three specific bound atomic refs.
Also, the various member functions are constrained by integral types not including `bool`, floating point types and pointer types, as opposed to the different template specializations specified for `atomic_ref`.
Other than not being able to specifiy the `memory_order` at run time, the intention is that the bound atomic ref types have the same functionality and API as `atomic_ref`.

Similarly for the atomic accessors, it was decided in SG1 in Kona 2022 to add four new types.
This proposal has chosen to make
a general exposition-only template `basic-atomic-accessor` which takes the `reference` type as a template parameter, and four alias templates for the specific atomic accessors.

Assuming both papers are approved, SG1 voted that similar changes to `atomic_ref` in P2616R3 (Making std::atomic notification/wait operations usable in more situations) should also be applied to `atomic-ref-bound`.  They are not yet in the wording of either paper, as we do not know what order LWG will apply them to the working draft.

## Exposition Only Or Not

As mentioned above, during SG1 review we introduced explicitly named symbols instead of making `basic-atomic-accessor` and `atomic-ref-bound` public.

The primary reason for not making the generic versions public is that the memory order is generally dictated by algorithmic considerations.

The only generic thing one may want to decide is whether to use atomics at all (e.g. as the function of the execution policy).

Consequently, we could not find a use-case where the generic types are useful. That said we also don't have a strong reason to not make them public - other than that it constrains implementers.

## Memory Order Conversions

A question was raised whether to make the various bounded `atomic_ref` and `atomic_accessor` variants constructible/convertible from each other.
We don't see many good reasons to enable that:

- `atomic_ref` is meant to be almost used `in-place` for individual operations e.g. `atomic_ref(a[i])++` and not much of an interface type
- atomic accessors are something you are supposed to add in a fairly local scope too
  - we don't expect folks to pass `mdspan` with atomic accessors around

However we identified a possible reason not to permit it: 

Writing functions which take `atomic_ref_MEM_ORDER` imply ordering behavior: allowing conversions would make it very easy to violate that expectation.

Consider the following:

```c++
void foo(atomic_ref_relaxed<int> counter);

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

We believe it would be potentially unexpected for `foo` to do any operations other than `relaxed` atomics on `counter`.
Likewise if `foo` were to take `atomic_ref_seq_cst` it would be surprising if it did `relaxed` atomic accesses.

# Open questions

This proposal uses alias templates to exposition-only types for `atomic_ref_relaxed`, `atomic_accessor`, etc.
However, we do not want to prescribe a particular implementation.
For instance, if an implementer wished to derive from an `atomic-ref-bound`-like type (to get a more
user friendly name mangling, which is something not normally covered by the Standard), would
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

The proposed changes are relative to [N4917](https://wg21.link/n4917):

## Bound atomic ref
### Change the header synopsis for atomic in [atomics.syn]:

`// [atomics.ref.pointer], partial specialization for pointers`\
`template<class T> struct atomic_ref<T*>;                                          // freestanding`\
[`// [atomics.ref.bounded], class tempalte atomic-ref-bound`]{.add}\
[`template<class T, class MemoryOrder>` _`atomic-ref-bound`_`; // exposition only`]{.add}\
[`template<class T> using atomic_ref_relaxed = `_`atomic-ref-bound`_`<T, memory_order_relaxed>; // freestanding`]{.add}\
[`template<class T> using atomic_ref_acq_rel = `_`atomic-ref-bound`_`<T, memory_order_acq_rel>; // freestanding`]{.add}\
[`template<class T> using atomic_ref_seq_cst = `_`atomic-ref-bound`_`<T, memory_order_seq_cst>; // freestanding`]{.add}\
` `\
`// [atomics.types.generic], class template atomic`\
`template<class T> struct atomic<T>;                                          // freestanding`



### Add the following just before [atomics.types.generic]:

<b>Class template `atomic-ref-bound` [atomics.refbound]</b>

<b>Exposition only helper [atomics.refbound.helpers]</b>

```c++
template<class T>
constexpr bool @_atomic-ref-non-generic-type()_@;
```

Returns: `true` if the class `atomic_ref<T>` has a member type definition `difference_type`, otherwise `false`.

[Note: This is true for a set of defined integral types, floating point types, and pointers.]

<b>Generic atomic-ref-bound [atomics.refbound.generic]</b>

<b>Overview [atomics.refbound.generic.overview]</b>

```c++
template <class T, memory_order MemoryOrder>
struct atomic-ref-bound {  // exposition only
   private:
    using atomic_ref_unbound = atomic_ref<T>;  // exposition only
    atomic_ref_unbound ref;                    // exposition only

    static constexpr memory_order store_ordering =
        MemoryOrder == memory_order_acq_rel ? memory_order_release
                                            : MemoryOrder;  // exposition only

    static constexpr memory_order load_ordering =
        MemoryOrder == memory_order_acq_rel ? memory_order_acquire
                                            : MemoryOrder;  // exposition only

    template<class T> friend class atomic-ref-bound<T, MemoryOrder>;

   public:
    using value_type = remove_cv_t<T>;
    static constexpr memory_order memory_ordering = MemoryOrder;
    static constexpr size_t required_alignment = atomic_ref_unbound::required_alignment;

    static constexpr bool is_always_lock_free = atomic_ref_unbound::is_always_lock_free;
    bool is_lock_free() const noexcept;

    explicit atomic-ref-bound(T&);
    explicit atomic_ref(T&&) = delete;
    atomic-ref-bound(const atomic-ref-bound&) noexcept;
    template<class U>
      atomic-ref-bound(const atomic-ref-bound<U, memory_ordering>&) noexcept;
    atomic-ref-bound& operator=(const atomic-ref-bound&) = delete;

    void store(value_type desired) const noexcept;
    value_type operator=(value_type desired) const noexcept;
    value_type load() const noexcept;
    operator value_type() const noexcept;

    value_type exchange(value_type desired) const noexcept;
    bool compare_exchange_weak(value_type& expected, value_type desired) const noexcept;
    bool compare_exchange_strong(value_type& expected, value_type desired) const noexcept;

    void wait(value_type old) const noexcept;
    void notify_one() const noexcept;
    void notify_all() const noexcept;

    auto address() const noexcept;
};
```
[1]{.pnum} Class `atomic-ref-bound` is for exposition only.

[2]{.pnum} *Mandates:*

   * [2.1]{.pnum} `is_trivially_copyable_v<T>` is `true`.

**Operations [atomics.refbound.generic.ops]**
```c++
bool is_lock_free() const noexcept;
```
[1]{.pnum} *Effects:* Equivalent to: `return ref.is_lock_free();`

```c++
explicit atomic-ref-bound(T& t);
```

[2]{.pnum} *Preconditions:* The referenced object is aligned to `required_alignment`.

[3]{.pnum} *Postconditions:* `ref` references the object referenced by `t`.

[4]{.pnum} *Throws:* Nothing.

```c++
atomic-ref-bound(const atomic-ref-bound& other) noexcept;
```
[5]{.pnum} *Effects:* Direct-non-list-initializes `ref` with `other.ref`.

```c++
template<class U>
  constexpr atomic-ref-bound(const atomic-ref-bound<U>& other) noexcept;
```

[6]{.pnum} *Constraints:*

   * [6.1]{.pnum} `T` and `U` are similar types ([conv.qual]), and

   * [6.2]{.pnum} `is_convertible_v<U*, T*>` is `true`.

[7]{.pnum} *Effects:* Direct-non-list-initializes `ref` with `other.ref`.

```c++
void store(value_type desired) const noexcept;
```
[8]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[9]{.pnum} *Effects:* Equivalent to: `ref.store(desired, store_ordering);`

```c++
value_type operator=(value_type desired) const noexcept;
```
[10]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[11]{.pnum} *Effects:* Equivalent to: `store(desired); return desired;`

```c++
value_type load() const noexcept;
```
[12]{.pnum} *Effects:* Equivalent to: `return ref.load(load_ordering);`

```c++
operator value_type() const noexcept;
```
[13]{.pnum} *Effects:* Equivalent to: `return load();`

```c++
value_type exchange(value_type desired) const noexcept;
```
[14]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[15]{.pnum} *Effects:* Equivalent to: `return ref.exchange(desired, memory_ordering);`


```c++
bool compare_exchange_weak(value_type& expected, value_type desired) const noexcept;
```
[16]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[17]{.pnum} *Effects:* Equivalent to: `return ref.compare_exchange_weak(expected, desired, memory_ordering, load_ordering);`

```c++
bool compare_exchange_strong(value_type& expected, value_type desired) const noexcept;
```
[18]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[19]{.pnum} *Effects:* Equivalent to: `return ref.compare_exchange_strong(expected, desired, memory_ordering, load_ordering);`

```c++
void wait(value_type old) const noexcept;
```
[20]{.pnum} *Effects:* Equivalent to: `ref.wait(old, load_ordering);`

```c++
void notify_one() const noexcept;
```
[21]{.pnum} *Effects:* Equivalent to: `ref.notify_one();`

```c++
void notify_all() const noexcept;
```
[22]{.pnum} *Effects:* Equivalent to: `ref.notify_all();`

```c++
auto address() const noexcept;
```
[23]{.pnum} *Effects:* Equivalent to: `return ref.address();`

<b>Non-generic atomic-ref-bound [atomics.refbound.nongeneric]</b>

<b>Overview [atomics.refbound.nongeneric.overview]</b>

```c++
template <arithmetic-or-pointer T, memory_order MemoryOrder>
requires(@_atomic-ref-non-generic-type_@<T>())
struct atomic-ref-bound<T, MemoryOrder> {  // exposition only
   private:
    using atomic_ref_unbound = atomic_ref<T>;  // exposition only
    atomic_ref_unbound ref;                    // exposition only

    static constexpr memory_order store_ordering =
        MemoryOrder == memory_order_acq_rel ? memory_order_release
                                            : MemoryOrder;  // exposition only

    static constexpr memory_order load_ordering =
        MemoryOrder == memory_order_acq_rel ? memory_order_acquire
                                            : MemoryOrder;  // exposition only

    template<class T> friend class atomic-ref-bound<T, MemoryOrder>;

   public:
    using value_type = remove_cv_t<T>;

   private:
    static constexpr bool is_integral_value = // exposition only
        is_integral_v<value_type> && !is_same_v<value_type, bool>;
    static constexpr bool is_floating_point_value = // exposition only
        is_floating_point_v<value_type>;
    static constexpr bool is_pointer_value = // exposition only
        is_pointer_v<value_type>;

   public:
    using difference_type = atomic_ref_bound::difference_type;
    static constexpr memory_order memory_ordering = MemoryOrder;
    static constexpr size_t required_alignment = atomic_ref_unbound::required_alignment;

    static constexpr bool is_always_lock_free = atomic_ref_unbound::is_always_lock_free;

    // [atomics.refbound.nongeneric.ops] Operations common with generic version
    bool is_lock_free() const noexcept;

    explicit atomic-ref-bound(T& t);
    explicit atomic-ref-bound(T&&) = delete;
    template<class U>
      atomic-ref-bound(const atomic-ref-bound<U, memory_ordering>&) noexcept;
    atomic-ref-bound(const atomic-ref-bound&) noexcept;
    atomic-ref-bound& operator=(const atomic-ref-bound&) = delete;

    void store(value_type desired) const noexcept;
    value_type operator=(value_type desired) const noexcept;
    value_type load() const noexcept;
    operator value_type() const noexcept;

    value_type exchange(value_type desired) const noexcept;
    bool compare_exchange_weak(value_type& expected, value_type desired) const noexcept;
    bool compare_exchange_strong(value_type& expected, value_type desired) const noexcept;

    void wait(value_type old) const noexcept;
    void notify_one() const noexcept;
    void notify_all() const noexcept;

    auto address() const noexcept;

    // [atomics.refbound.nongeneric.common] Common operations
    value_type fetch_add(difference_type operand) const noexcept
    value_type fetch_sub(difference_type operand) const noexcept;
    value_type fetch_max(value_type operand) const noexcept;
    value_type fetch_min(value_type operand) const noexcept;

    void store_add(difference_type operand) const noexcept;
    void store_sub(difference_type operand) const noexcept;
    void store_max(value_type operand) const noexcept;
    void store_min(value_type operand) const noexcept;

    value_type operator+=(difference_type operand) const noexcept;
    value_type operator-=(difference_type operand) const noexcept;

    // [atomics.refbound.nongeneric.integral] Integral only operations
    value_type fetch_and(value_type operand) const noexcept;
    value_type fetch_or (value_type operand) const noexcept;
    value_type fetch_xor(value_type operand) const noexcept;

    void store_and(value_type operand) const noexcept;
    void store_or (value_type operand) const noexcept;
    void store_xor(value_type operand) const noexcept;

    value_type operator&=(value_type operand) const noexcept;
    value_type operator|=(value_type operand) const noexcept;
    value_type operator^=(value_type operand) const noexcept;

    // [atomics.refbound.nongeneric.floatingpoint] Floating point only operations
    value_type fetch_fmaximum(value_type operand) const noexcept;
    value_type fetch_fminimum(value_type operand) const noexcept;
    value_type fetch_fmaximum_num(value_type operand) const noexcept;
    value_type fetch_fminimum_num(value_type operand) const noexcept;

    void store_fmaximum(value_type operand) const noexcept;
    void store_fminimum(value_type operand) const noexcept;
    void store_fmaximum_num(value_type operand) const noexcept;
    void store_fminimum_num(value_type operand) const noexcept;

    // [atomics.refbound.nongeneric.integralpointer] Integral and pointer type operations
    value_type operator++(int) const noexcept;
    value_type operator++() const noexcept;
    value_type operator--(int) const noexcept;
    value_type operator--() const noexcept;

};
```
[1]{.pnum} Class `atomic-ref-bound` is for exposition only.

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
[1]{.pnum} *Effects:* Equivalent to: `return ref.is_lock_free();`

```c++
explicit atomic-ref-bound(T& t);
```

[2]{.pnum} *Preconditions:* The referenced object is aligned to `required_alignment`.

[3]{.pnum} *Effects:* Direct-non-list-initializes `ref` with `t`.

[4]{.pnum} *Throws:* Nothing.

```c++
atomic-ref-bound(const atomic-ref-bound& other) noexcept;
```
[7]{.pnum} *Effects:* Direct-non-list-initializes `ref` with `other.ref`.

```c++
template<class U>
  constexpr atomic-ref-bound(const atomic-ref-bound<U>& other) noexcept;
```

[6]{.pnum} *Constraints:*

   * [6.1]{.pnum} `T` and `U` are similar types ([conv.qual]), and

   * [6.2]{.pnum} `is_convertible_v<U*, T*>` is `true`.

[7]{.pnum} *Effects:* Direct-non-list-initializes `ref` with `other.ref`.

```c++
void store(value_type desired) const noexcept;
```
[8]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[9]{.pnum} *Effects:* Equivalent to: `ref.store(desired, store_ordering);`

```c++
value_type operator=(value_type desired) const noexcept;
```
[10]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[11]{.pnum} *Effects:* Equivalent to: `store(desired); return desired;`

```c++
value_type load() const noexcept;
```
[12]{.pnum} *Effects:* Equivalent to: `return ref.load(load_ordering);`

```c++
operator value_type() const noexcept;
```
[13]{.pnum} *Effects:* Equivalent to: `return load();`

```c++
value_type exchange(value_type desired) const noexcept;
```
[14]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[15]{.pnum} *Effects:* Equivalent to: `return ref.exchange(desired, memory_ordering);`


```c++
bool compare_exchange_weak(value_type& expected, value_type desired) const noexcept;
```
[16]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[17]{.pnum} *Effects:* Equivalent to: `return ref.compare_exchange_weak(expected, desired, memory_ordering, load_ordering);`

```c++
bool compare_exchange_strong(value_type& expected, value_type desired) const noexcept;
```
[18]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[19]{.pnum} *Effects:* Equivalent to: `return ref.compare_exchange_strong(expected, desired, memory_ordering, load_ordering);`

```c++
void wait(T old) const noexcept;
```
[20]{.pnum} *Effects:* Equivalent to: `ref.wait(old, load_ordering);`

```c++
void notify_one() const noexcept;
```
[21]{.pnum} *Effects:* Equivalent to: `ref.notify_one();`

```c++
void notify_all() const noexcept;
```
[22]{.pnum} *Effects:* Equivalent to: `ref.notify_all();`

```c++
auto address() const noexcept;
```
[23]{.pnum} *Effects:* Equivalent to: `return ref.address();`

**Common Operations [atomics.refbound.nongeneric.common]**
```c++
value_type fetch_add(difference_type operand) const noexcept;
```
[1]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[2]{.pnum} *Effects:* Equivalent to: `return ref.fetch_add(operand, memory_ordering);`

```c++
value_type fetch_sub(difference_type operand) const noexcept;
```

[3]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[4]{.pnum} *Effects:* Equivalent to: `return ref.fetch_sub(operand, memory_ordering);`

```c++
value_type fetch_max(value_type operand) const noexcept;
```
[5]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[6]{.pnum} *Effects:* Equivalent to: `return ref.fetch_max(operand, memory_ordering);`

```c++
value_type fetch_min(value_type operand) const noexcept;
```

[7]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[8]{.pnum} *Effects:* Equivalent to: `return ref.fetch_min(operand, memory_ordering);`

```c++
void store_add(difference_type operand) const noexcept;
```
[9]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[10]{.pnum} *Effects:* Equivalent to: `ref.store_add(operand, memory_ordering);`

```c++
void store_sub(difference_type operand) const noexcept;
```
[11]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[12]{.pnum} *Effects:* Equivalent to: `ref.store_sub(operand, memory_ordering);`

```c++
void store_max(value_type operand) const noexcept;
```
[13]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[14]{.pnum} *Effects:* Equivalent to: `ref.store_max(operand, memory_ordering);`

```c++
void store_min(value_type operand) const noexcept;
```
[15]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[16]{.pnum} *Effects:* Equivalent to: `ref.store_min(operand, memory_ordering);`

```c++
value_type operator+=(difference_type operand) const noexcept;
```
[17]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[18]{.pnum} *Effects:* Equivalent to: `return fetch_add(operand) + operand;`

```c++
value_type operator-=(difference_type operand) const noexcept;
```
[19]{.pnum} *Constraints:* `is_const_v<T>` is `false`.

[20]{.pnum} *Effects:* Equivalent to: `return fetch_sub(operand) - operand;`

**Integral Operations [atomics.refbound.nongeneric.integral]**
```c++
value_type fetch_and(value_type operand) const noexcept;
```
[1]{.pnum} *Constraints:* `is_integral_value && !is_const_v<T>` is `true`.

[2]{.pnum} *Effects:* Equivalent to: `return ref.fetch_and(operand, memory_ordering);`

```c++
value_type fetch_or(value_type operand) const noexcept;
```
[3]{.pnum} *Constraints:* `is_integral_value && !is_const_v<T>` is `true`.

[4]{.pnum} *Effects:* Equivalent to: `return ref.fetch_or(operand, memory_ordering);`

```c++
value_type fetch_xor(value_type operand) const noexcept;
```
[5]{.pnum} *Constraints:* `is_integral_value && !is_const_v<T>` is `true`.

[6]{.pnum} *Effects:* Equivalent to: `return ref.fetch_xor(operand, memory_ordering);`

```c++
void store_and(value_type operand) const noexcept;
```
[7]{.pnum} *Constraints:* `is_integral_value && !is_const_v<T>` is `true`.

[8]{.pnum} *Effects:* Equivalent to: `ref.store_and(operand, memory_ordering);`

```c++
void store_or(value_type operand) const noexcept;
```
[9]{.pnum} *Constraints:* `is_integral_value && !is_const_v<T>` is `true`.

[10]{.pnum} *Effects:* Equivalent to: `ref.store_or(operand, memory_ordering);`

```c++
void store_xor(value_type operand) const noexcept;
```
[11]{.pnum} *Constraints:* `is_integral_value && !is_const_v<T>` is `true`.

[12]{.pnum} *Effects:* Equivalent to: `ref.store_xor(operand, memory_ordering);`

```c++
value_type operator&=(value_type operand) const noexcept;
```
[13]{.pnum} *Constraints:* `is_integral_value && !is_const_v<T>` is `true`.

[14]{.pnum} *Effects:* Equivalent to: `return fetch_and(operand) & operand;`

```c++
value_type operator|=(value_type operand) const noexcept;
```
[15]{.pnum} *Constraints:* `is_integral_value && !is_const_v<T>` is `true`.

[16]{.pnum} *Effects:* Equivalent to: `return fetch_or(operand) | operand;`

```c++
value_type operator^=(value_type operand) const noexcept;
```
[17]{.pnum} *Constraints:* `is_integral_value && !is_const_v<T>` is `true`.

[18]{.pnum} *Effects:* Equivalent to: `return fetch_xor(operand) ^ operand;`

**Floating Point Operations [atomics.refbound.nongeneric.floatingpoint]**
```c++
value_type fetch_fmaximum(value_type operand) const noexcept;
```
[1]{.pnum} *Constraints:* `is_floating_point_value && !is_const_v<T>` is `true`.

[2]{.pnum} *Effects:* Equivalent to: `return ref.fetch_fmaximum(operand, memory_ordering);`

```c++
value_type fetch_fminimum(value_type operand) const noexcept;
```
[3]{.pnum} *Constraints:* `is_floating_point_value && !is_const_v<T>` is `true`.

[4]{.pnum} *Effects:* Equivalent to: `return ref.fetch_fminimum(operand, memory_ordering);`

```c++
value_type fetch_fmaximum_num(value_type operand) const noexcept;
```
[5]{.pnum} *Constraints:* `is_floating_point_value && !is_const_v<T>` is `true`.

[6]{.pnum} *Effects:* Equivalent to: `return ref.fetch_fmaximum_num(operand, memory_ordering);`

```c++
value_type fetch_fminimum_num(value_type operand) const noexcept;
```
[7]{.pnum} *Constraints:* `is_floating_point_value && !is_const_v<T>` is `true`.

[8]{.pnum} *Effects:* Equivalent to: `return ref.fetch_fminimum_num(operand, memory_ordering);`

```c++
void store_fmaximum(value_type operand) const noexcept;
```
[9]{.pnum} *Constraints:* `is_floating_point_value && !is_const_v<T>` is `true`.

[10]{.pnum} *Effects:* Equivalent to: `ref.fetch_fmaximum(operand, memory_ordering);`

```c++
void store_fminimum(value_type operand) const noexcept;
```
[11]{.pnum} *Constraints:* `is_floating_point_value && !is_const_v<T>` is `true`.

[12]{.pnum} *Effects:* Equivalent to: `ref.fetch_fminimum(operand, memory_ordering);`

```c++
void store_fmaximum_num(value_type operand) const noexcept;
```
[13]{.pnum} *Constraints:* `is_floating_point_value && !is_const_v<T>` is `true`.

[14]{.pnum} *Effects:* Equivalent to: `ref.fetch_fmaximum_num(operand, memory_ordering);`

```c++
void store_fminimum_num(value_type operand) const noexcept;
```
[15]{.pnum} *Constraints:* `is_floating_point_value && !is_const_v<T>` is `true`.

[16]{.pnum} *Effects:* Equivalent to: `ref.fetch_fminimum_num(operand, memory_ordering);`

**Integral and Pointer Operations [atomics.refbound.nongeneric.integralpointer]**
```c++
value_type operator++(int) const noexcept;
```
[1]{.pnum} *Constraints:*

   * `is_integral_value || is_pointer_value` is `true`, and

   * `is_const_v<T>` is `false`.

[2]{.pnum} *Effects:* Equivalent to: `return fetch_add(1);`

```c++
value_type operator++() const noexcept;
```
[3]{.pnum} *Constraints:*

   * `is_integral_value || is_pointer_value` is `true`, and

   * `is_const_v<T>` is `false`.

[4]{.pnum} *Effects:* Equivalent to: `return fetch_add(1) + 1;`

```c++
value_type operator--(int) const noexcept;
```
[5]{.pnum} *Constraints:*

   * `is_integral_value || is_pointer_value` is `true`, and

   * `is_const_v<T>` is `false`.

[6]{.pnum} *Effects:* Equivalent to: `return fetch_sub(1);`

```c++
value_type operator--() const noexcept;
```
[7]{.pnum} *Constraints:*

   * `is_integral_value || is_pointer_value` is `true`, and

   * `is_const_v<T>` is `false`.

[8]{.pnum} *Effects:* Equivalent to: `return fetch_sub(1) - 1;`



<b>Memory Order Specific Atomic Refs [atomics.refbound.order]</b>

```c++
// all freestanding
namespace std {
template<class T>
using atomic_ref_relaxed = atomic-ref-bound<T, memory_order_relaxed>;

template<class T>
using atomic_ref_acq_rel = atomic-ref-bound<T, memory_order_acq_rel>;

template<class T>
using atomic_ref_seq_cst = atomic-ref-bound<T, memory_order_seq_cst>;
}
```

### In [version.syn]:

Update the feature test macro `__cpp_lib_atomic_ref`.

## Atomic Accessors

### Add to the mdspan header synopsis in [mdspan.syn]

`// [mdspan.accessor.default], class template default_accessor`\
`template<class ElementType>`\
`  class default_accessor;`\
` `\
[`// [atomics.accessor.atomic], class template basic-atomic-accessor`]{.add}\
[`template<class T, class ReferenceType>` class _`basic-atomic-accessor`_`; // exposition only`]{.add}\
[`template<class T> using atomic_accessor = `_`basic-atomic-accessor`_`<T, atomic_ref<T>>;`]{.add}\
[`template<class T> using atomic_accessor_relaxed = `_`basic-atomic-accessor`_`<T, atomic_ref_relaxed<T>>;`]{.add}\
[`template<class T> using atomic_accessor_acq_rel = `_`basic-atomic-accessor`_`<T, atomic_ref_acq_rel<T>>;`]{.add}\
[`template<class T> using atomic_accessor_seq_cst = `_`basic-atomic-accessor`_`<T, atomic_ref_seq_cst<T>>;`]{.add}\
` `\
`// [mdspan.mdspan], class template mdspan`\
`template<class ElementType, class Extents, class LayoutPolicy = layout_right,`\
`         class AccessorPolicy = default_accessor<ElementType>>`\
`  class mdspan;`


### Put the following before [mdspan.mdspan]:

<b>Class template `basic-atomic-accessor` [mdspan.accessor.atomic.basic]</b>

<b>General [mdspan.accessor.atomic.basic.overview]</b>

```c++
template <class ElementType, class ReferenceType>
struct @_basic-atomic-accessor_@ {  // exposition only
    using offset_policy = basic-atomic-accessor;
    using element_type = ElementType;
    using reference = ReferenceType;
    using data_handle_type = ElementType*;

    constexpr basic-atomic-accessor() noexcept = default;

    template <class OtherElementType>
    constexpr basic-atomic-accessor(default_accessor<OtherElementType>) noexcept;

    template <class OtherElementType>
    constexpr basic-atomic-accessor(basic-atomic-accessor<OtherElementType, ReferenceType>) noexcept;

    constexpr reference access(data_handle_type p, size_t i) const noexcept;
    constexpr data_handle_type offset(data_handle_type p, size_t i) const noexcept;
};
```

[1]{.pnum} Class `basic-atomic-accessor` is for exposition only.

[2]{.pnum} `basic-atomic-accessor` meets the accessor policy requirements.

[3]{.pnum} `ElementType` is required to be a complete object type that is neither an abstract class type nor an array type.

[4]{.pnum} Each specialization of `basic-atomic-accessor` is a trivially copyable type that models `semiregular`.

[5]{.pnum} `[0,n)` is an accessible range for an object `p` of type `data_handle_type` and an object of type `basic-atomic-accessor` if and only if `[p,p+n)` is a valid range.

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
