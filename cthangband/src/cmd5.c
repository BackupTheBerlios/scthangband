#define CMD5_C
/* File: cmd5.c */

/* Purpose: Spell/Prayer commands */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */


#include "angband.h"


static bool spirit_okay(int spirit, bool call);
static void print_spirits(int *valid_spirits,int num,int y, int x);
static s32b favour_annoyance(magic_type *f_ptr);
static void annoy_spirit(spirit_type *s_ptr,u32b amount);

/*
 * Extract the book index from the object data.
 */
static book_type *k_idx_to_book(int i)
{
	object_kind *k_ptr = k_info+i;
	switch (k_ptr->tval)
	{
		/* Only some objects can be spellbooks. */
		case TV_SORCERY_BOOK: case TV_THAUMATURGY_BOOK: case TV_CHARM:
		case TV_CONJURATION_BOOK: case TV_NECROMANCY_BOOK:
		{
			if (k_ptr->extra) return book_info+(k_info[i].extra-1);
			else return 0;
		}
		default:
		{
			return 0;
		}
	}
}

/* Find an arbitrary spell in a book. */
#define a_spell_from(B) ((B)->info+iilog((B)->flags))

static int book_to_school(book_type *b_ptr)
{
	switch (a_spell_from(b_ptr)->skill1)
	{
		case SKILL_SORCERY: return SCH_SORCERY;
		case SKILL_THAUMATURGY: return SCH_THAUMATURGY;
		case SKILL_CONJURATION: return SCH_CONJURATION;
		case SKILL_NECROMANCY: return SCH_NECROMANCY;

		/* Not one of these books. */
		default: return -1;
	}
}

static book_type *spirit_to_book(int i)
{
	switch (i)
	{
		case 0: return book_info+BK_LIFE_0;
		case 2: return book_info+BK_LIFE_1;
		case 4: return book_info+BK_LIFE_2;
		case 6: return book_info+BK_LIFE_3;
		case 1: return book_info+BK_WILD_0;
		case 3: return book_info+BK_WILD_1;
		case 5: return book_info+BK_WILD_2;
		case 7: return book_info+BK_WILD_3;
		default: return 0;
	}
}

/*
 * Find the number a hermetic spell is associated with.
 */
static int spell_to_num(magic_type *s_ptr)
{
	book_type *b_ptr;
	for (b_ptr = book_info; b_ptr < END_PTR(book_info); b_ptr++)
	{
		int s = book_to_school(b_ptr);
		assert(s < MAX_SCHOOL);
		if (s < 0) continue;

		if (s_ptr >= b_ptr->info && s_ptr < b_ptr->info+MAX_SPELLS_PER_BOOK)
		{
			return s_ptr - b_ptr->info + MAX_SPELLS_PER_BOOK * s;
		}
	}
	return -1;
}

/*
 * And vice versa.
 */
magic_type *num_to_spell(int i)
{
	book_type *b_ptr;
	assert (i >= 0);

	for (b_ptr = book_info; b_ptr < END_PTR(book_info); b_ptr++)
	{
		int s = book_to_school(b_ptr);
		assert(s < MAX_SCHOOL);
		if (s < 0) continue;

		if (i/MAX_SPELLS_PER_BOOK == s)
		{
			return b_ptr->info + i%MAX_SPELLS_PER_BOOK;
		}
	}
	return NULL;
}

#define MINDCRAFT_BOOK (book_info+BK_MIND)




/*
 *
 * Combine the relevant skills for a given spell, then
 * divide the total by four to give an effective 'level'
 * of spellcasting
 *
 * This function always returns a minimum of 1
 * even if the skill levels are zero
 *
 */
int spell_skill(const magic_type *s_ptr)
{
	int total = skill_set[s_ptr->skill1].value;

	if (s_ptr->skill2 == SKILL_NONE)
		total *= 2;
	else
		total += skill_set[s_ptr->skill2].value;

	total /= 4; /* This gives a total of 0-50 */
	if (total == 0) total++; /* So that we have a minimum of 1 */
	return (total);
}

/*
 * Give the stat a spell uses.
 */
static int spell_stat(const magic_type *s_ptr)
{
	switch (s_ptr->skill1)
	{
		case SKILL_THAUMATURGY:
		case SKILL_SORCERY:
		case SKILL_CONJURATION:
		case SKILL_NECROMANCY:
		case SKILL_HEDGE:
			return A_INT;
		case SKILL_MINDCRAFTING:
			return A_WIS;
		case SKILL_SHAMAN:
			return A_CHR;
		default: /* Paranoia */
			return A_STR;
	}
}
			

/*
 * Cast a spell (listed at an offset).
 */
static bool use_spell(magic_type *s_ptr)
{
	return use_known_power(-1024 + s_ptr->power, spell_skill(s_ptr));
}

/*
 * Check for low mana, etc., and adjust the fail rate appropriately.
 */
static void low_mana_check(int *chance, const magic_type *s_ptr)
{
	switch (s_ptr->skill1)
	{
		case SKILL_SORCERY:
		case SKILL_NECROMANCY:
		case SKILL_THAUMATURGY:
		case SKILL_CONJURATION:
		{
			if (s_ptr->mana > p_ptr->csp)
			{
				(*chance) += 5 * (s_ptr->mana - p_ptr->csp);
			}
			return;
		}
		case SKILL_MINDCRAFTING:
		{
			/* Not enough mana to cast */
			if (s_ptr->mana > p_ptr->cchi)
			{
				chance += 5 * (s_ptr->mana - p_ptr->cchi);
			}
			return;
		}
	}
}

/*
 * List the spells which can be cast from b_ptr
 */
static int build_spell_list(byte *s, const book_type *b_ptr)
{
	int i, j;
	assert(b_ptr);

	/* Extract spells */
	for (i = j = 0; i < MAX_SPELLS_PER_BOOK; i++)
	{
		/* Check for this spell */
		if (b_ptr->flags & (1L << i))
		{
			/* Collect this spell */
			s[j++] = i;
		}
	}
	return j;
}

/*
 * Returns spell chance of failure for an arbitrary spell
 */
static int spell_chance(const magic_type *s_ptr)
{
	/* Extract the base spell failure rate */
	int chance = s_ptr->fail;
	const int stat = spell_stat(s_ptr);
	const int minfail = adj_mag_fail[p_ptr->stat_ind[stat]];

	/* Reduce failure rate by "effective" level adjustment */
	chance -= 3 * (spell_skill(s_ptr) - s_ptr->min);

	/* Reduce failure rate by INT/WIS adjustment */
	chance -= 3 * (adj_mag_stat[p_ptr->stat_ind[stat]] - 1);

	/* Not enough mana to cast */
	low_mana_check(&chance, s_ptr);

	/* Minimum failure rate */
	if (chance < minfail) chance = minfail;

	/* Stunning makes spells harder */
	if (p_ptr->stun > 50) chance += 25;
	else if (p_ptr->stun) chance += 15;

	/* Always a 5 percent chance of working */
	if (chance > 95) chance = 95;

	/* Return the chance */
	return (chance);
}

/*
 * Determine if a spell is "okay" for the player to cast
 */
static bool magic_okay(const magic_type *s_ptr)
{
	return (s_ptr->min <= spell_skill(s_ptr));
}

/* Give experience to spell skills for a spell */
static void gain_spell_exp(magic_type *spell)
{
	bool check_mana = FALSE;
	int min_skill = spell->min * 2;

	int skill = spell->skill1;
	if (skill_set[skill].value < min_skill + 50)
	{
		skill_exp(skill);
		check_mana = TRUE;
	}

	skill = spell->skill2;
	if (skill != SKILL_NONE && skill_set[skill].value < min_skill + 50)
	{
		skill_exp(skill);
		check_mana = TRUE;
	}

	if (check_mana) skill_exp(SKILL_MANA);
}

/*
 * Evaluate "i op d".
 */
static int convert_magic_number(int i, char op, int d)
{
	switch (op)
	{
		case ';': return d;
		case '+': return i+d;
		case '-': return i-d;
		case '*': return i*d;
		case '/': return i/d;
		case '%': return i%d;

		/* Paranoia - bad symbol. */
		default: return -255;
	}
}

/*
 * Replace vstr with v in str, and evaluate formulae it appears in from left
 * to right.
 *
 * Hack - this can only interpret fairly simple formulae, and may need to be
 * replaced if this becomes insufficient.
 */
