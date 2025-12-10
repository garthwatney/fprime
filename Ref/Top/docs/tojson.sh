#!/bin/bash
#
# This script invokes the fpp-to-json on the fpp file and produces the following json files:
# fpp-analysis.json  fpp-ast.json  fpp-loc-map.json
#
fpp-to-json $(tr ';' ' ' < ../../build-fprime-automatic-native/Ref/Top/fpp-import-list) ../instances.fpp ../topology.fpp
