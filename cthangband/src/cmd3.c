#define CMD3_C
/* File: cmd3.c */

/* Purpose: Inventory commands */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"






/*
 * Display inventory
 */
void do_cmd_inven(void)
{
	char out_val[160];


	/* Note that we are in "inventory" mode */
	command_wrk = FALSE;


	/* Save the screen */
	Term_save();

	/* Hack -- show empty slots */
	item_tester_full = TRUE;

	/* Display the inventory */
	show_inven();

	/* Hack -- hide empty slots */
	item_tester_full = FALSE;

	/* Build a prompt */
   sprintf(out_val, "Inventory: carrying %d.%d pounds (%d%% of capacity). Command: ",
           total_weight / 10, total_weight % 10,
       (total_weight * 100) / ((adj_str_wgt[p_ptr->stat_ind[A_STR]] * 100) / 2));

	/* Get a command */
	prt(out_val, 0, 0);

	/* Get a new command */
	command_new = inkey();

	/* Restore the screen */
	Term_load();


	/* Process "Escape" */
	if (command_new == ESCAPE)
	{
		/* Reset stuff */
		command_new = 0;
		command_gap = 50;
	}

	/* Process normal keys */
	else
	{
		/* Hack -- Use "display" mode */
		command_see = TRUE;
	}
}


/*
 * Display equipment
 */
void do_cmd_equip(void)
{
	char out_val[160];


	/* Note that we are in "equipment" mode */
	command_wrk = TRUE;


	/* Save the screen */
	Term_save();

	/* Hack -- show empty slots */
	item_tester_full = TRUE;

	/* Display the equipment */
	show_equip();

	/* Hack -- undo the hack above */
	item_tester_full = FALSE;

	/* Build a prompt */
   sprintf(out_val, "Equipment: carrying %d.%d pounds (%d%% of capacity). Command: ",
           total_weight / 10, total_weight % 10,
       (total_weight * 100) / ((adj_str_wgt[p_ptr->stat_ind[A_STR]] * 100) / 2));

	/* Get a command */
	prt(out_val, 0, 0);

	/* Get a new command */
	command_new = inkey();

	/* Restore the screen */
	Term_load();


	/* Process "Escape" */
	if (command_new == ESCAPE)
	{
		/* Reset stuff */
		command_new = 0;
		command_gap = 50;
	}

	/* Process normal keys */
	else
	{
		/* Enter "display" mode */
		command_see = TRUE;
	}
}


/*
 * The "wearable" tester
 */
static bool item_tester_hook_wear(object_type *o_ptr)
{
	/* Check for a usable slot */
	if (wield_slot(o_ptr) >= INVEN_WIELD) return (TRUE);

	/* Assume not wearable */
	return (FALSE);
}

/*
 * Wield or wear a single item from the pack or floor
 */
