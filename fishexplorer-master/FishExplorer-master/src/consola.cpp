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

/* --> ver coment�rios em consola.h */

#include "consola.h"                      
#include <windows.h>
#include <stdio.h>

// Defini��o das vari�veis est�ticas
HANDLE Consola::hconsola = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE Consola::hStdin = GetStdHandle(STD_INPUT_HANDLE);;
HWND Consola::hwnd = GetConsoleWindow();

/*
Consola::Consola() {
	hconsola = GetStdHandle(STD_OUTPUT_HANDLE);
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	hwnd = GetConsoleWindow();
	// NT "upwards" apenas
	// O suporte para w95 e w98 � demasiado retorcido
	// e j� ningu�m usa esses "sistemas"
}
*/

void Consola::gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(hconsola, coord);
}

void Consola::clrscr() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	const COORD startCoords = { 0, 0 };
	DWORD dummy;

	GetConsoleScreenBufferInfo(hconsola, &csbi);
	FillConsoleOutputCharacter(hconsola,
		' ',
		csbi.dwSize.X * csbi.dwSize.Y,
		startCoords,
		&dummy);
	FillConsoleOutputAttribute(hconsola,
		csbi.wAttributes,
		csbi.dwSize.X * csbi.dwSize.Y,
		startCoords,
		&dummy);
	gotoxy(0, 0);  // reposicina no canto superior esquerdo
}

void Consola::setTextColor(WORD color) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hconsola, &csbi);
	WORD cor = csbi.wAttributes;
	cor &= 0xFFF0;  // isola os bits que interessam
	cor |= color;   // lga os bits da cor
	// duvidas na operaca��o bin�ria -> ver TI ou TAC
	SetConsoleTextAttribute(hconsola, cor);
	return;
}

void Consola::setBackgroundColor(WORD color) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hconsola, &csbi);
	WORD cor = csbi.wAttributes;
	cor &= 0xFF0F;
	cor |= (color << 4);  // coloca os bits da cor nos bits certos
	// duvidas na operaca��o bin�ria -> ver TI ou TAC
	SetConsoleTextAttribute(hconsola, cor);
}

void Consola::setScreenSize(int nLinhas, int nCols) {
	COORD tam;
	SMALL_RECT DisplayArea;

	tam.X = nCols;
	tam.Y = nLinhas;
	SetConsoleScreenBufferSize(hconsola, tam);
	// isto muda o tamanho da matriz/buffer (de caracteres)

	DisplayArea.Top = 0;
	DisplayArea.Left = 0;
	DisplayArea.Bottom = nLinhas - 1;
	DisplayArea.Right = nCols - 1;
	SetConsoleWindowInfo(hconsola, TRUE, &DisplayArea);  // isto muda o tamanho da area da janela em caracteres
}

char Consola::getch(void) {
	INPUT_RECORD irInputRecord;
	DWORD dwEventsRead;
	CHAR cChar;

	while (ReadConsoleInputA(hStdin, &irInputRecord, 1, &dwEventsRead)) /* Read key press */
		if (irInputRecord.EventType == KEY_EVENT
			&& irInputRecord.Event.KeyEvent.wVirtualKeyCode != VK_SHIFT
			&& irInputRecord.Event.KeyEvent.wVirtualKeyCode != VK_MENU
			&& irInputRecord.Event.KeyEvent.wVirtualKeyCode != VK_CONTROL)
		{

		cChar = irInputRecord.Event.KeyEvent.uChar.AsciiChar;
		ReadConsoleInputA(hStdin, &irInputRecord, 1, &dwEventsRead); /* Read key release */

		if (irInputRecord.Event.KeyEvent.wVirtualKeyCode == VK_LEFT) return ESQUERDA;
		if (irInputRecord.Event.KeyEvent.wVirtualKeyCode == VK_RIGHT) return DIREITA;
		if (irInputRecord.Event.KeyEvent.wVirtualKeyCode == VK_UP) return CIMA;
		if (irInputRecord.Event.KeyEvent.wVirtualKeyCode == VK_DOWN) return BAIXO;

		return cChar;
		}
	return EOF;
}


// setTextSizeXP - Usar isto apenas se a outra n�o funcionar (XP ou menos)
// O m�todo de funcionamento � bastante for�a-bruta
//   Procura uma fonte que cumpra os requisitos do novo tamanho
//   e muda para essa fonte (ou seja, muda tamb�ma fonte)
// Usa fun��es n�o documentadas da Microsoft (s�o secretas :) )
// Mais info: http://blogs.microsoft.co.il/blogs/pavely/archive/2009/07/23/changing-console-fonts.aspx
typedef BOOL(WINAPI * SetConsoleFont_)(HANDLE ConsoleOutput, DWORD FontIndex); // kernel32!SetConsoleFont
typedef BOOL(WINAPI * GetConsoleFontInfo_)(HANDLE ConsoleOutput, BOOL Unknown1, DWORD Unknown2, PCONSOLE_FONT_INFO ConsoleFontInfo); // kernel32!GetConsoleFontInfo
typedef DWORD(WINAPI * GetNumberOfConsoleFonts_)(); // kernel32!GetNumberOfConsoleFonts
