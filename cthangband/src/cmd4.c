#define CMD4_C
/* File: cmd4.c */

/* Purpose: Interface commands */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"


/*
 * Hack -- redraw the screen
 *
 * This command performs various low level updates, clears all the "extra"
 * windows, does a total redraw of the main window, and requests all of the
 * interesting updates and redraws that I can think of.
 *
 * This command is also used to "instantiate" the results of the user
 * selecting various things, such as graphics mode, so it must call
 * the "TERM_XTRA_REACT" hook before redrawing the windows.
 */
extern void do_cmd_redraw(void)
{
	int j;

	term *old = Term;


	/* Hack -- react to changes */
	Term_xtra(TERM_XTRA_REACT, 0);


	/* Combine and Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);


	/* Update torch */
	p_ptr->update |= (PU_TORCH);

	/* Update stuff */
	p_ptr->update |= (PU_BONUS | PU_HP | PU_MANA | PU_SPELLS);

	/* Forget lite/view */
	p_ptr->update |= (PU_UN_VIEW | PU_UN_LITE);

	/* Update lite/view */
	p_ptr->update |= (PU_VIEW | PU_LITE);

	/* Update monsters */
	p_ptr->update |= (PU_MONSTERS);

	/* Redraw everything */
	p_ptr->redraw |= (PR_WIPE | PR_BASIC | PR_EXTRA | PR_MAP | PR_EQUIPPY);

	/* Window stuff */
	p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_SPELL | PW_PLAYER | PW_SHOPS);

	/* Window stuff */
	p_ptr->window |= (PW_MESSAGE | PW_OVERHEAD | PW_MONSTER | PW_OBJECT | PW_OBJECT_DETAILS);

	/* Hack -- update */
	handle_stuff();


	/* Redraw every window */
	for (j = 0; j < 8; j++)
	{
		/* Dead window */
		if (!windows[j].term) continue;

		/* Activate */
		Term_activate(windows[j].term);

		/* Redraw */
		Term_redraw();

		/* Refresh */
		Term_fresh();

		/* Restore */
		Term_activate(old);
	}
}


/*
 * Hack -- change name
 */
void do_cmd_change_name(void)
{
	char	c;

	int		mode = 0;

	char	tmp[160];


	/* Enter "icky" mode */
	character_icky = TRUE;

	/* Save the screen */
	Term_save();

	/* Forever */
	while (1)
	{
		/* Display the player */
		display_player(mode);

		if (mode == 7)
		{
			mode = 0;
			display_player(mode);
		}

		/* Prompt */
		Term_putstr(2, 23, -1, TERM_WHITE,
			"['c' to change name, 'f' to file, 'h' to change mode, or ESC]");

		/* Query */
		c = inkey();

		/* Exit */
		if (c == ESCAPE) break;

		/* Change name */
		if (c == 'c')
		{
			get_name();
		}

		/* File dump */
		else if (c == 'f')
		{
			sprintf(tmp, "%s.txt", player_base);
			if (get_string("File name: ", tmp, 80))
			{
				if (tmp[0] && (tmp[0] != ' '))
				{
					file_character(tmp, FALSE);
				}
			}
		}

		/* Toggle mode */
		else if (c == 'h')
		{
			mode++;
		}

		/* Oops */
		else
		{
			bell();
		}

		/* Flush messages */
		msg_print(NULL);
	}

	/* Restore the screen */
	Term_load();

	/* Leave "icky" mode */
	character_icky = FALSE;


	/* Redraw everything */
	p_ptr->redraw |= (PR_WIPE | PR_BASIC | PR_EXTRA | PR_MAP | PR_EQUIPPY);

	handle_stuff();
}


/*
 * Recall the most recent message
 */
void do_cmd_message_one(void)
{
	/* Recall one message XXX XXX XXX */
	prt(format("> %s", message_str(0)), 0, 0);
}


/*
 * Show previous messages to the user	-BEN-
 *
 * The screen format uses line 0 and 23 for headers and prompts,
 * skips line 1 and 22, and uses line 2 thru 21 for old messages.
 *
 * This command shows you which commands you are viewing, and allows
 * you to "search" for strings in the recall.
 *
 * Note that messages may be longer than 80 characters, but they are
 * displayed using "infinite" length, with a special sub-command to
 * "slide" the virtual display to the left or right.
 *
 * Attempt to only hilite the matching portions of the string.
 */
void do_cmd_messages(void)
{
	int i, j, k, n, q;

	char shower[80];
	char finder[80];


	/* Wipe finder */
	strcpy(finder, "");

	/* Wipe shower */
	strcpy(shower, "");


	/* Total messages */
	n = message_num();

	/* Start on first message */
	i = 0;

	/* Start at leftmost edge */
	q = 0;


	/* Enter "icky" mode */
	character_icky = TRUE;

	/* Save the screen */
	Term_save();

	/* Process requests until done */
	while (1)
	{
		/* Clear screen */
		Term_clear();

		/* Dump up to 20 lines of messages */
		for (j = 0; (j < 20) && (i + j < n); j++)
		{
			cptr msg = message_str((short)(i+j));

			/* Apply horizontal scroll */
			msg = (strlen(msg) >= (size_t)q) ? (msg + q) : "";

			/* Dump the messages, bottom to top */
			Term_putstr(0, 21-j, -1, TERM_WHITE, msg);

			/* Hilite "shower" */
			if (shower[0])
			{
				cptr str = msg;

				/* Display matches */
				while ((str = strstr(str, shower)) != NULL)
				{
					int len = strlen(shower);

					/* Display the match */
					Term_putstr(str-msg, 21-j, len, TERM_YELLOW, shower);

					/* Advance */
					str += len;
				}
			}
		}

		/* Display header XXX XXX XXX */
		prt(format("Message Recall (%d-%d of %d), Offset %d",
		    i, i+j-1, n, q), 0, 0);

		/* Display prompt (not very informative) */
		prt("[Press 'p' for older, 'n' for newer, ..., or ESCAPE]", 23, 0);

		/* Get a command */
		k = inkey();

		/* Exit on Escape */
		if (k == ESCAPE) break;

		/* Hack -- Save the old index */
		j = i;

		/* Horizontal scroll */
		if (k == '4')
		{
			/* Scroll left */
			q = (q >= 40) ? (q - 40) : 0;

			/* Success */
			continue;
		}
		
		/* Horizontal scroll */
		if (k == '6')
		{
			/* Scroll right */
			q = q + 40;

			/* Success */
			continue;
		}
		
		/* Hack -- handle show */
		if (k == '=')
		{
			/* Prompt */
			prt("Show: ", 23, 0);

			/* Get a "shower" string, or continue */
			if (!askfor_aux(shower, 80)) continue;

			/* Okay */
			continue;
		}

		/* Hack -- handle find */
		if (k == '/')
		{
			int z;

			/* Prompt */
			prt("Find: ", 23, 0);

			/* Get a "finder" string, or continue */
			if (!askfor_aux(finder, 80)) continue;

			/* Show it */
			strcpy(shower, finder);

			/* Scan messages */
			for (z = i + 1; z < n; z++)
			{
				cptr msg = message_str((short)z);

				/* Search for it */
				if (strstr(msg, finder))
				{
					/* New location */
					i = z;

					/* Done */
					break;
				}
			}
		}

		/* Recall 1 older message */
		if ((k == '8') || (k == '\n') || (k == '\r'))
		{
			/* Go newer if legal */
			if (i + 1 < n) i += 1;
		}

		/* Recall 10 older messages */
		if (k == '+')
		{
			/* Go older if legal */
			if (i + 10 < n) i += 10;
		}

		/* Recall 20 older messages */
		if ((k == 'p') || (k == KTRL('P')) || (k == ' '))
		{
			/* Go older if legal */
			if (i + 20 < n) i += 20;
		}

		/* Recall 20 newer messages */
		if ((k == 'n') || (k == KTRL('N')))
		{
			/* Go newer (if able) */
			i = (i >= 20) ? (i - 20) : 0;
		}

		/* Recall 10 newer messages */
		if (k == '-')
		{
			/* Go newer (if able) */
			i = (i >= 10) ? (i - 10) : 0;
		}

		/* Recall 1 newer messages */
		if (k == '2')
		{
			/* Go newer (if able) */
			i = (i >= 1) ? (i - 1) : 0;
		}

		/* Hack -- Error of some kind */
		if (i == j) bell();
	}

	/* Restore the screen */
	Term_load();

	/* Leave "icky" mode */
	character_icky = FALSE;
}



/*
 * Number of cheating options
 */
#define CHEAT_MAX 8

/*
 * Cheating options
 */
static option_type cheat_info[CHEAT_MAX] =
{
	{ &cheat_peek,		FALSE,	255,	0x01, 0x00,
	"cheat_peek",		"Peek into object creation" },

	{ &cheat_hear,		FALSE,	255,	0x02, 0x00,
	"cheat_hear",		"Peek into monster creation" },

	{ &cheat_room,		FALSE,	255,	0x04, 0x00,
	"cheat_room",		"Peek into dungeon creation" },

	{ &cheat_xtra,		FALSE,	255,	0x08, 0x00,
	"cheat_xtra",		"Peek into something else" },

	{ &cheat_item,		FALSE,	255,	0x80, 0x00,
	"cheat_item",		"Know complete item info" },

	{ &cheat_live,		FALSE,	255,	0x20, 0x00,
	"cheat_live",		"Allow player to avoid death" },

	{ &cheat_skll,		FALSE,	255,	0x40, 0x00,
	"cheat_skll",		"Peek into skill rolls" },

	{ &cheat_wzrd,		FALSE,	255,	0x00, 0x02,
	"cheat_wzrd",		"Wizard (Debug) Mode active" }
};

/*
 * Interact with some options for cheating
 */
static void do_cmd_options_cheat(cptr info)
{
	char	ch;

	int		i, k = 0, n = CHEAT_MAX;

	char	buf[80];


	/* Clear screen */
	Term_clear();

	/* Interact with the player */
	while (TRUE)
	{
		/* Prompt XXX XXX XXX */
		sprintf(buf, "%s (RET to advance, y/n/x to set, ESC to accept) ", info);
		prt(buf, 0, 0);
	
		/* Display the options */
		for (i = 0; i < n; i++)
		{
			byte a = TERM_WHITE;

			/* Color current option */
			if (i == k) a = TERM_L_BLUE;

			/* Display the option text */
			sprintf(buf, "%-48s: %s  (%s)",
			    cheat_info[i].o_desc,
			    (*cheat_info[i].o_var ? "yes" : "no "),
			    cheat_info[i].o_text);
			c_prt(a, buf, i + 2, 0);
		}

		/* Hilite current option */
		move_cursor(k + 2, 50);

		/* Get a key */
		ch = inkey();

		/* Analyze */
		switch (ch)
		{
			case ESCAPE:
			{
				return;
			}

			case '-':
			case '8':
			{
				k = (n + k - 1) % n;
				break;
			}

			case ' ':
			case '\n':
			case '\r':
			case '2':
			{
				k = (k + 1) % n;
				break;
			}

			case 'y':
			case 'Y':
			case '6':
			{
				noscore |= (cheat_info[k].o_set * 256 + cheat_info[k].o_bit);
				(*cheat_info[k].o_var) = TRUE;
				k = (k + 1) % n;
				break;
			}

			case 'n':
			case 'N':
			case '4':
			{
				(*cheat_info[k].o_var) = FALSE;
				k = (k + 1) % n;
				break;
			}

			case 'x':
			case 'X':
			{
				(*cheat_info[k].o_var) ^= 1;
				k = (k + 1) % n;
				break;
			}
			default:
			{
				bell();
				break;
			}
		}
	}
}


static option_type autosave_info[3] =
{
	{ (bool *)&autosave_l,      FALSE, 255, 0x01, 0x00,
	    "autosave_l",    "Autosave when entering new levels" },

	{ (bool *)&autosave_t,      FALSE, 255, 0x02, 0x00,
	    "autosave_t",   "Timed autosave" },

	{ (bool *)&autosave_q,      FALSE, 255, 0x04, 0x00,
	    "autosave_q",   "Quiet autosaves" },
};
       
