#!/bin/bash
# HOST="localhost"   
# PORT=10000         

read -p 'Entrez le nombre de clients : ' nb
read -p 'Entrez le HOST : ' HOST
read -p 'Entrez le PORT : ' PORT


echo Start testing

	for i in $(seq 1 $nb)
	do
		./wclient  $HOST $PORT /spin.cgi?$i 
		./wclient $HOST $PORT /test_$i.html 
	done


