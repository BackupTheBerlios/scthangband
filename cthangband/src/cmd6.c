#define CMD6_C
/* File: cmd6.c */

/* Purpose: Object commands */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"

/*
 * This file includes code for eating food, drinking potions,
 * reading scrolls, aiming wands, using staffs, zapping rods,
 * and activating artifacts.
 *
 * In all cases, if the player becomes "aware" of the item's use
 * by testing it, mark it as "aware" and reward some experience
 * based on the object's level, always rounding up.  If the player
 * remains "unaware", mark that object "kind" as "tried".
 *
 * This code now correctly handles the unstacking of wands, staffs,
 * and rods.  Note the overly paranoid warning about potential pack
 * overflow, which allows the player to use and drop a stacked item.
 *
 * In all "unstacking" scenarios, the "used" object is "carried" as if
 * the player had just picked it up.  In particular, this means that if
 * the use of an item induces pack overflow, that item will be dropped.
 *
 * For simplicity, these routines induce a full "pack reorganization"
 * which not only combines similar items, but also reorganizes various
 * items to obey the current "sorting" method.  This may require about
 * 400 item comparisons, but only occasionally.
 *
 * There may be a BIG problem with any "effect" that can cause "changes"
 * to the inventory.  For example, a "scroll of recharging" can cause
 * a wand/staff to "disappear", moving the inventory up.  Luckily, the
 * scrolls all appear BEFORE the staffs/wands, so this is not a problem.
 * But, for example, a "staff of recharging" could cause MAJOR problems.
 * In such a case, it will be best to either (1) "postpone" the effect
 * until the end of the function, or (2) "change" the effect, say, into
 * giving a staff "negative" charges, or "turning a staff into a stick".
 * It seems as though a "rod of recharging" might in fact cause problems.
 * The basic problem is that the act of recharging (and destroying) an
 * item causes the inducer of that action to "move", causing "o_ptr" to
 * no longer point at the correct item, with horrifying results.
 *
 * Note that food/potions/scrolls no longer use bit-flags for effects,
 * but instead use the "sval" (which is also used to sort the objects).
 */


/*
 * Find the expected number of the power appropriate for an object.
 * As the appropriate power can be derived from one of several places, some
 * of these are offset to separate them. This gives the following ranges:
 *
 * -1023--769: spell power (byte)
 * -768--513: tval (byte, not determined here)
 * -511--257: name1 (byte)
 * -255--1: activation (byte)
 * 1-32767: k_idx (s16b)
 */
static int get_power(const object_type *o_ptr)
{
	if (o_ptr->activation)
	{
		return -256 + o_ptr->activation;
	}
	else if (o_ptr->name1)
	{
		return -512 + o_ptr->name1;
	}
	else
	{
		return o_ptr->k_idx;
	}
}

/*
 * Use an object of some kind, and become aware of it if it has demonstrated
 * what kind of object it is.
 * Return TRUE if it has been used, FALSE if it may not have been.
 */
static bool use_object(object_type *o_ptr, int dir)
{
	bool ident, use;
	int power = get_power(o_ptr);

	/* "detect traps" marks the area it has been used in if the effect is
	 * known. This is always true if the object was previously known to have
	 * that effect, so check this.
	 */
	object_type j_ptr[1];
	object_info_known(j_ptr, o_ptr, 0);
	ident = (power == get_power(j_ptr));

	if (!use_object_power(power, dir, &ident, &use) &&
		!use_object_power(-768+o_ptr->tval, dir, &ident, &use))
	{
		/* Paranoia - no known powers. */
		bell("Unknown power: %d", power);
		return FALSE;
	}

	/* Become aware of the object if it's now known. */
	if (ident && !object_aware_p(o_ptr))
	{
		object_aware(o_ptr);
	}

	return use;
}

/*
 * Calculate the energy needed to use a magical device.
 */
s16b item_use_energy(object_type *o_ptr)
{
	if (is_worn_p(o_ptr))
		return TURN_ENERGY/10;
	else
		return extract_energy[p_ptr->pspeed];
}

/*
 * Eat some food (from the pack or floor)
 */
