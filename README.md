# Simulador de Tabela de Memória  
Este repositório contém o código relativo ao trabalho prático do curso de Sistemas Operacionais ofertado pelo Departamento de Ciência da Computação. O trabalho consiste na implementação de uma memória virtual, assim como alguns algoritmos de substituição de páginas nessa memória. A proposta do trabalho é descrita na próxima seção. Em seguida, apresentamos as decisões que tomamos durante a implementação para resolver o problema proposto. Por fim, apresentamos e destacamos as estruturas importantes da implementação do código, assim como fazemos uma análise de cada um dos algoritmos implementados.  

# O Problema  
O problema envolve a implementação de uma memória virtual, na forma de uma tabela de quadros de memória, a qual armazena um nũmero limitado de endereços (páginas). Isso visa simular uma funcionalidade comum de sistemas operacionais, que é armazenar endereços nessa tabela de páginas de forma que não é necessário o acesso direto ao disco toda vez que um programa deseje ler ou escrever alguma informação, visto que esse acesso é bastante custoso.  
Além das estruturas da tabela de páginas e de uma página em si, também vê-se necessária a implementação de maneiras de gerir essa memória virtual, mais especificamente dos algoritmos de substituição de páginas. Esses algoritmos definem, a partir de algum critério, qual página deve ser removida da memória para a entrada de uma nova página (caso a memória virtual esteja cheia).  
Na entrada do problema, são especificados:  
- Qual algoritmo será utilizado (FIFO, LRU, 2a ou Random);
- Arquivo .log que contém a sequência de endereços de memória acessados;
- O tamanho de cada uma das páginas (de 2KB a 64KB);
- O tamanho total da memória física disponível (de 128KB a 16384KB);

# Escolhas de implementação  
A fim de implementar a memória virtual e as páginas, definimos as seguintes estruturas para cada uma:  
- Uma **Página** contém as seguintes informações:
  - Endereço (address): responsável por armazenar o endereço daquela página na memória;
  - Bit de referência (nxtVictim): esse campo pode ter valor 0 ou 1, e indica se a página em questão for referenciada (tanto em escrita quanto em leitura) mais de uma vez. É utilizado pelo algoritmo de substituição 2a;
  - Tempo desde último acesso (timeSinceAcc): armazena em qual momento da execução do programa aquela página foi referenciada pela última vez (escrita ou leitura). É utilizado pelo algoritmo de substituição LRU;
  - Bit de página suja (isDirty): esse bit indica se a página está "suja", ou seja, se o conteúdo dela sofreu alguma modificação. Ou seja, sua função é indicar se, quando a página for retirada da tabela, é necessário propagar as alterações ao disco;

- A **Tabela** contém as seguintes informações:
  - Páginas (pages): um arranjo que contém todas as páginas da tabela;
  - Última página usada (luIndex): esse número inteiro indica em qual posição do arranjo de páginas está aquela que foi a primeira a entrar na tabela;
  - Tamanho (size): armazena o tamanho máximo da tabela (quantas páginas ela consegue armazenar);
  - Ocupação atual (currOccupancy): diferentemente do tamanho, esse campo armazena quantos espaços da tabela estão ocupados até então. Seu valor máximo é igual ao valor de Tamanho;
  - Contador de tempo (clock): esse campo é responsável por armazenar o "tempo" atual da execução do programa. Será melhor descrito a seguir;
  - Page Faults (pageFaults): contador de page faults que ocorreram (quando uma página não é encontrada na tabela de deve ser buscada no disco);
  - Páginas Sujas (dirtyPages): contador de páginas sujas que necessitaram ser propagadas para o disco;

Além das estruturas acima, é relevante ressaltar que optamos por contar o tempo de execução do programa (clock, na estrutura da Tabela) utilizando um contador, que é acrescido de 1 a cada nova iteração sob o arquivo de entrada.
