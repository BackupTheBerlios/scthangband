#define OBJECT1_C
/* File: object1.c */

/* Purpose: Object code, part 1 */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"


/*
 * XXX XXX Hack -- note that "TERM_MULTI" is now just "TERM_VIOLET"
 * We will have to find a cleaner method for "MULTI_HUED" later.
 * There were only two multi-hued "flavors" (one potion, one food).
 * Plus five multi-hued "base-objects" (3 dragon scales, one blade
 * of chaos, and one something else).  See the SHIMMER_OBJECTS code
 * in "dungeon.c" and the object color extractor in "cave.c".
 */
#define TERM_MULTI      TERM_VIOLET


/*
 * Max sizes of the following arrays
 */
#define MAX_SYLLABLES 164       /* Used with scrolls (see below) */



/*
 * Syllables for scrolls (must be 1-4 letters each)
 */

static cptr syllables[MAX_SYLLABLES] =
{
	"a", "ab", "ag", "aks", "ala", "an", "ankh", "app",
	"arg", "arze", "ash", "aus", "ban", "bar", "bat", "bek",
	"bie", "bin", "bit", "bjor", "blu", "bot", "bu",
	"byt", "comp", "con", "cos", "cre", "dalf", "dan",
	"den", "der", "doe", "dok", "eep", "el", "eng", "er", "ere", "erk",
	"esh", "evs", "fa", "fid", "flit", "for", "fri", "fu", "gan",
	"gar", "glen", "gop", "gre", "ha", "he", "hyd", "i",
	"ing", "ion", "ip", "ish", "it", "ite", "iv", "jo",
	"kho", "kli", "klis", "la", "lech", "man", "mar",
	"me", "mi", "mic", "mik", "mon", "mung", "mur", "nag", "nej",
	"nelg", "nep", "ner", "nes", "nis", "nih", "nin", "o",
	"od", "ood", "org", "orn", "ox", "oxy", "pay", "pet",
	"ple", "plu", "po", "pot", "prok", "re", "rea", "rhov",
	"ri", "ro", "rog", "rok", "rol", "sa", "san", "sat",
	"see", "sef", "seh", "shu", "ski", "sna", "sne", "snik",
	"sno", "so", "sol", "sri", "sta", "sun", "ta", "tab",
	"tem", "ther", "ti", "tox", "trol", "tue", "turs", "u",
	"ulk", "um", "un", "uni", "ur", "val", "viv", "vly",
	"vom", "wah", "wed", "werg", "wex", "whon", "wun", "x",
    "yerg", "yp", "zun", "tri", "blaa", "jah", "bul", "on",
    "foo", "ju", "xuxu"
};




/*
 * Give a name to a random artefact.
 */
void get_table_name(char * out_string)
{
    int testcounter = (randint(3)) + 1;

    strcpy(out_string, "'");

    if (randint(3)==2)
    {
        while (testcounter--)
            strcat(out_string, syllables[(randint(MAX_SYLLABLES))-1]);
        }

    else
    {
        char Syllable[80];
        testcounter = (randint(2)) + 1;
        while (testcounter--)
        {
            get_rnd_line("elvish.txt", Syllable);
            strcat(out_string, Syllable);
        }
    }

    out_string[1] = toupper(out_string[1]);

    strcat(out_string, "'");

    out_string[MAX_TABLE_LEN] = '\0';

    return;
}

/*
 * Determine the u_idx of an item with a given k_idx.
 *
 * Return -2 if no match is found.
 */
s16b lookup_unident(byte p_id, byte s_id)
{
	s16b i;

	for (i = 0; i < MAX_U_IDX; i++)
		{
		unident_type *u_ptr = &u_info[i];
		if (u_ptr->p_id != p_id) continue;
		if (u_ptr->s_id != s_id) continue;
		return i;
	}
	return -2;
}


/*
 * Give a name to a scroll.
 *
 * out_string must be able to accept length characters.
 */
static void get_scroll_name(char * out_string, uint length)
{
	(*out_string) = '\0';
	/* Collect words until done */
	while (1)
	{
		int q, s;

		char tmp[80];

		/* Start a new word */
		tmp[0] = '\0';

		/* Choose one or two syllables */
		s = ((rand_int(100) < 30) ? 1 : 2);

		/* Add a one or two syllable word */
		for (q = 0; q < s; q++)
		{
			/* Add the syllable */
			strcat(tmp, syllables[rand_int(MAX_SYLLABLES)]);
		}

		/* Stop before getting too long */
		if (strlen(out_string) + 1 + strlen(tmp) > length) return;

		/* Add a space */
		if (strlen(out_string)) strcat(out_string, " ");

		/* Add the word */
		strcat(out_string, tmp);
	}
}

/*
 * Randomly generate names where appropriate.
 */
static void name_scrolls(void)
{
	u16b scroll_idx[z_info->u_max];
	int i, j, scrolls;

	/* Iterate through the scrolls, renaming each in turn. */
	for (i = 0, scrolls = -1; i < z_info->u_max; i++)
	{
		unident_type *u_ptr = &u_info[i];
		uint length;
		char *s = u_name+u_ptr->name;

		/* Ignore non-scrolls. */
		if (*s != CH_SCROLL) continue;

		/* Recognise IS_BASE scrolls. */
		if (*(s+1) == CH_IS_BASE)
		{
			*(s++) = CH_IS_BASE;
		}

		/* Make a note of this scroll. */
		scroll_idx[++scrolls] = i;

		/* Notice the maximum length. */
		length = strlen(s);

		do
		{
			/* Get a name */
			get_scroll_name(s, length);

			/* Check for duplicate names (first 4 characters) */
			for (j = 0; j < scrolls; j++)
			{
				unident_type *u2_ptr = &u_info[scroll_idx[j]];

				/* Ignore different scrolls. */
				if (strncmp(u_name+u_ptr->name, u_name+u2_ptr->name, 4)) continue;

				/* Too close. */
				break;
			}
		} while (j < scrolls);
	}
}



/*
 * Assign unidentified descriptions to objects.
 */
void flavor_init(void)
{
	int i,j;
	int base_only[256];
	C_TNEW(k_ptr_p_id, z_info->k_max, int);

	/* The default value of base_only is -1 so that a map from [0,255]
	 * is obvious. */
	for (i = 0; i < 256; i++) base_only[i] = -1;

	/* The p_id is stored in the u_idx field in init_k_info,
	 * so extract it to a separate array to avoid confusion. */
	for (i = 0; i < z_info->k_max; i++)
	{
		k_ptr_p_id[i] = k_info[i].u_idx;
	}

	/* Hack -- Use the "simple" RNG */
	Rand_quick = TRUE;

	/* Hack -- Induce consistant flavors */
	Rand_value = seed_flavor;

	/* Give names to the scroll entries in u_info. */
	name_scrolls();

	/*
	 * Then set the u_idxs of flavourless items appropriately.
	 */
	for (i = 0; i < z_info->u_max; i++)
	{
		unident_type *u_ptr = &u_info[i];

		if (!u_ptr->name)
			base_only[u_ptr->p_id] = i;
	}
	for (i = 0; i < z_info->k_max; i++)
	{
		if (base_only[k_ptr_p_id[i]] != -1)
			k_info[i].u_idx = base_only[k_ptr_p_id[i]];
	}
	/*
	 * Finally, give each k_idx with a variable description a valid s_id.
	 * We've already checked that there are enough to go around.
	 */
	for (i = 0; i < 256; i++)
	{
		s16b k_idx[z_info->k_max], u_idx[z_info->u_max];
		s16b k_top, u_top;

		/* Ignore flavourless p_ids */
		if (base_only[i] != -1) continue;

		/* Count all entried with this p_id in k_info */
		for (j = 0, k_top = 0; j < z_info->k_max; j++)
		{
			if (k_ptr_p_id[j] == i) k_idx[k_top++] = j;
		}

		/* And all those in u_info */
		for (j = 0, u_top = 0; j < z_info->u_max; j++)
		{
			if (u_info[j].p_id == i) u_idx[u_top++] = j;
		}

		for (j = 0; j < k_top; j++)
		{
			/* Pick a description from the list. */
			s16b k = rand_int(u_top--);

			/* Assign it to the k_idx in question */
			k_info[k_idx[j]].u_idx = u_idx[k];

			/* Remove it from further consideration. */
			u_idx[k] = u_idx[u_top];
		}
	}

	/* Hack -- Use the "complex" RNG */
	Rand_quick = FALSE;

	TFREE(k_ptr_p_id);
}


/*
 * Hack -- prepare the default object attr codes by tval
 *
 * XXX XXX XXX Off-load to "pref.prf" file
 */
static byte default_tval_to_attr(int tval)
{
	switch (tval)
	{
		case TV_SKELETON:
		case TV_BOTTLE:
		case TV_JUNK:
		{
			return (TERM_WHITE);
		}

		case TV_CHEST:
		{
			return (TERM_SLATE);
		}

		case TV_SHOT:
		case TV_BOLT:
		case TV_ARROW:
		{
			return (TERM_L_UMBER);
		}

		case TV_LITE:
		{
			return (TERM_YELLOW);
		}

		case TV_SPIKE:
		{
			return (TERM_SLATE);
		}

		case TV_BOW:
		{
			return (TERM_UMBER);
		}

		case TV_DIGGING:
		{
			return (TERM_SLATE);
		}

		case TV_HAFTED:
		case TV_POLEARM:
		case TV_SWORD:
		{
			return (TERM_L_WHITE);
		}

		case TV_BOOTS:
		case TV_GLOVES:
		case TV_CROWN:
		case TV_HELM:
		case TV_SHIELD:
		case TV_CLOAK:
		{
			return (TERM_L_UMBER);
		}

		case TV_SOFT_ARMOR:
		case TV_HARD_ARMOR:
		case TV_DRAG_ARMOR:
		{
			return (TERM_SLATE);
		}

		case TV_AMULET:
		{
			return (TERM_ORANGE);
		}

		case TV_RING:
		{
	    return (TERM_ORANGE);
		}

		case TV_STAFF:
		{
	    return (TERM_VIOLET);
		}

		case TV_WAND:
		{
	    return (TERM_VIOLET);
		}

		case TV_ROD:
		{
	    return (TERM_VIOLET);
		}

		case TV_SCROLL:
		{
			return (TERM_WHITE);
		}

		case TV_POTION:
		{
	    return (TERM_BLUE);
		}

		case TV_FLASK:
		{
			return (TERM_YELLOW);
		}

		case TV_FOOD:
		{
	    return (TERM_GREEN);
		}

	case TV_SORCERY_BOOK:
		{
	    return (TERM_L_BLUE);
		}

	case TV_THAUMATURGY_BOOK:
		{
			return (TERM_L_RED);
		}

    case TV_CONJURATION_BOOK:
        {
            return (TERM_ORANGE);
        }
    case TV_NECROMANCY_BOOK:
        {
            return (TERM_SLATE);
        }
	}

	return (TERM_WHITE);
}


/*
 * Hack -- prepare the default object char codes by tval
 *
 * XXX XXX XXX Off-load to "pref.prf" file (?)
 */
static byte default_tval_to_char(int tval)
{
	int i;

	/* Hack -- Guess at "correct" values for tval_to_char[] */
	for (i = 1; i < MAX_K_IDX; i++)
	{
		object_kind *k_ptr = &k_info[i];

		/* Use the first value we find */
		if (k_ptr->tval == tval) return (k_ptr->d_char);
	}

	/* Default to space */
	return (' ');
}



/*
 * Reset the "visual" lists
 *
 * This involves resetting various things to their "default"
 * state, and then loading the appropriate "user pref file"
 * based on the "use_graphics" flag.
 *
 * This is useful for switching "graphics" on/off
 */
void reset_visuals(void)
{
	int i;

	char buf[1024];

	/* Reset various attr/char maps. */
	process_pref_file_aux((char*)"F:---reset---");
	process_pref_file_aux((char*)"K:---reset---");
	process_pref_file_aux((char*)"U:---reset---");
	process_pref_file_aux((char*)"R:---reset---");

	/* Extract some info about monster memory colours. */
	for (i = 0; i < MAX_MONCOL; i++)
	{
		/* Hack - always default to white */
		moncol[i].attr = TERM_WHITE;
	}
	/* Extract attr/chars for equippy items (by tval) */
	for (i = 0; i < 128; i++)
	{
		/* Extract a default attr */
		tval_to_attr[i] = default_tval_to_attr(i);

		/* Extract a default char */
		tval_to_char[i] = default_tval_to_char(i);
	}

	/* Access the "font" or "graf" pref file, based on "use_graphics" */
	sprintf(buf, "%s-%s.prf", (use_graphics ? "graf" : "font"), ANGBAND_SYS);

	/* Process that file */
	process_pref_file(buf);
}









/*
 * Obtain the "flags" for an item
 */
void object_flags(object_type *o_ptr, u32b *f1, u32b *f2, u32b *f3)
{
	object_kind *k_ptr = &k_info[o_ptr->k_idx];

	/* Base object */
	(*f1) = k_ptr->flags1;
	(*f2) = k_ptr->flags2;
	(*f3) = k_ptr->flags3;

	/* Artifact */
	if (o_ptr->name1)
	{
		artifact_type *a_ptr = &a_info[o_ptr->name1];

		(*f1) = a_ptr->flags1;
		(*f2) = a_ptr->flags2;
		(*f3) = a_ptr->flags3;
	}

	/* Ego-item */
	if (o_ptr->name2)
	{
		ego_item_type *e_ptr = &e_info[o_ptr->name2];

		(*f1) |= e_ptr->flags1;
		(*f2) |= e_ptr->flags2;
		(*f3) |= e_ptr->flags3;
	}

    /* Random artifact ! */
    if (o_ptr->flags1 || o_ptr->flags2 || o_ptr->flags3)
    {
	(*f1) |= o_ptr->flags1;
	(*f2) |= o_ptr->flags2;
	(*f3) |= o_ptr->flags3;

	/* Hack - cursing an object via flags3 totally supercedes default values */
	if ((o_ptr->flags3 & TR3_CURSED) && !(o_ptr->flags3 & TR3_HEAVY_CURSE))
		*f3 &= ~TR3_HEAVY_CURSE;
    }

	/* Extra powers */
    if (!(o_ptr->art_name))
    {
        switch (o_ptr->xtra1)
        {
            case EGO_XTRA_SUSTAIN:
            {
                /* Choose a sustain */
                switch (o_ptr->xtra2 % 6)
                {
                    case 0: (*f2) |= (TR2_SUST_STR); break;
                    case 1: (*f2) |= (TR2_SUST_INT); break;
                    case 2: (*f2) |= (TR2_SUST_WIS); break;
                    case 3: (*f2) |= (TR2_SUST_DEX); break;
                        case 4: (*f2) |= (TR2_SUST_CON); break;
                    case 5: (*f2) |= (TR2_SUST_CHR); break;
                }

                break;
                }

            case EGO_XTRA_POWER:
            {
                /* Choose a power */
                    switch (o_ptr->xtra2 % 11)
                {
                    case 0: (*f2) |= (TR2_RES_BLIND); break;
                    case 1: (*f2) |= (TR2_RES_CONF); break;
                    case 2: (*f2) |= (TR2_RES_SOUND); break;
                    case 3: (*f2) |= (TR2_RES_SHARDS); break;
                        case 4: (*f2) |= (TR2_RES_NETHER); break;
                    case 5: (*f2) |= (TR2_RES_NEXUS); break;
                    case 6: (*f2) |= (TR2_RES_CHAOS); break;
                        case 7: (*f2) |= (TR2_RES_DISEN); break;
                    case 8: (*f2) |= (TR2_RES_POIS); break;
                    case 9: (*f2) |= (TR2_RES_DARK); break;
                    case 10: (*f2) |= (TR2_RES_LITE); break;
                }

                break;
            }

            case EGO_XTRA_ABILITY:
            {
                /* Choose an ability */
                switch (o_ptr->xtra2 % 8)
                {
                    case 0: (*f3) |= (TR3_FEATHER); break;
                    case 1: (*f3) |= (TR3_LITE); break;
                    case 2: (*f3) |= (TR3_SEE_INVIS); break;
                    case 3: (*f3) |= (TR3_TELEPATHY); break;
                    case 4: (*f3) |= (TR3_SLOW_DIGEST); break;
                        case 5: (*f3) |= (TR3_REGEN); break;
                    case 6: (*f2) |= (TR2_FREE_ACT); break;
                    case 7: (*f2) |= (TR2_HOLD_LIFE); break;
                }

                break;
            }

        }
        }
    }



struct object_extra {
	s16b k_idx;
	s16b tval;
	s16b u_idx;
};

/*
 * Create an object containing the known information about an object.
 *
 * o_ptr points to the original object.
 * j_ptr points to the object to be created.
 * x_ptr either points to an extra array in which is placed various information
 * not known to the player which is needed to define the object, or is NULL.
 * Ideally it should not be necessary.
 */
