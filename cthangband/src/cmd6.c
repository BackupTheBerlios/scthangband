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

static bool activate_random_artifact(object_type * o_ptr);


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
	int			ident;
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

	/* Identity not known yet */
	ident = FALSE;

	/* Analyze the food */
	switch (o_ptr->k_idx)
	{
		case OBJ_FOOD_POISON:
		{
			if (!(p_ptr->resist_pois || p_ptr->oppose_pois))
			{
				if (set_poisoned(p_ptr->poisoned + rand_int(10) + 10))
				{
					ident = TRUE;
				}
			}
			break;
		}

		case OBJ_FOOD_BLINDNESS:
		{
			if (!p_ptr->resist_blind)
			{
				if (set_blind(p_ptr->blind + rand_int(200) + 200))
				{
					ident = TRUE;
				}
			}
			break;
		}

		case OBJ_FOOD_PARANOIA:
		{
			if (!p_ptr->resist_fear)
			{
				if (set_afraid(p_ptr->afraid + rand_int(10) + 10))
				{
					ident = TRUE;
				}
			}
			break;
		}

		case OBJ_FOOD_CONFUSION:
		{
			if (!p_ptr->resist_conf)
			{
				if (set_confused(p_ptr->confused + rand_int(10) + 10))
				{
					ident = TRUE;
				}
			}
			break;
		}

		case OBJ_FOOD_HALLUCINATION:
		{
			if (!p_ptr->resist_chaos)
			{
				if (set_image(p_ptr->image + rand_int(250) + 250))
				{
					ident = TRUE;
				}
			}
			break;
		}

		case OBJ_FOOD_PARALYSIS:
		{
			if (!p_ptr->free_act)
			{
				if (set_paralyzed(p_ptr->paralyzed + rand_int(10) + 10))
				{
					ident = TRUE;
				}
			}
			break;
		}

		case OBJ_FOOD_DEC_STR:
		{
			take_hit(damroll(6, 6), "poisonous food.", MON_POISONOUS_FOOD);
			(void)do_dec_stat(A_STR);
			ident = TRUE;
			break;
		}

		case OBJ_FOOD_SICKNESS:
		{
			take_hit(damroll(6, 6), "poisonous food.", MON_POISONOUS_FOOD);
			(void)do_dec_stat(A_CON);
			ident = TRUE;
			break;
		}

		case OBJ_FOOD_DEC_INT:
		{
			take_hit(damroll(8, 8), "poisonous food.", MON_POISONOUS_FOOD);
			(void)do_dec_stat(A_INT);
			ident = TRUE;
			break;
		}

		case OBJ_FOOD_DEC_WIS:
		{
			take_hit(damroll(8, 8), "poisonous food.", MON_POISONOUS_FOOD);
			(void)do_dec_stat(A_WIS);
			ident = TRUE;
			break;
		}

		case OBJ_FOOD_UNHEALTH:
		{
			take_hit(damroll(10, 10), "poisonous food.", MON_POISONOUS_FOOD);
			(void)do_dec_stat(A_CON);
			ident = TRUE;
			break;
		}

		case OBJ_FOOD_DISEASE:
		{
			take_hit(damroll(10, 10), "poisonous food.", MON_POISONOUS_FOOD);
			(void)do_dec_stat(A_STR);
			ident = TRUE;
			break;
		}

		case OBJ_FOOD_CURE_POISON:
		{
			if (set_poisoned(0)) ident = TRUE;
			break;
		}

		case OBJ_FOOD_CURE_BLINDNESS:
		{
			if (set_blind(0)) ident = TRUE;
			break;
		}

		case OBJ_FOOD_CURE_PARANOIA:
		{
			if (set_afraid(0)) ident = TRUE;
			break;
		}

		case OBJ_FOOD_CURE_CONFUSION:
		{
			if (set_confused(0)) ident = TRUE;
			break;
		}

		case OBJ_FOOD_CURE_SERIOUS:
		{
			if (hp_player(damroll(4, 8))) ident = TRUE;
			break;
		}

		case OBJ_FOOD_RES_STR:
		{
			if (do_res_stat(A_STR)) ident = TRUE;
			break;
		}

		case OBJ_FOOD_RES_CON:
		{
			if (do_res_stat(A_CON)) ident = TRUE;
			break;
		}

		case OBJ_FOOD_RESTORING:
		{
			if (do_res_stat(A_STR)) ident = TRUE;
			if (do_res_stat(A_INT)) ident = TRUE;
			if (do_res_stat(A_WIS)) ident = TRUE;
			if (do_res_stat(A_DEX)) ident = TRUE;
			if (do_res_stat(A_CON)) ident = TRUE;
			if (do_res_stat(A_CHR)) ident = TRUE;
			break;
		}


		case OBJ_PIECE_OF_ELVISH_WAYBREAD:
		{
			msg_print("That tastes good.");
			(void)set_poisoned(0);
			(void)hp_player(damroll(4, 8));
			ident = TRUE;
			break;
		}

		default:
		{
			msg_print("That tastes good.");
			ident = normal_food = TRUE;
			break;
		}
	}


	/* Combine / Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	/* We have tried it */
	object_tried(o_ptr);

	/* The player is now aware of the object */
	if (ident && !object_aware_p(o_ptr))
	{
		object_aware(o_ptr);
	}

	/* Window stuff */
	p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_PLAYER);


	/* Food can feed the player */
	switch (p_ptr->prace)
	{
		case RACE_VAMPIRE:
		{
			set_food(p_ptr->food + (o_ptr->pval / 10));
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
			set_food(p_ptr->food + ((o_ptr->pval) / 20));
			break;
		}
		default:
		{
			set_food(p_ptr->food + o_ptr->pval);
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
	int		ident;



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

	/* Not identified yet */
	ident = FALSE;

	/* Analyze the potion */
	switch (o_ptr->k_idx)
	{
		case OBJ_POTION_WATER:
		case OBJ_POTION_APPLE_JUICE:
		case OBJ_POTION_SLIME_MOLD_JUICE:
		{
			msg_print("You feel less thirsty.");
			ident = TRUE;
			break;
		}

		case OBJ_POTION_SLOWNESS:
		{
			if (set_slow(p_ptr->slow + randint(25) + 15)) ident = TRUE;
			break;
		}

		case OBJ_POTION_SALT_WATER:
		{
			msg_print("The potion makes you vomit!");
			(void)set_food(PY_FOOD_STARVE - 1);
			(void)set_poisoned(0);
			(void)set_paralyzed(p_ptr->paralyzed + 4);
			ident = TRUE;
			break;
		}

		case OBJ_POTION_POISON:
		{
			if (!(p_ptr->resist_pois || p_ptr->oppose_pois))
			{
				if (set_poisoned(p_ptr->poisoned + rand_int(15) + 10))
				{
					ident = TRUE;
				}
			}
			break;
		}

		case OBJ_POTION_BLINDNESS:
		{
			if (!p_ptr->resist_blind)
			{
				if (set_blind(p_ptr->blind + rand_int(100) + 100))
				{
					ident = TRUE;
				}
			}
			break;
		}

        case OBJ_POTION_BOOZE: /* Booze */
		{
            if (!((p_ptr->resist_conf) || (p_ptr->resist_chaos)))
			{
				if (set_confused(p_ptr->confused + rand_int(20) + 15))
				{
					ident = TRUE;
				}
                if (randint(2)==1)
                {
                    if (set_image(p_ptr->image + rand_int(150) + 150))
                    {
                        ident = TRUE;
                    }
                }
                if (randint(13)==1)
                {
                    ident = TRUE;
                    if(randint(3)==1) lose_all_info();
                    else wiz_dark();
                    teleport_player(100);
                    wiz_dark();
                    msg_print("You wake up somewhere with a sore head...");
                    msg_print("You can't remember a thing, or how you got here!");
                }
			}
			break;
		}

		case OBJ_POTION_SLEEP:
		{
			if (!p_ptr->free_act)
			{
				if (set_paralyzed(p_ptr->paralyzed + rand_int(4) + 4))
				{
					ident = TRUE;
				}
			}
			break;
		}

		case OBJ_POTION_LOSE_MEMORIES:
		{
			if (!p_ptr->hold_life)
			{
				msg_print("You feel your memories fade.");
				lose_skills(10);
				ident = TRUE;
			}
			break;
		}

		case OBJ_POTION_RUINATION:
		{
			msg_print("Your nerves and muscles feel weak and lifeless!");
			take_hit(damroll(10, 10), "a potion of Ruination", MON_HARMFUL_POTION);
			(void)dec_stat(A_DEX, 25, TRUE);
			(void)dec_stat(A_WIS, 25, TRUE);
			(void)dec_stat(A_CON, 25, TRUE);
			(void)dec_stat(A_STR, 25, TRUE);
			(void)dec_stat(A_CHR, 25, TRUE);
			(void)dec_stat(A_INT, 25, TRUE);
			ident = TRUE;
			break;
		}

		case OBJ_POTION_DEC_STR:
		{
			if (do_dec_stat(A_STR)) ident = TRUE;
			break;
		}

		case OBJ_POTION_DEC_INT:
		{
			if (do_dec_stat(A_INT)) ident = TRUE;
			break;
		}

		case OBJ_POTION_DEC_WIS:
		{
			if (do_dec_stat(A_WIS)) ident = TRUE;
			break;
		}

		case OBJ_POTION_DEC_DEX:
		{
			if (do_dec_stat(A_DEX)) ident = TRUE;
			break;
		}

		case OBJ_POTION_DEC_CON:
		{
			if (do_dec_stat(A_CON)) ident = TRUE;
			break;
		}

		case OBJ_POTION_DEC_CHR:
		{
			if (do_dec_stat(A_CHR)) ident = TRUE;
			break;
		}

		case OBJ_POTION_DETONATIONS:
		{
			msg_print("Massive explosions rupture your body!");
			take_hit(damroll(50, 20), "a potion of Detonation", MON_HARMFUL_POTION);
			(void)set_stun(p_ptr->stun + 75);
			(void)set_cut(p_ptr->cut + 5000);
			ident = TRUE;
			break;
		}

		case OBJ_POTION_IOCAINE:
		{
			msg_print("A feeling of Death flows through your body.");
			take_hit(5000, "a potion of Death", MON_HARMFUL_POTION);
			ident = TRUE;
			break;
		}

		case OBJ_POTION_INFRA_VISION:
		{
			if (set_tim_infra(p_ptr->tim_infra + 100 + randint(100)))
			{
				ident = TRUE;
			}
			break;
		}

		case OBJ_POTION_DETECT_INVIS:
		{
			if (set_tim_invis(p_ptr->tim_invis + 12 + randint(12)))
			{
				ident = TRUE;
			}
			break;
		}

		case OBJ_POTION_SLOW_POISON:
		{
			if (set_poisoned(p_ptr->poisoned / 2)) ident = TRUE;
			break;
		}

		case OBJ_POTION_NEUTRALIZE_POISON:
		{
			if (set_poisoned(0)) ident = TRUE;
			break;
		}

		case OBJ_POTION_BOLDNESS:
		{
			if (set_afraid(0)) ident = TRUE;
			break;
		}

		case OBJ_POTION_SPEED:
		{
			if (!p_ptr->fast)
			{
				if (set_fast(randint(25) + 15)) ident = TRUE;
			}
			else
			{
				(void)set_fast(p_ptr->fast + 5);
			}
			break;
		}

		case OBJ_POTION_RES_HEAT:
		{
			if (set_oppose_fire(p_ptr->oppose_fire + randint(10) + 10))
			{
				ident = TRUE;
			}
			break;
		}

		case OBJ_POTION_RES_COLD:
		{
			if (set_oppose_cold(p_ptr->oppose_cold + randint(10) + 10))
			{
				ident = TRUE;
			}
			break;
		}

		case OBJ_POTION_HEROISM:
		{
            if (set_afraid(0)) ident = TRUE;
			if (set_hero(p_ptr->hero + randint(25) + 25)) ident = TRUE;
			if (hp_player(10)) ident = TRUE;
			break;
		}

		case OBJ_POTION_BERSERK_STR:
		{
            if (set_afraid(0)) ident = TRUE;
			if (set_shero(p_ptr->shero + randint(25) + 25)) ident = TRUE;
			if (hp_player(30)) ident = TRUE;
			break;
		}

		case OBJ_POTION_CURE_LIGHT:
		{
			if (hp_player(damroll(2, 8))) ident = TRUE;
			if (set_blind(0)) ident = TRUE;
			if (set_cut(p_ptr->cut - 10)) ident = TRUE;
			break;
		}

		case OBJ_POTION_CURE_SERIOUS:
		{
			if (hp_player(damroll(4, 8))) ident = TRUE;
			if (set_blind(0)) ident = TRUE;
			if (set_confused(0)) ident = TRUE;
			if (set_cut((p_ptr->cut / 2) - 50)) ident = TRUE;
			break;
		}

		case OBJ_POTION_CURE_CRITICAL:
		{
			if (hp_player(damroll(6, 8))) ident = TRUE;
			if (set_blind(0)) ident = TRUE;
			if (set_confused(0)) ident = TRUE;
			if (set_poisoned(0)) ident = TRUE;
			if (set_stun(0)) ident = TRUE;
			if (set_cut(0)) ident = TRUE;
			break;
		}

		case OBJ_POTION_HEALING:
		{
			if (hp_player(300)) ident = TRUE;
			if (set_blind(0)) ident = TRUE;
			if (set_confused(0)) ident = TRUE;
			if (set_poisoned(0)) ident = TRUE;
			if (set_stun(0)) ident = TRUE;
			if (set_cut(0)) ident = TRUE;
			break;
		}

		case OBJ_POTION_STAR_HEALING:
		{
			if (hp_player(1200)) ident = TRUE;
			if (set_blind(0)) ident = TRUE;
			if (set_confused(0)) ident = TRUE;
			if (set_poisoned(0)) ident = TRUE;
			if (set_stun(0)) ident = TRUE;
			if (set_cut(0)) ident = TRUE;
			break;
		}

		case OBJ_POTION_LIFE:
		{
			msg_print("You feel life flow through your body!");
			restore_level();
			hp_player(5000);
			(void)set_poisoned(0);
			(void)set_blind(0);
			(void)set_confused(0);
			(void)set_image(0);
			(void)set_stun(0);
			(void)set_cut(0);
			(void)do_res_stat(A_STR);
			(void)do_res_stat(A_CON);
			(void)do_res_stat(A_DEX);
			(void)do_res_stat(A_WIS);
			(void)do_res_stat(A_INT);
			(void)do_res_stat(A_CHR);
			ident = TRUE;
			break;
		}

		case OBJ_POTION_RES_MANA:
		{
			if (p_ptr->csp < p_ptr->msp)
			{
				p_ptr->csp = p_ptr->msp;
				p_ptr->csp_frac = 0;
				msg_print("Your feel your head clear.");
				p_ptr->redraw |= (PR_MANA);
				p_ptr->window |= (PW_PLAYER);
				p_ptr->window |= (PW_SPELL);
				ident = TRUE;
			}
			if (p_ptr->cchi < p_ptr->mchi)
			{
				p_ptr->cchi = p_ptr->mchi;
				p_ptr->chi_frac = 0;
				msg_print("Your feel your chi harmonise.");
				p_ptr->redraw |= (PR_MANA);
				p_ptr->window |= (PW_PLAYER);
				p_ptr->window |= (PW_SPELL);
				ident = TRUE;
			}
			break;
		}

		case OBJ_POTION_RES_LIFE_LEVELS:
		{
			if (restore_level()) ident = TRUE;
			break;
		}

		case OBJ_POTION_RES_STR:
		{
			if (do_res_stat(A_STR)) ident = TRUE;
			break;
		}

		case OBJ_POTION_RES_INT:
		{
			if (do_res_stat(A_INT)) ident = TRUE;
			break;
		}

		case OBJ_POTION_RES_WIS:
		{
			if (do_res_stat(A_WIS)) ident = TRUE;
			break;
		}

		case OBJ_POTION_RES_DEX:
		{
			if (do_res_stat(A_DEX)) ident = TRUE;
			break;
		}

		case OBJ_POTION_RES_CON:
		{
			if (do_res_stat(A_CON)) ident = TRUE;
			break;
		}

		case OBJ_POTION_RES_CHR:
		{
			if (do_res_stat(A_CHR)) ident = TRUE;
			break;
		}

		case OBJ_POTION_INC_STR:
		{
			if (do_inc_stat(A_STR)) ident = TRUE;
			break;
		}

		case OBJ_POTION_INC_INT:
		{
			if (do_inc_stat(A_INT)) ident = TRUE;
			break;
		}

		case OBJ_POTION_INC_WIS:
		{
			if (do_inc_stat(A_WIS)) ident = TRUE;
			break;
		}

		case OBJ_POTION_INC_DEX:
		{
			if (do_inc_stat(A_DEX)) ident = TRUE;
			break;
		}

		case OBJ_POTION_INC_CON:
		{
			if (do_inc_stat(A_CON)) ident = TRUE;
			break;
		}

		case OBJ_POTION_INC_CHR:
		{
			if (do_inc_stat(A_CHR)) ident = TRUE;
			break;
		}

		case OBJ_POTION_AUGMENTATION:
		{
			if (do_inc_stat(A_STR)) ident = TRUE;
			if (do_inc_stat(A_INT)) ident = TRUE;
			if (do_inc_stat(A_WIS)) ident = TRUE;
			if (do_inc_stat(A_DEX)) ident = TRUE;
			if (do_inc_stat(A_CON)) ident = TRUE;
			if (do_inc_stat(A_CHR)) ident = TRUE;
			break;
		}

		case OBJ_POTION_ENLIGHTENMENT:
		{
			msg_print("An image of your surroundings forms in your mind...");
			wiz_lite();
			ident = TRUE;
			break;
		}

		case OBJ_POTION_STAR_ENLIGHTENMENT:
		{
			msg_print("You begin to feel more enlightened...");
			msg_print(NULL);
			wiz_lite();
			(void)do_inc_stat(A_INT);
			(void)do_inc_stat(A_WIS);
			(void)detect_traps();
			mark_traps();
			(void)detect_doors();
			(void)detect_stairs();
			(void)detect_treasure();
			(void)detect_objects_gold();
			(void)detect_objects_normal();
			identify_pack();
			self_knowledge();
			ident = TRUE;
			break;
		}

		case OBJ_POTION_SELF_KNOWLEDGE:
		{
			msg_print("You begin to know yourself a little better...");
			msg_print(NULL);
			self_knowledge();
			ident = TRUE;
			break;
		}

		case OBJ_POTION_EXPERIENCE:
		{
				msg_print("You feel more experienced.");
				gain_skills(200);
				ident = TRUE;
			break;
		}

        case OBJ_POTION_RESISTANCE:
        {
            (void)set_oppose_acid(p_ptr->oppose_acid + randint(20) + 20);
			(void)set_oppose_elec(p_ptr->oppose_elec + randint(20) + 20);
			(void)set_oppose_fire(p_ptr->oppose_fire + randint(20) + 20);
			(void)set_oppose_cold(p_ptr->oppose_cold + randint(20) + 20);
			(void)set_oppose_pois(p_ptr->oppose_pois + randint(20) + 20);
            ident = TRUE;
        break;
        }

        case OBJ_POTION_CURING:
        {
            if (hp_player(50)) ident = TRUE;
            if (set_blind(0)) ident = TRUE;
			if (set_poisoned(0)) ident = TRUE;
			if (set_confused(0)) ident = TRUE;
			if (set_stun(0)) ident = TRUE;
			if (set_cut(0)) ident = TRUE;
            if (set_image(0)) ident = TRUE;
        break;
        }

        case OBJ_POTION_INVULNERABILITY:
        {
            (void)set_invuln(p_ptr->invuln + randint(7) + 7);
            ident = TRUE;
        break;
        }

        case OBJ_POTION_NEW_LIFE:
        {
        do_cmd_rerate();
        if (p_ptr->muta1 || p_ptr->muta2 || p_ptr->muta3)
        {
            msg_print("You are cured of all chaos features.");
            p_ptr->muta1 = p_ptr->muta2 = p_ptr->muta3 = 0;
            p_ptr->update |= PU_BONUS;
            handle_stuff();
        }
        ident = TRUE;
        break;
        }


	}

    if ((p_ptr->prace == RACE_SKELETON) && (randint(12)==1))
    {
        msg_print("Some of the fluid falls through your jaws!");
        potion_smash_effect(0, py, px, o_ptr->k_idx);
    }

	/* Combine / Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	/* The item has been tried */
	object_tried(o_ptr);

	/* An identification was made */
	if (ident && !object_aware_p(o_ptr))
	{
		object_aware(o_ptr);
	}

	/* Window stuff */
	p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_PLAYER);


	/* Potions can feed the player */
	(void)set_food(p_ptr->food + o_ptr->pval);


	/* Destroy a potion */
	item_increase(o_ptr, -1);
	item_describe(o_ptr);
	item_optimize(o_ptr);
}


