package main

import (
    "fmt"
    "time"
)

func tarefa(str chan string, cont int) {
    msg := <- str
    fmt.Println(msg)
    if cont == 0 {
        str <- "Oi main, bom dia, tudo bem?"
    } else {
        str <- "Certo, entendido"
        fmt.Println("(routine) finalizando")
    }
}

func main() {
    var msg = "" //string vazia
    
    //cria um canal de comunicacao nao-bufferizado
    str := make(chan string)
    
    go tarefa(str, 0)
    str <- "Olá, goroutine, bom dia!"
    msg = <- str
    fmt.Println(msg)
    
    go tarefa(str, 1)
    str <- "Tudo bem! Vou terminar tá?"
    msg = <- str
    fmt.Println(msg)
    
    time.Sleep(time.Second) // esperando a routine rodar por completo

    fmt.Println("(main) finalizando")
}