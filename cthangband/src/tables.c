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
byte adj_mag_study[] =
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
byte adj_mag_mana[] =
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
byte adj_mag_fail[] =
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
byte adj_mag_stat[] =
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
byte adj_chr_gold[] =
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
byte adj_int_dev[] =
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
byte adj_wis_sav[] =
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
byte adj_dex_dis[] =
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
byte adj_int_dis[] =
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
byte adj_dex_ta[] =
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
byte adj_str_td[] =
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
byte adj_dex_th[] =
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
byte adj_str_th[] =
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
byte adj_str_wgt[] =
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
byte adj_str_hold[] =
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
byte adj_str_dig[] =
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
byte adj_str_blow[] =
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
byte adj_dex_blow[] =
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
	12      /* 18/160-18/169 */,
	14      /* 18/170-18/179 */,
	16      /* 18/180-18/189 */,
	18      /* 18/190-18/199 */,
	20      /* 18/200-18/209 */,
	20      /* 18/210-18/219 */,
	20      /* 18/220+ */
};


/*
 * Stat Table (DEX) -- chance of avoiding "theft" and "falling"
 */
byte adj_dex_safe[] =
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
byte adj_con_fix[] =
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
byte adj_con_mhp[] =
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
 * This number ranges from a single blow/round for weak players to up to six
 * blows/round for powerful warriors.
 *
 * Note that certain artifacts and ego-items give "bonus" blows/round.
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
	/* 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11+ */

	/* 0  */
	{  1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   3 },

	/* 1  */
	{  1,   1,   1,   1,   2,   2,   3,   3,   3,   4,   4,   4 },

	/* 2  */
	{  1,   1,   2,   2,   3,   3,   4,   4,   4,   5,   5,   5 },

	/* 3  */
	{  1,   2,   2,   3,   3,   4,   4,   4,   5,   5,   5,   5 },

	/* 4  */
	{  1,   2,   2,   3,   3,   4,   4,   5,   5,   5,   5,   5 },

	/* 5  */
	{  2,   2,   3,   3,   4,   4,   5,   5,   5,   5,   5,   6 },

	/* 6  */
	{  2,   2,   3,   3,   4,   4,   5,   5,   5,   5,   5,   6 },

	/* 7  */
	{  2,   3,   3,   4,   4,   4,   5,   5,   5,   5,   5,   6 },

	/* 8  */
	{  3,   3,   3,   4,   4,   4,   5,   5,   5,   5,   6,   6 },

	/* 9  */
	{  3,   3,   4,   4,   4,   4,   5,   5,   5,   5,   6,   6 },

	/* 10 */
	{  3,   3,   4,   4,   4,   4,   5,   5,   5,   6,   6,   6 },

	/* 11+ */
	{  3,   3,   4,   4,   4,   4,   5,   5,   6,   6,   6,   6 },
};



/*
 * Store owners (exactly four "possible" owners per store, chosen randomly)
 * { name, purse, max greed, min greed, haggle_per, tolerance, race }
 */
