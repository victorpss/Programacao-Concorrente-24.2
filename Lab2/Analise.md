O objetivo desse trabalho foi avaliar a diferença entre processamentos do produto interno de 2 vetores de maneira sequencial e maneira concorrente. Como os elementos dos vetores são do tipo **float**, há uma grande diferença em guardar o resultado da multiplicação em **float** ou **double**.

Se guardamos em **double**, isso indica que a precisão da multiplicação é muito alta, de forma que veríamos arredondamento em uma casa decimal muito avançada, portanto, não é possível ver um erro aparente entre os resultados dos produtos internos feitos de modo sequencial e concorrente.
Foi o jeito que implementei neste trabalho e realmente encontrei esse resultado. No entanto, poderíamos trocar de **double** para **float**:

Se guardássemos em **float**, a precisão seria menor e, consequentemente, veríamos um truncamento do resultado bem mais cedo, de modo que o erro entre os diferentes cálculos do produto interno seria perceptível em poucas casas decimais. 
Realizei esse teste trocando o tipo do resultado e realmente foi possível encontrar esse erro.

<br></br>
*Por que vemos resultados diferentes se multiplicamos os mesmos números?*

Porque a forma de ir somando as multiplicações muda do sequencial para o concorrente! No sequencial, multiplicamos os elementos da primeira posição, guardamos, multiplicamos os elementos da segunda posição, somamos ao que estava guardado, guardamos, e repetimos esse processo até o final.
Na forma concorrente, as threads vão multiplicando diferentes conjuntos de elementos, e, portanto, a soma ocorre em uma ordem diferente, acontecendo de acordo com o fim da execução das threads, que pode não ser igual à forma da soma no modo sequencial,
e a magnitude dos números (em ponto flutuante) somados pode acarretar essa diferença de arredondamento. Ou seja, a ordem da soma muda o resultado!