static s16b toggle_frequency(s16b current)
{
	if (current == 0) return 50;
	if (current == 50) return 100;
	if (current == 100) return 250;
	if (current == 250) return 500;
	if (current == 500) return 1000;
	if (current == 1000) return 2500;
	if (current == 2500) return 5000;
	if (current == 5000) return 10000;
	if (current == 10000) return 25000;

	return 0;
}


/*
 * Interact with some options for cheating
 */
static void do_cmd_options_autosave(cptr info)
{
	char	ch;

	int     i, k = 0, n = 3;

	char	buf[80];


	/* Clear screen */
	Term_clear();

	/* Interact with the player */
	while (TRUE)
	{
		/* Prompt XXX XXX XXX */
		sprintf(buf, "%s (RET to advance, y/n to set, 'F' for frequency, ESC to accept) ", info);
		prt(buf, 0, 0);
	
		/* Display the options */
		for (i = 0; i < n; i++)
		{
			byte a = TERM_WHITE;

			/* Color current option */
			if (i == k) a = TERM_L_BLUE;

			/* Display the option text */
			sprintf(buf, "%-48s: %s  (%s)",
			    autosave_info[i].o_desc,
			    (*autosave_info[i].o_var ? "yes" : "no "),
			    autosave_info[i].o_text);
			c_prt(a, buf, i + 2, 0);
		}

		prt(format("Timed autosave frequency: every %d turns",  autosave_freq), 5, 0);


		/* Hilite current option */
		move_cursor(k + 2, 50);

		/* Get a key */
		ch = inkey();

		/* Analyze */
		switch (ch)
		{
			case ESCAPE:
			{
				return;
			}

			case '-':
			case '8':
			{
				k = (n + k - 1) % n;
				break;
			}

			case ' ':
			case '\n':
			case '\r':
			case '2':
			{
				k = (k + 1) % n;
				break;
			}

			case 'y':
			case 'Y':
			case '6':
			{

				(*autosave_info[k].o_var) = TRUE;
				k = (k + 1) % n;
				break;
			}

			case 'n':
			case 'N':
			case '4':
			{
				(*autosave_info[k].o_var) = FALSE;
				k = (k + 1) % n;
				break;
			}

			case 'f':
			case 'F':
			{
				autosave_freq = toggle_frequency(autosave_freq);
				prt(format("Timed autosave frequency: every %d turns",
				    autosave_freq), 5, 0);
			}

			default:
			{
				bell();
				break;
			}
		}
	}
}

/*
 * Display a text file on screen. Returns success or failure.
 */
static bool showfile(cptr name, byte col)
{
	int i = col;
	FILE *fp;
	char buf[1024];

	/* Build the filename */
	path_build(buf, 1024, ANGBAND_DIR_FILE, name);

	/* Check that the filename refers to a real file */
	if (!(fp = my_fopen(buf, "r"))) return FALSE;

	/* Dump the file to the screen */
	while (0 == my_fgets(fp, buf, 1024))
	{
		/* Display and advance */
		Term_putstr(0, i++, -1, TERM_WHITE, buf);
	}

	/* Close */
	my_fclose(fp);

	/* Flush it */
	Term_fresh();

	/* Success */
	return TRUE;
}

#define OPTPAGE_SPOILER	8

static bool opt_is_forced(int i)
{
	typedef struct force_type force_type;

	struct force_type
	{
		bool *forcing_opt;
		bool forcing_value;
		bool *forced_opt;
	};

	force_type *fs_ptr, force[] =
	{
		{&small_levels, FALSE, &dungeon_small},
		{&centre_view, FALSE, &no_centre_run},
		{&stack_force_notes, FALSE, &stack_force_notes_all},
		{&wear_confirm, FALSE, &confirm_wear_all},
		{0, 0, 0}
	};

	bool *o_var = option_info[i].o_var;

	for (fs_ptr = force; fs_ptr->forcing_opt; fs_ptr++)
	{
		if (o_var == fs_ptr->forced_opt &&
			*(fs_ptr->forcing_opt) == fs_ptr->forcing_value) return TRUE;
	}
	return FALSE;
}

/*
 * Interact with some options
 */
void do_cmd_options_aux(int page, cptr info)
{
	char	ch;

	int	i, k = 0, n = 0;

	int	opt[24];

	char	buf[80];


	/* Lookup the options */
	for (i = 0; i < 24; i++) opt[i] = 0;

	/* Scan the options */
	for (i = 0; option_info[i].o_desc; i++)
	{
		/* Notice options on this "page" */
		if (option_info[i].o_page == page) opt[n++] = i;
	}


	/* Clear screen */
	Term_clear();

	/* Hack - Give a short explanation at the bottom of the "Spoiler" menu */
	if (page == OPTPAGE_SPOILER && !showfile("spoiler.txt", n+3))
		c_prt(TERM_RED, "ANGBAND_DIR_FILE/spoiler.txt not found.", n+3, 0);

	/* Interact with the player */
	while (TRUE)
	{
		/* Prompt XXX XXX XXX */
		sprintf(buf, "%s (RET to advance, y/n to set, ESC to accept) ", info);
		prt(buf, 0, 0);
	
		/* Display the options */
		for (i = 0; i < n; i++)
		{
			byte a = TERM_WHITE;

			/* Color current option */
			if (i == k) a = TERM_L_BLUE;

			/* Display the option text */
			sprintf(buf, "%-48s: %s  (%s)",
			        option_info[opt[i]].o_desc,
			        (opt_is_forced(opt[i])) ? "N/A" :
			        (*option_info[opt[i]].o_var ? "yes" : "no "),
			        option_info[opt[i]].o_text);
			c_prt(a, buf, i + 2, 0);
		}

		/* Hilite current option */
		move_cursor(k + 2, 50);

		/* Get a key */
		ch = inkey();

		/* Analyze */
		switch (ch)
		{
			case ESCAPE:
			{
				return;
			}

			case '-':
			case '8':
			{
				do {
				k = (n + k - 1) % n;
				} while(opt_is_forced(opt[k]));
				break;
			}

			case ' ':
			case '\n':
			case '\r':
			case '2':
			{
				do {
				k = (k + 1) % n;
				} while(opt_is_forced(opt[k]));
				break;
			}

			case 'y':
			case 'Y':
			case '6':
			{
				(*option_info[opt[k]].o_var) = TRUE;
				do {
				k = (k + 1) % n;
				} while(opt_is_forced(opt[k]));
				break;
			}

			case 'n':
			case 'N':
			case '4':
			{
				(*option_info[opt[k]].o_var) = FALSE;
				do {
				k = (k + 1) % n;
				} while(opt_is_forced(opt[k]));
				break;
			}

			case 'x':
			case 'X':
			{
				(*option_info[opt[k]].o_var) ^= 1;
				do {
				k = (k + 1) % n;
				} while(opt_is_forced(opt[k]));
				break;
			}
			default:
			{
				bell();
				break;
			}
		}
	}
}

#define PRI(window, display) (windows[window].pri[display])
#define REP(window, display) (windows[window].rep[display])

/*
 * Modify the "window" options
 */
static void do_cmd_options_win(void)
{
	int i, j, d;

	int y = 0;
	int x = 0;

	char ch;

	bool go = TRUE, second = FALSE;

	/* Clear screen */
	Term_clear();

	/* Interact */
	while (go)
	{
		char min;
		bool okay = FALSE;
		s16b newpri = -1, newrep = -1; /* A bad value */

		cptr text;

		/* Choose a set of characters which has no other meaning in
		 * the layout */
		if (!rogue_like_commands)
			min = 'a'-1;
		else
			min = '0'-1;

		/* Prompt XXX XXX XXX */
		text = format("Window Flags (<dir>, +, -, ., %c-%c, ESC) ", min+1, min+10);
		prt(text, 0, 0);

		/* Display the windows */
		for (j = 0; j < 8; j++)
		{
			byte a = TERM_WHITE;

			cptr s = windows[j].name;

			/* Use color */
			if (use_color && (j == x)) a = TERM_L_BLUE;

			/* Window name, staggered, centered */
			Term_putstr(35 + j * 5 - strlen(s) / 2, 2 + j % 2, -1, a, s);
		}

		/* Display the options */
		for (i = 0; i < 16; i++)
		{
			byte a = TERM_WHITE;

			cptr str = window_flag_desc[i];

			/* Use color */
			if (use_color && (i == y)) a = TERM_L_BLUE;

			/* Unused option */
			if (!str) str = "(Unused option)";

			/* Flag name */
			Term_putstr(0, i + 5, -1, a, str);

			/* Display the windows */
			for (j = 0; j < 8; j++)
			{
				byte a = TERM_WHITE;

				char c1 = '.', c2 = ' ';

				/* Use color */
				if (use_color && (i == y) && (j == x)) a = TERM_L_BLUE;

				/* Active flags */
				if (REP(j,i)) c1 = min+REP(j,i);
				if (PRI(j,i)) c2 = min+PRI(j,i);

				/* Flag values */
				Term_putch(35 + j * 5, i + 5, a, c1);
				Term_putch(36 + j * 5, i + 5, a, c2);
			}
		}

		/* Place Cursor */
		Term_gotoxy(35 + x * 5+((second) ? 1 : 0), y + 5);

		/* Get key */
		ch = inkey();

		/* Analyze */
		switch (ch)
		{
			case ESCAPE:
			{
				okay = TRUE;
				go = FALSE;
				break;
			}

			case '+':
			{
				newpri = PRI(y,x) + 1;
				newrep = REP(y,x) + 1;
				second = FALSE;
				okay = TRUE;
				break;
				}
			case '-':
				{
				newpri = PRI(y,x) - 1;
				newrep = REP(y,x) - 1;
				second = FALSE;
				break;
			}
			case '\t':
			{
				second = !second;
				okay = TRUE;
				break;
			}
			case '.':
			{
				/* A REP of 0 means that the display is never
				 * shown, so treat in a special way. A PRI of
				 * 0 is entirely normal. */
				if (!second)
			{
					newrep = 0;
				}
				newpri = 0;
				second = FALSE;
				break;
			}
			default:
			{
				d = get_keymap_dir(ch);

				x = (x + ddx[d] + 8) % 8;
				y = (y + ddy[d] + 16) % 16;

				if (d)
				{
					second = FALSE;
					okay = TRUE;
			}
				else if (second)
				{
					newpri = ch-min;
					second = FALSE;
		}
				else
		{
					newrep = ch-min;
					second = TRUE;
			}
		}
	}

		if (newpri >= 0 && newpri <= 10)
	{
			PRI(x,y) = newpri;
			okay = TRUE;
	}
		if (newrep >= 0 && newrep <= 10)
			{
			REP(x,y) = newrep;
			okay = TRUE;
			}
		/* If it doesn't do anything sensible, complain. */
		if (!okay)
		{
			bell();
		}
	}

	/* Redraw windows. */
	p_ptr->window |= PW_RETURN;
}




/*
 * Set or unset various options.
 *
 * The user must use the "Ctrl-R" command to "adapt" to changes
 * in any options which control "visual" aspects of the game.
 */
