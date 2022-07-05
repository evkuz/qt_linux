#!/bin/bash

# For git savings copy from out into of repository qt libs

rsync -a ${EXTLIBS}/manipulator/ ./qtlibs

