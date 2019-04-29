// Biblioteca Consola para TP de POO
// �ltima alteracao: 17/18 - Nov 2018
// J

/*
Versoes e historial
Dez 2010 - Implementa��o: Funcoes da consola + exemplo + coment�rios
Nov 2013 - Pequenas correc��es
Dez 2015 - Coment�rios melhorados
Dez 2016 - Exemplo melhorado.
         - Compatibilidade com mingw automatizada
         - As fun��es passaram a ser est�ticas
Nov 2018 - Pequenos melhoramentos nos coment�rios
*/



#ifndef __CONSOLA_H__
#define __CONSOLA_H__

#include <windows.h>

class Consola {

	static HANDLE hconsola;
	static HANDLE hStdin;
	static HWND hwnd;

public:

	// para usar nas cores
	const static int PRETO = 0;
	const static int AZUL = 1;
	const static int VERDE = 2;
	const static int CYAN = 3;
	const static int VERMELHO = 4;
	const static int ROXO = 5;
	const static int AMARELO = 6;
	const static int BRANCO = 7;
	const static int CINZENTO = 8;
	const static int AZUL_CLARO = 9;
	const static int VERDE_CLARO = 10;
	const static int CYAN_CLARO = 11;
	const static int VERMELHO_CLARO = 12;
	const static int COR_DE_ROSA = 13;
	const static int AMARELO_CLARO = 14;
	const static int BRANCO_CLARO = 15;

	// para usar em getch
	const static char ESQUERDA = 1;
	const static char DIREITA = 2;
	const static char CIMA = 3;
	const static char BAIXO = 4;
	const static char ENTER = 13;
	const static char ESCAPE = 27;

	Consola() = delete;  // Nao quero objectos disto a proliferarem por ai.
	                     // As func��e s�o est�ticas por alguma raz�o.
	                     // ----------> C++ 2011 ou posterior

	// Posiciona o cursor na posi��o x,y
	// - Os proximos cout/cin ser�o feitos a partir da�
	static void gotoxy(int x, int y);

	// Limpa o ecr�
	// - Usa a c�r de fundo que estiver definida
	static void clrscr();

	// Muda a c�r das letras
	// - Os cout/cin seguintes usar�o essa c�r
	static void setTextColor(WORD color);

	// Muda a c�r de fundo
	// - Os printf/cout seguintes usar�o essa c�r
	// - Os clrsrc() seguintes usar�o essa c�r de fundo
	static void setBackgroundColor(WORD color);

	// Muda a dimens�o do ecr� para NLinhas x NCols
	// - O redimensionamento pode falhar se o tamanho
	//   indicado for excessivo ou se for demasiado
	//   pequeno
	// - Limpa o ecr� usando a c�r que estiver definida?
	static void setScreenSize(int nLinhas, int nCols);

	// L� um caracter sem precisar de "enter" no fim
	// - Util para fazer pausas do tipo
	//   "press any key to continue"
	// - Esta funcionalidade tamb�m se consegue de
	//   outras formas
	static char getch(void);
};

#endif