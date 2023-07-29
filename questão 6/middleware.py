from flask import request, jsonify
from functools import wraps
from user_utils import get_user_id_from_token

def authenticate_jwt_middleware(func):
      @wraps(func)  # decorador para preservar os dados da função de rota original
      def decorated(*args, **kwargs):
            # obtenção do token 
            token = request.headers.get("Authorization")

            # abaixo ocorrem as verificações de token e de usuário
            
            if not token:
                  return jsonify({"error": "Token not provided"}), 401

            user_id = get_user_id_from_token(token)

            if not user_id:
                  return jsonify({"error": "Invalid token"}), 401


            # se o token for válido, a aplicação continua para a "aplicação principal"
            return func(user_id=user_id, *args, **kwargs)

      return decorated
