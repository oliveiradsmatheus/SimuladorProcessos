#include <stdio.h>
#include <string.h>
#include <conio2.h>
#include <windows.h>
#include <unistd.h>
#include <time.h>

#include "bibliotecas\\interface.h"
#include "bibliotecas\\processo.h"
#include "bibliotecas\\fila.h"
#include "bibliotecas\\fila_circular.h"
#include "bibliotecas\\relatorio.h"

void fork (FilaC *prontos[TFP], Fila *bloqueados[TFB], Processo &pai) {
	Processo novo;

	novo = iniciar_processo();

	pai.cpid = novo.pid;
	novo.ppid = pai.pid;

	novo.tempoCPU = novo.tempoRestante = pai.tempoRestante;

	novo.prioridade = pai.prioridade;
	pai.qtdeFilhos++;
	pai.totalFilhos++;
	pai.estado = espera;

	prontos[novo.prioridade] = inserir_fila_c(prontos[novo.prioridade], novo);
}

void inserir_processos(FilaC *f[TFP]) {
	Processo p;
	char resp;

	do {
		p = iniciar_processo();

		quadro_completo(30, 8, 100, 16, 0, amarelo);
		inserir_titulo(30, 100, (char*)"Inserir Processo", 8);
		textbackground(amarelo);

		gotoxy(34, 10);
		printf("Informe o tempo do processo [0 - 100]: ");
		scanf("%d", &p.tempoCPU);
		while (p.tempoCPU < 1 || p.tempoCPU > 100) {
			msg((char*)"Informe um tempo valido");
			getch();
			limpar_msg();
			textbackground(amarelo);
			gotoxy(73, 10);
			printf("                           ");
			gotoxy(73, 10);
			scanf("%d", &p.tempoCPU);
		}
		p.tempoRestante = p.tempoCPU;

		gotoxy(34, 12);
		printf("Informe a prioridade do processo [0 - 9]: ");
		scanf("%d", &p.prioridade);
		while (p.prioridade < 0 || p.prioridade > 10) {
			msg((char*)"Informe uma prioridade valida");
			getch();
			limpar_msg();
			textbackground(amarelo);
			gotoxy(76, 12);
			printf("                        ");
			gotoxy(76, 12);
			scanf("%d", &p.tempoCPU);
		}

		f[p.prioridade] = inserir_fila_c(f[p.prioridade], p);

		gotoxy(34, 14);
		printf("Deseja inserir outro processo? [S] Sim / [N] Nao");
		resp = toupper(getch());
		while (resp != 'S' && resp != 'N')
			resp = toupper(getch());
	} while (resp != 'N');
}

void escalonar_processo(FilaC *prontos[TFP], Processo &exec) {
	int i = 0;
	char escalonado = 0;

	while (i < TFP && !escalonado) {
		if (!fila_c_vazia(prontos[i])) {
			prontos[i] = retirar_fila_c(prontos[i], exec);
			escalonado = 1;
		}
		i++;
	}
	if (!escalonado) {
		exec.pid = -1;
		exec.tempoRestante = 0;
	} else
		exec.estado = executando;
}

Fila *gerenciar_processo_bloqueado(FilaC *prontos[TFP], Fila *f) {
	Processo p;
	Fila *aux = inicializar_fila();
	
	while (!fila_vazia(f)) {
		f = retirar_fila(f, p);
		p.tempoBloqueado--;
		p.tBloqueadoTotal++;
		if (!p.tempoBloqueado)
			prontos[p.prioridade] = inserir_fila_c(prontos[p.prioridade], p);
		else
			aux = inserir_fila(aux, p);
	}
	
	return aux;
}

Fila *gerenciar_processo_zumbi(Fila *f) {
	Fila *aux = inicializar_fila();
	Processo p;
	
	while (!fila_vazia(f)) {
		f = retirar_fila(f, p);
		p.tBloqueadoFilho++;
		p.tBloqueadoTotal++;
		aux = inserir_fila(aux, p);
	}
	
	return aux;
}

void gerenciar_processos_bloqueados(FilaC *prontos[TFP], Fila *bloqueados[TFB]) {
	int i;

	for (i = 0; i < TFB - 1; i++)
		if (!fila_vazia(bloqueados[i]))
			bloqueados[i] = gerenciar_processo_bloqueado(prontos, bloqueados[i]);
	if (!fila_vazia(bloqueados[3]))
		bloqueados[3] = gerenciar_processo_zumbi(bloqueados[3]);
}

