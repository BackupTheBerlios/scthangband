#define FILES_C
/* File: files.c */

/* Purpose: code dealing with files (and death) */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"

static void signals_ignore_tstp(void);
static void signals_handle_tstp(void);


/*
 * You may or may not want to use the following "#undef".
 */
/* #undef _POSIX_SAVED_IDS */


/*
 * Hack -- drop permissions
 */
void safe_setuid_drop(void)
{

#ifdef SET_UID

# ifdef SAFE_SETUID

#  ifdef SAFE_SETUID_POSIX

	if (setuid(getuid()) != 0)
	{
		quit("setuid(): cannot set permissions correctly!");
	}
	if (setgid(getgid()) != 0)
	{
		quit("setgid(): cannot set permissions correctly!");
	}

#  else

	if (setreuid(geteuid(), getuid()) != 0)
	{
		quit("setreuid(): cannot set permissions correctly!");
	}
	if (setregid(getegid(), getgid()) != 0)
	{
		quit("setregid(): cannot set permissions correctly!");
	}

#  endif

# endif

#endif

}


/*
 * Hack -- grab permissions
 */
void safe_setuid_grab(void)
{

#ifdef SET_UID

# ifdef SAFE_SETUID

#  ifdef SAFE_SETUID_POSIX

	if (setuid(player_euid) != 0)
	{
		quit("setuid(): cannot set permissions correctly!");
	}
	if (setgid(player_egid) != 0)
	{
		quit("setgid(): cannot set permissions correctly!");
	}

#  else

	if (setreuid(geteuid(), getuid()) != 0)
	{
		quit("setreuid(): cannot set permissions correctly!");
	}
	if (setregid(getegid(), getgid()) != 0)
	{
		quit("setregid(): cannot set permissions correctly!");
	}

#  endif

# endif

#endif

}


/*
 * Extract the first few "tokens" from a buffer
 *
 * This function uses "colon" and "slash" as the delimeter characters.
 *
 * We never extract more than "num" tokens.  The "last" token may include
 * "delimeter" characters, allowing the buffer to include a "string" token.
 *
 * We save pointers to the tokens in "tokens", and return the number found.
 *
 * Hack -- Attempt to handle the 'c' character formalism
 *
 * Hack -- An empty buffer, or a final delimeter, yields an "empty" token.
 *
 * Hack -- We will always extract at least one token
 */


static s16b tokenize(char *buf, s16b num, char **tokens)
{
	int i = 0;

	char *s = buf;


	/* Process */
	while (i < num - 1)
	{
		char *t;

		/* Scan the string */
		for (t = s; *t; t++)
		{
			/* Found a delimiter */
			if ((*t == ':') || (*t == '/')) break;

			/* Handle single quotes */
			if (*t == '\'')
			{
				/* Advance */
				t++;

				/* Handle backslash */
				if (*t == '\\') t++;

				/* Require a character */
				if (!*t) break;

				/* Advance */
				t++;

				/* Hack -- Require a close quote */
				if (*t != '\'') *t = '\'';
			}

			/* Handle back-slash */
			if (*t == '\\') t++;
		}

		/* Nothing left */
		if (!*t) break;

		/* Nuke and advance */
		*t++ = '\0';

		/* Save the token */
		tokens[i++] = s;

		/* Advance */
		s = t;
	}

	/* Save the token */
	tokens[i++] = s;

	/* Number found */
	return (i);
}



/*
 * Insert a set of stats into the stat_default array.
 * Returns NULL on success, and an error string on failure.
 */
cptr add_stats(s16b sex, s16b race, s16b template, bool maximise, s16b st, s16b in, s16b wi, s16b dx, s16b co, s16b ch, cptr name)
{
	stat_default_type *sd_ptr;
	byte i;
	s16b stat[A_MAX];

	/* Don't even try after character generation. */
	if (!stat_default) return SUCCESS;

	stat[A_STR] = st;
	stat[A_INT] = in;
	stat[A_WIS] = wi;
	stat[A_DEX] = dx;
	stat[A_CON] = co;
	stat[A_CHR] = ch;

	/* Ignore nonsense values */
	if (maximise != DEFAULT_STATS)
	{
		if (sex < 0 || sex >= MAX_SEXES) return "no such sex";
		if (race < 0 || race >= MAX_RACES) return "no such race";
		if (template < 0 || template >= MAX_TEMPLATE) return "no such template";
		if (maximise != TRUE && maximise != FALSE) return "bad maximuse value";
		for (i = 0; i < A_MAX; i++)
		{
			if (maximise)
			{
				/* Stats above 17 are not allowed. */
				if (stat[i] > 17) return "bad stat value";

				/* Stats below 8 with external values below 3 are not allowed. */
				if (stat[i] < 8 && stat[i]+race_info[race].r_adj[i] +
					template_info[template].c_adj[i] < 3)
					return "bad stat value";
			}
			else
			{
				/* Stats above their maxima are not allowed. */
				if (stat[i] > maxstat(race, template, i))
					return "bad stat value";

				/* Stats below 3 are not allowed. */
				if (stat[i] < 3) return "bad stat value";
			}
		}
	}

	/* We should always get a name, but just in case... */
	if (!strlen(name)) name = "Unknown Soldier";

	/* The array should not fill up. */
	if (stat_default_total == MAX_STAT_DEFAULT)
	{
		/* Too many defaults, so give an error in the absence of a better idea. */
		return "too many stat values";
	}

	sd_ptr = &stat_default[stat_default_total];
	
	sd_ptr->sex = sex;
	sd_ptr->race = race;
	sd_ptr->template = template;
	sd_ptr->maximise = maximise;

	for (i = 0; i< A_MAX; i++)
	{
		sd_ptr->stat[i] = stat[i];
	}
	sd_ptr->name = quark_add(name);

	/* Require space for the name. */
	if (!sd_ptr->name) return "too many strings";

	/* Look for duplicates.
	 * These are common as saving creates a new copy of everything.
	 * We don't check the name because this may be randomly generated,
	 * and has no effect anyway.
	 */
	for (i = 0; i < stat_default_total; i++)
	{
		stat_default_type *sd2_ptr = &stat_default[i];
		if (sd_ptr->race != sd2_ptr->race) continue;
		if (sd_ptr->template != sd2_ptr->template) continue;
		if (sd_ptr->maximise != sd2_ptr->maximise) continue;
		if (sd_ptr->stat[0] != sd2_ptr->stat[0]) continue;
		if (sd_ptr->stat[1] != sd2_ptr->stat[1]) continue;
		if (sd_ptr->stat[2] != sd2_ptr->stat[2]) continue;
		if (sd_ptr->stat[3] != sd2_ptr->stat[3]) continue;
		if (sd_ptr->stat[4] != sd2_ptr->stat[4]) continue;
		if (sd_ptr->stat[5] != sd2_ptr->stat[5]) continue;

		/* Take the last name given to a stat set. */
		sd2_ptr->name = sd_ptr->name;
		break;
	}

	/* No duplicates, so advance the index */
	if (i == stat_default_total) stat_default_total++;
	return NULL;
}

/*
 * A macro to cause this particular set of maps to be reset to default
 * if required.
 */
#define reset_check(x_info, max) \
	if (!strcmp(buf+2, "---reset---")) \
	{ \
		for (i = 0; i < max; i++) \
		{ \
			x_info[i].x_attr = x_info[i].d_attr; \
			x_info[i].x_char = x_info[i].d_char; \
		} \
		return 0; \
	} \

/*
 * Parse a sub-file of the "extra info" (format shown below)
 *
 * Each "action" line has an "action symbol" in the first column,
 * followed by a colon, followed by some command specific info,
 * usually in the form of "tokens" separated by colons or slashes.
 *
 * Blank lines, lines starting with white space, and lines starting
 * with pound signs ("#") are ignored (as comments).
 *
 * Note the use of "tokenize()" to allow the use of both colons and
 * slashes as delimeters, while still allowing final tokens which
 * may contain any characters including "delimiters".
 *
 * Note the use of "strtol()" to allow all "integers" to be encoded
 * in decimal, hexidecimal, or octal form.
 *
 * Note that "monster zero" is used for the "player" attr/char, "object
 * zero" will be used for the "stack" attr/char, and "feature zero" is
 * used for the "nothing" attr/char.
 *
 * Parse another file recursively, see below for details
 *   %:<filename>
 *
 * Specify the save file version against which the pref file was created.
 *   O:<num>
 * (This is only currently used for object attr/chars.)
 *
 * Specify the attr/char values for "monsters" by race index
 *   R:<num>:<a>:<c>
 *
 * Specify the attr/char values for "objects" by kind index
 *   K:<num>:<a>:<c>
 *
 * Specify the attr/char values for "features" by feature index
 *   F:<num>:<a>:<c>
 *
 * Specify the attr/char values for unaware "objects" by indices
 *   U:<p_id>:<s_id>:<a>:<c>
 *
 * Specify the attr/char values for inventory "objects" by kind tval
 *   E:<tv>:<a>:<c>
 *
 * Define a macro action, given an encoded macro action
 *   A:<str>
 *
 * Create a normal macro, given an encoded macro trigger
 *   P:<str>
 *
 * Create a command macro, given an encoded macro trigger
 *   C:<str>
 *
 * Create a keyset mapping
 *   S:<key>:<key>:<dir>
 *
 * Turn an option off, given its name
 *   X:<str>
 *
 * Turn an option on, given its name
 *   Y:<str>
 *
 * Set the priority for a particular display and a given window
 *   W:<term name>:<display name>:<triggered>:<untriggered>
 *
 * Specify visual information, given an index, and some data
 *   V:<num>:<kv>:<rv>:<gv>:<bv>
 *
 * Specify a default set of initial statistics for spend_points
 *   D:<sex>:<race>:<class>:<maximise_mode>:<Str>:<Int>:<Wis>:<Dex>:<Con>:<Chr>:<Name>
 *
 * Specify the screen location of a redraw_stuff() display.
 *   L:<name>:<x>:<y>
 *
 * Returns NULL on success, an error message on failure.
 */
cptr process_pref_file_aux(char *buf, u16b *sf_flags)
{
	int i, j, n1, n2;

	char *zz[16];

	/* Skip "empty" lines */
	if (!buf[0]) return (0);

	/* Skip "blank" lines */
	if (isspace(buf[0])) return (0);

	/* Skip comments */
	if (buf[0] == '#') return (0);

	/* Require "?:*" format */
	if (buf[1] != ':') return "missing colon";


	/* Process "%:<fname>" */
	switch (buf[0])
	{
		/* Process %:<filename> -- process a given file. */
		case '%': 
		{
			/* Attempt to Process the given file */
			process_pref_file(buf + 2);
		}
		/* Process O:<version> -- save file version for this file. */
		case 'O': 
		{
			j = tokenize(buf+2, MAX_SF_VAR, zz);
			if (!j) return "format not O:<ver>:<ver>:...";
			for (i = 0; i < j; i++)
			{
				sf_flags[i] = strtol(zz[i], NULL, 0);
			}
			return 0;
		}
		/* Process "R:<num>:<a>/<c>" -- attr/char for monster races */
		case 'R':
		{
			reset_check(r_info, MAX_R_IDX)
			if (tokenize(buf+2, 3, zz) == 3)
			{
				monster_race *r_ptr;
				i = (huge)strtol(zz[0], NULL, 0);
				if (i < 0 || i > MAX_SHORT) return "no such monster";
				i = convert_r_idx(i, sf_flags, sf_flags_now);
				n1 = strtol(zz[1], NULL, 0);
				n2 = strtol(zz[2], NULL, 0);
				if (i >= MAX_R_IDX) return "no such monster";
				r_ptr = &r_info[i];
				if (n1) r_ptr->x_attr = n1;
				if (n2) r_ptr->x_char = n2;
				return (0);
			}
			else return "format not R:<num>:<a>/<c>";
		}
		/* Process "K:<num>:<a>/<c>"  -- attr/char for object kinds */
		case 'K':
		{
			reset_check(k_info, MAX_K_IDX)
			if (tokenize(buf+2, 3, zz) == 3)
			{
				object_kind *k_ptr;
				i = (huge)strtol(zz[0], NULL, 0);
				if (i < 0 || i > MAX_SHORT) return "no such object";
				i = convert_k_idx(i, sf_flags, sf_flags_now);
				n1 = strtol(zz[1], NULL, 0);
				n2 = strtol(zz[2], NULL, 0);
				if (i >= MAX_K_IDX) return "no such object";
				k_ptr = &k_info[i];
				if (n1) k_ptr->x_attr = n1;
				if (n2) k_ptr->x_char = n2;
				return (0);
			}
			else return "format not K:<num>:<a>/<c>";
		}
		/* 
		 * Simply extract the tokens and pass to squelch.c, as no action is
		 * appropriate without detailed knowledge.
		 */
		case 'Q':
		{
			int i = tokenize(buf+2, 16, zz);
			return process_pref_squelch(zz, i, sf_flags);
		}
		/* Process "U:<p_id>:<s_id>:<a>/<c>"  -- attr/char for unidentified objects */
		case 'U':
		{
			reset_check(u_info, MAX_U_IDX)
			if (tokenize(buf+2, 4, zz) == 4)
			{
				unident_type *u_ptr;
				i = (huge)strtol(zz[0], NULL, 0);
				j = (huge)strtol(zz[1], NULL, 0);
				n1 = strtol(zz[2], NULL, 0);
				n2 = strtol(zz[3], NULL, 0);
				if (i < 0 || i > 255 || j < 0 || j > 255)
					return "no such unidentified object";

				i = lookup_unident(i,j);
				if (i < 0 || i >= MAX_U_IDX)
					return "no such unidentified object";
				u_ptr = &u_info[i];
				if (n1) u_ptr->x_attr = n1;
				if (n2) u_ptr->x_char = n2;
				return (0);
			}
			else return "format not U:<p_id>:<s_id>:<a>/<c>";
		}

		/* Process "F:<num>:<a>/<c>" -- attr/char for terrain features */
		case 'F':
		{
			reset_check(f_info, MAX_F_IDX)
			if (tokenize(buf+2, 3, zz) == 3)
			{
				feature_type *f_ptr;
				i = (huge)strtol(zz[0], NULL, 0);
				n1 = strtol(zz[1], NULL, 0);
				n2 = strtol(zz[2], NULL, 0);
				if (i >= MAX_F_IDX) return "no such feature";
				f_ptr = &f_info[i];
				if (n1) f_ptr->x_attr = n1;
				if (n2) f_ptr->x_char = n2;
				return (0);
			}
			else return "format not F:<num>:<a>/<c>";
		}


		/* Process "E:<tv>:<a>/<c>" -- attr/char for equippy chars */
		case 'E':
		{
			if (tokenize(buf+2, 3, zz) == 3)
			{
				j = (byte)strtol(zz[0], NULL, 0) % 128;
				n1 = strtol(zz[1], NULL, 0);
				n2 = strtol(zz[2], NULL, 0);
				if (n1) tval_to_attr[j] = n1;
				if (n2) tval_to_char[j] = n2;
				return (0);
			}
			else return "format not E:<tv>:<a>/<c>";
		}


		/* Process "A:<str>" -- save an "action" for later */
		case 'A':
		{
			strnfmt(macro__buf, 1024, "%v", text_to_ascii_f1, buf+2);
			return (0);
		}

		/* Process "P:<str>" -- normal macro */
		case 'P':
		{
			macro_add(format("%v", text_to_ascii_f1, buf+2), macro__buf);
			return (0);
		}


		/* Process "C:<str>" -- create keymap */
		case 'C':
		{
			int mode;
	
			/* Hack - handle ---reset--- as a special case */
			if (!strcmp(buf+2, "---reset---"))
			{
				for (i = 0; i < MAX_UCHAR; i++)
				{
					for (mode = 0; mode < KEYMAP_MODES; mode++)
					{
						FREE(keymap_act[mode][i]);
					}
				}
				C_WIPE(keymap_act, (MAX_UCHAR+1)*KEYMAP_MODES, cptr);
				return SUCCESS;
			}
	
			if (tokenize(buf+2, 2, zz) != 2) return "format not C:<mode>:<key>";
	
			mode = strtol(zz[0], NULL, 0);
			if ((mode < 0) || (mode >= KEYMAP_MODES))
				return "no such keymap mode";
	
			if (strlen(format("%v", text_to_ascii_f1, zz[1])) != 1)
				return "no such key";
			i = (byte)(format(0)[0]);
	
			FREE(keymap_act[mode][i]);
	
			keymap_act[mode][i] = string_make(macro__buf);
	
			return (0);
		}
	
	
		/* Process "V:<num>:<kv>:<rv>:<gv>:<bv>" -- visual info */
		case 'V':
		{
			if (tokenize(buf+2, 5, zz) == 5)
			{
				i = (byte)strtol(zz[0], NULL, 0);
				angband_color_table[i][0] = (byte)strtol(zz[1], NULL, 0);
				angband_color_table[i][1] = (byte)strtol(zz[2], NULL, 0);
				angband_color_table[i][2] = (byte)strtol(zz[3], NULL, 0);
				angband_color_table[i][3] = (byte)strtol(zz[4], NULL, 0);
				return (0);
			}
			else return "format not V:<num>:<kv>:<rv>:<gv>:<bv>";
		}

		/* Process M:<attr>:<attr>... -- set monster memory colours */
		case 'M':
		{
			/* Expect M:xx:xx:xx:xx (MAX_MONCOL times) format. */
			if (strlen(buf) < MAX_MONCOL*3+1)
				return "format not M:<attr>:<attr>:...";
			for (i = 0; i < MAX_MONCOL; i++)
			{
				moncol_type *mc_ptr = &moncol[i];
				char c1 = buf[2+i*3];
				char c2 = buf[3+i*3];
				/* Read the second character first. */
				if (strchr(atchar, c2))
					mc_ptr->attr = strchr(atchar, c2)-atchar;
				else
					return "unidentified colour";
				/* Then read the first character, if present. */
				if (strchr(atchar, c1))
					mc_ptr->attr += 16*(strchr(atchar, c1)-atchar);
				else if (c1 != ' ')
					return "unidentified colour";
			}
			return 0;
		}
		
		/* Process "Y:<str>"/"X:<str>" -- turn option on/off */
		case 'Y': case 'X':
		{
			const option_type *op_ptr;
			for (op_ptr = option_info; op_ptr->o_desc; op_ptr++)
			{
				if (op_ptr->o_var && op_ptr->o_text &&
				    !strcmp(op_ptr->o_text, buf + 2))
				{
					/* Set the option. */
					(*op_ptr->o_var) = (buf[0] == 'Y');

					/* Do whatever should be done for it. */
					opt_special_effect(op_ptr);

					return SUCCESS;
				}
			}
			/* Not a real option. */
			return "no such option";
		}
		/* Process W:<term name>:<display name>:<triggered>:<untriggered>
		 * to a window flag.
		 */
		case 'W':
		{
			cptr goodpri = ".abcdefghij.0123456789";
			uint display, pri, rep;
			window_type *w_ptr;

			if (!strcmp(buf+2, "---reset---"))
			{
				for (display = 0; display < NUM_DISPLAY_FUNCS; display++)
				{
					for (w_ptr = windows; w_ptr < END_PTR(windows); w_ptr++)
					{
						w_ptr->rep[display] = 0;
						w_ptr->pri[display] = 0;
					}
				}
				return SUCCESS;
			}


			if (tokenize(buf+2, 4, zz) != 4)
				return "format not W:<window>:<display>:<triggered>:<untriggered>";

			/* Identify the term and display strings. */
			for (w_ptr = windows;; w_ptr++)
			{
				if (w_ptr == END_PTR(windows))
					return "no such window";
				if (!strcmp(w_ptr->name, zz[0])) break;
			}
			for (display = 0;; display++)
			{
				if (display == NUM_DISPLAY_FUNCS)
					return "no such display";
				if (!strcmp(display_func[display].name, zz[1])) break;
			}

			/* Identify the triggered and untriggered numbers. */
			if ((!strchr(goodpri, zz[2][0]) || zz[2][1]) ||
				!strchr(goodpri, zz[3][0]) || zz[3][1])
				return "no such priority";
			rep = (strchr(goodpri, zz[2][0])-goodpri)%11;
			pri = (strchr(goodpri, zz[3][0])-goodpri)%11;

			/* Enact the requests and return. */
			w_ptr->rep[display] = rep;
			w_ptr->pri[display] = pri;
			return SUCCESS;
		}
		/*
		 * Process D:<sex>:<race>:<class>:<maximise_mode>:<Str>:<Int>:<Wis>:<Dex>:<Con>:<Chr>:<Name> for initial stats 
		 */
		case 'D':
		{
			byte total = tokenize(buf+2, 11, zz);
			cptr err;
			/* We can accept D with or without a name, but everything else must be there. */
			switch (total)
			{
				/* No name given, so make one up. */
				case 10:
				{
					char name[32];
					create_random_name(ator(*zz[1]), name);
					err = add_stats(ator(*zz[0]), ator(*zz[1]),
					 ator(*zz[2]), strtol(zz[3], NULL, 0) != 0,
					 strtol(zz[4], NULL, 0), strtol(zz[5], NULL, 0),
					 strtol(zz[6], NULL, 0), strtol(zz[7], NULL, 0),
					 strtol(zz[8], NULL, 0), strtol(zz[9], NULL, 0), name);
					break;
				}
				case 11:
				{
					err = add_stats(ator(*zz[0]), ator(*zz[1]),
					 ator(*zz[2]), strtol(zz[3], NULL, 0) != 0,
					 strtol(zz[4], NULL, 0), strtol(zz[5], NULL, 0),
					 strtol(zz[6], NULL, 0), strtol(zz[7], NULL, 0),
					 strtol(zz[8], NULL, 0), strtol(zz[9], NULL, 0), zz[10]);
					break;
				}
				default:
				return "format not D:<sex>:<race>:<class>:<maximise>:<str>:<int>:<wis>:<dex>:<con>:<chr>:<name>";
			}
			return err;
		}
		case 'L':
		{
			if (tokenize(buf+2, 3, zz) == 3)
			{
				int x,y;
				co_ord *co_ptr;
				for (co_ptr = screen_coords; co_ptr < END_PTR(screen_coords);
					co_ptr++)
				{
					if (!strcmp(co_ptr->name, zz[0])) goto L_okay;
				}
				return "no such display";
L_okay:

				if (!isdigit(zz[1][0]) || !isdigit(zz[2][0]))
					return "non-numerical co-ordinate";

				x = atoi(zz[1]);
				y = atoi(zz[2]);

				if (x < -255 || x > 255 || y < -255 || y > 255)
					return "co-ordinates must be between -255 and 255";

				co_ptr->x = x;
				co_ptr->y = y;

				return SUCCESS;
			}
			else
			{
				return "format not L:<display>:<x>:<y>";
			}
		}
		default:
		{
			/* Failure */
			return "no such preference category";
		}
	}
}


