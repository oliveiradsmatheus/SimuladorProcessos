#define TFB 4 // Tipos de bloqueio, por exemplo: (mouse, teclado, wait)

struct Fila {
	Processo PCB;
	Fila *prox;
};

Fila *inicializar_fila() {
	return NULL;
}

void inicializar_filas (Fila *f[TFB]) {
	for (int i = 0; i < TFB; i++)
		f[i] = inicializar_fila();
}

char fila_vazia (Fila *f) {
	return f == NULL;
}

Fila *novo_processo (Processo p) {
	Fila *nova = new Fila;
	nova->PCB = p;
	nova->prox = NULL;
	return nova;
}

Fila *inserir_ordem (Fila *f, Processo p) {
	Fila *nova = novo_processo(p), *aux;

	if (!f)
		f = nova;
	else {
		if (p.pid < f->PCB.pid) {
			nova->prox = f;
			f = nova;
		} else {
			aux = f;
			while (aux->prox && p.pid > aux->prox->PCB.pid)
				aux = aux->prox;
			nova->prox = aux->prox;
			aux->prox = nova;
		}
	}
	return f;
}

Fila *inserir_fila (Fila *f, Processo p) {
	Fila *nova = novo_processo(p), *aux;

	if (!f)
		f = nova;
	else {
		aux = f;
		while (aux->prox)
			aux = aux->prox;
		aux->prox = nova;
	}
	return f;
}

Fila *retirar_fila (Fila *f, Processo &p) {
	Fila *aux;
	
	if (f) {
		aux = f;
		p = f->PCB;
		f = f->prox;
		delete aux;
	}
	return f;
}

void exibir_fila (Fila *f, int linha) {
	int c = 61;
	gotoxy(c, 21 + linha*2);
	switch(linha) {
		case 0:
			printf("[Mouse]:                                                        ", linha + 1);
			c += 9;
			break;
		case 1:
			printf("[Teclado]:                                                      ", linha + 1);
			c += 11;
			break;
		case 2:
			printf("[HD]:                                                           ", linha + 1);
			c += 6;
			break;
		case 3:
			printf("[Filho]:                                                        ", linha + 1);
			c += 9;
			break;
	}
	while (f) {
		gotoxy(c, 21 + linha*2);
		printf("P%d ", f->PCB.pid, f->PCB.qtdeFilhos, f->PCB.tBloqueadoFilho);
		c += 5;
		f = f->prox;
	}
}

void exibir_filas (Fila *f[TFB]) {
	textbackground(cinza_claro);
	textcolor(preto);
	
	for (int i = 0; i < TFB; i++)
		exibir_fila(f[i], i);
}
