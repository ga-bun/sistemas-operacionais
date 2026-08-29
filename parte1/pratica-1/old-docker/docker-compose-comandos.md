alpine linux
https://alpinelinux.org/downloads/

// iniciar o container
docker compose up -d --build

// lista os containers em excecução (adicionar -a pra listar todos)
docker ps

// parar um container
docker stop <container ID ou nome do container>

// iniciar um container que foi parado -> ai = modo interativo
docker start -ai <container ID ou nome do container>

// remover um container
docker rm <container ID ou nome do container>

// remover todos os containers inativos
docker container prune

// entrar no container no bash c
docker compose exec c-dev sh

// compilar
gcc -Wall -g main.c -o main ./main

// adicionar algo:
apk add --no-cache strace ltrace

// executando o comando proc com o docker ao invés disso
cat /proc/interrupts | grep -E "CPU0|SATA|nvme|timer|LOC"
// isso:
cat /proc/interrupts | grep -iE 'CPU|SATA|nvme|timer|LOC'


// observa as syscalls do programa dentro do container:
strace -f -o strace.log ./gerador_io

// salva em arquivo:
strace -f -o strace.log ./gerador_io