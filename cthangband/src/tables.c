#define TABLES_C
/* File: tables.c */

/* Purpose: Angband Tables */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"

/*
 * Global array for looping through the "keypad directions"
 */
s16b ddd[9] =
{ 2, 8, 6, 4, 3, 1, 9, 7, 5 };

/*
 * Global arrays for converting "keypad direction" into offsets
 */
s16b ddx[10] =
{ 0, -1, 0, 1, -1, 0, 1, -1, 0, 1 };

s16b ddy[10] =
{ 0, 1, 1, 1, 0, 0, 0, -1, -1, -1 };

/*
 * Global arrays for optimizing "ddx[ddd[i]]" and "ddy[ddd[i]]"
 */
s16b ddx_ddd[9] =
{ 0, 0, 1, -1, 1, -1, 1, -1, 0 };

s16b ddy_ddd[9] =
{ 1, -1, 0, 0, 1, 1, -1, -1, 0 };



/*
 * Global array for converting numbers to uppercase hecidecimal digit
 * This array can also be used to convert a number to an octal digit
 */
char hexsym[16] =
{
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};


/*
 * Stat Table: (INT) -- Number of half-spells per level
 * (CHR) -- Decrease in the annoyance of a favour
 */
const byte adj_mag_study[NUM_STAT_INDICES] =
{
	0       /* 3 */,
	0       /* 4 */,
	0       /* 5 */,
	0       /* 6 */,
	0       /* 7 */,
	1       /* 8 */,
	1       /* 9 */,
	1       /* 10 */,
	1       /* 11 */,
	2       /* 12 */,
	2       /* 13 */,
	2       /* 14 */,
	2       /* 15 */,
	2       /* 16 */,
	2       /* 17 */,
	2       /* 18/00-18/09 */,
	2       /* 18/10-18/19 */,
	2       /* 18/20-18/29 */,
	2       /* 18/30-18/39 */,
	2       /* 18/40-18/49 */,
	3       /* 18/50-18/59 */,
	3       /* 18/60-18/69 */,
	3       /* 18/70-18/79 */,
	3       /* 18/80-18/89 */,
	4       /* 18/90-18/99 */,
	4       /* 18/100-18/109 */,
	4       /* 18/110-18/119 */,
	5       /* 18/120-18/129 */,
	5       /* 18/130-18/139 */,
	5       /* 18/140-18/149 */,
	5       /* 18/150-18/159 */,
	5       /* 18/160-18/169 */,
	5       /* 18/170-18/179 */,
	5       /* 18/180-18/189 */,
	5       /* 18/190-18/199 */,
	5       /* 18/200-18/209 */,
    6       /* 18/210-18/219 */,
    6       /* 18/220+ */
};


/*
 * Stat Table (INT/WIS) -- extra quarter-mana/chi-points per skill percent
 */
const byte adj_mag_mana[NUM_STAT_INDICES] =
{
	0       /* 3 */,
	0       /* 4 */,
	0       /* 5 */,
	0       /* 6 */,
	0       /* 7 */,
	1       /* 8 */,
	2       /* 9 */,
	2       /* 10 */,
	2       /* 11 */,
	2       /* 12 */,
	2       /* 13 */,
	2       /* 14 */,
	2       /* 15 */,
	2       /* 16 */,
	2       /* 17 */,
	3       /* 18/00-18/09 */,
	3       /* 18/10-18/19 */,
	3       /* 18/20-18/29 */,
	3       /* 18/30-18/39 */,
	3       /* 18/40-18/49 */,
	4       /* 18/50-18/59 */,
	4       /* 18/60-18/69 */,
	5       /* 18/70-18/79 */,
	6       /* 18/80-18/89 */,
	7       /* 18/90-18/99 */,
	8       /* 18/100-18/109 */,
	9       /* 18/110-18/119 */,
	10      /* 18/120-18/129 */,
	11      /* 18/130-18/139 */,
	12      /* 18/140-18/149 */,
	13      /* 18/150-18/159 */,
	14      /* 18/160-18/169 */,
	15      /* 18/170-18/179 */,
	16      /* 18/180-18/189 */,
	16      /* 18/190-18/199 */,
    17      /* 18/200-18/209 */,
    17      /* 18/210-18/219 */,
    18      /* 18/220+ */
};


/*
 * Stat Table (INT/WIS/CHR) -- Minimum failure rate (percentage)
 */
const byte adj_mag_fail[NUM_STAT_INDICES] =
{
	99      /* 3 */,
	99      /* 4 */,
	99      /* 5 */,
	99      /* 6 */,
	99      /* 7 */,
	50      /* 8 */,
	30      /* 9 */,
	20      /* 10 */,
	15      /* 11 */,
	12      /* 12 */,
	11      /* 13 */,
	10      /* 14 */,
	9       /* 15 */,
	8       /* 16 */,
	7       /* 17 */,
	6       /* 18/00-18/09 */,
	6       /* 18/10-18/19 */,
	5       /* 18/20-18/29 */,
	5       /* 18/30-18/39 */,
	5       /* 18/40-18/49 */,
	4       /* 18/50-18/59 */,
	4       /* 18/60-18/69 */,
	4       /* 18/70-18/79 */,
	4       /* 18/80-18/89 */,
	3       /* 18/90-18/99 */,
	3       /* 18/100-18/109 */,
	2       /* 18/110-18/119 */,
	2       /* 18/120-18/129 */,
	2       /* 18/130-18/139 */,
	2       /* 18/140-18/149 */,
	1       /* 18/150-18/159 */,
	1       /* 18/160-18/169 */,
	1       /* 18/170-18/179 */,
	1       /* 18/180-18/189 */,
	1       /* 18/190-18/199 */,
	0       /* 18/200-18/209 */,
	0       /* 18/210-18/219 */,
	0       /* 18/220+ */
};


/*
 * Stat Table: (INT/WIS/CHR) -- Decreases failure rate (*3-3)
 */
const byte adj_mag_stat[NUM_STAT_INDICES] =
{
	0       /* 3 */,
	0       /* 4 */,
	0       /* 5 */,
	0       /* 6 */,
	0       /* 7 */,
	1       /* 8 */,
	1       /* 9 */,
	1       /* 10 */,
	1       /* 11 */,
	1       /* 12 */,
	1       /* 13 */,
	1       /* 14 */,
	2       /* 15 */,
	2       /* 16 */,
	2       /* 17 */,
	3       /* 18/00-18/09 */,
	3       /* 18/10-18/19 */,
	3       /* 18/20-18/29 */,
	3       /* 18/30-18/39 */,
	3       /* 18/40-18/49 */,
	4       /* 18/50-18/59 */,
	4       /* 18/60-18/69 */,
	5       /* 18/70-18/79 */,
	6       /* 18/80-18/89 */,
	7       /* 18/90-18/99 */,
	8       /* 18/100-18/109 */,
	9       /* 18/110-18/119 */,
	10      /* 18/120-18/129 */,
	11      /* 18/130-18/139 */,
	12      /* 18/140-18/149 */,
	13      /* 18/150-18/159 */,
	14      /* 18/160-18/169 */,
	15      /* 18/170-18/179 */,
	16      /* 18/180-18/189 */,
	17      /* 18/190-18/199 */,
	18      /* 18/200-18/209 */,
	19      /* 18/210-18/219 */,
	20      /* 18/220+ */
};


/*
 * Stat Table (CHR) -- payment percentages
 */
const byte adj_chr_gold[NUM_STAT_INDICES] =
{
	130     /* 3 */,
	125     /* 4 */,
	122     /* 5 */,
	120     /* 6 */,
	118     /* 7 */,
	116     /* 8 */,
	114     /* 9 */,
	112     /* 10 */,
	110     /* 11 */,
	108     /* 12 */,
	106     /* 13 */,
	104     /* 14 */,
	103     /* 15 */,
	102     /* 16 */,
	101     /* 17 */,
	100     /* 18/00-18/09 */,
	99      /* 18/10-18/19 */,
	98      /* 18/20-18/29 */,
	97      /* 18/30-18/39 */,
	96      /* 18/40-18/49 */,
	95      /* 18/50-18/59 */,
	94      /* 18/60-18/69 */,
	93      /* 18/70-18/79 */,
	92      /* 18/80-18/89 */,
	91      /* 18/90-18/99 */,
	90      /* 18/100-18/109 */,
	89      /* 18/110-18/119 */,
	88      /* 18/120-18/129 */,
	87      /* 18/130-18/139 */,
	86      /* 18/140-18/149 */,
	85      /* 18/150-18/159 */,
	84      /* 18/160-18/169 */,
	83      /* 18/170-18/179 */,
	82      /* 18/180-18/189 */,
	81      /* 18/190-18/199 */,
	80      /* 18/200-18/209 */,
    79      /* 18/210-18/219 */,
    78      /* 18/220+ */
};


/*
 * Stat Table (INT) -- Magic devices
 */
const byte adj_int_dev[NUM_STAT_INDICES] =
{
	0       /* 3 */,
	0       /* 4 */,
	0       /* 5 */,
	0       /* 6 */,
	0       /* 7 */,
	1       /* 8 */,
	1       /* 9 */,
	1       /* 10 */,
	1       /* 11 */,
	1       /* 12 */,
	1       /* 13 */,
	1       /* 14 */,
	2       /* 15 */,
	2       /* 16 */,
	2       /* 17 */,
	3       /* 18/00-18/09 */,
	3       /* 18/10-18/19 */,
	4       /* 18/20-18/29 */,
	4       /* 18/30-18/39 */,
	5       /* 18/40-18/49 */,
	5       /* 18/50-18/59 */,
	6       /* 18/60-18/69 */,
	6       /* 18/70-18/79 */,
	7       /* 18/80-18/89 */,
	7       /* 18/90-18/99 */,
	8       /* 18/100-18/109 */,
	9       /* 18/110-18/119 */,
	10      /* 18/120-18/129 */,
	11      /* 18/130-18/139 */,
	12      /* 18/140-18/149 */,
	13      /* 18/150-18/159 */,
	14      /* 18/160-18/169 */,
	15      /* 18/170-18/179 */,
	16      /* 18/180-18/189 */,
	17      /* 18/190-18/199 */,
	18      /* 18/200-18/209 */,
	19      /* 18/210-18/219 */,
	20      /* 18/220+ */
};


/*
 * Stat Table (WIS) -- Saving throw
 */
const byte adj_wis_sav[NUM_STAT_INDICES] =
{
	0       /* 3 */,
	0       /* 4 */,
	0       /* 5 */,
	0       /* 6 */,
	0       /* 7 */,
	1       /* 8 */,
	1       /* 9 */,
	1       /* 10 */,
	1       /* 11 */,
	1       /* 12 */,
	1       /* 13 */,
	1       /* 14 */,
	2       /* 15 */,
	2       /* 16 */,
	2       /* 17 */,
	3       /* 18/00-18/09 */,
	3       /* 18/10-18/19 */,
	3       /* 18/20-18/29 */,
	3       /* 18/30-18/39 */,
	3       /* 18/40-18/49 */,
	4       /* 18/50-18/59 */,
	4       /* 18/60-18/69 */,
	5       /* 18/70-18/79 */,
	5       /* 18/80-18/89 */,
	6       /* 18/90-18/99 */,
	7       /* 18/100-18/109 */,
	8       /* 18/110-18/119 */,
	9       /* 18/120-18/129 */,
	10      /* 18/130-18/139 */,
	11      /* 18/140-18/149 */,
	12      /* 18/150-18/159 */,
	13      /* 18/160-18/169 */,
	14      /* 18/170-18/179 */,
	15      /* 18/180-18/189 */,
	16      /* 18/190-18/199 */,
	17      /* 18/200-18/209 */,
	18      /* 18/210-18/219 */,
	19      /* 18/220+ */
};


/*
 * Stat Table (DEX) -- disarming
 */
const byte adj_dex_dis[NUM_STAT_INDICES] =
{
	0       /* 3 */,
	0       /* 4 */,
	0       /* 5 */,
	0       /* 6 */,
	0       /* 7 */,
	0       /* 8 */,
	0       /* 9 */,
	0       /* 10 */,
	0       /* 11 */,
	0       /* 12 */,
	1       /* 13 */,
	1       /* 14 */,
	1       /* 15 */,
	2       /* 16 */,
	2       /* 17 */,
	4       /* 18/00-18/09 */,
	4       /* 18/10-18/19 */,
	4       /* 18/20-18/29 */,
	4       /* 18/30-18/39 */,
	5       /* 18/40-18/49 */,
	5       /* 18/50-18/59 */,
	5       /* 18/60-18/69 */,
	6       /* 18/70-18/79 */,
	6       /* 18/80-18/89 */,
	7       /* 18/90-18/99 */,
	8       /* 18/100-18/109 */,
	8       /* 18/110-18/119 */,
	8       /* 18/120-18/129 */,
	8       /* 18/130-18/139 */,
	8       /* 18/140-18/149 */,
	9       /* 18/150-18/159 */,
	9       /* 18/160-18/169 */,
	9       /* 18/170-18/179 */,
	9       /* 18/180-18/189 */,
	9       /* 18/190-18/199 */,
	10      /* 18/200-18/209 */,
	10      /* 18/210-18/219 */,
	10      /* 18/220+ */
};


/*
 * Stat Table (INT) -- disarming
 */
const byte adj_int_dis[NUM_STAT_INDICES] =
{
	0       /* 3 */,
	0       /* 4 */,
	0       /* 5 */,
	0       /* 6 */,
	0       /* 7 */,
	1       /* 8 */,
	1       /* 9 */,
	1       /* 10 */,
	1       /* 11 */,
	1       /* 12 */,
	1       /* 13 */,
	1       /* 14 */,
	2       /* 15 */,
	2       /* 16 */,
	2       /* 17 */,
	3       /* 18/00-18/09 */,
	3       /* 18/10-18/19 */,
	3       /* 18/20-18/29 */,
	4       /* 18/30-18/39 */,
	4       /* 18/40-18/49 */,
	5       /* 18/50-18/59 */,
	6       /* 18/60-18/69 */,
	7       /* 18/70-18/79 */,
	8       /* 18/80-18/89 */,
	9       /* 18/90-18/99 */,
	10      /* 18/100-18/109 */,
	10      /* 18/110-18/119 */,
	11      /* 18/120-18/129 */,
	12      /* 18/130-18/139 */,
	13      /* 18/140-18/149 */,
	14      /* 18/150-18/159 */,
	15      /* 18/160-18/169 */,
	16      /* 18/170-18/179 */,
	17      /* 18/180-18/189 */,
	18      /* 18/190-18/199 */,
	19      /* 18/200-18/209 */,
	19      /* 18/210-18/219 */,
    20      /* 18/220+ */
};


/*
 * Stat Table (DEX) -- bonus to ac (plus 128)
 */
const byte adj_dex_ta[NUM_STAT_INDICES] =
{
	128 + -4        /* 3 */,
	128 + -3        /* 4 */,
	128 + -2        /* 5 */,
	128 + -1        /* 6 */,
	128 + 0 /* 7 */,
	128 + 0 /* 8 */,
	128 + 0 /* 9 */,
	128 + 0 /* 10 */,
	128 + 0 /* 11 */,
	128 + 0 /* 12 */,
	128 + 0 /* 13 */,
	128 + 0 /* 14 */,
	128 + 1 /* 15 */,
	128 + 1 /* 16 */,
	128 + 1 /* 17 */,
	128 + 2 /* 18/00-18/09 */,
	128 + 2 /* 18/10-18/19 */,
	128 + 2 /* 18/20-18/29 */,
	128 + 2 /* 18/30-18/39 */,
	128 + 2 /* 18/40-18/49 */,
	128 + 3 /* 18/50-18/59 */,
	128 + 3 /* 18/60-18/69 */,
	128 + 3 /* 18/70-18/79 */,
	128 + 4 /* 18/80-18/89 */,
	128 + 5 /* 18/90-18/99 */,
	128 + 6 /* 18/100-18/109 */,
	128 + 7 /* 18/110-18/119 */,
	128 + 8 /* 18/120-18/129 */,
	128 + 9 /* 18/130-18/139 */,
	128 + 9 /* 18/140-18/149 */,
	128 + 10        /* 18/150-18/159 */,
	128 + 11        /* 18/160-18/169 */,
	128 + 12        /* 18/170-18/179 */,
	128 + 13        /* 18/180-18/189 */,
	128 + 14        /* 18/190-18/199 */,
	128 + 15        /* 18/200-18/209 */,
	128 + 15        /* 18/210-18/219 */,
    128 + 16        /* 18/220+ */
};


/*
 * Stat Table (STR) -- bonus to dam (plus 128)
 */
const byte adj_str_td[NUM_STAT_INDICES] =
{
	128 + -2        /* 3 */,
	128 + -2        /* 4 */,
	128 + -1        /* 5 */,
	128 + -1        /* 6 */,
	128 + 0 /* 7 */,
	128 + 0 /* 8 */,
	128 + 0 /* 9 */,
	128 + 0 /* 10 */,
	128 + 0 /* 11 */,
	128 + 0 /* 12 */,
	128 + 0 /* 13 */,
	128 + 0 /* 14 */,
	128 + 0 /* 15 */,
	128 + 1 /* 16 */,
	128 + 2 /* 17 */,
	128 + 2 /* 18/00-18/09 */,
	128 + 2 /* 18/10-18/19 */,
	128 + 3 /* 18/20-18/29 */,
	128 + 3 /* 18/30-18/39 */,
	128 + 3 /* 18/40-18/49 */,
	128 + 3 /* 18/50-18/59 */,
	128 + 3 /* 18/60-18/69 */,
	128 + 4 /* 18/70-18/79 */,
	128 + 5 /* 18/80-18/89 */,
	128 + 5 /* 18/90-18/99 */,
	128 + 6 /* 18/100-18/109 */,
	128 + 7 /* 18/110-18/119 */,
	128 + 8 /* 18/120-18/129 */,
	128 + 9 /* 18/130-18/139 */,
	128 + 10        /* 18/140-18/149 */,
	128 + 11        /* 18/150-18/159 */,
	128 + 12        /* 18/160-18/169 */,
	128 + 13        /* 18/170-18/179 */,
	128 + 14        /* 18/180-18/189 */,
	128 + 15        /* 18/190-18/199 */,
	128 + 16        /* 18/200-18/209 */,
	128 + 18        /* 18/210-18/219 */,
	128 + 20        /* 18/220+ */
};


/*
 * Stat Table (DEX) -- bonus to hit (plus 128)
 */
const byte adj_dex_th[NUM_STAT_INDICES] =
{
	128 + -3        /* 3 */,
	128 + -2        /* 4 */,
	128 + -2        /* 5 */,
	128 + -1        /* 6 */,
	128 + -1        /* 7 */,
	128 + 0 /* 8 */,
	128 + 0 /* 9 */,
	128 + 0 /* 10 */,
	128 + 0 /* 11 */,
	128 + 0 /* 12 */,
	128 + 0 /* 13 */,
	128 + 0 /* 14 */,
	128 + 0 /* 15 */,
	128 + 1 /* 16 */,
	128 + 2 /* 17 */,
	128 + 3 /* 18/00-18/09 */,
	128 + 3 /* 18/10-18/19 */,
	128 + 3 /* 18/20-18/29 */,
	128 + 3 /* 18/30-18/39 */,
	128 + 3 /* 18/40-18/49 */,
	128 + 4 /* 18/50-18/59 */,
	128 + 4 /* 18/60-18/69 */,
	128 + 4 /* 18/70-18/79 */,
	128 + 4 /* 18/80-18/89 */,
	128 + 5 /* 18/90-18/99 */,
	128 + 6 /* 18/100-18/109 */,
	128 + 7 /* 18/110-18/119 */,
	128 + 8 /* 18/120-18/129 */,
	128 + 9 /* 18/130-18/139 */,
	128 + 9 /* 18/140-18/149 */,
	128 + 10        /* 18/150-18/159 */,
	128 + 11        /* 18/160-18/169 */,
	128 + 12        /* 18/170-18/179 */,
	128 + 13        /* 18/180-18/189 */,
	128 + 14        /* 18/190-18/199 */,
	128 + 15        /* 18/200-18/209 */,
	128 + 15        /* 18/210-18/219 */,
    128 + 16        /* 18/220+ */
};


/*
 * Stat Table (STR) -- bonus to hit (plus 128)
 */
const byte adj_str_th[NUM_STAT_INDICES] =
{
	128 + -3        /* 3 */,
	128 + -2        /* 4 */,
	128 + -1        /* 5 */,
	128 + -1        /* 6 */,
	128 + 0 /* 7 */,
	128 + 0 /* 8 */,
	128 + 0 /* 9 */,
	128 + 0 /* 10 */,
	128 + 0 /* 11 */,
	128 + 0 /* 12 */,
	128 + 0 /* 13 */,
	128 + 0 /* 14 */,
	128 + 0 /* 15 */,
	128 + 0 /* 16 */,
	128 + 0 /* 17 */,
	128 + 1 /* 18/00-18/09 */,
	128 + 1 /* 18/10-18/19 */,
	128 + 1 /* 18/20-18/29 */,
	128 + 1 /* 18/30-18/39 */,
	128 + 1 /* 18/40-18/49 */,
	128 + 1 /* 18/50-18/59 */,
	128 + 1 /* 18/60-18/69 */,
	128 + 2 /* 18/70-18/79 */,
	128 + 3 /* 18/80-18/89 */,
	128 + 4 /* 18/90-18/99 */,
	128 + 5 /* 18/100-18/109 */,
	128 + 6 /* 18/110-18/119 */,
	128 + 7 /* 18/120-18/129 */,
	128 + 8 /* 18/130-18/139 */,
	128 + 9 /* 18/140-18/149 */,
	128 + 10        /* 18/150-18/159 */,
	128 + 11        /* 18/160-18/169 */,
	128 + 12        /* 18/170-18/179 */,
	128 + 13        /* 18/180-18/189 */,
	128 + 14        /* 18/190-18/199 */,
	128 + 15        /* 18/200-18/209 */,
	128 + 15        /* 18/210-18/219 */,
    128 + 16        /* 18/220+ */
};


/*
 * Stat Table (STR) -- weight limit in deca-pounds
 */
const byte adj_str_wgt[NUM_STAT_INDICES] =
{
	5       /* 3 */,
	6       /* 4 */,
	7       /* 5 */,
	8       /* 6 */,
	9       /* 7 */,
	10      /* 8 */,
	11      /* 9 */,
	12      /* 10 */,
	13      /* 11 */,
	14      /* 12 */,
	15      /* 13 */,
	16      /* 14 */,
	17      /* 15 */,
	18      /* 16 */,
	19      /* 17 */,
	20      /* 18/00-18/09 */,
	22      /* 18/10-18/19 */,
	24      /* 18/20-18/29 */,
	26      /* 18/30-18/39 */,
	28      /* 18/40-18/49 */,
	30      /* 18/50-18/59 */,
    31      /* 18/60-18/69 */,
    31      /* 18/70-18/79 */,
    32      /* 18/80-18/89 */,
    32      /* 18/90-18/99 */,
    33      /* 18/100-18/109 */,
    33      /* 18/110-18/119 */,
    34      /* 18/120-18/129 */,
    34      /* 18/130-18/139 */,
    35      /* 18/140-18/149 */,
    35      /* 18/150-18/159 */,
    36      /* 18/160-18/169 */,
    36      /* 18/170-18/179 */,
    37      /* 18/180-18/189 */,
    37      /* 18/190-18/199 */,
    38      /* 18/200-18/209 */,
    38      /* 18/210-18/219 */,
    39      /* 18/220+ */
};


