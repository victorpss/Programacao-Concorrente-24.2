**OBJETIVO DO EXERCÍCIO:** Introduzir o mecanismo de sincronização por condição usando variáveis de condição da biblioteca Pthread.

**INSTRUÇÕES PARA RODAR O CÓDIGO (Preferencialmente ambiente Linux):**

>1 - Abra o terminal
>
>2 - Navegue até a pasta de sua preferência que irá receber o código 
>
>3 - Cole o comando ```git clone https://github.com/victorpss/Programacao-Concorrente-24.2.git```
>
>4 - Entre na pasta com o comando ```cd Programacao-Concorrente-24.2/Lab5```
>
>5 - Compile o programa de sincronização com o comando ```gcc -o [nome] soma-lock-atom.c -Wall```, substituindo [nome] com o nome de sua preferência para o arquivo executável que será gerado.
>
>6 - Rode o programa de sincronização com ```./[nome] [nthreads]```, substituindo [nome] pelo nome do executável escolhido e [nthreads] pela quantidade de threads que o programa irá conter. Exemplo: ```./soma 100```
>
>7 - Visualize o terminal e perceba os valores da variável 'soma' impressos.
>
>8(opcional) - Comente/descomente a linha 6 ```#define THREADS``` para mostrar/esconder o log das threads (ordem de execução e terminação).