owner_type owners[NUM_OWNERS] =
{
	/* Non-shops. */
	{"Your home", 0, 100, 100, 0, 99, 0, STORE_HOME},
	{"Hall of Records", 0, 100, 100, 0, 99, 0, STORE_HALL},
	/* Real shopkeepers. */
	{"Falilmawen the Friendly", 250, 170, 108, 5, 15, RACE_HOBBIT, STORE_GENERAL},
	{"Voirin the Cowardly", 500, 175, 108, 4, 12, RACE_HUMAN, STORE_GENERAL},
	{"Erashnak the Midget", 750, 170, 107, 5, 15, RACE_BROO, STORE_GENERAL},
	{"Grug the Comely", 1000, 165, 107, 6, 18, RACE_HALF_TITAN, STORE_GENERAL},
	{"Kon-Dar the Ugly", 10000, 210, 115, 5, 7, RACE_HALF_ORC, STORE_ARMOURY},
	{"Darg-Low the Grim", 15000, 190, 111, 4, 9, RACE_HUMAN, STORE_ARMOURY},
	{"Decado the Handsome", 25000, 200, 112, 4, 10, RACE_GREAT, STORE_ARMOURY},
	{"Elo Dragonscale", 30000, 200, 112, 4, 5, RACE_ELF, STORE_ARMOURY},
	{"Arnold the Beastly", 10000, 210, 115, 6, 6, RACE_BARBARIAN, STORE_WEAPON},
	{"Arndal Beast-Slayer", 15000, 185, 110, 5, 9, RACE_HALF_ELF, STORE_WEAPON},
	{"Edor the Short", 25000, 190, 115, 5, 7, RACE_HOBBIT, STORE_WEAPON},
	{"Oglign Dragon-Slayer", 30000, 195, 112, 4, 8, RACE_DWARF, STORE_WEAPON},
	{"Ludwig the Humble", 10000, 175, 109, 6, 15, RACE_DWARF, STORE_TEMPLE},
	{"Gunnar the Paladin", 15000, 185, 110, 5, 23, RACE_HALF_TROLL, STORE_TEMPLE},
	{"Sir Parsival the Pure", 25000, 180, 107, 6, 20, RACE_HIGH_ELF, STORE_TEMPLE},
	{"Asenath the Holy", 30000, 185, 109, 5, 15, RACE_HUMAN, STORE_TEMPLE},
	{"McKinnon", 10000, 175, 109, 6, 15, RACE_HUMAN, STORE_TEMPLE},
	{"Mistress Chastity", 15000, 185, 110, 5, 23, RACE_HIGH_ELF, STORE_TEMPLE},
	{"Hashnik the Druid", 25000, 180, 107, 6, 20, RACE_HOBBIT, STORE_TEMPLE},
	{"Finak", 30000, 185, 109, 5, 15, RACE_YEEK, STORE_TEMPLE},
	{"Mauser the Chemist", 10000, 190, 111, 5, 8, RACE_HALF_ELF, STORE_ALCHEMIST},
	{"Wizzle the Chaotic", 10000, 190, 110, 6, 8, RACE_HOBBIT, STORE_ALCHEMIST},
	{"Kakalrakakal", 15000, 200, 116, 6, 9, RACE_KLACKON, STORE_ALCHEMIST},
	{"Jal-Eth the Alchemist", 15000, 220, 111, 4, 9, RACE_ELF, STORE_ALCHEMIST},
	{"Skidney the Sorcerer", 15000, 200, 110, 7, 8, RACE_HALF_ELF, STORE_MAGIC},
	{"Buggerby the Great", 20000, 215, 113, 6, 10, RACE_GNOME, STORE_MAGIC},
	{"Kyria the Illusionist", 30000, 200, 110, 7, 10, RACE_HUMAN, STORE_MAGIC},
	{"Nikki the Necromancer", 30000, 175, 110, 5, 11, RACE_DARK_ELF, STORE_MAGIC},
	{"Randolph Carter", 15000, 175, 108, 4, 12, RACE_HUMAN, STORE_LIBRARY},
	{"Odnar the Sage", 20000, 120, 105, 6, 16, RACE_HIGH_ELF, STORE_LIBRARY},
	{"Gandar the Neutral", 25000, 120, 110, 7, 19, RACE_VAMPIRE, STORE_LIBRARY},
	{"Ro-sha the Patient", 30000, 140, 105, 6, 12, RACE_GOLEM, STORE_LIBRARY},
	{"Mordsan", 15000, 175, 108, 4, 12, RACE_HUMAN, STORE_INN},
	{"Furfoot Pobber", 20000, 120, 105, 6, 16, RACE_HOBBIT, STORE_INN},
	{"Oddo Yeekson", 25000, 120, 110, 7, 19, RACE_YEEK, STORE_INN},
	{"Dry-Bones", 30000, 140, 105, 6, 12, RACE_SKELETON, STORE_INN},
	{"Magd the Ruthless", 2000, 100, 100, 4, 12, RACE_HUMAN, STORE_PAWN},
	{"Drako Fairdeal", 4000, 100, 100, 6, 16, RACE_DRACONIAN, STORE_PAWN},
	{"Featherwing", 5000, 100, 100, 7, 19, RACE_SPRITE, STORE_PAWN},
	{"Xochinaggua", 10000, 100, 100, 6, 12, RACE_MIND_FLAYER, STORE_PAWN},
	{"Forovir the Cheap", 250, 170, 108, 5, 15, RACE_HUMAN, STORE_GENERAL},
	{"Ellis the Fool", 500, 175, 108, 4, 12, RACE_HUMAN, STORE_GENERAL},
	{"Filbert the Hungry", 750, 170, 107, 5, 15, RACE_VAMPIRE, STORE_GENERAL},
	{"Fthnargl Psathiggua", 1000, 165, 107, 6, 18, RACE_MIND_FLAYER, STORE_GENERAL},
	{"Delicatus", 10000, 210, 115, 5, 7, RACE_SPRITE, STORE_ARMOURY},
	{"Gruce the Huge", 15000, 190, 111, 4, 9, RACE_HALF_GIANT, STORE_ARMOURY},
	{"Animus", 25000, 200, 112, 4, 10, RACE_GOLEM, STORE_ARMOURY},
	{"Malvus", 30000, 200, 112, 4, 5, RACE_HALF_TITAN, STORE_ARMOURY},
	{"Drew the Skilled", 10000, 210, 115, 6, 6, RACE_HUMAN, STORE_WEAPON},
	{"Orrax Dragonson", 15000, 185, 110, 5, 9, RACE_DRACONIAN, STORE_WEAPON},
	{"Anthrax Disease-Carrier", 25000, 190, 115, 5, 7, RACE_BROO, STORE_WEAPON},
	{"Arkhoth the Stout", 30000, 195, 112, 4, 8, RACE_DWARF, STORE_WEAPON},
	{"Krikkik", 10000, 175, 109, 6, 15, RACE_KLACKON, STORE_TEMPLE},
	{"Morival the Wild", 15000, 185, 110, 5, 23, RACE_ELF, STORE_TEMPLE},
	{"Hoshak the Dark", 25000, 180, 107, 6, 20, RACE_IMP, STORE_TEMPLE},
	{"Atal the Wise", 30000, 185, 109, 5, 15, RACE_HUMAN, STORE_TEMPLE},
	{"Fanelath the Cautious", 10000, 190, 111, 5, 8, RACE_DWARF, STORE_ALCHEMIST},
	{"Runcie the Insane", 10000, 190, 110, 6, 8, RACE_HUMAN, STORE_ALCHEMIST},
	{"Grumbleworth", 15000, 200, 116, 6, 9, RACE_GNOME, STORE_ALCHEMIST},
	{"Flitter", 15000, 220, 111, 4, 9, RACE_SPRITE, STORE_ALCHEMIST},
	{"Solostoran", 15000, 200, 110, 7, 8, RACE_SPRITE, STORE_MAGIC},
	{"Achshe the Tentacled", 20000, 215, 113, 6, 10, RACE_MIND_FLAYER, STORE_MAGIC},
	{"Kaza the Noble", 30000, 200, 110, 7, 10, RACE_HIGH_ELF, STORE_MAGIC},
	{"Fazzil the Dark", 30000, 175, 110, 5, 11, RACE_DARK_ELF, STORE_MAGIC},
	{"Sarai the Swift", 15000, 175, 108, 4, 12, RACE_HUMAN, STORE_LIBRARY},
	{"Bodril the Seer", 20000, 120, 105, 6, 16, RACE_HIGH_ELF, STORE_LIBRARY},
	{"Veloin the Quiet", 25000, 120, 110, 7, 19, RACE_ZOMBIE, STORE_LIBRARY},
	{"Vanthylas the Learned", 30000, 140, 105, 6, 12, RACE_MIND_FLAYER, STORE_LIBRARY},
	{"Kleibons", 15000, 175, 108, 4, 12, RACE_KLACKON, STORE_INN},
	{"Prendegast", 20000, 120, 105, 6, 16, RACE_HOBBIT, STORE_INN},
	{"Straasha", 25000, 120, 110, 7, 19, RACE_DRACONIAN, STORE_INN},
	{"Allia the Servile", 30000, 140, 105, 6, 12, RACE_HUMAN, STORE_INN},
	{"Od the Penniless", 2000, 100, 100, 4, 12, RACE_ELF, STORE_PAWN},
	{"Xax", 4000, 100, 100, 6, 16, RACE_GOLEM, STORE_PAWN},
	{"Jake Small", 5000, 100, 100, 7, 19, RACE_HALF_GIANT, STORE_PAWN},
	{"Helga the Lost", 10000, 100, 100, 6, 12, RACE_HUMAN, STORE_PAWN},
	{"Eloise Long-Dead", 250, 170, 108, 5, 15, RACE_SPECTRE, STORE_GENERAL},
	{"Fundi the Slow", 500, 175, 108, 4, 12, RACE_ZOMBIE, STORE_GENERAL},
	{"Granthus", 750, 170, 107, 5, 15, RACE_SKELETON, STORE_GENERAL},
	{"Lorax the Suave", 1000, 165, 107, 6, 18, RACE_VAMPIRE, STORE_GENERAL},
	{"Selaxis", 10000, 210, 115, 5, 7, RACE_ZOMBIE, STORE_ARMOURY},
	{"Deathchill", 15000, 190, 111, 4, 9, RACE_SPECTRE, STORE_ARMOURY},
	{"Drios the Faint", 25000, 200, 112, 4, 10, RACE_SPECTRE, STORE_ARMOURY},
	{"Bathric the Cold", 30000, 200, 112, 4, 5, RACE_VAMPIRE, STORE_ARMOURY},
	{"Sarlyas the Rotten", 10000, 210, 115, 6, 6, RACE_ZOMBIE, STORE_WEAPON},
	{"Tuethic Bare-Bones", 15000, 185, 110, 5, 9, RACE_SKELETON, STORE_WEAPON},
	{"Bilious", 25000, 190, 115, 5, 7, RACE_BROO, STORE_WEAPON},
	{"Fasgul", 30000, 195, 112, 4, 8, RACE_ZOMBIE, STORE_WEAPON},
	{"Vhassa the Dead", 20000, 250, 150, 10, 5, RACE_ZOMBIE, STORE_BLACK},
	{"Kyn the Treacherous", 20000, 250, 150, 10, 5, RACE_VAMPIRE, STORE_BLACK},
	{"Bubonicus", 30000, 250, 150, 10, 5, RACE_BROO, STORE_BLACK},
	{"Corpselight", 30000, 250, 150, 10, 5, RACE_SPECTRE, STORE_BLACK},
	{"Parrish the Bloodthirsty", 20000, 250, 150, 10, 5, RACE_VAMPIRE, STORE_BLACK},
	{"Vile", 20000, 250, 150, 10, 5, RACE_SKELETON, STORE_BLACK},
	{"Prentice the Trusted", 30000, 250, 150, 10, 5, RACE_SKELETON, STORE_BLACK},
	{"Griella Humanslayer", 30000, 250, 150, 10, 5, RACE_IMP, STORE_BLACK},
	{"Angelface", 20000, 250, 150, 10, 5, RACE_VAMPIRE, STORE_BLACK},
	{"Flotsam the Bloated", 20000, 250, 150, 10, 5, RACE_ZOMBIE, STORE_BLACK},
	{"Nieval", 30000, 250, 150, 10, 5, RACE_VAMPIRE, STORE_BLACK},
	{"Anastasia the Luminous", 30000, 250, 150, 10, 5, RACE_SPECTRE, STORE_BLACK},
	{"Ossein the Literate", 15000, 175, 108, 4, 12, RACE_SKELETON, STORE_LIBRARY},
	{"Olvar Bookworm", 20000, 120, 105, 6, 16, RACE_VAMPIRE, STORE_LIBRARY},
	{"Shallowgrave", 25000, 120, 110, 7, 19, RACE_ZOMBIE, STORE_LIBRARY},
	{"D'ndrasn", 30000, 140, 105, 6, 12, RACE_MIND_FLAYER, STORE_LIBRARY},
	{"Fubble the Boring", 15000, 175, 108, 4, 12, RACE_ZOMBIE, STORE_INN},
	{"Bare-Bones Salluah", 20000, 120, 105, 6, 16, RACE_SKELETON, STORE_INN},
	{"Winsalt Once-Living", 25000, 120, 110, 7, 19, RACE_SPECTRE, STORE_INN},
	{"Agnes D'Arcy", 30000, 140, 105, 6, 12, RACE_VAMPIRE, STORE_INN},
	{"Lumin the Blue", 15000, 175, 108, 4, 12, RACE_SPECTRE, STORE_INN},
	{"Short Al", 20000, 120, 105, 6, 16, RACE_ZOMBIE, STORE_INN},
	{"Silent Faldus", 25000, 120, 110, 7, 19, RACE_ZOMBIE, STORE_INN},
	{"Quirmby the Strange", 30000, 140, 105, 6, 12, RACE_VAMPIRE, STORE_INN},
	{"Gloom the Phlegmatic", 2000, 100, 100, 4, 12, RACE_ZOMBIE, STORE_PAWN},
	{"Quick-Arm Vollaire", 4000, 100, 100, 6, 16, RACE_VAMPIRE, STORE_PAWN},
	{"Asenath", 5000, 100, 100, 7, 19, RACE_ZOMBIE, STORE_PAWN},
	{"Lord Filbert", 10000, 100, 100, 6, 12, RACE_VAMPIRE, STORE_PAWN},
	{"Butch", 250, 170, 108, 5, 15, RACE_HALF_ORC, STORE_GENERAL},
	{"Elbereth the Beautiful", 500, 175, 108, 4, 12, RACE_HIGH_ELF, STORE_GENERAL},
	{"Sarleth the Sneaky", 750, 170, 107, 5, 15, RACE_GNOME, STORE_GENERAL},
	{"Narlock", 1000, 165, 107, 6, 18, RACE_DWARF, STORE_GENERAL},
	{"Vengella the Cruel", 10000, 210, 115, 5, 7, RACE_HALF_TROLL, STORE_ARMOURY},
	{"Wyrana the Mighty", 15000, 190, 111, 4, 9, RACE_HUMAN, STORE_ARMOURY},
	{"Yojo II", 25000, 200, 112, 4, 10, RACE_DWARF, STORE_ARMOURY},
	{"Ranalar the Sweet", 30000, 200, 112, 4, 5, RACE_GREAT, STORE_ARMOURY},
	{"Uurda the Infectious", 10000, 210, 115, 5, 7, RACE_BROO, STORE_ARMOURY},
	{"Sparrow II", 15000, 190, 111, 4, 9, RACE_HALF_ELF, STORE_ARMOURY},
	{"Westmark the Fallen", 25000, 200, 112, 4, 10, RACE_GREAT, STORE_ARMOURY},
	{"Grob the Stupid", 30000, 200, 112, 4, 5, RACE_HALF_TROLL, STORE_ARMOURY},
	{"Ellefris the Paladin", 10000, 210, 115, 6, 6, RACE_BARBARIAN, STORE_WEAPON},
	{"K'trrik'k", 15000, 185, 110, 5, 9, RACE_KLACKON, STORE_WEAPON},
	{"Drocus Spiderfriend", 25000, 190, 115, 5, 7, RACE_DARK_ELF, STORE_WEAPON},
	{"Fungus Giant-Slayer", 30000, 195, 112, 4, 8, RACE_DWARF, STORE_WEAPON},
	{"Delantha", 10000, 210, 115, 6, 6, RACE_ELF, STORE_WEAPON},
	{"Solvistani the Ranger", 15000, 185, 110, 5, 9, RACE_HALF_ELF, STORE_WEAPON},
	{"Xoril the Slow", 25000, 190, 115, 5, 7, RACE_GOLEM, STORE_WEAPON},
	{"Aeon Flux", 30000, 195, 112, 4, 8, RACE_HALF_ELF, STORE_WEAPON},
	{"Xarillus", 10000, 190, 111, 5, 8, RACE_HUMAN, STORE_ALCHEMIST},
	{"Egbert the Old", 10000, 190, 110, 6, 8, RACE_DWARF, STORE_ALCHEMIST},
	{"Valindra the Proud", 15000, 200, 116, 6, 9, RACE_HIGH_ELF, STORE_ALCHEMIST},
	{"Taen the Alchemist", 15000, 220, 111, 4, 9, RACE_HUMAN, STORE_ALCHEMIST},
	{"Keldorn the Grand", 15000, 200, 110, 7, 8, RACE_DWARF, STORE_MAGIC},
	{"Philanthropus", 20000, 215, 113, 6, 10, RACE_HOBBIT, STORE_MAGIC},
	{"Agnar the Enchantress", 30000, 200, 110, 7, 10, RACE_HUMAN, STORE_MAGIC},
	{"Buliance the Necromancer", 30000, 175, 110, 5, 11, RACE_BROO, STORE_MAGIC},
	{"Charity the Necromancer", 20000, 250, 150, 10, 5, RACE_DARK_ELF, STORE_BLACK},
	{"Pugnacious the Pugilist", 20000, 250, 150, 10, 5, RACE_HALF_ORC, STORE_BLACK},
	{"Footsore the Lucky", 30000, 250, 150, 10, 5, RACE_BROO, STORE_BLACK},
	{"Sidria Lighfingered", 30000, 250, 150, 10, 5, RACE_HUMAN, STORE_BLACK},
	{"Porcina the Obese", 15000, 175, 108, 4, 12, RACE_HALF_ORC, STORE_LIBRARY},
	{"Glaruna Brandybreath", 20000, 120, 105, 6, 16, RACE_DWARF, STORE_LIBRARY},
	{"Furface Yeek", 25000, 120, 110, 7, 19, RACE_YEEK, STORE_LIBRARY},
	{"Bald Oggin", 30000, 140, 105, 6, 12, RACE_GNOME, STORE_LIBRARY},
	{"Aldous the Sleepy", 15000, 175, 108, 4, 12, RACE_HUMAN, STORE_INN},
	{"Grundy the Tall", 20000, 120, 105, 6, 16, RACE_HOBBIT, STORE_INN},
	{"Gobbleguts Thunderbreath", 25000, 120, 110, 7, 19, RACE_HALF_TROLL, STORE_INN},
	{"Silverscale", 30000, 140, 105, 6, 12, RACE_DRACONIAN, STORE_INN},
	{"Haneka the Small", 250, 170, 108, 5, 15, RACE_GNOME, STORE_GENERAL},
	{"Loirin the Mad", 500, 175, 108, 4, 12, RACE_HALF_GIANT, STORE_GENERAL},
	{"Wuto Poisonbreath", 750, 170, 107, 5, 15, RACE_DRACONIAN, STORE_GENERAL},
	{"Araaka the Rotund", 1000, 165, 107, 6, 18, RACE_DRACONIAN, STORE_GENERAL},
	{"Horbag the Unclean", 10000, 210, 115, 5, 7, RACE_HALF_ORC, STORE_ARMOURY},
	{"Elelen the Telepath", 15000, 190, 111, 4, 9, RACE_DARK_ELF, STORE_ARMOURY},
	{"Isedrelias", 25000, 200, 112, 4, 10, RACE_SPRITE, STORE_ARMOURY},
	{"Vegnar One-eye", 30000, 200, 112, 4, 5, RACE_CYCLOPS, STORE_ARMOURY},
	{"Nadoc the Strong", 10000, 210, 115, 6, 6, RACE_HOBBIT, STORE_WEAPON},
	{"Eramog the Weak", 15000, 185, 110, 5, 9, RACE_KOBOLD, STORE_WEAPON},
	{"Eowilith the Fair", 25000, 190, 115, 5, 7, RACE_VAMPIRE, STORE_WEAPON},
	{"Huimog Balrog-Slayer", 30000, 195, 112, 4, 8, RACE_HALF_ORC, STORE_WEAPON},
	{"Ibenidd the Chaste", 10000, 175, 109, 6, 15, RACE_HUMAN, STORE_TEMPLE},
	{"Eridish", 15000, 185, 110, 5, 23, RACE_HALF_TROLL, STORE_TEMPLE},
	{"Vrudush the Shaman", 25000, 180, 107, 6, 20, RACE_HALF_OGRE, STORE_TEMPLE},
	{"Haob the Berserker", 30000, 185, 109, 5, 15, RACE_BARBARIAN, STORE_TEMPLE},
	{"Cayd the Sweet", 10000, 190, 111, 5, 8, RACE_VAMPIRE, STORE_ALCHEMIST},
	{"Fulir the Dark", 10000, 190, 110, 6, 8, RACE_NIBELUNG, STORE_ALCHEMIST},
	{"Domli the Humble", 15000, 200, 116, 6, 9, RACE_DWARF, STORE_ALCHEMIST},
	{"Yaarjukka Demonspawn", 15000, 220, 111, 4, 9, RACE_IMP, STORE_ALCHEMIST},
	{"Vuirak the Wizard", 15000, 200, 110, 7, 8, RACE_BROO, STORE_MAGIC},
	{"Madish the Smart", 20000, 215, 113, 6, 10, RACE_BROO, STORE_MAGIC},
	{"Falebrimbor", 30000, 200, 110, 7, 10, RACE_HIGH_ELF, STORE_MAGIC},
	{"Felil-Gand the Subtle", 30000, 175, 110, 5, 11, RACE_DARK_ELF, STORE_MAGIC},
	{"Riatho the Juggler", 20000, 250, 150, 10, 5, RACE_HOBBIT, STORE_BLACK},
	{"Janaaka the Shifty", 20000, 250, 150, 10, 5, RACE_GNOME, STORE_BLACK},
	{"Cina the Rogue", 30000, 250, 150, 10, 5, RACE_GNOME, STORE_BLACK},
	{"Arunikki Greatclaw", 30000, 250, 150, 10, 5, RACE_DRACONIAN, STORE_BLACK},
	{"Asuunu the Learned", 15000, 175, 108, 4, 12, RACE_MIND_FLAYER, STORE_LIBRARY},
	{"Prirand the Dead", 20000, 120, 105, 6, 16, RACE_ZOMBIE, STORE_LIBRARY},
	{"Ronar the Iron", 25000, 120, 110, 7, 19, RACE_GOLEM, STORE_LIBRARY},
	{"Galil-Gamir", 30000, 140, 105, 6, 12, RACE_ELF, STORE_LIBRARY},
	{"Togdush the Quiet", 15000, 175, 108, 4, 12, RACE_KOBOLD, STORE_LIBRARY},
	{"Vurjaana the Studious", 20000, 120, 105, 6, 16, RACE_GNOME, STORE_LIBRARY},
	{"Yietram the Luminous", 25000, 120, 110, 7, 19, RACE_SPECTRE, STORE_LIBRARY},
	{"Thradrierith", 30000, 140, 105, 6, 12, RACE_DARK_ELF, STORE_LIBRARY},
	{"Etheraa the Furious", 15000, 175, 108, 4, 12, RACE_BARBARIAN, STORE_INN},
	{"Paetlan the Alcoholic", 20000, 120, 105, 6, 16, RACE_HUMAN, STORE_INN},
	{"Drang", 25000, 120, 110, 7, 19, RACE_HALF_OGRE, STORE_INN},
	{"Barbag the Sly", 30000, 140, 105, 6, 12, RACE_KOBOLD, STORE_INN},
	{"Poogor the Dumb", 250, 170, 108, 5, 15, RACE_BROO, STORE_GENERAL},
	{"Felorfiliand", 500, 175, 108, 4, 12, RACE_ELF, STORE_GENERAL},
	{"Maroka the Aged", 750, 170, 107, 5, 15, RACE_GNOME, STORE_GENERAL},
	{"Sasin the Bold", 1000, 165, 107, 6, 18, RACE_HALF_GIANT, STORE_GENERAL},
	{"Rodish the Chaotic", 10000, 210, 115, 5, 7, RACE_BROO, STORE_ARMOURY},
	{"Hesin Swordmaster", 15000, 190, 111, 4, 9, RACE_NIBELUNG, STORE_ARMOURY},
	{"Elvererith the Cheat", 25000, 200, 112, 4, 10, RACE_DARK_ELF, STORE_ARMOURY},
	{"Zzathath the Imp", 30000, 200, 112, 4, 5, RACE_IMP, STORE_ARMOURY},
	{"Peadus the Cruel", 10000, 210, 115, 6, 6, RACE_HUMAN, STORE_WEAPON},
	{"Vamog Slayer", 15000, 185, 110, 5, 9, RACE_HALF_OGRE, STORE_WEAPON},
	{"Hooshnak the Vicious", 25000, 190, 115, 5, 7, RACE_BROO, STORE_WEAPON},
	{"Balenn War-Dancer", 30000, 195, 112, 4, 8, RACE_BARBARIAN, STORE_WEAPON},
	{"Proogdish the Youthfull", 10000, 175, 109, 6, 15, RACE_HALF_OGRE, STORE_TEMPLE},
	{"Lumwise the Mad", 15000, 185, 110, 5, 23, RACE_YEEK, STORE_TEMPLE},
	{"Muirt the Virtuous", 25000, 180, 107, 6, 20, RACE_KOBOLD, STORE_TEMPLE},
	{"Dardobard the Weak", 30000, 185, 109, 5, 15, RACE_SPECTRE, STORE_TEMPLE},
	{"Gelaraldor the Herbmaster", 10000, 190, 111, 5, 8, RACE_HIGH_ELF, STORE_ALCHEMIST},
	{"Olelaldan the Wise", 10000, 190, 110, 6, 8, RACE_BARBARIAN, STORE_ALCHEMIST},
	{"Fthoglo the Demonicist", 15000, 200, 116, 6, 9, RACE_IMP, STORE_ALCHEMIST},
	{"Dridash the Alchemist", 15000, 220, 111, 4, 9, RACE_HALF_ORC, STORE_ALCHEMIST},
	{"Nelir the Strong", 10000, 190, 111, 5, 8, RACE_CYCLOPS, STORE_ALCHEMIST},
	{"Lignus the Pungent", 10000, 190, 110, 6, 8, RACE_HALF_ORC, STORE_ALCHEMIST},
	{"Tilba", 15000, 200, 116, 6, 9, RACE_HOBBIT, STORE_ALCHEMIST},
	{"Myrildric the Wealthy", 15000, 220, 111, 4, 9, RACE_HUMAN, STORE_ALCHEMIST},
	{"Thalegord the Shaman", 15000, 200, 110, 7, 8, RACE_BARBARIAN, STORE_MAGIC},
	{"Cthoaloth the Mystic", 20000, 215, 113, 6, 10, RACE_MIND_FLAYER, STORE_MAGIC},
	{"Ibeli the Illusionist", 30000, 200, 110, 7, 10, RACE_SKELETON, STORE_MAGIC},
	{"Heto the Necromancer", 30000, 175, 110, 5, 11, RACE_YEEK, STORE_MAGIC},
	{"Chaeand the Poor", 20000, 250, 150, 10, 5, RACE_HUMAN, STORE_BLACK},
	{"Afardorf the Brigand", 20000, 250, 150, 10, 5, RACE_BARBARIAN, STORE_BLACK},
	{"Lathaxl the Greedy", 30000, 250, 150, 10, 5, RACE_MIND_FLAYER, STORE_BLACK},
	{"Falarewyn", 30000, 250, 150, 10, 5, RACE_SPRITE, STORE_BLACK},
	{"Vosur the Wrinkled", 20000, 250, 150, 10, 5, RACE_NIBELUNG, STORE_BLACK},
	{"Araord the Handsome", 20000, 250, 150, 10, 5, RACE_GREAT, STORE_BLACK},
	{"Theradfrid the Loser", 30000, 250, 150, 10, 5, RACE_HUMAN, STORE_BLACK},
	{"One-Legged Eroolo", 30000, 250, 150, 10, 5, RACE_HALF_OGRE, STORE_BLACK},
	{"Rorbag Book-Eater", 15000, 175, 108, 4, 12, RACE_KOBOLD, STORE_LIBRARY},
	{"Kiriarikirk", 20000, 120, 105, 6, 16, RACE_KLACKON, STORE_LIBRARY},
	{"Rilin the Quiet", 25000, 120, 110, 7, 19, RACE_DWARF, STORE_LIBRARY},
	{"Isung the Lord", 30000, 140, 105, 6, 12, RACE_HIGH_ELF, STORE_LIBRARY},
	{"Kirakak", 15000, 175, 108, 4, 12, RACE_KLACKON, STORE_INN},
	{"Nafur the Wooden", 20000, 120, 105, 6, 16, RACE_GOLEM, STORE_INN},
	{"Grarak the Hospitable", 25000, 120, 110, 7, 19, RACE_HALF_GIANT, STORE_INN},
	{"Lona the Charismatic", 30000, 140, 105, 6, 12, RACE_GNOME, STORE_INN},
	{"Herranyth the Ruthless", 2000, 100, 100, 4, 12, RACE_HUMAN, STORE_PAWN},
	{"Gagrin Moneylender", 4000, 100, 100, 6, 16, RACE_YEEK, STORE_PAWN},
	{"Thrambor the Grubby", 5000, 100, 100, 7, 19, RACE_HALF_ELF, STORE_PAWN},
	{"Derigrin the Honest", 10000, 100, 100, 6, 12, RACE_HOBBIT, STORE_PAWN},
	{"Abiemar the Peasant", 250, 170, 108, 5, 15, RACE_HUMAN, STORE_GENERAL},
	{"Hurk the Poor", 500, 175, 108, 4, 12, RACE_HALF_ORC, STORE_GENERAL},
	{"Soalin the Wretched", 750, 170, 107, 5, 15, RACE_ZOMBIE, STORE_GENERAL},
	{"Merulla the Humble", 1000, 165, 107, 6, 18, RACE_ELF, STORE_GENERAL},
	{"Crediric the Brewer", 15000, 175, 108, 4, 12, RACE_HUMAN, STORE_INN},
	{"Nydudus the Slow", 20000, 120, 105, 6, 16, RACE_ZOMBIE, STORE_INN},
	{"Baurk the Busy", 25000, 120, 110, 7, 19, RACE_YEEK, STORE_INN},
	{"Seviras the Mindcrafter", 30000, 140, 105, 6, 12, RACE_HUMAN, STORE_INN},
	{"Munk the Barterer", 2000, 100, 100, 4, 12, RACE_HALF_OGRE, STORE_PAWN},
	{"Gadrialdur the Fair", 4000, 100, 100, 6, 16, RACE_HALF_ELF, STORE_PAWN},
	{"Ninar the Stooped", 5000, 100, 100, 7, 19, RACE_DWARF, STORE_PAWN},
	{"Adirath the Unmagical", 10000, 100, 100, 6, 12, RACE_BARBARIAN, STORE_PAWN},
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
 *      infra,
 *      template-choices
 *      initial template
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
        0,
        0x004E5,1,
    },
    {
        "Broo",
        { 2, -2, -1, -1, 2, -4 },
        5, 8, 9, 8, 8, 10, 14, 12,
        11, 140,
        14, 6,
        65,  6, 150, 20,
        61,  6, 120, 15,
        0,
        0x06E5,129,
    },
    {
        "Cyclops",
        { 4, -3, -3, -3, 4, -6 },
        6, 5, 5, 6, 6, 10, 17, 14,
        13, 130,
        50, 24,
        92, 10, 255, 60,
        80,  8, 235, 60,
        1,
        0x0007,77,
    },
    {
        "Dark-Elf",
        { -1, 3, 2, 2, -2, 1 },
        15, 25, 30, 16, 26, 24, 8, 13,
        9, 150,
		75, 75,
		60,  4, 100,  6,
		54,  4, 80,  6,
        5,
        0x0FFF,69,
    },
    {
        "Draconian",
        { 2, 1, 1, 1, 2, -3 },
        8, 15, 13, 10, 12, 20, 12, 12,
        11, 250,
        75, 33,
        76,  1, 160,  5,
        72,  1, 130,  5,
        2,
        0x0F7F,89,
    },
	{
		"Dwarf",
        {  2, -2,  2, -2,  2, -3 },
        12,  19,  20,  8,  24,  20, 15,  10,
        11,  125,
		35, 15,
		48,  3, 150, 10,
		46,  3, 120, 10,
		5,
        0x0065,16,
	},
	{
		"Elf",
        { -1,  2,  2,  1, -2,  2 },
        15,  16,  16,  14, 26,  24, 8, 15,
		8,  120,
		75, 75,
		60,  4, 100,  6,
		54,  4, 80,  6,
		3,
        0x0FFF,7,

	},
	{
		"Gnome",
		{ -1,  2,  0,  2,  1, -2 },
		20, 22, 22,  16, 22,  26, 7, 14,
        8,  135,
		50, 40,
		42,  3, 90,  6,
		39,  3, 75,  3,
		4,
        0x0F3F,13,
	},
    {
        "Golem",
        { 4, -5, -5, 0, 4, -4 },
        5, 5, 20, 8, 8, 16, 17, 10,
        12, 200,
        1, 100,
        66,  1, 200,  6,
        62,  1, 180,  6,
        4,
        0x0005,98,
    },
	{
        "Great-One",
		{  1,  2,  2,  2,  3,  2 },
		14, 15,  15,  14, 16, 26, 15, 13,
        10,  225,
        50, 50,
		82, 5, 190, 20,
		78,  6, 180, 15,
		0,
        0x0FFF,67,
	},
	{
		"Half-Elf",
        { -1,  1,  1,  1, -1,  1 },
		12,  13,  13,  12, 22,  22, 10,  12,
		9,  110,
		24, 16,
		66,  6, 130, 15,
		62,  6, 100, 10,
		2,
        0x0FFF,4,
	},
    {
        "Half-Giant",
        { 4, -2, -2, -2, 3, -3 },
        4, 2, 4, 6, 8, 10, 18, 12,
        13, 150,
        40, 10,
        100,10, 255, 65,
        80, 10, 240, 64,
        3,
        0x0045,75,
    },
    {
        "Half-Ogre",
        { 3, -1, -1, -1, 3, -3 },
        7, 5, 5, 6, 8, 10, 17, 10,
        12,  130,
        40, 10,
        92, 10, 255, 60,
        80,  8, 235, 60,
		3,
        0x04C5,74,
    },
	{
		"Half-Orc",
        {  2, -1,  0,  0,  1, -4 },
		7, 7, 7,  8,  10, 14, 14, 8,
        10,  110,
		11,  4,
		66,  1, 150,  5,
		62,  1, 120,  5,
		3,
        0x0FFF,19,
	},
    {
        "Half-Titan",
        { 5, 1, 1, -2, 3, 1 },
        5, 15, 12, 6, 12, 16, 18, 10,
        14, 255,
        100,30,
        111, 11, 255, 86,
        99, 11, 250, 86,
        0,
        0x0FFF,76,
    },
	{
		"Half-Troll",
		{ 4, -4, -2, -4,  3, -6 },
		5, 2, 2, 6,  8, 10, 17, 7,
        12,  137,
		20, 10,
		96, 10, 250, 50,
		84,  8, 225, 40,
		3,
        0x00C5,22,
	},
	{
		"High-Elf",
        {  1,  3,  2,  3,  1,  5 },
        14,  30, 30,  18,  16, 28, 13, 18,
		10,  200,
		100, 30,
		90, 10, 190, 20,
		82, 10, 180, 15,
		4,
        0x0FFF,7,
    },
	{
		"Hobbit",
		{ -2,  2,  1,  3,  2,  1 },
        25, 28, 28, 20, 34,  30, 7, 17,
		7,  110,
		21, 12,
		36,  3, 60,  3,
		33,  3, 50,  3,
		4,
        0x06E7,10,
	},
	{
		"Human",
		{  0,  0,  0,  0,  0,  0 },
		10,  10,  10,  10,  10,  20,  10,  10,
		10,  100,
		14,  6,
		72,  6, 180, 25,
		66,  4, 150, 20,
		0,
        0x0FFF,1,
	},
    {
        "Imp",
        { -1, -1, -1, 1, 2, -3 },
        7, 12, 9, 12, 8, 20, 12, 8,
        10, 110,
        13,  4,
        68,  1, 150,  5,
        64,  1, 120,  5,
        3,
        0x0627,94,
    },
    {
        "Klackon",
        { 2, -1, -1, 1, 2, -2 },
        20, 15, 15, 10, 8, 20, 12, 12,
        12, 135,
        20, 3,
        60,  3, 80,  4,
        54,  3, 70,  4,
        2,
        0x047D,84,
    },
    {
        "Kobold",
        { 1, -1, 0, 1, 0, -4 },
        8, 7, 8, 8, 12, 16, 13, 7,
        9, 125,
        11,  3,
        60,  1, 130,  5,
        55,  1, 100,  5,
        3,
        0x0465,82,
    },
    {
        "Mind-Flayer",
        { -3, 4, 4, 0, -2, -5 },
        20, 35, 25, 14, 20, 24, 7, 8,
        9, 140,
        100, 25,
        68,  6, 142, 15,
        63,  6, 112, 10,
        4,
        0x0F21,92,
    },
    {
        "Nibelung",
        { 1, -1, 2, 0, 2, -4 },
        13, 15, 20, 12, 20, 20, 13, 10,
        11, 135,
        40, 12,
        43,  3, 92,  6,
        40,  3, 78,  3,
        5,
        0x0F25,87,
    },
    {
        "Skeleton",
        { 0, -2, -2, 0, 1, -4 },
        5, 5, 15, 8, 8, 16, 13, 10,
        10, 145,
        100, 35,
        72,  6, 50, 5,
        66,  4, 50, 5,
        2,
        0x0F3F,102,
    },
    {
         "Spectre",
         { -5, 4, 4, 2, -3, -6 },
        20, 35, 30, 20, 20, 28, 5, 8,
        7, 180,
        100, 30,
        72, 6, 100, 25,
        66, 4, 100, 20,
         5,
         0x0F21,110,
    },
    {
        "Sprite",
        {  -4, 3, 3, 3, -2, 2 },
        20, 20, 20, 18, 30, 20, 6, 10,
        7, 175,
        50, 25,
        32,  2, 75,  2,
        29,  2, 65,  2,
        4,
        0x0EE3,124,
    },
    {
         "Vampire",
         { 3, 3, -1, -1, 1, 2 },
         14, 20, 20, 18, 12, 16, 12, 10,
         11, 200,
         100, 30,
           72,  6, 180, 25,
           66,  4, 150, 20,
         5,
         0x0FFF,113,
    },
    {
        "Yeek",
        { -2, 1, 1, 1, -2, -7 },
        12, 14, 20, 40, 20, 30, 8, 8,
        7, 100,
        14, 3,
        50,  3, 90,  6,
        50,  3, 75,  3,
        2,
        0x0FE7,78,
    },
    {
        "Zombie",
        { 2, -6, -6, 1, 4, -5 },
        5, 5, 18, 0, 8, 10, 15, 10,
        13, 135,
        100, 30,
        72, 6, 100, 25,
        66, 4, 100, 20,
        2,
        0x0005,107,
    }
};


