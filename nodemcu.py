from flask import Flask, render_template, request
import paho.mqtt.client as mqtt

app = Flask(__name__)

mqtt_server = "test.mosquitto.org"
mqtt_client = mqtt.Client("Flask")

@app.route("/")
def index():
    return render_template('index.html')

@app.route("/led-control1", methods=["POST"])
def led_control1():
    led_status = request.form.get("led-status")
    print(led_status)
    if led_status == "on":
        mqtt_client.connect(mqtt_server)
        mqtt_client.publish("switch1", "0")
    else:
        mqtt_client.connect(mqtt_server)
        mqtt_client.publish("switch1", "1")
    mqtt_client.disconnect()
    return render_template('index.html')


@app.route("/led-control2", methods=["POST"])
def led_control2():
    led_status = request.form.get("led-status")
    print(led_status)
    if led_status == "on":
        mqtt_client.connect(mqtt_server)
        mqtt_client.publish("switch2", "1")
    else:
        mqtt_client.connect(mqtt_server)
        mqtt_client.publish("switch2", "0")
    mqtt_client.disconnect()
    return render_template('index.html')
if __name__ == "__main__":
    app.run(debug=True)
