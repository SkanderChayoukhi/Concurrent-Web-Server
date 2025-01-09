#!/bin/bash



read -p 'Entrez le nombre de clients : ' nb
read -p 'Entrez le HOST : ' HOST
read -p 'Entrez le PORT : ' PORT

echo Start testing

	for i in $(seq 1 $nb) 
	do
		./wclient_multithreaded $HOST $PORT /spin.cgi?$i /test_1.html /test_2.html /test_3.html /test_4.html /test_5.html /index.html
	done