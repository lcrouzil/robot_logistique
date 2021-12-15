# publish with password
mosquitto_pub -h localhost -p 1883 -t home/bedroom/temperature -m 22

# subscribe with password
mosquitto_sub -h localhost -p 1883 -t home/bedroom/temperature
mosquitto_sub -h localhost -p 1883 -t #
