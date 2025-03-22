#! /usr/bin/env sh

if test $# -ne 1 -o ! -d $1; then
	echo "Usage: $0 d" >&2
	exit
fi

## Could probaby make it better
# Will fail if your file uses a number and then a space, has to be fixed in the regex

#wc -c * | sort -n | tail -2 | head -1 | rev | cut -d' ' -f1 | rev
wc -c $1/* | sort -n | tail -2 | head -1 | rev | grep -Eo '[^ [0-9]]' | sed ':a;N;$!ba;s/\n//g' | rev