/*
 * Helper function for "process_pref_file()"
 *
 * Input:
 *   v: output buffer array
 *   f: final character
 *
 * Output:
 *   result
 */
static cptr process_pref_file_expr(char **sp, char *fp)
{
	cptr v;

	char *b;
	char *s;

	char b1 = '[';
	char b2 = ']';

	char f = ' ';

	/* Initial */
	s = (*sp);

	/* Skip spaces */
	while (isspace(*s)) s++;

	/* Save start */
	b = s;

	/* Default */
	v = "?o?o?";

	/* Analyze */
	if (*s == b1)
	{
		const char *p;
		const char *t;

		/* Skip b1 */
		s++;

		/* First */
		t = process_pref_file_expr(&s, &f);

		/* Oops */
		if (!*t)
		{
			/* Nothing */
		}

		/* Function: IOR */
		else if (streq(t, "IOR"))
		{
			v = "0";
			while (*s && (f != b2))
			{
				t = process_pref_file_expr(&s, &f);
				if (*t && !streq(t, "0")) v = "1";
			}
		}

		/* Function: AND */
		else if (streq(t, "AND"))
		{
			v = "1";
			while (*s && (f != b2))
			{
				t = process_pref_file_expr(&s, &f);
				if (*t && streq(t, "0")) v = "0";
			}
		}

		/* Function: NOT */
		else if (streq(t, "NOT"))
		{
			v = "1";
			while (*s && (f != b2))
			{
				t = process_pref_file_expr(&s, &f);
				if (*t && !streq(t, "0")) v = "0";
			}
		}

		/* Function: EQU */
		else if (streq(t, "EQU"))
		{
			v = "1";
			if (*s && (f != b2))
			{
				t = process_pref_file_expr(&s, &f);
			}
			while (*s && (f != b2))
			{
				p = t;
				t = process_pref_file_expr(&s, &f);
				if (*t && !streq(p, t)) v = "0";
			}
		}

		/* Function: LEQ */
		else if (streq(t, "LEQ"))
		{
			v = "1";
			if (*s && (f != b2))
			{
				t = process_pref_file_expr(&s, &f);
			}
			while (*s && (f != b2))
			{
				p = t;
				t = process_pref_file_expr(&s, &f);
				if (*t && (strcmp(p, t) >= 0)) v = "0";
			}
		}

		/* Function: GEQ */
		else if (streq(t, "GEQ"))
		{
			v = "1";
			if (*s && (f != b2))
			{
				t = process_pref_file_expr(&s, &f);
			}
			while (*s && (f != b2))
			{
				p = t;
				t = process_pref_file_expr(&s, &f);
				if (*t && (strcmp(p, t) <= 0)) v = "0";
			}
		}

		/* Oops */
		else
		{
			while (*s && (f != b2))
			{
				t = process_pref_file_expr(&s, &f);
			}
		}

		/* Verify ending */
		if (f != b2) v = "?x?x?";

		/* Extract final and Terminate */
		if ((f = *s) != '\0') *s++ = '\0';
	}

	/* Other */
	else
	{
		/* Accept all printables except spaces and brackets */
		while (isprint(*s) && !strchr(" []", *s)) ++s;

		/* Extract final and Terminate */
		if ((f = *s) != '\0') *s++ = '\0';

		/* Variable */
		if (*b == '$')
		{
			/* System */
			if (streq(b+1, "SYS"))
			{
				v = ANGBAND_SYS;
			}

			/* Race */
			else if (streq(b+1, "RACE") && rp_ptr)
			{
				v = rp_ptr->title;
			}

			/* Template */
			else if (streq(b+1, "TEMPLATE") && cp_ptr)
			{
				v = cp_ptr->title;
			}

			/* Player */
			else if (streq(b+1, "PLAYER"))
			{
				v = player_base;
			}
			else
			{
				v = "0";
			}
		}

		/* Constant */
		else
		{
			v = b;
		}
	}

	/* Save */
	(*fp) = f;

	/* Save */
	(*sp) = s;

	/* Result */
	return (v);
}

/*
 * Helper function for process_pref_file().
 */
static bool process_pref_file_expr_p(char *str)
{
	char c;
	return strcmp(process_pref_file_expr(&str, &c), "0");
}

/*
 * Process the "user pref file" with the given name
 *
 * See the function above for a list of legal "commands".
 *
 * We also accept the special "?" and "%" directives, which
 * allow conditional evaluation and filename inclusion.
 */
errr process_pref_file(cptr name)
{
	FILE *fp;

	char buf[1024];

	int num;

	cptr err = 0;

	bool bypass = FALSE;

	/* Use the oldest version as a default. */
	u16b sf_flags[MAX_SF_VAR];
	WIPE(sf_flags, sf_flags);

	/* Look in ANGBAND_DIR_PREF. */
	if (!((fp = my_fopen_path(ANGBAND_DIR_PREF, name, "r"))) &&

	/* Look in ANGBAND_DIR_USER. */
		!((fp = my_fopen_path(ANGBAND_DIR_USER, name, "r"))))

	/* No such file */
		return FILE_ERROR_CANNOT_OPEN_FILE;


	/* Process the file */
	for (num = 0; !my_fgets(fp, buf, 1024); num++)
	{
		char buf2[1024];

		/* Skip "empty" lines */
		if (!buf[0]) continue;

		/* Skip "blank" lines */
		if (isspace(buf[0])) continue;

		/* Skip comments */
		if (buf[0] == '#') continue;


		/* Process "?:<expr>" */
		if ((buf[0] == '?') && (buf[1] == ':'))
		{
			/* Parse the expr */
			bypass = !process_pref_file_expr_p(buf+2);

			/* Continue */
			continue;
		}

		/* Apply conditionals */
		if (bypass) continue;


		/* Process "%:<file>" */
		if (buf[0] == '%')
		{
			/* Process that file if allowed */
			process_pref_file(buf + 2);

			/* Continue */
			continue;
		}

		/* Make a copy to make any error message clearer. */
		strcpy(buf2, buf);

		/* Process the line */
		err = process_pref_file_aux(buf2, sf_flags);

		/* Oops */
		if (err) break;
	}


	/* Error */
	if (err)
	{
		/* Useful error message */
		msg_format("Error \"%s\" in line %d of file '%s'.", err, num, name);
		msg_format("Parsing '%s'", buf);
	}

	/* Close the file */
	my_fclose(fp);

	/* Result */
	return (err) ? PARSE_ERROR_GENERIC : SUCCESS;
}







#ifdef CHECK_TIME

/*
 * Operating hours for ANGBAND (defaults to non-work hours)
 */
static char days[7][29] =
{
	"SUN:XXXXXXXXXXXXXXXXXXXXXXXX",
	"MON:XXXXXXXX.........XXXXXXX",
	"TUE:XXXXXXXX.........XXXXXXX",
	"WED:XXXXXXXX.........XXXXXXX",
	"THU:XXXXXXXX.........XXXXXXX",
	"FRI:XXXXXXXX.........XXXXXXX",
	"SAT:XXXXXXXXXXXXXXXXXXXXXXXX"
};

/*
 * Restict usage (defaults to no restrictions)
 */
static bool check_time_flag = FALSE;

#endif


/*
 * Handle CHECK_TIME
 */
errr check_time(void)
{

#ifdef CHECK_TIME

	time_t              c;
	struct tm		*tp;

	/* No restrictions */
	if (!check_time_flag) return (0);

	/* Check for time violation */
	c = time((time_t *)0);
	tp = localtime(&c);

	/* Violation */
	if (days[tp->tm_wday][tp->tm_hour + 4] != 'X') return (1);

#endif

	/* Success */
	return (0);
}



/*
 * Initialize CHECK_TIME
 */
errr check_time_init(void)
{

#ifdef CHECK_TIME

	FILE        *fp;

	char	buf[1024];


	/* Open the file */
	fp = my_fopen_path(ANGBAND_DIR_FILE, "time.txt", "r");

	/* No file, no restrictions */
	if (!fp) return (0);

	/* Assume restrictions */
	check_time_flag = TRUE;

	/* Parse the file */
	while (0 == my_fgets(fp, buf, 80))
	{
		/* Skip comments and blank lines */
		if (!buf[0] || (buf[0] == '#')) continue;

		/* Chop the buffer */
		buf[29] = '\0';

		/* Extract the info */
		if (prefix(buf, "SUN:")) strcpy(days[0], buf);
		if (prefix(buf, "MON:")) strcpy(days[1], buf);
		if (prefix(buf, "TUE:")) strcpy(days[2], buf);
		if (prefix(buf, "WED:")) strcpy(days[3], buf);
		if (prefix(buf, "THU:")) strcpy(days[4], buf);
		if (prefix(buf, "FRI:")) strcpy(days[5], buf);
		if (prefix(buf, "SAT:")) strcpy(days[6], buf);
	}

	/* Close it */
	my_fclose(fp);

#endif

	/* Success */
	return (0);
}



#ifdef CHECK_LOAD

#ifndef MAXHOSTNAMELEN
# define MAXHOSTNAMELEN  64
#endif

typedef struct statstime statstime;

struct statstime
{
	int                 cp_time[4];
	int                 dk_xfer[4];
	unsigned int        v_pgpgin;
	unsigned int        v_pgpgout;
	unsigned int        v_pswpin;
	unsigned int        v_pswpout;
	unsigned int        v_intr;
	int                 if_ipackets;
	int                 if_ierrors;
	int                 if_opackets;
	int                 if_oerrors;
	int                 if_collisions;
	unsigned int        v_swtch;
	long                avenrun[3];
	struct timeval      boottime;
	struct timeval      curtime;
};

/*
 * Maximal load (if any).
 */
static int check_load_value = 0;

#endif


/*
 * Handle CHECK_LOAD
 */
errr check_load(void)
{

#ifdef CHECK_LOAD

	struct statstime    st;

	/* Success if not checking */
	if (!check_load_value) return (0);

	/* Check the load */
	if (0 == rstat("localhost", &st))
	{
		long val1 = (long)(st.avenrun[2]);
		long val2 = (long)(check_load_value) * FSCALE;

		/* Check for violation */
		if (val1 >= val2) return (1);
	}

#endif

	/* Success */
	return (0);
}


/*
 * Initialize CHECK_LOAD
 */
errr check_load_init(void)
{

#ifdef CHECK_LOAD

	FILE        *fp;

	char	buf[1024];

	char	temphost[MAXHOSTNAMELEN+1];
	char	thishost[MAXHOSTNAMELEN+1];


	/* Open the "load" file */
	fp = my_fopen_path(ANGBAND_DIR_FILE, "load.txt", "r");

	/* No file, no restrictions */
	if (!fp) return (0);

	/* Default load */
	check_load_value = 100;

	/* Get the host name */
	(void)gethostname(thishost, (sizeof thishost) - 1);

	/* Parse it */
	while (0 == my_fgets(fp, buf, 1024))
	{
		int value;

		/* Skip comments and blank lines */
		if (!buf[0] || (buf[0] == '#')) continue;

		/* Parse, or ignore */
		if (sscanf(buf, "%s%d", temphost, &value) != 2) continue;

		/* Skip other hosts */
		if (!streq(temphost, thishost) &&
		    !streq(temphost, "localhost")) continue;

		/* Use that value */
		check_load_value = value;

		/* Done */
		break;
	}

	/* Close the file */
	my_fclose(fp);

#endif

	/* Success */
	return (0);
}




/*
 * Print long number with header at given row, column
 * Use the color for the number, not the header
 */
static void prt_lnum(cptr str, s32b num, int row, int col, byte color)
{
	int len = strlen(str);
	char out_val[32];
	put_str(str, row, col);
	(void)sprintf(out_val, "%9ld", (long)num);
	c_put_str(color, out_val, row, col + len);
}

/*
 * Print number with header at given row, column
 */
static void prt_num(cptr str, int num, int row, int col, byte color)
{
	int len = strlen(str);
	char out_val[32];
	put_str(str, row, col);
	put_str("   ", row, col + len);
	(void)sprintf(out_val, "%6ld", (long)num);
	c_put_str(color, out_val, row, col + len + 3);
}


/*
 * Determine the colour of a display based on the proportion of maximum.
 */
static byte percent_to_colour(s16b cur, s16b max)
{
	if (cur == max) return TERM_L_GREEN;
  	if (cur > (max * hitpoint_warn) / 10) return TERM_YELLOW;
	return TERM_RED;
}

/*
 * Print out a ratio (as for hit points).
 */
void prt_nums(cptr txt, int y, int minx, int maxx, int cur, int max)
{
	cptr temp = format(" %d/%d", cur, max);
	byte attr = percent_to_colour(cur, max);
	uint len = maxx+1-minx;
	
	/* Paranoia */
	if (maxx < minx) return;

	/* Clear a space. */
	Term_erase(minx, y, len);

	/* Print the text string in white. */
	if (txt) put_str(txt, y, minx);

	/* Trim initial numbers if the number string would be too long. */
	if (strlen(temp) > len) temp += strlen(temp)-len;

	/* Print the number string in colour. */
	c_put_str(attr, temp, y, maxx-strlen(temp));
}

static void choose_ammunition(object_ctype *wp_ptr, object_type **am_ptr)
{
	object_type *o_ptr;
	int i;
	byte tval = ammunition_type(wp_ptr);

	/* Use missiles from inventory if appropriate. */
	for (o_ptr = inventory; o_ptr <= inventory+INVEN_PACK; o_ptr++)
	{
		if (tval && (o_ptr->tval != tval)) continue;
		if (!strstr(quark_str(o_ptr->note), "@ff")) continue;
		(*am_ptr) = o_ptr;
		return;
	}

	/* Use a (+0,+0) missile with the appropriate tval. */
	for (i = 0; i < MAX_K_IDX; i++)
	{
		if (k_info[i].tval != tval) continue;
		object_prep(*am_ptr, i);
		return;
	}
	/* Use nothing if no suitable missiles are available. */
	object_wipe(*am_ptr);
	return;
}

static void choose_bow(object_ctype *am_ptr, object_type **wp_ptr)
{
	object_type *o_ptr;
	s16b k_idx;

	/* Use a launcher from inventory if appropriate and marked. */
	for (o_ptr = inventory; o_ptr <= inventory+INVEN_PACK; o_ptr++)
	{
		if (!strstr(quark_str(o_ptr->note), "@ff")) continue;
		if (ammunition_type(o_ptr) != am_ptr->tval) continue;
		*wp_ptr = o_ptr;
		return;
	}

	/* Hack - use an unmarked equipped launcher if appropriate. */
	if (ammunition_type(inventory+INVEN_BOW) == am_ptr->tval)
	{
		*wp_ptr = inventory+INVEN_BOW;
		return;
	}

	/* Find an appropriate bow. */
	k_idx = launcher_type(am_ptr);

	/* No launcher. */
	if (k_idx == OBJ_NOTHING)
	{
		object_wipe(*wp_ptr);
		return;
	}
	else
	{
		object_prep(*wp_ptr, k_idx);
		return;
	}
}

/*
 * Calculate the average critical damage from a melee attack.
 * This tends to underestimate the effects of critical hits, but is  fairly
 * accurate.
 */
static s32b critical_average(int weight, int plus, int dam)
{
	int j, p = weight + ((p_ptr->to_h + plus) * 5) +
		(skill_set[p_ptr->wield_skill].value);
	s32b c;

	if (p <= 0) return dam;
	if (p > 5000) p = 5000;

	for (j = weight+1, c = 0; j <= weight+650; j++)
	{
		if (j < 400) c += dam+5;
		else if (j < 700) c += dam+10;
		else if (j < 900) c += 2*dam+15;
		else if (j < 1300) c += 2*dam+20;
		else c += ((5*dam)/2)+25;
	}

	return dam+c/650*p/5000;
}

/*
 * Give the average base damage the player gains from martial arts based on
 * do_ma_attack(), which works as follows:
 * 
 * An attack is selected i times.
 * The game chooses attacks randomly until one is selected, which happens
 * (skill-ma_ptr->chance)/skill of the time.
 * The attack with the greatest skill is chosen. If there are two such,
 * the one chosen first is used.
 *
 * This does tend to underestimate the damage at high levels, but not by much.
 */
static s32b average_ma_damage(void)
{
	int i, j, k, t, skill = skill_set[SKILL_MA].value/2*2;
	s32b c[MAX_MA], d[MAX_MA], tdam, denom;
	for (t = 0; t < MAX_MA; t++)
	{
		if (ma_blows[t].min_level > skill) break;
	}
	if (!t || p_ptr->stun || p_ptr->confused)
	{
		martial_arts *ma_ptr = ma_blows+MAX_MA;
		s32b crit, dam = 60*ma_ptr->dd*(ma_ptr->ds+1)/2;
		for (k = 1, crit = 0; k <= 10; k++)
		{
			crit += critical_average(skill/2*k, ma_ptr->min_level/2, dam);
		}
		return crit/10;
	}

	/* Initialise d with the lowest blow. */
	WIPE(d,d);
	d[0] = 1;

	for (i = MAX(1, skill/14); i; i--)
	{
		bool overflow = FALSE;
		WIPE(c,c);

		for (j = 0; j < t; j++)
		{
			/* Calculate how likely the routine is to select attack j. */
			martial_arts *ma_ptr = ma_blows+j;
			int chance = MAX(0, skill - ma_ptr->chance);

			/* Transfer the chance for each previously selected attack. */
			for (k = 0; k < t; k++)
			{
				/* If j is a higher level attack, it is used instead of k. */
				if (ma_blows[k].min_level < ma_ptr->min_level)
				{
					c[j] += d[k]*chance;
				}
				/* Otherwise, k is still the chosen attack. */
				else
				{
					c[k] += d[k]*chance;
				}
			}
		}

		for (j = 0; j < t; j++)
		{
			if (c[j] > MAX_S32B/MA_INTERVAL) overflow = TRUE;
		}

		/* Copy back, rounding if necessary. */
		for (j = 0; j < t; j++)
		{
			if (overflow)
				d[j] = c[j]/MA_INTERVAL;
			else
				d[j] = c[j];
		}
	}

	for (j = denom = 0; j < MAX_MA; j++) denom += d[j];

	for (j = tdam = 0; j < MAX_MA; j++)
	{
		s32b crit, dam = 60*ma_blows[j].dd*(ma_blows[j].ds+1)/2;
		for (k = 1, crit = 0; k <= 10; k++)
		{
			crit += critical_average(skill/2*k, ma_blows[j].min_level/2, dam);
		}
		tdam += crit*d[j]/10;
	}

	return tdam/denom;
}

/*
 * Given an object to be scrutinised, find a weapon and some ammunition (if
 * appropriate) to use it.
 * 
 * *wp_ptr and *am_ptr will eventually refer to the weapon and ammunition
 * to be used, but should initially point to blank object_types.
 */
static int choose_weapon(object_type *o_ptr, object_type **wp_ptr,
	object_type **am_ptr)
{
	int slot;
	object_ctype *j_ptr = get_real_obj(o_ptr);

	/* Is o_ptr equipped? */
	if (j_ptr == inventory+INVEN_WIELD || j_ptr == inventory+INVEN_BOW)
	{
		slot = j_ptr-inventory;
	}
	else
	{
		slot = wield_slot(o_ptr);
	}

	/* Set the weapon and ammunition pointers. */
	switch (slot)
	{
		case INVEN_WIELD:
		{
			(*wp_ptr) = o_ptr;
			object_wipe(*am_ptr);
			break;
		}
		case INVEN_BOW:
		{
			(*wp_ptr) = o_ptr;
			choose_ammunition(o_ptr, am_ptr);
			break;
		}
		/* This is a missile, so wp_ptr is expected to be a bow. */
		default:
		{
			(*am_ptr) = o_ptr;
			choose_bow(o_ptr, wp_ptr);
			return INVEN_BOW;
		}
	}
	return slot;
}

/*
 * Now that's done, we do some statistical stuff.
 * We want to know the to-hit bonus, the damage bonus, the number of blows
 * per turn and the average damage per turn.
 */
static void weapon_stats_calc(object_ctype *wp_ptr,
	object_ctype *am_ptr, int slot, int slay, s16b *tohit, s16b *todam,
	s16b *weap_blow, s16b *mut_blow, s32b *damage)
{
	int power, damsides, damdice, dicedam;

	/* Start at 0. */
	(*tohit) = (*todam) = (*weap_blow) = (*mut_blow) = (*damage) = 0;

	/* Hack - bows without arrows can neither hit nor damage */
	if (slot == INVEN_BOW && !am_ptr->k_idx) return;

	/* Weapons use the intrinsic damage bonus */
	if (slot == INVEN_WIELD) (*todam) = p_ptr->dis_to_d;
	else (*todam) = 0;
	
	/* Everything uses the intrinsic hit bonus */
	(*tohit) = p_ptr->dis_to_h;

	/* Add in the weapons' bonuses */
	(*tohit) += wp_ptr->to_h;
	(*todam) += wp_ptr->to_d;

	(*tohit) += am_ptr->to_h;
	(*todam) += am_ptr->to_d;

	/* Calculate the power, number of blows, sides and dice. */
	if (slot == INVEN_WIELD)
	{
		power = 1;
		(*weap_blow) = p_ptr->num_blow;
		damsides = wp_ptr->ds;
		damdice = wp_ptr->dd;
	}
	else
	{
		u32b junk, f3;

		/* Hack -- Extract the "base power" */
		if (wp_ptr->k_idx) power = get_bow_mult(wp_ptr);
		/* Without a launcher everything has a power equal to throw_mult in cmd2.c */
		else power = 1;
	
		object_flags_known(wp_ptr, &junk, &junk, &f3);

		/* Apply the "Extra Might" flag */
		if (f3 & (TR3_XTRA_MIGHT)) power++;

		(*weap_blow) = p_ptr->num_fire;

		damsides = am_ptr->ds;
		damdice = am_ptr->dd;
	}
	
	dicedam = 60*damdice*(1+damsides)/2;

	/* Calculate the damage bonus. */
	(*damage) += 60*(*todam);
	
	/* Martial arts damage. */
	if (slot == INVEN_WIELD && ma_empty_hands())
	{
		(*damage) += average_ma_damage();
	}
	/* Add in the slays. */
	else if (slot == INVEN_WIELD)
	{
		(*damage) +=
			critical_average(wp_ptr->weight, wp_ptr->to_h, dicedam * slay);
	}
	else
	{
		(*damage) += dicedam;
		(*damage) *= slay;
	}

	/* Add extra melee damage from the VORPAL flag (see py_attack()). */
	if (slot == INVEN_WIELD)
	{
		u32b f1,f2,f3;
		object_flags_known(wp_ptr, &f1, &f2, &f3);

		/* Not VORPAL, so do nothing. */
		if (~f1 & TR1_VORPAL);
		/* Hack - Vorpal Blade is more powerful. */
		else if (wp_ptr->name1 == ART_VORPAL_BLADE)
		{
			(*damage) += dicedam * slay * 4 / 3;
		}
		/* Other VORPAL weapons. */
		else
		{
			(*damage) += dicedam * slay * 19 / 18;
		}			
	}

	/* BUG - this considers negative rolls. */
	(*damage) = MAX((*damage), 0);

	/* Round blows/turn up according to the energy used. 
	 * Hack - do not round mutated blows here, although they are rounded. */
	(*weap_blow) = TURN_ENERGY*60/(TURN_ENERGY*60/(*weap_blow));

	/* Consider the number of blows and bow multiplier. */
	(*damage) = (*damage)*power*(*weap_blow)/60;

	/* Finally, add the mutated attacks. The numbers are based on those in
	 * natural_attack(), which appear to differ from the ones in dump_chaos_feature(). */
	if (slot == INVEN_WIELD)
	{
		if (p_has_mutation(MUT_HORNS))
		{
			(*mut_blow)++;
			(*damage) += 60*6*(1+2)/2;
		}
		if (p_has_mutation(MUT_SCOR_TAIL))
		{
			(*mut_blow)++;
			(*damage) += 60*7*(1+3)/2;
		}
		if (p_has_mutation(MUT_BEAK))
		{
			(*mut_blow)++;
			(*damage) += 60*4*(1+2)/2;
		}
		if (p_has_mutation(MUT_TRUNK))
		{
			(*mut_blow)++;
			(*damage) += 60*4*(1+1)/2;
		}
		if (p_has_mutation(MUT_TENTACLES))
		{
			(*mut_blow)++;
			(*damage) += 60*5*(1+2)/2;
		}
	}
}

