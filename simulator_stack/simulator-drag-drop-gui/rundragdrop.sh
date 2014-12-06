echo "Running main.py script"
python main.py
echo "Running the fileWrite.py script"
python fileWrite.py
mv output.xml input.xml
cp input.xml scene.urdf
echo "Complete."
echo "Overwrite scene.urdf file in kraken_visualization/kraken/scenes/scene.urdf with the file here."
