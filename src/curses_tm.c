#include "curses.h"

#include <ncurses.h>
#include <locale.h>

void curses_init(void)
{
	setlocale(LC_ALL, "");
	initscr();
	raw();
	noecho();
	cbreak();
	keypad(stdscr, 1);
	curs_set(0);

	start_color();
	for (uint8_t f = 0; f < 0x10; f++) {
		for (uint8_t b = 0; b < 0x10; b++) {
			init_pair(COLOR(f, b), f, b);
		}
	}
}

void curses_quit(void)
{
	curs_set(1);
	keypad(stdscr, 0);
	nocbreak();
	echo();
	noraw();
	endwin();
}

void curses_clear(void)
{
	erase();
}

void curses_show(void)
{
	refresh();
}

int curses_input(void)
{
	return getch();
}

void curses_putc(int x, int y, char c)
{
	mvaddch(y, x, c);
}

void curses_puts(int x, int y, char *s)
{
	mvaddstr(y, x, s);
}

void curses_color(uint8_t color)
{
	attron(COLOR_PAIR(color));
}

int curses_width(void)
{
	return getmaxx(stdscr);
}

int curses_height(void)
{
	return getmaxy(stdscr);
}