/*
 * Calculate the number of blows and the damage done by a given weapon using
 * a specified slay multiplier, together with the to-hit and to-damage bonuses
 * of the weapon.
 */
void weapon_stats(object_type *o_ptr, int slay, s16b *tohit, s16b *todam, s16b *weap_blow, s16b *mut_blow, s32b *damage)
{
	/* wp_ptr and am_ptr point to the weapon and (where appropriate) ammunition
	 * being considered. wp and am are temporary representations of these, which
	 * differ in one crucial respect: am is discarded at the end, but the contents
	 * of wp is copied to wp_ptr. */
	object_type wp, am, old, *wp_ptr = &wp, *am_ptr = &am;

	/* A backup copy of p_ptr for if the original is changed. This is because
	 * there is no way to find out what would happen if a change was made
	 * without of actually making that change.
	 * It assumes that p_ptr contains everything update_stuff affects. */
	player_type p2_body, *p2_ptr = &p2_body;

	/* Find a weapon and missile to use o_ptr, and set slot to
	 * wield_slot(wp_ptr), treating non-weapons specially. */
	int slot = choose_weapon(o_ptr, &wp_ptr, &am_ptr);

	/* Wield the weapon, make a copy of the real stats and use the stats for
	 * the weapon. */
	object_copy(&old, inventory+slot);
	if (wp_ptr != inventory+slot) object_copy(inventory+slot, wp_ptr);

	COPY(p2_ptr, p_ptr, player_type);
	p_ptr->update |= PU_BONUS | PU_QUIET;
	update_stuff();

	/* Actually calculate everything. */
	weapon_stats_calc(wp_ptr, am_ptr, slot, slay, tohit, todam, weap_blow,
		mut_blow, damage);

	/* Now that's done, we need only replace the standard weapon. */
	object_copy(inventory+slot, &old);

	/* Re-run calc_bonuses() to ensure that everything not in p2_ptr is
	 * correct. */
	p_ptr->update |= PU_BONUS | PU_QUIET;
	update_stuff();

	/* Return p_ptr to its original state if needed. */
	COPY(p_ptr, p2_ptr, player_type);
}

/*
 * Prints the following information on the screen.
 *
 * If missile is false, melee damage is shown.
 * Otherwise, if there is ammunition for the equipped missile weapon which
 * is inscribed with @ff, missile damage with those missiles is shown.
 * Otherwise, damage with (+0,+0) missiles of the appropriate type with an
 * sval of 1 is shown.
 */
static void display_player_sides(bool missile)
{
	s16b show_tohit, show_todam, weap_blow, mut_blow;
	s32b damage;
	cptr temp;

	/* The last column of the numbers. See prt_num() */
	const int col = 1+strlen("+ To Damage ")+3+6;

	object_type j_ptr[1], *o_ptr = &inventory[(missile) ? INVEN_BOW : INVEN_WIELD];

	object_info_known(j_ptr, o_ptr);

	weapon_stats(j_ptr, 1, &show_tohit, &show_todam, &weap_blow, &mut_blow, &damage);

	put_str((missile) ? "Missile" : "Melee", 9, 3);

	/* Dump the bonuses to hit/dam */
	prt_num("+ To Hit    ", show_tohit, 10, 1, TERM_L_BLUE);
	prt_num("+ To Damage ", show_todam, 11, 1, TERM_L_BLUE);

	/* Dump the number of attack per round */
	put_str("Attack/Round", 12, 1);
	if (!mut_blow)
		temp = format("%d,%d", weap_blow/60, weap_blow%60);
	else
		temp = format("%d,%d+%d", weap_blow/60, weap_blow%60, mut_blow);
	/* Display string, right-aligned. */
	c_put_str(TERM_L_BLUE, temp, 12, col-strlen(temp));

	/* Dump the damage per round */
	put_str("Damage/Round", 13, 1);
	temp = format("%ld,%ld", damage/60, damage%60);
	c_put_str(TERM_L_BLUE, temp, 13, col-strlen(temp));

	prt_nums("Hit Points:", 9, 52, 76, p_ptr->chp, p_ptr->mhp);
	prt_nums("Spell Points:", 10, 52, 76, p_ptr->csp, p_ptr->msp);
	prt_nums("Chi Points:", 11, 52, 76, p_ptr->cchi, p_ptr->mchi);

	prt_lnum("Gold           ", p_ptr->au, 13, 52, TERM_L_GREEN);
}

/*
 * Prints the following information on the screen.
 *
 * For this to look right, the following should be spaced the
 * same as in the prt_lnum code... -CFT
 */
static void display_player_xp(void)
{
	prt_lnum("Experience ", p_ptr->exp, 10, 28, TERM_L_GREEN);

	prt_lnum("Exp Factor ", p_ptr->expfact, 13, 28, TERM_L_GREEN);
}


/* The next routine would look horrible without this. */
#define remember(col, msg) {strcpy(text[i], msg); colour[i++] = col;}

/*
 * Displays a few temporary effects on the screen.
 * Returns whether something was displayed.
 */
static bool display_player_bonus(void)
{
	byte i = 0, j;
	cptr text[5] = {"Blessed","Stone Skin","Heroic","Safe from Evil","Berserk"};
	byte colour[5] = {TERM_L_GREEN, TERM_SLATE, TERM_YELLOW, TERM_WHITE, TERM_L_RED};
	const byte pos[5][5] = {{11}, {10, 12}, {9, 11, 13}, {9, 10, 12, 13}, {9, 10, 11, 12, 13}};
	byte x[5];
	if (p_ptr->blessed) x[i++] = 0;
	if (p_ptr->shield) x[i++] = 1;
	if (p_ptr->hero) x[i++] = 2;
	if (p_ptr->protevil) x[i++] = 3;
	if (p_ptr->shero) x[i++] = 4;
	for (j = i; j;)
	{
		j--;
		c_put_str(colour[x[j]], text[x[j]], pos[i-1][j], 34);
	}
	return (i > 0);
}


/*
 * Hack -- pass color info around this file
 */
static byte likert_color = TERM_WHITE;


/*
 * Returns a "rating" of x depending on y
 */
static cptr likert(int x, int y)
{
	/* Paranoia */
	if (y <= 0) y = 1;

	/* Negative value */
	if (x < 0)
	{
		likert_color = TERM_L_DARK;
		return ("Very Bad");
	}

	/* Analyze the value */
	switch ((x / y))
	{
		case 0:
		case 1:
		{
			likert_color = TERM_RED;
			return ("Bad");
		}
		case 2:
		{
			likert_color = TERM_L_RED;
			return ("Poor");
		}
		case 3:
		case 4:
		{
			likert_color = TERM_ORANGE;
			return ("Fair");
		}
		case 5:
		{
			likert_color = TERM_YELLOW;
			return ("Good");
		}
		case 6:
		{
			likert_color = TERM_YELLOW;
			return ("Very Good");
		}
		case 7:
		case 8:
		{
			likert_color = TERM_L_GREEN;
			return ("Excellent");
		}
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		{
			likert_color = TERM_GREEN;
			return ("Superb");
		}
		case 14:
		case 15:
		case 16:
		case 17:
		{
			likert_color = TERM_BLUE;
			return ("Incredible");
		}
		default:
		{
			likert_color = TERM_VIOLET;
			return ("Stupendous");
		}
	}
}


/*
 * Prints ratings on certain abilities
 *
 * This code is "imitated" elsewhere to "dump" a character sheet.
 */
static void display_player_various(void)
{
    int         tmp, damdice, damsides, dambonus, blows;
	int			xthn, xthb, xfos, xsrh;
	int			xdis, xdev, xsav, xstl;
	cptr	  desc;
    int         muta_att = 0;
	object_type		*o_ptr;

	if (p_has_mutation(MUT_HORNS))     muta_att++;
	if (p_has_mutation(MUT_SCOR_TAIL)) muta_att++;
	if (p_has_mutation(MUT_BEAK))      muta_att++;
	if (p_has_mutation(MUT_TRUNK))     muta_att++;
	if (p_has_mutation(MUT_TENTACLES)) muta_att++;

	/* Fighting Skill (with current weapon) */
	o_ptr = &inventory[INVEN_WIELD];
	tmp = p_ptr->to_h + o_ptr->to_h;
	xthn = p_ptr->skill_thn + (tmp * BTH_PLUS_ADJ);

	/* Shooting Skill (with current bow and normal missile) */
	o_ptr = &inventory[INVEN_BOW];
	tmp = p_ptr->to_h + o_ptr->to_h;
	xthb = p_ptr->skill_thb + (tmp * BTH_PLUS_ADJ);


     /* Average damage per round */
     o_ptr = &inventory[INVEN_WIELD];
     dambonus = p_ptr->dis_to_d;
     if (object_known_p(o_ptr)) dambonus += o_ptr->to_d;
     damdice = o_ptr->dd;
     damsides = o_ptr->ds;   /* dam += (o_ptr->dd * (o_ptr->ds + 1)) >> 1; */
     blows = p_ptr->num_blow;
     /* dam *= p_ptr->num_blow; */

     /* Basic abilities */

	xdis = p_ptr->skill_dis;
	xdev = p_ptr->skill_dev;
	xsav = p_ptr->skill_sav;
	xstl = p_ptr->skill_stl;
	xsrh = p_ptr->skill_srh;
	xfos = p_ptr->skill_fos;

	put_str("Fighting    :", 16, 1);
	desc = likert(xthn, 12);
	c_put_str(likert_color, desc, 16, 15);

	put_str("Bows/Throw  :", 17, 1);
	desc = likert(xthb, 12);
	c_put_str(likert_color, desc, 17, 15);

	put_str("Saving Throw:", 18, 1);
	desc = likert(xsav, 6);
	c_put_str(likert_color, desc, 18, 15);

	put_str("Stealth     :", 19, 1);
	desc = likert(xstl, 1);
	c_put_str(likert_color, desc, 19, 15);


	put_str("Perception  :", 16, 28);
	desc = likert(xfos, 6);
	c_put_str(likert_color, desc, 16, 42);

	put_str("Searching   :", 17, 28);
	desc = likert(xsrh, 6);
	c_put_str(likert_color, desc, 17, 42);

	put_str("Disarming   :", 18, 28);
	desc = likert(xdis, 8);
	c_put_str(likert_color, desc, 18, 42);

	put_str("Magic Device:", 19, 28);
	desc = likert(xdev, 6);
	c_put_str(likert_color, desc, 19, 42);


	put_str("+ To AC:", 16, 55);
	put_str(format("%6d", p_ptr->dis_to_a), 16, 69);

	put_str("Base AC:", 17, 55);
	put_str(format("%6d", p_ptr->dis_ac), 17, 69);

	put_str("Infra-Vision:", 19, 55);
	put_str(format("%d feet", p_ptr->see_infra * 10), 19, 69);
}



/*
 * Obtain the "flags" for the player as if he was an item
 */
void player_flags(u32b *f1, u32b *f2, u32b *f3)
{
	/* Clear */
	(*f1) = (*f2) = (*f3) = 0L;

	/* Elf */
	if (p_ptr->prace == RACE_ELF) (*f2) |= (TR2_RES_LITE);

	/* Hobbit */
	if (p_ptr->prace == RACE_HOBBIT) (*f2) |= (TR2_SUST_DEX);

	/* Gnome */
    if (p_ptr->prace == RACE_GNOME) (*f2) |= (TR2_FREE_ACT);

	/* Dwarf */
	if (p_ptr->prace == RACE_DWARF) (*f2) |= (TR2_RES_BLIND);

	/* Half-Orc */
	if (p_ptr->prace == RACE_HALF_ORC) (*f2) |= (TR2_RES_DARK);

	/* Half-Troll */
    if (p_ptr->prace == RACE_HALF_TROLL)
        {
            (*f2) |= (TR2_SUST_STR);
            if ((skill_set[SKILL_RACIAL].value/2) > 14)
            {
                (*f3) = (TR3_REGEN);
            }
        }

	/* Dunadan */
    if (p_ptr->prace == RACE_GREAT)

        {
            (*f2) |= (TR2_SUST_CON);
            (*f3) |= (TR3_REGEN); /* Great ones heal fast */
        }

	/* High Elf */
	if (p_ptr->prace == RACE_HIGH_ELF) (*f2) |= (TR2_RES_LITE);
    if (p_ptr->prace == RACE_HIGH_ELF) (*f3) |= (TR3_SEE_INVIS);

    if (p_ptr->prace == RACE_BARBARIAN) (*f2) |= (TR2_RES_FEAR);
    else if (p_ptr->prace == RACE_HALF_OGRE)
    {
            (*f2) |= (TR2_SUST_STR);
            (*f2) |= (TR2_RES_DARK);
        }
    else if (p_ptr->prace == RACE_HALF_GIANT)
    {
        (*f2) |= (TR2_RES_SHARDS);
        (*f2) |= (TR2_SUST_STR);
    }
    else if (p_ptr->prace == RACE_HALF_TITAN)
    {
        (*f2) |= (TR2_RES_CHAOS);
    }
    else if (p_ptr->prace == RACE_CYCLOPS)
    {
        (*f2) |= (TR2_RES_SOUND);
    }
    else if (p_ptr->prace == RACE_YEEK)
    {
        (*f2) |= (TR2_RES_ACID);
        if ((skill_set[SKILL_RACIAL].value/2) > 19)
        {
            (*f2) |= (TR2_IM_ACID);
        }
    }
    else if (p_ptr->prace == RACE_KLACKON)
    {
        if ((skill_set[SKILL_RACIAL].value/2) > 9) (*f1) |= TR1_SPEED;
        (*f2) |= (TR2_RES_CONF);
        (*f2) |= (TR2_RES_ACID);
    }
    else if (p_ptr->prace == RACE_KOBOLD)
    {
        (*f2) |= (TR2_RES_POIS);
    }
    else if (p_ptr->prace == RACE_NIBELUNG)
    {
        (*f2) |= (TR2_RES_DISEN);
        (*f2) |= (TR2_RES_DARK);
    }
    else if (p_ptr->prace == RACE_DARK_ELF)
    {
        (*f2) |= (TR2_RES_DARK);
        if ((skill_set[SKILL_RACIAL].value/2) > 19)
        {
            (*f3) |= (TR3_SEE_INVIS);
        }
    }
    else if (p_ptr->prace == RACE_DRACONIAN)
    {
            (*f3) |= TR3_FEATHER;
        if ((skill_set[SKILL_RACIAL].value/2) > 4)
        {
            (*f2) |= (TR2_RES_FIRE);
        }
        if ((skill_set[SKILL_RACIAL].value/2) > 9)
        {
            (*f2) |= (TR2_RES_COLD);
        }
        if ((skill_set[SKILL_RACIAL].value/2) > 14)
        {
            (*f2) |= (TR2_RES_ACID);
        }
        if ((skill_set[SKILL_RACIAL].value/2) > 19)
        {
            (*f2) |= (TR2_RES_ELEC);
        }
        if ((skill_set[SKILL_RACIAL].value/2) > 34)
        {
            (*f2) |= (TR2_RES_POIS);
        }

    }
    else if (p_ptr->prace == RACE_MIND_FLAYER)
    {
        (*f2) |= (TR2_SUST_INT);
        (*f2) |= (TR2_SUST_WIS);
        if ((skill_set[SKILL_RACIAL].value/2) > 14)
        {
            (*f3) |= (TR3_SEE_INVIS);
        }
        if ((skill_set[SKILL_RACIAL].value/2) > 29)
        {
            (*f3) |= (TR3_TELEPATHY);
        }
    }
    else if (p_ptr->prace == RACE_IMP)
    {
        (*f2) |= (TR2_RES_FIRE);
        if ((skill_set[SKILL_RACIAL].value/2) > 9)
        {
            (*f3) |= (TR3_SEE_INVIS);
        }
    }
    else if (p_ptr->prace == RACE_GOLEM)
    {
        (*f3) |= (TR3_SEE_INVIS);
        (*f2) |= (TR2_FREE_ACT);
        (*f2) |= (TR2_RES_POIS);
        (*f3) |= (TR3_SLOW_DIGEST);
        if ((skill_set[SKILL_RACIAL].value/2) > 34)
        {
            (*f2) |= (TR2_HOLD_LIFE);
        }
    }
    else if (p_ptr->prace == RACE_SKELETON)
    {
        (*f3) |= (TR3_SEE_INVIS);
        (*f2) |= (TR2_RES_SHARDS);
        (*f2) |= (TR2_HOLD_LIFE);
        (*f2) |= (TR2_RES_POIS);
        if ((skill_set[SKILL_RACIAL].value/2) > 9)
        {
            (*f2) |= (TR2_RES_COLD);
        }
    }
    else if (p_ptr->prace == RACE_ZOMBIE)
    {
        (*f3) |= (TR3_SEE_INVIS);
        (*f2) |= (TR2_HOLD_LIFE);
        (*f2) |= (TR2_RES_NETHER);
        (*f2) |= (TR2_RES_POIS);
        (*f3) |= (TR3_SLOW_DIGEST);
        if ((skill_set[SKILL_RACIAL].value/2) > 4)
        {
            (*f2) |= (TR2_RES_COLD);
        }
    }
    else if (p_ptr->prace == RACE_VAMPIRE)
    {
       (*f2) |= (TR2_HOLD_LIFE);
       (*f2) |= (TR2_RES_DARK);
       (*f2) |= (TR2_RES_NETHER);
       (*f3) |= (TR3_LITE);
       (*f2) |= (TR2_RES_POIS);
       (*f2) |= (TR2_RES_COLD);
    }
    else if (p_ptr->prace == RACE_SPECTRE)
    {

        (*f2) |= (TR2_RES_COLD);
        (*f3) |= (TR3_SEE_INVIS);
        (*f2) |= (TR2_HOLD_LIFE);
        (*f2) |= (TR2_RES_NETHER);
        (*f2) |= (TR2_RES_POIS);
        (*f3) |= (TR3_SLOW_DIGEST);
        if ((skill_set[SKILL_RACIAL].value/2) > 34)
        {
            (*f3) |= TR3_TELEPATHY;
        }
    }
    else if (p_ptr->prace == RACE_SPRITE)
    {
        (*f2) |= (TR2_RES_LITE);
        (*f3) |= (TR3_FEATHER);
        if ((skill_set[SKILL_RACIAL].value/2) > 9)
            (*f1) |= (TR1_SPEED);
    }
    else if (p_ptr->prace == RACE_BROO)
    {
        (*f2) |= (TR2_RES_SOUND);
        (*f2) |= (TR2_RES_CONF);
    }
    if (skill_set[SKILL_MINDCRAFTING].value >= 80) (*f3) |= TR3_TELEPATHY;
        if (p_has_mutation(MUT_FLESH_ROT))
        {
            (*f3) &= ~(TR3_REGEN);
        }

        if ((p_has_mutation(MUT_XTRA_FAT)) || 
			(p_has_mutation(MUT_XTRA_LEGS)) ||
            (p_has_mutation(MUT_SHORT_LEG)))
        {
            (*f1) |= TR1_SPEED;
        }

        if (p_has_mutation(MUT_ELEC_TOUC))
        {
            (*f3) |= TR3_SH_ELEC;
        }

        if (p_has_mutation(MUT_FIRE_BODY))
        {
            (*f3) |= TR3_SH_FIRE;
            (*f3) |= TR3_LITE;
        }

        if (p_has_mutation(MUT_WINGS))
        {
            (*f3) |= TR3_FEATHER;
        }

        if (p_has_mutation(MUT_FEARLESS))
        {
            (*f2) |= (TR2_RES_FEAR);
        }

        if (p_has_mutation(MUT_REGEN))
        {
            (*f3) |= TR3_REGEN;
        }

        if (p_has_mutation(MUT_ESP))
        {
            (*f3) |= TR3_TELEPATHY;
        }
		if (p_has_mutation(MUT_ESP))
		{
			(*f3) |= TR3_TELEPATHY;
		}
		
		if (p_has_mutation(MUT_MOTION))
		{
			(*f2) |= TR2_FREE_ACT;
		}
		
		if (p_has_mutation(MUT_SUS_STATS))
		{
			(*f2) |= TR2_SUST_CON;
			if ((skill_set[SKILL_RACIAL].value/2) > 9)
				(*f2) |= TR2_SUST_STR;
			if ((skill_set[SKILL_RACIAL].value/2) > 19)
				(*f2) |= TR2_SUST_DEX;
			if ((skill_set[SKILL_RACIAL].value/2) > 29)
				(*f2) |= TR2_SUST_WIS;
			if ((skill_set[SKILL_RACIAL].value/2) > 39)
				(*f2) |= TR2_SUST_INT;
			if ((skill_set[SKILL_RACIAL].value/2) > 49)
				(*f2) |= TR2_SUST_CHR;
		}
}


/*
 * Equippy chars
 */
void display_player_equippy(int y, int x)
{
	int i;

	byte a;
	char c;

	object_type *o_ptr;


	/* Dump equippy chars */
	for (i=INVEN_WIELD; i<INVEN_POUCH_1; i++)
	{
		/* Object */
		o_ptr = &inventory[i];

        a = object_attr(o_ptr);
        c = object_char(o_ptr);

        if ((!equippy_chars) ||
            (!o_ptr->k_idx)) /* Clear the part of the screen */
            {
              c = ' ';
              a = TERM_DARK;
            }

		/* Dump */
		Term_putch(x+i-INVEN_WIELD, y, a, c);
	}
}


/*
 * Helper function, see below
 */
