#define SQUELCH_C
/*
 * squelch.c
 * Based on a version by Iain McFall 22/Aug/2000       imcfall@lineone.net
 * This file contains functions for the menu management and 
 * actual hiding of unwanted items
 */

#include "angband.h"

#define HIDE_NONE    0
#define HIDE_V_BAD	1
#define HIDE_CURSED	2
#define HIDE_AVERAGE 3
#define HIDE_GOOD	4
#define HIDE_V_GOOD	5
#define HIDE_ALL		6
#define HIDE_CATS	7


#define ITEM_SWORD			1
#define ITEM_HAFTED			2
#define ITEM_BLUNT			3
#define ITEM_AXE                        4
#define ITEM_BOOK_DAEM                  5
#define ITEM_BOW                        6
#define ITEM_AMMO                       7
#define ITEM_DIGGER                     8

#define ITEM_BODY_ARMOUR        9
#define ITEM_DRAG_ARMOUR        10
#define ITEM_OTHER_ARMOUR       11

#define ITEM_RING                       12
#define ITEM_AMULET                     13

#define ITEM_POTION1            14
#define ITEM_POTION2            15
#define ITEM_SCROLL                     16
#define ITEM_ROD                        17
#define ITEM_STAFF                      18
#define ITEM_WAND                       19

#define ITEM_FOOD                       20
#define ITEM_CRAP                       21
#define ITEM_FIRESTONE          22
#define ITEM_ESSENCE            23
#define ITEM_PARCHMENT          24
#define ITEM_INSTRUMENT         25
#define ITEM_RUNE                       26
#define ITEM_STONE                      27
#define ITEM_BOOK_SPIR          28
#define ITEM_BOOK_SONG          29
#define ITEM_BOOK_SYMB          30

#define ITEM_BOOK_VALA          31
#define ITEM_BOOK_MAGE          32
#define ITEM_BOOK_SHAD          33
#define ITEM_BOOK_CHAO          34
#define ITEM_BOOK_NETH          35
#define ITEM_BOOK_CRUS          36
#define ITEM_BOOK_SIGA          37
#define ITEM_BOOK_MAGI          38
#define ITEM_BOOK_PRAY          39
#define ITEM_BOOK_ILLU          40
#define ITEM_BOOK_TRIB          41

#define ITEM_CORPSE                     42

#define ITEM_MAX                        256


static int cat_type[256];

static byte *squelch_kind;

/* Allow squelching to be disabled with an option. */
static bool allow_squelch = TRUE;

/*
 * Initialise the various squelch arrays.
 */
void init_squelch(void)
{
	int i;

	/* Create (and wipe) the kind array. */
	squelch_kind = C_NEW(MAX_K_IDX, byte);
	for (i = 0; i < MAX_K_IDX; i++) squelch_kind[i] = HIDE_NONE;

	/* Assume that everything has its own category to start with. */
	for (i = 0; i < 256; i++) cat_type[i] = i;

	/* Gold has no category. */
	cat_type[TV_GOLD] = 0;
}

/*
 * Process "Q:<k_idx>:<type>" -- squelch settings for object kinds.
 */
cptr process_pref_squelch(char **zz, int n, u16b *sf_flags)
{
	long l;
	int i;

	/* Process a reset request. */
	if (n == 1 && !strcmp(zz[0], "---reset---"))
	{
		for (i = 0; i < MAX_K_IDX; i++)
		{
			squelch_kind[i] = HIDE_NONE;
		}
	}
	else if (n == 2 && !strcmp(zz[0], "allow_squelch"))
	{
		if (!strcmp(zz[1], "Y")) allow_squelch = TRUE;
		else if (!strcmp(zz[1], "N")) allow_squelch = FALSE;
		else return "allow_squelch must be Y or N";
	}
	else if (n != 2)
	{
		return "format not Q:<k_idx>:<type>";
	}
	else
	{
		/* Both parameters must be given as numbers. */
		if (!isdigit(zz[0][0]) || !isdigit(zz[1][0]))
			return "non-numerical input";

		/* Read the k_idx into l and check that it's plausible. */
		l = strtol(zz[0], NULL, 0);
		if (l < 0 || l > MAX_SHORT) return "no such object";

		/* Check that it corresponds to a real object. */
		i = convert_k_idx(l, sf_flags, sf_flags_now);
		if (i >= MAX_K_IDX) return "no such object";

		/* Read the squelch setting into i and check that it's valid. */
		l = strtol(zz[1], NULL, 0);
		if (l < HIDE_NONE || l > HIDE_ALL)
			return "no such squelch setting";

		/* Set the squelch setting appropriately. */
		squelch_kind[i] = l;
	}

	/* If it gets this far, it worked. */
	return SUCCESS;
}