void do_cmd_options(void)
{
	int k;


	/* Enter "icky" mode */
	character_icky = TRUE;

	/* Save the screen */
	Term_save();


	/* Interact */
	while (1)
	{
		/* Clear screen */
		Term_clear();

		/* Why are we here */
		prt("Game Options", 2, 0);

		/* Give some choices */
		prt("(1) User Interface Options", 4, 5);
		prt("(2) Disturbance Options", 5, 5);
		prt("(3) Creature Options", 6, 5);
		prt("(4) Object Options", 7, 5);
		prt("(5) Performance Options", 8, 5);
		prt("(6) Miscellaneous Options",9,5);

		/* Special choices */
		prt("(D) Base Delay Factor", 11, 5);
		prt("(H) Hitpoint Warning", 12, 5);
		prt("(A) Autosave Options", 13, 5);


		/* Window flags */
		prt("(W) Window Flags", 14, 5);

		/* Spoilers */
		prt("(S) Spoiler Options", 16, 5);
		
		/* Cheating */
		prt("(C) Cheating Options", 17, 5);

		/* Prompt */
		prt("Command: ", 18, 0);

		/* Get command */
		k = inkey();

		/* Exit */
		if (k == ESCAPE) break;

		/* Analyze */
		switch (k)
		{
			/* General Options */
			case '1':
			{
				/* Process the general options */
				do_cmd_options_aux(1, "User Interface Options");
				break;
		}

			/* Disturbance Options */
			case '2':
		{
				/* Spawn */
				do_cmd_options_aux(2, "Disturbance Options");
				break;
		}

			/* Creature Options */
			case '3':
		{
				/* Spawn */
				do_cmd_options_aux(3, "Creature Options");
				break;
			}

			/* Object Options */
			case '4':
			{
				/* Spawn */
				do_cmd_options_aux(4, "Object Options");
				break;
			}

			/* Performance Options */
			case '5':
			{
				do_cmd_options_aux(5, "Performance Options");
				break;
			}

			/* Misc Options */
			case '6':
				{
					do_cmd_options_aux(6,"Miscellaneous Options");
					break;
				}

			/* Spoiler Options */
			case 'S':
				{
					do_cmd_options_aux(OPTPAGE_SPOILER, "Spoiler Options");
					break;
				}
			/* Cheating Options */
			case 'C':
			{
				/* Spawn */
				do_cmd_options_cheat("Cheaters never win");
				break;
			}

			case 'a':
			case 'A':
			{
				do_cmd_options_autosave("Autosave");
				break;
			}

			/* Window flags */
			case 'W':
			case 'w':
			{
				/* Spawn */
				do_cmd_options_win();
				break;
			}

			/* Hack -- Delay Speed */
			case 'D':
			case 'd':
			{
				/* Prompt */
				prt("Command: Base Delay Factor", 18, 0);

				/* Get a new value */
				while (1)
				{
					int msec = delay_factor * delay_factor * delay_factor;
					prt(format("Current base delay factor: %d (%d msec)",
					           delay_factor, msec), 22, 0);
					prt("Delay Factor (0-9 or ESC to accept): ", 20, 0);
					k = inkey();
					if (k == ESCAPE) break;
					if (isdigit(k)) delay_factor = D2I(k);
					else bell();
				}

				break;
			}

			/* Hack -- hitpoint warning factor */
			case 'H':
			case 'h':
			{
				/* Prompt */
				prt("Command: Hitpoint Warning", 18, 0);

				/* Get a new value */
				while (1)
				{
					prt(format("Current hitpoint warning: %d0%%",
					           hitpoint_warn), 22, 0);
					prt("Hitpoint Warning (0-9 or ESC to accept): ", 20, 0);
					k = inkey();
					if (k == ESCAPE) break;
					if (isdigit(k)) hitpoint_warn = D2I(k);
					else bell();
				}

				break;
			}

			/* Unknown option */
			default:
			{
				/* Oops */
				bell();
				break;
			}
		}

		/* Flush messages */
		msg_print(NULL);
	}


	/* Restore the screen */
	Term_load();

	/* Leave "icky" mode */
	character_icky = FALSE;
}



/*
 * Ask for a "user pref line" and process it
 *
 * XXX XXX XXX Allow absolute file names?
 */
void do_cmd_pref(void)
{
	char buf[80];

	/* Default */
	strcpy(buf, "");

	/* Ask for a "user pref command" */
	if (!get_string("Pref: ", buf, 80)) return;

	/* Process that pref command */
	(void)process_pref_file_aux(buf);
}


#ifdef ALLOW_MACROS

/*
 * Hack -- append all current macros to the given file
 */
static errr macro_dump(cptr fname)
{
	int i;

	FILE *fff;

	char buf[1024];


	/* Build the filename */
	path_build(buf, 1024, ANGBAND_DIR_USER, fname);

	/* File type is "TEXT" */
	FILE_TYPE(FILE_TYPE_TEXT);

	/* Append to the file */
	fff = my_fopen(buf, "a");

	/* Failure */
	if (!fff) return (-1);


	/* Skip space */
	fprintf(fff, "\n\n");

	/* Start dumping */
	fprintf(fff, "# Automatic macro dump\n\n");

	/* Dump them */
	for (i = 0; i < macro__num; i++)
	{
		/* Start the macro */
		fprintf(fff, "# Macro '%d'\n\n", i);

		/* Extract the action */
		ascii_to_text(buf, macro__act[i]);

		/* Dump the macro */
		fprintf(fff, "A:%s\n", buf);

		/* Extract the action */
		ascii_to_text(buf, macro__pat[i]);

		/* Dump normal macros */
		fprintf(fff, "P:%s\n", buf);

		/* End the macro */
		fprintf(fff, "\n\n");
	}

	/* Start dumping */
	fprintf(fff, "\n\n\n\n");


	/* Close */
	my_fclose(fff);

	/* Success */
	return (0);
}


/*
 * Hack -- ask for a "trigger" (see below)
 *
 * Note the complex use of the "inkey()" function from "util.c".
 *
 * Note that both "flush()" calls are extremely important.
 */
static void do_cmd_macro_aux(char *buf)
{
	int i, n = 0;

	char tmp[1024];


	/* Flush */
	flush();

	/* Do not process macros */
	inkey_base = TRUE;

	/* First key */
	i = inkey();

	/* Read the pattern */
	while (i)
	{
		/* Save the key */
		buf[n++] = i;

		/* Do not process macros */
		inkey_base = TRUE;

		/* Do not wait for keys */
		inkey_scan = TRUE;

		/* Attempt to read a key */
		i = inkey();
	}

	/* Terminate */
	buf[n] = '\0';

	/* Flush */
	flush();


	/* Convert the trigger */
	ascii_to_text(tmp, buf);

	/* Hack -- display the trigger */
	Term_addstr(-1, TERM_WHITE, tmp);
}

#endif


/*
 * Hack -- ask for a keymap "trigger" (see below)
 *
 * Note that both "flush()" calls are extremely important.  This may
 * no longer be true, since "util.c" is much simpler now.  XXX XXX XXX
 */
static void do_cmd_macro_aux_keymap(char *buf)
{
	char tmp[1024];


	/* Flush */
	flush();


	/* Get a key */
	buf[0] = inkey();
	buf[1] = '\0';


	/* Convert to ascii */
	ascii_to_text(tmp, buf);

	/* Hack -- display the trigger */
	Term_addstr(-1, TERM_WHITE, tmp);


	/* Flush */
	flush();
}


/*
 * Hack -- append all keymaps to the given file
 */
static errr keymap_dump(cptr fname)
{
	int i;

	FILE *fff;

	char key[1024];
	char buf[1024];

	int mode;


	/* Roguelike */
	if (rogue_like_commands)
	{
		mode = KEYMAP_MODE_ROGUE;
	}

	/* Original */
	else
	{
		mode = KEYMAP_MODE_ORIG;
	}


	/* Build the filename */
	path_build(buf, 1024, ANGBAND_DIR_USER, fname);

	/* File type is "TEXT" */
	FILE_TYPE(FILE_TYPE_TEXT);

	/* Append to the file */
	fff = my_fopen(buf, "a");

	/* Failure */
	if (!fff) return (-1);


	/* Skip space */
	fprintf(fff, "\n\n");

	/* Start dumping */
	fprintf(fff, "# Automatic keymap dump\n\n");

	/* Dump them */
	for (i = 0; i < 256; i++)
	{
		cptr act;

		/* Loop up the keymap */
		act = keymap_act[mode][i];

		/* Skip empty keymaps */
		if (!act) continue;

		/* Encode the key */
		buf[0] = i;
		buf[1] = '\0';
		ascii_to_text(key, buf);

		/* Encode the action */
		ascii_to_text(buf, act);

		/* Dump the macro */
		fprintf(fff, "A:%s\n", buf);
                fprintf(fff, "C:%d:%s\n", mode, key);
	}

	/* Start dumping */
	fprintf(fff, "\n\n\n");


	/* Close */
	my_fclose(fff);

	/* Success */
	return (0);
}



/*
 * Interact with "macros"
 *
 * Note that the macro "action" must be defined before the trigger.
 *
 * Could use some helpful instructions on this page.  XXX XXX XXX
 */