void do_cmd_eat_food(object_type *o_ptr)
{
	bool	normal_food = FALSE;



	/* Restrict choices to food */
	item_tester_tval = TV_FOOD;

	/* Get an item if we weren't passed one */
	if(!o_ptr)
	{
		errr err;
		/* Get an item (from inven or floor) */
		if (!((o_ptr = get_item(&err, "Eat which item? ", FALSE, TRUE, TRUE))))
		{
			if (err == -2) msg_print("You have nothing to eat.");
			return;
		}
	}

	item_tester_tval = TV_FOOD;
	if(!item_tester_okay(o_ptr))
	{
		msg_print("You can't eat that!");
		item_tester_tval = 0;
		return;
	}
	item_tester_tval = 0;

	/* Sound */
	sound(SOUND_EAT);


	/* Take a turn */
	energy_use = item_use_energy(o_ptr);

	/* Process the object's effects. */
	normal_food = !use_object(o_ptr, 0);

	/* We have tried it */
	object_tried(o_ptr);

	/* Recalculate/redraw stuff (later) */
	update_object(o_ptr, 0);

	/* Food can feed the player */
	switch (p_ptr->prace)
	{
		case RACE_VAMPIRE:
		{
			add_flag(TIMED_FOOD, (o_ptr->pval / 10));
			msg_print("Mere victuals hold scant sustenance for a being such as yourself.");
			if (p_ptr->food < PY_FOOD_ALERT)   /* Hungry */
				msg_print("Your hunger can only be satisfied with fresh blood!");
			break;
		}
		case RACE_SKELETON:
		{
	        if (normal_food)
	        {
				object_type q_ptr[1];

				msg_print("The food falls through your jaws!");

				/* Create the item (should this be object_copy()?) */
				object_copy(q_ptr, o_ptr);

				/* Drop the object from heaven */
				drop_near(q_ptr, -1, py, px);
			}
			else
			{
				msg_print("The food falls through your jaws and vanishes!");
			}
			break;
		}
		case RACE_GOLEM:
		case RACE_ZOMBIE:
		case RACE_SPECTRE:
		{
			msg_print("The food of mortals is poor sustenance for you.");
			add_flag(TIMED_FOOD, ((o_ptr->pval) / 20));
			break;
		}
		default:
		{
			add_flag(TIMED_FOOD, o_ptr->pval);
		}
	}



	/* Destroy a food */
	item_increase(o_ptr, -1);
	item_describe(o_ptr);
	item_optimize(o_ptr);
}
    
/*
 * Quaff a potion (from the pack or the floor)
 */
void do_cmd_quaff_potion(object_type *o_ptr)
{
	/* Restrict choices to potions */
	item_tester_tval = TV_POTION;

	/* Get an item if we weren't passed one */
	if(!o_ptr)
	{
		errr err;
		/* Get an item (from inven or floor) */
		if (!((o_ptr = get_item(&err, "Quaff which potion? ", TRUE, TRUE, TRUE))))
		{
			if (err == -2) msg_print("You have no potions to quaff.");
			return;
		}
	}

	item_tester_tval = TV_POTION;
	if(!item_tester_okay(o_ptr))
	{
		msg_print("That is not a potion!");
		return;
	}
	item_tester_tval = 0;

	/* Sound */
	sound(SOUND_QUAFF);


	/* Take a turn */
	energy_use = item_use_energy(o_ptr);

	/* Analyze the potion (which is always used up). */
	use_object(o_ptr, 0);

    if ((p_ptr->prace == RACE_SKELETON) && (randint(12)==1))
    {
        msg_print("Some of the fluid falls through your jaws!");
        potion_smash_effect(0, py, px, o_ptr->k_idx);
    }

	/* Recalculate/redraw stuff (later) */
	update_object(o_ptr, 0);

	/* The item has been tried */
	object_tried(o_ptr);


	/* Potions can feed the player (should this be altered for skeletons?) */
	(void)add_flag(TIMED_FOOD, o_ptr->pval);


	/* Destroy a potion */
	item_increase(o_ptr, -1);
	item_describe(o_ptr);
	item_optimize(o_ptr);
}


/*
 * Try to curse an object. Return TRUE if an effect was observed.
 */
static bool curse_object(int o_idx, int e_idx, cptr where)
{
	object_type *o_ptr = inventory+o_idx;

	if (!o_ptr->k_idx) return FALSE;

	if (allart_p(o_ptr) && one_in(2))
	{
		/* Cool */
		msg_format("A terrible black aura tries to surround your %s, "
			"but your %v resists the effects!",
			where, object_desc_f3, o_ptr, FALSE, 3);
		return TRUE;
	}

	/* Oops */
	msg_format("A terrible black aura blasts your %v!",
		object_desc_f3, o_ptr, FALSE, 3);

	/* Shatter the object. */
	o_ptr->name1 = 0;
	o_ptr->name2 = e_idx;

	apply_magic_2(o_ptr, dun_depth);

	/* Tell the player the bad news */
	o_ptr->ident |= (IDENT_SENSE);
		
	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Recalculate mana */
	p_ptr->update |= (PU_MANA);

	/* Window stuff */
	p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_PLAYER);

	return TRUE;
}