static void convert_magic_text(char *buf, uint max, cptr str,
	cptr vstr, int v)
{
	const int vl = strlen(vstr);
	int i;
	cptr s;
	char *t, op;
	bool b;
	for (s = str, t = buf, i = 0, op = ';', b = FALSE; *s && t < buf+max-1; )
	{
		int d = 0;
		if (prefix(s, vstr))
		{
			/* Interpret the symbol. */
			d = v;
			s += vl;
			b = TRUE;
			i = convert_magic_number(i, op, d);
		}
		else if (!b)
		{
			/* Formulae always start with vstr. */
			*t++ = *s++;
		}
		else if (isdigit(*s))
		{
			d = (int)strtol(s, 0, 0);
			while (isdigit(*s)) s++;
			i = convert_magic_number(i, op, d);
		}
		else if (*s == ';')
		{
			/* Add the number. */
			t += strnfmt(t, buf+max-t, "%d", i);
			b = FALSE;
		}
		else
		{
			op = *s++;
 			assert(strchr(";+-*/%", op));
		}
	}

	if (op != ';')
		/* Add the final number. */
		strnfmt(t, buf+max-t, "%d", i);
	else
		/* Simply terminate. */
		*t = '\0';
}

static void convert_magic_text_f3(char *buf, uint max, cptr UNUSED fmt,
	va_list *vp)
{
	cptr str = va_arg(*vp, cptr);
	cptr vstr = va_arg(*vp, cptr);
	int v = va_arg(*vp, int);

	convert_magic_text(buf, max, str, vstr, v); 
}

/*
 * Hack - describe powers which are too strange to describe below.
 */
static cptr magic_info_special(magic_type *s_ptr)
{
	int l = spell_skill(s_ptr);
	switch (s_ptr->power)
	{
		case SP_MINOR_DISPLACEMENT:
		{
			if (l < 25)
				return " range 10";
			else
				return " range LEV+2";
		}
		case SP_TELEKINETIC_WAVE:
		{
			if (l < 40)
				return " dam LEV*3";
			else
				return " dam LEV*4";
		}
		default:
		{
			return NULL;
		}
	}
}

static void get_magic_info(char *p, uint max, magic_type *s_ptr)
{
	cptr str = s_ptr->desc;
	if (!str) str = magic_info_special(s_ptr);

	assert(str);

	/* Convert the symbols in str, if any. */
	str = format("%v", convert_magic_text_f3, str, "LEV", spell_skill(s_ptr));
	str = format("%v", convert_magic_text_f3, str, "CHP", p_ptr->chp);

	sprintf(p, "%.*s", max-1, str);
}

/*
 * Return the energy used by casting a spell
 * This starts at 100 and then drops exponentially until
 * it reaches 10, then stops
 */
u16b spell_energy(u16b skill,u16b min)
{
	u32b en;
	
	/* Safety check to prevent overflows */
	if (min >= skill) 
	{
		/* Base calculation gives a square curve */
		en=TURN_ENERGY+((min-skill)*(min-skill)*TURN_ENERGY/100);
		if (en > 3*TURN_ENERGY) en = 3*TURN_ENERGY;
	}
	else
	{
		/* base calculation to give an inverse curve */
		en = 3*TURN_ENERGY/(skill-min);
		/* Force limits */
		if (en > TURN_ENERGY) en = TURN_ENERGY;
		if (en < TURN_ENERGY/10) en = TURN_ENERGY/10;
	}

	return (u16b)(en);
}

/*
 * Determine the energy required to cast a given spell
 */
static u16b magic_energy(magic_type *s_ptr)
{
	return spell_energy(spell_skill(s_ptr), s_ptr->min);
}

/*
 * Create a string describing a spell in a particular style.
 *
 * i is the index of the spell.
 * s_ptr is the spell (or 0 for the top line).
 * comment is the magic info for the spell's desc parameter.
 *
 * It should return the intended top line if called with s_ptr = 0.
 * Otherwise, it should return the desired description.
 * In either case, a return of 0 causes 
 */

static cptr cantrip_string(int i, magic_type *s_ptr, cptr comment)
{
	if (!s_ptr) return format("     %35s%s", "Name", "Sk Fail Info");

	if (spell_skill(s_ptr) < s_ptr->min) comment = " too hard";

	return format("  %c) %-35s%2d %3d%%%s",
		I2A(i), s_ptr->name, s_ptr->min*2, spell_chance(s_ptr), comment);
}

static cptr favour_string(int i, magic_type *s_ptr, cptr comment)
{
	if (!s_ptr) return format("     %-35s%s", "Name", "Sk Time Fail Info");

	if (spell_skill(s_ptr) < s_ptr->min) comment = " too hard";

	return format("  %c) %-35s%2d %4d %3d%%%s",
		I2A(i), s_ptr->name, s_ptr->min*2, magic_energy(s_ptr),
		spell_chance(s_ptr), comment);
}

static cptr mindcraft_string(int i, magic_type *s_ptr, cptr comment)
{
	if (!s_ptr) return format("     %-30s%s", "Name","Sk  Chi Time Fail Info");

	/* Don't print "too hard" spells at all. */
	if (spell_skill(s_ptr) < s_ptr->min) return 0;

	return format("  %c) %-30s%2d %4d %4d %3d%%%s", I2A(i), s_ptr->name,
		s_ptr->min*2, s_ptr->mana, magic_energy(s_ptr), spell_chance(s_ptr),
		comment);
}

/*
 * As above, but with some colour information.
 */
static cptr c_mindcraft_string(int i, magic_type *s_ptr, cptr comment)
{
	cptr str = mindcraft_string(i, s_ptr, comment);
	if (s_ptr && s_ptr->mana > p_ptr->cchi) str = format("$o%s", str);
	return str;
}

static cptr spell_string(int i, magic_type *s_ptr, cptr comment)
{
	cptr type;

	if (!s_ptr)
		return format("     %-26s%s", "Name", "Ty(Sk) Mana Time Fail Info");

	switch(s_ptr->skill2)
	{
		case SKILL_CORPORIS: type = "Co"; break;
		case SKILL_NATURAE: type = "Na"; break;
		case SKILL_VIS: type = "Vi"; break;
		case SKILL_ANIMAE: type = "An"; break;
		default: type = "  ";
	}

	if (s_ptr->flags & MAGIC_FORGOT)
	{
		comment = " forgotten";
	}
	else if (~s_ptr->flags & MAGIC_LEARNED)
	{
		if (spell_skill(s_ptr)<s_ptr->min)
		{
			 comment = " too hard";
		}
		else
		{
			comment = " unknown";
		}
	}
	else if (~s_ptr->flags & MAGIC_WORKED)
	{
		comment = " untried";
	}


	return format("  %c) %-26s%s(%2d) %4d %4d %3d%%%s",
		I2A(i), s_ptr->name, type, s_ptr->min*2, s_ptr->mana,
		magic_energy(s_ptr), spell_chance(s_ptr), comment);
}

/*
 * Print a list of spells of some sort (for casting or learning)
 */
static int print_spell_list(byte *spells, book_type *b_ptr, int num,
	int y, int x, cptr (*get)(int, magic_type *, cptr))
{
	int i;
	char info[80];
	cptr str;

 	/* Hack - Treat an 'x' value of -1 as a request for a default value. */
	if (x == -1) x = 15;

   /* Title the list */
    prt((*get)(0, 0, 0), y++, x);

    /* Dump the spells. */
	for (i = 0; i < num; i++)
	{
		/* Access the spell. */
		magic_type *s_ptr = &(b_ptr->info[spells[i]]);

		get_magic_info(info, sizeof(info), s_ptr);
			
		/* Get the spell description. */
		str = (*get)(i, s_ptr, info);

		/* Print if allowed. */
		if (str) mc_put_fmt(y++, x, "%s%v", str, clear_f0);
	}

	/* Clear the bottom line */
	prt("", y, x);

	/* Return it. */
	return y;
}

static int print_cantrips(byte *spells, book_type *b_ptr, int num, int y, int x)
{
	return print_spell_list(spells, b_ptr, num, y, x, cantrip_string);
}

static int print_favours(byte *spells, book_type *b_ptr, int num, int y, int x)
{
	return print_spell_list(spells, b_ptr, num, y, x, favour_string);
}

static int print_spells(byte *spells, int num, int y, int x, book_type *b_ptr)
{
	return print_spell_list(spells, b_ptr, num, y, x, spell_string);
}

