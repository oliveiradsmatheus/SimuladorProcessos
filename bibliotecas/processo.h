#define pronto 'p'
#define espera 'e'
#define executando 'x'
#define quantum 5

static pid_t num_pid = 1; // Variável estática apenas para criar o número do processo.

struct Processo
{
	int qtdeFilhos, totalFilhos, tempoRestante, tempoCPU, tempoBloqueado, tempoTotal, prioridade, tBloqueadoFilho, tBloqueadoTotal;
	pid_t pid, ppid, cpid;
	char estado; // P = pronto, E = espera, X = executando
};

Processo iniciar_processo(void) {
	Processo p;

	p.pid = num_pid++;
	p.ppid = 0;
	p.cpid = 0;
	
	p.qtdeFilhos = p.totalFilhos = 0;
	
	p.tempoRestante = 0;
	p.tempoCPU = 0;
	p.tempoBloqueado = 0;
	p.tempoTotal = 0;
	
	p.tBloqueadoFilho = 0;
	p.tBloqueadoTotal = 0;
	
	p.prioridade = 0;
	p.estado = pronto;

	return p;
}

void exibir_processo_vazio(int tempo) {
	gotoxy(65, 8);
	printf("Processo em execucao: Nenhum                                ");
	gotoxy(65, 10);
	printf("Tempo Restante: 0                                           ");
	gotoxy(65, 12);
	printf("Prioridade: Nenhuma          ");
	gotoxy(65, 14);
	printf("Fatia de tempo: Nenhuma                ");
	gotoxy(65, 16);
	printf("Tempo total: %d               ", tempo);
}

void exibir_processo(Processo exec, int q, int tempo) {
	gotoxy(65, 8);
	printf("Processo em execucao: P%d                ", exec.pid);
	gotoxy(105, 8);
	printf("Pai: P%d             ", exec.ppid);
	gotoxy(65, 10);
	printf("Tempo Restante: %d                  ", exec.tempoRestante);
	gotoxy(105, 10);
	printf("Filhos: %d           ", exec.qtdeFilhos);
	gotoxy(65, 12);
	printf("Prioridade: %d          ", exec.prioridade);
	gotoxy(65, 14);
	printf("Fatia de tempo: %d                ", q);
	gotoxy(65, 16);
	printf("Tempo total: %d               ", tempo);
}