/*
 * Curse the players armor
 */
bool curse_armor(void)
{
	return curse_object(INVEN_BODY, EGO_BLASTED, "armour");
}

/*
 * Curse the players weapon
 */
bool curse_weapon(void)
{
	return curse_object(INVEN_WIELD, EGO_SHATTERED, "weapon");
}



/*
 * Read a scroll (from the pack or floor).
 *
 * Certain scrolls can be "aborted" without losing the scroll.  These
 * include scrolls with no effects but recharge or identify, which are
 * cancelled before use.  XXX Reading them still takes a turn, though.
 */
void do_cmd_read_scroll(object_type *o_ptr)
{
	int used_up;

	/* Check some conditions */
	if (p_ptr->blind)
	{
		msg_print("You can't see anything.");
		return;
	}
	if (no_lite())
	{
		msg_print("You have no light to read by.");
		return;
	}
	if (p_ptr->confused)
	{
		msg_print("You are too confused!");
		return;
	}


	/* Restrict choices to scrolls */
	item_tester_tval = TV_SCROLL;

	/* Get an item if we weren't passed one */
	if(!o_ptr)
	{
		errr err;
		/* Get an item (from inven or floor) */
		if (!((o_ptr = get_item(&err, "Read which scroll? ", TRUE, TRUE, TRUE))))
		{
			if (err == -2) msg_print("You have no scrolls to read.");
			return;
		}
	}

	item_tester_tval = TV_SCROLL;
	if(!item_tester_okay(o_ptr))
	{
		msg_print("That is not a scroll!");
		item_tester_tval = 0;
		return;
	}
	item_tester_tval = 0;

	/* Take a turn */
	energy_use = item_use_energy(o_ptr);

	/* Analyze the scroll */
	used_up = use_object(o_ptr, 0);


	/* Recalculate/redraw stuff (later) */
	update_object(o_ptr, 0);

	/* The item was tried */
	object_tried(o_ptr);

	/* Hack -- allow certain scrolls to be "preserved" */
	if (!used_up) return;


	/* Destroy a scroll */
	item_increase(o_ptr, -1);
	item_describe(o_ptr);
	item_optimize(o_ptr);
}



/*
 * Determine the chance of using a magical device as a/b (rand_int(b) < a).
 *
 * NB: This does not check that the device is suitable for this calculation.
 */
void get_device_chance(object_type *o_ptr, int *num, int *denom)
{
	/* Find the object level. */
	int lev;
	if (is_wand_p(k_info+o_ptr->k_idx))
	{
		lev = wand_power(k_info+o_ptr->k_idx);
	}
	else if (artifact_p(o_ptr))
	{
		/* Use the artefact level for normal artefacts. */
		lev = a_info[o_ptr->name1].level;
	}
	else
	{
		/* Extract the base item level otherwise. */
		lev = object_k_level(k_info+o_ptr->k_idx);
	}

	/* Hack - Limit the difficulty. */
	lev = MIN(lev, 50);

	/* Base chance of success */
	*num = USE_DEVICE;
	*denom = p_ptr->skill_dev;

	/* Confusion hurts skill */
	if (p_ptr->confused) *denom /= 2;

	/* Higher level objects are harder */
	*denom -= lev;

	/* Give everyone a (slight) chance */
	if (*denom < USE_DEVICE)
	{
		int factor = USE_DEVICE - *denom + 1;
		*num *= factor;
		*denom = USE_DEVICE * factor;
	}

	/* Convert num to the desired form. */
	(*num) = 1+(*denom)-(*num);
}

/*
 * Determine if an attempt to use a magical device succeeded.
 */
static bool use_device_p(object_type *o_ptr)
{
	int num, denom;
	get_device_chance(o_ptr, &num, &denom);
	return (rand_int(denom) < num);
}

/*
 * Use a staff.			-RAK-
 *
 * One charge of one staff disappears.
 *
 * Hack -- staffs of identify can be "cancelled".
 */