static int print_mindcraft(book_type *b_ptr, int x, int y, bool colour)
{
	byte spells[MAX_SPELLS_PER_BOOK];
	int num = build_spell_list(spells, b_ptr);
	return print_spell_list(spells, b_ptr, num, y, x, (colour) ?
		c_mindcraft_string : mindcraft_string);
}

/*
 * Determine if a spell is "okay" for the player to cast or study
 * The spell must be legible, not forgotten, and also, to cast,
 * it must be known, and to study, it must not be known.
 */
static bool spell_okay(magic_type *s_ptr, bool known)
{
	/* Spell is illegal */
	if (!magic_okay(s_ptr)) return (FALSE);

    /* Spell is forgotten */
    if (s_ptr->flags & MAGIC_FORGOT)
	{
		/* Never okay */
		return (FALSE);
	}

	/* Spell is learned */
	if (s_ptr->flags & MAGIC_LEARNED)
	{
		/* Okay to cast, not to study */
		return (known);
	}

	/* Okay to study, not to cast */
	return (!known);
}



/*
 * Allow user to choose a spell from the given book.
 *
 * If a valid spell is chosen, saves it in '*sn' and returns TRUE
 * If the user hits escape, returns FALSE, and set '*sn' to -1
 * If there are no legal choices, returns FALSE, and sets '*sn' to -2
 *
 * The "prompt" should be "cast", "recite", or "study"
 * The "known" should be TRUE for cast/pray, FALSE for study
 */
static int get_spell(int *sn, cptr prompt, bool known, book_type *b_ptr)
{
	int		i;
	int		spell = -1;
	int		num = 0;
	int		ask;

	byte		spells[64];

	bool		flag, redraw, okay;
	char		choice;

	magic_type	*s_ptr;

	char		out_val[160];

	cptr p = "spell";


 #ifdef ALLOW_REPEAT
 
     /* Get the spell, if available */
     if (repeat_pull(sn)) {
         
         /* Verify the spell */
         if (spell_okay(&b_ptr->info[*sn], known)) {
                    
             /* Success */
             return (TRUE);
         }
     }
     
 #endif /* ALLOW_REPEAT -- TNB */
 

	/* Extract spells */
	for (spell = 0; spell < 32; spell++)
	{
		/* Check for this spell */
		if (b_ptr->flags & (1L << spell))
		{
			/* Collect this spell */
			spells[num++] = spell;
		}
	}

	/* Assume no usable spells */
	okay = FALSE;

	/* Assume no spells available */
	(*sn) = -2;

	/* Check for "okay" spells */
	for (i = 0; i < num; i++)
	{
		/* Look for "okay" spells */
		if (spell_okay(&b_ptr->info[spells[i]], known)) okay = TRUE;
	}

	/* No "okay" spells */
	if (!okay) return (FALSE);

	/* Assume cancelled */
	*sn = (-1);

	/* Nothing chosen yet */
	flag = FALSE;

	if (show_choices_main)
	{
		/* Show list */
		redraw = TRUE;
		Term_save();
		print_spells(spells, num, 1, -1, b_ptr);
	}		
	else
	{
	/* No redraw yet */
	redraw = FALSE;
	}

	/* Show choices */
	if (show_choices)
	{
		/* Update */
		p_ptr->window |= (PW_SPELL);
	}


	/* Build a prompt (accept all spells) */
	strnfmt(out_val, 78, "(%^ss %c-%c, *=List, ESC=exit) %^s which %s? ",
		p, I2A(0), I2A(num - 1), prompt, p);

	/* Get a spell from the user */
	while (!flag && get_com(out_val, &choice))
	{
		/* Request redraw */
		if ((choice == ' ') || (choice == '*') || (choice == '?'))
		{
			/* Show the list */
			if (!redraw)
			{
				/* Show list */
				redraw = TRUE;

				/* Save the screen */
				Term_save();

				/* Display a list of spells */
				print_spells(spells, num, 1, -1, b_ptr);
			}

			/* Hide the list */
			else
			{
				/* Hide list */
				redraw = FALSE;

				/* Restore the screen */
				Term_load();
			}

			/* Redo asking */
			continue;
		}


		/* Note verify */
		ask = (isupper(choice));

		/* Lowercase */
		if (ask) choice = tolower(choice);

		/* Extract request */
		i = (islower(choice) ? A2I(choice) : -1);

		/* Totally Illegal */
		if ((i < 0) || (i >= num))
		{
			bell(0);
			continue;
		}

		/* Save the spell index */
		spell = spells[i];

		/* Require "okay" spells */
		if (!spell_okay(&b_ptr->info[spell], known))
		{
			bell(0);
			msg_format("You may not %s that %s.", prompt, p);
			continue;
		}

		/* Verify it */
		if (ask)
		{
			char tmp_val[160];

			/* Access the spell */
			s_ptr = &b_ptr->info[spell%32];

			/* Prompt */
			strnfmt(tmp_val, 78, "%^s %s (%d mana, %d%% fail)? ",
				prompt, s_ptr->name, s_ptr->mana, spell_chance(s_ptr));

			/* Belay that order */
			if (!get_check(tmp_val)) continue;
		}

		/* Stop the loop */
		flag = TRUE;
	}


	/* Restore the screen */
	if (redraw) Term_load();


	/* Show choices */
	if (show_choices)
	{
		/* Update */
		p_ptr->window |= (PW_SPELL);
	}


	/* Abort if needed */
	if (!flag) return (FALSE);

	/* Save the choice */
	(*sn) = spell;

 #ifdef ALLOW_REPEAT
 
 	repeat_push(*sn);
 
 #endif /* ALLOW_REPEAT -- TNB */
 
	/* Success */
	return (TRUE);
}

/*
 * Allow user to choose a cantrip from the given charm.
 *
 * If a valid cantrip is chosen, saves it in '*sn' and returns TRUE
 * If the user hits escape, returns FALSE, and set '*sn' to -1
 * If there are no legal choices, returns FALSE, and sets '*sn' to -2
 *
 */
static int get_cantrip(int *sn, book_type *b_ptr)
{
	int		i;
	int		spell = -1;
	int		num = 0;
	int		ask;

	byte		spells[64];

	bool		flag, redraw, okay;
	char		choice;

	magic_type	*s_ptr;

	char		out_val[160];

 #ifdef ALLOW_REPEAT
 
     /* Get the spell, if available */
     if (repeat_pull(sn)) {
         
         /* Verify the spell */
         if (magic_okay(&b_ptr->info[*sn])) {
                    
             /* Success */
             return (TRUE);
         }
     }
     
 #endif /* ALLOW_REPEAT -- TNB */
 

	/* Count the spells out. */
	num = build_spell_list(spells, b_ptr);

	/* Assume no usable spells */
	okay = FALSE;

	/* Assume no spells available */
	(*sn) = -2;

	/* Check for "okay" spells */
	for (i = 0; i < num; i++)
	{
		/* Look for "okay" spells */
		if (magic_okay(b_ptr->info+spells[i])) okay = TRUE;
	}

	/* No "okay" spells */
	if (!okay) return (FALSE);

	/* Assume cancelled */
	*sn = (-1);

	/* Nothing chosen yet */
	flag = FALSE;

	if (show_choices_main)
	{
		/* Show list */
		redraw = TRUE;
		Term_save();
		print_cantrips(spells, b_ptr, num, 1, 20);
	}		
	else
	{
	/* No redraw yet */
	redraw = FALSE;
	}

	/* Show choices */
	if (show_choices)
	{
		/* Update */
		p_ptr->window |= (PW_SPELL);
	}


	/* Build a prompt (accept all spells) */
	strnfmt(out_val, 78, "(%c-%c, *=List, ESC=exit) Cast which cantrip? ",
		I2A(0), I2A(num - 1));

	/* Get a spell from the user */
	while (!flag && get_com(out_val, &choice))
	{
		/* Request redraw */
		if ((choice == ' ') || (choice == '*') || (choice == '?'))
		{
			/* Show the list */
			if (!redraw)
			{
				/* Show list */
				redraw = TRUE;

				/* Save the screen */
				Term_save();

				/* Display a list of spells */
				print_cantrips(spells, b_ptr, num, 1, 20);
			}

			/* Hide the list */
			else
			{
				/* Hide list */
				redraw = FALSE;

				/* Restore the screen */
				Term_load();
			}

			/* Redo asking */
			continue;
		}


		/* Note verify */
		ask = (isupper(choice));

		/* Lowercase */
		if (ask) choice = tolower(choice);

		/* Extract request */
		i = (islower(choice) ? A2I(choice) : -1);

		/* Totally Illegal */
		if ((i < 0) || (i >= num))
		{
			bell(0);
			continue;
		}

		/* Save the spell index */
		spell = spells[i];

		/* Require "okay" spells */
		if (!magic_okay(&b_ptr->info[spell]))
		{
			bell(0);
			msg_print("You may not cast that cantrip.");
			continue;
		}

		/* Verify it */
		if (ask)
		{
			char tmp_val[160];

			/* Access the spell */
			s_ptr = &(b_ptr->info[spell]);

			/* Prompt */
			strnfmt(tmp_val, 78, "cast %s (%d%% fail)? ",
				&b_ptr->info[spell].name, spell_chance(s_ptr));

			/* Belay that order */
			if (!get_check(tmp_val)) continue;
		}

		/* Stop the loop */
		flag = TRUE;
	}


	/* Restore the screen */
	if (redraw) Term_load();


	/* Show choices */
	if (show_choices)
	{
		/* Update */
		p_ptr->window |= (PW_SPELL);
	}


	/* Abort if needed */
	if (!flag) return (FALSE);

	/* Save the choice */
	(*sn) = spell;

 #ifdef ALLOW_REPEAT
 
 	repeat_push(*sn);
 
 #endif /* ALLOW_REPEAT -- TNB */
 
	/* Success */
	return (TRUE);
}