void do_cmd_macros(void)
{
	int i;

	char tmp[1024];

	char buf[1024];

	int mode;


	/* Roguelike */
	if (rogue_like_commands)
	{
		mode = KEYMAP_MODE_ROGUE;
	}

	/* Original */
	else
	{
		mode = KEYMAP_MODE_ORIG;
	}

	/* File type is "TEXT" */
	FILE_TYPE(FILE_TYPE_TEXT);


	/* Enter "icky" mode */
	character_icky = TRUE;

	/* Save screen */
	Term_save();


	/* Process requests until done */
	while (1)
	{
		/* Clear screen */
		Term_clear();

		/* Describe */
		prt("Interact with Macros", 2, 0);


		/* Describe that action */
		prt("Current action (if any) shown below:", 20, 0);

		/* Analyze the current action */
		ascii_to_text(buf, macro__buf);

		/* Display the current action */
		prt(buf, 22, 0);


		/* Selections */
		prt("(1) Load a user pref file", 4, 5);
#ifdef ALLOW_MACROS
		prt("(2) Append macros to a file", 5, 5);
		prt("(3) Query a macro", 6, 5);
		prt("(4) Create a macro", 7, 5);
		prt("(5) Remove a macro", 8, 5);
		prt("(6) Append keymaps to a file", 9, 5);
		prt("(7) Query a keymap", 10, 5);
		prt("(8) Create a keymap", 11, 5);
		prt("(9) Remove a keymap", 12, 5);
		prt("(0) Enter a new action", 13, 5);
#endif /* ALLOW_MACROS */

		/* Prompt */
		prt("Command: ", 16, 0);

		/* Get a command */
		i = inkey();

		/* Leave */
		if (i == ESCAPE) break;

		/* Load a 'macro' file */
		else if (i == '1')
		{
			/* Prompt */
			prt("Command: Load a user pref file", 16, 0);

			/* Prompt */
			prt("File: ", 18, 0);

  			/* Default filename */
			sprintf(tmp, "%s.prf", player_name);
  
  			/* Ask for a file */
			if (!askfor_aux(tmp, 80)) continue;
  
  			/* Process the given filename */
			if (0 != process_pref_file(tmp))
			{
				/* Prompt */
				msg_print("Could not load file!");
			}
		}

#ifdef ALLOW_MACROS

		/* Save macros */
		else if (i == '2')
		{
			/* Prompt */
			prt("Command: Append macros to a file", 16, 0);

			/* Prompt */
			prt("File: ", 18, 0);

 			/* Default filename */
			sprintf(tmp, "%s.prf", player_name);

			/* Ask for a file */
			if (!askfor_aux(tmp, 80)) continue;

			/* Drop priv's */
			safe_setuid_drop();

			/* Dump the macros */
			(void)macro_dump(tmp);

			/* Grab priv's */
			safe_setuid_grab();

			/* Prompt */
			msg_print("Appended macros.");
		}

		/* Query a macro */
		else if (i == '3')
		{
			int k;

			/* Prompt */
			prt("Command: Query a macro", 16, 0);

			/* Prompt */
			prt("Trigger: ", 18, 0);

			/* Get a macro trigger */
			do_cmd_macro_aux(buf);

			/* Acquire action */
			k = macro_find_exact(buf);

			/* Nothing found */
			if (k < 0)
			{
				/* Prompt */
				msg_print("Found no macro.");
			}

			/* Found one */
			else
			{
				/* Obtain the action */
				strcpy(macro__buf, macro__act[k]);

				/* Analyze the current action */
				ascii_to_text(buf, macro__buf);

				/* Display the current action */
				prt(buf, 22, 0);

				/* Prompt */
				msg_print("Found a macro.");
			}
		}

		/* Create a macro */
		else if (i == '4')
		{
			/* Prompt */
			prt("Command: Create a macro", 16, 0);

			/* Prompt */
			prt("Trigger: ", 18, 0);

			/* Get a macro trigger */
			do_cmd_macro_aux(buf);

			/* Clear */
			clear_from(20);

			/* Prompt */
			prt("Action: ", 20, 0);

			/* Convert to text */
			ascii_to_text(tmp, macro__buf);

			/* Get an encoded action */
			if (askfor_aux(tmp, 80))
			{
				/* Convert to ascii */
				text_to_ascii(macro__buf, tmp);

				/* Link the macro */
				macro_add(buf, macro__buf);

				/* Prompt */
				msg_print("Added a macro.");
			}
		}

		/* Remove a macro */
		else if (i == '5')
		{
			/* Prompt */
			prt("Command: Remove a macro", 16, 0);

			/* Prompt */
			prt("Trigger: ", 18, 0);

			/* Get a macro trigger */
			do_cmd_macro_aux(buf);

			/* Link the macro */
			macro_add(buf, buf);

			/* Prompt */
			msg_print("Removed a macro.");
		}

		/* Save keymaps */
		else if (i == '6')
		{
			/* Prompt */
			prt("Command: Append keymaps to a file", 16, 0);

			/* Prompt */
			prt("File: ", 18, 0);

			/* Default filename */
			sprintf(tmp, "%s.prf", player_name);

			/* Ask for a file */
			if (!askfor_aux(tmp, 80)) continue;

			/* Drop priv's */
			safe_setuid_drop();

			/* Dump the macros */
			(void)keymap_dump(tmp);

			/* Grab priv's */
			safe_setuid_grab();

			/* Prompt */
			msg_print("Appended keymaps.");
		}

		/* Query a keymap */
		else if (i == '7')
		{
			cptr act;

			/* Prompt */
			prt("Command: Query a keymap", 16, 0);

			/* Prompt */
			prt("Keypress: ", 18, 0);

			/* Get a keymap trigger */
			do_cmd_macro_aux_keymap(buf);

			/* Look up the keymap */
			act = keymap_act[mode][(byte)(buf[0])];

			/* Nothing found */
			if (!act)
			{
				/* Prompt */
				msg_print("Found no keymap.");
			}

			/* Found one */
			else
			{
				/* Obtain the action */
				strcpy(macro__buf, act);

				/* Analyze the current action */
				ascii_to_text(buf, macro__buf);

				/* Display the current action */
				prt(buf, 22, 0);

				/* Prompt */
				msg_print("Found a keymap.");
			}
		}

		/* Create a keymap */
		else if (i == '8')
		{
			/* Prompt */
			prt("Command: Create a keymap", 16, 0);

			/* Prompt */
			prt("Keypress: ", 18, 0);

			/* Get a keymap trigger */
			do_cmd_macro_aux_keymap(buf);

			/* Clear */
			clear_from(20);

			/* Prompt */
			prt("Action: ", 20, 0);

			/* Convert to text */
			ascii_to_text(tmp, macro__buf);

			/* Get an encoded action */
			if (askfor_aux(tmp, 80))
			{
				/* Convert to ascii */
				text_to_ascii(macro__buf, tmp);

				/* Free old keymap */
				string_free(keymap_act[mode][(byte)(buf[0])]);

				/* Make new keymap */
				keymap_act[mode][(byte)(buf[0])] = string_make(macro__buf);

				/* Prompt */
				msg_print("Added a keymap.");

				/* XXX Hack -- See main-win.c */
				angband_keymap_flag = TRUE;
			}
		}

		/* Remove a keymap */
		else if (i == '9')
		{
			/* Prompt */
			prt("Command: Remove a keymap", 16, 0);

			/* Prompt */
			prt("Keypress: ", 18, 0);

			/* Get a keymap trigger */
			do_cmd_macro_aux_keymap(buf);

			/* Free old keymap */
			string_free(keymap_act[mode][(byte)(buf[0])]);

			/* Make new keymap */
			keymap_act[mode][(byte)(buf[0])] = NULL;

			/* Prompt */
			msg_print("Removed a keymap.");

			/* XXX Hack -- See main-win.c */
			angband_keymap_flag = TRUE;
		}

		/* Enter a new action */
		else if (i == '0')
		{
			/* Prompt */
			prt("Command: Enter a new action", 16, 0);

			/* Go to the correct location */
			Term_gotoxy(0, 22);

			/* Hack -- limit the value */
			tmp[80] = '\0';

			/* Get an encoded action */
			if (!askfor_aux(buf, 80)) continue;

			/* Extract an action */
			text_to_ascii(macro__buf, buf);
		}

#endif /* ALLOW_MACROS */

		/* Oops */
		else
		{
			/* Oops */
			bell();
		}

		/* Flush messages */
		msg_print(NULL);
	}

	character_icky = FALSE;

	/* Load screen */
	Term_load();
}

#ifdef ALLOW_VISUALS

typedef struct visual_type visual_type;

struct visual_type {
	cptr text;	/* A text string describing the option */
	void (*dump)(FILE *fff); /* A function that dumps the preferences to an open file. */
	/* A function that retrieves the attrs and chars for the current entry. */
	void (*load)(uint n, uint *da, uint *dc, uint *ca, uint *cc);
	/* A function that retrieves the name for the current entry. */
	char* (*name)(char *buf, uint n);
	/* A function that saves the attrs and chars for the current entry. */
	void (*save)(uint n, uint ca, uint cc);
	bool (*reject)(uint n);
	s16b max; /* The number of entries available. */
	bool attr; /* Has a colour. */
	bool chars; /* Has a character. */
};

static visual_type visual[5];
	
/* Just in case a specific reference needs to be made. */
#define VISUAL_MONSTER	(&visual[0])
#define VISUAL_OBJECT	(&visual[1])
#define VISUAL_FEATURE	(&visual[2])
#define VISUAL_MONCOL	(&visual[3])
#define VISUAL_UNIDENT	(&visual[4])

/*
 * Find the name of an unidentified object.
 */
static char *visual_name_unident(char *buf, uint n)
{
	/* Set everything up. k_info[1] is arbitrary, but it certainly exists. */
	object_kind hack_k, *k_ptr = &k_info[1];
	object_type hack_o, *o_ptr = &hack_o;

	/* Put *k_ptr somewhere safe and clear it. */
	COPY(&hack_k, k_ptr, object_kind);
	WIPE(k_ptr, object_kind);

	/* Set up *k_ptr and *o_ptr. The tval is arbitrary. */
	k_ptr->tval = TV_FOOD;
	k_ptr->cost = 1;
	k_ptr->u_idx = n;
	object_prep(o_ptr, k_ptr-k_info);

	/* Get the name. */
	object_desc(buf, o_ptr, FALSE, 0);

	/* Replace the real *k_ptr */
	COPY(k_ptr, &hack_k, object_kind);
	
	return buf;
}

static char *visual_name_mon(char *buf, uint n)
{
	strcpy(buf, r_name+r_info[n].name);
	return buf;
}

static char *visual_name_feat(char *buf, uint n)
{
	strcpy(buf, f_name+f_info[n].name);
	return buf;
}

static char *visual_name_moncol(char *buf, uint n)
{
	strcpy(buf, moncol[n].name);
	return buf;
}

static char *visual_name_obj(char *buf, uint n)
{
	/* Create the object */
	object_type forge;
	object_prep(&forge, n);

	/* Describe the object */
	object_desc_store(buf, &forge, FALSE, 0);
	
	/* Return the description */
	return buf;
}
	

/*
 * Dump an arbitrary set of attr/char definitions to an open file.
 *
 * x_info is the array from which this is derived. It must have a
 * 'name' entry, an x_attr entry giving the current attr and an
 * x_char entry giving the current char.
 *
 * x_name is the name array such that x_name+x_info[i].name gives the
 * name of item i.
 *
 * max is the total number of entries in array.
 *
 * startchar is a character which indicates the type of string. This must
 * be the same as in process_pref_file_aux().
 *
 * initstring is a comment string which is displayed before the dump. It is
 * formatted as a comment as it is printed.
 */
 
#define dump_visual(vs_ptr, x_info, startchar, initstring) \
{\
	s16b i; \
	char buf[1024]; \
\
	/* Start dumping */ \
	fprintf(fff, "\n\n"); \
	fprintf(fff, "# %s\n", initstring); \
	fprintf(fff, "%c:---reset---\n\n", startchar); \
\
	/* Dump entries */ \
	for (i = 0; i < vs_ptr->max; i++) \
	{ \
\
		/* Skip non-entries */ \
		if (vs_ptr->reject && (*vs_ptr->reject)(i)) continue; \
\
		/* Skip default entries */ \
		if ((x_info[i].x_attr == x_info[i].d_attr) && \
			(x_info[i].x_char == x_info[i].d_char)) continue; \
\
		/* Dump a comment */ \
		fprintf(fff, "# %s\n", (*vs_ptr->name)(buf, i)); \
\
		/* Dump the attr/char info */ \
		fprintf(fff, "%c:%d:0x%02X:0x%02X\n\n", startchar, \
		i, (byte)(x_info[i].x_attr), (byte)(x_info[i].x_char)); \
	} \
\
	/* All done */ \
	fprintf(fff, "\n\n\n\n"); \
}

static void visual_dump_mon(FILE *fff)
dump_visual(VISUAL_MONSTER, r_info, 'R', "Monster attr/char definitions")

static void visual_dump_obj(FILE *fff)
dump_visual(VISUAL_OBJECT, k_info, 'K', "Object attr/char definitions")

static void visual_dump_feat(FILE *fff)
dump_visual(VISUAL_FEATURE, f_info, 'F', "Feature attr/char definitions")

static void visual_dump_unident(FILE *fff)
{
	s16b i;
	char buf[ONAME_MAX];

	visual_type *vs_ptr = VISUAL_UNIDENT;

	/* Start dumping */
	fprintf(fff, "\n\n");
	fprintf(fff, "# %s\n\n", "Unidentified object attr/char definitions");
	fprintf(fff, "%c:---reset---\n\n", 'U');

	/* Dump entries */
	for (i = 0; i < MAX_U_IDX; i++)
	{
		unident_type *u_ptr = &u_info[i];

		/* Skip non-entries */
		if ((*vs_ptr->reject)(i)) continue;

		/* Skip default entries */ \
		if ((u_info[i].x_attr == u_info[i].d_attr) &&
			(u_info[i].x_char == u_info[i].d_char)) continue;

		/* Dump a comment */
		fprintf(fff, "# %s\n", (*vs_ptr->name)(buf, i));

		/* Dump the attr/char info */
		fprintf(fff, "%c:%d:%d:0x%02X:0x%02X\n\n", 'U',
		u_ptr->p_id, u_ptr->s_id, (byte)(u_ptr->x_attr), (byte)(u_ptr->x_char));
	}

	/* All done */
	fprintf(fff, "\n\n\n\n");
}

/*
 * Load a name and a set of visual preferences.
 *
 * x_info is the array from which this is derived. It must have a 'name' entry,
 * an 'x_attr' entry, an 'x_char' entry, a 'd_attr' entry and a 'd_char' entry.
 * x_attr and x_char are the user-defined attr and char, and d_attr and d_char
 * the default ones.
 *
 * x_name is the name array such that x_name+x_info[n].name gives the name for entry n.
 */
#define load_visual(x_info, x_name) \
{ \
	(*da) = (byte)(x_info[n].d_attr); \
	(*dc) = (byte)(x_info[n].d_char); \
	(*ca) = (byte)(x_info[n].x_attr); \
	(*cc) = (byte)(x_info[n].x_char); \
}