void do_cmd_wield(void)
{
	errr err;
	int  slot;

	object_type forge;
	object_type *q_ptr;

	object_type *o_ptr;

	cptr act;


	/* Restrict the choices */
	item_tester_hook = item_tester_hook_wear;

	/* Get an item (from inven or floor) */
	if (!((o_ptr = get_item(&err, "Wear/Wield which item? ", FALSE, TRUE, TRUE))))
	{
		if (err == -2) msg_print("You have nothing you can wear or wield.");
		return;
	}


	/* Check the slot */
	slot = wield_slot(o_ptr);

	/* Prevent wielding into a cursed slot */
	if (cursed_p(&inventory[slot]))
	{
		/* Message */
		msg_format("The %v you are %s appears to be cursed.", object_desc_f3,
			&inventory[slot], FALSE, 0, describe_use(slot));

		/* Cancel the command */
		return;
	}



    if ((cursed_p(o_ptr)) && (wear_confirm)
        && (object_known_p(o_ptr) || (o_ptr->ident & (IDENT_SENSE_CURSED))))
    {
        if (!(get_check(format("Really use the %v {cursed}? ", 
			object_desc_f3, o_ptr, FALSE, 0))))
            return;
    }
	/* confirm_wear_all is triggered whenever something may be cursed.
	 * Slots are excluded because items to be placed in them are always
	 * created uncursed. */
	else if (confirm_wear_all && ~o_ptr->ident & IDENT_SENSE_CURSED && wield_slot(o_ptr) >= INVEN_WIELD && wield_slot(o_ptr) <= INVEN_FEET)
	{
        if (!(get_check(format("Really use the %v? ", 
			object_desc_f3, o_ptr, FALSE, 3))))
            return;
	}

	/* Take a turn */
	energy_use = extract_energy[p_ptr->pspeed];

	/* Get local object */
	q_ptr = &forge;

	/* Obtain local object */
	object_copy(q_ptr, o_ptr);

	/* Modify quantity */
	q_ptr->number = 1;

	/* Decrease the item. */
	item_increase(o_ptr, -1);
	item_optimize(o_ptr);

	/* Access the wield slot */
	o_ptr = &inventory[slot];

	/* Take off existing item */
	if (o_ptr->k_idx)
	{
		/* Take off existing item */
		(void)inven_takeoff(slot, 255);
	}

	/* Wear the new stuff */
	object_copy(o_ptr, q_ptr);

	/* Increase the weight */
	total_weight += q_ptr->weight;

	/* Increment the equip counter by hand */
	equip_cnt++;

	/* Where is the item now */
	if (slot == INVEN_WIELD)
	{
		act = "You are wielding";
	}
	else if (slot == INVEN_BOW)
	{
		act = "You are shooting with";
	}
	else if (slot == INVEN_LITE)
	{
		act = "Your light source is";
	}
	else if (slot >= INVEN_POUCH_1)
	{
		act = "You have readied";
	}
	else
	{
		act = "You are wearing";
	}

	/* Message */
	msg_format("%s %v (%c).", act,
		object_desc_f3, o_ptr, TRUE, 3, index_to_label(slot));

	/* Auto-curse */
	{
		u32b f1, f2, f3;
	object_flags(o_ptr, &f1, &f2, &f3);
	if (f3 & TR3_AUTO_CURSE)
	{
			curse(o_ptr);
		}
	}

	/* Cursed! */
	if (cursed_p(o_ptr))
	{
		/* Warn the player */
		msg_print("Oops! It feels deathly cold!");

		/* Make a note of it (only useful for rings and amulets). */
		k_info[o_ptr->k_idx].tried = TRUE;
	}

	/* Display the object if required */
	object_track(o_ptr);

	/* Note whether it is cursed or not. */
	o_ptr->ident |= (IDENT_SENSE_CURSED);

	/* Note that it has been tried. */
	o_ptr->ident |= (IDENT_TRIED);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Recalculate torch */
	p_ptr->update |= (PU_TORCH);

	/* Recalculate mana */
	p_ptr->update |= (PU_MANA);

    p_ptr->redraw |= (PR_EQUIPPY);

	/* Window stuff */
	p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_PLAYER);
}



/*
 * Take off an item
 */
void do_cmd_takeoff(void)
{
	errr err;

	object_type *o_ptr;


	/* Get an item (from equip) */
	if (!((o_ptr = get_item(&err, "Take off which item? ", TRUE, FALSE, FALSE))))
	{
		if (err == -2) msg_print("You are not wearing anything to take off.");
		return;
	}


	/* Item is cursed */
	if (cursed_p(o_ptr))
	{
		/* Oops */
		msg_print("Hmmm, it seems to be cursed.");

		/* Nope */
		return;
	}


	/* Take a turn */
	energy_use = extract_energy[p_ptr->pspeed];

	/* Take off the item */
	(void)inven_takeoff(cnv_obj_to_idx(o_ptr), 255);

    p_ptr->redraw |= (PR_EQUIPPY);
}


/*
 * Drop an item
 */
void do_cmd_drop(void)
{
	errr err;
	int  amt = 1;

	object_type *o_ptr;

	/* Get an item (from equip or inven) */
	if (!((o_ptr = get_item(&err, "Drop which item? ", TRUE, TRUE, FALSE))))
	{
		if (err == -2) msg_print("You have nothing to drop.");
		return;
	}


	/* Hack -- Cannot remove cursed items */
	if (is_worn_p(o_ptr) && cursed_p(o_ptr))
	{
		/* Oops */
		msg_print("Hmmm, it seems to be cursed.");

		/* Nope */
		return;
	}


	/* See how many items */
	if (o_ptr->number > 1)
	{
		/* Get a quantity */
		amt = get_quantity(NULL, o_ptr->number,TRUE);

		/* Allow user abort */
		if (amt <= 0) return;
	}


	/* Take a partial turn */
	energy_use = (extract_energy[p_ptr->pspeed]+1)/2;

	/* Drop (some of) the item */
	inven_drop(cnv_obj_to_idx(o_ptr), amt);

    p_ptr->redraw |= (PR_EQUIPPY);
}