/*
 * Player Templates
 *
 *      Title, choices
 *      {STR,INT,WIS,DEX,CON,CHR},
 *      {CLOSE, SLASH, STAB, CRUSH, MISSILE,
 *      TOUGH, DEVICE, DISARM, PERCEPTION, SAVE, SEARCH, STEALTH,
 *      MA, MINDCRAFT, CHI, SHAMAN, HEDGE, MANA, PSEUDO-ID}
 *
 *      Hermetic skills are handled separately.
 */
player_template template_info[MAX_TEMPLATE] =
{
	{
		"Adventurer", 0,
		{ 3, 1, -1, 2, 2, -1},
		{20, 20, 20, 20, 20,
		1, 5, 5, 10, 20, 5, 5,
		0, 0, 0, 0, 5, 0, 20}
	},
	{
		"Swashbuckler", 0,
		{1, 0, -2, 3, 2, 1},
		{15, 35, 15, 15, 15,
		1, 5, 5, 15, 15, 5, 5,
		0, 0, 0, 0, 0, 0, 30}
	},
	{
		"Gladiator", 0,
		{5, -1, -1, 1, 3, -1},
		{30, 30, 30, 30, 10,
		2, 5, 5, 10, 10, 5, 5,
		1, 0, 0, 0, 0, 0, 40}
	},
	{
		"Warrior-Monk", 0,
		{ 2, 0, -2, 5, 1, -1},
		{35, 10, 10, 10, 10,
		1, 5, 5, 15, 10, 5, 15,
		4, 0, 0, 0, 0, 0, 30}
	},
	{
		"Zen-Monk", 0,
		{ 2, -1, 2, 2, 0, -1},
		{25, 10, 10, 10, 10,
		0, 5, 5, 15, 10, 10, 10,
		2, 1, 1, 0, 0, 0, 40}
	},
	{
		"Assassin", 0,
		{ 1, 0, 0, 3, 2, -2},
		{30, 20, 20, 20, 10,
		0, 5, 20, 20, 10, 20, 30,
		0, 0, 0, 0, 0, 0, 30}
    },
    {
		"Ranger", 0,
		{ 2, -1, -1, 3, 3, 2},
		{15, 15, 15, 15, 35,
		1, 5, 10, 30, 10, 20, 20,
		0, 0, 0, 2, 0, 0, 10}
	},
	{
		"Shaman", 0,
		{ -1, 1, 1, -2, 2, 5},
		{15, 15, 15, 15, 15,
		0, 10, 5, 15, 20, 10, 5,
		0, 0, 0, 5, 0, 0, 20}
    },
	{
		"Mindcrafter", 0,
		{ -2, 2, 5, -1, 0, 1},
		{15, 15, 15, 15, 15,
		0, 10, 5, 15, 15, 10, 5,
		0, 4, 2, 0, 0, 0, 15}
	},
	{
		"Wizard", 3,
		{-2, 5, 2, 1, -1, 1},
		{10, 10, 10, 10, 10,
		0, 25, 5, 15, 20, 10, 5,
		0, 0, 0, 0, 0, 4, 5}
	},
	{
		"Warlock", 1,
		{1, 3, -1, 0, 2, -1},
		{15, 15, 15, 20, 10,
		1, 10, 5, 10, 15, 10, 5,
		0, 0, 0, 0, 0, 2, 10}
	},
	{
		"Powerweaver", 2,
		{-3, 3, 3, -2, -2, 3},
		{5, 5, 5, 5, 5,
		0, 15, 5, 10, 15, 10, 5,
		0, 3, 2, 3, 0, 2, 10}
	},
	{
		"Tourist", 0,
		{0, 0, 0, 0, 2, -1},
		{10, 10, 10, 10, 10,
		0, 5, 5, 5, 5, 5, 5,
		0, 0, 0, 0, 0, 0, 20}
	},
};




