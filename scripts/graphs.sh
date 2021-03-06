#!/bin/bash
inputfile=$1
shift
quantity=$1
shift
cat $inputfile | python scripts/repeat.py $quantity |  build/tool/aishell/aishell --no-prompt | python python/graphs.py --exclude "$@"