static void display_player_flag_aux(int row, int col,
			cptr str, int n, u32b flag)
{
	int i;

	u32b f[3];


	/* Header */
	c_put_str(TERM_WHITE, str, row, col);

	/* Advance */
	col += strlen(str) + 1;


	/* Check equipment */
	for (i=INVEN_WIELD; i<INVEN_POUCH_1; i++)
	{
		object_type *o_ptr;

		/* Object */
		o_ptr = &inventory[i];

		/* Known flags */
		object_flags_known(o_ptr, &f[0], &f[1], &f[2]);

		/* Default */
		c_put_str(TERM_SLATE, ".", row, col);

		/* Check flags */
		if (f[n-1] & flag) c_put_str(TERM_WHITE, "+", row, col);

		/* Advance */
		col++;
	}

	/* Player flags */
	player_flags(&f[0], &f[1], &f[2]);

	/* Default */
	c_put_str(TERM_SLATE, ".", row, col);

	/* Check flags */
	if (f[n-1] & flag)
		i = 1;
	else
		i = 0;

	/*
	 * Hack - include temporary resistances individually.
	 * It's important to note that OPPOSE_COL(0)=TERM_DARK=0.
	 */
	if (n == 2 && flag & TR2_RES_ACID) i |= OPPOSE_COL(p_ptr->oppose_acid)*2;
	if (n == 2 && flag & TR2_RES_ELEC) i |= OPPOSE_COL(p_ptr->oppose_elec)*2;
	if (n == 2 && flag & TR2_RES_FIRE) i |= OPPOSE_COL(p_ptr->oppose_fire)*2;
	if (n == 2 && flag & TR2_RES_COLD) i |= OPPOSE_COL(p_ptr->oppose_cold)*2;
	if (n == 2 && flag & TR2_RES_POIS) i |= OPPOSE_COL(p_ptr->oppose_pois)*2;
	if (n == 2 && flag & TR2_RES_FEAR && !(f[1] & TR2_RES_FEAR))
		i |= OPPOSE_COL(MAX(p_ptr->hero, p_ptr->shero))*2;
	if (n == 3 && flag & TR3_TELEPATHY && !(f[2] & TR3_TELEPATHY))
		i |= OPPOSE_COL(p_ptr->tim_esp)*2;

	if (i == 1)
		n = TERM_WHITE;
	else if (i == 0)
		n = TERM_SLATE;
	else
		n = i/2;

	if (i & 1) c_put_str(n, "+", row, col);
	else if (i) c_put_str(n, "*", row, col);
	else c_put_str(n, ".", row, col);

}


/*
 * Special display, part 1
 */
static void display_player_flag_info(void)
{
	int row;
	int col;


	/*** Set 1 ***/

	row = 13;
	col = 1;

	display_player_equippy(row-2, col+7);

	c_put_str(TERM_WHITE, "abcdefghijkl@", row-1, col+7);

	display_player_flag_aux(row+0, col, "Acid :", 2, (TR2_RES_ACID | TR2_IM_ACID));
	display_player_flag_aux(row+1, col, "Elec :", 2, (TR2_RES_ELEC | TR2_IM_ELEC));
	display_player_flag_aux(row+2, col, "Fire :", 2, (TR2_RES_FIRE | TR2_IM_FIRE));
	display_player_flag_aux(row+3, col, "Cold :", 2, (TR2_RES_COLD | TR2_IM_COLD));
	display_player_flag_aux(row+4, col, "Poisn:", 2, TR2_RES_POIS);
	display_player_flag_aux(row+5, col, "Light:", 2, TR2_RES_LITE);
	display_player_flag_aux(row+6, col, "Dark :", 2, TR2_RES_DARK);
	display_player_flag_aux(row+7, col, "Shard:", 2, TR2_RES_SHARDS);
    display_player_flag_aux(row+8, col, "Blind:", 2, TR2_RES_BLIND);
    display_player_flag_aux(row+9, col, "Conf :", 2, TR2_RES_CONF);

	/*** Set 2 ***/

	row = 13;
	col = 24;

	display_player_equippy(row-2, col+8);

	c_put_str(TERM_WHITE, "abcdefghijkl@", row-1, col+8);

    display_player_flag_aux(row+0, col, "Sound :", 2, TR2_RES_SOUND);
    display_player_flag_aux(row+1, col, "Nether:", 2, TR2_RES_NETHER);
    display_player_flag_aux(row+2, col, "Nexus :", 2, TR2_RES_NEXUS);
    display_player_flag_aux(row+3, col, "Chaos :", 2, TR2_RES_CHAOS);
    display_player_flag_aux(row+4, col, "Disnch:", 2, TR2_RES_DISEN);
    display_player_flag_aux(row+5, col, "Fear  :", 2, TR2_RES_FEAR);
    display_player_flag_aux(row+6, col, "Reflct:", 2, TR2_REFLECT);
    display_player_flag_aux(row+7, col, "AuFire:", 3, TR3_SH_FIRE);
    display_player_flag_aux(row+8, col, "AuElec:", 3, TR3_SH_ELEC);

	/*** Set 3 ***/

	row = 13;
	col = 48;

	display_player_equippy(row-2, col+15);

    c_put_str(TERM_WHITE, "abcdefghijkl@", row-1, col+15);

    display_player_flag_aux(row+0, col, "Speed        :", 1, TR1_SPEED);
    display_player_flag_aux(row+1, col, "Free Action  :", 2, TR2_FREE_ACT);
    display_player_flag_aux(row+2, col, "See Invisible:", 3, TR3_SEE_INVIS);
    display_player_flag_aux(row+3, col, "Hold Life    :", 2, TR2_HOLD_LIFE);
    display_player_flag_aux(row+4, col, "Telepathy    :", 3, TR3_TELEPATHY);
    display_player_flag_aux(row+5, col, "Slow Digest  :", 3, TR3_SLOW_DIGEST);
    display_player_flag_aux(row+6, col, "Regeneration :", 3, TR3_REGEN);
    display_player_flag_aux(row+7, col, "Levitation   :", 3, TR3_FEATHER);
    display_player_flag_aux(row+8, col, "Perm Lite    :", 3, TR3_LITE);
}

/*
 * Return a colour based on the status of a skill
 */
static byte skill_colour(int skill_index)
{
	player_skill *sk_ptr = &skill_set[skill_index];

	/* The skill either cannot be increased at this level, or is at 0%
	 * and has never been successfully used. */
	bool max = ((!sk_ptr->max_value && !sk_ptr->experience)
		|| (sk_ptr->value == 100) || !skill_check_possible(skill_index));

	/* The skill is below its maximum value */
	bool drained = (sk_ptr->max_value > sk_ptr->value);

	if (drained && max) return TERM_L_WHITE;
	else if (drained) return TERM_ORANGE;
	else if (max) return TERM_WHITE;
	else return TERM_YELLOW;
}

/*
 * Display the information for a given skill at a given position.
 */
static void display_player_skills_aux(int skill)
{
	const player_skill *sk_ptr = skill_set+skill;
	cptr name;
	char *s, buf[30];

	/* Hack - use a special name for some skills. */
	switch (skill)
	{
		case SKILL_SLASH: case SKILL_STAB: case SKILL_CRUSH:
		{
			name = format("%.*ss", strlen(sk_ptr->name)-3, sk_ptr->name);
			break;
		}
		case SKILL_CORPORIS: case SKILL_VIS:
		case SKILL_NATURAE: case SKILL_ANIMAE:
		{
			name = format("Magice %s", sk_ptr->name);
			break;
		}
		case SKILL_DEVICE:
		{
			name = "Devices"; break;
		}
		case SKILL_CHI:
		{
			name = "Chi Balance"; break;
		}
		default:
		{
			name = sk_ptr->name;
		}
	}

	sprintf(buf, "%-17s%d%%", name, sk_ptr->value);

	/* This should always be true. */
	if (strchr(buf, ' '))
	{
		/* Find the start of the last set of spaces, and make it a colon. */
		for (s = strrchr(buf, ' '); *s == ' '; s--);
		*++s = ':';
	}

	/* Print at the desired position. */
	c_put_str(skill_colour(skill), buf, sk_ptr->y, sk_ptr->x);
}

/*
 * Display all the player's base skill levels
 */

static void display_player_skills(void)
{
	int skill;

	/* Headers. */
	put_str("Everyman Skills",1,7);
	put_str("===============",2,7);
	put_str("Specialist Skills",1,31);
	put_str("=================",2,31);
	put_str("Hermetic Skills",1,56);
	put_str("===============",2,56);

	/* Skills */
	for (skill = 0; skill < (int)N_ELEMENTS(skill_set); skill++)
	{
		display_player_skills_aux(skill);
	}
}

/*
 * Special display, part 2a
 */
static void display_player_misc_info(void)
{
	char	buf[80];

	/* Display basics */
	put_str("Name      :", 2, 1);
	put_str("Sex       :", 3, 1);
	put_str("Race      :", 4, 1);
	put_str("Template  :", 5, 1);

	c_put_str(TERM_L_BLUE, player_name, 2, 13);
	c_put_str(TERM_L_BLUE, sp_ptr->title, 3, 13);
	c_put_str(TERM_L_BLUE, rp_ptr->title, 4, 13);
	c_put_str(TERM_L_BLUE, cp_ptr->title, 5, 13);

	/* Display extras */
	put_str("Hits(Max) :", 7, 1);
	put_str("Mana(Max) :", 8, 1);

	(void) sprintf(buf, "%d(%d)", (int) p_ptr->chp, (int) p_ptr->mhp);
    c_put_str(TERM_L_BLUE, buf, 7, 13);
    (void) sprintf(buf, "%d(%d)", (int) p_ptr->csp, (int) p_ptr->msp);
	c_put_str(TERM_L_BLUE, buf, 8, 13);	
}


/*
 * Calculate the modifier to stat i from equipment
 */
int equip_mod(int i)
{
	/* Calculate equipment adjustment */
	int e_adj = 0;
      
	/* Icky formula to deal with the 18 barrier */
	if ((p_ptr->stat_max[i]>18) && (p_ptr->stat_top[i]>18))
		e_adj = (p_ptr->stat_top[i] - p_ptr->stat_max[i])/10;
	if ((p_ptr->stat_max[i]<=18) && (p_ptr->stat_top[i]<=18))
		e_adj = p_ptr->stat_top[i] - p_ptr->stat_max[i];
	if ((p_ptr->stat_max[i]<=18) && (p_ptr->stat_top[i]>18))
		e_adj = (p_ptr->stat_top[i] - 18)/10 - p_ptr->stat_max[i] + 18;

        if ((p_ptr->stat_max[i]>18) && (p_ptr->stat_top[i]<=18))
            e_adj = p_ptr->stat_top[i] - (p_ptr->stat_max[i]-18)/10 - 19;
      
	/* Deduct template and race bonuses if in maximize */
	if (maximise_mode)
	{
		e_adj -= rp_ptr->r_adj[i];
		e_adj -= cp_ptr->c_adj[i];
	}
	return e_adj;
}

/*
 * Special display, part 2b
 * 
 * How to print out the modifications and sustains.
 * Positive mods with no sustain will be light green.
 * Positive mods with a sustain will be dark green.
 * Sustains (with no modification) will be a dark green 's'.
 * Negative mods (from a curse) will be red.
 * Huge mods (>9), like from MICoMorgoth, will be a '*'
 * No mod, no sustain, will be a slate '.'
 */
static void display_player_stat_info(void)
{
	int i, e_adj;
	int stat_col, stat;
	int row, col;

	object_type *o_ptr;
	u32b f1, f2, f3;
	s16b k_idx;

	byte a;
	char c;

	char buf[80];


	/* Column */
	stat_col = 24;

	/* Row */
	row = 3;

	/* Print out the labels for the columns */
	c_put_str(TERM_WHITE, "Stat", row-1, stat_col);
	c_put_str(TERM_BLUE, "Intrnl", row-1, stat_col+5);
    c_put_str(TERM_L_BLUE, "Rce Cls Mod", row-1, stat_col+12);
	c_put_str(TERM_L_GREEN, "Actual", row-1, stat_col+24);
	c_put_str(TERM_YELLOW, "Currnt", row-1, stat_col+31);
  
	/* Display the stats */
	for (i = 0; i < 6; i++)
	{
		/* Calculate equipment adjustment */
		e_adj = equip_mod(i);

		/* Reduced name of stat */
		c_put_str(TERM_WHITE, stat_names_reduced[i], row+i, stat_col);
      
		/* Internal "natural" max value.  Maxes at 18/100 */
		/* This is useful to see if you are maxed out     */
		strnfmt(buf, sizeof(buf), "%v", cnv_stat_f1, p_ptr->stat_max[i]);
		c_put_str(TERM_BLUE, buf, row+i, stat_col+5);
      
		/* Race, template, and equipment modifiers */
		(void) sprintf(buf, "%3d", (int) rp_ptr->r_adj[i]);
		c_put_str(TERM_L_BLUE, buf, row+i, stat_col+12);
		(void) sprintf(buf, "%3d", (int) cp_ptr->c_adj[i]);
		c_put_str(TERM_L_BLUE, buf, row+i, stat_col+16);
		(void) sprintf(buf, "%3d", (int) e_adj);
		c_put_str(TERM_L_BLUE, buf, row+i, stat_col+20);
      
		/* Actual maximal modified value */
		strnfmt(buf, sizeof(buf), "%v", cnv_stat_f1, p_ptr->stat_top[i]);
		c_put_str(TERM_L_GREEN, buf, row+i, stat_col+24);
      
		/* Only display stat_use if not maximal */
		if (p_ptr->stat_use[i] < p_ptr->stat_top[i])
		{
			strnfmt(buf, sizeof(buf), "%v", cnv_stat_f1, p_ptr->stat_use[i]);
			c_put_str(TERM_YELLOW, buf, row+i, stat_col+31);
		}
	}

	/* Column */
	col = stat_col + 39;

	/* Header and Footer */
	c_put_str(TERM_WHITE, "abcdefghijkl@", row-1, col);
	c_put_str(TERM_L_GREEN, "Modifications", row+6, col);

	/* Process equipment */
	for (i=INVEN_WIELD; i<INVEN_POUCH_1; i++)
	{
		/* Access object */
		o_ptr = &inventory[i];

		/* Object kind */
		k_idx = o_ptr->k_idx;

		/* Acquire "known" flags */
		object_flags_known(o_ptr, &f1, &f2, &f3);

		/* Initialize color based of sign of pval. */
		for (stat=0; stat<6; stat++)
		{
			/* Default */
			a = TERM_SLATE;
			c = '.';

			/* Boost */
			if (f1 & 1<<stat)
			{
				/* Default */
				c = '*';

				/* Good */
				if (o_ptr->pval > 0)
				{
					/* Good */
					a = TERM_L_GREEN;

					/* Label boost */
					if (o_ptr->pval < 10) c = '0' + o_ptr->pval;
				}
				
				/* Bad */
				if (o_ptr->pval < 0)
				{
					/* Bad */
					a = TERM_RED;

					/* Label boost */
					if (o_ptr->pval < 10) c = '0' - o_ptr->pval;
				}
			}

			/* Sustain */
			if (f2 & 1<<stat)
			{
				/* Dark green "s" */
				a = TERM_GREEN;
				if (c == '.')
					c = 's';
			}

			/* Dump proper character */
			Term_putch(col, row+stat, a, c);
		}

		/* Advance */
		col++;
    }

	/* Player flags */
	player_flags(&f1, &f2, &f3);

	/* Check stats */
	for (stat=0; stat<6; stat++)
	{
		int dummy = 0;
		/* Default */
		a = TERM_SLATE;
		c = '.';

        /* Chaos Features ... */
			if (stat == A_STR)
            {
                if (p_has_mutation(MUT_HYPER_STR)) dummy += 4;
                if (p_has_mutation(MUT_PUNY))   dummy -= 4;
            }
            else if (stat == A_WIS || stat == A_INT)
            {
                if (p_has_mutation(MUT_HYPER_INT)) dummy += 4;
                if (p_has_mutation(MUT_MORONIC)) dummy -= 4;
            }
            else if (stat == A_DEX)
            {
				if (p_has_mutation(MUT_IRON_SKIN)) dummy -= 1;
				if (p_has_mutation(MUT_LIMBER)) dummy += 3;
				if (p_has_mutation(MUT_ARTHRITIS)) dummy -= 3;
            }
            else if (stat == A_CON)
            {
                if (p_has_mutation(MUT_RESILIENT)) dummy += 4;
                if (p_has_mutation(MUT_XTRA_FAT)) dummy += 2;
                if (p_has_mutation(MUT_ALBINO)) dummy -= 4;
                if (p_has_mutation(MUT_FLESH_ROT)) dummy -= 2;
            }
            else if (stat == A_CHR)
            {
                if (p_has_mutation(MUT_SILLY_VOI)) dummy -= 4;
                if (p_has_mutation(MUT_BLANK_FAC)) dummy -= 1;
                if (p_has_mutation(MUT_FLESH_ROT)) dummy -= 1;
                if (p_has_mutation(MUT_SCALES)) dummy -= 1;
                if (p_has_mutation(MUT_WART_SKIN)) dummy -= 2;
				if (p_has_mutation(MUT_ILL_NORM)) dummy = 0;
            }


			/* Boost */
            if (dummy)
			{
				/* Default */
				c = '*';

				/* Good */
                if (dummy > 0)
				{
					/* Good */
					a = TERM_L_GREEN;

					/* Label boost */
                    if (dummy < 10) c = '0' + dummy;
				}
				
				/* Bad */
                if (dummy < 0)
				{
					/* Bad */
					a = TERM_RED;

					/* Label boost */
                    if (dummy < 10) c = '0' - dummy;
				}
			}


		/* Sustain */
		if (f2 & 1<<stat)
		{
			/* Dark green "s" */
			a = TERM_GREEN;
			if (c == '.')
				c = 's';
		}


		/* Dump */
		Term_putch(col, row+stat, a, c);
    }
}

/*
 * Display how race and class affect stats, but compressed to fit into type
 * 1 display.
 * Also display name, sex race and template (but not age, height, etc.).
 */
static void display_player_stat_info_birth(void)
{
	int i, e_adj;
	int stat_col;
	int row;

	char buf[80];

	/* Name, Sex, Race, template */
	put_str("Name        :", 2, 1);
	put_str("Sex         :", 3, 1);
	put_str("Race        :", 4, 1);
	put_str("Template    :", 5, 1);

	c_put_str(TERM_L_BLUE, player_name, 2, 15);
	c_put_str(TERM_L_BLUE, sp_ptr->title, 3, 15);
	c_put_str(TERM_L_BLUE, rp_ptr->title, 4, 15);
	c_put_str(TERM_L_BLUE, cp_ptr->title, 5, 15);

	/* Column */
	stat_col = 42;

	/* Row */
	row = 2;

	/* Display the stats */
	for (i = 0; i < 6; i++)
	{
		/* Calculate equipment adjustment (not always 0) */
		e_adj = equip_mod(i);

		/* Reduced name of stat */
		c_put_str(TERM_WHITE, stat_names_reduced[i], row+i, stat_col);
      
		/* Internal "natural" max value.  Maxes at 18/100 */
		/* This is useful to see if you are maxed out     */
		strnfmt(buf, sizeof(buf), "%v", cnv_stat_f1, p_ptr->stat_max[i]);
		c_put_str(TERM_BLUE, buf, row+i, stat_col+5);
      
		/* Race, template, and equipment modifiers */
		(void) sprintf(buf, "%3d", (int) rp_ptr->r_adj[i]);
		c_put_str(TERM_L_BLUE, buf, row+i, stat_col+12);
		(void) sprintf(buf, "%3d", (int) cp_ptr->c_adj[i]);
		c_put_str(TERM_L_BLUE, buf, row+i, stat_col+16);
		(void) sprintf(buf, "%3d", (int) e_adj);
		c_put_str(TERM_L_BLUE, buf, row+i, stat_col+20);
      
		/* Actual maximal modified value */
		strnfmt(buf, sizeof(buf), "%v", cnv_stat_f1, p_ptr->stat_top[i]);
		c_put_str(TERM_L_GREEN, buf, row+i, stat_col+24);
      
		/* Only display stat_use if not maximal */
		if (p_ptr->stat_use[i] < p_ptr->stat_top[i])
		{
			strnfmt(buf, sizeof(buf), "%v", cnv_stat_f1, p_ptr->stat_use[i]);
			c_put_str(TERM_YELLOW, buf, row+i, stat_col+31);
		}
	}
}

/*
 * Object flag names
 */
static cptr object_flag_names[96] =
{
	"Add Str",
	"Add Int",
	"Add Wis",
	"Add Dex",
	"Add Con",
	"Add Chr",
	NULL,
	NULL,
	"Add Stea.",
	"Add Sear.",
	"Add Infra",
	"Add Tun..",
	"Add Speed",
	"Add Blows",
    "Chaotic",
    "Vampiric",
	"Slay Anim.",
	"Slay Evil",
	"Slay Und.",
	"Slay Demon",
	"Slay Orc",
	"Slay Troll",
	"Slay Giant",
	"Slay Drag.",
	"Kill Drag.",
    "Sharpness",
	"Impact",
    "Poison Brd",
	"Acid Brand",
	"Elec Brand",
	"Fire Brand",
	"Cold Brand",

	"Sust Str",
	"Sust Int",
	"Sust Wis",
	"Sust Dex",
	"Sust Con",
	"Sust Chr",
	"Rand Res",
	"Rand Pwr",
	"Imm Acid",
	"Imm Elec",
	"Imm Fire",
	"Imm Cold",
	"Rand R/P",
    "Reflect",
	"Free Act",
	"Hold Life",
	"Res Acid",
	"Res Elec",
	"Res Fire",
	"Res Cold",
	"Res Pois",
    "Res Fear",
	"Res Lite",
	"Res Dark",
	"Res Blind",
	"Res Conf",
	"Res Sound",
	"Res Shard",
	"Res Neth",
	"Res Nexus",
	"Res Chaos",
	"Res Disen",



    "Aura Fire",

    "Aura Elec",
 	NULL,
	"AutoCurse",
    "NoTeleport",
    "AntiMagic",
    "WraithForm",
    "EvilCurse",
 	"Easy Know",
 	"Hide Type",
	"Show Mods",
	"Insta Art",
    "Levitate",
	"Lite",
	"See Invis",
	"Telepathy",
	"Digestion",
	"Regen",
	"Xtra Might",
	"Xtra Shots",
	"Ign Acid",
	"Ign Elec",
	"Ign Fire",
	"Ign Cold",
	"Activate",
	"Drain Exp",
	"Teleport",
	"Aggravate",
	"Blessed",
	"Cursed",
	"Hvy Curse",
	"Prm Curse"
};

/*
 * Return the flags the player is assumed to know his character to have.
 */