void do_cmd_use_staff(object_type *o_ptr)
{
	bool use_charge;


	/* Restrict choices to wands */
	item_tester_tval = TV_STAFF;

	/* Get an item if we weren't already passed one */
	if(!o_ptr)
	{
		errr err;
		/* Get an item (from inven or floor) */
		if (!((o_ptr = get_item(&err, "Use which staff? ", FALSE, TRUE, TRUE))))
		{
			if (err == -2) msg_print("You have no staff to use.");
			return;
		}
	}

	item_tester_tval = TV_STAFF;
	if(!item_tester_okay(o_ptr))
	{
		msg_print("That is not a staff!");
		item_tester_tval = 0;
		return;
	}
	item_tester_tval = 0;

	/* Mega-Hack -- refuse to use a pile from the ground */
	if ((!is_inventory_p(o_ptr)) && (o_ptr->number > 1))
	{
		msg_print("You must first pick up the staffs.");
		return;
	}


	/* Take a turn */
	energy_use = item_use_energy(o_ptr);

	/* Roll for failure. */
	if (!use_device_p(o_ptr))
	{
		if (flush_failure) flush();
		msg_print("You failed to use the staff properly.");
		return;
	}

	/* Notice empty staffs */
	if (o_ptr->pval <= 0)
	{
		if (flush_failure) flush();
		msg_print("The staff has no charges left.");
		o_ptr->ident |= (IDENT_EMPTY);
		return;
	}


	/* Sound */
	sound(SOUND_ZAP);



	/* Analyze the staff */
	use_charge = use_object(o_ptr, 0);


	/* Recalculate/redraw stuff (later) */
	update_object(o_ptr, 0);

	/* Tried the item */
	object_tried(o_ptr);


	/* Hack -- some uses are "free" */
	if (!use_charge) return;


	/* Use a single charge */
	o_ptr->pval--;

	/* Gain exp */
	skill_exp(SKILL_DEVICE);

	/* XXX Hack -- unstack if necessary */
	if ((is_inventory_p(o_ptr)) && (o_ptr->number > 1))
	{
		object_type forge;
		object_type *q_ptr;

		/* Get local object */
		q_ptr = &forge;

		/* Obtain a local object */
		object_copy(q_ptr, o_ptr);

		/* Modify quantity */
		q_ptr->number = 1;

		/* Restore the charges */
		o_ptr->pval++;

		/* Unstack the used item */
		o_ptr->number--;
		total_weight -= q_ptr->weight;
		inven_carry(q_ptr);

		/* Message */
		msg_print("You unstack your staff.");
	}

	/* Describe charges in the pack */
	item_charges(o_ptr);
}


/*
 * Aim a wand (from the pack, pouch or floor).
 *
 * Use a single charge from a single item.
 * Handle "unstacking" in a logical manner.
 *
 * For simplicity, you cannot use a stack of items from the
 * ground.  This would require too much nasty code.
 *
 * There are no wands which can "destroy" themselves, in the inventory
 * or on the ground, so we can ignore this possibility.  Note that this
 * required giving "wand of wonder" the ability to ignore destruction
 * by electric balls.
 *
 * All wands can be "cancelled" at the "Direction?" prompt for free.
 *
 * Note that the basic "bolt" wands do slightly less damage than the
 * basic "bolt" rods, but the basic "ball" wands do the same damage
 * as the basic "ball" rods.
 */
