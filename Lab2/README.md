**OBJETIVO DO EXERCÍCIO:** Projetar, implementar e avaliar uma solução concorrente para o problema de calcular o produto interno de dois vetores de números reais.

**INSTRUÇÕES PARA RODAR O CÓDIGO (Preferencialmente ambiente Linux):**

>1 - Abra o terminal
>
>2 - Navegue até a pasta de sua preferência que irá receber o código 
>
>3 - Cole o comando ```git clone https://github.com/victorpss/Programacao-Concorrente-24.2.git```
>
>4 - Entre na pasta com o comando ```cd Programacao-Concorrente-24.2/Lab2```
>
>5 - Compile o programa sequencial com o comando ```gcc -o [nome] sequencial.c -Wall```, substituindo [nome] com o nome de sua preferência para o arquivo executável que será gerado.
>
>6 - Rode o programa com ```./[nome] [mthreads] [nelementos]```, substituindo [nome] pelo nome do executável escolhido e [nelementos] pela quantidade de elementos do tipo inteiro que os 2 vetores terão. Exemplo: ```./sequencial 6```. Serão printados na tela:
> - A dimensão dos vetores;
> - Os elementos de cada vetor;
> - O produto interno calculado sequencialmente.
>
>7 - Compile o programa concorrente com o comando ```gcc -o [nome] concorrente.c -Wall```, substituindo [nome] com o nome de sua preferência para o arquivo executável que será gerado.
>
>8 - Rode o programa ```./[nome] [mthreads] vetores```, substituindo [nome] pelo nome do executável escolhido, [mthreads] pela quantidade de threads que realizarão o cálculo do produto. 'vetores' é o nome do arquivo que foi criado no programa sequencial e que está guardando os vetores gerados. Exemplo: ```./concorrente 3 vetores```. Serão printados na tela:
> - O resultado do produto interno calculado no programa concorrente;
> - O resultado do produto interno calculado no programa sequencial;
> - A variação relativa entre os 2 resultados.
> - Um aviso de fim da thread principal.
