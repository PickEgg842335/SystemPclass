cmd_/home/pi/DHT11/dht11_kernel/modules.order := {   echo /home/pi/DHT11/dht11_kernel/dht11.ko; :; } | awk '!x[$$0]++' - > /home/pi/DHT11/dht11_kernel/modules.order
