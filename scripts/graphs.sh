#!/bin/bash
build/tool/aishell/aishell -i $1 | python scripts/graphs.py