/*
 * Hack -- the spell information table.
 *
 *   Spell Encumbrance,
 *
 *   Array of { Lev, Mana, Fail, Exp/Lev, school, type }
 */
player_magic magic_info =
{
	300, /* Max weight for spellcasting */
	{
		/* Sorcery */
		{
			{ 1, 1, 23, 4, SCH_SORCERY, SP_ANIMAE },
			{ 1, 2, 24, 4, SCH_SORCERY, SP_CORPORIS },
			{ 3, 3, 25, 1, SCH_SORCERY, SP_NATURAE },
			{ 3, 3, 30, 1, SCH_SORCERY, SP_VIS },
			{ 4, 4, 30, 1, SCH_SORCERY, SP_ANIMAE },
			{ 5, 5, 35, 5, SCH_SORCERY, SP_CORPORIS },
			{ 6, 5, 30, 4, SCH_SORCERY, SP_ANIMAE },
			{ 7, 7, 75, 9, SCH_SORCERY, SP_VIS },

			{ 9, 7, 75, 8, SCH_SORCERY, SP_NATURAE },
			{ 10, 7, 75, 8, SCH_SORCERY, SP_NATURAE },
			{ 11, 7, 75, 7, SCH_SORCERY, SP_CORPORIS },
			{ 13, 7, 50, 6, SCH_SORCERY, SP_ANIMAE },
			{ 18, 12, 60, 8, SCH_SORCERY, SP_CORPORIS },
			{ 22, 12, 60, 8, SCH_SORCERY, SP_CORPORIS },
			{ 28, 20, 70, 15, SCH_SORCERY, SP_ANIMAE },
			{ 33, 30, 75, 20, SCH_SORCERY, SP_NATURAE },

			{ 3, 3, 25, 15, SCH_SORCERY, SP_NATURAE },
			{ 10, 10, 70, 40, SCH_SORCERY, SP_VIS },
			{ 10, 10, 80, 40, SCH_SORCERY, SP_ANIMAE },
			{ 12, 12, 80, 40, SCH_SORCERY, SP_NATURAE },
			{ 14, 10, 60, 25, SCH_SORCERY, SP_ANIMAE },
			{ 20, 18, 85, 50, SCH_SORCERY, SP_ANIMAE },
			{ 20, 18, 60, 25, SCH_SORCERY, SP_CORPORIS },
			{ 25, 25, 75, 19, SCH_SORCERY, SP_CORPORIS },

			{ 10, 10, 40, 20, SCH_SORCERY, SP_VIS },
			{ 25, 25, 75, 70, SCH_SORCERY, SP_NATURAE },
			{ 25, 30, 95, 160, SCH_SORCERY, SP_VIS },
			{ 30, 40, 80, 120, SCH_SORCERY, SP_NATURAE },
			{ 40, 80, 95, 200, SCH_SORCERY, SP_VIS },
			{ 40, 100, 95, 200, SCH_SORCERY, SP_VIS },
			{ 42, 50, 90, 175, SCH_SORCERY, SP_NATURAE },
			{ 45, 70, 75, 250, SCH_SORCERY, SP_ANIMAE },
		},
		/* Thaumaturgy */
		{
			{ 1, 1, 20, 4, SCH_THAUMATURGY, SP_NATURAE },
			{ 1, 2, 22, 4, SCH_THAUMATURGY, SP_NATURAE },
			{ 2, 2, 25, 4, SCH_THAUMATURGY, SP_VIS },
			{ 5, 5, 30, 1, SCH_THAUMATURGY, SP_ANIMAE },
			{ 9, 6, 50, 1, SCH_THAUMATURGY, SP_VIS },
			{ 13, 9, 45, 6, SCH_THAUMATURGY, SP_VIS },
			{ 14, 9, 45, 6, SCH_THAUMATURGY, SP_VIS },
			{ 15, 9, 35, 5, SCH_THAUMATURGY, SP_CORPORIS },

			{ 17, 10, 25, 5, SCH_THAUMATURGY, SP_VIS },
			{ 19, 12, 45, 9, SCH_THAUMATURGY, SP_VIS },
			{ 21, 13, 45, 10, SCH_THAUMATURGY, SP_VIS },
			{ 23, 15, 50, 11, SCH_THAUMATURGY, SP_VIS },
			{ 25, 16, 50, 12, SCH_THAUMATURGY, SP_VIS },
			{ 25, 18, 60, 8, SCH_THAUMATURGY, SP_CORPORIS },
			{ 30, 20, 80, 15, SCH_THAUMATURGY, SP_ANIMAE },
			{ 35, 40, 85, 40, SCH_THAUMATURGY, SP_NATURAE },

			{ 11, 7, 45, 9, SCH_THAUMATURGY, SP_CORPORIS },
			{ 15, 15, 80, 35, SCH_THAUMATURGY, SP_VIS },
			{ 16, 14, 80, 35, SCH_THAUMATURGY, SP_NATURAE },
			{25, 25, 85, 100, SCH_THAUMATURGY, SP_NATURAE },
			{ 30, 25, 85, 150, SCH_THAUMATURGY, SP_ANIMAE },
			{ 42, 50, 85, 250, SCH_THAUMATURGY, SP_CORPORIS },
			{ 45, 90, 80, 250, SCH_THAUMATURGY, SP_NATURAE },
			{ 47, 100, 90, 250, SCH_THAUMATURGY, SP_ANIMAE },

			{ 20, 20, 66, 8, SCH_THAUMATURGY, SP_NATURAE },
			{ 35, 32, 85, 35, SCH_THAUMATURGY, SP_NATURAE },
			{ 37, 34, 75, 40, SCH_THAUMATURGY, SP_VIS },
			{ 41, 42, 85, 100, SCH_THAUMATURGY, SP_VIS },
			{ 43, 44, 80, 150, SCH_THAUMATURGY, SP_NATURAE },
			{ 45, 48, 85, 200, SCH_THAUMATURGY, SP_VIS },
			{ 47, 75, 80, 200, SCH_THAUMATURGY, SP_CORPORIS },
			{ 49, 100, 85, 250, SCH_THAUMATURGY, SP_ANIMAE }
		},
		/* Conjuration Magic */
		{
			{ 1, 1, 50, 3, SCH_CONJURATION, SP_CORPORIS },
			{ 3, 3, 50, 4, SCH_CONJURATION, SP_ANIMAE },
			{ 5, 5, 75, 8, SCH_CONJURATION, SP_VIS },
			{ 6, 6, 80, 8, SCH_CONJURATION, SP_ANIMAE },
			{ 7, 7, 40, 4, SCH_CONJURATION, SP_CORPORIS },
			{ 9, 9, 60, 6, SCH_CONJURATION, SP_NATURAE },
			{ 14, 12, 60, 6, SCH_CONJURATION, SP_VIS },
			{ 17, 15, 60, 5, SCH_CONJURATION, SP_CORPORIS },

			{ 20, 20, 80, 8, SCH_CONJURATION, SP_NATURAE },
			{ 24, 22, 60, 8, SCH_CONJURATION, SP_ANIMAE },
			{ 28, 24, 60, 8, SCH_CONJURATION, SP_VIS },
			{ 30, 25, 70, 9, SCH_CONJURATION, SP_ANIMAE },
			{ 33, 28, 80, 12, SCH_CONJURATION, SP_NATURAE },
			{ 35, 30, 70, 10, SCH_CONJURATION, SP_CORPORIS },
			{ 40, 35, 80, 15, SCH_CONJURATION, SP_CORPORIS },
			{ 42, 40, 70, 12, SCH_CONJURATION, SP_ANIMAE },

			{ 15, 15, 80, 20, SCH_CONJURATION, SP_VIS },
			{ 24, 24, 70, 25, SCH_CONJURATION, SP_ANIMAE },
			{ 26, 26, 70, 30, SCH_CONJURATION, SP_ANIMAE },
			{ 30, 30,  70, 35, SCH_CONJURATION, SP_ANIMAE },
			{ 35, 70, 80, 100, SCH_CONJURATION, SP_VIS },
			{ 40, 100, 90, 250, SCH_CONJURATION, SP_CORPORIS },
			{ 42, 50, 50, 75, SCH_CONJURATION, SP_VIS },
			{ 45, 100, 90, 200, SCH_CONJURATION, SP_ANIMAE },

			{ 30, 30, 60, 50, SCH_CONJURATION, SP_ANIMAE },
			{ 35, 50, 90, 100, SCH_CONJURATION, SP_ANIMAE },
			{ 36, 80, 80, 150, SCH_CONJURATION, SP_ANIMAE },
			{ 39, 80, 80, 150, SCH_CONJURATION, SP_ANIMAE },
			{ 42, 100, 80, 200, SCH_CONJURATION, SP_ANIMAE },
			{ 47, 100, 80, 150, SCH_CONJURATION, SP_ANIMAE },
			{ 48, 100, 80, 200, SCH_CONJURATION, SP_ANIMAE },
			{ 49, 100, 80, 220, SCH_CONJURATION, SP_ANIMAE }
		},
		/* Necromancy */
		{
			{ 1, 1, 25, 4, SCH_NECROMANCY,SP_ANIMAE },
			{ 2, 2, 25, 4, SCH_NECROMANCY,SP_VIS },
			{ 2, 2, 25, 4, SCH_NECROMANCY,SP_ANIMAE },
			{ 3, 3, 27, 3, SCH_NECROMANCY,SP_NATURAE },
			{ 5, 5, 30, 4, SCH_NECROMANCY,SP_ANIMAE },
			{ 7, 10, 75, 6, SCH_NECROMANCY,SP_CORPORIS },
			{ 9, 9, 30, 4, SCH_NECROMANCY,SP_CORPORIS },
			{ 10, 10, 30, 4, SCH_NECROMANCY,SP_ANIMAE },

			{ 12, 12, 40, 5, SCH_NECROMANCY,SP_VIS },
			{ 13, 12, 30, 4, SCH_NECROMANCY,SP_VIS },
			{ 18, 15, 50, 10, SCH_NECROMANCY,SP_ANIMAE },
			{ 23, 20, 60, 16, SCH_NECROMANCY,SP_CORPORIS },
			{ 30, 75, 50, 30, SCH_NECROMANCY,SP_NATURAE },
			{ 33, 35, 60, 16, SCH_NECROMANCY,SP_VIS },
			{ 37, 25, 95, 25, SCH_NECROMANCY,SP_CORPORIS },
			{ 45, 50, 95, 150, SCH_NECROMANCY,SP_CORPORIS },

			{ 10, 20, 80, 180, SCH_NECROMANCY,SP_CORPORIS },
			{ 10, 15, 80, 30, SCH_NECROMANCY,SP_ANIMAE },
			{ 11, 11, 30, 15, SCH_NECROMANCY,SP_VIS },
			{ 30, 25, 75, 50, SCH_NECROMANCY,SP_CORPORIS },
			{ 33, 35, 60, 125, SCH_NECROMANCY,SP_VIS },
			{ 33, 90, 70, 90, SCH_NECROMANCY,SP_NATURAE },
			{ 40, 40, 70, 200, SCH_NECROMANCY,SP_VIS },
			{ 40, 75, 80, 100, SCH_NECROMANCY,SP_CORPORIS },

			{ 20, 20, 75, 50, SCH_NECROMANCY,SP_VIS },
			{ 25, 66, 95 , 250, SCH_NECROMANCY,SP_CORPORIS },
			{ 30, 40, 95, 250, SCH_NECROMANCY,SP_NATURAE },
			{ 33, 35, 70, 40, SCH_NECROMANCY,SP_ANIMAE },
			{ 37, 35, 80, 70, SCH_NECROMANCY,SP_VIS },
			{ 42, 120, 95, 250, SCH_NECROMANCY,SP_VIS },
			{ 45, 100, 90, 250, SCH_NECROMANCY,SP_CORPORIS },
			{ 47, 100, 90, 250, SCH_NECROMANCY,SP_CORPORIS }
		},
	}
};

