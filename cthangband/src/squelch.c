#define SQUELCH_C
/*
 * squelch.c
 * Based on a version by Iain McFall 22/Aug/2000       imcfall@lineone.net
 * This file contains functions for the menu management and 
 * actual hiding of unwanted items
 */

#include "angband.h"

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
		ang_sort(has_sub, order, j, ang_sort_comp_wci, ang_sort_swap_wci);
		
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
 * Display a selection of objects as for a menu.
 * Return how far many rows are used to display it.
 */
int display_item_category(int *max, bool (*item_good)(int, name_centry *),
	name_centry *start, name_centry **choice)
{
	int i, num, ymax;
	name_centry *cat;

	/* A list of the valid options for this prompt. */
	cptr body =	option_chars;
	char sym[61];

	assert(choice+60); /* Caller. */

	/* Clear screen */
	clear_from(1);

	/* Clear the Array */
	WIPE(sym, sym);

	/* Print all tval's and their descriptions */
	for (num = 0, cat = start; (num < 60) && cat->str; cat++)
	{
		for (i = 0; i < z_info->k_max; i++)
		{
			if ((*item_good)(i, cat)) break;
		}

		/* No good options exist in this category. */
		if (i == z_info->k_max) continue;

		/* Save the symbol used to select the category. */
		sym[num] = body[num];

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

	/* Save the number of options. */
	*max = num;

	/* Return the number of lines used (which may be useful. */
	return ymax;
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
 */
static PURE bool destroy_it(object_ctype *o1_ptr)
{
	cptr s;
	object_type o_ptr[1];

	/* Unsetting allow_squelch prevents all squelching. */
	if (!allow_squelch) return FALSE;

	object_info_known(o_ptr, o1_ptr);

	/* Don't hide hidden things. */
	if (hidden_p(o_ptr)) return FALSE;

	/* Things inscribed with !k or !K won't be destroyed! */
	s = get_inscription(o_ptr);
	if (strstr(s, "!k") || strstr(s, "!K")) return FALSE;

	/*
	 * Other things are destroyed if the "destroy" setting is at least as good
	 * as that justified by the feeling.
	 */
	return (k_info[o_ptr->k_idx].squelch >= object_quality(o1_ptr));
}


/*
 * If o_ptr points to a stack of objects in which one or more is to be
 * squelched, set *o_ptr to it and return TRUE.
 * Otherwise, set *o_ptr to NULL and return FALSE.
 */
static bool squelch_object(object_type **o_ptr)
{
	/* Find the next squelched object, if any. */
	for (; *o_ptr; next_object(o_ptr))
	{
		if (destroy_it(*o_ptr)) return TRUE;
	}

	/* Nothing left to squelch, so finish. */
	return FALSE;
}


/*
 * Process a sequence of commands to squelch a stack of objects.
 * Finish once the player has examined (and possibly squelched) everything,
 * if any of the commands take energy or if various "impossible" things happen.
 *
 * This should be similar to (but simpler than) process_player(), 
 *
 * This messes around with keymaps in order to allow keymap to be a sequence
 * of keypresses to execute for every squelched object. It would not work if
 * inkey() did anything more complex than advance inkey_next or set it to 0.
 *
 * These commands should take no energy, although the game will simply stop
 * processing after the first command which does take energy if it does so.
 *
 * The object being squelched will always be accessible as ! at the first
 * command,start as !, but any action which may involve other objects should
 * inscribe the object appropriately beforehand.
 *
 * If there is a keymap in operation as squelching begins, the command sequence
 * here is added to the beginning of it and it is used for any additional
 * keypresses the game requires when processing each object. The keymap given
 * here is reapplied whenever the game reaches the command prompt after it has
 * finished one iteration.
 */
static void process_objects(object_type *o_ptr)
{
	cptr keymap = "K!";

	/* Remember how much energy should really be used. */
	int old_energy_use = energy_use;

	/* Nothing to do if squelching is disabled. */
	if (!allow_squelch) return;

	/* Place the cursor on the player */
	move_cursor_relative(py, px);

	/* Repeat until some energy is used. */
	for (energy_use = 0; !energy_use && o_ptr; )
	{
		/* Notice stuff (if needed) */
		if (p_ptr->notice) notice_stuff();

		/* Update stuff (if needed) */
		if (p_ptr->update) update_stuff();

		/* Redraw stuff (if needed) */
		if (p_ptr->redraw) redraw_stuff();


		/* Refresh (optional) */
		if (fresh_before) Term_fresh();

		/* Select a new squelched object if the old one is finished with. */
		if (gnext_clear())
		{
			/* Find the next object. */
			if (!squelch_object(&o_ptr))
			{
				/* None left. */
				break;
			}
		}

		/* Weird conditions. */
		if (p_ptr->paralyzed || p_ptr->stun >= 100 || resting || running ||
			command_rep || !alive || death || new_level_flag || command_rep ||
			inventory[INVEN_PACK].k_idx)
		{
			break;
		}

		/* Hack - let the player clear any pending keys. */
		msg_print(NULL);

		/* Highlight this object. */
		object_track(o_ptr);

		/* Find the next object now, as this one may not exist later. */
		next_object(&o_ptr);

		/* Start the keymap again. */
		set_gnext(keymap);

		/* Get a command (normal) */
		request_command(FALSE);

		/* Process the command */
		process_command();
	}

	/* Add back the energy which was used initially. */
	energy_use += old_energy_use;
}

/*
 * Start searching the floor for "crap".
 * Return FALSE if the game is already squelching something.
 * Unset PN_FSQUELCH, as t
 */
void squelch_grid(void)
{
	int o = cave[py][px].o_idx;
	if (o) process_objects(o_list+o);
}

/*
 * Start searching the inventory for "crap".
 * Return FALSE if the game is already squelching something.
 */
void squelch_inventory(void)
{
	process_objects(inventory+INVEN_TOTAL-1);
}