static void player_flags_known(u32b *f1, u32b *f2, u32b *f3)
{
	object_type *o_ptr;

	/* Assume the racial flags are known. */
	player_flags(f1, f2, f3);

	/* Check the equipment. */
	for (o_ptr = inventory+INVEN_WIELD; o_ptr <= inventory+INVEN_FEET; o_ptr++)
	{
		object_type j_ptr[1];

		/* Known object things. */
		object_info_known(j_ptr, o_ptr);

		/* Add the flags. */
		(*f1) |= j_ptr->flags1;
		(*f2) |= j_ptr->flags2;
		(*f3) |= j_ptr->flags3;
	}
}

/*
 * Return TRUE if the player knows his character possesses one of the
 * indicated flags in the given variable.
 */
bool PURE player_has_flag_known(int set, u32b flag)
{
	u32b f[3];
	assert(set > 0 && set < 4);
	player_flags_known(f, f+1, f+2);
	return (f[set-1] & flag) != 0;
}

/*
 * Summarize resistances
 */
static void display_player_ben(void)
{
	int x, y;
	
	u32b f[3];

	/* Get the known flags. */
	player_flags_known(f, f+1, f+2);

	/* Scan cols */
	for (x = 0; x < 6; x++)
	{
		/* Split each variable in two. */
		u32b b = f[x/2];
		if (x % 2) b >>= 16;

		/* Scan rows */
		for (y = 0; y < 16; y++)
		{
			byte a = TERM_SLATE;
			char c = '.';

			cptr name = object_flag_names[16*x+y];

			/* No name */
			if (!name) continue;

			/* Dump name */
			Term_putstr(x * 13, y + 4, -1, TERM_WHITE, name);

			/* Dump colon */
			Term_putch(x * 13 + 10, y + 4, TERM_WHITE, ':');

			/* Check flag */
			if (b & (1<<y))
			{
				a = TERM_WHITE;
				c = '+';
			}

			/* Dump flag */
			Term_putch(x * 13 + 11, y + 4, a, c);
		}
	}
}


/*
 * Summarize resistances
 */
static void display_player_ben_one(int mode)
{
	int i, n, x, y;
	
	object_type *o_ptr;

	u32b f1, f2, f3;

	u16b b[13][6];


	/* Scan equipment */
	for (i = INVEN_WIELD; i < INVEN_POUCH_1; i++)
	{
		/* Index */
		n = (i - INVEN_WIELD);

		/* Object */
		o_ptr = &inventory[i];

		/* Known object flags */
		object_flags_known(o_ptr, &f1, &f2, &f3);

		/* Incorporate */
		b[n][0] = (short)(f1 & 0xFFFF); 
		b[n][1] = (short)(f1 >> 16);
		b[n][2] = (short)(f2 & 0xFFFF);
		b[n][3] = (short)(f2 >> 16);
		b[n][4] = (short)(f3 & 0xFFFF);
		b[n][5] = (short)(f3 >> 16);
	}


	/* Index */
	n = 12;

	/* Player flags */
	player_flags(&f1, &f2, &f3);
	
	/* Incorporate */
	b[n][0] = (short)(f1 & 0xFFFF);
	b[n][1] = (short)(f1 >> 16);
	b[n][2] = (short)(f2 & 0xFFFF);
	b[n][3] = (short)(f2 >> 16);
	b[n][4] = (short)(f3 & 0xFFFF);
	b[n][5] = (short)(f3 >> 16);


	/* Scan cols */
	for (x = 0; x < 3; x++)
	{
		/* Equippy */
		display_player_equippy(2, x * 26 + 11);

		/* Label */
		Term_putstr(x * 26 + 11, 3, -1, TERM_WHITE, "abcdefghijkl@");

		/* Scan rows */
		for (y = 0; y < 16; y++)
		{
			cptr name = object_flag_names[48*mode+16*x+y];

			/* No name */
			if (!name) continue;

			/* Dump name */
			Term_putstr(x * 26, y + 4, -1, TERM_WHITE, name);

			/* Dump colon */
			Term_putch(x * 26 + 10, y + 4, TERM_WHITE, ':');

			/* Check flags */
			for (n = 0; n < 13; n++)
			{
				byte a = TERM_SLATE;
				char c = '.';

				/* Check flag */
				if (b[n][3*mode+x] & (1<<y))
				{
					a = TERM_WHITE;
					c = '+';
				}

				/* Dump flag */
				Term_putch(x * 26 + 11 + n, y + 4, a, c);
			}
		}
	}
}

/*
 * Display the name, sex, race, template and stats.
 */
static void display_player_name_stats(void)
{
	char	buf[80];
	byte i;

		/* Name, Sex, Race, template */
		put_str("Name        :", 2, 1);
		put_str("Sex         :", 3, 1);
		put_str("Race        :", 4, 1);
		put_str("Template    :", 5, 1);

		c_put_str(TERM_L_BLUE, player_name, 2, 15);
		c_put_str(TERM_L_BLUE, sp_ptr->title, 3, 15);
		c_put_str(TERM_L_BLUE, rp_ptr->title, 4, 15);
		c_put_str(TERM_L_BLUE, cp_ptr->title, 5, 15);

		/* Age, Height, Weight, Social, Birthday */
		prt_num("Age          ", (int)p_ptr->age, 2, 32, TERM_L_BLUE);
		prt_num("Height       ", (int)p_ptr->ht, 3, 32, TERM_L_BLUE);
		prt_num("Weight       ", (int)p_ptr->wt, 4, 32, TERM_L_BLUE);
		prt_num("Social Class ", (int)p_ptr->sc, 5, 32, TERM_L_BLUE);
		put_str("Birthday",6,32);
		c_put_str(TERM_L_BLUE,format("%v", day_to_date_f1, p_ptr->birthday),
			6,48);

		/* Display the stats */
		for (i = 0; i < 6; i++)
		{
			/* Special treatment of "injured" stats */
			if (p_ptr->stat_cur[i] < p_ptr->stat_max[i])
			{
				int value;

				/* Use lowercase stat name */
				put_str(stat_names_reduced[i], 2 + i, 61);

				/* Get the current stat */
				value = p_ptr->stat_use[i];

				/* Obtain the current stat (modified) */
				strnfmt(buf, sizeof(buf), "%v", cnv_stat_f1, p_ptr->stat_max[i]);

				/* Display the current stat (modified) */
				c_put_str(TERM_YELLOW, buf, 2 + i, 66);

				/* Acquire the max stat */
				value = p_ptr->stat_top[i];

				/* Obtain the maximum stat (modified) */
				strnfmt(buf, sizeof(buf), "%v", cnv_stat_f1, value);

				/* Display the maximum stat (modified) */
				c_put_str(TERM_L_GREEN, buf, 2 + i, 73);
			}

			/* Normal treatment of "normal" stats */
			else
			{
				/* Assume uppercase stat name */
				put_str(stat_names[i], 2 + i, 61);

				/* Obtain the current stat (modified) */
				strnfmt(buf, sizeof(buf), "%v", cnv_stat_f1, p_ptr->stat_use[i]);

				/* Display the current stat (modified) */
				c_put_str(TERM_L_GREEN, buf, 2 + i, 66);
			}
		}
}

/*
 * Display the character on the screen (various modes)
 *
 * The top two and bottom two lines are left blank.
 *
 * Mode 0 = standard display with skills
 * Mode 1 = standard display with history and bonus details
 * Mode 2 = skills display
 * Mode 3 = summary of various things
 * Mode 4 = current flags (combined)
 * Mode 5 = current flags (part 1)
 * Mode 6 = current flags (part 2)
 * Mode 7 = chaos features
 */
void display_player(int mode)
{
	/* XXX XXX XXX */
    if (p_mutated() && !(skip_chaos_features))
        mode = (mode % 8);
    else
        mode = (mode % 7);

	/* Erase screen */
	clear_from(0);

	switch (mode)
	{
		case -1: /* During character creation only. */
		{
			/* Name, sex, race, template and stats (compressed) */
			display_player_stat_info_birth();

			/* Extra info (missile) */
			display_player_sides(TRUE);

			/* Experience */
			display_player_xp();
			break;
		}
		case 0:
		{
			/* Name, sex, race, template, age, height, weight, social class and stats. */
			display_player_name_stats();

			/* Extra info (melee) */
			display_player_sides(FALSE);

			/* Temporary bonuses */
			if (!display_player_bonus())

			/* Experience if no temporary bonuses */
			display_player_xp();

			/* Display "various" info */
			put_str("(Miscellaneous Abilities)", 15, 25);
			display_player_various();

			break;
		}
		case 1:
		{
			byte i;

			/* Name, sex, race, template, age, height, weight, social class and stats. */
			display_player_name_stats();

			/* Extra info (missile) */
			display_player_sides(TRUE);

			/* Experience */
			display_player_xp();

		/* Display "history" info */
			put_str("(Character Background)", 15, 25);

			for (i = 0; i < 4; i++)
			{
				put_str(history[i], i + 16, 10);
			}

			break;
	}
		case 2:
	{
		/* Do nothing */
		display_player_skills();
			break;
	}
		case 3: /* Special */
	{
		/* See "http://www.cs.berkeley.edu/~davidb/angband.html" */

		/* Dump the info */
		display_player_misc_info();
		display_player_stat_info();
		display_player_flag_info();
			break;
	}
		case 4: /* Special */
	{
		display_player_ben();
			break;
	}
		case 7:
    {
        do_cmd_knowledge_chaos_features();
			break;
    }
		default: /* Special */
	{
		display_player_ben_one((mode-1) % 2);
	}
}
}

static void dump_final_messages(FILE * OutFile)
{
	short i;

	if (!OutFile) return;

	/* Reverse order */
	for(i=9;i>=0;i--)
	{
		fprintf(OutFile,"%s\n",message_str(i));
	}
}

/*
 * Return whether the player has some equipment.
 */
static bool equip_cnt(void)
{
	int i;
	for (i = INVEN_WIELD; i <= INVEN_POUCH_6; i++)
	{
		if (inventory[i].k_idx) return TRUE;
	}
	return FALSE;
}

/*
 * Return whether the player has some inventory.
 */
static bool inven_cnt(void)
{
	int i;
	for (i = 0; i < INVEN_PACK; i++)
	{
		if (inventory[i].k_idx) return TRUE;
	}
	return FALSE;
}

/*
 * Hack -- Dump a character description file
 *
 * XXX XXX XXX Allow the "full" flag to dump additional info,
 * and trigger its usage from various places in the code.
 */
errr file_character(cptr name)
{
	int			i, x, y;

	byte		a;
	char		c;

	cptr		paren = ")";

	int			fd = -1;

	FILE		*fff = NULL;

	char		buf[1024];


	/* Drop priv's */
	safe_setuid_drop();

	/* Build the filename */
	strnfmt(buf, 1024, "%v", path_build_f2, ANGBAND_DIR_USER, name);

	/* File type is "TEXT" */
	FILE_TYPE(FILE_TYPE_TEXT);

	/* Check for existing file */
	fd = fd_open(buf, O_RDONLY);

	/* Existing file */
	if (fd >= 0)
	{
		char out_val[160];

		/* Close the file */
		(void)fd_close(fd);

		/* Build query */
		(void)sprintf(out_val, "Replace existing file %s? ", buf);

		/* Ask */
		if (get_check(out_val)) fd = -1;
	}

	/* Open the non-existing file */
	if (fd < 0) fff = my_fopen(buf, "w");

	/* Grab priv's */
	safe_setuid_grab();


	/* Invalid file */
	if (!fff)
	{
		/* Message */
		msg_print("Character dump failed!");
		msg_print(NULL);

		/* Error */
		return (-1);
	}


	/* Begin dump */
	fprintf(fff, "  [%s %s Character Dump]\n\n", GAME_NAME, GAME_VERSION);


	/* Display player */
	display_player(0);

	/* Dump part of the screen */
	for (y = 2; y < 22; y++)
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

	/* Show Skills */
	display_player(2);

	/* Dump part of the screen */
	for (y = 1; y < 22; y++)
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


	/* Display history */
	display_player(1);

	/* Dump part of the screen */
	for (y = 15; y < 20; y++)
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


        fprintf(fff, "\n\n  [Miscellaneous information]\n");
        if (ironman_shop)
            fprintf(fff, "\n Ironman Shops:     ON");
        else
            fprintf(fff, "\n Ironman Shops:     OFF");

        if (maximise_mode)
            fprintf(fff, "\n Maximize Mode:      ON");
        else
            fprintf(fff, "\n Maximize Mode:      OFF");

        if (preserve_mode)
            fprintf(fff, "\n Preserve Mode:      ON");
        else
            fprintf(fff, "\n Preserve Mode:      OFF");

        if (auto_scum)
            fprintf(fff, "\n Autoscum:           ON");
        else
            fprintf(fff, "\n Autoscum:           OFF");

		if (dungeon_small)
			fprintf(fff, "\n Small Levels:       Always");
		else
		{
			if (small_levels)
				fprintf(fff, "\n Small Levels:       ON");
			else
				fprintf(fff, "\n Small Levels:       OFF");
		}

        if (empty_levels)
            fprintf(fff, "\n Arena Levels:       ON");
        else
            fprintf(fff, "\n Arena Levels:       OFF");

        if (multi_stair)
            fprintf(fff, "\n Long Stairs:       ON");
        else
            fprintf(fff, "\n Long Stairs:       OFF");

		i = p_ptr->max_dlv+dun_defs[cur_dungeon].offset;
		fprintf(fff, "\n Recall Depth:       Level %d (%d')\n", i, 50 *i);


        if (noscore)
            fprintf(fff, "\n You have done something illegal.");

        if (stupid_monsters)
            fprintf(fff, "\n Your opponents are behaving stupidly.");


    { /* Monsters slain */
        int k;
        s32b Total = 0;

        for (k = 1; k < MAX_R_IDX; k++)
        {
            monster_race *r_ptr = &r_info[k];

			/* Skip "fake" monsters. */
			if (is_fake_monster(r_ptr)) continue;

            if (r_ptr->flags1 & (RF1_UNIQUE))
            {
                bool dead = (r_ptr->max_num == 0);
                if (dead)
                {
                    Total++;
                }
            }
            else
            {
                s16b This = r_ptr->r_pkills;
                if (This > 0)
                {
                    Total += This;
                }
            }
        }

		for (i = y = 0; i < MAX_CAVES; i++)
		{
			dun_type *d_ptr = dun_defs+i;
			quest_type *q_ptr;
			bool quests_exist = FALSE;
			
			for (q_ptr = q_list; q_ptr < q_list+z_info->quests; q_ptr++)
			{
				/* Skip quests elsewhere. */
				if (q_ptr->dungeon != i) continue;

				/* Unfinished quests found. */
				if (q_ptr->cur_num < q_ptr->max_num) goto next_cave;

				/* Found a finished quest. */
				quests_exist = TRUE;
			}

			/* No quests found. */
			if (!quests_exist) continue;

			/* Put a gap before and after the section. */
			if (!y) fprintf(fff, "\n");
			y = 1;

			/* Print a message. */
			fprintf(fff, " You have conquered %s.\n", dun_name+d_ptr->name);

next_cave:
			continue; /* Don't ask me, it just prevents a GCC warning. */
		}

		fprintf(fff, "\n");

		for (i = y = 0; i < z_info->quests; i++)
		{
			if (q_list[i].cur_num == q_list[i].max_num) y++;
		}
		if (y) fprintf(fff, " You have completed %d quests.", y);

        if (Total < 1)
            fprintf(fff," You have defeated no enemies yet.\n");
        else if (Total == 1)

            fprintf(fff," You have defeated one enemy.\n");
        else
           fprintf(fff,"\n You have defeated %ld enemies.\n", Total);
    }
    

    if (p_mutated())
    {
        fprintf(fff, "\n\n  [Chaos Features]\n\n");
        dump_chaos_features(fff);
    }


	/* Skip some lines */
	fprintf(fff, "\n\n");


	/* Dump the equipment */
	if (equip_cnt())
	{
		fprintf(fff, "  [Character Equipment]\n\n");
		for (i = INVEN_WIELD; i < INVEN_TOTAL; i++)
		{
			my_fprintf(fff, "%c%s %v\n", index_to_label(&inventory[i]), paren,
				object_desc_f3, &inventory[i], OD_ART | OD_NOHIDE, 3);

			/* Describe random object attributes */
			identify_fully_file(inventory+i, fff);
		}
		fprintf(fff, "\n\n");
	}

	/* Dump the inventory */
	fprintf(fff, "  [Character Inventory]\n\n");
	for (i = 0; i < INVEN_PACK && inventory[i].k_idx; i++)
	{
		my_fprintf(fff, "%c%s %v\n", index_to_label(&inventory[i]), paren,
			object_desc_f3, &inventory[i], OD_ART | OD_NOHIDE, 3);

		/* Describe random object attributes */
		identify_fully_file(inventory+i, fff);
	}
	fprintf(fff, "\n\n");

	/* Dump each home in turn */
	for (x = 0; x < MAX_STORES_TOTAL; x++)
	{
		store_type *st_ptr = store+x;
		int town = x/MAX_STORES_PER_TOWN;

		/* Not your home. */
		if (!st_ptr->bought) continue;

		/* Empty */
		if (!st_ptr->stock_num)
		{
			fprintf(fff, "  [Home in %s empty]\n\n", dun_name+dun_defs[town].shortname);
			continue;
		}
		/* Dump the contents */
		fprintf(fff, "[Home Inventory (%s)]\n\n", dun_name+dun_defs[town].shortname);
		for (i = 0; i < st_ptr->stock_num; i++)
		{
			my_fprintf(fff, "%c%s %v\n", I2A(i%12), paren,
				object_desc_f3, &st_ptr->stock[i], OD_ART | OD_NOHIDE, 3);

			/* Describe random object attributes */
			identify_fully_file(&st_ptr->stock[i], fff);
		}
		fprintf(fff, "\n");
	}

	fprintf(fff, "  [Last Ten Messages]\n\n");
	dump_final_messages(fff);

	/* Close it */
	my_fclose(fff);


	/* Message */
	msg_print("Character dump successful.");
	msg_print(NULL);

	/* Success */
	return (0);
}



/*
 * Peruse the On-Line-Help, starting at the given file.
 */
void do_cmd_help(cptr name)
{
	/* Hack -- default file */
	if (!name) name = syshelpfile;

	/* Peruse the help file. */
	show_link(name);
}






/*
 * Recursive file perusal.
 *
 * Return FALSE on "ESCAPE", otherwise TRUE.
 *
 * Process various special text in the input file, including
 * the "menu" structures used by the "help file" system.
 *
 * XXX XXX XXX Consider using a temporary file.
 *
 * XXX XXX XXX Allow the user to "save" the current file.
 */

/*
 * Convert a "color letter" into an "actual" color
 * The colors are taken from the atchar table.
 */
int color_char_to_attr(char c)
{
	uint i;
	for (i = 0; atchar[i]; i++)
	{
		if (c == atchar[i]) return i;
	}
	return (-1);
}


typedef struct link_det link_det;
struct link_det
{
	char tag[32];
	char key;
	int x;
	int y;
	int line;
};

/*
 * A structure to hold (some of == XXX) the hyperlink information.
 * This prevents excessive use of stack.
 */
typedef struct hyperlink hyperlink_type;
struct hyperlink
{
	/* Path buffer */
	char path[1024];

	/* General buffer */
	char rbuf[1024];

	/* Hold a string to find */
	char finder[81];

	/* Hold a string to show */
	char shower[81];

	/* Describe this thing */
	char caption[128];

	/* Hypertext info */
	link_det link[400];

	int cur_link;	/* Currently selected link. */
	int max_link;	/* Number of links known. */
};

static cptr *help_files = NULL;

typedef struct link_type link_type;
struct link_type
{
	cptr str; /* The line which includes the links. */
	cptr file; /* The file the links appear in. */
	long pos; /* The position in the file of the line after the link. */
};

static link_type *links = NULL;
static int num_links;

#define MAX_LINKS 1024

/* The option currently selected */
#define MAX_HELP_STRS	5
#define CUR_HELP_STR	help_str_list[help_strs-1]
static cptr help_str_list[MAX_HELP_STRS];
static int help_strs = 0;

/*
 * Return the currently selected help string.
 */
cptr cur_help_str(void)
{
	if (help_strs) return CUR_HELP_STR;
	else return NULL;
}

/*
 * Remember or forget a help request. Only the latest one is
 * currently processed.
 */
void help_track(cptr str)
{
	/* Remove one. */
	if (!str)
	{
		if (help_strs) help_strs--;
	}
	else
	{
		/* Too many strings memorised. */
		if (help_strs == MAX_HELP_STRS)
		{
			int i;
			for (i = 1; i < MAX_HELP_STRS; i++)
			{
				help_str_list[i-1] = help_str_list[i];
			}
		}
		else
		{
			help_strs++;
		}
	
		/* Set the current string */
		CUR_HELP_STR = str;
	}

	/* Window stuff */
	if (!is_keymap_or_macro()) p_ptr->window |= PW_HELP;
}

/*
 * Attempt to open a file in a standard Angband directory for show_file_aux().
 */
static FILE *show_fopen(hyperlink_type *h_ptr, cptr path, cptr name)
{
	FILE *fff;

	/* Build the filename */
	strnfmt(h_ptr->path, 1024, "%v", path_build_f2, path, name);

	/* Grab permission */
	safe_setuid_grab();

	/* Open the file */
	fff = my_fopen(h_ptr->path, "r");

	/* Drop permission */
	safe_setuid_drop();

	return fff;
}

/*
 * Copy a reflowed version of a file to a temporary file, return a pointer to
 * the latter and store its name in h_ptr->path.
 *
 * It doesn't use any of the display code to find out how much it will fit on
 * a line, but it probably should...
 */
static FILE *reflow_file(FILE *fff, hyperlink_type *h_ptr)
{
	bool skip;
	cptr s;
	int w,h,space;
	int x;
	FILE *ftmp = my_fopen_temp(h_ptr->path, sizeof(h_ptr->path));
	Term_get_size(&w, &h);

	/* Don't use my_fgets as that needs short lines and it will process the
	 * output file anyway. */
	for (x = 0, skip = FALSE; !my_fgets_long(h_ptr->rbuf, 1024, fff); )
	{
		/* Interpret conditions. */
		if (prefix(h_ptr->rbuf, CC_IF_PREFIX))
		{
			skip = !process_pref_file_expr_p(h_ptr->rbuf+strlen(CC_IF_PREFIX));
			continue;
		}

		/* Skip excluded lines. */
		if (skip) continue;

		for (s = h_ptr->rbuf; *s;)
		{
			if ((int)strlen(s) < w-x) break;

			for (h = 0, space = 0; h < w-x; h++)
			{
				/* Allow extra space for hyperlinks. */
				if (prefix(s+h, "*****"))
				{
					uint i = strcspn(s+h, "[");
					x -= i;
					h += i;
				}
				if (s[h] == ' ') space = h;
			}

			if ((int)strlen(s) < w-x) break;

			/* If there are no spaces, print the line. */
			if (!space) space = w-x-1;

			fprintf(ftmp, "%.*s\n", space, s);
			s += space+1;
			x = 0;
		}

		/* Print the end of the line. */
		fprintf(ftmp, "%s", s);

		/* If the buffer was shorter than the line, only take a short
		 * line as the first part of the next line. */
		x = (strchr(s, '\n')) ? 0 : strlen(s);
	}
	my_fclose(fff);
	my_fclose(ftmp);

	/* Deal with the temporary file from now on. */
	fff = ftmp;

	/* Grab permission */
	safe_setuid_grab();

	/* Open the temporary file for reading. */
	fff = my_fopen(h_ptr->path, "r");

	/* Drop permission */
	safe_setuid_drop();

	return fff;
}

