ifconfig | grep inet\ addr: > /home/dyutimoy/catkin_ws/src/kraken_3.0/Scripts/launch/yaml/web_cam_1.yaml
awk 'NR==1{print $2}' /home/dyutimoy/catkin_ws/src/kraken_3.0/Scripts/launch/yaml/web_cam_1.yaml > /home/dyutimoy/catkin_ws/src/kraken_3.0/Scripts/launch/yaml/web_cam_2.yaml
sed 's/addr:/address: "/g' /home/dyutimoy/catkin_ws/src/kraken_3.0/Scripts/launch/yaml/web_cam_2.yaml > /home/dyutimoy/catkin_ws/src/kraken_3.0/Scripts/launch/yaml/web_cam_3.yaml
sed 's/$/."/' /home/dyutimoy/catkin_ws/src/kraken_3.0/Scripts/launch/yaml/web_cam_3.yaml > /home/dyutimoy/catkin_ws/src/kraken_3.0/Scripts/launch/yaml/web_cam_4.yaml