void do_cmd_aim_wand(object_type *o_ptr)
{
	int dir;


	/* Restrict choices to wands */
	item_tester_tval = TV_WAND;

	/* Get an item if we weren't passed one */
	if(!o_ptr)
	{
		errr err;
		/* Get an item (from inven or floor) */
		if (!((o_ptr = get_item(&err, "Aim which wand? ", TRUE, TRUE, TRUE))))
		{
			if (err == -2) msg_print("You have no wand to aim.");
			return;
		}
	}

	item_tester_tval = TV_WAND;
	if(!item_tester_okay(o_ptr))
	{
		msg_print("That is not a wand!");
		item_tester_tval = 0;
		return;
	}
	item_tester_tval = 0;

	/* Mega-Hack -- refuse to aim a pile from the ground */
	if ((!is_inventory_p(o_ptr)) && (o_ptr->number > 1))
	{
		msg_print("You must first pick up the wands.");
		return;
	}


	/* Allow direction to be cancelled for free */
	if (!get_aim_dir(&dir)) return;


	/* Take a turn */
	energy_use = item_use_energy(o_ptr);;

	/* Roll for failure. */
	if (!use_device_p(o_ptr))
	{
		if (flush_failure) flush();
		msg_print("You failed to use the wand properly.");
		return;
	}

	/* The wand is already empty! */
	if (o_ptr->pval <= 0)
	{
		if (flush_failure) flush();
		msg_print("The wand has no charges left.");
		o_ptr->ident |= (IDENT_EMPTY);
		return;
	}


	/* Sound */
	sound(SOUND_ZAP);

	/* Use the object (which always takes a charge). */
	use_object(o_ptr, dir);

	/* Recalculate/redraw stuff (later) */
	update_object(o_ptr, 0);

	/* Mark it as tried */
	object_tried(o_ptr);

	/* Use a single charge */
	o_ptr->pval--;

	/* Gain exp */
	skill_exp(SKILL_DEVICE);

	/* Hack -- unstack if necessary */
	if ((is_inventory_p(o_ptr)) && (o_ptr->number > 1))
	{
		object_type forge;
		object_type *q_ptr;

		/* Get local object */
		q_ptr = &forge;

		/* Obtain a local object */
		object_copy(q_ptr, o_ptr);

		/* Modify quantity */
		q_ptr->number = 1;

		/* Restore the charges */
		o_ptr->pval++;

		/* Unstack the used item */
		o_ptr->number--;
		total_weight -= q_ptr->weight;
		inven_carry(q_ptr);

		/* Message */
		msg_print("You unstack your wand.");
	}

	/* Describe the charges in the pack */
	item_charges(o_ptr);
}


/*
 * Activate (zap) a Rod
 *
 * Unstack fully charged rods as needed.
 *
 * Hack -- rods of perception/genocide can be "cancelled"
 * All rods can be cancelled at the "Direction?" prompt
 */
void do_cmd_zap_rod(object_type *o_ptr)
{
	int dir;

	/* Hack -- let perception get aborted */
	bool use_charge = TRUE;


	/* Restrict choices to rods */
	item_tester_tval = TV_ROD;

	/* Get an item if we do not already have one */
	if(!o_ptr)
	{
		errr err;
		/* Get an item (from inven or floor) */
		if (!((o_ptr = get_item(&err, "Zap which rod? ", FALSE, TRUE, TRUE))))
		{
			if (err == -2) msg_print("You have no rod to zap.");
			return;
		}
	}

	item_tester_tval = TV_ROD;
	if(!item_tester_okay(o_ptr))
	{
		msg_print("That is not a rod!");
		item_tester_tval = 0;
		return;
	}
	item_tester_tval = 0;

	/* Mega-Hack -- refuse to zap a pile from the ground */
	if ((!is_inventory_p(o_ptr)) && (o_ptr->number > 1))
	{
		msg_print("You must first pick up the rods.");
		return;
	}


	/* Get a direction (unless KNOWN not to need it) */
	if (!object_aware_p(o_ptr))
	{
		/* Get a direction, allow cancel */
		if (!get_aim_dir(&dir)) return;
	}
	else
	{
		dir = 0;
	}


	/* Take a turn */
	energy_use = item_use_energy(o_ptr);

	/* Still charging */
	if (o_ptr->timeout)
	{
		if (flush_failure) flush();
		msg_print("The rod is still charging.");
		return;
	}

	/* Roll for failure. */
	if (!use_device_p(o_ptr))
	{
		if (flush_failure) flush();
		msg_print("You failed to use the rod properly.");
		return;
	}

	/* Sound */
	sound(SOUND_ZAP);

	/* Analyze the rod */
	use_charge = use_object(o_ptr, dir);


	/* Recalculate/redraw stuff (later) */
	update_object(o_ptr, 0);

	/* Tried the object */
	object_tried(o_ptr);

	/* Do nothing more if cancelled. */
	if (!use_charge) return;

	/* Most rods simply use the pval to decide their recharging rate. */
	if (o_ptr->pval >= 0) o_ptr->timeout = o_ptr->pval;
	else
	{
		/* A few rods use negative pvals to indicate a variable timeout. */
		int timeout_table[][2] =
		{
			{	11,20	}, /* Trap Location */
			{	11,21	}, /* Illumination */
			{	16,30	}, /* Disarming */
		};

		int *t = timeout_table[-1-o_ptr->pval];
		o_ptr->timeout = rand_range(t[0], t[1]);
	}

	/* Gain exp */
	skill_exp(SKILL_DEVICE);

	/* XXX Hack -- unstack if necessary */
	if ((is_inventory_p(o_ptr)) && (o_ptr->number > 1))
	{
		object_type forge;
		object_type *q_ptr;

		/* Get local object */
		q_ptr = &forge;

		/* Obtain a local object */
		object_copy(q_ptr, o_ptr);

		/* Modify quantity */
		q_ptr->number = 1;

		/* Restore "charge" */
		o_ptr->timeout = 0;

		/* Unstack the used item */
		o_ptr->number--;
		total_weight -= q_ptr->weight;
		inven_carry(q_ptr);

		/* Message */
		msg_print("You unstack your rod.");
	}
}