/*
 * Find the specified text in a file after a point, return the line
 * at which it occurs.
 */
static int find_text(FILE *fff, hyperlink_type *h_ptr, int minline)
{
	int y;

	/* This are trivial without a search string. */
	if (!h_ptr->finder[0]) return minline;

	/* Start at the beginning. */
	rewind(fff);

	/* Find and skip the indicated line. */
	for (y = 0; y <= minline; y++)
	{
		/* Skip a line */
		if (my_fgets(fff, h_ptr->rbuf, 1024)) return minline;
	}

	/* Search from it. */
	while (!my_fgets(fff, h_ptr->rbuf, 1024))
	{
		if (strstr(h_ptr->rbuf, h_ptr->finder)) return y;
		y++;
	}

	/* No match, so do nothing. */
	return minline;
}

/*
 * Return TRUE if s is a non-printed line which is preserved by reflow_file().
 */
static bool fake_line(cptr s)
{
	if (prefix(s, CC_LINK_PREFIX)) return TRUE;
	if (prefix(s, "|||||")) return TRUE;
	return FALSE;
}

/*
 * Show a section of a help file between stated limits.
 */
static void show_page(FILE *fff, hyperlink_type *h_ptr, int miny, int maxy, int minline)
{
	char link_colour = 'W', link_colour_sel = 's';

	int y, l;
	char sbuf[1024];
	cptr buf;

	/* Output buffer (the code below never converts strings to longer ones,
	 * so 1024 is always enough). */
	char out_buf[1024], *out_ptr;

	/* Start at the beginning. */
	rewind(fff);

	/* Find the indicated line. */
	for (y = 0, buf = sbuf; y < minline; y++)
	{
		/* Skip a line */
		if (my_fgets(fff, sbuf, 1024)) break;

		/* Get the link colors */
		if (prefix(buf, "|||||"))
		{
			link_colour = buf[5];
			link_colour_sel = buf[6];
		}

		/* Ignore unprinted lines. */
		if (fake_line(buf)) y--;
	}

	Term_gotoxy(0,miny);

	/* Get a cur_link which is on screen if possible. */
	while ((h_ptr->cur_link > 0) &&
		(h_ptr->link[h_ptr->cur_link].y >= minline + (maxy - miny + 1)))
	{
		h_ptr->cur_link--;
	}
	while ((h_ptr->cur_link < h_ptr->max_link) &&
		(h_ptr->link[h_ptr->cur_link].y < minline))
	{
		h_ptr->cur_link++;
	}

	/* Dump the next 20 (or more in bigscreen) lines of the file */
	for (y = miny, l = minline; y <= maxy; l++)
	{
		cptr s;
		out_ptr = out_buf;

		/* Get a line of the file or stop */
		if (my_fgets(fff, sbuf, 1024)) break;

		/* Get a color */
		if (prefix(sbuf, "#####"))
		{
			buf = &sbuf[6];
			out_ptr += sprintf(out_ptr, "$%c", buf[-1]);
		}
		else buf = sbuf;

		/* Ignore unprinted lines. */
		if (fake_line(buf))
		{
			l--;
			continue;
		}

		/* Dump the line */
		for (s = buf; *s; )
		{
			/* Link */
			if (h_ptr->shower[0] && prefix(s, h_ptr->shower))
			{
				out_ptr += sprintf(out_ptr, "$<$y%s$>", h_ptr->shower);

				s += strlen(h_ptr->shower);
			}
			/* Hyperlink ? */
			else if (prefix(s, "*****"))
			{
				int thiscol;

				/* Zap the link info */
				cptr u = strchr(s, '[');

				/* Not a real link. */
				if (!u)
				{
					*out_ptr++ = *s++;
				}

				if ((h_ptr->link[h_ptr->cur_link].x == s-buf) &&
					(h_ptr->link[h_ptr->cur_link].y == l))
					thiscol = link_colour_sel;
				else
					thiscol = link_colour;

				out_ptr += sprintf(out_ptr, "$<$%c", thiscol);

				/* Ok print the link name */
				for (u++; *u && *u != ']'; )
				{
					if (prefix(u, "$")) u++;
					*out_ptr++ = *u++;
				}
				out_ptr += sprintf(out_ptr, "$>");
				s = (*u) ? u+1 : u;
			}
			/* Remove HTML ? */
			else if (prefix(s, "{{{{{"))
			{
				cptr u = strchr(s, '}');
				if (u)
				{
					s = u+1;
				}
				else
				{
					*out_ptr++ = *s++;
				}
			}
			else
			{
				*out_ptr++ = *s++;
			}
		}

		/* Print the line. */
		strcpy(out_ptr, "\n");
		mc_roff(out_buf);

		/* Count the printed lines */
		y++;
	}
}

/*
 * Display a screen of text in a non-interactive way.
 * fff is initially positioned after the current section marker.
 */
static void display_help_page_aux(FILE *fff)
{
	cptr path;
	int x,y;
	hyperlink_type h_ptr[1];

	FILE *ftmp;
	
	/* Clear everything to start. */
	WIPE(h_ptr, hyperlink_type);

	/* Create a temporary file to store the current section in. */
	if (!((ftmp = my_fopen_temp(h_ptr->path, sizeof(h_ptr->path))))) return;

	/* Copy the rest of this section. */
	while (!my_fgets_long(h_ptr->rbuf, 1024, fff) &&
		!prefix(h_ptr->rbuf, CC_LINK_PREFIX))
	{
		fprintf(ftmp, "%s", h_ptr->rbuf);
	}

	my_fclose(ftmp);

	/* Put the temporary file name somewhere safe. */
	path = string_make(h_ptr->path);

	/* Open this file for reading. */
	ftmp = my_fopen(path, "r");

	/* Reformat the text to fit in the window. */
	ftmp = reflow_file(ftmp, h_ptr);

	/* Delete the old temporary file. */
	fd_kill(path);
	FREE(path);

	/* Find the height of the window. */
	Term_get_size(&x, &y);

	/* Display the help. */
	show_page(ftmp, h_ptr, 0, y-1, 0);

	my_fclose(ftmp);

	/* Remove the temporary files */
	fd_kill(h_ptr->path);
}

/*
 * If link is mentioned as a link in links[], return the name of the file it
 * appears in.
 */
static cptr link_name_to_file(cptr link)
{
	link_type *l_ptr;
	for (l_ptr = links; l_ptr < links+num_links; l_ptr++)
	{
		if (strstr(l_ptr->str, link)) return l_ptr->file;
	}
	return NULL;
}

/* Forward declare. */
static char show_link_aux(cptr link);

/*
 * A show_file() function based on ToME's version.
 */
static char show_file_aux(cptr name, cptr what, cptr link)
{
	int i, k, x;

	/* Number of "real" lines passed by */
	int next = 0;

	/* Number of "real" lines in the file */
	int size = 0;

	/* Current line. */
	int line = 0;

	/* This function is from a standard help directory. */
	bool is_temp_file = FALSE;

	/* Current help file */
	FILE *fff = NULL;

	/* Char array type of hyperlink info */
	hyperlink_type h_ptr[1];

	/* Pointer to general buffer in the above */
	char *buf;

	/* Read size of screen for big-screen stuff -pav- */
	int wid, hgt;

	/* Clear hyperlink data */
	WIPE(h_ptr, hyperlink_type);

	/* Setup buffer pointer */
	buf = h_ptr->rbuf;

	/* Wipe the links */
	for (i = 0; i < 400; i++)
	{
		h_ptr->link[i].x = -1;
	}

	/* Hack XXX XXX XXX */
	if (what)
	{
		/* h_ptr->caption */
		strcpy(h_ptr->caption, what);

		/* Access the "file" */
		strcpy(h_ptr->path, name);

		/* Grab permission */
		safe_setuid_grab();

		/* Open */
		fff = my_fopen(h_ptr->path, "r");

		/* Drop permission */
		safe_setuid_drop();
	}

	/* Looking for help files now, which will be rewritten. */
	if (!fff) is_temp_file = TRUE;

	/* Look in "help" */
	if (!fff && ((fff = show_fopen(h_ptr, ANGBAND_DIR_HELP, name))))
	{
		/* Caption. */
		sprintf(h_ptr->caption, "Help file '%s'", name);
	}

	/* Look in "info" */
	if (!fff && ((fff = show_fopen(h_ptr, ANGBAND_DIR_INFO, name))))
	{
		/* Caption. */
		sprintf(h_ptr->caption, "Info file '%s'", name);
	}

	/* Look in "file" */
	if (!fff && ((fff = show_fopen(h_ptr, ANGBAND_DIR_FILE, name))))
	{
		/* Caption. */
		sprintf(h_ptr->caption, "File '%s'", name);
	}

	/* Oops */
	if (!fff)
	{
		/* Message */
		msg_format("Cannot open '%s'.", name);
		msg_print(NULL);

		/* Oops */
		return 1;
	}

	/*
	 * Copy to a temporary file and reflow (approximately).
	 * This allows the program to move up or down a line easily whatever
	 * the formatting is.
	 */
	if (is_temp_file) fff = reflow_file(fff, h_ptr);

	/* Pre-Parse the file */
	while (TRUE)
	{
		/* Read a line or stop */
		if (my_fgets(fff, h_ptr->rbuf, 1024)) break;

		/* Get a color */
		if (prefix(h_ptr->rbuf, "#####"))
		{
			buf = &h_ptr->rbuf[6];
		}
		else buf = h_ptr->rbuf;

		/* Tag ? */
		if (prefix(buf, CC_LINK_PREFIX))
		{
			if (link && strstr(buf+CC_LINK_LEN, link))
			{
				line = next;
			}
		}

		x = 0;
		while (buf[x])
		{
			/* Hyperlink ? */
			if (prefix(buf + x, "*****"))
			{
				int xx = x + 5, stmp, xdeb = x + 5, z;
				char tmp[20];

				for (z = 0; z < 20; z++) tmp[z] = '\0';

				h_ptr->link[h_ptr->max_link].x = x;
				h_ptr->link[h_ptr->max_link].y = next;

				if (buf[xx] == '/')
				{
					xx++;
					h_ptr->link[h_ptr->max_link].key = buf[xx];
					xx++;
					xdeb += 2;
				}
				else
				{
					h_ptr->link[h_ptr->max_link].key = 0;
				}

				/* Zap the link info */
				while (buf[xx] != '*')
				{
					h_ptr->link[h_ptr->max_link].tag[xx - xdeb] = buf[xx];
					xx++;
				}
				h_ptr->link[h_ptr->max_link].tag[xx - xdeb] = '\0';
				xx++;
				stmp = xx;
				while (buf[xx] != '[')
				{
					tmp[xx - stmp] = buf[xx];
					xx++;
				}
				xx++;
				tmp[xx - stmp] = '\0';
				h_ptr->link[h_ptr->max_link].line = -atoi(tmp);
				h_ptr->max_link++;
			}
			x++;
		}

		/* Count the "real" lines */
		if (!fake_line(buf)) next++;
	}

	/* Save the number of "real" lines */
	size = next;


	/* Display the file until a ?, escape, backspace or delete is received. */
	for (k = 1; !strchr("?\033\b\177", k);)
	{
		/* Clear screen */
		clear_from(0);

		Term_get_size(&wid, &hgt);

		/* Restart when necessary */
		if (line >= size) line = 0;

		/* Display the text of the file. */
		show_page(fff, h_ptr, 1, hgt - 3, line);

		/* Show a general "title" */
		prt(format("[%s %s, %s, Line %d/%d]", GAME_NAME, GAME_VERSION,
			   h_ptr->caption, line, size), 0, 0);

		/* Prompt -- small files */
		if (size <= (hgt - 4))
		{
			/* Wait for it */
			prt("[Press ESC to exit.]", hgt - 1, 0);
		}

		/* Prompt -- large files */
		else
		{
			/* Wait for it */
			prt("[Press 2, 8, 4, 6, /, =, #, %, backspace, or ESC to exit.]", hgt - 1, 0);
		}

		/* Get a keypress */
		k = inkey();

		/* The current line may not stay in the same place, so start again at
		 * the top of the file. */
		if (k == RESIZE_INKEY_KEY)
		{
			/* Close the file */
			my_fclose(fff);

			/* Delete the file if temporary. */
			if (is_temp_file) fd_kill(h_ptr->path);

			/* Display it again. */
			return show_file_aux(name, what, 0);
		}

		/* Hack -- try showing */
		else if (k == '=')
		{
			/* Get "h_ptr->shower" */
			prt("Show: ", hgt - 1, 0);
			(void)askfor_aux(h_ptr->shower, 80);
		}

		/* Hack -- try finding */
		else if (k == '/')
		{
			/* Get "h_ptr->finder" */
			prt("Find: ", hgt - 1, 0);
			if (askfor_aux(h_ptr->finder, 80))
			{
				/* Give repeated searching the expected result. */
				if (!strcmp(h_ptr->shower, h_ptr->finder)) line++;

				/* Find it. */
				line = find_text(fff, h_ptr, line)-1;

				/* Show it */
				strcpy(h_ptr->shower, h_ptr->finder);
			}
		}

		/* Hack -- go to a specific line */
		else if (k == '#')
		{
			char tmp[81];
			prt("Goto Line: ", hgt - 1, 0);
			strcpy(tmp, "0");
			if (askfor_aux(tmp, 80))
			{
				line = atoi(tmp);
			}
		}

		/* Hack -- go to a specific file */
		else if (k == '%')
		{
			char tmp[81];
			prt("Goto File: ", hgt - 1, 0);
			strcpy(tmp, "help.hlp");
			if (askfor_aux(tmp, 80))
			{
				k  = show_file_aux(tmp, NULL, 0);
			}
		}

		/* Hack -- Allow backing up */
		else if (k == '-' || k == '9')
		{
			line = line - (hgt - 4);
			if (line < 0) line = 0;
		}

		else if (k == '8')
		{
			line--;
			if (line < 0) line = 0;
		}

		/* Hack -- Advance a single line */
		else if (k == '2')
		{
			line = line + 1;
		}

		/* Advance one page */
		else if (k == ' ' || k == '3')
		{
			line = line + (hgt - 4);
		}

		/* Jump to start */
		else if (k == '7')
		{
			line = 0;
		}
		
		/* Jump to end */
		else if (k == '1')
		{
			line = size - (hgt - 4);
		}

		/* Advance one link */
		else if ((k == '6') || (k == '\t'))
		{
			h_ptr->cur_link++;
			if (h_ptr->cur_link >= h_ptr->max_link)
				h_ptr->cur_link = h_ptr->max_link - 1;

			if (h_ptr->link[h_ptr->cur_link].y < line)
				line = h_ptr->link[h_ptr->cur_link].y;
			if (h_ptr->link[h_ptr->cur_link].y >= line + (hgt - 4))
				line = h_ptr->link[h_ptr->cur_link].y - (hgt - 4);
		}
		/* Return one link */
		else if (k == '4')
		{
			h_ptr->cur_link--;
			if (h_ptr->cur_link < 0) h_ptr->cur_link = 0;

			if (h_ptr->link[h_ptr->cur_link].y < line)
				line = h_ptr->link[h_ptr->cur_link].y;
			if (h_ptr->link[h_ptr->cur_link].y >= line + (hgt - 4))
				line = h_ptr->link[h_ptr->cur_link].y - (hgt - 4);
		}

		/* Recurse on numbers */
		else if (k == '\r')
		{
			link_det *ld_ptr = &h_ptr->link[h_ptr->cur_link];
			if (ld_ptr->x != -1) k = show_link_aux(ld_ptr->tag);
		}
		else
		{
			/* No other key ? lets look for a shortcut */
			for (i = 0; i < h_ptr->max_link; i++)
			{
				link_det *ld_ptr = &h_ptr->link[i];
				if (ld_ptr->key == k)
				{
					cptr link = ld_ptr->tag;
					cptr file = link_name_to_file(link);

					/* Recurse on that file */
					k = show_file_aux(file, NULL, link);
				}
			}
		}
		if (line < 0) line = 0;
	}

	/* Close the file */
	my_fclose(fff);

	/* Delete the file if temporary. */
	if (is_temp_file) fd_kill(h_ptr->path);

	/* Return something equivalent to the last key pressed.
	 * Only escape is currently used in a parent function. */
	return (k == ESCAPE) ? k : 1;
}

/*
 * Hack -- display the contents of a file on the screen
 * Only a wrapper around show_file_aux() now.
 */
void show_file(cptr name, cptr what)
{
	add_resize_hook(resize_inkey);
	show_file_aux(name, what, 0);
	delete_resize_hook(resize_inkey);
}

/*
 * Given the name of a link, show the file which contains it.
 * If link is NULL, use the help last requested.
 */
static char show_link_aux(cptr link)
{
	if (!link) link = cur_help_str();
	if (link)
	{
		cptr file = link_name_to_file(link);
		return show_file_aux(file, NULL, link);
	}
	/* Paranoia - no links available. */
	else
	{
		bell("No help string selected!");
		return 0;
	}
}

/*
 * Display some help text via show_file_aux() based on its link name.
 */
void show_link(cptr link)
{
	/* Save the screen. */
	int t = Term_save_aux();
	character_icky = TRUE;

	/* Allow help. */
	help_track("option=?");

	/* Allow resize. */
	add_resize_hook(resize_inkey);

	/* Display the file. */
	show_link_aux(link);

	/* Forget resize. */
	delete_resize_hook(resize_inkey);

	/* Forget help. */
	help_track(NULL);

	/* Restore the screen. */
	character_icky = FALSE;
	Term_load_aux(t);
	Term_release(t);
}

static void init_links(void)
{
	cptr *file;
	char buf[1025];
	int max;
	link_type ilinks[MAX_LINKS];

	FILE_TYPE(FILE_TYPE_TEXT);

	for (file = help_files, max = 0; *file; file++)
	{
		FILE *fff = my_fopen_path(ANGBAND_DIR_HELP, *file, "r");

		if (!fff) quit_fmt("Could not open %s.", *file);

		while (!my_fgets_long(buf, sizeof(buf), fff))
		{
			/* Not a link. */
			if (!prefix(buf, CC_LINK_PREFIX)) continue;

			/* Too many links. */
			if (max == MAX_LINKS) quit("Too many links found");

			/* Ignore overflowing links. */
			if (!strchr(buf, '\n')) continue;

			ilinks[max].str = string_make(buf+CC_LINK_LEN);
			ilinks[max].file = *file;
			ilinks[max].pos = ftell(fff);
			max++;
		}

		fclose(fff);
	}

	/* Allow links to be re-initialised. */
	if (links) FREE(links);

	/* Copy everything to a permanent location. */
	C_MAKE(links, max, link_type);
	C_COPY(links, ilinks, max, link_type);
	num_links = max;
}

/*
 * Find the specified position in a file, if legal.
 * Return TRUE if this is successful.
 *
 * Hack - this function does not react to changes in the files.
 */
static bool find_link(FILE *fff, const link_type *l_ptr)
{
	/* Try to return to the known file position. */
	if (l_ptr->pos >= 0)
	{
		return (!fseek(fff, l_ptr->pos, SEEK_SET));
	}
	/* Find the link from scratch. */
	else
	{
		char buf[1025];
		while (!my_fgets_long(buf, sizeof(buf), fff))
		{
			/* Found it. */
			if (prefix(buf, CC_LINK_PREFIX) &&
				!strcmp(buf+CC_LINK_LEN, l_ptr->str)) return TRUE;
		}
	}
	/* Failed. */
	return FALSE;
}

/*
 * Initialise the help_files[] array above.
 * Return false if the base help file was not found, true otherwise.
 */
void init_help_files(void)
{
	int i;
	FILE *fff;
	char buf[1024];

	FILE_TYPE(FILE_TYPE_TEXT);

	if (help_files) FREE(help_files);

	if (!((fff = my_fopen_path(ANGBAND_DIR_HELP, syshelpfile, "r"))))
	{
		quit_fmt("Cannot open '%s'!", syshelpfile);
	}
		

	/* Count the file references. */
	for (i = 1; !my_fgets(fff, buf, 1024);)
	{
		if (prefix(buf, "%%%%F ")) i++;
	}

	/* Create the help_files array. */
	help_files = C_NEW(i, cptr);

	/* Hack - The last element must be NULL. */
	help_files[--i] = NULL;

	/* Return to the start of the file. */
	fseek(fff, 0, SEEK_SET);

	/* Fill the help_files array. */
	while (!my_fgets(fff, buf, 1024))
	{
		if (prefix(buf, "%%%%F "))
		{
			/* Fill in the help_files array (backwards). */
			help_files[--i] = string_make(buf+6);
		}
	}

	my_fclose(fff);

	init_links();
}

/*
 * Centre a string on a specified line of the current term.
 */
static void put_str_centre(cptr str, int y)
{
	int l = strlen(str);
	int x = MAX(0, (Term->wid-l)/2);
	put_str(str, y, x);
}

void display_help_page(cptr str)
{
	FILE *fff;
	link_type *l_ptr;

	for (l_ptr = links; l_ptr < links+num_links; l_ptr++)
	{
		if (strstr(l_ptr->str, str))
		{
			FILE_TYPE(FILE_TYPE_TEXT);

			/* Open the file. */
			fff = my_fopen_path(ANGBAND_DIR_HELP, l_ptr->file, "r");
			if (!fff) return;

			/* Find a previously remembered position in it. */
			if (find_link(fff, l_ptr))
			{
				/* Display it. */
				display_help_page_aux(fff);
			}

			/* And finally close it. */
			fclose(fff);

			/* Done. */
			return;
		}
	}

	/* Oops - no-one's written the rquested help file. */
	put_str_centre(
		"Sorry, help for this command is not available.", Term->hgt/2-1);

	put_str_centre(
		"Please contact " MAINTAINER " for further information.", Term->hgt/2+1);
}


/*
 * Process the player name.
 * Extract a clean "base name".
 * Build the savefile name if needed.
 */
