for i in `find ./ -name '*.cpp'`; do
	astyle -A1 -yej -f -S $i
done;
