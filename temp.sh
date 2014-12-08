git remote add main-repo https://github.com/iit-kgp-auv-team/kraken_3.0.git
git remote add fork-manvi https://github.com/ManviG/kraken_3.0.git
git checkout master
git checkout -b manvi-temp
git pull main-repo master
git push fork-manvi manvi-temp
