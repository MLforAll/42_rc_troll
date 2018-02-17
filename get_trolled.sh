#!/bin/sh

ssh kelian@51.255.193.81 "buff=\$(cat /var/www/mlforall/42/troll/pool/rc/trolled.txt); if [ -z \"\$buff\" ]; then echo \"Ya didn't catch anyone!\"; else echo \"\$buff\"; fi"