/*
 * Destroy an item
 */
void do_cmd_destroy(void)
{
	errr err;
	int			 amt = 1;
	int			old_number;

	bool		force = FALSE;

	object_type		*o_ptr;


	/* Hack -- force destruction */
	if (command_arg > 0) force = TRUE;

	/* Restrict the choices */
	item_tester_hook = item_tester_hook_destroy;

	/* Get an item (from equip or inven or floor) */
	if (!((o_ptr = get_item(&err, "Destroy which item? ", TRUE, TRUE, TRUE))))
	{
		if (err == -2) msg_print("You have nothing to destroy.");
		return;
	}


	/* See how many items */
	if (o_ptr->number > 1)
	{
		/* Get a quantity */
		amt = get_quantity(NULL, o_ptr->number,TRUE);

		/* Allow user abort */
		if (amt <= 0) return;
	}


	/* Describe the object */
	old_number = o_ptr->number;
	o_ptr->number = amt;
	o_ptr->number = old_number;

	/* Verify unless quantity given */
	if (!force)
	{
		if (!((auto_destroy) && (object_value(o_ptr)<1)))
		{
			/* Make a verification */
			if (!get_check(format("Really destroy %v? ",
				object_desc_f3, o_ptr, TRUE, 3))) return;
			
		}
	}

	/* Take a turn */
	energy_use = extract_energy[p_ptr->pspeed];

	/* Artifacts cannot be destroyed */
	if (allart_p(o_ptr))
	{

        energy_use = 0;

		/* Message */
		msg_format("You cannot destroy %v.", object_desc_f3, o_ptr, TRUE, 3);

		/* We know how valuable it might be. */
		o_ptr->ident |= (IDENT_SENSE_VALUE);

		/* We know that it's extraordinary. */
		o_ptr->ident |= (IDENT_SENSE_HEAVY);

		/* Combine the pack */
		p_ptr->notice |= (PN_COMBINE);

      p_ptr->redraw |= (PR_EQUIPPY);

		/* Window stuff */
		p_ptr->window |= (PW_INVEN | PW_EQUIP);

		/* Done */
		return;
	}

	/* Message */
	msg_format("You destroy %v.", object_desc_f3, o_ptr, TRUE, 3);

	/* Eliminate the item */
	item_increase(o_ptr, -amt);
	item_describe(o_ptr);
	item_optimize(o_ptr);
}


/*
 * Destroy whole pack (and equip)
 * This routine will keep artifacts if 'preserve' is on.
 * Dean Anderson
 */
void destroy_pack(void)
{
	int                 i,amt;

	/* Simply destroy every item */
	for (i = INVEN_TOTAL-1; i>=0; i--)
	{
		object_type *o_ptr = &inventory[i];

		/* Skip non-objects */
		if (!o_ptr->k_idx) continue;
		amt = o_ptr->number;
		/* Mega-Hack -- preserve artifacts */
		if (preserve_mode)
		{
			/* Hack -- Preserve unknown artifacts */
			if (artifact_p(o_ptr))
			{
				/* Mega-Hack -- Preserve the artifact */
				a_info[o_ptr->name1].cur_num = 0;
			}
		}
	/* Eliminate the item (from the pack) */
		item_increase(o_ptr, -amt);
		item_optimize(o_ptr);
	}
}


/*
 * Observe an item which has been *identify*-ed
 */
void do_cmd_observe(void)
{
	errr err;

	object_type		*o_ptr;


	/* Get an item (from equip or inven or floor) */
	if (!((o_ptr = get_item(&err, "Examine which item? ", TRUE, TRUE, TRUE))))
	{
		if (err == -2) msg_print("You have nothing to examine.");
		return;
	}

	/* Describe */
	msg_format("Examining %v...", object_desc_f3, o_ptr, TRUE, 3);

	/* Describe it fully */
	if (!identify_fully_aux(o_ptr, FALSE)) msg_print("You see nothing special.");
}



/*
 * Remove the inscription from an object
 * XXX Mention item (when done)?
 */
