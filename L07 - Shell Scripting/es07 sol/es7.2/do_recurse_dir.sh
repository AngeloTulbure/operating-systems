
# do_recurse_dir.sh dir start N outfile


cd "$1"

X=0
for i in * ; do
  if [[ -d "$i" ]] ; then  
  	"$0" "$i" "$2" "$3" "$4"
  elif [[ -f "$i" && "$i" = $2* ]] ; then
  	X=`expr $X + 1`
  fi
done

if [[ $X -gt $3 ]] ; then
    echo `pwd` $X >> "$4"
fi