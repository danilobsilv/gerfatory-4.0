from datetime import datetime
from flask import Flask, request


app = Flask(__name__)


# middleware que registra o horário e o id de cada requisição
class RequestTimeMiddleware:
    def __init__(self, app):
        self.app = app
        self.file_name = "request_log.txt"
        self.counter = 0

      # Método chamado quando uma requisição é recebida pela aplicação
    def __call__(self, environ, start_response):
        self.counter += 1
        request_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        environ['request_time'] = request_time
        environ['request_id'] = self.counter
        return self.app(environ, start_response)


# config do middleware para ser usada na aplicação flask
app.wsgi_app = RequestTimeMiddleware(app.wsgi_app)


# Rota principal que responde com a mensagem "REQUISIÇÃO" e registra o horário e o ID
@app.route('/')
def index():
    request_time = request.environ.get('request_time', 'N/A')
    request_id = request.environ.get('request_id', 'N/A')
    response = f"REQUISIÇÃO! Request ID: {request_id}, Request Time: {request_time}"
    
    # Grava a informação no arquivo de texto
    with open("request_log.txt", "a") as f:
        f.write(f"Request ID: {request_id}, Request Time: {request_time}\n")
    
    return response

if __name__ == '__main__':
    app.run()
