# 🧠 Simulador de Ciclo de Vida de Processos Linux

💻 **Disciplina:** Sistemas Operacionais I  
📚 **Curso:** Ciência da Computação — FIPP / UNOESTE  
👨‍💻 **Autor:** Matheus Oliveira da Silva  

---

## 📖 Sobre o Projeto

Este projeto implementa um **simulador do ciclo de vida de processos** inspirado no modelo de **processos do Linux**, com **interface visual em modo texto (Conio2)**.

O objetivo é **demonstrar o funcionamento interno do sistema operacional** no gerenciamento de processos — desde sua criação até a finalização — abordando **escalonamento, bloqueio, fork, preempção e estados de execução.**

---

## 🎯 Objetivos

- Compreender como o **sistema operacional Linux** gerencia processos.  
- Simular **transições de estados** de processos (Novo, Pronto, Executando, Bloqueado e Finalizado).  
- Implementar um **escalonador com prioridades** e filas dinâmicas.  
- Gerar **relatórios e estatísticas** de desempenho do sistema simulado.  
- Proporcionar uma **interface amigável** utilizando **Conio2**.

---

## ⚙️ Funcionalidades Principais

- 🔹 **Criação de processos (fork)** e herança de atributos do processo pai  
- 🔹 **Escalonamento por prioridade** com filas circulares (Round-Robin dentro de prioridades)  
- 🔹 **Bloqueio e desbloqueio** por diferentes causas: mouse, teclado, HD e espera por filhos  
- 🔹 **Gerenciamento de processos zumbis e filhos**  
- 🔹 **Preempção** quando há processo de maior prioridade  
- 🔹 **Relatórios automáticos** ao final da simulação com estatísticas detalhadas  
- 🔹 **Interface amigável** em modo texto (cores, menus, quadros e mensagens) usando **Conio2**

---

## 🧩 Estrutura de Dados Utilizadas

O simulador faz uso de **estruturas dinâmicas** que representam as filas de processos nos diferentes estados:

| Estrutura | Tipo | Descrição |
|------------|------|-----------|
| `FilaC` | Fila Circular | Representa os processos **prontos**, organizados por **níveis de prioridade (0–9)** |
| `Fila` | Fila Linear | Representa os processos **bloqueados** por tipo de evento |
| `Processo` | Struct | Estrutura que guarda o **PCB (Process Control Block)** de cada processo |
| `TFP` | Constante (10) | Quantidade de níveis de prioridade |
| `TFB` | Constante (4) | Tipos de bloqueio (Mouse, Teclado, HD, Filho) |

---

## 🔄 Ciclo de Vida Simulado

O programa reproduz as principais transições de estado de um processo:


Durante a execução:
- O **escalonador** seleciona o processo de **maior prioridade** disponível.  
- O processo pode ser **bloqueado** por eventos ou **preemptado** se surgir um processo mais prioritário.  
- Pode ocorrer um **fork**, criando um processo filho.  
- Quando termina, o processo é movido para a lista de **terminados**, e o tempo total de execução é registrado.

---

## 🧠 Pseudocódigo do Escalonador

A seguir está o funcionamento básico da função principal da simulação:

```text
função simular():
    enquanto (existirem processos não finalizados):
        para cada nível de prioridade de 0 a 9:
            se houver processo pronto nessa prioridade:
                processo ← próximo da fila
                executar_processo(processo)
                se processo terminou:
                    mover para fila de finalizados
                senão se bloqueou:
                    mover para fila de bloqueados
                fim-se
            fim-se
        fim-para
        atualizar estados de bloqueados
    fim-enquanto

função executar_processo(processo):
    mostrar informações na interface
    tempo -= 1
    se evento aleatório ocorrer:
        processo entra em bloqueio (mouse, teclado, HD ou filho)
    se processo for filho e pai ainda ativo:
        pai entra em bloqueio até término do filho
