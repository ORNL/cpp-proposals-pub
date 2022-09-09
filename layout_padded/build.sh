#!/bin/bash

curl https://api.csswg.org/bikeshed/ -F file=@layout_padded.bs -F force=1 > layout_padded.html