favour_type favour_info[MAX_SPHERE][32] =
{
	/* Life Favours */
	{
		{ 1, 1, 10},
		{ 1, 2, 15},
		{ 1, 2, 20},
		{ 3, 2, 25},
		{ 3, 3, 27},
		{ 4, 4, 28},
		{ 5, 4, 32},
		{ 7, 5, 38},

		{ 7, 6, 38},
		{ 9, 6, 38},
		{ 9, 7, 40},
		{ 10, 8, 38},
		{ 10, 8, 40},
		{ 11, 8, 42},
		{ 20, 16, 60},
		{ 33, 55, 90},

		{ 15, 14, 50},
		{ 16, 14, 80},
		{ 17, 14, 55},
		{ 24, 20, 55},
		{ 25, 20, 70},
		{ 25, 25, 80},
		{ 39, 32, 95},
		{ 44, 44, 80},

		{ 5, 5, 50},
		{ 15, 14, 50},
		{ 30, 50, 80},
		{ 35, 70, 90},
		{ 40, 50, 80},
		{ 40, 40, 80},
		{ 42, 90, 85},
		{ 45, 90, 85},
	},
			/* Nature Favours*/
	{
		{ 1, 1, 23},
		{ 3, 3, 25},
		{ 3, 3, 25},
		{ 4, 4, 35},
		{ 4, 4, 50},
		{ 4, 5, 50},
		{ 5, 5, 50},
		{ 5, 5, 35},

		{ 5, 5, 40},
		{ 5, 5, 30},
		{ 7, 6, 45},
		{ 7, 6, 40},
		{ 9, 6, 30},
		{ 19, 12, 55},
		{ 25, 25, 90},
		{ 40, 100, 95},

		{ 7, 7, 20},
		{ 9, 12, 40},
		{ 10, 12, 75},
		{ 15, 20, 85},
		{ 30, 30, 90},
		{ 37, 40, 90},
		{ 38, 45, 75},
		{ 40, 90, 90},


		{ 20, 18, 60},
		{ 23, 23, 80},
		{ 25, 25, 75},
		{ 30, 27, 75},
		{ 35, 30, 85},
		{ 37, 35, 90},
		{ 40, 90, 95},
		{ 40, 75, 65},
	},
};

