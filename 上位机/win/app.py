# -*- coding: UTF-8 -*-
# 引入FLASK
from flask import Flask, request, redirect
from flask import send_from_directory
from PIL import ImageGrab
app = Flask(__name__, static_url_path='/')

@app.route('/', methods=['GET'])
def index():
     
    im = ImageGrab.grab()
    im.save('pc.png')

    return send_from_directory('','pc.png')
    #return "aaaaa"

if __name__ == '__main__':
    app.run(host='0.0.0.0',port=5005,debug=False)




