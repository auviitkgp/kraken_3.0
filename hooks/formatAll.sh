for i in `find ./ -name '*.cpp'`; do
	astyle -yej -f -S $i
done;
