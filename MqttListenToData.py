import paho.mqtt.client as mqtt
from DataToDB import sensor_data_handler

# MQTT settings
mqtt_broker = "test.mosquitto.org"
mqtt_port = 1883
keep_alive_interval = 60
mqtt_topic = "BAAA/room1/#"

# Subscribe to all sensors from the base topic
def on_connect(client, userdata, flags, rc):
    client.subscribe(mqtt_topic, 0)

# Save data into DB table
def on_message(client, userdata, msg):
    print("MQTT topic: " + msg.topic)
    print("Data: " + str(msg.payload).strip("'b"))
    sensor_data_handler(msg.payload)

def on_subscribe(client, userdata, flags, rc):
    pass




client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(mqtt_broker, int(mqtt_port), int(keep_alive_interval))
client.loop_forever()