void do_cmd_uninscribe(void)
{
	errr err;

	object_type *o_ptr;


	/* Get an item (from equip or inven or floor) */
	if (!((o_ptr = get_item(&err, "Un-inscribe which item? ", TRUE, TRUE, TRUE))))
	{
		if (err == -2) msg_print("You have nothing to un-inscribe.");
		return;
	}

	/* Nothing to remove */
	if (!o_ptr->note)
	{
		msg_print("That item had no inscription to remove.");
		return;
	}

	/* Message */
	msg_print("Inscription removed.");

	/* Remove the incription */
	o_ptr->note = 0;

	/* Combine the pack */
	p_ptr->notice |= (PN_COMBINE);

	/* Window stuff */
	p_ptr->window |= (PW_INVEN | PW_EQUIP);
}


/*
 * Inscribe an object with a comment
 */
void do_cmd_inscribe(void)
{
	errr err;

	object_type		*o_ptr;

	char		out_val[80];


	/* Get an item (from equip or inven or floor) */
	if (!((o_ptr = get_item(&err, "Inscribe which item? ", TRUE, TRUE, TRUE))))
	{
		if (err == -2) msg_print("You have nothing to inscribe.");
		return;
	}

	/* Message */
	msg_format("Inscribing %v.", object_desc_f3, o_ptr, TRUE, 3);
	msg_print(NULL);

		/* Start with the old inscription */
	strcpy(out_val, quark_str(o_ptr->note));

	/* Get a new inscription (possibly empty) */
	if (get_string("Inscription: ", out_val, 80))
	{
		/* Save the inscription */
		o_ptr->note = quark_add(out_val);

		/* Combine the pack */
		p_ptr->notice |= (PN_COMBINE);

		/* Window stuff */
		p_ptr->window |= (PW_INVEN | PW_EQUIP);
	}

		/* Make a note of the change. */
	message_add(format("Inscribed %v as %s.", object_desc_f3, o_ptr, TRUE, 3,
		quark_str(o_ptr->note)));
}



/*
 * An "item_tester_hook" for refilling lanterns
 */
static bool item_tester_refill_lantern(object_type *o_ptr)
{
	/* Flasks of oil are okay */
	if (o_ptr->tval == TV_FLASK) return (TRUE);

	/* Lanterns are okay */
	if (o_ptr->k_idx == OBJ_BRASS_LANTERN) return (TRUE);

	/* Assume not okay */
	return (FALSE);
}


/*
 * Refill the players lamp (from the pack or floor)
 */
static void do_cmd_refill_lamp(object_type *o_ptr)
{
	object_type *j_ptr;


	/* Restrict the choices */
	item_tester_hook = item_tester_refill_lantern;

	/* Get an item if we weren't passed one */
	if (!o_ptr)
	{
		errr err;
		/* Get an (from inven or floor) */
		if (!((o_ptr = get_item(&err, "Refill with which flask? ", TRUE, TRUE, TRUE))))
		{
			if (err == -2) msg_print("You have no flasks of oil.");
			return;
		}
	}

	item_tester_hook = item_tester_refill_lantern;
	if(!item_tester_okay(o_ptr))
	{
		msg_print("You can't refill a lantern from that!");
		item_tester_hook = 0;
		return;
	}
	item_tester_hook = 0;

	/* Take a partial turn */
	energy_use = (extract_energy[p_ptr->pspeed]+1)/2;

	/* Access the lantern */
	j_ptr = &inventory[INVEN_LITE];

	/* Refuel */
	j_ptr->pval += o_ptr->pval;

	/* Message */
	msg_print("You fuel your lamp.");

	/* Comment */
	if (j_ptr->pval >= FUEL_LAMP)
	{
		j_ptr->pval = FUEL_LAMP;
		msg_print("Your lamp is full.");
	}

	/* Decrease the item */
	item_increase(o_ptr, -1);
	item_describe(o_ptr);
	item_optimize(o_ptr);

	/* Recalculate torch */
	p_ptr->update |= (PU_TORCH);
}



/*
 * An "item_tester_hook" for refilling torches
 */
static bool item_tester_refill_torch(object_type *o_ptr)
{
	/* Torches are okay */
	if (o_ptr->k_idx == OBJ_WOODEN_TORCH) return (TRUE);

	/* Assume not okay */
	return (FALSE);
}


/*
 * Refuel the players torch (from the pack or floor)
 */
