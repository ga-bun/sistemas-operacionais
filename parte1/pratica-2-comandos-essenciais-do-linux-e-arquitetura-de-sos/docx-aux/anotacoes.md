useradd

-d pro home-dir
-g pra adicionar no grupo (o grupo tem que existir) -> esse é o grupo primario

-G -> grupo secundario

-s pra setar o path do user login shell


https://pt.console-linux.com/?p=20030
https://linuxize.com/post/linux-chown-command/
https://linuxize.com/post/chgrp-command-in-linux/

| **r** | **w** | **x** | **octal** | Permissão |
| --- | --- | --- | --- | --- |
| 0 | 0 | 0 | 0 | Nenhuma permissão |
| 0 | 0 | 1 | 1 | Execução |
| 0 | 1 | 0 | 2 | Escrita |
| 0 | 1 | 1 | 3 | Escrita e execução |
| 1 | 0 | 0 | 4 | Leitura |
| 1 | 0 | 1 | 5 | Leitura e execução |
| 1 | 1 | 0 | 6 | Leitura e escrita |
| 1 | 1 | 1 | 7 | Leitura, escrita e execução |

Coloque no formato numérico: -rwx r-x r - -
754
• Coloque no formato numérico: -rwx r-x - - x
751
• Coloque no formato numérico: -r-x rwx r - x
575

Permissões: [ugoa] [+-=] [rwx]

- ugoa                        dono/user (u), grupo (g), outros (o), todos (a)
- +-=                          adiciona (+), retira (-), define (=) a permissão
- rwx                           permissão a ser alterada


• Coloque no formato simbólico: Remover o direito de execução do arquivo em nivel de proprietário
u-x
• Coloque no formato simbólico: remover a permissão de leitura para outros
o-r
• Coloque no formato simbólico: Defina a permissão como total para todos
a+rwx


