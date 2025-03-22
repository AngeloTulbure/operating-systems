#! /usr/bin/env sh

read dir
test -d "/${dir}" && ls -R ${dir} > "${HOME}/dir_structure_${USER}.out"