/*
 * Allow user to choose a favour from the given spirit.
 *
 * If a valid favour is chosen, saves it in '*sn' and returns TRUE
 * If the user hits escape, returns FALSE, and set '*sn' to -1
 * If there are no legal choices, returns FALSE, and sets '*sn' to -2
 *
 */
static int get_favour(int *sn, book_type *b_ptr)
{
	int		i;
	int		spell = -1;
	int		num = 0;
	int		ask;

	byte		spells[64];

	bool		flag, redraw, okay;
	char		choice;

	magic_type	*s_ptr;

	char		out_val[160];

 #ifdef ALLOW_REPEAT
 
     /* Get the spell, if available */
     if (repeat_pull(sn)) {
         
         /* Verify the spell */
         if (magic_okay(&b_ptr->info[*sn])) {
                    
             /* Success */
             return (TRUE);
         }
     }
     
 #endif /* ALLOW_REPEAT -- TNB */
 

	/* Count the spells out. */
	num = build_spell_list(spells, b_ptr);

	/* Assume no usable spells */
	okay = FALSE;

	/* Assume no spells available */
	(*sn) = -2;

	/* Check for "okay" spells */
	for (i = 0; i < num; i++)
	{
		/* Look for "okay" spells */
		if (magic_okay(&b_ptr->info[spells[i]])) okay = TRUE;
	}

	/* No "okay" spells */
	if (!okay) return (FALSE);

	/* Assume cancelled */
	*sn = (-1);

	/* Nothing chosen yet */
	flag = FALSE;

	if (show_choices_main)
	{
		/* Show list */
		redraw = TRUE;
		Term_save();
		print_favours(spells, b_ptr, num, 1, -1);
	}		
	else
	{
	/* No redraw yet */
	redraw = FALSE;
	}

	/* Show choices */
	if (show_choices)
	{
		/* Update */
		p_ptr->window |= (PW_SPELL);
	}


	/* Build a prompt (accept all spells) */
	strnfmt(out_val, 78, "(%c-%c, *=List, ESC=exit) invoke which favour? ",
		I2A(0), I2A(num - 1));

	/* Get a spell from the user */
	while (!flag && get_com(out_val, &choice))
	{
		/* Request redraw */
		if ((choice == ' ') || (choice == '*') || (choice == '?'))
		{
			/* Show the list */
			if (!redraw)
			{
				/* Show list */
				redraw = TRUE;

				/* Save the screen */
				Term_save();

				/* Display a list of spells */
				print_favours(spells, b_ptr, num, 1, -1);
			}

			/* Hide the list */
			else
			{
				/* Hide list */
				redraw = FALSE;

				/* Restore the screen */
				Term_load();
			}

			/* Redo asking */
			continue;
		}


		/* Note verify */
		ask = (isupper(choice));

		/* Lowercase */
		if (ask) choice = tolower(choice);

		/* Extract request */
		i = (islower(choice) ? A2I(choice) : -1);

		/* Totally Illegal */
		if ((i < 0) || (i >= num))
		{
			bell(0);
			continue;
		}

		/* Save the spell index */
		spell = spells[i];

		/* Require "okay" spells */
		if (!magic_okay(&b_ptr->info[spell]))
		{
			bell(0);
			msg_print("You may not invoke that favour.");
			continue;
		}

		/* Verify it */
		if (ask)
		{
			char tmp_val[160];

			/* Access the spell */
			s_ptr = &b_ptr->info[spell];

			/* Prompt */
			strnfmt(tmp_val, 78, "%s (%d mana, %d%% fail)? ",
				s_ptr->name, s_ptr->mana, spell_chance(s_ptr));

			/* Belay that order */
			if (!get_check(tmp_val)) continue;
		}

		/* Stop the loop */
		flag = TRUE;
	}

	/* Restore the screen */
	if (redraw) Term_load();


	/* Show choices */
	if (show_choices)
	{
		/* Update */
		p_ptr->window |= (PW_SPELL);
	}


	/* Abort if needed */
	if (!flag) return (FALSE);

	/* Save the choice */
	(*sn) = spell;

 #ifdef ALLOW_REPEAT
 
 	repeat_push(*sn);
 
 #endif /* ALLOW_REPEAT -- TNB */
 
	/* Success */
	return (TRUE);
}

/*
 * Allow user to choose a spirit.
 *
 * If a valid spell is chosen, saves it in '*sn' and returns TRUE
 * If the user hits escape, returns FALSE, and set '*sn' to -1
 * If there are no legal choices, returns FALSE, and sets '*sn' to -2
 *
 * The "prompt" should be "form a pact with" or "call upon"
 * The "call" should be TRUE for call upon, FALSE for call a pact with
 */
int get_spirit(int *sn, cptr prompt, bool call)
{
	int		i;
	int		ask;
	bool		flag, redraw, okay;
	char		choice;
	char		out_val[160];
	int valid_spirits[MAX_SPIRITS],total;

 #ifdef ALLOW_REPEAT
 
     /* Get the spirit, if available */
     if (repeat_pull(sn)) {
         
         /* Verify the spirit */
         if (spirit_okay(*sn, call)) {
                    
             /* Success */
             return (TRUE);
         }
     }
     
 #endif /* ALLOW_REPEAT -- TNB */
 

	/* Assume no usable spells */
	okay = FALSE;

	/* Assume no spirits available */
	(*sn) = -2;

	/* Check for "okay" spirits */
	total=0;
	for (i = 0; i < MAX_SPIRITS; i++)
	{
			/* Look for "okay" spirits */
			if (spirit_okay(i, call))
			{
				okay = TRUE;
				valid_spirits[total++] = i;
			}
	}

	/* No "okay" spells */
	if (!okay) return (FALSE);

	/* Assume cancelled */
	*sn = (-1);

	/* Nothing chosen yet */
	flag = FALSE;

	/* Show choices */
	if (show_choices)
	{
		/* Update */
		p_ptr->window |= (PW_SPELL);
	}


	if (show_choices_main)
	{
		/* Show list */
		redraw = TRUE;
		Term_save();
		print_spirits(valid_spirits,total,1, -1);
	}
	else
	{
		/* No redraw yet */
		redraw = FALSE;
	}
	/* Build a prompt (accept all spirits) */
	strnfmt(out_val, 78, "(%c-%c, *=List, ESC=exit) %^s which spirit? ",
		I2A(0), I2A(total - 1), prompt);

	/* Get a spell from the user */
	while (!flag && get_com(out_val, &choice))
	{
		/* Request redraw */
		if ((choice == ' ') || (choice == '*') || (choice == '?'))
		{
			/* Show the list */
			if (!redraw)
			{
				/* Show list */
				redraw = TRUE;

				/* Save the screen */
				Term_save();

				/* Display a list of spirits */
				print_spirits(valid_spirits,total,1, -1);
			}

			/* Hide the list */
			else
			{
				/* Hide list */
				redraw = FALSE;

				/* Restore the screen */
				Term_load();
			}

			/* Redo asking */
			continue;
		}

		/* Default option if unambiguous. */
		if (choice == '\n' && total == 1)
		{
			choice = 'a';
		}

		/* Note verify */
		ask = (isupper(choice));

		/* Lowercase */
		if (ask) choice = tolower(choice);

		/* Extract request */
		i = (islower(choice) ? valid_spirits[A2I(choice)] : isdigit(choice) ? choice-'0' : -1);

		/* Totally Illegal */
		if ((i < 0) || (i >= MAX_SPIRITS))
		{
			bell(0);
			continue;
		}

		/* Require "okay" spells */
		if (!spirit_okay(i, call))
		{
			bell(0);
			msg_format("You may not %s that spirit.", prompt);
			continue;
		}

		/* Stop the loop */
		flag = TRUE;
	}

	/* Restore the screen */
	if (redraw) Term_load();


	/* Show choices */
	if (show_choices)
	{
		/* Update */
		p_ptr->window |= (PW_SPELL);
	}


	/* Abort if needed */
	if (!flag) return (FALSE);

	/* Save the choice */
	(*sn) = i;

 #ifdef ALLOW_REPEAT
 
 	repeat_push(*sn);
 
 #endif /* ALLOW_REPEAT -- TNB */
 
	/* Success */
	return (TRUE);
}


