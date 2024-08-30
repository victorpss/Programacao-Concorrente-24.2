Com todos os gráficos gerados, foi possível notar a vantagem do uso da concorrência em matrizes de dimensões bem altas, com o tempo de processamento diminuindo quase que proporcionalmente ao aumento do número de threads (visto até 4 threads).

Esse fenômeno se deve ao fato da máquina de teste (a minha) possuir 4 núcleos, o que permitiu que cada processador rodasse 1 thread simultaneamente para realizar o cálculo da multiplicação, diferentemente do programa sequencial ou o de 1 thread que jogava a carga de trabalho para um processador só e acabava levando mais tempo visto que não é possível realizar várias operações simultaneamente.

A diminuição da eficiência também leva esse fato em consideração, já que um número de threads maior do que a quantidade de núcleos pode acarretar em diversas trocas de contexto realizadas pelo sistema operacional, interferindo na eficiência do programa.

O aumento da aceleração até 4 threads também segue a mesma linha de raciocínio. Após 4 threads, não será notado um aumento significativo da aceleração por não ser possível realizar mais do que 4 operações ao mesmo tempo na máquina com processador de 4 núcleos.
