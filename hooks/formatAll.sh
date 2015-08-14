extensions=('*.cpp' '*.hpp' '*.h')

for j in "${extensions[@]}"; do
	find ./ -name $j >> .tmp.txt
	echo $j
done;

for i in `cat .tmp.txt`; do
	astyle -A1 -yej -f -S $i
done;

rm .tmp.txt