/*
 * Dump the squelch settings to an open stream.
 */
void squelch_dump(FILE *fff)
{
	cptr s;
	int i;

	/* Title. */
	fprintf(fff, "\n\n# Automatic squelch option dump\n\n");

	/* Reset. */
	fprintf(fff, "# Reset squelch options\nQ:--reset--\n\n");

	/* Save allow_squelch. */
	if (allow_squelch)
		fprintf(fff, "# Enable squelching\nQ:Allow_squelch:Y");
	else
		fprintf(fff, "# Disable squelching\nQ:Allow_squelch:N");

	/* Save the object settings. */
	for (i = 0; i < MAX_K_IDX; i++)
	{
		/* Nothing more to say. */
		if (!squelch_kind[i]) continue;

		/* Save the setting. */
		my_fprintf(fff, "# %v\n", object_k_name_f1, i);
		fprintf(fff, "Q:%d:%d\n\n", i, squelch_kind[i]);

		/* Not a real object, but dump it anyway. */
		s = k_name+k_info[i].name;

		if (!*s) s = "Unknown object";

		fprintf(fff, "# %s\nQ:%d:%d", s, i, squelch_kind[i]);
	}
}

/*
 * Save squelch prefs to a file of the player's choosing.
 */
static void save_squelch_prefs(int miny)
{
	FILE *fff;
	char buf[256];

	put_str("File: ", miny, 0);

	sprintf(buf, "%.251s.prf", player_base);

	/* Request choice, accept abort. */
	if (!askfor_aux(buf, 256)) return;

	/* Open the file. */
	fff = my_fopen_path(ANGBAND_DIR_USER, buf, "a");
	if (!fff)
	{
		msg_print("Failed!");
		return;
	}

	/* Make a note of the version. */
	dump_version(fff);

	/* Save the options. */
	squelch_dump(fff);

	/* Finished. */
	my_fclose(fff);

	msg_print("Done.");
}

/*
 * Sorting hook for get_names()
 *
 * Sort from highest frequency to lowest.
 */
static bool ang_sort_comp_wci(vptr u, vptr v, int a, int b)
{
	int *has_sub = (int*)u;
	int *order = (int*)v;
	return (has_sub[order[a]] >= has_sub[order[b]]);
}

/*
 * Swapping hook for wiz_create_item()
 */
static void ang_sort_swap_wci(vptr UNUSED u, vptr v, int a, int b)
{
	int *order = (int*)v;
	int temp;
	temp = order[a];
	order[a] = order[b];
	order[b] = temp;
}

/*
 * Print the names of each object in the given choice table into obuf[],
 * and trim them until each is shorter than len-4 letters long.
 *
 * Starting with the commonest word present in a too-long string,
 * words are removed until the total length is within the maximum.
 * 
 * There are len spaces between the starts of adjacent entries.
 * The actual format is "[x] entry ", making 5 unavailable.
 *
 * This may treat repeated words inconsistently, e.g. by
 * removing the first "of" in "pack of cards of Tarot"
 * when a "die of dodecahedral shape" is shortened,
 */
