**OBJETIVO DO EXERCÍCIO:** Usar o padrão leitores e escritores com prioridade para operações de escrita.

**INSTRUÇÕES PARA RODAR O CÓDIGO (Preferencialmente ambiente Linux):**

>1 - Abra o terminal
>
>2 - Navegue até a pasta de sua preferência que irá receber o código 
>
>3 - Cole o comando ```git clone https://github.com/victorpss/Programacao-Concorrente-24.2.git```
>
>4 - Entre na pasta com o comando ```cd Programacao-Concorrente-24.2/Lab6```
>
>5 - Compile o programa com o comando ```gcc -o [nome] lab6.c list_int.c -Wall```, substituindo [nome] com o nome de sua preferência para o arquivo executável que será gerado.
>
>6 - Rode o programa com ```./[nome] [nthreads]```, substituindo [nome] pelo nome do executável escolhido e [nthreads] pela quantidade de threads que o programa irá conter. Exemplo: ```./lab 20```
>
>7 - Visualize o terminal e perceba a ordem das operações sendo executadas ou entrando em fila.

<br></br>
**Análise:** O programa escrito deverá respeitar a prioridade das operações de escrita, isto é, sempre que existir thread realizando escrita ou thread(s) na fila para escrita, novas operações de leitura não podem começar. Para isso, algumas condições são verificadas para garantir tal objetivo:
- Para iniciar uma escrita, deverá ser verificado se alguma thread está realizando a escrita ou a leitura. Caso positivo, essa thread é bloqueada com o comando ```pthread_cond_wait``` e entra na fila de escrita. Caso contrário, ela pode iniciar a escrita.
- Para iniciar uma leitura, deverá ser verificado se alguma thread está realizando a escrita ou se existem threads na fila para escrever, pois a prioridade é delas. Caso positivo, a thread é bloqueada com o comando ```pthread_cond_wait``` e entra na fila para leitura (e só lê após o esgotamento da fila de escrita). Caso contrário, ela pode iniciar a leitura.
- Para finalizar uma escrita, deverá ser verificado se tem alguém na fila de escrita. Caso positivo, através do comando ```pthread_cond_signal```, a próxima thread na fila será liberada para realizar escrita (ela só sai da fila de bloqueados, não sabemos quando ela tomará posse da CPU). Caso contrário, posso liberar a leitura para todas as threads na fila de leitura com o comando ```pthread_cond_broadcast```.
- Para finalizar uma leitura, verifica-se o novo número de leitores. Se for 0, libera uma thread na fila de escrita com o comando ```pthread_cond_signal```.

<br></br>
**Recorte das execuções:**

![Print do terminal](terminal.png)
- Em 1, a thread 15 entra para iniciar uma escrita.
- Em 2, ela termina e libera a leitura para todas as threads na fila de leitura, visto que não existe thread na fila de escrita.
- Em 3, a thread 11 inicia a leitura.
- A partir de 4, a thread 3 inicia e finaliza uma leitura 2 vezes.
- Em 5, a thread 3 chega para realizar uma escrita, no entanto, como a thread 11 ainda não terminou a leitura, ela entra na fila de escrita.
- Em 6, a thread 11 finaliza a leitura e libera todas as threads da fila de espera da escrita.
- Em 7, a thread 7 inicia escrita. Mesmo com a thread 3 já liberada para a escrita, a política de escalonamento favoreceu a entrada do 7 (que acabou de chegar) antes do 3.
- Em 8, a thread 7 inicia novamente uma escrita e finaliza em seguida.
- Em 9, a thread 3 finalmente inicia a escrita.
- Em 10, a thread 3 finaliza a escrita e libera todas as threads que entraram na fila de leitura (pois a prioridade de escrita as obrigaram), visto que nesse momento não há thread na fila de escrita.

Posteriormente, as threads vão alternando ciclos de leitura e escrita na mesma lógica dessa sequência de execuções, respeitando a prioridade das threads de escrita.
