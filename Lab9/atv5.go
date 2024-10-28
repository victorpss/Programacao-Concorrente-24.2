package main

import (
    "fmt"
    "math"
    "os"
    "strconv"
)

func ehPrimo(n int) bool{
    if n <= 1 {
        return false
    }
    if n == 2 {
        return true
    }
    if n%2 == 0 {
        return false
    }
    
    for i:=3; i<int(math.Sqrt(float64(n)))+1; i+=2 {
        if n%i==0 {
            return false
        }
    }
    
    return true;
}

func main() {
    var total int = 0
    
    // altere entre true e false para mostrar/esconder os números primos encontrados na hora de executar o programa
    var print_prime bool = false 
    // obs: não necessariamente serão printados em ordem crescente, visto que as goroutines são executadas de forma concorrente
    
    if len(os.Args) <= 2 {
        fmt.Println("Execute com:", os.Args[0], "<N inteiros> <M goroutines>")
        return
    }

    n, err := strconv.Atoi(os.Args[1])
    if err != nil {
        fmt.Println("Erro ao converter", os.Args[1], "para inteiro.")
		return
    }
    
    m, err := strconv.Atoi(os.Args[2])
    if err != nil {
        fmt.Println("Erro ao converter", os.Args[2], "para inteiro.")
		return
    }
    
    // criando o canal com buffer de tamanho N
    num_chan := make(chan int, n)
    // canal para obter quantos valores primos cada goroutine achou
    cont_chan := make(chan int, m)
    
    // carregando o canal com os números de 1 a N
    for j:=1; j<=n; j++ {
        num_chan <- j
    }
    
    // fechando o canal para escritas
    close(num_chan)
    
	// criando as goroutines para avaliarem o canal de numeros
    for k:=0 ; k<m; k++ {
        go func(){
            cont := 0
            for num := range num_chan {
                if ehPrimo(num){
                    cont++;
                    if print_prime {
                        fmt.Println("Primo encontrado:", num)
                    }
                }
            }
            cont_chan <- cont
        }()
    }
    
    for i:=0; i<m; i++ {
        total += <-cont_chan
    }
    
    fmt.Println("Total de primos encontrados até", n, ":", total)
}