/*
 * Curse the players armor
 */
bool curse_armor(void)
{
	object_type *o_ptr;

	C_TNEW(o_name, ONAME_MAX, char);


	/* Curse the body armor */
	o_ptr = &inventory[INVEN_BODY];

	/* Nothing to curse */
	if (!o_ptr->k_idx)
	{
		TFREE(o_name);
		return (FALSE);
	}


	/* Describe */
	strnfmt(o_name, ONAME_MAX, "%v", object_desc_f3, o_ptr, FALSE, 3);

	/* Attempt a saving throw for artifacts */
    if (allart_p(o_ptr) && (rand_int(100) < 50))
	{
		/* Cool */
		msg_format("A %s tries to %s, but your %s resists the effects!",
		           "terrible black aura", "surround your armor", o_name);
	}

	/* not artifact or failed save... */
	else
	{
		/* Oops */
		msg_format("A terrible black aura blasts your %s!", o_name);

		/* Blast the armor */
		o_ptr->name1 = 0;
		o_ptr->name2 = EGO_BLASTED;
		o_ptr->to_a = 0 - randint(5) - randint(5);
		o_ptr->to_h = 0;
		o_ptr->to_d = 0;
		o_ptr->ac = 0;
		o_ptr->dd = 0;
		o_ptr->ds = 0;
        o_ptr->flags1 = 0;
        o_ptr->flags2 = 0;
        o_ptr->flags3 = 0;
		o_ptr->art_name = 0;

		/* Curse it */
		o_ptr->ident |= (IDENT_CURSED);

		/* Break it */
		o_ptr->ident |= (IDENT_BROKEN);

		/* Tell the player the bad news */
		o_ptr->ident |= (IDENT_SENSE);

		/* Recalculate bonuses */
		p_ptr->update |= (PU_BONUS);

		/* Recalculate mana */
		p_ptr->update |= (PU_MANA);

		/* Window stuff */
		p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_PLAYER);
	}

	TFREE(o_name);

	return (TRUE);
}


/*
 * Curse the players weapon
 */
