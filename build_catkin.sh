# Build the repository
# 1. Will git clean everything
# 2. Will run make clean everywhere
# 3. Will build the whole repository

REPO_PATH=~/catkin_ws/src/kraken_catkin
CATKIN_WS_PATH=~/catkin_ws

cd &&
cd $REPO_PATH &&
git clean -fdX &&
cd &&
cd $CATKIN_WS_PATH &&
catkin_make clean &&
catkin_make
