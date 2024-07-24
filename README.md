# Escalonador de workflow de processos
Projeto de o Sistemas Operacionais (2024.1) desenvolvido em linguagem C para escalonar processos

## Ambiente
Especificações da máquina utilizada para testes:

```
 Versão do compilador: gcc version 9.4.0 (Ubuntu 9.4.0-1ubuntu1~20.04.1) 
 
 Sistema Operacional: Ubuntu 20.04.3 LTS
```

## Setup
- Configurando arquivo de entrada
```sh
touch programs.txt
# edita o arquivo no seguinte formato
# <id> <comando> <dependencia>
# exemplo: 1 teste15 0,#
```
- Compilando
```sh
make all
```

> Exemplo de [programs.txt](./programs.txt)

## Execução

```sh
./escalona {numero_de_cores}
```
