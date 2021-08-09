#!/bin/bash

curl https://api.csswg.org/bikeshed/ -F file=@P1673.bs -F force=1 > P1673.html
