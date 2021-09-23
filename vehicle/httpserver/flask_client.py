from flask import Flask, request


app = Flask(__name__)

# @app.route('/')
# def index():
#     return 'Hello World'

@app.route('/')
def requestdata():
    return "Hello! Your IP is {} and you are using {}: ".format(request.remote_addr,  
                                                                request.user_agent)

if __name__ == "__main__":
    app.run()