/*
 * Determine if a spirit is "okay" for the player to form a pact with to or call upon
 * Set call to true to test if a spirit can be called upon
 * Set call to false to test if a spirit can form a pact
 */
static bool spirit_okay(int spirit, bool call)
{

	const int plev = MAX(1, skill_set[SKILL_SHAMAN].value/2);
	spirit_type *s_ptr;

	/* Access the spell */
    s_ptr = &(spirits[spirit]);

	/* Spirit is too powerful */
	if (s_ptr->minskill > plev) return (FALSE);

	/* Spirit has a pact */
    if (s_ptr->pact)
	{
		/* Okay to call upon, not to form a pact with */
		return (call);
	}

	/* Okay to form a pact with, not to call upon */
	return (!call);
}


/*
 * Print a list of spirits (for initiating to or calling upon)
 */
static void print_spirits(int *valid_spirits,int num,int y, int x)
{
	int                     i;
	const int plev = MAX(1, skill_set[SKILL_SHAMAN].value/2);
	spirit_type              *s_ptr;
	cptr            comment;
	char            out_val[160];
	char			full_name[80];

	/* Hack - Treat an 'x' value of -1 as a request for a default value. */
	if (x == -1) x = 15;

    /* Title the list */
    prt("", y, x);
	put_str("Name", y, x + 5);
	put_str("Info", y, x + 48);


    /* Dump the spirits */
	for (i = 0; i < num; i++)
	{
		/* Access the spirit */
		s_ptr = &(spirits[valid_spirits[i]]);

		/* Analyze the spirit */
		if (s_ptr->minskill > plev)
		{
			comment = " too powerful";
		}
		else if(!(s_ptr->pact))
		{
			comment = " no pact";
		}
		else if(s_ptr->annoyance > 15)
		{
			comment = " furious";
		}
		else if(s_ptr->annoyance > 8)
		{
			comment = " angry";
		}
		else if(s_ptr->annoyance > 3)
		{
			comment = " annoyed";
		}
		else if(s_ptr->annoyance > 0)
		{
			comment = " irritated";
		}
		else
		{
			comment = " placated";
		}
		/* Pre-process the spirit name and description */
		sprintf(full_name,"%s, %s",s_ptr->name,s_ptr->desc);
		/* Now insert it into the line */
		sprintf(out_val, "  %c/%d) %-42s%s",
		I2A(i), valid_spirits[i], full_name,comment);
		prt(out_val, y + i + 1, x);
	}

	/* Clear the bottom line */
	prt("", y + i + 1, x);
}


/*
 * Is it a book?
 */
static PURE bool item_tester_book(object_type *o_ptr)
{
	switch (o_ptr->tval)
	{
		case TV_SORCERY_BOOK:
		case TV_THAUMATURGY_BOOK:
		case TV_CONJURATION_BOOK:
		case TV_NECROMANCY_BOOK:
			return TRUE;
		default:
			return FALSE;
	}
}

/*
 * Does it have a book_type associated with it?
 */
static PURE bool item_tester_book_etc(object_type *o_ptr)
{
	return (k_idx_to_book(o_ptr->k_idx) != 0);
}

bool PURE display_spells_p(object_type *o_ptr)
{
	item_tester_hook = item_tester_book_etc;
	return item_tester_okay(o_ptr);
}

void display_spells(int y, int x, object_type *o_ptr)
{
	book_type *b_ptr;
	int		num;

	byte		spells[64];

	assert(display_spells_p(o_ptr));

	/* Access the item's spell list. */
	b_ptr = k_idx_to_book(o_ptr->k_idx);

	/* Count the spells out. */
	num = build_spell_list(spells, b_ptr);

	/* Display the spells */
	print_spells(spells, num, y, x, b_ptr);
}

/*
 * Peruse the spells/prayers in a Book
 *
 * Note that *all* spells in the book are listed
 *
 * Note that browsing is allowed while confused or blind,
 * and in the dark, primarily to allow browsing in stores.
 */
void do_cmd_browse(object_type *o_ptr)
{
	/* Get an item if we do not already have one */
	if(!o_ptr)
	{
		errr err;

		/* Restrict choices to books */
		item_tester_hook = item_tester_book_etc;

		/* Get an item (from inven or floor) */
		if (!((o_ptr = get_item(&err, "Browse which book? ", FALSE, TRUE, TRUE))))
		{
			if (err == -2) msg_print("You have no books that you can read.");
			return;
		}
	}

	if(!display_spells_p(o_ptr))
	{
		msg_print("You can't read that.");
		return;
	}

	/* Track the object kind */
	object_kind_track(o_ptr->k_idx);

	/* Hack -- Handle stuff */
	handle_stuff();

	/* Save the screen */
	Term_save();

	/* Display the spells */
	display_spells(1, -1, o_ptr);

	/* Clear the top line */
	prt("", 0, 0);

	/* Prompt user */
	put_str("[Press any key to continue]", 0, 23);

	/* Wait for key */
	(void)inkey();

	/* Restore the screen */
	Term_load();
}




/*
 * Study a book to gain a new spell/prayer
 */
void do_cmd_study(void)
{
	errr err;
	int	i;
	int	spell_school = 0;
	int	spell = -1;

	cptr p = "spell";

	object_type             *o_ptr;
	book_type *b_ptr;
	magic_type *s_ptr;

	if (p_ptr->blind)
	{
		msg_print("You cannot see!");
		return;
	}

	if (p_ptr->confused)
	{
		msg_print("You are too confused!");
		return;
	}

	if (!(p_ptr->new_spells))
	{
		msg_format("You cannot learn any new %ss!", p);
		return;
	}

	msg_format("You can learn %d new %s%s.", p_ptr->new_spells, p,
		(p_ptr->new_spells == 1?"":"s"));
	msg_print(NULL);


	/* Restrict choices to "useful" books */
	item_tester_hook = item_tester_book;

	/* Get an item (from inven or floor) */
	if (!((o_ptr = get_item(&err, "Study which book? ", FALSE, TRUE, TRUE))))
	{
		if (err == -2) msg_print("You have no books that you can read.");
		return;
	}

	/* Access the item's spell list. */
	b_ptr = k_idx_to_book(o_ptr->k_idx);
	assert(b_ptr);

	spell_school=o_ptr->tval - 90;

	/* Track the object kind */
	object_kind_track(o_ptr->k_idx);

	/* Hack -- Handle stuff */
	handle_stuff();

	/* Ask for a spell, allow cancel */
	if (!get_spell(&spell, "study", FALSE, b_ptr)
	&& (spell == -1)) return;

	/* Nothing to study */
	if (spell < 0)
	{
		/* Message */
		msg_format("You cannot learn any %ss in that book.", p);

		/* Abort */
		return;
	}

	/* Find the spell. */
	s_ptr = b_ptr->info+spell;

	/* Take a turn */
	energy_use = extract_energy[p_ptr->pspeed];


	/* Learn the spell */
	s_ptr->flags |= MAGIC_LEARNED;

	/* Find the next open entry in "spell_order[]" */
	for (i = 0; i < 128; i++)
	{
		/* Stop at the first empty space */
		if (spell_order[i] == 255) break;
	}

	assert(i < 128);

	/* Add the spell to the known list */
	spell_order[i++] = spell_to_num(s_ptr);

	/* Mention the result */
	msg_format("You have learned the %s of %s.", p, s_ptr->name);

	/* Sound */
	sound(SOUND_STUDY);

	/* Update spells. */
	p_ptr->update |= PU_SPELLS;
	update_stuff();

	/* Redraw Study Status */
	p_ptr->redraw |= (PR_STUDY);
}