void get_names(char **obuf, char *this, int num, int *choice, uint len,
	void (*print_f1)(char *, uint, cptr, va_list *))
{
	int i, j, k, l;
	char *sub[10];
	int has_sub[10], times[10], order[10];

	for (i = 0; i < num; i++)
	{
		strnfmt(obuf[i], ONAME_MAX, "%v", print_f1, choice[i]);
	}

	/* Look for prefixes to cut. */
	for (i = 0; i < num; i++)
	{
		/* Short enough already. */
		if (strlen(obuf[i]) < len-4) continue;

		/* Copy to temporary string. */
		strcpy(this, obuf[i]);

		/* Locate each word. */
		for (j = 0; j < 10; j++)
		{
			/* Find the end of the current word or the start of the string. */
			if (j)
				sub[j] = strchr(sub[j-1], ' ');
			else
				sub[j] = this;

			/* Set times and order to default values. */
			times[j] = 1;
			order[j] = j;
			has_sub[j] = 0;
			
			/* Don't continue past the end of the name. */
			if (!sub[j]) break;

			/* Don't change the string for the first word. */
			if (!j) continue;

			/* End last word and advance. */
			*(sub[j]++) = '\0';
		}
		
		/* Look for duplicated words. */
		for (k = 1; k < j; k++)
		{
			for (l = 0; l < k; l++)
			{
				/* Ignore different words. */
				if (strcmp(sub[l], sub[k])) continue;
				
				/* Increment the number of previous occurrences */
				times[k]++;
			}
		}
		/* Calculate how often each word appears in other entries. */
		for (k = 0; k < j; k++)
		{
			for (l = has_sub[k] = 0; l < num; l++)
			{
				cptr time;
				int m;
				for (m = 1, time = obuf[l];; m++)
				{
					/* Find the next copy. */
					time = strstr(time, sub[k]);

					/* Not enough found. */
					if (!time) break;
					
					/* Don't consider this copy again. */
					time++;

					/* Go to the next copy. */
					if (m < times[k]) continue;

					/* This is a real substring. */
					has_sub[k]++;

					/* Finish. */
					break;
				}
			}
		}
		
		/* Sort the list. */
		ang_sort_comp = ang_sort_comp_wci;
		ang_sort_swap = ang_sort_swap_wci;
		ang_sort(has_sub, order, j);
		
		k = 0;
		/* Remove words, most common first. */
		while (strlen(obuf[i]) > len-5)
		{
			/* Replace the word in every string in which it appears. */
			for (l = 0;l < num; l++)
			{
				char *a, *b;

				/* Does it appear here? */
				if (!((a = strstr(obuf[l], sub[k])))) continue;

				/* Actually remove the substring. */
				for (b = a--+strlen(sub[k]);(*a = *b); a++, b++);
			}
			
			/* Mext. */
			k++;
		}
	}
}

/*
 * A list of tvals and their textual names
 */
static name_centry tval_names_squelch[] =
{
	{TV_SWORD,	"Sword"},
	{TV_POLEARM,	"Polearm"},
	{TV_HAFTED,	"Hafted Weapon"},
	{TV_DIGGING,	"Digger"},
	{TV_BOW,	"Bow"},
	{TV_ARROW,	"Missile"},
	{TV_CHEST,	"Chest"},
	{TV_DRAG_ARMOR,	"Dragon Scale Mail"},
	{TV_HARD_ARMOR,	"Hard Armor"},
	{TV_SOFT_ARMOR,	"Soft Armor"},
	{TV_SHIELD,	"Shield"},
	{TV_CROWN,	"Crown"},
	{TV_HELM,	"Helm"},
	{TV_GLOVES,	"Gloves"},
	{TV_BOOTS,	"Boots"},
	{TV_CLOAK,	"Cloak"},
	{TV_RING,	"Ring"},
	{TV_AMULET,	"Amulet"},
	{TV_POTION,	"Potion"},
	{TV_SCROLL,	"Scroll"},
	{TV_WAND,	"Wand"},
	{TV_STAFF,	"Staff"},
	{TV_ROD,	"Rod"},
	{TV_SORCERY_BOOK,	"Spellbook"},
	{TV_CHARM,	"Charm"},
	{TV_SKELETON, "Other thing"},
	{0, NULL}
};

/*
 * Turn every tval in use into one present in tval_names_squelch.
 */
static int get_category_tval(int tval)
{
	switch (tval)
	{
		case TV_ARROW: case TV_BOLT: case TV_SHOT:
			return TV_ARROW;
		case TV_SKELETON: case TV_BOTTLE: case TV_JUNK: case TV_SPIKE:
		case TV_LITE: case TV_FLASK: 
			return TV_SKELETON;
		case TV_SORCERY_BOOK: case TV_THAUMATURGY_BOOK:
		case TV_CONJURATION_BOOK: case TV_NECROMANCY_BOOK:
			return TV_SORCERY_BOOK;
		default:
			return tval;
	}
}

