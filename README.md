# NodeMCU-to-PHP

This is a simple Arduino sketch that runs on a NodeMCU with a DHT11 temperature/humidity sensor attached. It connects to wifi, and every 30 seconds it gets measurements from the sensor and submits them to the webserver. A PHP file receives these values and stores them on the server in a MariaDB. The PHP also presents the data to a user if opened in a browser. There is some basic security in the form of a shared key and MD5 hash.