/*
 * Cast a spell
 */
void do_cmd_cast(void)
{
	errr err;
	int	spell, chance;
	int	plev = 0;
	int	spell_school = 0;

	const cptr prayer = "spell";

	object_type	*o_ptr;
	book_type *b_ptr;

	magic_type	*s_ptr;

	/* Require lite */
	if (p_ptr->blind || no_lite())
	{
		msg_print("You cannot see!");
		return;
	}

	/* Not when confused */
	if (p_ptr->confused)
	{
		msg_print("You are too confused!");
		return;
	}


	/* Restrict choices to spell books */
	item_tester_hook = item_tester_book;

	/* Get an item (from inven or floor) */
	if (!((o_ptr = get_item(&err, "Use which book? ", FALSE, TRUE, TRUE))))
	{
        if (err == -2) msg_format("You have no %s books!", prayer);
		return;
	}

	/* Access the item's spell list. */
	b_ptr = k_idx_to_book(o_ptr->k_idx);
	assert(b_ptr);

	spell_school=o_ptr->tval - 90;

	/* Track the object kind */
	object_kind_track(o_ptr->k_idx);

	/* Hack -- Handle stuff */
	handle_stuff();

	/* Ask for a spell */
	if (!get_spell(&spell,"cast", TRUE, b_ptr))
	{
		if (spell == -2)
		msg_format("You don't know any %ss in that book.", prayer);
		return;
	}


	/* Access the spell */
	s_ptr = &b_ptr->info[spell];

	plev = spell_skill(s_ptr);

	/* Verify "dangerous" spells */
	if (s_ptr->mana > p_ptr->csp)
	{
		/* Warning */
		msg_format("You do not have enough mana to %s this %s.",
			"cast",
			prayer);

		/* Verify */
		if (!get_check("Attempt it anyway? ")) return;
	}


	/* Spell failure chance */
	chance = spell_chance(s_ptr);

	/* Failed spell */
	if (rand_int(100) < chance)
	{
		if (flush_failure) flush();

		msg_format("You failed to get the %s off!", prayer);

		if (o_ptr->tval == TV_THAUMATURGY_BOOK && (randint(100)<spell))
		{
			msg_print("You produce a chaotic effect!");
			wild_magic(spell);
		}
	}

	/* Process spell */
	else
	{
		if (!use_spell(s_ptr)) return;

		/* A spell was cast */
		s_ptr->flags |= MAGIC_WORKED;

		/* Gain experience with spell skills and mana */
		gain_spell_exp(s_ptr);
	}

	/* Take some time - a spell of your level takes 100, lower level spells take less */
	energy_use = spell_energy((u16b)plev,(u16b)(s_ptr->min));

	/* Sufficient mana */
	if (s_ptr->mana <= p_ptr->csp)
	{
		/* Use some mana */
		p_ptr->csp -= s_ptr->mana;
	}

	/* Over-exert the player */
	else
	{
		int oops = s_ptr->mana - p_ptr->csp;

		/* No mana left */
		p_ptr->csp = 0;
		p_ptr->csp_frac = 0;

		/* Message */
		msg_print("You faint from the effort!");

		/* Hack -- Bypass free action */
		(void)add_flag(TIMED_PARALYZED, randint(5 * oops + 1));

		/* Damage CON (possibly permanently) */
		if (rand_int(100) < 50)
		{
			bool perm = (rand_int(100) < 25);

			/* Message */
			msg_print("You have damaged your health!");

			/* Reduce constitution */
			(void)dec_stat(A_CON, 15 + randint(10), perm);
		}
	}

	/* Redraw mana */
	p_ptr->redraw |= (PR_MANA);

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);
	p_ptr->window |=(PW_SPELL);
}

/*
 * Cast a cantrip
 */
void do_cmd_cantrip(void)
{
	errr err;
	int	spell, chance;

	const cptr prayer = "cantrip";
	bool item_break = FALSE;

	object_type	*o_ptr;

	magic_type	*s_ptr;
	book_type *b_ptr;

	/* Require lite */
	if (p_ptr->blind || no_lite())
	{
		msg_print("You cannot see!");
		return;
	}

	/* Not when confused */
	if (p_ptr->confused)
	{
		msg_print("You are too confused!");
		return;
	}


	/* Restrict choices to charms */
	item_tester_tval = TV_CHARM;

	/* Get an item (from inven or floor) */
	if (!((o_ptr = get_item(&err, "Use which charm? ", TRUE, TRUE, TRUE))))
	{
        if (err == -2) msg_print("You have no charms!");
		return;
	}

	/* Access the item's spell list. */
	b_ptr = k_idx_to_book(o_ptr->k_idx);

	/* Track the object kind */
	object_kind_track(o_ptr->k_idx);

	/* Hack -- Handle stuff */
	handle_stuff();

	/* Ask for a spell */
	if (!get_cantrip(&spell, b_ptr))
	{
		if (spell == -2)
		msg_format("You don't know any %ss for that charm.", prayer);
		return;
	}

	s_ptr = &(b_ptr->info[spell]);

	/* Spell failure chance */
	chance = spell_chance(s_ptr);

	/* Failed spell */
	if (rand_int(100) < chance)
	{
		if (flush_failure) flush();

		msg_format("You failed to cast the %s!", prayer);
		/* Charm *always* breaks if the spell fails */
		item_break = TRUE;
	}

	/* Process spell */
	else
	{
		/* Aborted spells cost nothing. */
		if (!use_spell(s_ptr)) return;

		/* A cantrip was cast */
		/* Gain experience with hedge skill */
		if (skill_set[SKILL_HEDGE].value < s_ptr->min + 50) {
			skill_exp(SKILL_HEDGE);
		}
	}

	/* Take some time - a cantrip always takes 100, unless the charm is in a pouch */
	energy_use = item_use_energy(o_ptr);

	/* If item is going to break, give it a chance of survival at low skill levels, on
	 * the assumption that the user didn't manage to do anything to the charm.  This will
	 * allow the practice of hedge magic at low skill levels without destroying so many
	 * charms.
	 *
	 * Otherwise, give it a slight chance of breaking anyway.
	 */
	if (item_break) {
		if (rand_int(10) > skill_set[SKILL_HEDGE].value) {
			msg_print("The charm remains completely inert.");
			item_break = FALSE;
		}
	} else {
		if (rand_int(1000) < s_ptr->mana) item_break=TRUE;
	}

	if (item_break)
	{
		/* Dangerous Hack -- Destroy the item */
		msg_print("The charm crumbles, drained of magic.");

		/* Reduce and describe item. */
		item_increase(o_ptr, -1);
		item_describe(o_ptr);
		item_optimize(o_ptr);
	}
	/* Redraw mana */
	p_ptr->redraw |= (PR_MANA);

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);
	p_ptr->window |=(PW_SPELL);
}


/*
 * calculate the annoyance factor of a favour
 */
static s32b favour_annoyance(magic_type *f_ptr)
{
	s32b annoy;

	/* Base annoyance is taken from the spell */
	annoy = f_ptr->mana + rand_int(10)+5;

	/* decrease based on charisma bonus for studying and skill*/
	annoy -= (adj_mag_study[p_ptr->stat_ind[A_CHR]] * (skill_set[SKILL_SHAMAN].value/20));

	/* make sure there is at least a bit */
	if (annoy <2) annoy = 2;
	
	return (annoy);
}

/*
 * annoy a spirit
 */
static void annoy_spirit(spirit_type *s_ptr,u32b amount)
{
	u32b old_annoy;
	p_ptr->redraw |= (PR_SPIRIT);
	old_annoy=s_ptr->annoyance;
	s_ptr->annoyance += amount;
	
	if ((s_ptr->annoyance > 15) && (old_annoy < 16))
	{
		msg_format("%s is furious.",s_ptr->name);
	}
	else if((s_ptr->annoyance > 8) && (old_annoy < 9))
	{
		msg_format("%s gets angry.",s_ptr->name);
	}
	else if((s_ptr->annoyance > 3) && (old_annoy < 4))
	{
		msg_format("%s is getting annoyed.",s_ptr->name);
	}
	else if((s_ptr->annoyance > 0) && (old_annoy < 1))
	{
		msg_format("You have irritated %s.",s_ptr->name);
	}
}