/*
 * Stat Table (STR) -- weapon weight limit in pounds
 */
const byte adj_str_hold[NUM_STAT_INDICES] =
{
	4       /* 3 */,
	5       /* 4 */,
	6       /* 5 */,
	7       /* 6 */,
	8       /* 7 */,
	10      /* 8 */,
	12      /* 9 */,
	14      /* 10 */,
	16      /* 11 */,
	18      /* 12 */,
	20      /* 13 */,
	22      /* 14 */,
	24      /* 15 */,
	26      /* 16 */,
	28      /* 17 */,
	30      /* 18/00-18/09 */,
	30      /* 18/10-18/19 */,
	35      /* 18/20-18/29 */,
	40      /* 18/30-18/39 */,
	45      /* 18/40-18/49 */,
	50      /* 18/50-18/59 */,
	55      /* 18/60-18/69 */,
	60      /* 18/70-18/79 */,
	65      /* 18/80-18/89 */,
	70      /* 18/90-18/99 */,
	80      /* 18/100-18/109 */,
	80      /* 18/110-18/119 */,
	80      /* 18/120-18/129 */,
	80      /* 18/130-18/139 */,
	80      /* 18/140-18/149 */,
	90      /* 18/150-18/159 */,
	90      /* 18/160-18/169 */,
	90      /* 18/170-18/179 */,
	90      /* 18/180-18/189 */,
	90      /* 18/190-18/199 */,
	100     /* 18/200-18/209 */,
	100     /* 18/210-18/219 */,
	100     /* 18/220+ */
};


/*
 * Stat Table (STR) -- digging value
 */
const byte adj_str_dig[NUM_STAT_INDICES] =
{
	0       /* 3 */,
	0       /* 4 */,
	1       /* 5 */,
	2       /* 6 */,
	3       /* 7 */,
	4       /* 8 */,
	4       /* 9 */,
	5       /* 10 */,
	5       /* 11 */,
	6       /* 12 */,
	6       /* 13 */,
	7       /* 14 */,
	7       /* 15 */,
	8       /* 16 */,
	8       /* 17 */,
	9       /* 18/00-18/09 */,
	10      /* 18/10-18/19 */,
	12      /* 18/20-18/29 */,
	15      /* 18/30-18/39 */,
	20      /* 18/40-18/49 */,
	25      /* 18/50-18/59 */,
	30      /* 18/60-18/69 */,
	35      /* 18/70-18/79 */,
	40      /* 18/80-18/89 */,
	45      /* 18/90-18/99 */,
	50      /* 18/100-18/109 */,
	55      /* 18/110-18/119 */,
	60      /* 18/120-18/129 */,
	65      /* 18/130-18/139 */,
	70      /* 18/140-18/149 */,
	75      /* 18/150-18/159 */,
	80      /* 18/160-18/169 */,
	85      /* 18/170-18/179 */,
	90      /* 18/180-18/189 */,
	95      /* 18/190-18/199 */,
    100     /* 18/200-18/209 */,
    100     /* 18/210-18/219 */,
    100     /* 18/220+ */
};


/*
 * Stat Table (STR) -- help index into the "blow" table
 */
const byte adj_str_blow[NUM_STAT_INDICES] =
{
	3       /* 3 */,
	4       /* 4 */,
	5       /* 5 */,
	6       /* 6 */,
	7       /* 7 */,
	8       /* 8 */,
	9       /* 9 */,
	10      /* 10 */,
	11      /* 11 */,
	12      /* 12 */,
	13      /* 13 */,
	14      /* 14 */,
	15      /* 15 */,
	16      /* 16 */,
	17      /* 17 */,
	20 /* 18/00-18/09 */,
	30 /* 18/10-18/19 */,
	40 /* 18/20-18/29 */,
	50 /* 18/30-18/39 */,
	60 /* 18/40-18/49 */,
	70 /* 18/50-18/59 */,
	80 /* 18/60-18/69 */,
	90 /* 18/70-18/79 */,
	100 /* 18/80-18/89 */,
	110 /* 18/90-18/99 */,
	120 /* 18/100-18/109 */,
	130 /* 18/110-18/119 */,
	140 /* 18/120-18/129 */,
	150 /* 18/130-18/139 */,
	160 /* 18/140-18/149 */,
	170 /* 18/150-18/159 */,
	180 /* 18/160-18/169 */,
	190 /* 18/170-18/179 */,
	200 /* 18/180-18/189 */,
	210 /* 18/190-18/199 */,
	220 /* 18/200-18/209 */,
	230 /* 18/210-18/219 */,
	240 /* 18/220+ */
};


/*
 * Stat Table (DEX) -- index into the "blow" table
 */
const byte adj_dex_blow[NUM_STAT_INDICES] =
{
	0       /* 3 */,
	0       /* 4 */,
	0       /* 5 */,
	0       /* 6 */,
	0       /* 7 */,
	0       /* 8 */,
	0       /* 9 */,
	1       /* 10 */,
	1       /* 11 */,
	1       /* 12 */,
	1       /* 13 */,
	1       /* 14 */,
	1       /* 15 */,
	1       /* 16 */,
	1       /* 17 */,
	1       /* 18/00-18/09 */,
	2       /* 18/10-18/19 */,
	2       /* 18/20-18/29 */,
	2       /* 18/30-18/39 */,
	2       /* 18/40-18/49 */,
	3       /* 18/50-18/59 */,
	3       /* 18/60-18/69 */,
	4       /* 18/70-18/79 */,
	4       /* 18/80-18/89 */,
	5       /* 18/90-18/99 */,
	6       /* 18/100-18/109 */,
	7       /* 18/110-18/119 */,
	8       /* 18/120-18/129 */,
	9       /* 18/130-18/139 */,
	10      /* 18/140-18/149 */,
	11      /* 18/150-18/159 */,
	11      /* 18/160-18/169 */,
	11      /* 18/170-18/179 */,
	11      /* 18/180-18/189 */,
	11      /* 18/190-18/199 */,
	11      /* 18/200-18/209 */,
	11      /* 18/210-18/219 */,
	11      /* 18/220+ */
};


/*
 * Stat Table (DEX) -- chance of avoiding "theft" and "falling"
 */
const byte adj_dex_safe[NUM_STAT_INDICES] =
{
	0       /* 3 */,
	1       /* 4 */,
	2       /* 5 */,
	3       /* 6 */,
	4       /* 7 */,
	5       /* 8 */,
	5       /* 9 */,
	6       /* 10 */,
	6       /* 11 */,
	7       /* 12 */,
	7       /* 13 */,
	8       /* 14 */,
	8       /* 15 */,
	9       /* 16 */,
	9       /* 17 */,
	10      /* 18/00-18/09 */,
	10      /* 18/10-18/19 */,
	15      /* 18/20-18/29 */,
	15      /* 18/30-18/39 */,
	20      /* 18/40-18/49 */,
	25      /* 18/50-18/59 */,
	30      /* 18/60-18/69 */,
	35      /* 18/70-18/79 */,
	40      /* 18/80-18/89 */,
	45      /* 18/90-18/99 */,
	50      /* 18/100-18/109 */,
	60      /* 18/110-18/119 */,
	70      /* 18/120-18/129 */,
	80      /* 18/130-18/139 */,
	90      /* 18/140-18/149 */,
	100     /* 18/150-18/159 */,
	100     /* 18/160-18/169 */,
	100     /* 18/170-18/179 */,
	100     /* 18/180-18/189 */,
	100     /* 18/190-18/199 */,
	100     /* 18/200-18/209 */,
	100     /* 18/210-18/219 */,
	100     /* 18/220+ */
};


/*
 * Stat Table (CON) -- base regeneration rate
 */
const byte adj_con_fix[NUM_STAT_INDICES] =
{
	0       /* 3 */,
	0       /* 4 */,
	0       /* 5 */,
	0       /* 6 */,
	0       /* 7 */,
	0       /* 8 */,
	0       /* 9 */,
	0       /* 10 */,
	0       /* 11 */,
	0       /* 12 */,
	0       /* 13 */,
	1       /* 14 */,
	1       /* 15 */,
	1       /* 16 */,
	1       /* 17 */,
	2       /* 18/00-18/09 */,
	2       /* 18/10-18/19 */,
	2       /* 18/20-18/29 */,
	2       /* 18/30-18/39 */,
	2       /* 18/40-18/49 */,
	3       /* 18/50-18/59 */,
	3       /* 18/60-18/69 */,
	3       /* 18/70-18/79 */,
	3       /* 18/80-18/89 */,
	3       /* 18/90-18/99 */,
	4       /* 18/100-18/109 */,
	4       /* 18/110-18/119 */,
	5       /* 18/120-18/129 */,
	6       /* 18/130-18/139 */,
	6       /* 18/140-18/149 */,
	7       /* 18/150-18/159 */,
	7       /* 18/160-18/169 */,
	8       /* 18/170-18/179 */,
	8       /* 18/180-18/189 */,
	8       /* 18/190-18/199 */,
	9       /* 18/200-18/209 */,
	9       /* 18/210-18/219 */,
	9       /* 18/220+ */
};


/*
 * Stat Table (CON) -- extra half-hitpoints per level (plus 128)
 */
const byte adj_con_mhp[NUM_STAT_INDICES] =
{
	128 + -5        /* 3 */,
	128 + -3        /* 4 */,
	128 + -2        /* 5 */,
	128 + -1        /* 6 */,
	128 + 0 /* 7 */,
	128 + 0 /* 8 */,
	128 + 0 /* 9 */,
	128 + 0 /* 10 */,
	128 + 0 /* 11 */,
	128 + 0 /* 12 */,
	128 + 0 /* 13 */,
	128 + 0 /* 14 */,
	128 + 1 /* 15 */,
	128 + 1 /* 16 */,
	128 + 2 /* 17 */,
	128 + 3 /* 18/00-18/09 */,
	128 + 4 /* 18/10-18/19 */,
	128 + 4 /* 18/20-18/29 */,
	128 + 4 /* 18/30-18/39 */,
	128 + 4 /* 18/40-18/49 */,
	128 + 5 /* 18/50-18/59 */,
	128 + 6 /* 18/60-18/69 */,
	128 + 7 /* 18/70-18/79 */,
	128 + 8 /* 18/80-18/89 */,
	128 + 9 /* 18/90-18/99 */,
	128 + 10        /* 18/100-18/109 */,
	128 + 11        /* 18/110-18/119 */,
	128 + 12        /* 18/120-18/129 */,
	128 + 13        /* 18/130-18/139 */,
	128 + 14        /* 18/140-18/149 */,
	128 + 15        /* 18/150-18/159 */,
	128 + 16        /* 18/160-18/169 */,
	128 + 18        /* 18/170-18/179 */,
	128 + 20        /* 18/180-18/189 */,
	128 + 22        /* 18/190-18/199 */,
    128 + 25        /* 18/200-18/209 */,
    128 + 26        /* 18/210-18/219 */,
    128 + 27        /* 18/220+ */
};


/*
 * This table is used to help calculate the number of blows the player can
 * make in a single round of attacks (one player turn) with a normal weapon.
 *
 * This number ranges from a single blow/round for weak players to up to five
 * blows/round for powerful warriors.
 *
 * Note that certain artifacts and ego-items give "bonus" blows/round, that up
 * to three blows per round can be gained from weapon skill, and that the total
 * by all methods is capped at 10 blows per round.
 *
 * To get "P", we look up the relevant "adj_str_blow[]" (see above),
 * multiply it by "mul", and then divide it by "div", rounding down.
 *
 * To get "D", we look up the relevant "adj_dex_blow[]" (see above),
 * note especially column 6 (DEX 18/101) and 11 (DEX 18/150).
 *
 * The player gets "blows_table[P][D]" blows/round, as shown below,
 * up to a maximum of "num" blows/round, plus any "bonus" blows/round.
 */
byte blows_table[12][12] =
{
	/* P/D */
	/*  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11+ */

	/* 0  */
	{  30,  30,  30,  30,  30,  30,  60,  60,  60,  60,  60,  90},

	/* 1  */
	{  30,  30,  30,  30,  60,  60,  90,  90,  90, 120, 120, 120},

	/* 2  */
	{  30,  30,  60,  60,  90,  90, 120, 120, 120, 150, 150, 150},

	/* 3  */
	{  30,  60,  60,  90,  90, 120, 120, 120, 150, 150, 150, 150},

	/* 4  */
	{  30,  60,  60,  90,  90, 120, 120, 150, 150, 150, 150, 150},

	/* 5  */
	{  60,  60,  90,  90, 120, 120, 150, 150, 150, 150, 150, 150},

	/* 5  */
	{  60,  60,  90,  90, 120, 120, 150, 150, 150, 150, 150, 150},

	/* 7  */
	{  60,  90,  90, 120, 120, 120, 150, 150, 150, 150, 150, 150},

	/* 8  */
	{  90,  90,  90, 120, 120, 120, 150, 150, 150, 150, 150, 150},

	/* 9  */
	{  90,  90, 120, 120, 120, 120, 150, 150, 150, 150, 150, 150},

	/* 10 */
	{  90,  90, 120, 120, 120, 120, 150, 150, 150, 150, 150, 150},

	/* 11+ */
	{  90,  90, 120, 120, 120, 120, 150, 150, 150, 150, 150, 150},
};




/*
 * This table allows quick conversion from "speed" to "energy"
 * The basic function WAS ((S>=110) ? (S-110) : (100 / (120-S)))
 * Note that table access is *much* quicker than computation.
 *
 * Note that the table has been changed at high speeds.  From
 * "Slow (-40)" to "Fast (+30)" is pretty much unchanged, but
 * at speeds above "Fast (+30)", one approaches an asymptotic
 * effective limit of 50 energy per turn.  This means that it
 * is relatively easy to reach "Fast (+30)" and get about 40
 * energy per turn, but then speed becomes very "expensive",
 * and you must get all the way to "Fast (+50)" to reach the
 * point of getting 45 energy per turn.  After that point,
 * furthur increases in speed are more or less pointless,
 * except to balance out heavy inventory.
 */

/* Hack - a short macro to convert values based on a turn energy of 100 
 * to ones based on TURN_ENERGY */
#define TE *TURN_ENERGY/100

 /*
  * This table has been inverted for Cthangband. The new values
  * are 1000/x where x is the old value. This gives the same spread
  * but subtracting extract_energy[n] each move and always adding
  * 10 per turn, rather than adding extract_energy[n] each turn and
  * always subtracting 100.
  *
  * This has been done to allow the seperating out of movement speed
  * and attack speed.
  */
u16b extract_energy[200] =
{
	/* Slow */     1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,
	/* Slow */     1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,
	/* Slow */     1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,
	/* Slow */     1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,
	/* Slow */     1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,
	/* Slow */     1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,
	/* S-50 */     1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,1000 TE,
	/* S-40 */     500 TE,500 TE,500 TE,500 TE,500 TE,500 TE,500 TE,500 TE,500 TE,500 TE,
	/* S-30 */     500 TE,500 TE,500 TE,500 TE,500 TE,500 TE,500 TE,333 TE,333 TE,333 TE,
	/* S-20 */     333 TE,333 TE,333 TE,333 TE,333 TE,250 TE,250 TE,250 TE,250 TE,250 TE,
	/* S-10 */     200 TE,200 TE,200 TE,200 TE,167 TE,167 TE,143 TE,143 TE,125 TE,111 TE,
	/* Norm */    100 TE,91 TE,83 TE,77 TE,71 TE,67 TE,63 TE,59 TE,56 TE,53 TE,
	/* F+10 */    50 TE,48 TE,45 TE,43 TE,42 TE,40 TE,38 TE,37 TE,36 TE,34 TE,
	/* F+20 */    33 TE,32 TE,31 TE,30 TE,29 TE,29 TE,28 TE,28 TE,27 TE,27 TE,
	/* F+30 */    26 TE,26 TE,26 TE,26 TE,25 TE,25 TE,25 TE,24 TE,24 TE,24 TE,
	/* F+40 */    24 TE,24 TE,24 TE,23 TE,23 TE,23 TE,23 TE,23 TE,23 TE,23 TE,
	/* F+50 */    22 TE,22 TE,22 TE,22 TE,22 TE,22 TE,22 TE,22 TE,22 TE,22 TE,
	/* F+60 */    21 TE,21 TE,21 TE,21 TE,21 TE,21 TE,21 TE,21 TE,21 TE,21 TE,
	/* F+70 */    20 TE,20 TE,20 TE,20 TE,20 TE,20 TE,20 TE,20 TE,20 TE,20 TE,
	/* Fast */    20 TE,20 TE,20 TE,20 TE,20 TE,20 TE,20 TE,20 TE,20 TE,20 TE,
};


/*
 * Player Sexes
 *
 *      Title,
 *      Winner
 */
player_sex sex_info[MAX_SEXES] =
{
	{
		"Female",
		"Queen"
	},
	
	{
		"Male",
		"King"
	}
};


/* Dwarves */
static cptr dwarf_syllable1[] =
{
	"B", "D", "F", "G", "Gl", "H", "K", "L", "M", "N", "R", "S", "T", "Th", "V",
	0
};

static cptr dwarf_syllable2[] =
{
	"a", "e", "i", "o", "oi", "u", 0
};

static cptr dwarf_syllable3[] =
{
	"bur", "fur", "gan", "gnus", "gnar", "li", "lin", "lir", "mli", "nar",
	"nus", "rin", "ran", "sin", "sil", "sur", 0
};

static cptr *dwarf_syllables[] =
{
	dwarf_syllable1,
	dwarf_syllable2,
	dwarf_syllable3,
};

/* Elves */
static cptr elf_syllable1[] =
{
	"Al", "An", "Bal", "Bel", "Cal", "Cel", "El", "Elr", "Elv", "Eow", "Ear",
	"F", "Fal", "Fel", "Fin", "G", "Gal", "Gel", "Gl", "Is", "Lan", "Leg",
	"Lom", "N", "Nal", "Nel",  "S", "Sal", "Sel", "T", "Tal", "Tel", "Thr",
	"Tin", 0
};

static cptr elf_syllable2[] =
{
	"a", "adrie", "ara", "e", "ebri", "ele", "ere", "i", "io", "ithra", "ilma",
	"il-Ga", "ili", "o", "orfi", "u", "y", 0
};

static cptr elf_syllable3[] =
{
	"l", "las", "lad", "ldor", "ldur", "linde", "lith", "mir", "n", "nd",
	"ndel", "ndil", "ndir", "nduil", "ng", "mbor", "r", "rith", "ril", "riand",
	"rion", "s", "thien", "viel", "wen", "wyn", 0
};

cptr *elf_syllables[] =
{
	elf_syllable1,
	elf_syllable2,
	elf_syllable3,
};

/* Gnomes */
static cptr gnome_syllable1[] =
{
	"Aar", "An", "Ar", "As", "C", "H", "Han", "Har", "Hel", "Iir", "J", "Jan",
	"Jar", "K", "L", "M", "Mar", "N", "Nik", "Os", "Ol", "P", "R", "S", "Sam",
	"San", "T", "Ter", "Tom", "Ul", "V", "W", "Y", 0
};

static cptr gnome_syllable2[] =
{
	"a", "aa",  "ai", "e", "ei", "i", "o", "uo", "u", "uu", 0
};

static cptr gnome_syllable3[] =
{
	"ron", "re", "la", "ki", "kseli", "ksi", "ku", "ja", "ta", "na", "namari",
	"neli", "nika", "nikki", "nu", "nukka", "ka", "ko", "li", "kki", "rik",
	"po", "to", "pekka", "rjaana", "rjatta", "rjukka", "la", "lla", "lli",
	"mo", "nni", 0
};

static cptr *gnome_syllables[] =
{
	gnome_syllable1,
	gnome_syllable2,
	gnome_syllable3,
};

/* Hobbit */
static cptr hobbit_syllable1[] =
{
	"B", "Ber", "Br", "D", "Der", "Dr", "F", "Fr", "G", "H", "L", "Ler", "M",
	"Mer", "N", "P", "Pr", "Per", "R", "S", "T", "W", 0
};

static cptr hobbit_syllable2[] =
{
	"a", "e", "i", "ia", "o", "oi", "u", 0
};

static cptr hobbit_syllable3[] =
{
	"bo", "ck", "decan", "degar", "do", "doc", "go", "grin", "lba", "lbo",
	"lda", "ldo", "lla", "ll", "lo", "m", "mwise", "nac", "noc", "nwise", "p",
	"ppin", "pper", "tho", "to", 0
};

cptr *hobbit_syllables[] =
{
	hobbit_syllable1,
	hobbit_syllable2,
	hobbit_syllable3,
};

/* Human */
static cptr human_syllable1[] =
{
	"Ab", "Ac", "Ad", "Af", "Agr", "Ast", "As", "Al", "Adw", "Adr", "Ar", "B",
	"Br", "C", "Cr", "Ch", "Cad", "D", "Dr", "Dw", "Ed", "Eth", "Et", "Er",
	"El", "Eow", "F", "Fr", "G", "Gr", "Gw", "Gal", "Gl", "H", "Ha", "Ib",
	"Jer", "K", "Ka", "Ked", "L", "Loth", "Lar", "Leg", "M", "Mir", "N", "Nyd",
	"Ol", "Oc", "On", "P", "Pr", "R", "Rh", "S", "Sev", "T", "Tr", "Th", "V",
	"Y", "Z", "W", "Wic", 0
};

static cptr human_syllable2[] =
{
	"a", "ae", "au", "ao", "are", "ale", "ali", "ay", "ardo", "e", "ei", "ea",
	"eri", "era", "ela", "eli", "enda", "erra", "i", "ia", "ie", "ire", "ira",
	"ila", "ili", "ira", "igo", "o", "oa", "oi", "oe", "ore", "u", "y", 0
};

static cptr human_syllable3[] =
{
	"a", "and", "b", "bwyn", "baen", "bard", "c", "ctred", "cred", "ch", "can",
	"d", "dan", "don", "der", "dric", "dfrid", "dus", "f", "g", "gord", "gan",
	"l", "li", "lgrin", "lin", "lith", "lath", "loth", "ld", "ldric", "ldan",
	"m", "mas", "mos", "mar", "mond", "n", "nydd", "nidd", "nnon", "nwan",
	"nyth", "nad", "nn", "nnor", "nd", "p", "r", "ron", "rd", "s", "sh",
	"seth", "sean", "t", "th", "tha", "tlan", "trem", "tram", "v", "vudd",
	"w", "wan", "win", "wyn", "wyr", "wyr", "wyth", 0
};