static void visual_load_mon(uint n, uint *da, uint *dc, uint *ca, uint *cc)
load_visual(r_info, r_name)

static void visual_load_feat(uint n, uint *da, uint *dc, uint *ca, uint *cc)
load_visual(f_info, f_name)

static void visual_load_unident(uint n, uint *da, uint *dc, uint *ca, uint *cc)
load_visual(u_info, u_name)

static void visual_load_obj(uint n, uint *da, uint *dc, uint *ca, uint *cc)
	load_visual(k_info, k_name)

/*
 * Save an arbitrary set of attr/char definitions to x_info[n].
 */
#define save_visual(x_info) \
{\
	x_info[n].x_attr = ca; \
	x_info[n].x_char = cc; \
}

static void visual_save_mon(uint n, uint ca, uint cc)
save_visual(r_info)

static void visual_save_obj(uint n, uint ca, uint cc)
save_visual(k_info)

static void visual_save_feat(uint n, uint ca, uint cc)
save_visual(f_info)

static void visual_save_unident(uint n, uint ca, uint cc)
save_visual(u_info)

static void visual_dump_moncol(FILE *fff)
{
	s16b n;
	cptr atchar="dwsorgbuDWvyRGBU";
	char out[80] = "M";

	for (n = 0; n < MAX_MONCOL; n++)
	{
		moncol_type *mc_ptr = &moncol[n];
		char c1 = atchar[mc_ptr->attr/16];
		char c2 = atchar[mc_ptr->attr%16];

		/* A leading space looks neater than a leading d.*/
		if (c1 == 'd') c1 = ' ';
		strcat(out, format(":%c%c", c1, c2));
	}
	/* Start dumping */
	fprintf(fff, "\n\n");
	fprintf(fff, "# Monster memory attr definitions\n\n");
	fprintf(fff, out);
	fprintf(fff, "\n\n\n\n");
}

static void visual_load_moncol(uint n, uint *da, uint UNUSED *dc, uint *ca, uint UNUSED *cc)
{
	moncol_type *mc_ptr = &moncol[n];
	(*da) = TERM_WHITE;
	(*ca) = mc_ptr->attr;
}

static void visual_save_moncol(uint n, uint ca, uint UNUSED cc)
{
	moncol_type *mc_ptr = &moncol[n];
	mc_ptr->attr = ca;
}

static bool visual_reject_feat(uint n)
{
	return (f_info[n].mimic != n);
}

static bool visual_reject_unident(uint n)
{
	return (u_info[n].s_id == SID_BASE);
}

static bool visual_reject_obj(uint n)
{
	return (k_info[n].name == 0);
}

static visual_type visual[5] = {
	{"monster attr/chars", visual_dump_mon, visual_load_mon, visual_name_mon, visual_save_mon, 0, MAX_R_IDX, TRUE, TRUE},
	{"object attr/chars", visual_dump_obj, visual_load_obj, visual_name_obj, visual_save_obj, visual_reject_obj, MAX_K_IDX, TRUE, TRUE},
	{"feature attr/chars", visual_dump_feat, visual_load_feat, visual_name_feat, visual_save_feat, visual_reject_feat, MAX_F_IDX, TRUE, TRUE},
	{"monster memory attrs", visual_dump_moncol, visual_load_moncol, visual_name_moncol, visual_save_moncol, 0, MAX_MONCOL, TRUE, FALSE},
	{"unidentified object attr/chars", visual_dump_unident, visual_load_unident, visual_name_unident, visual_save_unident, visual_reject_unident, 0, TRUE, TRUE},
};

#endif /* ALLOW_VISUALS */


/*
 * Interact with "visuals"
 */
void do_cmd_visuals(void)
{
#ifdef ALLOW_VISUALS

/* The number of members of the visual[] array. */
#define VISUALS (sizeof(visual)/sizeof(visual_type))

#else /* ALLOW_VISUALS */

#define VISUALS 0

#endif /* ALLOW_VISUALS */

/* The line on which the command prompt appears. */
#define CMDLINE (7+2*VISUALS)

	byte i;

	FILE *fff;

	char tmp[160];

	char buf[1024];

	/* Hack - MAX_U_IDX is a variable. */
	VISUAL_UNIDENT->max = MAX_U_IDX;

	/* File type is "TEXT" */
	FILE_TYPE(FILE_TYPE_TEXT);


	/* Enter "icky" mode */
	character_icky = TRUE;

	/* Save the screen */
	Term_save();


	/* Interact until done */
	while (1)
	{
		/* Clear screen */
		Term_clear();

		/* Ask for a choice */
		prt("Interact with Visuals", 2, 0);

		prt("(a) Load a user pref file", 4, 5);
#ifdef ALLOW_VISUALS
		/* Give some choices */
		for (i = 0; i < VISUALS; i++)
		{
			visual_type *vs_ptr = &visual[i];
			prt(format("(%c) Dump %s", 'b'+i, vs_ptr->text), 5+i, 5);
			prt(format("(%c) Change %s", 'b'+i+VISUALS, vs_ptr->text), 5+i+VISUALS, 5);
		}
#endif
		prt(format("(%c) Reset visuals", 'b'+2*VISUALS), 5+2*VISUALS, 5);

		/* Prompt */
		prt("Command: ", CMDLINE, 0);

		/* Prompt */
		i = inkey();

		/* Done */
		if (i == ESCAPE) break;

		/* Load a 'pref' file */
		else if (i == 'a')
		{
			/* Prompt */
			prt("Command: Load a user pref file", CMDLINE, 0);

			/* Prompt */
			prt("File: ", CMDLINE+2, 0);

			/* Default filename */
			sprintf(tmp, "user-%s.prf", ANGBAND_SYS);

			/* Query */
			if (!askfor_aux(tmp, 70)) continue;

			/* Process the given filename */
			(void)process_pref_file(tmp);
		}

#ifdef ALLOW_VISUALS

		/* Dump a visual file. */
		else if (i > 'a' && i < 'b'+VISUALS)
		{
			visual_type *vs_ptr = &visual[i-'b'];

			/* Prompt */
			prt(format("Command: Dump %s", vs_ptr->text), CMDLINE, 0);

			/* Prompt */
			prt("File: ", CMDLINE+2, 0);

			/* Default filename */
			sprintf(tmp, "user-%s.prf", ANGBAND_SYS);

			/* Get a filename */
			if (!askfor_aux(tmp, 70)) continue;

			/* Build the filename */
			path_build(buf, 1024, ANGBAND_DIR_USER, tmp);

			/* Drop priv's */
			safe_setuid_drop();

			/* Append to the file */
			fff = my_fopen(buf, "a");

			/* Grab priv's */
			safe_setuid_grab();

			/* Failure */
			if (!fff) continue;

			(*(vs_ptr->dump))(fff);

			/* Close */
			my_fclose(fff);

			/* Message */
			msg_format("Dumped %s.", vs_ptr->text);
		}

		/* Modify a visual function. */
		else if (i > 'a'+VISUALS && i < 'b'+2*VISUALS)
		{
			visual_type *vs_ptr = &visual[i-'b'-VISUALS];

			int r = vs_ptr->max-1, inc, *out, max, num;

			bool started = FALSE;

			prt(format("Command: Change %s", vs_ptr->text), CMDLINE, 0);

			/* Hack -- query until done */
			while (1)
			{
				int da, dc, ca, cc;
				char *text;
				cptr prompt;
dcv_retry:

				/* Grab the information for the current entry.*/
				(*(vs_ptr->load))(r, &da, &dc, &ca, &cc);
				
				(*(vs_ptr->name))(buf, r);

				/* Label the object */
				Term_putstr(5, CMDLINE+2, -1, TERM_WHITE,
				            format("Number = %d, Name = %-40.40s",
				                   r, buf));

				/* Display the default/current attr/char. */
				Term_putstr(40, CMDLINE+4, -1, TERM_WHITE, "<< ? >>");
				Term_putstr(40, CMDLINE+5, -1, TERM_WHITE, "<< ? >>");

				Term_putch(43, CMDLINE+4, (vs_ptr->attr) ? da : TERM_WHITE, (vs_ptr->chars) ? dc : '#');
				Term_putch(43, CMDLINE+5, (vs_ptr->attr) ? ca : TERM_WHITE, (vs_ptr->chars) ? cc : '#');

				if (vs_ptr->attr && vs_ptr->chars)
					text = format("Default attr/char = %3u / %3u", da, dc);
				else if (vs_ptr->attr)
					text = format("Default attr = %3u", da);
				else if (vs_ptr->chars)
					text = format("Default char = %3u", dc);
				else
					text = (char*)"A white #";
				Term_putstr(10, CMDLINE+4, -1, TERM_WHITE, text);

				if (vs_ptr->attr && vs_ptr->chars)
					text = format("Current attr/char = %3u / %3u", ca, cc);
				else if (vs_ptr->attr)
					text = format("Current attr = %3u", ca);
				else if (vs_ptr->chars)
					text = format("Current char = %3u", cc);
				else
					text = (char*)"A white #";
				Term_putstr(10, CMDLINE+5, -1, TERM_WHITE, text);

				/* Prompt */
				Term_putstr(0, CMDLINE+7, -1, TERM_WHITE,
				            format("Command (n/N%s%s): ", 
					    (vs_ptr->attr) ? "/a/A" : "",
					    (vs_ptr->chars) ? "/c/C" : ""));

				/* Get a command */
				if (started)
				{
					i = inkey();
				}
				else
				{
					i = 'n';
					started = TRUE;
				}

				/* All done */
				if (i == ESCAPE) break;

				/* Split i into base character and modifier. */
				inc = (iscntrl(i)) ? 0 : (islower(i)) ? 1 : -1;
				i = 'a'+i-(iscntrl(i) ? KTRL('A') : (islower(i)) ? 'a' : 'A');

				switch (i)
				{
					case 'n':
						prompt = "Select number of desired entry: ";
						out = &r;
						max = vs_ptr->max;
						break;
					case 'a':
						prompt = "Select number of desired colour: ";
						out = &ca;
						max = 256;
						break;
					case 'c':
						prompt = "Select number of desired character: ";
						out = &cc;
						max = 256;
						break;
					/* Not a valid response. */
					default:
						goto dcv_retry;
				}

				/* Analyze */
				if (inc == 0)
				{
					char str[5] = "";

					inc = 0;

					if (get_string(prompt, str, 4))
					{
						num = strtol(str, NULL, 0);

						/* Number out of bounds. */
						if (num < 0 || num >= max) continue;

						/* Invalid number in bounds. */
						if (i == 'n' && vs_ptr->reject &&
							(*vs_ptr->reject)(num)) continue;

						/* Everything as it should be. */
						(*out) = num;
					}
				}
				else
				{
					(*out) = ((*out) + max + inc) % max;

				/* Handle rejection criteria if necessary. */
				if (i == 'n' && vs_ptr->reject)
					while ((*vs_ptr->reject)(r))
						r = (r+inc+vs_ptr->max) % vs_ptr->max;
				}
				/* Save the information if it has changed.*/
				if (strchr("ac", i)) (*(vs_ptr->save))(r, (byte)ca, (byte)cc);
			}
		}

#endif

		/* Reset visuals */
		else if (i == 'b'+2*VISUALS)
		{
			/* Reset */
			reset_visuals();

			/* Message */
			msg_print("Visual attr/char tables reset.");
		}

		/* Unknown option */
		else
		{
			bell();
		}

		/* Flush messages */
		msg_print(NULL);
	}


	/* Restore the screen */
	Term_load();

	/* Leave "icky" mode */
	character_icky = FALSE;
}


/*
 * Interact with "colors"
 */