void object_info_known(object_type *j_ptr, object_type *o_ptr, object_extra *x_ptr)
{
	bool spoil = FALSE;
	int i;

	object_kind *k_ptr = &k_info[o_ptr->k_idx];

	object_wipe(j_ptr);

	/* If needed, place the k_idx in x_ptr. */
	if (x_ptr)
	{
		x_ptr->k_idx = o_ptr->k_idx;
		x_ptr->tval = o_ptr->tval;
		x_ptr->u_idx = k_ptr->u_idx;
	}

	/* Some flags are always assumed to be known. */
	j_ptr->discount = o_ptr->discount;
	j_ptr->number = o_ptr->number;
	j_ptr->weight = o_ptr->weight;
	j_ptr->ident = o_ptr->ident;
	/* j_ptr->handed = o_ptr->handed; */ /* Unused */
	j_ptr->note = o_ptr->note;
	
	/* Some flags are known for aware objects. */
	if (object_aware_p(o_ptr))
	{
		j_ptr->k_idx = o_ptr->k_idx;
		j_ptr->tval = o_ptr->tval;
		j_ptr->sval = o_ptr->sval;
		j_ptr->dd = o_ptr->dd;
		j_ptr->ds = o_ptr->ds;
		j_ptr->ac = o_ptr->ac;
	}
	/* As values of 0 are often special, use a blank object, the tval from the
	 * base type and and a special sval for unaware objects.
	 */
	else
	{
		j_ptr->k_idx = lookup_kind(0,0); /* != 0 */
		j_ptr->tval = o_base[u_info[k_ptr->u_idx].p_id].tval;
		j_ptr->sval = SV_UNKNOWN;
	}

	/* Some flags are known for identified objects. */
	if (object_known_p(o_ptr))
	{
		j_ptr->name1 = o_ptr->name1;
		j_ptr->name2 = o_ptr->name2;
		j_ptr->art_name = o_ptr->art_name;
		j_ptr->to_h = o_ptr->to_h;
		j_ptr->to_d = o_ptr->to_d;
		j_ptr->to_a = o_ptr->to_a;
		j_ptr->pval = o_ptr->pval;
		/* The player never knows how long the timeout is. */
		j_ptr->timeout = !!(o_ptr->timeout); 
	}

	/* Some flags are only used internally */
	/* j_ptr->next_o_idx = o_ptr->next_o_idx; */
	/* j_ptr->held_m_idx = o_ptr->held_m_idx; */

	/* Hack - as the inventory list is only differentiated by location,
	 * and this cannot be transferred to j_ptr, use a null iy and non-null ix
	 * to indicate the slot used.
	 */
	if (o_ptr >= inventory && o_ptr < inventory+INVEN_TOTAL)
	{
		j_ptr->iy = 0;
		j_ptr->ix = o_ptr-inventory+1;
	}
	else
	{
		j_ptr->iy = o_ptr->iy;
		j_ptr->ix = o_ptr->ix;
	}

	/* Assume that the player has noticed certain things when using an
	 * unidentified item. The IDENT_TRIED flag is currently only set when
	 * an item is worn, which works as none of the flags below have an
	 * effect otherwise, and the flag is only checked here.
	 */
	if (spoil_flag && o_ptr->ident & IDENT_TRIED)
	{
		/* Get all flags */
		object_flags(o_ptr, &(j_ptr->flags1), &(j_ptr->flags2), &(j_ptr->flags3));
		
		/* Clear non-obvious flags */
	 	j_ptr->flags1 &= (TR1_STR | TR1_INT | TR1_WIS | TR1_DEX | TR1_CON | TR1_CHR | TR1_INFRA | TR1_SPEED | TR1_BLOWS);

		/* *Hack* - notice if the modifier is cancelled out by other modifiers */
		for (i = 0; i < 6; i++)
		{
			if (p_ptr->stat_top[i] == 3 && (j_ptr->flags1 & TR1_STR << i) && equip_mod(i) >= 0)
			{
				j_ptr->flags1 &= ~(TR1_STR << i);
			}
		}
		/* This should just check the effect of the weapon, but it's easier this way. */
		if (p_ptr->num_blow == 1) j_ptr->flags1 &= ~(TR1_BLOWS);
				
		j_ptr->flags2 = 0L;
		j_ptr->flags3 &= (TR3_LITE | TR3_XTRA_MIGHT | TR3_XTRA_SHOTS);
		/* Don't assume that the multiplier is always known. */
		if (!spoil_base) j_ptr->flags3 &= ~(TR3_XTRA_MIGHT);

		/* Hack - Set flags known through cumber_*(). Luckily, a mysterious
		 * failure to encumber the player can only mean one thing. */
		switch (wield_slot(j_ptr))
		{
			case INVEN_HANDS:
			{
				if (cumber_glove(j_ptr) && !cumber_glove(o_ptr))
					j_ptr->flags2 |= TR2_FREE_ACT;
				break;
			}
			case INVEN_HEAD:
			{
				/* Telepathy is fairly obvious, but this makes it clear. */
				if (cumber_helm(j_ptr) && !cumber_helm(o_ptr))
					j_ptr->flags3 |= TR3_TELEPATHY;
				break;
			}
		}
			
			

		/* If a pval-based flag is known from experience, set the pval. */
		if (j_ptr->flags1 & TR1_PVAL_MASK)
		{
			j_ptr->pval = o_ptr->pval;
		}
	}

	/* Find the flags the player can know about unaware items.
	 * Hack - ignore other things the player is certain to know, as the
	 * player wants clear potions and the like.
	 */
	if (spoil_base && !object_aware_p(o_ptr))
	{
		o_base_type *ob_ptr = o_base+u_info[k_ptr->u_idx].p_id;

		j_ptr->flags1 |= ob_ptr->flags1;
		j_ptr->flags2 |= ob_ptr->flags2;
		j_ptr->flags3 |= ob_ptr->flags3;
	}
		

	/* Check for cursing even if unidentified */
	if ((o_ptr->ident & IDENT_CURSED) && (o_ptr->ident & IDENT_SENSE_CURSED))
		j_ptr->flags3 |= TR3_CURSED;

	if (cheat_item && (
#ifdef SPOIL_ARTIFACTS
	/* Full knowledge for some artifacts */
	allart_p(o_ptr) ||
#endif
#ifdef SPOIL_EGO_ITEMS
	/* Full knowledge for some ego-items */
	ego_item_p(o_ptr) ||
#endif
	(!allart_p(o_ptr) && !ego_item_p(o_ptr)))) spoil = TRUE;

	/* Base objects */
	if ((spoil_base || spoil || o_ptr->ident & IDENT_MENTAL) &&
	(object_known_p(o_ptr) || object_aware_p(o_ptr)))
	{
	j_ptr->flags1 |= k_ptr->flags1;
	j_ptr->flags2 |= k_ptr->flags2;
	j_ptr->flags3 |= k_ptr->flags3;
	}
		
	/* Must be identified for further details. */
	if (!object_known_p(o_ptr)) return;

    /* Ego-item (known basic flags) */
	if (o_ptr->name2 && (spoil_ego || spoil || o_ptr->ident & IDENT_MENTAL))
	{
		ego_item_type *e_ptr = &e_info[o_ptr->name2];

		j_ptr->flags1 |= e_ptr->flags1;
		j_ptr->flags2 |= e_ptr->flags2;
		j_ptr->flags3 |= e_ptr->flags3;
	}

	/* Pre-defined artifacts (known basic flags) */
	if (o_ptr->name1 && (spoil_art || spoil || o_ptr->ident & IDENT_MENTAL))
	{
		artifact_type *a_ptr = &a_info[o_ptr->name1];

		j_ptr->flags1 |= a_ptr->flags1;
		j_ptr->flags2 |= a_ptr->flags2;
		j_ptr->flags3 |= a_ptr->flags3;
	}

	/* The random powers of *IDENTIFIED* items are known. */
	if (spoil || o_ptr->ident & IDENT_MENTAL)
	{
		j_ptr->flags2 &= ~(TR2_RAND_RESIST | TR2_RAND_POWER | TR2_RAND_EXTRA);
	}

	/* Check for both types of cursing */
	if ((o_ptr->ident & IDENT_CURSED) && (o_ptr->ident & IDENT_SENSE_CURSED))
		j_ptr->flags3 |= TR3_CURSED;
	else if (o_ptr->ident & IDENT_SENSE_CURSED)
		j_ptr->flags3 &= ~(TR3_CURSED | TR3_HEAVY_CURSE | TR3_PERMA_CURSE);

	/* Need full knowledge or spoilers */
	if (!spoil && !(o_ptr->ident & IDENT_MENTAL)) return;

    /* Random artifact ! */
    if (o_ptr->flags1 || o_ptr->flags2 || o_ptr->flags3)
    {
	j_ptr->flags1 |= o_ptr->flags1;
	j_ptr->flags2 |= o_ptr->flags2;
	j_ptr->flags3 |= o_ptr->flags3;

    }

	/* Full knowledge for *identified* objects */
	if (!(o_ptr->ident & IDENT_MENTAL)) return;


    if (!(o_ptr->art_name))
    {
        /* Extra powers */
        switch (o_ptr->xtra1)
        {
            case EGO_XTRA_SUSTAIN:
            {
                /* Choose a sustain */
                switch (o_ptr->xtra2 % 6)
                {
                    case 0: j_ptr->flags2 |= (TR2_SUST_STR); break;
                    case 1: j_ptr->flags2 |= (TR2_SUST_INT); break;
                    case 2: j_ptr->flags2 |= (TR2_SUST_WIS); break;
                    case 3: j_ptr->flags2 |= (TR2_SUST_DEX); break;
                    case 4: j_ptr->flags2 |= (TR2_SUST_CON); break;
                    case 5: j_ptr->flags2 |= (TR2_SUST_CHR); break;
                }

                break;
            }

            case EGO_XTRA_POWER:
            {
                /* Choose a power */
                switch (o_ptr->xtra2 % 11)
                {
                    case 0: j_ptr->flags2 |= (TR2_RES_BLIND); break;
                    case 1: j_ptr->flags2 |= (TR2_RES_CONF); break;
                    case 2: j_ptr->flags2 |= (TR2_RES_SOUND); break;
                    case 3: j_ptr->flags2 |= (TR2_RES_SHARDS); break;
                    case 4: j_ptr->flags2 |= (TR2_RES_NETHER); break;
                    case 5: j_ptr->flags2 |= (TR2_RES_NEXUS); break;
                    case 6: j_ptr->flags2 |= (TR2_RES_CHAOS); break;
                    case 7: j_ptr->flags2 |= (TR2_RES_DISEN); break;
                    case 8: j_ptr->flags2 |= (TR2_RES_POIS); break;
                    case 9: j_ptr->flags2 |= (TR2_RES_DARK); break;
                    case 10: j_ptr->flags2 |= (TR2_RES_LITE); break;
                }

                break;
            }

            case EGO_XTRA_ABILITY:
            {
                /* Choose an ability */
                switch (o_ptr->xtra2 % 8)
                {
                    case 0: j_ptr->flags3 |= (TR3_FEATHER); break;
                    case 1: j_ptr->flags3 |= (TR3_LITE); break;
                    case 2: j_ptr->flags3 |= (TR3_SEE_INVIS); break;
                    case 3: j_ptr->flags3 |= (TR3_TELEPATHY); break;
                    case 4: j_ptr->flags3 |= (TR3_SLOW_DIGEST); break;
                    case 5: j_ptr->flags3 |= (TR3_REGEN); break;
                    case 6: j_ptr->flags2 |= (TR2_FREE_ACT); break;
                    case 7: j_ptr->flags2 |= (TR2_HOLD_LIFE); break;
                }

                break;
            }
        }
    }
}




/*
 * Obtain the "flags" for an item which are known to the player
 */
void object_flags_known(object_type *o_ptr, u32b *f1, u32b *f2, u32b *f3)
{
	object_type j;
	object_info_known(&j, o_ptr, 0);
	(*f1) = j.flags1;
	(*f2) = j.flags2;
	(*f3) = j.flags3;
}





/*
 * Parse through the string until the combination of
 * flags is compatible with the current rejection
 * criteria or the end of the string is reached.
 * Return the first "safe" character.
 */
static cptr find_next_good_flag(cptr s, byte reject, byte require)
{
	int bad;

	/* If this string doesn't actually start with a flag-altering
	 * character, there is nothing to do. */
	if (!is_cx_char(*s))
	{
		return s;
	}

	for (bad = 0;;s++)
	{
		char flag = 1<<find_ci(*s);
		char mod = find_cm(*s);

		/* Ordinary characters do nothing. */
		if (*s & 0xE0);
				
		/* NULs are parsed elewhere. */
		else if (*s == '\0') return s;

		/* CH_NORM always reduces restrictions */
		else if (mod == CM_NORM)
		{
			bad &= ~(flag);
		}

		/*
		 * If a flag which isn't present is required
		 * or a flag which is present is forbidden,
		 * the set of "wrong" flags increases.
		 */
		else if (((mod == CM_TRUE) && (reject & flag)) ||
			((mod == CM_FALSE) && (require & flag)))
		{
			bad |= flag;
		}
		/*
		 * If a flag which is present is required
		 * or a flag which isn't present is forbidden,
		 * the set of "wrong" flags decreases.
		 */
		else if (((mod == CM_TRUE) && (require & flag)) ||
			((mod == CM_FALSE) && (reject & flag)))
		{
			bad &= ~flag;
		}

		if (!bad) break;
	}

	/* The loop finished with a flag-altering character, but
	 * the character after it may be printable. */
 	return s+1;
}

/*
 * Print a char "c" into a string "t", as if by sprintf(t, "%c", c),
 * and return a pointer to the terminator (t + 1).
 */
static char *object_desc_chr(char *t, char c)
{
	/* Copy the char */
	*t++ = c;

	/* Terminate */
	*t = '\0';

	/* Result */
	return (t);
}


/*
 * Print a string "s" into a string "t", as if by strcpy(t, s),
 * and return a pointer to the terminator.
 */
static char *object_desc_str(char *t, cptr s)
{
	/* Copy the string */
	while (*s) *t++ = *s++;

	/* Terminate */
	*t = '\0';

	/* Result */
	return (t);
}



/*
 * Print an unsigned number "n" into a string "t", as if by
 * sprintf(t, "%u", n), and return a pointer to the terminator.
 */
static char *object_desc_num(char *t, uint n)
{
	uint p;

	/* Find "size" of "n" */
	for (p = 1; n >= p * 10; p = p * 10) /* loop */;

	/* Dump each digit */
	while (p >= 1)
	{
		/* Dump the digit */
		*t++ = '0' + n / p;

		/* Remove the digit */
		n = n % p;

		/* Process next digit */
		p = p / 10;
	}

	/* Terminate */
	*t = '\0';

	/* Result */
	return (t);
}




/*
 * Print an signed number "v" into a string "t", as if by
 * sprintf(t, "%+d", n), and return a pointer to the terminator.
 * Note that we always print a sign, either "+" or "-".
 */
static char *object_desc_int(char *t, sint v)
{
	uint p, n;

	/* Negative */
	if (v < 0)
	{
		/* Take the absolute value */
		n = 0 - v;

		/* Use a "minus" sign */
		*t++ = '-';
	}

	/* Positive (or zero) */
	else
	{
		/* Use the actual number */
		n = v;

		/* Use a "plus" sign */
		*t++ = '+';
	}

	/* Find "size" of "n" */
	for (p = 1; n >= p * 10; p = p * 10) /* loop */;

	/* Dump each digit */
	while (p >= 1)
	{
		/* Dump the digit */
		*t++ = '0' + n / p;

		/* Remove the digit */
		n = n % p;

		/* Process next digit */
		p = p / 10;
	}

	/* Terminate */
	*t = '\0';

	/* Result */
	return (t);
}


/*
 * Creates a description of the item "o_ptr", and stores it in "out_val".
 *
 * One can choose the "verbosity" of the description, including whether
 * or not the "number" of items should be described, and how much detail
 * should be used when describing the item.
 *
 * The given "buf" must be 80 chars long to hold the longest possible
 * description, which can get pretty long, including incriptions, such as:
 * "no more Maces of Disruption (Defender) (+10,+10) [+5] (+3 to stealth)".
 * Note that the inscription will be clipped to keep the total description
 * under 79 chars (plus a terminator).
 *
 * Note the use of "object_desc_num()" and "object_desc_int()" as hyper-efficient,
 * portable, versions of some common "sprintf()" commands.
 *
 * Note that all ego-items (when known) append an "Ego-Item Name", unless
 * the item is also an artifact, which should NEVER happen.
 *
 * Note that all artifacts (when known) append an "Artifact Name", so we
 * have special processing for "Specials" (artifact Lites, Rings, Amulets).
 * The "Specials" never use "modifiers" if they are "known", since they
 * have special "descriptions", such as "The Necklace of the Dwarves".
 *
 * Special Lite's use the "k_info" base-name (Phial, Star, or Arkenstone),
 * plus the artifact name, just like any other artifact, if known.
 *
 * Special Ring's and Amulet's, if not "aware", use the same code as normal
 * rings and amulets, and if "aware", use the "k_info" base-name (Ring or
 * Amulet or Necklace).  They will NEVER "append" the "k_info" name.  But,
 * they will append the artifact name, just like any artifact, if known.
 *
 * None of the Special Rings/Amulets are "EASY_KNOW", though they could be,
 * at least, those which have no "pluses", such as the three artifact lites.
 *
 * If "pref" then a "numeric" prefix will be pre-pended.
 *
 * Mode:
 *   0 -- The Cloak of Death
 *   1 -- The Cloak of Death [1,+3]
 *   2 -- The Cloak of Death [1,+3] (+2 to Stealth)
 *   3 -- The Cloak of Death [1,+3] (+2 to Stealth) {nifty}
 */

/* The number of strings which can be in the process of being printed at once. */
#define MAX_NAME_LEVEL	8

