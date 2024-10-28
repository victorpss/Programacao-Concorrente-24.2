package main
import "fmt"

func tarefa(str chan string) {
    fmt.Println(<-str, "(impresso pela Goroutine)") // lê e esvazia o que tá no canal
    str <- "Goroutine envia para Main: Oi main, bom dia, tudo bem?" // enche o canal, tarefa é bloqueada e vai para a rotina main
    
    // o canal é esvaziado na Main...
    // o canal é preenchido novamente na Main e volta para cá
    
    fmt.Println(<-str, "(impresso pela Goroutine)")
    str <- "Goroutine envia para Main: Certo, entendido"
    
    <-str // esvazia canal
    fmt.Println("Goroutine imprime: finalizando")
    str <- "" // enche o canal só para voltar para a main
}

func main() {
    str := make(chan string) //cria um canal de comunicacao nao-bufferizado
    
    go tarefa(str) // cria a outra rotina e envia o canal
    str <- "Main envia para Goroutine: Olá, goroutine, bom dia!" // enche o canal, main é bloqueada e vai para a rotina tarefa
    
    fmt.Println(<-str, "(impresso pela Main)")
    str <- "Main envia para Goroutine: Tudo bem! Vou terminar tá?"
    
    fmt.Println(<-str, "(impresso pela Main)")
    str <- "" // enche o canal só para voltar para a goroutine
    
    <-str
    fmt.Println("Main imprime: finalizando")
}