void process_player_name(void)
{
	int i, k = 0;


	/* Cannot be too long */
	if (strlen(player_name) > 15)
	{
		/* Name too long */
		quit_fmt("The name '%s' is too long!", player_name);
	}

	/* Cannot contain "icky" characters */
	for (i = 0; player_name[i]; i++)
	{
		/* No control characters */
		if (iscntrl(player_name[i]))
		{
			/* Illegal characters */
			quit_fmt("The name '%s' contains control chars!", player_name);
		}
	}


#ifdef MACINTOSH

	/* Extract "useful" letters */
	for (i = 0; player_name[i]; i++)
	{
		char c = player_name[i];

		/* Convert "dot" to "underscore" */
		if (c == '.') c = '_';

		/* Accept all the letters */
		player_base[k++] = c;
	}

#else

	/* Extract "useful" letters */
	for (i = 0; player_name[i]; i++)
	{
		char c = player_name[i];

		/* Accept some letters */
		if (isalpha(c) || isdigit(c)) player_base[k++] = c;

		/* Convert space, dot, and underscore to underscore */
		else if (strchr(". _", c)) player_base[k++] = '_';
	}

#endif


#if defined(WINDOWS) || defined(MSDOS)
	/* Hack -- max length */
	if (k > 8) k = 8;
#endif

	/* Terminate */
	player_base[k] = '\0';

	/* Require a "base" name */
	if (!player_base[0]) strcpy(player_base, "PLAYER");


	/* Change the savefile name */
	if (!savefile[0])
	{
		char temp[128];

#ifdef SAVEFILE_USE_UID
		/* Rename the savefile, using the player_uid and player_base */
		(void)sprintf(temp, "%d.%s", player_uid, player_base);
#else
		/* Rename the savefile, using the player_base */
		(void)sprintf(temp, "%s", player_base);
#endif

#ifdef VM
		/* Hack -- support "flat directory" usage on VM/ESA */
		(void)sprintf(temp, "%s.sv", player_base);
#endif /* VM */

		/* Build the filename */
		strnfmt(savefile, 1024, "%v", path_build_f2, ANGBAND_DIR_SAVE, temp);
	}
}


/*
 * Gets a name for the character, reacting to name changes.
 *
 * Assumes that "display_player(0)" has just been called
 *
 * Perhaps we should NOT ask for a name (at "birth()") on
 * Unix machines?  XXX XXX
 *
 * What a horrible name for a global function.  XXX XXX XXX
 *
 * Returns FALSE if the player presses ESCAPE in askfor_aux(), TRUE otherwise.
 */
bool get_name(void)
{
	char tmp[32];

	/* We start with the original name. */
	bool changed = FALSE;

	/* Clear last line */
	clear_from(22);

	/* Prompt and ask */
	prt("[Enter your player's name above, or hit ESCAPE]", 23, 2);

	/* Ask until happy */
	while (1)
	{
		/* Go to the "name" field */
		move_cursor(2, 15);

		/* Save the player name */
		strcpy(tmp, player_name);

		/* Get an input, ignore "Escape" */
		if (askfor_aux(tmp, 15))
		{
			strcpy(player_name, tmp);
			changed = TRUE;
		}

		/* Process the player name */
		process_player_name();

		/* All done */
		break;
	}

	/* Pad the name (to clear junk) */
	sprintf(tmp, "%-15.15s", player_name);

	/* Re-Draw the name (in light blue) */
	c_put_str(TERM_L_BLUE, tmp, 2, 15);

	/* Erase the prompt, etc */
	clear_from(22);
	
	return changed;
}


/* The string used to indicate that the player quit. */
#define SUICIDE_STRING "Woke up"

/*
 * Hack -- commit suicide
 */
void do_cmd_suicide(void)
{
	int i;

	/* Flush input */
	flush();

	/* Verify Retirement */
	if (total_winner)
	{
		/* Verify */
		if (!get_check("Do you want to retire? ")) return;
	}

	/* Verify Suicide */
	else
	{
		/* Verify */
		if (!get_check("Do you really want to abandon your quest? ")) return;
        if (!noscore)
        {
			/* Special Verification for suicide */
			prt("Please verify QUIT by typing the '@' sign: ", 0, 0);
			flush();
			i = inkey();
			prt("", 0, 0);
			if (i != '@') return;
		}
	}

	/* Stop playing */
	alive = FALSE;

	/* Kill the player */
	death = TRUE;

	/* Cause of death */
	died_from = SUICIDE_STRING;

	{
		monster_race *r_ptr = r_info+MON_SUICIDE;
		if (r_ptr->r_deaths < MAX_SHORT) r_ptr->r_deaths++;
	}
}



/*
 * Save the game
 */
void do_cmd_save_game(bool is_autosave)
{
	/* Enable backwards compatibility */
	bool as_4_1_0 = FALSE;
	
    /* Autosaves do not disturb */
    if (!is_autosave)
    {
        /* Disturb the player */
        disturb(1);

#ifdef ALLOW_410_SAVES
	/* Determine the appropriate save version */
	as_4_1_0 = get_check("Save a 4.1.0 save file? ");
#endif
    }
	else if (!autosave_q)
	{
		msg_print("Autosaving the game...");
	}

	/* Clear messages */
	if (!is_autosave || !autosave_q) msg_print(NULL);

	/* Handle stuff */
	handle_stuff();

	/* Message */
	if (!is_autosave || !autosave_q) prt("Saving game...", 0, 0);

	/* Refresh */
	Term_fresh();

	/* The player is not dead */
	died_from = "(saved)";

	/* Forbid suspend */
	signals_ignore_tstp();

	/* Save the player */
	if (save_player(as_4_1_0))
	{
		if (!is_autosave || !autosave_q) prt("Saving game... done.", 0, 0);
	}

	/* Save failed (oops) */
	else
	{
		prt("Saving game... failed!", 0, 0);
	}

	/* Allow suspend again */
	signals_handle_tstp();

	/* Refresh */
	Term_fresh();

	/* Note that the player is not dead */
	died_from = "nobody (yet!)";
}



/*
 * Hack -- Calculates the total number of points earned		-JWT-
 */
static long total_points(void)
{
	return p_ptr->exp + 100*p_ptr->max_dlv+dun_defs[cur_dungeon].offset;
}



/*
 * Centers a string within a 31 character string		-JWT-
 */
static void center_string(char *buf, cptr str)
{
	int i, j;

	/* Total length */
	i = strlen(str);

	/* Necessary border */
	j = 15 - i / 2;

	/* Mega-Hack */
	(void)sprintf(buf, "%*s%s%*s", j, "", str, 31 - i - j, "");
}



/*
 * Save a "bones" file for a dead character
 *
 * Note that we will not use these files until Angband 2.8.0, and
 * then we will only use the name and level on which death occured.
 *
 * Should probably attempt some form of locking...
 */
static void make_bones(void)
{
	FILE                *fp;


	if ((p_ptr->prace == RACE_SKELETON) || (p_ptr->prace == RACE_ZOMBIE) ||
		(p_ptr->prace == RACE_SPECTRE) || (p_ptr->prace == RACE_VAMPIRE))
	{
		return;
	}


	/* Ignore wizards and borgs */
	if (!(noscore & 0x00FF))
	{
		/* Ignore people who die in town (and on impossible levels). */
		if (dun_level > 0 && dun_level < 1000)
		{
			/* XXX XXX XXX "Bones" name */
			char tmp[8];
			sprintf(tmp, "bone.%03d", dun_level);

			/* Attempt to open the bones file */
			fp = my_fopen_path(ANGBAND_DIR_BONE, tmp, "r");

			/* Close it right away */
			if (fp) my_fclose(fp);

			/* Do not over-write a previous ghost */
			if (fp) return;

			/* File type is "TEXT" */
			FILE_TYPE(FILE_TYPE_TEXT);

			/* Try to write a new "Bones File" */
			fp = my_fopen_path(ANGBAND_DIR_BONE, tmp, "w");

			/* Not allowed to write it?  Weird. */
			if (!fp) return;

			/* Save the info */
			fprintf(fp, "%s\n", player_name);
			fprintf(fp, "%d\n", p_ptr->mhp);
			fprintf(fp, "%d\n", p_ptr->prace);
			fprintf(fp, "%d\n", p_ptr->ptemplate);

			/* Close and save the Bones file */
			my_fclose(fp);
		}
	}
}


/*
 * Redefinable "print_tombstone" action
 */
static bool (*tombstone_aux)(void) = NULL;


/*
 * Display a "tomb-stone"
 */
static void print_tomb(void)
{
	bool done = FALSE;

	/* Do we use a special tombstone ? */
	if (tombstone_aux)
	{
		/* Use tombstone hook */
		done = (*tombstone_aux)();
	}

	/* Print the text-tombstone */
	if (!done)
	{

	char	tmp[160];

	char	buf[1024];

	FILE        *fp;

	time_t	ct = time((time_t)0);


	/* Clear screen */
	Term_clear();

	/* Build the filename */
	strnfmt(buf, 1024, "%v", path_build_f2, ANGBAND_DIR_FILE, "dead.txt");

	/* Open the News file */
	fp = my_fopen_path(ANGBAND_DIR_FILE, "dead.txt", "r");

	/* Dump */
	if (fp)
	{
		int i = 0;

		/* Dump the file to the screen */
		while (0 == my_fgets(fp, buf, 1024))
		{
			/* Display and advance */
			put_str(buf, i++, 0);
		}

		/* Close */
		my_fclose(fp);
	}


	center_string(buf, player_name);

	put_str(buf, 6, 11);

	/* King or Queen */
	if (total_winner)
	{
		center_string(buf, "the");
		put_str(buf, 7, 11);
		center_string(buf,"Magnificent");
		put_str(buf,8,11);
	}
	/* Normal */
	else
	{
		center_string(buf, "the");
		put_str(buf, 8, 11);
	}


	center_string(buf, rp_ptr->title);
	put_str(buf, 9, 11);


	center_string(buf, cp_ptr->title);
	put_str(buf, 10, 11);

	(void)sprintf(tmp, "Exp: %ld", (long)p_ptr->exp);
	center_string(buf, tmp);
	put_str(buf, 12, 11);

	(void)sprintf(tmp, "AU: %ld", (long)p_ptr->au);
	center_string(buf, tmp);
	put_str(buf, 13, 11);

	if (dun_level)
		(void)sprintf(tmp, "Killed on Level %d", dun_level);
	else
		(void)strcpy(tmp, "Killed on the Surface");
	center_string(buf, tmp);
	put_str(buf, 14, 11);

	if (dun_level > 0)
		sprintf(tmp, "of %s", dun_name+dun_defs[wild_grid[wildy][wildx].dungeon].name);
	else if (is_town_p(wildy, wildx))
		sprintf(tmp, "in %s", dun_name+dun_defs[wild_grid[wildy][wildx].dungeon].shortname);
	else if (wild_grid[wildy][wildx].dungeon < MAX_CAVES)
		sprintf(tmp, "near %s", dun_name+dun_defs[wild_grid[wildy][wildx].dungeon].name);
	else
		strcpy(tmp, "in the Wilderness");
	center_string(buf, tmp);
	put_str(buf, 15, 11);

	(void)sprintf(tmp, "by %.*s.", 24, died_from);
	center_string(buf, tmp);
	put_str(buf, 16, 11);

	(void)sprintf(tmp, "%-.24s", ctime(&ct));
	center_string(buf, tmp);
	put_str(buf, 18, 11);

	msg_format("Goodbye, %s!", player_name);
	}
}


/*
 * Display some character info
 */
static void show_info(void)
{
	int			i;

	object_type		*o_ptr;

	/* Hack -- Know everything in the inven/equip */
	for (i = 0; i < INVEN_TOTAL; i++)
	{
		o_ptr = &inventory[i];

		/* Skip non-objects */
		if (!o_ptr->k_idx) continue;
		
		/* Aware and Known */
		object_aware(o_ptr);
		object_known(o_ptr);
	}


	/* Hack -- Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Handle stuff */
	handle_stuff();

	/* Flush all input keys */
	flush();

	/* Flush messages */
	msg_print(NULL);


	/* Describe options */
	prt("You may now dump a character record to one or more files.", 21, 0);
	prt("Then, hit RETURN to see the character, or ESC to abort.", 22, 0);

	/* Dump character records as requested */
	while (TRUE)
	{
		char out_val[160];

		/* Prompt */
		put_str("Filename: ", 23, 0);

		/* Default */
		strcpy(out_val, "");

		/* Ask for filename (or abort) */
		if (!askfor_aux(out_val, 60)) return;

		/* Return means "show on screen" */
		if (!out_val[0]) break;

		/* Save screen */
		Term_save();

		/* Dump a character file */
		(void)file_character(out_val);

		/* Load screen */
		Term_load();
	}


	/* Display player */
	display_player(0);

	/* Prompt for inventory */
	prt("Hit any key to see more information (ESC to abort): ", 23, 0);

	/* Allow abort at this point */
	if (inkey() == ESCAPE) return;


	/* Show equipment and inventory */

	/* Equipment -- if any */
	if (equip_cnt())
	{
		Term_clear();
		item_tester_full = TRUE;
		show_equip();
		prt("You are using: -more-", 0, 0);
		if (inkey() == ESCAPE) return;
	}

	/* Inventory -- if any */
	if (inven_cnt())
	{
		Term_clear();
		item_tester_full = TRUE;
		show_inven();
		prt("You are carrying: -more-", 0, 0);
		if (inkey() == ESCAPE) return;
	}
}








/*
 * The "highscore" file descriptor, if available.
 */
int highscore_fd = -1;


/*
 * Seek score 'i' in the highscore file
 */
static int highscore_seek(int i)
{
	/* Seek for the requested record */
	return (fd_seek(highscore_fd, (huge)(i) * sizeof(high_score)));
}


/*
 * Read one score from the highscore file
 */
static errr highscore_read(high_score *score)
{
	/* Read the record, note failure */
	return (fd_read(highscore_fd, (char*)(score), sizeof(high_score)));
}


/*
 * Write one score to the highscore file
 */
static int highscore_write(high_score *score)
{
	/* Write the record, note failure */
	return (fd_write(highscore_fd, (char*)(score), sizeof(high_score)));
}




/*
 * Just determine where a new score *would* be placed
 * Return the location (0 is best) or -1 on failure
 */
static int highscore_where(high_score *score)
{
	int			i;

	high_score		the_score;

	/* Paranoia -- it may not have opened */
	if (highscore_fd < 0) return (-1);

	/* Go to the start of the highscore file */
	if (highscore_seek(0)) return (-1);

	/* Read until we get to a higher score */
	for (i = 0; i < MAX_HISCORES; i++)
	{
		if (highscore_read(&the_score)) return (i);
		if (strcmp(the_score.pts, score->pts) < 0) return (i);
	}

	/* The "last" entry is always usable */
	return (MAX_HISCORES - 1);
}


/*
 * Actually place an entry into the high score file
 * Return the location (0 is best) or -1 on "failure"
 */
static int highscore_add(high_score *score)
{
	int			i, slot;
	bool		done = FALSE;

	high_score		the_score, tmpscore;


	/* Paranoia -- it may not have opened */
	if (highscore_fd < 0) return (-1);

	/* Determine where the score should go */
	slot = highscore_where(score);

	/* Hack -- Not on the list */
	if (slot < 0) return (-1);

	/* Hack -- prepare to dump the new score */
	the_score = (*score);

	/* Slide all the scores down one */
	for (i = slot; !done && (i < MAX_HISCORES); i++)
	{
		/* Read the old guy, note errors */
		if (highscore_seek(i)) return (-1);
		if (highscore_read(&tmpscore)) done = TRUE;

		/* Back up and dump the score we were holding */
		if (highscore_seek(i)) return (-1);
		if (highscore_write(&the_score)) return (-1);

		/* Hack -- Save the old score, for the next pass */
		the_score = tmpscore;
	}

	/* Return location used */
	return (slot);
}



/*
 * Display the scores in a given range.
 * Assumes the high score list is already open.
 * Only five entries per line, too much info.
 *
 * Mega-Hack -- allow "fake" entry at the given position.
 */
void display_scores_aux(int from, int to, int note, high_score *score)
{
	int		i, j, k, n, attr, place;
	int psc;

	high_score	the_score;

	char	out_val[256];
	char	tmp_val[160];


	/* Paranoia -- it may not have opened */
	if (highscore_fd < 0) return;


	/* Assume we will show the first 10 */
	if (from < 0) from = 0;
	if (to < 0) to = 10;
	if (to > MAX_HISCORES) to = MAX_HISCORES;


	/* Seek to the beginning */
	if (highscore_seek(0)) return;

	/* Hack -- Count the high scores */
	for (i = 0; i < MAX_HISCORES; i++)
	{
		if (highscore_read(&the_score)) break;
	}

	/* Hack -- allow "fake" entry to be last */
	if ((note == i) && score) i++;

	/* Forget about the last entries */
	if (i > to) i = to;


	/* Show 5 per page, until "done" */
	for (k = from, place = k+1; k < i; k += 5)
	{
		/* Clear screen */
		Term_clear();

		/* Title */
        put_str("              sCthangband Hall of Fame", 0, 0);

		/* Indicate non-top scores */
		if (k > 0)
		{
			sprintf(tmp_val, "(from position %d)", k + 1);
			put_str(tmp_val, 0, 40);
		}

		/* Dump 5 entries */
		for (j = k, n = 0; j < i && n < 5; place++, j++, n++)
		{
			int pr, pc, clev, mlev, cdun, mdun;

			cptr user, gold, when, aged;


			/* Hack -- indicate death in yellow */
			attr = (j == note) ? TERM_YELLOW : TERM_WHITE;


			/* Mega-Hack -- insert a "fake" record */
			if ((note == j) && score)
			{
				the_score = (*score);
				attr = TERM_L_GREEN;
				score = NULL;
				note = -1;
				j--;
			}

			/* Read a normal record */
			else
			{
				/* Read the proper record */
				if (highscore_seek(j)) break;
				if (highscore_read(&the_score)) break;
			}

			/* Extract the race/template */
			pr = atoi(the_score.p_r);
			pc = atoi(the_score.p_c);
			/* Pull the subclass info out of the class entry (archaic score files only) */
			psc=pc/100;
			pc=pc-(100*psc);


			/* Extract the level info */
			clev = atoi(the_score.cur_lev);
			mlev = atoi(the_score.max_lev);
			cdun = atoi(the_score.cur_dun);
			mdun = atoi(the_score.max_dun);

			/* Hack -- extract the gold and such */
			for (user = the_score.uid; isspace(*user); user++) /* loop */;
			for (when = the_score.day; isspace(*when); when++) /* loop */;
			for (gold = the_score.gold; isspace(*gold); gold++) /* loop */;
			for (aged = the_score.turns; isspace(*aged); aged++) /* loop */;

			/* Dump some info */
			sprintf(out_val, "%3d.%9s  %s the %s %s",
			        place, the_score.pts, the_score.who,
			        race_info[pr].title, template_info[pc].title);

			/* Dump the first line */
			c_put_str((byte)attr, out_val, n*4 + 2, 0);

			/* Another line of info */
			sprintf(out_val, "               %s %s on %s %d",
				(strcmp(the_score.how, "Woke up")) ? "Killed by" : "",
			        the_score.how, "Dungeon Level", cdun);

			/* Hack -- some people die in the town */
			if (!cdun)
			{
				sprintf(out_val, "               Killed by %s on the surface",
				        the_score.how);
			}

			/* Append a "maximum level" */
			if (mdun > cdun) strcat(out_val, format(" (Max %d)", mdun));

			/* Dump the info */
			c_put_str((byte)attr, out_val, n*4 + 3, 0);

			/* And still another line of info */
			sprintf(out_val,
			        "               (User %s, Date %s, Gold %s, Turn %s).",
			        user, when, gold, aged);
			c_put_str((byte)attr, out_val, n*4 + 4, 0);
		}


		/* Wait for response */
		prt("[Press ESC to quit, any other key to continue.]", 23, 17);
		j = inkey();
		prt("", 23, 0);

		/* Hack -- notice Escape */
		if (j == ESCAPE) break;
	}
}


/*
 * Hack -- Display the scores in a given range and quit.
 *
 * This function is only called from "main.c" when the user asks
 * to see the "high scores".
 */
void display_scores(int from, int to)
{
	char buf[1024];

	/* Build the filename */
	strnfmt(buf, 1024, "%v", path_build_f2, ANGBAND_DIR_APEX, "scores.raw");

	/* Open the binary high score file, for reading */
	highscore_fd = fd_open(buf, O_RDONLY);

	/* Paranoia -- No score file */
	if (highscore_fd < 0) quit("Score file unavailable.");

	/* Clear screen */
	Term_clear();

	/* Display the scores */
	display_scores_aux(from, to, -1, NULL);

	/* Shut the high score file */
	(void)fd_close(highscore_fd);

	/* Forget the high score fd */
	highscore_fd = -1;

	/* Quit */
	quit(NULL);
}

/*
 * templare_score - selectively list highscores based on template
 * -KMW-
 */
void template_score(int ptemplate)
{

	register int i = 0, j, m = 0;
	int pr, pc, clev;
	high_score the_score;
	char buf[1024], out_val[256],tmp_str[80];
	
	sprintf(tmp_str,"The Masters of the %s Profession",template_info[ptemplate].title);
	prt(tmp_str,5,0);
	/* Build the filename */
	strnfmt(buf, 1024, "%v", path_build_f2, ANGBAND_DIR_APEX, "scores.raw");

	highscore_fd = fd_open(buf, O_RDONLY);

	if (highscore_fd < 0)
	{
		msg_print("Score file unavailable.");
		msg_print(NULL);
		return;
	}

	if (highscore_seek(0)) return;

	for (i = 0; i < MAX_HISCORES; i++)
		if (highscore_read(&the_score)) break;

	m=0;
	j=0;
	clev = 0;

	while ((m < 10) && (j < MAX_HISCORES))
	{
		if (highscore_seek(j)) break;
		if (highscore_read(&the_score)) break;
		pr = atoi(the_score.p_r);
		pc = atoi(the_score.p_c);
		clev = atoi(the_score.cur_lev);
		if (pc == ptemplate)
		{
			sprintf(out_val, "%3d) %s the %s (Score %d)",
			    (m + 1), the_score.who,
				race_info[pr].title,atoi(the_score.pts));
			prt(out_val, (m + 7), 0);
			m++;
		}
		j++;
	}

	/* Now, list the active player if they qualify */
	if (p_ptr->ptemplate == ptemplate)
	{
		sprintf(out_val, "You) %s the %s (Score %ld)",
		player_name,
		race_info[p_ptr->prace].title, total_points());
		prt(out_val, (m + 8), 0);
	}

	(void)fd_close(highscore_fd);
	highscore_fd = -1;
	msg_print("Hit any key to continue");
	msg_print(NULL);
	for (j=5;j<18;j++)
		prt("",j,0);
}


/*
 * Race Legends
 * -KMW- 
 */