static cptr *human_syllables[] =
{
	human_syllable1,
	human_syllable2,
	human_syllable3,
};

/* Orc */
static cptr orc_syllable1[] =
{
	"B", "Er", "G", "Gr", "H", "P", "Pr", "R", "V", "Vr", "T", "Tr", "M", "Dr",
	0
};

static cptr orc_syllable2[] =
{
	"a", "i", "o", "oo", "u", "ui", 0
};

static cptr orc_syllable3[] =
{
	"dash", "dish", "dush", "gar", "gor", "gdush", "lo", "gdish", "k", "lg",
	"nak", "rag", "rbag", "rg", "rk", "ng", "nk", "rt", "ol", "urk", "shnak",
	"mog", "mak", "rak", 0
};

static cptr *orc_syllables[] =
{
	orc_syllable1,
	orc_syllable2,
	orc_syllable3,
};

/* Klackon */
static cptr klackon_syllable1[] =
{
	"K'", "K", "Kri", "Kir", "Kiri", "Iriki", "Irik", "Karik", "Iri","Akri", 0
};

static cptr klackon_syllable2[] =
{
	"arak", "i", "iri", "ikki", "ki", "kiri","ikir","irak","arik","k'","r", 0
};

static cptr klackon_syllable3[] =
{
	"akkak", "ak", "ik", "ikkik", "irik", "arik", "kidik", "kii", "k", "ki",
	"riki","irk", 0
};

static cptr *klackon_syllables[] =
{
	klackon_syllable1,
	klackon_syllable2,
	klackon_syllable3,
};

static cptr cthuloid_syllable1[] =
{
	"Cth","Az","Fth","Ts","Xo","Q'N","R'L","Ghata","L","Zz","Fl","Cl","S","Y", 0
};

static cptr cthuloid_syllable2[] =
{
	"nar","loi","ul","lu","noth","thon","ath","'N","rhy","oth","aza","agn","oa",
	"og", 0
};

static cptr cthuloid_syllable3[] =
{
	"l","a","u","oa","oggua","oth","ath","aggua","lu","lo","loth","lotha","agn",
	"axl", 0
};

static cptr *cthuloid_syllables[] =
{
	cthuloid_syllable1,
	cthuloid_syllable2,
	cthuloid_syllable3,
};

/* Various bonuses different races get at various levels of skill. */

static race_bonus_type dark_elf_bonuses[] =
{
	{0, SKILL_RACIAL, 40, TR3, iilog(TR3_SEE_INVIS), TRUE},
};

static race_bonus_type draconian_bonuses[] =
{
	{0, SKILL_RACIAL, 10, TR2, iilog(TR2_RES_FIRE), TRUE},
	{0, SKILL_RACIAL, 20, TR2, iilog(TR2_RES_COLD), TRUE},
	{0, SKILL_RACIAL, 30, TR2, iilog(TR2_RES_ACID), TRUE},
	{0, SKILL_RACIAL, 40, TR2, iilog(TR2_RES_ELEC), TRUE},
	{0, SKILL_RACIAL, 70, TR2, iilog(TR2_RES_POIS), TRUE},
};

static race_bonus_type golem_bonuses[] =
{
	{0, SKILL_RACIAL, 70, TR2, iilog(TR2_HOLD_LIFE), TRUE},
};

static race_bonus_type half_troll_bonuses[] =
{
	{0, SKILL_RACIAL, 30, TR3, iilog(TR3_REGEN), TRUE},
};

static race_bonus_type imp_bonuses[] =
{
	{0, 0, 0, TR0, iilog(TR0_RES_ELDRITCH), 100},
	{0, SKILL_RACIAL, 20, TR3, iilog(TR3_SEE_INVIS), TRUE},
};

static race_bonus_type mind_flayer_bonuses[] =
{
	{0, SKILL_RACIAL, 30, TR3, iilog(TR3_SEE_INVIS), TRUE},
	{0, SKILL_RACIAL, 60, TR3, iilog(TR3_TELEPATHY), TRUE},
};

static race_bonus_type skeleton_bonuses[] =
{
	{0, 0, 0, TR0, iilog(TR0_RES_ELDRITCH), -1},
	{0, SKILL_RACIAL, 20, TR2, iilog(TR2_RES_COLD), TRUE},
};

static race_bonus_type spectre_bonuses[] =
{
	{0, 0, 0, TR0, iilog(TR0_RES_ELDRITCH), -1},
	{0, SKILL_RACIAL, 70, TR3, iilog(TR3_TELEPATHY), TRUE},
};

static race_bonus_type vampire_bonuses[] =
{
	{0, 0, 0, TR0, iilog(TR0_RES_ELDRITCH), -1},
};

static race_bonus_type yeek_bonuses[] =
{
	{0, SKILL_RACIAL, 40, TR2, iilog(TR2_IM_ACID), TRUE},
};

static race_bonus_type zombie_bonuses[] =
{
	{0, 0, 0, TR0, iilog(TR0_RES_ELDRITCH), -1},
	{0, SKILL_RACIAL, 10, TR2, iilog(TR2_RES_COLD), TRUE},
	{0, SKILL_RACIAL, 24, TR0, iilog(TR0_NO_CUT), TRUE},
};

/*
 * Player Races
 *
 *      Title,
 *      {STR,INT,WIS,DEX,CON,CHR},
 *      disarm, device, save, stealth, search, perception, melee, missile,
 *      hitdie, exp base,
 *      Age (Base, Mod),
 *      Male (Hgt, Wgt),
 *      Female (Hgt, Wgt)
 *      infra, initial history chart, name style,
 *      starting inventory (3 items),
 */