bool curse_weapon(void)
{
	object_type *o_ptr;

	C_TNEW(o_name, ONAME_MAX, char);


	/* Curse the weapon */
	o_ptr = &inventory[INVEN_WIELD];

	/* Nothing to curse */
	if (!o_ptr->k_idx)
	{
		TFREE(o_name);
		return (FALSE);
	}


	/* Describe */
	strnfmt(o_name, ONAME_MAX, "%v", object_desc_f3, o_ptr, FALSE, 3);

	/* Attempt a saving throw */
    if (allart_p(o_ptr) && (rand_int(100) < 50))
	{
		/* Cool */
		msg_format("A %s tries to %s, but your %s resists the effects!",
		           "terrible black aura", "surround your weapon", o_name);
	}

	/* not artifact or failed save... */
	else
	{
		/* Oops */
		msg_format("A terrible black aura blasts your %s!", o_name);

		/* Shatter the weapon */
		o_ptr->name1 = 0;
		o_ptr->name2 = EGO_SHATTERED;
		o_ptr->to_h = 0 - randint(5) - randint(5);
		o_ptr->to_d = 0 - randint(5) - randint(5);
		o_ptr->to_a = 0;
		o_ptr->ac = 0;
		o_ptr->dd = 0;
		o_ptr->ds = 0;
        o_ptr->flags1 = 0;
        o_ptr->flags2 = 0;
        o_ptr->flags3 = 0;
		o_ptr->art_name = 0;


		/* Curse it */
		o_ptr->ident |= (IDENT_CURSED);

		/* Break it */
		o_ptr->ident |= (IDENT_BROKEN);

		/* Tell the player the bad news */
		o_ptr->ident |= (IDENT_SENSE);
		
		/* Recalculate bonuses */
		p_ptr->update |= (PU_BONUS);

		/* Recalculate mana */
		p_ptr->update |= (PU_MANA);

		/* Window stuff */
		p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_PLAYER);
	}

	TFREE(o_name);

	/* Notice */
	return (TRUE);
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
	int			k, used_up, ident;

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

	/* Not identified yet */
	ident = FALSE;

	/* Assume the scroll will get used up */
	used_up = TRUE;

	/* Analyze the scroll */
	switch (o_ptr->k_idx)
	{
		case OBJ_SCROLL_DARKNESS:
		{
            if (!(p_ptr->resist_blind) && !(p_ptr->resist_dark))
			{
				(void)set_blind(p_ptr->blind + 3 + randint(5));
			}
			if (unlite_area(10, 3)) ident = TRUE;
			break;
		}

		case OBJ_SCROLL_AGGRAVATE_MONSTER:
		{
			msg_print("There is a high pitched humming noise.");
			aggravate_monsters(1);
			ident = TRUE;
			break;
		}

		case OBJ_SCROLL_CURSE_ARMOUR:
		{
			if (curse_armor()) ident = TRUE;
			break;
		}

		case OBJ_SCROLL_CURSE_WEAPON:
		{
			if (curse_weapon()) ident = TRUE;
			break;
		}

		case OBJ_SCROLL_SUMMON_MONSTER:
		{
			switch (rand_int(9))
			{
				case 0: case 1: case 2: k = 1; break;
				case 3: case 4: case 5: case 6: k = 2; break;
				default: k = 3;
			}

			for (; k; k--)
			{
				if (summon_specific(py, px, (dun_depth), 0))
				{
					ident = TRUE;
				}
			}
			break;
		}

		case OBJ_SCROLL_SUMMON_UNDEAD:
		{
			switch (rand_int(9))
			{
				case 0: case 1: case 2: k = 1; break;
				case 3: case 4: case 5: case 6: k = 2; break;
				default: k = 3;
			}

			for (; k; k--)
			{
				if (summon_specific(py, px, (dun_depth), SUMMON_UNDEAD))
				{
					ident = TRUE;
				}
			}
			break;
		}

		case OBJ_SCROLL_TRAP_CREATION:
		{
			if (trap_creation()) ident = TRUE;
			break;
		}

		case OBJ_SCROLL_PHASE_DOOR:
		{
			teleport_player(10);
			ident = TRUE;
			break;
		}

		case OBJ_SCROLL_TELEPORTATION:
		{
			teleport_player(100);
			ident = TRUE;
			break;
		}

		case OBJ_SCROLL_TELEPORT_LEVEL:
		{
			(void)teleport_player_level();
			ident = TRUE;
			break;
		}

		case OBJ_SCROLL_WORD_OF_RECALL:
		{
			set_recall(FALSE);
			ident = TRUE;
			break;
		}

		case OBJ_SCROLL_IDENTIFY:
		{
			ident = TRUE;
			if (!ident_spell()) used_up = FALSE;
			break;
		}

		case OBJ_SCROLL_STAR_IDENTIFY:
		{
			ident = TRUE;
			if (!identify_fully()) used_up = FALSE;
			break;
		}

		case OBJ_SCROLL_REMOVE_CURSE:
		{
			if (remove_curse())
			{
				msg_print("You feel as if someone is watching over you.");
				ident = TRUE;
			}
			break;
		}

		case OBJ_SCROLL_STAR_REMOVE_CURSE:
		{
			remove_all_curse();
			ident = TRUE;
			break;
		}

		case OBJ_SCROLL_ENCHANT_ARMOUR:
		{
			ident = TRUE;
			if (!enchant_spell(0, 0, 1)) used_up = FALSE;
			break;
		}

		case OBJ_SCROLL_ENCHANT_WEAPON_TO_HIT:
		{
			if (!enchant_spell(1, 0, 0)) used_up = FALSE;
			ident = TRUE;
			break;
		}

		case OBJ_SCROLL_ENCHANT_WEAPON_TO_DAM:
		{
			if (!enchant_spell(0, 1, 0)) used_up = FALSE;
			ident = TRUE;
			break;
		}

		case OBJ_SCROLL_STAR_ENCHANT_ARMOUR:
		{
			if (!enchant_spell(0, 0, randint(3) + 2)) used_up = FALSE;
			ident = TRUE;
			break;
		}

		case OBJ_SCROLL_STAR_ENCHANT_WEAPON:
		{
			if (!enchant_spell(randint(3), randint(3), 0)) used_up = FALSE;
			ident = TRUE;
			break;
		}

		case OBJ_SCROLL_RECHARGING:
		{
			if (!recharge(60)) used_up = FALSE;
			ident = TRUE;
			break;
		}

		case OBJ_SCROLL_LIGHT:
		{
			if (lite_area(damroll(2, 8), 2)) ident = TRUE;
			break;
		}

		case OBJ_SCROLL_MAGIC_MAPPING:
		{
			map_area();
			ident = TRUE;
			break;
		}

		case OBJ_SCROLL_TREASURE_DETECTION:
		{
			if (detect_treasure()) ident = TRUE;
			if (detect_objects_gold()) ident = TRUE;
			break;
		}

		case OBJ_SCROLL_OBJECT_DETECTION:
		{
			if (detect_objects_normal()) ident = TRUE;
			break;
		}

		case OBJ_SCROLL_TRAP_DETECTION:
		{
			if (detect_traps()) ident = TRUE;
			if (object_aware_p(o_ptr) || ident) mark_traps();
			break;
		}

		case OBJ_SCROLL_DOOR_STAIR_LOCATION:
		{
			if (detect_doors()) ident = TRUE;
			if (detect_stairs()) ident = TRUE;
			break;
		}

		case OBJ_SCROLL_DETECT_INVIS:
		{
			if (detect_monsters_invis()) ident = TRUE;
			break;
		}

		case OBJ_SCROLL_SATISFY_HUNGER:
		{
			if (set_food(PY_FOOD_MAX - 1)) ident = TRUE;
			break;
		}

		case OBJ_SCROLL_BLESSING:
		{
			if (set_blessed(p_ptr->blessed + randint(12) + 6)) ident = TRUE;
			break;
		}

		case OBJ_SCROLL_HOLY_CHANT:
		{
			if (set_blessed(p_ptr->blessed + randint(24) + 12)) ident = TRUE;
			break;
		}

		case OBJ_SCROLL_HOLY_PRAYER:
		{
			if (set_blessed(p_ptr->blessed + randint(48) + 24)) ident = TRUE;
			break;
		}

		case OBJ_SCROLL_MONSTER_CONFUSION:
		{
			if (p_ptr->confusing == 0)
			{
				msg_print("Your hands begin to glow.");
				p_ptr->confusing = TRUE;
				ident = TRUE;
			}
			break;
		}

		case OBJ_SCROLL_PROTECTION_FROM_EVIL:
		{
			k = 3 * (skill_set[SKILL_DEVICE].value/2);
			if (set_protevil(p_ptr->protevil + randint(25) + k)) ident = TRUE;
			break;
		}

		case OBJ_SCROLL_RUNE_OF_PROTECTION:
		{
			warding_glyph();
			ident = TRUE;
			break;
		}

		case OBJ_SCROLL_TRAP_DOOR_DESTRUCTION:
		{
			if (destroy_doors_touch()) ident = TRUE;
			break;
		}

		case OBJ_SCROLL_STAR_DESTRUCTION:
		{
			destroy_area(py, px, 15, TRUE);
			ident = TRUE;
			break;
		}

		case OBJ_SCROLL_DISPEL_UNDEAD:
		{
			if (dispel_undead(60)) ident = TRUE;
			break;
		}

		case OBJ_SCROLL_GENOCIDE:
		{
			(void)genocide(TRUE);
			ident = TRUE;
			break;
		}

		case OBJ_SCROLL_MASS_GENOCIDE:
		{
			(void)mass_genocide(TRUE);
			ident = TRUE;
			break;
		}

		case OBJ_SCROLL_ACQUIREMENT:
		{
			acquirement(py, px, 1, TRUE);
			ident = TRUE;
			break;
		}

		case OBJ_SCROLL_STAR_ACQUIREMENT:
		{
			acquirement(py, px, randint(2) + 1, TRUE);
			ident = TRUE;
			break;
		}

        /* New Zangband scrolls */
        case OBJ_SCROLL_FIRE:
        {
            fire_ball(GF_FIRE, 0,
                150, 4); /* Note: "Double" damage since it is centered on
                            the player ... */
            if (!(p_ptr->oppose_fire || p_ptr->resist_fire || p_ptr->immune_fire))
                take_hit(50+randint(50), "a Scroll of Fire", MON_HARMFUL_SCROLL);
            ident = TRUE;
            break;
        }


        case OBJ_SCROLL_ICE:
        {
            fire_ball(GF_ICE, 0,
                175, 4);
            if (!(p_ptr->oppose_cold || p_ptr->resist_cold || p_ptr->immune_cold))
                take_hit(100+randint(100), "a Scroll of Ice", MON_HARMFUL_SCROLL);
            ident = TRUE;
            break;
        }

        case OBJ_SCROLL_CHAOS:
        {
            fire_ball(GF_CHAOS, 0,
                222, 4);
            if (!p_ptr->resist_chaos)
                take_hit(111+randint(111), "a Scroll of Chaos", MON_HARMFUL_SCROLL);
            ident = TRUE;
            break;
        }

        case OBJ_SCROLL_RUMOUR:
        {
            msg_print("There is message on the scroll. It says:");
            msg_print(NULL);
            switch(randint(20))
            {   case 1:
                    msg_format("%v", get_rnd_line_f1, "chainswd.txt");
                    break;
                case 2:
                    msg_format("%v", get_rnd_line_f1, "error.txt");
                    break;
                case 3: case 4: case 5:
                    msg_format("%v", get_rnd_line_f1, "death.txt");
                    break;
                default:
                    msg_format("%v", get_rnd_line_f1, "rumors.txt");

            }
            msg_print(NULL);
            msg_print("The scroll disappears in a puff of smoke!");
            ident = TRUE;
            break;
        }

        case OBJ_SCROLL_ARTIFACT_CREATION:
        {
            (void) artifact_scroll();
            ident = TRUE;
            break;
        }
	}


	/* Combine / Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	/* The item was tried */
	object_tried(o_ptr);

	/* An identification was made */
	if (ident && !object_aware_p(o_ptr))
	{
		object_aware(o_ptr);
	}

	/* Window stuff */
	p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_PLAYER);


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
	int			ident, k;

	/* Hack -- let staffs of identify get aborted */
	bool use_charge = TRUE;


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

	/* Not identified yet */
	ident = FALSE;

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
	switch (o_ptr->k_idx)
	{
		case OBJ_STAFF_DARKNESS:
		{
            if (!(p_ptr->resist_blind) && !(p_ptr->resist_dark))
			{
				if (set_blind(p_ptr->blind + 3 + randint(5))) ident = TRUE;
			}
			if (unlite_area(10, 3)) ident = TRUE;
			break;
		}

		case OBJ_STAFF_SLOWNESS:
		{
			if (set_slow(p_ptr->slow + randint(30) + 15)) ident = TRUE;
			break;
		}

		case OBJ_STAFF_HASTE_MONSTERS:
		{
			if (speed_monsters()) ident = TRUE;
			break;
		}

		case OBJ_STAFF_SUMMONING:
		{
			switch (rand_int(32))
			{
				case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
					k = 1; break;
				case 20: case 21: case 22: case 23: case 24: case 25: case 26:
				case 27: case 28:
					k = 3; break;
				case 29: case 30: case 31:
					k = 4; break;
				default: k = 2; break;
			}
			for (; k; k--)
			{
				if (summon_specific(py, px, (dun_depth), 0))
				{
					ident = TRUE;
				}
			}
			break;
		}

		case OBJ_STAFF_TELEPORTATION:
		{
			teleport_player(100);
			ident = TRUE;
			break;
		}

		case OBJ_STAFF_PERCEPTION:
		{
			if (!ident_spell()) use_charge = FALSE;
			ident = TRUE;
			break;
		}

		case OBJ_STAFF_REMOVE_CURSE:
		{
			if (remove_curse())
			{
				if (!p_ptr->blind)
				{
					msg_print("The staff glows blue for a moment...");
				}
				ident = TRUE;
			}
			break;
		}

		case OBJ_STAFF_STARLIGHT:
		{
			if (!p_ptr->blind)
			{
				msg_print("The end of the staff glows brightly...");
			}
			for (k = 0; k < 8; k++) lite_line(ddd[k]);
			ident = TRUE;
			break;
		}

		case OBJ_STAFF_LIGHT:
		{
			if (lite_area(damroll(2, 8), 2)) ident = TRUE;
			break;
		}

		case OBJ_STAFF_ENLIGHTENMENT:
		{
			map_area();
			ident = TRUE;
			break;
		}

		case OBJ_STAFF_TREASURE_LOCATION:
		{
			if (detect_treasure()) ident = TRUE;
			if (detect_objects_gold()) ident = TRUE;
			break;
		}

		case OBJ_STAFF_OBJECT_LOCATION:
		{
			if (detect_objects_normal()) ident = TRUE;
			break;
		}

		case OBJ_STAFF_TRAP_LOCATION:
		{
			if (detect_traps()) ident = TRUE;
			if (object_aware_p(o_ptr) || ident) mark_traps();
			break;
		}

		case OBJ_STAFF_DOOR_STAIR_LOCATION:
		{
			if (detect_doors()) ident = TRUE;
			if (detect_stairs()) ident = TRUE;
			break;
		}

		case OBJ_STAFF_DETECT_INVIS:
		{
			if (detect_monsters_invis()) ident = TRUE;
			break;
		}

		case OBJ_STAFF_DETECT_EVIL:
		{
			if (detect_monsters_evil()) ident = TRUE;
			break;
		}

		case OBJ_STAFF_CURE_LIGHT:
		{
			if (hp_player(randint(8))) ident = TRUE;
			break;
		}

		case OBJ_STAFF_CURING:
		{
			if (set_blind(0)) ident = TRUE;
			if (set_poisoned(0)) ident = TRUE;
			if (set_confused(0)) ident = TRUE;
			if (set_stun(0)) ident = TRUE;
			if (set_cut(0)) ident = TRUE;
            if (set_image(0)) ident = TRUE;
			break;
		}

		case OBJ_STAFF_HEALING:
		{
			if (hp_player(300)) ident = TRUE;
			if (set_stun(0)) ident = TRUE;
			if (set_cut(0)) ident = TRUE;
			break;
		}

		case OBJ_STAFF_THE_MAGI:
		{
			if (do_res_stat(A_INT)) ident = TRUE;
			if (p_ptr->csp < p_ptr->msp)
			{
				p_ptr->csp = p_ptr->msp;
				p_ptr->csp_frac = 0;
				ident = TRUE;
				msg_print("Your feel your head clear.");
				p_ptr->redraw |= (PR_MANA);
				p_ptr->window |= (PW_PLAYER);
				p_ptr->window |= (PW_SPELL);
			}
			if (p_ptr->cchi < p_ptr->mchi)
			{
				p_ptr->cchi = p_ptr->mchi;
				p_ptr->chi_frac = 0;
				ident = TRUE;
				msg_print("Your feel your chi harmonise.");
				p_ptr->redraw |= (PR_MANA);
				p_ptr->window |= (PW_PLAYER);
				p_ptr->window |= (PW_SPELL);
			}
			break;
		}

		case OBJ_STAFF_SLEEP_MONSTERS:
		{
			if (sleep_monsters((skill_set[SKILL_DEVICE].value/2))) ident = TRUE;
			break;
		}

		case OBJ_STAFF_SLOW_MONSTERS:
		{
			if (slow_monsters((skill_set[SKILL_DEVICE].value/2))) ident = TRUE;
			break;
		}

		case OBJ_STAFF_SPEED:
		{
			if (!p_ptr->fast)
			{
				if (set_fast(randint(30) + 15)) ident = TRUE;
			}
			else
			{
				(void)set_fast(p_ptr->fast + 5);
			}
			break;
		}

		case OBJ_STAFF_PROBING:
		{
			probing();
			ident = TRUE;
			break;
		}

		case OBJ_STAFF_DISPEL_EVIL:
		{
			if (dispel_evil(60)) ident = TRUE;
			break;
		}

		case OBJ_STAFF_POWER:
		{
			if (dispel_monsters(120)) ident = TRUE;
			break;
		}

		case OBJ_STAFF_HOLINESS:
		{
			if (dispel_evil(120)) ident = TRUE;
			k = 3 * (skill_set[SKILL_DEVICE].value/2);
			if (set_protevil(p_ptr->protevil + randint(25) + k)) ident = TRUE;
			if (set_poisoned(0)) ident = TRUE;
			if (set_afraid(0)) ident = TRUE;
			if (hp_player(50)) ident = TRUE;
			if (set_stun(0)) ident = TRUE;
			if (set_cut(0)) ident = TRUE;
			break;
		}

		case OBJ_STAFF_GENOCIDE:
		{
			(void)genocide(TRUE);
			ident = TRUE;
			break;
		}

		case OBJ_STAFF_EARTHQUAKES:
		{
			earthquake(py, px, 10);
			ident = TRUE;
			break;
		}

		case OBJ_STAFF_STAR_DESTRUCTION:
		{
			destroy_area(py, px, 15, TRUE);
			ident = TRUE;
			break;
		}
	}


	/* Combine / Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	/* Tried the item */
	object_tried(o_ptr);

	/* An identification was made */
	if (ident && !object_aware_p(o_ptr))
	{
		object_aware(o_ptr);
	}

	/* Window stuff */
	p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_PLAYER);


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
 * Choose a random wand from a list for the effect of a wand of wonder.
 */
