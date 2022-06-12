import speech_recognition as sr     # import the library
from struct import pack
import paho.mqtt.client
import os

client_id = 'PC_0'
client = mqtt.Client(client_id = client_id,
                     protocol=mqtt.MQTTv31)

client.connect(os.environ["MQTT_BROKER"], 1883)
device_id = client_id+"/stt/"
print(sr.Microphone.list_microphone_names()) 

r = sr.Recognizer()                 # initialize recognizer
with sr.Microphone(device_index=2) as source:     # mention source it will be either Microphone or audio files.
    print("Diga o comando: ")

    while True:
        r.adjust_for_ambient_noise(source)
        
        audio = r.listen(source, phrase_time_limit=5)        # listen to the source
        #with open("microphone-results.wav", "wb") as f:
        #    f.write(audio.get_wav_data())
        #audio.play()
        try:
            text = r.recognize_google(audio, language="pt-BR")    # use recognizer to convert our audio into text part.
            print("{}".format(text))

            if text == "sair" or text == "encerrar":
                break
            elif text == "liga o abajur":
                print("ligando")
                client.publish(device_id+"abajur", "1", qos=0)
            elif text == "desliga o abajur":
                print("desligando")
                client.publish(device_id+"abajur", "0", qos=0)
            elif text == "liga a luz da escada":
                print("ligando")
                client.publish(device_id+"escada", "1", qos=0)
            elif text == "desliga a luz da escada":
                print("desligando")
                client.publish(device_id+"escada", "0", qos=0)
            

        except:
            print("Sorry could not recognize your voice")    # In case of voice not recognized  clearly