/*
 * Choose an item category to squelch (or, indeed, for any other purpose).
 * Returns the category if one was chosen, NULL otherwise.
 */
name_centry *choose_item_category(bool (*item_good)(int, name_centry *),
	bool *abort, name_centry *start, cptr prompt, bool squelch)
{
	const int max = MAX_K_IDX;
	const bool sym_from_cat = FALSE;

	int i, num, ymax;
	name_centry *cat;

	cptr s;
	char ch;

	name_centry *choice[60];

	/* A list of the valid options for this prompt. */
	cptr body =	option_chars;
	char sym[61];

	/* Clear screen */
	Term_clear();

	/* Clear the Array */
	WIPE(sym, sym);

	/* Print all tval's and their descriptions */
	for (num = 0, cat = start; (num < 60) && cat->str; cat++)
	{
		for (i = 0; i < max; i++)
		{
			if ((*item_good)(i, cat)) break;
		}

		/* No good options exist in this category. */
		if (i == max) continue;

		/* Save the symbol used to select the category. */
		if (sym_from_cat) sym[num] = cat->idx;
		else sym[num] = body[num];

		/* Save the category. */
		choice[num++] = cat;
	}

	/* Allow for three columns of names. */
	ymax = (num+2)/3;

	/* Print everything out. */
	for (i = 0; i < num; i++)
	{
		int row = 2 + (i % ymax);
		int col = 30 * (i / ymax);

		mc_put_fmt(row, col, "$![%c] %.25s", sym[i], choice[i]->str);
	}

	/* Paranoia - no non-empty categories (needs info file weirdness). */
	if (!num)
	{
		msg_print("Squelch menu not available as the character knows of no objects.");
		(*abort) = TRUE;
		return (0);
	}

	/* Other options. */
	if (squelch)
	{
		cptr dis = (allow_squelch) ? "dis" : "en";	
		mc_put_fmt(ymax+3, 0, "[Ctrl-A] %sable auto-squelching", dis);
		mc_put_fmt(ymax+4, 0, "[Ctrl-S] Save squelch settings");
	}

	/* Choose! */
	if (!get_com(prompt, &ch))
	{
		(*abort) = TRUE;
	}
	else
	{
		s = strchr(sym, ch);
		(*abort) = FALSE;

		if (squelch && ch == KTRL('A'))
		{
			allow_squelch = !allow_squelch;
		}
		else if (squelch && ch == KTRL('S'))
		{
			save_squelch_prefs(ymax+6);
		}
		else if (s && s < sym+num)
		{
			return choice[s - sym];
		}
		else
		{
			bell("No such category");
		}
	}

	/* No category selected. */
	return 0;
}

/*
 * Return whether k_info[i] can be generated and is part of the specified
 * category.
 */
static bool good_squelch_object(int a, name_centry *cat)
{
	/* Hack -- skip items which only have special generation methods. */
	if (!kind_created_p(k_info+a)) return FALSE;

	/* Unknown object. */
	if (!spoil_base && !k_info[a].seen) return FALSE;

	/* Simply check the tval. */
	return (get_category_tval(k_info[a].tval) == cat->idx);
}

/*
 * Set qual[x] if setting squelch_kind[k_idx] to x makes sense.
 * Do nothing if not, as qual[] should have been initialised by the game.
 */
static void set_good_squelch_settings(bool *qual, int k_idx)
{
	/* Nothing and everything are always possible. */
	qual[HIDE_NONE] = qual[HIDE_ALL] = TRUE;

	/* If pseudoid is available, include the categories it gives. */
	if (k_can_sense(k_idx))
	{
		qual[HIDE_V_BAD] = qual[HIDE_CURSED] = qual[HIDE_AVERAGE] =
		qual[HIDE_GOOD] = qual[HIDE_V_GOOD] = TRUE;
	}

	else if (k_can_curse(k_idx))
	{
		qual[HIDE_CURSED] = TRUE;
	}
}