static void do_cmd_refill_torch(object_type *o_ptr)
{
	object_type *j_ptr;


	/* Restrict the choices */
	item_tester_hook = item_tester_refill_torch;

	/* Get an item if we weren't passed one */
	if(!o_ptr)
	{
		errr err;
		/* Get an (from inven or floor) */
		if (!((o_ptr = get_item(&err, "Refuel with which torch? ", FALSE, TRUE, TRUE))))
		{
			if (err == -2) msg_print("You have no extra torches.");
			return;
		}
	}

	item_tester_hook = item_tester_refill_torch;
	if(!item_tester_okay(o_ptr))
	{
		msg_print("You can't refill a torch with that!");
		item_tester_hook = 0;
		return;
	}
	item_tester_hook = 0;

	/* Take a partial turn */
	energy_use = (extract_energy[p_ptr->pspeed]+1)/2;

	/* Access the primary torch */
	j_ptr = &inventory[INVEN_LITE];

	/* Refuel */
	j_ptr->pval += o_ptr->pval + 5;

	/* Message */
	msg_print("You combine the torches.");

	/* Over-fuel message */
	if (j_ptr->pval >= FUEL_TORCH)
	{
		j_ptr->pval = FUEL_TORCH;
		msg_print("Your torch is fully fueled.");
	}

	/* Refuel message */
	else
	{
		msg_print("Your torch glows more brightly.");
	}

	/* Decrease the item. */
	item_increase(o_ptr, -1);
	item_describe(o_ptr);
	item_optimize(o_ptr);

	/* Recalculate torch */
	p_ptr->update |= (PU_TORCH);
}




/*
 * Refill the players lamp, or restock his torches
 */
void do_cmd_refill(object_type *j_ptr)
{
	object_type *o_ptr;

	/* Get the light */
	o_ptr = &inventory[INVEN_LITE];

	/* It is nothing */
	if (o_ptr->tval != TV_LITE)
	{
		msg_print("You are not wielding a light.");
	}

	/* It's a lamp */
	else if (o_ptr->k_idx == OBJ_BRASS_LANTERN)
	{
		do_cmd_refill_lamp(j_ptr);
	}

	/* It's a torch */
	else if (o_ptr->k_idx == OBJ_WOODEN_TORCH)
	{
		do_cmd_refill_torch(j_ptr);
	}

	/* No torch to refill */
	else
	{
		msg_print("Your light cannot be refilled.");
	}
}






/*
 * Target command
 */
void do_cmd_target(void)
{
	/* Target set */
	if (target_set(TARGET_KILL))
	{
		msg_print("Target Selected.");
	}

	/* Target aborted */
	else
	{
		msg_print("Target Aborted.");
	}
}



/*
 * Look command
 */
void do_cmd_look(void)
{
	/* Look around */
	if (target_set(TARGET_LOOK))
	{
		msg_print("Target Selected.");
	}
}



/*
 * Allow the player to examine other sectors on the map
 */
void do_cmd_locate(void)
{
	int		dir, y1, x1, y2, x2;

	char	tmp_val[80];

	char	out_val[160];


	/* Start at current panel */
	y2 = y1 = panel_row;
	x2 = x1 = panel_col;

	/* Show panels until done */
	while (1)
	{
		/* Describe the location */
		if ((y2 == y1) && (x2 == x1))
		{
			tmp_val[0] = '\0';
		}
		else
		{
			sprintf(tmp_val, "%s%s of",
			        ((y2 < y1) ? " North" : (y2 > y1) ? " South" : ""),
			        ((x2 < x1) ? " West" : (x2 > x1) ? " East" : ""));
		}

		/* Prepare to ask which way to look */
		sprintf(out_val,
		        "Map sector [%d,%d], which is%s your sector.  Direction?",
		        y2, x2, tmp_val);

		/* Assume no direction */
		dir = 0;

		/* Get a direction */
		while (!dir)
		{
			char command;

			/* Get a command (or Cancel) */
			if (!get_com(out_val, &command)) break;

			/* Extract the action (if any) */
			dir = get_keymap_dir(command);

			/* Error */
			if (!dir) bell();
		}

		/* No direction */
		if (!dir) break;

		/* Apply the motion */
		y2 += ddy[dir];
		x2 += ddx[dir];

		/* Verify the row */
		if (y2 > max_panel_rows) y2 = max_panel_rows;
		else if (y2 < 0) y2 = 0;

		/* Verify the col */
		if (x2 > max_panel_cols) x2 = max_panel_cols;
		else if (x2 < 0) x2 = 0;

		/* Handle "changes" */
		if ((y2 != panel_row) || (x2 != panel_col) || centre_view)
		{
			/* Save the new panel info */
			panel_row = y2;
			panel_col = x2;

			/* Recalculate the boundaries */
			panel_bounds();

			/* Update stuff */
			p_ptr->update |= (PU_MONSTERS);

			/* Redraw map */
			p_ptr->redraw |= (PR_MAP);

			/* Handle stuff */
			handle_stuff();
		}
	}


	/* Recenter the map around the player */
	verify_panel();

	/* Update stuff */
	p_ptr->update |= (PU_MONSTERS);

	/* Redraw map */
	p_ptr->redraw |= (PR_MAP);

	/* Window stuff */
	p_ptr->window |= (PW_OVERHEAD);

	/* Handle stuff */
	handle_stuff();
}






