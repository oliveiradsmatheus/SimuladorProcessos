int qtde_processos(Fila *f) {
	int i = 0;
	
	while (f) {
		i++;
		f = f->prox;
	}
	
	return i;
}

int qtde_bloqueados(Fila *f) {
	int i = 0;
	
	while (f) {
		if (f->PCB.tBloqueadoTotal)
			i++;
		f = f->prox;
	}
	
	return i;
}

float media_bloqueio(Fila *f) {
	float soma = 0, i = 0;
	
	while (f) {
		if (f->PCB.tBloqueadoTotal) {
			soma += (float)f->PCB.tBloqueadoTotal;
			i++;
		}
		f = f->prox;
	}
	
	if (i > 0)
		return soma/i;
	return 0;
}

int exec_sem_bloqueio(Fila *f) {
	int i = 0;
	
	while (f) {
		if (!f->PCB.tBloqueadoTotal)
			i++;
		f = f->prox;
	}
	
	return i;
}

void limpar_tela_processos(void) {
	int i, c = 10;
	for (i = 15; i < 25; i++) {
		gotoxy(c, i);
		printf("                                                                                                              ");
	}
}

void exibir_processos_terminados(Fila *f) {
	int linha = 16;
	
	textbackground(amarelo);
	while (f) {
		if(linha == 24) {
			msg((char*)"Pressione qualquer tecla para avancar");
			textbackground(amarelo);
			getch();
			limpar_tela_processos();
			linha = 16;
		}
		gotoxy(14, linha);
		printf("P%d - Tempo de execucao: %d", f->PCB.pid, f->PCB.tempoTotal + f->PCB.tBloqueadoTotal);
		gotoxy(46, linha);
		printf("[Bloq. rec.: %d]", f->PCB.tBloqueadoTotal - f->PCB.tBloqueadoFilho);
		gotoxy(69, linha);
		printf("Filhos criados: %d ", f->PCB.totalFilhos);
		gotoxy(93, linha);
		printf("Bloqueado pelo Filho: %d", f->PCB.tBloqueadoFilho);
		linha++;
		f = f->prox;
	}
}