/* Julian Lighton's improved code */
void object_desc(char *buf, object_type *o1_ptr, int pref, int mode)
{
	/* The strings from which buf is compiled. */
	C_TNEW(strings, 8, cptr) = {0,0,0,0,0,0,0,0};

/* If a CH_ARTICLE would cause an article to be prepended, this is set
 * to ARTICLE_REQUEST. If one is found, it is changed to ARTICLE_LETTER.
 * Once the initial name-writing loop is finished, the program searches
 * the output string for the first letter or number, and adds "a" or "an"
 * to the beginning as appropriate. */

#define ARTICLE_NO	0
#define ARTICLE_REQUEST	1
#define ARTICLE_LETTER	2

	int wants_article = ARTICLE_NO;

	int this_level = 0;

	byte reject, current;


	cptr s;

	int                     power,i;

	bool	aware, known;

	bool	show_weapon = FALSE, show_armour = FALSE;

	cptr	r[MAX_NAME_LEVEL];
	char            *t;

	char            p1 = '(', p2 = ')';
	char            b1 = '[', b2 = ']';
	char            c1 = '{', c2 = '}';

	/* tmp_val_base is twice as large as necessary as little bounds checking
	 * is performed. This probably isn't good enough. */
	C_TNEW(tmp_val_base, ONAME_MAX*2, char);
	char		*tmp_val = tmp_val_base;

	u32b            f1, f2, f3;

	object_kind	*k_ptr = &k_info[o1_ptr->k_idx];
	unident_type *u_ptr;
	o_base_type *ob_ptr;
	object_type o_ptr[1];
	object_extra x_ptr[1];

	/* Extract the known information. */
	object_info_known(o_ptr, o1_ptr, x_ptr);

	k_ptr = &k_info[o_ptr->k_idx];

	/* If k_ptr is nonsensical, use k_info[0] instead. */
	/* if (!k_ptr->name) k_ptr = k_info; */

	u_ptr = u_info+x_ptr->u_idx;
	ob_ptr = o_base+u_ptr->p_id;

	object_flags(o1_ptr, &f1, &f2, &f3);

	known = (object_known_p(o_ptr) ? TRUE : FALSE);
	aware = (object_aware_p(o_ptr) ? TRUE : FALSE);

	reject = current = 0;

	strings[CI_BASE] = ob_name+ob_ptr->name;
	strings[CI_FLAVOUR] = u_name+u_ptr->name;
	strings[CI_K_IDX] = k_name+k_ptr->name;
	strings[CI_EGO] = e_name+e_info[o_ptr->name2].name;
	if (o1_ptr->art_name)
		strings[CI_ARTEFACT] = quark_str(o_ptr->art_name);
	else
		strings[CI_ARTEFACT] = a_name+a_info[o_ptr->name1].name;

	/* If any of the above have been set to an empty string, they shall be
	 * ignored. */
	for (i = 0; i < 8; i++)
	{
		if (strings[i] && !strings[i][0]) reject |= 1 <<i;
	}

	/* Identified artefacts, and all identified objects if
	 * show_flavors is unset, do not include a FLAVOUR string. */
	if (aware && (artifact_p(o_ptr) || plain_descriptions))
		reject |= 1 << CI_FLAVOUR;

	/* Singular objects take no plural. */
	if (o_ptr->number == 1) reject |= 1 << CI_PLURAL;

	/* Start dumping the result */
	t = tmp_val;

	/* Find start, looking for first CH_IS_BASE. */
	for (power = 7;; power--)
	{
		/* Default to CI_BASE. */
		if (power == -1)
		{
			current |= 1<< CI_BASE;
			r[this_level] = strings[CI_BASE];
			break;
		}

		s = strings[power];

		/* Reject non-strings. */
		if (!s) continue;

		/* Reject rejected strings. */
		if (reject & 1<<power) continue;

		/* Accept if initial character is CH_IS_BASE */
		if (*s == CH_IS_BASE)
		{
			current |= 1<< power;
			r[this_level] = strings[power]+1;
			break;
		}
	}

	/*
	 * Add an article if required. This is only possible at the start
	 * of the output string because of the grammatical problems
	 * that are created if I do otherwise. It also makes it easier
	 * to work with.
	 */

	/* No prefix */
	if (!pref)
	{
		/* Nothing */
	}

	/* Hack -- None left */
	else if (o_ptr->number <= 0 && o_ptr->k_idx)
	{
		t = object_desc_str(t, "no more ");
	}

	/* Extract the number */
	else if (o_ptr->number > 1)
	{
		t = object_desc_num(t, o1_ptr->number);
		t = object_desc_chr(t, ' ');
	}

	/* Hack -- The only one of its kind */
	else if (allart_p(o_ptr))
	{
		t = object_desc_str(t, "the ");
	}
	/* A single one (later) */
	else
	{
		wants_article = ARTICLE_REQUEST;

		/* Leave enough space to prepend "An ". */
		tmp_val += 3;
		t += 3;
	}

	/* Copy the string */
	while (TRUE)
	{
		/* Normal */
		if (*r[this_level] & 0xE0)
		{
			/* Copy */
			*t++ = *r[this_level]++;
		}
		/* Return to original string or finish. */
		else if (*r[this_level] == '\0')
		{
			if (this_level)
			{
				r[--this_level]++;
			}
			else
			{
				*t = '\0';
				break;
			}
		}
		/* Handle CH_ARTICLE later. */
		else if (*r[this_level] == CH_ARTICLE)
		{
			r[this_level]++;
			if (wants_article == ARTICLE_REQUEST)
				wants_article = ARTICLE_LETTER;
		}
		/* Normalise flag */
		else if (find_cm(*r[this_level]) == CM_NORM)
		{
			r[this_level]++;
		}
		/* Change flags. As this returns a switch flag,
		 * advance to the next potential active character. */
		else if (find_cm(*r[this_level]) != CM_ACT)
		{
			r[this_level] = find_next_good_flag(r[this_level], reject | current, ~(reject | current));
	  	}
		/* Pluralizer (internal) */
		else if (*r[this_level] == CM_ACT+CI_PLURAL)
		{
			/* Add a plural if possible */
			if (t != tmp_val)
			{
				char k = t[-1];

				/* XXX XXX XXX Mega-Hack */

				/* Hack -- "Cutlass-es" and "Torch-es" */
				if ((k == 's') || (k == 'h')) *t++ = 'e';

				/* Add an 's' */
				*t++ = 's';
			}
			r[this_level]++;
		}
		/* Paranoia - too many levels.
		 * As "current" prevents the same string from being
		 * started recursively, this can't happen. */
		else if (this_level == MAX_NAME_LEVEL-1)
		{
			r[this_level]++;
		}
		/* Switch to another string */
		else if (strings[find_ci(*r[this_level])])
		{
			int temp = find_ci(*r[this_level]);
			current |= 1<<temp;
			r[++this_level] = strings[temp];
		}
		/* Paranoia - nothing else makes sense. */
		else
		{
			r[this_level]++;
		}
	}

	/* Replace articles now the following string is known. */
	if (wants_article == ARTICLE_LETTER)
	{
		/* Find the first letter/number. */
		for (s = tmp_val; !isalnum(*s) && *s; s++);

		/* And copy an article to it without termination if one is found. */
		if (is_a_vowel(*s) || *s == '8')
		{
			tmp_val -= 3;
			strncpy(tmp_val, "an ", 3);
		}
		else if (isalnum(*s))
		{
			tmp_val -= 2;
			strncpy(tmp_val, "a ", 2);
  		}
	}

	/* The rest follows a hard-coded format. */
	TFREE(strings);

	/* No more details wanted */
	if (mode < 1) goto copyback;


	/* Hack -- Chests must be described in detail */
	if (o_ptr->tval == TV_CHEST)
	{
		/* Not searched yet */
		if (!known)
		{
			/* Nothing */
		}

			/* May be "empty" */
		else if (!o1_ptr->pval)
			{
				t = object_desc_str(t, " (empty)");
			}

		/* May be "disarmed" */
		else if (o1_ptr->pval < 0)
		{
			if (chest_traps[o1_ptr->pval])
			{
				t = object_desc_str(t, " (disarmed)");
			}
			else
			{
				t = object_desc_str(t, " (unlocked)");
			}
		}

		/* Describe the traps, if any */
		else
		{
			/* Describe the traps */
			switch (chest_traps[o_ptr->pval])
			{
				case 0:
				{
					t = object_desc_str(t, " (Locked)");
					break;
				}
				case CHEST_LOSE_STR:
				{
					t = object_desc_str(t, " (Poison Needle)");
					break;
				}
				case CHEST_LOSE_CON:
				{
					t = object_desc_str(t, " (Poison Needle)");
					break;
				}
				case CHEST_POISON:
				{
					t = object_desc_str(t, " (Gas Trap)");
					break;
				}
				case CHEST_PARALYZE:
				{
					t = object_desc_str(t, " (Gas Trap)");
					break;
				}
				case CHEST_EXPLODE:
				{
					t = object_desc_str(t, " (Explosion Device)");
					break;
				}
				case CHEST_SUMMON:
				{
					t = object_desc_str(t, " (Summoning Runes)");
					break;
				}
				default:
				{
					t = object_desc_str(t, " (Multiple Traps)");
					break;
				}
			}
		}
	}


	/* Display the item like a weapon */
	if (f3 & (TR3_SHOW_MODS)) show_weapon = TRUE;
	if (o_ptr->to_h && o_ptr->to_d) show_weapon = TRUE;

	/* Display the item like armour */
	if (f3 & TR3_SHOW_ARMOUR) show_armour = TRUE;
	if (o_ptr->ac) show_armour = TRUE;

	/* Dump base weapon info if known. */
	if (aware) switch (o_ptr->tval)
	{
		/* Missiles and Weapons */
		case TV_SHOT:
		case TV_BOLT:
		case TV_ARROW:
		case TV_HAFTED:
		case TV_POLEARM:
		case TV_SWORD:
		case TV_DIGGING:

		/* Append a "damage" string */
		t = object_desc_chr(t, ' ');
		t = object_desc_chr(t, p1);
		t = object_desc_num(t, o_ptr->dd);
		t = object_desc_chr(t, 'd');
		t = object_desc_num(t, o_ptr->ds);
		t = object_desc_chr(t, p2);

		/* All done */
		break;


		/* Bows get a special "damage string" */
		case TV_BOW:

		/* Mega-Hack -- Extract the "base power" */
		power = (o1_ptr->sval % 10);

		/* Apply the "Extra Might" flag */
		if (f3 & (TR3_XTRA_MIGHT)) power++;

		/* Append a special "damage" string */
		t = object_desc_chr(t, ' ');
		t = object_desc_chr(t, p1);
		t = object_desc_chr(t, 'x');
		t = object_desc_num(t, power);
		t = object_desc_chr(t, p2);

		/* All done */
		break;

		/* Other stuff, if allowed */
		default:
		if (spoil_dam && o_ptr->ds && o_ptr->dd)
		{
			t = object_desc_chr(t, ' ');
			t = object_desc_chr(t, p1);
			t = object_desc_num(t, o1_ptr->dd);
			t = object_desc_chr(t, 'd');
			t = object_desc_num(t, o1_ptr->ds);
			t = object_desc_chr(t, p2);
		}
	}


	/* Add the weapon bonuses */
	if (known)
	{
		/* Show the tohit/todam on request */
		if (show_weapon)
		{
			t = object_desc_chr(t, ' ');
			t = object_desc_chr(t, p1);
			t = object_desc_int(t, o_ptr->to_h);
			t = object_desc_chr(t, ',');
			t = object_desc_int(t, o_ptr->to_d);
			t = object_desc_chr(t, p2);
		}

		/* Show the tohit if needed */
		else if (o_ptr->to_h)
		{
			t = object_desc_chr(t, ' ');
			t = object_desc_chr(t, p1);
			t = object_desc_int(t, o_ptr->to_h);
			t = object_desc_chr(t, p2);
		}

		/* Show the todam if needed */
		else if (o_ptr->to_d)
		{
			t = object_desc_chr(t, ' ');
			t = object_desc_chr(t, p1);
			t = object_desc_int(t, o_ptr->to_d);
			t = object_desc_chr(t, p2);
		}
	}


	/* Add the armor bonuses */
	if (known)
	{
		/* Show the armor class info */
		if (show_armour)
		{
			t = object_desc_chr(t, ' ');
			t = object_desc_chr(t, b1);
			t = object_desc_num(t, o_ptr->ac);
			t = object_desc_chr(t, ',');
			t = object_desc_int(t, o1_ptr->to_a);
			t = object_desc_chr(t, b2);
		}

		/* No base armor, but does increase armor */
		else if (o_ptr->to_a)
		{
			t = object_desc_chr(t, ' ');
			t = object_desc_chr(t, b1);
			t = object_desc_int(t, o_ptr->to_a);
			t = object_desc_chr(t, b2);
		}
	}

	/* Hack -- always show base armor */
	else if (show_armour)
	{
		t = object_desc_chr(t, ' ');
		t = object_desc_chr(t, b1);
		t = object_desc_num(t, o_ptr->ac);
		t = object_desc_chr(t, b2);
	}


	/* No more details wanted */
	if (mode < 2) goto copyback;


	/* Hack -- Wands and Staffs have charges */
	if (known &&
	    ((o1_ptr->tval == TV_STAFF) ||
	     (o1_ptr->tval == TV_WAND)))
	{
		/* Dump " (N charges)" */
		t = object_desc_chr(t, ' ');
		t = object_desc_chr(t, p1);
		t = object_desc_num(t, o1_ptr->pval);
		t = object_desc_str(t, " charge");
		if (o1_ptr->pval != 1) t = object_desc_chr(t, 's');
		t = object_desc_chr(t, p2);
	}

	/* Hack -- Rods have a "charging" indicator */
	else if (o_ptr->tval == TV_ROD)
	{
		/* Hack -- Dump " (charging)" if relevant */
		if (o_ptr->pval) t = object_desc_str(t, " (charging)");
	}

	/* Hack -- Process Lanterns/Torches */
	else if ((o_ptr->tval == TV_LITE) && (!allart_p(o1_ptr)))
	{
		/* Hack -- Turns of light for normal lites */
		t = object_desc_str(t, " (with ");
		t = object_desc_num(t, o_ptr->pval);
		t = object_desc_str(t, " turns of light)");
	}


	/* Dump "pval" flags for wearable items */
	if (f1 & TR1_PVAL_MASK)
	{
		u32b g1;
		if (known)
		{
			g1 = f1;
		}
		/* Find the known flags if the pval is known. */
		else
		{
			object_type j;
			object_info_known(&j, o1_ptr, 0);
			if (j.pval)
			{
				g1 = j.flags1;
			}
			else
			{
				g1 = 0;
			}
		}

		if (g1 & (TR1_PVAL_MASK))
	{
		/* Start the display */
		t = object_desc_chr(t, ' ');
		t = object_desc_chr(t, p1);

			/* Dump the "pval" itself */
			t = object_desc_int(t, o1_ptr->pval);

			/* Differentiate known pvals from deduced ones. */
			if (!known) t = object_desc_chr(t, '?');

		/* Do not display the "pval" flags */
		if (f3 & (TR3_HIDE_TYPE))
		{
			/* Nothing */
		}
		else
		{

/* What actually needs to be done with the strings below. */
#define ADD_PV(X) \
	t = object_desc_chr(t, ' '); \
	t = object_desc_str(t, (X))

			/* Give details about a single bonus. */
			switch (g1 & TR1_PVAL_MASK)
			{
				case TR1_STR: ADD_PV("str"); break;
				case TR1_INT: ADD_PV("int"); break;
				case TR1_WIS: ADD_PV("wis"); break;
				case TR1_DEX: ADD_PV("dex"); break;
				case TR1_CON: ADD_PV("con"); break;
				case TR1_CHR: ADD_PV("chr"); break;
				case TR1_STEALTH: ADD_PV("stealth"); break;
				case TR1_SEARCH: ADD_PV("search"); break;
				case TR1_INFRA: ADD_PV("infra"); break;
				case TR1_TUNNEL: ADD_PV("dig"); break;
				case TR1_SPEED: ADD_PV("movement speed"); break;
				case TR1_BLOWS: ADD_PV("attack speed"); break;
			}
		}

		/* Finish the display */
		t = object_desc_chr(t, p2);
	}
	}

	/* Indicate "charging" artifacts XXX XXX XXX */
	if (known && o1_ptr->timeout)
	{
		/* Hack -- Dump " (charging)" if relevant */
		t = object_desc_str(t, " (g)");
	}


	/* Combine the user-defined and automatic inscriptions if required. */
	if (mode >= 3)
	{
		cptr k[4];
		int i = 0;

		/* This is long enough for "(2147483647)" and for "100% off".*/
		char tmp2[2][13];

		/* Find the sections of inscription. */

		/* Obtain the value this would have if uncursed if allowed. If this
		 * differs from the present value, put parentheses around the number.
		 * Hack - suppress for empty slots.
		 */
		if (spoil_value && spoil_base && o1_ptr->k_idx)
		{
			object_type j_body, *j_ptr = &j_body;
			object_extra x_ptr[1];
			s32b value;
			bool worthless;

			object_info_known(j_ptr, o1_ptr, x_ptr);

			/* Without the appropriate spoiler flags, ignore the fact that
			 * it is an ego item or artefact. */
			if ((!spoil_ego && j_ptr->name2) || (!spoil_art && j_ptr->name1))
			{
				object_type j2_ptr[1];
				j_ptr->name2 = j_ptr->name1 = 0;
				object_info_known(j2_ptr, j_ptr, 0);
				j_ptr = j2_ptr;
			}

			/* Hack - object_value() needs a real k_idx and tval. */
			j_ptr->k_idx = x_ptr->k_idx;
			j_ptr->tval = x_ptr->tval;

			value = object_value(j_ptr);
			worthless = !value;
			if (worthless && cursed_p(j_ptr))
			{
				j_ptr->ident &= ~(IDENT_CURSED);
				j_ptr->flags3 &= ~(TR3_CURSED | TR3_HEAVY_CURSE);
	 			value = object_value(j_ptr);
			}

			/* Let the player known when a known cursed item is not broken. */
			if (worthless && value)
			{
	 			sprintf(tmp2[0], "(%ld)", value);
			}
			else
			{
	 			sprintf(tmp2[0], "%ld", value);
			}
			k[i++] = tmp2[0];
		}

		/* Hack - this must be <9 character long. See above. */
		if (o1_ptr->discount)
		{
			sprintf(tmp2[1], "%d%% off", o1_ptr->discount);
			k[i++] = tmp2[1];
		}

		if (*((k[i] = find_feeling(o1_ptr))) != '\0') i++;
		if (o1_ptr->note) k[i++] = quark_str(o1_ptr->note);

		if (i && t < tmp_val+ONAME_MAX-4)
		{
			t = object_desc_chr(t, ' ');
			t = object_desc_chr(t, c1);

			/* Append the inscriptions from bottom to top. */
			while (i-- && t < tmp_val+ONAME_MAX-2)
			{ 
				/* This leaves enough space for formatting. */
				sprintf(t, "%.*s", MAX(0, ONAME_MAX-(t-tmp_val)-2), k[i]);
				t = strchr(t, '\0');

				/* Put a comma if there is room for at least one character after it. */
				if (i && t < tmp_val+ONAME_MAX-4) t = object_desc_str(t, ", ");
			}
			t = object_desc_chr(t, c2);
		}
	}

copyback:
	/* Here's where we dump the built string into buf. */
	tmp_val[ONAME_MAX-1] = '\0';
	t = tmp_val;
	while((*(buf++) = *(t++))) ; /* copy the string over */

	TFREE(tmp_val_base);
}


/*
 * Hack -- describe an item currently in a store's inventory
 * This allows an item to *look* like the player is "aware" of it
 */
void object_desc_store(char *buf, object_type *o_ptr, int pref, int mode)
{
	/* Save the "aware" flag */
	bool hack_aware = k_info[o_ptr->k_idx].aware;

	/* Save the "known" flag */
	bool hack_known = (o_ptr->ident & (IDENT_KNOWN)) ? TRUE : FALSE;

	/* Save "spoil_value" */
	bool hack_value = spoil_value;

	/* Save the "plain_descriptions" flag */
	bool old_plain_descriptions = plain_descriptions;

	/* Set the "known" flag */
	o_ptr->ident |= (IDENT_KNOWN);

	/* Force "aware" for description */
	k_info[o_ptr->k_idx].aware = TRUE;

	/* Unset "spoil_value" unless "auto_haggle" is not set. */
	spoil_value &= !(auto_haggle);

	/* Set the "plain_descriptions" flag */
	plain_descriptions = TRUE;

	/* Describe the object */
	object_desc(buf, o_ptr, pref, mode);


	/* Restore "aware" flag */
	k_info[o_ptr->k_idx].aware = hack_aware;

	/* Clear the known flag */
	if (!hack_known) o_ptr->ident &= ~(IDENT_KNOWN);

	/* Restore "spoil_value" */
	spoil_value = hack_value;

	/* Restore the "plain_descriptions" flag */
	plain_descriptions = old_plain_descriptions;

}




/*
 * Determine the "Activation" (if any) for an artifact
 * Return a string, or NULL for "no activation"
 */
