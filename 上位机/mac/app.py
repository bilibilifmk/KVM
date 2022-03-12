# -*- coding: UTF-8 -*-
# 引入FLASK
from flask import Flask, request, redirect

from flask import send_from_directory
import pyscreenshot as ImageGrab
app = Flask(__name__, static_url_path='/')

@app.route('/', methods=['GET'])
def index():
    im = ImageGrab.grab(bbox=(0, 90, 1920, 1080))
    im.save("pc.png")
    return send_from_directory('','pc.png')

if __name__ == '__main__':
    app.run(host='0.0.0.0',port=5005,debug=False)

