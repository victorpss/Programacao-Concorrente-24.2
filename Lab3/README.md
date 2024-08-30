**OBJETIVO DO EXERCÍCIO:** Projetar e implementar uma solução concorrente para o problema de multiplicação de matrizes, coletar informações sobre o seu tempo de execução e calcular o ganho de desempenho obtido.

**INSTRUÇÕES PARA RODAR O CÓDIGO (Preferencialmente ambiente Linux):**

>1 - Abra o terminal
>
>2 - Navegue até a pasta de sua preferência que irá receber o código 
>
>3 - Cole o comando ```git clone https://github.com/victorpss/Programacao-Concorrente-24.2.git```
>
>4 - Entre na pasta com o comando ```cd Programacao-Concorrente-24.2/Lab3```
>
>5 - Compile o programa gerador de matrizes com o comando ```gcc -o [nome] geraMatrizBinario.c -Wall```, substituindo [nome] com o nome de sua preferência para o arquivo executável que será gerado.
>
>6 - Compile o programa sequencial com o comando ```gcc -o [nome] sequencial.c -Wall```, substituindo [nome] com o nome de sua preferência para o arquivo executável que será gerado.
>
>7 - Compile o programa concorrente com o comando ```gcc -o [nome] concorrente.c -Wall```, substituindo [nome] com o nome de sua preferência para o arquivo executável que será gerado.
>
>8 - Rode o programa gerador de matrizes com ```./[nome] [nlinhas] [mcolunas] [arquivoBinario]```, substituindo [nome] pelo nome do executável escolhido, [nlinhas] pela quantidade de linhas que a matriz irá conter, [mcolunas] pela quantidade de colunas e [arquivoBinario] pelo nome do arquivo binário que irá conter a matriz gerada. Exemplo: ```./geraMatrizBinario 2 3 matriz1```
>
>9 - Repita o passo anterior, desta vez escolhendo outro valor (ou igual) para linhas e colunas e outro nome para o arquivo binário gerado (OBS: se não quiser enfrentar problemas futuros, escolha o nº de linhas igual ao nº de colunas da primeira matriz gerada). ```./geraMatrizBinario 3 4 matriz2```
>
>10 - Rode o programa sequencial com o comando ```./[nome] [entrada1] [entrada2] [saida]```, substituindo [nome] com o nome do executável escolhido, [entrada1] e [entrada2] pelos nomes escolhidos para os arquivos binários com as matrizes e [saida] para o nome do arquivo binário de saída contendo o resultado da multiplicação das 2 matrizes de entrada. Será printado:
> - O tempo necessário para a inicialização do programa;
> - O tempo necessário para o processamento;
> - O tempo necessário para a finalização.
>
>11 - Rode o programa concorrente com o comando ```./[nome] [entrada1] [entrada2] [saida] [nthreads]```, substituindo [nome] com o nome do executável escolhido, [entrada1] e [entrada2] pelos nomes escolhidos para os arquivos binários com as matrizes, [saida] para o nome do arquivo binário de saída contendo o resultado da multiplicação das 2 matrizes de entrada e [nthreads] para a quantidade de threads criadas para realizar a operação. Será printado:
> - O tempo necessário para a inicialização do programa;
> - O tempo necessário para o processamento;
> - O tempo necessário para a finalização.