void race_score(int race_num)
{
	register int i = 0, j, m = 0;
	int pr, pc, clev, lastlev,psc;
	high_score the_score;
	char buf[1024], out_val[256], tmp_str[80];
	
	lastlev = 0;
	(void) sprintf(tmp_str,"The %s Heroes of Note",race_info[race_num].title);
	prt(tmp_str, 5, 15);

	/* Build the filename */
	strnfmt(buf, 1024, "%v", path_build_f2, ANGBAND_DIR_APEX, "scores.raw");

	highscore_fd = fd_open(buf, O_RDONLY);

	if (highscore_fd < 0)
	{
		msg_print("Score file unavailable.");
		msg_print(NULL);
		return;
	}

	if (highscore_seek(0)) return;

	for (i = 0; i < MAX_HISCORES; i++) {
		if (highscore_read(&the_score)) break;
	}

	m=0;
	j=0;

	while ((m < 10) && (j < MAX_HISCORES))
	{
		if (highscore_seek(j)) break;
		if (highscore_read(&the_score)) break;
		pr = atoi(the_score.p_r);
		pc = atoi(the_score.p_c);
		psc=pc/100;
		pc=pc-(psc*100);
		clev = atoi(the_score.cur_lev);
		if (pr == race_num)
		{
			sprintf(out_val, "%3d) %s, the %s (Score %d)",
			    (m + 1), the_score.who,
			template_info[pc].title, atoi(the_score.pts));
			prt(out_val, (m + 7), 0);
			m++;
			lastlev = clev;  
		}
		j++;
	}

	/* add player if qualified */
	if (p_ptr->prace == race_num)
	{
		sprintf(out_val, "You) %s, the %s (Score %ld)",
		    player_name, 
			cp_ptr->title,total_points());
		prt(out_val, (m + 8), 0);
	}

	(void)fd_close(highscore_fd);
	highscore_fd = -1;
	msg_print("Hit any key to continue");
	msg_print(NULL);
	for (j=5;j<18;j++)
		prt("",j,0);
}


/*
 * Determine the player details for the high score table.
 */
static void get_details(high_score *the_score)
{
	time_t ct = time((time_t*)0);
	s16b best = 0;

	/* Save the version */
	sprintf(the_score->what, "%s", GAME_VERSION);

	/* Calculate and save the points */
	sprintf(the_score->pts, "%9ld", MIN(999999999, total_points()));

	/* Save the current gold */
	sprintf(the_score->gold, "%9ld", MIN(999999999, p_ptr->au));

	/* Save the current turn */
	sprintf(the_score->turns, "%9ld", MIN(999999999, turn));

#ifdef HIGHSCORE_DATE_HACK
	/* Save the date in a hacked up form (9 chars) */
	sprintf(the_score->day, "%-.6s %-.2s", ctime(&ct) + 4, ctime(&ct) + 22);
#else
	/* Save the date in standard form (8 chars) */
	strftime(the_score->day, 9, "%m/%d/%y", localtime(&ct));
#endif

	/* Save the player name (15 chars) */
	sprintf(the_score->who, "%-.15s", player_name);

	/* Save the player info XXX XXX XXX */
	sprintf(the_score->uid, "%7d", player_uid);
	sprintf(the_score->sex, "%c", (p_ptr->psex ? 'm' : 'f'));
	sprintf(the_score->p_r, "%2d", p_ptr->prace);
	sprintf(the_score->p_c, "%2d", p_ptr->ptemplate);

	/* Save the level if a dungeon */
	sprintf(the_score->cur_dun, "%3d", (dun_level) ? dun_depth : 0);

	/* Find the deepest level in the current dungeon. */
	best = p_ptr->max_dlv+dun_defs[cur_dungeon].offset;

	sprintf(the_score->max_dun, "%3d", best);

	/* Save the cause of death (31 chars) */
	sprintf(the_score->how, "%-.31s", died_from);
}

/*
 * Dump a record file for a dead character into lib/apex/logfile.txt or similar.
 *
 * Format:
 * V:version UI:uid Ti:date Sc:score Tu:turns Ki:killed by
 * Dp:depth Du:dungeon Sx:sex Ra:race Tm:template Na:name
 */
static void make_record(high_score *score)
{
	time_t ct = time(NULL);
	FILE *fp;
	char str[1024];
	cptr dun_str;
	int dun;

	strnfmt(str, 1024, "%v", path_build_f2, ANGBAND_DIR_APEX, "logfile.txt");

	fp = my_fopen_path(ANGBAND_DIR_APEX, "logfile.txt", "a");

	/* Silently give up for now. */
	if (!fp)
	{
		return;
	}

	/*  */
	if (ct == -1)
	{
		strcpy(str, "(time)");
	}
	else
	{
		strftime(str, 1024, "%d %b %Y %H:%M:%S", localtime(&ct));
	}

	dun = wild_grid[wildy][wildx].dungeon;
	if (dun < MAX_CAVES)
	{
		dun_str = dun_name+dun_defs[dun].shortname;
	}
	else
	{
		dun_str = "Wilderness";
	}

	/* Dump everything, trimming any leading spaces first. */

#define TRIM(X) (X+strspn(X, " "))

	fprintf(fp, "V:%s ", TRIM(score->what));
#ifdef SET_UID
	fprintf(fp, "UI:%s ", TRIM(score->uid));
#endif /* SET_UID */
	fprintf(fp, "Ti:%s ", str);
	fprintf(fp, "Sc:%s ", TRIM(score->pts));
	fprintf(fp, "Tu:%s ", TRIM(score->turns));
	fprintf(fp, "HP:%d ", p_ptr->mhp);
	fprintf(fp, "Ki:%s ", TRIM(score->how));
	fprintf(fp, "Dp:%s ", TRIM(score->cur_dun));
	fprintf(fp, "Du:%s ", dun_str);
	fprintf(fp, "Sx:%s ", sex_info[p_ptr->psex].title);
	fprintf(fp, "Ra:%s ", race_info[p_ptr->prace].title);
	fprintf(fp, "Tm:%s ", template_info[p_ptr->ptemplate].title);
	fprintf(fp, "Na:%s\n", TRIM(score->who));
	my_fclose(fp);
}

/*
 * Enters a players name on a hi-score table, if "legal", and in any
 * case, displays some relevant portion of the high score list.
 *
 * Assumes "signals_ignore_tstp()" has been called.
 */
static errr top_twenty(void)
{
	int		  j;

	high_score   the_score;

	/* Clear screen */
	Term_clear();

	/* Clear the record */
	WIPE(&the_score, high_score);

	/* Create a new record */
	get_details(&the_score);

	/* Make a record entry. */
	make_record(&the_score);

	/* No score file */
	if (highscore_fd < 0)
	{
		msg_print("Score file unavailable.");
		msg_print(NULL);
		return (0);
	}

#ifndef SCORE_BORGS
	/* Borg-mode pre-empts scoring */
	if (noscore & 0x00F0)
	{
		msg_print("Score not registered for borgs.");
		msg_print(NULL);
		display_scores_aux(0, 10, -1, NULL);
		return (0);
	}
#endif

#ifndef SCORE_CHEATERS
	/* Cheaters are not scored */
	if (noscore & 0xFF02)
	{
		msg_print("Score not registered for cheaters.");
		msg_print(NULL);
		display_scores_aux(0, 10, -1, NULL);
		return (0);
	}
#endif

	/* Quitters are only scored if allowed. */
	if (
#ifdef SCORE_QUITTERS
		!score_quitters &&
#endif /* SCORE_QUITTERS */
		!total_winner && streq(died_from, SUICIDE_STRING))
	{
		msg_print("Score not registered due to quitting.");
		msg_print(NULL);
		display_scores_aux(0, 10, -1, NULL);
		return (0);
	}

	/* Interupted */
	if (!total_winner && streq(died_from, "Interrupting"))
	{
		msg_print("Score not registered due to interruption.");
		msg_print(NULL);
		display_scores_aux(0, 10, -1, NULL);
		return (0);
	}


	/* Lock (for writing) the highscore file, or fail */
	if (fd_lock(highscore_fd, F_WRLCK)) return (1);

	/* Add a new entry to the score list, see where it went */
	j = highscore_add(&the_score);

	/* Unlock the highscore file, or fail */
	if (fd_lock(highscore_fd, F_UNLCK)) return (1);


	/* Hack -- Display the top fifteen scores */
	if (j < 10)
	{
		display_scores_aux(0, 15, j, NULL);
	}

	/* Display the scores surrounding the player */
	else
	{
		display_scores_aux(0, 5, j, NULL);
		display_scores_aux(j - 2, j + 7, j, NULL);
	}


	/* Success */
	return (0);
}


/*
 * Predict the players location, and display it.
 */
errr predict_score(void)
{
	int		  j;

	high_score   the_score;


	/* No score file */
	if (highscore_fd < 0)
	{
		msg_print("Score file unavailable.");
		msg_print(NULL);
		return (0);
	}

	/* Create a new record */
	get_details(&the_score);

	/* Hack -- no time needed */
	strcpy(the_score.day, "TODAY");

	/* See where the entry would be placed */
	j = highscore_where(&the_score);


	/* Hack -- Display the top fifteen scores */
	if (j < 10)
	{
		display_scores_aux(0, 15, j, &the_score);
	}

	/* Display some "useful" scores */
	else
	{
		display_scores_aux(0, 5, -1, NULL);
		display_scores_aux(j - 2, j + 7, j, &the_score);
	}


	/* Success */
	return (0);
}










/*
 * Change the player into a King!			-RAK-
 */
static void kingly(void)
{
	/* Hack -- retire in town */
	dun_level = 0;

	/* Fake death */
	died_from = "Ripe Old Age";

	/* Hack -- Instant Gold */
	p_ptr->au += 10000000L;

	/* Clear screen */
	Term_clear();

	/* Display a crown */
	put_str("#", 1, 34);
	put_str("#####", 2, 32);
	put_str("#", 3, 34);
	put_str(",,,  $$$  ,,,", 4, 28);
	put_str(",,=$   \"$$$$$\"   $=,,", 5, 24);
	put_str(",$$        $$$        $$,", 6, 22);
	put_str("*>         <*>         <*", 7, 22);
	put_str("$$         $$$         $$", 8, 22);
	put_str("\"$$        $$$        $$\"", 9, 22);
	put_str("\"$$       $$$       $$\"", 10, 23);
	put_str("*#########*#########*", 11, 24);
	put_str("*#########*#########*", 12, 24);

	/* Display a message */
	put_str("Veni, Vidi, Vici!", 15, 26);
	put_str("I came, I saw, I conquered!", 16, 21);
	put_str(format("All Hail the Mighty %s!", sp_ptr->winner), 17, 22);

	/* Flush input */
	flush();

	/* Wait for response */
	pause_line();
}


/*
 * Close up the current game (player may or may not be dead)
 *
 * This function is called only from "main.c" and "signals.c".
 */
void close_game(void)
{
	char buf[1024];


	/* Handle stuff */
	handle_stuff();

	/* Flush the messages */
	msg_print(NULL);

	/* Flush the input */
	flush();


	/* No suspending now */
	signals_ignore_tstp();


	/* Hack -- Character is now "icky" */
	character_icky = TRUE;


	/* Build the filename */
	strnfmt(buf, 1024, "%v", path_build_f2, ANGBAND_DIR_APEX, "scores.raw");

	/* Open the high score file, for reading/writing */
	highscore_fd = fd_open(buf, O_RDWR);


	/* Handle death */
	if (death)
	{
		/* Handle retirement */
		if (total_winner) kingly();

		/* Save memories */
		if (!save_player(FALSE)) msg_print("death save failed!");

		/* Dump bones file */
		make_bones();

		/* You are dead */
		print_tomb();

		/* Show more info */
		show_info();

		/* Handle score, show Top scores */
		top_twenty();
	}

	/* Still alive */
	else
	{
		/* Save the game */
		do_cmd_save_game(FALSE);

		/* Prompt for scores XXX XXX XXX */
		prt("Press Return (or Escape).", 0, 40);

		/* Predict score (or ESCAPE) */
		if (inkey() != ESCAPE) predict_score();
	}


	/* Shut the high score file */
	(void)fd_close(highscore_fd);

	/* Forget the high score fd */
	highscore_fd = -1;


	/* Allow suspending now */
	signals_handle_tstp();
}


/*
 * Handle abrupt death of the visual system
 *
 * This routine is called only in very rare situations, and only
 * by certain visual systems, when they experience fatal errors.
 *
 * XXX XXX Hack -- clear the death flag when creating a HANGUP
 * save file so that player can see tombstone when restart.
 */
void exit_game_panic(void)
{
	/* If nothing important has happened, just quit */
	if (!character_generated || character_saved) quit("panic");

	/* Mega-Hack -- see "msg_print()" */
	msg_flag = FALSE;

	/* Clear the top line */
	prt("", 0, 0);

	/* Hack -- turn off some things */
	disturb(1);

	/* Mega-Hack -- Delay death */
	if (p_ptr->chp < 0) death = FALSE;

	/* Hardcode panic save */
	panic_save = 1;

	/* Forbid suspend */
	signals_ignore_tstp();

	/* Indicate panic save */
	died_from = "(panic save)";

	/* Panic save, or get worried */
	if (!save_player(FALSE)) quit("panic save failed!");

	/* Successful panic save */
	quit("panic save succeeded!");
}


static errr get_rnd_line(const char * file_name, uint len, char * output)
{
	FILE        *fp;

	char	buf[1024];
    int lines=0, line, counter;

	/* Open the file */
	fp = my_fopen_path(ANGBAND_DIR_FILE, file_name, "r");

    /* Failed */
    if (!fp) return (-1);


	/* Parse the file */
	while (!my_fgets(fp, buf, sizeof(buf)))
	{
		/* Stop at the first line which is neither empty nor a comment. */
		if (buf[0] && buf[0] != '#')
		{
			lines = atoi(buf);
			break;
		}
	}

	/* Some sort of file error occurred. */
	if (!lines) return (1);

    line = randint(lines);
    for (counter = 0; counter <= line; counter++)
    {
    if (!(0 == my_fgets(fp, buf, sizeof(buf))))
        return (1);
    else if (counter == line)
        break;
    }

    sprintf(output, "%.*s", len-1, buf);

	/* Close the file */
    my_fclose(fp);

    return (0);
}

/*
 * Process the above as a vstrnfmt_aux function.
 *
 * Format: 
 * "%v", get_rnd_line_f1, (cptr)file_name
 * or:
 * "%.*v", (int)len, get_rnd_line_f1, file_name
 */
void get_rnd_line_f1(char *buf, uint max, cptr UNUSED fmt, va_list *vp)
{
	cptr file_name = va_arg(*vp, cptr);

	/* Report errors. */
	switch (get_rnd_line(file_name, max, buf))
	{
		case -1:
			strnfmt(buf, max, "Error: '%s' not found.", file_name);
			break;
		case 1:
			strnfmt(buf, max, "Error: '%s' failed to parse.", file_name);
	}
}

#ifdef HANDLE_SIGNALS


#include <signal.h>


/*
 * Wrapper around signal() which it is safe to take the address
 * of, in case signal itself is hidden by some some macro magic.
 */
static Signal_Handler_t wrap_signal(int sig, Signal_Handler_t handler)
{
	return signal(sig, handler);
}

/* Call this instead of calling signal() directly. */  
Signal_Handler_t (*signal_aux)(int, Signal_Handler_t) = wrap_signal;


/*
 * Handle signals -- suspend
 *
 * Actually suspend the game, and then resume cleanly
 */
static void handle_signal_suspend(int sig)
{
	/* Disable handler */
	(void)(*signal_aux)(sig, SIG_IGN);

#ifdef SIGSTOP

	/* Flush output */
	Term_fresh();

	/* Suspend the "Term" */
	Term_xtra(TERM_XTRA_ALIVE, 0);

	/* Suspend ourself */
	(void)kill(0, SIGSTOP);

	/* Resume the "Term" */
	Term_xtra(TERM_XTRA_ALIVE, 1);

	/* Redraw the term */
	Term_redraw();

	/* Flush the term */
	Term_fresh();

#endif

	/* Restore handler */
	(void)(*signal_aux)(sig, handle_signal_suspend);
}


/*
 * Handle signals -- simple (interrupt and quit)
 *
 * This function was causing a *huge* number of problems, so it has
 * been simplified greatly.  We keep a global variable which counts
 * the number of times the user attempts to kill the process, and
 * we commit suicide if the user does this a certain number of times.
 *
 * We attempt to give "feedback" to the user as he approaches the
 * suicide thresh-hold, but without penalizing accidental keypresses.
 *
 * To prevent messy accidents, we should reset this global variable
 * whenever the user enters a keypress, or something like that.
 */
static void handle_signal_simple(int sig)
{
	/* Disable handler */
	(void)(*signal_aux)(sig, SIG_IGN);


	/* Nothing to save, just quit */
	if (!character_generated || character_saved) quit(NULL);


	/* Count the signals */
	signal_count++;


	/* Terminate dead characters */
	if (death)
	{
		/* Mark the savefile */
		died_from = "Abortion";

		/* Close stuff */
		close_game();

		/* Quit */
		quit("interrupt");
	}

	/* Allow suicide (after 5) */
	else if (signal_count >= 5)
	{
		/* Cause of "death" */
		died_from = "Interrupting";

		/* Stop playing */
		alive = FALSE;

		/* Suicide */
		death = TRUE;

		{
			monster_race *r_ptr = r_info+MON_SUICIDE;
			if (r_ptr->r_deaths < MAX_SHORT) r_ptr->r_deaths++;
		}

		/* Close stuff */
		close_game();

		/* Quit */
		quit("interrupt");
	}

	/* Give warning (after 4) */
	else if (signal_count >= 4)
	{
		/* Make a noise */
		Term_xtra(TERM_XTRA_NOISE, 0);

		/* Clear the top line */
		Term_erase(0, 0, 255);

		/* Display the cause */
		Term_putstr(0, 0, -1, TERM_WHITE, "Contemplating suicide!");

		/* Flush */
		Term_fresh();
	}

	/* Give warning (after 2) */
	else if (signal_count >= 2)
	{
		/* Make a noise */
		Term_xtra(TERM_XTRA_NOISE, 0);
	}

	/* Restore handler */
	(void)(*signal_aux)(sig, handle_signal_simple);
}


/*
 * Handle signal -- abort, kill, etc
 */
static void handle_signal_abort(int sig)
{
	/* Disable handler */
	(void)(*signal_aux)(sig, SIG_IGN);


	/* Nothing to save, just quit */
	if (!character_generated || character_saved) quit(NULL);


	/* Clear the bottom line */
	Term_erase(0, 23, 255);

	/* Give a warning */
	Term_putstr(0, 23, -1, TERM_RED,
	            "A gruesome software bug LEAPS out at you!");

	/* Message */
	Term_putstr(45, 23, -1, TERM_RED, "Panic save...");

	/* Flush output */
	Term_fresh();

	/* Panic Save */
	panic_save = 1;

	/* Panic save */
	died_from = "(panic save)";

	/* Forbid suspend */
	signals_ignore_tstp();

	/* Attempt to save */
	if (save_player(FALSE))
	{
		Term_putstr(45, 23, -1, TERM_RED, "Panic save succeeded!");
	}

	/* Save failed */
	else
	{
		Term_putstr(45, 23, -1, TERM_RED, "Panic save failed!");
	}

	/* Flush output */
	Term_fresh();

	/* Quit */
	quit("software bug");
}


#endif /* HANDLE_SIGNALS */


/*
 * Ignore SIGTSTP signals (keyboard suspend)
 */
static void signals_ignore_tstp(void)
{

#ifdef HANDLE_SIGNALS
# ifdef SIGTSTP
	(void)(*signal_aux)(SIGTSTP, SIG_IGN);
# endif
#endif /* HANDLE_SIGNALS */

}

/*
 * Handle SIGTSTP signals (keyboard suspend)
 */
static void signals_handle_tstp(void)
{

#ifdef HANDLE_SIGNALS
# ifdef SIGTSTP
	(void)(*signal_aux)(SIGTSTP, handle_signal_suspend);
# endif
#endif /* HANDLE_SIGNALS */

}


/*
 * Prepare to handle the relevant signals
 */
void signals_init(void)
{
#ifdef HANDLE_SIGNALS
#ifdef SIGHUP
	(void)(*signal_aux)(SIGHUP, SIG_IGN);
#endif


#ifdef SIGTSTP
	(void)(*signal_aux)(SIGTSTP, handle_signal_suspend);
#endif


#ifdef SIGINT
	(void)(*signal_aux)(SIGINT, handle_signal_simple);
#endif

#ifdef SIGQUIT
	(void)(*signal_aux)(SIGQUIT, handle_signal_simple);
#endif


#ifdef SIGFPE
	(void)(*signal_aux)(SIGFPE, handle_signal_abort);
#endif

#ifdef SIGILL
	(void)(*signal_aux)(SIGILL, handle_signal_abort);
#endif

#ifdef SIGTRAP
	(void)(*signal_aux)(SIGTRAP, handle_signal_abort);
#endif

#ifdef SIGIOT
	(void)(*signal_aux)(SIGIOT, handle_signal_abort);
#endif

#ifdef SIGKILL
	(void)(*signal_aux)(SIGKILL, handle_signal_abort);
#endif

#ifdef SIGBUS
	(void)(*signal_aux)(SIGBUS, handle_signal_abort);
#endif

#ifdef SIGSEGV
	(void)(*signal_aux)(SIGSEGV, handle_signal_abort);
#endif

#ifdef SIGTERM
	(void)(*signal_aux)(SIGTERM, handle_signal_abort);
#endif

#ifdef SIGPIPE
	(void)(*signal_aux)(SIGPIPE, handle_signal_abort);
#endif

#ifdef SIGEMT
	(void)(*signal_aux)(SIGEMT, handle_signal_abort);
#endif

#ifdef SIGDANGER
	(void)(*signal_aux)(SIGDANGER, handle_signal_abort);
#endif

#ifdef SIGSYS
	(void)(*signal_aux)(SIGSYS, handle_signal_abort);
#endif

#ifdef SIGXCPU
	(void)(*signal_aux)(SIGXCPU, handle_signal_abort);
#endif

#ifdef SIGPWR
	(void)(*signal_aux)(SIGPWR, handle_signal_abort);
#endif
#endif /* HANDLE_SIGNALS */
}

/*
 * Quit the game in an orderly fashion if an assert() call fails.
 */
void assert_fail(cptr error, cptr file, int line)
{
	void (*assert_fmt)(cptr fmt, ...);

	/* Forbid suspending. */
	signals_ignore_tstp();

	/* Save the game if allowed. */
#ifdef DEBUG_ASSERT_SAVE
	save_player(FALSE);
#endif /* DEBUG_ASSERT_SAVE */

	/* Quit via the specified mechanism. */
#ifdef DEBUG_ASSERT_CORE
	assert_fmt = core_fmt;
#else /* DEBUG_ASSERT_CORE */
	assert_fmt = quit_fmt;
#endif /* DEBUG_ASSERT_CORE */

	(*assert_fmt)("\nAssertion failed: %s\nin file %s\non line %d\n\n",
		error, file, line);
}
