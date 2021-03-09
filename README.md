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

# Implementação  
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

# Execução
A fim de executar o programa, primeiramente deve-se compilá-lo com o comando `make`. Uma vez que o arquivo executável tenha sido criado, esperam-se como parâmetros os seguintes:  
`tp2virtual [algoritmo] arquivo.log 4 128`  
Onde:
- tp2virtual: é o nome do arquivo executável que foi gerado após o `make`;
- [algoritmo]: especifica qual algoritmo de substituição de páginas deverá ser utilizado;
  - First In First Out: "fifo";
  - Least Recently Used: "lru";
  - Segunda Chance: "2a";
  - Random: "rand";
- arquivo.log: deve ser o nome do arquivo do tipo .log que contém a sequência de endereços de memória acessados, assim como o tipo de acesso (leitura ou escrita) de cada endereço;
- Esse parâmetro define o tamanho de uma página, podendo ser de 2 a 64 (deve ser potência de 2);
- Esse parâmetro define o tamanho total da memória física disponível para o processo, podendo ser de 128 a 16384 (deve ser potência de 2);

## Passo-a-passo do código
Na função **main**, primeiramente os parâmetros fornecidos pelo usuário são armazenados em variáveis, e o tamanho total da tabela de páginas é calculado ao se dividir o tamanho total da memória física disponível pelo tamanho de uma página. Além disso, é calculado também quantos bits menos significativos devem ser descartados para identificar a página associada a um endereço.
Após essas definições, inicia-se a tabela de páginas, e também é criada uma página auxiliar. Com isso, o arquivo .log é aberto e itera-se por cada uma de suas linhas. Para cada linha, o endereço da página é salvo na página auxiliar que foi criada, e a letra "W" ou "R" que é lida do arquivo define se a tabela será atualizada com base em um comando de leitura ou de escrita - essa atualização é responsabilidade da função `updateMemory`, que será descrita abaixo.
Por fim, depois da passagem por todo o arquivo, este é fechado e mostra-se ao usuário o tempo gasto para executar o programa, assim como o **total de page faults (Páginas lidas)** e o **total de dirty pages (Páginas escritas)** que tiveram que ser propagadas para o disco, e o programa se encerra.

Dentro do arquivo **table.c**, está definida a função **updateMemory**, a qual é responsável pela atualização e manutenção da tabela de páginas (essa função é chamada na main em cada iteração pelo arquivo). Nela, além de ser  feito o controle do tempo atual de execução (contador clock), é feita a inserção/substituição de páginas na tabela.
Primeiramente, executa-se a função `alreadyExists`, a qual informa se a página em questão já está inserida na tabela, assim qual é a posição na tabela que a nova página deve ser inserida (varia de acordo com o algoritmo de substituição selecionado). A partir do retorno, existem três casos possíveis:
- A página não existe na tabela, e a tabela não está cheia
  - Nesse caso, a página é inserida na próxima posição livre da tabela
  - O contador de page faults é acrescido de 1
  - A ocupação atual da tabela é acrescida de 1
- A página não existe na tabela, e a tabela está cheia
  - Nesse caso, a página que está na posição retornada pela função `alreadyExists` é substituída pela nova página
  - O contador de page faults é acrescido de 1
  - Se a página retirada estiver "suja" (isDirty ser true) o contador de dirty pages é acrescido de 1
- A página já existe na tabela
  - Nesse caso, não ocorre substituição. O bit de referência é atualizado para 1
E, em todo caso, se for uma operação de escrita a página é definida como "suja" (dirtyBit é setado como 1).
