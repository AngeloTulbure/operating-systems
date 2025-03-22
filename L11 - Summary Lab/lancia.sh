#! /usr/bin/env sh

FILEOUT=$1
TIMEOUT=$2
FASE=$3

#child
(echo 'bbb\nabh\nhbf\ndsadsa\nsadm' && sleep 1000) > $FILEOUT &
PID=$!

#dad
sleep $TIMEOUT
kill -9 $PID
grep $FASE $FILEOUT