static cptr_ch qual_str[HIDE_CATS] =
{
	{IDX(HIDE_NONE) "$wDestroy None-->"},
	{IDX(HIDE_V_BAD) "$GVery Bad-->"},
	{IDX(HIDE_CURSED) "$gCursed-->"},
	{IDX(HIDE_AVERAGE) "$uAverage-->"},
	{IDX(HIDE_GOOD) "$bGood-->"},
	{IDX(HIDE_V_GOOD) "$vEgo-->"},
	{IDX(HIDE_ALL) "$rALL!"}
};

/*
 * Modify the squelch_kind according to the player's wishes.
 */
void do_cmd_options_squelch(void)
{
	bool qual[HIDE_CATS], qual2[HIDE_CATS], abort;

	const int max = MAX_K_IDX;
	bool (*item_good)(int, name_centry *) = good_squelch_object;
	void (*print_f1)(char *, uint, cptr, va_list *) = object_k_name_f1;

	int i, num;
	int col, row;
	int mx, my;
	uint len, yent;
	name_centry *cat;

	cptr s;
	char at, ch;

	int	choice[60];

	/* A list of the valid options for this prompt. */
	cptr body =	option_chars;
	char buf[80];

	C_TNEW(bufx, 61*ONAME_MAX, char);
	char *obuf[61];

	Term_get_size(&mx, &my);

	/* Turn obuf[] into a 2 dimensional array. */
	for (i = 0; i < 61; i++) obuf[i] = bufx+i*ONAME_MAX;

	while (1)
	{
		cat = choose_item_category(item_good, &abort, tval_names_squelch,
			"Select a category/option:", TRUE);

		/* Escape. */
		if (abort) break;

		/* Not a category. */
		if (!cat) continue;

		WIPE(qual, qual);

		/* Clear screen again */
		Term_clear();

		/* Initialise various things for this object list. */

		/* We have to search the whole itemlist. */
		for (num = 0, i = 1; (num < 60) && (i < max); i++)
		{
			if ((*item_good)(i, cat)) choice[num++] = i;
		}

		/* Find the quality settings any object can have. */
		for (i = 0; i < num; i++) set_good_squelch_settings(qual, choice[i]);

		/* choose_item_category shouldn't allow empty categories. */
		assert(num);

		yent = my-3;
		len = mx/((num+yent-1)/(yent));

		/* Get names for each valid object which can all fit on screen at once. */
		get_names(obuf, obuf[60], num, choice, len, print_f1);

		/* The question is always the same. */
		sprintf(buf, "What kind of %.27s? [%c-%c, ^a (all), ^n (none), ^s (step)]",
			cat->str, body[0], body[num-1]);

		/* Now loop through until the player has finished. */
		while (1)
		{
			/* Move the cursor */
			Term_gotoxy(0, 1);

			/* Dump the right choices for this category */
			for (i = 0; i < HIDE_CATS; i++)
			{
				if (qual[i]) mc_roff(qual_str[i].str);
			}

			for (i = 0; i < num; i++)
			{
				row = 3 + (i % yent);
				col = len * (i / yent);
				ch = body[i];
				at = qual_str[squelch_kind[choice[i]]].str[1];

				/* Print it. */
				mc_put_fmt(row, col, "[%c] $%c%.*^s", ch, at, len-4, obuf[i]);
			}
			
			/* Choose! */
			if (!get_com(buf, &ch)) break;
			s = strchr(body, ch);

			if (s && s < body+num)
			{
				i = choice[s-body];
				WIPE(qual2, qual2);
				set_good_squelch_settings(qual2, i);
				do
				{
					if (squelch_kind[i]++ == HIDE_ALL)
						squelch_kind[i] = HIDE_NONE;
				}
				while (!qual2[squelch_kind[i]]);
			}
			/* Destroy All! */
			else if (ch == KTRL('A'))
			{
				for (i = 0; i < num; i++)
				{
					squelch_kind[choice[i]] = HIDE_ALL;
				}

				continue;
			}

			/* Destroy None */
			if (ch == KTRL('N'))
			{
				for (i = 0; i < num; i++)
				{
					squelch_kind[choice[i]] = HIDE_NONE;
				}

				continue;
			}

			/* Step All */
			if (ch == KTRL('S'))
			{
				for (i = 0; i < num; i++)
				{
					byte *b = &squelch_kind[choice[i]];
					do
					{
						if ((*b)++ == HIDE_ALL)
							*b = HIDE_NONE;
					}
					while (!qual[*b]);
				}

				continue;
			}
		} /*squelch Selection */

	}/* category Selection */

	/* Free the temporary name buffer. */
	TFREE(bufx);

	/* Squelch stuff as needed (later). */
	p_ptr->notice |= PN_FSQUELCH | PN_ISQUELCH;

	return;
}