/*
 * The table of "symbol info" -- each entry is a string of the form
 * "X:desc" where "X" is the trigger, and "desc" is the "info".
 */
static cptr ident_info[] =
{
	" :A dark grid",
	"!:A potion (or oil)",
	"\":An amulet (or necklace)",
	"#:A wall (or secret door)",
	"$:Treasure (gold or gems)",
	"%:A vein (magma or quartz)",
	/* "&:unused", */
	"':An open door",
	"(:Soft armor",
	"):A shield",
	"*:A vein with treasure",
	"+:A closed door",
	",:Food (or mushroom patch)",
	"-:A wand (or rod)",
	".:Floor",
	"/:A polearm (Axe/Pike/etc)",
	/* "0:unused", */
	"1:Entrance to General Store",
	"2:Entrance to Armory",
	"3:Entrance to Weaponsmith",
	"4:Entrance to Temple",
	"5:Entrance to Alchemy shop",
	"6:Entrance to Magic store",
	"7:Entrance to Black Market",
	"8:Entrance to your home",
	"9:Entrance to Book Store",
	"::Rubble",
    ";:A glyph of warding / explosive rune",
	"<:An up staircase",
	"=:A ring",
	">:A down staircase",
	"?:A scroll",
	"@:You",
	"A:Golem",
	"B:Bird",
	"C:Canine",
	"D:Ancient Dragon/Wyrm",
	"E:Elemental",
	"F:Dragon Fly",
	"G:Ghost",
	"H:Hybrid",
	"I:Insect",
	"J:Snake",
	"K:Killer Beetle",
	"L:Lich",
	"M:Multi-Headed Reptile",
	/* "N:unused", */
	"O:Ogre",
	"P:Giant Humanoid",
	"Q:Quylthulg (Pulsing Flesh Mound)",
	"R:Reptile/Amphibian",
	"S:Spider/Scorpion/Tick",
	"T:Troll",
	"U:Major Demon",
	"V:Vampire",
	"W:Wight/Wraith/etc",
	"X:Xorn/Xaren/etc",
	"Y:Yeti",
	"Z:Zephyr Hound",
	"[:Hard armor",
	"\\:A hafted weapon (mace/whip/etc)",
	"]:Misc. armor",
	"^:A trap",
	"_:A staff",
	/* "`:unused", */
	"a:Ant",
	"b:Bat",
	"c:Centipede",
	"d:Dragon",
	"e:Floating Eye",
	"f:Feline",
	"g:Ghoul",
	"h:Hobbit/Elf/Dwarf",
	"i:Beings of Ib",
	"j:Jelly",
	"k:Kobold",
	"l:Louse",
	"m:Mold",
	"n:Naga",
	"o:Orc",
	"p:Person/Human",
	"q:Quadruped",
	"r:Rodent",
	"s:Skeleton",
	"t:Townsperson",
	"u:Minor Demon",
	"v:Vortex",
	"w:Worm/Worm-Mass",
	/* "x:unused", */
	"y:Yeek",
	"z:Zombie/Mummy",
	"{:A missile (arrow/bolt/shot)",
	"|:An edged weapon (sword/dagger/etc)",
	"}:A launcher (bow/crossbow/sling)",
	"~:A tool (or miscellaneous item)",
	NULL
};



/*
 * Sorting hook -- Comp function -- see below
 *
 * We use "u" to point to array of monster indexes,
 * and "v" to select the type of sorting to perform on "u".
 */