cantrip_type cantrip_info[32] =
{
	{ 1, 1, 20},
	{ 1, 1, 33},
	{ 2, 1, 33},
	{ 2, 2, 33},
	{ 3, 3, 33},
	{ 5, 5, 40},
	{ 6, 6, 33},
	{ 7, 7, 44},

	{ 8, 8, 40},
	{ 9, 9, 60},
	{ 10, 10, 50},
	{ 11, 11, 50},
	{ 13, 11, 50},
	{ 14, 12, 50},
	{ 15, 13, 50},
	{ 16, 14, 50},

	{ 17, 15, 50},
	{ 18, 16, 50},
	{ 19, 17, 33},
	{ 20, 20, 50},
	{ 23, 22, 60},
	{ 25, 24, 60},
	{ 28, 25, 70},
	{ 30, 28, 60},

	{ 35, 30, 80},
	{ 39, 36, 80},
	{ 42, 37, 60},
	{ 44, 38, 70},
	{ 46, 40, 66},
	{ 47, 42, 80},
	{ 48, 60, 70},
	{ 50, 125, 80}
};

/* Bitwise mask for spells to split them into books */

u32b spell_flags[4]=
{
    0x000000ff,
    0x0000ff00,
    0x00ff0000,
    0xff000000
};

/*
 * 
 * Bitwise mask for cantrips to split them into charms.
 * Note the strange values, because cantrips are ordered
 * by difficulty, but charms each hold cantrips of a given
 * theme.
 *
 */
u32b cantrip_flags[7]=
{
	0x10300221, /* Elemental cantrips (Charm = Sulphur)*/
	0x4a080010, /* Movement cantrips (Charm = Hemlock)*/
	0x00442080, /* Healing cantrips (Charm = Unicorn Horn) */
	0x20001508, /* Visions cantrips (Charm = Crystal)*/
	0x80800804, /* Illusion cantrips (Charm = Fly Agaric)*/
	0x0003c000, /* Protection cantrips (Charm = Garlic)*/
	0x05000042 /* Artifice cantrips (Charm = Geode)*/
};