player_race race_info[MAX_RACES] =
{
	{
        "Barbarian",
        { 3, -2,  -1,  1,  2, -2 },
        8, 0, 12,  8,  12, 14, 14, 13,
        11, 120,
        14, 8,
        82, 5, 200, 20,
        78,  6, 190, 15,
		{RP_BARBARIAN, 0},
		{0, 0, TR2_RES_FEAR, 0},
		NULL, 0, 0, 0,
        0, 1, RACE_BLANK, human_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
    },
    {
        "Broo",
        { 2, -2, -1, -1, 2, -4 },
        5, 8, 9, 8, 8, 10, 14, 12,
        11, 140,
        14, 6,
        65,  6, 150, 20,
        61,  6, 120, 15,
		{RP_BROO, 0},
		{0, 0, TR2_RES_SOUND | TR2_RES_CONF, 0},
		NULL, 0, MUT_POLYMORPH, 10,
        0, 129, RACE_BLANK, orc_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
    },
    {
        "Cyclops",
        { 4, -3, -3, -3, 4, -6 },
        6, 5, 5, 6, 6, 10, 17, 14,
        13, 130,
        50, 24,
        92, 10, 255, 60,
        80,  8, 235, 60,
		{RP_CYCLOPS, 0},
		{0, 0, TR2_RES_SOUND, 0},
		NULL, 0, 0, 0,
        1, 77, RACE_BLANK, dwarf_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
    },
    {
        "Dark-Elf",
        { -1, 3, 2, 2, -2, 1 },
        15, 25, 30, 16, 26, 24, 8, 13,
        9, 150,
		75, 75,
		60,  4, 100,  6,
		54,  4, 80,  6,
		{RP_DARK_ELF, 0},
		{0, 0, TR2_RES_DARK, 0},
		dark_elf_bonuses, N_ELEMENTS(dark_elf_bonuses), 0, 0,
        5, 69, RACE_BLANK, elf_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
    },
    {
        "Draconian",
        { 2, 1, 1, 1, 2, -3 },
        8, 15, 13, 10, 12, 20, 12, 12,
        11, 250,
        75, 33,
        76,  1, 160,  5,
        72,  1, 130,  5,
		{RP_DRACONIAN, 0},
		{0, 0, 0, TR3_FEATHER},
		draconian_bonuses, N_ELEMENTS(draconian_bonuses), 0, 0,
        2, 89, RACE_BLANK, gnome_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
    },
	{
		"Dwarf",
        {  2, -2,  2, -2,  2, -3 },
        12,  19,  20,  8,  24,  20, 15,  10,
        11,  125,
		35, 15,
		48,  3, 150, 10,
		46,  3, 120, 10,
		{RP_DWARF, 0},
		{0, 0, TR2_RES_BLIND, 0},
		NULL, 0, 0, 0,
		5, 16, RACE_BLANK, dwarf_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
	},
	{
		"Elf",
        { -1,  2,  2,  1, -2,  2 },
        15,  16,  16,  14, 26,  24, 8, 15,
		8,  120,
		75, 75,
		60,  4, 100,  6,
		54,  4, 80,  6,
		{0, 0},
		{0, 0, TR2_RES_LITE, 0},
		NULL, 0, 0, 0,
		3, 7, RACE_BLANK, elf_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
	},
	{
		"Gnome",
		{ -1,  2,  0,  2,  1, -2 },
		20, 22, 22,  16, 22,  26, 7, 14,
        8,  135,
		50, 40,
		42,  3, 90,  6,
		39,  3, 75,  3,
		{RP_GNOME, 0},
		{0, 0, TR2_FREE_ACT, 0},
		NULL, 0, 0, 0,
		4, 13, RACE_BLANK, gnome_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
	},
    {
        "Golem",
        { 4, -5, -5, 0, 4, -4 },
        5, 5, 20, 8, 8, 16, 17, 10,
        12, 200,
        1, 100,
        66,  1, 200,  6,
        62,  1, 180,  6,
		{RP_GOLEM, 0},
		{TR0_AC_SK | TR0_NO_CUT | TR0_NO_STUN, 0, TR2_RES_POIS | TR2_FREE_ACT,
			TR3_SEE_INVIS | TR3_SLOW_DIGEST},
		golem_bonuses, N_ELEMENTS(golem_bonuses), 0, 0,
        4, 98, RACE_BLANK, dwarf_syllables,
		{
			{OBJ_SCROLL_SATISFY_HUNGER, 0, 0, 0, 2, 5},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
    },
	{
        "Great-One",
		{  1,  2,  2,  2,  3,  2 },
		14, 15,  15,  14, 16, 26, 15, 13,
        10,  225,
        50, 50,
		82, 5, 190, 20,
		78,  6, 180, 15,
		{RP_GREAT, RP_GREAT_2},
		{0, 0, TR2_SUST_CON, TR3_REGEN},
		NULL, 0, 0, 0,
		0, 67, RACE_BLANK, human_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
	},
	{
		"Half-Elf",
        { -1,  1,  1,  1, -1,  1 },
		12,  13,  13,  12, 22,  22, 10,  12,
		9,  110,
		24, 16,
		66,  6, 130, 15,
		62,  6, 100, 10,
		{0, 0},
		{0, 0, 0, 0},
		NULL, 0, 0, 0,
		2, 4, RACE_BLANK, elf_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
	},
    {
        "Half-Giant",
        { 4, -2, -2, -2, 3, -3 },
        4, 2, 4, 6, 8, 10, 18, 12,
        13, 150,
        40, 10,
        100,10, 255, 65,
        80, 10, 240, 64,
		{RP_HALF_GIANT, 0},
		{0, 0, TR2_SUST_STR | TR2_RES_SHARDS, 0},
		NULL, 0, 0, 0,
        3, 75, RACE_BLANK, dwarf_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
    },
    {
        "Half-Ogre",
        { 3, -1, -1, -1, 3, -3 },
        7, 5, 5, 6, 8, 10, 17, 10,
        12,  130,
        40, 10,
        92, 10, 255, 60,
        80,  8, 235, 60,
		{RP_HALF_OGRE, 0},
		{0, 0, TR2_RES_DARK | TR2_SUST_STR, 0},
		NULL, 0, 0, 0,
		3, 74, RACE_BLANK, orc_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
    },
	{
		"Half-Orc",
        {  2, -1,  0,  0,  1, -4 },
		7, 7, 7,  8,  10, 14, 14, 8,
        10,  110,
		11,  4,
		66,  1, 150,  5,
		62,  1, 120,  5,
		{RP_HALF_ORC, 0},
		{0, 0, TR2_RES_DARK, 0},
		NULL, 0, 0, 0,
		3, 19, RACE_ORC, orc_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
	},
    {
        "Half-Titan",
        { 5, 1, 1, -2, 3, 1 },
        5, 15, 12, 6, 12, 16, 18, 10,
        14, 255,
        100,30,
        111, 11, 255, 86,
        99, 11, 250, 86,
		{RP_HALF_TITAN, 0},
		{0, 0, TR2_RES_CHAOS | TR2_RES_CONF | TR2_SUST_STR, 0},
		NULL, 0, 0, 0,
        0, 76, RACE_BLANK, human_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
    },
	{
		"Half-Troll",
		{ 4, -4, -2, -4,  3, -6 },
		5, 2, 2, 6,  8, 10, 17, 7,
        12,  137,
		20, 10,
		96, 10, 250, 50,
		84,  8, 225, 40,
		{RP_HALF_TROLL, 0},
		{0, 0, TR2_SUST_STR, 0},
		half_troll_bonuses, N_ELEMENTS(half_troll_bonuses), 0, 0,
		3, 22, RACE_TROLL, orc_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
	},
	{
		"High-Elf",
        {  1,  3,  2,  3,  1,  5 },
        14,  30, 30,  18,  16, 28, 13, 18,
		10,  200,
		100, 30,
		90, 10, 190, 20,
		82, 10, 180, 15,
		{0, 0},
		{0, 0, TR2_RES_LITE, TR3_SEE_INVIS},
		NULL, 0, 0, 0,
		4, 7, RACE_BLANK, elf_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
    },
	{
		"Hobbit",
		{ -2,  2,  1,  3,  2,  1 },
        25, 28, 28, 20, 34,  30, 7, 17,
		7,  110,
		21, 12,
		36,  3, 60,  3,
		33,  3, 50,  3,
		{RP_HOBBIT, 0},
		{0, 0, TR2_SUST_DEX, 0},
		NULL, 0, 0, 0,
		4, 10, RACE_BLANK, hobbit_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
	},
	{
		"Human",
		{  0,  0,  0,  0,  0,  0 },
		10,  10,  10,  10,  10,  20,  10,  10,
		10,  100,
		14,  6,
		72,  6, 180, 25,
		66,  4, 150, 20,
		{0, 0},
		{0, 0, 0, 0},
		NULL, 0, 0, 0,
		0, 1, RACE_BLANK, human_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
	},
    {
        "Imp",
        { -1, -1, -1, 1, 2, -3 },
        7, 12, 9, 12, 8, 20, 12, 8,
        10, 110,
        13,  4,
        68,  1, 150,  5,
        64,  1, 120,  5,
		{RP_IMP, 0},
		{0, 0, TR2_RES_FIRE, 0},
		imp_bonuses, N_ELEMENTS(imp_bonuses), MUT_HORNS, 60,
        3, 94, RACE_BLANK, cthuloid_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
    },
    {
        "Klackon",
        { 2, -1, -1, 1, 2, -2 },
        20, 15, 15, 10, 8, 20, 12, 12,
        12, 135,
        20, 3,
        60,  3, 80,  4,
        54,  3, 70,  4,
		{RP_KLACKON, 0},
		{TR0_SPEED_SK, 0, TR2_RES_ACID | TR2_RES_CONF, 0},
		NULL, 0, 0, 0,
        2, 84, RACE_BLANK, klackon_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
    },
    {
        "Kobold",
        { 1, -1, 0, 1, 0, -4 },
        8, 7, 8, 8, 12, 16, 13, 7,
        9, 125,
        11,  3,
        60,  1, 130,  5,
        55,  1, 100,  5,
		{RP_KOBOLD, 0},
		{0, 0, TR2_RES_POIS, 0},
		NULL, 0, 0, 0,
        3, 82, RACE_BLANK, orc_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
    },
    {
        "Mind-Flayer",
        { -3, 4, 4, 0, -2, -5 },
        20, 35, 25, 14, 20, 24, 7, 8,
        9, 140,
        100, 25,
        68,  6, 142, 15,
        63,  6, 112, 10,
		{RP_MIND_FLAYER, 0},
		{0, 0, TR2_SUST_INT | TR2_SUST_WIS, 0},
		mind_flayer_bonuses, N_ELEMENTS(mind_flayer_bonuses),
		MUT_TENTACLES, 60,
        4, 92, RACE_BLANK, cthuloid_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
    },
    {
        "Nibelung",
        { 1, -1, 2, 0, 2, -4 },
        13, 15, 20, 12, 20, 20, 13, 10,
        11, 135,
        40, 12,
        43,  3, 92,  6,
        40,  3, 78,  3,
		{RP_NIBELUNG, 0},
		{0, 0, TR2_RES_DARK | TR2_RES_DISEN, 0},
		NULL, 0, 0, 0,
        5, 87, RACE_BLANK, dwarf_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
    },
    {
        "Skeleton",
        { 0, -2, -2, 0, 1, -4 },
        5, 5, 15, 8, 8, 16, 13, 10,
        10, 145,
        100, 35,
        72,  6, 50, 5,
        66,  4, 50, 5,
		{RP_SKELETON, 0},
		{TR0_NO_CUT, 0,
			TR2_HOLD_LIFE | TR2_RES_POIS | TR2_RES_SHARDS, TR3_SEE_INVIS},
		skeleton_bonuses, N_ELEMENTS(skeleton_bonuses), 0, 0,
        2, 102, RACE_UNDEAD, human_syllables,
		{
			{OBJ_SCROLL_SATISFY_HUNGER, 0, 0, 0, 2, 5},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
    },
    {
         "Spectre",
         { -5, 4, 4, 2, -3, -6 },
        20, 35, 30, 20, 20, 28, 5, 8,
        7, 180,
        100, 30,
        72, 6, 100, 25,
        66, 4, 100, 20,
		{RP_SPECTRE, 0},
		{TR0_NO_CUT, 0, TR2_HOLD_LIFE | TR2_RES_COLD | 
			TR2_RES_POIS | TR2_RES_NETHER, TR3_SEE_INVIS | TR3_SLOW_DIGEST},
		spectre_bonuses, N_ELEMENTS(spectre_bonuses), 0, 0,
         5, 110, RACE_UNDEAD, human_syllables,
		{
			{OBJ_SCROLL_SATISFY_HUNGER, 0, 0, 0, 2, 5},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
    },
    {
        "Sprite",
        {  -4, 3, 3, 3, -2, 2 },
        20, 20, 20, 18, 30, 20, 6, 10,
        7, 175,
        50, 25,
        32,  2, 75,  2,
        29,  2, 65,  2,
		{RP_SPRITE, 0},
		{TR0_SPEED_SK, 0, TR2_RES_LITE, TR3_FEATHER},
		NULL, 0, 0, 0,
        4, 124, RACE_BLANK, elf_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
    },
    {
         "Vampire",
         { 3, 3, -1, -1, 1, 2 },
         14, 20, 20, 18, 12, 16, 12, 10,
         11, 200,
         100, 30,
           72,  6, 180, 25,
           66,  4, 150, 20,
		{RP_VAMPIRE, 0},
		{0, 0, TR2_HOLD_LIFE | TR2_RES_COLD | TR2_RES_DARK | 
			TR2_RES_NETHER | TR2_RES_POIS, TR3_LITE},
		vampire_bonuses, N_ELEMENTS(vampire_bonuses), MUT_HYPN_GAZE, 60,
         5, 113, RACE_UNDEAD, human_syllables,
		{
			{OBJ_SCROLL_SATISFY_HUNGER, 0, 0, 0, 2, 5},
			{OBJ_SCROLL_LIGHT, 0, 0, 0, 3, 7},
			{OBJ_SCROLL_DARKNESS, 0, 0, 0, 2, 5},
		},
    },
    {
        "Yeek",
        { -2, 1, 1, 1, -2, -7 },
        12, 14, 20, 40, 20, 30, 8, 8,
        7, 100,
        14, 3,
        50,  3, 90,  6,
        50,  3, 75,  3,
		{RP_YEEK, 0},
		{0, 0, TR2_RES_ACID, 0},
		yeek_bonuses, N_ELEMENTS(yeek_bonuses), MUT_SHRIEK, 60,
        2, 78, RACE_BLANK, hobbit_syllables,
		{
			{OBJ_RATION_OF_FOOD, 0, 0, 0, 3, 7},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
    },
    {
        "Zombie",
        { 2, -6, -6, 1, 4, -5 },
        5, 5, 18, 0, 8, 10, 15, 10,
        13, 135,
        100, 30,
        72, 6, 100, 25,
        66, 4, 100, 20,
		{RP_ZOMBIE, 0},
		{0, 0, TR2_HOLD_LIFE | TR2_RES_NETHER | TR2_RES_POIS,
			TR3_SEE_INVIS | TR3_SLOW_DIGEST},
		zombie_bonuses, N_ELEMENTS(zombie_bonuses), 0, 0,
        2, 107, RACE_UNDEAD, human_syllables,
		{
			{OBJ_SCROLL_SATISFY_HUNGER, 0, 0, 0, 2, 5},
			{OBJ_WOODEN_TORCH, 0, 0, 0, 3, 7},
			{0, 0, 0, 0, 0, 0},
		},
    }
};


/*
 * Player Templates
 *
 * Title, choices, art1_bias, art2_bias, art2_chance,
 * {STR,INT,WIS,DEX,CON,CHR},
 * {toughness, mana, disarm, device, save, stealth, search, perception, close,
 *    missile, martial arts,
 *  corporis, animae, vis, naturae, necromancy, sorcery, conjuration,
 *    thaumaturgy, mindcrafting, chi,
 *  racial, stab, slash, crush, shaman, hedge, pseudo-id}
 *
 * 1/4 of the time, artifact_scroll() will use a bias derived from this template
 * in deciding which artefact to create.
 * art2_chance% of the time, this bias is art2_bias. Otherwise, it is art1_bias.
 * 0 creates an artefact via the normal process, adding biases randomly.
 */
player_template template_info[MAX_TEMPLATE] =
{
	{
		"Adventurer", 0, BIAS_ROGUE, BIAS_WARRIOR, 25,
		{ 3, 1, -1, 2, 2, -1},
		{1, 0, 5, 5, 20, 5, 5, 10, 20, 20, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 20, 20, 20, 0, 5, 20},
		{
			{OBJ_RING_RES_FEAR, 0, 0, 0, 1, 1},
			{OBJ_CUTLASS, 0, 0, 0, 1, 1},
			{OBJ_LUMP_OF_SULPHUR, 0, 0, 0, 1, 1},
			{OBJ_RING_SUSTAIN_STR, 0, 0, 0, 1, 1},
			{0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0},
		},
	},
	{
		"Swashbuckler", 0, BIAS_WARRIOR, 0, 0,
		{1, 0, -2, 3, 2, 1},
		{1, 0, 5, 5, 15, 5, 5, 15, 15, 15, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 35, 15, 15, 0, 0, 30},
		{
			{OBJ_POTION_SPEED, 0, 0, 0, 1, 1},
			{OBJ_RAPIER, 0, 0, 0, 1, 1},
			{OBJ_HARD_LEATHER_ARMOUR, 0, 0, 0, 1, 1},
			{0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0},
		},
	},
	{
		"Gladiator", 0, BIAS_WARRIOR, 0, 0,
		{5, -1, -1, 1, 3, -1},
		{2, 0, 5, 5, 10, 5, 5, 10, 30, 10, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 30, 30, 30, 0, 0, 40},
		{
			{OBJ_RING_FREE_ACTION, 0, 0, 0, 1, 1},
			{OBJ_BROAD_SWORD, 0, 0, 0, 1, 1},
			{OBJ_SMALL_METAL_SHIELD, 0, 0, 0, 1, 1},
			{OBJ_RING_RES_FEAR, 0, 0, 0, 1, 1},
			{0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0},
		},
	},
	{
		"Warrior-Monk", 0, BIAS_WARRIOR, 0, 0,
		{ 2, 0, -2, 5, 1, -1},
		{1, 0, 5, 5, 10, 15, 5, 15, 35, 10, 4,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 10, 10, 10, 0, 0, 30},
		{
			{OBJ_RING_SUSTAIN_DEX, 0, 0, 0, 1, 1},
			{OBJ_SCROLL_MONSTER_CONFUSION, 0, 0, 0, 1, 1},
			{OBJ_SOFT_LEATHER_ARMOUR, 0, 0, 0, 1, 1},
			{OBJ_RING_SUSTAIN_STR, 0, 0, 0, 1, 1},
			{0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0},
		},
	},
	{
		"Zen-Monk", 0, BIAS_PRIESTLY, 0, 40,
		{ 2, -1, 2, 2, 0, -1},
		{0, 0, 5, 5, 10, 10, 10, 15, 25, 10, 2,
		0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
		0, 10, 10, 10, 0, 0, 40},
		{
			{OBJ_RING_SUSTAIN_WIS, 0, 0, 0, 1, 1},
			{OBJ_SOFT_LEATHER_ARMOUR, 0, 0, 0, 1, 1},
			{OBJ_SCROLL_MONSTER_CONFUSION, 0, 0, 0, 1, 1},
			{OBJ_RING_SUSTAIN_CON, 0, 0, 0, 1, 1},
			{0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0},
		},
	},
	{
		"Assassin", 0, BIAS_ROGUE, BIAS_WARRIOR, 25,
		{ 1, 0, 0, 3, 2, -2},
		{0, 0, 20, 5, 10, 30, 20, 20, 30, 10, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 20, 20, 20, 0, 0, 30},
 		{
			{OBJ_RING_RES_POISON, 0, 0, 0, 1, 1},
			{OBJ_DAGGER, EGO_BRAND_POIS, EI_EGO, 0, 1, 1},
			{OBJ_SOFT_LEATHER_ARMOUR, 0, 0, 0, 1, 1},
			{OBJ_RING_RES_DISENCHANTMENT, 0, 0, 0, 1, 1},
			{0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0},
		},
	},
	{
		"Ranger", 0, BIAS_RANGER, BIAS_WARRIOR, 30,
		{ 2, -1, -1, 3, 3, 2},
		{1, 0, 10, 5, 10, 20, 20, 30, 15, 35, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 15, 15, 15, 2, 0, 10},
		{
			{OBJ_LONG_BOW, 0, 0, 0, 1, 1},
			{OBJ_ARROW, 0, 0, 0, 15, 45},
			{OBJ_HARD_LEATHER_ARMOUR, 0, 0, 0, 1, 1},
			{0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0},
		},
	},
	{
		"Shaman", 0, BIAS_PRIESTLY, 0, 0,
		{ -1, 1, 1, -2, 2, 5},
		{0, 0, 5, 10, 20, 5, 10, 15, 15, 15, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 15, 15, 15, 5, 0, 20},
		{
			{OBJ_QUARTERSTAFF, 0, 0, 0, 1, 1},
			{OBJ_POTION_HEALING, 0, 0, 0, 1, 1},
			{OBJ_SCROLL_PROTECTION_FROM_EVIL, 0, 0, 0, 1, 1},
			{0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0},
		},
    },
	{
		"Mindcrafter", 0, BIAS_PRIESTLY, 0, 40,
		{ -2, 2, 5, -1, 0, 1},
		{0, 0, 5, 10, 15, 5, 10, 15, 15, 15, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 4, 2,
		0, 15, 15, 15, 0, 0, 15},
		{
			{OBJ_RING_SUSTAIN_WIS, 0, 0, 0, 1, 1},
			{OBJ_SHORT_SWORD, 0, 0, 0, 1, 1},
			{OBJ_SOFT_LEATHER_ARMOUR, 0, 0, 0, 1, 1},
			{OBJ_RING_RES_CONFUSION, 0, 0, 0, 1, 1},
			{0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0},
		},
	},
	{
		"Wizard", 3, BIAS_MAGE, 0, 0,
		{-2, 5, 2, 1, -1, 1},
		{0, 4, 5, 25, 20, 5, 10, 15, 10, 10, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 10, 10, 10, 0, 0, 5},
		{

			{OBJ_RING_SUSTAIN_INT, 0, 0, 0, 1, 1},
			{OBJ_POTION_RES_MANA, 0, 0, 0, 1, 1},
			{OBJ_SOFT_LEATHER_ARMOUR, 0, 0, 0, 1, 1},
			{OBJ_RING_RES_LIGHT_AND_DARKNESS, 0, 0, 0, 1, 1},
			{0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0},
		},
	},
	{
		"Warlock", 1, BIAS_MAGE, 0, 0,
		{1, 3, -1, 0, 2, -1},
		{1, 2, 5, 10, 15, 5, 10, 10, 15, 10, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 15, 15, 20, 0, 0, 10},
		{
			{OBJ_RING_SUSTAIN_INT, 0, 0, 0, 1, 1},
			{OBJ_SMALL_SWORD, 0, 0, 0, 1, 1},
			{OBJ_SOFT_LEATHER_ARMOUR, 0, 0, 0, 1, 1},
			{OBJ_RING_SUSTAIN_STR, 0, 0, 0, 1, 1},
			{0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0},
		},
	},
	{
		"Powerweaver", 2, BIAS_MAGE, 0, 0,
		{-3, 3, 3, -2, -2, 3},
		{0, 2, 5, 15, 15, 5, 10, 10, 5, 5, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 3, 2,
		0, 5, 5, 5, 3, 0, 10},
		{
			{OBJ_RING_SUSTAIN_INT, 0, 0, 0, 1, 1},
			{OBJ_RING_SUSTAIN_WIS, 0, 0, 0, 1, 1},
			{OBJ_POTION_RES_MANA, 0, 0, 0, 1, 1},
			{OBJ_RING_SEE_INVIS, 0, 0, 0, 1, 1},
			{OBJ_RING_RES_LIGHT_AND_DARKNESS, 0, 0, 0, 1, 1},
			{0, 0, 0, 0, 0, 0},
		},
	},
	{
		"Tourist", 0, BIAS_ROGUE, BIAS_WARRIOR, 25,
		{0, 0, 0, 0, 2, -1},
		{0, 0, 5, 5, 5, 5, 5, 5, 10, 10, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 10, 10, 10, 0, 0, 20},
		{
			{OBJ_DAGGER, 0, 0, 0, 1, 1},
			{OBJ_HARD_LEATHER_BOOTS, 0, 0, 0, 1, 1},
			{OBJ_CLOAK, 0, 0, 0, 1, 1},
			{0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0},
		},
	},
};




/*
 * Hack -- the spell information tables.
 *
 *   Array of { Lev, Mana, Fail, Exp/Lev, school, type }
 */
static magic_type sorcery_spells[32] =
{
	{"Detect Monsters", "", 1, 1, 23, 4, SKILL_SORCERY, SKILL_ANIMAE, SP_DETECT_MONSTERS, 0},
	{"Phase Door", "range 10", 1, 2, 24, 4, SKILL_SORCERY, SKILL_CORPORIS, SP_PHASE_DOOR, 0},
	{"Detect Doors and Traps", "", 3, 3, 25, 1, SKILL_SORCERY, SKILL_NATURAE, SP_DETECT_DOORS_AND_TRAPS, 0},
	{"Light Area", "dam LEV/2+10", 3, 3, 30, 1, SKILL_SORCERY, SKILL_VIS, SP_LIGHT_AREA, 0},
	{"Confuse Monster", "", 4, 4, 30, 1, SKILL_SORCERY, SKILL_ANIMAE, SP_CONFUSE_MONSTER, 0},
	{"Teleport", "range LEV*5", 5, 5, 35, 5, SKILL_SORCERY, SKILL_CORPORIS, SP_TELEPORT, 0},
	{"Sleep Monster", "", 6, 5, 30, 4, SKILL_SORCERY, SKILL_ANIMAE, SP_SLEEP_MONSTER, 0},
	{"Recharging", "", 7, 7, 75, 9, SKILL_SORCERY, SKILL_VIS, SP_RECHARGING, 0},
	{"Magic Mapping", "", 9, 7, 75, 8, SKILL_SORCERY, SKILL_NATURAE, SP_MAGIC_MAPPING, 0},
	{"Identify", "", 10, 7, 75, 8, SKILL_SORCERY, SKILL_NATURAE, SP_IDENTIFY, 0},
	{"Slow Monster", "", 11, 7, 75, 7, SKILL_SORCERY, SKILL_CORPORIS, SP_SLOW_MONSTER, 0},
	{"Mass Sleep", "", 13, 7, 50, 6, SKILL_SORCERY, SKILL_ANIMAE, SP_MASS_SLEEP, 0},
	{"Teleport Away", "", 18, 12, 60, 8, SKILL_SORCERY, SKILL_CORPORIS, SP_TELEPORT_AWAY, 0},
	{"Haste Self", "dur LEV;+dLEV+20", 22, 12, 60, 8, SKILL_SORCERY, SKILL_CORPORIS, SP_HASTE_SELF, 0},
	{"Detection True", "", 28, 20, 70, 15, SKILL_SORCERY, SKILL_ANIMAE, SP_DETECTION_TRUE, 0},
	{"Identify True", "", 33, 30, 75, 20, SKILL_SORCERY, SKILL_NATURAE, SP_IDENTIFY_TRUE, 0},
	{"Detect Objects & Treasure", "", 3, 3, 25, 15, SKILL_SORCERY, SKILL_NATURAE, SP_DETECT_OBJECTS_AND_TREASURE, 0},
	{"Detect Enchantment", "", 10, 10, 70, 40, SKILL_SORCERY, SKILL_VIS, SP_DETECT_ENCHANTMENT, 0},
	{"Charm Monster", "", 10, 10, 80, 40, SKILL_SORCERY, SKILL_ANIMAE, SP_CHARM_MONSTER, 0},
	{"Dimension Door", "range LEV+2", 12, 12, 80, 40, SKILL_SORCERY, SKILL_NATURAE, SP_DIMENSION_DOOR, 0},
	{"Sense Minds", "dur 25+d30", 14, 10, 60, 25, SKILL_SORCERY, SKILL_ANIMAE, SP_SENSE_MINDS, 0},
	{"Self Knowledge", "", 20, 18, 85, 50, SKILL_SORCERY, SKILL_ANIMAE, SP_SELF_KNOWLEDGE, 0},
	{"Teleport Level", "", 20, 18, 60, 25, SKILL_SORCERY, SKILL_CORPORIS, SP_TELEPORT_LEVEL, 0},
	{"Word of Recall", "delay 15+d21", 25, 25, 75, 19, SKILL_SORCERY, SKILL_CORPORIS, SP_WORD_OF_RECALL, 0},
	{"Stasis", "", 10, 10, 40, 20, SKILL_SORCERY, SKILL_VIS, SP_STASIS, 0},
	{"Telekinesis", "max wgt LEV*15/10;.LEV*15%10", 25, 25, 75, 70, SKILL_SORCERY, SKILL_NATURAE, SP_TELEKINESIS, 0},
	{"Explosive Rune", "dam 7d7+LEV/2", 25, 30, 95, 160, SKILL_SORCERY, SKILL_VIS, SP_EXPLOSIVE_RUNE, 0},
	{"Clairvoyance", "dur 25+d30", 30, 40, 80, 120, SKILL_SORCERY, SKILL_NATURAE, SP_CLAIRVOYANCE, 0},
	{"Enchant Weapon", "", 40, 80, 95, 200, SKILL_SORCERY, SKILL_VIS, SP_ENCHANT_WEAPON, 0},
	{"Enchant Armour", "", 40, 100, 95, 200, SKILL_SORCERY, SKILL_VIS, SP_ENCHANT_ARMOUR, 0},
	{"Alchemy", "", 42, 50, 90, 175, SKILL_SORCERY, SKILL_NATURAE, SP_ALCHEMY, 0},
	{"Globe of Invulnerability", "dur 8+d8", 45, 70, 75, 250, SKILL_SORCERY, SKILL_ANIMAE, SP_GLOBE_OF_INVULNERABILITY, 0},
};

static magic_type thaumaturgy_info[32] =
{
	{"Magic Missile", "dam LEV-1/5+3;d4", 1, 1, 20, 4, SKILL_THAUMATURGY, SKILL_NATURAE, SP_MAGIC_MISSILE, 0},
	{"Trap / Door Destruction", "", 1, 2, 22, 4, SKILL_THAUMATURGY, SKILL_NATURAE, SP_TRAP_DOOR_DESTRUCTION, 0},
	{"Flash of Light", "dam LEV/2+10", 2, 2, 25, 4, SKILL_THAUMATURGY, SKILL_VIS, SP_FLASH_OF_LIGHT, 0},
	{"Touch of Confusion", "", 5, 5, 30, 1, SKILL_THAUMATURGY, SKILL_ANIMAE, SP_TOUCH_OF_CONFUSION, 0},
	{"Mana Burst", "dam 3d5+LEV*5/4", 9, 6, 50, 1, SKILL_THAUMATURGY, SKILL_VIS, SP_MANA_BURST, 0},
	{"Fire Bolt", "dam LEV-5/4+6;d8", 13, 9, 45, 6, SKILL_THAUMATURGY, SKILL_VIS, SP_FIRE_BOLT, 0},
	{"Fist of Force", "dam LEV-5/4+8;d8", 14, 9, 45, 6, SKILL_THAUMATURGY, SKILL_VIS, SP_FIST_OF_FORCE, 0},
	{"Teleport Self", "range LEV*5", 15, 9, 35, 5, SKILL_THAUMATURGY, SKILL_CORPORIS, SP_TELEPORT_SELF, 0},
	{"Wonder", "random", 17, 10, 25, 5, SKILL_THAUMATURGY, SKILL_VIS, SP_WONDER, 0},
	{"Chaos Bolt", "dam LEV-5/4+10;d8", 19, 12, 45, 9, SKILL_THAUMATURGY, SKILL_VIS, SP_CHAOS_BOLT, 0},
	{"Sonic Boom", "dam LEV+45", 21, 13, 45, 10, SKILL_THAUMATURGY, SKILL_VIS, SP_SONIC_BOOM, 0},
	{"Doom Bolt", "dam LEV-5/4+11;d8", 23, 15, 50, 11, SKILL_THAUMATURGY, SKILL_VIS, SP_DOOM_BOLT, 0},
	{"Fire Ball", "dam LEV+55", 25, 16, 50, 12, SKILL_THAUMATURGY, SKILL_VIS, SP_FIRE_BALL, 0},
	{"Teleport Other", "", 25, 18, 60, 8, SKILL_THAUMATURGY, SKILL_CORPORIS, SP_TELEPORT_OTHER, 0},
	{"Word of Destruction", "", 30, 20, 80, 15, SKILL_THAUMATURGY, SKILL_ANIMAE, SP_WORD_OF_DESTRUCTION, 0},
	{"Invoke Chaos", "dam LEV+66", 35, 40, 85, 40, SKILL_THAUMATURGY, SKILL_NATURAE, SP_INVOKE_CHAOS, 0},
	{"Polymorph Other", "", 11, 7, 45, 9, SKILL_THAUMATURGY, SKILL_CORPORIS, SP_POLYMORPH_OTHER, 0},
	{"Chain Lightning", "dam LEV/10+5;d8", 15, 15, 80, 35, SKILL_THAUMATURGY, SKILL_VIS, SP_CHAIN_LIGHTNING, 0},
	{"Arcane Binding", "", 16, 14, 80, 35, SKILL_THAUMATURGY, SKILL_NATURAE, SP_ARCANE_BINDING, 0},
	{"Disintegrate", "dam LEV+80", 25, 25, 85, 100, SKILL_THAUMATURGY, SKILL_NATURAE, SP_DISINTEGRATE, 0},
	{"Alter Reality", "", 30, 25, 85, 150, SKILL_THAUMATURGY, SKILL_ANIMAE, SP_ALTER_REALITY, 0},
	{"Polymorph Self", "", 42, 50, 85, 250, SKILL_THAUMATURGY, SKILL_CORPORIS, SP_POLYMORPH_SELF, 0},
	{"Chaos Branding", "", 45, 90, 80, 250, SKILL_THAUMATURGY, SKILL_NATURAE, SP_CHAOS_BRANDING, 0},
	{"Summon Demon", "", 47, 100, 90, 250, SKILL_THAUMATURGY, SKILL_ANIMAE, SP_SUMMON_DEMON, 0},
	{"Beam of Gravity", "dam LEV/10+9;d8", 20, 20, 66, 8, SKILL_THAUMATURGY, SKILL_NATURAE, SP_BEAM_OF_GRAVITY, 0},
	{"Meteor Swarm", "dam LEV*3/2; each", 35, 32, 85, 35, SKILL_THAUMATURGY, SKILL_NATURAE, SP_METEOR_SWARM, 0},
	{"Flame Strike", "dam LEV+75", 37, 34, 75, 40, SKILL_THAUMATURGY, SKILL_VIS, SP_FLAME_STRIKE, 0},
	{"Call Chaos", "dam 75 / 150", 41, 42, 85, 100, SKILL_THAUMATURGY, SKILL_VIS, SP_CALL_CHAOS, 0},
	{"Shard Ball", "dam LEV+120", 43, 44, 80, 150, SKILL_THAUMATURGY, SKILL_NATURAE, SP_SHARD_BALL, 0},
	{"Mana Storm", "dam LEV*2+300", 45, 48, 85, 200, SKILL_THAUMATURGY, SKILL_VIS, SP_MANA_STORM, 0},
	{"Breathe Chaos", "dam CHP", 47, 75, 80, 200, SKILL_THAUMATURGY, SKILL_CORPORIS, SP_BREATHE_CHAOS, 0},
	{"Call the Void", "dam 3*175", 49, 100, 85, 250, SKILL_THAUMATURGY, SKILL_ANIMAE, SP_CALL_THE_VOID, 0},
};

static magic_type conjuration_spells[32] =
{
	{"Phase Door", "range 10", 1, 1, 50, 3, SKILL_CONJURATION, SKILL_CORPORIS, SP_PHASE_DOOR, 0},
	{"Mind Blast", "dam LEV-1/5+3;d3", 3, 3, 50, 4, SKILL_CONJURATION, SKILL_ANIMAE, SP_MIND_BLAST, 0},
	{"Tarot Draw", "random", 5, 5, 75, 8, SKILL_CONJURATION, SKILL_VIS, SP_TAROT_DRAW, 0},
	{"Reset Recall", "", 6, 6, 80, 8, SKILL_CONJURATION, SKILL_ANIMAE, SP_RESET_RECALL, 0},
	{"Teleport", "range LEV*4", 7, 7, 40, 4, SKILL_CONJURATION, SKILL_CORPORIS, SP_TELEPORT, 0},
	{"Dimension Door", "range LEV+2", 9, 9, 60, 6, SKILL_CONJURATION, SKILL_NATURAE, SP_DIMENSION_DOOR, 0},
	{"Planar Spying", "dur 25+d30", 14, 12, 60, 6, SKILL_CONJURATION, SKILL_VIS, SP_PLANAR_SPYING, 0},
	{"Teleport Away", "", 17, 15, 60, 5, SKILL_CONJURATION, SKILL_CORPORIS, SP_TELEPORT_AWAY, 0},
	{"Summon Object", "max wgt LEV*15/10;.LEV*15%10", 20, 20, 80, 8, SKILL_CONJURATION, SKILL_NATURAE, SP_SUMMON_OBJECT, 0},
	{"Summon Animal", "", 24, 22, 60, 8, SKILL_CONJURATION, SKILL_ANIMAE, SP_SUMMON_ANIMAL, 0},
	{"Phantasmal Servant", "", 28, 24, 60, 8, SKILL_CONJURATION, SKILL_VIS, SP_PHANTASMAL_SERVANT, 0},
	{"Summon Monster", "", 30, 25, 70, 9, SKILL_CONJURATION, SKILL_ANIMAE, SP_SUMMON_MONSTER, 0},
	{"Conjure Elemental", "", 33, 28, 80, 12, SKILL_CONJURATION, SKILL_NATURAE, SP_CONJURE_ELEMENTAL, 0},
	{"Teleport Level", "", 35, 30, 70, 10, SKILL_CONJURATION, SKILL_CORPORIS, SP_TELEPORT_LEVEL, 0},
	{"Word of Recall", "delay 15+d21", 40, 35, 80, 15, SKILL_CONJURATION, SKILL_CORPORIS, SP_WORD_OF_RECALL, 0},
	{"Banish", "", 42, 40, 70, 12, SKILL_CONJURATION, SKILL_ANIMAE, SP_BANISH, 0},
	{"Joker Card", "", 15, 15, 80, 20, SKILL_CONJURATION, SKILL_VIS, SP_JOKER_CARD, 0},
	{"Summon Spiders", "", 24, 24, 70, 25, SKILL_CONJURATION, SKILL_ANIMAE, SP_SUMMON_SPIDERS, 0},
	{"Summon Reptiles", "", 26, 26, 70, 30, SKILL_CONJURATION, SKILL_ANIMAE, SP_SUMMON_REPTILES, 0},
	{"Summon Hounds", "", 30, 30, 70, 35, SKILL_CONJURATION, SKILL_ANIMAE, SP_SUMMON_HOUNDS, 0},
	{"Planar Branding", "", 35, 70, 80, 100, SKILL_CONJURATION, SKILL_VIS, SP_PLANAR_BRANDING, 0},
	{"Planar Being", "", 40, 100, 90, 250, SKILL_CONJURATION, SKILL_CORPORIS, SP_PLANAR_BEING, 0},
	{"Death Dealing", "dam LEV*3", 42, 50, 50, 75, SKILL_CONJURATION, SKILL_VIS, SP_DEATH_DEALING, 0},
	{"Summon Reaver", "", 45, 100, 90, 200, SKILL_CONJURATION, SKILL_ANIMAE, SP_SUMMON_REAVER, 0},
	{"Planar Divination", "", 30, 30, 60, 50, SKILL_CONJURATION, SKILL_ANIMAE, SP_PLANAR_DIVINATION, 0},
	{"Planar Lore", "", 35, 50, 90, 100, SKILL_CONJURATION, SKILL_ANIMAE, SP_PLANAR_LORE, 0},
	{"Summon Undead", "", 36, 80, 80, 150, SKILL_CONJURATION, SKILL_ANIMAE, SP_SUMMON_UNDEAD, 0},
	{"Summon Dragon", "", 39, 80, 80, 150, SKILL_CONJURATION, SKILL_ANIMAE, SP_SUMMON_DRAGON, 0},
	{"Mass Summons", "", 42, 100, 80, 200, SKILL_CONJURATION, SKILL_ANIMAE, SP_MASS_SUMMONS, 0},
	{"Summon Demon", "", 47, 100, 80, 150, SKILL_CONJURATION, SKILL_ANIMAE, SP_SUMMON_DEMON, 0},
	{"Summon Ancient Dragon", "", 48, 100, 80, 200, SKILL_CONJURATION, SKILL_ANIMAE, SP_SUMMON_ANCIENT_DRAGON, 0},
	{"Summon Greater Undead", "", 49, 100, 80, 220, SKILL_CONJURATION, SKILL_ANIMAE, SP_SUMMON_GREATER_UNDEAD, 0},
};

static magic_type necromancy_spells[32] =
{
	{"Detect Unlife", "", 1, 1, 25, 4, SKILL_NECROMANCY, SKILL_ANIMAE, SP_DETECT_UNLIFE, 0},
	{"Malediction", "dam LEV-1/5+3;d3", 2, 2, 25, 4, SKILL_NECROMANCY, SKILL_VIS, SP_MALEDICTION, 0},
	{"Detect Evil", "", 2, 2, 25, 4, SKILL_NECROMANCY, SKILL_ANIMAE, SP_DETECT_EVIL, 0},
	{"Stinking Cloud", "dam LEV/2+10", 3, 3, 27, 3, SKILL_NECROMANCY, SKILL_NATURAE, SP_STINKING_CLOUD, 0},
	{"Black Sleep", "", 5, 5, 30, 4, SKILL_NECROMANCY, SKILL_ANIMAE, SP_BLACK_SLEEP, 0},
	{"Resist Poison", "dur 20+d20", 7, 10, 75, 6, SKILL_NECROMANCY, SKILL_CORPORIS, SP_RESIST_POISON, 0},
	{"Horrify", "", 9, 9, 30, 4, SKILL_NECROMANCY, SKILL_CORPORIS, SP_HORRIFY, 0},
	{"Enslave Undead", "", 10, 10, 30, 4, SKILL_NECROMANCY, SKILL_ANIMAE, SP_ENSLAVE_UNDEAD, 0},
	{"Orb of Entropy", "dam 3d6+LEV*5/4", 12, 12, 40, 5, SKILL_NECROMANCY, SKILL_VIS, SP_ORB_OF_ENTROPY, 0},
	{"Nether Bolt", "dam LEV-5/4+6;d8", 13, 12, 30, 4, SKILL_NECROMANCY, SKILL_VIS, SP_NETHER_BOLT, 0},
	{"Terror", "", 18, 15, 50, 10, SKILL_NECROMANCY, SKILL_ANIMAE, SP_TERROR, 0},
	{"Vampiric Drain", "dam LEV/15+2;* 5+d15", 23, 20, 60, 16, SKILL_NECROMANCY, SKILL_CORPORIS, SP_VAMPIRIC_DRAIN, 0},
	{"Poison Branding", "", 30, 75, 50, 30, SKILL_NECROMANCY, SKILL_NATURAE, SP_POISON_BRANDING, 0},
	{"Dispel Good", "dam LEV*4", 33, 35, 60, 16, SKILL_NECROMANCY, SKILL_VIS, SP_DISPEL_GOOD, 0},
	{"Genocide", "", 37, 25, 95, 25, SKILL_NECROMANCY, SKILL_CORPORIS, SP_GENOCIDE, 0},
	{"Restore Life", "", 45, 50, 95, 150, SKILL_NECROMANCY, SKILL_CORPORIS, SP_RESTORE_LIFE, 0},
	{"Berserk", "dur 25+d25", 10, 20, 80, 180, SKILL_NECROMANCY, SKILL_CORPORIS, SP_BERSERK, 0},
	{"Invoke Spirits", "random", 10, 15, 80, 30, SKILL_NECROMANCY, SKILL_ANIMAE, SP_INVOKE_SPIRITS, 0},
	{"Dark Bolt", "dam LEV-5/4+4", 11, 11, 30, 15, SKILL_NECROMANCY, SKILL_VIS, SP_DARK_BOLT, 0},
	{"Battle Frenzy", "max dur 50", 30, 25, 75, 50, SKILL_NECROMANCY, SKILL_CORPORIS, SP_BATTLE_FRENZY, 0},
	{"Vampirism True", "dam 3*100", 33, 35, 60, 125, SKILL_NECROMANCY, SKILL_VIS, SP_VAMPIRISM_TRUE, 0},
	{"Vampiric Branding", "", 33, 90, 70, 90, SKILL_NECROMANCY, SKILL_NATURAE, SP_VAMPIRIC_BRANDING, 0},
	{"Darkness Storm", "dam 120", 40, 40, 70, 200, SKILL_NECROMANCY, SKILL_VIS, SP_DARKNESS_STORM, 0},
	{"Mass Genocide", "", 40, 75, 80, 100, SKILL_NECROMANCY, SKILL_CORPORIS, SP_MASS_GENOCIDE, 0},
	{"Death Ray", "", 20, 20, 75, 50, SKILL_NECROMANCY, SKILL_VIS, SP_DEATH_RAY, 0},
	{"Raise the Dead", "", 25, 66, 95, 250, SKILL_NECROMANCY, SKILL_CORPORIS, SP_RAISE_THE_DEAD, 0},
	{"Esoteria", "", 30, 40, 95, 250, SKILL_NECROMANCY, SKILL_NATURAE, SP_ESOTERIA, 0},
	{"Word of Death", "dam LEV*3", 33, 35, 70, 40, SKILL_NECROMANCY, SKILL_ANIMAE, SP_WORD_OF_DEATH, 0},
	{"Evocation", "dam LEV*4", 37, 35, 80, 70, SKILL_NECROMANCY, SKILL_VIS, SP_EVOCATION, 0},
	{"Hellfire", "dam 666", 42, 120, 95, 250, SKILL_NECROMANCY, SKILL_VIS, SP_HELLFIRE, 0},
	{"Omnicide", "", 45, 100, 90, 250, SKILL_NECROMANCY, SKILL_CORPORIS, SP_OMNICIDE, 0},
	{"Wraithform", "dur LEV/2;+dLEV/2", 47, 100, 90, 250, SKILL_NECROMANCY, SKILL_CORPORIS, SP_WRAITHFORM, 0},
};

static magic_type life_spells[32] =
{
	{"Detect Evil", "", 1, 1, 10, 0, SKILL_SHAMAN, SKILL_NONE, SP_DETECT_EVIL, 0},
	{"Cure Light Wounds", "heal 2d10", 1, 2, 15, 0, SKILL_SHAMAN, SKILL_NONE, SP_CURE_LIGHT_WOUNDS, 0},
	{"Bless", "dur 12+d12 turns", 1, 2, 20, 0, SKILL_SHAMAN, SKILL_NONE, SP_BLESS, 0},
	{"Remove Fear", "", 3, 2, 25, 0, SKILL_SHAMAN, SKILL_NONE, SP_REMOVE_FEAR, 0},
	{"Call Light", "dam LEV/2+10", 3, 3, 27, 0, SKILL_SHAMAN, SKILL_NONE, SP_CALL_LIGHT, 0},
	{"Detect Traps and Secret Doors", "", 4, 4, 28, 0, SKILL_SHAMAN, SKILL_NONE, SP_DETECT_TRAPS_AND_SECRET_DOORS, 0},
	{"Cure Medium Wounds", "heal 4d10", 5, 4, 32, 0, SKILL_SHAMAN, SKILL_NONE, SP_CURE_MEDIUM_WOUNDS, 0},
	{"Satisfy Hunger", "", 7, 5, 38, 0, SKILL_SHAMAN, SKILL_NONE, SP_SATISFY_HUNGER, 0},
	{"Remove Curse", "", 7, 6, 38, 0, SKILL_SHAMAN, SKILL_NONE, SP_REMOVE_CURSE, 0},
	{"Cure Poison", "", 9, 6, 38, 0, SKILL_SHAMAN, SKILL_NONE, SP_CURE_POISON, 0},
	{"Cure Critical Wounds", "heal 8d10", 9, 7, 40, 0, SKILL_SHAMAN, SKILL_NONE, SP_CURE_CRITICAL_WOUNDS, 0},
	{"Sense Unseen", "dur 24+d24", 10, 8, 38, 0, SKILL_SHAMAN, SKILL_NONE, SP_SENSE_UNSEEN, 0},
	{"Holy Orb", "dam 3d6+LEV*5/4", 10, 8, 40, 0, SKILL_SHAMAN, SKILL_NONE, SP_HOLY_ORB, 0},
	{"Protection from Evil", "dur d25+LEV*3", 11, 8, 42, 0, SKILL_SHAMAN, SKILL_NONE, SP_PROTECTION_FROM_EVIL, 0},
	{"Healing", "heal 300", 20, 16, 60, 0, SKILL_SHAMAN, SKILL_NONE, SP_HEALING, 0},
	{"Glyph of Warding", "", 33, 55, 90, 0, SKILL_SHAMAN, SKILL_NONE, SP_GLYPH_OF_WARDING, 0},
	{"Exorcism", "dam LEV;+LEV", 15, 14, 50, 0, SKILL_SHAMAN, SKILL_NONE, SP_EXORCISM, 0},
	{"Dispel Curse", "", 16, 14, 80, 0, SKILL_SHAMAN, SKILL_NONE, SP_DISPEL_CURSE, 0},
	{"Dispel Undead & Demons", "dam LEV*3;+LEV*3", 17, 14, 55, 0, SKILL_SHAMAN, SKILL_NONE, SP_DISPEL_UNDEAD_AND_DEMONS, 0},
	{"Day of the Dove", "", 24, 20, 55, 0, SKILL_SHAMAN, SKILL_NONE, SP_DAY_OF_THE_DOVE, 0},
	{"Dispel Evil", "dam LEV*4", 25, 20, 70, 0, SKILL_SHAMAN, SKILL_NONE, SP_DISPEL_EVIL, 0},
	{"Banish", "", 25, 25, 80, 0, SKILL_SHAMAN, SKILL_NONE, SP_BANISH, 0},
	{"Holy Word", "d LEV*4;/h 1000", 39, 32, 95, 0, SKILL_SHAMAN, SKILL_NONE, SP_HOLY_WORD, 0},
	{"Warding True", "", 44, 44, 80, 0, SKILL_SHAMAN, SKILL_NONE, SP_WARDING_TRUE, 0},
	{"Heroism", "dur 25+d25", 5, 5, 50, 0, SKILL_SHAMAN, SKILL_NONE, SP_HEROISM, 0},
	{"Prayer", "dur 48+d48", 15, 14, 50, 0, SKILL_SHAMAN, SKILL_NONE, SP_PRAYER, 0},
	{"Bless Weapon", "", 30, 50, 80, 0, SKILL_SHAMAN, SKILL_NONE, SP_BLESS_WEAPON, 0},
	{"Restoration", "", 35, 70, 90, 0, SKILL_SHAMAN, SKILL_NONE, SP_RESTORATION, 0},
	{"Healing True", "heal 2000", 40, 50, 80, 0, SKILL_SHAMAN, SKILL_NONE, SP_HEALING_TRUE, 0},
	{"Holy Vision", "", 40, 40, 80, 0, SKILL_SHAMAN, SKILL_NONE, SP_HOLY_VISION, 0},
	{"Divine Intervention", "h300/dLEV*4;+388", 42, 90, 85, 0, SKILL_SHAMAN, SKILL_NONE, SP_DIVINE_INTERVENTION, 0},
	{"Holy Invulnerability", "dur 7+d7", 45, 90, 85, 0, SKILL_SHAMAN, SKILL_NONE, SP_HOLY_INVULNERABILITY, 0},
};

static magic_type wild_spells[32] =
{
	{"Detect Creatures", "", 1, 1, 23, 0, SKILL_SHAMAN, SKILL_NONE, SP_DETECT_CREATURES, 0},
	{"First Aid", "heal 2d8", 3, 3, 25, 0, SKILL_SHAMAN, SKILL_NONE, SP_FIRST_AID, 0},
	{"Detect Doors and Traps", "", 3, 3, 25, 0, SKILL_SHAMAN, SKILL_NONE, SP_DETECT_DOORS_AND_TRAPS, 0},
	{"Foraging", "", 4, 4, 35, 0, SKILL_SHAMAN, SKILL_NONE, SP_FORAGING, 0},
	{"Daylight", "dam LEV/2+10", 4, 4, 50, 0, SKILL_SHAMAN, SKILL_NONE, SP_DAYLIGHT, 0},
	{"Animal Taming", "", 4, 5, 50, 0, SKILL_SHAMAN, SKILL_NONE, SP_ANIMAL_TAMING, 0},
	{"Resist Environment", "dur 20+d20", 5, 5, 50, 0, SKILL_SHAMAN, SKILL_NONE, SP_RESIST_ENVIRONMENT, 0},
	{"Cure Wounds & Poison", "", 5, 5, 35, 0, SKILL_SHAMAN, SKILL_NONE, SP_CURE_WOUNDS_AND_POISON, 0},
	{"Stone to Mud", "", 5, 5, 40, 0, SKILL_SHAMAN, SKILL_NONE, SP_STONE_TO_MUD, 0},
	{"Lightning Bolt", "dam LEV-5/4+3;d8", 5, 5, 30, 0, SKILL_SHAMAN, SKILL_NONE, SP_LIGHTNING_BOLT, 0},
	{"Nature Awareness", "", 7, 6, 45, 0, SKILL_SHAMAN, SKILL_NONE, SP_NATURE_AWARENESS, 0},
	{"Frost Bolt", "dam LEV-5/4+5;d8", 7, 6, 40, 0, SKILL_SHAMAN, SKILL_NONE, SP_FROST_BOLT, 0},
	{"Ray of Sunlight", "dam 6d8", 9, 6, 30, 0, SKILL_SHAMAN, SKILL_NONE, SP_RAY_OF_SUNLIGHT, 0},
	{"Entangle", "", 19, 12, 55, 0, SKILL_SHAMAN, SKILL_NONE, SP_ENTANGLE, 0},
	{"Summon Animal", "", 25, 25, 90, 0, SKILL_SHAMAN, SKILL_NONE, SP_SUMMON_ANIMAL, 0},
	{"Herbal Healing", "heal 1000", 40, 100, 95, 0, SKILL_SHAMAN, SKILL_NONE, SP_HERBAL_HEALING, 0},
	{"Door Building", "", 7, 7, 20, 0, SKILL_SHAMAN, SKILL_NONE, SP_DOOR_BUILDING, 0},
	{"Stair Building", "", 9, 12, 40, 0, SKILL_SHAMAN, SKILL_NONE, SP_STAIR_BUILDING, 0},
	{"Stone Skin", "dur 20+d30", 10, 12, 75, 0, SKILL_SHAMAN, SKILL_NONE, SP_STONE_SKIN, 0},
	{"Resistance True", "dur 20+d20", 15, 20, 85, 0, SKILL_SHAMAN, SKILL_NONE, SP_RESISTANCE_TRUE, 0},
	{"Animal Friendship", "", 30, 30, 90, 0, SKILL_SHAMAN, SKILL_NONE, SP_ANIMAL_FRIENDSHIP, 0},
	{"Stone Tell", "", 37, 40, 90, 0, SKILL_SHAMAN, SKILL_NONE, SP_STONE_TELL, 0},
	{"Wall of Stone", "", 38, 45, 75, 0, SKILL_SHAMAN, SKILL_NONE, SP_WALL_OF_STONE, 0},
	{"Protect from Corrosion", "", 40, 90, 90, 0, SKILL_SHAMAN, SKILL_NONE, SP_PROTECT_FROM_CORROSION, 0},
	{"Earthquake", "rad 10", 20, 18, 60, 0, SKILL_SHAMAN, SKILL_NONE, SP_EARTHQUAKE, 0},
	{"Whirlwind Attack", "", 23, 23, 80, 0, SKILL_SHAMAN, SKILL_NONE, SP_WHIRLWIND_ATTACK, 0},
	{"Blizzard", "dam LEV+70", 25, 25, 75, 0, SKILL_SHAMAN, SKILL_NONE, SP_BLIZZARD, 0},
	{"Lightning Storm", "dam LEV+90", 30, 27, 75, 0, SKILL_SHAMAN, SKILL_NONE, SP_LIGHTNING_STORM, 0},
	{"Whirlpool", "dam LEV+100", 35, 30, 85, 0, SKILL_SHAMAN, SKILL_NONE, SP_WHIRLPOOL, 0},
	{"Call Sunlight", "dam 75", 37, 35, 90, 0, SKILL_SHAMAN, SKILL_NONE, SP_CALL_SUNLIGHT, 0},
	{"Elemental Branding", "", 40, 90, 95, 0, SKILL_SHAMAN, SKILL_NONE, SP_ELEMENTAL_BRANDING, 0},
	{"Nature's Wrath", "dam LEV*4;+LEV+100", 40, 75, 65, 0, SKILL_SHAMAN, SKILL_NONE, SP_NATURES_WRATH, 0},
};

static magic_type cantrip_info[32] =
{
	{"Zap", "dam LEV-1/5+3;d3", 1, 1, 20, 0, SKILL_HEDGE, SKILL_NONE, SP_ZAP, 0},
	{"Wizard Lock", "", 1, 1, 33, 0, SKILL_HEDGE, SKILL_NONE, SP_WIZARD_LOCK, 0},
	{"Detect Invisibility", "", 2, 1, 33, 0, SKILL_HEDGE, SKILL_NONE, SP_DETECT_INVISIBILITY, 0},
	{"Detect Monsters", "", 2, 2, 33, 0, SKILL_HEDGE, SKILL_NONE, SP_DETECT_MONSTERS, 0},
	{"Blink", "range 10", 3, 3, 33, 0, SKILL_HEDGE, SKILL_NONE, SP_BLINK, 0},
	{"Light Area", "dam 2dLEV/2", 5, 5, 40, 0, SKILL_HEDGE, SKILL_NONE, SP_LIGHT_AREA, 0},
	{"Trap & Door Destruction", "", 6, 6, 33, 0, SKILL_HEDGE, SKILL_NONE, SP_TRAP_DOOR_DESTRUCTION, 0},
	{"Cure Light Wounds", "heal 2d8", 7, 7, 44, 0, SKILL_HEDGE, SKILL_NONE, SP_CURE_LIGHT_WOUNDS, 0},
	{"Detect Doors & Traps", "", 8, 8, 40, 0, SKILL_HEDGE, SKILL_NONE, SP_DETECT_DOORS_AND_TRAPS, 0},
	{"Phlogiston", "", 9, 9, 60, 0, SKILL_HEDGE, SKILL_NONE, SP_PHLOGISTON, 0},
	{"Detect Treasure", "", 10, 10, 50, 0, SKILL_HEDGE, SKILL_NONE, SP_DETECT_TREASURE, 0},
	{"Detect Enchantment", "", 11, 11, 50, 0, SKILL_HEDGE, SKILL_NONE, SP_DETECT_ENCHANTMENT, 0},
	{"Detect Objects", "", 13, 11, 50, 0, SKILL_HEDGE, SKILL_NONE, SP_DETECT_OBJECTS, 0},
	{"Cure Poison", "", 14, 12, 50, 0, SKILL_HEDGE, SKILL_NONE, SP_CURE_POISON, 0},
	{"Resist Cold", "dur 20+d20", 15, 13, 50, 0, SKILL_HEDGE, SKILL_NONE, SP_RESIST_COLD, 0},
	{"Resist Fire", "dur 20+d20", 16, 14, 50, 0, SKILL_HEDGE, SKILL_NONE, SP_RESIST_FIRE, 0},
	{"Resist Lightning", "dur 20+d20", 17, 15, 50, 0, SKILL_HEDGE, SKILL_NONE, SP_RESIST_LIGHTNING, 0},
	{"Resist Acid", "dur 20+d20", 18, 16, 50, 0, SKILL_HEDGE, SKILL_NONE, SP_RESIST_ACID, 0},
	{"Cure Medium Wounds", "heal 4d8", 19, 17, 33, 0, SKILL_HEDGE, SKILL_NONE, SP_CURE_MEDIUM_WOUNDS, 0},
	{"Teleport", "range LEV*5", 20, 20, 50, 0, SKILL_HEDGE, SKILL_NONE, SP_TELEPORT, 0},
	{"Stone to Mud", "", 23, 22, 60, 0, SKILL_HEDGE, SKILL_NONE, SP_STONE_TO_MUD, 0},
	{"Ray of Light", "dam 6d8", 25, 24, 60, 0, SKILL_HEDGE, SKILL_NONE, SP_RAY_OF_LIGHT, 0},
	{"Satisfy Hunger", "", 28, 25, 70, 0, SKILL_HEDGE, SKILL_NONE, SP_SATISFY_HUNGER, 0},
	{"See Invisible", "dur 24+d24", 30, 28, 60, 0, SKILL_HEDGE, SKILL_NONE, SP_SEE_INVISIBLE, 0},
	{"Recharging", "", 35, 30, 80, 0, SKILL_HEDGE, SKILL_NONE, SP_RECHARGING, 0},
	{"Teleport Level", "", 39, 36, 80, 0, SKILL_HEDGE, SKILL_NONE, SP_TELEPORT_LEVEL, 0},
	{"Identify", "", 42, 37, 60, 0, SKILL_HEDGE, SKILL_NONE, SP_IDENTIFY, 0},
	{"Teleport Away", "", 44, 38, 70, 0, SKILL_HEDGE, SKILL_NONE, SP_TELEPORT_AWAY, 0},
	{"Elemental Ball", "dam LEV+75", 46, 40, 66, 0, SKILL_HEDGE, SKILL_NONE, SP_ELEMENTAL_BALL, 0},
	{"Detection", "", 47, 42, 80, 0, SKILL_HEDGE, SKILL_NONE, SP_DETECTION, 0},
	{"Word of Recall", "delay 15+d21", 48, 60, 70, 0, SKILL_HEDGE, SKILL_NONE, SP_WORD_OF_RECALL, 0},
	{"Clairvoyance", "dur 25+30", 50, 125, 80, 0, SKILL_HEDGE, SKILL_NONE, SP_CLAIRVOYANCE, 0},
};

static magic_type mindcraft_powers[MAX_MINDCRAFT_POWERS] =
{
	/* Det. monsters/traps */
	{"Precognition", "", 0, 1, 15, 0, SKILL_MINDCRAFTING, SKILL_NONE, SP_PRECOGNITION, 0},
	/* ~MM */
	{"Neural Blast", "dam LEV-1/4+3;dLEV/15+3", 1, 1, 20, 0, SKILL_MINDCRAFTING, SKILL_NONE, SP_NEURAL_BLAST, 0}, 
	/* Phase/dimension door (range 10 for sk 0-49, LEV+2 above) */
	{"Minor Displacement", NULL, 3, 2, 25, 0, SKILL_MINDCRAFTING, SKILL_NONE, SP_MINOR_DISPLACEMENT, 0},
	/* Tele. Self / All */
	{"Major Displacement", "range LEV*5", 7, 6, 35, 0, SKILL_MINDCRAFTING, SKILL_NONE, SP_MAJOR_DISPLACEMENT, 0}, 
	{"Domination", "", 9, 7, 50, 0, SKILL_MINDCRAFTING, SKILL_NONE, SP_DOMINATION, 0},
	/* Telekinetic "bolt" */
	{"Pulverise", "dam LEV-5/4+8;d8", 11, 7, 30, 0, SKILL_MINDCRAFTING, SKILL_NONE, SP_PULVERISE, 0},
	/* Psychic/physical defenses */
	{"Character Armour", "dur LEV", 13, 12, 50, 0, SKILL_MINDCRAFTING, SKILL_NONE, SP_CHARACTER_ARMOUR, 0},
	{"Psychometry", "", 15, 12, 60, 0, SKILL_MINDCRAFTING, SKILL_NONE, SP_PSYCHOMETRY, 0},
	/* Ball -> LOS */
	{"Mind Wave", "dam LEV-5/10+1*LEV", 18, 10, 45, 0, SKILL_MINDCRAFTING, SKILL_NONE, SP_MIND_WAVE, 0},
	{"Adrenaline Channeling", "dur 11-LEV*3/2", 23, 15, 50, 0, SKILL_MINDCRAFTING, SKILL_NONE, SP_ADRENALINE_CHANNELING, 0},
 	/* Convert enemy HP to mana */
 	{"Psychic Drain", "dam LEV/2;d6", 25, 10, 40, 0, SKILL_MINDCRAFTING, SKILL_NONE, SP_PSYCHIC_DRAIN, 0},
	/* Ball -> LOS (dam LEV*3 for sk 0-79, LEV*4 above) */
	{"Telekinetic Wave", NULL, 29, 20, 45, 0, SKILL_MINDCRAFTING, SKILL_NONE, SP_TELEKINETIC_WAVE, 0},
};


book_type book_info[MAX_BK] =
{
	{IDX(BK_SORC_0) sorcery_spells, 0x000000ff},
	{IDX(BK_SORC_1) sorcery_spells, 0x0000ff00},
	{IDX(BK_SORC_2) sorcery_spells, 0x00ff0000},
	{IDX(BK_SORC_3) sorcery_spells, 0xff000000},
	{IDX(BK_NECRO_0) necromancy_spells, 0x000000ff},
	{IDX(BK_NECRO_1) necromancy_spells, 0x0000ff00},
	{IDX(BK_NECRO_2) necromancy_spells, 0x00ff0000},
	{IDX(BK_NECRO_3) necromancy_spells, 0xff000000},
	{IDX(BK_THAUM_0) thaumaturgy_info, 0x000000ff},
	{IDX(BK_THAUM_1) thaumaturgy_info, 0x0000ff00},
	{IDX(BK_THAUM_2) thaumaturgy_info, 0x00ff0000},
	{IDX(BK_THAUM_3) thaumaturgy_info, 0xff000000},
	{IDX(BK_CONJ_0) conjuration_spells, 0x000000ff},
	{IDX(BK_CONJ_1) conjuration_spells, 0x0000ff00},
	{IDX(BK_CONJ_2) conjuration_spells, 0x00ff0000},
	{IDX(BK_CONJ_3) conjuration_spells, 0xff000000},
	{IDX(BK_CHARM_SULPHUR) cantrip_info, 0x10300221},
	{IDX(BK_CHARM_HEMLOCK) cantrip_info, 0x4a080010},
	{IDX(BK_CHARM_UNICORN) cantrip_info, 0x00442080},
	{IDX(BK_CHARM_CRYSTAL) cantrip_info, 0x20001508},
	{IDX(BK_CHARM_AGARIC) cantrip_info, 0x80800804},
	{IDX(BK_CHARM_GARLIC) cantrip_info, 0x0003c000},
	{IDX(BK_CHARM_GEODE) cantrip_info, 0x05000042},
	{IDX(BK_LIFE_0) life_spells, 0x000000ff},
	{IDX(BK_LIFE_1) life_spells, 0x0000ff00},
	{IDX(BK_LIFE_2) life_spells, 0x00ff0000},
	{IDX(BK_LIFE_3) life_spells, 0xff000000},
	{IDX(BK_WILD_0) wild_spells, 0x000000ff},
	{IDX(BK_WILD_1) wild_spells, 0x0000ff00},
	{IDX(BK_WILD_2) wild_spells, 0x00ff0000},
	{IDX(BK_WILD_3) wild_spells, 0xff000000},
	{IDX(BK_MIND) mindcraft_powers, 0x0000fff},
};

/*
 * Player's Skills
 *
 * name,increase,0,0,0,0,0,exp_to_raise,x,y
 */
 player_skill skill_set[MAX_SKILLS] =
 {
	{IDX(SKILL_TOUGH) "Toughness","You are toughening up.",
		0,0,0,0,0,5,5,10},
	{IDX(SKILL_MANA) "Mana Channeling",
		"You are getting better at channeling mana.",0,0,0,0,0,6,54,14},
	{IDX(SKILL_DISARM) "Disarming","You are getting better at disarming.",
		0,0,0,0,0,1,5,19},
	{IDX(SKILL_DEVICE) "Magical Devices",
		"You are getting better with magical devices.",0,0,0,0,0,1,5,12},
	{IDX(SKILL_SAVE) "Resistance","You are building your resistances.",
		0,0,0,0,0,1,5,13},
	{IDX(SKILL_STEALTH) "Stealth","You are getting more stealthy.",
		0,0,0,0,0,1,5,20},
	{IDX(SKILL_SEARCH) "Searching","You are getting better at searching.",
		0,0,0,0,0,1,5,16},
	{IDX(SKILL_PERCEPTION) "Perception","You are getting more perceptive.",
		0,0,0,0,0,1,5,15},
	{IDX(SKILL_CLOSE) "Close Combat","Your combat skills are increasing.",
		0,0,0,0,0,10,5,4},
	{IDX(SKILL_MISSILE) "Missile","Your accuracy is increasing.",
		0,0,0,0,0,4,5,8},
	{IDX(SKILL_MA) "Martial Arts","Your martial arts are improving.",
		0,0,0,0,0,9,30,4},
	{IDX(SKILL_CORPORIS) "Corporis","Your magice corporis is improving.",
		0,0,0,0,0,2,54,9},
	{IDX(SKILL_ANIMAE) "Animae","Your magice animae magic is improving.",
		0,0,0,0,0,2,54,12},
	{IDX(SKILL_VIS) "Vis","Your magice vis is improving.",0,0,0,0,0,2,54,10},
	{IDX(SKILL_NATURAE) "Naturae","Your magice naturae is improving.",
		0,0,0,0,0,2,54,11},
	{IDX(SKILL_NECROMANCY) "Necromancy","Your necromancy is improving.",
		0,0,0,0,0,3,54,5},
	{IDX(SKILL_SORCERY) "Sorcery","Your sorcery is improving.",
		0,0,0,0,0,3,54,7},
	{IDX(SKILL_CONJURATION) "Conjuration","Your conjuration is improving.",
		0,0,0,0,0,3,54,6},
	{IDX(SKILL_THAUMATURGY) "Thaumaturgy","Your thaumaturgy is improving.",
		0,0,0,0,0,3,54,4},
	{IDX(SKILL_MINDCRAFTING) "Mindcrafting","Your mind is getting stronger.",
		0,0,0,0,0,5,30,6},
	{IDX(SKILL_CHI) "Chi","Your Chi flow is improving.",0,0,0,0,0,6,30,7},
	{IDX(SKILL_RACIAL) "Innate Powers","Your innate powers are improving.",
		0,0,0,0,0,1,30,13},
	{IDX(SKILL_STAB) "Stabbing Weapons",
		"Your skill with stabbing weapons is improving.",0,0,0,0,0,10,5,6},
	{IDX(SKILL_SLASH) "Slashing Weapons",
		"Your skill with slashing weapons is improving.",0,0,0,0,0,10,5,5},
	{IDX(SKILL_CRUSH) "Crushing Weapons",
		"Your skill with crushing weapons is improving.",0,0,0,0,0,10,5,7},
	{IDX(SKILL_SHAMAN) "Spirit Lore",
		"You are learning more about the spirit world",0,0,0,0,0,6,30,9},
	{IDX(SKILL_HEDGE) "Hedge Magic","You are getting better at hedge magic.",
		0,0,0,0,0,3,30,11},
	{IDX(SKILL_PSEUDOID) "Item Sensing","You feel more able to judge items.",
		0,0,0,0,0,1,5,17},
};

/*
 * Each chest has a certain set of traps, determined by pval
 * Each chest has a "pval" from 1 to the chest level (max 55)
 * If the "pval" is negative then the trap has been disarmed
 * The "pval" of a chest determines the quality of its treasure
 * Note that disarming a trap on a chest also removes the lock.
 */
byte chest_traps[64] =
{
	0,                                      /* 0 == empty */
	(CHEST_POISON),
	(CHEST_LOSE_STR),
	(CHEST_LOSE_CON),
	(CHEST_LOSE_STR),
	(CHEST_LOSE_CON),                       /* 5 == best small wooden */
	0,
	(CHEST_POISON),
	(CHEST_POISON),
	(CHEST_LOSE_STR),
	(CHEST_LOSE_CON),
	(CHEST_POISON),
	(CHEST_LOSE_STR | CHEST_LOSE_CON),
	(CHEST_LOSE_STR | CHEST_LOSE_CON),
	(CHEST_LOSE_STR | CHEST_LOSE_CON),
	(CHEST_SUMMON),                 /* 15 == best large wooden */
	0,
	(CHEST_LOSE_STR),
	(CHEST_LOSE_CON),
	(CHEST_PARALYZE),
	(CHEST_LOSE_STR | CHEST_LOSE_CON),
	(CHEST_SUMMON),
	(CHEST_PARALYZE),
	(CHEST_LOSE_STR),
	(CHEST_LOSE_CON),
	(CHEST_EXPLODE),                        /* 25 == best small iron */
	0,
	(CHEST_POISON | CHEST_LOSE_STR),
	(CHEST_POISON | CHEST_LOSE_CON),
	(CHEST_LOSE_STR | CHEST_LOSE_CON),
	(CHEST_EXPLODE | CHEST_SUMMON),
	(CHEST_PARALYZE),
	(CHEST_POISON | CHEST_SUMMON),
	(CHEST_SUMMON),
	(CHEST_EXPLODE),
	(CHEST_EXPLODE | CHEST_SUMMON), /* 35 == best large iron */
	0,
	(CHEST_SUMMON),
	(CHEST_EXPLODE),
	(CHEST_EXPLODE | CHEST_SUMMON),
	(CHEST_EXPLODE | CHEST_SUMMON),
	(CHEST_POISON | CHEST_PARALYZE),
	(CHEST_EXPLODE),
	(CHEST_EXPLODE | CHEST_SUMMON),
	(CHEST_EXPLODE | CHEST_SUMMON),
	(CHEST_POISON | CHEST_PARALYZE),        /* 45 == best small steel */
	0,
	(CHEST_LOSE_STR | CHEST_LOSE_CON),
	(CHEST_LOSE_STR | CHEST_LOSE_CON),
	(CHEST_POISON | CHEST_PARALYZE | CHEST_LOSE_STR),
	(CHEST_POISON | CHEST_PARALYZE | CHEST_LOSE_CON),
	(CHEST_POISON | CHEST_LOSE_STR | CHEST_LOSE_CON),
	(CHEST_POISON | CHEST_LOSE_STR | CHEST_LOSE_CON),
	(CHEST_POISON | CHEST_PARALYZE | CHEST_LOSE_STR | CHEST_LOSE_CON),
	(CHEST_POISON | CHEST_PARALYZE),
	(CHEST_POISON | CHEST_PARALYZE),        /* 55 == best large steel */
	(CHEST_EXPLODE | CHEST_SUMMON),
	(CHEST_EXPLODE | CHEST_SUMMON),
	(CHEST_EXPLODE | CHEST_SUMMON),
	(CHEST_EXPLODE | CHEST_SUMMON),
	(CHEST_EXPLODE | CHEST_SUMMON),
	(CHEST_EXPLODE | CHEST_SUMMON),
	(CHEST_EXPLODE | CHEST_SUMMON),
	(CHEST_EXPLODE | CHEST_SUMMON),
};


/*
 * Hack -- the "basic" color names (see "TERM_xxx")
 */
cptr color_names[16] =
{
	"Dark",
	"White",
	"Slate",
	"Orange",
	"Red",
	"Green",
	"Blue",
	"Umber",
	"Light Dark",
	"Light Slate",
	"Violet",
	"Yellow",
	"Light Red",
	"Light Green",
	"Light Blue",
	"Light Umber",
};

/*
 * The character used to represent each colour.
 *
 * This is hardcoded.
 */
cptr atchar = "dwsorgbuDWvyRGBU";

/*
 * Abbreviations of healthy stats
 */
cptr stat_names[6] =
{
	"STR: ", "INT: ", "WIS: ", "DEX: ", "CON: ", "CHR: "
};

/*
 * Abbreviations of damaged stats
 */
cptr stat_names_reduced[6] =
{
	"Str: ", "Int: ", "Wis: ", "Dex: ", "Con: ", "Chr: "
};


/*
 * Available Options (full to 3,24).
 * In addition, 7,2 and 7,8-15 are used for cheat options, and the
 * rest of 7,0-15 is reserved for them to ensure that "noscore" is correct.
 *
 * Note that options which are only set at character creation but are active
 * throughout the game should always be followed by a BIRTHR option which 
 * should contain a pointer to the variable actually read in the game.
 *
 * Note also that these BIRTHR options have no text name. This, and the lack
 * of a BIRTHR option screen, prevents them from being set.
 */
option_type option_info[] =
{
	/*** User-Interface ***/

	{ &rogue_like_commands, FALSE, OPTS_UI,      0, 0,
	"rogue_like_commands",  "Rogue-like commands" },

	{ &quick_messages,              TRUE, OPTS_UI,      0, 1,
	"quick_messages",               "Quick -more- prompts" },

	{ &quick_prompt,		TRUE,	OPTS_UI,	2, 27,
	"quick_prompt",			"Quick [y/n] prompts" },

	{ &other_query_flag,    FALSE, OPTS_UI,      0, 2,
	"other_query_flag",             "Prompt for various information" },

	{ &carry_query_flag,    FALSE, OPTS_OBJ,      0, 3,
	"carry_query_flag",             "Prompt before picking things up" },

	{ &use_old_target,              TRUE, OPTS_MON,      0, 4,
	"use_old_target",               "Use old target by default" },

	{ &always_pickup,               TRUE, OPTS_OBJ,      0, 5,
	"always_pickup",                "Pick things up by default" },

	{ &always_repeat,               TRUE, OPTS_UI,      0, 6,
	"always_repeat",                "Repeat obvious commands" },

	{ &depth_in_feet,               TRUE, OPTS_DISPLAY,      0, 7,
	"depth_in_feet",                "Show dungeon level in feet" },

	{ &stack_force_notes,   TRUE, OPTS_OBJ,      0, 8,
	"stack_force_notes",    "Merge inscriptions when stacking" },

	{ &stack_force_notes_all,	FALSE, OPTS_OBJ,	3,	1,
	"stack_force_notes_all",	"Merge all inscriptions when stacking" },

	{ &stack_force_costs,   FALSE, OPTS_OBJ,      0, 9,
	"stack_force_costs",    "Merge discounts when stacking" },

	{ &show_labels,                 TRUE, OPTS_OBJ,      0, 10,
	"show_labels",                  "Show labels in object listings" },

	{ &show_weights,                TRUE, OPTS_OBJ,      0, 11,
	"show_weights",                 "Show weights in object listings" },

	{ &show_choices,                TRUE, OPTS_DISPLAY,      0, 12,
	"show_choices",                 "Show choices in certain sub-windows" },

    { &show_details,                TRUE, OPTS_DISPLAY,      0, 13,
	"show_details",                 "Show more detailed monster descriptons" },

	{ &show_choices_main,		TRUE,	OPTS_DISPLAY,	2,26,
	"show_choices_main",		"Show certain choices in the main window"},
	
    { &ring_bell,                   FALSE, OPTS_DISPLAY,      0, 14,
	"ring_bell",                    "Audible bell (on errors, etc)" },
    /* Changed to default to FALSE -- it's so extremely annoying!!! -TY */

	{ &use_color,                   TRUE, OPTS_DISPLAY,      0, 15,
	"use_color",                    "Use color if possible (slow)" },


	/*** Disturbance ***/

	{ &find_ignore_stairs,  FALSE, OPTS_DISTURB,      0, 16,
	"find_ignore_stairs",   "Run past stairs" },

	{ &find_ignore_doors,   FALSE, OPTS_DISTURB,      0, 17,
	"find_ignore_doors",    "Run through open doors" },

	{ &find_cut,                    TRUE, OPTS_DISTURB,      0, 18,
	"find_cut",                             "Run past known corners" },

	{ &find_examine,                TRUE, OPTS_DISTURB,      0, 19,
	"find_examine",                 "Run into potential corners" },

	{ &stop_corner,	FALSE, OPTS_DISTURB,	3, 0,
	"stop_corner",	"Stop at corners"},

	{ &disturb_move,                FALSE, OPTS_DISTURB,      0, 20,
	"disturb_move",                 "Disturb whenever any monster moves" },

	{ &disturb_near,                TRUE, OPTS_DISTURB,      0, 21,
	"disturb_near",                 "Disturb whenever viewable monster moves" },

	{ &disturb_panel,               TRUE, OPTS_DISTURB,      0, 22,
	"disturb_panel",                "Disturb whenever map panel changes" },

	{ &disturb_state,               TRUE, OPTS_DISTURB,      0, 23,
	"disturb_state",                "Disturb whenever player state changes" },

	{ &disturb_dawn,	FALSE, OPTS_DISTURB,	3, 9,
	"disturb_dawn",	"Disturb when the sun rises or sets" },

	{ &disturb_minor,               FALSE, OPTS_DISTURB,      0, 24,
	"disturb_minor",                "Disturb whenever boring things happen" },

	{ &alert_failure,               FALSE, OPTS_DISTURB,      0, 27,
	"alert_failure",                "Alert user to various failures" },

    { &last_words,                  FALSE, OPTS_MISC,      0, 28,
      "last_words",                 "Get last words when the character dies" },

    { &small_levels,                TRUE, OPTS_MISC,      0, 30,
       "small_levels",              "Allow unusually small dungeon levels" },

    { &empty_levels,                TRUE, OPTS_MISC,      0, 31,
        "empty_levels",             "Allow empty 'arena' levels" },

	/*** Game-Play ***/

	{ &auto_haggle,                 TRUE, OPTS_MISC,      1, 0,
	"auto_haggle",                  "Auto-haggle in stores" },

	{ &verbose_haggle,	FALSE, OPTS_MISC, 2, 25,
	"verbose_haggle", "Verbose auto_haggle messages" },

	{ &auto_scum,                   TRUE, OPTS_MISC,      1, 1,
	"auto_scum",                    "Auto-scum for good levels" },

	{ &stack_allow_items,   TRUE, OPTS_OBJ,      1, 2,
	"stack_allow_items",    "Allow weapons and armor to stack" },

	{ &stack_allow_wands,   TRUE, OPTS_OBJ,      1, 3,
	"stack_allow_wands",    "Allow wands/staffs/rods to stack" },

	{ &expand_look,                 FALSE, OPTS_MISC,      1, 4,
	"expand_look",                  "Expand the power of the look command" },

	{ &expand_list,                 FALSE, OPTS_MISC,      1, 5,
	"expand_list",                  "Expand the power of the list commands" },

	{ &view_perma_grids,    TRUE, OPTS_MISC,      1, 6,
	"view_perma_grids",             "Map remembers all perma-lit grids" },

	{ &view_torch_grids,    FALSE, OPTS_MISC,      1, 7,
	"view_torch_grids",             "Map remembers all torch-lit grids" },

	{ &dungeon_align,               TRUE, OPTS_MISC,      1, 8,
	"dungeon_align",                "Generate dungeons with aligned rooms" },

	{ &dungeon_stair,               TRUE, OPTS_MISC,      1, 9,
	"dungeon_stair",                "Generate dungeons with connected stairs" },

	{ &dungeon_small,               FALSE, OPTS_MISC,      1, 10,
	"dungeon_small",                "Usually generate small dungeons" },

	{ &flow_by_sound,               FALSE, OPTS_MON,      1, 11,
	"flow_by_sound",                "Monsters chase current location (v.slow)" },

	{ &flow_by_smell,               TRUE, OPTS_MON,      1, 12,
	"flow_by_smell",                "Monsters chase recent locations (v.slow)" },



    { &equippy_chars,               TRUE, OPTS_OBJ,      1, 14,
        "equippy_chars",           "Display 'equippy' chars" },

	{ &smart_learn,                 TRUE, OPTS_MON,      1, 15,
	"smart_learn",                  "Monsters learn from their mistakes" },

	{ &smart_cheat,                 FALSE, OPTS_MON,      1, 16,
	"smart_cheat",                  "Monsters exploit players weaknesses" },

 #ifdef ALLOW_EASY_OPEN
 	{ &easy_open,                   TRUE, OPTS_MISC,      1, 17,
 	"easy_open",                    "Open and close automatically" },
 #endif /* ALLOW_EASY_OPEN -- TNB */
 
 #ifdef ALLOW_EASY_DISARM
	{ &easy_disarm,                 TRUE, OPTS_MISC,      1, 18,
 	"easy_disarm",                  "Disarm traps automatically" },
 #endif /* ALLOW_EASY_DISARM -- TNB */


	/*** Efficiency ***/

	{ &view_reduce_lite,    FALSE, OPTS_PERF,      1, 19,
	"view_reduce_lite",             "Reduce lite-radius when running" },

	{ &view_reduce_view,    FALSE, OPTS_PERF,      1, 20,
	"view_reduce_view",             "Reduce view-radius in town" },

	{ &avoid_abort,                 FALSE, OPTS_PERF,      1, 21,
	"avoid_abort",                  "Avoid checking for user abort" },

	{ &avoid_other,                 FALSE, OPTS_PERF,      1, 22,
	"avoid_other",                  "Avoid processing special colors" },

	{ &flush_error,	TRUE, OPTS_PERF, 3, 19,
	"flush_error", "Flush input on incorrect keypresses." },

	{ &flush_failure,               TRUE, OPTS_PERF,      1, 23,
	"flush_failure",                "Flush input on various failures" },

	{ &flush_disturb,               FALSE, OPTS_PERF,      1, 24,
	"flush_disturb",                "Flush input whenever disturbed" },

	{ &fresh_before,                TRUE, OPTS_PERF,      1, 26,
	"fresh_before",                 "Flush output before every command" },

	{ &fresh_after,                 FALSE, OPTS_PERF,      1, 27,
	"fresh_after",                  "Flush output after every command" },

	{ &fresh_message,               FALSE, OPTS_PERF,      1, 28,
	"fresh_message",                "Flush output after every message" },

	{ &compress_savefile,   FALSE, OPTS_PERF,      1, 29,
	"compress_savefile",    "Compress messages in savefiles" },

    { &hilite_player,               FALSE, OPTS_DISPLAY,      1, 30,
	"hilite_player",                "Hilite the player with the cursor" },

	{ &view_yellow_lite,    TRUE, OPTS_PERF,      1, 31,
	"view_yellow_lite",             "Use special colors for torch-lit grids" },

	{ &view_bright_lite,    TRUE, OPTS_PERF,      2, 1,
	"view_bright_lite",             "Use special colors for 'viewable' grids" },

	{ &view_granite_lite,   TRUE, OPTS_PERF,      2, 2,
	"view_granite_lite",    "Use special colors for wall grids (slow)" },

	{ &view_special_lite,   TRUE, OPTS_PERF,      2, 3,
	"view_special_lite",    "Use special colors for floor grids (slow)" },

    { &skip_chaos_features,        FALSE, OPTS_MISC, 2, 4,
       "skip_chaos_features",      "Skip chaos features in 'C'haracter Display" },

    { &plain_descriptions,    FALSE, OPTS_OBJ, 2, 5,
        "plain_descriptions", "Plain object descriptions" },

    { &stupid_monsters,      FALSE, OPTS_MON, 2, 6,
        "stupid_monsters",  "Monsters behave stupidly" },

    { &auto_destroy,        TRUE, OPTS_OBJ, 2, 7,
        "auto_destroy",     "No query to destroy known worthless items" },

    { &wear_confirm,        TRUE, OPTS_OBJ,2, 8,
        "confirm_wear",     "Confirm to wear/wield known cursed items" },

    { &confirm_wear_all,        TRUE, OPTS_OBJ,3, 7,
        "confirm_wear_all",     "Confirm to wear/wield potentially cursed items" },

    { &confirm_stairs,      FALSE, OPTS_UI, 2, 9,
        "confirm_stairs",   "Prompt before exiting a dungeon level" },

    { &disturb_allies,        FALSE, OPTS_DISTURB, 2, 10,
        "disturb_allies",     "Disturb when visible allies move" },

    { &multi_stair,        TRUE, OPTS_MISC, 2, 11,
        "multi_stair",     "Stairs can be longer than one level" },
	
	{ &rand_unbiased,    FALSE, OPTS_PERF,2,12,
	"rand_unbiased","Random numbers have bias removed (slow)"},

	{ &unify_commands,  FALSE, OPTS_OBJ,2,13,
	"unify_commands","Use a single 'u'se command for all objects"},

	{ &testing_stack,               TRUE, OPTS_OBJ, 2, 14,
    "testing_stack",                "Allow objects to stack on floor" },

	{ &show_piles, FALSE, OPTS_OBJ, 3, 21,
	"show_piles", "Show stacks with a special colour/character" },

	{ &testing_carry,               TRUE, OPTS_MON, 2, 15,
    "testing_carry",                "Allow monsters to carry objects" },

	{ &centre_view,               TRUE, OPTS_DISPLAY, 2, 16,
    "centre_view",                "Centre view around player (v.slow)" },

	{&macro_edit, TRUE, OPTS_UI, 3, 11,
	"macro_edit", "Use macros as edit keys in prompts" },

	{ &beginner_help, TRUE, OPTS_UI, 0, 25,
	"beginner_help",	"Display beginner help on startup" },

	{&scroll_edge, FALSE, OPTS_DISPLAY, 3, 13,
	"scroll_edge", "Scroll until detection reaches the edge"},

	{&allow_fake_colour, TRUE, OPTS_DISPLAY, 3, 24,
	"allow_fake_colour", "Use special monochrome display for emphasis"},

	{ &no_centre_run,               FALSE, OPTS_PERF, 2, 17,
    "no_centre_run",                "Do not centre view whilst running" },

#ifdef USE_X11
	{ &track_mouse, TRUE, OPTS_PERF, 3, 22,
	"track_mouse", "Track the cursor in various extra windows." },
#endif

	{ &auto_more,                   FALSE,  OPTS_DISTURB,      3, 12,
	"auto_more",                    "Automatically clear '-more-' prompts" },

#ifdef SCORE_QUITTERS
	{ &score_quitters_w,		FALSE, OPTS_BIRTH, 0, 26,
	"score_quitter",			"Remember scores of those who quit the game" },

	{ &score_quitters, 0, OPTS_BIRTHR, 3, 14, 0, ""},
#endif /* SCORE_QUITTERS */

	{ &maximise_mode_w,               TRUE, OPTS_BIRTH, 2, 18,
    "maximise_mode",                "Include race/template bonuses in stat calcs" },

	{ &maximise_mode, 0, OPTS_BIRTHR, 3, 15, 0, ""},

	{ &preserve_mode_w,               TRUE, OPTS_BIRTH, 2, 19,
    "preserve_mode",                "Artifacts are not lost if you never saw them" },

	{ &preserve_mode, 0, OPTS_BIRTHR, 3, 16, 0, ""},

	{ &allow_quickstart,		TRUE,	OPTS_BIRTH, 3, 4,
	"allow_quickstart",	"Allow the player to start by using Quick-Start"},
	
	{ &allow_pickstats,		TRUE,	OPTS_BIRTH, 3, 6,
	"allow_pickstats",	"Allow the player to choose pre-set stats"},

#ifdef USE_MAIN_C
	{ &display_credits,	TRUE,	OPTS_BIRTH, 3, 5,
	"display_credits",	"Require a keypress to clear the initial screen"},
#endif

	{ &use_autoroller,               TRUE, OPTS_BIRTH, 2, 20,
    "use_autoroller",                "Allow stats to be rolled repeatedly with minima" },

	{ &spend_points, TRUE, OPTS_BIRTH, 2, 21,
    "spend_points", "Allow stats to be chosen by spending points" },

    { &speak_unique_w,                FALSE, OPTS_BIRTH,      0, 29,
       "speak_unique",              "Allow shopkeepers and uniques to speak" },

	{ &speak_unique,	0,	OPTS_BIRTHR, 3, 20, 0, ""},

	{ &ironman_shop_w,               FALSE, OPTS_BIRTH, 2, 22,
    "ironman_shop",                "Shops (except for libraries) are locked" },

	{ &ironman_shop, 0, OPTS_BIRTHR, 3, 17, 0, ""},

	{ &ironman_feeling_w,	FALSE, OPTS_BIRTH, 3, 3,
	"ironman_feeling",	"Level feelings are only given after 2500 turns" },

	{ &ironman_feeling, 0, OPTS_BIRTHR, 3, 18, 0, ""},

	{ &chaos_patrons_w,	TRUE,	OPTS_BIRTH, 3, 23,
	"chaos_patrons",	"Chaos patrons give your character gifts"},

	{ &chaos_patrons, TRUE, OPTS_BIRTHR, 2, 24, 0, ""},

	{ &spoil_base,			FALSE, OPTS_SPOIL,	2, 31,
	"spoil_base",			"Gain knowledge of normal items" },

	{ &spoil_ego,			FALSE, OPTS_SPOIL,	2, 30,
	"spoil_ego",			"Gain knowledge of ego items" },
    
	{ &spoil_art,                  FALSE, OPTS_SPOIL,     2, 28,
	"spoil_art",			"Gain knowledge of standard artifacts" },

	{ &spoil_value,	FALSE, OPTS_SPOIL,	2, 23,
	"spoil_value",	"Gain knowledge of the apparent prices of items" },

	{ &spoil_mon,			FALSE, OPTS_SPOIL,	2, 0,
	"spoil_mon",			"Gain knowledge of monsters" },

	{ &spoil_stat,			FALSE, OPTS_SPOIL,	2,	29,
	"spoil_stat",			"Gain knowledge of the effects of stat changes"},

	{ &spoil_flag,	FALSE, OPTS_SPOIL, 3, 10,
	"spoil_flag",	"Gain knowledge of the effects of various flags."},

	{ &spoil_dam,	FALSE, OPTS_SPOIL,	3,	8,
	"spoil_dam",	"Gain knowledge of melee weapon damage."},

	{ &cheat_peek,		FALSE,	OPTS_CHEAT,	7, 8,
	"cheat_peek",		"Peek into object creation" },

	{ &cheat_hear,		FALSE, OPTS_CHEAT,	7, 9,
	"cheat_hear",		"Peek into monster creation" },

	{ &cheat_room,		FALSE,	OPTS_CHEAT,	7, 10,
	"cheat_room",		"Peek into dungeon creation" },

	{ &cheat_xtra,		FALSE,	OPTS_CHEAT,	7, 11,
	"cheat_xtra",		"Peek into something else" },

	{ &cheat_item,		FALSE,	OPTS_CHEAT,	7, 15,
	"cheat_item",		"Know complete item info" },

	{ &cheat_live,		FALSE,	OPTS_CHEAT,	7, 13,
	"cheat_live",		"Allow player to avoid death" },

	{ &cheat_skll,		FALSE,	OPTS_CHEAT,	7, 14,
	"cheat_skll",		"Peek into skill rolls" },

#ifdef ALLOW_WIZARD
	{ &cheat_wzrd,		FALSE,	OPTS_CHEAT,	7, 1,
	"cheat_wzrd",		"Wizard (Debug) Mode active" },
#endif /* ALLOW_WIZARD */

	{ &cheat_save,	FALSE,	OPTS_CHEAT, 7, 12,
	"cheat_save", "Don't auto-save on in-game events"},
	

	/*** End of Table ***/

	{ NULL,                 0, 0, 0, 0,
	NULL,                   NULL }
};

/* "if (option_force[].1 == option_force[].2) option_force[].3 is irrelevant." */
force_type option_force[] =
{
	{&small_levels, FALSE, &dungeon_small},
	{&centre_view, FALSE, &no_centre_run},
	{&stack_force_notes, FALSE, &stack_force_notes_all},
	{&wear_confirm, FALSE, &confirm_wear_all},
	{&flow_by_sound, FALSE, &flow_by_smell},
	{&use_graphics, TRUE, &use_color},
	{&use_color, FALSE, &allow_fake_colour},
	{0, 0, 0}
};

cptr chaos_patron_shorts[MAX_PATRON] =
{
    "Thed",
    "Ragnaglar",
    "Cacodemon",
    "Malia",
    "Pochnargo",

    "Thanatar",
    "Gbaji",
    "Vivamort",
    "Krarsht",
    "Wakboth",

    "Bagog",
    "Gark",
    "Ikadz",
    "Kajabor",
    "Krjalk",

    "Ompalam"
};

int chaos_stats[MAX_PATRON] =
{
    A_CON,  /* Thed */
    A_CON,  /* Ragnaglar */
    A_STR,  /* Cacodemon */
    A_STR,  /* Malia */
    A_STR,  /* Pochnargo */

    A_INT,  /* Thanatar */
    A_STR,  /* Gbaji */
    A_INT,  /* Vivamort */
    A_CON,  /* Krarsht */
    A_CHR,  /* Wakboth */

    -1,     /* Bagog */
    A_STR,  /* Gark */
    A_CHR,  /* Ikadz */
    A_CON,  /* Kajabor */
    A_INT,  /* Krjalk */

    A_STR,  /* Ompalam */
};




int chaos_rewards[MAX_PATRON][20] =
{

    /* Thed, Mother of Broo: */
    {
      REW_WRATH, REW_CURSE_WP, REW_CURSE_AR, REW_RUIN_ABL, REW_LOSE_ABL,
      REW_IGNORE, REW_IGNORE, REW_IGNORE, REW_POLY_WND, REW_POLY_SLF,
      REW_POLY_SLF, REW_POLY_SLF, REW_GAIN_ABL, REW_GAIN_ABL, REW_GAIN_EXP,
      REW_GOOD_OBJ, REW_CHAOS_WP, REW_GREA_OBJ, REW_AUGM_ABL, REW_AUGM_ABL
    },

    /* Ragnaglar the Unclean: */
    {
      REW_WRATH, REW_CURSE_WP, REW_CURSE_AR, REW_H_SUMMON, REW_SUMMON_M,
      REW_SUMMON_M, REW_IGNORE, REW_IGNORE, REW_POLY_WND, REW_POLY_WND,
      REW_POLY_SLF, REW_HEAL_FUL, REW_HEAL_FUL, REW_GAIN_ABL, REW_SER_UNDE,
      REW_CHAOS_WP, REW_GOOD_OBJ, REW_GOOD_OBJ, REW_GOOD_OBS, REW_GOOD_OBS
    },

    /* Cacodemon, Spawn of the Devil: */
    {
      REW_WRATH, REW_WRATH, REW_HURT_LOT, REW_PISS_OFF, REW_H_SUMMON,
      REW_SUMMON_M, REW_IGNORE, REW_IGNORE, REW_DESTRUCT, REW_SER_UNDE,
      REW_GENOCIDE, REW_MASS_GEN, REW_MASS_GEN, REW_DISPEL_C, REW_GOOD_OBJ,
      REW_CHAOS_WP, REW_GOOD_OBS, REW_GOOD_OBS, REW_AUGM_ABL, REW_AUGM_ABL
    },

    /* Malia, Mistress of Disease: */
    {
      REW_WRATH, REW_WRATH, REW_CURSE_WP, REW_CURSE_AR, REW_RUIN_ABL,
      REW_IGNORE, REW_IGNORE, REW_SER_UNDE, REW_DESTRUCT, REW_GENOCIDE,
      REW_MASS_GEN, REW_MASS_GEN, REW_HEAL_FUL, REW_GAIN_ABL, REW_GAIN_ABL,
      REW_CHAOS_WP, REW_GOOD_OBS, REW_GOOD_OBS, REW_AUGM_ABL, REW_AUGM_ABL
    },

    /* Pochnargo the Mutator: */
    {
      REW_TY_CURSE, REW_TY_CURSE, REW_PISS_OFF, REW_RUIN_ABL, REW_LOSE_ABL,
      REW_IGNORE, REW_POLY_SLF, REW_POLY_SLF, REW_POLY_WND, REW_POLY_WND,
      REW_GENOCIDE, REW_DISPEL_C, REW_GOOD_OBJ, REW_GOOD_OBJ, REW_SER_MONS,
      REW_GAIN_ABL, REW_CHAOS_WP, REW_GAIN_EXP, REW_AUGM_ABL, REW_GOOD_OBS
    },


    /* Thanatar the Assassin: */
    {
      REW_WRATH, REW_TY_CURSE, REW_PISS_OFF, REW_H_SUMMON, REW_H_SUMMON,
      REW_IGNORE, REW_IGNORE, REW_IGNORE, REW_POLY_WND, REW_POLY_SLF,
      REW_POLY_SLF, REW_SER_DEMO, REW_HEAL_FUL, REW_GAIN_ABL, REW_GAIN_ABL,
      REW_CHAOS_WP, REW_DO_HAVOC, REW_GOOD_OBJ, REW_GREA_OBJ, REW_GREA_OBS
    },
    
    /* Gbaji the Deceiver: */
    {
      REW_TY_CURSE, REW_HURT_LOT, REW_CURSE_WP, REW_CURSE_AR, REW_RUIN_ABL,
      REW_SUMMON_M, REW_LOSE_EXP, REW_POLY_SLF, REW_POLY_SLF, REW_POLY_WND,
      REW_SER_UNDE, REW_HEAL_FUL, REW_HEAL_FUL, REW_GAIN_EXP, REW_GAIN_EXP,
      REW_CHAOS_WP, REW_GOOD_OBJ, REW_GOOD_OBS, REW_GREA_OBS, REW_AUGM_ABL
    },


    /* Vivamort the Vampire Lord: */
    {
      REW_WRATH, REW_PISS_OFF, REW_RUIN_ABL, REW_LOSE_EXP, REW_H_SUMMON,
      REW_IGNORE, REW_IGNORE, REW_IGNORE, REW_IGNORE, REW_POLY_SLF,
      REW_POLY_SLF, REW_MASS_GEN, REW_SER_DEMO, REW_HEAL_FUL, REW_CHAOS_WP,
      REW_CHAOS_WP, REW_GOOD_OBJ, REW_GAIN_EXP, REW_GREA_OBJ, REW_AUGM_ABL
    },

    /* Krarsht, the Hungry One: */
    {
      REW_WRATH, REW_TY_CURSE, REW_PISS_OFF, REW_CURSE_WP, REW_RUIN_ABL,
      REW_IGNORE, REW_IGNORE, REW_POLY_SLF, REW_POLY_SLF, REW_POLY_WND,
      REW_GOOD_OBJ, REW_GOOD_OBJ, REW_SER_MONS, REW_HEAL_FUL, REW_GAIN_EXP,
      REW_GAIN_ABL, REW_CHAOS_WP, REW_GOOD_OBS, REW_GREA_OBJ, REW_AUGM_ABL
    },

    /* Wakboth, the Devil: */
    {
      REW_WRATH, REW_CURSE_AR, REW_CURSE_WP, REW_CURSE_WP, REW_CURSE_AR,
      REW_IGNORE, REW_IGNORE, REW_IGNORE, REW_POLY_SLF, REW_POLY_SLF,
      REW_POLY_WND, REW_HEAL_FUL, REW_HEAL_FUL, REW_GAIN_EXP, REW_AUGM_ABL,
      REW_GOOD_OBJ, REW_GOOD_OBJ, REW_CHAOS_WP, REW_GREA_OBJ, REW_GREA_OBS
    },

    /* Bagog, the Scorpion Queen: */
    {
      REW_WRATH, REW_SER_DEMO, REW_CURSE_WP, REW_CURSE_AR, REW_LOSE_EXP,
      REW_GAIN_ABL, REW_LOSE_ABL, REW_POLY_WND, REW_POLY_SLF, REW_IGNORE,
      REW_DESTRUCT, REW_MASS_GEN, REW_CHAOS_WP, REW_GREA_OBJ, REW_HURT_LOT,
      REW_AUGM_ABL, REW_RUIN_ABL, REW_H_SUMMON, REW_GREA_OBS, REW_AUGM_ABL
    },

    /* Gark the Calm: */
    {
      REW_WRATH, REW_HURT_LOT, REW_HURT_LOT, REW_H_SUMMON, REW_H_SUMMON,
      REW_IGNORE, REW_IGNORE, REW_IGNORE, REW_SER_MONS, REW_SER_DEMO,
      REW_POLY_SLF, REW_POLY_WND, REW_HEAL_FUL, REW_GOOD_OBJ, REW_GOOD_OBJ,
      REW_CHAOS_WP, REW_GOOD_OBS, REW_GOOD_OBS, REW_GREA_OBJ, REW_GREA_OBS
    },

    /* Ikadz, Lord of Torture: */
    {
      REW_WRATH, REW_PISS_OFF, REW_PISS_OFF, REW_RUIN_ABL, REW_LOSE_ABL,
      REW_LOSE_EXP, REW_IGNORE, REW_IGNORE, REW_POLY_WND, REW_SER_DEMO,
      REW_POLY_SLF, REW_HEAL_FUL, REW_HEAL_FUL, REW_GOOD_OBJ, REW_GAIN_EXP,
      REW_GAIN_EXP, REW_CHAOS_WP, REW_GAIN_ABL, REW_GREA_OBJ, REW_AUGM_ABL
    },

    /* Kajabor: */
    {
      REW_WRATH, REW_PISS_OFF, REW_HURT_LOT, REW_RUIN_ABL, REW_LOSE_ABL,
      REW_LOSE_EXP, REW_IGNORE, REW_IGNORE, REW_IGNORE, REW_POLY_SLF,
      REW_POLY_SLF, REW_POLY_WND, REW_HEAL_FUL, REW_GOOD_OBJ, REW_GAIN_ABL,
      REW_GAIN_ABL, REW_SER_UNDE, REW_CHAOS_WP, REW_GREA_OBJ, REW_AUGM_ABL
    },

    /* Krjalk, the Traitor: */
    {
      REW_WRATH, REW_CURSE_WP, REW_CURSE_AR, REW_RUIN_ABL, REW_LOSE_ABL,
      REW_LOSE_EXP, REW_IGNORE, REW_POLY_SLF, REW_POLY_SLF, REW_POLY_SLF,
      REW_POLY_SLF, REW_POLY_WND, REW_HEAL_FUL, REW_CHAOS_WP, REW_GREA_OBJ,
      REW_GAIN_ABL, REW_GAIN_ABL, REW_GAIN_EXP, REW_GAIN_EXP, REW_AUGM_ABL
    },

    /* Ompalam, Lord of Slavery: */
    {
      REW_WRATH, REW_HURT_LOT, REW_PISS_OFF, REW_LOSE_ABL, REW_LOSE_EXP,
      REW_IGNORE,   REW_IGNORE,   REW_DISPEL_C, REW_DO_HAVOC, REW_DO_HAVOC,
      REW_POLY_SLF, REW_POLY_SLF, REW_GAIN_EXP, REW_GAIN_ABL, REW_GAIN_ABL,
      REW_SER_MONS, REW_GOOD_OBJ, REW_CHAOS_WP, REW_GREA_OBJ, REW_GOOD_OBS
    }
};

#ifdef VERBOSE_MARTIAL_ARTS
#define MA(BRIEF,VERBOSE) VERBOSE
#else /* VERBOSE_MARTIAL_ARTS */
#define MA(BRIEF,VERBOSE) BRIEF
#endif /* VERBOSE_MARTIAL_ARTS */

martial_arts ma_blows[MAX_MA+1] =
{
	{ "You punch %v.",                          2, 0, 1, 4, 0 },
	{ "You kick %v.",                           4, 0, 1, 6, 0 },
	{ "You strike %v.",                         6, 0, 1, 7, 0 },
	{MA("You knee %v.", "You hit %v with your knee."), 10, 9, 2, 3, MA_KNEE },
	{MA("You hit %v.", "You hit %v with your elbow."), 14, 9, 1, 8, 0 },
	{ "You butt %v.",                           18, 19, 2, 5, 0 },
	{ "You kick %v.",                           22, 19, 3, 4, MA_SLOW },
	{ "You uppercut %v.",                       26, 23, 4, 4, 6 },
	{ "You double-kick %v.",                    32, 29, 5, 4, 8 },
	{MA("You hit %v.", "You hit %v with a Cat's Claw."), 40, 39, 5, 5, 0 },
	{MA("You kick %v.", "You hit %v with a jump kick."), 50, 49, 5, 6, 10 },
	{MA("You hit %v.", "You hit %v with an Eagle's Claw."), 58, 49, 6, 6, 0 },
	{MA("You kick %v.", "You hit %v with a circle kick."), 66, 59, 6, 8, 10 },
	{MA("You punch %v.", "You hit %v with an Iron Fist."), 74, 69, 8, 8, 10 },
	{MA("You kick %v.", "You hit %v with a flying kick."), 82, 69, 8, 10, 12 },
	{MA("You punch %v.", "You hit %v with a Dragon Fist."),
		90, 69, 10, 10, 16},
	{MA("You punch %v.", "You hit %v with a Crushing Blow."),
		96, 69, 10, 12, 18},

	/* A weak unarmed attack for unskilled characters. */
	{ "You hit %v.",							0, 0, 1, 1, 0},
};

/*
 * Window definitions. Defined here to ensure that the names are available 
 * before any code which isn't system specific is run.
 * Everything else is blank, to be filled in later.
 */

#define BLANK_32 \
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

#define BLANK_WINDOW(name) \
{0, name, BLANK_32, BLANK_32, iilog(PW_NONE), 0},

window_type windows[ANGBAND_TERM_MAX] = {
BLANK_WINDOW(GAME_NAME)
BLANK_WINDOW("Mirror")
BLANK_WINDOW("Recall")
BLANK_WINDOW("Choice")
BLANK_WINDOW("Xtra-1")
BLANK_WINDOW("Xtra-2")
BLANK_WINDOW("Xtra-3")
BLANK_WINDOW("Xtra-4")
};

/*
 * The array of monster recall colour options
 */
moncol_type moncol[MAX_MONCOL] = {
{"Deaths", TERM_WHITE},
{"Flavour text", TERM_WHITE},
{"Depth/speed", TERM_WHITE},
{"Defensive auras", TERM_WHITE},
{"Escort", TERM_WHITE},
{"Inate attacks", TERM_WHITE},
{"Breath attacks", TERM_WHITE},
{"Magical attacks", TERM_WHITE},
{"AC and HP", TERM_WHITE},
{"Door/wall/monster/object abilities.", TERM_WHITE},
{"Ease of detection, breeding and regeneration.", TERM_WHITE},
{"Weaknesses", TERM_WHITE},
{"Elemental resistances", TERM_WHITE},
{"Non-elemental resistances", TERM_WHITE},
{"Non-elemental immunities", TERM_WHITE},
{"How observant a monster is", TERM_WHITE},
{"Drop at point of death", TERM_WHITE},
{"Melee attacks", TERM_WHITE},
{"Quest monster status", TERM_WHITE},
};


wild_type wild_grid[12][12] = {
	/* {terrain,town,seed,roadmap,dun_min,dun_max}
	 * all entries except terrain are zeroed at this point, with values being filled in later
	 *
	 * terrain 0 = impassable sea
	 * terrain 1 = coast
	 * terrains 2 - 5 are depths of forest
	 *
	 */
	{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},},
	{ {0,0,0,0,0,0},{1,0,0,0,0,0},{1,0,0,0,0,0},{1,0,0,0,0,0},{1,0,0,0,0,0},{1,0,0,0,0,0},{1,0,0,0,0,0},{1,0,0,0,0,0},{1,0,0,0,0,0},{1,0,0,0,0,0},{1,0,0,0,0,0},{0,0,0,0,0,0},},
	{ {0,0,0,0,0,0},{1,0,0,0,0,0},{2,0,0,0,0,0},{2,0,0,0,0,0},{2,0,0,0,0,0},{4,0,0,0,0,0},{2,0,0,0,0,0},{3,0,0,0,0,0},{2,0,0,0,0,0},{2,0,0,0,0,0},{1,0,0,0,0,0},{0,0,0,0,0,0},},
	{ {0,0,0,0,0,0},{1,0,0,0,0,0},{2,0,0,0,0,0},{3,0,0,0,0,0},{4,0,0,0,0,0},{5,0,0,0,0,0},{4,0,0,0,0,0},{4,0,0,0,0,0},{3,0,0,0,0,0},{2,0,0,0,0,0},{1,0,0,0,0,0},{0,0,0,0,0,0},},
	{ {0,0,0,0,0,0},{1,0,0,0,0,0},{2,0,0,0,0,0},{4,0,0,0,0,0},{5,0,0,0,0,0},{5,0,0,0,0,0},{5,0,0,0,0,0},{4,0,0,0,0,0},{3,0,0,0,0,0},{2,0,0,0,0,0},{1,0,0,0,0,0},{0,0,0,0,0,0},},
	{ {0,0,0,0,0,0},{1,0,0,0,0,0},{2,0,0,0,0,0},{3,0,0,0,0,0},{4,0,0,0,0,0},{5,0,0,0,0,0},{4,0,0,0,0,0},{4,0,0,0,0,0},{3,0,0,0,0,0},{2,0,0,0,0,0},{1,0,0,0,0,0},{0,0,0,0,0,0},},
	{ {0,0,0,0,0,0},{1,0,0,0,0,0},{3,0,0,0,0,0},{4,0,0,0,0,0},{4,0,0,0,0,0},{4,0,0,0,0,0},{4,0,0,0,0,0},{5,0,0,0,0,0},{4,0,0,0,0,0},{3,0,0,0,0,0},{1,0,0,0,0,0},{0,0,0,0,0,0},},
	{ {0,0,0,0,0,0},{1,0,0,0,0,0},{3,0,0,0,0,0},{4,0,0,0,0,0},{5,0,0,0,0,0},{4,0,0,0,0,0},{3,0,0,0,0,0},{4,0,0,0,0,0},{3,0,0,0,0,0},{2,0,0,0,0,0},{1,0,0,0,0,0},{0,0,0,0,0,0},},
	{ {0,0,0,0,0,0},{1,0,0,0,0,0},{2,0,0,0,0,0},{3,0,0,0,0,0},{4,0,0,0,0,0},{3,0,0,0,0,0},{2,0,0,0,0,0},{3,0,0,0,0,0},{3,0,0,0,0,0},{2,0,0,0,0,0},{1,0,0,0,0,0},{0,0,0,0,0,0},},
	{ {0,0,0,0,0,0},{1,0,0,0,0,0},{2,0,0,0,0,0},{3,0,0,0,0,0},{3,0,0,0,0,0},{2,0,0,0,0,0},{2,0,0,0,0,0},{2,0,0,0,0,0},{2,0,0,0,0,0},{2,0,0,0,0,0},{1,0,0,0,0,0},{0,0,0,0,0,0},},
	{ {0,0,0,0,0,0},{1,0,0,0,0,0},{1,0,0,0,0,0},{1,0,0,0,0,0},{1,0,0,0,0,0},{1,0,0,0,0,0},{1,0,0,0,0,0},{1,0,0,0,0,0},{1,0,0,0,0,0},{1,0,0,0,0,0},{1,0,0,0,0,0},{0,0,0,0,0,0},},
	{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},},
};

/* Shamanic spirits */
spirit_type spirits[MAX_SPIRITS] =
{
	{"","a minor life spirit",0x000000ff,0,0,SPIRIT_LIFE,1, 15},
	{"","a minor wild spirit",0x000000ff,0,0,SPIRIT_NATURE,1, 15},

	{"","a lesser life spirit",0x0000ff00,0,0,SPIRIT_LIFE,7, 30},
	{"","a lesser wild spirit",0x0000ff00,0,0,SPIRIT_NATURE,5, 30},

	{"","a greater life spirit",0x00ff0000,0,0,SPIRIT_LIFE,15, 45},
	{"","a greater wild spirit",0x00ff0000,0,0,SPIRIT_NATURE,7, 45},

	{"","a major life spirit",0xff000000,0,0,SPIRIT_LIFE,5, 60},
	{"","a major wild spirit",0xff000000,0,0,SPIRIT_NATURE,20, 60},
};

/*
 * Hack -- possible "insult" messages
 */
static cptr desc_insult[] =
{
	"%^s insults you!",
	"%^s insults your mother!",
	"%^s gives you the finger!",
	"%^s humiliates you!",
	"%^s defiles you!",
	"%^s dances around you!",
	"%^s makes obscene gestures!",
	"%^s moons you!",
	0
};

/*
 * Hack -- possible "hero worship" messages
 */
static cptr desc_worship[] =
{
	"%^s looks up at you!",
	"%^s asks how many dragons you've killed!",
	"%^s asks for your autograph!",
	"%^s tries to shake your hand!",
	"%^s pretends to be you!",
	"%^s dances around you!",
	"%^s tugs at your clothing!",
	"%^s asks if you will adopt him!",
	0
};


/*
 * Hack -- possible "moan" messages
 */
static cptr desc_moan[] =
{
	"%^s seems sad about something.",
	"%^s asks if you have seen his dogs.",
	"%^s tells you to get off his land.",
	"%^s mumbles something about mushrooms.",
	0
};

blow_method_type blow_methods[NUM_BLOW_METHODS] =
{
	{RBF_TOUCH | RBF_CUT | RBF_STUN, "hit", "%^s hits %s.", NULL, "%^s misses %s.", "HIT"},
	{RBF_TOUCH, "touch", "%^s touches %s.", NULL, "%^s misses %s.", "TOUCH"},
	{RBF_TOUCH | RBF_STUN, "punch", "%^s punches %s.", NULL, "%^s misses %s.", "PUNCH"},
	{RBF_TOUCH | RBF_STUN, "kick", "%^s kicks %s.", NULL, "%^s misses %s.", "KICK"},
	{RBF_TOUCH | RBF_CUT, "claw", "%^s claws %s.", NULL, "%^s misses %s.", "CLAW"},
	{RBF_TOUCH | RBF_CUT, "bite", "%^s bites %s.", NULL, "%^s misses %s.", "BITE"},
	{RBF_TOUCH, "sting", "%^s stings %s.", NULL, "%^s misses %s.", "STING"},
	{RBF_TOUCH | RBF_STUN, "butt", "%^s butts %s.", NULL, "%^s misses %s.", "BUTT"},
	{RBF_TOUCH | RBF_STUN, "crush", "%^s crushes %s.", NULL, "%^s misses %s.", "CRUSH"},
	{RBF_TOUCH, "engulf", "%^s engulfs %s.", NULL, "%^s misses %s.", "ENGULF"},
	{RBF_TOUCH, "crawl on you", "%^s crawl on %s.", NULL, NULL, "CRAWL"},
	{0, "drool on you", "%^s drool on %s.", NULL, NULL, "DROOL"},
	{0, "spit", "%^s spits on %s.", NULL, NULL, "SPIT"},
	{0, "gaze", "%^s gazes at %s.", NULL, NULL, "GAZE"},
	{0, "wail", "%^s wails at %s.", NULL, NULL, "WAIL"},
	{0, "release spores", "%^s release spores at %s.", NULL, NULL, "SPORE"},
	{0, "hero worship", "%^s hero worships %s.", desc_worship, NULL, "WORSHIP"},
	{RBF_WAKE, "beg", "%^s begs %s for money.", NULL, NULL, "BEG"},
	{RBF_WAKE, "insult", "%^s insults %s.", desc_insult, NULL, "INSULT"},
	{RBF_WAKE, "moan", "%^s moans at %s.", desc_moan, NULL, "MOAN"},
};

/*
 * The initial co-ordinates of the things displayed by do_cmd_redraw().
 */
redraw_type screen_coords[NUM_SCREEN_COORDS] =
{
	{IDX(XY_TIME) "time", 0, 1, BORDER_WIDTH},
	{IDX(XY_GOLD) "gold", 0, 3, BORDER_WIDTH},
	{IDX(XY_EQUIPPY) "equippy", 0, 4, BORDER_WIDTH},
	{IDX(XY_STAT+A_STR) "strength", 0, 5, BORDER_WIDTH},
	{IDX(XY_STAT+A_INT) "intelligence", 0, 6, BORDER_WIDTH},
	{IDX(XY_STAT+A_WIS) "wisdom", 0, 7, BORDER_WIDTH},
	{IDX(XY_STAT+A_DEX) "dexterity", 0, 8, BORDER_WIDTH},
	{IDX(XY_STAT+A_CON) "constitution", 0, 9, BORDER_WIDTH},
	{IDX(XY_STAT+A_CHR) "charisma", 0, 10, BORDER_WIDTH},
	{IDX(XY_AC) "armour", 0, 12, BORDER_WIDTH},
	{IDX(XY_HP) "hit points", 0, 13, BORDER_WIDTH},
	{IDX(XY_SP) "spell points", 0, 14, BORDER_WIDTH},
	{IDX(XY_CHI) "chi points", 0, 15, BORDER_WIDTH},
	{IDX(XY_LIFE_SPIRIT) "life spirits", 0, 17, BORDER_WIDTH},
	{IDX(XY_WILD_SPIRIT) "wild spirits", 0, 18, BORDER_WIDTH},
	{IDX(XY_INFO) "monster state", 0, 19, BORDER_WIDTH},
	{IDX(XY_ENERGY) "energy used", 0, 20, BORDER_WIDTH},
	{IDX(XY_CUT) "cut", 0, 21, BORDER_WIDTH},
	{IDX(XY_STUN) "stunning", 0, 22, BORDER_WIDTH},
	{IDX(XY_HUNGRY) "hunger", 0, 23, 6},
	{IDX(XY_BLIND) "blindness", 7, -1, 5},
	{IDX(XY_CONFUSED) "confusion", 13, -1, 8},
	{IDX(XY_AFRAID) "fear", 22, -1, 6},
	{IDX(XY_POISONED) "poison", 29, -1, 8},
	{IDX(XY_STATE) "searching", 38, -1, 10},
	{IDX(XY_SPEED) "speed", 51, -1, 14},
	{IDX(XY_STUDY) "study", 63, -1, 5},
	{IDX(XY_DEPTH) "depth", 69, -1, 9},
	{IDX(XY_PARALYSED) "paralysis", 0, 0, 0},
	{IDX(XY_IMAGE) "hallucination", 0, 0, 0},
	{IDX(XY_FAST) "temporary speed", 0, 0, 0},
	{IDX(XY_SLOW) "temporary slowing", 0, 0, 0},
	{IDX(XY_SHIELD) "Stone skin", 0, 0, 0},
	{IDX(XY_BLESSED) "Blessedness", 0, 0, 0},
	{IDX(XY_HERO) "Heroism", 0, 0, 0},
	{IDX(XY_BERSERK) "Berserkness", 0, 0, 0},
	{IDX(XY_PROTEVIL) "Protection from Evil", 0, 0, 0},
	{IDX(XY_WRAITH) "Wraith form", 0, 0, 0},
	{IDX(XY_INVULN) "Invulnerability", 0, 0, 0},
	{IDX(XY_ESP) "Temporary ESP", 0, 0, 0},
	{IDX(XY_INVIS) "Ability to see invisible things", 0, 0, 0},
	{IDX(XY_INFRA) "Extra infra-red vision", 0, 0, 0},
	{IDX(XY_RACID) "Acid resistance", 0, 0, 0},
	{IDX(XY_RELEC) "Electricity resistance", 0, 0, 0},
	{IDX(XY_RFIRE) "Fire resistance", 0, 0, 0},
	{IDX(XY_RCOLD) "Cold resistance", 0, 0, 0},
	{IDX(XY_RPOIS) "Poison resistance", 0, 0, 0},
};

/* A list of the flags for explosion types understood by project(). */
cptr explode_flags[] =
{
	"ELEC",
	"POIS",
	"ACID",
	"COLD",
	"FIRE",
	"","","","",
	"MISSILE",
	"ARROW",
	"PLASMA",
	"",
	"WATER",
	"LITE",
	"DARK",
	"LITE_WEAK",
	"DARK_WEAK",
	"",
	"SHARDS",
	"SOUND",
	"CONFUSION",
	"FORCE",
	"INERTIA",
	"",
	"MANA",
	"METEOR",
	"ICE",
	"",
	"CHAOS",
	"NETHER",
	"DISENCHANT",
	"NEXUS",
	"TIME",
	"GRAVITY",
	"","","","",
	"KILL_WALL",
	"KILL_DOOR",
	"KILL_TRAP",
	"","","",
	"MAKE_DOOR",
	"MAKE_TRAP",
	"","","",
	"OLD_CLONE",
	"OLD_POLY",
	"OLD_HEAL",
	"OLD_SPEED",
	"OLD_SLOW",
	"OLD_CONF",
	"OLD_SLEEP",
	"OLD_DRAIN",
	"","",
	"AWAY_UNDEAD",
	"AWAY_EVIL",
	"AWAY_ALL",
	"TURN_UNDEAD",
	"TURN_EVIL",
	"TURN_ALL",
	"DISP_UNDEAD",
	"DISP_EVIL",
	"DISP_ALL",
	"DISP_DEMON",
	"DISP_LIVING",
	"SHARD",
	"NUKE",
	"MAKE_GLYPH",
	"STASIS",
	"STONE_WALL",
	"DEATH_RAY",
	"STUN",
	"HOLY_FIRE",
	"HELL_FIRE",
	"DISINTEGRATE",
	"CHARM",
	"CONTROL_UNDEAD",
	"CONTROL_ANIMAL",
	"PSI",
	"PSI_DRAIN",
	"TELEKINESIS",
	"JAM_DOOR",
	"DOMINATION",
	"DISP_GOOD",
	NULL
};

/* A list of the types of coins. This can't be taken from k_info.txt because of
 * the duplicate entries. */
cptr coin_types[] =
{
	"","","COPPER", /* 482 */
	"","","SILVER", /* 485 */
	"","","","","GOLD", /* 490 */
	"","","","","","MITHRIL", /* 496 */
	"ADAMANTIUM", /* 497 */
	NULL
};

/*
 * The table of "symbol info" -- each entry is a string of the form
 * "X:desc" where "X" is the trigger, and "desc" is the "info".
 */
name_centry ident_info[] =
{
	{' ', "A dark grid"},
	{'!', "A potion (or oil)"},
	{'"', "An amulet (or necklace)"},
	{'#', "A wall (or secret door)"},
	{'$', "Treasure (gold or gems)"},
	{'%', "A vein (magma or quartz)"},
	/* {'&', "unused"}, */
	{'\'', "An open door"},
	{'(', "Soft armor"},
	{')', "A shield"},
	{'*', "A vein with treasure"},
	{'+', "A closed door"},
	{',', "Food (or mushroom patch)"},
	{'-', "A wand (or rod)"},
	{'.', "Floor"},
	{'/', "A polearm (Axe/Pike/etc)"},
	/* {'0', "unused"}, */
	{'1', "Entrance to General Store"},
	{'2', "Entrance to Armory"},
	{'3', "Entrance to Weaponsmith"},
	{'4', "Entrance to Temple"},
	{'5', "Entrance to Alchemy shop"},
	{'6', "Entrance to Magic store"},
	{'7', "Entrance to Black Market"},
	{'8', "Entrance to your home"},
	{'9', "Entrance to Book Store"},
	{':', "Rubble"},
    {';', "A glyph of warding / explosive rune"},
	{'<', "An up staircase"},
	{'=', "A ring"},
	{'>', "A down staircase"},
	{'?', "A scroll"},
	{'@', "You"},
	{'A', "Golem"},
	{'B', "Bird"},
	{'C', "Canine"},
	{'D', "Ancient Dragon/Wyrm"},
	{'E', "Elemental"},
	{'F', "Dragon Fly"},
	{'G', "Ghost"},
	{'H', "Hybrid"},
	{'I', "Insect"},
	{'J', "Snake"},
	{'K', "Killer Beetle"},
	{'L', "Lich"},
	{'M', "Multi-Headed Reptile"},
	/* {'N', "unused"}, */
	{'O', "Ogre"},
	{'P', "Giant Humanoid"},
	{'Q', "Quylthulg (Pulsing Flesh Mound)"},
	{'R', "Reptile/Amphibian"},
	{'S', "Spider/Scorpion/Tick"},
	{'T', "Troll"},
	{'U', "Major Demon"},
	{'V', "Vampire"},
	{'W', "Wight/Wraith/etc"},
	{'X', "Xorn/Xaren/etc"},
	{'Y', "Yeti"},
	{'Z', "Zephyr Hound"},
	{'[', "Hard armor"},
	{'\\', ":A hafted weapon (mace/whip/etc)"},
	{']', "Misc. armor"},
	{'^', "A trap"},
	{'_', "A staff"},
	/* {'`', "unused"}, */
	{'a', "Ant"},
	{'b', "Bat"},
	{'c', "Centipede"},
	{'d', "Dragon"},
	{'e', "Floating Eye"},
	{'f', "Feline"},
	{'g', "Ghoul"},
	{'h', "Hobbit/Elf/Dwarf"},
	{'i', "Beings of Ib"},
	{'j', "Jelly"},
	{'k', "Kobold"},
	{'l', "Louse"},
	{'m', "Mold"},
	{'n', "Naga"},
	{'o', "Orc"},
	{'p', "Person/Human"},
	{'q', "Quadruped"},
	{'r', "Rodent"},
	{'s', "Skeleton"},
	{'t', "Townsperson"},
	{'u', "Minor Demon"},
	{'v', "Vortex"},
	{'w', "Worm/Worm-Mass"},
	/* {'x', "unused"}, */
	{'y', "Yeek"},
	{'z', "Zombie/Mummy"},
	{'{', "A missile (arrow/bolt/shot)"},
	{'|', "An edged weapon (sword/dagger/etc)"},
	{'}', "A launcher (bow/crossbow/sling)"},
	{'~', "A tool (or miscellaneous item)"},
	{0, "Unknown Symbol"}
};

cptr_ch feeling_str[SENSE_MAX] =
{
	{IDX(SENSE_NONE) ""},
	{IDX(SENSE_C_ART) "terrible"},
	{IDX(SENSE_C_EGO) "worthless"},
	{IDX(SENSE_CP_OBJ) "very bad"},
	{IDX(SENSE_C_OBJ) "cursed"},
	{IDX(SENSE_BROKEN) "useless"},
	{IDX(SENSE_U_OBJ) "average"},
	{IDX(SENSE_G_OBJ) "good"},
	{IDX(SENSE_GP_OBJ) "very good"},
	{IDX(SENSE_G_EGO) "excellent"},
	{IDX(SENSE_G_ART) "special"},
	{IDX(SENSE_EMPTY) "empty"},
	{IDX(SENSE_TRIED) "tried"},
	{IDX(SENSE_PCURSE) "poss. cursed"},
	{IDX(SENSE_Q_OBJ) "uncursed"},
	{IDX(SENSE_QP_OBJ) "powerful"},
	{IDX(SENSE_Q_ART) "unbreakable"},
};

cptr option_chars =
	"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789;:'@#~<>/?";

natural_attack natural_attacks[5] =
{
	{MUT_SCOR_TAIL, 7, 3, 5, GF_POIS, "tail"},
	{MUT_HORNS, 6, 2, 15, GF_HIT, "horns"},
	{MUT_BEAK, 4, 2, 5, GF_HIT, "beak"},
	{MUT_TRUNK, 4, 1, 35, GF_HIT, "trunk"},
	{MUT_TENTACLES, 5, 2, 5, GF_HELL_FIRE, "tentacles"},
};