static bool ang_sort_comp_hook(vptr u, vptr v, int a, int b)
{
	u16b *who = (u16b*)(u);

	u16b *why = (u16b*)(v);

	int w1 = who[a];
	int w2 = who[b];

	int z1, z2;


	/* Sort by player kills */
	if (*why >= 4)
	{
		/* Extract player kills */
		z1 = r_info[w1].r_pkills;
		z2 = r_info[w2].r_pkills;

		/* Compare player kills */
		if (z1 < z2) return (TRUE);
		if (z1 > z2) return (FALSE);
	}


	/* Sort by total kills */
	if (*why >= 3)
	{
		/* Extract total kills */
		z1 = r_info[w1].r_tkills;
		z2 = r_info[w2].r_tkills;

		/* Compare total kills */
		if (z1 < z2) return (TRUE);
		if (z1 > z2) return (FALSE);
	}


	/* Sort by monster level */
	if (*why >= 2)
	{
		/* Extract levels */
		z1 = r_info[w1].level;
		z2 = r_info[w2].level;

		/* Compare levels */
		if (z1 < z2) return (TRUE);
		if (z1 > z2) return (FALSE);
	}


	/* Sort by monster experience */
	if (*why >= 1)
	{
		/* Extract experience */
		z1 = r_info[w1].mexp;
		z2 = r_info[w2].mexp;

		/* Compare experience */
		if (z1 < z2) return (TRUE);
		if (z1 > z2) return (FALSE);
	}


	/* Compare indexes */
	return (w1 <= w2);
}


/*
 * Sorting hook -- Swap function -- see below
 *
 * We use "u" to point to array of monster indexes,
 * and "v" to select the type of sorting to perform.
 */
static void ang_sort_swap_hook(vptr u, vptr v, int a, int b)
{
	u16b *who = (u16b*)(u);

	u16b holder;

	/* XXX XXX */
	v = v ? v : 0;

	/* Swap */
	holder = who[a];
	who[a] = who[b];
	who[b] = holder;
}



/*
 * Identify a character, allow recall of monsters
 *
 * Several "special" responses recall "mulitple" monsters:
 *   ^A (all monsters)
 *   ^U (all unique monsters)
 *   ^N (all non-unique monsters)
 *
 * The responses may be sorted in several ways, see below.
 *
 * Note that the player ghosts are ignored. XXX XXX XXX
 */
static void do_cmd_query_symbol_aux(u16b *who)
{
	int		i, n, r_idx;
	char	sym, query;
	char	buf[128];

	bool	all = FALSE;
	bool	uniq = FALSE;
	bool	norm = FALSE;

	bool	recall = FALSE;

	u16b	why = 0;


	/* Get a character, or abort */
	if (!get_com("Enter character to be identified: ", &sym)) return;

	/* Find that character info, and describe it */
	for (i = 0; ident_info[i]; ++i)
	{
		if (sym == ident_info[i][0]) break;
	}

	/* Describe */
	if (sym == KTRL('A'))
	{
		all = TRUE;
		strcpy(buf, "Full monster list.");
	}
	else if (sym == KTRL('U'))
	{
		all = uniq = TRUE;
		strcpy(buf, "Unique monster list.");
	}
	else if (sym == KTRL('N'))
	{
		all = norm = TRUE;
		strcpy(buf, "Non-unique monster list.");
	}
	else if (ident_info[i])
	{
		sprintf(buf, "%c - %s.", sym, ident_info[i] + 2);
	}
	else
	{
		sprintf(buf, "%c - %s.", sym, "Unknown Symbol");
	}

	/* Display the result */
	prt(buf, 0, 0);


	/* Collect matching monsters */
	for (n = 0, i = 1; i < MAX_R_IDX; i++)
	{
		monster_race *r_ptr = &r_info[i];

		/* Skip "fake" monsters. */
		if (is_fake_monster(r_ptr)) continue;

		/* Nothing to recall */
		if (!spoil_mon && !r_ptr->r_sights) continue;

		/* Require non-unique monsters if needed */
		if (norm && (r_ptr->flags1 & (RF1_UNIQUE))) continue;

		/* Require unique monsters if needed */
		if (uniq && !(r_ptr->flags1 & (RF1_UNIQUE))) continue;

		/* Collect "appropriate" monsters */
		if (all || (r_ptr->d_char == sym)) who[n++] = i;
	}

	/* Nothing to recall */
	if (!n) return;


	/* Prompt XXX XXX XXX */
	put_str("Recall details? (k/p/y/n): ", 0, 40);

	/* Query */
	query = inkey();

	/* Restore */
	prt(buf, 0, 0);


	/* Sort by kills (and level) */
	if (query == 'k')
	{
		why = 4;
		query = 'y';
	}

	/* Sort by level */
	if (query == 'p')
	{
		why = 2;
		query = 'y';
	}

	/* Catch "escape" */
	if (query != 'y') return;


	/* Sort if needed */
	if (why)
	{
		/* Select the sort method */
		ang_sort_comp = ang_sort_comp_hook;
		ang_sort_swap = ang_sort_swap_hook;

		/* Sort the array */
		ang_sort(who, &why, n);
	}


	/* Start at the end */
	i = n - 1;

	/* Scan the monster memory */
	while (1)
	{
		/* Extract a race */
		r_idx = who[i];

		/* Hack -- Auto-recall */
		monster_race_track(r_idx);

		/* Hack -- Handle stuff */
		handle_stuff();

		/* Hack -- Begin the prompt */
		roff_top(r_idx);

		/* Hack -- Complete the prompt */
		Term_addstr(-1, TERM_WHITE, " [(r)ecall, ESC]");

		/* Interact */
		while (1)
		{
			/* Recall */
			if (recall)
			{
				/* Save the screen */
				Term_save();

				/* Recall on screen */
				screen_roff(who[i]);

				/* Hack -- Complete the prompt (again) */
				Term_addstr(-1, TERM_WHITE, " [(r)ecall, ESC]");
			}

			/* Command */
			query = inkey();

			/* Unrecall */
			if (recall)
			{
				/* Restore */
				Term_load();
			}

			/* Normal commands */
			if (query != 'r') break;

			/* Toggle recall */
			recall = !recall;
		}

		/* Stop scanning */
		if (query == ESCAPE) break;

		/* Move to "prev" monster */
		if (query == '-')
		{
			if (++i == n)
			{
				i = 0;
				if (!expand_list) break;
			}
		}

		/* Move to "next" monster */
		else
		{
			if (i-- == 0)
			{
				i = n - 1;
				if (!expand_list) break;
			}
		}
	}


	/* Re-display the identity */
	prt(buf, 0, 0);
}