/*
 * Give a special "angry spirit" effect some of the time (dependent on favour
 * level).
 *
 * This makes more severe punishments more common for high level spells,
 * compounding the relative rarity that any punishment will occur.
 *
 * Return the extra anger this should cause to avoid double messages.
 */
static int spirit_punish(spirit_type *s_ptr, magic_type *f_ptr)
{
	s32b i = rand_int(100000);

	/* Redraw stuff (always). */
	p_ptr->redraw |= (PR_SPIRIT);

	/* Do nothing from 40-85% of the time. */
	if (rand_int(100) > s_ptr->punish_chance) return 0;

	/* Warn of a punishment. */
	msg_format("%s is enraged!", s_ptr->name);

	if (cheat_peek)
	{
		msg_format("Roll: %ld. Need %d for summon, %d for abandonment.",
			i, f_ptr->min * 1000, f_ptr->min * f_ptr->min);
	}

 	/* Abandonment. up to 2% chance for a level 45 favour. */
	if (i < f_ptr->min * f_ptr->min)
	{
		msg_format("%s disowns you!", s_ptr->name);

		s_ptr->pact = FALSE;

		/* You won't see this one again... */
		generate_spirit_name(s_ptr);

		/* No other effect makes sense now. */
		return 0;
	}

	/* Summoning, up to 43% chance for a level 45 favour. */
	if (i < f_ptr->min * 1000)
	{
		/* Choose an appropriate summon type most of the time. */
		int j, type = (!rand_int(3)) ? 0 : (s_ptr->sphere == SPIRIT_NATURE) ?
			SUMMON_ANIMAL : SUMMON_LIVING;

		msg_format("%s sends some monsters to teach you a lesson!", s_ptr->name);

		for (j = 0; j < 1000; j++)
		{
			if (summon_specific(py, px, dun_depth, type) && !rand_int(3)) break;
		}

		/* Nothing happened. */
		if (j == 1000) msg_format("They get lost on the way...");
	}

	/* Annoy the spirit even more. */
	return rand_range(15, 150);
}

/*
 * invoke a favour
 */
void do_cmd_invoke(void)
{
	int	spell, chance;
	int spirit;

	spirit_type	*s_ptr;
	book_type *b_ptr;
	magic_type	*f_ptr;

	/* Not when confused */
	if (p_ptr->confused)
	{
		msg_print("You are too confused!");
		return;
	}

	/* Ask for a spirit */
	if (!get_spirit(&spirit,"call upon",TRUE))
	{
		if (spirit == -2)
		{
			msg_print("You must form a pact with a spirit before they will listen.");
		}
		return;
	}

	/* Get a pointer to the spirit */
	s_ptr = &(spirits[spirit]);

	/* Get a pointer to its spells. */
	b_ptr = spirit_to_book(spirit);

	/* Hack -- Handle stuff */
	handle_stuff();

	/* Ask for a favour */
	if (!get_favour(&spell, b_ptr))
	{
		if (spell == -2)
		msg_print("Strange - spirits normally know spells!");
		return;
	}


	f_ptr = &b_ptr->info[spell];

	/* Spell failure chance */
	chance = spell_chance(f_ptr);

	/* Normal energy use. */
	energy_use = magic_energy(f_ptr);

	/* Don't punish those silly enough to pray to an angry spirit too harshly. */
	if (s_ptr->annoyance)
	{
		if (flush_failure) flush();
		msg_format("You feel that %s isn't listening...", s_ptr->name);
		/* And sit still for a little while to avoid this being a short wait command. */
		energy_use = extract_energy[p_ptr->pspeed];
	}
	/* Failed spell */
	else if (rand_int(100) < chance)
	{
		int anger;
		if (flush_failure) flush();
		msg_format("%s refuses your call!",s_ptr->name);
		/* Chance for retribution based on level of favour */
		anger = spirit_punish(s_ptr, f_ptr);
		/* The spirit gets somewhat pissed off if it hasn't left. */
		if (s_ptr->pact)
			annoy_spirit(s_ptr,anger + rand_int(favour_annoyance(f_ptr)));
	}
	/* Process spell */
	else
	{
		/* The spirit gets pissed off (even with aborted spells). */
		annoy_spirit(s_ptr,favour_annoyance(f_ptr));

		/* Abort. */
		if (use_spell(f_ptr))
		{
			/* Gain experience with spirit lore skill */
			if (skill_set[SKILL_SHAMAN].value < f_ptr->min * 2 + 50)
				skill_exp(SKILL_SHAMAN);
		}
	}

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);
	p_ptr->window |=(PW_SPELL);
}



/*
 * Return a line of help text appropriate for the given mindcraft power
 * at the given level. 
 * NB: This may return the format buffer.
 */
static cptr mindcraft_help(const int power, const int skill)
{
	switch (power)
	{
		case SP_PRECOGNITION:
		{
			cptr board[6], init = "Detects";
			int j = 0;

			if (skill >= 90)
				return "Lights the dungeon and detects everything in it.";

			if (skill <= 39) board[j++] = "monsters";
			else board[j++] = "all monsters";
			if (skill >= 10) board[j++] = "traps";
			if (skill >= 30) board[j++] = "doors";
			if (skill >= 30) board[j++] = "stairs";
			if (skill >= 40) board[j++] = "walls";
			if (skill >= 60) board[j++] = "objects";
			return list_flags(init, "and", board, j);
		}
		case SP_NEURAL_BLAST:
		{
			return "Fires a bolt of mental energy at a monster.";
		}
		case SP_MINOR_DISPLACEMENT:
		{
			if (skill < 50) return "Teleports you a short distance away.";
			else return "Teleports you to a nearby spot of your choosing.";
		}
		case SP_MAJOR_DISPLACEMENT:
		{
			if (skill < 60) return "Teleports you far away.";
			else return "Teleports you, and other nearby monsters, far away.";
		}
		case SP_DOMINATION:
		{
			if (skill < 60) return "Charms a monster.";
			else return "Charms all nearby monsters.";
		}
		case SP_PULVERISE:
		{
			return "Creates a ball of sound at a location of your choosing.";
		}
		case SP_CHARACTER_ARMOUR:
		{
			cptr board[6];
			cptr init = "Gives AC and resistance to";
			int j = 0;

			if (skill >= 30) board[j++] = "acid";
			if (skill >= 40) board[j++] = "fire";
			if (skill >= 50) board[j++] = "cold";
			if (skill >= 60) board[j++] = "electricity";
			if (skill >= 70) board[j++] = "poison";

			if (j)
				return list_flags(init, "and", board, j);
			else
				return "Helps to protect you from melee attack.";
		}
		case SP_PSYCHOMETRY:
		{
			if (skill >= 80) return "Identifies an object.";
			else return "Pseudo-identifies an object.";
		}
		case SP_MIND_WAVE:
		{
			if (skill >= 50)
				return "Fires mental energy at all visible monsters.";
			else
				return "Fires mental energy at nearby monsters.";
		}
		case SP_ADRENALINE_CHANNELING:
		{
			if (skill >= 70)
				return "Heals you, hastes you and drives you berserk.";
			else
				return "Heals you, hastes you and makes you heroic.";
		}
		case SP_PSYCHIC_DRAIN:
		{
			return "Fires mental energy at nearby monsters to gain extra chi.";
		}
		case SP_TELEKINETIC_WAVE:
		{
			return "Harms, stuns and teleports nearby monsters.";
		}
		default: /* Paranoia */
		{
			return "Does something which has no help written for it.";
		}
	}
}

/*
 * Allow user to choose a mindcrafter power.
 *
 * If a valid spell is chosen, saves it in '*sn' and returns TRUE
 * If the user hits escape, returns FALSE, and set '*sn' to -1
 * If there are no legal choices, returns FALSE, and sets '*sn' to -2
 *
 * The "prompt" should be "cast", "recite", or "study"
 * The "known" should be TRUE for cast/pray, FALSE for study
 *
 * nb: This function has a (trivial) display bug which will be obvious
 * when you run it. It's probably easy to fix but I haven't tried,
 * sorry.
 */
