.section  .data
    TOPO_HEAP: .quad 0                  #Ponteiro para o topo da Heap, nesse momento ela está vazia
    INICIO_HEAP: .quad 0                #Ponteiro fixo para o início da heap
    TOPO_ALOC: .quad 0			#Ponteiro para o fim da alocação
    ANDARILHO: .quad 0                  #Ponteiro que caminha pela heap em alocaMem
    BEST_FIT: .quad 0                   #Ponteiro pra o espaço achado
    BEST_FLAG: .quad 0                 	#Flag para saber se algum bloco foi encontrado
    BYTES_A_ALOCAR: .quad 0             #Guarda o numero de bytes a alocar
    BYTES_PARTICIONA: .quad 0		#Guarda o numero de bytes de uma partição
    ENDEREÇO_A_DESALOCAR: .quad 0       #Guarda o endereço da variavel a dar free

    livre: .string "-"
    ocupado: .string "+"
    mapa_vazio: .string "\nMapa vazio\n\n"
    bytesFlags: .string "################"
    str2: .string "\n\n"
    str4: .string "API para alocação de mémoria em Assembly\n\n"

.section .text
.globl alocaMem 
.globl iniciaAlocador
.globl liberaMem
.globl finalizaAlocador
.globl imprimeMapa

iniciaAlocador:
    pushq %rbp
    movq %rsp,%rbp
    movq $str4,%rdi
    call printf
    movq $12,%rax
    movq $0,%rdi                        #Passa 0 para %rdi para se retornar o valor de brk em %rax
    syscall
    movq %rax,TOPO_HEAP                 #Passa os valores de brk para as variáveis globais
    movq %rax,INICIO_HEAP
    movq %rax,TOPO_ALOC               
    pop %rbp
    ret


alocaMem:
    pushq %rbp
    movq %rsp,%rbp
    movq %rdi, BYTES_A_ALOCAR
    movq INICIO_HEAP, %rax
    movq TOPO_HEAP, %rbx
    cmpq %rbx, %rax
    je AlocaEspacoNovo 
    movq $0, BEST_FLAG                  #Reseta a Flag do Best Fit
    movq INICIO_HEAP, %rax                 
    movq %rax, ANDARILHO                #ANDARILHO = Começo da HEAP
    addq $16, ANDARILHO
 
    AchaLivre:
    movq ANDARILHO,%rax                 #%rax == ANDARILHO
    movq -16(%rax), %rbx                #Onde esta armazenado o sinal de ocupado ou livre
    cmpq $0, %rbx                       #Confere se ta livre
    je ConfereTamanho			#Se sim, verifica o tamanho
    Else:
    movq ANDARILHO,%rax
    movq -8(%rax), %rbx                 #%rbx recebe o tamanho do espaco alocado no nodo
    addq %rbx, %rax
    addq $16, %rax                      #%rax desloca o tamanho do espaco alocado + 16 bytes
    movq %rax, ANDARILHO                #ANDARILHO se desloca para o proximo nodo
    movq TOPO_ALOC, %rbx
    cmpq %rbx, %rax                     #Verifica se ANDARILHO chegou no topo da alocação
    jl AchaLivre			#Se não, continua andando na lista
    movq BEST_FLAG, %rax
    movq $0, %rbx
    cmpq %rax, %rbx 			#Verifica se achou um best fit
    je NovoBloco			#Se não, aloca um novo bloco no final da lista
    jmp Condiciona			#Ajusta o Best Fit pra receber o novo bloco
    Retorno:
    movq BEST_FIT, %rax
    movq $1, -16(%rax)			#Marca como ocupado o bloco
    pop %rbp
    ret					#Retorna para a main com o valor do endereco alocado em %rax

    ConfereTamanho:
    movq ANDARILHO, %rax
    movq -8(%rax), %rbx                 
    movq BYTES_A_ALOCAR, %rax
    cmpq %rax, %rbx			#Compara os tamanhos dos blocos
    jge MelhorBloco                     #Se for maior, verifica se ele é o menor dos achados
    jmp Else                            #Se não tem espaço volta a procurar novo nodo

    MelhorBloco:
    movq BEST_FLAG, %rax
    movq $0, %rbx
    cmpq %rax, %rbx			#Verifica se ja tem um best fit
    je SetBloco				#Se não, vai você mesmo
    movq BEST_FIT, %rax
    movq ANDARILHO, %rbx
    movq -8(%rax), %rax
    movq -8(%rbx), %rbx
    cmpq %rax, %rbx			#Verifica se o novo achado é melhor que o antigo
    jge SetBloco			#Se sim, salva o novo bloco
    jmp Else                            #Volta pra percorrer a lista

    SetBloco:
    movq ANDARILHO, %rax
    movq %rax, BEST_FIT			#Salva o novo bloco na variavel global
    movq $1, BEST_FLAG			#Seta a Flag como 1
    jmp Else                            #Volta pra percorrer a lista
    
    NovoBloco:
    movq TOPO_ALOC, %rbx
    addq BYTES_A_ALOCAR, %rbx
    addq $16, %rbx			#Calcula o espaço nescessário para o novo bloco
    movq TOPO_HEAP, %rax
    cmpq %rax, %rbx                     #Confere se cabe na Heap
    jge AlocaEspacoNovo                 #Se não aloca um novo bloco de memoria
    movq TOPO_ALOC, %rax
    addq $16, %rax
    movq $1, -16(%rax)			#Seta a flag
    movq BYTES_A_ALOCAR, %rbx
    movq %rbx, -8(%rax)			#Coloca o tamanho do bloco no header
    movq %rax, TOPO_ALOC		
    addq %rbx, TOPO_ALOC		#Atualiza o topo da alocação
    
    pop %rbp
    ret					#Retorna para a main com o valor do endereco alocado em %rax
    
    AlocaEspacoNovo:
    movq $4096, %rbx
    addq TOPO_HEAP, %rbx
    movq $12, %rax
    movq %rbx, %rdi
    syscall
    movq %rax, TOPO_HEAP		#Atualiza o topo da heap
    jmp NovoBloco			#Volta pra salvar o bloco

    Condiciona:
    movq BEST_FIT, %rax
    movq -8(%rax), %rax
    movq BYTES_A_ALOCAR, %rbx
    addq $16, %rbx
    cmpq %rax, %rbx 			#Compara se cabe um novo cabeçalho 
    jge Retorno 				#Se não cabe, salva com um pouco de espaço a mais
    subq %rbx, %rax
    movq %rax, BYTES_PARTICIONA		#Salva o espaço do novo bloco
    movq BEST_FIT, %rax
    movq BYTES_A_ALOCAR, %rbx
    movq %rbx, -8(%rax)			#Atualiza o cabeçalho da nova alocação
    addq BYTES_A_ALOCAR, %rax
    addq $16, %rax			
    movq $0, -16(%rax)
    movq BYTES_PARTICIONA, %rbx
    movq %rbx, -8(%rax)			#Atualiza o cabeçalho da partição
    jmp Retorno				#Volta e finaliza a função