void do_cmd_colors(void)
{
	int i;

	FILE *fff;

	char tmp[160];

	char buf[1024];


	/* File type is "TEXT" */
	FILE_TYPE(FILE_TYPE_TEXT);


	/* Enter "icky" mode */
	character_icky = TRUE;

	/* Save the screen */
	Term_save();


	/* Interact until done */
	while (1)
	{
		/* Clear screen */
		Term_clear();

		/* Ask for a choice */
		prt("Interact with Colors", 2, 0);

		/* Give some choices */
		prt("(1) Load a user pref file", 4, 5);
#ifdef ALLOW_COLORS
		prt("(2) Dump colors", 5, 5);
		prt("(3) Modify colors", 6, 5);
#endif

		/* Prompt */
		prt("Command: ", 8, 0);

		/* Prompt */
		i = inkey();

		/* Done */
		if (i == ESCAPE) break;

		/* Load a 'pref' file */
		if (i == '1')
		{
			/* Prompt */
			prt("Command: Load a user pref file", 8, 0);

			/* Prompt */
			prt("File: ", 10, 0);

			/* Default file */
			sprintf(tmp, "user-%s.prf", ANGBAND_SYS);

			/* Query */
			if (!askfor_aux(tmp, 70)) continue;

			/* Process the given filename */
			(void)process_pref_file(tmp);

			/* Mega-Hack -- react to changes */
			Term_xtra(TERM_XTRA_REACT, 0);

			/* Mega-Hack -- redraw */
			Term_redraw();
		}

#ifdef ALLOW_COLORS

		/* Dump colors */
		else if (i == '2')
		{
			/* Prompt */
			prt("Command: Dump colors", 8, 0);

			/* Prompt */
			prt("File: ", 10, 0);

			/* Default filename */
			sprintf(tmp, "user-%s.prf", ANGBAND_SYS);

			/* Get a filename */
			if (!askfor_aux(tmp, 70)) continue;

			/* Build the filename */
			path_build(buf, 1024, ANGBAND_DIR_USER, tmp);

			/* Drop priv's */
			safe_setuid_drop();

			/* Append to the file */
			fff = my_fopen(buf, "a");

			/* Grab priv's */
			safe_setuid_grab();

			/* Failure */
			if (!fff) continue;

			/* Start dumping */
			fprintf(fff, "\n\n");
			fprintf(fff, "# Color redefinitions\n\n");

			/* Dump colors */
			for (i = 0; i < 256; i++)
			{
				int kv = angband_color_table[i][0];
				int rv = angband_color_table[i][1];
				int gv = angband_color_table[i][2];
				int bv = angband_color_table[i][3];

				cptr name = "unknown";

				/* Skip non-entries */
				if (!kv && !rv && !gv && !bv) continue;

				/* Extract the color name */
				if (i < 16) name = color_names[i];

				/* Dump a comment */
				fprintf(fff, "# Color '%s'\n", name);

				/* Dump the monster attr/char info */
				fprintf(fff, "V:%d:0x%02X:0x%02X:0x%02X:0x%02X\n\n",
				        i, kv, rv, gv, bv);
			}

			/* All done */
			fprintf(fff, "\n\n\n\n");

			/* Close */
			my_fclose(fff);

			/* Message */
			msg_print("Dumped color redefinitions.");
		}

		/* Edit colors */
		else if (i == '3')
		{
			static int a = 0;

			/* Prompt */
			prt("Command: Modify colors", 8, 0);

			/* Hack -- query until done */
			while (1)
			{
				cptr name;

				/* Clear */
				clear_from(10);

				/* Exhibit the normal colors */
				for (i = 0; i < 16; i++)
				{
					/* Exhibit this color */
					Term_putstr(i*4, 20, -1, (byte)a, "###");

					/* Exhibit all colors */
					Term_putstr(i*4, 22, -1, (byte)i, format("%3d", i));
				}

				/* Describe the color */
				name = ((a < 16) ? color_names[a] : "undefined");

				/* Describe the color */
				Term_putstr(5, 10, -1, TERM_WHITE,
				            format("Color = %d, Name = %s", a, name));

				/* Label the Current values */
				Term_putstr(5, 12, -1, TERM_WHITE,
				            format("K = 0x%02x / R,G,B = 0x%02x,0x%02x,0x%02x",
				                   angband_color_table[a][0],
				                   angband_color_table[a][1],
				                   angband_color_table[a][2],
				                   angband_color_table[a][3]));

				/* Prompt */
				Term_putstr(0, 14, -1, TERM_WHITE,
				            "Command (n/N/k/K/r/R/g/G/b/B): ");

				/* Get a command */
				i = inkey();

				/* All done */
				if (i == ESCAPE) break;

				/* Analyze */
				if (i == 'n') a = (byte)(a + 1);
				if (i == 'N') a = (byte)(a - 1);
				if (i == 'k') angband_color_table[a][0] = (byte)(angband_color_table[a][0] + 1);
				if (i == 'K') angband_color_table[a][0] = (byte)(angband_color_table[a][0] - 1);
				if (i == 'r') angband_color_table[a][1] = (byte)(angband_color_table[a][1] + 1);
				if (i == 'R') angband_color_table[a][1] = (byte)(angband_color_table[a][1] - 1);
				if (i == 'g') angband_color_table[a][2] = (byte)(angband_color_table[a][2] + 1);
				if (i == 'G') angband_color_table[a][2] = (byte)(angband_color_table[a][2] - 1);
				if (i == 'b') angband_color_table[a][3] = (byte)(angband_color_table[a][3] + 1);
				if (i == 'B') angband_color_table[a][3] = (byte)(angband_color_table[a][3] - 1);

				/* Hack -- react to changes */
				Term_xtra(TERM_XTRA_REACT, 0);

				/* Hack -- redraw */
				Term_redraw();
			}
		}

#endif

		/* Unknown option */
		else
		{
			bell();
		}

		/* Flush messages */
		msg_print(NULL);
	}


	/* Restore the screen */
	Term_load();

	/* Leave "icky" mode */
	character_icky = FALSE;
}


/*
 * Note something in the message recall
 */
void do_cmd_note(void)
{
	char buf[80];

	/* Default */
	strcpy(buf, "");

	/* Input */
	if (!get_string("Note: ", buf, 60)) return;

	/* Ignore empty notes */
	if (!buf[0] || (buf[0] == ' ')) return;

	/* Add the note to the message recall */
	msg_format("Note: %s", buf);
}


/*
 * Mention the current version
 */
