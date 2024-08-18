**OBJETIVO DO EXERCÍCIO:** Escrever um programa concorrente com M threads, para somar 1 a cada elemento de um vetor de N elementos do tipo inteiro (Para cada elemento $a_i$ do vetor, calcular o novo valor ($a_i$ + 1) e escrever o resultado na mesma posição do elemento).
A tarefa completa deverá ser dividida entre as M threads de forma mais balanceada possível (as threads devem receber a mesma carga de trabalho). Os valores de M e N devem ser informados na chamada do programa.


**INSTRUÇÕES PARA RODAR O CÓDIGO (Preferencialmente ambiente Linux):**

>1 - Abra o terminal
>
>2 - Navegue até a pasta de sua preferência que irá receber o código 
>
>3 - Cole o comando ```git clone https://github.com/victorpss/Programacao-Concorrente-24.2.git```
>
>4 - Entre na pasta com o comando ```cd Programacao-Concorrente-24.2/Lab1```
>
>5 - Compile o programa com o comando ```gcc -o [nome] atividade5.c```, substituindo [nome] com o nome de sua preferência para o arquivo executável que será gerado.
>
>6 - Rode o programa com ```./[nome] [mthreads] [nelementos]```, substituindo [nome] pelo nome do executável escolhido, [mthreads] pela quantidade de threads que serão criadas, e [nelementos] pela quantidade de elementos do tipo inteiro que o vetor irá ter. Exemplo: ```./main 2 4```