void atualizar_tabelas(FilaC *prontos[TFP], Fila *bloqueados[TFB], Processo exec, int q, int tempo) {
	if (exec.pid == -1)
		exibir_processo_vazio(tempo);
	else
		exibir_processo(exec, q, tempo);
	exibir_filas_c(prontos);
	exibir_filas(bloqueados);
}

void bloquear_processo(Fila *bloqueados[TFB], Processo &exec, int prioridade) {
	exec.estado = espera;
	exec.tempoBloqueado = rand() % 10 + 1;
	bloqueados[prioridade] = inserir_fila(bloqueados[prioridade], exec);
	exec.pid = -1;
	exec.tempoRestante = 0;
}

void preemptar(FilaC *prontos[TFP], Processo exec) {
	exec.estado = pronto;
	prontos[exec.prioridade] = inserir_fila_c(prontos[exec.prioridade], exec);
}

Fila *verificar_permissao_morte(Fila *bloqueados[TFB], Fila *terminados, Processo &proc, char &matar_processo) {
	Fila *nova = inicializar_fila();
	Processo p;

	while (!fila_vazia(bloqueados[3])) {
		bloqueados[3] = retirar_fila(bloqueados[3], p);
		if (!p.qtdeFilhos) {
			proc = p;
			matar_processo = 1;
		} else
			nova = inserir_fila(nova, p);
	}
	bloqueados[3] = nova;
	
	return terminados;
}

FilaC *decrementar_filho_prontos(FilaC *f, Processo &exc, char &decrementado) {
	FilaC *aux = inicializar_fila_c();
	Processo p;
	
	while (!fila_c_vazia(f)) {
		f = retirar_fila_c(f, p);
		if (p.pid == exc.ppid){
			if (p.qtdeFilhos > 0) {
				p.qtdeFilhos--;
				exc = p;
			}
			decrementado = 1;
		}
		aux = inserir_fila_c(aux, p);
	}
	
	return aux;
}

Fila *decrementar_filho_bloqueados(Fila *f, Processo &exc, char &decrementado) {
	Fila *aux = inicializar_fila();
	Processo p;
	
	while (!fila_vazia(f)) {
		f = retirar_fila(f, p);
		if (p.pid == exc.ppid){
			if (p.qtdeFilhos > 0) {
				p.qtdeFilhos--;
				exc = p;
			}
			decrementado = 1;
		}
		aux = inserir_fila(aux, p);
	}
	
	return aux;
}

Fila *finalizar_processo(FilaC *prontos[TFP], Fila *bloqueados[TFB], Fila *terminados, Processo &p) {
	int i;
	char decrementado, matar_processo = 1;
	
	while (matar_processo) {
		matar_processo = decrementado = 0;
		
		terminados = inserir_ordem(terminados, p);
		if (p.ppid > 0) {
			prontos[p.prioridade] = decrementar_filho_prontos(prontos[p.prioridade], p, decrementado);
			i = 0;
			while (i < TFB && !decrementado) {
				bloqueados[i] = decrementar_filho_bloqueados(bloqueados[i], p, decrementado);
				i++;
			}
		}
		if (decrementado)
			terminados = verificar_permissao_morte(bloqueados, terminados, p, matar_processo);
	}
	
	p.pid = -1;
	p.tempoRestante = 0;
	
	return terminados;
}

void executar_processo(FilaC *prontos[TFP], Fila *bloqueados[TFB], Processo &exec, int &q) {
	int aleat = rand() % 20 + 1;
	
	if (exec.pid != -1) {
		exec.tempoRestante--;
		exec.tempoTotal++;
		q++;
		
		if (exec.tempoRestante > 0)
			switch (aleat) {
				case 1: case 2: case 3:
					bloquear_processo(bloqueados, exec, aleat - 1);
					break;
				case 4: case 5: case 6:
					fork(prontos, bloqueados, exec);
					break;
			}
	}
}