static int choose_random_wand(void)
{
	int low_wands[] =
	{
		OBJ_WAND_LIGHT,
		OBJ_WAND_TAME_MONSTER,
		OBJ_WAND_COLD_BOLT,
		OBJ_WAND_FIRE_BOLT,
		OBJ_WAND_STONE_TO_MUD,
		OBJ_WAND_POLYMORPH,
		OBJ_WAND_HEAL_MONSTER,
		OBJ_WAND_HASTE_MONSTER,
		OBJ_WAND_SLOW_MONSTER,
		OBJ_WAND_CONFUSE_MONSTER,
		OBJ_WAND_SLEEP_MONSTER,
		OBJ_WAND_DRAIN_LIFE,
		OBJ_WAND_TRAP_DOOR_DESTRUCTION,
		OBJ_WAND_MAGIC_MISSILE,
		OBJ_WAND_CLONE_MONSTER,
		OBJ_WAND_SCARE_MONSTER,
		OBJ_WAND_TELEPORT_OTHER,
		OBJ_WAND_DISARMING,
		OBJ_WAND_ELEC_BALL,
		OBJ_WAND_COLD_BALL,
		OBJ_WAND_FIRE_BALL,
		OBJ_WAND_STINKING_CLOUD,
		OBJ_WAND_ACID_BALL,
		OBJ_WAND_ACID_BOLT,
	};
	return low_wands[rand_int(N_ELEMENTS(low_wands))];
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
	int item;
	int			ident, k_idx, dir;


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

	/* Not identified yet */
	ident = FALSE;

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

	/* Gain exp */
	skill_exp(SKILL_DEVICE);


	/* XXX Hack -- Extract the "k_idx" effect */
	k_idx = o_ptr->k_idx;

	/* XXX Hack -- Wand of wonder can do anything before it */
	if (k_idx == OBJ_WAND_WONDER)
		k_idx = choose_random_wand();

	/* Analyze the wand */
	switch (k_idx)
	{
		case OBJ_WAND_HEAL_MONSTER:
		{
			if (heal_monster(dir)) ident = TRUE;
			break;
		}

		case OBJ_WAND_HASTE_MONSTER:
		{
			if (speed_monster(dir,(skill_set[SKILL_DEVICE].value/2))) ident = TRUE;
			break;
		}

		case OBJ_WAND_CLONE_MONSTER:
		{
			if (clone_monster(dir)) ident = TRUE;
			break;
		}

		case OBJ_WAND_TELEPORT_OTHER:
		{
			if (teleport_monster(dir)) ident = TRUE;
			break;
		}

		case OBJ_WAND_DISARMING:
		{
			if (disarm_trap(dir)) ident = TRUE;
			break;
		}

		case OBJ_WAND_TRAP_DOOR_DESTRUCTION:
		{
			if (destroy_door(dir)) ident = TRUE;
			break;
		}

		case OBJ_WAND_STONE_TO_MUD:
		{
			if (wall_to_mud(dir)) ident = TRUE;
			break;
		}

		case OBJ_WAND_LIGHT:
		{
			msg_print("A line of blue shimmering light appears.");
			lite_line(dir);
			ident = TRUE;
			break;
		}

		case OBJ_WAND_SLEEP_MONSTER:
		{
			if (sleep_monster(dir,(skill_set[SKILL_DEVICE].value/2))) ident = TRUE;
			break;
		}

		case OBJ_WAND_SLOW_MONSTER:
		{
			if (slow_monster(dir,(skill_set[SKILL_DEVICE].value/2))) ident = TRUE;
			break;
		}

		case OBJ_WAND_CONFUSE_MONSTER:
		{
			if (confuse_monster(dir, 10)) ident = TRUE;
			break;
		}

		case OBJ_WAND_SCARE_MONSTER:
		{
			if (fear_monster(dir, 10)) ident = TRUE;
			break;
		}

		case OBJ_WAND_DRAIN_LIFE:
		{
			if (drain_life(dir, 75)) ident = TRUE;
			break;
		}

		case OBJ_WAND_POLYMORPH:
		{
			if (poly_monster(dir,(skill_set[SKILL_DEVICE].value/2))) ident = TRUE;
			break;
		}

		case OBJ_WAND_STINKING_CLOUD:
		{
			fire_ball(GF_POIS, dir, 12, 2);
			ident = TRUE;
			break;
		}

		case OBJ_WAND_MAGIC_MISSILE:
		{
			fire_bolt_or_beam(20, GF_MISSILE, dir, damroll(2, 6));
			ident = TRUE;
			break;
		}

		case OBJ_WAND_ACID_BOLT:
		{
            fire_bolt_or_beam(20, GF_ACID, dir, damroll(3, 8));
			ident = TRUE;
			break;
        }

        case OBJ_WAND_TAME_MONSTER:
		{
           if (charm_monster(dir, 45))
             ident = TRUE;
           break;
        }

		case OBJ_WAND_FIRE_BOLT:
		{
			fire_bolt_or_beam(20, GF_FIRE, dir, damroll(6, 8));
			ident = TRUE;
			break;
		}

		case OBJ_WAND_COLD_BOLT:
		{
			fire_bolt_or_beam(20, GF_COLD, dir, damroll(3, 8));
			ident = TRUE;
			break;
		}

		case OBJ_WAND_ACID_BALL:
		{
			fire_ball(GF_ACID, dir, 60, 2);
			ident = TRUE;
			break;
		}

		case OBJ_WAND_ELEC_BALL:
		{
            fire_ball(GF_ELEC, dir, 32, 2);
			ident = TRUE;
			break;
		}

		case OBJ_WAND_FIRE_BALL:
		{
			fire_ball(GF_FIRE, dir, 72, 2);
			ident = TRUE;
			break;
		}

		case OBJ_WAND_COLD_BALL:
		{
			fire_ball(GF_COLD, dir, 48, 2);
			ident = TRUE;
			break;
		}

		case OBJ_WAND_WONDER:
		{
			msg_print("Oops.  Wand of wonder activated.");
			break;
		}

		case OBJ_WAND_DRAGON_FIRE:
		{
			fire_ball(GF_FIRE, dir, 100, 3);
			ident = TRUE;
			break;
		}

		case OBJ_WAND_DRAGON_COLD:
		{
			fire_ball(GF_COLD, dir, 80, 3);
			ident = TRUE;
			break;
		}

		case OBJ_WAND_DRAGON_BREATH:
		{
			switch (randint(5))
			{
				case 1:
				{
					fire_ball(GF_ACID, dir, 100, -3);
					break;
				}

				case 2:
				{
					fire_ball(GF_ELEC, dir, 80, -3);
					break;
				}

				case 3:
				{
					fire_ball(GF_FIRE, dir, 100, -3);
					break;
				}

				case 4:
				{
					fire_ball(GF_COLD, dir, 80, -3);
					break;
				}

				default:
				{
					fire_ball(GF_POIS, dir, 60, -3);
					break;
				}
			}

			ident = TRUE;
			break;
		}

		case OBJ_WAND_ANNIHILATION:
		{
			if (drain_life(dir, 125)) ident = TRUE;
			break;
		}
        case OBJ_WAND_SHARD_BALL:
        {
            fire_ball(GF_SHARD, dir, 75 + (randint(50)), 2);
            ident = TRUE;
			break;
        }
	}


	/* Combine / Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	/* Mark it as tried */
	object_tried(o_ptr);

	/* Apply identification */
	if (ident && !object_aware_p(o_ptr))
	{
		object_aware(o_ptr);
	}

	/* Window stuff */
	p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_PLAYER);


	/* Use a single charge */
	o_ptr->pval--;

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
		item = inven_carry(q_ptr);

		/* Message */
		msg_print("You unstack your wand.");
	}

	/* Describe the charges in the pack */
	item_charges(o_ptr);
}


/*
 * Recognise a directional rod. Should match do_cmd_zap_rod().
 */