/*
 * A wrapper around do_cmd_query_symbol_aux() to allocate the who array.
 */
void do_cmd_query_symbol(void)
{
	C_TNEW(who, MAX_R_IDX, u16b);
	do_cmd_query_symbol_aux(who);
	TFREE(who);
}


/* 'Handle' an object, doing whatever seems the sensible thing to it... */
void do_cmd_handle(void)
{
	errr err;

	object_type *o_ptr;

	/* Get an item (from equip or inven) */
	if (!((o_ptr = get_item(&err, "Use which item? ", TRUE, TRUE, TRUE))))
	{
		if (err == -2) msg_print("You have nothing to use.");
		return;
	}

	/* First test Wielded items */
	if (is_worn_p(o_ptr))
	{
		/* Try to activate the wielded item, whatever it is */
		do_cmd_activate(o_ptr);
		return;
	}
	/* The item is in our inventory or in a pouch*/
	switch(o_ptr->tval)
	{
	case TV_STAFF:
		{
			do_cmd_use_staff(o_ptr);
			break;
		}
	case TV_WAND:
		{
			do_cmd_aim_wand(o_ptr);
			break;
		}
	case TV_ROD:
		{
			do_cmd_zap_rod(o_ptr);
			break;
		}
	case TV_SCROLL:
		{
			do_cmd_read_scroll(o_ptr);
			break;
		}
	case TV_POTION:
		{
			do_cmd_quaff_potion(o_ptr);
			break;
		}
	case TV_FLASK:case TV_LITE:
		{
			do_cmd_refill(o_ptr);
			break;
		}
	case TV_FOOD:
		{
			do_cmd_eat_food(o_ptr);
			break;
		}
	case TV_SORCERY_BOOK:
	case TV_THAUMATURGY_BOOK:
	case TV_CONJURATION_BOOK:
	case TV_NECROMANCY_BOOK:
		{
			do_cmd_browse(o_ptr);
			break;
		}
	case TV_CHARM:
	{
		int item = cnv_obj_to_idx(o_ptr);
		get_cantrip(&item, k_info[o_ptr->k_idx].extra);
		break;
	}
	default:
		{
			item_tester_hook=item_tester_hook_wear;
			if(item_tester_okay(o_ptr))
			{
				msg_print("That item must be wielded to be used.");
			}
			else
			{
				msg_print("That item can't be used directly.");
			}
			item_tester_hook=0;
			return;
		}
	}
}

