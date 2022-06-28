#!/bin/bash

FILENAME=$1
DBNAME=ntop

# Unflux 1.x
curl -XPOST "http://localhost:8086/write?db=$DBNAME" --data-binary @$FILENAME

/bin/rm $FILENAME