static int get_mindcraft_power(book_type *b_ptr, int *sn)
{
	int                     i;

	int                     num = 0;
    int y = 1, UNREAD(maxy);
    int x = 15;
	int  psi = spell_skill(&b_ptr->info[0]);
    bool            flag, redraw;
    int             ask;
	char            choice;
	char            out_val[160];
	magic_type *s_ptr;
    
    cptr p = "power";

	/* Assume cancelled */
	*sn = (-1);

 #ifdef ALLOW_REPEAT
 
     /* Get the spell, if available */
     if (repeat_pull(sn)) {
         
         /* Verify the spell */
         if (b_ptr->info[*sn].min <= psi) {
 
             /* Success */
             return (TRUE);
         }
     }
     
 #endif /* ALLOW_REPEAT -- TNB */
 

	/* Nothing chosen yet */
	flag = FALSE;
	if (show_choices_main)
	{
		/* Show list */
		redraw = TRUE;
		Term_save();
		maxy = print_mindcraft(b_ptr, x, y, FALSE);
	}		
	else
	{
	/* No redraw yet */
	redraw = FALSE;
	}

	for (i = 0; i < MAX_MINDCRAFT_POWERS; i++)
	{
		if (b_ptr->info[i].min <= psi) num++;
	}

	/* Build a prompt (accept all spells) */
    strnfmt(out_val, 78, "(%^ss %c-%c, *=List, ESC=exit) Use which %s? ",
		p, I2A(0), I2A(num - 1), p);

	/* Get a spell from the user */
	while (!flag && get_com(out_val, &choice))
	{
		/* Request redraw */
        if ((choice == ' ') || (choice == '*') || (choice == '?'))
		{
            /* Show the list */
			if (!redraw)
			{
				/* Show list */
				redraw = TRUE;

				/* Save the screen */
				Term_save();

			    /* Display a list of spells */
				maxy = print_mindcraft(b_ptr, x,y, FALSE);
			}

			/* Hide the list */
			else
			{
				/* Hide list */
				redraw = FALSE;

				/* Restore the screen */
				Term_load();
			}

	    /* Redo asking */
			continue;
		}



		/* Note verify */
		ask = (isupper(choice));

		/* Lowercase */
		if (ask) choice = tolower(choice);

		/* Extract request */
		i = (islower(choice) ? A2I(choice) : -1);


		/* Totally Illegal */
		if ((i < 0) || (i >= num))
		{
			bell(0);
			continue;
		}

		/* Save the spell index */
		s_ptr = &b_ptr->info[i];

        /* Verify it */
		if (ask)
		{
			char tmp_val[160];

			/* Print a description of the power, given space. */
			if (redraw)
				put_str(mindcraft_help(s_ptr->power, psi*2), maxy, x);

			/* Prompt */
            strnfmt(tmp_val, 78, "Use %s? ", b_ptr->info[i].name);

			/* Belay that order */
			if (!get_check(tmp_val))
			{
				Term_erase(x, maxy, 255);
				continue;
			}
		}


		/* Stop the loop */
		flag = TRUE;
	}


	/* Restore the screen */
	if (redraw) Term_load();


	/* Show choices */
	if (show_choices)
	{
		/* Update */
		p_ptr->window |= (PW_SPELL);
	}


	/* Abort if needed */
	if (!flag) return (FALSE);

	/* Save the choice */
	(*sn) = i;
 
 #ifdef ALLOW_REPEAT
 
     repeat_push(*sn);
 
 #endif /* ALLOW_REPEAT -- TNB */

	/* Success */
	return (TRUE);
}


void do_cmd_mindcraft(void)
{
    int   n = 0,  b = 0;
    int chance;
	book_type *b_ptr = MINDCRAFT_BOOK;
    int psi = spell_skill(&b_ptr->info[0]);
	magic_type *s_ptr;
    
    /* not if confused */
    if (p_ptr->confused) {
	msg_print("You are too confused!");
	return;
    }
    
	/* get power */
	if (!get_mindcraft_power(b_ptr, &n))  return;
	
	s_ptr = &b_ptr->info[n];
    
    /* Verify "dangerous" spells */
    if (s_ptr->mana > p_ptr->cchi) {
	/* Warning */
	msg_print("You do not have enough chi to use this power.");
	/* Verify */
	if (!get_check("Attempt it anyway? ")) return;
    }
    
    /* Spell failure chance */
    chance = spell_chance(s_ptr);

    /* Failed spell */
	if (rand_int(100) < chance)
	{
	    if (flush_failure) flush();
	    msg_format("You failed to concentrate hard enough!");

	    if (randint(100) < (chance/2))
		{    /* Backfire */
			b = randint(100);
			if (b < 5)
			{
				msg_print("Oh, no! Your mind has gone blank!");
				lose_all_info();
			}
			else if (b < 15)
			{
				msg_print("Weird visions seem to dance before your eyes...");
				add_flag(TIMED_IMAGE, 5 + randint(10));
			}
			else if (b < 45)
			{
				msg_print("Your brain is addled!");
				add_flag(TIMED_CONFUSED, randint(8));
			}
			else if (b < 90)
			{
				add_flag(TIMED_STUN, randint(8));
			}
			else
			{   /* Mana storm */
				msg_print("Your mind unleashes its power in an uncontrollable storm!");
				project(1, 2+psi/10, py, px,
                psi * 2, GF_MANA,PROJECT_JUMP|PROJECT_KILL|PROJECT_GRID|PROJECT_ITEM);
				if (p_ptr->cchi < s_ptr->mana)
				{
				p_ptr->cchi = MAX(0, p_ptr->cchi - psi * MAX(1, psi/10));
				} else {
					p_ptr->cchi = MAX(s_ptr->mana, p_ptr->cchi - psi * MAX(1, psi/10));
				}
			}
	    }
	}
	else
	{
		/* Aborted. */
		if (!use_spell(s_ptr)) return;
		
		/* Get some practice */
		if (skill_set[SKILL_MINDCRAFTING].value < s_ptr->min * 2 + 50) {
			skill_exp(SKILL_MINDCRAFTING);
			skill_exp(SKILL_CHI);
		}
	}
	/* Take a turn */
	energy_use = magic_energy(s_ptr);

	/* Sufficient mana */
	if (s_ptr->mana <= p_ptr->cchi)
	{
		/* Use some mana */
		p_ptr->cchi -= s_ptr->mana;
	}

	/* Over-exert the player */
	else
	{
		int oops = s_ptr->mana - p_ptr->cchi;

		/* No mana left */
		p_ptr->cchi = 0;
		p_ptr->chi_frac = 0;

		/* Message */
		msg_print("You faint from the effort!");

		/* Hack -- Bypass free action */
        (void)add_flag(TIMED_PARALYZED, randint(5 * oops + 1));

		/* Damage WIS (possibly permanently) */
		if (rand_int(100) < 50)
		{
			bool perm = (rand_int(100) < 25);

			/* Message */
			msg_print("You have damaged your mind!");

			/* Reduce constitution */
			(void)dec_stat(A_WIS, 15 + randint(10), perm);
		}
	}

	/* Redraw mana */
	p_ptr->redraw |= (PR_MANA);

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);
	p_ptr->window |= (PW_SPELL);

}



/*
 * Hack -- Display all known spells in a window
 *
 * XXX XXX XXX Need to analyze size of the window.
 *
 * XXX XXX XXX Need more color coding.
 */
void display_spell_list(void)
{
	print_mindcraft(MINDCRAFT_BOOK, 1, 1, TRUE);
}

#ifdef CHECK_ARRAYS

/*
 * Check various things about the magic_info[] array.
 */
void check_magic_info(void)
{
	cptr error = 0;
	const book_type *b_ptr;
	const magic_type *s_ptr;
	char buf[1024];
	int i;
	for (b_ptr = book_info; b_ptr < END_PTR(book_info); b_ptr++)
	{
		if (!b_ptr->flags) quit_fmt("Book %d is empty.", b_ptr-book_info);

		for (i = 0; i < MAX_SPELLS_PER_BOOK; i++)
		{
			if (~b_ptr->flags & (1L << i)) continue;

			s_ptr = b_ptr->info+i;

			/* Try to generate the extra string. Failure gives an assert()
			 * error.
			 */
			get_magic_info(buf, 1023, b_ptr->info+i);

			/* Bounds test a few parameters. */
			if (s_ptr->skill1 >= MAX_SKILLS || s_ptr->skill2 >= MAX_SKILLS)
			{
				error = "uses a bad skill";
			}

			if (s_ptr->flags) error = "sets flags on initialisation";
			if (s_ptr->min > 50) error = "has a minimum skill over 50";

			if (error) quit_fmt("The %s spell %s.", s_ptr->name, error);
		}
	}
}

#endif /* CHECK_ARRAYS */
