#!/usr/bin/env python
from __future__ import print_function
from os.path import dirname, abspath, join as path_join
import re

mydir = dirname(abspath(__file__))
github_bits = "https://github.com/ORNL/cpp-proposals-pub/blob/master/P0009/reference-implementation/include/experimental/bits"

with open(path_join(mydir, "mdspan_single_file.hpp"), "w+") as o:
    with open(path_join(mydir, "include", "experimental", "mdspan")) as mdspan:
        for line in mdspan.readlines():
            m = re.match(r'#include\s*"bits/(.+)"', line)
            if m:
                with open(path_join(mydir, "include", "experimental", "bits", m.group(1))) as included:
                    for linenum, ln in enumerate(included.readlines()):
                        print("#line {0} {1}/{2}#{0}".format(linenum+1, github_bits, m.group(1)), file=o)
                        print(ln, file=o, end='')
            else:
                print(line, file=o, end='')

