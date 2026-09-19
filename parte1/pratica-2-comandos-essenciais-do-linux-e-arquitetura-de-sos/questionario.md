# Questionário

1. Monte uma tabela com os benefícios e deficiências mais relevantes das principais arquiteturas de sistemas operacionais

De acordo com o livro texto da disciplina:

| Arquitetura | Benefícios | Deficiências |
|---|---|---|
| **Sistemas monolíticos** | Desempenho devido à comunicação direta entre componentes do núcleo | Problemas de robustez do sistema, um erro pode propagar para todo o núcleo; Problemas com desenvolvimento: os componentes podem ser interdependentes, dificulta manutenção/evolução |
| **Sistemas micronúcleo** | Modularidade, serviços independentes; flexibilidade, serviços podem ser carregados/desativados individualmente; robustez, caso um falhe, só ele falha | Como todo aspecto de alto nível é implementado fora do núcleo, a comunicação por mensagem pode gerar um problema de desempenho, pois cada chamada de sistema tem custo elevado |
| **Sistemas em camadas** | Mais organizada, os níveis tem privilégios diferentes,  | Pode ter problemas de desempenho, devido ao empilhamento das camadas; A separação das funções nessas camadas às vezes não é tão simples, tem funcionalidades que podem estar ou devem estar em várias delas (ex: gestão de entrada/saida) |
| **Sistemas híbridos** | Traz de volta ao núcleo componentes mais críticos, ainda mantém certa modularidade (micronucleo/camadas) mas garante mais desempenho (monolito) | Aumenta a complexidade, não é tão claro a definição dos componentes como é em camadas/micronúcleo |
| **Máquinas virtuais (avançado)** | Isolamento, permite execução de código independente de plataforma, baixo custo | Sobrecarga do sistema físico que comporta essas máquinas virtuais |
| **Contêineres (avançado)** | Benefícios parecidos com os da máquina virtual, isolamento, aproveita bem o hardware porque separa em domínios (espaço de usuário) | Sobrecarga do sistema físico também, complexidade a mais em termos de gerência |
| **Sistemas exonúcleo** | Acesso controlado aos recursos do hardware, cada aplicação implementa o que for mais adequado, isso pode favorecer o desempenho da aplicação específica | Não há abstração, os componentes fora do núcleo devem aplicar tudo (LibOs), sobrecarga de desempenho para o sistema (chamadas de sistema para o núcleo), complexidade |
| **Sistemas uninúcleo (unikernel)** | O núcleo, bibliotecas e aplicação compilados e ligados enre si, aumenta muito desempenho. Só entra no código final, os componentes necessários, sistema compacto  | Qualquer mudança é necessário recompilação. A separação entre os componentes não existe, então não pode ser usada em sistemas que precisam de aplicações independentes |


2. Sobre as afirmações a seguir, relativas às diversas arquiteturas de sistemas operacionais,
indique quais são incorretas, justificando sua resposta:
- (a) Uma máquina virtual de sistema é construída para suportar uma aplicação escrita em
uma linguagem de programação específica, como Java.

- (b) Um hipervisor convidado executa sobre um sistema operacional hospedeiro.

- (c) Em um sistema operacional micronúcleo, os diversos componentes do sistema são construídos como módulos interconectados executando dentro do núcleo.

- (d) Núcleos monolíticos são muito utilizados devido à sua robustez e facilidade de manutenção.

- (e) Em um sistema operacional micronúcleo, as chamadas de sistema são implementadas
através de trocas de mensagens.

3. Pesquisa que tipo de arquitetura é mais apropriada para os seguintes sistemas (justifique o
porque):

- (a) Sistemas Críticos (ex: Aeronáutica)

- (b) Servidores

- (c) Sistemas Distribuídos

- (d) Sistemas Embarcados

- (e) Sistemas de Tempo Real

- (f) Sistemas Móveis