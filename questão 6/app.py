from flask import Flask, jsonify
from middleware import authenticate_jwt_middleware


app = Flask(__name__)


@app.route("/protected-resource")
@authenticate_jwt_middleware
def protected_resource(user_id):
    # nesse ponto, a requisição já teria passado pelo middleware e já estaria autenticada
    # já seria capaz de usar o id do usuário para acessar suas informações
    return jsonify({"message": "You have access to this protected resource!"})

if __name__ == "__main__":
    app.run(debug=True)