static bool directional_rod_p(s16b k_idx)
{
	switch (k_idx)
	{
		case OBJ_ROD_TRAP_LOCATION:
		case OBJ_ROD_DOOR_STAIR_LOCATION:
		case OBJ_ROD_PERCEPTION:
		case OBJ_ROD_RECALL:
		case OBJ_ROD_ILLUMINATION:
		case OBJ_ROD_ENLIGHTENMENT:
		case OBJ_ROD_DETECTION:
		case OBJ_ROD_PROBING:
		case OBJ_ROD_CURING:
		case OBJ_ROD_HEALING:
		case OBJ_ROD_RESTORATION:
		case OBJ_ROD_SPEED:
		case OBJ_ROD_TELEPORT_OTHER:
        case OBJ_ROD_HAVOC:
		{
			return FALSE;
		}
		case OBJ_ROD_DISARMING:
		case OBJ_ROD_LIGHT:
		case OBJ_ROD_SLEEP_MONSTER:
		case OBJ_ROD_SLOW_MONSTER:
		case OBJ_ROD_DRAIN_LIFE:
		case OBJ_ROD_POLYMORPH:
		case OBJ_ROD_ACID_BOLT:
		case OBJ_ROD_ELEC_BOLT:
		case OBJ_ROD_FIRE_BOLT:
		case OBJ_ROD_COLD_BOLT:
		case OBJ_ROD_ACID_BALL:
		case OBJ_ROD_ELEC_BALL:
		case OBJ_ROD_FIRE_BALL:
		case OBJ_ROD_COLD_BALL:
		{
			return TRUE;
		}
		default:
		{
			return FALSE;
		}
	}
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
	int			ident, dir;

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
	if (directional_rod_p(o_ptr->k_idx) || !object_aware_p(o_ptr))
	{
		/* Get a direction, allow cancel */
		if (!get_aim_dir(&dir)) return;
	}


	/* Take a turn */
	energy_use = item_use_energy(o_ptr);

	/* Not identified yet */
	ident = FALSE;

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

	/* Gain exp */
	skill_exp(SKILL_DEVICE);




	/* Analyze the rod */
	switch (o_ptr->k_idx)
	{
		case OBJ_ROD_TRAP_LOCATION:
		{
			if (detect_traps()) ident = TRUE;
			if (object_aware_p(o_ptr) || ident) mark_traps();
			break;
		}

		case OBJ_ROD_DOOR_STAIR_LOCATION:
		{
			if (detect_doors()) ident = TRUE;
			if (detect_stairs()) ident = TRUE;
			break;
		}

		case OBJ_ROD_PERCEPTION:
		{
			ident = TRUE;
			if (!ident_spell()) use_charge = FALSE;
			break;
		}

		case OBJ_ROD_RECALL:
		{
			set_recall(FALSE);
			ident = TRUE;
			break;
		}

		case OBJ_ROD_ILLUMINATION:
		{
			if (lite_area(damroll(2, 8), 2)) ident = TRUE;
 			break;
		}

		case OBJ_ROD_ENLIGHTENMENT:
		{
			map_area();
			ident = TRUE;
			break;
		}

		case OBJ_ROD_DETECTION:
		{
			detect_all();
			ident = TRUE;
			break;
		}

		case OBJ_ROD_PROBING:
		{
			probing();
			ident = TRUE;
			break;
		}

		case OBJ_ROD_CURING:
		{
			if (set_blind(0)) ident = TRUE;
			if (set_poisoned(0)) ident = TRUE;
			if (set_confused(0)) ident = TRUE;
			if (set_stun(0)) ident = TRUE;
			if (set_cut(0)) ident = TRUE;
            if (set_image(0)) ident = TRUE;
			break;
		}

		case OBJ_ROD_HEALING:
		{
			if (hp_player(500)) ident = TRUE;
			if (set_stun(0)) ident = TRUE;
			if (set_cut(0)) ident = TRUE;
			break;
		}

		case OBJ_ROD_RESTORATION:
		{
			if (restore_level()) ident = TRUE;
			if (do_res_stat(A_STR)) ident = TRUE;
			if (do_res_stat(A_INT)) ident = TRUE;
			if (do_res_stat(A_WIS)) ident = TRUE;
			if (do_res_stat(A_DEX)) ident = TRUE;
			if (do_res_stat(A_CON)) ident = TRUE;
			if (do_res_stat(A_CHR)) ident = TRUE;
			break;
		}

		case OBJ_ROD_SPEED:
		{
			if (!p_ptr->fast)
			{
				if (set_fast(randint(30) + 15)) ident = TRUE;
			}
			else
			{
				(void)set_fast(p_ptr->fast + 5);
			}
			break;
		}

		case OBJ_ROD_TELEPORT_OTHER:
		{
			if (teleport_monster(dir)) ident = TRUE;
			break;
		}

		case OBJ_ROD_DISARMING:
		{
			if (disarm_trap(dir)) ident = TRUE;
			break;
		}

		case OBJ_ROD_LIGHT:
		{
			msg_print("A line of blue shimmering light appears.");
			lite_line(dir);
			ident = TRUE;
			break;
		}

		case OBJ_ROD_SLEEP_MONSTER:
		{
			if (sleep_monster(dir,(skill_set[SKILL_DEVICE].value/2))) ident = TRUE;
			break;
		}

		case OBJ_ROD_SLOW_MONSTER:
		{
			if (slow_monster(dir,(skill_set[SKILL_DEVICE].value/2))) ident = TRUE;
			break;
		}

		case OBJ_ROD_DRAIN_LIFE:
		{
			if (drain_life(dir, 75)) ident = TRUE;
			break;
		}

		case OBJ_ROD_POLYMORPH:
		{
			if (poly_monster(dir,(skill_set[SKILL_DEVICE].value/2))) ident = TRUE;
			break;
		}

		case OBJ_ROD_ACID_BOLT:
		{
			fire_bolt_or_beam(10, GF_ACID, dir, damroll(6, 8));
			ident = TRUE;
			break;
		}

		case OBJ_ROD_ELEC_BOLT:
		{
			fire_bolt_or_beam(10, GF_ELEC, dir, damroll(3, 8));
			ident = TRUE;
			break;
		}

		case OBJ_ROD_FIRE_BOLT:
		{
			fire_bolt_or_beam(10, GF_FIRE, dir, damroll(8, 8));
			ident = TRUE;
			break;
		}

		case OBJ_ROD_COLD_BOLT:
		{
			fire_bolt_or_beam(10, GF_COLD, dir, damroll(5, 8));
			ident = TRUE;
			break;
		}

		case OBJ_ROD_ACID_BALL:
		{
			fire_ball(GF_ACID, dir, 60, 2);
			ident = TRUE;
			break;
		}

		case OBJ_ROD_ELEC_BALL:
		{
			fire_ball(GF_ELEC, dir, 32, 2);
			ident = TRUE;
			break;
		}

		case OBJ_ROD_FIRE_BALL:
		{
			fire_ball(GF_FIRE, dir, 72, 2);
			ident = TRUE;
			break;
		}

		case OBJ_ROD_COLD_BALL:
		{
			fire_ball(GF_COLD, dir, 48, 2);
			ident = TRUE;
			break;
		}

        case OBJ_ROD_HAVOC:
		{
            call_chaos(skill_set[SKILL_DEVICE].value/2);
			ident = TRUE;
			break;
		}
	}


	/* Combine / Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	/* Tried the object */
	object_tried(o_ptr);

	/* Successfully determined the object function */
	if (ident && !object_aware_p(o_ptr))
	{
		object_aware(o_ptr);
	}

	/* Window stuff */
	p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_PLAYER);

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



/*
 * Hack -- activate the ring of power
 */
static void ring_of_power(int dir)
{
	/* Pick a random effect */
	switch (randint(10))
	{
		case 1:
		case 2:
		{
			/* Message */
			msg_print("You are surrounded by a malignant aura.");

			/* Decrease all stats (permanently) */
			(void)dec_stat(A_STR, 50, TRUE);
			(void)dec_stat(A_INT, 50, TRUE);
			(void)dec_stat(A_WIS, 50, TRUE);
			(void)dec_stat(A_DEX, 50, TRUE);
			(void)dec_stat(A_CON, 50, TRUE);
			(void)dec_stat(A_CHR, 50, TRUE);

			/* Lose some experience (permanently) */
			p_ptr->exp -= (p_ptr->exp / 4);

			break;
		}

		case 3:
		{
			/* Message */
			msg_print("You are surrounded by a powerful aura.");

			/* Dispel monsters */
			dispel_monsters(1000);

			break;
		}

		case 4:
		case 5:
		case 6:
		{
			/* Mana Ball */
			fire_ball(GF_MANA, dir, 300, 3);

			break;
		}

		case 7:
		case 8:
		case 9:
		case 10:
		{
			/* Mana Bolt */
			fire_bolt(GF_MANA, dir, 250);

			break;
		}
	}
}




/*
 * Enchant some bolts
 */
