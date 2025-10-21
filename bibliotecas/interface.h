#define preto 0
#define azul 1
#define verde 2
#define azul_claro 3
#define vermelho 4
#define roxo 5
#define ocre 6
#define cinza_claro 7
#define cinza 8
#define azul_medio 9
#define verde_claro 10
#define verde_agua 11
#define salmao 12
#define magenta 13
#define amarelo 14
#define branco 15

void moldura(int CI, int LI, int CF, int LF, int CorT, int CorF) {
	int i;
	textcolor(CorT);
	textbackground(CorF);
	gotoxy(CI, LI);
	printf("%c", 201);
	gotoxy(CF, LI);
	printf("%c", 187);
	gotoxy(CI, LF);
	printf("%c", 200);
	gotoxy(CF, LF);
	printf("%c", 188);
	for (i = CI + 1; i < CF; i++) {
		gotoxy(i, LI);
		printf("%c", 205);
		gotoxy(i, LF);
		printf("%c", 205);
	}
	for (i = LI + 1; i < LF; i++) {
		gotoxy(CI, i);
		printf("%c", 186);
		gotoxy(CF, i);
		printf("%c", 186);
	}
	textbackground(7);
	textcolor(0);
}

void fundo_quadro(int CI, int LI, int CF, int LF, int Cor) {
	int L = LI + 1, C = CI + 1;
	char linha[150] = "";
	while (CI < CF - 1) {
		strcat(linha, " ");
		CI++;
	}
	textbackground(Cor);
	while (L < LF) {
		gotoxy(C, L);
		printf("%s", linha);
		L++;
	}
}

void sombra(int CI, int LI, int CF, int LF, int CorF) {
	int i;
	textbackground(CorF);
	for (i = LI; i < LF; i++) {
		gotoxy(CF + 1, i + 1);
		printf("  ");
	}
	for (i = CI; i < CF + 2; i++) {
		gotoxy(i + 1, LF + 1);
		printf(" ");
	}
	textbackground(7);
}

void inserir_titulo(int CI, int CF, char *titulo, int Linha) {
	int pos = (CF + CI) / 2 - (strlen(titulo) + 2) / 2;
	if (strlen(titulo)) {
		textbackground(0);
		textcolor(14);
		gotoxy(pos, Linha);
		printf(" %s ", titulo);
		textcolor(0);
		textbackground(7);
	}
}

void retirar_cursor(void) {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void ligar_cursor(void) {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = TRUE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void dimensionar_prompt(char *C, char *L) {
	char linha[30];
	strcpy(linha, "mode con cols=");
	strcat(linha, C);
	strcat(linha, " lines=");
	strcat(linha, L);
	system(linha);
}

void moldura_completa(int CI, int LI, int CF, int LF, int CorT, int Cor) {
	fundo_quadro(CI, LI, CF, LF, Cor);
	moldura(CI, LI, CF, LF, CorT, Cor);
}

void limpar_linha(int Num) {
	gotoxy(2, Num);
	printf("                                                                                                                                ");
}

void limpar_tela(void) {
	int i;
	textbackground(verde_agua);
	for (i = 5; i < 31; i++)
		limpar_linha(i);
}

void linha_moldura(int Linha, int CorF, int CorT) {
	int i;
	textbackground(CorF);
	textbackground(CorT);
	gotoxy(1, Linha);
	printf("%c", 204);
	gotoxy(100, Linha);
	printf("%c", 185);
	for (i = 2; i < 100; i++) {
		gotoxy(i, Linha);
		printf("%c", 205);
	}
}

void coluna_moldura(int LI, int LF, int Coluna, int CorF, int CorT) {
	int i;
	textbackground(CorF);
	textbackground(CorT);
	gotoxy(Coluna, LI);
	printf("%c", 203);
	gotoxy(Coluna, LF);
	printf("%c", 202);
	for (i = LI + 1; i < LF; i++) {
		gotoxy(Coluna, i);
		printf("%c", 186);
	}
}

void quadro_completo(int CI, int LI, int CF, int LF, int CorT, int CorF) {
	moldura(CI, LI, CF, LF, CorT, CorF);
	fundo_quadro(CI, LI, CF, LF, CorF);
	sombra(CI, LI, CF, LF, 0);
}

void exibir_texto(int TamC, int Linha, int CorT, int CorF, char *texto, char *titulo) {
	int CI = TamC / 2 - (strlen(texto) / 2 + 2), CF = TamC / 2 + (strlen(texto) / 2 + 2), LI = Linha - 1, LF = Linha + 1;
	quadro_completo(CI, LI, CF, LF, CorT, CorF);
	inserir_titulo(CI, CF, titulo, LI);
	textcolor(CorT);
	textbackground(CorF);
	gotoxy(TamC / 2 - strlen(texto) / 2, LI + 1);
	printf("%s", texto);
}

void mensagem_final(void) {
	dimensionar_prompt((char*)"80", (char*)"30");
	moldura(1, 1, 80, 30, 0, 11);
	fundo_quadro(1, 1, 80, 30, 11);
	exibir_texto(80, 4, 0, amarelo, (char*)"TRABALHO BIMESTRAL - SISTEMAS OPERACIONAIS I", (char*)"");
	exibir_texto(80, 9, 0, amarelo, (char*)"CICLO DE VIDA DE UM PROCESSO", (char*)"");
	exibir_texto(80, 16, 0, cinza_claro, (char*)"102421080 - MATHEUS OLIVEIRA DA SILVA", (char*)"");
	exibir_texto(80, 26, 0, amarelo, (char*)"PROF. ME. ROBSON SISCOUTTO", (char*)"");
}

void limpar_msg() {
	textbackground(amarelo);
	gotoxy(6, 32);
	printf("                                                                                                                       ");
}

void msg(char* msg) {
	limpar_msg();
	gotoxy(6, 32);
	printf("%s", msg);
	textbackground(cinza_claro);
}

void iniciar_interface(void) {
    SetConsoleTitle("Sistemas Operacionais I - Ciclo de Vida de um Processo");
	retirar_cursor();
	dimensionar_prompt((char*)"130", (char*)"35");
	moldura_completa(1, 1, 130, 35, 0, verde_agua);
	exibir_texto(130, 2, 0, 14, (char*)"Simulador de Execucao Processos", (char*)"");
}

void iniciar_quadros(void) {
	limpar_tela();
	quadro_completo(4, 6, 54, 28, 0, cinza_claro);  // Quadro da fila de processos prontos
	inserir_titulo(4, 54, (char*)"Fila de Prontos - Prioridade", 6);
	quadro_completo(59, 6, 125, 17, 0, cinza_claro); // Quadro do processo em execução
	inserir_titulo(59, 125, (char*)"Processo em Execucao", 6);
	quadro_completo(59 ,20 ,125 ,28, 0, cinza_claro); // Quadro da fila de processos bloqueados
	inserir_titulo(59, 125, (char*)"Fila de Bloqueados", 20);
	quadro_completo(4, 31, 125, 33, 0, amarelo); // Quadro de mensagem
	inserir_titulo(4, 125, (char*)"Mensagem", 31);
	textcolor(preto);
	textbackground(cinza_claro);
	msg((char*)"Pressione qualquer tecla para interromper a simulacao!");
}