/*
 * Names of the spells (hermetic)
 */
cptr spell_names[MAX_SCHOOL][32] =
{
    /*** Sorcery Spells ***/

    {
	/* Common Sorcery Spellbooks */
		"Detect Monsters",
		"Phase Door",
		"Detect Doors and Traps",
		"Light Area",
		"Confuse Monster",
		"Teleport",
		"Sleep Monster",
		"Recharging",

		"Magic Mapping",
		"Identify",
		"Slow Monster",
		"Mass Sleep",
		"Teleport Away",
		"Haste Self",
		"Detection True",
		"Identify True",

	/* Rare Sorcery Spellbooks */
		"Detect Objects & Treasure",
		"Detect Enchantment",
		"Charm Monster",
		"Dimension Door",
		"Sense Minds",
		"Self Knowledge",
		"Teleport Level",
		"Word of Recall",

		"Stasis",
		"Telekinesis",
		"Explosive Rune",
		"Clairvoyance",
		"Enchant Weapon",
		"Enchant Armour",
		"Alchemy",
		"Globe of Invulnerability"

    },
    /*** Thaumaturgy Spells ***/
    {
	/* Common Thaumaturgy Spellbooks */
		"Magic Missile",
		"Trap / Door Destruction",
		"Flash of Light",
		"Touch of Confusion",
		"Mana Burst",
		"Fire Bolt",
		"Fist of Force",
		"Teleport Self",

		"Wonder",
		"Chaos Bolt",
		"Sonic Boom",
		"Doom Bolt",
		"Fire Ball",
		"Teleport Other",
		"Word of Destruction",
		"Invoke Chaos",

	/* Rare Thaumaturgy Spellbooks */
		"Polymorph Other",
		"Chain Lightning",
		"Arcane Binding",
		"Disintegrate",
		"Alter Reality",
		"Polymorph Self",
		"Chaos Branding",
		"Summon Demon",

		"Beam of Gravity",
		"Meteor Swarm",
		"Flame Strike",
		"Call Chaos",
		"Shard Ball",
		"Mana Storm",
		"Breathe Chaos",
		"Call the Void"
    },
    /* Conjuration Spellbooks */
    {
        /* Common Conjuration Spellbooks */
		"Phase Door",
		"Mind Blast",
		"Tarot Draw",
		"Reset Recall",
		"Teleport",
		"Dimension Door",
		"Planar Spying",
		"Teleport Away",

		"Summon Object",
		"Summon Animal",
		"Phantasmal Servant",
		"Summon Monster",
		"Conjure Elemental",
		"Teleport Level",
		"Word of Recall",
		"Banish",

        /* Rare Conjuration Spellbooks */
		"Joker Card",
		"Summon Spiders",
		"Summon Reptiles",
		"Summon Hounds",
		"Planar Branding",
		"Planar Being",
		"Death Dealing",
		"Summon Reaver",

		"Planar Divination",
		"Planar Lore",
		"Summon Undead",
		"Summon Dragon",
		"Mass Summons",
		"Summon Demon",
		"Summon Ancient Dragon",
		"Summon Greater Undead"
    },
    /* Necromancy Spellbooks */
    {
	/* Common Necromancy Spellbooks */
		"Detect Unlife",
		"Malediction",
		"Detect Evil",
		"Stinking Cloud",
		"Black Sleep",
		"Resist Poison",
		"Horrify",
		"Enslave Undead",

		"Orb of Entropy",
		"Nether Bolt",
		"Terror",
		"Vampiric Drain",
		"Poison Branding",
		"Dispel Good",
		"Genocide",
		"Restore Life",

	/* Rare Necromancy Spellbooks */
		"Berserk",
		"Invoke Spirits",
		"Dark Bolt",
		"Battle Frenzy",
		"Vampirism True",
		"Vampiric Branding",
		"Darkness Storm",
		"Mass Genocide",

		"Death Ray",
		"Raise the Dead",
		"Esoteria",
		"Word of Death",
		"Evocation",
		"Hellfire",
		"Omnicide",
		"Wraithform"
    }
};

cptr cantrip_names[32]=
{
		"Zap", /* Elemental (1) */
		"Wizard Lock", /* Artifice (1) */
        "Detect Invisibility", /* Illusion (1) */
        "Detect Monsters", /* Visions (1) */
        
		"Blink", /* Movement (1) */
        "Light Area", /* Elemental (2) */
        "Trap & Door Destruction", /* Artifice (2) */
        "Cure Light Wounds", /* Healing (1) */

        "Detect Doors & Traps", /* Visions (2) */
        "Phlogiston", /* Elemental (3) */
        "Detect Treasure", /* Visions (3) */
        "Detect Enchantment", /* Illusion (2) */
        
		"Detect Objects", /* Visions (4) */
        "Cure Poison", /* Healing (2) */
        "Resist Cold", /* Protection (1) */
        "Resist Fire", /* Protection (2) */

        "Resist Lightning", /* Protection (3) */
        "Resist Acid", /* Protection (4) */
        "Cure Medium Wounds", /* Healing (3) */
        "Teleport", /* Movement (2) */
        
		"Stone to Mud", /* Elemental (4) */
        "Ray of Light", /* Elemental (5) */
        "Satisfy Hunger", /* Healing (4) */
        "See Invisible", /* Illusion (3) */

        "Recharging", /* Artifice (3) */
        "Teleport Level", /* Movement (3) */
        "Identify", /* Artifice (4) */
        "Teleport Away", /* Movement (4) */
        
		"Elemental Ball", /* Elemental (6) */
        "Detection", /* Visions (5) */
        "Word of Recall", /* Movement (5) */
        "Clairvoyance" /* Illusion (4) */
};

/* Favour names */
cptr favour_names[MAX_SPHERE][32]=
{
    /*** Life Spirit Favours ***/
	{
		"Detect Evil",
		"Cure Light Wounds",
		"Bless",
		"Remove Fear",
		"Call Light",
		"Detect Traps and Secret Doors",
		"Cure Medium Wounds",
		"Satisfy Hunger",

		"Remove Curse",
		"Cure Poison",   
		"Cure Critical Wounds",
		"Sense Unseen",
		"Holy Orb",
		"Protection from Evil",
		"Healing",
		"Glyph of Warding",

		"Exorcism",
		"Dispel Curse",
		"Dispel Undead & Demons",
		"Day of the Dove",
		"Dispel Evil",
		"Banish",      
		"Holy Word",
		"Warding True",

		"Heroism",
		"Prayer",
		"Bless Weapon",
		"Restoration",
		"Healing True",
		"Holy Vision",
		"Divine Intervention",
		"Holy Invulnerability"
	},
    /*** Wild Spirit Favours ***/
    {
		"Detect Creatures",
		"First Aid",
		"Detect Doors and Traps",
		"Foraging",
		"Daylight",
		"Animal Taming",
		"Resist Environment",
		"Cure Wounds & Poison",

		"Stone to Mud",
		"Lightning Bolt",
		"Nature Awareness",
		"Frost Bolt",
		"Ray of Sunlight",
		"Entangle",
		"Summon Animal",
		"Herbal Healing",

		"Door Building",
		"Stair Building",
		"Stone Skin",
		"Resistance True",
		"Animal Friendship",
		"Stone Tell",
		"Wall of Stone",
		"Protect from Corrosion",

		"Earthquake",
		"Whirlwind Attack",
		"Blizzard",
		"Lightning Storm",
		"Whirlpool",
		"Call Sunlight",
		"Elemental Branding",
		"Nature's Wrath"
    },
};

