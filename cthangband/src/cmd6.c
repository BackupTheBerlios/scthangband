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
		apply_magic_2(o_ptr, dun_depth);

		/* Recalculate/redraw stuff (later) */
		update_object(o_ptr, 0);

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
 * Use whatever power an object has, and set three variables to represent the
 * effect.
 *
 * power identifies the power used, calculated as in get_power().
 * dir indicates a direction which has been chosen 
 * The calling function may remove/drain the object if use is unset, but must
 * do so if it is set.
 */
static errr do_power(int power, int plev, int dir, bool known, bool *use, bool *ident)
{
	bool b;
	int i;

	/* Assume that the object was used without identifying it by default. */
	(*use) = TRUE;
	(*ident) = FALSE;

	switch (power)
	{
		case OBJ_FOOD_POISON:
		{
			if (!(p_ptr->resist_pois || p_ptr->oppose_pois))
			{
				if (add_flag(TIMED_POISONED, rand_int(10) + 10))
				{
					(*ident) = TRUE;
				}
			}
			return SUCCESS;
		}

		case OBJ_FOOD_BLINDNESS:
		{
			if (!p_ptr->resist_blind)
			{
				if (add_flag(TIMED_BLIND, rand_int(200) + 200))
				{
					(*ident) = TRUE;
				}
			}
			return SUCCESS;
		}

		case OBJ_FOOD_PARANOIA:
		{
			if (!p_ptr->resist_fear)
			{
				if (add_flag(TIMED_AFRAID, rand_int(10) + 10))
				{
					(*ident) = TRUE;
				}
			}
			return SUCCESS;
		}

		case OBJ_FOOD_CONFUSION:
		{
			if (!p_ptr->resist_conf)
			{
				if (add_flag(TIMED_CONFUSED, rand_int(10) + 10))
				{
					(*ident) = TRUE;
				}
			}
			return SUCCESS;
		}

		case OBJ_FOOD_HALLUCINATION:
		{
			if (!p_ptr->resist_chaos)
			{
				if (add_flag(TIMED_IMAGE, rand_int(250) + 250))
				{
					(*ident) = TRUE;
				}
			}
			return SUCCESS;
		}

		case OBJ_FOOD_PARALYSIS:
		{
			if (!p_ptr->free_act)
			{
				if (add_flag(TIMED_PARALYZED, rand_int(10) + 10))
				{
					(*ident) = TRUE;
				}
			}
			return SUCCESS;
		}

		case OBJ_FOOD_DEC_STR:
		{
			take_hit(damroll(6, 6), "poisonous food.", MON_POISONOUS_FOOD);
			(void)do_dec_stat(A_STR);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_FOOD_SICKNESS:
		{
			take_hit(damroll(6, 6), "poisonous food.", MON_POISONOUS_FOOD);
			(void)do_dec_stat(A_CON);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_FOOD_DEC_INT:
		{
			take_hit(damroll(8, 8), "poisonous food.", MON_POISONOUS_FOOD);
			(void)do_dec_stat(A_INT);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_FOOD_DEC_WIS:
		{
			take_hit(damroll(8, 8), "poisonous food.", MON_POISONOUS_FOOD);
			(void)do_dec_stat(A_WIS);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_FOOD_UNHEALTH:
		{
			take_hit(damroll(10, 10), "poisonous food.", MON_POISONOUS_FOOD);
			(void)do_dec_stat(A_CON);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_FOOD_DISEASE:
		{
			take_hit(damroll(10, 10), "poisonous food.", MON_POISONOUS_FOOD);
			(void)do_dec_stat(A_STR);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_FOOD_CURE_POISON:
		{
			if (set_flag(TIMED_POISONED, 0)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_FOOD_CURE_BLINDNESS:
		{
			if (set_flag(TIMED_BLIND, 0)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_FOOD_CURE_PARANOIA:
		{
			if (set_flag(TIMED_AFRAID, 0)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_FOOD_CURE_CONFUSION:
		{
			if (set_flag(TIMED_CONFUSED, 0)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_FOOD_CURE_SERIOUS:
		{
			if (hp_player(damroll(4, 8))) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_FOOD_RES_STR:
		{
			if (do_res_stat(A_STR)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_FOOD_RES_CON:
		{
			if (do_res_stat(A_CON)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_FOOD_RESTORING:
		{
			if (do_res_stat(A_STR)) (*ident) = TRUE;
			if (do_res_stat(A_INT)) (*ident) = TRUE;
			if (do_res_stat(A_WIS)) (*ident) = TRUE;
			if (do_res_stat(A_DEX)) (*ident) = TRUE;
			if (do_res_stat(A_CON)) (*ident) = TRUE;
			if (do_res_stat(A_CHR)) (*ident) = TRUE;
			return SUCCESS;
		}


		case OBJ_PIECE_OF_ELVISH_WAYBREAD:
		{
			msg_print("That tastes good.");
			(void)set_flag(TIMED_POISONED, 0);
			(void)hp_player(damroll(4, 8));
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_WATER:
		case OBJ_POTION_APPLE_JUICE:
		case OBJ_POTION_SLIME_MOLD_JUICE:
		{
			msg_print("You feel less thirsty.");
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_SLOWNESS:
		{
			if (add_flag(TIMED_SLOW, randint(25) + 15)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_SALT_WATER:
		{
			msg_print("The potion makes you vomit!");
			(void)set_flag(TIMED_FOOD, PY_FOOD_STARVE - 1);
			(void)set_flag(TIMED_POISONED, 0);
			(void)add_flag(TIMED_PARALYZED, 4);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_POISON:
		{
			if (!(p_ptr->resist_pois || p_ptr->oppose_pois))
			{
				if (add_flag(TIMED_POISONED, rand_int(15) + 10))
				{
					(*ident) = TRUE;
				}
			}
			return SUCCESS;
		}

		case OBJ_POTION_BLINDNESS:
		{
			if (!p_ptr->resist_blind)
			{
				if (add_flag(TIMED_BLIND, rand_int(100) + 100))
				{
					(*ident) = TRUE;
				}
			}
			return SUCCESS;
		}

        case OBJ_POTION_BOOZE: /* Booze */
		{
            if (!((p_ptr->resist_conf) || (p_ptr->resist_chaos)))
			{
				if (add_flag(TIMED_CONFUSED, rand_int(20) + 15))
				{
					(*ident) = TRUE;
				}
                if (randint(2)==1)
                {
                    if (add_flag(TIMED_IMAGE, rand_int(150) + 150))
                    {
                        (*ident) = TRUE;
                    }
                }
                if (randint(13)==1)
                {
                    (*ident) = TRUE;
                    if(randint(3)==1) lose_all_info();
                    else wiz_dark();
                    teleport_player(100);
                    wiz_dark();
                    msg_print("You wake up somewhere with a sore head...");
                    msg_print("You can't remember a thing, or how you got here!");
                }
			}
			return SUCCESS;
		}

		case OBJ_POTION_SLEEP:
		{
			if (!p_ptr->free_act)
			{
				if (add_flag(TIMED_PARALYZED, rand_int(4) + 4))
				{
					(*ident) = TRUE;
				}
			}
			return SUCCESS;
		}

		case OBJ_POTION_LOSE_MEMORIES:
		{
			if (!p_ptr->hold_life)
			{
				msg_print("You feel your memories fade.");
				lose_skills(10);
				(*ident) = TRUE;
			}
			return SUCCESS;
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
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_DEC_STR:
		{
			if (do_dec_stat(A_STR)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_DEC_INT:
		{
			if (do_dec_stat(A_INT)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_DEC_WIS:
		{
			if (do_dec_stat(A_WIS)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_DEC_DEX:
		{
			if (do_dec_stat(A_DEX)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_DEC_CON:
		{
			if (do_dec_stat(A_CON)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_DEC_CHR:
		{
			if (do_dec_stat(A_CHR)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_DETONATIONS:
		{
			msg_print("Massive explosions rupture your body!");
			take_hit(damroll(50, 20), "a potion of Detonation", MON_HARMFUL_POTION);
			(void)add_flag(TIMED_STUN, 75);
			(void)add_flag(TIMED_CUT, 5000);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_IOCAINE:
		{
			msg_print("A feeling of Death flows through your body.");
			take_hit(5000, "a potion of Death", MON_HARMFUL_POTION);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_INFRA_VISION:
		{
			if (add_flag(TIMED_INFRA, 100 + randint(100)))
			{
				(*ident) = TRUE;
			}
			return SUCCESS;
		}

		case OBJ_POTION_DETECT_INVIS:
		{
			if (add_flag(TIMED_INVIS, 12 + randint(12)))
			{
				(*ident) = TRUE;
			}
			return SUCCESS;
		}

		case OBJ_POTION_SLOW_POISON:
		{
			if (set_flag(TIMED_POISONED, p_ptr->poisoned / 2)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_NEUTRALIZE_POISON:
		{
			if (set_flag(TIMED_POISONED, 0)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_BOLDNESS:
		{
			if (set_flag(TIMED_AFRAID, 0)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_SPEED:
		{
			if (!p_ptr->fast)
			{
				if (set_flag(TIMED_FAST, randint(25) + 15)) (*ident) = TRUE;
			}
			else
			{
				(void)add_flag(TIMED_FAST, 5);
			}
			return SUCCESS;
		}

		case OBJ_POTION_RES_HEAT:
		{
			if (add_flag(TIMED_OPPOSE_FIRE, randint(10) + 10))
			{
				(*ident) = TRUE;
			}
			return SUCCESS;
		}

		case OBJ_POTION_RES_COLD:
		{
			if (add_flag(TIMED_OPPOSE_COLD, randint(10) + 10))
			{
				(*ident) = TRUE;
			}
			return SUCCESS;
		}

		case OBJ_POTION_HEROISM:
		{
            if (set_flag(TIMED_AFRAID, 0)) (*ident) = TRUE;
			if (add_flag(TIMED_HERO, randint(25) + 25)) (*ident) = TRUE;
			if (hp_player(10)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_BERSERK_STR:
		{
            if (set_flag(TIMED_AFRAID, 0)) (*ident) = TRUE;
			if (add_flag(TIMED_SHERO, randint(25) + 25)) (*ident) = TRUE;
			if (hp_player(30)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_CURE_LIGHT:
		{
			if (hp_player(damroll(2, 8))) (*ident) = TRUE;
			if (set_flag(TIMED_BLIND, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_CUT, p_ptr->cut - 10)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_CURE_SERIOUS:
		{
			if (hp_player(damroll(4, 8))) (*ident) = TRUE;
			if (set_flag(TIMED_BLIND, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_CONFUSED, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_CUT, (p_ptr->cut / 2) - 50)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_CURE_CRITICAL:
		{
			if (hp_player(damroll(6, 8))) (*ident) = TRUE;
			if (set_flag(TIMED_BLIND, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_CONFUSED, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_POISONED, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_STUN, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_CUT, 0)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_HEALING:
		{
			if (hp_player(300)) (*ident) = TRUE;
			if (set_flag(TIMED_BLIND, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_CONFUSED, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_POISONED, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_STUN, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_CUT, 0)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_STAR_HEALING:
		{
			if (hp_player(1200)) (*ident) = TRUE;
			if (set_flag(TIMED_BLIND, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_CONFUSED, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_POISONED, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_STUN, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_CUT, 0)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_LIFE:
		{
			msg_print("You feel life flow through your body!");
			restore_level();
			hp_player(5000);
			(void)set_flag(TIMED_POISONED, 0);
			(void)set_flag(TIMED_BLIND, 0);
			(void)set_flag(TIMED_CONFUSED, 0);
			(void)set_flag(TIMED_IMAGE, 0);
			(void)set_flag(TIMED_STUN, 0);
			(void)set_flag(TIMED_CUT, 0);
			(void)do_res_stat(A_STR);
			(void)do_res_stat(A_CON);
			(void)do_res_stat(A_DEX);
			(void)do_res_stat(A_WIS);
			(void)do_res_stat(A_INT);
			(void)do_res_stat(A_CHR);
			(*ident) = TRUE;
			return SUCCESS;
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
				(*ident) = TRUE;
			}
			if (p_ptr->cchi < p_ptr->mchi)
			{
				p_ptr->cchi = p_ptr->mchi;
				p_ptr->chi_frac = 0;
				msg_print("Your feel your chi harmonise.");
				p_ptr->redraw |= (PR_MANA);
				p_ptr->window |= (PW_PLAYER);
				p_ptr->window |= (PW_SPELL);
				(*ident) = TRUE;
			}
			return SUCCESS;
		}

		case OBJ_POTION_RES_LIFE_LEVELS:
		{
			if (restore_level()) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_RES_STR:
		{
			if (do_res_stat(A_STR)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_RES_INT:
		{
			if (do_res_stat(A_INT)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_RES_WIS:
		{
			if (do_res_stat(A_WIS)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_RES_DEX:
		{
			if (do_res_stat(A_DEX)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_RES_CON:
		{
			if (do_res_stat(A_CON)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_RES_CHR:
		{
			if (do_res_stat(A_CHR)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_INC_STR:
		{
			if (do_inc_stat(A_STR)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_INC_INT:
		{
			if (do_inc_stat(A_INT)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_INC_WIS:
		{
			if (do_inc_stat(A_WIS)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_INC_DEX:
		{
			if (do_inc_stat(A_DEX)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_INC_CON:
		{
			if (do_inc_stat(A_CON)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_INC_CHR:
		{
			if (do_inc_stat(A_CHR)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_AUGMENTATION:
		{
			if (do_inc_stat(A_STR)) (*ident) = TRUE;
			if (do_inc_stat(A_INT)) (*ident) = TRUE;
			if (do_inc_stat(A_WIS)) (*ident) = TRUE;
			if (do_inc_stat(A_DEX)) (*ident) = TRUE;
			if (do_inc_stat(A_CON)) (*ident) = TRUE;
			if (do_inc_stat(A_CHR)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_ENLIGHTENMENT:
		{
			msg_print("An image of your surroundings forms in your mind...");
			wiz_lite();
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_STAR_ENLIGHTENMENT:
		{
			msg_print("You begin to feel more enlightened...");
			msg_print(NULL);
			wiz_lite();
			(void)do_inc_stat(A_INT);
			(void)do_inc_stat(A_WIS);
			(void)detect_traps();
			(void)detect_doors();
			(void)detect_stairs();
			(void)detect_treasure();
			(void)detect_objects_gold();
			(void)detect_objects_normal();
			identify_pack();
			self_knowledge();
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_SELF_KNOWLEDGE:
		{
			msg_print("You begin to know yourself a little better...");
			msg_print(NULL);
			self_knowledge();
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_POTION_EXPERIENCE:
		{
				msg_print("You feel more experienced.");
				gain_skills(200);
				(*ident) = TRUE;
			return SUCCESS;
		}

        case OBJ_POTION_RESISTANCE:
        {
            (void)add_flag(TIMED_OPPOSE_ACID, randint(20) + 20);
			(void)add_flag(TIMED_OPPOSE_ELEC, randint(20) + 20);
			(void)add_flag(TIMED_OPPOSE_FIRE, randint(20) + 20);
			(void)add_flag(TIMED_OPPOSE_COLD, randint(20) + 20);
			(void)add_flag(TIMED_OPPOSE_POIS, randint(20) + 20);
            (*ident) = TRUE;
        return SUCCESS;
        }

        case OBJ_POTION_CURING:
        {
            if (hp_player(50)) (*ident) = TRUE;
            if (set_flag(TIMED_BLIND, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_POISONED, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_CONFUSED, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_STUN, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_CUT, 0)) (*ident) = TRUE;
            if (set_flag(TIMED_IMAGE, 0)) (*ident) = TRUE;
        return SUCCESS;
        }

        case OBJ_POTION_INVULNERABILITY:
        {
            (void)add_flag(TIMED_INVULN, randint(7) + 7);
            (*ident) = TRUE;
        return SUCCESS;
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
        (*ident) = TRUE;
        return SUCCESS;
        }
		case OBJ_SCROLL_DARKNESS:
		{
            if (!(p_ptr->resist_blind) && !(p_ptr->resist_dark))
			{
				(void)add_flag(TIMED_BLIND, 3 + randint(5));
			}
			if (unlite_area(10, 3)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_AGGRAVATE_MONSTER:
		{
			msg_print("There is a high pitched humming noise.");
			aggravate_monsters(1);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_CURSE_ARMOUR:
		{
			if (curse_armor()) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_CURSE_WEAPON:
		{
			if (curse_weapon()) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_SUMMON_MONSTER:
		{
			switch (rand_int(9))
			{
				case 0: case 1: case 2: i = 1; break;
				case 3: case 4: case 5: case 6: i = 2; break;
				default: i = 3;
			}

			for (; i; i--)
			{
				if (summon_specific(py, px, (dun_depth), SUMMON_ALL))
				{
					(*ident) = TRUE;
				}
			}
			return SUCCESS;
		}

		case OBJ_SCROLL_SUMMON_UNDEAD:
		{
			switch (rand_int(9))
			{
				case 0: case 1: case 2: i = 1; break;
				case 3: case 4: case 5: case 6: i = 2; break;
				default: i = 3;
			}

			for (; i; i--)
			{
				if (summon_specific(py, px, (dun_depth), SUMMON_UNDEAD))
				{
					(*ident) = TRUE;
				}
			}
			return SUCCESS;
		}

		case OBJ_SCROLL_TRAP_CREATION:
		{
			if (trap_creation()) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_PHASE_DOOR:
		{
			teleport_player(10);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_TELEPORTATION:
		{
			teleport_player(100);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_TELEPORT_LEVEL:
		{
			(void)teleport_player_level();
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_WORD_OF_RECALL:
		{
			set_recall(FALSE);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_IDENTIFY:
		{
			(*ident) = TRUE;
			if (!ident_spell()) (*use) = FALSE;
			return SUCCESS;
		}

		case OBJ_SCROLL_STAR_IDENTIFY:
		{
			(*ident) = TRUE;
			if (!identify_fully()) (*use) = FALSE;
			return SUCCESS;
		}

		case OBJ_SCROLL_REMOVE_CURSE:
		{
			if (remove_curse())
			{
				msg_print("You feel as if someone is watching over you.");
				(*ident) = TRUE;
			}
			return SUCCESS;
		}

		case OBJ_SCROLL_STAR_REMOVE_CURSE:
		{
			remove_all_curse();
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_ENCHANT_ARMOUR:
		{
			(*ident) = TRUE;
			if (!enchant_spell(0, 0, 1)) (*use) = FALSE;
			return SUCCESS;
		}

		case OBJ_SCROLL_ENCHANT_WEAPON_TO_HIT:
		{
			if (!enchant_spell(1, 0, 0)) (*use) = FALSE;
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_ENCHANT_WEAPON_TO_DAM:
		{
			if (!enchant_spell(0, 1, 0)) (*use) = FALSE;
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_STAR_ENCHANT_ARMOUR:
		{
			if (!enchant_spell(0, 0, randint(3) + 2)) (*use) = FALSE;
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_STAR_ENCHANT_WEAPON:
		{
			if (!enchant_spell(randint(3), randint(3), 0)) (*use) = FALSE;
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_RECHARGING:
		{
			if (!recharge(60)) (*use) = FALSE;
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_LIGHT:
		{
			if (lite_area(damroll(2, 8), 2)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_MAGIC_MAPPING:
		{
			map_area();
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_TREASURE_DETECTION:
		{
			if (detect_treasure()) (*ident) = TRUE;
			if (detect_objects_gold()) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_OBJECT_DETECTION:
		{
			if (detect_objects_normal()) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_TRAP_DETECTION:
		case OBJ_STAFF_TRAP_LOCATION:
		case OBJ_ROD_TRAP_LOCATION:
		{
			if (known || detect_traps_p())
			{
				(*ident) = TRUE;
				detect_traps();
			}
			return SUCCESS;
		}

		case OBJ_SCROLL_DOOR_STAIR_LOCATION:
		{
			if (detect_doors()) (*ident) = TRUE;
			if (detect_stairs()) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_DETECT_INVIS:
		{
			if (detect_monsters_invis()) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_SATISFY_HUNGER:
		{
			if (set_flag(TIMED_FOOD, PY_FOOD_MAX - 1)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_BLESSING:
		{
			if (add_flag(TIMED_BLESSED, randint(12) + 6)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_HOLY_CHANT:
		{
			if (add_flag(TIMED_BLESSED, randint(24) + 12)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_HOLY_PRAYER:
		{
			if (add_flag(TIMED_BLESSED, randint(48) + 24)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_MONSTER_CONFUSION:
		{
			if (p_ptr->confusing == 0)
			{
				msg_print("Your hands begin to glow.");
				p_ptr->confusing = TRUE;
				(*ident) = TRUE;
			}
			return SUCCESS;
		}

		case OBJ_SCROLL_PROTECTION_FROM_EVIL:
		{
			i = 3 * (skill_set[SKILL_DEVICE].value/2);
			if (add_flag(TIMED_PROTEVIL, randint(25) + i)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_RUNE_OF_PROTECTION:
		{
			warding_glyph();
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_TRAP_DOOR_DESTRUCTION:
		{
			if (destroy_doors_touch()) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_STAR_DESTRUCTION:
		{
			destroy_area(py, px, 15, TRUE);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_DISPEL_UNDEAD:
		{
			if (dispel_undead(60)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_GENOCIDE:
		{
			if (genocide(TRUE) == POWER_ERROR_ABORT) (*use) = FALSE;
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_MASS_GENOCIDE:
		{
			(void)mass_genocide(TRUE);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_ACQUIREMENT:
		{
			acquirement(py, px, 1, TRUE);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_SCROLL_STAR_ACQUIREMENT:
		{
			acquirement(py, px, randint(2) + 1, TRUE);
			(*ident) = TRUE;
			return SUCCESS;
		}

        /* New Zangband scrolls */
        case OBJ_SCROLL_FIRE:
        {
            fire_ball(GF_FIRE, 0,
                150, 4); /* Note: "Double" damage since it is centered on
                            the player ... */
            if (!(p_ptr->oppose_fire || p_ptr->resist_fire || p_ptr->immune_fire))
                take_hit(50+randint(50), "a Scroll of Fire", MON_HARMFUL_SCROLL);
            (*ident) = TRUE;
            return SUCCESS;
        }


        case OBJ_SCROLL_ICE:
        {
            fire_ball(GF_ICE, 0,
                175, 4);
            if (!(p_ptr->oppose_cold || p_ptr->resist_cold || p_ptr->immune_cold))
                take_hit(100+randint(100), "a Scroll of Ice", MON_HARMFUL_SCROLL);
            (*ident) = TRUE;
            return SUCCESS;
        }

        case OBJ_SCROLL_CHAOS:
        {
            fire_ball(GF_CHAOS, 0,
                222, 4);
            if (!p_ptr->resist_chaos)
                take_hit(111+randint(111), "a Scroll of Chaos", MON_HARMFUL_SCROLL);
            (*ident) = TRUE;
            return SUCCESS;
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
            (*ident) = TRUE;
            return SUCCESS;
        }

        case OBJ_SCROLL_ARTIFACT_CREATION:
        {
            (void) artifact_scroll();
            (*ident) = TRUE;
            return SUCCESS;
        }
		case OBJ_STAFF_DARKNESS:
		{
            if (!(p_ptr->resist_blind) && !(p_ptr->resist_dark))
			{
				if (add_flag(TIMED_BLIND, 3 + randint(5))) (*ident) = TRUE;
			}
			if (unlite_area(10, 3)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_SLOWNESS:
		{
			if (add_flag(TIMED_SLOW, randint(30) + 15)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_HASTE_MONSTERS:
		{
			if (speed_monsters()) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_SUMMONING:
		{
			switch (rand_int(32))
			{
				case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
					i = 1; break;
				case 20: case 21: case 22: case 23: case 24: case 25: case 26:
				case 27: case 28:
					i = 3; break;
				case 29: case 30: case 31:
					i = 4; break;
				default: i = 2; break;
			}
			for (; i; i--)
			{
				if (summon_specific(py, px, (dun_depth), SUMMON_ALL))
				{
					(*ident) = TRUE;
				}
			}
			return SUCCESS;
		}

		case OBJ_STAFF_TELEPORTATION:
		{
			teleport_player(100);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_PERCEPTION:
		{
			if (!ident_spell()) (*use) = FALSE;
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_REMOVE_CURSE:
		{
			if (remove_curse())
			{
				if (!p_ptr->blind)
				{
					msg_print("The staff glows blue for a moment...");
				}
				(*ident) = TRUE;
			}
			return SUCCESS;
		}

		case OBJ_STAFF_STARLIGHT:
		{
			if (!p_ptr->blind)
			{
				msg_print("The end of the staff glows brightly...");
			}
			for (i = 0; i < 8; i++) lite_line(ddd[i]);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_LIGHT:
		{
			if (lite_area(damroll(2, 8), 2)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_ENLIGHTENMENT:
		{
			map_area();
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_TREASURE_LOCATION:
		{
			if (detect_treasure()) (*ident) = TRUE;
			if (detect_objects_gold()) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_OBJECT_LOCATION:
		{
			if (detect_objects_normal()) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_DOOR_STAIR_LOCATION:
		{
			if (detect_doors()) (*ident) = TRUE;
			if (detect_stairs()) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_DETECT_INVIS:
		{
			if (detect_monsters_invis()) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_DETECT_EVIL:
		{
			if (detect_monsters_evil()) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_CURE_LIGHT:
		{
			if (hp_player(randint(8))) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_CURING:
		{
			if (set_flag(TIMED_BLIND, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_POISONED, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_CONFUSED, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_STUN, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_CUT, 0)) (*ident) = TRUE;
            if (set_flag(TIMED_IMAGE, 0)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_HEALING:
		{
			if (hp_player(300)) (*ident) = TRUE;
			if (set_flag(TIMED_STUN, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_CUT, 0)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_THE_MAGI:
		{
			if (do_res_stat(A_INT)) (*ident) = TRUE;
			if (p_ptr->csp < p_ptr->msp)
			{
				p_ptr->csp = p_ptr->msp;
				p_ptr->csp_frac = 0;
				(*ident) = TRUE;
				msg_print("Your feel your head clear.");
				p_ptr->redraw |= (PR_MANA);
				p_ptr->window |= (PW_PLAYER);
				p_ptr->window |= (PW_SPELL);
			}
			if (p_ptr->cchi < p_ptr->mchi)
			{
				p_ptr->cchi = p_ptr->mchi;
				p_ptr->chi_frac = 0;
				(*ident) = TRUE;
				msg_print("Your feel your chi harmonise.");
				p_ptr->redraw |= (PR_MANA);
				p_ptr->window |= (PW_PLAYER);
				p_ptr->window |= (PW_SPELL);
			}
			return SUCCESS;
		}

		case OBJ_STAFF_SLEEP_MONSTERS:
		{
			if (sleep_monsters((skill_set[SKILL_DEVICE].value/2))) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_SLOW_MONSTERS:
		{
			if (slow_monsters((skill_set[SKILL_DEVICE].value/2))) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_SPEED:
		{
			if (!p_ptr->fast)
			{
				if (set_flag(TIMED_FAST, randint(30) + 15)) (*ident) = TRUE;
			}
			else
			{
				(void)add_flag(TIMED_FAST, 5);
			}
			return SUCCESS;
		}

		case OBJ_STAFF_PROBING:
		{
			probing();
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_DISPEL_EVIL:
		{
			if (dispel_evil(60)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_POWER:
		{
			if (dispel_monsters(120)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_HOLINESS:
		{
			if (dispel_evil(120)) (*ident) = TRUE;
			i = 3 * (skill_set[SKILL_DEVICE].value/2);
			if (add_flag(TIMED_PROTEVIL, randint(25) + i)) (*ident) = TRUE;
			if (set_flag(TIMED_POISONED, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_AFRAID, 0)) (*ident) = TRUE;
			if (hp_player(50)) (*ident) = TRUE;
			if (set_flag(TIMED_STUN, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_CUT, 0)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_GENOCIDE:
		{
			if (genocide(TRUE) == POWER_ERROR_ABORT) (*use) = FALSE;
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_EARTHQUAKES:
		{
			earthquake(py, px, 10);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_STAFF_STAR_DESTRUCTION:
		{
			destroy_area(py, px, 15, TRUE);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_HEAL_MONSTER:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			if (heal_monster(dir)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_HASTE_MONSTER:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			if (speed_monster(dir,(skill_set[SKILL_DEVICE].value/2))) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_CLONE_MONSTER:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			if (clone_monster(dir)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_TELEPORT_OTHER:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			if (teleport_monster(dir)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_DISARMING:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			if (disarm_trap(dir)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_TRAP_DOOR_DESTRUCTION:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			if (destroy_door(dir)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_STONE_TO_MUD:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			if (wall_to_mud(dir)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_LIGHT:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			msg_print("A line of blue shimmering light appears.");
			lite_line(dir);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_SLEEP_MONSTER:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			if (sleep_monster(dir,(skill_set[SKILL_DEVICE].value/2))) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_SLOW_MONSTER:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			if (slow_monster(dir,(skill_set[SKILL_DEVICE].value/2))) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_CONFUSE_MONSTER:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			if (confuse_monster(dir, 10)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_SCARE_MONSTER:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			if (fear_monster(dir, 10)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_DRAIN_LIFE:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			if (drain_life(dir, 75)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_POLYMORPH:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			if (poly_monster(dir,(skill_set[SKILL_DEVICE].value/2))) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_STINKING_CLOUD:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			fire_ball(GF_POIS, dir, 12, 2);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_MAGIC_MISSILE:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			fire_bolt_or_beam(20, GF_MISSILE, dir, damroll(2, 6));
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_ACID_BOLT:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
            fire_bolt_or_beam(20, GF_ACID, dir, damroll(3, 8));
			(*ident) = TRUE;
			return SUCCESS;
        }

        case OBJ_WAND_TAME_MONSTER:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
           if (charm_monster(dir, 45))
             (*ident) = TRUE;
           return SUCCESS;
        }

		case OBJ_WAND_FIRE_BOLT:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			fire_bolt_or_beam(20, GF_FIRE, dir, damroll(6, 8));
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_COLD_BOLT:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			fire_bolt_or_beam(20, GF_COLD, dir, damroll(3, 8));
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_ACID_BALL:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			fire_ball(GF_ACID, dir, 60, 2);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_ELEC_BALL:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
            fire_ball(GF_ELEC, dir, 32, 2);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_FIRE_BALL:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			fire_ball(GF_FIRE, dir, 72, 2);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_COLD_BALL:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			fire_ball(GF_COLD, dir, 48, 2);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_DRAGON_FIRE:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			fire_ball(GF_FIRE, dir, 100, 3);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_DRAGON_COLD:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			fire_ball(GF_COLD, dir, 80, 3);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_DRAGON_BREATH:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
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

			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_WAND_ANNIHILATION:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			if (drain_life(dir, 125)) (*ident) = TRUE;
			return SUCCESS;
		}
        case OBJ_WAND_SHARD_BALL:
        {
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
            fire_ball(GF_SHARD, dir, 75 + (randint(50)), 2);
            (*ident) = TRUE;
			return SUCCESS;
        }
		case OBJ_WAND_WONDER:
		{
			return do_power(choose_random_wand(), plev, dir, FALSE, use, ident);
		}

		case OBJ_ROD_DOOR_STAIR_LOCATION:
		{
			if (detect_doors()) (*ident) = TRUE;
			if (detect_stairs()) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_ROD_PERCEPTION:
		{
			(*ident) = TRUE;
			if (!ident_spell()) (*use) = FALSE;
			return SUCCESS;
		}

		case OBJ_ROD_RECALL:
		{
			set_recall(FALSE);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_ROD_ILLUMINATION:
		{
			if (lite_area(damroll(2, 8), 2)) (*ident) = TRUE;
 			return SUCCESS;
		}

		case OBJ_ROD_ENLIGHTENMENT:
		{
			map_area();
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_ROD_DETECTION:
		{
			detect_all();
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_ROD_PROBING:
		{
			probing();
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_ROD_CURING:
		{
			if (set_flag(TIMED_BLIND, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_POISONED, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_CONFUSED, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_STUN, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_CUT, 0)) (*ident) = TRUE;
            if (set_flag(TIMED_IMAGE, 0)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_ROD_HEALING:
		{
			if (hp_player(500)) (*ident) = TRUE;
			if (set_flag(TIMED_STUN, 0)) (*ident) = TRUE;
			if (set_flag(TIMED_CUT, 0)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_ROD_RESTORATION:
		{
			if (restore_level()) (*ident) = TRUE;
			if (do_res_stat(A_STR)) (*ident) = TRUE;
			if (do_res_stat(A_INT)) (*ident) = TRUE;
			if (do_res_stat(A_WIS)) (*ident) = TRUE;
			if (do_res_stat(A_DEX)) (*ident) = TRUE;
			if (do_res_stat(A_CON)) (*ident) = TRUE;
			if (do_res_stat(A_CHR)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_ROD_SPEED:
		{
			if (!p_ptr->fast)
			{
				if (set_flag(TIMED_FAST, randint(30) + 15)) (*ident) = TRUE;
			}
			else
			{
				(void)add_flag(TIMED_FAST, 5);
			}
			return SUCCESS;
		}

		case OBJ_ROD_TELEPORT_OTHER:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			if (teleport_monster(dir)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_ROD_DISARMING:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			if (disarm_trap(dir)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_ROD_LIGHT:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			msg_print("A line of blue shimmering light appears.");
			lite_line(dir);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_ROD_SLEEP_MONSTER:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			if (sleep_monster(dir,(skill_set[SKILL_DEVICE].value/2))) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_ROD_SLOW_MONSTER:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			if (slow_monster(dir,(skill_set[SKILL_DEVICE].value/2))) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_ROD_DRAIN_LIFE:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			if (drain_life(dir, 75)) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_ROD_POLYMORPH:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			if (poly_monster(dir,(skill_set[SKILL_DEVICE].value/2))) (*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_ROD_ACID_BOLT:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			fire_bolt_or_beam(10, GF_ACID, dir, damroll(6, 8));
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_ROD_ELEC_BOLT:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			fire_bolt_or_beam(10, GF_ELEC, dir, damroll(3, 8));
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_ROD_FIRE_BOLT:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			fire_bolt_or_beam(10, GF_FIRE, dir, damroll(8, 8));
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_ROD_COLD_BOLT:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			fire_bolt_or_beam(10, GF_COLD, dir, damroll(5, 8));
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_ROD_ACID_BALL:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			fire_ball(GF_ACID, dir, 60, 2);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_ROD_ELEC_BALL:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			fire_ball(GF_ELEC, dir, 32, 2);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_ROD_FIRE_BALL:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			fire_ball(GF_FIRE, dir, 72, 2);
			(*ident) = TRUE;
			return SUCCESS;
		}

		case OBJ_ROD_COLD_BALL:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			fire_ball(GF_COLD, dir, 48, 2);
			(*ident) = TRUE;
			return SUCCESS;
		}

        case OBJ_ROD_HAVOC:
		{
            call_chaos(skill_set[SKILL_DEVICE].value/2);
			(*ident) = TRUE;
			return SUCCESS;
		}

           case ACT_SUNLIGHT-256:
            {
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                msg_print("A line of sunlight appears.");
                lite_line(dir);
                   return SUCCESS;
               }

            case ACT_BO_MISS_1-256:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                msg_print("It glows extremely brightly...");
				fire_bolt(GF_MISSILE, dir, damroll(2, 6));
				return SUCCESS;
			}

            case ACT_BA_POIS_1-256:
			{
				if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                msg_print("It throbs deep green...");
				fire_ball(GF_POIS, dir, 12, 3);
				return SUCCESS;
			}


            case ACT_BO_ELEC_1-256:
			{
				if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                msg_print("It is covered in sparks...");
				fire_bolt(GF_ELEC, dir, damroll(4, 8));
				return SUCCESS;
			}

            case ACT_BO_ACID_1-256:
			{
				if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                msg_print("It is covered in acid...");
				fire_bolt(GF_ACID, dir, damroll(5, 8));
				return SUCCESS;
			}

            case ACT_BO_COLD_1-256:
            {
				if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                msg_print("It is covered in frost...");
				fire_bolt(GF_COLD, dir, damroll(6, 8));
				return SUCCESS;
			}

            case ACT_BO_FIRE_1-256:
			{
				if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                msg_print("It is covered in fire...");
				fire_bolt(GF_FIRE, dir, damroll(9, 8));
				return SUCCESS;
			}

            case ACT_BA_COLD_1-256:
            {
				if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                msg_print("It is covered in frost...");
				fire_ball(GF_COLD, dir, 48, 2);
				return SUCCESS;
			}

            case ACT_BA_FIRE_1-256:
			{
				if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                msg_print("It glows an intense red...");
				fire_ball(GF_FIRE, dir, 72, 2);
				return SUCCESS;
			}

            case ACT_DRAIN_1-256:
            {

				if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                msg_print("It glows black...");
                if (drain_life(dir, 100))
				return SUCCESS;
			}

            case ACT_BA_COLD_2-256:
            {
				if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                msg_print("It glows an intense blue...");
				fire_ball(GF_COLD, dir, 100, 2);
				return SUCCESS;
			}


            case ACT_BA_ELEC_2-256:
            {
				if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                msg_print("It crackles with electricity...");
                fire_ball(GF_ELEC, dir, 100, 3);
				return SUCCESS;
			}


            case ACT_DRAIN_2-256:
			{
				if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                msg_print("It glows black...");
				drain_life(dir, 120);
				return SUCCESS;
			}

            case ACT_VAMPIRE_1-256:
            {
				if (!dir) return POWER_ERROR_NO_SUCH_DIR;
               for (i = 0; i < 3; i++)
               {
                   if (drain_life(dir, 50))
                       hp_player(50);
                    }
                   return SUCCESS;
                }

            case ACT_BO_MISS_2-256:
            {
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                msg_print("It grows magical spikes...");
				fire_bolt(GF_ARROW, dir, 150);
				return SUCCESS;
			}

            case ACT_BA_FIRE_2-256:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                msg_print("It glows deep red...");
				fire_ball(GF_FIRE, dir, 120, 3);
				return SUCCESS;
			}

            case ACT_BA_COLD_3-256:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                msg_print("It glows bright white...");
				fire_ball(GF_COLD, dir, 200, 3);
				return SUCCESS;
			}

            case ACT_BA_ELEC_3-256:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                msg_print("It glows deep blue...");
				fire_ball(GF_ELEC, dir, 250, 3);
				return SUCCESS;
			}

            case ACT_WHIRLWIND-256:
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
             return SUCCESS;
            }

            case ACT_VAMPIRE_2-256:
            {
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
               for (i = 0; i < 3; i++)
               {
                   if (drain_life(dir, 100))
                       hp_player(100);
                    }

                   return SUCCESS;
                }


            case ACT_CALL_CHAOS-256:
            {
                msg_print("It glows in scintillating colours...");
                call_chaos(skill_set[SKILL_DEVICE].value/2);
                return SUCCESS;
            }

            case ACT_SHARD-256:
            {
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                fire_ball(GF_SHARD, dir,
                        120 + (plev), 2);
                return SUCCESS;
            }

            case ACT_DISP_EVIL-256:
			{
                msg_print("It floods the area with goodness...");
				dispel_evil(plev * 5);
				return SUCCESS;
			}

            case ACT_BA_MISS_3-256:
            {
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                    msg_print("You breathe the elements.");
                    fire_ball(GF_MISSILE, dir, 300, -4);
                return SUCCESS;
            }


            case ACT_DISP_GOOD-256:
			{
                msg_print("It floods the area with evil...");
                dispel_good(plev * 5);
				return SUCCESS;
			}


/* Activate for other offensive action */

            case ACT_CONFUSE-256:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                msg_print("It glows in scintillating colours...");
				confuse_monster(dir, 20);
				return SUCCESS;
			}

            case ACT_SLEEP-256:
			{
                msg_print("It glows deep blue...");
				sleep_monsters_touch((skill_set[SKILL_DEVICE].value/2));
				return SUCCESS;
			}

            case ACT_QUAKE-256:
            {
                earthquake(py, px, 10);
                   return SUCCESS;
                }

            case ACT_TERROR-256:
            {
                turn_monsters(40 + plev);
                return SUCCESS;
            }
            case ACT_TELE_AWAY-256:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                   (void)fire_beam(GF_AWAY_ALL, dir, plev);
                   return SUCCESS;
			}

            case ACT_BANISH_EVIL-256:
            {
                if (banish_evil(100))
                {
                    msg_print("The power of the artifact banishes evil!");
                }
                return SUCCESS;
            }

            case ACT_GENOCIDE-256:
			{
                msg_print("It glows deep blue...");
				if (genocide(TRUE) == POWER_ERROR_ABORT) (*use) = FALSE;
				return SUCCESS;
			}

            case ACT_MASS_GENO-256:
            {
                msg_print("It lets out a long, shrill note...");
				(void)mass_genocide(TRUE);
				return SUCCESS;
			}


/* Activate for summoning / charming */


           case ACT_CHARM_ANIMAL-256:
           {
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
             (void) charm_animal(dir, plev);
             return SUCCESS;
            }

            case ACT_CHARM_UNDEAD-256:
            {
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
               (void)control_one_undead(dir, plev);
                   return SUCCESS;
                }


            case ACT_CHARM_OTHER-256:
            {
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                 (void) charm_monster(dir, plev);
               return SUCCESS;
            }


            case ACT_CHARM_ANIMALS-256:
            {
            (void) charm_animals(plev * 2);
             return SUCCESS;
            }

            case ACT_CHARM_OTHERS-256:
            {
               charm_monsters(plev * 2);
		       return SUCCESS;
            }


            case ACT_SUMMON_ANIMAL-256:
            {
                 (void)summon_specific_friendly(py, px, plev, SUMMON_ANIMAL_RANGER, TRUE);
                     return SUCCESS;
                }

            case ACT_SUMMON_PHANTOM-256:
            {
                msg_print("You summon a phantasmal servant.");
                (void)summon_specific_friendly(py, px, (dun_depth), SUMMON_PHANTOM, TRUE);
                return SUCCESS;
            }

            case ACT_SUMMON_ELEMENTAL-256:
            {
                       if (randint(3) == 1) {
                   if (summon_specific((int)py,(int)px, (int)(plev * 1.5),
                           SUMMON_ELEMENTAL)) {
                   msg_print("An elemental materializes...");
                   msg_print("You fail to control it!");
                   }
               } else {
                   if (summon_specific_friendly((int)py, (int)px,
				   	(int)(plev * 1.5), SUMMON_ELEMENTAL, plev == 50))
				{
                   msg_print("An elemental materializes...");
                   msg_print("It seems obedient to you.");
                   }
               }
               return SUCCESS;
            }

            case ACT_SUMMON_DEMON-256:
            {
                       if (randint(3) == 1) {
                   if (summon_specific((int)py, (int)px, (int)(plev * 1.5),
                           SUMMON_DEMON)) {
                   msg_print("The area fills with a stench of sulphur and brimstone.");
                   msg_print("'NON SERVIAM! Wretch! I shall feast on thy mortal soul!'");
                   }
               } else {
                   if (summon_specific_friendly((int)py, (int)px, (int)(plev * 1.5),
                                SUMMON_DEMON, plev == 50 )) {
                   msg_print("The area fills with a stench of sulphur and brimstone.");
                   msg_print("'What is thy bidding... Master?'");
                   }
               }
               return SUCCESS;
            }

            case ACT_SUMMON_UNDEAD-256:
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
                           ((plev > 24) && (randint(3) == 1)))) {
                   msg_print("Cold winds begin to blow around you, carrying with them the stench of decay...");
                   msg_print("Ancient, long-dead forms arise from the ground to serve you!");
                   }
               }
               return SUCCESS;
            }
                



/* Activate for healing */


            case ACT_CURE_LW-256:
            {
                (void)set_flag(TIMED_AFRAID, 0);
				(void)hp_player(30);
                return SUCCESS;
            }

            case ACT_CURE_MW-256:
            {
                msg_print("It radiates deep purple...");
				hp_player(damroll(4, 8));
				(void)set_flag(TIMED_CUT, (p_ptr->cut / 2) - 50);
				return SUCCESS;
			}


            case ACT_CURE_POISON-256:
			{
                msg_print("It glows deep blue...");
				(void)set_flag(TIMED_AFRAID, 0);
				(void)set_flag(TIMED_POISONED, 0);
				return SUCCESS;
			}

            case ACT_REST_LIFE-256:
            {
                msg_print("It glows a deep red...");
				restore_level();
				return SUCCESS;
			}

            case ACT_REST_ALL-256:
            {
                msg_print("It glows a deep green...");
                (void)do_res_stat(A_STR);
                (void)do_res_stat(A_INT);
                (void)do_res_stat(A_WIS);
                (void)do_res_stat(A_DEX);
                (void)do_res_stat(A_CON);
                (void)do_res_stat(A_CHR);
                (void)restore_level();
                   return SUCCESS;
                }


            case ACT_CURE_700-256:
			{
                msg_print("It glows deep blue...");
				msg_print("You feel a warm tingling inside...");
                (void)hp_player(700);
				(void)set_flag(TIMED_CUT, 0);
				return SUCCESS;
			}


            case ACT_CURE_1000-256:
			{
                msg_print("It glows a bright white...");
				msg_print("You feel much better...");
				(void)hp_player(1000);
				(void)set_flag(TIMED_CUT, 0);
				return SUCCESS;
			}

/* Activate for timed effect */

            case ACT_ESP-256:
            {
                (void)add_flag(TIMED_ESP, randint(30) + 25);
                   return SUCCESS;
                }

            case ACT_BERSERK-256:
            {
				(void)add_flag(TIMED_SHERO, randint(50) + 50);
				(void)add_flag(TIMED_BLESSED, randint(50) + 50);
                return SUCCESS;
            }

            case ACT_PROT_EVIL-256:
            {
                msg_print("It lets out a shrill wail...");
				i = 3 * plev;
				(void)add_flag(TIMED_PROTEVIL, randint(25) + i);
				return SUCCESS;
			}

            case ACT_RESIST_ALL-256:
			{
                msg_print("It glows many colours...");
                (void)add_flag(TIMED_OPPOSE_ACID, randint(40) + 40);
                (void)add_flag(TIMED_OPPOSE_ELEC, randint(40) + 40);
                (void)add_flag(TIMED_OPPOSE_FIRE, randint(40) + 40);
                (void)add_flag(TIMED_OPPOSE_COLD, randint(40) + 40);
                (void)add_flag(TIMED_OPPOSE_POIS, randint(40) + 40);
				return SUCCESS;
			}


            case ACT_SPEED-256:
			{
                msg_print("It glows bright green...");
				if (!p_ptr->fast)
				{
					(void)set_flag(TIMED_FAST, randint(20) + 20);
				}
				else
				{
					(void)add_flag(TIMED_FAST, 5);
				}
				return SUCCESS;
			}

            case ACT_XTRA_SPEED-256:
			{
                msg_print("It glows brightly...");
				if (!p_ptr->fast)
				{
					(void)set_flag(TIMED_FAST, randint(75) + 75);
				}
				else
				{
					(void)add_flag(TIMED_FAST, 5);
				}
				return SUCCESS;
			}

            case ACT_WRAITH-256:
            {
                add_flag(TIMED_WRAITH, randint(plev/2) + (plev/2));
                return SUCCESS;
            }


            case ACT_INVULN-256:
            {
                (void)add_flag(TIMED_INVULN, randint(8) + 8);
                   return SUCCESS;
                }



/* Activate for general purpose effect (detection etc.) */

            case ACT_LIGHT-256:
            {
                msg_print("It wells with clear light...");
				lite_area(damroll(2, 15), 3);
				return SUCCESS;
			}

            case ACT_MAP_LIGHT-256:
			{
                msg_print("It shines brightly...");
				map_area();
				lite_area(damroll(2, 15), 3);
				return SUCCESS;
			}

            case ACT_DETECT_ALL-256:
			{
                msg_print("It glows bright white...");
				msg_print("An image forms in your mind...");
				detect_all();
				return SUCCESS;
			}

            case ACT_DETECT_XTRA-256:
			{
                msg_print("It glows brightly...");
                detect_all();
				probing();
                identify_fully();
				return SUCCESS;
			}

            case ACT_ID_FULL-256:
            {
            msg_print("It glows yellow...");
            identify_fully();
                   return SUCCESS;
                }


            case ACT_ID_PLAIN-256:
			{

                if (!ident_spell()) (*ident) = FALSE;
				return SUCCESS;
			}

            case ACT_RUNE_EXPLO-256:
            {
                msg_print("It glows bright red...");
                   explosive_rune();
                   return SUCCESS;
            }

            case ACT_RUNE_PROT-256:
            {
                    msg_print("It glows light blue...");
                   warding_glyph();
                   return SUCCESS;
                }

            case ACT_SATIATE-256:
            {
                (void)set_flag(TIMED_FOOD, PY_FOOD_MAX - 1);
                   return SUCCESS;
                }

            case ACT_DEST_DOOR-256:
            {
                msg_print("It glows bright red...");
				destroy_doors_touch();
				return SUCCESS;
			}

            case ACT_STONE_MUD-256:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                msg_print("It pulsates...");
				wall_to_mud(dir);
				return SUCCESS;
			}

            case ACT_RECHARGE-256:
			{

				recharge(60);
				return SUCCESS;
			}

            case ACT_ALCHEMY-256:
            {
                    msg_print("It glows bright yellow...");
                   (void) alchemy();
                   return SUCCESS;
                }


            case ACT_DIM_DOOR-256:
           {
				if (!dimension_door(plev, 10)) (*ident) = FALSE;
                 return SUCCESS;
            }


            case ACT_TELEPORT-256:
			{
                msg_print("It twists space around you...");
				teleport_player(100);
				return SUCCESS;
			}

			case ACT_TELEPORT_WAIT-256:
			{
				teleport_player(100);
				return SUCCESS;
			}

		case ACT_RECALL-256:
		{
			if (dun_level && (p_ptr->max_dlv > dun_level) && (recall_dungeon == cur_dungeon))
			{
				if (get_check("Reset recall depth? "))
				p_ptr->max_dlv = dun_level;
			}

			msg_print("It glows soft white...");
			set_recall(FALSE);
			return SUCCESS;
		}
	          case OBJ_RING_ACID:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                fire_ball(GF_ACID, dir, 50, 2);
                (void)add_flag(TIMED_OPPOSE_ACID, randint(20) + 20);

				return SUCCESS;
			}

            case OBJ_RING_ICE:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                fire_ball(GF_COLD, dir, 50, 2);
                (void)add_flag(TIMED_OPPOSE_COLD, randint(20) + 20);
				return SUCCESS;
            }

            case OBJ_RING_FIRE:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                fire_ball(GF_FIRE, dir, 50, 2);
                (void)add_flag(TIMED_OPPOSE_FIRE, randint(20) + 20);
				return SUCCESS;
            }
			case OBJ_DSM_BLUE:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("You breathe lightning.");
				fire_ball(GF_ELEC, dir, 100, -2);
				return SUCCESS;
			}

			case OBJ_DSM_WHITE:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("You breathe frost.");
				fire_ball(GF_COLD, dir, 110, -2);
				return SUCCESS;
			}

			case OBJ_DSM_BLACK:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("You breathe acid.");
				fire_ball(GF_ACID, dir, 130, -2);
				return SUCCESS;
			}

			case OBJ_DSM_GREEN:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("You breathe poison gas.");
				fire_ball(GF_POIS, dir, 150, -2);
				return SUCCESS;
			}

			case OBJ_DSM_RED:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("You breathe fire.");
				fire_ball(GF_FIRE, dir, 200, -2);
				return SUCCESS;
			}

			case OBJ_DSM_MULTI_HUED:
			{
				if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				i = rand_int(5);
				msg_format("You breathe %s.",
				           ((i == 1) ? "lightning" :
				            ((i == 2) ? "frost" :
				             ((i == 3) ? "acid" :
				              ((i == 4) ? "poison gas" : "fire")))));
				fire_ball(((i == 1) ? GF_ELEC :
				           ((i == 2) ? GF_COLD :
				            ((i == 3) ? GF_ACID :
				             ((i == 4) ? GF_POIS : GF_FIRE)))),
				          dir, 250, -2);
				return SUCCESS;
			}

			case OBJ_DSM_BRONZE:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("You breathe confusion.");
				fire_ball(GF_CONFUSION, dir, 120, -2);
				return SUCCESS;
			}

			case OBJ_DSM_GOLD:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("You breathe sound.");
				fire_ball(GF_SOUND, dir, 130, -2);
				return SUCCESS;
			}

			case OBJ_DSM_CHAOS:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				i = rand_int(2);
				msg_format("You breathe %s.",
				           ((i == 1 ? "chaos" : "disenchantment")));
				fire_ball((i == 1 ? GF_CHAOS : GF_DISENCHANT),
				          dir, 220, -2);
				return SUCCESS;
			}

			case OBJ_DSM_LAW:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				i = rand_int(2);
				msg_format("You breathe %s.",
				           ((i == 1 ? "sound" : "shards")));
				fire_ball((i == 1 ? GF_SOUND : GF_SHARDS),
				          dir, 230, -2);
				return SUCCESS;
			}

			case OBJ_DSM_BALANCE:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				i = rand_int(4);
				msg_format("You breathe %s.",
				           ((i == 1) ? "chaos" :
				            ((i == 2) ? "disenchantment" :
				             ((i == 3) ? "sound" : "shards"))));
				fire_ball(((i == 1) ? GF_CHAOS :
				           ((i == 2) ? GF_DISENCHANT :
				            ((i == 3) ? GF_SOUND : GF_SHARDS))),
				          dir, 250, -2);
				return SUCCESS;
			}

			case OBJ_DSM_PSEUDO:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				i = rand_int(2);
				msg_format("You breathe %s.",
				           ((i == 0 ? "light" : "darkness")));
				fire_ball((i == 0 ? GF_LITE : GF_DARK), dir, 200, -2);
				return SUCCESS;
			}

			case OBJ_DSM_POWER:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("You breathe the elements.");
                fire_ball(GF_MISSILE, dir, 300, -3);
				return SUCCESS;
			}
			case ART_POLARIS-512:
            {
				msg_print("The essence wells with clear light...");
				lite_area(damroll(2, 15), 3);
				return SUCCESS;
			}

			case ART_XOTH-512:
			{
				msg_print("The essence shines brightly...");
				map_area();
				lite_area(damroll(2, 15), 3);
				return SUCCESS;
			}

			case ART_TRAPEZOHEDRON-512:
			{
                msg_print("The gemstone flashes bright red!");
				wiz_lite();
                msg_print("The gemstone drains your vitality...");
                take_hit(damroll(3,8), "the Gemstone 'Trapezohedron'", MON_DANGEROUS_EQUIPMENT);
				(void)detect_traps();
				(void)detect_doors();
				(void)detect_stairs();
                if (get_check("Activate recall? "))
                {
					set_recall(FALSE);
                }

				return SUCCESS;
			}


			case ART_LOBON-512:
			{
				msg_print("The amulet lets out a shrill wail...");
				i = 3 * (skill_set[SKILL_DEVICE].value/2);
				(void)add_flag(TIMED_PROTEVIL, randint(25) + i);
				return SUCCESS;
			}

			case ART_ALHAZRED-512:
			{
				msg_print("The amulet floods the area with goodness...");
				dispel_evil((skill_set[SKILL_DEVICE].value/2) * 5);
				return SUCCESS;
			}

            case ART_MAGIC-512:
			{

			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                msg_print("You order Frakir to strangle your opponent.");
                if (drain_life(dir, 100))
				return SUCCESS;
			}


			case ART_BAST-512:
			{
				msg_print("The ring glows brightly...");
				if (!p_ptr->fast)
				{
					(void)set_flag(TIMED_FAST, randint(75) + 75);
				}
				else
				{
					(void)add_flag(TIMED_FAST, 5);
				}
				return SUCCESS;
			}

			case ART_ELEMFIRE-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("The ring glows deep red...");
				fire_ball(GF_FIRE, dir, 120, 3);
				return SUCCESS;
			}

			case ART_ELEMICE-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("The ring glows bright white...");
				fire_ball(GF_COLD, dir, 200, 3);
				return SUCCESS;
			}

			case ART_ELEMSTORM-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("The ring glows deep blue...");
				fire_ball(GF_ELEC, dir, 250, 3);
				return SUCCESS;
			}

			case ART_NYARLATHOTEP-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("The ring glows intensely black...");
				ring_of_power(dir);
				return SUCCESS;
			}


			case ART_RAZORBACK-512:
			{
				msg_print("Your armor is surrounded by lightning...");
				for (i = 0; i < 8; i++) fire_ball(GF_ELEC, ddd[i], 150, 3);
				return SUCCESS;
			}

			case ART_BLADETURNER-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                    msg_print("You breathe the elements.");
                    fire_ball(GF_MISSILE, dir, 300, -4);
                msg_print("Your armor glows many colours...");
                (void)set_flag(TIMED_AFRAID, 0);
				(void)add_flag(TIMED_SHERO, randint(50) + 50);
				(void)hp_player(30);
				(void)add_flag(TIMED_BLESSED, randint(50) + 50);
				(void)add_flag(TIMED_OPPOSE_ACID, randint(50) + 50);
				(void)add_flag(TIMED_OPPOSE_ELEC, randint(50) + 50);
				(void)add_flag(TIMED_OPPOSE_FIRE, randint(50) + 50);
				(void)add_flag(TIMED_OPPOSE_COLD, randint(50) + 50);
				(void)add_flag(TIMED_OPPOSE_POIS, randint(50) + 50);
				return SUCCESS;
			}


			case ART_SOULKEEPER-512:
			{
				msg_print("Your armor glows a bright white...");
				msg_print("You feel much better...");
				(void)hp_player(1000);
				(void)set_flag(TIMED_CUT, 0);
				return SUCCESS;
			}

			case ART_VAMPLORD-512:
			{
                msg_print("A heavenly choir sings...");
                (void)set_flag(TIMED_POISONED, 0);
                (void)set_flag(TIMED_CUT, 0);
                (void)set_flag(TIMED_STUN, 0);
                (void)set_flag(TIMED_CONFUSED, 0);
                (void)set_flag(TIMED_BLIND, 0);
                (void)add_flag(TIMED_HERO, randint(25) + 25);
                (void)hp_player(777);
				return SUCCESS;
			}

			case ART_ORCS-512:
			{
				msg_print("Your armor glows deep blue...");
				if (genocide(TRUE) == POWER_ERROR_ABORT) (*use) = FALSE;
				return SUCCESS;
			}

			case ART_OGRELORDS-512:
			{
				msg_print("Your armor glows bright red...");
				destroy_doors_touch();
				return SUCCESS;
			}

            case ART_POWER-512: case ART_MASK-512:
        
            {
                turn_monsters(40 + (skill_set[SKILL_DEVICE].value/2));
                return SUCCESS;
        
            }


			case ART_SKULLKEEPER-512:
			{
				msg_print("Your helm glows bright white...");
				msg_print("An image forms in your mind...");
				detect_all();
				return SUCCESS;
			}

			case ART_SUN-512:
			{
				msg_print("Your crown glows deep yellow...");
				msg_print("You feel a warm tingling inside...");
                (void)hp_player(700);
				(void)set_flag(TIMED_CUT, 0);
				return SUCCESS;
			}


			case ART_BARZAI-512:
			{
				msg_print("Your cloak glows many colours...");
				(void)add_flag(TIMED_OPPOSE_ACID, randint(20) + 20);
				(void)add_flag(TIMED_OPPOSE_ELEC, randint(20) + 20);
				(void)add_flag(TIMED_OPPOSE_FIRE, randint(20) + 20);
				(void)add_flag(TIMED_OPPOSE_COLD, randint(20) + 20);
				(void)add_flag(TIMED_OPPOSE_POIS, randint(20) + 20);
				return SUCCESS;
			}

			case ART_DARKNESS-512:
			{
				msg_print("Your cloak glows deep blue...");
				sleep_monsters_touch((skill_set[SKILL_DEVICE].value/2));
				return SUCCESS;
			}

			case ART_SWASHBUCKLER-512:
			{
				msg_print("Your cloak glows bright yellow...");
				recharge(60);
				return SUCCESS;
			}

			case ART_SHIFTER-512:
			{
				msg_print("Your cloak twists space around you...");
				teleport_player(100);
				return SUCCESS;
			}

			case ART_NYOGTHA-512:
			{
				msg_print("Your cloak glows a deep red...");
				restore_level();
				return SUCCESS;
			}


			case ART_LIGHT-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("Your gloves glow extremely brightly...");
				fire_bolt(GF_MISSILE, dir, damroll(2, 6));
				return SUCCESS;
			}

			case ART_IRONFIST-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("Your gauntlets are covered in fire...");
				fire_bolt(GF_FIRE, dir, damroll(9, 8));
				return SUCCESS;
			}

			case ART_GHOULS-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("Your gauntlets are covered in frost...");
				fire_bolt(GF_COLD, dir, damroll(6, 8));
				return SUCCESS;
			}

			case ART_WHITESPARK-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("Your gauntlets are covered in sparks...");
				fire_bolt(GF_ELEC, dir, damroll(4, 8));
				return SUCCESS;
			}

			case ART_DEAD-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("Your gauntlets are covered in acid...");
				fire_bolt(GF_ACID, dir, damroll(5, 8));
				return SUCCESS;
			}

			case ART_COMBAT-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("Your cesti grows magical spikes...");
				fire_bolt(GF_ARROW, dir, 150);
				return SUCCESS;
			}


			case ART_ITHAQUA-512:
			{
				msg_print("A wind swirls around your boots...");
				if (!p_ptr->fast)
				{
					(void)set_flag(TIMED_FAST, randint(20) + 20);
				}
				else
				{
					(void)add_flag(TIMED_FAST, 5);
				}
				return SUCCESS;
			}

			case ART_DANCING-512:
			{
				msg_print("Your boots glow deep blue...");
				(void)set_flag(TIMED_AFRAID, 0);
				(void)set_flag(TIMED_POISONED, 0);
				return SUCCESS;
			}


			case ART_FAITH-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("Your dagger is covered in fire...");
				fire_bolt(GF_FIRE, dir, damroll(9, 8));
				return SUCCESS;
			}

			case ART_HOPE-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("Your dagger is covered in frost...");
				fire_bolt(GF_COLD, dir, damroll(6, 8));
				return SUCCESS;
			}

			case ART_CHARITY-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("Your dagger is covered in sparks...");
				fire_bolt(GF_ELEC, dir, damroll(4, 8));
				return SUCCESS;
			}

			case ART_THOTH-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("Your dagger throbs deep green...");
				fire_ball(GF_POIS, dir, 12, 3);
				return SUCCESS;
			}

			case ART_ICICLE-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("Your dagger is covered in frost...");
				fire_ball(GF_COLD, dir, 48, 2);
				return SUCCESS;
			}

            case ART_KARAKAL-512:
			{
                switch(randint(13))
                {
                    case 1: case 2: case 3: case 4: case 5:
                        teleport_player(10);
                        return SUCCESS;
                    case 6: case 7: case 8: case 9: case 10:
                        teleport_player(222);
                        return SUCCESS;
                    case 11: case 12:
                        (void)stair_creation();
                        return SUCCESS;
                    default:
						if(get_check("Leave this level? "))
						{
							change_level(dun_level, START_RANDOM);
                        }
						return SUCCESS;
                }
			}

			case ART_STARLIGHT-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("Your sword glows an intense blue...");
				fire_ball(GF_COLD, dir, 100, 2);
				return SUCCESS;
			}

            case ART_DAWN-512:
            {
                msg_print("Your sword flickers black for a moment...");
                (void)summon_specific_friendly(py, px, (dun_depth), SUMMON_REAVER, TRUE);
                return SUCCESS;
            }

			case ART_EVERFLAME-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("Your sword glows an intense red...");
				fire_ball(GF_FIRE, dir, 72, 2);
				return SUCCESS;
			}


			case ART_THEODEN-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("Your axe blade glows black...");
				drain_life(dir, 120);
				return SUCCESS;
			}

			case ART_ODIN-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
                msg_print("Your spear crackles with electricity...");
                fire_ball(GF_ELEC, dir, 100, 3);
				return SUCCESS;
			}

			case ART_DESTINY-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("Your spear pulsates...");
				wall_to_mud(dir);
				return SUCCESS;
			}

			case ART_TROLLS-512:
			{
				msg_print("Your axe lets out a long, shrill note...");
				(void)mass_genocide(TRUE);
				return SUCCESS;
			}

			case ART_SPLEENSLICER-512:
			{
				msg_print("Your battle axe radiates deep purple...");
				hp_player(damroll(4, 8));
				(void)set_flag(TIMED_CUT, (p_ptr->cut / 2) - 50);
				return SUCCESS;
			}

			case ART_GNORRI-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("Your trident glows deep red...");
				teleport_monster(dir);
				return SUCCESS;
			}

			case ART_GHARNE-512:
			{
                if (dun_level && (p_ptr->max_dlv > dun_level) && (recall_dungeon == cur_dungeon))
                {
                    if (get_check("Reset recall depth? "))
                    p_ptr->max_dlv = dun_level;

                }
                
				msg_print("Your scythe glows soft white...");
				set_recall(FALSE);
				return SUCCESS;
			}


			case ART_TOTILA-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("Your flail glows in scintillating colours...");
				confuse_monster(dir, 20);
				return SUCCESS;
			}

			case ART_FIRESTAR-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("Your morning star rages in fire...");
				fire_ball(GF_FIRE, dir, 72, 3);
				return SUCCESS;
			}

			case ART_THUNDER-512:
			{
				msg_print("Your mace glows bright green...");
				if (!p_ptr->fast)
				{
					(void)set_flag(TIMED_FAST, randint(20) + 20);
				}
				else
				{
					(void)add_flag(TIMED_FAST, 5);
				}
				return SUCCESS;
			}

			case ART_ERIRIL-512:
			{
				msg_print("Your quarterstaff glows yellow...");
				if (!ident_spell()) (*use) = FALSE;
				return SUCCESS;
			}

			case ART_ATAL-512:
			{
				msg_print("Your quarterstaff glows brightly...");
                detect_all();
				probing();
                identify_fully();
				return SUCCESS;
			}

			case ART_JUSTICE-512:
			{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				msg_print("Your hammer glows white...");
				drain_life(dir, 90);
				return SUCCESS;
			}


			case ART_DEATH-512:
			{
				msg_print("Your crossbow glows deep red...");
				(void)brand_bolts();
				return SUCCESS;
			}

		case TV_FOOD-768:
		{
			msg_print("That tastes good.");
			(*ident) = TRUE;
			(*use) = FALSE;
			return SUCCESS;
		}

		case SP_PRECOGNITION-1024:
		{
			if (plev > 44)
			{
				wiz_lite();
			}
			else if (plev > 19)
			{
				map_area();
			}
			if (plev < 30)
			{
				b = detect_monsters_normal();
				if (plev > 19)  b |=  detect_monsters_invis();
				if (plev > 14)
				{
					b |= detect_doors();
					b |= detect_stairs();
				}
				if (plev > 4)
				{
					b |=  detect_traps();
				}
			}
			else
			{
				b = detect_all();
			}
			if (!b)  msg_print("You feel safe.");
			return SUCCESS;
		}
		case SP_NEURAL_BLAST-1024:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			if (randint(100) < plev * 2)
			{
				fire_beam(GF_PSI, dir, damroll(3 + ((plev - 1) / 4), (3+plev/15)));
			}
			else
			{
				fire_ball(GF_PSI, dir, damroll(3 + ((plev - 1) / 4), (3+plev/15)), 0);
			}
			return SUCCESS;
		}
		case SP_MINOR_DISPLACEMENT-1024:
		{
			if (plev < 25)
			{
				teleport_player(10);
			}
			else
			{
				if (!dimension_door(plev, 20)) (*use) = FALSE;
			}
			return SUCCESS;
		}
		case SP_MAJOR_DISPLACEMENT-1024:
		{
			if (plev > 29) banish_monsters(plev);
			teleport_player(plev * 5);
			return SUCCESS;
		}
		case SP_DOMINATION-1024:
		{
			if (plev < 30)
			{
				if (!dir) return POWER_ERROR_NO_SUCH_DIR;
				fire_ball(GF_DOMINATION, dir, plev, 0);
			}
			else
			{
				charm_monsters(plev * 2);
			}
			return SUCCESS;
		}
		case SP_PULVERISE-1024:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			fire_ball(GF_SOUND, dir, damroll(8+((plev-5)/4), 8),(plev > 20 ? (plev-20)/8 + 1 : 0));
			return SUCCESS;
		}
		case SP_CHARACTER_ARMOUR-1024:
		{
			add_flag(TIMED_SHIELD, plev);
			if (plev > 14)   add_flag(TIMED_OPPOSE_ACID, plev);
			if (plev > 19)   add_flag(TIMED_OPPOSE_FIRE, plev);
			if (plev > 24)   add_flag(TIMED_OPPOSE_COLD, plev);
			if (plev > 29)   add_flag(TIMED_OPPOSE_ELEC, plev);
			if (plev > 34)   add_flag(TIMED_OPPOSE_POIS, plev);
			return SUCCESS;
		}
		case SP_PSYCHOMETRY-1024:
		{
			if (plev < 40)
			{
				psychometry();
			}
			else
			{
				ident_spell();
			}
			return SUCCESS;
		}
		case SP_MIND_WAVE-1024:
		{
			msg_print("Mind-warping forces emanate from your brain!");
			if (plev < 25)
			{
				project(0, 2+plev/10, py, px,(plev*3)/2, GF_PSI, PROJECT_KILL);
			}
			else
			{
				(void)mindblast_monsters(plev * ((plev-5) / 10 + 1));
			}
			return SUCCESS;
		}
		case SP_ADRENALINE_CHANNELING-1024:
		{
			set_flag(TIMED_AFRAID, 0);
			set_flag(TIMED_STUN, 0);
			hp_player(plev);
			b = 10 + randint((plev*3)/2);
			if (plev < 35)
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
			return SUCCESS;
		}
		case SP_PSYCHIC_DRAIN-1024:
		{
			if (!dir) return POWER_ERROR_NO_SUCH_DIR;
			b = damroll(plev/2, 6);
			if (fire_ball(GF_PSI_DRAIN, dir, b,  0 + (plev-25)/10))
			p_ptr->energy -= randint(TURN_ENERGY*15/10);
			return SUCCESS;
		}
		case SP_TELEKINETIC_WAVE-1024:
		{
			msg_print("A wave of pure physical force radiates out from your body!");
			project(0, 3+plev/10, py, px,
            plev * (plev > 39 ? 4 : 3), GF_TELEKINESIS, PROJECT_KILL|PROJECT_ITEM|PROJECT_GRID);
			return SUCCESS;
		}

		default:
		{
			/* The object wasn't handled here... */
			return POWER_ERROR_NO_SUCH_POWER;
		}
	}
}

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
	bool ident, use, known;
	int power = get_power(o_ptr);

	/* plev is only used for random artefacts here, for which this is the
	 * correct formula.
	 */
	const int plev = MAX(1, skill_set[SKILL_DEVICE].value/2);

	/* "detect traps" marks the area it has been used in if the effect is
	 * known. This is always true if the object was previously known to have
	 * that effect, so check this.
	 */
	object_type j_ptr[1];
	object_info_known(j_ptr, o_ptr, 0);
	known = (power == get_power(j_ptr));

	switch (do_power(power, plev, dir, known, &use, &ident))
	{
		/* Get a direction, then try again. */
		case POWER_ERROR_NO_SUCH_DIR:
		{
			if (!get_aim_dir(&dir)) return FALSE;
			return use_object(o_ptr, dir);
		}
		/* Look for powers based on tval. */
		case POWER_ERROR_NO_SUCH_POWER:
		{
			known = (o_ptr->tval == j_ptr->tval);
			switch (do_power(-768+o_ptr->tval, plev, dir, known, &use, &ident))
			{
				case POWER_ERROR_NO_SUCH_DIR:
				{
					if (!get_aim_dir(&dir)) return FALSE;
					return use_object(o_ptr, dir);
				}
				/* Paranoia - nothing is known. */
				case POWER_ERROR_NO_SUCH_POWER:
				{
					bell("Unknown power: %d", power);
					return FALSE;
				}
			}
			break;
		}
	}

	/* Become aware of the object if it's now known. */
	if (ident && !object_aware_p(o_ptr))
	{
		object_aware(o_ptr);
	}

	return use;
}

/*
 * Use a power listed by index.
 */
bool use_known_power(int power, int plev)
{
	bool ident, use;
	errr err = do_power(power, plev, 0, TRUE, &use, &ident);
	if (err == POWER_ERROR_NO_SUCH_DIR)
	{
		int dir;
		if (!get_aim_dir(&dir)) return FALSE;
		err = do_power(power, plev, dir, TRUE, &use, &ident);
	}
	if (err == POWER_ERROR_NO_SUCH_POWER)
	{
		bell("Unknown power: %d", power);
		return FALSE;
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

	/* Gain exp */
	skill_exp(SKILL_DEVICE);

	/* Use the object (which always takes a charge). */
	use_object(o_ptr, dir);

	/* Recalculate/redraw stuff (later) */
	update_object(o_ptr, 0);

	/* Mark it as tried */
	object_tried(o_ptr);

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

	/* Gain exp */
	skill_exp(SKILL_DEVICE);

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
	}
}
