#define POWERS_C
/* File: powers.c */

/* Purpose: Specific actions of objects */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

/*
 * This file contains code to perform an action appropriate to a specific
 * object, spell, etc.. Code which is specific to a particular access method
 * should go elsewhere, although there are some fairly generic access functions
 * here.
 *
 * N.B. The effects here have not been tested more widely than they are used
 * here. Care must therefore be taken when calling a power via a new
 * mechanism to ensure that it uses I/O, etc., as the new caller expects.
 */

#include "angband.h"

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
 * Use a power from an object of some kind.
 * Return TRUE if the power exists, FALSE otherwise.
 *
 * power is the index of the power to use.
 * dir is the direction the power is to be used in, or 0 if unspecified.
 * *ident is TRUE if the player should know that this object has this power.
 *   It is set if the power has observable effects.
 * *use is set to TRUE if the power was used, FALSE if it was aborted, etc.
 *   Its initial valuo is ignored.
 */
bool use_object_power(const int power, int dir, bool *ident, bool *use)
{
	/* plev is only used for random artefacts here, for which this is the
	 * correct formula.
	 */
	const int plev = MAX(1, skill_set[SKILL_DEVICE].value/2);

	switch (do_power(power, plev, dir, *ident, use, ident))
	{
		/* Get a direction, then try again. */
		case POWER_ERROR_NO_SUCH_DIR:
		{
			/* Finish if no direction is given. */
			if (!get_aim_dir(&dir))
			{
				(*use) = FALSE;
				return TRUE;
			}

			/* Otherwise, try again. */
			return use_object_power(power, dir, ident, use);
		}
		/* Look for powers based on tval. */
		case POWER_ERROR_NO_SUCH_POWER:
		{
			return FALSE;
		}
		default:
		{
			return TRUE;
		}
	}
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
