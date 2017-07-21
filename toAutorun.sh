#!/bin/bash

sudo chmod u+x $1
sudo cp $1 /bin
sudo touch /lib/systemd/system/$1.service

sudo sh -c "echo '[Unit]'>>/lib/systemd/system/$1.service"
sudo sh -c "echo 'Description'=NOTHING>>/lib/systemd/system/$1.service"
sudo sh -c "echo 'After=multi-user.target'>>/lib/systemd/system/$1.service"
sudo sh -c "echo ''>>/lib/systemd/system/$1.service"
sudo sh -c "echo '[Service]'>>/lib/systemd/system/$1.service"
sudo sh -c "echo 'Type=idle'>>/lib/systemd/system/$1.service"
sudo sh -c "echo 'ExecStart'=/bin/$1>>/lib/systemd/system/$1.service"
sudo sh -c "echo ''>>/lib/systemd/system/$1.service"
sudo sh -c "echo '[Install]'>>/lib/systemd/system/$1.service"
sudo sh -c "echo 'WantedBy=multi-user.target'>>/lib/systemd/system/$1.service"

sudo chmod 644 /lib/systemd/system/$1.service

sudo systemctl daemon-reload
sudo systemctl enable $1.service
