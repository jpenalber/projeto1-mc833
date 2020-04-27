#!/bin/sh

#./bin/server.x > server_out

alltimes=$(grep % $1)

declare -a inserir_array
inserir_len=1
declare -a remover_array
remover_len=1
declare -a listar_titulo_array
listar_titulo_len=1
declare -a listar_genero_array
listar_genero_len=1
declare -a nome_array
nome_len=1
declare -a info_array
info_len=1
declare -a tudo_array
tudo_len=1

inserir_array[0]="inserir"
remover_array[0]="remover"
listar_titulo_array[0]="listar_titulo"
listar_genero_array[0]="listar_genero"
nome_array[0]="nome"
info_array[0]="info"
tudo_array[0]="tudo"

for time in $alltimes; do
    reqtype=$(echo $time | cut -d- -f2)"_array"
    reqlen=$(echo $time | cut -d- -f2)"_len"
    time=$(echo $time | cut -d- -f3)

    eval "$reqtype[${!reqlen}]=$time"
    eval "$reqlen=$(expr ${!reqlen} + 1)"
done

echo "" > server.csv
len=$(expr $tudo_len - 1)
for i in $(seq 0 $len); do
    echo -n ${inserir_array[i]}"," >> server.csv
    #echo -n ${remover_array[i]}"," >> server.csv
    echo -n ${listar_titulo_array[i]}"," >> server.csv
    echo -n ${listar_genero_array[i]}"," >> server.csv
    echo -n ${nome_array[i]}"," >> server.csv
    echo -n ${info_array[i]}"," >> server.csv
    echo ${tudo_array[i]} >> server.csv
done