cptr item_activation(object_type *o_ptr)
{
	u32b f1, f2, f3;

	/* Extract the flags */
	object_flags(o_ptr, &f1, &f2, &f3);

	/* Require activation ability */
	if (!(f3 & (TR3_ACTIVATE))) return "nothing";

	/* Randarts set xtra1 to denote the use of xtra2 for their activations. */
    if (o_ptr->xtra1 == EGO_XTRA_ACTIVATE)
    {
        switch (o_ptr->xtra2)
        {
            case ACT_SUNLIGHT:
            {
                return "beam of sunlight every 10 turns";
            }
            case ACT_BO_MISS_1:
            {
                return "magic missile (2d6) every 2 turns";
            }
            case ACT_BA_POIS_1:
            {
                return "stinking cloud (12), rad. 3, every 4+d4 turns";
            }
            case ACT_BO_ELEC_1:
            {
                return "lightning bolt (4d8) every 6+d6 turns";
            }
            case ACT_BO_ACID_1:
            {
                return "acid bolt (5d8) every 5+d5 turns";
            }
            case ACT_BO_COLD_1:
            {
                return "frost bolt (6d8) every 7+d7 turns";
            }
            case ACT_BO_FIRE_1:
            {
                return "fire bolt (9d8) every 8+d8 turns";
            }
            case ACT_BA_COLD_1:
            {
                return "ball of cold (48) every 400 turns";
            }
            case ACT_BA_FIRE_1:
            {
                return "ball of fire (72) every 400 turns";
            }
            case ACT_DRAIN_1:
            {
                return "drain life (100) every 100+d100 turns";
            }
            case ACT_BA_COLD_2:
            {
                return "ball of cold (100) every 300 turns";
            }
            case ACT_BA_ELEC_2:
            {
                return "ball of lightning (100) every 500 turns";
            }
            case ACT_DRAIN_2:
            {
                return "drain life (120) every 400 turns";
            }
            case ACT_VAMPIRE_1:
            {
                return "vampiric drain (3*50) every 400 turns";
            }
            case ACT_BO_MISS_2:
            {
                return "arrows (150) every 90+d90 turns";
            }
            case ACT_BA_FIRE_2:
            {
                return "fire ball (120) every 225+d225 turns";
            }
            case ACT_BA_COLD_3:
            {
                return "ball of cold (200) every 325+d325 turns";
            }
            case ACT_WHIRLWIND:
            {
                return "whirlwind attack every 250 turns";
            }
            case ACT_VAMPIRE_2:
            {
                return "vampiric drain (3*100) every 400 turns";
            }
            case ACT_CALL_CHAOS:
            {
                return "call chaos every 350 turns";
            }
            case ACT_SHARD:
            {
                return "shard ball (120+level) every 400 turns";
            }
            case ACT_DISP_EVIL:
            {
                return "dispel evil (level*5) every 300+d300 turns";
            }
            case ACT_DISP_GOOD:
            {
                return "dispel good (level*5) every 300+d300 turns";
            }
            case ACT_BA_MISS_3:
            {
                return "elemental breath (300) every 500 turns";
            }
            case ACT_CONFUSE:
            {
                return "confuse monster every 15 turns";
            }
            case ACT_SLEEP:
            {
                return "sleep nearby monsters every 55 turns";
            }
            case ACT_QUAKE:
            {
                return "earthquake (rad 10) every 50 turns";
            }
            case ACT_TERROR:
            {
                return "terror every 3 * (level+10) turns";
            }
            case ACT_TELE_AWAY:
            {
                return "teleport away every 200 turns";
            }
            case ACT_BANISH_EVIL:
            {
                return "banish evil every 250+d250 turns";
            }
            case ACT_GENOCIDE:
            {
                return "genocide every 500 turns";
            }
            case ACT_MASS_GENO:
            {
                return "mass genocide every 1000 turns";
            }
            case ACT_CHARM_ANIMAL:
            {
                return "charm animal every 300 turns";
            }
            case ACT_CHARM_UNDEAD:
            {
                return "enslave undead every 333 turns";
            }
            case ACT_CHARM_OTHER:
            {
                return "charm monster every 400 turns";
            }
            case ACT_CHARM_ANIMALS:
            {
                return "animal friendship every 500 turns";
            }
            case ACT_CHARM_OTHERS:
            {
                return "mass charm every 750 turns";
            }
            case ACT_SUMMON_ANIMAL:
            {
                return "summon animal every 200+d300 turns";
            }
            case ACT_SUMMON_PHANTOM:
            {
                return "summon phantasmal servant every 200+d200 turns";
            }
            case ACT_SUMMON_ELEMENTAL:
            {
                return "summon elemental every 750 turns";
            }
            case ACT_SUMMON_DEMON:
            {
                return "summon demon every 666+d333 turns";
            }
            case ACT_SUMMON_UNDEAD:
            {
                return "summon undead every 666+d333 turns";
            }
            case ACT_CURE_LW:
            {
                return "remove fear & heal 30 hp every 10 turns";
            }
            case ACT_CURE_MW:
            {
                return "heal 4d8 & wounds every 3+d3 turns";
            }
            case ACT_CURE_POISON:
            {
                return "remove fear and cure poison every 5 turns";
            }
            case ACT_REST_LIFE:
            {
                return "restore life levels every 450 turns";
            }
            case ACT_REST_ALL:
            {
                return "restore stats and life levels every 750 turns";
            }
            case ACT_CURE_700:
            {
                return "heal 700 hit points every 250 turns";
            }
            case ACT_CURE_1000:
            {
                return "heal 1000 hit points every 888 turns";
            }
            case ACT_ESP:
            {
                return "temporary ESP (dur 25+d30) every 200 turns";
            }
            case ACT_BERSERK:
            {
                return "heroism and berserk (dur 50+d50) every 100+d100 turns";
            }
            case ACT_PROT_EVIL:
            {
                return "protect evil (dur level*3 + d25) every 225+d225 turns";
            }
            case ACT_RESIST_ALL:
            {
                return "resist elements (dur 40+d40) every 200 turns";
            }
            case ACT_SPEED:
            {
                return "speed (dur 20+d20) every 250 turns";
            }
            case ACT_XTRA_SPEED:
            {
                return "speed (dur 75+d75) every 200+d200 turns";
            }
            case ACT_WRAITH:
            {
                return "wraith form (level/2 + d(level/2)) every 1000 turns";
            }
            case ACT_INVULN:
            {
                return "invulnerability (dur 8+d8) every 1000 turns";
            }
            case ACT_LIGHT:
            {
                return "light area (dam 2d15) every 10+d10 turns";
            }
            case ACT_MAP_LIGHT:
            {
                return "light (dam 2d15) & map area every 50+d50 turns";
            }
            case ACT_DETECT_ALL:
            {
                return "detection every 55+d55 turns";
            }
            case ACT_DETECT_XTRA:
            {
                return "detection, probing and identify true every 1000 turns";
            }
            case ACT_ID_FULL:
            {
                return "identify true every 750 turns";
            }
            case ACT_ID_PLAIN:
            {
                return "identify spell every 10 turns";
            }
            case ACT_RUNE_EXPLO:
            {
                return "explosive rune every 200 turns";
            }
            case ACT_RUNE_PROT:
            {
                return "rune of protection every 400 turns";
            }
            case ACT_SATIATE:
            {
                return "satisfy hunger every 200 turns";
            }
            case ACT_DEST_DOOR:
            {
                return "destroy doors every 10 turns";
            }
            case ACT_STONE_MUD:
            {
                return "stone to mud every 5 turns";
            }
            case ACT_RECHARGE:
            {
                return "recharging every 70 turns";
            }
            case ACT_ALCHEMY:
            {
                return "alchemy every 500 turns";
            }
            case ACT_DIM_DOOR:
            {
                return "dimension door every 100 turns";
            }
            case ACT_TELEPORT:
            {
                return "teleport (range 100) every 45 turns";
            }
            case ACT_RECALL:
            {
                return "word of recall every 200 turns";
            }
            default:
            {
                return "something undefined";
            }
        }
    }

	/* Some artifacts can be activated */
	switch (o_ptr->name1)
	{
		case ART_FAITH:
		{
			return "fire bolt (9d8) every 8+d8 turns";
		}
		case ART_HOPE:
		{
			return "frost bolt (6d8) every 7+d7 turns";
		}
		case ART_CHARITY:
		{
			return "lightning bolt (4d8) every 6+d6 turns";
		}
		case ART_THOTH:
		{
			return "stinking cloud (12) every 4+d4 turns";
		}
		case ART_ICICLE:
		{
			return "frost ball (48) every 5+d5 turns";
		}
		case ART_DANCING:
		{
			return "remove fear and cure poison every 5 turns";
		}
		case ART_STARLIGHT:
		{
			return "frost ball (100) every 300 turns";
		}
        case ART_DAWN:
        {
            return "summon a Black Reaver every 500+d500 turns";
        }
		case ART_EVERFLAME:
		{
			return "fire ball (72) every 400 turns";
		}
		case ART_FIRESTAR:
		{
			return "large fire ball (72) every 100 turns";
		}
		case ART_ITHAQUA:
		{
			return "haste self (20+d20 turns) every 200 turns";
		}
		case ART_THEODEN:
		{
			return "drain life (120) every 400 turns";
		}
		case ART_JUSTICE:
		{
			return "drain life (90) every 70 turns";
		}
		case ART_OGRELORDS:
		{
			return "door and trap destruction every 10 turns";
		}
		case ART_GHARNE:
		{
			return "word of recall every 200 turns";
		}
		case ART_THUNDER:
		{
			return "haste self (20+d20 turns) every 100+d100 turns";
		}
		case ART_ERIRIL:
		{
			return "identify every 10 turns";
		}
		case ART_ATAL:
		{
            return "probing, detection and full id  every 1000 turns";
		}
		case ART_TROLLS:
		{
			return "mass genocide every 1000 turns";
		}
		case ART_SPLEENSLICER:
		{
			return "cure wounds (4d7) every 3+d3 turns";
		}
		case ART_DEATH:
		{
			return "fire branding of bolts every 999 turns";
		}
        case ART_KARAKAL:
		{
            return "a getaway every 35 turns";
		}
		case ART_ODIN:
		{
            return "lightning ball (100) every 500 turns";
		}
		case ART_DESTINY:
		{
			return "stone to mud every 5 turns";
		}
		case ART_SOULKEEPER:
		{
			return "heal (1000) every 888 turns";
		}
		case ART_VAMPLORD:
		{
            return ("heal (777), curing and heroism every 300 turns");
		}
		case ART_ORCS:
		{
			return "genocide every 500 turns";
		}
		case ART_NYOGTHA:
		{
			return "restore life levels every 450 turns";
		}
		case ART_GNORRI:
		{
			return "teleport away every 150 turns";
		}
		case ART_BARZAI:
		{
			return "resistance (20+d20 turns) every 111 turns";
		}
		case ART_DARKNESS:
		{
			return "Sleep II every 55 turns";
		}
		case ART_SWASHBUCKLER:
		{
			return "recharge item I every 70 turns";
		}
		case ART_SHIFTER:
		{
			return "teleport every 45 turns";
		}
		case ART_TOTILA:
		{
			return "confuse monster every 15 turns";
		}
		case ART_LIGHT:
		{
			return "magic missile (2d6) every 2 turns";
		}
		case ART_IRONFIST:
		{
			return "fire bolt (9d8) every 8+d8 turns";
		}
		case ART_GHOULS:
		{
			return "frost bolt (6d8) every 7+d7 turns";
		}
		case ART_WHITESPARK:
		{
			return "lightning bolt (4d8) every 6+d6 turns";
		}
		case ART_DEAD:
		{
			return "acid bolt (5d8) every 5+d5 turns";
		}
		case ART_COMBAT:
		{
			return "a magical arrow (150) every 90+d90 turns";
		}
		case ART_SKULLKEEPER:
		{
			return "detection every 55+d55 turns";
		}
		case ART_SUN:
		{
            return "heal (700) every 250 turns";
		}
		case ART_RAZORBACK:
		{
			return "star ball (150) every 1000 turns";
		}
		case ART_BLADETURNER:
		{
            return "breathe elements (300), berserk rage, bless, and resistance";
		}
		case ART_POLARIS:
		{
			return "illumination every 10+d10 turns";
		}
		case ART_XOTH:
		{
            return "magic mapping and light every 50+d50 turns";
		}
		case ART_TRAPEZOHEDRON:
		{
            return "clairvoyance and recall, draining you";
		}
		case ART_ALHAZRED:
		{
			return "dispel evil (x5) every 300+d300 turns";
		}
		case ART_LOBON:
		{
			return "protection from evil every 225+d225 turns";
		}
        case ART_MAGIC:
        {
            return "a strangling attack (100) every 100+d100 turns";
        }
		case ART_BAST:
		{
			return "haste self (75+d75 turns) every 150+d150 turns";
		}
		case ART_ELEMFIRE:
		{
			return "large fire ball (120) every 225+d225 turns";
		}
		case ART_ELEMICE:
		{
			return "large frost ball (200) every 325+d325 turns";
		}
		case ART_ELEMSTORM:
		{
			return "large lightning ball (250) every 425+d425 turns";
		}
		case ART_NYARLATHOTEP:
		{
			return "bizarre things every 450+d450 turns";
		}
        case ART_POWER: case ART_MASK:
        {
            return "rays of fear in every direction";
        }
	}


    if (o_ptr->name2 == EGO_PLANAR)
    {
       return "teleport every 50+d50 turns";
    }

    if (o_ptr->tval == TV_RING)
    {
        switch(o_ptr->sval)
        {
            case SV_RING_FLAMES:
                return "ball of fire and resist fire";
            case SV_RING_ICE:
                return "ball of cold and resist cold";
            case SV_RING_ACID:
                return "ball of acid and resist acid";
            default:
                return "a bad ring activation";
        }
    }

	/* Require dragon scale mail */
	if (o_ptr->tval != TV_DRAG_ARMOR) return "a bad miscellaneous activation";

	/* Branch on the sub-type */
	switch (o_ptr->sval)
	{
		case SV_DRAGON_BLUE:
		{
			return "breathe lightning (100) every 450+d450 turns";
		}
		case SV_DRAGON_WHITE:
		{
			return "breathe frost (110) every 450+d450 turns";
		}
		case SV_DRAGON_BLACK:
		{
			return "breathe acid (130) every 450+d450 turns";
		}
		case SV_DRAGON_GREEN:
		{
			return "breathe poison gas (150) every 450+d450 turns";
		}
		case SV_DRAGON_RED:
		{
			return "breathe fire (200) every 450+d450 turns";
		}
		case SV_DRAGON_MULTIHUED:
		{
			return "breathe multi-hued (250) every 225+d225 turns";
		}
		case SV_DRAGON_BRONZE:
		{
			return "breathe confusion (120) every 450+d450 turns";
		}
		case SV_DRAGON_GOLD:
		{
			return "breathe sound (130) every 450+d450 turns";
		}
		case SV_DRAGON_CHAOS:
		{
			return "breathe chaos/disenchant (220) every 300+d300 turns";
		}
		case SV_DRAGON_LAW:
		{
			return "breathe sound/shards (230) every 300+d300 turns";
		}
		case SV_DRAGON_BALANCE:
		{
			return "You breathe balance (250) every 300+d300 turns";
		}
		case SV_DRAGON_SHINING:
		{
			return "breathe light/darkness (200) every 300+d300 turns";
		}
		case SV_DRAGON_POWER:
		{
			return "breathe the elements (300) every 300+d300 turns";
		}
	}


	/* Oops */
	return "a bad dragon scale mail activation";
}


/*
 * Allocate and return a string listing a set of flags in a specified format.
 * This should never be called with parameters longer than 1024 characters.
 */
static cptr list_flags(cptr init, cptr conj, cptr *flags, int total)
{
	int i;
	char *s, *t;

	/* Paranoia. */
	if (!init || !conj) return "";

	s = format("%s ", init);
	t = strchr(s, '\0');
	if (total > 2)
	{
		for (i = 0; i < total-2; i++)
		{
			sprintf(t, "%s, ", flags[i]);
			t += strlen(t);
		}
	}
	if (total > 1)
	{
		sprintf(t, "%s %s ", flags[total-2], conj);
		t += strlen(t);
	}
	if (total)
	{
		sprintf(t, "%s.", flags[total-1]);
	}
	else
	{
		strcpy(t, "nothing.");
	}
	return s;
}

/* The maximum number of strings allowed for identify_fully_aux().
 * This should always be greater than the greatest possible number, as one is
 * currently used for termination. */
#define MAX_IFA	128

/* Information passed from identify_fully_get() to the output functions. */
typedef struct ifa_type ifa_type;

struct ifa_type
{
	cptr txt;
	bool alloc;
	byte attr;
};

/*
 * Set i_ptr->txt as an allocated string, remember the fact.
 */
static void alloc_ifa(ifa_type *i_ptr, cptr str)
{
	i_ptr->alloc = TRUE;
	i_ptr->txt = string_make(str);
}

/* Shorthand notation for res_stat_details() */

/* Flags for an item which affects all stats (for convenience). */
#define A_ALL ((1<<A_STR)+(1<<A_INT)+(1<<A_WIS)+(1<<A_DEX)+(1<<A_CON)+(1<<A_CHR))

/* A basic test */
#define test(x,y) (x[stat_res[y]] - x[p_ptr->stat_ind[y]])

/* The special test required to calculate blows. */
#define blows(x) (blows_table[MIN(adj_str_blow[x[A_STR]]*mul/div,11)][MIN(adj_dex_blow[x[A_DEX]], 11)])

/* Save the string to the info array, and note that it needs to be freed. */
#define descr(str) {info[*i].attr = attr; alloc_ifa(info+((*i)++), str);}

/* A format which does a basic test, and saves the output in a string as above. */
#define dotest(x,y,str) if ((dif = test(x,y))) descr(str)

#define DIF ABS(dif)
#define DIF_INC ((dif > 0) ? "increases" : "decreases")
#define DIF_DEC ((dif < 0) ? "increases" : "decreases")
#define DIF_MOR ((dif > 0) ? "more" : "less")
#define DIF_LES ((dif < 0) ? "more" : "less")

#define CERT ((act & (A_INCREASE | A_DEC10 | A_DEC25)) ? "at least " : "")

#define A_RESTORE	0x01
#define A_INCREASE	0x02
#define A_INCRES	(A_INCREASE | A_RESTORE)
#define A_DEC10	0x04	/* A small temporary decrease, as do_dec_stat(). */
#define A_DEC25	0x08	/* A large permanent decrease, as a potion of ruination. */

/* Just in case birthrand.diff isn't applied. */
#ifndef A_MAX
#define A_MAX 6
#endif

#define CMPU(X) (pn_ptr->X - po_ptr->X)
#define CMP(X) ((dif = CMPU(X)))
#define CMPUJ(X) CMPU(X[j])
#define CMPJ(X) ((dif = CMPUJ(X)))
#define CMPS(X) ((dif = X[pn_ptr->stat_ind[j]] - X[po_ptr->stat_ind[j]]))
#define CMPT(X,Y) ((dif = X[pn_ptr->stat_ind[Y]] - X[po_ptr->stat_ind[Y]]))

/*
 * Actually describe a stat modifying item.
 * As the modifications have taken place, this just queries the old and new
 * versions of p_ptr to see what has changed.
 */
static void res_stat_details_comp(player_type *pn_ptr, player_type *po_ptr, int *i, ifa_type *info, byte act)
{
	int j,dif;
	byte attr;
	cptr stats[6] = {"strength", "intelligence", "wisdom", "dexterity", "constitution", "charisma"};

	for (j = 0; j < A_MAX; j++)
	{
		/* Give the modifier in white. */
		attr = TERM_WHITE;

		if (CMPJ(stat_max) > 0)
		{
			descr(format("It adds %s%d to your %s.", CERT, dif, stats[j]));
		}
		else if (dif)
		{
			descr(format("It removes %s%d from your %s.", CERT, -dif, stats[j]));
		}
		else if (CMPJ(stat_cur))
		{
			descr(format("It restores your %s.", stats[j]));
		}
		else if (CMPJ(stat_add) > 0)
		{
			descr(format("It adds %d to your %s.", dif, stats[j]));
		}
		else if (dif)
		{
			descr(format("It removes %d from your %s.", -dif, stats[j]));
		}

		/* No effect, so boring. */
		if (!CMPJ(stat_use)) continue;

		/* Give its effects in grey. */
		attr = TERM_L_WHITE;

		switch (j)
		{
			case A_CHR:
			if (CMPS(adj_mag_study)) descr(format("  It causes you to annoy spirits %s.", DIF_LES));
			if (CMPS(adj_mag_fail)) descr(format("  It %s your maximum spiritual success rate by %d%%.", DIF_DEC, DIF));
			if (CMPS(adj_mag_stat)) descr(format("  It %s your spiritual success rates.", DIF_INC));
			if (CMPS(adj_chr_gold)) descr(format("  It %s your bargaining power.", DIF_DEC));
			break;
			case A_WIS:
			if (CMPS(adj_mag_mana)) descr(format("  It gives you %d %s chi at 100%% skill (%d now).", DIF*25, DIF_MOR, CMPU(mchi)));
			if (CMPS(adj_mag_fail)) descr(format("  It %s your maximum mindcraft success rate by %d%%.", DIF_DEC, DIF));
			if (CMPS(adj_mag_stat)) descr(format("  It %s your mindcraft success rates.", DIF_INC));
			if (CMPS(adj_wis_sav)) descr(format("  It %s your saving throw by %d%%.", DIF_INC, DIF));
			break;
			case A_INT: /* Rubbish in the case of icky gloves or heavy armour. */
			if (CMPS(adj_mag_study)) descr(format("  It allows you to learn %d %s spells at 100%% skill.", DIF*25, DIF_MOR));
			if (CMPS(adj_mag_mana)) descr(format("  It gives you %d %s mana at 100%% skill (%d now).", DIF*25, DIF_MOR, CMPU(msp)));
			if (CMPS(adj_mag_fail)) descr(format("  It %s your maximum spellcasting success rate by %d%%.", DIF_DEC, DIF));
			if (CMPS(adj_mag_stat)) descr(format("  It %s your spellcasting success rates.", DIF_INC));
			if (CMP(skill_dev)) descr(format("  It %s your success rate with magical devices.", DIF_INC));
			break;
			case A_CON:
			if (CMPS(adj_con_fix)) descr(format("  It %s your regeneration rate.", DIF_INC));
			if (CMPS(adj_con_mhp)) descr(format("  It gives you %d %s hit points at 100%% skill (%d now).", DIF*25, DIF_MOR, CMPU(mhp)));
			break;
			case A_DEX:
			if (CMP(dis_to_a)) descr(format("  It %s your AC by %d.", DIF_INC, DIF));
			if (CMP(dis_to_h)) descr(format("  It %s your chance to hit opponents by %d.", DIF_INC, DIF));
			/* Fix me - this also covers stunning, but is affected by saving throw. */
			if (CMPS(adj_dex_safe)) descr(format("  It makes you %d%% %s resistant to theft.", DIF, DIF_MOR));
			break;
			case A_STR:
			if (CMP(dis_to_d)) descr(format("  It %s your ability to damage opponents by %d.", DIF_INC, DIF));
			if (CMPS(adj_str_wgt)) descr(format("  It %s your maximum carrying capacity by %d.", DIF_INC, DIF));
			if (CMPS(adj_str_hold)) descr(format("  It makes you %s able to use heavy weapons.", DIF_MOR));
			if (CMP(skill_dig)) descr(format("  It allows you to dig %s effectively.", DIF_MOR));
		}

		/* A couple of things which depend on two stats. */
		if (j == A_DEX || (j == A_STR && !CMPU(stat_ind[A_DEX])))
		{
			if (CMP(num_blow)) descr(format("  It %s your number of blows by %d,%d", DIF_INC, DIF/60, DIF%60));
		}
		if (j == A_DEX || (j == A_INT && !CMPU(stat_ind[A_DEX])))
		{
			int dif2;
			if ((dif2 = CMPS(adj_dex_dis)+CMPT(adj_int_dis, A_INT)))
			{
				dif = dif2;
				descr(format("  It %s your disarming skill.", DIF_INC));
			}
		}
	}
}

