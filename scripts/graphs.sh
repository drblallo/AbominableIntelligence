#!/bin/bash
if [ -n "$2" ]; then
  cat $1 | python scripts/repeat.py $2 |  build/tool/aishell/aishell --no-prompt | python scripts/graphs.py
else
  cat $1 | python scripts/repeat.py 1 |  build/tool/aishell/aishell --no-prompt | python scripts/graphs.py
fi
