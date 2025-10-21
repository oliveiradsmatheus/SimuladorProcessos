#define TFP 10 // Prioridades

struct FilaC {
	Processo PCB;
	FilaC *prox;
};

FilaC *inicializar_fila_c (void) {
	return NULL;
}

void inicializar_filas_c (FilaC *f[TFP]) {
	for (int i = 0; i < TFP; i++)
		f[i] = inicializar_fila_c();
}

char fila_c_vazia (FilaC *f) {
	return f == NULL;
}

FilaC *novo_processo_fila_c (Processo p) {
	FilaC *nova = new FilaC;
	nova->PCB = p;
	nova->prox = NULL;
	return nova;
}

FilaC *inserir_fila_c (FilaC *f, Processo p) {
	FilaC *nova = novo_processo_fila_c(p), *aux;
	if (!f)
		f = nova;
	else {
		aux = f;
		while (aux->prox != f)
			aux = aux->prox;
		aux->prox = nova;
	}
	nova->prox = f;
	return f;
}

FilaC *retirar_fila_c (FilaC *f, Processo &p) {
	FilaC *aux = f;
	if (f->prox == f) {
		p = f->PCB;
		delete f;
		return NULL;
	} else {
		while (aux->prox != f)
			aux = aux->prox;
		aux->prox = f->prox;
		aux = f;
		f = aux->prox;
		p = aux->PCB;
		delete aux;
		return f;
	}
}

void exibir_fila_c (FilaC *f, int linha) {
	int c;
	FilaC *inicio = f;
	c = 6;
	gotoxy(c, 8 + linha*2);
	printf("[%d]:                                            ", linha);
	c += 6;
	if (f) {
		gotoxy(c, 8 + linha*2);
		printf("P%d ", f->PCB.pid);
		c += 5;
		f = f->prox;
	}
	while (f != inicio) {
		gotoxy(c, 8 + linha*2);
		printf("P%d ", f->PCB.pid, f->PCB.tBloqueadoFilho);
		c += 5;
		f = f->prox;
	}
}

void exibir_filas_c (FilaC *f[TFB]) {
	textbackground(cinza_claro);
	textcolor(preto);
	for (int i = 0; i < TFP; i++)
		exibir_fila_c(f[i], i);
}