/*
 * Increase a stat by the minimum amount possible.
 */
static void change_stat_min(int stat, int act)
{
	int i;
	for (i = 0; i < A_MAX; i++)
	{
		/* Doesn't affect this stat. */
		if (~stat & 1<<i) continue;
		
		/* Restores this stat. */
		if (act & A_RESTORE) (void)res_stat(i);

		/* Affect this stat (as inc_stat(), but minimal). */
		if ((act & A_INCREASE) && (p_ptr->stat_cur[i] < 118))
		{
			int gain, value = p_ptr->stat_cur[i];
			if (value < 18 || value > 115) gain = 1;
			else gain = ((118-value)/2+3)/2+1;
			p_ptr->stat_cur[i] += gain;
			p_ptr->stat_max[i] += gain;
		}
		
		/* A temporary loss of 10 */
		if ((act & A_DEC10) && (p_ptr->stat_cur[i] > 3))
		{
			int loss, cur = p_ptr->stat_cur[i];
			if (cur < 19) loss = 1;

			else loss =
#if 0 /* It's always 5, so don't waste time calculating it. */
			MAX((((cur-18)/2+1)/2+2)*10/100, 10/2);
#else
			5;
#endif
			if (cur > 18 && cur-loss <= 18)
				p_ptr->stat_cur[i] = 18;
			else
				p_ptr->stat_cur[i] -= loss;
		}
		/* A permanent loss of 25 */
		if ((act & A_DEC25) && (p_ptr->stat_max[i] > 3))
		{
			int loss, cur = p_ptr->stat_max[i];
			if (cur < 19) loss = 1;
			else loss =
#if 0 /* It's always 10, so don't waste time calculating it. */
			MAX((((cur-18)/2+1)/2+2)*25/100, 25/2);
#else
			12;
#endif
			if (p_ptr->stat_cur[i] > 18 && p_ptr->stat_cur[i]-loss <= 18)
				p_ptr->stat_cur[i] = 18;
			else
				p_ptr->stat_cur[i] = MAX(p_ptr->stat_cur[i]-loss, 3);

			if (p_ptr->stat_max[i] > 18 && p_ptr->stat_max[i]-loss <= 18)
				p_ptr->stat_max[i] = 18;
			else
				p_ptr->stat_max[i] -= loss;
		}
	}
}

static void get_stat_flags(object_type *o_ptr, byte *stat, byte *act, s16b *pval)
{
	int j;
	struct convtype {
	byte tval;
	byte sval;
	byte act;
	byte stat;
	} conv[] = {
		/* Increase stat potions */
		{TV_POTION, SV_POTION_INC_STR, A_INCRES, 1<<A_STR},
		{TV_POTION, SV_POTION_INC_INT, A_INCRES, 1<<A_INT},
		{TV_POTION, SV_POTION_INC_WIS, A_INCRES, 1<<A_WIS},
		{TV_POTION, SV_POTION_INC_DEX, A_INCRES, 1<<A_DEX},
		{TV_POTION, SV_POTION_INC_CON, A_INCRES, 1<<A_CON},
		{TV_POTION, SV_POTION_INC_CHR, A_INCRES, 1<<A_CHR},
		{TV_POTION, SV_POTION_STAR_ENLIGHTENMENT, A_INCRES, (1<<A_INT | 1<<A_WIS)},
		{TV_POTION, SV_POTION_AUGMENTATION, A_INCRES, A_ALL},
		/* Restore stat potions */
		{TV_POTION, SV_POTION_RES_STR, A_RESTORE, 1<<A_STR},
		{TV_POTION, SV_POTION_RES_INT, A_RESTORE, 1<<A_INT},
		{TV_POTION, SV_POTION_RES_WIS, A_RESTORE, 1<<A_WIS},
		{TV_POTION, SV_POTION_RES_DEX, A_RESTORE, 1<<A_DEX},
		{TV_POTION, SV_POTION_RES_CON, A_RESTORE, 1<<A_CON},
		{TV_POTION, SV_POTION_RES_CHR, A_RESTORE, 1<<A_CHR},
		{TV_POTION, SV_POTION_LIFE, A_RESTORE, A_ALL},
		/* Restore stat mushrooms */
		{TV_FOOD, SV_FOOD_RESTORE_STR, A_RESTORE, 1<<A_STR},
		{TV_FOOD, SV_FOOD_RESTORE_CON, A_RESTORE, 1<<A_CON},
		{TV_FOOD, SV_FOOD_RESTORING, A_RESTORE, A_ALL},
		/* Other restore stat items */
		{TV_STAFF, SV_STAFF_THE_MAGI, A_RESTORE, 1<<A_INT},
		{TV_ROD, SV_ROD_RESTORATION, A_RESTORE, A_ALL},
		/* Decrease stat potions */
		{TV_POTION, SV_POTION_DEC_STR, A_DEC10, 1<<A_STR},
		{TV_POTION, SV_POTION_DEC_INT, A_DEC10, 1<<A_INT},
		{TV_POTION, SV_POTION_DEC_WIS, A_DEC10, 1<<A_WIS},
		{TV_POTION, SV_POTION_DEC_DEX, A_DEC10, 1<<A_DEX},
		{TV_POTION, SV_POTION_DEC_CON, A_DEC10, 1<<A_CON},
		{TV_POTION, SV_POTION_DEC_CHR, A_DEC10, 1<<A_CHR},
		{TV_POTION, SV_POTION_RUINATION, A_DEC25, A_ALL},
		/* Decrease stat food */
		{TV_FOOD, SV_FOOD_WEAKNESS, A_DEC10, A_STR},
		{TV_FOOD, SV_FOOD_SICKNESS, A_DEC10, A_CON},
		{TV_FOOD, SV_FOOD_STUPIDITY, A_DEC10, A_INT},
		{TV_FOOD, SV_FOOD_NAIVETY, A_DEC10, A_WIS},
		{TV_FOOD, SV_FOOD_UNHEALTH, A_DEC10, A_CON},
		{TV_FOOD, SV_FOOD_DISEASE, A_DEC10, A_STR},
		{0,0,0,0},
	};

	*stat = 0;

	/* Start with the permanent modifiers items in */
	if (o_ptr->flags1 & TR1_STR) *stat |= 1<<A_STR;
	if (o_ptr->flags1 & TR1_INT) *stat |= 1<<A_INT;
	if (o_ptr->flags1 & TR1_WIS) *stat |= 1<<A_WIS;
	if (o_ptr->flags1 & TR1_DEX) *stat |= 1<<A_DEX;
	if (o_ptr->flags1 & TR1_CON) *stat |= 1<<A_CON;
	if (o_ptr->flags1 & TR1_CHR) *stat |= 1<<A_CHR;

	/* If one of these flags exist, the modifier refers to it. */
	if (*stat) *pval = o_ptr->pval;
	else *pval = 0;

	/* Look for stat-affecting potions, food, etc..*/
	for (j = 0, *act = 0; conv[j].tval; j++)
	{
		if (conv[j].tval == o_ptr->tval && conv[j].sval == o_ptr->sval)
		{
			*stat |= conv[j].stat;
			*act |= conv[j].act;
		}
	}
}

/*
 * Find out what, if any, stat changes the given item causes.
 */
static void res_stat_details(object_type *o_ptr, object_extra *x_ptr, int *i, ifa_type *info)
{
	byte stat, act;
	s16b pval;
	object_type j_ptr[1];
	player_type p2_ptr[1];

	/* Find out the stat flags and pval (if relevant). */
	get_stat_flags(o_ptr, &stat, &act, &pval);

	/* Boring */
	if (!stat) return;

	/* As update_stuff() can cause potentially irreversible changes,
	* use a copy instead. */

	COPY(p2_ptr, p_ptr, player_type);
	p_ptr = p2_ptr;

	/*
	 * Carry out a pessimistic version of any restore/increase the item
	 * gives.
	 */
	if (act & (A_RESTORE | A_INCREASE)) change_stat_min(stat, act);

	/* An item that gives no effects for being worn. */
	if (!pval)
	{
		/* Correct bonuses quietly. */
		p_ptr->update = PU_BONUS | PU_QUIET;
		update_stuff();

		/* Compare the original with the same with this object wielded. */
		res_stat_details_comp(p2_ptr, &p_body, i, info, act);
	}
	/* A worn item. */
	else if (!o_ptr->iy && o_ptr->ix > INVEN_WIELD && o_ptr->ix <= INVEN_TOTAL)
	{
		/* Hack - use the original o_ptr. */
		o_ptr = inventory+o_ptr->ix-1;

		/* Put o_ptr somewhere safe. */
		object_copy(j_ptr, o_ptr);
		object_wipe(o_ptr);

		/* Correct bonuses quietly. */
		p_ptr->update = PU_BONUS | PU_QUIET;
		update_stuff();

		/* Compare the original with the same with the item removed. */
		res_stat_details_comp(&p_body, p2_ptr, i, info, act);

		/* Replace o_ptr. */
		object_copy(o_ptr, j_ptr);
	}
	/* A wearable item. */
	else
	{
		int slot = wield_slot(o_ptr);

		/* Put the currently worn item somewhere safe. */
		object_copy(j_ptr, inventory+slot);

		/* Hack - the player should know how to wear everything. */
		if (slot == -1)
		{
			object_type tmp;
			object_prep(&tmp, x_ptr->k_idx);
			slot = wield_slot(&tmp);

			/* Paranoia - not a wearable item? */
			if (slot == -1)
			{
				p_ptr = &p_body;
				return;
			}
		}

		/* Put o_ptr in its place. */
		object_copy(inventory+slot, o_ptr);

		/* Correct bonuses quietly. */
		p_ptr->update = PU_BONUS | PU_QUIET;
		update_stuff();

		/* Compare the original with the same with this object wielded. */
		res_stat_details_comp(p2_ptr, &p_body, i, info, act);

			/* Replace inveotory+slot */
			object_copy(inventory+slot, j_ptr);
	}

	/* Return p_ptr to normal. */
	p_ptr = &p_body;
}


/* Offset for a continued string, -1 as these start with a space anyway. */
#define IFD_OFFSET	3

/*
 * Find the number of characters in in before the last space before the
 * maxth character.
 */
static int wrap_str(cptr in, int max)
{
	int len = strlen(in);
	cptr t;
	
	/* Short enough anyway. */
	if (len < max) return len;
	
	/* Find the last space before (or including) the max character. */
	for (t = in+max-1; *t != ' '; t--);

	/* Return the offset. */
	return (int)(t-in);
}

/*
 * Show the flags an object has in an interactive way.
 */
static void identify_fully_show(ifa_type *i_ptr)
{
	int k, len, minx = 15, xlen = Term->wid - minx+1, maxy = Term->hgt;
	cptr s = "";

	/* Save the screen */
	Term_save();

	/* Erase the screen */
	for (k = 1; k < maxy; k++) prt("", k, (minx > 2) ? minx-2 : 0);

	/* Label the information */
	prt("     Item Attributes:", 1, minx);

	/* We will print on top of the map. */
	for (k = 2;;)
	{
		/* Hack - turn the default colour (black) into white. */
		byte attr = (i_ptr->attr) ? i_ptr->attr : TERM_WHITE;
		
		/* Grab the next string. */
		if (*s == '\0')
		{
			s = (i_ptr++)->txt;

			/* Finished. */
			if (!s) break;
		}

		/* Show the info */

		/* A space means that this is a continuation, so give an offset. */
		if (*s == ' ')
		{
			len = wrap_str(s, xlen-IFD_OFFSET);
			c_prt(attr, format("%.*s", len, s), k++, minx+IFD_OFFSET);
		}
		/* If not, don't. */
		else
		{
			len = wrap_str(s, xlen);
			c_prt(attr, format("%.*s", len, s), k++, minx);
		}

		/* Find the next segment. */
		s += len;

		/* Every 20 entries (lines 2 to 21), start over */
		if ((k == maxy-1) && (i_ptr->txt))
		{
			prt("-- more --", k, minx);
			inkey();
			for (; k > 2; k--) prt("", k, minx);
		}
	}

	/* Wait for it */
	prt("[Press any key to continue]", k, minx);
	inkey();

	/* Restore the screen */
	Term_load();
}


/*
 * Show the first maxy-1 lines of the information stored in info.
 */
static void identify_fully_dump(ifa_type *i_ptr)
{
	int j,k, len, minx = 0, xlen = Term->wid - minx+1, maxy = Term->hgt;
	cptr s = "";

	/* Label the information */
	prt("     Item Attributes:", 1, minx);

	for (k = 2, j = 0; k <= maxy; k++)
	{
		/* Hack - turn the default colour (black) into white. */
		byte attr = (i_ptr->attr) ? i_ptr->attr : TERM_WHITE;
		
		/* Grab the next string. */
		if (*s == '\0')
		{
			s = (i_ptr++)->txt;

			/* Finished. */
			if (!s) break;
		}

		/* Show the info */

		/* A space means that this is a continuation, so give an offset. */
		if (*s == ' ')
		{
			len = wrap_str(s, xlen-IFD_OFFSET);
			c_prt(attr, format("%.*s", len, s), k, minx+IFD_OFFSET);
		}
		/* If not, don't. */
		else
		{
			len = wrap_str(s, xlen);
			c_prt(attr, format("%.*s", len, s), k, minx);
		}
		
		/* Find the next segment. */
		s += len;
	}
}

#define IFDF_OFFSET 5

/*
 * Dump the information stored in info to a specified file.
 */
static void identify_fully_dump_file(FILE *fff, ifa_type *i_ptr)
{
	int j,k, len, x, xlen = 80;
	cptr s = "";

	for (k = 2, j = 0;;)
	{
		/* Grab the next string. */
		if (*s == '\0')
		{
			s = (i_ptr++)->txt;
			if (!s) break;
		}

		/* Show the info */

		/* Standard description offset. */
		x = IFDF_OFFSET;

		/* Give an extra offset for a continuation. */
		if (*s == ' ') x += IFD_OFFSET;

		len = wrap_str(s, xlen-x);
		fprintf(fff, "%*s%.*s\n", x, "", len, s);
		
		/* Find the next segment. */
		s += len;
	}
}


/*
 * Clear any allocated strings in info[].
 */
static void identify_fully_clear(ifa_type *i_ptr)
{
	do
	{
		if (i_ptr->alloc) string_free(i_ptr->txt);
	}
	while ((++i_ptr)->txt);
}

/* Set brief to suppress various strings in identify_fully_get(). */
static bool brief = FALSE;

/*
 * Find the sval of a launcher which can fire a given missile.
 */
static int launcher_type(object_type *o_ptr)
{
	tval_ammo_type *tv_ptr;

	for (tv_ptr = tval_ammo; tv_ptr->bow_sval; tv_ptr++)
	{
		/* Found something */
		if (tv_ptr->ammo_tval == o_ptr->tval) return tv_ptr->bow_sval;
	}

	/* Nothing */
	return (-1);
}

/* A wrapper around list_flags() for identify_fully_get(), provided for
 * clarity and maintainability. */
#define do_list_flags(init, conj, flags, total) \
	alloc_ifa(info+i++, list_flags(init, conj, flags, total))

/*
 * Find the strings which describe the flags of o_ptr, place them in info
 * and note whether each was allocated.
 *
 * Return the total number of strings.
 */
