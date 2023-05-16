---
title: "Extending Linear Algebra Support to Batched Operations"
document: 
date: today
audience: LWG
author:
  - name: Mark Hoemmen 
    email: <mhoemmen@nvidia.com>
  - name: Kim Liegeois
    email: <knliege@sandia.gov>
  - name: Christian Trott 
    email: <crtrott@sandia.gov>
toc: true
---


# Revision History

## Initial Version 2023-01 Mailing

- Initial version for SG review

# Why we should extend support to batched linear algebra

blah blah

# Existing Practice

## Vendor Libraries

mkl blah blah, cublas blah blah, rocblas blah blah

## Open Source Practice

KokkosKernels blah blah, BLAS proposal blah blah, Magma??

# Extending std::linalg

## Principle

Higher rank arguments

## Challenge 1: Reductions

output argument

## Challenge 2: Multiple Scaling factors

extra mdspan args?

