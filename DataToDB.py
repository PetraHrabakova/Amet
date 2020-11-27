import mariadb
import sys

# Instantiate Connection
try:
    conn = mariadb.connect(
    user="root",
    password="rainbow",
    host="localhost",
    port=3306,
    database = "mobetraproject",
    autocommit = True)
except mariadb.Error as e:
    print(f"Error connecting to MariaDB Platform: {e}")
    sys.exit(1)


# Functions to push the sensor data to database

# Functions to save temperature to DB table
cur = conn.cursor()
def temperature(cur, t):
    cur.execute("INSERT INTO mobetraproject.measurement(temperature) "
                "VALUES (?)", (t))

def humidity(cur, h):
    cur.execute("INSERT INTO mobetraproject.measurement(humidity) "
                "VALUES (?)", (h))


def brightness(cur, b):
    cur.execute("INSERT INTO mobetraproject.measurement(brightness) "
                "VALUES (?)", (b))


def GPS(cur, l):
    cur.execute("INSERT INTO mobetraproject.measurement(location) "
                "VALUES (?)", (l))


# Master function to select DB function base on MQTT topic
def sensor_data_handler(topic):
    if topic == "BAAA/room1/Temperature":
        temperature()
    elif topic == "BAAA/room1/Humidity":
        humidity()
    elif topic == "BAAA/room1/Brightness":
        brightness()
    elif topic == "BAAA/room1/Location":
        GPS()