/*
 * Hook to determine if an object is activatable
 */
static bool item_tester_hook_activate(object_type *o_ptr)
{
	u32b f1, f2, f3;

	/* Not known */
	if (!object_known_p(o_ptr)) return (FALSE);

	/* Extract the flags */
	object_flags(o_ptr, &f1, &f2, &f3);

	/* Check activation flag */
	if (f3 & (TR3_ACTIVATE)) return (TRUE);

	/* Assume not */
	return (FALSE);
}



typedef struct timeout_type timeout_type;
struct timeout_type
{
	int idx;
	s16b min;
	s16b max;
};

static const timeout_type timeouts[] =
{
	{ACT_SUNLIGHT-256, 10, 10},
	{ACT_BO_MISS_1-256, 2, 2},
	{ACT_BA_POIS_1-256, 4, 7},
	{ACT_BO_ELEC_1-256, 6, 11},
	{ACT_BO_ACID_1-256, 5, 9},
	{ACT_BO_COLD_1-256, 7, 13},
	{ACT_BO_FIRE_1-256, 8, 15},
	{ACT_BA_COLD_1-256, 400, 400},
	{ACT_BA_FIRE_1-256, 400, 400},
	{ACT_DRAIN_1-256, 100, 200},
	{ACT_BA_COLD_2-256, 300, 300},
	{ACT_BA_ELEC_2-256, 500, 500},
	{ACT_DRAIN_2-256, 400, 400},
	{ACT_VAMPIRE_1-256, 400, 400},
	{ACT_BO_MISS_2-256, 90, 179},
	{ACT_BA_FIRE_2-256, 225, 449},
	{ACT_BA_COLD_3-256, 325, 649},
	{ACT_BA_ELEC_3-256, 425, 849},
	{ACT_WHIRLWIND-256, 250, 250},
	{ACT_VAMPIRE_2-256, 400, 400},
	{ACT_CALL_CHAOS-256, 350, 350},
	{ACT_SHARD-256, 400, 400},
	{ACT_DISP_EVIL-256, 300, 599},
	{ACT_BA_MISS_3-256, 500, 500},
	{ACT_DISP_GOOD-256, 300, 599},
	{ACT_CONFUSE-256, 15, 15},
	{ACT_SLEEP-256, 55, 55},
	{ACT_QUAKE-256, 50, 50},
	/* {ACT_TERROR-256, 3*plev+30, 3*plev+30}, */
	{ACT_TELE_AWAY-256, 200, 200},
	{ACT_BANISH_EVIL-256, 251, 500},
	{ACT_GENOCIDE-256, 500, 500},
	{ACT_MASS_GENO-256, 1000, 1000},
	{ACT_CHARM_ANIMAL-256, 300, 300},
	{ACT_CHARM_UNDEAD-256, 333, 333},
	{ACT_CHARM_OTHER-256, 400, 400},
	{ACT_CHARM_ANIMALS-256, 500, 500},
	{ACT_CHARM_OTHERS-256, 750, 750},
	{ACT_SUMMON_ANIMAL-256, 201, 500},
	{ACT_SUMMON_PHANTOM-256, 201, 400},
	{ACT_SUMMON_ELEMENTAL-256, 750, 750},
	{ACT_SUMMON_DEMON-256, 667, 999},
	{ACT_SUMMON_UNDEAD-256, 667, 999},
	{ACT_CURE_LW-256, 10, 10},
	{ACT_CURE_MW-256, 3, 5},
	{ACT_CURE_POISON-256, 5, 5},
	{ACT_REST_LIFE-256, 450, 450},
	{ACT_REST_ALL-256, 750, 750},
	{ACT_CURE_700-256, 250, 250},
	{ACT_CURE_1000-256, 888, 888},
	{ACT_ESP-256, 200, 200},
	{ACT_BERSERK-256, 101, 200},
	{ACT_PROT_EVIL-256, 225, 449},
	{ACT_RESIST_ALL-256, 200, 200},
	{ACT_SPEED-256, 250, 250},
	{ACT_XTRA_SPEED-256, 200, 399},
	{ACT_WRAITH-256, 1000, 1000},
	{ACT_INVULN-256, 1000, 1000},
	{ACT_LIGHT-256, 10, 19},
	{ACT_MAP_LIGHT-256, 50, 99},
	{ACT_DETECT_ALL-256, 55, 109},
	{ACT_DETECT_XTRA-256, 1000, 1000},
	{ACT_ID_FULL-256, 750, 750},
	{ACT_ID_PLAIN-256, 10, 10},
	{ACT_RUNE_EXPLO-256, 200, 200},
	{ACT_RUNE_PROT-256, 400, 400},
	{ACT_SATIATE-256, 200, 200},
	{ACT_DEST_DOOR-256, 10, 10},
	{ACT_STONE_MUD-256, 5, 5},
	{ACT_RECHARGE-256, 70, 70},
	{ACT_ALCHEMY-256, 500, 500},
	{ACT_DIM_DOOR-256, 100, 100},
	{ACT_TELEPORT-256, 45, 45},
	{ACT_RECALL-256, 200, 200},
	{ACT_TELEPORT_WAIT-256, 51, 100},
	{OBJ_RING_ACID, 50, 99},
	{OBJ_RING_ICE, 50, 99},
	{OBJ_RING_FIRE, 50, 99},
	{OBJ_DSM_BLUE, 450, 899},
	{OBJ_DSM_WHITE, 450, 899},
	{OBJ_DSM_BLACK, 450, 899},
	{OBJ_DSM_GREEN, 450, 899},
	{OBJ_DSM_RED, 450, 899},
	{OBJ_DSM_MULTI_HUED, 225, 449},
	{OBJ_DSM_BRONZE, 450, 899},
	{OBJ_DSM_GOLD, 450, 899},
	{OBJ_DSM_CHAOS, 300, 599},
	{OBJ_DSM_LAW, 300, 599},
	{OBJ_DSM_BALANCE, 300, 599},
	{OBJ_DSM_PSEUDO, 300, 599},
	{OBJ_DSM_POWER, 300, 599},
	{ART_POLARIS-512, 10, 19},
	{ART_XOTH-512, 50, 99},
	{ART_TRAPEZOHEDRON-512, 20, 39},
	{ART_LOBON-512, 225, 449},
	{ART_ALHAZRED-512, 300, 599},
	{ART_DWARVES-512, 100, 199},
	{ART_MAGIC-512, 100, 199},
	{ART_BAST-512, 150, 299},
	{ART_ELEMFIRE-512, 225, 449},
	{ART_ELEMICE-512, 325, 649},
	{ART_ELEMSTORM-512, 425, 849},
	{ART_NYARLATHOTEP-512, 450, 899},
	{ART_RAZORBACK-512, 1000, 1000},
	{ART_BLADETURNER-512, 400, 400},
	{ART_SOULKEEPER-512, 888, 888},
	{ART_VAMPLORD-512, 300, 300},
	{ART_ORCS-512, 500, 500},
	{ART_OGRELORDS-512, 10, 10},
	/* {ART_POWER-512, 3*plev+30, 3*plev+30}, */
	/* {ART_MASK-512, 3*plev+30, 3*plev+30}, */
	{ART_SKULLKEEPER-512, 55, 109},
	{ART_SUN-512, 250, 250},
	{ART_BARZAI-512, 111, 111},
	{ART_DARKNESS-512, 55, 55},
	{ART_SWASHBUCKLER-512, 70, 70},
	{ART_SHIFTER-512, 45, 45},
	{ART_NYOGTHA-512, 450, 450},
	{ART_LIGHT-512, 2, 2},
	{ART_IRONFIST-512, 8, 15},
	{ART_GHOULS-512, 7, 13},
	{ART_WHITESPARK-512, 6, 11},
	{ART_DEAD-512, 5, 9},
	{ART_COMBAT-512, 90, 179},
	{ART_ITHAQUA-512, 200, 200},
	{ART_DANCING-512, 5, 5},
	{ART_FAITH-512, 8, 15},
	{ART_HOPE-512, 7, 13},
	{ART_CHARITY-512, 6, 11},
	{ART_THOTH-512, 4, 7},
	{ART_ICICLE-512, 5, 9},
	{ART_KARAKAL-512, 35, 35},
	{ART_STARLIGHT-512, 300, 300},
	{ART_DAWN-512, 500, 999},
	{ART_EVERFLAME-512, 400, 400},
	{ART_THEODEN-512, 400, 400},
	{ART_ODIN-512, 500, 500},
	{ART_DESTINY-512, 5, 5},
	{ART_TROLLS-512, 1000, 1000},
	{ART_SPLEENSLICER-512, 3, 5},
	{ART_GNORRI-512, 150, 150},
	{ART_GHARNE-512, 200, 200},
	{ART_TOTILA-512, 15, 15},
	{ART_FIRESTAR-512, 100, 100},
	{ART_THUNDER-512, 100, 199},
	{ART_ERIRIL-512, 10, 10},
	{ART_ATAL-512, 1000, 1000},
	{ART_JUSTICE-512, 70, 70},
	{ART_DEATH-512, 999, 999},
};

