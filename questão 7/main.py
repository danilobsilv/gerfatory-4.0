import socket
# Configurações do servidor da caldeira:
servidor = "192.168.0.5"  
porta_destino = 4545 


FALHA = 1
TAM_MEU_BUFFER = 1000
TAMANHO_STRING = 10000
leTempA = "sta0"
leTemp = "st-0"
leTempI = "sti0"
leNo = "sno0"
leAltura = "sh-0"
qOn = 100000.0
qOff = 0.0
niOn = 100.00
niOff = 0.0

file_name = "questão 7\decisoes_log.txt"
ultimas_decisoes = []

def criar_socket_local():
    return socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def enviar_comando(socket_local, comando):
    try:
        endereco_destino = (servidor, porta_destino)
        socket_local.sendto(comando.encode(), endereco_destino)
    except Exception as e:
        print("Erro ao enviar comando:", e)

def receber_resposta(socket_local):
    try:
        data, endereco = socket_local.recvfrom(1024)
        return data.decode().strip()
    except Exception as e:
        print("Erro ao receber resposta:", e)
        return None


def salvar_no_arquivo(decisao):
    valor_sem_prefixo = decisao.split("st-")[-1]
    with open(file_name, "a") as arquivo:
        arquivo.write(valor_sem_prefixo + "\n")

if __name__ == "__main__":
    socket_local = criar_socket_local()

    try:
        while True:
            # Menu de opções para o usuário
            print("Menu:")
            print("1 - Obter valor de Medição T (C)")
            print("2 - Obter valor de Virtual QT (Js)")
            print("3 - Calcular razão entre Medição T (C) e Virtual QT (Js)")
            print("4 - Calcular multiplicação entre Medição T (C) e Virtual QT (Js)")
            print("5 - Sair")

            opcao = input("Digite o número da opção desejada: ")

            if opcao == "1":
                # Comando para obter o valor de Medição T (C)
                enviar_comando(socket_local, "st-0")
                resposta = receber_resposta(socket_local)
                if resposta:
                    salvar_no_arquivo(resposta)
                    print("Medição T (C):", resposta)

            elif opcao == "2":
                # Comando para obter o valor de Virtual QT (Js)
                enviar_comando(socket_local, "sno0")
                resposta = receber_resposta(socket_local)
                if resposta:
                    salvar_no_arquivo(resposta)
                    print("Virtual QT (Js):", resposta)

            elif opcao == "3":
                # Comando para obter o valor de Medição T (C)
                enviar_comando(socket_local, "st-0")
                resposta_t = receber_resposta(socket_local)

                # Comando para obter o valor de Virtual QT (Js)
                enviar_comando(socket_local, "sno0")
                resposta_qt = receber_resposta(socket_local)

                if resposta_t and resposta_qt:
                    try:
                        razao = float(resposta_t) / float(resposta_qt)
                        salvar_no_arquivo(razao)
                        print("Razão entre Medição T (C) e Virtual QT (Js):", razao)
                    except ValueError:
                        print("Erro ao calcular razão. Verifique os valores recebidos.")

            elif opcao == "4":
                # Comando para obter o valor de Medição T (C)
                enviar_comando(socket_local, "st-0")
                resposta_t = receber_resposta(socket_local)

                # Comando para obter o valor de Virtual QT (Js)
                enviar_comando(socket_local, "sno0")
                resposta_qt = receber_resposta(socket_local)

                if resposta_t and resposta_qt:
                    try:
                        multiplicacao = float(resposta_t) * float(resposta_qt)
                        salvar_no_arquivo(multiplicacao)
                        print("Multiplicação entre Medição T (C) e Virtual QT (Js):", multiplicacao)
                    except ValueError:
                        print("Erro ao calcular multiplicação. Verifique os valores recebidos.")

            elif opcao == "5":
                break

            else:
                print("Opção inválida. Digite um número válido.")

    except KeyboardInterrupt:
        print("Encerrando a conexão.")

    finally:
        socket_local.close()