static bool brand_bolts(void)
{
	int i;

	/* Use the first acceptable bolts */
	for (i = 0; i < INVEN_PACK; i++)
	{
		object_type *o_ptr = &inventory[i];

		/* Skip non-bolts */
		if (o_ptr->tval != TV_BOLT) continue;

		/* Skip artifacts and ego-items */
        if (allart_p(o_ptr) || ego_item_p(o_ptr))
            continue;

		/* Skip cursed/broken items */
		if (cursed_p(o_ptr) || broken_p(o_ptr)) continue;

		/* Randomize */
		if (rand_int(100) < 75) continue;

		/* Message */
		msg_print("Your bolts are covered in a fiery aura!");

		/* Ego-item */
		o_ptr->name2 = EGO_FLAME;

		/* Enchant */
		enchant(o_ptr, rand_int(3) + 4, ENCH_TOHIT | ENCH_TODAM);

		/* Notice */
		return (TRUE);
	}

	/* Flush */
	if (flush_failure) flush();

	/* Fail */
	msg_print("The fiery enchantment failed.");

	/* Notice */
	return (TRUE);
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
    int         i, k, dir, chance;


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

    if (o_ptr->art_name)
    {
        (void) activate_random_artifact(o_ptr);
		/* Window stuff */
		p_ptr->window |= (PW_INVEN | PW_EQUIP);

		/* Success */
		return;
    }


	/* Artifacts */
    else if (o_ptr->name1)
	{
		/* Choose effect */
		switch (o_ptr->name1)
		{
			case ART_POLARIS:
            {
				msg_print("The essence wells with clear light...");
				lite_area(damroll(2, 15), 3);
				o_ptr->timeout = rand_int(10) + 10;
				break;
			}

			case ART_XOTH:
			{
				msg_print("The essence shines brightly...");
				map_area();
				lite_area(damroll(2, 15), 3);
				o_ptr->timeout = rand_int(50) + 50;
				break;
			}

			case ART_TRAPEZOHEDRON:
			{
                msg_print("The gemstone flashes bright red!");
				wiz_lite();
                msg_print("The gemstone drains your vitality...");
                take_hit(damroll(3,8), "the Gemstone 'Trapezohedron'", MON_DANGEROUS_EQUIPMENT);
				(void)detect_traps();
				mark_traps();
				(void)detect_doors();
				(void)detect_stairs();
                if (get_check("Activate recall? "))
                {
					set_recall(FALSE);
                }

                o_ptr->timeout = rand_int(20) + 20;
				break;
			}


			case ART_LOBON:
			{
				msg_print("The amulet lets out a shrill wail...");
				k = 3 * (skill_set[SKILL_DEVICE].value/2);
				(void)set_protevil(p_ptr->protevil + randint(25) + k);
				o_ptr->timeout = rand_int(225) + 225;
				break;
			}

			case ART_ALHAZRED:
			{
				msg_print("The amulet floods the area with goodness...");
				dispel_evil((skill_set[SKILL_DEVICE].value/2) * 5);
				o_ptr->timeout = rand_int(300) + 300;
				break;
			}

            case ART_MAGIC:
			{

                msg_print("You order Frakir to strangle your opponent.");
				if (!get_aim_dir(&dir)) return;
                if (drain_life(dir, 100))
                    o_ptr->timeout = rand_int(100) + 100;
				break;
			}


			case ART_BAST:
			{
				msg_print("The ring glows brightly...");
				if (!p_ptr->fast)
				{
					(void)set_fast(randint(75) + 75);
				}
				else
				{
					(void)set_fast(p_ptr->fast + 5);
				}
				o_ptr->timeout = rand_int(150) + 150;
				break;
			}

			case ART_ELEMFIRE:
			{
				msg_print("The ring glows deep red...");
				if (!get_aim_dir(&dir)) return;
				fire_ball(GF_FIRE, dir, 120, 3);
				o_ptr->timeout = rand_int(225) + 225;
				break;
			}

			case ART_ELEMICE:
			{
				msg_print("The ring glows bright white...");
				if (!get_aim_dir(&dir)) return;
				fire_ball(GF_COLD, dir, 200, 3);
				o_ptr->timeout = rand_int(325) + 325;
				break;
			}

			case ART_ELEMSTORM:
			{
				msg_print("The ring glows deep blue...");
				if (!get_aim_dir(&dir)) return;
				fire_ball(GF_ELEC, dir, 250, 3);
				o_ptr->timeout = rand_int(425) + 425;
				break;
			}

			case ART_NYARLATHOTEP:
			{
				msg_print("The ring glows intensely black...");
				if (!get_aim_dir(&dir)) return;
				ring_of_power(dir);
				o_ptr->timeout = rand_int(450) + 450;
				break;
			}


			case ART_RAZORBACK:
			{
				msg_print("Your armor is surrounded by lightning...");
				for (i = 0; i < 8; i++) fire_ball(GF_ELEC, ddd[i], 150, 3);
				o_ptr->timeout = 1000;
				break;
			}

			case ART_BLADETURNER:
			{
                if (!get_aim_dir(&dir)) return;
                    msg_print("You breathe the elements.");
                    fire_ball(GF_MISSILE, dir, 300, -4);
                msg_print("Your armor glows many colours...");
                (void)set_afraid(0);
				(void)set_shero(p_ptr->shero + randint(50) + 50);
				(void)hp_player(30);
				(void)set_blessed(p_ptr->blessed + randint(50) + 50);
				(void)set_oppose_acid(p_ptr->oppose_acid + randint(50) + 50);
				(void)set_oppose_elec(p_ptr->oppose_elec + randint(50) + 50);
				(void)set_oppose_fire(p_ptr->oppose_fire + randint(50) + 50);
				(void)set_oppose_cold(p_ptr->oppose_cold + randint(50) + 50);
				(void)set_oppose_pois(p_ptr->oppose_pois + randint(50) + 50);
				o_ptr->timeout = 400;
				break;
			}


			case ART_SOULKEEPER:
			{
				msg_print("Your armor glows a bright white...");
				msg_print("You feel much better...");
				(void)hp_player(1000);
				(void)set_cut(0);
				o_ptr->timeout = 888;
				break;
			}

			case ART_VAMPLORD:
			{
                msg_print("A heavenly choir sings...");
                (void)set_poisoned(0);
                (void)set_cut(0);
                (void)set_stun(0);
                (void)set_confused(0);
                (void)set_blind(0);
                (void)set_hero(p_ptr->hero + randint(25) + 25);
                (void)hp_player(777);
                o_ptr->timeout = 300;
				break;
			}

			case ART_ORCS:
			{
				msg_print("Your armor glows deep blue...");
				(void)genocide(TRUE);
				o_ptr->timeout = 500;
				break;
			}

			case ART_OGRELORDS:
			{
				msg_print("Your armor glows bright red...");
				destroy_doors_touch();
				o_ptr->timeout = 10;
				break;
			}

            case ART_POWER: case ART_MASK:
        
            {
                turn_monsters(40 + (skill_set[SKILL_DEVICE].value/2));
               o_ptr->timeout = 3 * ((skill_set[SKILL_DEVICE].value/2) + 10);

                break;
        
            }


			case ART_SKULLKEEPER:
			{
				msg_print("Your helm glows bright white...");
				msg_print("An image forms in your mind...");
				detect_all();
				o_ptr->timeout = rand_int(55) + 55;
				break;
			}

			case ART_SUN:
			{
				msg_print("Your crown glows deep yellow...");
				msg_print("You feel a warm tingling inside...");
                (void)hp_player(700);
				(void)set_cut(0);
                o_ptr->timeout = 250;
				break;
			}


			case ART_BARZAI:
			{
				msg_print("Your cloak glows many colours...");
				(void)set_oppose_acid(p_ptr->oppose_acid + randint(20) + 20);
				(void)set_oppose_elec(p_ptr->oppose_elec + randint(20) + 20);
				(void)set_oppose_fire(p_ptr->oppose_fire + randint(20) + 20);
				(void)set_oppose_cold(p_ptr->oppose_cold + randint(20) + 20);
				(void)set_oppose_pois(p_ptr->oppose_pois + randint(20) + 20);
				o_ptr->timeout = 111;
				break;
			}

			case ART_DARKNESS:
			{
				msg_print("Your cloak glows deep blue...");
				sleep_monsters_touch((skill_set[SKILL_DEVICE].value/2));
				o_ptr->timeout = 55;
				break;
			}

			case ART_SWASHBUCKLER:
			{
				msg_print("Your cloak glows bright yellow...");
				recharge(60);
				o_ptr->timeout = 70;
				break;
			}

			case ART_SHIFTER:
			{
				msg_print("Your cloak twists space around you...");
				teleport_player(100);
				o_ptr->timeout = 45;
				break;
			}

			case ART_NYOGTHA:
			{
				msg_print("Your cloak glows a deep red...");
				restore_level();
				o_ptr->timeout = 450;
				break;
			}


			case ART_LIGHT:
			{
				msg_print("Your gloves glow extremely brightly...");
				if (!get_aim_dir(&dir)) return;
				fire_bolt(GF_MISSILE, dir, damroll(2, 6));
				o_ptr->timeout = 2;
				break;
			}

			case ART_IRONFIST:
			{
				msg_print("Your gauntlets are covered in fire...");
				if (!get_aim_dir(&dir)) return;
				fire_bolt(GF_FIRE, dir, damroll(9, 8));
				o_ptr->timeout = rand_int(8) + 8;
				break;
			}

			case ART_GHOULS:
			{
				msg_print("Your gauntlets are covered in frost...");
				if (!get_aim_dir(&dir)) return;
				fire_bolt(GF_COLD, dir, damroll(6, 8));
				o_ptr->timeout = rand_int(7) + 7;
				break;
			}

			case ART_WHITESPARK:
			{
				msg_print("Your gauntlets are covered in sparks...");
				if (!get_aim_dir(&dir)) return;
				fire_bolt(GF_ELEC, dir, damroll(4, 8));
				o_ptr->timeout = rand_int(6) + 6;
				break;
			}

			case ART_DEAD:
			{
				msg_print("Your gauntlets are covered in acid...");
				if (!get_aim_dir(&dir)) return;
				fire_bolt(GF_ACID, dir, damroll(5, 8));
				o_ptr->timeout = rand_int(5) + 5;
				break;
			}

			case ART_COMBAT:
			{
				msg_print("Your cesti grows magical spikes...");
				if (!get_aim_dir(&dir)) return;
				fire_bolt(GF_ARROW, dir, 150);
				o_ptr->timeout = rand_int(90) + 90;
				break;
			}


			case ART_ITHAQUA:
			{
				msg_print("A wind swirls around your boots...");
				if (!p_ptr->fast)
				{
					(void)set_fast(randint(20) + 20);
				}
				else
				{
					(void)set_fast(p_ptr->fast + 5);
				}
				o_ptr->timeout = 200;
				break;
			}

			case ART_DANCING:
			{
				msg_print("Your boots glow deep blue...");
				(void)set_afraid(0);
				(void)set_poisoned(0);
				o_ptr->timeout = 5;
				break;
			}


			case ART_FAITH:
			{
				msg_print("Your dagger is covered in fire...");
				if (!get_aim_dir(&dir)) return;
				fire_bolt(GF_FIRE, dir, damroll(9, 8));
				o_ptr->timeout = rand_int(8) + 8;
				break;
			}

			case ART_HOPE:
			{
				msg_print("Your dagger is covered in frost...");
				if (!get_aim_dir(&dir)) return;
				fire_bolt(GF_COLD, dir, damroll(6, 8));
				o_ptr->timeout = rand_int(7) + 7;
				break;
			}

			case ART_CHARITY:
			{
				msg_print("Your dagger is covered in sparks...");
				if (!get_aim_dir(&dir)) return;
				fire_bolt(GF_ELEC, dir, damroll(4, 8));
				o_ptr->timeout = rand_int(6) + 6;
				break;
			}

			case ART_THOTH:
			{
				msg_print("Your dagger throbs deep green...");
				if (!get_aim_dir(&dir)) return;
				fire_ball(GF_POIS, dir, 12, 3);
				o_ptr->timeout = rand_int(4) + 4;
				break;
			}

			case ART_ICICLE:
			{
				msg_print("Your dagger is covered in frost...");
				if (!get_aim_dir(&dir)) return;
				fire_ball(GF_COLD, dir, 48, 2);
				o_ptr->timeout = rand_int(5) + 5;
				break;
			}

            case ART_KARAKAL:
			{
                switch(randint(13))
                {
                    case 1: case 2: case 3: case 4: case 5:
                        teleport_player(10);
                        break;
                    case 6: case 7: case 8: case 9: case 10:
                        teleport_player(222);
                        break;
                    case 11: case 12:
                        (void)stair_creation();
                        break;
                    default:
                        if(get_check("Leave this level? "))
                        {
				change_level(dun_level, START_RANDOM);
                        }
                }
                o_ptr->timeout = 35;
				break;
			}

			case ART_STARLIGHT:
			{
				msg_print("Your sword glows an intense blue...");
				if (!get_aim_dir(&dir)) return;
				fire_ball(GF_COLD, dir, 100, 2);
				o_ptr->timeout = 300;
				break;
			}

            case ART_DAWN:
            {
                msg_print("Your sword flickers black for a moment...");
                (void)summon_specific_friendly(py, px, (dun_depth), SUMMON_REAVER, TRUE);
                o_ptr->timeout = 500 + randint(500);
                break;
            }

			case ART_EVERFLAME:
			{
				msg_print("Your sword glows an intense red...");
				if (!get_aim_dir(&dir)) return;
				fire_ball(GF_FIRE, dir, 72, 2);
				o_ptr->timeout = 400;
				break;
			}


			case ART_THEODEN:
			{
				msg_print("Your axe blade glows black...");
				if (!get_aim_dir(&dir)) return;
				drain_life(dir, 120);
				o_ptr->timeout = 400;
				break;
			}

			case ART_ODIN:
			{
                msg_print("Your spear crackles with electricity...");
				if (!get_aim_dir(&dir)) return;
                fire_ball(GF_ELEC, dir, 100, 3);
				o_ptr->timeout = 500;
				break;
			}

			case ART_DESTINY:
			{
				msg_print("Your spear pulsates...");
				if (!get_aim_dir(&dir)) return;
				wall_to_mud(dir);
				o_ptr->timeout = 5;
				break;
			}

			case ART_TROLLS:
			{
				msg_print("Your axe lets out a long, shrill note...");
				(void)mass_genocide(TRUE);
				o_ptr->timeout = 1000;
				break;
			}

			case ART_SPLEENSLICER:
			{
				msg_print("Your battle axe radiates deep purple...");
				hp_player(damroll(4, 8));
				(void)set_cut((p_ptr->cut / 2) - 50);
				o_ptr->timeout = rand_int(3) + 3;
				break;
			}

			case ART_GNORRI:
			{
				msg_print("Your trident glows deep red...");
				if (!get_aim_dir(&dir)) return;
				teleport_monster(dir);
				o_ptr->timeout = 150;
				break;
			}

			case ART_GHARNE:
			{
                if (dun_level && (p_ptr->max_dlv > dun_level) && (recall_dungeon == cur_dungeon))
                {
                    if (get_check("Reset recall depth? "))
                    p_ptr->max_dlv = dun_level;

                }
                
				msg_print("Your scythe glows soft white...");
				set_recall(FALSE);
				o_ptr->timeout = 200;
				break;
			}


			case ART_TOTILA:
			{
				msg_print("Your flail glows in scintillating colours...");
				if (!get_aim_dir(&dir)) return;
				confuse_monster(dir, 20);
				o_ptr->timeout = 15;
				break;
			}

			case ART_FIRESTAR:
			{
				msg_print("Your morning star rages in fire...");
				if (!get_aim_dir(&dir)) return;
				fire_ball(GF_FIRE, dir, 72, 3);
				o_ptr->timeout = 100;
				break;
			}

			case ART_THUNDER:
			{
				msg_print("Your mace glows bright green...");
				if (!p_ptr->fast)
				{
					(void)set_fast(randint(20) + 20);
				}
				else
				{
					(void)set_fast(p_ptr->fast + 5);
				}
				o_ptr->timeout = rand_int(100) + 100;
				break;
			}

			case ART_ERIRIL:
			{
				msg_print("Your quarterstaff glows yellow...");
				if (!ident_spell()) return;
				o_ptr->timeout = 10;
				break;
			}

			case ART_ATAL:
			{
				msg_print("Your quarterstaff glows brightly...");
                detect_all();
				probing();
                identify_fully();
                o_ptr->timeout = 1000;
				break;
			}

			case ART_JUSTICE:
			{
				msg_print("Your hammer glows white...");
				if (!get_aim_dir(&dir)) return;
				drain_life(dir, 90);
				o_ptr->timeout = 70;
				break;
			}


			case ART_DEATH:
			{
				msg_print("Your crossbow glows deep red...");
				(void)brand_bolts();
				o_ptr->timeout = 999;
				break;
			}
		}

	/* Gain experience if the activation took place. */
	if (o_ptr->timeout)
		skill_exp(SKILL_DEVICE);


		/* Window stuff */
		p_ptr->window |= (PW_INVEN | PW_EQUIP);

		/* Done */
		return;
	}




    else if (o_ptr->name2 == EGO_PLANAR)
    {
        teleport_player(100);
        o_ptr->timeout = 50 + randint(50);

        /* Window stuff */
		p_ptr->window |= (PW_INVEN | PW_EQUIP);

		/* Done */
		return;
    }


	/* Hack -- Dragon Scale Mail can be activated as well */
	if (o_ptr->tval == TV_DRAG_ARMOR)
	{
		/* Get a direction for breathing (or abort) */
		if (!get_aim_dir(&dir)) return;

		/* Branch on the sub-type */
		switch (o_ptr->k_idx)
		{
			case OBJ_DSM_BLUE:
			{
				msg_print("You breathe lightning.");
				fire_ball(GF_ELEC, dir, 100, -2);
				o_ptr->timeout = rand_int(450) + 450;
				break;
			}

			case OBJ_DSM_WHITE:
			{
				msg_print("You breathe frost.");
				fire_ball(GF_COLD, dir, 110, -2);
				o_ptr->timeout = rand_int(450) + 450;
				break;
			}

			case OBJ_DSM_BLACK:
			{
				msg_print("You breathe acid.");
				fire_ball(GF_ACID, dir, 130, -2);
				o_ptr->timeout = rand_int(450) + 450;
				break;
			}

			case OBJ_DSM_GREEN:
			{
				msg_print("You breathe poison gas.");
				fire_ball(GF_POIS, dir, 150, -2);
				o_ptr->timeout = rand_int(450) + 450;
				break;
			}

			case OBJ_DSM_RED:
			{
				msg_print("You breathe fire.");
				fire_ball(GF_FIRE, dir, 200, -2);
				o_ptr->timeout = rand_int(450) + 450;
				break;
			}

			case OBJ_DSM_MULTI_HUED:
			{
				chance = rand_int(5);
				msg_format("You breathe %s.",
				           ((chance == 1) ? "lightning" :
				            ((chance == 2) ? "frost" :
				             ((chance == 3) ? "acid" :
				              ((chance == 4) ? "poison gas" : "fire")))));
				fire_ball(((chance == 1) ? GF_ELEC :
				           ((chance == 2) ? GF_COLD :
				            ((chance == 3) ? GF_ACID :
				             ((chance == 4) ? GF_POIS : GF_FIRE)))),
				          dir, 250, -2);
				o_ptr->timeout = rand_int(225) + 225;
				break;
			}

			case OBJ_DSM_BRONZE:
			{
				msg_print("You breathe confusion.");
				fire_ball(GF_CONFUSION, dir, 120, -2);
				o_ptr->timeout = rand_int(450) + 450;
				break;
			}

			case OBJ_DSM_GOLD:
			{
				msg_print("You breathe sound.");
				fire_ball(GF_SOUND, dir, 130, -2);
				o_ptr->timeout = rand_int(450) + 450;
				break;
			}

			case OBJ_DSM_CHAOS:
			{
				chance = rand_int(2);
				msg_format("You breathe %s.",
				           ((chance == 1 ? "chaos" : "disenchantment")));
				fire_ball((chance == 1 ? GF_CHAOS : GF_DISENCHANT),
				          dir, 220, -2);
				o_ptr->timeout = rand_int(300) + 300;
				break;
			}

			case OBJ_DSM_LAW:
			{
				chance = rand_int(2);
				msg_format("You breathe %s.",
				           ((chance == 1 ? "sound" : "shards")));
				fire_ball((chance == 1 ? GF_SOUND : GF_SHARDS),
				          dir, 230, -2);
				o_ptr->timeout = rand_int(300) + 300;
				break;
			}

			case OBJ_DSM_BALANCE:
			{
				chance = rand_int(4);
				msg_format("You breathe %s.",
				           ((chance == 1) ? "chaos" :
				            ((chance == 2) ? "disenchantment" :
				             ((chance == 3) ? "sound" : "shards"))));
				fire_ball(((chance == 1) ? GF_CHAOS :
				           ((chance == 2) ? GF_DISENCHANT :
				            ((chance == 3) ? GF_SOUND : GF_SHARDS))),
				          dir, 250, -2);
				o_ptr->timeout = rand_int(300) + 300;
				break;
			}

			case OBJ_DSM_PSEUDO:
			{
				chance = rand_int(2);
				msg_format("You breathe %s.",
				           ((chance == 0 ? "light" : "darkness")));
				fire_ball((chance == 0 ? GF_LITE : GF_DARK), dir, 200, -2);
				o_ptr->timeout = rand_int(300) + 300;
				break;
			}

			case OBJ_DSM_POWER:
			{
				msg_print("You breathe the elements.");
                fire_ball(GF_MISSILE, dir, 300, -3);
				o_ptr->timeout = rand_int(300) + 300;
				break;
			}
		}



		/* Window stuff */
		p_ptr->window |= (PW_INVEN | PW_EQUIP);

		/* Success */
		return;
	}

    else if (o_ptr->tval == TV_RING)
    {
		/* Get a direction for breathing (or abort) */
		if (!get_aim_dir(&dir)) return;
        switch (o_ptr->k_idx)
        {
            case OBJ_RING_ACID:
			{
                fire_ball(GF_ACID, dir, 50, 2);
                (void)set_oppose_acid(p_ptr->oppose_acid + randint(20) + 20);
                o_ptr->timeout = rand_int(50) + 50;
				break;
			}

            case OBJ_RING_ICE:
			{
                fire_ball(GF_COLD, dir, 50, 2);
                (void)set_oppose_cold(p_ptr->oppose_cold + randint(20) + 20);
                o_ptr->timeout = rand_int(50) + 50;
				break;
            }

            case OBJ_RING_FIRE:
			{
                fire_ball(GF_FIRE, dir, 50, 2);
                (void)set_oppose_fire(p_ptr->oppose_fire + randint(20) + 20);
                o_ptr->timeout = rand_int(50) + 50;
				break;
            }
        }
    

		/* Window stuff */
		p_ptr->window |= (PW_INVEN | PW_EQUIP);

		/* Success */
		return;

    }


	/* Mistake */
	msg_print("Oops.  That object cannot be activated.");
}