void do_cmd_version(void)
{
   /* Silly message */
    msg_format("You are playing %s %d.%d.%d.", GAME_NAME, 
	           VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
	/* These are ANSI standard constants so should work on any compiler */
	msg_format("(Compiled %s %s)", __TIME__, __DATE__);
}



/*
 * Array of feeling strings
 */
static cptr do_cmd_feeling_text[11] =
{
	"You're not sure about this level yet.",
	"You feel there is something special about this level.",
	"You nearly faint as horrible visions of death fill your mind!",
	"This level looks very dangerous.",
	"You have a very bad feeling...",
	"You have a bad feeling...",
	"You feel nervous.",
	"You feel your luck is turning...",
	"You don't like the look of this place.",
	"This level looks reasonably safe.",
	"What a boring place..."
};


/*
 * Display the feeling.
 * if ironman_feeling is set, this gives no useful information before the 2500th
 * turn.
 */
void do_cmd_feeling(bool FeelingOnly)
{
	/* Verify the feeling */
	if (feeling < 0) feeling = 0;
	if (feeling > 10) feeling = 10;

	/* No useful feeling in town, but print the town name */
	if (dun_level <= 0)
	{
	/* If you are in a town */
		if(!FeelingOnly)
		{
			if(wild_grid[wildy][wildx].dungeon < MAX_TOWNS)
			{
				msg_format("You are in %s.",town_defs[cur_town].name);
			}
			else if(wild_grid[wildy][wildx].dungeon < MAX_CAVES)
			{
				msg_format("You are outside %s.",dun_defs[wild_grid[wildy][wildx].dungeon].name);
			}
			else
			{
				msg_print("You are wandering around outside.");
			}
		}
		return;
	}
	
	/* You are in a dungeon, so... */
	if(!FeelingOnly)
	{
		msg_format("You are in %s.",dun_defs[cur_dungeon].name);

		/* Show quest status */
		if (is_quest(dun_level))
		{
			print_quest_message();
		}
	}

	if (ironman_feeling && turn-old_turn < 2500)
	{
		/* Give an uninformative message if requested. */
		if (!FeelingOnly) msg_print(do_cmd_feeling_text[0]);
	}
	else
	{
		/* Display the feeling */
		msg_print(do_cmd_feeling_text[feeling]);
	}

	if ((cheat_wzrd) || (cheat_skll))
	{
		msg_format("Maximum %d for skills on this level",(dun_depth) * 3 / 2);
	}
}





/*
 * Hack -- load a screen dump from a file
 */
void do_cmd_load_screen(void)
{
	int y, x;

	bool okay = TRUE;

	FILE *fff;

	char buf[1024];


	/* Build the filename */
	path_build(buf, 1024, ANGBAND_DIR_USER, "dump.txt");

	/* Append to the file */
	fff = my_fopen(buf, "r");

	/* Oops */
	if (!fff) return;


	/* Enter "icky" mode */
	character_icky = TRUE;

	/* Save the screen */
	Term_save();

	/* Clear the screen */
	Term_clear();


	/* Load the screen */
	for (y = 0; okay && (y < 24); y++)
	{
		/* Get a line of data */
		if (my_fgets(fff, buf, 1024)) okay = FALSE;

		/* Show each row */
		for (x = 0; x < 79; x++)
		{
			byte a = (buf[2*x]-' ')%32;
			unsigned char c = (buf[2*x+1]-' ')+(((buf[2*x]-' ')/0x10)*0x40);

			/* Put the attr/char */
			Term_draw(x, y, a, c);
		}
	}


	/* Close it */
	my_fclose(fff);


	/* Message */
	msg_print("Screen dump loaded.");
	msg_print(NULL);


	/* Restore the screen */
	Term_load();

	/* Leave "icky" mode */
	character_icky = FALSE;
}



/*
 * Redefinable "save_screen" action
 */
static void (*screendump_aux)(void) = NULL;






/*
 * Hack -- save a screen dump to a file
 */
void do_cmd_save_screen(void)
{
	/* Do we use a special screendump function ? */
	if (screendump_aux)
	{
		/* Dump the screen to a graphics file */
		(*screendump_aux)();
	}
	else /* Dump the screen as text */
	{
		int y, x;

		byte a = 0;
		unsigned char c = ' ';

		FILE *fff;

		char buf[1024];


		/* Build the filename */
		path_build(buf, 1024, ANGBAND_DIR_USER, "dump.txt");

		/* File type is "TEXT" */
		FILE_TYPE(FILE_TYPE_TEXT);

		/* Hack -- drop permissions */
		safe_setuid_drop();

		/* Append to the file */
		fff = my_fopen(buf, "w");

		/* Hack -- grab permissions */
		safe_setuid_grab();

		/* Oops */
		if (!fff) return;


		/* Enter "icky" mode */
		character_icky = TRUE;

		/* Save the screen */
		Term_save();


		/* Dump the screen (reloadable version) */
		for (y = 0; y < 24; y++)
		{
			/* Dump each row */
			for (x = 0; x < 79; x++)
			{
				/* Get the attr/char */
				(void)(Term_what(x, y, &a, &c));

				/* c can take all sorts of strange values, so dump an
				 * abstract representation using characters in the range
				 * 32-95 only.
				 */

				/* Dump them */
				buf[2*x] = (a%0x10)+(c/0x40)*0x10+' ';
				buf[2*x+1] = (c%0x40)+' ';
			}

			/* Terminate */
			buf[2*x] = '\0';

			/* End the row */
			fprintf(fff, "%s\n", buf);
		}

		/* Skip a line */
		fprintf(fff, "\n");

		/* Dump the screen (original version, requires "printable" characters) */
		for (y = 0; y < 24; y++)
		{
			/* Dump each row */
			for (x = 0; x < 79; x++)
			{
				/* Get the attr/char */
				(void)(Term_what(x, y, &a, &c));

				/* Dump it */
				buf[x] = c;
			}

			/* Terminate */
			buf[x] = '\0';

			/* End the row */
			fprintf(fff, "%s\n", buf);
		}

		/* Skip a line */
		fprintf(fff, "\n");


		/* Dump the screen */
		for (y = 0; y < 24; y++)
		{
			/* Dump each row */
			for (x = 0; x < 79; x++)
			{
				char hack[17] = "dwsorgbuDWvyRGBU";

				/* Get the attr/char */
				(void)(Term_what(x, y, &a, &c));

				/* Dump it */
				buf[x] = hack[a&0x0F];
			}

			/* Terminate */
			buf[x] = '\0';

			/* End the row */
			fprintf(fff, "%s\n", buf);
		}

		/* Skip a line */
		fprintf(fff, "\n");



		/* Close it */
		my_fclose(fff);


		/* Message */
		msg_print("Screen dump saved.");
		msg_print(NULL);


		/* Restore the screen */
		Term_load();

		/* Leave "icky" mode */
		character_icky = FALSE;
	}
}


/*
 * A simple function to include a coloured symbol in a text file for
 * show_file().
 */
static cptr get_symbol_aux(byte a, char c)
{
 	cptr atchar="dwsorgbuDWvyRGBU";

	/* Hack - show_file() does not display unprintable characters,
	 * so replace them with something it will display. */
	if (!isprint(c)) c = '#';

	return format("[[[[[%c%c]", atchar[a], c);
}

/*
 * A wrapper to find an appropriate attr/char combination from the *_info
 * array. The isprint() check is necessary as show_file() reacts badly to
 * unprintable characters.
 */
#define get_symbol(x_ptr) \
	get_symbol_aux((x_ptr)->x_attr, \
		isprint((x_ptr)->x_char) ? (x_ptr)->x_char : (x_ptr)->d_char)

/*
 * Check the status of "artifacts"
 */
static void do_cmd_knowledge_artifacts(void)
{
	int i, k, z, x, y;

	FILE *fff;

	char file_name[1024];

	char base_name[80];

	bool okay[MAX_A_IDX];


	/* Temporary file */
	if (path_temp(file_name, 1024)) return;

	/* Open a new file */
	fff = my_fopen(file_name, "w");

	/* Scan the artifacts */
	for (k = 0; k < MAX_A_IDX; k++)
	{
		artifact_type *a_ptr = &a_info[k];

		/* Default */
		okay[k] = FALSE;

		/* Skip "empty" artifacts */
		if (!a_ptr->name) continue;

		/* Skip "uncreated" artifacts */
		if (!a_ptr->cur_num) continue;

		/* Assume okay */
		okay[k] = TRUE;
	}

	/* Check the dungeon */
	for (y = 0; y < cur_hgt; y++)
	{
		for (x = 0; x < cur_wid; x++)
		{
			cave_type *c_ptr = &cave[y][x];

			s16b this_o_idx, next_o_idx = 0;

			/* Scan all objects in the grid */
			for (this_o_idx = c_ptr->o_idx; this_o_idx; this_o_idx = next_o_idx)
			{
				object_type *o_ptr;
			
				/* Acquire object */
				o_ptr = &o_list[this_o_idx];

				/* Acquire next object */
				next_o_idx = o_ptr->next_o_idx;

				/* Ignore non-artifacts */
				if (!artifact_p(o_ptr)) continue;

				/* Ignore known items */
				if (object_known_p(o_ptr)) continue;

				/* Note the artifact */
				okay[o_ptr->name1] = FALSE;
			}
		}
	}

	/* Check the inventory and equipment */
	for (i = 0; i < INVEN_TOTAL; i++)
	{
		object_type *o_ptr = &inventory[i];

		/* Ignore non-objects */
		if (!o_ptr->k_idx) continue;

		/* Ignore non-artifacts */
		if (!artifact_p(o_ptr)) continue;

		/* Ignore known items */
		if (object_known_p(o_ptr)) continue;

		/* Note the artifact */
		okay[o_ptr->name1] = FALSE;
	}

	/* Scan the artifacts */
	for (k = 0; k < MAX_A_IDX; k++)
	{
		artifact_type *a_ptr = &a_info[k];

		/* List "dead" ones */
		if (!okay[k]) continue;

		/* Paranoia */
		strcpy(base_name, "Unknown Artifact");

		/* Obtain the base object type */
		z = lookup_kind(a_ptr->tval, a_ptr->sval);

		/* Real object */
		if (z)
		{
			object_type forge;
			object_type *q_ptr;

			/* Get local object */
			q_ptr = &forge;

			/* Create fake object */
			object_prep(q_ptr, z);

			/* Make it an artifact */
			q_ptr->name1 = k;

			/* Describe the artifact */
			object_desc_store(base_name, q_ptr, FALSE, 0);
		}

		/* Hack -- Build the artifact name */
		fprintf(fff, " %s   The %s\n",
			get_symbol((&k_info[lookup_kind(a_ptr->tval, a_ptr->sval)])), base_name);
	}

	/* Close the file */
	my_fclose(fff);

	/* Display the file contents */
	show_file(file_name, "Artifacts Seen");

	/* Remove the file */
	fd_kill(file_name);
}


/*
 * Display known uniques
 *
 * Note that the player ghosts are ignored.  XXX XXX XXX
 */
static void do_cmd_knowledge_uniques(void)
{
	int k;

	FILE *fff;

	char file_name[1024];

	cptr atchar="dwsorgbuDWvyRGBU";

	/* Temporary file */
	if (path_temp(file_name, 1024)) return;

	/* Open a new file */
	fff = my_fopen(file_name, "w");

	/* Scan the monster races */
	for (k = 1; k < MAX_R_IDX-1; k++)
	{
		monster_race *r_ptr = &r_info[k];

		/* Only print Uniques */
		if (r_ptr->flags1 & (RF1_UNIQUE))
		{
			bool dead = (r_ptr->max_num == 0);

			/* Only display "known" uniques */
			if (dead || spoil_mon || r_ptr->r_sights)
			{
				fprintf(fff, " %s %c [[[[[%c%s is %s]\n",
				 get_symbol(r_ptr), (r_ptr->flags1 & RF1_GUARDIAN) ? '!' : ' ',
				        (dead) ? atchar[TERM_L_DARK] : atchar[TERM_WHITE], (r_name + r_ptr->name),
				        (dead ? "dead" : "alive"));
			}
		}
	}

	/* Close the file */
	my_fclose(fff);

	/* Display the file contents */
	show_file(file_name, "Known Uniques");

	/* Remove the file */
	fd_kill(file_name);
}


static void plural_aux(char * Name)
	{
	cptr initstrings[] = {" of ", " to ", 0};
	cptr plurals[][2] = {
		{"young", "young"}, /* Dark young of Shub-Niggurath */
		{"Manes", "Manes"}, /* Manes */
		{"Ninja", "Ninja"}, /* Ninja */
		{"kelman", "kelmen"}, /* Pukelman */
		{"child", "children"}, /* Small child */
		{" Man", " Men"}, /* Wild Man */
		{"ex", "ices"}, /* X vortex */
		{"olf", "olves"}, /* X Wolf */
		{"thief", "thieves"}, /* X thief (NOT to be mistaken for chief) */
		{"ay", "ays"}, /* Stairway to hell */
		{"ouse", "ice"}, /* X mouse/louse */
		{"y", "ies"}, /* Harpy, Jelly, etc. */
		{"ch", "ches"}, /* mushroom patch, lich, etc. */
		{"s", "ses"}, /* moss, Colossus, etc. */
		{"", "s"}, /* everything else */
	};
	int NameLen = strlen(Name);
	byte i;
	if (NameLen == 0) return;
	for (i = 0; initstrings[i]; i++)
	{
		cptr aider = strstr(Name, initstrings[i]);
		if (aider > Name)
 		{
			char dummy[80] = "";
			strncpy(dummy, Name, aider - Name);
			plural_aux(dummy);
			strcat(dummy, aider);
		strcpy (Name, dummy);
		return;
	}
	        }
	for (i = 0; plurals[i]; i++)
		{
		if (streq(&(Name[NameLen-strlen(plurals[i][0])]), plurals[i][0]))
		{
			strcpy(&Name[NameLen-strlen(plurals[i][0])], plurals[i][1]);
		return;
	}
	}
	}

/*
 * Gives the long form of a name for do_cmd_knowledge_kill_count, etc..
 */
void full_name(char * Name, bool plural, bool article, bool strangearticle)
	{
	/* Hack - give "long" version of creeping X coins */
	if (strstr(Name, "coins"))
	{
		char string[strlen(Name)+9];
		sprintf(string, "pile of %s", Name);
		strcpy(Name, string);
	}
	if (plural)
	{
		plural_aux(Name);
		if (article)
	{
			char string[strlen(Name)+6];
			sprintf(string, "some %s", Name);
			strcpy(Name, string);
	}
	}
	/* Give an indefinite article as required. Plurals never take indefinite articles. */
	else if (article)
	{
		char string[strlen(Name)+4];
		/* Testing whether the word starts with a vowel or not is usually sufficient. */
		bool an = is_a_vowel(Name[0]);
		/* There needs to be a way of overriding this, however. */
		if (strangearticle) an = !an;
		/* Use the string we have decided upon. */
		sprintf(string, "%s %s", (an) ? "an" : "a", Name);
		strcpy(Name, string);
	}
}

/*
 * Display current pets
 *
 */
static void do_cmd_knowledge_pets(void)
{
	int i;

	FILE *fff;

	monster_type * m_ptr;

	int t_friends = 0;
	int t_levels = 0;
	int max_friends = 0;
	int show_upkeep = 0;
	int upkeep_divider = 20;

	char file_name[1024];


	/* Temporary file */
	if (path_temp(file_name, 1024)) return;

	/* Open a new file */
	fff = my_fopen(file_name, "w");

	/* Process the monsters (backwards) */
	for (i = m_max - 1; i >= 1; i--)
	{
		/* Access the monster */
		m_ptr = &m_list[i];

		/* Ignore "dead" monsters */
		if (!m_ptr->r_idx) continue;

		/* Calculate "upkeep" for friendly monsters */
		if (m_ptr->smart & (SM_ALLY))
		{
			char pet_name[80];
			monster_race *r_ptr = &r_info[m_ptr->r_idx];
			t_friends++;
			t_levels += r_ptr->level;
			monster_desc(pet_name, m_ptr, 0x88);
			strcat(pet_name, "\n");
			fprintf(fff,"%s %s\n", get_symbol(r_ptr), pet_name);
		}
	}

	max_friends = 1 + (skill_set[SKILL_RACIAL].value / (upkeep_divider*2));
	if (t_friends > max_friends)
	{
		show_upkeep = (t_levels);

		if (show_upkeep > 100) show_upkeep = 100;
		else if (show_upkeep < 10) show_upkeep = 10;
	}


	fprintf(fff,"----------------------------------------------\n");
	fprintf(fff,"              Total: %d all%s.\n", t_friends, (t_friends==1?"y":"ies"));
	fprintf(fff," Max without upkeep: %d.\n",max_friends);
	fprintf(fff,"             Upkeep: %d%% mana.\n", show_upkeep);


	/* Close the file */
	my_fclose(fff);

	/* Display the file contents */
	show_file(file_name, "Current Allies");

	/* Remove the file */
	fd_kill(file_name);
}



/*
 * Count the number of monsters killed. As we want to calculate the total before
 * displaying anything, we run this first without output, and then with.
 */
static int count_kills(FILE *fff, bool noisy)
{
	/* Monsters slain */
	int kk, Total = 0;

	for (kk = 1; kk < MAX_R_IDX-1; kk++)
	{
		monster_race *r_ptr = &r_info[kk];

		s16b This = r_ptr->r_pkills;

		Total += This;
		if (This && noisy)
		{
			char string[80];
			strcpy(string, r_name+r_ptr->name);
			full_name(string, This > 1, FALSE, FALSE);

			if (r_ptr->flags1 & (RF1_UNIQUE) && This == 1)
				fprintf(fff, " %s   %s\n", get_symbol(r_ptr), (r_name + r_ptr->name));
			else
				fprintf(fff, " %s   %d %s\n", get_symbol(r_ptr), This, string);
		}
	}
	return Total;
}

/*
 * Total kill count
 *
 * Note that the player ghosts are ignored.  XXX XXX XXX
 */
static void do_cmd_knowledge_kill_count(void)
{
	FILE *fff;

	char file_name[1024];

	s32b Total = 0;


	/* Temporary file */
	if (path_temp(file_name, 1024)) return;

	/* Open a new file */
	fff = my_fopen(file_name, "w");

	/* Count monsters slain */
	Total = count_kills(fff, FALSE);

		if (Total < 1)
			fprintf(fff,"You have defeated no enemies yet.\n\n");
		else if (Total == 1)
			fprintf(fff,"You have defeated one enemy.\n\n");
		else
		fprintf(fff,"You have defeated %ld enemies.\n\n", Total);

	/* Display the species-by-species breakdown. */
	(void)count_kills(fff, TRUE);

	fprintf(fff,"----------------------------------------------\n");
	fprintf(fff,"   Total: %lu creature%s killed.\n", Total, (Total==1?"":"s"));

	/* Close the file */
	my_fclose(fff);

	/* Display the file contents */
	show_file(file_name, "Kill Count");

	/* Remove the file */
	fd_kill(file_name);
}


/*
 * Sorting hook for do_cmd_knowledge_deaths().
 *
 * Sort from high number of kills to low, and then from low monster level to high.
 */
static bool ang_sort_comp_deaths(vptr u, vptr UNUSED v, int a, int b)
{
	s16b *races = (s16b*)u;
	bool dif = r_info[races[a]].r_deaths-r_info[races[b]].r_deaths;
	if (!dif) dif = r_info[races[b]].level-r_info[races[a]].level;
	return (dif >= 0);
}

/*
 * Swapping hook for do_cmd_knowledge_deaths()
 */
static void ang_sort_swap_deaths(vptr u, vptr UNUSED v, int a, int b)
{
	s16b *races = (s16b*)u;
	s16b c = races[a];
	races[a]=races[b];
	races[b]=c;
}
/*
 * Display those monsters who have directly killed players in the current
 * save file. Note that non-living causes (e.g. poison) aren't recorded.
 */
static void do_cmd_knowledge_deaths(void)
{
	FILE *fff;

	char file_name[1024];

	typedef struct death_type death_type;
	
	s16b races[MAX_R_IDX];

	s32b i, Uniques = 0, Races = 0, Deaths = 0;

	/* Temporary file */
	if (path_temp(file_name, 1024)) return;

	/* Open a new file */
	fff = my_fopen(file_name, "w");

	/* Count the monsters who have killed some ancestors. */
	for (i = 0; i < MAX_R_IDX; i++)
	{
		if (r_info[i].r_deaths) races[Races++] = i;
	}
	
	/* Are there any? */
	if (!Races)
	{
		fprintf(fff, "None of your ancestors have been killed directly by a monster here.");
	}
	else
	{
		/* Sort the list by numbers of deaths. */
		ang_sort_comp = ang_sort_comp_deaths;
		ang_sort_swap = ang_sort_swap_deaths;
		ang_sort(races, 0, Races);
		fprintf(fff, "The following monster%s have claimed some of your ancestors' deaths here:\n", (Races == 1) ? "" : "s");

		/* Display the sorted list. */
		for (i = 0; i < Races; i++)
		{
			char string[80];
			monster_race *r_ptr = &r_info[races[i]];
			bool plural = r_ptr->r_deaths > 1 && ~r_ptr->flags1 & RF1_UNIQUE;
			bool article = r_ptr->r_deaths == 1 && ~r_ptr->flags1 & RF1_UNIQUE;

			/* Grab the string. */
			strcpy(string, r_name+r_ptr->name);

			/* Don't leave a single capital letter in non-unique names as it looks odd. */
			if (~r_ptr->flags1 & RF1_UNIQUE) string[0] = FORCELOWER(string[0]);

			/* Pluralise or add an indefinite article as required. */
			full_name(string, plural, article, FALSE);

			/* Format the string, including the monster's ASCII representation. */
			fprintf(fff, " %s   %d w%s killed by %s\n", get_symbol(r_ptr), r_ptr->r_deaths, (r_ptr->r_deaths == 1) ? "as" : "ere", string);

			/* Count the total. */
			Deaths+=r_ptr->r_deaths;
			if (r_ptr->flags1 & RF1_UNIQUE) Uniques+=r_ptr->r_deaths;
		}

		/* Display a summary at the bottom. */
		fprintf(fff,"----------------------------------------------\n");
		fprintf(fff,"Total: Killed %lu times by %lu types of creature (%lu times by uniques).\n", Deaths, Races, Uniques);
	}

	/* Close the file */
	my_fclose(fff);

	/* Display the file contents */
	show_file(file_name, "Causes of death");

	/* Remove the file */
	fd_kill(file_name);
}

/*
 * Display known objects
 */
static void do_cmd_knowledge_objects(void)
{
	int k;

	FILE *fff;

	char o_name[ONAME_MAX];

	char file_name[1024];


	/* Temporary file */
	if (path_temp(file_name, 1024)) return;

	/* Open a new file */
	fff = my_fopen(file_name, "w");

	/* Scan the object kinds */
	for (k = 1; k < MAX_K_IDX; k++)
	{
		object_kind *k_ptr = &k_info[k];

		/* Hack -- skip artifacts */
		if (k_ptr->flags3 & (TR3_INSTA_ART)) continue;

		/* List known flavored objects */
		if (u_info[k_ptr->u_idx].s_id && k_ptr->aware)
		{
			object_type *i_ptr;
			object_type object_type_body;

			/* Get local object */
			i_ptr = &object_type_body;

			/* Create fake object */
			object_prep(i_ptr, k);

			/* Describe the object */
			object_desc_store(o_name, i_ptr, FALSE, 0);

			/* Print a message */
			fprintf(fff, " %s   %s\n", get_symbol_aux(object_attr(i_ptr), object_char(i_ptr)), o_name);
		}
	}

	/* Close the file */
	my_fclose(fff);

	/* Display the file contents */
    show_file(file_name, "Known Objects");

	/* Remove the file */
	fd_kill(file_name);
}

/*
 * List chaos features we have...
 *
 */
void do_cmd_knowledge_chaos_features(void)
{

	FILE *fff;

	char file_name[1024];


	/* Temporary file */
	if (path_temp(file_name, 1024)) return;

	/* Open a new file */
	fff = my_fopen(file_name, "w");

	if (fff) dump_chaos_features(fff);

	/* Close the file */
	my_fclose(fff);

	/* Display the file contents */
	show_file(file_name, "Chaos Features");

	/* Remove the file */
	fd_kill(file_name);
}


#define STORE_NONE 99

/*
 * Notice if a given town has any real shops.
 */
bool shops_good(int town)
{
	/* The current town has space allocated for shops. */
	if (town_defs[town].numstores)
	{
	 	town_type *t_ptr = &town_defs[town];
		int i;
 		for (i = 0; i < t_ptr->numstores; i++)
		{
			/* Found a real store */
			if (t_ptr->store[i] != STORE_NONE) return TRUE;
		}
	}
	return FALSE;
}

/*
 * Displays a list of the shops in a given town.
 */
void shops_display(int town)
{
	const int offset = MAX_STORES_PER_TOWN*town;
	int i,j;
	town_type *t_ptr = &town_defs[town];

	/* Paranoia (?) - no town */
	if (town >= MAX_TOWNS)
	{
		if (alert_failure) message_add("ERROR: No current town.");
		bell();
		return;
	}
	clear_from(0);
	Term_putstr(0,0, Term->wid, TERM_WHITE,
		format("List of shops in %s",
			dun_defs[town].shortname));

	for (i = 0, j = 1; i < t_ptr->numstores; i++)
	{
		feature_type *f_ptr;

		/* Only count real stores */
		if (t_ptr->store[i] == STORE_NONE) continue;
 
 		f_ptr = &f_info[FEAT_SHOP_HEAD+store[i+offset].type];
 
		/* Display the name of the store. */
		Term_putstr(0, ++j, Term->wid, TERM_WHITE, store_title(i+offset));

		/* Put the character used at the top. */
		Term_putch(0, j, f_ptr->x_attr, f_ptr->x_char);
	}
}	


/*
 * Allow the player to see which shops are in each town, what the maximum
 * price of each is and how greedy the shopkeepers are.
 *
 * Maybe it should work on the basis of the last thing the player knew, but
 * it seems like too much effort for little gain.
 */
static void do_cmd_knowledge_shops(void)
{
	/* I hope this isn't too silly... */
	int town = cur_town;
	
	/* Paranoia (?) - no town */
	if (town >= MAX_TOWNS) town = 0;
	for (;;)
	{
		char c;
		shops_display(town);
		prt("Press +,- or 0-7 to change displayed town, or ESC to exit.", 22, 0);
		Term_fresh();
		c = inkey();
		Term_putch(0,0,TERM_YELLOW,c);
		if (c == ESCAPE)
		{
			break;
		}
		else if (c == '+')
		{
			do
			{
				town=(town+1)%MAX_TOWNS;
				
			} while (!shops_good(town));
		}
		else if (c == '-')
		{
			do
			{
				town=(town+MAX_TOWNS-1)%MAX_TOWNS;
				
			} while (!shops_good(town));
		}
		else if (c >= '0' && c < '0'+MAX_TOWNS)
		{
			if (shops_good(c-'0'))
			{
				town = c-'0';
			}
		}
	}
}

/*
 * Interact with "knowledge"
 */
void do_cmd_knowledge(void)
{
	int i;


	/* File type is "TEXT" */
	FILE_TYPE(FILE_TYPE_TEXT);


	/* Enter "icky" mode */
	character_icky = TRUE;

	/* Save the screen */
	Term_save();


	/* Interact until done */
	while (1)
	{
		typedef struct knowledge_type knowledge_type;
		struct knowledge_type {
		cptr text;
		void (*func)(void);
		};
		knowledge_type knowledge[] = {
		{"known artifacts", do_cmd_knowledge_artifacts},
		{"knwon uniques", do_cmd_knowledge_uniques},
		{"known objects", do_cmd_knowledge_objects},
		{"kill count", do_cmd_knowledge_kill_count},
		{"ancestral causes of death", do_cmd_knowledge_deaths},
		{"chaos features", do_cmd_knowledge_chaos_features},
		{"current allies", do_cmd_knowledge_pets},
		{"shop prices", do_cmd_knowledge_shops},
		};
		byte max_knowledge = sizeof(knowledge)/sizeof(knowledge_type);
		/* Clear screen */
		Term_clear();

		/* Ask for a choice */
		prt("Display current knowledge", 2, 0);

		/* Give some choices */
		for (i = 0; i < max_knowledge; i++)
		{
			knowledge_type *kn_ptr = &knowledge[i];
			prt(format("(%d) Display %s", i+1, kn_ptr->text), 4+i, 5);
		}

		/* Prompt */
		prt("Command: ", 5+i, 0);

		/* Prompt */
		i = inkey();

		/* Done */
		if (i == ESCAPE) break;

		/* Known options (see above) */
		else if (i > '0' && i < '1'+max_knowledge)
		{
			knowledge_type *kn_ptr = &knowledge[i-'1'];
			(*(kn_ptr->func))();
		}

		/* Unknown option */
		else
		{
			bell();
		}

		/* Flush messages */
		msg_print(NULL);
	}


	/* Restore the screen */
	Term_load();

	/*
	 * For some reason (probably incompatability with 2.8.1) the above
	 * does not seem to work here... hence the following line
	 */
	do_cmd_redraw();

	/* Leave "icky" mode */
	character_icky = FALSE;
}

