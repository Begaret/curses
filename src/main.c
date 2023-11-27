#include "curses.h" 

#include <stdlib.h>
#include <time.h>

typedef struct static_data_t static_data_t;
struct static_data_t
{
	char tile;
	short color;
	uint32_t flags;
};

typedef enum flags_t flags_t;
enum flags_t
{
	F_BLOCKS_MOVEMENT = 1 << 0
};

typedef struct entity_t entity_t;
struct entity_t
{
	int d;
	int x;
	int y;
};

typedef struct map_t map_t;
struct map_t
{
	short		*	t;
	entity_t	*	e;
	int w;
	int h;
};

map_t map;

int map_get_entity(int x, int y)
{
	/*
	if (x >= 0 && x < map.w && y >= 0 && y < map.h)
		return map.e[x + y * map.w].d;
	return -1;
	*/

	if (x >= 0 && x < map.w && y >= 0 && y < map.h) {
		for (int i = 0; i < 64; i++) {
			if (map.e[i].x == x && map.e[i].y == y) {
				return map.e[i].d;
			} else {
				return 0;
			}
		}
	}

	return -1;
}

int map_get_terrain(int x, int y)
{
	if (x >= 0 && x < map.w && y >= 0 && y < map.h)
		return map.t[x + y * map.w];
	return -1;
}

int main(int argc, char *argv[])
{
	int w = 80;
	int h = 24;

	if (argc == 3) {
		w = atoi(argv[1]);
		h = atoi(argv[2]);
	}

	srand(time(NULL));

	static_data_t data[5] = {
		{.tile = '?', .color = COLOR(C_BLACK, C_BLACK), .flags = 0},
		{.tile = '#', .color = COLOR(C_LGREEN, C_BLACK), .flags = 0},
		{.tile = ',', .color = COLOR(C_GREEN, C_BLACK), .flags = 0},
		{.tile = '@', .color = COLOR(C_WHITE, C_BLACK), .flags = F_BLOCKS_MOVEMENT},
		{.tile = '@', .color = COLOR(C_LGRAY, C_BLACK), .flags = F_BLOCKS_MOVEMENT},
	};

	map.w = w;
	map.h = h;
	map.t = malloc(sizeof(*map.t) * map.w * map.h);
	map.e = malloc(sizeof(*map.e) * 64);

	for (int i = 0; i < map.w * map.h; i++) {
		int r = rand() % 10;
		if (r <= 2) {
			map.t[i] = 1;
		} else {
			map.t[i] = 2;
		}

		/*
		r = rand() % 512;
		if (r <= 2) {
			map.e[i] = (entity_t){.d = 4, .x = (i % map.w), .y = (i / map.w)};
		} else {
			map.e[i] = (entity_t){.d = 0, .x = (i % map.w), .y = (i / map.w)};	// no entity
		}*/
	}

	for (int i = 0; i < 64; i++) {
		int r = rand() % 32;
		if (r == 0) {
			map.e[i] = (entity_t){.d = 4, .x = rand() % map.w, .y = rand() % map.h};
		} else {
			map.e[i] = (entity_t){.d = 0, .x = 0, .y = 0};
		}
	}

	entity_t p = {.d = 3, .x = map.w / 2, .y = map.h / 2 };

	curses_init();
	const int scr_w = curses_width();
	const int scr_h = curses_height();

	while (1) {
		int cam_x = p.x - (scr_w / 2);
		int cam_y = p.y - (scr_h / 2);

		curses_clear();

		for (int i = 0; i < map.w * map.h; i++) {
			int mx = i % map.w;
			int my = i / map.w;
			int x = mx - cam_x;
			int y = my - cam_y;

			int d = 0;
			if ((d = map_get_entity(mx, my)) > 0) {
				curses_color(data[d].color);
				curses_putc(x, y, data[d].tile);
			} else if ((d = map_get_terrain(mx, my)) > 0) {
				curses_color(data[d].color);
				curses_putc(x, y, data[d].tile);
			}
		}

		curses_color(data[p.d].color);
		curses_putc(p.x - cam_x, p.y - cam_y, data[p.d].tile);

		curses_color(COLOR(C_WHITE, C_BLACK));
		curses_puts(0, 0, "Hello, world!");

		curses_show();

		int k = curses_input();
		if (k == 'q') {
			break;
		} else if (k == K_LEFT) {
			int d = map_get_entity(p.x - 1, p.y);
			if (d >= 0 && !(data[d].flags & F_BLOCKS_MOVEMENT))
				p.x--;
		} else if (k == K_RIGHT) {
			int d = map_get_entity(p.x + 1, p.y);
			if (d >= 0 && !(data[d].flags & F_BLOCKS_MOVEMENT))
				p.x++;
		} else if (k == K_UP) {
			int d = map_get_entity(p.x, p.y - 1);
			if (d >= 0 && !(data[d].flags & F_BLOCKS_MOVEMENT))
				p.y--;
		} else if (k == K_DOWN) {
			int d = map_get_entity(p.x, p.y + 1);
			if (d >= 0 && !(data[d].flags & F_BLOCKS_MOVEMENT))
				p.y++;
		}
	}

	free(map.t);
	free(map.e);
	curses_quit();
	return 0;
}