static bool activate_random_artifact(object_type * o_ptr)
{
    int plev = skill_set[SKILL_DEVICE].value/2;
    int k, dir, dummy = 0;

	if (plev == 0) plev++;

    if (!(o_ptr->art_name)) return FALSE; /* oops? */


/* Activate for attack */
    switch (o_ptr->xtra2)
    {
            case ACT_SUNLIGHT:
            {
                if (!get_aim_dir(&dir)) return FALSE;
                msg_print("A line of sunlight appears.");
                lite_line(dir);
                o_ptr->timeout = 10;
                   break;
                }

            case ACT_BO_MISS_1:
			{
                msg_print("It glows extremely brightly...");
                if (!get_aim_dir(&dir)) return FALSE;
				fire_bolt(GF_MISSILE, dir, damroll(2, 6));
				o_ptr->timeout = 2;
				break;
			}

            case ACT_BA_POIS_1:
			{
                msg_print("It throbs deep green...");
                if (!get_aim_dir(&dir)) return FALSE;
				fire_ball(GF_POIS, dir, 12, 3);
				o_ptr->timeout = rand_int(4) + 4;
				break;
			}


            case ACT_BO_ELEC_1:
			{
                msg_print("It is covered in sparks...");
                if (!get_aim_dir(&dir)) return FALSE;
				fire_bolt(GF_ELEC, dir, damroll(4, 8));
				o_ptr->timeout = rand_int(6) + 6;
				break;
			}

            case ACT_BO_ACID_1:
			{
                msg_print("It is covered in acid...");
                if (!get_aim_dir(&dir)) return FALSE;
				fire_bolt(GF_ACID, dir, damroll(5, 8));
				o_ptr->timeout = rand_int(5) + 5;
				break;
			}

            case ACT_BO_COLD_1:
            {
                msg_print("It is covered in frost...");
                if (!get_aim_dir(&dir)) return FALSE;
				fire_bolt(GF_COLD, dir, damroll(6, 8));
				o_ptr->timeout = rand_int(7) + 7;
				break;
			}

            case ACT_BO_FIRE_1:
			{
                msg_print("It is covered in fire...");
                if (!get_aim_dir(&dir)) return FALSE;
				fire_bolt(GF_FIRE, dir, damroll(9, 8));
				o_ptr->timeout = rand_int(8) + 8;
				break;
			}

            case ACT_BA_COLD_1:
            {
                msg_print("It is covered in frost...");
                if (!get_aim_dir(&dir)) return FALSE;
				fire_ball(GF_COLD, dir, 48, 2);
                o_ptr->timeout = 400;
				break;
			}

            case ACT_BA_FIRE_1:
			{
                msg_print("It glows an intense red...");
                if (!get_aim_dir(&dir)) return FALSE;
				fire_ball(GF_FIRE, dir, 72, 2);
				o_ptr->timeout = 400;
				break;
			}

            case ACT_DRAIN_1:
            {

                msg_print("It glows black...");
                if (!get_aim_dir(&dir)) return FALSE;
                if (drain_life(dir, 100))
                    o_ptr->timeout = rand_int(100) + 100;
				break;
			}

            case ACT_BA_COLD_2:
            {
                msg_print("It glows an intense blue...");
                if (!get_aim_dir(&dir)) return FALSE;
				fire_ball(GF_COLD, dir, 100, 2);
				o_ptr->timeout = 300;
				break;
			}


            case ACT_BA_ELEC_2:
            {
                msg_print("It crackles with electricity...");
                if (!get_aim_dir(&dir)) return FALSE;
                fire_ball(GF_ELEC, dir, 100, 3);
				o_ptr->timeout = 500;
				break;
			}


            case ACT_DRAIN_2:
			{
                msg_print("It glows black...");
                if (!get_aim_dir(&dir)) return FALSE;
				drain_life(dir, 120);
				o_ptr->timeout = 400;
				break;
			}

            case ACT_VAMPIRE_1:
            {
            if (!get_aim_dir(&dir)) return FALSE;
               for (dummy = 0; dummy < 3; dummy++)
               {
                   if (drain_life(dir, 50))
                       hp_player(50);
                    }

                o_ptr->timeout = 400;
                   break;
                }

            case ACT_BO_MISS_2:
            {
                msg_print("It grows magical spikes...");
                if (!get_aim_dir(&dir)) return FALSE;
				fire_bolt(GF_ARROW, dir, 150);
				o_ptr->timeout = rand_int(90) + 90;
				break;
			}

            case ACT_BA_FIRE_2:
			{
                msg_print("It glows deep red...");
                if (!get_aim_dir(&dir)) return FALSE;
				fire_ball(GF_FIRE, dir, 120, 3);
				o_ptr->timeout = rand_int(225) + 225;
				break;
			}

            case ACT_BA_COLD_3:
			{
                msg_print("It glows bright white...");
                if (!get_aim_dir(&dir)) return FALSE;
				fire_ball(GF_COLD, dir, 200, 3);
				o_ptr->timeout = rand_int(325) + 325;
				break;
			}

            case ACT_BA_ELEC_3:
			{
                msg_print("It glows deep blue...");
                if (!get_aim_dir(&dir)) return FALSE;
				fire_ball(GF_ELEC, dir, 250, 3);
				o_ptr->timeout = rand_int(425) + 425;
				break;
			}

            case ACT_WHIRLWIND:
         {
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
             o_ptr->timeout = 250;
             break;
            }

            case ACT_VAMPIRE_2:
            {
            if (!get_aim_dir(&dir)) return FALSE;
               for (dummy = 0; dummy < 3; dummy++)
               {
                   if (drain_life(dir, 100))
                       hp_player(100);
                    }

                o_ptr->timeout = 400;
                   break;
                }


            case ACT_CALL_CHAOS:
            {
                msg_print("It glows in scintillating colours...");
                call_chaos(skill_set[SKILL_DEVICE].value/2);
                o_ptr->timeout = 350;
                break;
            }

            case ACT_SHARD:
            {
                if (!get_aim_dir(&dir)) return FALSE;
                fire_ball(GF_SHARD, dir,
                        120 + (plev), 2);
                o_ptr->timeout = 400;
                break;
            }

            case ACT_DISP_EVIL:
			{
                msg_print("It floods the area with goodness...");
				dispel_evil(plev * 5);
				o_ptr->timeout = rand_int(300) + 300;
				break;
			}


            case ACT_DISP_GOOD:
			{
                msg_print("It floods the area with evil...");
                dispel_good(plev * 5);
				o_ptr->timeout = rand_int(300) + 300;
				break;
			}

            case ACT_BA_MISS_3:
            {
                if (!get_aim_dir(&dir)) return FALSE;
                    msg_print("You breathe the elements.");
                    fire_ball(GF_MISSILE, dir, 300, -4);
                o_ptr->timeout = 500;
                break;
            }



/* Activate for other offensive action */

            case ACT_CONFUSE:
			{
                msg_print("It glows in scintillating colours...");
                if (!get_aim_dir(&dir)) return FALSE;
				confuse_monster(dir, 20);
				o_ptr->timeout = 15;
				break;
			}

            case ACT_SLEEP:
			{
                msg_print("It glows deep blue...");
				sleep_monsters_touch((skill_set[SKILL_DEVICE].value/2));
				o_ptr->timeout = 55;
				break;
			}

            case ACT_QUAKE:
            {
                earthquake(py, px, 10);
                o_ptr->timeout = 50;
                   break;
                }

            case ACT_TERROR:
            {
                turn_monsters(40 + plev);
               o_ptr->timeout = 3 * (plev + 10);
                break;
            }
            case ACT_TELE_AWAY:
			{
                if (!get_aim_dir(&dir)) return FALSE;
                   (void)fire_beam(GF_AWAY_ALL, dir, plev);
                   o_ptr->timeout = 200;
                   break;
			}

            case ACT_BANISH_EVIL:
            {
                if (banish_evil(100))
                {
                    msg_print("The power of the artifact banishes evil!");
                }
                o_ptr->timeout = 250 + randint(250);
                break;
            }

            case ACT_GENOCIDE:
			{
                msg_print("It glows deep blue...");
				(void)genocide(TRUE);
				o_ptr->timeout = 500;
				break;
			}

            case ACT_MASS_GENO:
            {
                msg_print("It lets out a long, shrill note...");
				(void)mass_genocide(TRUE);
				o_ptr->timeout = 1000;
				break;
			}


/* Activate for summoning / charming */


           case ACT_CHARM_ANIMAL:
           {
             if (!get_aim_dir(&dir)) return FALSE;
             (void) charm_animal(dir, plev);
             o_ptr->timeout = 300;
             break;
            }

            case ACT_CHARM_UNDEAD:
            {
             if (!get_aim_dir(&dir)) return FALSE;
               (void)control_one_undead(dir, plev);
               o_ptr->timeout = 333;
                   break;
                }


            case ACT_CHARM_OTHER:
            {
                 if (!get_aim_dir(&dir)) return FALSE;
                 (void) charm_monster(dir, plev);
                 o_ptr->timeout = 400;
               break;
            }


            case ACT_CHARM_ANIMALS:
            {
            (void) charm_animals(plev * 2);
            o_ptr->timeout = 500;
             break;
            }

            case ACT_CHARM_OTHERS:
            {
               charm_monsters(plev * 2);
               o_ptr->timeout = 750;
		       break;
            }


            case ACT_SUMMON_ANIMAL:
            {
                 (void)summon_specific_friendly(py, px, plev, SUMMON_ANIMAL_RANGER, TRUE);
                 o_ptr->timeout = 200 + randint(300);
                     break;
                }

            case ACT_SUMMON_PHANTOM:
            {
                msg_print("You summon a phantasmal servant.");
                (void)summon_specific_friendly(py, px, (dun_depth), SUMMON_PHANTOM, TRUE);
                o_ptr->timeout = 200 + randint(200);
                break;
            }

            case ACT_SUMMON_ELEMENTAL:
            {
                       if (randint(3) == 1) {
                   if (summon_specific((int)py,(int)px, (int)(plev * 1.5),
                           SUMMON_ELEMENTAL)) {
                   msg_print("An elemental materializes...");
                   msg_print("You fail to control it!");
                   }
               } else {
                   if (summon_specific_friendly((int)py, (int)px, (int)(plev * 1.5),
                                SUMMON_ELEMENTAL,(bool)(plev == 50 ? TRUE : FALSE))) {
                   msg_print("An elemental materializes...");
                   msg_print("It seems obedient to you.");
                   }
               }
               o_ptr->timeout = 750;
               break;
            }

            case ACT_SUMMON_DEMON:
            {
                       if (randint(3) == 1) {
                   if (summon_specific((int)py, (int)px, (int)(plev * 1.5),
                           SUMMON_DEMON)) {
                   msg_print("The area fills with a stench of sulphur and brimstone.");
                   msg_print("'NON SERVIAM! Wretch! I shall feast on thy mortal soul!'");
                   }
               } else {
                   if (summon_specific_friendly((int)py, (int)px, (int)(plev * 1.5),
                                SUMMON_DEMON, (bool)(plev == 50 ? TRUE : FALSE))) {
                   msg_print("The area fills with a stench of sulphur and brimstone.");
                   msg_print("'What is thy bidding... Master?'");
                   }
               }
               o_ptr->timeout = 666 + randint(333);
               break;
            }

            case ACT_SUMMON_UNDEAD:
            {
               if (randint(3) == 1) {
                   if (summon_specific((int)py, (int)px, (int)(plev * 1.5),
                           (plev > 47 ? SUMMON_HI_UNDEAD : SUMMON_UNDEAD))) {
                   msg_print("Cold winds begin to blow around you, carrying with them the stench of decay...");
                   msg_print("'The dead arise... to punish you for disturbing them!'");
                   }
               } else {
                   if (summon_specific_friendly((int)py,(int)px, (int)(plev * 1.5),
                       (plev > 47 ? SUMMON_HI_UNDEAD : SUMMON_UNDEAD),
                           (bool)(((plev > 24) && (randint(3) == 1)) ? TRUE : FALSE))) {
                   msg_print("Cold winds begin to blow around you, carrying with them the stench of decay...");
                   msg_print("Ancient, long-dead forms arise from the ground to serve you!");
                   }
               }
               o_ptr->timeout = 666 + randint(333);
               break;
            }
                



/* Activate for healing */


            case ACT_CURE_LW:
            {
                (void)set_afraid(0);
				(void)hp_player(30);
                o_ptr->timeout = 10;
                break;
            }

            case ACT_CURE_MW:
            {
                msg_print("It radiates deep purple...");
				hp_player(damroll(4, 8));
				(void)set_cut((p_ptr->cut / 2) - 50);
				o_ptr->timeout = rand_int(3) + 3;
				break;
			}


            case ACT_CURE_POISON:
			{
                msg_print("It glows deep blue...");
				(void)set_afraid(0);
				(void)set_poisoned(0);
				o_ptr->timeout = 5;
				break;
			}

            case ACT_REST_LIFE:
            {
                msg_print("It glows a deep red...");
				restore_level();
				o_ptr->timeout = 450;
				break;
			}

            case ACT_REST_ALL:
            {
                msg_print("It glows a deep green...");
                (void)do_res_stat(A_STR);
                (void)do_res_stat(A_INT);
                (void)do_res_stat(A_WIS);
                (void)do_res_stat(A_DEX);
                (void)do_res_stat(A_CON);
                (void)do_res_stat(A_CHR);
                (void)restore_level();
                o_ptr->timeout = 750;
                   break;
                }


            case ACT_CURE_700:
			{
                msg_print("It glows deep blue...");
				msg_print("You feel a warm tingling inside...");
                (void)hp_player(700);
				(void)set_cut(0);
                o_ptr->timeout = 250;
				break;
			}


            case ACT_CURE_1000:
			{
                msg_print("It glows a bright white...");
				msg_print("You feel much better...");
				(void)hp_player(1000);
				(void)set_cut(0);
				o_ptr->timeout = 888;
				break;
			}

/* Activate for timed effect */

            case ACT_ESP:
            {
                (void)set_tim_esp(p_ptr->tim_esp + randint(30) + 25);
                o_ptr->timeout = 200;
                   break;
                }

            case ACT_BERSERK:
            {
				(void)set_shero(p_ptr->shero + randint(50) + 50);
				(void)set_blessed(p_ptr->blessed + randint(50) + 50);
                o_ptr->timeout = 100 + randint(100);
                break;
            }

            case ACT_PROT_EVIL:
            {
                msg_print("It lets out a shrill wail...");
				k = 3 * plev;
				(void)set_protevil(p_ptr->protevil + randint(25) + k);
				o_ptr->timeout = rand_int(225) + 225;
				break;
			}

            case ACT_RESIST_ALL:
			{
                msg_print("It glows many colours...");
                (void)set_oppose_acid(p_ptr->oppose_acid + randint(40) + 40);
                (void)set_oppose_elec(p_ptr->oppose_elec + randint(40) + 40);
                (void)set_oppose_fire(p_ptr->oppose_fire + randint(40) + 40);
                (void)set_oppose_cold(p_ptr->oppose_cold + randint(40) + 40);
                (void)set_oppose_pois(p_ptr->oppose_pois + randint(40) + 40);
                o_ptr->timeout = 200;
				break;
			}


            case ACT_SPEED:
			{
                msg_print("It glows bright green...");
				if (!p_ptr->fast)
				{
					(void)set_fast(randint(20) + 20);
				}
				else
				{
					(void)set_fast(p_ptr->fast + 5);
				}
                o_ptr->timeout = 250;
				break;
			}

            case ACT_XTRA_SPEED:
			{
                msg_print("It glows brightly...");
				if (!p_ptr->fast)
				{
					(void)set_fast(randint(75) + 75);
				}
				else
				{
					(void)set_fast(p_ptr->fast + 5);
				}
                o_ptr->timeout = rand_int(200) + 200;
				break;
			}

            case ACT_WRAITH:
            {
                set_shadow(p_ptr->wraith_form + randint(plev/2) + (plev/2));
                o_ptr->timeout = 1000;
                break;
            }


            case ACT_INVULN:
            {
                (void)set_invuln(p_ptr->invuln + randint(8) + 8);
                o_ptr->timeout = 1000;
                   break;
                }



/* Activate for general purpose effect (detection etc.) */

            case ACT_LIGHT:
            {
                msg_print("It wells with clear light...");
				lite_area(damroll(2, 15), 3);
				o_ptr->timeout = rand_int(10) + 10;
				break;
			}

            case ACT_MAP_LIGHT:
			{
                msg_print("It shines brightly...");
				map_area();
				lite_area(damroll(2, 15), 3);
				o_ptr->timeout = rand_int(50) + 50;
				break;
			}

            case ACT_DETECT_ALL:
			{
                msg_print("It glows bright white...");
				msg_print("An image forms in your mind...");
				detect_all();
				o_ptr->timeout = rand_int(55) + 55;
				break;
			}

            case ACT_DETECT_XTRA:
			{
                msg_print("It glows brightly...");
                detect_all();
				probing();
                identify_fully();
                o_ptr->timeout = 1000;
				break;
			}

            case ACT_ID_FULL:
            {
            msg_print("It glows yellow...");
            identify_fully();
            o_ptr->timeout = 750;
                   break;
                }


            case ACT_ID_PLAIN:
			{

                if (!ident_spell()) return FALSE;
				o_ptr->timeout = 10;
				break;
			}

            case ACT_RUNE_EXPLO:
            {
                msg_print("It glows bright red...");
                   explosive_rune();
                   o_ptr->timeout = 200;
                   break;
            }

            case ACT_RUNE_PROT:
            {
                    msg_print("It glows light blue...");
                   warding_glyph();
                   o_ptr->timeout = 400;
                   break;
                }

            case ACT_SATIATE:
            {
                (void)set_food(PY_FOOD_MAX - 1);
                o_ptr->timeout = 200;
                   break;
                }

            case ACT_DEST_DOOR:
            {
                msg_print("It glows bright red...");
				destroy_doors_touch();
				o_ptr->timeout = 10;
				break;
			}

            case ACT_STONE_MUD:
			{
                msg_print("It pulsates...");
                if (!get_aim_dir(&dir)) return FALSE;
				wall_to_mud(dir);
				o_ptr->timeout = 5;
				break;
			}

            case ACT_RECHARGE:
			{

				recharge(60);
				o_ptr->timeout = 70;
				break;
			}

            case ACT_ALCHEMY:
            {
                    msg_print("It glows bright yellow...");
                   (void) alchemy();
                   o_ptr->timeout = 500;
                   break;
                }


            case ACT_DIM_DOOR:
           {
				if (!dimension_door(plev, 10)) return FALSE;
                 break;
            }


            case ACT_TELEPORT:
			{
                msg_print("It twists space around you...");
				teleport_player(100);
				o_ptr->timeout = 45;
				break;
			}

            case ACT_RECALL:
			{
                if (dun_level && (p_ptr->max_dlv > dun_level) && (recall_dungeon == cur_dungeon))
                {
                    if (get_check("Reset recall depth? "))
                    p_ptr->max_dlv = dun_level;

                }
                
                msg_print("It glows soft white...");
				set_recall(FALSE);
				o_ptr->timeout = 200;
				break;
			}
            default:
                msg_format("Unknown activation effect: %d.", o_ptr->xtra2);
                return FALSE;
        }
    return TRUE;
}