/*
 * Player's Skills
 *
 * name,increase,0,0,0,0,0,exp_to_raise
 */
 player_skill skill_set[MAX_SKILLS] =
 {
	 { "Toughness","You are toughening up.",0,0,0,0,0,5},
	 { "Mana Channeling","You are getting better at channeling mana.",0,0,0,0,0,6},
	 { "Disarming","You are getting better at disarming.",0,0,0,0,0,1},
	 { "Magical Devices","You are getting better with magical devices.",0,0,0,0,0,1},
	 { "Resistance","You are building your resistances.",0,0,0,0,0,1},
	 { "Stealth","You are getting more stealthy.",0,0,0,0,0,1},
	 { "Searching","You are getting better at searching.",0,0,0,0,0,1},
	 { "Perception","You are getting more perceptive.",0,0,0,0,0,1},
	 { "Close Combat","Your combat skills are increasing.",0,0,0,0,0,10},
	 { "Missile","Your accuracy is increasing.",0,0,0,0,0,4},
	 { "Martial Arts","Your martial arts are improving.",0,0,0,0,0,9},
	 { "Corporis","Your magice corporis is improving.",0,0,0,0,0,2},
	 { "Animae","Your magice animae magic is improving.",0,0,0,0,0,2},
	 { "Vis","Your magice vis is improving.",0,0,0,0,0,2},
	 { "Naturae","Your magice naturae is improving.",0,0,0,0,0,2},
	 { "Necromancy","Your necromancy is improving.",0,0,0,0,0,3},
	 { "Sorcery","Your sorcery is improving.",0,0,0,0,0,3},
	 { "Conjuration","Your conjuration is improving.",0,0,0,0,0,3},
	 { "Thaumaturgy","Your thaumaturgy is improving.",0,0,0,0,0,3},
	 { "Mindcrafting","Your mind is getting stronger.",0,0,0,0,0,5},
	 { "Chi","Your Chi flow is improving.",0,0,0,0,0,6},
	 { "Innate Racial","Your innate skills are improving.",0,0,0,0,0,1},
	 { "Stabbing Weapons","Your skill with stabbing weapons is improving.",0,0,0,0,0,10},
	 { "Slashing Weapons","Your skill with slashing weapons is improving.",0,0,0,0,0,10},
	 { "Crushing Weapons","Your skill with crushing weapons is improving.",0,0,0,0,0,10},
	 { "Spirit Lore","You are learning more about the spirit world",0,0,0,0,0,6},
	 { "Hedge Magic","You are getting better at hedge magic.",0,0,0,0,0,3},
	 { "Item Sensing","You feel more able to judge items.",0,0,0,0,0,1},
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
 * Certain "screens" always use the main screen, including News, Birth,
 * Dungeon, Tomb-stone, High-scores, Macros, Colors, Visuals, Options.
 *
 * Later, special flags may allow sub-windows to "steal" stuff from the
 * main window, including File dump (help), File dump (artifacts, uniques),
 * Character screen, Small scale map, Previous Messages, Store screen, etc.
 *
 * The "ctrl-g" command (or pseudo-command) should perhaps grab a snapshot
 * of the main screen into any interested windows.
 */
cptr window_flag_desc[32] =
{
	"Display inventory",
	"Display equipment",
	"Display spell list",
	"Display character",
	"Display nearby monsters",
	"Display skills",
	"Display messages",
	"Display overhead view",
	"Display monster recall",
	"Display object recall",
	"Display object details",
	/*"Display snap-shot"*/ NULL,
	"Display shop names",
	"Display help",
	/*"Display borg messages"*/ NULL,
	/*"Display borg status"*/ NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};


/*
 * Available Options (full to 3,13), 7 reserved for cheat options
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

	{ &depth_in_feet,               TRUE, OPTS_UI,      0, 7,
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

	{ &inscribe_depth,	TRUE,	OPTS_OBJ,	3,	2,
	"inscribe_depth",	"Inscribe depth on items at creation" },

	{ &show_choices,                TRUE, OPTS_UI,      0, 12,
	"show_choices",                 "Show choices in certain sub-windows" },

    { &show_details,                TRUE, OPTS_UI,      0, 13,
	"show_details",                 "Show more detailed monster descriptons" },

	{ &show_choices_main,		TRUE,	OPTS_UI,	2,26,
	"show_choices_main",		"Show certain choices in the main window"},
	
    { &ring_bell,                   FALSE, OPTS_UI,      0, 14,
	"ring_bell",                    "Audible bell (on errors, etc)" },
    /* Changed to default to FALSE -- it's so extremely annoying!!! -TY */

	{ &use_color,                   TRUE, OPTS_UI,      0, 15,
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

    { &speak_unique,                FALSE, OPTS_MISC,      0, 29,
       "speak_unique",              "Allow shopkeepers and uniques to speak" },

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

	{ &stack_allow_rods, TRUE, OPTS_OBJ, 3, 13,
	"stack_allow_rods", "Allow recharging rods to stock" },

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



    { &player_symbols,              TRUE, OPTS_UI,      1, 13,
      "player_symbols",             "Use special symbols for the player char"},

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

	{ &flush_failure,               TRUE, OPTS_PERF,      1, 23,
	"flush_failure",                "Flush input on various failures" },

	{ &flush_disturb,               FALSE, OPTS_PERF,      1, 24,
	"flush_disturb",                "Flush input whenever disturbed" },

	{ &flush_command,               FALSE, OPTS_PERF,      1, 25,
	"flush_command",                "Flush input before every command" },

	{ &fresh_before,                TRUE, OPTS_PERF,      1, 26,
	"fresh_before",                 "Flush output before every command" },

	{ &fresh_after,                 FALSE, OPTS_PERF,      1, 27,
	"fresh_after",                  "Flush output after every command" },

	{ &fresh_message,               FALSE, OPTS_PERF,      1, 28,
	"fresh_message",                "Flush output after every message" },

	{ &compress_savefile,   FALSE, OPTS_PERF,      1, 29,
	"compress_savefile",    "Compress messages in savefiles" },

    { &hilite_player,               FALSE, OPTS_UI,      1, 30,
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

	{ &testing_carry,               TRUE, OPTS_MON, 2, 15,
    "testing_carry",                "Allow monsters to carry objects" },

	{ &centre_view,               TRUE, OPTS_UI, 2, 16,
    "centre_view",                "Centre view around player (v.slow)" },

	{&macro_edit, TRUE, OPTS_UI, 3, 11,
	"macro_edit", "Use macros as edit keys in prompts" },

	{&scroll_edge, FALSE, OPTS_UI, 3, 13,
	"scroll_edge", "Scroll until detection reaches the edge"},

	{ &no_centre_run,               FALSE, OPTS_PERF, 2, 17,
    "no_centre_run",                "Do not centre view whilst running" },

	{ &auto_more,                   FALSE,  OPTS_DISTURB,      3, 12,
	"auto_more",                    "Automatically clear '-more-' prompts" },

	{ &score_quitters,		FALSE, OPTS_BIRTH, 2, 29,
	"score_quitter",			"Remember scores of those who quit the game" },

	{ &maximise_mode,               TRUE, OPTS_BIRTH, 2, 18,
    "maximise_mode",                "Include race/template bonuses in stat calcs" },

	{ &preserve_mode,               TRUE, OPTS_BIRTH, 2, 19,
    "preserve_mode",                "Artifacts are not lost if you never saw them" },

	{ &allow_quickstart,		TRUE,	OPTS_BIRTH, 3, 4,
	"allow_quickstart",	"Allow the player to start by using Quick-Start"},
	
	{ &allow_pickstats,		TRUE,	OPTS_BIRTH, 3, 6,
	"allow_pickstats",	"Allow the player to choose pre-set stats"},

#if !defined(MACINTOSH) && !defined(WINDOWS) && !defined(ACORN)
	{ &display_credits,	TRUE,	OPTS_BIRTH, 3, 5,
	"display_credits",	"Require a keypress to clear the initial screen"},
#endif

	{ &use_autoroller,               TRUE, OPTS_BIRTH, 2, 20,
    "use_autoroller",                "Allow stats to be rolled repeatedly with minima" },

	{ &spend_points,               FALSE, OPTS_BIRTH, 2, 21,
    "spend_points",                "Allow stats to be chosen by spending points" },

	{ &ironman_shop,               FALSE, OPTS_BIRTH, 2, 22,
    "ironman_shop",                "Shops (except for libraries) are locked" },

	{ &ironman_feeling,	FALSE, OPTS_BIRTH, 3, 3,
	"ironman_feeling",	"Level feelings are only given after 2500 turns" },

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
	{&stack_allow_wands, FALSE, &stack_allow_rods},
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

martial_arts ma_blows[MAX_MA] =
{
#ifdef VERBOSE_MARTIAL_ARTS
	{ "You hit %s.",							0, 0, 1, 1, 0},
    { "You punch %s.",                          1, 0, 1, 4, 0 },
    { "You kick %s.",                           2, 0, 1, 6, 0 },
    { "You strike %s.",                         3, 0, 1, 7, 0 },
    { "You hit %s with your knee.",             5, 5, 2, 3, MA_KNEE },
    { "You hit %s with your elbow.",            7, 5, 1, 8, 0 },
    { "You butt %s.",                           9, 10, 2, 5, 0 },
    { "You kick %s.",                           11, 10, 3, 4, MA_SLOW },
    { "You uppercut %s.",                       13, 12, 4, 4, 6 },
    { "You double-kick %s.",                    16, 15, 5, 4, 8 },
    { "You hit %s with a Cat's Claw.",          20, 20, 5, 5, 0 },
    { "You hit %s with a jump kick.",           25, 25, 5, 6, 10 },
    { "You hit %s with an Eagle's Claw.",       29, 25, 6, 6, 0 },
    { "You hit %s with a circle kick.",         33, 30, 6, 8, 10 },
    { "You hit %s with an Iron Fist.",          37, 35, 8, 8, 10 },
    { "You hit %s with a flying kick.",         41, 35, 8, 10, 12 },
    { "You hit %s with a Dragon Fist.",       45, 35, 10, 10, 16 },
    { "You hit %s with a Crushing Blow.",         48, 35, 10, 12, 18 },
#else
    { "You hit %s.",							0, 0, 1, 1, 0},
	{ "You punch %s.",                          1, 0, 1, 4, 0 },
    { "You kick %s.",                           2, 0, 1, 6, 0 },
    { "You strike %s.",                         3, 0, 1, 7, 0 },
    { "You knee %s.",             5, 5, 2, 3, MA_KNEE },
    { "You hit %s.",            7, 5, 1, 8, 0 },
    { "You butt %s.",                           9, 10, 2, 5, 0 },
    { "You kick %s.",                           11, 10, 3, 4, MA_SLOW },
    { "You uppercut %s.",                       13, 12, 4, 4, 6 },
    { "You double-kick %s.",                    16, 15, 5, 4, 8 },
    { "You hit %s.",          20, 20, 5, 5, 0 },
    { "You kick %s.",           25, 25, 5, 6, 10 },
    { "You hit %s.",       29, 25, 6, 6, 0 },
    { "You kick %s.",         33, 30, 6, 8, 10 },
    { "You punch %s.",          37, 35, 8, 8, 10 },
    { "You kick %s.",         41, 35, 8, 10, 12 },
    { "You punch %s.",       45, 35, 10, 10, 16 },
    { "You punch %s.",       48, 35, 10, 12, 18 },
#endif
};


mindcraft_power mindcraft_powers[MAX_MINDCRAFT_POWERS] = {
    /* Skill gained,  cost,  %fail,  name */
        { 0,   1,  15, "Precognition" },       /* Det. monsters/traps */
        { 1,   1,  20, "Neural Blast" },     /* ~MM */
        { 3,   2,  25, "Minor Displacement" }, /* Phase/dimension door */
        { 7,   6,  35, "Major Displacement" }, /* Tele. Self / All */
        { 9,   7,  50, "Domination" },
        { 11,  7,  30, "Pulverise" },      /* Telekinetic "bolt" */
        { 13, 12,  50, "Character Armour" },   /* Psychic/physical defenses */
        { 15, 12,  60, "Psychometry" },
        { 18, 10,  45, "Mind Wave" },          /* Ball -> LOS */
        { 23, 15,  50, "Adrenaline Channeling" },
        { 25, 10,  40, "Psychic Drain" },      /* Convert enemy HP to mana */
        { 29, 20,  45, "Telekinetic Wave" },   /* Ball -> LOS */
};


/*
 * Window definitions. Defined here to ensure that the names are available ASAP.
 * Everything else is blank, to be filled in later.
 */

#define BLANK_32 \
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

#define BLANK_WINDOW(name) \
{0, name, BLANK_32, BLANK_32, iilog(PW_NONE), 0},

window_type windows[8] = {
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
	{"","a minor life spirit",0,0,0x000000ff,SPIRIT_LIFE,1},
	{"","a minor wild spirit",0,0,0x000000ff,SPIRIT_NATURE,1},

	{"","a lesser life spirit",0,0,0x0000ff00,SPIRIT_LIFE,7},
	{"","a lesser wild spirit",0,0,0x0000ff00,SPIRIT_NATURE,5},

	{"","a greater life spirit",0,0,0x00ff0000,SPIRIT_LIFE,15},
	{"","a greater wild spirit",0,0,0x00ff0000,SPIRIT_NATURE,7},

	{"","a major life spirit",0,0,0xff000000,SPIRIT_LIFE,5},
	{"","a major wild spirit",0,0,0xff000000,SPIRIT_NATURE,20},
};
