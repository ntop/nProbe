#!/bin/bash

if [ -z "$1" ]; then
    ELASTICSEARCH=http://localhost:9200
else
    ELASTICSEARCH=$1
fi

if [ -z "$2" ]; then
    CURL=curl
else
    CURL="curl --user $2"
fi

if [ -z "$3" ]; then
    KIBANA_INDEX=".kibana"
else
    KIBANA_INDEX=".kibana_$3"
fi

echo $CURL
DIR=dashboards

for file in $DIR/search/*.json
do
    name=`basename $file .json`
    echo "Loading search $name:"
    $CURL -XPUT $ELASTICSEARCH/$KIBANA_INDEX/search/$name \
        -d @$file || exit 1
    echo
done

for file in $DIR/visualization/*.json
do
    name=`basename $file .json`
    echo "Loading visualization $name:"
    $CURL -XPUT $ELASTICSEARCH/$KIBANA_INDEX/visualization/$name \
        -d @$file || exit 1
    echo
done

for file in $DIR/dashboard/*.json
do
    name=`basename $file .json`
    echo "Loading dashboard $name:"
    $CURL -XPUT $ELASTICSEARCH/$KIBANA_INDEX/dashboard/$name \
        -d @$file || exit 1
    echo
done

for file in $DIR/index-pattern/*.json
do
    name=`basename $file .json`
    printf -v escape "%q" $name
    echo "Loading index pattern $escape:"

    $CURL -XPUT $ELASTICSEARCH/$KIBANA_INDEX/index-pattern/$escape \
        -d @$file || exit 1
    echo
done