/*
 * Return the minimum squelch setting for an object for it to be suitable for
 * destruction.
 */
static int PURE object_quality(object_ctype *o_ptr)
{
	switch (find_feeling(o_ptr))
	{
		case SENSE_C_ART: case SENSE_C_EGO:
			return HIDE_V_BAD;
		case SENSE_CP_OBJ: case SENSE_C_OBJ: case SENSE_BROKEN:
			return HIDE_CURSED;
		case SENSE_U_OBJ:
			return HIDE_AVERAGE;
		case SENSE_G_OBJ:
			return HIDE_GOOD;
		case SENSE_G_EGO:
			return HIDE_V_GOOD;

		/* Artefacts are the best category of weapon. */
		case SENSE_G_ART: case SENSE_Q_ART:

		/* Not enough is known to squelch by quality. */
		case SENSE_Q_OBJ: case SENSE_QP_OBJ: case SENSE_GP_OBJ:

		/* Empty things cannot be squelched on this basis. */
		case SENSE_EMPTY:

		/* Feelings aware items never have. */
		case SENSE_TRIED: case SENSE_PCURSE:
		default:
			return HIDE_ALL;
	}
}


/*
 * Determine whether a particular object should be squelched or not.
 * It gets a bit paranoid at times but I'm pretty sure that it won't
 * destroy anything you didn't tell it to...  Sensing stuff in the rest
 * of the program can be a bit strange.
 * Broken items get squelched with cursed. Is that right?	
 * Items sensed as 'uncursed' but not known are ignored.
 *
 * Hack - objects inscribed by inscribe_depth are never squelched.
 */   
static PURE bool destroy_it(object_ctype *o1_ptr)
{
	object_type o_ptr[1];

	/* Unsetting allow_squelch prevents all squelching. */
	if (!allow_squelch) return FALSE;

	object_info_known(o_ptr, o1_ptr);

	/* Don't hide hidden things. */
	if (hidden_p(o_ptr)) return FALSE;

	/* Inscribed things won't be destroyed! */
	if (strstr(quark_str(o_ptr->note), "!k")) return FALSE;

	/*
	 * Other things are destroyed if the "destroy" setting is at least as good
	 * as that justified by the feeling.
	 */
	return (squelch_kind[o_ptr->k_idx] >= object_quality(o1_ptr));
}


/* Check the floor for "crap" */
void squelch_grid(void)
{
	int o_idx;

	/* Scan the pile of objects */
	for (o_idx = cave[py][px].o_idx; o_idx; o_idx = o_list[o_idx].next_o_idx)
	{
		/* Acquire object */
		object_type *o_ptr = o_list+o_idx;

		/* Do we want it? */
		if (destroy_it(o_ptr))
		{
			/* Print a message */
			msg_format("Hiding %v.", object_desc_f3, o_ptr, TRUE, 3);

			/* Destroy the item */
			object_hide(o_ptr);
		}
	}
}

/*
 * Check the inventory for "crap"
 * This object does not disappear immediately, but it will appear in a special
 * way in the inventory and can be selected at any option prompt with !.
 */
void squelch_inventory(void)
{
	int i;

	for (i=0; i<INVEN_PACK; i++)
	{
		object_type *o_ptr = &inventory[i];

		/* Do we want it? */
		if (!destroy_it(o_ptr)) continue;

		/* Print a message */
		msg_format("Hiding %v.", object_desc_f3, o_ptr, TRUE, 3);
				
		/* Hide the item. */
		object_hide(o_ptr);
	}
}