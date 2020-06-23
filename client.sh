declare -a inserir_array
inserir_len=61
declare -a remover_array
remover_len=61
declare -a listar_titulo_array
listar_titulo_len=61
declare -a listar_genero_array
listar_genero_len=61
declare -a nome_array
nome_len=61
declare -a info_array
info_len=61
declare -a tudo_array
tudo_len=61

inserir_array[0]="inserir"
remover_array[0]="remover"
listar_titulo_array[0]="listar_titulo"
listar_genero_array[0]="listar_genero"
nome_array[0]="nome"
info_array[0]="info"
tudo_array[0]="tudo"

for i in $(seq 1 61); do
    ./bin/client.x < ./in/inserir_client_in > client_out
    time=$(grep % client_out)
    time=${time//% }
    # echo $time
    inserir_array[i]=$time

    # echo -n "remove\n"${i}"\n" >> ./in/remove_client_in
    # ./bin/client.x < ./in/tudo_client_in > client_out
    # time=$(grep % client_out)
    # time=${time//% }
    # tudo_array[i]=$time

    ./bin/client.x < ./in/listar_titulo_client_in > client_out
    time=$(grep % client_out)
    time=${time//% }
    listar_titulo_array[i]=$time

    ./bin/client.x < ./in/listar_genero_client_in > client_out
    time=$(grep % client_out)
    time=${time//% }
    listar_genero_array[i]=$time

    ./bin/client.x < ./in/nome_client_in > client_out
    time=$(grep % client_out)
    time=${time//% }
    nome_array[i]=$time

    ./bin/client.x < ./in/info_client_in > client_out
    time=$(grep % client_out)
    time=${time//% }
    info_array[i]=$time

    ./bin/client.x < ./in/tudo_client_in > client_out
    time=$(grep % client_out)
    time=${time//% }
    tudo_array[i]=$time

    # sleep 2
done

echo "" > client.csv
len=$(expr $tudo_len - 1)
for i in $(seq 0 $len); do
    echo -n ${inserir_array[i]}"," >> client.csv
    echo -n ${remover_array[i]}"," >> client.csv
    echo -n ${listar_titulo_array[i]}"," >> client.csv
    echo -n ${listar_genero_array[i]}"," >> client.csv
    echo -n ${nome_array[i]}"," >> client.csv
    echo -n ${info_array[i]}"," >> client.csv
    echo ${tudo_array[i]} >> client.csv
done
