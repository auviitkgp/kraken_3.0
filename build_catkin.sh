# Build the repository
# 1. Will git clean everything
# 2. Will run make clean everywhere
# 3. Will build the whole repository

cd "$(dirname "$0")" &&
git clean -fdX &&
cd ../../ &&
catkin_make clean &&
catkin_make