liberaMem:
    pushq %rbp
    movq %rsp,%rbp    
    movq %rdi,ENDEREÇO_A_DESALOCAR
    movq ENDEREÇO_A_DESALOCAR,%rax      #rax recebe a posição exata de memória que deseja liberar
    movq $0,-16(%rax)                   #Seta como livre a flag
    popq %rbp
    ret


finalizaAlocador:
    pushq %rbp
    movq %rsp,%rbp
    movq $12, %rax
    movq INICIO_HEAP, %rdi              #Restaura a brk para o endereço original
    syscall
    movq %rdi, TOPO_ALOC
    movq %rdi, TOPO_HEAP		#evita segfault ao imprimir uma heap finalizada
    popq %rbp
    ret


imprimeMapa:
    pushq %rbp
    movq %rsp,%rbp
    
    movq TOPO_ALOC,%rax                 
    cmpq %rax,INICIO_HEAP               #Confere se o mapa está vazio para evitar segfault
    jne  Imprime
    mov $mapa_vazio,%rdi
    call printf
    popq %rbp
    ret
    
    Imprime:
    movq INICIO_HEAP, %rax              #Coloca o inicio da heap em rax
    addq $16, %rax                      #Desloca o rax pra primeira variavel
    movq %rax, ANDARILHO
    Loop:
    mov $bytesFlags,%rdi
    call printf
    movq ANDARILHO, %rax                #restaura %rax com o ANDARILHO
    movq -16(%rax), %rbx                #Coloca a flag de ocupado ou livre em %rbx
    cmpq $0, %rbx                       #Confere se está ocupado ou livre
    je  ImprimeLivre                    #se estiver livre imprime - vezes o número de bytes livres
    jne ImprimeOcupado                  #se estiver ocupado imprime + vees o número de bytes ocupados

    ImprimeLivre:
    movq ANDARILHO, %rax
    movq -8(%rax), %rbx                 #Move a quantidade de bytes ocupados para %rbx
    LoopLivre:                          #Parte do código que imprime o sinal de ocupado ou livre n vezes
    mov $livre, %rdi
    call printf
    subq $1, %rbx                       #Subtrai %rbx em 1 até chegar em 0, para imprimir os n bytes
    cmpq $0, %rbx
    jg LoopLivre                        #Enquanto %rbx for menor que 0 continua no loop
    movq ANDARILHO, %rax                #Restaura %rax
    movq -8(%rax), %rbx                 
    addq $16, %rbx
    addq %rbx, ANDARILHO                #desloca para o próximo nodo
    movq ANDARILHO,%rbx
    cmpq TOPO_ALOC, %rbx                #Confere se alcançou o fim da heap
    jl  Loop
    jmp Final

    ImprimeOcupado:
    movq ANDARILHO, %rax
    movq -8(%rax), %rbx                 #Move a quantidade de bytes ocupados para %rbx
    LoopOcupado:                        #Parte do código que imprime o sinal de ocupado ou livre n vezes
    mov $ocupado, %rdi
    call printf                         
    subq $1, %rbx                       #Subtrai %rbx em 1 até chegar em 0, para imprimir os n bytes
    cmpq $0, %rbx
    jg LoopOcupado                      #Enquanto %rbx for menor que 0 continua no loop
    movq ANDARILHO, %rax                #Restaura %rax
    movq -8(%rax), %rbx                 
    addq $16, %rbx
    addq %rbx, ANDARILHO                #desloca para o próximo nodo
    movq ANDARILHO,%rbx
    cmpq TOPO_ALOC, %rbx                #Confere se alcançou o fim da heap
    jl  Loop
    jmp Final

    Final:
    movq $str2,%rdi
    call printf
    popq %rbp
    ret



