sudo mariadb
sudo mysql -u root -p -e "CREATE DATABASE IF NOT EXISTS labo2GD;"
sudo mysql -u root -e "CREATE USER IF NOT EXISTS 'labo2GillesDurnez'@'localhost' IDENTIFIED BY 'labo2GillesDurnezPW';"
sudo mysql -u root -e "USE labo2GD;"
sudo mysql -u root -e "GRANT ALL PRIVILEGES ON labo2GD.* TO 'labo2GillesDurnez'@'localhost';"
sudo mysql -u root -e "CREATE TABLE IF NOT EXISTS comments( time VARCHAR(30), username VARCHAR(30), comment VARCHAR(150));"
