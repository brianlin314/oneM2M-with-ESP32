from flask import Flask, request
import requests, base64, time
import xmltodict
import json

OM2M_URL = "http://192.168.0.87:8282/~"
CSE_ID = "/mn-cse/"
CSE_NAME = "mn-name"
LOGIN="admin"
PSWD="admin"
OM2M_BASE = OM2M_URL+CSE_ID
auth_headers = {"X-M2M-ORIGIN":LOGIN+":"+PSWD}
# The other accepted value is application/xml
common_headers = {"Accept": "application/json"}

app = Flask(__name__)



@app.route('/get_data', methods=['POST'])
def receive_data():
    data = request.get_json()
    print("Received data:", data)

    header_cin = {"Content-Type":"application/xml;ty=4"}
    # body_cin = """
    # <m2m:cin xmlns:m2m="http://www.onem2m.org/xml/protocols">
    #     <cnf>{0}</cnf>
    #     <con>{1}</con>
    # </m2m:cin>
    # """.format(mode, base64.b64encode(cin).decode("utf-8"))
    body_cin = xmltodict.unparse({'m2m:cin': {'@xmlns:m2m': 'http://www.onem2m.org/xml/protocols',
                                           'cnf': 'message',
                                           'con': '''
                                            <obj>;
                                                <str name="appId" val="DHT22"/>;
                                                <str name="humidity" val="{} "/>
                                                <str name="temperature" val="{} "/>
                                            </obj>
                                            '''.format(data['humidity'], data['temperature'])
                                           }}, full_document=False)
    print(body_cin)
    response = requests.post(OM2M_BASE+CSE_NAME+"/DHT22/DATA", data=body_cin, headers={**auth_headers, **common_headers, **header_cin})
    print(response)

    return "Data received successfully!"




if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)