static void identify_fully_get(object_type *o1_ptr, ifa_type *info)
{
	int                     i = 0, j;

	cptr board[16];

	object_type o_ptr[1];
	object_extra x_ptr[1];

	/* Paranoia - no object. */
	if (!o1_ptr || !o1_ptr->k_idx) return;

	/* Extract the known info */
	object_info_known(o_ptr, o1_ptr, x_ptr);

	/* Mega-Hack -- describe activation */
	if (o_ptr->flags3 & (TR3_ACTIVATE))
	{
		info[i++].txt = "It can be activated for...";
		info[i++].txt = item_activation(o_ptr);
		info[i++].txt = "...if it is being worn.";
	}

	/* Describe use of the base object, if any. */
	if (spoil_base && k_info[o_ptr->k_idx].text)
	{
		info[i++].txt = k_text+k_info[o_ptr->k_idx].text;
	}

	/* Hack -- describe lite's */
	if (o_ptr->tval == TV_LITE)
	{
		if (allart_p(o_ptr))
		{
			info[i++].txt = "It provides light (radius 3) forever.";
		}
		else if (o_ptr->sval == SV_LITE_LANTERN)
		{
			info[i++].txt = "It provides light (radius 2) when fueled.";
		}
		else
		{
			info[i++].txt = "It provides light (radius 1) when fueled.";
		}
	}

	/* Hack - describe the wield skill of weaponry. */
	switch (wield_skill(o_ptr->tval, o_ptr->sval))
	{
 	case SKILL_CLOSE:
 		info[i++].txt="It trains your close combat skill.";
 				break;
	case SKILL_CRUSH:
 		info[i++].txt="It trains your crushing weapons skill.";
 				break;
 	case SKILL_STAB:
 		info[i++].txt="It trains your stabbing weapons skill.";
 		break;
	case SKILL_SLASH:
 		info[i++].txt="It trains your slashing weapons skill.";
 			break;
	case  SKILL_MISSILE:
 		info[i++].txt="It trains your missile skill.";
 		break;
 	}


	/* And then describe it fully */

	j = 0;
	
	/* Recognise items which affect stats (detailed) */
	if (!brief && spoil_stat)
	{
		res_stat_details(o_ptr, x_ptr, &i, info);
	}
	/* If brevity is required or spoilers are not, put stats with the other
	 * pval effects. */
	else
	{
		if (o_ptr->flags1 & (TR1_STR)) board[j++] = "strength";
		if (o_ptr->flags1 & (TR1_INT)) board[j++] = "intelligence";
		if (o_ptr->flags1 & (TR1_WIS)) board[j++] = "wisdom";
		if (o_ptr->flags1 & (TR1_DEX)) board[j++] = "dexterity";
		if (o_ptr->flags1 & (TR1_CON)) board[j++] = "constitution";
		if (o_ptr->flags1 & (TR1_CHR)) board[j++] = "charisma";
	}
	if (o_ptr->flags1 & (TR1_STEALTH)) board[j++] = "stealth";
	if (o_ptr->flags1 & (TR1_SEARCH)) board[j++] = "searching";
	if (o_ptr->flags1 & (TR1_INFRA)) board[j++] = "infravision";
	if (o_ptr->flags1 & (TR1_TUNNEL)) board[j++] = "ability to tunnel";
	if (o_ptr->flags1 & (TR1_SPEED)) board[j++] = "movement speed";
	if (o_ptr->flags1 & (TR1_BLOWS)) board[j++] = "attack speed";
	if (j)
	{
		if (o_ptr->pval > 0)
		{
			board[j] = format("It adds %d to your", o_ptr->pval);
		}
		else if (o_ptr->pval < 0)
		{
			board[j] = format("It removes %d from your", -o_ptr->pval);
		}
		else
		{
			board[j] = "It affects your";
		}
		do_list_flags(board[j], "and", board, j);
	}


    if (o_ptr->flags1 & (TR1_CHAOTIC))
	{
        info[i++].txt = "It produces chaotic effects.";
	}

    if (o_ptr->flags1 & (TR1_VAMPIRIC))
	{
        info[i++].txt = "It drains life from your foes.";
	}

    if (o_ptr->flags1 & (TR1_IMPACT))
	{
		info[i++].txt = "It can cause earthquakes.";
	}

    if (o_ptr->flags1 & (TR1_VORPAL))
	{
        info[i++].txt = "It is very sharp and can cut your foes.";
	}

	/* Calculate actual damage of weapons. 
	 * This only considers slays and brands at the moment. */
	if (spoil_dam)
	{
		s16b tohit, todam, weap_blow, mut_blow;
		s32b dam;
		bool slay = FALSE;
		/* Calculate x15 slays. */
		j = 0;
		if ((o_ptr->flags1 & TR1_ALL_SLAY_DRAGON) == TR1_X15_DRAGON)
			board[j++]  = "dragons";
		if (j)
		{
			weapon_stats(o_ptr, 15, &tohit, &todam, &weap_blow, &mut_blow, &dam);
			slay = TRUE;
			do_list_flags(format("It causes %d,%d damage to", dam/60, dam%60),
				"and", board, j);
		}

		/* Calculate x5 slays. */
		j = 0;
		if ((o_ptr->flags1 & TR1_ALL_SLAY_DRAGON) == TR1_KILL_DRAGON)
			board[j++] = "dragons";

		if (j)
		{
			weapon_stats(o_ptr, 5, &tohit, &todam, &weap_blow, &mut_blow, &dam);
			slay = TRUE;
			do_list_flags(format("It causes %d,%d damage to", dam/60, dam%60),
				"and", board, j);
		}

		/* Calculate x3 slays */
		j = 0;
		if ((o_ptr->flags1 & TR1_ALL_SLAY_DRAGON) == TR1_SLAY_DRAGON)
			board[j++] = "dragons";
		if (o_ptr->flags1 & TR1_SLAY_ORC) board[j++] = "orcs";
		if (o_ptr->flags1 & TR1_SLAY_TROLL) board[j++] = "trolls";
		if (o_ptr->flags1 & TR1_SLAY_GIANT) board[j++] = "giants";
		if (o_ptr->flags1 & TR1_SLAY_DEMON) board[j++] = "demons";
		if (o_ptr->flags1 & TR1_SLAY_UNDEAD) board[j++] = "undead";
		if (j)
		{
			weapon_stats(o_ptr, 3, &tohit, &todam, &weap_blow, &mut_blow, &dam);
			slay = TRUE;
			do_list_flags(format("It causes %d,%d damage to", dam/60, dam%60),
				"and", board, j);
		}
		/* Calculate brands */
		j = 0;
		if (o_ptr->flags1 & TR1_BRAND_FIRE) board[j++] = "fire";
		if (o_ptr->flags1 & TR1_BRAND_COLD) board[j++] = "cold";
		if (o_ptr->flags1 & TR1_BRAND_ELEC) board[j++] = "electricity";
		if (o_ptr->flags1 & TR1_BRAND_ACID) board[j++] = "acid";
		if (o_ptr->flags1 & TR1_BRAND_POIS) board[j++] = "poison";
		if (j)
		{
			weapon_stats(o_ptr, 3, &tohit, &todam, &weap_blow, &mut_blow, &dam);
			slay = TRUE;
			do_list_flags(format("It causes %d,%d damage via", dam/60, dam%60),
				"and", board, j);
		}
		/* Calculate x2 slays */
		j = 0;
		if (o_ptr->flags1 & TR1_SLAY_EVIL) board[j++] = "evil monsters";
		if (o_ptr->flags1 & TR1_SLAY_ANIMAL) board[j++] = "animals";
		if (j)
		{
			weapon_stats(o_ptr, 2, &tohit, &todam, &weap_blow, &mut_blow, &dam);
			slay = TRUE;
			do_list_flags(format("It causes %d,%d damage to", dam/60, dam%60),
				"and", board, j);
		}

		/* Give the damage a weapon does, excluding throwing weapons in brief mode. */
		if (!brief || (wield_slot(o_ptr) == INVEN_WIELD) ||
			(wield_slot(o_ptr) == INVEN_BOW) || (launcher_type(o_ptr) != -1))
		{
			weapon_stats(o_ptr, 1, &tohit, &todam, &weap_blow, &mut_blow, &dam);
			j = 0;
			if (slay) board[j++] = "all other monsters";
			else if (dam) board[j++] = "all monsters";
			if (j)
			{
				do_list_flags(format("It causes %d,%d damage to", dam/60,
					dam%60), "and", board, j);
			}
		}

		/* Describe blows per turn. */
		switch (wield_slot(o_ptr))
		{
			case INVEN_WIELD: *board = "blows"; break;
			case INVEN_BOW: *board = "shots"; break;
			default: *board = 0;
		}
		if (*board)
		{
			alloc_ifa(info+i++, format("It gives you %d,%d %s per turn",
				weap_blow/60, weap_blow%60, *board));
		}
	}
	/* Without spoil_dam, simply list the slays. */
	else
	{
		/* Calculate brands */
		j = 0;
		if (o_ptr->flags1 & TR1_BRAND_FIRE) board[j++] = "fire";
		if (o_ptr->flags1 & TR1_BRAND_COLD) board[j++] = "cold";
		if (o_ptr->flags1 & TR1_BRAND_ELEC) board[j++] = "electricity";
		if (o_ptr->flags1 & TR1_BRAND_ACID) board[j++] = "acid";
		if (o_ptr->flags1 & TR1_BRAND_POIS) board[j++] = "poison";
		if (j)
		{
			do_list_flags("It causes extra damage via", "and", board, j);
		}
		
		j = 0;
		if ((o_ptr->flags1 & TR1_ALL_SLAY_DRAGON) == TR1_X15_DRAGON)
		{
			board[j++] = "dragons";
		}
		if (j)
		{
			do_list_flags("It causes a huge amount of extra damage to", "and",
				board, j);
		}

		j = 0;
		if ((o_ptr->flags1 & TR1_ALL_SLAY_DRAGON) == TR1_KILL_DRAGON)
		{
			board[j++] = "dragons";
		}
		if (j)
		{
			do_list_flags("It causes a lot of extra damage to", "and", board,
				j);
		}

		j = 0;
		if ((o_ptr->flags1 & TR1_ALL_SLAY_DRAGON) == TR1_SLAY_DRAGON)
			board[j++] = "dragons";
		if (o_ptr->flags1 & TR1_SLAY_ORC) board[j++] = "orcs";
		if (o_ptr->flags1 & TR1_SLAY_TROLL) board[j++] = "trolls";
		if (o_ptr->flags1 & TR1_SLAY_GIANT) board[j++] = "giants";
		if (o_ptr->flags1 & TR1_SLAY_DEMON) board[j++] = "demons";
		if (o_ptr->flags1 & TR1_SLAY_UNDEAD) board[j++] = "undead";
		if (o_ptr->flags1 & TR1_SLAY_EVIL) board[j++] = "evil monsters";
		if (o_ptr->flags1 & TR1_SLAY_ANIMAL) board[j++] = "animals";
		if (j)
		{
			do_list_flags("It causes extra damage to", "and", board, j);
		}
	}

	j = 0;
	if (o_ptr->flags2 & (TR2_SUST_STR)) board[j++] = "strength";
	if (o_ptr->flags2 & (TR2_SUST_INT)) board[j++] = "intelligence";
	if (o_ptr->flags2 & (TR2_SUST_WIS)) board[j++] = "wisdom";
	if (o_ptr->flags2 & (TR2_SUST_DEX)) board[j++] = "dexterity";
	if (o_ptr->flags2 & (TR2_SUST_CON)) board[j++] = "constitution";
	if (o_ptr->flags2 & (TR2_SUST_CHR)) board[j++] = "charisma";
	if (j == 6)
	{
		info[i++].txt = "It sustains all of your stats.";
	}
	else if (j)
	{
		do_list_flags("It sustains your", "and", board, j);
	}

	j = 0;
	if (o_ptr->flags2 & (TR2_IM_ACID)) board[j++] = "acid";
	if (o_ptr->flags2 & (TR2_IM_ELEC)) board[j++] = "electricity";
	if (o_ptr->flags2 & (TR2_IM_FIRE)) board[j++] = "fire";
	if (o_ptr->flags2 & (TR2_IM_COLD)) board[j++] = "cold";
	if (o_ptr->flags2 & (TR2_FREE_ACT)) board[j++] = "paralysis";
	if (o_ptr->flags2 & (TR2_RES_FEAR)) board[j++] = "fear";
	if (j)
	{
		do_list_flags("It provides immunity to", "and", board, j);
	}

	j = 0;
	if (o_ptr->flags2 & (TR2_RES_ACID)) board[j++] = "acid";
	if (o_ptr->flags2 & (TR2_RES_ELEC)) board[j++] = "electricity";
	if (o_ptr->flags2 & (TR2_RES_FIRE)) board[j++] = "fire";
	if (o_ptr->flags2 & (TR2_RES_COLD)) board[j++] = "cold";
	if (o_ptr->flags2 & (TR2_RES_POIS)) board[j++] = "poison";
	if (o_ptr->flags2 & (TR2_RES_LITE)) board[j++] = "light";
	if (o_ptr->flags2 & (TR2_RES_DARK)) board[j++] = "dark";
	if (o_ptr->flags2 & (TR2_HOLD_LIFE)) board[j++] = "life draining";
	if (o_ptr->flags2 & (TR2_RES_BLIND)) board[j++] = "blindness";
	if (o_ptr->flags2 & (TR2_RES_CONF)) board[j++] = "confusion";
	if (o_ptr->flags2 & (TR2_RES_SOUND)) board[j++] = "sound";
	if (o_ptr->flags2 & (TR2_RES_SHARDS)) board[j++] = "shards";
	if (o_ptr->flags2 & (TR2_RES_NETHER)) board[j++] = "nether";
	if (o_ptr->flags2 & (TR2_RES_NEXUS)) board[j++] = "nexus";
	if (o_ptr->flags2 & (TR2_RES_CHAOS)) board[j++] = "chaos";
	if (o_ptr->flags2 & (TR2_RES_DISEN)) board[j++] = "disenchantment";
	if (j)
	{
		do_list_flags("It provides resistance to", "and", board, j);
	}

    if (o_ptr->flags3 & (TR3_WRAITH))
    {
        info[i++].txt = "It renders you incorporeal.";
    }
	if (o_ptr->flags3 & (TR3_FEATHER))
	{
        info[i++].txt = "It allows you to levitate.";
	}
	if (o_ptr->flags3 & (TR3_LITE))
	{
		info[i++].txt = "It provides permanent light.";
	}
	if (o_ptr->flags3 & (TR3_SEE_INVIS))
	{
		info[i++].txt = "It allows you to see invisible monsters.";
	}
	if (o_ptr->flags3 & (TR3_TELEPATHY))
	{
		info[i++].txt = "It gives telepathic powers.";
	}
	if (o_ptr->flags3 & (TR3_SLOW_DIGEST))
	{
		info[i++].txt = "It slows your metabolism.";
	}
	if (o_ptr->flags3 & (TR3_REGEN))
	{
		info[i++].txt = "It speeds your regenerative powers.";
	}
    if (o_ptr->flags2 & (TR2_REFLECT))
    {
        info[i++].txt = "It reflects bolts and arrows.";
    }
    if (o_ptr->flags3 & (TR3_SH_FIRE))
    {
        info[i++].txt = "It produces a fiery sheath.";
    }
    if (o_ptr->flags3 & (TR3_SH_ELEC))
    {
        info[i++].txt = "It produces an electric sheath.";
    }
    if (o_ptr->flags3 & (TR3_NO_MAGIC))
    {
        info[i++].txt = "It produces an anti-magic shell.";
    }
    if (o_ptr->flags3 & (TR3_NO_TELE))
    {
        info[i++].txt = "It prevents teleportation.";
    }
	if (o_ptr->flags3 & (TR3_XTRA_MIGHT))
	{
		info[i++].txt = "It fires missiles with extra might.";
	}
	if (o_ptr->flags3 & (TR3_XTRA_SHOTS))
	{
		info[i++].txt = "It fires missiles excessively fast.";
	}

	if (cumber_glove(o_ptr))
	{
		if (o_ptr->ident & (IDENT_MENTAL | IDENT_TRIED))
		{
			/* Hack - the "no encumbrance" flag isn't set without spoilers.
			 * so double-check as the player has been told. */
			if (cumber_glove(o1_ptr))
				info[i++].txt = "It inhibits spellcasting.";
		}
		else
		{
			info[i++].txt = "It may inhibit spellcasting.";
		}
	}

	if (cumber_helm(o_ptr))
	{
		if (o_ptr->ident & (IDENT_MENTAL | IDENT_TRIED))
		{
			/* Hack - the "no encumbrance" flag isn't set without spoilers,
			 * so double-check as the player has been told. */
			if (cumber_helm(o1_ptr))
				info[i++].txt = "It inhibits mindcrafting.";
		}
		else
		{
			info[i++].txt = "It may inhibit mindcrafting.";
		}
	}

	if (o_ptr->flags3 & (TR3_DRAIN_EXP))
	{
		info[i++].txt = "It drains experience.";
	}
	if (o_ptr->flags3 & (TR3_TELEPORT))
	{
		info[i++].txt = "It induces random teleportation.";
	}
	if (o_ptr->flags3 & (TR3_AGGRAVATE))
	{
		info[i++].txt = "It aggravates nearby creatures.";
	}

	if (o_ptr->flags3 & (TR3_BLESSED))
	{
		info[i++].txt = "It has been blessed by the gods.";
	}

	/* Describe random possibilities if not *identified*. 
	 * Note that this only has a precise meaning for artefacts. 
	 * Bug - LITE can be mentioned both here and in its own right. Unfortunately it's not easy to fix. */
	if (~o_ptr->ident & IDENT_MENTAL && !cheat_item)
	{
		if (o_ptr->flags2 & (TR2_RAND_RESIST))
		{
			if (o_ptr->name1) info[i++].txt = "It gives you a random resistance.";
			else info[i++].txt = "It may give you a random resistance.";
		}
		if (o_ptr->flags2 & (TR2_RAND_POWER))
		{
			if (o_ptr->name1) info[i++].txt = "It gives you a random power.";
			else info[i++].txt = "It may give you a random power.";
		}
		if (o_ptr->flags2 & (TR2_RAND_EXTRA))
		{
			if (o_ptr->name1) info[i++].txt = "It gives you a random power or resistance.";
			else info[i++].txt = "It may give you a random power or resistance.";
		}
	}

		if (o_ptr->flags3 & (TR3_PERMA_CURSE))
		{
			info[i++].txt = "It is permanently cursed.";
		}
		else if (o_ptr->flags3 & (TR3_HEAVY_CURSE))
		{
			info[i++].txt = "It is heavily cursed.";
		}
	else if (o_ptr->flags3 & (TR3_CURSED))
		{
			info[i++].txt = "It is cursed.";
		}

	if (o_ptr->flags3 & (TR3_AUTO_CURSE))
	{
		info[i++].txt = "It will curse itself.";
	}

    if (o_ptr->flags3 & (TR3_TY_CURSE))
    {
        info[i++].txt = "It carries an ancient foul curse.";
    }

	if ((o_ptr->flags3 & (TR3_IGNORE_ALL)) == TR3_IGNORE_ALL)
	{
		info[i++].txt = "It cannot be harmed by the elements.";
	}
	else if (!(o_ptr->flags3 & (TR3_IGNORE_ALL)))
	{
		/* Say nothing. */
	}
	/* State the missing flags. */
	else
	{
		j = 0;
		if (~o_ptr->flags3 & (TR3_IGNORE_ACID)) board[j++] = "acid";
		if (~o_ptr->flags3 & (TR3_IGNORE_ELEC)) board[j++] = "electricity";
		if (~o_ptr->flags3 & (TR3_IGNORE_FIRE)) board[j++] = "fire";
		if (~o_ptr->flags3 & (TR3_IGNORE_COLD)) board[j++] = "cold";

		if (o_ptr->ident & IDENT_MENTAL || cheat_item)
		{
			board[j] = "can";
		}
		else
		{
			board[j] = "may be able to be";
		}
		if (j)
		{
			do_list_flags(format("It %s be harmed by", board[j]), "and", board, j);
		}
	}
}




/*
 * Describe a "fully identified" item
 * If full is set, the game describes the item as if it was fully identified.
 */
bool identify_fully_aux(object_type *o_ptr, byte flags)
{
	bool full = (flags & 0x01) != 0;
	bool paged = (flags & 0x02) == 0;

	ifa_type info[MAX_IFA];

	C_WIPE(info, MAX_IFA, ifa_type);

	if (full)
	{
		bool hack_known, hack_aware;
		hack_known = (object_known_p(o_ptr) != FALSE);
		hack_aware = (object_aware_p(o_ptr) != FALSE);
		object_known(o_ptr);
		object_aware(o_ptr);

		identify_fully_get(o_ptr, info);
	
		if (!hack_aware) k_info[o_ptr->k_idx].aware = FALSE;
		if (!hack_known) o_ptr->ident &= ~(IDENT_KNOWN);
	}
	else
	{
		identify_fully_get(o_ptr, info);
	}

	if (paged)
	{
		identify_fully_show(info);
	}
	else
	{
		identify_fully_dump(info);
	}

	/* Clear any allocated strings. */
	identify_fully_clear(info);

	/* Gave knowledge */
	return (TRUE);
}


/*
 * Describe item details to a specified stream.
 */
void identify_fully_file(object_type *o_ptr, FILE *fff)
{
	ifa_type info[MAX_IFA];

	C_WIPE(info, MAX_IFA, ifa_type);

	/* Set "brief" mode. */
	brief = TRUE;

	/* Grab the flags. */
	identify_fully_get(o_ptr, info);

	/* Dump the flags, wrapping at 80 characters. */
	identify_fully_dump_file(fff, info);

	/* Clear any allocated strings. */
	identify_fully_clear(info);


	/* Leave "brief" mode. */
	brief = FALSE;
}

/*
 * Convert an inventory index into a one character label
 * Note that the label does NOT distinguish inven/equip.
 */
