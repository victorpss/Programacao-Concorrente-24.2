# Atividade 2

**OBJETIVO DO EXERCÍCIO:** Mostrar como usar channels para comunicação entre goroutines.

**INSTRUÇÕES PARA RODAR O CÓDIGO (Preferencialmente ambiente Linux):**

>1 - Abra o terminal
>
>2 - Navegue até a pasta de sua preferência que irá receber o código 
>
>3 - Cole o comando ```git clone https://github.com/victorpss/Programacao-Concorrente-24.2.git```
>
>4 - Entre na pasta com o comando ```cd Programacao-Concorrente-24.2/Lab9```
>
>5 - Rode o programa com ```go run atv2.go```
>
>6 - Veja o diálogo entre as goroutines no terminal.


# Atividade 5

**OBJETIVO DO EXERCÍCIO:** Usar os recursos de concorrência de Go, com comunicação e sincronização via canais, para resolver o problema de encontrar numeros primos em uma lista de entrada

**INSTRUÇÕES PARA RODAR O CÓDIGO (Preferencialmente ambiente Linux):**

>1 - Siga até o passo 4 da Atividade 2
>
>2 - Compile o programa com o comando ```go build atv5.go```
>
>3 - Rode o programa com o comando ```./atv5 [ninteiros] [mgoroutines]```, substituindo ```[ninteiros]``` com a quantidade de inteiros que será avaliada e ```[mgoroutines]``` para indicar quantas goroutines serão criadas para executar o programa. Exemplo: ```./atv5 100 10```
>
>4 - Veja no terminal a quantidade de números primos encontrados até o inteiro indicado no passo anterior.
>
>5 (opcional) - Altere, no código, a variável ```print_prime``` na linha 34 para ```true``` para printar todos os primos encontrados até o inteiro indicado e volte ao passo 2.
>
>*OBS: Os primos não serão necessariemente printados em ordem crescente devido à execução concorrente das goroutines.*
