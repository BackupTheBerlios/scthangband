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


static void print_favours(byte *spells, book_type *b_ptr, int num, int y, int x);
static bool spirit_okay(int spirit, bool call);
static void print_spirits(int *valid_spirits,int num,int y, int x);
static void rustproof(void);
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

static int k_idx_to_school(int i)
{
	book_type *b_ptr = k_idx_to_book(i);
	if (!b_ptr) return -2;
	for (i = 0; i < MAX_SPELLS_PER_BOOK; i++)
	{
		if (b_ptr->flags & (1L << i))
			return b_ptr->info[i].skill1;
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
		case MIND_DISP_1:
		{
			if (l < 25)
				return " range 10";
			else
				return " range LEV+2";
		}
		case MIND_TK_WAVE:
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

#define MAGIC_LEARNED 0x01
#define MAGIC_WORKED 0x02
#define MAGIC_FORGOT 0x04

/*
 * *Hack* - set the spell flags for an object based on the spell_*[] arrays.
 */
static void get_spell_flags(int k_idx)
{
	book_type *b_ptr = k_idx_to_book(k_idx);
	int i, sk = k_idx_to_school(k_idx);

	assert(b_ptr);

	/* No flags associated with this item. */
	if (sk < 0) return;

	for (i = 0; i < MAX_SPELLS_PER_BOOK; i++)
	{
		if (b_ptr->flags & (1L << i))
		{
			magic_type *s_ptr = &b_ptr->info[i];
			s_ptr->flags = 0;
			if (spell_learned[sk] & (1L << i)) s_ptr->flags |= MAGIC_LEARNED;
			if (spell_worked[sk] & (1L << i)) s_ptr->flags |= MAGIC_WORKED;
			if (spell_forgotten[sk] & (1L << i)) s_ptr->flags |= MAGIC_FORGOT;
		}
	}
}

static void set_spell_flags(int k_idx)
{
	book_type *b_ptr = k_idx_to_book(k_idx);
	int i, sk = k_idx_to_school(k_idx);

	assert(b_ptr);

	/* No flags associated with this item. */
	if (sk < 0) return;

	spell_learned[sk] = 0;
	spell_worked[sk] = 0;
	spell_forgotten[sk] = 0;

	for (i = 0; i < MAX_SPELLS_PER_BOOK; i++)
	{
		if (b_ptr->flags & (1L << i))
		{
			magic_type *s_ptr = &b_ptr->info[i];
			s_ptr->flags = 0;
			if (s_ptr->flags & MAGIC_LEARNED) spell_learned[sk] |= (1L << i);
			if (s_ptr->flags & MAGIC_WORKED) spell_worked[sk] |= (1L << i);
			if (s_ptr->flags & MAGIC_FORGOT) spell_forgotten[sk] |= (1L << i);
		}
	}
}

/*
 * Print a list of spells (for browsing or casting or viewing)
 */
static void print_spells(byte *spells, int num, int y, int x, book_type *b_ptr)
{
	int                     i, spell;

	magic_type              *s_ptr;

	cptr            comment, type;

	char            info[80];

	char            out_val[160];


	/* Hack - Treat an 'x' value of -1 as a request for a default value. */
	if (x == -1) x = 15;

	assert(b_ptr);

    /* Title the list */


    prt("", y, x);
	put_str("Name", y, x + 5);
	put_str("Ty(Sk) Mana Time Fail Info", y, x + 31);


    /* Dump the spells */
	for (i = 0; i < num; i++)
	{
		/* Access the spell */
		spell = spells[i];

	/* Access the spell */

		s_ptr = &b_ptr->info[spell];

		/* XXX XXX Could label spells above the players level */

		/* Get extra info */
		get_magic_info(info, 15, s_ptr);

		/* Use that info */
		comment = info;

		/* Analyze the spell */
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
		else if (s_ptr->flags & MAGIC_WORKED)
		{
			comment = " untried";
		}

		switch(s_ptr->skill2)
		{
			case SKILL_CORPORIS:
				type = "Co";
				break;
			case SKILL_NATURAE:
				type = "Na";
				break;
			case SKILL_VIS:
				type = "Vi";
				break;
			case SKILL_ANIMAE:
				type = "An";
				break;
			default:
				type = "  ";
		}

		/* Dump the spell --(-- */
		sprintf(out_val, "  %c) %-26s%s(%2d) %4d %4d %3d%%%s",
		I2A(i), s_ptr->name, type, s_ptr->min*2, s_ptr->mana,
			magic_energy(s_ptr), spell_chance(s_ptr), comment);
		prt(out_val, y + i + 1, x);
	}

	/* Clear the bottom line */
	prt("", y + i + 1, x);
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
 * Print a list of cantrips (for casting or learning)
 */
static void print_cantrips(book_type *b_ptr, byte *spells, int num,
	int y, int x)
{
	int                     i, spell;

	const int plev = MAX(1, skill_set[SKILL_HEDGE].value/2);
	magic_type              *s_ptr;

	cptr            comment;

	char info[80];

	char            out_val[160];

    /* Title the list */
    prt("", y, x);
	put_str("Name", y, x + 5);
	put_str("Sk Fail Info", y, x + 40);


    /* Dump the favours */
	for (i = 0; i < num; i++)
	{
		/* Access the favour */
		spell = spells[i];

		s_ptr = &(b_ptr->info[spell]);

		get_magic_info(info, 15, s_ptr);

		comment = info;

		if (s_ptr->min > plev)
		{
			comment = " too hard";
		}
			
		/* Dump the favour --(-- */
		sprintf(out_val, "  %c) %-35s%2d %3d%%%s",
		I2A(i), b_ptr->info[spell].name, /* spell */
		s_ptr->min*2, spell_chance(s_ptr), comment);
		prt(out_val, y + i + 1, x);
	}

	/* Clear the bottom line */
	prt("", y + i + 1, x);
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
		print_cantrips(b_ptr, spells, num, 1, 20);
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
				print_cantrips(b_ptr, spells, num, 1, 20);
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
 * Print a list of favours (for invoking)
 */
static void print_favours(byte *spells, book_type *b_ptr, int num, int y, int x)
{
	int                     i, spell;

	const int plev = MAX(1, skill_set[SKILL_SHAMAN].value/2);
	magic_type              *s_ptr;

	cptr            comment;

	char info[80];

	char            out_val[160];

	/* Hack - Treat an 'x' value of -1 as a request for a default value. */
	if (x == -1) x = 15;

    /* Title the list */
    prt("", y, x);
	put_str("Name", y, x + 5);
	put_str("Sk Time Fail Info", y, x + 40);


    /* Dump the favours */
	for (i = 0; i < num; i++)
	{
		/* Access the favour */
		spell = spells[i];

		s_ptr = &b_ptr->info[spell];

		get_magic_info(info, 80, s_ptr);

		comment = info;

		if (s_ptr->min > plev)
		{
			comment = " too hard";
		}
			
		/* Dump the favour --(-- */
		sprintf(out_val, "  %c) %-35s%2d %4d %3d%%%s",
		I2A(i), s_ptr->name, s_ptr->min*2, magic_energy(s_ptr),
			spell_chance(s_ptr), comment);
		prt(out_val, y + i + 1, x);
	}

	/* Clear the bottom line */
	prt("", y + i + 1, x);
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


static void rustproof(void)
{
	errr err;

	object_type	*o_ptr;

	/* Select a piece of armour */
	item_tester_hook = item_tester_hook_armour;

	/* Get an item (from equip or inven or floor) */
	if (!((o_ptr = get_item(&err, "Rustproof which piece of armour? ", TRUE, TRUE, TRUE))))
	{
		if (err == -2) msg_print("You have nothing to rustproof.");
		return;
	}


	o_ptr->flags3 |= TR3_IGNORE_ACID;

	if ((o_ptr->to_a < 0) && !(o_ptr->ident & IDENT_CURSED))
	{
		msg_format("%s %v look%s as good as new!",
			((is_inventory_p(o_ptr)) ? "Your" : "The"),
			object_desc_f3, o_ptr, FALSE, 0, ((o_ptr->number > 1) ? "" : "s"));
			o_ptr->to_a = 0;
	}

	msg_format("%s %v %s now protected against corrosion.",
		((is_inventory_p(o_ptr)) ? "Your" : "The"),
		object_desc_f3, o_ptr, FALSE, 0, ((o_ptr->number > 1) ? "are" : "is"));

	return;
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


	/* Take a turn */
	energy_use = extract_energy[p_ptr->pspeed];


	/* Learn the spell */
	b_ptr->info[spell].flags |= MAGIC_LEARNED;

	/* Find the next open entry in "spell_order[]" */
	for (i = 0; i < 128; i++)
	{
		/* Stop at the first empty space */
		if (spell_order[i] == 255) break;
	}

	/* Add the spell to the known list */
	spell_order[i++] = spell;

	/* Mention the result */
	msg_format("You have learned the %s of %s.",
		p, b_ptr->info[spell].name);

	/* Sound */
	sound(SOUND_STUDY);

	/* Update spells. */
	p_ptr->update |= PU_SPELLS;
	update_stuff();

	/* Redraw Study Status */
	p_ptr->redraw |= (PR_STUDY);
}

void do_poly_wounds(int cause)
{

    s16b wounds = p_ptr->cut, hit_p = (p_ptr->mhp - p_ptr->chp);
    s16b change = damroll(skill_set[SKILL_TOUGH].value/2, 5);
    bool Nasty_effect = (randint(5)==1);

    if (!(wounds || hit_p || Nasty_effect)) return;

    if (Nasty_effect)
        {
            msg_print("A new wound was created!");
            take_hit(change, "a polymorphed wound", cause);
            set_flag(TIMED_CUT, change);
        }
    else
        {
            msg_print("Your wounds are polymorphed into less serious ones.");
            hp_player(change);
            set_flag(TIMED_CUT, (p_ptr->cut)-(change/2));
        }
}

void do_poly_self(void)
{
int effects = randint(2);
int tmp = 0;
int new_race;
int more_effects = TRUE;
char buf[1024];

msg_print("You feel a change coming over you...");

while (effects-- && more_effects)
    {
        switch (randint(12))
        {
        case 1: case 2:
            do_poly_wounds(MON_FATAL_POLYMORPH);
            break;
        case 3: case 4:
            (void) gain_chaos_feature(0);
            break;
		case 5: case 6: case 7: /* Racial polymorph! Uh oh... */
          {
            do { new_race = randint(MAX_RACES) -1; } while (new_race == p_ptr->prace);

            msg_format("You turn into a%s %s!",
                ((new_race == RACE_ELF
                  || new_race == RACE_IMP)?"n":""),
                race_info[new_race].title);

                p_ptr->prace = new_race;
                rp_ptr = &race_info[p_ptr->prace];

				/* Access the "race" pref file */
				sprintf(buf, "%s.prf", rp_ptr->title);

				/* Process that file */
				process_pref_file(buf);

				/* Access the "font" or "graf" pref file, based on "use_graphics" */
				sprintf(buf, "%s-%s.prf", (use_graphics ? "graf" : "font"), ANGBAND_SYS);

				/* Process that file */
				process_pref_file(buf);

                /* Experience factor */
                p_ptr->expfact = rp_ptr->r_exp;

            /* Calculate the height/weight for males */
            if (p_ptr->psex == SEX_MALE)
            {
                p_ptr->ht = randnor(rp_ptr->m_b_ht, rp_ptr->m_m_ht);
                p_ptr->wt = randnor(rp_ptr->m_b_wt, rp_ptr->m_m_wt);
            }

            /* Calculate the height/weight for females */
                else if (p_ptr->psex == SEX_FEMALE)
            {
                p_ptr->ht = randnor(rp_ptr->f_b_ht, rp_ptr->f_m_ht);
                p_ptr->wt = randnor(rp_ptr->f_b_wt, rp_ptr->f_m_wt);
            }


            p_ptr->update |= (PU_BONUS);

            handle_stuff();
          }
          lite_spot(py, px);
          more_effects = FALSE; /* Stop here! */
          break;
        case 8: /* Purposedly "leaks" into default */
            msg_print("You polymorph into an abomination!");
            while (tmp < 6)
            {
                (void)dec_stat(tmp, randint(6)+6, (randint(3)==1));
                tmp++;
            }
            if (randint(6)==1)
            {
                msg_print("You find living difficult in your present form!");
                take_hit(damroll(randint(skill_set[SKILL_TOUGH].value/2),skill_set[SKILL_TOUGH].value/2), "a lethal chaos feature", MON_FATAL_POLYMORPH);
            }
            /* No break; here! */
        default:
            chaos_feature_shuffle();
    }
    }

}

static void phlogiston (void)
{

    int max_flog = 0;
    object_type * o_ptr = &inventory[INVEN_LITE];

	/* It's a lamp */
    if (o_ptr->k_idx == OBJ_BRASS_LANTERN)
	{
        max_flog = FUEL_LAMP;
	}

	/* It's a torch */
    else if (o_ptr->k_idx == OBJ_WOODEN_TORCH)
	{
        max_flog = FUEL_TORCH;
	}

	/* No torch to refill */
	else
	{
        msg_print("You are not wielding anything which uses phlogiston.");
        return;
	}

    if (o_ptr->pval >= max_flog)
    {
        msg_print("No more phlogiston can be put in this item.");
        return;
    }

	/* Refuel */
    o_ptr->pval += (max_flog / 2);

	/* Message */
    msg_print("You add phlogiston to your light item.");

	/* Comment */
    if (o_ptr->pval >= max_flog)
	{
        o_ptr->pval = max_flog;
        msg_print("Your light item is full.");
	}


	/* Recalculate torch */
	p_ptr->update |= (PU_TORCH);
}
    

static void brand_weapon(int brand_type)
{
	object_type *o_ptr = &inventory[INVEN_WIELD];

	/* you can never modify artifacts / ego-items */
    /* you can never modify cursed items */
    /* TY: You _can_ modify broken items (if you're silly enough) */
	if (o_ptr->k_idx && !allart_p(o_ptr) && !ego_item_p(o_ptr) && !cursed_p(o_ptr))
	{
		cptr act = NULL;
		byte name2;

	    switch (brand_type)
	    {
	        case 4:
	            act = "seems very unstable now.";
				name2 = EGO_PLANAR;
	            break;
	        case 3:
	            act = "thirsts for blood!";
				name2 = EGO_VAMPIRIC;
	            break;
	        case 2:
	            act = "is coated with poison.";
	            name2 = EGO_BRAND_POIS;
	            break;
	        case 1:
	            act = "is engulfed in raw chaos!";
	            name2 = EGO_CHAOTIC;
	            break;
	        default:
			if (rand_int(100) < 25)
			{
				act = "is covered in a fiery shield!";
				name2 = EGO_BRAND_FIRE;
			}
			else
			{
				act = "glows deep, icy blue!";
				name2 = EGO_BRAND_COLD;
			}
	    }

		/* Display a message. */
		msg_format("Your %v %s", object_desc_f3, o_ptr, FALSE, 0, act);

		/* Apply the enchantment. */
		o_ptr->name2 = name2;
		apply_magic_2(o_ptr, dun_depth);
		enchant(o_ptr, rand_int(3) + 4, ENCH_TOHIT | ENCH_TODAM);
	}

	else
	{
		if (flush_failure) flush();

        msg_print("The Branding failed.");
	}
}


static void call_the_(void)
{
	int i;
				
    if (cave_floor_bold(py-1,px-1) && cave_floor_bold(py-1, px) &&
        cave_floor_bold(py-1,px+1) && cave_floor_bold(py,px-1) &&
        cave_floor_bold(py,px+1) && cave_floor_bold(py+1,px-1) &&
        cave_floor_bold(py+1,px) && cave_floor_bold(py+1,px+1))
	{
		for (i = 1; i < 10; i++)
	    if (i-5) fire_ball(GF_SHARD, i,
				175, 2);

		for (i = 1; i < 10; i++)
	    if (i-5) fire_ball(GF_MANA, i,
				175, 3);
		
		for (i = 1; i < 10; i++)
	    if (i-5) fire_ball(GF_NUKE, i,
				175, 4);
	}
    else
    {
        msg_format("You %s the %s too close to a wall!",
            "cast",
            "spell");
        msg_print("There is a loud explosion!");
        destroy_area(py, px, 20+(skill_set[SKILL_THAUMATURGY].value/2), TRUE);
        msg_print("The dungeon collapses...");
        take_hit(100 + (randint(150)), "a suicidal Call the Void", MON_CALLING_THE_VOID);
    }
}

 /* Fetch an item (teleport it right underneath the caster) */
 void fetch(int dir, int wgt, bool require_los)
 {
       int ty, tx, i;
       bool flag;
       cave_type *c_ptr;
   object_type *o_ptr;

   /* Check to see if an object is already there */
   if(cave[py][px].o_idx)
   {
       msg_print("You can't fetch when you're already standing on something.");
       return;
   }

   /* Use a target */
   if(dir==5 && target_okay())
   {
       tx = target_col;
       ty = target_row;
       if(distance(py, px, ty, tx)>MAX_RANGE)
       {
           msg_print("You can't fetch something that far away!");
           return;
       }
       c_ptr = &cave[ty][tx];

       if (require_los && (!player_has_los_bold(ty,tx)))
       {
            msg_print("You have no direct line of sight to that location.");
            return;
        }
   }
   else
   {
       /* Use a direction */
       ty = py; /* Where to drop the item */
       tx = px;
       flag = FALSE;
       do
       {
           ty += ddy[dir];
           tx += ddx[dir];
           c_ptr = &cave[ty][tx];
           if ((distance(py, px, ty, tx)> MAX_RANGE)
               || !cave_floor_bold(ty, tx)) return;
       } while(!c_ptr->o_idx);
   }
   o_ptr = &o_list[c_ptr->o_idx];
   if (o_ptr->weight > wgt)
   {   /* Too heavy to 'fetch' */
       msg_print("The object is too heavy.");
       return;
   }
   i = c_ptr->o_idx;
   c_ptr->o_idx = 0;
   cave[py][px].o_idx = i; /* 'move' it */
   o_ptr->iy = (byte)py;
   o_ptr->ix = (byte)px;


   note_spot(py,px);
   p_ptr->redraw |= PR_MAP;

 }




static void wild_magic(int spell)
{
    int counter = 0;

    switch(randint(spell) + randint(8) + 1)

    {
        case 1: case 2: case 3:
            teleport_player(10);
            break;
        case 4: case 5: case 6:
            teleport_player(100);
            break;
        case 7: case 8:
            teleport_player(200);
            break;
        case 9: case 10: case 11:
            unlite_area(10,3);
            break;
        case 12: case 13: case 14:
            lite_area(damroll(2,3),2);
            break;
        case 15:
            destroy_doors_touch();
            break;
        case 16: case 17:
            wall_breaker(50);
			break;
        case 18:
            sleep_monsters_touch(50);
            break;
        case 19: case 20:
            trap_creation();
            break;
        case 21: case 22:
            door_creation();
            break;
        case 23: case 24: case 25:
            aggravate_monsters(1);
            break;
        case 26:
            earthquake(py, px, 5);
            break;
        case 27: case 28:
            (void) gain_chaos_feature(0);
            break;
        case 29: case 30:
            apply_disenchant(0);
            break;
        case 31:
            lose_all_info();
            break;
        case 32:
            fire_ball(GF_CHAOS, 0, spell + 5, 1 + (spell/10));
            break;
        case 33:
            wall_stone();
            break;
        case 34: case 35:
		{
			int wild_monsters[] =
			{
				SUMMON_MOULD,
				SUMMON_BAT,
				SUMMON_QUYLTHULG,
				SUMMON_VORTEX,
				SUMMON_TREASURE,
				SUMMON_MIMIC,
			};
			int type = wild_monsters[rand_int(N_ELEMENTS(wild_monsters))];
            while (counter++ < 8)
            {
            (void) summon_specific(py, px, ((dun_depth) * 3) / 2, type);
                    }
            break;
		}
        case 36: case 37:
            activate_hi_summon();
            break;
        case 38:
            summon_reaver();
			break;
        default:
            activate_ty_curse();
    }
    return;
}

/*
 * Cast a spell
 */
void do_cmd_cast(void)
{
	errr err;
	int	spell, dir;
	int	chance, beam;
	int	plev = 0;
	int	spell_school = 0, dummy = 0;
	int	i;

	bool	none_came = FALSE;
	const cptr prayer = "spell";

	object_type	*o_ptr;
	book_type *b_ptr;

	magic_type	*s_ptr;

        char	ppp[80];

        char	tmp_val[160];

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

	/* Hack - copy the flags for this book to the spells. */
	get_spell_flags(o_ptr->k_idx);

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

    beam = plev;


	/* Spells.  */
	switch (spell_school)
	{
	case 0: /* * Sorcery * */
	  switch (spell)
	  {
	   case 0: /* Detect Monsters */
			(void)detect_monsters_normal();
		       break;
	   case 1: /* Phase Door */
			teleport_player(10);
		       break;
	   case 2: /* Detect Doors and Traps */
			(void)detect_traps();
			(void)detect_doors();
			(void)detect_stairs();
		       break; 
       case 3: /* Light Area */
			(void)lite_area(damroll(2, (plev / 2)), (plev / 10) + 1);
            break;
	   case 4: /* Confuse Monster */
			if (!get_aim_dir(&dir)) return;
            (void)confuse_monster(dir, ( plev * 3) / 2 );
			break;
	   case 5: /* Teleport Self */
            teleport_player(plev * 5);
		       break;
	   case 6: /* Sleep Monster */
			if (!get_aim_dir(&dir)) return;
			(void)sleep_monster(dir,plev);
		       break;
	   case 7: /* Recharging */
               (void)recharge(plev * 2);
		       break;
	   case 8: /* Magic Mapping */
			map_area();
		       break;
	   case 9: /* Identify */
			(void)ident_spell();
		       break;
	   case 10: /* Slow Monster */
			if (!get_aim_dir(&dir)) return;
			(void)slow_monster(dir,plev);
		       break;
	   case 11: /* Mass Sleep */
			(void)sleep_monsters(plev);
		       break;
	   case 12: /* Teleport Away */
			if (!get_aim_dir(&dir)) return;
               (void)fire_beam(GF_AWAY_ALL, dir, plev);
		       break;
	   case 13: /* Haste Self */
			if (!p_ptr->fast)
			{
				(void)set_flag(TIMED_FAST, randint(20 + (plev) ) + plev);
			}
			else
			{
				(void)add_flag(TIMED_FAST, randint(5));
			}
		       break;
	   case 14: /* Detection True */
			(void)detect_all();
		       break;
	   case 15: /* Identify True */
			identify_fully();
		       break;
       case 16: /* Detect Objects and Treasure*/
			(void)detect_objects_normal();
			(void)detect_treasure();
			(void)detect_objects_gold();
		       break;
       case 17: /* Detect Enchantment */
			(void)detect_objects_magic();
		       break;
       case 18: /* Charm Monster */
                 if (!get_aim_dir(&dir)) return;
                 (void) charm_monster(dir, plev);
               break;
		case 19: /* Dimension Door */
		{
			if (!dimension_door(plev, 10)) return;
			break;
		}

       case 20: /* Sense Minds */
            (void)add_flag(TIMED_ESP, randint(30) + 25);
		       break;
       case 21: /* Self knowledge */
           (void)self_knowledge();
               break;
	   case 22: /* Teleport Level */
			(void)teleport_player_level();
		       break;
	   case 23: /* Word of Recall */
			{
				set_recall(TRUE);
				break;
			}
       case 24: /* Stasis */
			if (!get_aim_dir(&dir)) return;
			(void)stasis_monster(dir,plev);
		       break;
       case 25: /* Telekinesis */
         if (!get_aim_dir(&dir)) return;
         fetch(dir, plev*15, FALSE);
         break;
       case 26: /* Recharging True -- replaced by Explosive Rune */
               explosive_rune();
		       break;
	   case 27: /* Clairvoyance */
			wiz_lite();
            if (!(p_ptr->telepathy))
            {
                (void)add_flag(TIMED_ESP, randint(30) + 25);
            }
		       break;
	   case 28: /* Enchant Weapon */
			(void)enchant_spell(rand_int(4) + 1, rand_int(4) + 1, 0);
		       break;
	   case 29: /* Enchant Armour */
			(void)enchant_spell(0, 0, rand_int(3) + 2);
		       break;
	   case 30: /* Alchemy */
		       (void) alchemy();
		       break;
	   case 31: /* Globe of Invulnerability */
			(void)add_flag(TIMED_INVULN, randint(8) + 8);
		       break;
	       default:
		 msg_format("You cast an unknown Sorcery spell: %d.", spell);
		 msg_print(NULL);
	   }
      break;
	case 1: /* * Thaumaturgy * */
	   switch (spell)
	   {
		case 0: /* Magic Missile */
				if (!get_aim_dir(&dir)) return;
				fire_bolt_or_beam(beam-10, GF_MISSILE, dir,
						  damroll(3 + ((plev - 1) / 5), 4));
                break;
        case 1: /* Trap / Door destruction, was: Blink */
			(void)destroy_doors_touch();
			break;
        case 2: /* Flash of Light == Light Area */
			(void)lite_area(damroll(2, (plev / 2)), (plev / 10) + 1);
			break; 
        case 3: /* Touch of Confusion */
            if (!(p_ptr->confusing))
            {
                msg_print("Your hands start glowing.");
                p_ptr->confusing = TRUE;
            }
			break;
       case 4: /* Manaburst */
             if (!get_aim_dir(&dir)) return;
             fire_ball(GF_MISSILE, dir,
            (damroll(3, 5) + plev +
             (plev / 4)),
            ((plev < 30) ? 2 : 3));
          /* Shouldn't actually use GF_MANA, as it will destroy all
       * items on the floor */
             break;
        case 5: /* Fire Bolt */
			if (!get_aim_dir(&dir)) return;
			fire_bolt_or_beam(beam, GF_FIRE, dir,
				damroll(8+((plev-5)/4), 8));
			break;
        case 6: /* Fist of Force ("Fist of Fun") */
			if (!get_aim_dir(&dir)) return;
           fire_ball(GF_DISINTEGRATE, dir,
               damroll(8+((plev-5)/4), 8), 0);
            break;
		case 7: /* Teleport Self */
			teleport_player(plev * 5);
			break;
        case 8: /* Wonder */
           {
           /* This spell should become more useful (more
              controlled) as the player gains experience levels.
              Thus, add 1/5 of the player's level to the die roll.
              This eliminates the worst effects later on, while
              keeping the results quite random.  It also allows
              some potent effects only at high level. */

               int die = randint(100) + plev / 5;

               if (!get_aim_dir(&dir)) return;
               if (die > 100)
                   msg_print ("You feel a surge of power!");
               if (die < 8) clone_monster (dir);
               else if (die < 14) speed_monster (dir,plev);
               else if (die < 26) heal_monster (dir);
               else if (die < 31) poly_monster (dir,plev);
               else if (die < 36)
                   fire_bolt_or_beam (beam - 10,
                   GF_MISSILE, dir,
                   damroll(3 + ((plev - 1) / 5), 4));
               else if (die < 41) confuse_monster (dir, plev);
               else if (die < 46) fire_ball (GF_POIS, dir, 20 + (plev / 2), 3);
               else if (die < 51) lite_line (dir);
               else if (die < 56)
                   fire_bolt_or_beam (beam - 10, GF_ELEC, dir,
                   damroll(3+((plev-5)/4),8));
               else if (die < 61)
                   fire_bolt_or_beam (beam - 10, GF_COLD, dir,
                   damroll(5+((plev-5)/4),8));
               else if (die < 66)
                   fire_bolt_or_beam (beam, GF_ACID, dir,
                   damroll(6+((plev-5)/4),8));
               else if (die < 71)
                   fire_bolt_or_beam (beam, GF_FIRE, dir,
                   damroll(8+((plev-5)/4),8));
               else if (die < 76) drain_life (dir, 75);
               else if (die < 81) fire_ball (GF_ELEC, dir, 30 + plev / 2, 2);
               else if (die < 86) fire_ball (GF_ACID, dir, 40 + plev, 2);
               else if (die < 91) fire_ball (GF_ICE, dir, 70 + plev, 3);
               else if (die < 96) fire_ball (GF_FIRE, dir, 80 + plev, 3);
               else if (die < 101) drain_life (dir, 100 + plev);
               else if (die < 104) earthquake (py, px, 12);
               else if (die < 106) destroy_area (py, px, 15, TRUE);
               else if (die < 108) genocide(TRUE);
               else if (die < 110) dispel_monsters (120);
               else /* RARE */
               {
                   dispel_monsters (150);
                   slow_monsters(plev);
                   sleep_monsters(plev);
                   hp_player (300);
               }
               break;
           }
		case 9: /* Chaos Bolt */
			if (!get_aim_dir(&dir)) return;
			fire_bolt_or_beam(beam, GF_CHAOS, dir,
				damroll(10+((plev-5)/4), 8));
			break;
        case 10: /* Sonic Boom */
                   project(0, 2+plev/10, py, px,
               45+plev, GF_SOUND, PROJECT_KILL|PROJECT_ITEM);
                   break;
        case 11: /* Doom Bolt -- always beam in 2.0.7 or later */
				if (!get_aim_dir(&dir)) return;
                fire_beam(GF_MANA, dir, damroll(11+((plev-5)/4), 8));
			break;
		case 12: /* Fire Ball */
			if (!get_aim_dir(&dir)) return;
			fire_ball(GF_FIRE, dir,
					55 + (plev), 2);
			break;
		case 13: /* Teleport Other */
           if (!get_aim_dir(&dir)) return;
               (void)fire_beam(GF_AWAY_ALL, dir, plev);
			break;
		case 14: /* Word of Destruction */
			destroy_area(py, px, 15, TRUE);
			break;
		case 15: /* Invoke chaos */
			if (!get_aim_dir(&dir)) return;
			fire_ball(GF_CHAOS, dir,
					66 + (plev), (plev / 5));
			break;
        case 16: /* Polymorph Other */
			if (!get_aim_dir(&dir)) return;
			(void)poly_monster(dir,plev);
			break;
        case 17: /* Chain Lightning */
          for (dir = 0; dir <= 9; dir++)
            fire_beam(GF_ELEC, dir, damroll(5+(plev/10), 8));
           break;
        case 18: /* Arcane Binding == Charging */
			(void)recharge(40);
			break;
        case 19: /* Disintegration */
			if (!get_aim_dir(&dir)) return;
           fire_ball(GF_DISINTEGRATE, dir,
               80 + (plev), 3 + (plev/40));
               break;
        case 20: /* Alter Reality */
			msg_print("The world changes!");
			change_level(dun_level, START_RANDOM);
			break;
        case 21: /* Polymorph Self */
            do_poly_self();
	    break;
        case 22: /* Chaos Branding */
		brand_weapon(1);
		break;
        case 23: /* Summon monster, demon */
		if (randint(3) == 1)
		{
			if (summon_specific(py, px, (plev*3)/2, SUMMON_DEMON))
			{
				msg_print("The area fills with a stench of sulphur and brimstone.");
				msg_print("'NON SERVIAM! Wretch! I shall feast on thy mortal soul!'");
			}
        	}
		else
		{
			if (summon_specific_friendly((int)py,(int) px, (plev*3)/2,
				SUMMON_DEMON, plev == 50))
			{
				msg_print("The area fills with a stench of sulphur and brimstone.");
				msg_print("'What is thy bidding... Master?'");
			}
		}
		break;
        case 24: /* Beam of Gravity */
			if (!get_aim_dir(&dir)) return;
                fire_beam(GF_GRAVITY, dir, damroll(9+((plev-5)/4), 8));
            break;
        case 25: /* Meteor Swarm  */
           {
		       int x, y, dx, dy, d, count = 0;
		       int b = 10 + randint(10); 
		       for (i = 0; i < b; i++) {
			   do {
			       count++;
			       if (count > 1000)  break;
			       x = px - 5 + randint(10);
			       y = py - 5 + randint(10);
			       dx = (px > x) ? (px - x) : (x - px);
			       dy = (py > y) ? (py - y) : (y - py);
			       /* Approximate distance */
                   d = (dy > dx) ? (dy + (dx>>1)) : (dx + (dy>>1));
               } while ((d > 5) || (!(player_has_los_bold(y, x))));
			   
			   if (count > 1000)   break;
			   count = 0;
               project(0, 2, y, x, (plev*3)/2, GF_METEOR, PROJECT_KILL|PROJECT_JUMP|PROJECT_ITEM);
		       }
		   }
	           break;
		case 26: /* Flame Strike */
			fire_ball(GF_FIRE, 0,
                150 + (2*plev), 8);
			break;
        case 27: /* Call Chaos */
            call_chaos(plev);
			break;
        case 28: /* Shard Ball */
			if (!get_aim_dir(&dir)) return;
			fire_ball(GF_SHARD, dir,
					120 + (plev), 2);
			break;
        case 29: /* Mana Storm */
			if (!get_aim_dir(&dir)) return;
			fire_ball(GF_MANA, dir,
				300 + (plev * 2), 4);
            break;
        case 30: /* Breathe chaos */
               if (!get_aim_dir(&dir)) return;
               fire_ball(GF_CHAOS,dir,p_ptr->chp,
                     -2);
               break;
		case 31: /* Call the Void */
			call_the_();
			break;
		default:
		  msg_format("You cast an unknown Thaumaturgy spell: %d.", spell);
		  msg_print(NULL);
	    }
	   break;
    case 2: /* Conjuration */
    switch (spell)
    {
        case 0: /* Phase Door */
			teleport_player(10);
        break;
        case 1: /* Mind Blast */
               if (!get_aim_dir(&dir)) return;
                 fire_bolt_or_beam(beam-10, GF_PSI, dir,
                              damroll(3 + ((plev - 1) / 5), 3));
        break;
        case 2: /* Tarot Draw */

           {
                /* A limited power 'wonder' spell */

               int die = die = (randint(110)) + plev / 5;
               /* get a level bonus */

            msg_print("You shuffle your Tarot deck and draw a card...");

            if (die < 7 )
            {
                msg_print("Oh no! It's the Blasted Tower!");
                for (dummy = 0; dummy < randint(3); dummy++)
                    (void)activate_hi_summon();
            }
            else if (die < 14)
            {
                msg_print("Oh no! It's the Devil!");
                (void) summon_specific(py, px, (dun_depth), SUMMON_DEMON);
            }
            else if (die < 18 )
            {
                msg_print("Oh no! It's the Hanged Man.");
                activate_ty_curse();
            }
            else if (die < 22 )
            {
                msg_print("It's the swords of discord.");
                aggravate_monsters(1);
            }
            else if (die < 26)
            {
                msg_print("It's the Fool.");
                (void) do_dec_stat(A_INT);
                (void) do_dec_stat(A_WIS);
            }
            else if (die < 30)
            {
                msg_print("It's a picture of a strange monster.");
                if (!(summon_specific(py, px, ((dun_depth) * 3) / 2, 32 + randint(6))))
                    none_came = TRUE;
            }
            else if (die < 33)
            {
                msg_print("It's the Moon.");
                unlite_area(10,3);
            }
            else if (die < 38)
            {
                msg_print("It's the Wheel of Fortune.");
                wild_magic((randint(32))-1);
            }
            else if (die < 40)
            {
                msg_print("It's a teleport card.");
                teleport_player(10);
            }
            else if (die <42)
            {
                msg_print("It's the Star.");
                add_flag(TIMED_BLESSED, plev);
            }
            else if (die <47)
            {
                msg_print("It's a teleport card.");
                teleport_player(100);
            }
            else if (die <52)
            {
                msg_print("It's a teleport card.");
                teleport_player(200);
            }
            else if (die <60)
            {
                msg_print("It's the Tower.");
                wall_breaker(plev);
            }
            else if (die <72)
            {
                msg_print("It's Temperance.");
                sleep_monsters_touch(plev);
            }
            else if (die <80)
            {
                msg_print("It's the Tower.");
                earthquake(py, px, 5);
            }
            else if (die<82)
            {
                msg_print("It's a picture of a friendly monster.");
                if (!(summon_specific_friendly(py, px, ((dun_depth) * 3) / 2, SUMMON_MOULD, FALSE)))
                    none_came = TRUE;
            }
            else if (die<84)
            {
                msg_print("It's a picture of a friendly monster.");
                if (!(summon_specific_friendly(py, px, ((dun_depth) * 3) / 2, SUMMON_BAT, FALSE)))
                    none_came = TRUE;
            }
            else if (die<86)
            {
                msg_print("It's a picture of a friendly monster.");
                if (!(summon_specific_friendly(py, px, ((dun_depth) * 3) / 2, SUMMON_VORTEX, FALSE)))
                    none_came = TRUE;
            }
            else if (die<88)
            {
                msg_print("It's a picture of a friendly monster.");
                if (!(summon_specific_friendly(py, px, ((dun_depth) * 3) / 2, SUMMON_TREASURE, FALSE)))
                    none_came = TRUE;
            }
            else if (die<96)
            {
                msg_print("It's the Lovers.");
                if (!get_aim_dir(&dir)) return;
                (void) charm_monster(dir, MIN(plev, 20));
            }
            else if (die<101)
            {
                msg_print("It's the Hermit.");
                wall_stone();
            }
            else if (die< 111)
            {
                msg_print("It's the Judgement.");
                do_cmd_rerate();
                if (p_ptr->muta1 || p_ptr->muta2 || p_ptr->muta3)
                {
                    msg_print("You are cured of all chaos features.");
                    p_ptr->muta1 = p_ptr->muta2 = p_ptr->muta3 = 0;
                    p_ptr->update |= PU_BONUS;
                    handle_stuff();
                }
                
            }
            else if (die < 120)
            {
                msg_print("It's the Sun.");
                wiz_lite();
            }
            else
            {
                msg_print("It's the World.");
                    msg_print("You feel more experienced.");
					gain_skills(100);
                }

           }
        break;
        case 3: /* Reset Recall */
            {
                /* Prompt */
                sprintf(ppp, "Reset to which level (1-%d): ", p_ptr->max_dlv);

                /* Default */
                sprintf(tmp_val, "%d", MAX(dun_level,1));

                /* Ask for a level */
                if (!get_string(ppp, tmp_val, 10)) return;

                /* Extract request */
                dummy = atoi(tmp_val);

                /* Paranoia */
                if (dummy < 1) dummy = 1;

                /* Paranoia */
                if (dummy > p_ptr->max_dlv) dummy = p_ptr->max_dlv;

                /* Accept request */
                msg_format("Recall depth set to level %d (%d').", dummy, dummy * 50 );
            }
        break;
        case 4: /* Teleport Self */
            teleport_player(plev * 4);
        break;
        case 5: /* Dimension Door */
       {
			if (!dimension_door(plev, 10)) return;
             break;
            }
        case 6: /* Planar Spying */
            (void)add_flag(TIMED_ESP, randint(30) + 25);
        break;
        case 7: /* Teleport Away */
			if (!get_aim_dir(&dir)) return;
               (void)fire_beam(GF_AWAY_ALL, dir, plev);
        break;
        case 8: /* Summon Object */
             if (!get_aim_dir(&dir)) return;
                 fetch(dir, plev*15, TRUE);
        break;
        case 9: /* Summon Animal */
        {
            msg_print ("You reach out your mind to the wilderness...");
            if (randint(5)>2)
            {
              if (!(summon_specific_friendly(py, px, plev, SUMMON_ANIMAL_RANGER, FALSE)))
                none_came = TRUE;
            }
            else
            {
                if (summon_specific(py, px, plev, SUMMON_ANIMAL))
                {
                    msg_print("The summoned animal gets angry!");
                }
                else
                {
                    none_came = TRUE;
                }
            }
        }
        break;
        case 10: /* Phantasmal Servant */
               if (summon_specific_friendly(py, px, (plev*3)/2, SUMMON_PHANTOM, FALSE))
               {
                    msg_print ("'Your wish, master?'");
                }
                else
                {
                    none_came = TRUE;
                }
        break;
        case 11: /* Summon Monster */
        {
            msg_print ("You reach out your mind...");
            if (randint(5)>2)
            {
             if (!(summon_specific_friendly(py, px, plev, 0, FALSE)))
                none_came = TRUE;
            }
            else
            {
                if (summon_specific(py, px, plev, SUMMON_ALL))
                {
                    msg_print("The summoned creature gets angry!");
                }
                else
                {
                    none_came = TRUE;
                }
            }
        }
        break;
        case 12: /* Conjure Elemental */
        {
            if (randint(6)>3)
            {
             if (!(summon_specific_friendly(py, px, plev, SUMMON_ELEMENTAL, FALSE)))
                none_came = TRUE;
            }
            else
            {
                if (summon_specific(py, px, plev, SUMMON_ELEMENTAL))
                {
                      msg_print("You fail to control the elemental creature!");
                }
                else
                {
                    none_came = TRUE;
                }
            }
        }

        break;
        case 13: /* Teleport Level */
			(void)teleport_player_level();
        break;
        case 14: /* Word of Recall */
			{
				set_recall(TRUE);
				break;
			}
        case 15: /* Banish */
             banish_monsters(plev*4);
        break;
        case 16: /* Joker Card */
            msg_print("You concentrate on a joker card...");
            switch(randint(4))
            {
                case 1: dummy = SUMMON_MOULD; break;
                case 2: dummy = SUMMON_BAT; break;
                case 3: dummy = SUMMON_VORTEX; break;
                case 4: dummy = SUMMON_TREASURE; break;

            }
            if (randint(2)==1)
            {
                if (summon_specific(py, px, plev, dummy))
                    msg_print("The summoned creature gets angry!");
                 else
                    none_came = TRUE;
                }
            else
            {
                if (!(summon_specific_friendly(py, px, plev, dummy, FALSE)))
                    none_came = TRUE;
            }
        break;
        case 17: /* Summon Spiders */
        {
            msg_print ("You reach out your mind along the planar webs...");
            if (randint(5)>2)
            {
                if (!(summon_specific_friendly(py, px, plev, SUMMON_SPIDER, TRUE)))
                    none_came = TRUE;
            }
            else
            {
                if (summon_specific(py, px, plev, SUMMON_SPIDER))
                {
                    msg_print("The summoned spiders get angry!");
                }
                else
                {
                    none_came = TRUE;
                }
            }
        }
        break;
        case 18: /* Summon Reptiles */
        {
            msg_print ("You reach out your mind to the cold, damp places...");
            if (randint(5)>2)
            {
             if (!(summon_specific_friendly(py, px, plev, SUMMON_HYDRA, TRUE)))
                none_came = TRUE;
            }
            else
            {
                if (summon_specific(py, px, plev, SUMMON_HYDRA))
                {
                    msg_print("The summoned reptile gets angry!");
                }
                else
                {
                    none_came = TRUE;
                }
            }
        }
        break;
        case 19: /* Summon Hounds */
        {
            msg_print ("You reach out your mind to the elemental planes...");
            if (randint(5)>2)
            {
             if (!(summon_specific_friendly(py, px, plev, SUMMON_HOUND, TRUE)))
                none_came = TRUE;
            }
            else
            {
                if (summon_specific(py, px, plev, SUMMON_HOUND))
                {
                    msg_print("The summoned hounds get angry!");
                }
                else
                {
                    none_came = TRUE;
                }
            }
        }

        break;
        case 20: /* Planar Branding */
            brand_weapon(4);
        break;
        case 21: /* Planar Being */
        if (randint(8)==1) dummy = 103;
        else dummy = 30;
        if (gain_chaos_feature(dummy))
            msg_print("You have turned into a Planar Being.");
        break;
        case 22: /* Death Dealing */
            (void)dispel_living(plev * 3);
        break;
        case 23: /* Summon Reaver */
        {
            msg_print ("You reach out your mind to the planes of order...");
            if (randint(10)>3)
            {
              if (!(summon_specific_friendly(py, px, plev, SUMMON_REAVER, FALSE)))
                none_came = TRUE;
            }
            else
            {
                if (summon_specific(py, px, plev, SUMMON_REAVER))
                {
                    msg_print("The summoned Black Reaver gets angry!");
                }
                else
                {
                    none_came = TRUE;
                }
            }
        }
        break;
        case 24: /* Planar Divination */
			(void)detect_all();
        break;
        case 25: /* Planar Lore */
            identify_fully();
        break;
        case 26: /* Summon Undead */
        {
            msg_print ("You reach out your mind to beyond the grave...");
            if (randint(10)>3)
            {
             if (!(summon_specific_friendly(py, px, plev, SUMMON_UNDEAD, FALSE)))
                none_came = TRUE;
            }
            else
            {
                if (summon_specific(py, px, plev, SUMMON_UNDEAD))
                {
                    msg_print("The summoned undead creature gets angry!");
                }
                else
                {
                    none_came = TRUE;
                }
            }
        }
        break;
        case 27: /* Summon Dragon */
        {
            msg_print ("You reach out your mind to the treasure troves...");
            if (randint(10)>3)
            {
             if (!(summon_specific_friendly(py, px, plev, SUMMON_DRAGON, FALSE)))
                none_came = TRUE;
            }
            else
            {
                if (summon_specific(py, px, plev, SUMMON_DRAGON))
                {
                    msg_print("The summoned dragon gets angry!");
                }
                else
                {
                    none_came = TRUE;
                }
            }
        }

        break;
        case 28: /* Mass Summon */
        {
            none_came = TRUE;
            msg_print ("You concentrate on several images at once...");
            for (dummy = 0; dummy < 3 + (plev / 10); dummy++)
            {
                if (randint(10)>3)
                {
                 if (summon_specific_friendly(py, px, plev, 0, FALSE))
                    none_came = FALSE;
                }
                else
                {
                    if (summon_specific(py, px, plev, SUMMON_ALL))
                    {
                        msg_print("A summoned creature gets angry!");
                        none_came = FALSE;
                    }
                }
            }
        }
        break;
        case 29: /* Summon Demon */
        {
            msg_print ("You reach out your mind to the pits of hell...");
            if (randint(10)>3)
            {
             if (!(summon_specific_friendly(py, px, plev, SUMMON_DEMON, FALSE)))
                none_came = TRUE;
            }
            else
            {
                if (summon_specific(py, px, plev, SUMMON_DEMON))
                {
                    msg_print("The summoned demon gets angry!");
                }
                else
                {
                    none_came = TRUE;
                }
            }
        }
        break;
        case 30: /* Summon Ancient Dragon */
        {
            msg_print ("You reach out your mind to the ancient caves...");
            if (randint(10)>3)
            {
             if (!(summon_specific_friendly(py, px, plev, SUMMON_HI_DRAGON, FALSE)))
                none_came = TRUE;
            }
            else
            {
                if (summon_specific(py, px, plev, SUMMON_HI_DRAGON))
                {
                    msg_print("The summoned ancient dragon gets angry!");
                }
                else
                {
                    none_came = TRUE;
                }
            }
        }

        break;
        case 31: /* Summon Greater Undead */
        {
            msg_print ("You reach out your mind to the darkest tombs...");
            if (randint(10)>3)
            {
             if (!(summon_specific_friendly(py, px, plev, SUMMON_HI_UNDEAD, FALSE)))
                none_came = TRUE;
            }
            else
            {
                if (summon_specific(py, px, plev, SUMMON_HI_UNDEAD))
                {
                    msg_print("The summoned greater undead creature gets angry!");
                }
                else
                {
                    none_came = TRUE;
                }
            }
        }
        break;
        default:
        msg_format("You cast an unknown Conjuration spell: %d.", spell);
        msg_print(NULL);
    }
    if (none_came)
    {
        msg_print("Nobody answers to your call.");
    }
    break;
	case 3: /* Necromancy */
	  switch (spell)
	  {
       case 0: /* Detect Undead & Demons -> Unlife*/
       (void) detect_monsters_nonliving();
		       break;
       case 1: /* Malediction */
         if (!get_aim_dir(&dir)) return;
         /* A radius-0 ball may (1) be aimed at objects etc.,
          * and will affect them; (2) may be aimed at ANY
          * visible monster, unlike a 'bolt' which must travel
          * to the monster. */

         fire_ball(GF_HELL_FIRE, dir,
           damroll(3 + ((plev - 1) / 5), 3), 0);
         if (randint(5)==1) {   /* Special effect first */
         dummy = randint(1000);
         if (dummy == 666)
           fire_bolt(GF_DEATH_RAY, dir, plev);
         else if (dummy < 500)
           fire_bolt(GF_TURN_ALL, dir, plev);
         else if (dummy < 800)
           fire_bolt(GF_OLD_CONF, dir, plev);
         else
           fire_bolt(GF_STUN, dir, plev);
         }
         break;
       case 2: /* Detect Evil */
			(void)detect_monsters_evil();
		       break; 
	   case 3: /* Stinking Cloud */
			if (!get_aim_dir(&dir)) return;
			fire_ball(GF_POIS, dir,
				10 + (plev / 2), 2);
		       break;
	   case 4: /* Black Sleep */
			if (!get_aim_dir(&dir)) return;
			(void)sleep_monster(dir,plev);
		       break;
	   case 5: /* Resist Poison */
			(void)add_flag(TIMED_OPPOSE_POIS, randint(20) + 20);
		       break;
       case 6: /* Horrify */
			if (!get_aim_dir(&dir)) return;
			(void)fear_monster(dir, plev);
            (void) stun_monster(dir, plev);
		       break;
       case 7: /* Enslave the Undead */
         if (!get_aim_dir(&dir)) return;
           (void)control_one_undead(dir, plev);
               break;
       case 8: /* Orb of Entropy */
         if (!get_aim_dir(&dir)) return;
         fire_ball(GF_OLD_DRAIN, dir,
           (damroll(3, 6) + plev + (plev / 4)),
           ((plev < 30) ? 2 : 3));
               break;
       case 9: /* Nether Bolt */
			if (!get_aim_dir(&dir)) return;
            fire_bolt_or_beam(beam, GF_NETHER, dir,
				damroll(6+((plev-5)/4), 8));
		       break;
       case 10: /* Terror */
             turn_monsters(30+plev);
             break;
	   case 11: /* Vampiric Drain */
       if (!get_aim_dir(&dir)) return;
       dummy = plev + randint(plev) * MAX(1, plev/10);   /* Dmg */
                 if (drain_life(dir, dummy)) {
           (void)hp_player(dummy);
           /* Gain nutritional sustenance: 150/hp drained */
           /* A Food ration gives 5000 food points (by contrast) */
           /* Don't ever get more than "Full" this way */
           /* But if we ARE Gorged,  it won't cure us */
           dummy = p_ptr->food + MIN(5000, 100 * dummy);
           if (p_ptr->food < PY_FOOD_MAX)   /* Not gorged already */
             (void)set_flag(TIMED_FOOD, dummy >= PY_FOOD_MAX ? PY_FOOD_MAX-1 : dummy);
       }
         break;
       case 12: /* Poison Branding */
            brand_weapon(2);
		       break;
       case 13: /* Dispel Good */
            (void)dispel_good(plev * 4);
		       break;
	   case 14: /* Genocide */
			(void)genocide(TRUE);
		       break;
	   case 15: /* Restore Life */
			(void)restore_level();
		       break;
	   case 16: /* Berserk */
            (void)add_flag(TIMED_SHERO, randint(25) + 25);
			(void)hp_player(30);
			(void)set_flag(TIMED_AFRAID, 0);
		       break;
       case 17: /* Invoke Spirits */
           {
               int die = randint(100) + plev / 5;
               if (!get_aim_dir(&dir)) return;

              msg_print("You call on the power of the dead...");
               if (die > 100)
                 msg_print ("You feel a surge of eldritch force!");

               if (die < 8) {
               msg_print("Oh no! Mouldering forms rise from the earth around you!");
               (void) summon_specific(py, px, (dun_depth), SUMMON_UNDEAD);
               } else if (die < 14) {
               msg_print("An unnamable evil brushes against your mind...");
               add_flag(TIMED_AFRAID, randint(4) + 4);
               } else if (die < 26) {
               msg_print("Your head is invaded by a horde of gibbering spectral voices...");
               add_flag(TIMED_CONFUSED, randint(4) + 4);
               } else if (die < 31) {
               poly_monster (dir,plev);
               } else if (die < 36) {
               fire_bolt_or_beam (beam - 10,
                          GF_MISSILE, dir,
                          damroll(3 + ((plev - 1) / 5), 4));
               } else if (die < 41) {
               confuse_monster (dir, plev);
               } else if (die < 46) {
               fire_ball (GF_POIS, dir, 20 + (plev / 2), 3);
               } else if (die < 51) {
               lite_line (dir);
               } else if (die < 56) {
               fire_bolt_or_beam (beam - 10, GF_ELEC, dir,
                          damroll(3+((plev-5)/4),8));
               } else if (die < 61) {
               fire_bolt_or_beam (beam - 10, GF_COLD, dir,
                          damroll(5+((plev-5)/4),8));
               } else if (die < 66) {
               fire_bolt_or_beam (beam, GF_ACID, dir,
                          damroll(6+((plev-5)/4),8));
               } else if (die < 71) {
               fire_bolt_or_beam (beam, GF_FIRE, dir,
                          damroll(8+((plev-5)/4),8));
               } else if (die < 76) {
               drain_life (dir, 75);
               } else if (die < 81) {
               fire_ball (GF_ELEC, dir, 30 + plev / 2, 2);
               } else if (die < 86) {
               fire_ball (GF_ACID, dir, 40 + plev, 2);
               } else if (die < 91) {
               fire_ball (GF_ICE, dir, 70 + plev, 3);
               } else if (die < 96) {
               fire_ball (GF_FIRE, dir, 80 + plev, 3);
               } else if (die < 101) {
               drain_life (dir, 100 + plev);
               } else if (die < 104) {
               earthquake (py, px, 12);
               } else if (die < 106) {
               destroy_area (py, px, 15, TRUE);
               } else if (die < 108) {
               genocide(TRUE);
               } else if (die < 110) {
               dispel_monsters (120);
               } else { /* RARE */
               dispel_monsters (150);
               slow_monsters(plev);
               sleep_monsters(plev);
               hp_player (300);
               }

               if (die < 31)
                 msg_print("Sepulchral voices chuckle. 'Soon you will join us, mortal.'");
               break;
           }
	   case 18: /* Dark Bolt */
			if (!get_aim_dir(&dir)) return;
			fire_bolt_or_beam(beam, GF_DARK, dir,
				damroll(4+((plev-5)/4), 8));
		       break;
       case 19: /* Battle Frenzy */
			(void)add_flag(TIMED_SHERO, randint(25) + 25);
            (void)hp_player(30);
			(void)set_flag(TIMED_AFRAID, 0);
			if (!p_ptr->fast)
			{
				(void)set_flag(TIMED_FAST, randint(20 + (plev / 2) ) + (plev / 2));
			}
			else
			{
				(void)add_flag(TIMED_FAST, randint(5));
			}
		       break;
        case 20: /* Vampirism True */
			if (!get_aim_dir(&dir)) return;
           for (dummy = 0; dummy < 3; dummy++)
           {
               if (drain_life(dir, 100))
                   hp_player(100);
                }
                   break;
        case 21: /* Vampiric Branding */
            brand_weapon(3);
		       break;
       case 22: /* Darkness Storm */
			if (!get_aim_dir(&dir)) return;
			fire_ball(GF_DARK, dir,
					120, 4);
		       break;
        case 23: /* Mass Genocide */
			(void)mass_genocide(TRUE);
		       break;
       case 24: /* Death Ray */
			if (!get_aim_dir(&dir)) return;
			(void)death_ray(dir, plev);
		       break;
       case 25: /* Raise the Dead */
                   if (randint(3) == 1) {
               if (summon_specific(py, px, (plev*3)/2,
                       (plev > 47 ? SUMMON_HI_UNDEAD : SUMMON_UNDEAD))) {
               msg_print("Cold winds begin to blow around you, carrying with them the stench of decay...");
               msg_print("'The dead arise... to punish you for disturbing them!'");
               }
           } else {
               if (summon_specific_friendly((int)py,(int)px, (plev*3)/2,
                       (plev > 47 ? SUMMON_HI_UNDEAD : SUMMON_UNDEAD),
                       ((plev > 24) && (randint(3) == 1)))) {
               msg_print("Cold winds begin to blow around you, carrying with them the stench of decay...");
               msg_print("Ancient, long-dead forms arise from the ground to serve you!");
               }
           }
           break;
       case 26: /* Esoteria */
		if (randint(50)>plev)
		    (void) ident_spell();
		else
		    identify_fully();
		       break;
       case 27: /* Word of Death */
	    (void)dispel_living(plev * 3);
		       break;
       case 28: /* Evocation       */
        (void)dispel_monsters(plev * 4);
         turn_monsters(plev*4);
         banish_monsters(plev*4);
		       break;
       case 29: /* Hellfire */
			if (!get_aim_dir(&dir)) return;
            fire_ball(GF_HELL_FIRE, dir,
                    666, 3);
            take_hit(50+randint(50), "the strain of casting Hellfire", MON_CASTING_HELLFIRE);
            break;
        case 30: /* Omnicide */
         p_ptr->csp -= 100;  /* Display doesn't show mana cost (100)
       * as deleted until the spell has finished. This gives a
       * false impression of how high your mana is climbing.
       * Therefore, 'deduct' the cost temporarily before entering the
       * loop, then add it back at the end so that the rest of the
       * program can deduct it properly */
         for (i = 1; i < m_max; i++)
         {
             monster_type    *m_ptr = &m_list[i];
             monster_race    *r_ptr = &r_info[m_ptr->r_idx];

             /* Paranoia -- Skip dead monsters */
             if (!m_ptr->r_idx) continue;

             /* Hack -- Skip Unique Monsters */
             if (r_ptr->flags1 & (RF1_UNIQUE)) continue;

			 /* Skip Quest Monsters */
			 if (r_ptr->flags1 & RF1_GUARDIAN) continue;

             /* Delete the monster */
             delete_monster_idx(i,TRUE);

             /* Take damage */
             take_hit(randint(4), "the strain of casting Omnicide", MON_CASTING_MASS_GENOCIDE);

             /* Absorb power of dead soul */
             p_ptr->csp++;

             /* Visual feedback */
             move_cursor_relative(py, px);

             /* Redraw */
             p_ptr->redraw |= (PR_HP | PR_MANA);

             /* Window stuff */
             p_ptr->window |= (PW_PLAYER);
			 p_ptr->window |=(PW_SPELL);

             /* Handle */
             handle_stuff();

             /* Fresh */
             Term_fresh();

             /* Delay */
             Term_xtra(TERM_XTRA_DELAY,
               delay_factor * delay_factor * delay_factor);
         }
         p_ptr->csp += 100;   /* Restore, ready to be deducted properly */

         break;
        case 31: /* Wraithform */
        add_flag(TIMED_WRAITH, randint(plev/2) + (plev/2));
        break;
	       default:
		 msg_format("You cast an unknown Necromancy spell: %d.", spell);
		 msg_print(NULL);
	   }
	  break;
	default:
		  msg_format("You cast a spell from an unknown school: school %d, spell %d.", spell_school, spell);
		  msg_print(NULL);
	    }
			/* A spell was cast */
		if (~s_ptr->flags & MAGIC_LEARNED)
		{
			/* The spell worked */
			s_ptr->flags |= MAGIC_LEARNED;
		}
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

	/* Hack - copy the flags for this book to the arrays. */
	set_spell_flags(o_ptr->k_idx);

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
	int	spell, dir;
	int	chance, beam;
	const int plev = MAX(1, skill_set[SKILL_HEDGE].value/2);
	int	dummy = 0;

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

    beam = plev;


	/* Spells.  */
	  switch (spell)
	  {
        case 0: /* Zap */
               if (!get_aim_dir(&dir)) return;
                 fire_bolt_or_beam(beam-10, GF_ELEC, dir,
                              damroll(3 + ((plev - 1) / 5), 3));
        break;
        case 1: /* Wizard Lock */
            if (!(get_aim_dir(&dir))) break;
            (void) wizard_lock(dir);
        break;
        case 2: /* Detect Invisibility */
            (void)detect_monsters_invis();
        break;
        case 3: /* Detect Monsters */
			(void)detect_monsters_normal();
        break;
        case 4: /* Blink */
            teleport_player(10);
        break;
        case 5: /* Light Area */
			(void)lite_area(damroll(2, (plev / 2)), (plev / 10) + 1);
        break;
        case 6: /* Trap & Door Destruction */
            if (!(get_aim_dir(&dir))) return;
            (void) destroy_door(dir);
        break;
        case 7: /* Cure Light Wounds */
            (void) hp_player(damroll(2, 8));
            (void) set_flag(TIMED_CUT, p_ptr->cut - 10);
        break;
        case 8: /* Detect Doors & Traps */
			(void)detect_traps();
			(void)detect_doors();
			(void)detect_stairs();
        break;
        case 9: /* Phlogiston */
            phlogiston();
        break;
        case 10: /* Detect Treasure */
			(void)detect_treasure();
			(void)detect_objects_gold();

        break;
        case 11: /* Detect Enchantment */
			(void)detect_objects_magic();
        break;
        case 12: /* Detect Object */
			(void)detect_objects_normal();
        break;
        case 13: /* Cure Poison */
			(void)set_flag(TIMED_POISONED, 0);
        break;
        case 14: /* Resist Cold */
			(void)add_flag(TIMED_OPPOSE_COLD, randint(20) + 20);
        break;
        case 15: /* Resist Fire */
			(void)add_flag(TIMED_OPPOSE_FIRE, randint(20) + 20);
        break;
        case 16: /* Resist Lightning */
			(void)add_flag(TIMED_OPPOSE_ELEC, randint(20) + 20);
        break;
        case 17: /* Resist Acid */
            (void)add_flag(TIMED_OPPOSE_ACID, randint(20) + 20);
        break;
        case 18: /* Cure Medium Wounds */
            (void)hp_player(damroll(4, 8));
            (void)set_flag(TIMED_CUT, (p_ptr->cut / 2) - 50);
        break;
        case 19: /* Teleport */
            teleport_player(plev * 5);
        break;
        case 20: /* Stone to Mud */
			if (!get_aim_dir(&dir)) return;
			(void)wall_to_mud(dir);
        break;
        case 21: /* Ray of Light */
			if (!get_aim_dir(&dir)) return;
            msg_print("A line of light appears.");
			lite_line(dir);
        break;
        case 22: /* Satisfy Hunger */
			(void)set_flag(TIMED_FOOD, PY_FOOD_MAX - 1);
        break;
        case 23: /* See Invisible */
			(void)add_flag(TIMED_INVIS, randint(24) + 24);
        break;
        case 24: /* Recharging */
               (void)recharge(plev * 2);
               break;
        case 25: /* Teleport Level */
			(void)teleport_player_level();
        break;
        case 26: /* Identify */
			(void)ident_spell();
        break;
        case 27: /* Teleport Away */
			if (!get_aim_dir(&dir)) return;
               (void)fire_beam(GF_AWAY_ALL, dir, plev);
        break;
        case 28: /* Elemental Ball */
			if (!get_aim_dir(&dir)) return;
            switch (randint(4))
            {
                case 1: dummy = GF_FIRE; break;
                case 2: dummy = GF_ELEC; break;
                case 3: dummy = GF_COLD; break;
                default: dummy = GF_ACID;
            }
            fire_ball(dummy, dir,
                    75 + (plev), 2);
        break;
        case 29: /* Detection */
			(void)detect_all();
        break;
        case 30: /* Word of Recall */
			{
			set_recall(TRUE);
				break;
            }
        case 31: /* Clairvoyance */
			wiz_lite();
            if (!(p_ptr->telepathy))
            {
                (void)add_flag(TIMED_ESP, randint(30) + 25);
            }
        break;
        default:
        msg_format("You cast an unknown cantrip: %d.", spell);
        msg_print(NULL);
    }
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
	const int plev = MAX(1, skill_set[SKILL_SHAMAN].value/2);
	int	spell, dir;
	int	chance, beam;
	int	favour_sphere = 0;
	int spirit;
	bool	none_came = FALSE;

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


	/* Access the spell */
	favour_sphere = s_ptr->sphere;

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
		/* The spirit gets pissed off */
		annoy_spirit(s_ptr,favour_annoyance(f_ptr));

    beam = plev;
	/* Spells.  */
	/* Spells.  */
	switch (favour_sphere)
	{
	case 0: /* * LIFE * */
	  switch (spell)
	  {
	   case 0: /* Detect Evil */
			(void)detect_monsters_evil();
		       break;
	   case 1: /* Cure Light Wounds */
			(void)hp_player(damroll(2, 10));
			(void)set_flag(TIMED_CUT, p_ptr->cut - 10);
		       break;
	   case 2: /* Bless */
			(void)add_flag(TIMED_BLESSED, randint(12) + 12);
		       break; 
	   case 3: /* Remove Fear */
			(void)set_flag(TIMED_AFRAID, 0);
		       break;
	   case 4: /* Call Light */
			(void)lite_area(damroll(2, (plev / 2)), (plev / 10) + 1);
		       break;
	   case 5: /* Detect Traps + Secret Doors */
			(void)detect_traps();
			(void)detect_doors();
			(void)detect_stairs();
		       break;
	   case 6: /* Cure Medium Wounds */
			(void)hp_player(damroll(4, 10));
			(void)set_flag(TIMED_CUT, (p_ptr->cut / 2) - 20);
		       break;
	   case 7: /* Satisfy Hunger */
			(void)set_flag(TIMED_FOOD, PY_FOOD_MAX - 1);
		       break;
	   case 8: /* Remove Curse */
			remove_curse();
		       break;
	   case 9: /* Cure Poison */
			(void)set_flag(TIMED_POISONED, 0);
		       break;
	   case 10: /* Cure Critical Wounds */
			(void)hp_player(damroll(8, 10));
			(void)set_flag(TIMED_STUN, 0);
			(void)set_flag(TIMED_CUT, 0);
		       break;
	   case 11: /* Sense Unseen */
			(void)add_flag(TIMED_INVIS, randint(24) + 24);
		       break;
	   case 12: /* Orb or Draining */
	   if (!get_aim_dir(&dir)) return;
            fire_ball(GF_HOLY_FIRE, dir,
				(damroll(3, 6) + plev + (plev /  4)),((plev < 30) ? 2 : 3));
		       break;
	   case 13: /* Protection from Evil */
			(void)add_flag(TIMED_PROTEVIL, randint(25) + 3 * plev);
		       break;
	   case 14: /* Healing */
			(void)hp_player(300);
			(void)set_flag(TIMED_STUN, 0);
			(void)set_flag(TIMED_CUT, 0);
		       break;
	   case 15: /* Glyph of Warding */
			warding_glyph();
		       break;
       case 16: /* Exorcism */
         (void) dispel_undead(plev);
         (void) dispel_demons(plev);
         (void) turn_evil(plev);
               break;
	   case 17: /* Dispel Curse */
			(void)remove_all_curse();
		       break;
       case 18: /* Dispel Undead + Demons */
            (void)dispel_undead(plev * 3);
        (void)dispel_demons(plev * 3);
			break;
       case 19: /* 'Day of the Dove' */
                  charm_monsters(plev * 2);
		       break;
       case 20: /* Dispel Evil */
			(void)dispel_evil(plev * 4);
		       break;
	   case 21: /* Banishment */
			if (banish_evil(100))
			{
				msg_print("The power of your god banishes evil!");
			}
			break;
	   case 22: /* Holy Word */
	   (void)dispel_evil(plev * 4);
			(void)hp_player(1000);
			(void)set_flag(TIMED_AFRAID, 0);
			(void)set_flag(TIMED_POISONED, 0);
			(void)set_flag(TIMED_STUN, 0);
			(void)set_flag(TIMED_CUT, 0);
		       break;
	   case 23: /* Warding True */
		warding_glyph();
		glyph_creation();
		       break;
	   case 24: /* Heroism */
			(void)add_flag(TIMED_HERO, randint(25) + 25);
			(void)hp_player(10);
			(void)set_flag(TIMED_AFRAID, 0);
		       break;
	   case 25: /* Prayer */
			(void)add_flag(TIMED_BLESSED, randint(48) + 48);
		       break;
       case 26:
            bless_weapon();
            break;
	   case 27: /* Restoration */
			(void)do_res_stat(A_STR);
			(void)do_res_stat(A_INT);
			(void)do_res_stat(A_WIS);
			(void)do_res_stat(A_DEX);
			(void)do_res_stat(A_CON);
			(void)do_res_stat(A_CHR);
			(void)restore_level();
		       break;
       case 28: /* Healing True */
			(void)hp_player(2000);
			(void)set_flag(TIMED_STUN, 0);
			(void)set_flag(TIMED_CUT, 0);
		       break;
       case 29: /* Holy Vision */
		identify_fully();
		       break;
       case 30: /* Divine Intervention */
         project(0, 1, py, px, 777, GF_HOLY_FIRE,   PROJECT_KILL);
         dispel_monsters(plev * 4);
         slow_monsters(plev * 4);
         stun_monsters(plev*4);
         confuse_monsters(plev*4);
         turn_monsters(plev*4);
         stasis_monsters(plev*4);
         (void)add_flag(TIMED_SHERO, randint(25) + 25);
         (void)hp_player(300);
         if (!p_ptr->fast) {   /* Haste */
         (void)set_flag(TIMED_FAST, randint(20 + (plev) ) + plev);
         } else {
         (void)add_flag(TIMED_FAST, randint(5));
         }
         (void)set_flag(TIMED_AFRAID, 0);
         break;
	   case 31: /* Holy Invulnerability */
			(void)add_flag(TIMED_INVULN, randint(7) + 7);
		       break;
	       default:
		 msg_format("You cast an unknown Life spell: %d.", spell);
		 msg_print(NULL);
	   }
	  break;
	case 1: /* * NATURE * */
	  switch (spell)
	  {
	   case 0: /* Detect Creatures */
			(void)detect_monsters_normal();
		       break;
	   case 1: /* First Aid */
			(void)hp_player(damroll(2, 8));
			(void)set_flag(TIMED_CUT, p_ptr->cut - 15);
		       break;
	   case 2: /* Detect Doors + Traps */
			(void)detect_traps();
			(void)detect_doors();
			(void)detect_stairs();
		       break; 
	   case 3: /* Produce Food */
			(void)set_flag(TIMED_FOOD, PY_FOOD_MAX - 1);
		       break;
       case 4: /* Daylight */
               (void)lite_area(damroll(2, (plev / 2)), (plev / 10) + 1);
            if ((p_ptr->prace == RACE_VAMPIRE) && !(p_ptr->resist_lite))
            {
                msg_print("The daylight scorches your flesh!");
                take_hit(damroll(2,2), "daylight", MON_LIGHT);
                            }
               break;
       case 5: /* Animal Taming */
         if (!get_aim_dir(&dir)) return;
         (void) charm_animal(dir, plev);
         break;
       case 6: /* Resist Environment */
			(void)add_flag(TIMED_OPPOSE_COLD, randint(20) + 20);
			(void)add_flag(TIMED_OPPOSE_FIRE, randint(20) + 20);
			(void)add_flag(TIMED_OPPOSE_ELEC, randint(20) + 20);
		       break;
       case 7: /* Cure Wounds + Poison */
            (void)set_flag(TIMED_CUT, 0);
			(void)set_flag(TIMED_POISONED, 0);
		       break;
	   case 8: /* Stone to Mud */
			if (!get_aim_dir(&dir)) return;
			(void)wall_to_mud(dir);
		       break;
	   case 9: /* Lightning Bolt */
				if (!get_aim_dir(&dir)) return;
				fire_bolt_or_beam(beam-10, GF_ELEC, dir,
						  damroll(3+((plev-5)/4), 8));
		       break;
       case 10: /* Nature Awareness -- downgraded */
			map_area();
			(void)detect_traps();
			(void)detect_doors();
			(void)detect_stairs();
			(void)detect_monsters_normal();
            break;
	   case 11: /* Frost Bolt */
			if (!get_aim_dir(&dir)) return;
			fire_bolt_or_beam(beam-10, GF_COLD, dir,
				damroll(5+((plev-5)/4), 8));
		       break;
	   case 12: /* Ray of Sunlight */
			if (!get_aim_dir(&dir)) return;
			msg_print("A line of sunlight appears.");
			lite_line(dir);
		       break;
	   case 13: /* Entangle */
			slow_monsters(plev * 4);
		       break;
       case 14: /* Summon Animals */
             if (!(summon_specific_friendly(py, px, plev, SUMMON_ANIMAL_RANGER, TRUE)))
                none_came = TRUE;
             break;
      case 15: /* Herbal Healing */
			(void)hp_player(1000);
			(void)set_flag(TIMED_STUN, 0);
			(void)set_flag(TIMED_CUT, 0);
			(void)set_flag(TIMED_POISONED, 0);
		       break;
       case 16: /* Door Building */
			(void)door_creation();
		       break;
       case 17: /* Stair Building */
			(void)stair_creation();
		       break;
       case 18: /* Stone Skin */
			(void)add_flag(TIMED_SHIELD, randint(20) + 30);
		       break;
       case 19: /* Resistance True */
			(void)add_flag(TIMED_OPPOSE_ACID, randint(20) + 20);
			(void)add_flag(TIMED_OPPOSE_ELEC, randint(20) + 20);
			(void)add_flag(TIMED_OPPOSE_FIRE, randint(20) + 20);
			(void)add_flag(TIMED_OPPOSE_COLD, randint(20) + 20);
			(void)add_flag(TIMED_OPPOSE_POIS, randint(20) + 20);
		       break;
        case 20: /* Animal Friendship */
        (void) charm_animals(plev * 2);
         break;
	   case 21: /* Stone Tell */
		identify_fully();
		       break;
       case 22: /* Wall of Stone */
		(void)wall_stone();
		       break;
       case 23: /* Protection from Corrosion */
               rustproof();
		       break;
       case 24: /* Earthquake */
			earthquake(py, px, 10);
		       break;
       case 25: /* Whirlwind Attack */
         {
         int y = 0, x = 0;
         cave_type       *c_ptr;
         monster_type    *m_ptr;

         for (dir = 0; dir <= 9; dir++) {
             y = py + ddy[dir];
             x = px + ddx[dir];
             c_ptr = &cave[y][x];

             /* Get the monster */
             m_ptr = &m_list[c_ptr->m_idx];

             /* Hack -- attack monsters */
             if (c_ptr->m_idx && (m_ptr->ml || cave_floor_bold(y, x)))
           py_attack(y, x);
         }
         }
         break;
       case 26: /* Blizzard */
			if (!get_aim_dir(&dir)) return;
			fire_ball(GF_COLD, dir,
				70 + (plev), (plev/12)+1);
		       break;
	   case 27: /* Lightning Storm */
			if (!get_aim_dir(&dir)) return;
			fire_ball(GF_ELEC, dir,
				90 + (plev), (plev/12)+1);
		       break;
	   case 28: /* Whirlpool */
			if (!get_aim_dir(&dir)) return;
			fire_ball(GF_WATER, dir,
				100 + (plev), (plev/12)+1);
		       break;
	   case 29: /* Call Sunlight */

			fire_ball(GF_LITE, 0, 150, 8);
			wiz_lite();
            if ((p_ptr->prace == RACE_VAMPIRE) && !(p_ptr->resist_lite))
            {
                msg_print("The sunlight scorches your flesh!");
                take_hit(50, "sunlight", MON_LIGHT);
            }
		       break;
	   case 30: /* Elemental Brand */
            brand_weapon(0);
		       break;
	   case 31: /* Nature's Wrath */
            (void)dispel_monsters(plev * 4);
            earthquake(py, px, 20 + (plev / 2) );
         project(0, 1+plev/12, py, px,
             100+plev, GF_DISINTEGRATE, PROJECT_KILL|PROJECT_ITEM);
		       break;
	       default:
		 msg_format("You cast an unknown Nature spell: %d.", spell);
		 msg_print(NULL);
	   }
      if (none_came)
        msg_print("No animals arrive.");
	  break;
	 default:
		  msg_format("You invoke a favour from an unknown sphere: sphere %d, spell %d.", favour_sphere, spell);
		  msg_print(NULL);
	    }
		/* Gain experience with spirit lore skill */
		if (skill_set[SKILL_SHAMAN].value < f_ptr->min * 2 + 50) {
			skill_exp(SKILL_SHAMAN);
		}
	}

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);
	p_ptr->window |=(PW_SPELL);
}



/*
 * Display mindcrafting powers
 * Returns the y co-ordinate of the lowest line used.
 */
static int print_mindcraft(book_type *b_ptr, int x, int y, bool colour)
{
	int i, chance, psi = spell_skill(&b_ptr->info[0]);
	magic_type *s_ptr;
	char		comment[80];
	/* Display a list of spells */
	prt("", y, x);
	put_str("Name", y, x + 5);
	put_str("Sk  Chi Time Fail Info", y, x + 35);

	/* Dump the spells */
	for (i = 0, y++; i < MAX_SPELLS_PER_BOOK; i++)
	{
		byte a = TERM_WHITE;
		char psi_desc[80];

		if (~b_ptr->flags & (1L << i)) continue;

		/* Access the spell */
		s_ptr = &b_ptr->info[i];

		if (s_ptr->min > psi)   break;

		chance = spell_chance(s_ptr);

		/* Warn if there's not enough mana to cast */
		if (colour && s_ptr->mana > p_ptr->cchi) a = TERM_ORANGE;

		/* Get info */
		get_magic_info(comment, 80, s_ptr);
				    
		/* Dump the spell --(-- */
		sprintf(psi_desc, "  %c) %-30s%2d %4d %4d %3d%%%s",
		I2A(i), s_ptr->name,
		s_ptr->min*2, s_ptr->mana, magic_energy(s_ptr), chance, comment);
		prt(psi_desc, y++, x);
	}

	/* Clear the bottom line */
	prt("", y, x);

	return y;
}

/*
 * Return a line of help text appropriate for the given mindcraft power
 * at the given level. This is used immediately, so the format buffer is fine.
 */
static cptr mindcraft_help(const int power, const int skill)
{
	switch (power)
	{
		case MIND_PRECOG:
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
		case MIND_BLAST:
		{
			return "Fires a bolt of mental energy at a monster.";
		}
		case MIND_DISP_1:
		{
			if (skill < 50) return "Teleports you a short distance away.";
			else return "Teleports you to a nearby spot of your choosing.";
		}
		case MIND_DISP_2:
		{
			if (skill < 60) return "Teleports you far away.";
			else return "Teleports you, and other nearby monsters, far away.";
		}
		case MIND_DOMINATION:
		{
			if (skill < 60) return "Charms a monster.";
			else return "Charms all nearby monsters.";
		}
		case MIND_PULVERISE:
		{
			return "Creates a ball of sound at a location of your choosing.";
		}
		case MIND_ARMOUR:
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
		case MIND_PSYCH:
		{
			if (skill >= 80) return "Identifies an object.";
			else return "Pseudo-identifies an object.";
		}
		case MIND_M_WAVE:
		{
			if (skill >= 50)
				return "Fires mental energy at all visible monsters.";
			else
				return "Fires mental energy at nearby monsters.";
		}
		case MIND_ADRENALINE:
		{
			if (skill >= 70)
				return "Heals you, hastes you and drives you berserk.";
			else
				return "Heals you, hastes you and makes you heroic.";
		}
		case MIND_PSY_DRAIN:
		{
			return "Fires mental energy at nearby monsters to gain extra chi.";
		}
		case MIND_TK_WAVE:
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
				put_str(mindcraft_help(i, psi*2), maxy, x);

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
    int dir;
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
	    /* spell code */
	    switch (n)
		{
		case MIND_PRECOG:
			if (psi > 44)
			{
				wiz_lite();
			}
			else if (psi > 19)
			{
				map_area();
			}
			if (psi < 30)
			{
				b = detect_monsters_normal();
				if (psi > 19)  b |=  detect_monsters_invis();
				if (psi > 14)
				{
					b |= detect_doors();
					b |= detect_stairs();
				}
				if (psi > 4)
				{
					b |=  detect_traps();
				}
			}
			else
			{
				b = detect_all();
			}
			if (!b)  msg_print("You feel safe.");
			break;
		case MIND_BLAST:
			if (!get_aim_dir(&dir)) return;
			if (randint(100) < psi * 2)
			{
				fire_beam(GF_PSI, dir, damroll(3 + ((psi - 1) / 4), (3+psi/15)));
			}
			else
			{
				fire_ball(GF_PSI, dir, damroll(3 + ((psi - 1) / 4), (3+psi/15)), 0);
			}
			break;
		case MIND_DISP_1:
			if (psi < 25)
			{
				teleport_player(10);
			}
			else
			{
				if (!dimension_door(psi, 20)) return;
			}
			break;
		case MIND_DISP_2:
			if (psi > 29) banish_monsters(psi);
			teleport_player(psi * 5);
			break;
		case MIND_DOMINATION:
			if (psi < 30)
			{
				if (!get_aim_dir(&dir)) return;
				fire_ball(GF_DOMINATION, dir, psi, 0);
			}
			else
			{
				charm_monsters(psi * 2);
			}
			break;
		case MIND_PULVERISE:
			if (!get_aim_dir(&dir)) return;
			fire_ball(GF_SOUND, dir, damroll(8+((psi-5)/4), 8),(psi > 20 ? (psi-20)/8 + 1 : 0));
			break;
		case MIND_ARMOUR:
			add_flag(TIMED_SHIELD, psi);
			if (psi > 14)   add_flag(TIMED_OPPOSE_ACID, psi);
			if (psi > 19)   add_flag(TIMED_OPPOSE_FIRE, psi);
			if (psi > 24)   add_flag(TIMED_OPPOSE_COLD, psi);
			if (psi > 29)   add_flag(TIMED_OPPOSE_ELEC, psi);
			if (psi > 34)   add_flag(TIMED_OPPOSE_POIS, psi);
			break;
		case MIND_PSYCH:
			if (psi < 40)
			{
				psychometry();
			}
			else
			{
				ident_spell();
			}
			break;
		case MIND_M_WAVE:
			msg_print("Mind-warping forces emanate from your brain!");
			if (psi < 25)
			{
				project(0, 2+psi/10, py, px,(psi*3)/2, GF_PSI, PROJECT_KILL);
			}
			else
			{
				(void)mindblast_monsters(psi * ((psi-5) / 10 + 1));
			}
			break;
		case MIND_ADRENALINE:
			set_flag(TIMED_AFRAID, 0);
			set_flag(TIMED_STUN, 0);
			hp_player(psi);
			b = 10 + randint((psi*3)/2);
			if (psi < 35)
			{
				add_flag(TIMED_HERO, b);
			}
			else
			{
				add_flag(TIMED_SHERO, b);
			}
			if (!p_ptr->fast)
			{   /* Haste */
				(void)set_flag(TIMED_FAST, b);
			}
			else
			{
				(void)add_flag(TIMED_FAST, b);
			}
			break;
		case MIND_PSY_DRAIN:
			if (!get_aim_dir(&dir)) return;
			b = damroll(psi/2, 6);
			if (fire_ball(GF_PSI_DRAIN, dir, b,  0 + (psi-25)/10))
			p_ptr->energy -= randint(TURN_ENERGY*15/10);
			break;
		case MIND_TK_WAVE:
			msg_print("A wave of pure physical force radiates out from your body!");
			project(0, 3+psi/10, py, px,
            psi * (psi > 39 ? 4 : 3), GF_TELEKINESIS, PROJECT_KILL|PROJECT_ITEM|PROJECT_GRID);
			break;
	    default:
			msg_print("Zap?");
		}
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