s16b index_to_label(int i)
{
	/* Indexes for "inven" are easy */
	if (i < INVEN_WIELD) return (I2A(i));

	/* Indexes for "equip" are offset */
	return (I2A(i - INVEN_WIELD));
}


/*
 * Convert a label into the index of an item in the "inven"
 * Return "-1" if the label does not indicate a real item
 */
static s16b label_to_inven(int c)
{
	int i;

	/* Convert */
	i = (islower(c) ? A2I(c) : -1);

	/* Verify the index */
	if ((i < 0) || (i > INVEN_PACK)) return (-1);

	/* Empty slots can never be chosen */
	if (!inventory[i].k_idx) return (-1);

	/* Return the index */
	return (i);
}


/*
 * Convert a label into the index of a item in the "equip"
 * Return "-1" if the label does not indicate a real item
 */
static s16b label_to_equip(int c)
{
	int i;

	/* Convert */
	i = (islower(c) ? A2I(c) : -1) + INVEN_WIELD;

	/* Verify the index */
	if ((i < INVEN_WIELD) || (i >= INVEN_TOTAL)) return (-1);

	/* Empty slots can never be chosen */
	if (!inventory[i].k_idx) return (-1);

	/* Return the index */
	return (i);
}



/*
 * Determine which equipment slot (if any) an item likes
 */
s16b wield_slot(object_type *o_ptr)
{
	/* Slot for equipment */
	switch (o_ptr->tval)
	{
		case TV_DIGGING:
		case TV_HAFTED:
		case TV_POLEARM:
		case TV_SWORD:
		{
			return (INVEN_WIELD);
		}

		case TV_BOW:
		{
			return (INVEN_BOW);
		}

		case TV_RING:
		{
			/* Use the right hand if unused */
			if (!inventory[INVEN_RIGHT].k_idx) return (INVEN_RIGHT);

			/* Use the left hand for swapping if possible */
			if (!inventory[INVEN_LEFT].k_idx || !(inventory[INVEN_LEFT].ident & IDENT_CURSED)) return (INVEN_LEFT);

			/* Use the right otherwise */
			return (INVEN_RIGHT);
		}

		case TV_AMULET:
		{
			return (INVEN_NECK);
		}

		case TV_LITE:
		{
			return (INVEN_LITE);
		}

		case TV_DRAG_ARMOR:
		case TV_HARD_ARMOR:
		case TV_SOFT_ARMOR:
		{
			return (INVEN_BODY);
		}

		case TV_CLOAK:
		{
			return (INVEN_OUTER);
		}

		case TV_SHIELD:
		{
			return (INVEN_ARM);
		}

		case TV_CROWN:
		case TV_HELM:
		{
			return (INVEN_HEAD);
		}

		case TV_GLOVES:
		{
			return (INVEN_HANDS);
		}

		case TV_BOOTS:
		{
			return (INVEN_FEET);
		}

		case TV_POTION:
		case TV_SCROLL:
		case TV_WAND:
		case TV_CHARM:
		{
			/* Use the pouch 1 first */
			if (!inventory[INVEN_POUCH_1].k_idx) return (INVEN_POUCH_1);
			if (!inventory[INVEN_POUCH_2].k_idx) return (INVEN_POUCH_2);
			if (!inventory[INVEN_POUCH_3].k_idx) return (INVEN_POUCH_3);
			if (!inventory[INVEN_POUCH_4].k_idx) return (INVEN_POUCH_4);
			if (!inventory[INVEN_POUCH_5].k_idx) return (INVEN_POUCH_5);
			return (INVEN_POUCH_6);
		}
	}

	/* No slot available */
	return (-1);
}


/*
 * Return a string mentioning how a given item is carried
 */
static cptr mention_use(int i)
{
	cptr p;

	/* Examine the location */
	switch (i)
	{
		case INVEN_WIELD: p = "Wielding"; break;
		case INVEN_BOW:   p = "Shooting"; break;
		case INVEN_LEFT:  p = "On left hand"; break;
		case INVEN_RIGHT: p = "On right hand"; break;
		case INVEN_NECK:  p = "Around neck"; break;
		case INVEN_LITE:  p = "Light source"; break;
		case INVEN_BODY:  p = "On body"; break;
		case INVEN_OUTER: p = "About body"; break;
		case INVEN_ARM:   p = "On arm"; break;
		case INVEN_HEAD:  p = "On head"; break;
		case INVEN_HANDS: p = "On hands"; break;
		case INVEN_FEET:  p = "On feet"; break;
		case INVEN_POUCH_1: p = "In pouch";break;
		case INVEN_POUCH_2: p = "In pouch";break;
		case INVEN_POUCH_3: p = "In pouch";break;
		case INVEN_POUCH_4: p = "In pouch";break;
		case INVEN_POUCH_5: p = "In pouch";break;
		case INVEN_POUCH_6: p = "In pouch";break;
		default:          p = "In pack"; break;
	}

	/* Hack -- Heavy weapon */
	if (i == INVEN_WIELD)
	{
		object_type *o_ptr;
		o_ptr = &inventory[i];
		if (adj_str_hold[p_ptr->stat_ind[A_STR]] < o_ptr->weight / 10)
		{
			p = "Just lifting";
		}
	}

	/* Hack -- Heavy bow */
	if (i == INVEN_BOW)
	{
		object_type *o_ptr;
		o_ptr = &inventory[i];
		if (adj_str_hold[p_ptr->stat_ind[A_STR]] < o_ptr->weight / 10)
		{
			p = "Just holding";
		}
	}

	/* Return the result */
	return (p);
}


/*
 * Return a string describing how a given item is being worn.
 * Currently, only used for items in the equipment, not inventory.
 */
cptr describe_use(int i)
{
	cptr p;

	switch (i)
	{
		case INVEN_WIELD: p = "attacking monsters with"; break;
		case INVEN_BOW:   p = "shooting missiles with"; break;
		case INVEN_LEFT:  p = "wearing on your left hand"; break;
		case INVEN_RIGHT: p = "wearing on your right hand"; break;
		case INVEN_NECK:  p = "wearing around your neck"; break;
		case INVEN_LITE:  p = "using to light the way"; break;
		case INVEN_BODY:  p = "wearing on your body"; break;
		case INVEN_OUTER: p = "wearing on your back"; break;
		case INVEN_ARM:   p = "wearing on your arm"; break;
		case INVEN_HEAD:  p = "wearing on your head"; break;
		case INVEN_HANDS: p = "wearing on your hands"; break;
		case INVEN_FEET:  p = "wearing on your feet"; break;
		case INVEN_POUCH_1: p = "carrying in a pouch";break;
		case INVEN_POUCH_2: p = "carrying in a pouch";break;
		case INVEN_POUCH_3: p = "carrying in a pouch";break;
		case INVEN_POUCH_4: p = "carrying in a pouch";break;
		case INVEN_POUCH_5: p = "carrying in a pouch";break;
		case INVEN_POUCH_6: p = "carrying in a pouch";break;
		default:          p = "carrying in your pack"; break;
	}

	/* Hack -- Heavy weapon */
	if (i == INVEN_WIELD)
	{
		object_type *o_ptr;
		o_ptr = &inventory[i];
		if (adj_str_hold[p_ptr->stat_ind[A_STR]] < o_ptr->weight / 10)
		{
			p = "just lifting";
		}
	}

	/* Hack -- Heavy bow */
	if (i == INVEN_BOW)
	{
		object_type *o_ptr;
		o_ptr = &inventory[i];
		if (adj_str_hold[p_ptr->stat_ind[A_STR]] < o_ptr->weight / 10)
		{
			p = "just holding";
		}
	}

	/* Return the result */
	return p;
}

/*
 * Check an item against the item tester info
 */
bool item_tester_okay(object_type *o_ptr)
{
	/* Hack -- allow listing empty slots */
	if (item_tester_full) return (TRUE);

	/* Require an item */
	if (!o_ptr->k_idx) return (FALSE);

	/* Hack -- ignore "gold" */
	if (o_ptr->tval == TV_GOLD) return (FALSE);

	/* Check the tval */
	if (item_tester_tval)
	{
	/* Is it a spellbook? If so, we need a hack -- TY */
	if (item_tester_tval<=TV_NECROMANCY_BOOK && item_tester_tval>=TV_SORCERY_BOOK)
	    return ((o_ptr->tval <= TV_NECROMANCY_BOOK) && (o_ptr->tval >= TV_SORCERY_BOOK));
	else
	    if (!(item_tester_tval == o_ptr->tval)) return (FALSE);
	}

	/* Check the hook */
	if (item_tester_hook)
	{
		if (!(*item_tester_hook)(o_ptr)) return (FALSE);
	}

	/* Assume okay */
	return (TRUE);
}




/*
 * Choice window "shadow" of the "show_inven()" function
 */
void display_inven(void)
{
	register        int i, n, z = 0;

	object_type *o_ptr;

	byte    attr = TERM_WHITE;

	char    tmp_val[80];

	C_TNEW(o_name, ONAME_MAX, char);


	/* Find the "final" slot */
	for (i = 0; i < INVEN_PACK; i++)
	{
		o_ptr = &inventory[i];

		/* Skip non-objects */
		if (!o_ptr->k_idx) continue;

		/* Track */
		z = i + 1;
	}

	/* Display the pack */
	for (i = 0; i < z; i++)
	{
		/* Examine the item */
		o_ptr = &inventory[i];

		/* Start with an empty "index" */
		tmp_val[0] = tmp_val[1] = tmp_val[2] = ' ';

		/* Is this item "acceptable"? */
		if (item_tester_okay(o_ptr))
		{
			/* Prepare an "index" */
			tmp_val[0] = (byte)(index_to_label(i)); 

			/* Bracket the "index" --(-- */
			tmp_val[1] = ')';
		}

		/* Display the index (or blank space) */
		Term_putstr(0, i, 3, TERM_WHITE, tmp_val);

		/* Obtain an item description */
		object_desc(o_name, o_ptr, TRUE, 3);

		/* Obtain the length of the description */
		n = strlen(o_name);

		/* Get a color */
		attr = tval_to_attr[o_ptr->tval % 128];

		/* Hack -- fake monochrome */
		if (!use_color) attr = TERM_WHITE;

		/* Display the entry itself */
		Term_putstr(3, i, n, attr, o_name);

		/* Erase the rest of the line */
		Term_erase(3+n, i, 255);

		/* Display the weight if needed */
		if (show_weights && o_ptr->weight)
		{
			int wgt = o_ptr->weight * o_ptr->number;
			sprintf(tmp_val, "%3d.%1d lb", wgt / 10, wgt % 10);
			Term_putstr(71, i, -1, TERM_WHITE, tmp_val);
		}
	}

	/* Erase the rest of the window */
	for (i = z; i < Term->hgt; i++)
	{
		/* Erase the line */
		Term_erase(0, i, 255);
	}
	TFREE(o_name);
}



/*
 * Choice window "shadow" of the "show_equip()" function
 */
void display_equip(void)
{
	register        int i, n;
	object_type *o_ptr;
	byte    attr = TERM_WHITE;

	char    tmp_val[80];

	C_TNEW(o_name, ONAME_MAX, char);


	/* Display the equipment */
	for (i = INVEN_WIELD; i < INVEN_TOTAL; i++)
	{
		/* Examine the item */
		o_ptr = &inventory[i];

		/* Start with an empty "index" */
		tmp_val[0] = tmp_val[1] = tmp_val[2] = ' ';

		/* Is this item "acceptable"? */
		if (item_tester_okay(o_ptr))
		{
			/* Prepare an "index" */
			tmp_val[0] = (byte)(index_to_label(i));

			/* Bracket the "index" --(-- */
			tmp_val[1] = ')';
		}

		/* Display the index (or blank space) */
		Term_putstr(0, i - INVEN_WIELD, 3, TERM_WHITE, tmp_val);

		/* Obtain an item description */
		object_desc(o_name, o_ptr, TRUE, 3);

		/* Obtain the length of the description */
		n = strlen(o_name);

		/* Get the color */
		attr = tval_to_attr[o_ptr->tval % 128];

		/* Hack -- fake monochrome */
		if (!use_color) attr = TERM_WHITE;

		/* Display the entry itself */
		Term_putstr(3, i - INVEN_WIELD, n, attr, o_name);

		/* Erase the rest of the line */
		Term_erase(3+n, i - INVEN_WIELD, 255);

		/* Display the slot description (if needed) */
		if (show_labels)
		{
			Term_putstr(61, i - INVEN_WIELD, -1, TERM_WHITE, "<--");
			Term_putstr(65, i - INVEN_WIELD, -1, TERM_WHITE, mention_use(i));
		}

		/* Display the weight (if needed) */
		if (show_weights && o_ptr->weight)
		{
			int wgt = o_ptr->weight * o_ptr->number;
			int col = (show_labels ? 52 : 71);
			sprintf(tmp_val, "%3d.%1d lb", wgt / 10, wgt % 10);
			Term_putstr(col, i - INVEN_WIELD, -1, TERM_WHITE, tmp_val);
		}
	}

	/* Erase the rest of the window */
	for (i = INVEN_TOTAL - INVEN_WIELD; i < Term->hgt; i++)
	{
		/* Clear that line */
		Term_erase(0, i, 255);
	}
	TFREE(o_name);
}






/*
 * Display the inventory.
 *
 * Hack -- do not display "trailing" empty slots
 */
void show_inven(void)
{
	int             i, j, k, l, z = 0;
	int             col, len, lim;

	object_type     *o_ptr;

	C_TNEW(o_name, ONAME_MAX, char);

	char    tmp_val[80];

	int             out_index[23];
	byte    out_color[23];
	char    out_desc[23][80];


	/* Starting column */
	col = command_gap;

	/* Default "max-length" */
	len = 79 - col;

	/* Maximum space allowed for descriptions */
	lim = 79 - 3;

	/* Require space for weight (if needed) */
	if (show_weights) lim -= 9;

	/* Require space for icon */
	if (equippy_chars) lim -= 2;

	/* Find the "final" slot */
	for (i = 0; i < INVEN_PACK; i++)
	{
		o_ptr = &inventory[i];

		/* Skip non-objects */
		if (!o_ptr->k_idx) continue;

		/* Track */
		z = i + 1;
	}

	/* Display the inventory */
	for (k = 0, i = 0; i < z; i++)
	{
		o_ptr = &inventory[i];

		/* Is this item acceptable? */
		if (!item_tester_okay(o_ptr)) continue;

		/* Describe the object */
		object_desc(o_name, o_ptr, TRUE, 3);

		/* Hack -- enforce max length */
		o_name[lim] = '\0';

		/* Save the object index, color, and description */
		out_index[k] = i;
		out_color[k] = tval_to_attr[o_ptr->tval % 128];
		(void)strcpy(out_desc[k], o_name);

		/* Find the predicted "line length" */
		l = strlen(out_desc[k]) + 5;

		/* Be sure to account for the weight */
		if (show_weights) l += 9;

		/* Account for icon if displayed */
		if (equippy_chars) l += 2;

		/* Maintain the maximum length */
		if (l > len) len = l;

		/* Advance to next "line" */
		k++;
	}

	/* Find the column to start in */
	col = (len > 76) ? 0 : (79 - len);

	/* Output each entry */
	for (j = 0; j < k; j++)
	{
		/* Get the index */
		i = out_index[j];

		/* Get the item */
		o_ptr = &inventory[i];

		/* Clear the line */
		prt("", j + 1, col ? col - 2 : col);

		/* Prepare an index --(-- */
		sprintf(tmp_val, "%c)", index_to_label(i));

		/* Clear the line with the (possibly indented) index */
		put_str(tmp_val, j + 1, col);

		/* Display graphics for object, if desired */
		if (equippy_chars)
		{
			byte  a = object_attr(o_ptr);
			char c = object_char(o_ptr);
			
#ifdef AMIGA
			if (a & 0x80) a |= 0x40;
#endif

			Term_draw(col + 3, j + 1, a, c);
		}

		/* Display the entry itself */
		c_put_str(out_color[j], out_desc[j], j + 1, equippy_chars ? (col + 5) : (col + 3));

		/* Display the weight if needed */
		if (show_weights)
		{
			int wgt = o_ptr->weight * o_ptr->number;
			(void)sprintf(tmp_val, " %3d.%1d lb", wgt / 10, wgt % 10);
			put_str(tmp_val, j + 1, 71);
		}
	}

	/* Make a "shadow" below the list (only if needed) */
	if (j && (j < 23)) prt("", j + 1, col ? col - 2 : col);

	/* Save the new column */
	command_gap = col;

	TFREE(o_name);
}



/*
 * Display the equipment.
 */
void show_equip(void)
{
	int                     i, j, k, l;
	int                     col, len, lim;

	object_type             *o_ptr;

	char            tmp_val[80];

	C_TNEW(o_name, ONAME_MAX, char);

	int                     out_index[23];
	byte            out_color[23];
	char            out_desc[23][80];


	/* Starting column */
	col = command_gap;

	/* Maximal length */
	len = 79 - col;

	/* Maximum space allowed for descriptions */
	lim = 79 - 3;

	/* Require space for labels (if needed) */
	if (show_labels) lim -= (14 + 2);

	/* Require space for weight (if needed) */
	if (show_weights) lim -= 9;
	if (equippy_chars) lim -= 2;

	/* Scan the equipment list */
	for (k = 0, i = INVEN_WIELD; i < INVEN_TOTAL; i++)
	{
		o_ptr = &inventory[i];

		/* Is this item acceptable? */
		if (!item_tester_okay(o_ptr)) continue;

		/* Description */
		object_desc(o_name, o_ptr, TRUE, 3);

		/* Truncate the description */
		o_name[lim] = 0;

		/* Save the color */
		out_index[k] = i;
		out_color[k] = tval_to_attr[o_ptr->tval % 128];
		(void)strcpy(out_desc[k], o_name);

		/* Extract the maximal length (see below) */
		l = strlen(out_desc[k]) + (2 + 3);

		/* Increase length for labels (if needed) */
		if (show_labels) l += (14 + 2);

		/* Increase length for weight (if needed) */
		if (show_weights) l += 9;
		if (equippy_chars) l += 2;

		/* Maintain the max-length */
		if (l > len) len = l;

		/* Advance the entry */
		k++;
	}

	/* Hack -- Find a column to start in */
	col = (len > 76) ? 0 : (79 - len);

	/* Output each entry */
	for (j = 0; j < k; j++)
	{
		/* Get the index */
		i = out_index[j];

		/* Get the item */
		o_ptr = &inventory[i];

		/* Clear the line */
		prt("", j + 1, col ? col - 2 : col);

		/* Prepare an index --(-- */
		sprintf(tmp_val, "%c)", index_to_label(i));

		/* Clear the line with the (possibly indented) index */
		put_str(tmp_val, j+1, col);

				if (equippy_chars)
		{
			byte a = object_attr(o_ptr);
			char c = object_char(o_ptr);
			
#ifdef AMIGA
			if (a & 0x80) a |= 0x40;
#endif

			Term_draw(col + 3, j + 1, a, c);
		}

		/* Use labels */
		if (show_labels)
		{
			/* Mention the use */
			(void)sprintf(tmp_val, "%-14s: ", mention_use(i));
			put_str(tmp_val, j+1, equippy_chars ? col + 5 : col + 3);

			/* Display the entry itself */
			c_put_str(out_color[j], out_desc[j], j+1, equippy_chars ? col + 21 : col + 19);
		}

		/* No labels */
		else
		{
			/* Display the entry itself */
			c_put_str(out_color[j], out_desc[j], j+1, equippy_chars ? col + 5 : col + 3);
		}

		/* Display the weight if needed */
		if (show_weights)
		{
			int wgt = o_ptr->weight * o_ptr->number;
			(void)sprintf(tmp_val, " %3d.%d lb", wgt / 10, wgt % 10);
			put_str(tmp_val, j+1, 71);
		}
	}

	/* Make a "shadow" below the list (only if needed) */
	if (j && (j < 23)) prt("", j + 1, col ? col - 2 : col);

	/* Save the new column */
	command_gap = col;

	TFREE(o_name);
}