#ifdef CHECK_ARRAYS
void check_timeouts(void)
{
	bool checked[65535];
	const timeout_type *a_ptr, *b_ptr;
	for (a_ptr = timeouts; a_ptr < END_PTR(timeouts); a_ptr++)
	{
		if (a_ptr->min > a_ptr->max)
			quit_fmt("min > max for timeout %d.", a_ptr->idx);
	}
	for (a_ptr = timeouts; a_ptr < END_PTR(timeouts)-1; a_ptr++)
	{
		for (b_ptr = a_ptr+1; b_ptr < END_PTR(timeouts); b_ptr++)
		{
			if (a_ptr->idx == b_ptr->idx)
				quit_fmt("Timeout %d is listed twice.", a_ptr->idx);
		}
	}
}
#endif /* CHECK_ARRAYS */

/*
 * Add the appropriate timeout for a rod or activatable object.
 * Complain if an object has no listed timeout as a timeout of 0 should be
 * listed explicitly.
 */
static int activation_timeout(int activation)
{
	switch (activation)
	{
		case ACT_TERROR-256:
		{
			int plev = MAX(1, skill_set[SKILL_DEVICE].value/2);
			return 3*(plev+10);
		}
		case ART_POWER-512:
		case ART_MASK-512:
		{
			int plev = MAX(0, skill_set[SKILL_DEVICE].value/2);
			return 3*(plev+10);
		}
		default:
		{
			const timeout_type *ptr;
			for (ptr = timeouts; ptr < END_PTR(timeouts); ptr++)
			{
				if (ptr->idx == activation)
					return rand_range(ptr->min, ptr->max);
			}

			/* Paranoia - every activation should be present. */
			bell("Unknown timeout requested.");
			return 0;
		}
	}
}