void menu (FilaC *prontos[TFP], char &sair) {
	char op;

	do {
		quadro_completo(30, 8, 100, 16, 0, amarelo);
		inserir_titulo(30, 100, (char*)"Menu", 8);
		textbackground(amarelo);
		gotoxy(34, 10);
		printf("[A] - Inserir processos");
		gotoxy(34, 12);
		printf("[BACKSPACE] - Retornar a simulacao");
		gotoxy(34, 14);
		printf("[ESC] - Finalizar simulacao");
		
		op = toupper(getch());

		switch (op) {
			case 'A':
				inserir_processos(prontos);
				break;
			case 27:
				sair = 1;
				op = 8;
		}

	} while (op != 8);
}

char processo_maior_prioridade(FilaC *prontos[TFP], Processo exec) {
	int i = 0;
	while(i < exec.prioridade && fila_c_vazia(prontos[i]))
		i++;
	if (i == exec.prioridade)
		return 0;
	return 1;
}

Fila *simular(FilaC *prontos[TFP], Fila *bloqueados[TFB], Fila *terminados, int &tempo, int& bloqueados_fork, int &bloqueados_evento) {
	int q = 1;
	char sair = 0;
	Processo exec;

	escalonar_processo(prontos, exec);
	do {
		iniciar_quadros();
		if (exec.pid == -1 || processo_maior_prioridade(prontos, exec)) {
			if (exec.pid > 0)
				preemptar(prontos, exec);
			escalonar_processo(prontos, exec);
		}
		while (!_kbhit()) {
			atualizar_tabelas(prontos, bloqueados, exec, q, tempo);
			
			Sleep(1000);

			gerenciar_processos_bloqueados(prontos, bloqueados);
			executar_processo(prontos, bloqueados, exec, q);

			if (q == quantum || !exec.tempoRestante || exec.pid == -1 || processo_maior_prioridade(prontos, exec)) {
				q = 1;
				if (exec.pid > 0) {
					if (!exec.tempoRestante)
						if (!exec.qtdeFilhos)
							terminados = finalizar_processo(prontos, bloqueados, terminados, exec);
						else
							bloquear_processo(bloqueados, exec, 3);
					else
						preemptar(prontos, exec);
				}
				escalonar_processo(prontos, exec);
			}
			tempo++;
		}
		getch();
		menu(prontos, sair);
	} while(!sair);
	
	msg((char*)"Simulacao encerrada - Pressione qualquer tecla para avancar para a pagina de relatorios!");
	getch();
	
	return terminados;
}

void relatorios(FilaC *prontos[TFP], Fila *bloqueados[TFB], Fila *terminados, int tempo, int& bloqueados_fork, int &bloqueados_evento) {
	int processos_terminados = qtde_processos(terminados);
	limpar_tela();

	quadro_completo(4, 6, 125, 28, 0, cinza_claro);
	inserir_titulo(4, 125, (char*)"Relatorios", 6);
	gotoxy(10, 8);
	printf("Quantidade de processos finalizados: %d", processos_terminados);
	gotoxy(70, 8);
	printf("Tempo total da simulacao: %d un.", tempo);
	gotoxy(10, 10);
	printf("Total de Processos que foram bloqueados: %d", qtde_bloqueados(terminados));
	gotoxy(70, 10);
	printf("Tempo medio de bloqueio: %.2f un.", media_bloqueio(terminados));
	gotoxy(10, 12);
	printf("Quantidade de processos executados sem bloqueio: %d", exec_sem_bloqueio(terminados));
	quadro_completo(9, 14, 120, 25, preto, amarelo);
	inserir_titulo(9, 120, (char*)"Processos finalizados", 14);
	exibir_processos_terminados(terminados);

	msg((char*)"Pressione qualquer tecla para sair");
	getch();
}

void executar(void) {
	FilaC *prontos[TFP];
	Fila *bloqueados[TFB];
	Fila *terminados = inicializar_fila();
	int bloqueados_fork = 0, bloqueados_evento, tempo = 0;
	
	
	iniciar_interface();
	inicializar_filas_c(prontos);
	inicializar_filas(bloqueados);
	
	
	quadro_completo(4, 31, 125, 33, 0, amarelo); // Quadro de mensagem
	inserir_titulo(4, 125, (char*)"Mensagem", 31);
	inserir_processos(prontos);

	srand(time(NULL));

	terminados = simular(prontos, bloqueados, terminados, tempo, bloqueados_fork, bloqueados_evento);
	relatorios(prontos, bloqueados, terminados, tempo, bloqueados_fork, bloqueados_evento);
	
	mensagem_final();
	getch();
}

int main (void) {
	executar();

	return 0;
}