/*
 * Verify the choice of an item.
 *
 * The item can be negative to mean "item on floor".
 */
static bool verify(cptr prompt, int item)
{
	C_TNEW(o_name, ONAME_MAX, char);

	char    out_val[160];

	object_type *o_ptr;

	/* Inventory */
	if (item >= 0)
	{
		o_ptr = &inventory[item];
	}
	
	/* Floor */
	else
	{
		o_ptr = &o_list[0 - item];
	}
	
	/* Describe */
	object_desc(o_name, o_ptr, TRUE, 3);

	/* Prompt */
	(void)sprintf(out_val, "%s %s? ", prompt, o_name);

	TFREE(o_name);

	/* Query */
	return (get_check(out_val));
}


/*
 * Hack -- allow user to "prevent" certain choices
 *
 * The item can be negative to mean "item on floor".
 */
static bool get_item_allow(int item)
{
	cptr s;

	object_type *o_ptr;

	/* Inventory */
	if (item >= 0)
	{
		o_ptr = &inventory[item];
	}
	
	/* Floor */
	else
	{
		o_ptr = &o_list[0 - item];
	}
	
	/* Find a '!' */
	s = strchr(quark_str(o_ptr->note), '!');

	/* Process preventions */
	while (s)
	{
		/* Check the "restriction" */
		if ((s[1] == command_cmd) || (s[1] == '*'))
		{
			/* Verify the choice */
			if (!verify("Really try", item)) return (FALSE);
		}

		/* Find another '!' */
		s = strchr(s + 1, '!');
	}

	/* Allow it */
	return (TRUE);
}



/*
 * Auxiliary function for "get_item()" -- test an index
 */
static bool get_item_okay(int i)
{
	/* Illegal items */
	if ((i < 0) || (i >= INVEN_TOTAL)) return (FALSE);

	/* Verify the item */
	if (!item_tester_okay(&inventory[i])) return (FALSE);

	/* Assume okay */
	return (TRUE);
}



/*
 * Find the "first" inventory object with the given "tag".
 *
 * A "tag" is a char "n" appearing as "@n" anywhere in the
 * inscription of an object.
 *
 * Also, the tag "@xn" will work as well, where "n" is a tag-char,
 * and "x" is the "current" command_cmd code.
 */
static int get_tag(int *cp, char tag)
{
	int i;
	cptr s;


	/* Check every object */
	for (i = 0; i < INVEN_TOTAL; ++i)
	{
		object_type *o_ptr = &inventory[i];

		/* Skip non-objects */
		if (!o_ptr->k_idx) continue;

		/* Find a '@' */
		s = strchr(quark_str(o_ptr->note), '@');

		/* Process all tags */
		while (s)
		{
			/* Check the normal tags */
			if (s[1] == tag)
			{
				/* Save the actual inventory ID */
				*cp = i;

				/* Success */
				return (TRUE);
			}

			/* Check the special tags */
			else if (s[2] == tag && s[1] == command_cmd)
			{
				/* Save the actual inventory ID */
				*cp = i;

				/* Success */
				return (TRUE);
			}

			/* Find another '@' */
			s = strchr(s + 1, '@');
		}
	}

	/* No such tag */
	return (FALSE);
}



/*
 * Let the user select an item, return its "index"
 *
 * The selected item must satisfy the "item_tester_hook()" function,
 * if that hook is set, and the "item_tester_tval", if that value is set.
 *
 * All "item_tester" restrictions are cleared before this function returns.
 *
 * The user is allowed to choose acceptable items from the equipment,
 * inventory, or floor, respectively, if the proper flag was given,
 * and there are any acceptable items in that location.  Note that
 * the equipment or inventory are displayed (even if no acceptable
 * items are in that location) if the proper flag was given.
 *
 * Note that the user must press "-" to specify the item on the floor,
 * and there is no way to "examine" the item on the floor, while the
 * use of "capital" letters will "examine" an inventory/equipment item,
 * and prompt for its use.
 *
 * If a legal item is selected, we save it in "cp" and return TRUE.
 * If this "legal" item is on the floor, we use a "cp" equal to zero
 * minus the dungeon index of the item on the floor.
 *
 * Otherwise, we return FALSE, and set "cp" to:
 *   -1 for "User hit space/escape"
 *   -2 for "No legal items to choose"
 *
 * Global "command_new" is used when viewing the inventory or equipment
 * to allow the user to enter a command while viewing those screens, and
 * also to induce "auto-enter" of stores, and other such stuff.
 *
 * Global "command_see" may be set before calling this function to start
 * out in "browse" mode.  It is cleared before this function returns.
 *
 * Global "command_wrk" is used to choose between equip/inven listings.
 * If it is TRUE then we are viewing inventory, else equipment.
 *
 * Global "command_gap" is used to indent the inven/equip tables, and
 * to provide some consistancy over time.  It shrinks as needed to hold
 * the various tables horizontally, and can only be reset by calling this
 * function with "command_see" being FALSE, or by pressing ESCAPE from
 * this function, or by hitting "escape" while viewing the inven/equip.
 *
 * We always erase the prompt when we are done.
 *
 * Note that "Term_save()" / "Term_load()" blocks must not overlap.
 */
bool get_item(int *cp, cptr pmt, bool equip, bool inven, bool floor)
{
	cave_type *c_ptr = &cave[py][px];

	s16b this_o_idx, next_o_idx = 0;

	char n1, n2, which = ' ';

	int k, i1, i2, e1, e2;

    bool done, item;
    int ver;

    bool allow_floor = FALSE;

	char tmp_val[160];
	char out_val[160];

	int term = 0;
	void (*old_resize_hook)(void);

#ifdef ALLOW_REPEAT
     
     /* Get the item index */
     if (repeat_pull(cp)) {
         
         /* Floor item? */
         if (*cp < 0) {
         	
 			/* Scan all objects in the grid */
 			for (this_o_idx = c_ptr->o_idx; this_o_idx; this_o_idx = next_o_idx)
 			{
 				object_type *o_ptr;
 
 				/* Acquire object */
 				o_ptr = &o_list[this_o_idx];
 
 				/* Acquire next object */
 				next_o_idx = o_ptr->next_o_idx;
 
 				/* Validate the item */
 				if (!item_tester_okay(o_ptr)) continue;
 
 				/* Save the index */
 				(*cp) = 0 - this_o_idx;
 				
 				/* Forget the item_tester_tval restriction */
 				item_tester_tval = 0;
 		
 				/* Forget the item_tester_hook restriction */
 				item_tester_hook = NULL;
 				
 				/* Success */
 				return (TRUE);
 	        }
         }
         
         /* Verify the item */
         else if (get_item_okay(*cp)) {
         
 	        /* Forget the item_tester_tval restriction */
 	        item_tester_tval = 0;
 	
 	        /* Forget the item_tester_hook restriction */
 	        item_tester_hook = NULL;
 	        
 	        /* Success */
 	        return (TRUE);
         }
     }
 
 #endif /* ALLOW_REPEAT -- TNB */

	/* Paranoia XXX XXX XXX */
	msg_print(NULL);


	/* Not done */
	done = FALSE;

	/* No item selected */
	item = FALSE;

	/* Default to "no item" (see above) */
	*cp = -1;


	/* Full inventory */
	i1 = 0;
	i2 = INVEN_PACK - 1;

	/* Forbid inventory */
	if (!inven) i2 = -1;

	/* Restrict inventory indexes */
	while ((i1 <= i2) && (!get_item_okay(i1))) i1++;
	while ((i1 <= i2) && (!get_item_okay(i2))) i2--;


	/* Full equipment */
	e1 = INVEN_WIELD;
	e2 = INVEN_TOTAL - 1;

	/* Forbid equipment */
	if (!equip) e2 = -1;

	/* Restrict equipment indexes */
	while ((e1 <= e2) && (!get_item_okay(e1))) e1++;
	while ((e1 <= e2) && (!get_item_okay(e2))) e2--;


	/* Restrict floor usage */
	if (floor)
	{
		/* Scan all objects in the grid */
		for (this_o_idx = c_ptr->o_idx; this_o_idx; this_o_idx = next_o_idx)
		{
			object_type *o_ptr;
			
			/* Acquire object */
			o_ptr = &o_list[this_o_idx];

			/* Acquire next object */
			next_o_idx = o_ptr->next_o_idx;

			/* Accept the item on the floor if legal */
			if (item_tester_okay(o_ptr)) allow_floor = TRUE;
		}
	}


	/* Require at least one legal choice */
	if (!allow_floor && (i1 > i2) && (e1 > e2))
	{
		/* Cancel command_see */
		command_see = FALSE;

		/* Hack -- Nothing to choose */
		*cp = -2;

		/* Done */
		done = TRUE;
	}

	/* Analyze choices */
	else
	{
		/* Hack -- Reset display width */
		if (!command_see) command_gap = 50;

		/* Hack -- Start on equipment if requested */
		if (command_see && command_wrk && equip)
		{
			command_wrk = TRUE;
		}

		/* Use inventory if allowed */
		else if (inven)
		{
			command_wrk = FALSE;
		}

		/* Use equipment if allowed */
		else if (equip)
		{
			command_wrk = TRUE;
		}

		/* Use inventory for floor */
		else
		{
			command_wrk = FALSE;
		}
	}


	/* Hack - set no-op resize function for simplicity. */
	old_resize_hook = term_screen->resize_hook;
	term_screen->resize_hook = resize_inkey;

	/* Allow the user to choose to see everything. */
	command_see |= show_choices_main;
 
	/* Hack -- start out in "display" mode */
	if (command_see) term = Term_save_aux();


	/* Repeat until done */
	while (!done)
	{
		/* Show choices */
		if (show_choices)
		{
			/* Update */
			p_ptr->window |= (PW_INVEN | PW_EQUIP);
		}

		/* Inventory screen */
		if (!command_wrk)
		{
			/* Extract the legal requests */
			n1 = I2A(i1);
			n2 = I2A(i2);

			/* Redraw if needed */
			if (command_see) show_inven();
		}

		/* Equipment screen */
		else
		{
			/* Extract the legal requests */
			n1 = I2A(e1 - INVEN_WIELD);
			n2 = I2A(e2 - INVEN_WIELD);

			/* Redraw if needed */
			if (command_see) show_equip();
		}

		/* Viewing inventory */
		if (!command_wrk)
		{
			/* Begin the prompt */
			sprintf(out_val, "Inven:");

			/* Some legal items */
			if (i1 <= i2)
			{
				/* Build the prompt */
				sprintf(tmp_val, " %c-%c,",
					index_to_label(i1), index_to_label(i2));

				/* Append */
				strcat(out_val, tmp_val);
			}

			/* Indicate ability to "view" */
			if (!command_see) strcat(out_val, " * to see,");

			/* Append */
			if (equip) strcat(out_val, " / for Equip,");
		}

		/* Viewing equipment */
		else
		{
			/* Begin the prompt */
			sprintf(out_val, "Equip:");

			/* Some legal items */
			if (e1 <= e2)
			{
				/* Build the prompt */
				sprintf(tmp_val, " %c-%c,",
					index_to_label(e1), index_to_label(e2));

				/* Append */
				strcat(out_val, tmp_val);
			}

			/* Indicate ability to "view" */
			if (!command_see) strcat(out_val, " * to see,");

			/* Append */
			if (inven) strcat(out_val, " / for Inven,");
		}

		/* Indicate legality of the "floor" item */
		if (allow_floor) strcat(out_val, " - for floor,");

		/* Finish the prompt */
		strcat(out_val, " ESC");

		/* Build the prompt */
		sprintf(tmp_val, "(%s) %s", out_val, pmt);

		/* Show the prompt */
		prt(tmp_val, 0, 0);


		/* Get a key */
		which = inkey();

		/* Parse it */
		switch (which)
		{
			case RESIZE_INKEY_KEY:
			{
				/* Resize the screen as normal. */
				(*old_resize_hook)();

				/* Put this screen in term */
				Term_release(term);
				term = Term_save_aux();
				break;
			}
			case ESCAPE:
			{
				command_gap = 50;
				done = TRUE;
				break;
			}

			case '*':
			case '?':
			case ' ':
			{
				/* Show/hide the list */
				if (!command_see)
				{
					if (!term) term = Term_save_aux();
					command_see = TRUE;
				}
				else
				{
					Term_load_aux(term);
					command_see = FALSE;
				}
				break;
			}

			case '/':
			{
				/* Verify legality */
				if (!inven || !equip)
				{
					bell();
					break;
				}

				/* Fix screen */
				if (command_see)
				{
					Term_load_aux(term);
				}

				/* Switch inven/equip */
				command_wrk = !command_wrk;

				/* Need to redraw */
				break;
			}

			case '-':
			{
				/* Use floor item */
				if (allow_floor)
				{
                    /* Scan all objects in the grid */
					for (this_o_idx = c_ptr->o_idx; this_o_idx; this_o_idx = next_o_idx)
					{
						object_type *o_ptr;
						
						/* Acquire object */
						o_ptr = &o_list[this_o_idx];

						/* Acquire next object */
						next_o_idx = o_ptr->next_o_idx;

						/* Skip illegal items */
						if (!item_tester_okay(o_ptr)) continue;

						/* Skip non-verified items */
						if (other_query_flag && !verify("Try", 0 - this_o_idx)) continue;

						/* Skip non-acceptable items */
						if (other_query_flag && !get_item_allow(0 - this_o_idx)) continue;

						/* Accept choice */
						(*cp) = 0 - this_o_idx;

						item = TRUE;
						done = TRUE;
						break;
					}

					/* Outer break */
					if (done) break;
				}

				/* Oops */
				bell();
				break;
			}

			case '0':
			case '1': case '2': case '3':
			case '4': case '5': case '6':
			case '7': case '8': case '9':
			{
				/* XXX XXX Look up that tag */
				if (!get_tag(&k, which))
				{
					bell();
					break;
				}

				/* Hack -- Verify item */
				if ((k < INVEN_WIELD) ? !inven : !equip)
				{
					bell();
					break;
				}

				/* Validate the item */
				if (!get_item_okay(k))
				{
					bell();
					break;
				}

				/* Allow player to "refuse" certain actions */
				if (!get_item_allow(k))
				{
					done = TRUE;
					break;
				}

				/* Use that item */
				(*cp) = k;
				item = TRUE;
				done = TRUE;
				break;
			}

			case '\n':
			case '\r':
			{
				/* Choose "default" inventory item */
				if (!command_wrk)
				{
					k = ((i1 == i2) ? i1 : -1);
				}

				/* Choose "default" equipment item */
				else
				{
					k = ((e1 == e2) ? e1 : -1);
				}

				/* Validate the item */
				if (!get_item_okay(k))
				{
					bell();
					break;
				}

				/* Allow player to "refuse" certain actions */
				if (!get_item_allow(k))
				{
					done = TRUE;
					break;
				}

				/* Accept that choice */
				(*cp) = k;
				item = TRUE;
				done = TRUE;
				break;
			}

			/*
			 * Select a broken item if possible.
			 * Otherwise select a cursed item or nothing.
			 */
			case 'z': case 'Z':
			{
				int start, end, i, cursed = -1;
				/* Find the range. */
				if (command_wrk)
				{
					start = INVEN_WIELD-1;
					end = INVEN_TOTAL;
				}
				else
				{
					start = 0;
					end = INVEN_PACK;
				}
				/* Search the items for something cursed or worthless. */
				for (i = start ; i < end; i++)
				{
					object_type *o_ptr = &inventory[i];
					/* Skip invalid objects */
					if (!get_item_okay(i)) continue;
					/* Skip specified objects */
					if (strstr(quark_str(o_ptr->note), "!k")) continue;
					/* Skip objects which are not worthless */
					if (object_value(o_ptr) > 0) continue;
					/* Notice the first cursed item */
					if (cursed_p(o_ptr) && o_ptr->ident & IDENT_SENSE_CURSED)
					{
						if (cursed < 0) cursed = i;
					}
					/* Return the first non-cursed worthless item */
					else
					{
						break;
					}
				}
				/* No broken items, so return any cursed ones found */
				if (i == end) i = cursed;

				/* There was a broken or cursed item. */
				if (i != -1)
				{
					/* Continue, preserving case */
				which += index_to_label(i) - 'z';
				}
			}

			/* Select the most/least valuable object in the selection. */
			case 'x': case 'X': case 'y': case 'Y':
			if (strchr("XxYy", which) && spoil_value)
			{
				int i, start, end, best;
				bool high = strchr("Xx", which) != NULL;
				bool upper = isupper(which);
				s32b best_price = UNREAD_VALUE;
				if (command_wrk)
				{
					start = INVEN_WIELD-1;
					end = INVEN_TOTAL;
				}
				else
				{
					start = 0;
					end = INVEN_PACK;
				}
				/* Look through the items, finding the best value. */
				for (i = start, best = -1; i < end; i++)
				{
					object_type *o_ptr = &inventory[i];
					s32b this_price;
					if (!get_item_okay(i)) continue;
					this_price = object_value(o_ptr) * o_ptr->number;
					if (best >= start)
					{
						if (high)
						{
							if (this_price < best_price) continue;
						}
						else
						{
							if (this_price > best_price) continue;
						}	
					}
					best = i;
					best_price = this_price;
				}
				/* Paranoia */
				if (best < 0)
				{
					bell();
					break;
				}
				else
				{
					/* Continue, preserving case */
					which = index_to_label(best);
					if (upper) which += 'A'-'a';
				}
			}
			default:
			{
				/* Extract "query" setting */
				ver = isupper(which);
				if (ver) which = tolower(which);

				/* Convert letter to inventory index */
				if (!command_wrk)
				{
					k = label_to_inven(which);
				}

				/* Convert letter to equipment index */
				else
				{
					k = label_to_equip(which);
				}

				/* Validate the item */
				if (!get_item_okay(k))
				{
					bell();
					break;
				}

				/* Verify, abort if requested */
				if (ver && !verify("Try", k))
				{
					done = TRUE;
					break;
				}

				/* Allow player to "refuse" certain actions */
				if (!get_item_allow(k))
				{
					done = TRUE;
					break;
				}

				/* Accept that choice */
				(*cp) = k;
				item = TRUE;
				done = TRUE;
				break;
			}
		}
	}


	/* Fix the screen if necessary */
	if (command_see) Term_load_aux(term);

	/* Forget the saved screen, if any. */
	Term_release(term);

	/* Hack -- Cancel "display" */
	command_see = FALSE;


	/* Forget the item_tester_tval restriction */
	item_tester_tval = 0;

	/* Forget the item_tester_hook restriction */
	item_tester_hook = NULL;

	/* Show item if possible. */
	if (item)
	{
		if (*cp < 0)
			object_track(&o_list[-(*cp)]);
		else
			object_track(&inventory[(*cp)]);
	}

	/* Clean up */
	if (show_choices)
	{
		/* Update */
		p_ptr->window |= (PW_INVEN | PW_EQUIP);
	}


	/* Clear the prompt line */
	prt("", 0, 0);
 
 #ifdef ALLOW_REPEAT
 
     if (item) repeat_push(*cp);
     
 #endif /* ALLOW_REPEAT */

	/* Reset resize hook. */
	term_screen->resize_hook = old_resize_hook;

	/* Return TRUE if something was picked */
	return (item);
}