/*
 * Activate a wielded object.  Wielded objects never stack.
 * And even if they did, activatable objects never stack.
 *
 * Note that it always takes a turn to activate an artifact, even if
 * the user hits "escape" at the "direction" prompt.
 */
void do_cmd_activate(object_type *o_ptr)
{
	/* Prepare the hook */
	item_tester_hook = item_tester_hook_activate;

	/* Get an item if we weren't passed one from the inventory*/
	if(!o_ptr)
	{
		errr err;

		/* Get an item (from equip) */
		if (!((o_ptr = get_item(&err, "Activate which item? ", TRUE, FALSE, FALSE))))
		{
			if (err == -2) msg_print("You have nothing to activate.");
			return;
		}
	}

	/* Prepare the hook */
	item_tester_hook = item_tester_hook_activate;
	/* Verify that the item can be used */
	if(!item_tester_okay(o_ptr))
	{
		msg_print("You can't activate that!");
		item_tester_hook = 0;
		return;
	}
	/* Clear the hook */
	item_tester_hook = 0;

	/* Take a turn */
	energy_use = item_use_energy(o_ptr);


	/* Roll for failure. */
	if (!use_device_p(o_ptr))
	{
		if (flush_failure) flush();
		msg_print("You failed to activate it properly.");
		return;
	}

	/* Check the recharge */
	if (o_ptr->timeout)
	{
		msg_print("It whines, glows and fades...");
		return;
	}


	/* Activate the artifact */
	msg_print("You activate it...");

	/* Sound */
	sound(SOUND_ZAP);

	if (use_object(o_ptr, 0))
	{
		/* Set the timeout if the object has been used. */
		o_ptr->timeout += activation_timeout(get_power(o_ptr));

		/* Window stuff */
		p_ptr->window |= (PW_INVEN | PW_EQUIP);

		/* Gain exp */
		skill_exp(SKILL_DEVICE);
	}
}
