#define SPELLS2_C
/* File: spells2.c */

/* Purpose: Spell code (part 2) */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"

/* Chance of using syllables to form the name instead of the "template" files */
#define TABLE_NAME      45
#define A_CURSED        13
#define WEIRD_LUCK      12
#define BIAS_LUCK       20
/* Bias luck needs to be higher than weird luck, since it is usually tested
several times... */
#define ACTIVATION_CHANCE 3
/* Time between an item becoming uncursed, and cursing itself again. */
#define CURSE_TIMEOUT	30

static bool detect_monsters_string(cptr Match);

/*
 * Teleport the player to a nearby square of the player's choice.
 * Returns FALSE if aborted, TRUE otherwise.
 */
bool dimension_door(int plev, int fail_dis)
{
	int ii,ij;

	msg_print("You open a dimensional gate. Choose a destination.");
	if (!tgt_pt(&ii,&ij)) return FALSE;
	p_ptr->energy -= 6*TURN_ENERGY/10 - plev*TURN_ENERGY/100;

	/* Bad target or bad luck. */
	if (!cave_empty_bold(ij,ii) || (cave[ij][ii].info & CAVE_ICKY) ||
		(cave[ij][ii].feat == FEAT_WATER) ||
		(distance(ij,ii,py,px) > plev + 2) || (!rand_int(plev * plev / 2)))
	{
		msg_print("You fail to exit the astral plane correctly!");
		p_ptr->energy -= TURN_ENERGY;
		teleport_player(fail_dis);
	}
	else /* Success */
	{
		teleport_player_to(ij,ii);
	}
	return TRUE;
}

/*
 * Increase players hit points, notice effects
 */
bool hp_player(int num)
{
	/* Healing needed */
	if (p_ptr->chp < p_ptr->mhp)
	{
		/* Gain hitpoints */
		p_ptr->chp += num;

		/* Enforce maximum */
		if (p_ptr->chp >= p_ptr->mhp)
		{
			p_ptr->chp = p_ptr->mhp;
			p_ptr->chp_frac = 0;
		}

		/* Redraw */
		p_ptr->redraw |= (PR_HP);

		/* Window stuff */
		p_ptr->window |= (PW_PLAYER);

		/* Heal 0-4 */
		if (num < 5)
		{
			msg_print("You feel a little better.");
		}
		
		/* Heal 5-14 */
		else if (num < 15)
		{
			msg_print("You feel better.");
		}

		/* Heal 15-34 */                
		else if (num < 35)
		{
			msg_print("You feel much better.");
		}

		/* Heal 35+ */
		else
		{
			msg_print("You feel very good.");
		}

		/* Notice */
		return (TRUE);
	}

	/* Ignore */
	return (FALSE);
}



/*
 * Leave a "glyph of warding" which prevents monster movement
 */
void warding_glyph(void)
{
	/* XXX XXX XXX */
	if (!cave_clean_bold(py, px))
	{
		msg_print("The object resists the spell.");
		return;
	}

	/* Create a glyph */
	cave_set_feat(py, px, FEAT_GLYPH);
}

void explosive_rune(void)
{
	/* XXX XXX XXX */
	if (!cave_clean_bold(py, px))
	{
		msg_print("The object resists the spell.");
		return;
	}

	/* Create a glyph */
    cave_set_feat(py, px, FEAT_MINOR_GLYPH);
}



/*
 * Array of stat "descriptions"
 */
static cptr desc_stat_pos[] =
{
	"strong",
	"smart",
	"wise",
	"dextrous",
	"healthy",
	"cute"
};


/*
 * Array of stat "descriptions"
 */
static cptr desc_stat_neg[] =
{
	"weak",
	"stupid",
	"naive",
	"clumsy",
	"sickly",
	"ugly"
};


/*
 * Lose a "point"
 */
bool do_dec_stat(int stat)
{
	bool sust = FALSE;

	/* Access the "sustain" */
	switch (stat)
	{
		case A_STR: if (p_ptr->sustain_str) sust = TRUE; break;
		case A_INT: if (p_ptr->sustain_int) sust = TRUE; break;
		case A_WIS: if (p_ptr->sustain_wis) sust = TRUE; break;
		case A_DEX: if (p_ptr->sustain_dex) sust = TRUE; break;
		case A_CON: if (p_ptr->sustain_con) sust = TRUE; break;
		case A_CHR: if (p_ptr->sustain_chr) sust = TRUE; break;
	}

	/* Sustain */
	if (sust)
	{
		/* Message */
		msg_format("You feel %s for a moment, but the feeling passes.",
			   desc_stat_neg[stat]);

		/* Notice effect */
		return (TRUE);
	}

	/* Attempt to reduce the stat */
	if (dec_stat(stat, 10, FALSE))
	{
		/* Message */
		msg_format("You feel very %s.", desc_stat_neg[stat]);

		/* Notice effect */
		return (TRUE);
	}

	/* Nothing obvious */
	return (FALSE);
}


/*
 * Restore lost "points" in a stat
 */
bool do_res_stat(int stat)
{
	/* Attempt to increase */
	if (res_stat(stat))
	{
		/* Message */
		msg_format("You feel less %s.", desc_stat_neg[stat]);

		/* Notice */
		return (TRUE);
	}

	/* Nothing obvious */
	return (FALSE);
}


/*
 * Gain a "point" in a stat
 */
bool do_inc_stat(int stat)
{
	bool res;

	/* Restore strength */
	res = res_stat(stat);

	/* Attempt to increase */
	if (inc_stat(stat))
	{
		/* Message */
		msg_format("Wow!  You feel very %s!", desc_stat_pos[stat]);

		/* Notice */
		return (TRUE);
	}

	/* Restoration worked */
	if (res)
	{
		/* Message */
		msg_format("You feel less %s.", desc_stat_neg[stat]);

		/* Notice */
		return (TRUE);
	}

	/* Nothing obvious */
	return (FALSE);
}



/*
 * Identify everything being carried.
 * Done by a potion of "self knowledge".
 */
void identify_pack(void)
{
	int                 i;

	/* Simply identify and know every item */
	for (i = 0; i < INVEN_TOTAL; i++)
	{
		object_type *o_ptr = &inventory[i];

		/* Skip non-objects */
		if (!o_ptr->k_idx) continue;

		/* Aware and Known */
		object_aware(o_ptr);
		object_known(o_ptr);
	}
}






/*
 * Used by the "enchant" function (chance of failure)
 * (modified for Zangband, we need better stuff there...) -- TY
 */
static int enchant_table[16] =
{
	0, 10,  50, 100, 200,
    300, 400, 500, 650, 800,
    950, 987, 993, 995, 998,
	1000
};


/*
 * Removes curses from items in inventory
 *
 * Note that Items which are "Perma-Cursed" (The One Ring,
 * The Crown of Morgoth) can NEVER be uncursed.
 *
 * Note that if "all" is FALSE, then Items which are
 * "Heavy-Cursed" (Mormegil, Calris, and Weapons of Morgul)
 * will not be uncursed.
 */
static int remove_curse_aux(int all)
{
	int             i, cnt = 0;

	/* Attempt to uncurse items being worn */
	for (i = INVEN_WIELD; i < INVEN_TOTAL; i++)
	{
		u32b f1, f2, f3;

		object_type *o_ptr = &inventory[i];

		/* Skip non-objects */
		if (!o_ptr->k_idx) continue;

		/* Uncursed already */
		if (!cursed_p(o_ptr)) continue;

		/* Extract the flags */
		object_flags(o_ptr, &f1, &f2, &f3);

		/* Heavily Cursed Items need a special spell */
		if (!all && (f3 & (TR3_HEAVY_CURSE))) continue;

		/* Perma-Cursed Items can NEVER be uncursed */
		if (f3 & (TR3_PERMA_CURSE)) continue;

		/* Hack - as "cursed" items can become average, good or messageless
		when they are uncursed, don't leave value information behind */

		if (streq(find_feeling(o_ptr), "cursed"))
			o_ptr->ident &= ~(IDENT_SENSE_VALUE | IDENT_SENSE_HEAVY);

		/* Uncurse it */
		o_ptr->ident &= ~(IDENT_CURSED);

		/* Hack -- Assume felt */
		o_ptr->ident |= (IDENT_SENSE_CURSED);

        if (o_ptr->flags3 & (TR3_CURSED))
            o_ptr->flags3 &= ~(TR3_CURSED);

        if (o_ptr->flags3 & (TR3_HEAVY_CURSE))
            o_ptr->flags3 &= ~(TR3_HEAVY_CURSE);

		/* You can't palm that ring of speed (-20) off quite so easily... */
		if (broken_p(o_ptr))
			o_ptr->ident |= (IDENT_SENSE);

		/* Recalculate the bonuses */
		p_ptr->update |= (PU_BONUS);

		/* Window stuff */
		p_ptr->window |= (PW_EQUIP);

		/* Set auto_curse timeout */
		curse_turn = turn + CURSE_TIMEOUT;

		/* Count the uncursings */
		cnt++;
	}

	/* Return "something uncursed" */
	return (cnt);
}


/*
 * Remove most curses
 */
bool remove_curse(void)
{
	return (remove_curse_aux(FALSE));
}

/*
 * Remove all curses
 */
bool remove_all_curse(void)
{
	return (remove_curse_aux(TRUE));
}



/*
 * Restores any drained experience
 */
bool restore_level(void)
{
	int i,did_it;
	
	/* Assume nothing happened */
	did_it = FALSE;

	for (i=0;i<MAX_SKILLS;i++)
	{
		/* Restore experience */
		if (skill_set[i].value < skill_set[i].max_value)
		{
			skill_set[i].value = skill_set[i].max_value;
			did_it = TRUE;
		}
	}

	if (did_it)
	{
		/* Hack - prevent the update of arbitrary things (I haven't checked
		 * that this is necessary, but it may be). */
		u32b hack_update = p_ptr->update;
		
		/* Message */
		msg_print("You feel your life energies returning.");
		/* Did something */

		/* Recalculate hit points, mana and spells, but nothing else. */
		p_ptr->update = PU_HP | PU_MANA | PU_SPELLS;
		update_stuff();

		p_ptr->update = hack_update;

		return (TRUE);
	}

	/* No effect */
	return (FALSE);
}


/*
 * Turns an object into gold, gain some of its value in a shop
 */
bool alchemy(void)
{
	int                     item, amt = 1;
	int                     old_number;
    long        price;

	bool            force = FALSE;

	object_type             *o_ptr;

	C_TNEW(o_name, ONAME_MAX, char);

	char            out_val[160];


	/* Hack -- force destruction */
	if (command_arg > 0) force = TRUE;

	/* Restrict the choices */
	item_tester_hook = item_tester_hook_destroy;

	/* Get an item (from equip or inven or floor) */
    if (!get_item(&item, "Turn which item to gold? ", TRUE, TRUE, TRUE))
	{
		if (item == -2) msg_print("You have nothing to turn to gold.");
		TFREE(o_name);
		return FALSE;
	}

	/* Get the item (in the pack) */
	if (item >= 0)
	{
		o_ptr = &inventory[item];
	}

	/* Get the item (on the floor) */
	else
	{
		o_ptr = &o_list[0 - item];
	}


	/* See how many items */
	if (o_ptr->number > 1)
	{
		/* Get a quantity */
		amt = get_quantity(NULL, o_ptr->number,TRUE);

		/* Allow user abort */
		if (amt <= 0)
		{
			TFREE(o_name);
			return FALSE;
		}
	}


	/* Describe the object */
	old_number = o_ptr->number;
	o_ptr->number = amt;
	object_desc(o_name, o_ptr, TRUE, 3);
	o_ptr->number = old_number;

	/* Verify unless quantity given */
	if (!force)
	{
        if (!((auto_destroy) && (object_value(o_ptr)<1)))
        {
            /* Make a verification */
            sprintf(out_val, "Really turn %s to gold? ", o_name);
            if (!get_check(out_val))
			{
				TFREE(o_name);
				return FALSE;
			}
        }
	}

    /* Artifacts cannot be destroyed */
    if (allart_p(o_ptr))
	{
		/* Message */
		msg_format("You fail to turn %s to gold!", o_name);

		/* We have "felt" it (again) */
		o_ptr->ident |= (IDENT_SENSE_VALUE);

		/* Combine the pack */
		p_ptr->notice |= (PN_COMBINE);

		/* Window stuff */
		p_ptr->window |= (PW_INVEN | PW_EQUIP);

		TFREE(o_name);

		/* Done */
		return FALSE;
	}

    price = object_value_real(o_ptr);

    if (price <= 0)
	/* Message */
	msg_format("You turn %s to fool's gold.", o_name);
    else
	{
	    price /= 3;

	if (amt > 1) price *= amt;

	    if (price > 30000) price = 30000;
	    msg_format("You turn %s to %ld coins worth of gold.", o_name, price);
	    p_ptr->au += price;

	    /* Redraw gold */
			p_ptr->redraw |= (PR_GOLD);

			/* Window stuff */
	    p_ptr->window |= (PW_PLAYER);

	}

	/* Eliminate the item (from the pack) */
	if (item >= 0)
	{
		inven_item_increase(item, -amt);
		inven_item_describe(item);
		inven_item_optimize(item);
	}

	/* Eliminate the item (from the floor) */
	else
	{
		floor_item_increase(0 - item, -amt);
		floor_item_describe(0 - item);
		floor_item_optimize(0 - item);
	}

	TFREE(o_name);
    return TRUE;
}




/*
 * self-knowledge... idea from nethack.  Useful for determining powers and
 * resistences of items.  It saves the screen, clears it, then starts listing
 * attributes, a screenful at a time.  (There are a LOT of attributes to
 * list.  It will probably take 2 or 3 screens for a powerful character whose
 * using several artifacts...) -CFT
 *
 * It is now a lot more efficient. -BEN-
 *
 * See also "identify_fully()".
 *
 * XXX XXX XXX Use the "show_file()" method, perhaps.
 */
void self_knowledge(void)
{
	int             i = 0, j, k;

	u32b    f1 = 0L, f2 = 0L, f3 = 0L;

	object_type     *o_ptr;

    char Dummy[80];

	cptr    info[128];

    int plev = skill_set[SKILL_RACIAL].value/2;

    strcpy (Dummy, "");

	/* Acquire item flags from equipment */
	for (k = INVEN_WIELD; k < INVEN_TOTAL; k++)
	{
		u32b t1, t2, t3;

		o_ptr = &inventory[k];

		/* Skip non-objects */
		if (!o_ptr->k_idx) continue;

		/* Extract the flags */
		object_flags(o_ptr, &t1, &t2, &t3);

		/* Extract flags */
		f1 |= t1;
		f2 |= t2;
		f3 |= t3;
	}


    /* Racial powers... */
    switch (p_ptr->prace)
    {
	case RACE_NIBELUNG: case RACE_DWARF:
	{
	    info[i++] = "You can find traps, doors and stairs (cost 5).";
	}
    break;
	case RACE_HOBBIT:
	{
		info[i++] = "You can produce food (cost 10).";
	}
	break;
	case RACE_GNOME:
	{
		sprintf(Dummy, "You can teleport, range %d (cost %d).",
			(1+plev), (5 + (plev/5)));
		info[i++] = Dummy;
	}
	break;
	case RACE_HALF_ORC:
	{
	    info[i++] = "You can remove fear (cost 5).";
	}
	break;
	case RACE_HALF_TROLL:
	{
	    info[i++] = "You enter berserk fury (cost 12).";
	}
	break;
	case RACE_GREAT:
	{
	    info[i++] = "You can dream travel (cost 50).";
	    info[i++] = "You can dream a better self (cost 75).";
	}
	break;
	case RACE_BARBARIAN:
	{
	    info[i++] = "You can enter berserk fury (cost 10).";
	}
	break;
	case RACE_HALF_OGRE:
	{
	    info[i++] = "You can set an Explosive Rune (cost 35).";
	}
	break;
	case RACE_HALF_GIANT:
	{
	    info[i++] = "You can break stone walls (cost 10).";
	}
	break;
	case RACE_HALF_TITAN:
	{
	    info[i++] = "You can probe monsters (cost 20).";
	}
	break;
	case RACE_CYCLOPS:
	{
		sprintf(Dummy, "You can throw a boulder, dam. %d (cost 15).",
		    3 * plev);
		info[i++] = Dummy;
	}
	break;
	case RACE_YEEK:
	{
	    info[i++] = "You can make a terrifying scream (cost 15).";
	}
	break;
	case RACE_KLACKON:
	{
		sprintf(Dummy, "You can spit acid, dam. %d (cost 9).",
		    plev);
		info[i++] = Dummy;
	}
	break;
	case RACE_KOBOLD:
	{
		sprintf(Dummy,
		    "You can throw a dart of poison, dam. %d (cost 8).",
		    plev);
		info[i++] = Dummy;
	}
	break;
	case RACE_DARK_ELF:
	{
		sprintf(Dummy, "You can cast a Magic Missile, dam %d (cost 2).",
		( 3 + ((plev-1) / 5) ) );
		info[i++] = Dummy;
	}
	break;
	case RACE_DRACONIAN:
	{
	    sprintf(Dummy, "You can breathe, dam. %d (cost %d).", 2 * plev,
		plev);
	    info[i++] = Dummy;
	}
	break;
	case RACE_MIND_FLAYER:
	{
        sprintf(Dummy, "You can mind blast your enemies, dam %d (cost 12).",
             plev);
        info[i++] = Dummy;
	}
	break;
	case RACE_IMP:
	{
	    if (plev > 29)
	    {
		sprintf(Dummy, "You can cast a Fire Ball, dam. %d (cost 15).",
		    plev);
		info[i++] = Dummy;
	    }
	    else if (plev > 8)
	    {
		sprintf(Dummy, "You can cast a Fire Bolt, dam. %d (cost 15).",
		    plev);
		info[i++] = Dummy;
	    }
	}
	break;
	case RACE_GOLEM:
	{
	    info[i++] = "You can turn your skin to stone, dur d20+30 (cost 15).";
	}
	break;
	case RACE_ZOMBIE: case RACE_SKELETON:
	{
	    info[i++] = "You can restore lost life forces (cost 30).";
	}
	break;
    case RACE_VAMPIRE:
    {
            sprintf(Dummy, "You can steal life from a foe, dam. %d-%d (cost %d).",
                plev+MAX(1, plev/10), plev+plev*MAX(1, plev/10), 1+(plev/3));
            info[i++] = Dummy;
    }
    break;
    case RACE_SPECTRE:
    {
            info[i++] = "You can wail to terrify your enemies (cost 3).";
    }
    break;
    case RACE_BROO:
    {
            info[i++] = "You can growl to terrify your enemies (cost 3).";
    }
    break;
    case RACE_SPRITE:
    {
            info[i++] = "You can throw magic dust which induces sleep (cost 12).";
    }
    break;
	default:
	break;
    }

    /* Handle chaos features */

	if (p_ptr->muta1)
	{
		if (p_ptr->muta1 & MUT1_SPIT_ACID)
		{
			info[i++] = "You can spit acid (dam lvl).";
		}
		if (p_ptr->muta1 & MUT1_BR_FIRE)
		{
			info[i++] = "You can breathe fire (dam lvl * 2).";
		}
		if (p_ptr->muta1 & MUT1_HYPN_GAZE)
		{
			info[i++] = "Your gaze is hypnotic.";
		}
		if (p_ptr->muta1 & MUT1_TELEKINES)
		{
			info[i++] = "You are telekinetic.";
		}
		if (p_ptr->muta1 & MUT1_VTELEPORT)
		{
			info[i++] = "You can teleport at will.";
		}
		if (p_ptr->muta1 & MUT1_MIND_BLST)
		{
			info[i++] = "You can Mind Blast your enemies (3 to 12d3 dam).";
		}
		if (p_ptr->muta1 & MUT1_RADIATION)
		{
			info[i++] = "You can emit hard radiation at will (dam lvl * 2).";
		}
		if (p_ptr->muta1 & MUT1_VAMPIRISM)
		{
			info[i++] = "You can drain life from a foe like a vampire (dam lvl * 2).";
		}
		if (p_ptr->muta1 & MUT1_SMELL_MET)
		{
			info[i++] = "You can smell nearby precious metal.";
		}
		if (p_ptr->muta1 & MUT1_SMELL_MON)
		{
			info[i++] = "You can smell nearby monsters.";
		}
		if (p_ptr->muta1 & MUT1_BLINK)
		{
			info[i++] = "You can teleport yourself short distances.";
		}
		if (p_ptr->muta1 & MUT1_EAT_ROCK)
		{
			info[i++] = "You can consume solid rock.";
		}
		if (p_ptr->muta1 & MUT1_SWAP_POS)
		{
			info[i++] = "You can switch locations with another being.";
		}
		if (p_ptr->muta1 & MUT1_SHRIEK)
		{
			info[i++] = "You can emit a horrible shriek (dam 4 * lvl).";
		}
		if (p_ptr->muta1 & MUT1_ILLUMINE)
		{
			info[i++] = "You can emit bright light.";
		}
		if (p_ptr->muta1 & MUT1_DET_CURSE)
		{
			info[i++] = "You can feel the danger of evil magic.";
		}
		if (p_ptr->muta1 & MUT1_BERSERK)
		{
			info[i++] = "You can drive yourself into a berserk frenzy.";
		}
		if (p_ptr->muta1 & MUT1_POLYMORPH)
		{
			info[i++] = "You can polymorph yourself at will.";
		}
		if (p_ptr->muta1 & MUT1_MIDAS_TCH)
		{
			info[i++] = "You can turn ordinary items to gold.";
		}
		if (p_ptr->muta1 & MUT1_GROW_MOLD)
		{
			info[i++] = "You can cause mold to grow near you.";
		}
		if (p_ptr->muta1 & MUT1_RESIST)
		{
			info[i++] = "You can harden yourself to the ravages of the elements.";
		}
		if (p_ptr->muta1 & MUT1_EARTHQUAKE)
		{
			info[i++] = "You can bring down the dungeon around your ears.";
		}
		if (p_ptr->muta1 & MUT1_EAT_MAGIC)
		{
			info[i++] = "You can consume magic energy for your own use.";
		}
		if (p_ptr->muta1 & MUT1_WEIGH_MAG)
		{
			info[i++] = "You can feel the strength of the magics affecting you.";
		}
		if (p_ptr->muta1 & MUT1_STERILITY)
		{
			info[i++] = "You can cause mass impotence.";
		}
		if (p_ptr->muta1 & MUT1_PANIC_HIT)
		{
			info[i++] = "You can run for your life after hitting something.";
		}
		if (p_ptr->muta1 & MUT1_DAZZLE)
		{
			info[i++] = "You can emit confusing, blinding radiation.";
		}
		if (p_ptr->muta1 & MUT1_EYE_BEAM)
		{
			info[i++] = "Your eyes can fire beams of light (dam 2 * lvl).";
		}
		if (p_ptr->muta1 & MUT1_RECALL)
		{
			info[i++] = "You can travel between town and the depths.";
		}
		if (p_ptr->muta1 & MUT1_BANISH)
		{
			info[i++] = "You can send evil creatures directly to Hell.";
		}
		if (p_ptr->muta1 & MUT1_COLD_TOUCH)
		{
			info[i++] = "You can freeze things with a touch (dam 3 * lvl).";
		}
		if (p_ptr->muta1 & MUT1_LAUNCHER)
		{
			info[i++] = "You can hurl objects with great force.";
		}
	}
    if (p_ptr->muta2)
    {
		if (p_ptr->muta2 & MUT2_BERS_RAGE)
		{
			info[i++] = "You are subject to berserker fits.";
		}
		if (p_ptr->muta2 & MUT2_COWARDICE)
		{
			info[i++] = "You are subject to cowardice.";
		}
		if (p_ptr->muta2 & MUT2_RTELEPORT)
		{
			info[i++] = "You are teleporting randomly.";
		}
		if (p_ptr->muta2 & MUT2_ALCOHOL)
		{
			info[i++] = "Your body produces alcohol.";
		}
		if (p_ptr->muta2 & MUT2_HALLU)
		{
			info[i++] = "You have a hallucinatory insanity.";
		}
		if (p_ptr->muta2 & MUT2_FLATULENT)
		{
			info[i++] = "You are subject to uncontrollable flatulence.";
		}
		if (p_ptr->muta2 & MUT2_PROD_MANA)
		{
			info[i++] = "You are producing magical energy uncontrollably.";
		}
		if (p_ptr->muta2 & MUT2_ATT_DEMON)
		{
			info[i++] = "You attract demons.";
		}
		if (p_ptr->muta2 & MUT2_SCOR_TAIL)
		{
			info[i++] = "You have a scorpion tail (poison, 3d7).";
		}
		if (p_ptr->muta2 & MUT2_HORNS)
		{
			info[i++] = "You have horns (dam. 2d6).";
		}
		if (p_ptr->muta2 & MUT2_BEAK)
		{
			info[i++] = "You have a beak (dam. 2d4).";
		}
		if (p_ptr->muta2 & MUT2_SPEED_FLUX)
		{
			info[i++] = "You move faster or slower randomly.";
		}
		if (p_ptr->muta2 & MUT2_BANISH_ALL)
		{
			info[i++] = "You sometimes cause nearby creatures to vanish.";
		}
		if (p_ptr->muta2 & MUT2_EAT_LIGHT)
		{
			info[i++] = "You sometimes feed off of the light around you.";
		}
		if (p_ptr->muta2 & MUT2_TRUNK)
		{
			info[i++] = "You have an elephantine trunk (dam 1d4).";
		}
		if (p_ptr->muta2 & MUT2_ATT_ANIMAL)
		{
			info[i++] = "You attract animals.";
		}
		if (p_ptr->muta2 & MUT2_TENTACLES)
		{
			info[i++] = "You have evil looking tentacles (dam 2d5).";
		}
		if (p_ptr->muta2 & MUT2_RAW_CHAOS)
		{
			info[i++] = "You occasionally are surrounded with raw chaos.";
		}
		if (p_ptr->muta2 & MUT2_NORMALITY)
		{
			info[i++] = "You may be chaotic, but you're recovering.";
		}
		if (p_ptr->muta2 & MUT2_WRAITH)
		{
			info[i++] = "You fade in and out of physical reality.";
		}
		if (p_ptr->muta2 & MUT2_POLY_WOUND)
		{
			info[i++] = "Your health is subject to chaotic forces.";
		}
		if (p_ptr->muta2 & MUT2_WASTING)
		{
			info[i++] = "You have a horrible wasting disease.";
		}
		if (p_ptr->muta2 & MUT2_ATT_DRAGON)
		{
			info[i++] = "You attract dragons.";
		}
		if (p_ptr->muta2 & MUT2_WEIRD_MIND)
		{
			info[i++] = "Your mind randomly expands and contracts.";
		}
		if (p_ptr->muta2 & MUT2_NAUSEA)
		{
			info[i++] = "You have a seriously upset stomach.";
		}
		if (p_ptr->muta2 & MUT2_CHAOS_GIFT)
		{
			info[i++] = "Chaos deities give you gifts.";
		}
		if (p_ptr->muta2 & MUT2_WALK_SHAD)
		{
			info[i++] = "You occasionally stumble into other shadows.";
		}
		if (p_ptr->muta2 & MUT2_WARNING)
		{
			info[i++] = "You receive warnings about your foes.";
		}
		if (p_ptr->muta2 & MUT2_INVULN)
		{
			info[i++] = "You occasionally feel invincible.";
		}
		if (p_ptr->muta2 & MUT2_SP_TO_HP)
		{
			info[i++] = "Your blood sometimes rushes to your muscles.";
		}
		if (p_ptr->muta2 & MUT2_HP_TO_SP)
		{
			info[i++] = "Your blood sometimes rushes to your head.";
		}
		if (p_ptr->muta2 & MUT2_DISARM)
		{
			info[i++] = "You occasionally stumble and drop things.";
		}
      }

	if (p_ptr->muta3)
	{
		if (p_ptr->muta3 & MUT3_HYPER_STR)
		{
			info[i++] = "You are superhumanly strong (+4 STR).";
		}
		if (p_ptr->muta3 & MUT3_PUNY)
		{
			info[i++] = "You are puny (-4 STR).";
		}
		if (p_ptr->muta3 & MUT3_HYPER_INT)
		{
			info[i++] = "Your brain is a living computer (+4 INT/WIS).";
		}
		if (p_ptr->muta3 & MUT3_MORONIC)
		{
			info[i++] = "You are moronic (-4 INT/WIS).";
		}
		if (p_ptr->muta3 & MUT3_RESILIENT)
		{
			info[i++] = "You are very resilient (+4 CON).";
		}
		if (p_ptr->muta3 & MUT3_XTRA_FAT)
		{
			info[i++] = "You are extremely fat (+2 CON, -2 speed).";
		}
		if (p_ptr->muta3 & MUT3_ALBINO)
		{
			info[i++] = "You are albino (-4 CON).";
		}
		if (p_ptr->muta3 & MUT3_FLESH_ROT)
		{
			info[i++] = "Your flesh is rotting (-2 CON, -1 CHR).";
		}
		if (p_ptr->muta3 & MUT3_SILLY_VOI)
		{
			info[i++] = "Your voice is a silly squeak (-4 CHR).";
		}
		if (p_ptr->muta3 & MUT3_BLANK_FAC)
		{
			info[i++] = "Your face is featureless (-1 CHR).";
		}
		if (p_ptr->muta3 & MUT3_ILL_NORM)
		{
			info[i++] = "Your appearance is masked with illusion.";
		}
		if (p_ptr->muta3 & MUT3_XTRA_EYES)
		{
			info[i++] = "You have an extra pair of eyes (+15 search).";
		}
		if (p_ptr->muta3 & MUT3_MAGIC_RES)
		{
			info[i++] = "You are resistant to magic.";
		}
		if (p_ptr->muta3 & MUT3_XTRA_NOIS)
		{
			info[i++] = "You make a lot of strange noise (-3 stealth).";
		}
		if (p_ptr->muta3 & MUT3_INFRAVIS)
		{
			info[i++] = "You have remarkable infravision (+3).";
		}
		if (p_ptr->muta3 & MUT3_XTRA_LEGS)
		{
			info[i++] = "You have an extra pair of legs (+3 speed).";
		}
		if (p_ptr->muta3 & MUT3_SHORT_LEG)
		{
			info[i++] = "Your legs are short stubs (-3 speed).";
		}
		if (p_ptr->muta3 & MUT3_ELEC_TOUC)
		{
			info[i++] = "Electricity is running through your veins.";
		}
		if (p_ptr->muta3 & MUT3_FIRE_BODY)
		{
#if 0
			/* Unnecessary, actually... */
			info[i++] = "Your body is enveloped in flames.";
#endif
		}
		if (p_ptr->muta3 & MUT3_WART_SKIN)
		{
			info[i++] = "Your skin is covered with warts (-2 CHR, +5 AC).";
		}
		if (p_ptr->muta3 & MUT3_SCALES)
		{
			info[i++] = "Your skin has turned into scales (-1 CHR, +10 AC).";
		}
		if (p_ptr->muta3 & MUT3_IRON_SKIN)
		{
			info[i++] = "Your skin is made of steel (-1 DEX, +25 AC).";
		}
		if (p_ptr->muta3 & MUT3_WINGS)
		{
			info[i++] = "You have wings.";
		}
		if (p_ptr->muta3 & MUT3_FEARLESS)
		{
			/* Unnecessary */
		}
		if (p_ptr->muta3 & MUT3_REGEN)
		{
			/* Unnecessary */
		}
		if (p_ptr->muta3 & MUT3_ESP)
		{
			/* Unnecessary */
		}
		if (p_ptr->muta3 & MUT3_LIMBER)
		{
			info[i++] = "Your body is very limber (+3 DEX).";
		}
		if (p_ptr->muta3 & MUT3_ARTHRITIS)
		{
			info[i++] = "Your joints ache constantly (-3 DEX).";
		}
		if (p_ptr->muta3 & MUT3_RES_TIME)
		{
			info[i++] = "You are protected from the ravages of time.";
		}
		if (p_ptr->muta3 & MUT3_VULN_ELEM)
		{
			info[i++] = "You are susceptible to damage from the elements.";
		}
		if (p_ptr->muta3 & MUT3_MOTION)
		{
			info[i++] = "Your movements are precise and forceful (+1 STL).";
		}
		if (p_ptr->muta3 & MUT3_SUS_STATS)
		{
			/* Unnecessary */
		}
	}



	if (p_ptr->blind)
	{
		info[i++] = "You cannot see.";
	}
	if (p_ptr->confused)
	{
		info[i++] = "You are confused.";
	}
	if (p_ptr->afraid)
	{
		info[i++] = "You are terrified.";
	}
	if (p_ptr->cut)
	{
		info[i++] = "You are bleeding.";
	}
	if (p_ptr->stun)
	{
		info[i++] = "You are stunned.";
	}
	if (p_ptr->poisoned)
	{
		info[i++] = "You are poisoned.";
	}
	if (p_ptr->image)
	{
		info[i++] = "You are hallucinating.";
	}

	if (p_ptr->aggravate)
	{
		info[i++] = "You aggravate monsters.";
	}
	if (p_ptr->teleport)
	{
		info[i++] = "Your position is very uncertain.";
	}

	if (p_ptr->blessed)
	{
		info[i++] = "You feel rightous.";
	}
	if (p_ptr->hero)
	{
		info[i++] = "You feel heroic.";
	}
	if (p_ptr->shero)
	{
		info[i++] = "You are in a battle rage.";
	}
	if (p_ptr->protevil)
	{
		info[i++] = "You are protected from evil.";
	}
	if (p_ptr->shield)
	{
		info[i++] = "You are protected by a mystic shield.";
	}
	if (p_ptr->invuln)
	{
		info[i++] = "You are temporarily invulnerable.";
	}
    if (p_ptr->wraith_form)
    {
        info[i++] = "You are temporarily incorporeal.";
    }
	if (p_ptr->confusing)
	{
		info[i++] = "Your hands are glowing dull red.";
	}
	if (p_ptr->sneaking)
	{
		info[i++] = "You are sneaking around very carefully.";
	}
	if (p_ptr->new_spells)
	{
		info[i++] = "You can learn some spells.";
	}
	if (p_ptr->word_recall)
	{
		info[i++] = "You will soon be recalled.";
	}
	if (p_ptr->see_infra)
	{
		info[i++] = "Your eyes are sensitive to infrared light.";
	}
	if (p_ptr->see_inv)
	{
		info[i++] = "You can see invisible creatures.";
	}
	if (p_ptr->ffall)
	{
        info[i++] = "You can fly.";
	}
	if (p_ptr->free_act)
	{
		info[i++] = "You have free action.";
	}
	if (p_ptr->regenerate)
	{
		info[i++] = "You regenerate quickly.";
	}
	if (p_ptr->slow_digest)
	{
		info[i++] = "Your appetite is small.";
	}
	if (p_ptr->telepathy)
	{
		info[i++] = "You have ESP.";
	}
	if (p_ptr->hold_life)
	{
		info[i++] = "You have a firm hold on your life force.";
	}

    if (p_ptr->reflect)
    {
        info[i++] = "You reflect arrows and bolts.";
    }

    if (p_ptr->sh_fire)
    {
        info[i++] = "You are surrounded with a fiery aura.";
    }
    if (p_ptr->sh_elec)
    {
        info[i++] = "You are surrounded with electricity.";
    }

    if (p_ptr->anti_magic)
    {
        info[i++] = "You are surrounded by an anti-magic shell.";
    }
    if (p_ptr->anti_tele)
    {
        info[i++] = "You cannot teleport.";
    }
	if (p_ptr->lite)
	{
		info[i++] = "You are carrying a permanent light.";
	}

	if (p_ptr->immune_acid)
	{
		info[i++] = "You are completely immune to acid.";
	}
	else if ((p_ptr->resist_acid) && (p_ptr->oppose_acid))
	{
		info[i++] = "You resist acid exceptionally well.";
	}
	else if ((p_ptr->resist_acid) || (p_ptr->oppose_acid))
	{
		info[i++] = "You are resistant to acid.";
	}

	if (p_ptr->immune_elec)
	{
		info[i++] = "You are completely immune to lightning.";
	}
	else if ((p_ptr->resist_elec) && (p_ptr->oppose_elec))
	{
		info[i++] = "You resist lightning exceptionally well.";
	}
	else if ((p_ptr->resist_elec) || (p_ptr->oppose_elec))
	{
		info[i++] = "You are resistant to lightning.";
	}

	if (p_ptr->immune_fire)
	{
		info[i++] = "You are completely immune to fire.";
	}
	else if ((p_ptr->resist_fire) && (p_ptr->oppose_fire))
	{
		info[i++] = "You resist fire exceptionally well.";
	}
	else if ((p_ptr->resist_fire) || (p_ptr->oppose_fire))
	{
		info[i++] = "You are resistant to fire.";
	}

	if (p_ptr->immune_cold)
	{
		info[i++] = "You are completely immune to cold.";
	}
	else if ((p_ptr->resist_cold) && (p_ptr->oppose_cold))
	{
		info[i++] = "You resist cold exceptionally well.";
	}
	else if ((p_ptr->resist_cold) || (p_ptr->oppose_cold))
	{
		info[i++] = "You are resistant to cold.";
	}

	if ((p_ptr->resist_pois) && (p_ptr->oppose_pois))
	{
		info[i++] = "You resist poison exceptionally well.";
	}
	else if ((p_ptr->resist_pois) || (p_ptr->oppose_pois))
	{
		info[i++] = "You are resistant to poison.";
	}

	if (p_ptr->resist_lite)
	{
		info[i++] = "You are resistant to bright light.";
	}
	if (p_ptr->resist_dark)
	{
		info[i++] = "You are resistant to darkness.";
	}
	if (p_ptr->resist_conf)
	{
		info[i++] = "You are resistant to confusion.";
	}
	if (p_ptr->resist_sound)
	{
		info[i++] = "You are resistant to sonic attacks.";
	}
	if (p_ptr->resist_disen)
	{
		info[i++] = "You are resistant to disenchantment.";
	}
	if (p_ptr->resist_chaos)
	{
		info[i++] = "You are resistant to chaos.";
	}
	if (p_ptr->resist_shard)
	{
		info[i++] = "You are resistant to blasts of shards.";
	}
	if (p_ptr->resist_nexus)
	{
		info[i++] = "You are resistant to nexus attacks.";
	}
	if (p_ptr->resist_neth)
	{
		info[i++] = "You are resistant to nether forces.";
	}
	if (p_ptr->resist_fear)
	{
		info[i++] = "You are completely fearless.";
	}
	if (p_ptr->resist_blind)
	{
		info[i++] = "Your eyes are resistant to blindness.";
	}


	if (p_ptr->sustain_str)
	{
		info[i++] = "Your strength is sustained.";
	}
	if (p_ptr->sustain_int)
	{
		info[i++] = "Your intelligence is sustained.";
	}
	if (p_ptr->sustain_wis)
	{
		info[i++] = "Your wisdom is sustained.";
	}
	if (p_ptr->sustain_con)
	{
		info[i++] = "Your constitution is sustained.";
	}
	if (p_ptr->sustain_dex)
	{
		info[i++] = "Your dexterity is sustained.";
	}
	if (p_ptr->sustain_chr)
	{
		info[i++] = "Your charisma is sustained.";
	}

	if (f1 & (TR1_STR))
	{
		info[i++] = "Your strength is affected by your equipment.";
	}
	if (f1 & (TR1_INT))
	{
		info[i++] = "Your intelligence is affected by your equipment.";
	}
	if (f1 & (TR1_WIS))
	{
		info[i++] = "Your wisdom is affected by your equipment.";
	}
	if (f1 & (TR1_DEX))
	{
		info[i++] = "Your dexterity is affected by your equipment.";
	}
	if (f1 & (TR1_CON))
	{
		info[i++] = "Your constitution is affected by your equipment.";
	}
	if (f1 & (TR1_CHR))
	{
		info[i++] = "Your charisma is affected by your equipment.";
	}

	if (f1 & (TR1_STEALTH))
	{
		info[i++] = "Your stealth is affected by your equipment.";
	}
	if (f1 & (TR1_SEARCH))
	{
		info[i++] = "Your searching ability is affected by your equipment.";
	}
	if (f1 & (TR1_INFRA))
	{
		info[i++] = "Your infravision is affected by your equipment.";
	}
	if (f1 & (TR1_TUNNEL))
	{
		info[i++] = "Your digging ability is affected by your equipment.";
	}
	if (f1 & (TR1_SPEED))
	{
		info[i++] = "Your speed is affected by your equipment.";
	}
	if (f1 & (TR1_BLOWS))
	{
		info[i++] = "Your attack speed is affected by your equipment.";
	}


	/* Access the current weapon */
	o_ptr = &inventory[INVEN_WIELD];

	/* Analyze the weapon */
	if (o_ptr->k_idx)
	{
		/* Indicate Blessing */
		if (f3 & (TR3_BLESSED))
		{
			info[i++] = "Your weapon has been blessed by the gods.";
		}

	if (f1 & (TR1_CHAOTIC))
		{
	    info[i++] = "Your weapon is branded with the Yellow Sign.";
	}
	/* Hack */
		if (f1 & (TR1_IMPACT))
		{
			info[i++] = "The impact of your weapon can cause earthquakes.";
       }

	if (f1 & (TR1_VORPAL))
		{
	    info[i++] = "Your weapon is very sharp.";

	}

	if (f1 & (TR1_VAMPIRIC))
		{
	    info[i++] = "Your weapon drains life from your foes.";
	}

		/* Special "Attack Bonuses" */
		if (f1 & (TR1_BRAND_ACID))
		{
			info[i++] = "Your weapon melts your foes.";
		}
		if (f1 & (TR1_BRAND_ELEC))
		{
			info[i++] = "Your weapon shocks your foes.";
		}
		if (f1 & (TR1_BRAND_FIRE))
		{
			info[i++] = "Your weapon burns your foes.";
		}
		if (f1 & (TR1_BRAND_COLD))
		{
			info[i++] = "Your weapon freezes your foes.";
		}

	if (f1 & (TR1_BRAND_POIS))
		{
	    info[i++] = "Your weapon poisons your foes.";
		}



		/* Special "slay" flags */
		if (f1 & (TR1_SLAY_ANIMAL))
		{
			info[i++] = "Your weapon strikes at animals with extra force.";
		}
		if (f1 & (TR1_SLAY_EVIL))
		{
			info[i++] = "Your weapon strikes at evil with extra force.";
		}
		if (f1 & (TR1_SLAY_UNDEAD))
		{
			info[i++] = "Your weapon strikes at undead with holy wrath.";
		}
		if (f1 & (TR1_SLAY_DEMON))
		{
			info[i++] = "Your weapon strikes at demons with holy wrath.";
		}
		if (f1 & (TR1_SLAY_ORC))
		{
			info[i++] = "Your weapon is especially deadly against orcs.";
		}
		if (f1 & (TR1_SLAY_TROLL))
		{
			info[i++] = "Your weapon is especially deadly against trolls.";
		}
		if (f1 & (TR1_SLAY_GIANT))
		{
			info[i++] = "Your weapon is especially deadly against giants.";
		}
		switch (f1 & TR1_ALL_SLAY_DRAGON)
		{
			case TR1_SLAY_DRAGON:
			info[i++] = "Your weapon is especially deadly against dragons.";
			break;
			case TR1_KILL_DRAGON:
			info[i++] = "Your weapon is a great bane of dragons.";
			break;
			case TR1_X15_DRAGON:
			info[i++] = "Your weapon is incredibly deadly against dragons.";
			break;
		}
	}


	/* Save the screen */
	Term_save();

	/* Erase the screen */
	for (k = 1; k < 24; k++) prt("", k, 13);

	/* Label the information */
	prt("     Your Attributes:", 1, 15);

	/* We will print on top of the map (column 13) */
	for (k = 2, j = 0; j < i; j++)
	{
		/* Show the info */
		prt(info[j], k++, 15);

		/* Every 20 entries (lines 2 to 21), start over */
		if ((k == 22) && (j+1 < i))
		{
			prt("-- more --", k, 15);
			inkey();
			for (; k > 2; k--) prt("", k, 15);
		}
	}

	/* Pause */
	prt("[Press any key to continue]", k, 13);
	inkey();

	/* Restore the screen */
	Term_load();
}






/*
 * Forget everything
 */
bool lose_all_info(void)
{
	int                 i;

	/* Forget info about objects */
	for (i = 0; i < INVEN_TOTAL; i++)
	{
		object_type *o_ptr = &inventory[i];

		/* Skip non-objects */
		if (!o_ptr->k_idx) continue;

		/* Allow "protection" by the MENTAL flag */
		if (o_ptr->ident & (IDENT_MENTAL)) continue;

		/* Hack -- Clear the "empty" flag */
		o_ptr->ident &= ~(IDENT_EMPTY);

		/* Hack -- Clear the "known" flag */
		o_ptr->ident &= ~(IDENT_KNOWN);

		/* Items still not forgotten (i.e. have easy_know) keep feelings */
		if (object_known_p(o_ptr)) continue;
 
		/* Hack -- Clear the "felt" flags */
		o_ptr->ident &= ~(IDENT_SENSE_VALUE);

		/* Hack -- Clear the "felt curses" flag for unworn items */
		if (i < INVEN_WIELD) o_ptr->ident &= ~(IDENT_SENSE_CURSED);

		/* Hack -- Clear the "heavily felt" flag */
		o_ptr->ident &= ~(IDENT_SENSE_HEAVY);

		/* Hack - Clear the "powerful" flags */
		o_ptr->ident &= ~(IDENT_POWER_ALL);
	}

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Combine / Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	/* Window stuff */
	p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_PLAYER);

	/* Mega-Hack -- Forget the map */
	wiz_dark();

	/* It worked */
	return (TRUE);
}



/*
 * Set CAVE_TRAP for every square covered by detect_traps().
 * This is a separate function as trap detection could caused by an
 * unidentified object. It must cover the same area, of course.
 */
void mark_traps(void)
{
	int x,y;

	for (y = panel_row_min; y <= panel_row_max; y++)
	{
		for (x = panel_col_min; x <= panel_col_max; x++)
		{
			cave[y][x].info |= (CAVE_TRAP);
		}
	}

	p_ptr->redraw |= PR_MAP;
	redraw_stuff();
}
	

/*
 * Detect all traps on current panel
 */
bool detect_traps(void)
{
	int y, x;

	bool detect = FALSE;

	cave_type *c_ptr;


	/* Scan the current panel */
	for (y = panel_row_min; y <= panel_row_max; y++)
	{
		for (x = panel_col_min; x <= panel_col_max; x++)
		{
			/* Access the grid */
			c_ptr = &cave[y][x];

			/* Detect invisible traps */
			if (c_ptr->feat == FEAT_INVIS)
			{
				/* Pick a trap */
				pick_trap(y, x);
			}
			
			/* Detect traps */
			if ((c_ptr->feat >= FEAT_TRAP_HEAD) &&
			    (c_ptr->feat <= FEAT_TRAP_TAIL))
			{
				/* Hack -- Memorize */
				c_ptr->info |= (CAVE_MARK);

				/* Redraw */
				lite_spot(y, x);

				/* Obvious */
				detect = TRUE;
			}
		}
	}

	/* Describe */
	if (detect)
	{
		msg_print("You sense the presence of traps!");
	}

	/* Result */
	return (detect);
}



/*
 * Detect all doors on current panel
 */
bool detect_doors(void)
{
	int y, x;

	bool detect = FALSE;

	cave_type *c_ptr;


	/* Scan the panel */
	for (y = panel_row_min; y <= panel_row_max; y++)
	{
		for (x = panel_col_min; x <= panel_col_max; x++)
		{
			c_ptr = &cave[y][x];

			/* Detect secret doors */
			if (c_ptr->feat == FEAT_SECRET)
			{
				/* Pick a door XXX XXX XXX */
				replace_secret_door(y,x);
			}

			/* Detect doors */
			if (((c_ptr->feat >= FEAT_DOOR_HEAD) &&
			     (c_ptr->feat <= FEAT_DOOR_HEAD)) ||
			    ((c_ptr->feat == FEAT_OPEN) ||
			     (c_ptr->feat == FEAT_BROKEN)))
			{
				/* Hack -- Memorize */
				c_ptr->info |= (CAVE_MARK);

				/* Redraw */
				lite_spot(y, x);

				/* Obvious */
				detect = TRUE;
			}
		}
	}

	/* Describe */
	if (detect)
	{
		msg_print("You sense the presence of doors!");
	}

	/* Result */
	return (detect);
}


/*
 * Detect all stairs on current panel
 */
bool detect_stairs(void)
{
	int y, x;

	bool detect = FALSE;

	cave_type *c_ptr;


	/* Scan the panel */
	for (y = panel_row_min; y <= panel_row_max; y++)
	{
		for (x = panel_col_min; x <= panel_col_max; x++)
		{
			c_ptr = &cave[y][x];

			/* Detect stairs */
			if ((c_ptr->feat == FEAT_LESS) ||
			    (c_ptr->feat == FEAT_MORE))
			{
				/* Hack -- Memorize */
				c_ptr->info |= (CAVE_MARK);

				/* Redraw */
				lite_spot(y, x);

				/* Obvious */
				detect = TRUE;
			}
		}
	}

	/* Describe */
	if (detect)
	{
		msg_print("You sense the presence of stairs!");
	}

	/* Result */
	return (detect);
}


/*
 * Detect any treasure on the current panel
 */
bool detect_treasure(void)
{
	int y, x;

	bool detect = FALSE;

	cave_type *c_ptr;


	/* Scan the current panel */
	for (y = panel_row_min; y <= panel_row_max; y++)
	{
		for (x = panel_col_min; x <= panel_col_max; x++)
		{
			c_ptr = &cave[y][x];

			/* Notice embedded gold */
			if ((c_ptr->feat == FEAT_MAGMA_H) ||
			    (c_ptr->feat == FEAT_QUARTZ_H))
			{
				/* Expose the gold */
				c_ptr->feat += 0x02;
			}

			/* Magma/Quartz + Known Gold */
			if ((c_ptr->feat == FEAT_MAGMA_K) ||
			    (c_ptr->feat == FEAT_QUARTZ_K))
			{
				/* Hack -- Memorize */
				c_ptr->info |= (CAVE_MARK);

				/* Redraw */
				lite_spot(y, x);

				/* Detect */
				detect = TRUE;
			}
		}
	}

	/* Describe */
	if (detect)
	{
		msg_print("You sense the presence of buried treasure!");
	}



	/* Result */
	return (detect);
}



/*
 * Detect all "gold" objects on the current panel
 */
bool detect_objects_gold(void)
{
	int i, y, x;

	bool detect = FALSE;


	/* Scan objects */
	for (i = 1; i < o_max; i++)
	{
		object_type *o_ptr = &o_list[i];

		/* Skip dead objects */
		if (!o_ptr->k_idx) continue;

		/* Skip held objects */
		if (o_ptr->held_m_idx) continue;

		/* Location */
		y = o_ptr->iy;
		x = o_ptr->ix;

		/* Only detect nearby objects */
		if (!panel_contains(y, x)) continue;
		
		/* Detect "gold" objects */
		if (o_ptr->tval == TV_GOLD)
		{
			/* Hack -- memorize it */
			o_ptr->marked = TRUE;

			/* Redraw */
			lite_spot(y, x);

			/* Detect */
			detect = TRUE;
		}
	}

	/* Describe */
	if (detect)
	{
		msg_print("You sense the presence of treasure!");
	}

    if (detect_monsters_string("$*"))
    {
        detect = TRUE;
    }

	/* Result */
	return (detect);
}


/*
 * Detect all "normal" objects on the current panel
 */
bool detect_objects_normal(void)
{
	int i, y, x;

	bool detect = FALSE;


	/* Scan objects */
	for (i = 1; i < o_max; i++)
	{
		object_type *o_ptr = &o_list[i];

		/* Skip dead objects */
		if (!o_ptr->k_idx) continue;

		/* Skip held objects */
		if (o_ptr->held_m_idx) continue;

		/* Location */
		y = o_ptr->iy;
		x = o_ptr->ix;

		/* Only detect nearby objects */
		if (!panel_contains(y, x)) continue;
		
		/* Detect "real" objects */
		if (o_ptr->tval != TV_GOLD)
		{
			/* Hack -- memorize it */
			o_ptr->marked = TRUE;

			/* Redraw */
			lite_spot(y, x);

			/* Detect */
			detect = TRUE;
		}
	}

	/* Describe */
	if (detect)
	{
		msg_print("You sense the presence of objects!");
	}

    if (detect_monsters_string("!=?|"))
    {
        detect = TRUE;
    }

	/* Result */
	return (detect);
}


/*
 * Detect all "magic" objects on the current panel.
 *
 * This will light up all spaces with "magic" items, including artifacts,
 * ego-items, potions, scrolls, books, rods, wands, staves, amulets, rings,
 * and "enchanted" items of the "good" variety.
 *
 * It can probably be argued that this function is now too powerful.
 */
bool detect_objects_magic(void)
{
	int i, y, x, tv;

	bool detect = FALSE;


	/* Scan all objects */
	for (i = 1; i < o_max; i++)
	{
		object_type *o_ptr = &o_list[i];
		
		/* Skip dead objects */
		if (!o_ptr->k_idx) continue;

		/* Skip held objects */
		if (o_ptr->held_m_idx) continue;

		/* Location */
		y = o_ptr->iy;
		x = o_ptr->ix;

		/* Only detect nearby objects */
		if (!panel_contains(y,x)) continue;
		
		/* Examine the tval */
		tv = o_ptr->tval;

		/* Artifacts, misc magic items, or enchanted wearables */
	if (allart_p(o_ptr) || ego_item_p(o_ptr) ||
		    (tv == TV_AMULET) || (tv == TV_RING) ||
		    (tv == TV_STAFF) || (tv == TV_WAND) || (tv == TV_ROD) ||
		    (tv == TV_SCROLL) || (tv == TV_POTION) ||
	    (tv == TV_SORCERY_BOOK) ||
	    (tv == TV_THAUMATURGY_BOOK) ||
        (tv == TV_CONJURATION_BOOK) || (tv == TV_NECROMANCY_BOOK) ||
		    ((o_ptr->to_a > 0) || (o_ptr->to_h + o_ptr->to_d > 0)))
		{
			/* Memorize the item */
			o_ptr->marked = TRUE;

			/* Redraw */
			lite_spot(y, x);

			/* Detect */
			detect = TRUE;
		}
	}

	/* Describe */
	if (detect)
	{
		msg_print("You sense the presence of magic objects!");
	}

	/* Return result */
	return (detect);
}


/*
 * Detect all "normal" monsters on the current panel
 */
bool detect_monsters_normal(void)
{
	int             i, y, x;

	bool    flag = FALSE;


	/* Scan monsters */
	for (i = 1; i < m_max; i++)
	{
		monster_type *m_ptr = &m_list[i];
		monster_race *r_ptr = &r_info[m_ptr->r_idx];

		/* Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Location */
		y = m_ptr->fy;
		x = m_ptr->fx;

		/* Only detect nearby monsters */
		if (!panel_contains(y, x)) continue;
		
		/* Detect all non-invisible monsters */
	if ((!(r_ptr->flags2 & (RF2_INVISIBLE)))
	    || p_ptr->see_inv || p_ptr->tim_invis)
		{
			/* Repair visibility later */
			repair_monsters = TRUE;

			/* Hack -- Detect monster */
			m_ptr->mflag |= (MFLAG_MARK | MFLAG_SHOW);

			/* Hack -- See monster */
			m_ptr->ml = TRUE;
			
			/* Redraw */
			lite_spot(y, x);

			/* Detect */
			flag = TRUE;
		}
	}

	/* Describe */
	if (flag)
	{
		/* Describe result */
		msg_print("You sense the presence of monsters!");
	}

	/* Update window */
	p_ptr->window |= PW_VISIBLE;

	/* Result */
	return (flag);
}


/*
 * Detect all "invisible" monsters on current panel
 */
bool detect_monsters_invis(void)
{
	int             i, y, x;

	bool    flag = FALSE;


	/* Scan monsters */
	for (i = 1; i < m_max; i++)
	{
		monster_type *m_ptr = &m_list[i];
		monster_race *r_ptr = &r_info[m_ptr->r_idx];

		/* Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Location */
		y = m_ptr->fy;
		x = m_ptr->fx;

		/* Only detect nearby monsters */
		if (!panel_contains(y, x)) continue;
		
		/* Detect invisible monsters */
		if (r_ptr->flags2 & (RF2_INVISIBLE))
		{
			/* Take note that they are invisible */
			r_ptr->r_flags2 |= (RF2_INVISIBLE);

			/* Update monster recall window */
			if (monster_race_idx == m_ptr->r_idx)
			{
				/* Window stuff */
				p_ptr->window |= (PW_MONSTER);
			}

			/* Repair visibility later */
			repair_monsters = TRUE;

			/* Hack -- Detect monster */
			m_ptr->mflag |= (MFLAG_MARK | MFLAG_SHOW);

			/* Hack -- See monster */
			m_ptr->ml = TRUE;
			
			/* Redraw */
			lite_spot(y, x);

			/* Detect */
			flag = TRUE;
		}
	}

	/* Describe */
	if (flag)
	{
		/* Describe result */
		msg_print("You sense the presence of invisible creatures!");
	}

	/* Update window */
	p_ptr->window |= PW_VISIBLE;

	/* Result */
	return (flag);
}



/*
 * Detect all "evil" monsters on current panel
 */
bool detect_monsters_evil(void)
{
	int             i, y, x;

	bool    flag = FALSE;


	/* Scan monsters */
	for (i = 1; i < m_max; i++)
	{
		monster_type *m_ptr = &m_list[i];
		monster_race *r_ptr = &r_info[m_ptr->r_idx];

		/* Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Location */
		y = m_ptr->fy;
		x = m_ptr->fx;

		/* Only detect nearby monsters */
		if (!panel_contains(y, x)) continue;
		
		/* Detect evil monsters */
		if (r_ptr->flags3 & (RF3_EVIL))
		{
			/* Take note that they are evil */
			r_ptr->r_flags3 |= (RF3_EVIL);

			/* Update monster recall window */
			if (monster_race_idx == m_ptr->r_idx)
			{
				/* Window stuff */
				p_ptr->window |= (PW_MONSTER);
			}

			/* Repair visibility later */
			repair_monsters = TRUE;

			/* Hack -- Detect monster */
			m_ptr->mflag |= (MFLAG_MARK | MFLAG_SHOW);

			/* Hack -- See monster */
			m_ptr->ml = TRUE;
			
			/* Redraw */
			lite_spot(y, x);

			/* Detect */
			flag = TRUE;
		}
	}

	/* Describe */
	if (flag)
	{
		/* Describe result */
		msg_print("You sense the presence of evil creatures!");
	}

	/* Update window */
	p_ptr->window |= PW_VISIBLE;

	/* Result */
	return (flag);
}




/*
 * Detect all (string) monsters on current panel
 */
static bool detect_monsters_string(cptr Match)
{
	int             i, y, x;

	bool    flag = FALSE;


	/* Scan monsters */
	for (i = 1; i < m_max; i++)
	{
		monster_type *m_ptr = &m_list[i];
		monster_race *r_ptr = &r_info[m_ptr->r_idx];

		/* Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Location */
		y = m_ptr->fy;
		x = m_ptr->fx;

		/* Only detect nearby monsters */
		if (!panel_contains(y, x)) continue;
		
		/* Detect evil monsters */
        if (strchr(Match, r_ptr->d_char))
    
		{

			/* Update monster recall window */
			if (monster_race_idx == m_ptr->r_idx)
			{
				/* Window stuff */
				p_ptr->window |= (PW_MONSTER);
			}

			/* Repair visibility later */
			repair_monsters = TRUE;

			/* Hack -- Detect monster */
			m_ptr->mflag |= (MFLAG_MARK | MFLAG_SHOW);

			/* Hack -- See monster */
			m_ptr->ml = TRUE;
			
			/* Redraw */
			lite_spot(y, x);

			/* Detect */
			flag = TRUE;
		}
	}

	/* Describe */
	if (flag)
	{
		/* Describe result */
        msg_print("You sense the presence of monsters!");
	}

	/* Update window */
	p_ptr->window |= PW_VISIBLE;

	/* Result */
	return (flag);
}


/*
 * A "generic" detect monsters routine, tagged to flags3
 */
#if 0
bool detect_monsters_xxx(u32b match_flag)
{
	int             i, y, x;

	bool    flag = FALSE;
    cptr desc_monsters = "weird monsters";


	/* Scan monsters */
	for (i = 1; i < m_max; i++)
	{
		monster_type *m_ptr = &m_list[i];
		monster_race *r_ptr = &r_info[m_ptr->r_idx];

		/* Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Location */
		y = m_ptr->fy;
		x = m_ptr->fx;

		/* Only detect nearby monsters */
		if (!panel_contains(y, x)) continue;
		
		/* Detect evil monsters */
		if (r_ptr->flags3 & (match_flag))
		{
			/* Take note that they are something */
			r_ptr->r_flags3 |= (match_flag);

			/* Update monster recall window */
			if (monster_race_idx == m_ptr->r_idx)
			{
				/* Window stuff */
				p_ptr->window |= (PW_MONSTER);
			}

			/* Repair visibility later */
			repair_monsters = TRUE;

			/* Hack -- Detect monster */
			m_ptr->mflag |= (MFLAG_MARK | MFLAG_SHOW);

			/* Hack -- See monster */
			m_ptr->ml = TRUE;
			
			/* Redraw */
			lite_spot(y, x);

			/* Detect */
			flag = TRUE;
		}
	}

	/* Describe */
	if (flag)
	{
	switch (match_flag)
		{ case RF3_DEMON:
	    desc_monsters = "demons";
			break;
		  case RF3_UNDEAD:
	    desc_monsters = "the undead";
			break;
		}
		/* Describe result */
		msg_format("You sense the presence of %s!", desc_monsters);
		msg_print(NULL);

		/* Update window */
		p_ptr->window |= PW_VISIBLE;
	}

	/* Result */
	return (flag);
}
#endif


/*
 * Detect everything
 */
bool detect_all(void)
{
	bool detect = FALSE;

	/* Detect everything */
	if (detect_traps()) detect = TRUE;
	if (detect_doors()) detect = TRUE;
	if (detect_stairs()) detect = TRUE;
	if (detect_treasure()) detect = TRUE;
	if (detect_objects_gold()) detect = TRUE;
	if (detect_objects_normal()) detect = TRUE;
	if (detect_monsters_invis()) detect = TRUE;
	if (detect_monsters_normal()) detect = TRUE;
	if (detect) mark_traps();
	
	/* Result */
	return (detect);
}



/*
 * Create stairs at the player location
 */
void stair_creation(void)
{
	/* XXX XXX XXX */
	if (!cave_valid_bold(py, px))
	{
		msg_print("The object resists the spell.");
		return;
	}

	/* XXX XXX XXX */
	delete_object(py, px);

	/* Create a staircase */
	if (dun_level <= 0)
	{
		return;
	}
	else if (is_quest(dun_level) || (dun_level >= dun_defs[cur_dungeon].max_level))
	{
		if(dun_defs[cur_dungeon].tower)
		{
			cave_set_feat(py, px, FEAT_MORE);
		}
		else
		{
			cave_set_feat(py, px, FEAT_LESS);
		}
	}
	else if (rand_int(100) < 50)
	{
		cave_set_feat(py, px, FEAT_MORE);
	}
	else
	{
		cave_set_feat(py, px, FEAT_LESS);
	}
}




/*
 * Hook to specify "weapon"
 */
static bool item_tester_hook_weapon(object_type *o_ptr)
{
	switch (o_ptr->tval)
	{
		case TV_SWORD:
		case TV_HAFTED:
		case TV_POLEARM:
		case TV_DIGGING:
		case TV_BOW:
		case TV_BOLT:
		case TV_ARROW:
		case TV_SHOT:
		{
			return (TRUE);
		}
	}

	return (FALSE);
}


/*
 * Hook to specify "armour"
 */
bool item_tester_hook_armour(object_type *o_ptr)
{
	switch (o_ptr->tval)
	{
		case TV_DRAG_ARMOR:
		case TV_HARD_ARMOR:
		case TV_SOFT_ARMOR:
		case TV_SHIELD:
		case TV_CLOAK:
		case TV_CROWN:
		case TV_HELM:
		case TV_BOOTS:
		case TV_GLOVES:
		{
			return (TRUE);
		}
	}

	return (FALSE);
}



/*
 * Enchants a plus onto an item.                        -RAK-
 *
 * Revamped!  Now takes item pointer, number of times to try enchanting,
 * and a flag of what to try enchanting.  Artifacts resist enchantment
 * some of the time, and successful enchantment to at least +0 might
 * break a curse on the item.  -CFT
 *
 * Note that an item can technically be enchanted all the way to +15 if
 * you wait a very, very, long time.  Going from +9 to +10 only works
 * about 5% of the time, and from +10 to +11 only about 1% of the time.
 *
 * Note that this function can now be used on "piles" of items, and
 * the larger the pile, the lower the chance of success.
 */
bool enchant(object_type *o_ptr, int n, int eflag)
{
	int i, chance, prob;

	bool res = FALSE;

    bool a = (allart_p(o_ptr));

	u32b f1, f2, f3;

	/* Extract the flags */
	object_flags(o_ptr, &f1, &f2, &f3);


	/* Large piles resist enchantment */
	prob = o_ptr->number * 100;

	/* Missiles are easy to enchant */
	if ((o_ptr->tval == TV_BOLT) ||
	    (o_ptr->tval == TV_ARROW) ||
	    (o_ptr->tval == TV_SHOT))
	{
		prob = prob / 20;
	}

	/* Try "n" times */
	for (i=0; i<n; i++)
	{
		/* Hack -- Roll for pile resistance */
		if (rand_int(prob) >= 100) continue;

		/* Enchant to hit */
		if (eflag & (ENCH_TOHIT))
		{
			if (o_ptr->to_h < 0) chance = 0;
			else if (o_ptr->to_h > 15) chance = 1000;
			else chance = enchant_table[o_ptr->to_h];

			if ((randint(1000) > chance) && (!a || (rand_int(100) < 50)))
			{
				o_ptr->to_h++;
				res = TRUE;

				/* only when you get it above -1 -CFT */
				if (cursed_p(o_ptr) &&
				    (!(f3 & (TR3_PERMA_CURSE))) &&
				    (o_ptr->to_h >= 0) && (rand_int(100) < 25))
				{
					msg_print("The curse is broken!");
					o_ptr->ident &= ~(IDENT_CURSED);
					o_ptr->ident |= (IDENT_SENSE_CURSED);

                    if (o_ptr->flags3 & (TR3_CURSED))
                        o_ptr->flags3 &= ~(TR3_CURSED);
                    if (o_ptr->flags3 & (TR3_HEAVY_CURSE))
                        o_ptr->flags3 &= ~(TR3_HEAVY_CURSE);

				}
			}
		}

		/* Enchant to damage */
		if (eflag & (ENCH_TODAM))
		{
			if (o_ptr->to_d < 0) chance = 0;
			else if (o_ptr->to_d > 15) chance = 1000;
			else chance = enchant_table[o_ptr->to_d];

			if ((randint(1000) > chance) && (!a || (rand_int(100) < 50)))
			{
				o_ptr->to_d++;
				res = TRUE;

				/* only when you get it above -1 -CFT */
				if (cursed_p(o_ptr) &&
				    (!(f3 & (TR3_PERMA_CURSE))) &&
				    (o_ptr->to_d >= 0) && (rand_int(100) < 25))
				{
					msg_print("The curse is broken!");
					o_ptr->ident &= ~(IDENT_CURSED);
					o_ptr->ident |= (IDENT_SENSE_CURSED);

                    if (o_ptr->flags3 & (TR3_CURSED))
                        o_ptr->flags3 &= ~(TR3_CURSED);
                    if (o_ptr->flags3 & (TR3_HEAVY_CURSE))
                        o_ptr->flags3 &= ~(TR3_HEAVY_CURSE);

				}
			}
		}

		/* Enchant to armor class */
		if (eflag & (ENCH_TOAC))
		{
			if (o_ptr->to_a < 0) chance = 0;
			else if (o_ptr->to_a > 15) chance = 1000;
			else chance = enchant_table[o_ptr->to_a];

			if ((randint(1000) > chance) && (!a || (rand_int(100) < 50)))
			{
				o_ptr->to_a++;
				res = TRUE;

				/* only when you get it above -1 -CFT */
				if (cursed_p(o_ptr) &&
				    (!(f3 & (TR3_PERMA_CURSE))) &&
				    (o_ptr->to_a >= 0) && (rand_int(100) < 25))
				{
					msg_print("The curse is broken!");
					o_ptr->ident &= ~(IDENT_CURSED);
					o_ptr->ident |= (IDENT_SENSE_CURSED);

                    if (o_ptr->flags3 & (TR3_CURSED))
                        o_ptr->flags3 &= ~(TR3_CURSED);
                    if (o_ptr->flags3 & (TR3_HEAVY_CURSE))
                        o_ptr->flags3 &= ~(TR3_HEAVY_CURSE);
				}
			}
		}
	}

	/* Failure */
	if (!res) return (FALSE);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Combine / Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	/* Window stuff */
	p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_PLAYER);

	/* Success */
	return (TRUE);
}

/* Just in case */
#ifndef allart_p
#define allart_p(T) \
	((artifact_p(T) || (T)->art_name) ? TRUE : FALSE)
#endif

/*
 * Enchant an item (in the inventory or on the floor)
 * Note that "num_ac" requires armour, else weapon
 * Returns TRUE if attempted, FALSE if cancelled
 */
bool enchant_spell(int num_hit, int num_dam, int num_ac)
{
	int                     item;
	bool            okay = FALSE;

	object_type             *o_ptr;

	C_TNEW(o_name, ONAME_MAX, char);


	/* Assume enchant weapon */
	item_tester_hook = item_tester_hook_weapon;

	/* Enchant armor if requested */
	if (num_ac) item_tester_hook = item_tester_hook_armour;

	/* Get an item (from equip or inven or floor) */
	if (!get_item(&item, "Enchant which item? ", TRUE, TRUE, TRUE))
	{
		if (item == -2) msg_print("You have nothing to enchant.");
		TFREE(o_name);
		return (FALSE);
	}

	/* Get the item (in the pack) */
	if (item >= 0)
	{
		o_ptr = &inventory[item];
	}

	/* Get the item (on the floor) */
	else
	{
		o_ptr = &o_list[0 - item];
	}


	/* Description */
	object_desc(o_name, o_ptr, FALSE, 0);

	/* Describe */
	msg_format("%s %s glow%s brightly!",
		   ((item >= 0 && !allart_p(o_ptr)) ? "Your" : "The"), o_name,
		   ((o_ptr->number > 1) ? "" : "s"));

	/* Enchant */
	if (enchant(o_ptr, num_hit, ENCH_TOHIT)) okay = TRUE;
	if (enchant(o_ptr, num_dam, ENCH_TODAM)) okay = TRUE;
	if (enchant(o_ptr, num_ac, ENCH_TOAC)) okay = TRUE;

	/* Failure */
	if (!okay)
	{
		/* Flush */
		if (flush_failure) flush();

		/* Message */
		msg_print("The enchantment failed.");
	}
	else
	{
		object_desc(o_name, o_ptr, FALSE, 1);
		full_name(o_name, (o_ptr->number > 1), TRUE, FALSE);
	
		/* Describe again */
		msg_format("You now %s %s", (item >= 0) ? "have" : "see", o_name);
	}	

	/* Something happened */
	TFREE(o_name);
	return (TRUE);
}


static void curse_artifact (object_type * o_ptr)
{
	    if (o_ptr->pval) o_ptr->pval = 0 - ((o_ptr->pval) + randint(4));
	    if (o_ptr->to_a) o_ptr->to_a = 0 - ((o_ptr->to_a) + randint(4));
	    if (o_ptr->to_h) o_ptr->to_h = 0 - ((o_ptr->to_h) + randint(4));
	    if (o_ptr->to_d) o_ptr->to_d = 0 - ((o_ptr->to_d) + randint(4));
	    o_ptr->flags3 |= ( TR3_HEAVY_CURSE | TR3_CURSED );
	    if (randint(4)==1) o_ptr-> flags3 |= TR3_PERMA_CURSE;
	    if (randint(3)==1) o_ptr-> flags3 |= TR3_TY_CURSE;
	    if (randint(2)==1) o_ptr-> flags3 |= TR3_AGGRAVATE;
	    if (randint(3)==1) o_ptr-> flags3 |= TR3_DRAIN_EXP;
	    if (randint(2)==1) o_ptr-> flags3 |= TR3_TELEPORT;
        else if (randint(3)==1) o_ptr->flags3 |= TR3_NO_TELE;
        if (randint(3)==1) o_ptr->flags3 |= TR3_NO_MAGIC;
	    o_ptr->ident |= IDENT_CURSED;

}

static void random_plus (object_type * o_ptr, bool is_scroll)
{

    int this_type = (o_ptr->tval<TV_BOOTS?23:19);

    if (artifact_bias == BIAS_WARRIOR)
    {
	if (!(o_ptr->flags1 & TR1_STR))
	{
	    o_ptr->flags1 |= TR1_STR;
	    if (randint(2)==1) return; /* 50% chance of being a "free" power */
	}
	if (!(o_ptr->flags1 & TR1_CON))
	{
	    o_ptr->flags1 |= TR1_CON;
	    if (randint(2)==1) return;
	}

	if (!(o_ptr->flags1 & TR1_DEX))
	{
	    o_ptr->flags1 |= TR1_DEX;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_MAGE)
    {
	if (!(o_ptr->flags1 & TR1_INT))
	{
	    o_ptr->flags1 |= TR1_INT;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_PRIESTLY)
    {
	if (!(o_ptr->flags1 & TR1_WIS))
	{
	    o_ptr->flags1 |= TR1_WIS;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_RANGER)
    {
	if (!(o_ptr->flags1 & TR1_CON))
	{
	    o_ptr->flags1 |= TR1_CON;
	    if (randint(2)==1) return; /* 50% chance of being a "free" power */
	}
	if (!(o_ptr->flags1 & TR1_DEX))
	{
	    o_ptr->flags1 |= TR1_DEX;
	    if (randint(2)==1) return;
	}

	if (!(o_ptr->flags1 & TR1_STR))
	{
	    o_ptr->flags1 |= TR1_STR;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_ROGUE)
    {
	if (!(o_ptr->flags1 & TR1_STEALTH))
	{
	    o_ptr->flags1 |= TR1_STEALTH;
	    if (randint(2)==1) return;
	}
	if (!(o_ptr->flags1 & TR1_SEARCH))
	{
	    o_ptr->flags1 |= TR1_SEARCH;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_STR)
    {
	if (!(o_ptr->flags1 & TR1_STR))
	{
	    o_ptr->flags1 |= TR1_STR;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_WIS)
    {
	if (!(o_ptr->flags1 & TR1_WIS))
	{
	    o_ptr->flags1 |= TR1_WIS;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_INT)
    {
	if (!(o_ptr->flags1 & TR1_INT))
	{
	    o_ptr->flags1 |= TR1_INT;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_DEX)
    {
	if (!(o_ptr->flags1 & TR1_DEX))
	{
	    o_ptr->flags1 |= TR1_DEX;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_CON)
    {
	if (!(o_ptr->flags1 & TR1_CON))
	{
	    o_ptr->flags1 |= TR1_CON;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_CHR)
    {
	if (!(o_ptr->flags1 & TR1_CHR))
	{
	    o_ptr->flags1 |= TR1_CHR;
	    if (randint(2)==1) return;
	}
    }



    switch (randint(this_type))
    {
    case 1: case 2:
	o_ptr->flags1 |= TR1_STR;
/*  if (is_scroll) msg_print ("It makes you feel strong!"); */
	if (!(artifact_bias) && randint(13)!=1)
	    artifact_bias = BIAS_STR;
	else if (!(artifact_bias) && randint(7)==1)
	    artifact_bias = BIAS_WARRIOR;
	break;
    case 3: case 4:
	o_ptr->flags1 |= TR1_INT;
/*  if (is_scroll) msg_print ("It makes you feel smart!"); */
	if (!(artifact_bias) && randint(13)!=1)
	    artifact_bias = BIAS_INT;
	else if (!(artifact_bias) && randint(7)==1)
	    artifact_bias = BIAS_MAGE;
	break;
    case 5: case 6:
	o_ptr->flags1 |= TR1_WIS;
/*  if (is_scroll) msg_print ("It makes you feel wise!"); */
	if (!(artifact_bias) && randint(13)!=1)
	    artifact_bias = BIAS_WIS;
	else if (!(artifact_bias) && randint(7)==1)
	    artifact_bias = BIAS_PRIESTLY;
	break;
    case 7: case 8:
	o_ptr->flags1 |= TR1_DEX;
/*  if (is_scroll) msg_print ("It makes you feel nimble!"); */
	if (!(artifact_bias) && randint(13)!=1)
	    artifact_bias = BIAS_DEX;
	else if (!(artifact_bias) && randint(7)==1)
	    artifact_bias = BIAS_ROGUE;
	break;
    case 9: case 10:
	o_ptr->flags1 |= TR1_CON;
/*  if (is_scroll) msg_print ("It makes you feel healthy!"); */
	if (!(artifact_bias) && randint(13)!=1)
	    artifact_bias = BIAS_CON;
	else if (!(artifact_bias) && randint(9)==1)
	    artifact_bias = BIAS_RANGER;
	break;
    case 11: case 12:
	o_ptr->flags1 |= TR1_CHR;
/*  if (is_scroll) msg_print ("It makes you look great!"); */
	if (!(artifact_bias) && randint(13)!=1)
	    artifact_bias = BIAS_CHR;
	break;
    case 13: case 14:
	o_ptr->flags1 |= TR1_STEALTH;
/*  if (is_scroll) msg_print ("It looks muffled."); */
	if (!(artifact_bias) && randint(3)==1)
	    artifact_bias = BIAS_ROGUE;
	break;
    case 15: case 16:
	o_ptr->flags1 |= TR1_SEARCH;
/*  if (is_scroll) msg_print ("It makes you see better."); */
	if (!(artifact_bias) && randint(9)==1)
	    artifact_bias = BIAS_RANGER;
	break;
    case 17: case 18:
	o_ptr->flags1 |= TR1_INFRA;
/*  if (is_scroll) msg_print ("It makes you see tiny red animals.");*/
	break;
    case 19:
	o_ptr->flags1 |= TR1_SPEED;
/*  if (is_scroll) msg_print ("It makes you move faster!"); */
	if (!(artifact_bias) && randint(11)==1)
	    artifact_bias = BIAS_ROGUE;
	break;
    case 20: case 21:
	o_ptr->flags1 |= TR1_TUNNEL;
/*  if (is_scroll) msg_print ("Gravel flies from it!"); */
	break;
    case 22: case 23:
	if (o_ptr->tval == TV_BOW) random_plus(o_ptr, is_scroll);
	else
	{
	o_ptr->flags1 |= TR1_BLOWS;
/*  if (is_scroll) msg_print ("It seems faster!"); */
	if (!(artifact_bias) && randint(11)==1)
	    artifact_bias = BIAS_WARRIOR;
	}
	break;
    }

}


void random_resistance (object_type * o_ptr, bool is_scroll, int specific)
{

  if (!specific) /* To avoid a number of possible bugs */
  {
    if (artifact_bias == BIAS_ACID)
    {
	if (!(o_ptr->flags2 & TR2_RES_ACID))
	{
	    o_ptr->flags2 |= TR2_RES_ACID;
	    if (randint(2)==1) return;
	}
    if (randint(BIAS_LUCK)==1 && !(o_ptr->flags2 & TR2_IM_ACID))
	{
	    o_ptr->flags2 |= TR2_IM_ACID;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_ELEC)
    {
	if (!(o_ptr->flags2 & TR2_RES_ELEC))
	{
	    o_ptr->flags2 |= TR2_RES_ELEC;
	    if (randint(2)==1) return;
	}
    if (o_ptr->tval >= TV_CLOAK && o_ptr->tval <= TV_HARD_ARMOR &&
        ! (o_ptr->flags3 & TR3_SH_ELEC))
        {
            o_ptr->flags2 |= TR3_SH_ELEC;
            if (randint(2)==1) return;
        }
    if (randint(BIAS_LUCK)==1 && !(o_ptr->flags2 & TR2_IM_ELEC))
	{
	    o_ptr->flags2 |= TR2_IM_ELEC;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_FIRE)
    {
	if (!(o_ptr->flags2 & TR2_RES_FIRE))
	{
	    o_ptr->flags2 |= TR2_RES_FIRE;
	    if (randint(2)==1) return;
	}
    if (o_ptr->tval >= TV_CLOAK && o_ptr->tval <= TV_HARD_ARMOR &&
        ! (o_ptr->flags3 & TR3_SH_FIRE))
        {
            o_ptr->flags3 |= TR3_SH_FIRE | TR3_LITE;
            if (randint(2)==1) return;
        }
    if (randint(BIAS_LUCK)==1 && !(o_ptr->flags2 & TR2_IM_FIRE))
	{
	    o_ptr->flags2 |= TR2_IM_FIRE;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_COLD)
    {
	if (!(o_ptr->flags2 & TR2_RES_COLD))
	{
	    o_ptr->flags2 |= TR2_RES_COLD;
	    if (randint(2)==1) return;
	}
    if (randint(BIAS_LUCK)==1 && !(o_ptr->flags2 & TR2_IM_COLD))
	{
	    o_ptr->flags2 |= TR2_IM_COLD;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_POIS)
    {
	if (!(o_ptr->flags2 & TR2_RES_POIS))
	{
	    o_ptr->flags2 |= TR2_RES_POIS;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_WARRIOR)
    {
	if (randint(3)!=1 && (!(o_ptr->flags2 & TR2_RES_FEAR)))
	{
	    o_ptr->flags2 |= TR2_RES_FEAR;
	    if (randint(2)==1) return;
	}
    if ((randint(3)==1) && (!(o_ptr->flags3 & TR3_NO_MAGIC)))
    {
        o_ptr->flags3 |= TR3_NO_MAGIC;
        if (randint(2)==1) return;
    }
    }
    else if (artifact_bias == BIAS_NECROMANTIC)
    {
	if (!(o_ptr->flags2 & TR2_RES_NETHER))
	{
	    o_ptr->flags2 |= TR2_RES_NETHER;
	    if (randint(2)==1) return;
	}
	if (!(o_ptr->flags2 & TR2_RES_POIS))
	{
	    o_ptr->flags2 |= TR2_RES_POIS;
	    if (randint(2)==1) return;
	}
	if (!(o_ptr->flags2 & TR2_RES_DARK))
	{
	    o_ptr->flags2 |= TR2_RES_DARK;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_CHAOS)
    {
	if (!(o_ptr->flags2 & TR2_RES_CHAOS))
	{
	    o_ptr->flags2 |= TR2_RES_CHAOS;
	    if (randint(2)==1) return;
	}
	if (!(o_ptr->flags2 & TR2_RES_CONF))
	{
	    o_ptr->flags2 |= TR2_RES_CONF;
	    if (randint(2)==1) return;
	}
	if (!(o_ptr->flags2 & TR2_RES_DISEN))
	{
	    o_ptr->flags2 |= TR2_RES_DISEN;
	    if (randint(2)==1) return;
	}
    }
  }

    switch (specific?specific:randint(41))
    {
    case 1:
    if (randint(WEIRD_LUCK)!=1)
        random_resistance(o_ptr, is_scroll, specific);
	else
	{
	o_ptr->flags2 |= TR2_IM_ACID;
/*  if (is_scroll) msg_print("It looks totally incorruptible."); */
	if (!(artifact_bias))
	    artifact_bias = BIAS_ACID;
	}
	break;
    case 2:
    if (randint(WEIRD_LUCK)!=1)
	    random_resistance(o_ptr, is_scroll, specific);
	else
	{
	o_ptr->flags2 |= TR2_IM_ELEC;
/*  if (is_scroll) msg_print("It looks completely grounded."); */
	if (!(artifact_bias))
	    artifact_bias = BIAS_ELEC;
	}
	break;
    case 3:
    if (randint(WEIRD_LUCK)!=1)
	    random_resistance(o_ptr, is_scroll, specific);
	else
	{
	o_ptr->flags2 |= TR2_IM_COLD;
/*  if (is_scroll) msg_print("It feels very warm."); */
	if (!(artifact_bias))
	    artifact_bias = BIAS_COLD;
	}
	break;
    case 4:
    if (randint(WEIRD_LUCK)!=1)
	    random_resistance(o_ptr, is_scroll, specific);
	else
	{
	o_ptr->flags2 |= TR2_IM_FIRE;
/*  if (is_scroll) msg_print("It feels very cool."); */
	if (!(artifact_bias))
	    artifact_bias = BIAS_FIRE;
	}
	break;
    case 5: case 6: case 13:
	o_ptr->flags2 |= TR2_RES_ACID;
/*  if (is_scroll) msg_print("It makes your stomach rumble."); */
	if (!(artifact_bias))
	    artifact_bias = BIAS_ACID;
	break;
    case 7: case 8: case 14:
	o_ptr->flags2 |= TR2_RES_ELEC;
/*  if (is_scroll) msg_print("It makes you feel grounded."); */
    if (!(artifact_bias))
	    artifact_bias = BIAS_ELEC;
	break;
    case 9: case 10: case 15:
	o_ptr->flags2 |= TR2_RES_FIRE;
/*  if (is_scroll) msg_print("It makes you feel cool!");*/
	if (!(artifact_bias))
	    artifact_bias = BIAS_FIRE;
	break;
    case 11: case 12: case 16:
	o_ptr->flags2 |= TR2_RES_COLD;
/*  if (is_scroll) msg_print("It makes you feel full of hot air!");*/
	if (!(artifact_bias))
	    artifact_bias = BIAS_COLD;
	break;
    case 17: case 18:
	o_ptr->flags2 |= TR2_RES_POIS;
/*  if (is_scroll) msg_print("It makes breathing easier for you."); */
	if (!(artifact_bias) && randint(4)!=1)
	    artifact_bias = BIAS_POIS;
	else if (!(artifact_bias) && randint(2)==1)
	    artifact_bias = BIAS_NECROMANTIC;
	else if (!(artifact_bias) && randint(2)==1)
	    artifact_bias = BIAS_ROGUE;
	break;
    case 19: case 20:
	o_ptr->flags2 |= TR2_RES_FEAR;
/*  if (is_scroll) msg_print("It makes you feel brave!"); */
	if (!(artifact_bias) && randint(3)==1)
	    artifact_bias = BIAS_WARRIOR;
	break;
    case 21:
	o_ptr->flags2 |= TR2_RES_LITE;
/*  if (is_scroll) msg_print("It makes everything look darker.");*/
	break;
    case 22:
	o_ptr->flags2 |= TR2_RES_DARK;
/*  if (is_scroll) msg_print("It makes everything look brigher.");*/
	break;
    case 23: case 24:
	o_ptr->flags2 |= TR2_RES_BLIND;
/*  if (is_scroll) msg_print("It makes you feel you are wearing glasses.");*/
	break;
    case 25: case 26:
	o_ptr->flags2 |= TR2_RES_CONF;
/*  if (is_scroll) msg_print("It makes you feel very determined.");*/
	if (!(artifact_bias) && randint(6)==1)
	    artifact_bias = BIAS_CHAOS;
	break;
    case 27: case 28:
	o_ptr->flags2 |= TR2_RES_SOUND;
/*  if (is_scroll) msg_print("It makes you feel deaf!");*/
	break;
    case 29: case 30:
	o_ptr->flags2 |= TR2_RES_SHARDS;
/*  if (is_scroll) msg_print("It makes your skin feel thicker.");*/
	break;
    case 31: case 32:
	o_ptr->flags2 |= TR2_RES_NETHER;
/*  if (is_scroll) msg_print("It makes you feel like visiting a graveyard!");*/
	if (!(artifact_bias) && randint(3)==1)
	    artifact_bias = BIAS_NECROMANTIC;
	break;
    case 33: case 34:
	o_ptr->flags2 |= TR2_RES_NEXUS;
/*  if (is_scroll) msg_print("It makes you feel normal.");*/
	break;
    case 35: case 36:
	o_ptr->flags2 |= TR2_RES_CHAOS;
/*  if (is_scroll) msg_print("It makes you feel very firm.");*/
	if (!(artifact_bias) && randint(2)==1)
	    artifact_bias = BIAS_CHAOS;
	break;
    case 37: case 38:
	o_ptr->flags2 |= TR2_RES_DISEN;
/*  if (is_scroll) msg_print("It is surrounded by a static feeling.");*/
	break;
    case 39:
    if (o_ptr->tval >= TV_CLOAK && o_ptr->tval <= TV_HARD_ARMOR)
        o_ptr->flags3 |= TR3_SH_ELEC;
    else
	    random_resistance(o_ptr, is_scroll, specific);
    if (!(artifact_bias))
	    artifact_bias = BIAS_ELEC;
    break;
    case 40:
    if (o_ptr->tval >= TV_CLOAK && o_ptr->tval <= TV_HARD_ARMOR)
        o_ptr->flags3 |= TR3_SH_FIRE | TR3_LITE;
    else
	    random_resistance(o_ptr, is_scroll, specific);
    if (!(artifact_bias))
        artifact_bias = BIAS_FIRE;
    break;
    case 41:
    if (o_ptr->tval == TV_SHIELD || o_ptr->tval == TV_CLOAK ||
        o_ptr->tval == TV_HELM || o_ptr->tval == TV_HARD_ARMOR)
        o_ptr->flags2 |= TR2_REFLECT;
    else
	    random_resistance(o_ptr, is_scroll, specific);
    break;
    }
}

static void random_misc (object_type * o_ptr, bool is_scroll)
{

    if (artifact_bias == BIAS_RANGER)
    {
	if (!(o_ptr->flags2 & TR2_SUST_CON))
	{
	    o_ptr->flags2 |= TR2_SUST_CON;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_STR)
    {
	if (!(o_ptr->flags2 & TR2_SUST_STR))
	{
	    o_ptr->flags2 |= TR2_SUST_STR;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_WIS)
    {
	if (!(o_ptr->flags2 & TR2_SUST_WIS))
	{
	    o_ptr->flags2 |= TR2_SUST_WIS;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_INT)
    {
	if (!(o_ptr->flags2 & TR2_SUST_INT))
	{
	    o_ptr->flags2 |= TR2_SUST_INT;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_DEX)
    {
	if (!(o_ptr->flags2 & TR2_SUST_DEX))
	{
	    o_ptr->flags2 |= TR2_SUST_DEX;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_CON)
    {
	if (!(o_ptr->flags2 & TR2_SUST_CON))
	{
	    o_ptr->flags2 |= TR2_SUST_CON;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_CHR)
    {
	if (!(o_ptr->flags2 & TR2_SUST_CHR))
	{
	    o_ptr->flags2 |= TR2_SUST_CHR;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_CHAOS)
    {
	if (!(o_ptr->flags3 & TR3_TELEPORT))
	{
	    o_ptr->flags3 |= TR3_TELEPORT;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_FIRE)
    {
	if (!(o_ptr->flags3 & TR3_LITE))
	{
	    o_ptr->flags3 |= TR3_LITE; /* Freebie */
	}
    }


    switch (randint(31))
    {
    case 1:
	o_ptr->flags2 |= TR2_SUST_STR;
/*  if (is_scroll) msg_print ("It makes you feel you cannot become weaker."); */
	if (!artifact_bias)
	    artifact_bias = BIAS_STR;
	break;
    case 2:
	o_ptr->flags2 |= TR2_SUST_INT;
/*  if (is_scroll) msg_print ("It makes you feel you cannot become more stupid.");*/
	if (!artifact_bias)
	    artifact_bias = BIAS_INT;
	break;
    case 3:
	o_ptr->flags2 |= TR2_SUST_WIS;
/*  if (is_scroll) msg_print ("It makes you feel you cannot become simpler.");*/
	if (!artifact_bias)
	    artifact_bias = BIAS_WIS;
	break;
    case 4:
	o_ptr->flags2 |= TR2_SUST_DEX;
/*  if (is_scroll) msg_print ("It makes you feel you cannot become clumsier.");*/
	if (!artifact_bias)
	    artifact_bias = BIAS_DEX;
	break;
    case 5:
	o_ptr->flags2 |= TR2_SUST_CON;
/*  if (is_scroll) msg_print ("It makes you feel you cannot become less healthy.");*/
	if (!artifact_bias)
	    artifact_bias = BIAS_CON;
	break;
    case 6:
	o_ptr->flags2 |= TR2_SUST_CHR;
/*  if (is_scroll) msg_print ("It makes you feel you cannot become uglier.");*/
	if (!artifact_bias)
	    artifact_bias = BIAS_CHR;
	break;
    case 7: case 8: case 14:
	o_ptr->flags2 |= TR2_FREE_ACT;
/*  if (is_scroll) msg_print ("It makes you feel like a young rebel!");*/
	break;
    case 9:
	o_ptr->flags2 |= TR2_HOLD_LIFE;
/*  if (is_scroll) msg_print ("It makes you feel immortal.");*/
	if (!artifact_bias && (randint(5)==1))
	    artifact_bias = BIAS_PRIESTLY;
	else if (!artifact_bias && (randint(6)==1))
	    artifact_bias = BIAS_NECROMANTIC;
	break;
    case 10: case 11:
	o_ptr->flags3 |= TR3_LITE;
/*  if (is_scroll) msg_print ("It starts shining.");*/
	break;
    case 12: case 13:
	o_ptr->flags3 |= TR3_FEATHER;
/*  if (is_scroll) msg_print ("It feels lighter.");*/
	break;
    case 15: case 16: case 17:
	o_ptr->flags3 |= TR3_SEE_INVIS;
/*  if (is_scroll) msg_print ("It makes you see the air!");*/
	break;
    case 18:
	o_ptr->flags3 |= TR3_TELEPATHY;
/*  if (is_scroll) msg_print ("It makes you hear voices inside your head!");*/
	if (!artifact_bias && (randint(9)==1))
	    artifact_bias = BIAS_MAGE;
	break;
    case 19: case 20:
	o_ptr->flags3 |= TR3_SLOW_DIGEST;
/*  if (is_scroll) msg_print ("It makes you feel less hungry.");*/
	break;
    case 21: case 22:
	o_ptr->flags3 |= TR3_REGEN;
/*  if (is_scroll) msg_print ("It looks as good as new.");*/
	break;
    case 23:
	o_ptr->flags3 |= TR3_TELEPORT;
/*  if (is_scroll) msg_print ("Its position feels uncertain!");*/
	break;
    case 24: case 25: case 26:
	if (o_ptr->tval>=TV_BOOTS) random_misc(o_ptr, is_scroll);
	else
	{
		o_ptr->flags3 |= TR3_SHOW_MODS;
		o_ptr->to_a = 4 + (randint(11));
	}
	break;
    case 27: case 28: case 29:
	o_ptr->flags3 |= TR3_SHOW_MODS;
	o_ptr->to_h += 4 + (randint(11));
	o_ptr->to_d += 4 + (randint(11));
	break;
    case 30:
        o_ptr->flags3 |= TR3_NO_MAGIC;
        break;
    case 31:
        o_ptr->flags3 |= TR3_NO_TELE;
        break;
    }


}

static void random_slay (object_type * o_ptr, bool is_scroll)
{

    if (artifact_bias == BIAS_CHAOS && !(o_ptr->tval == TV_BOW))
    {
	if (!(o_ptr->flags1 & TR1_CHAOTIC))
	{
	    o_ptr->flags1 |= TR1_CHAOTIC;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_PRIESTLY &&
	    (o_ptr->tval == TV_SWORD || o_ptr->tval == TV_POLEARM) &&
	    !(o_ptr->flags3 & TR3_BLESSED))

    {
	o_ptr->flags3 |= TR3_BLESSED; /* A free power for "priestly"
					     random artifacts */
    }


    else if (artifact_bias == BIAS_NECROMANTIC && !(o_ptr->tval == TV_BOW))
    {
	if (!(o_ptr->flags1 & TR1_VAMPIRIC))
	{
	    o_ptr->flags1 |= TR1_VAMPIRIC;
	    if (randint(2)==1) return;
	}
	if (!(o_ptr->flags1 & TR1_BRAND_POIS) && (randint(2)==1))
	{
	    o_ptr->flags1 |= TR1_BRAND_POIS;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_RANGER && !(o_ptr->tval == TV_BOW))
    {
	if (!(o_ptr->flags1 & TR1_SLAY_ANIMAL))
	{
	    o_ptr->flags1 |= TR1_SLAY_ANIMAL;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_ROGUE && !(o_ptr->tval == TV_BOW))
    {
	if (!(o_ptr->flags1 & TR1_BRAND_POIS))
	{
	    o_ptr->flags1 |= TR1_BRAND_POIS;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_POIS && !(o_ptr->tval == TV_BOW))
    {
	if (!(o_ptr->flags1 & TR1_BRAND_POIS))
	{
	    o_ptr->flags1 |= TR1_BRAND_POIS;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_FIRE && !(o_ptr->tval == TV_BOW))
    {
	if (!(o_ptr->flags1 & TR1_BRAND_FIRE))
	{
	    o_ptr->flags1 |= TR1_BRAND_FIRE;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_COLD && !(o_ptr->tval == TV_BOW))
    {
	if (!(o_ptr->flags1 & TR1_BRAND_COLD))
	{
	    o_ptr->flags1 |= TR1_BRAND_COLD;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_ELEC && !(o_ptr->tval == TV_BOW))
    {
	if (!(o_ptr->flags1 & TR1_BRAND_ELEC))
	{
	    o_ptr->flags1 |= TR1_BRAND_ELEC;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_ACID && !(o_ptr->tval == TV_BOW))
    {
	if (!(o_ptr->flags1 & TR1_BRAND_ACID))
	{
	    o_ptr->flags1 |= TR1_BRAND_ACID;
	    if (randint(2)==1) return;
	}
    }
    else if (artifact_bias == BIAS_LAW && !(o_ptr->tval == TV_BOW))
    {
	if (!(o_ptr->flags1 & TR1_SLAY_EVIL))
	{
	    o_ptr->flags1 |= TR1_SLAY_EVIL;
	    if (randint(2)==1) return;
	}
	if (!(o_ptr->flags1 & TR1_SLAY_UNDEAD))
	{
	    o_ptr->flags1 |= TR1_SLAY_UNDEAD;
	    if (randint(2)==1) return;
	}
	if (!(o_ptr->flags1 & TR1_SLAY_DEMON))
	{
	    o_ptr->flags1 |= TR1_SLAY_DEMON;
	    if (randint(2)==1) return;
	}
    }

  if (!(o_ptr->tval == TV_BOW))
  {
    switch (randint(34))
    {
    case 1: case 2:
	o_ptr->flags1 |= TR1_SLAY_ANIMAL;
/*  if (is_scroll) msg_print ("You start hating animals.");*/
	break;
    case 3: case 4:
	o_ptr->flags1 |= TR1_SLAY_EVIL;
/*  if (is_scroll) msg_print ("You hate evil creatures.");*/
	if (!artifact_bias && (randint(2)==1))
	    artifact_bias = BIAS_LAW;
	else if (!artifact_bias && (randint(9)==1))
	    artifact_bias = BIAS_PRIESTLY;
	break;
    case 5: case 6:
	o_ptr->flags1 |= TR1_SLAY_UNDEAD;
/*  if (is_scroll) msg_print ("You hate undead creatures.");*/
	if (!artifact_bias && (randint(9)==1))
	    artifact_bias = BIAS_PRIESTLY;
	break;
    case 7: case 8:
	o_ptr->flags1 |= TR1_SLAY_DEMON;
/*  if (is_scroll) msg_print ("You hate demons.");*/
	if (!artifact_bias && (randint(9)==1))
	    artifact_bias = BIAS_PRIESTLY;
	break;
    case 9: case 10:
	o_ptr->flags1 |= TR1_SLAY_ORC;
/*  if (is_scroll) msg_print ("You hate orcs.");*/
	break;
    case 11: case 12:
	o_ptr->flags1 |= TR1_SLAY_TROLL;
/*  if (is_scroll) msg_print ("You hate trolls.");*/
	break;
    case 13: case 14:
	o_ptr->flags1 |= TR1_SLAY_GIANT;
/*  if (is_scroll) msg_print ("You hate giants.");*/
	break;
    case 15: case 16:
	o_ptr->flags1 |= TR1_SLAY_DRAGON;
/*  if (is_scroll) msg_print ("You hate dragons.");*/
	break;
    case 17: 
	o_ptr->flags1 |= TR1_KILL_DRAGON;
/*  if (is_scroll) msg_print ("You feel an intense hatred of dragons.");*/
	break;
    case 18:  case 19:
	if (o_ptr->tval == TV_SWORD)
	    {   o_ptr->flags1 |= TR1_VORPAL;
/*      if (is_scroll) msg_print ("It looks extremely sharp!");*/
		if (!artifact_bias && (randint(9)==1))
		    artifact_bias = BIAS_WARRIOR;
	    }
	else random_slay(o_ptr, is_scroll);
	break;
    case 20:
	o_ptr->flags1 |= TR1_IMPACT;
/*  if (is_scroll) msg_print ("The ground trembles beneath you.");*/
	break;
    case 21: case 22:
	o_ptr->flags1 |= TR1_BRAND_FIRE;
/*  if (is_scroll) msg_print ("It feels hot!");*/
	if (!artifact_bias)
	    artifact_bias = BIAS_FIRE;
	break;
    case 23: case 24:
	o_ptr->flags1 |= TR1_BRAND_COLD;
/*  if (is_scroll) msg_print ("It feels cold!");*/
	if (!artifact_bias)
	    artifact_bias = BIAS_COLD;
	break;
    case 25: case 26:
	o_ptr->flags1 |= TR1_BRAND_ELEC;
/*  if (is_scroll) msg_print ("Ouch! You get zapped!");*/
	if (!artifact_bias)
	    artifact_bias = BIAS_ELEC;
	break;
    case 27: case 28:
	o_ptr->flags1 |= TR1_BRAND_ACID;
/*  if (is_scroll) msg_print ("Its smell makes you feel dizzy.");*/
	if (!artifact_bias)
	    artifact_bias = BIAS_ACID;
	break;
    case 29: case 30:
	o_ptr->flags1 |= TR1_BRAND_POIS;
/*  if (is_scroll) msg_print ("It smells rotten.");*/
	if (!artifact_bias && (randint(3)!=1))
	    artifact_bias = BIAS_POIS;
	else if (!artifact_bias && randint(6)==1)
	    artifact_bias = BIAS_NECROMANTIC;
	else if (!artifact_bias)
	    artifact_bias = BIAS_ROGUE;
	break;
    case 31: case 32:
	o_ptr->flags1 |= TR1_VAMPIRIC;
/*  if (is_scroll) msg_print ("You think it bit you!");*/
	if (!artifact_bias)
	    artifact_bias = BIAS_NECROMANTIC;
	break;
	default:
	o_ptr->flags1 |= TR1_CHAOTIC;
/*  if (is_scroll) msg_print ("It looks very confusing.");*/
	if (!artifact_bias)
	    artifact_bias = BIAS_CHAOS;
	break;
      }
  }
  else
  {
    switch (randint(6))
    {
	case 1: case 2: case 3:
	o_ptr->flags3 |= TR3_XTRA_MIGHT;
/*  if (is_scroll) msg_print ("It looks mightier than before."); */
	if (!artifact_bias && randint(9)==1)
	    artifact_bias = BIAS_RANGER;
	break;
	default:
	o_ptr->flags3 |= TR3_XTRA_SHOTS;
/*  if (is_scroll) msg_print ("It seems faster!"); */
	if (!artifact_bias && randint(9)==1)
	    artifact_bias = BIAS_RANGER;
	break;
    }

  }

}

static void give_activation_power (object_type * o_ptr)
{

    int type = 0, chance = 0;

    if (artifact_bias)
    {
        if (artifact_bias == BIAS_ELEC)
        {
            if (randint(3)!=1)
            {
                type = ACT_BO_ELEC_1;
            }
            else if (randint(5)!=1)
            {
                type = ACT_BA_ELEC_2;
            }
            else
            {
                type = ACT_BA_ELEC_3;
            }
            chance = 101;
        }
        else if (artifact_bias == BIAS_POIS)
        {
            type = ACT_BA_POIS_1;
            chance = 101;
        }
        else if (artifact_bias == BIAS_FIRE)
        {
            if (randint(3)!=1)
            {
                type = ACT_BO_FIRE_1;
            }
            else if (randint(5)!=1)
            {
                type = ACT_BA_FIRE_1;
            }
            else
            {
                type = ACT_BA_FIRE_2;
            }
            chance = 101;
        }
        else if (artifact_bias == BIAS_COLD)
        {
            chance = 101;
            if (randint(3)!=1)
                type = ACT_BO_COLD_1;
            else if (randint(3)!=1)
                type = ACT_BA_COLD_1;
            else if (randint(3)!=1)
                type = ACT_BA_COLD_2;
            else
                type = ACT_BA_COLD_3;
        }
        else if (artifact_bias == BIAS_CHAOS)
        {
            chance = 50;
            if (randint(6)==1)
                type = ACT_SUMMON_DEMON;
            else
                type = ACT_CALL_CHAOS;
        }
        else if (artifact_bias == BIAS_PRIESTLY)
        {
            chance = 101;

            if (randint(13)==1)
                type = ACT_CHARM_UNDEAD;
            else if (randint(12)==1)
                type = ACT_BANISH_EVIL;
            else if (randint(11)==1)
                type = ACT_DISP_EVIL;
            else if (randint(10)==1)
                type = ACT_PROT_EVIL;
            else if (randint(9)==1)
                type = ACT_CURE_1000;
            else if (randint(8)==1)
                type = ACT_CURE_700;
            else if (randint(7)==1)
                type = ACT_REST_ALL;
            else if (randint(6)==1)
                type = ACT_REST_LIFE;
            else
                type = ACT_CURE_MW;
        }
        else if (artifact_bias == BIAS_NECROMANTIC)
        {
            chance = 101;
            if (randint(66)==1)
                type = ACT_WRAITH;
            else if (randint(13)==1)
                type = ACT_DISP_GOOD;
            else if (randint(9)==1)
                type = ACT_MASS_GENO;
            else if (randint(8)==1)
                type = ACT_GENOCIDE;
            else if (randint(13)==1)
                type = ACT_SUMMON_UNDEAD;
            else if (randint(9)==1)
                type = ACT_VAMPIRE_2;
            else if (randint(6)==1)
                type = ACT_CHARM_UNDEAD;
            else
                type = ACT_VAMPIRE_1;
        }
        else if (artifact_bias == BIAS_LAW)
        {
            chance = 101;
            if (randint(8)==1)
                type = ACT_BANISH_EVIL;
            else if (randint(4)==1)
                type = ACT_DISP_EVIL;
            else
                type = ACT_PROT_EVIL;
        }
        else if (artifact_bias == BIAS_ROGUE)
        {
            chance = 101;
            if (randint(50)==1)
                type = ACT_SPEED;
            else if (randint(4)==1)
                type = ACT_SLEEP;
            else if (randint(3)==1)
                type = ACT_DETECT_ALL;
            else if (randint(8)==1)
                type = ACT_ID_FULL;
            else
                type = ACT_ID_PLAIN;
        }
        else if (artifact_bias == BIAS_MAGE)
        {
            chance = 66;
            if (randint(20)==1)
                type = SUMMON_ELEMENTAL;
            else if (randint(10)==1)
                type = SUMMON_PHANTOM;
            else if (randint(5)==1)
                type = ACT_RUNE_EXPLO;
            else
                type = ACT_ESP;
        }
        else if (artifact_bias == BIAS_WARRIOR)
        {
            chance = 80;
                if (randint(100)==1)
                    type = ACT_INVULN;
                else
                    type = ACT_BERSERK;
        }
        else if (artifact_bias == BIAS_RANGER)
        {
            chance = 101;
            if (randint(20)==1)
                type = ACT_CHARM_ANIMALS;
            else if (randint(7)==1)
                type = ACT_SUMMON_ANIMAL;
            else if (randint(6)==1)
                type = ACT_CHARM_ANIMAL;
            else if (randint(4)==1)
                type = ACT_RESIST_ALL;
            else if (randint(3)==1)
                type = ACT_SATIATE;
            else
                type = ACT_CURE_POISON;
        }
    }

    while (!(type) || (randint(100)>=chance))
    {
        type = randint(255);
        switch (type)
        {
            case ACT_SUNLIGHT: case ACT_BO_MISS_1:
            case ACT_BA_POIS_1: case ACT_BO_ELEC_1:
            case ACT_BO_ACID_1: case ACT_BO_COLD_1: case ACT_BO_FIRE_1:
            case ACT_CONFUSE: case ACT_SLEEP: case ACT_QUAKE:
            case ACT_CURE_LW: case ACT_CURE_MW: case ACT_CURE_POISON:
            case ACT_BERSERK: case ACT_LIGHT: case ACT_MAP_LIGHT:
            case ACT_DEST_DOOR: case ACT_STONE_MUD: case ACT_TELEPORT:
                chance = 101;
                break;
            case ACT_BA_COLD_1: case ACT_BA_FIRE_1: case ACT_DRAIN_1:
            case ACT_TELE_AWAY: case ACT_ESP: case ACT_RESIST_ALL:
            case ACT_DETECT_ALL: case ACT_RECALL:
            case ACT_SATIATE: case ACT_RECHARGE:
                chance = 85;
                break;
            case ACT_TERROR: case ACT_PROT_EVIL: case ACT_ID_PLAIN:
                chance = 75;
                break;
            case ACT_DRAIN_2: case ACT_VAMPIRE_1: case ACT_BO_MISS_2:
            case ACT_BA_FIRE_2: case ACT_REST_LIFE:
                chance = 66;
                break;
            case ACT_BA_COLD_3: case ACT_BA_ELEC_3: case ACT_WHIRLWIND:
            case ACT_VAMPIRE_2: case ACT_CHARM_ANIMAL:
                chance = 50;
                break;
            case ACT_SUMMON_ANIMAL:
                chance = 40;
                break;
            case ACT_DISP_EVIL: case ACT_BA_MISS_3: case ACT_DISP_GOOD:
            case ACT_BANISH_EVIL: case ACT_GENOCIDE: case ACT_MASS_GENO:
            case ACT_CHARM_UNDEAD: case ACT_CHARM_OTHER: case ACT_SUMMON_PHANTOM:
            case ACT_REST_ALL:
            case ACT_RUNE_EXPLO:
                chance = 33;
                break;
            case ACT_CALL_CHAOS: case ACT_SHARD:
            case ACT_CHARM_ANIMALS: case ACT_CHARM_OTHERS:
            case ACT_SUMMON_ELEMENTAL: case ACT_CURE_700:
            case ACT_SPEED: case ACT_ID_FULL: case ACT_RUNE_PROT:
                chance = 25;
                break;
            case ACT_CURE_1000: case ACT_XTRA_SPEED:
            case ACT_DETECT_XTRA: case ACT_DIM_DOOR:
                chance = 10;
                break;
            case ACT_SUMMON_UNDEAD: case ACT_SUMMON_DEMON:
            case ACT_WRAITH: case ACT_INVULN: case ACT_ALCHEMY:
                chance = 5;
                break;
            default:
                chance = 0;
        }
    }

    /* A type was chosen... */
	o_ptr->xtra1 = EGO_XTRA_ACTIVATE;
    o_ptr->xtra2 = type;
    o_ptr->flags3 |= TR3_ACTIVATE;
    o_ptr->timeout = 0;
}

static void get_random_name(char * return_name, bool armour, int power)
{
    if (randint(100)<=TABLE_NAME)
	get_table_name(return_name);
    else
    {
    char NameFile[16];
    switch (armour)
    {
	case 1:
	    switch(power)
	    {
	case 0:
	strcpy(NameFile, "a_cursed.txt");
	break;
		case 1:
		strcpy(NameFile, "a_low.txt");
		break;
		case 2:
		strcpy(NameFile, "a_med.txt");
		break;
		default:
		strcpy(NameFile, "a_high.txt");
	    }
	break;
	default:
	    switch(power)
	    {
	case 0:
	strcpy(NameFile, "w_cursed.txt");
	break;
		case 1:
		strcpy(NameFile, "w_low.txt");
		break;
		case 2:
		strcpy(NameFile, "w_med.txt");
		break;
		default:
		strcpy(NameFile, "w_high.txt");
	    }

    }

    get_rnd_line(NameFile, return_name);
    }
}

bool create_artifact(object_type *o_ptr, bool a_scroll)
{

char new_name[80];
int has_pval = 0;
int powers = randint(5) + 1;
int max_type = (o_ptr->tval<TV_BOOTS?7:5);
int power_level;
s32b total_flags;
bool a_cursed = FALSE;

int warrior_artifact_bias = 0;

artifact_bias = 0;

if (a_scroll && (randint(4)==1))
{
    switch (p_ptr->ptemplate)
    {
	case TPL_SWASHBUCKLER:
	case TPL_GLADIATOR: case TPL_WARRIOR_MONK:
	    artifact_bias = BIAS_WARRIOR;
	    break;
    case TPL_MAGE: case TPL_WARLOCK: case TPL_POWERWEAVER:
	    artifact_bias = BIAS_MAGE;
	    break;
	case TPL_SHAMAN:
	    artifact_bias = BIAS_PRIESTLY;
	    break;
	case TPL_ASSASSIN: case TPL_TOURIST: case TPL_ADVENTURER:
	    artifact_bias = BIAS_ROGUE;
	    warrior_artifact_bias = 25;
	    break;
	case TPL_RANGER:
	    artifact_bias = BIAS_RANGER;
	    warrior_artifact_bias = 30;
	    break;
    case TPL_MINDCRAFTER: case TPL_ZEN_MONK:
        if (randint(5)>2) artifact_bias = BIAS_PRIESTLY;
        break;
    }
}

if ((randint(100) <= warrior_artifact_bias) && a_scroll) artifact_bias = BIAS_WARRIOR;

strcpy(new_name,"");

if ((!a_scroll) && (randint(A_CURSED)==1)) a_cursed = TRUE;

while ((randint(powers) == 1) || (randint(7)==1) || randint(10)==1) powers++;

if ((!a_cursed) && (randint(WEIRD_LUCK)==1)) powers *= 2;

if (a_cursed) powers /= 2;

/* Main loop */

while(powers--)
{
  switch (randint(max_type))
	{
	case 1: case 2:
		random_plus(o_ptr, a_scroll);
		has_pval = TRUE;
		break;
	case 3: case 4:
	random_resistance(o_ptr, a_scroll, FALSE);
		break;
	case 5:
		random_misc(o_ptr, a_scroll);
		break;
	case 6: case 7:
		random_slay(o_ptr, a_scroll);
		break;
	default:
		if(cheat_wzrd) msg_print ("Switch error in create_artifact!");
		powers++;
	}
};

if (has_pval)
    {
	if (o_ptr->flags1 & TR1_BLOWS)
	    o_ptr->pval = randint(2) + 1;
	else
	{
	    do
	    { o_ptr->pval++; }
	    while (o_ptr->pval<randint(5) || randint(o_ptr->pval)==1);
	}
	if (o_ptr->pval > 4 && (randint(WEIRD_LUCK)!=1))
	    o_ptr->pval = 4;
    }

/* give it some plusses... */
    if (o_ptr->tval>=TV_BOOTS)
	o_ptr->to_a += randint(o_ptr->to_a>19?1:20-o_ptr->to_a);
    else
	{
	    o_ptr->to_h += randint(o_ptr->to_h>19?1:20-o_ptr->to_h);
	    o_ptr->to_d += randint(o_ptr->to_d>19?1:20-o_ptr->to_d);
	}
    o_ptr->flags3 |= (TR3_IGNORE_ALL); /* Just to be sure */
    total_flags = flag_cost(o_ptr, TRUE);
    if (cheat_peek) msg_format("%ld", total_flags);

if (a_cursed) curse_artifact(o_ptr);

if ((!a_cursed)
    && (randint((o_ptr->tval>=TV_BOOTS)
        ?ACTIVATION_CHANCE * 2:ACTIVATION_CHANCE)==1))
{
    o_ptr->xtra2 = 0;
    give_activation_power(o_ptr);
}


if(o_ptr->tval>=TV_BOOTS)
{
    if (a_cursed) power_level = 0;
    else if (total_flags<10000) power_level = 1;
    else if (total_flags<20000) power_level = 2;
    else power_level = 3;
}

else
{
    if (a_cursed) power_level = 0;
    else if (total_flags<15000) power_level = 1;
    else if (total_flags<30000) power_level = 2;
    else power_level = 3;
}

if (a_scroll)
    {
         char dummy_name[80];
         strcpy(dummy_name, "");
         identify_fully_aux(o_ptr, FALSE);
         o_ptr->ident |= IDENT_STOREB; /* This will be used later on... */
         if (!(get_string("What do you want to call the artifact? ", dummy_name, 80)))
        strcpy(new_name,"(a DIY Artifact)");
         else
        {
           strcpy(new_name,"called '");
           strcat(new_name,dummy_name);
           strcat(new_name,"'");
        }
        /* Identify it fully */
        object_aware(o_ptr);
        object_known(o_ptr);

        /* Mark the item as fully known */
        o_ptr->ident |= (IDENT_MENTAL);

     }

else

    get_random_name(new_name, (bool)(o_ptr->tval >= TV_BOOTS), power_level);

    if (cheat_xtra)
    {
	if (artifact_bias)
	    msg_format("Biased artifact: %d.", artifact_bias);
	else
	    msg_print("No bias in artifact.");
    }

       /* Save the inscription */
	o_ptr->art_name = quark_add(new_name);

	/* Window stuff */
		p_ptr->window |= (PW_INVEN | PW_EQUIP);
    return TRUE;

}


bool artifact_scroll(void)
{
	int                     item;
	bool            okay = FALSE;

	object_type             *o_ptr;

	C_TNEW(o_name, ONAME_MAX, char);


	/* Assume enchant weapon */
	item_tester_hook = item_tester_hook_weapon;

	/* Get an item (from equip or inven or floor) */
	if (!get_item(&item, "Enchant which item? ", TRUE, TRUE, TRUE))
	{
		if (item == -2) msg_print("You have nothing to enchant.");
		TFREE(o_name);
		return (FALSE);
	}

	/* Get the item (in the pack) */
	if (item >= 0)
	{
		o_ptr = &inventory[item];
	}

	/* Get the item (on the floor) */
	else
	{
		o_ptr = &o_list[0 - item];
	}


	/* Description */
	object_desc(o_name, o_ptr, FALSE, 0);

	/* Describe */
    msg_format("%s %s radiate%s a blinding light!",
		   ((item >= 0) ? "Your" : "The"), o_name,
		   ((o_ptr->number > 1) ? "" : "s"));

    if (o_ptr->name1 || o_ptr->art_name)
    {
	msg_format("The %s %s already %s!",
	       o_name, ((o_ptr->number > 1) ? "are" : "is"),
	       ((o_ptr->number > 1) ? "artifacts" : "an artifact"));
	okay = FALSE;
	    }

    else   if (o_ptr->name2)
    {
	msg_format("The %s %s already %s!",
	       o_name, ((o_ptr->number > 1) ? "are" : "is"),
	       ((o_ptr->number > 1) ? "ego items" : "an ego item"));
	okay = FALSE;
    }

    else
    {
	if (o_ptr->number > 1)
	    {
		msg_print("Not enough enough energy to enchant more than one object!");
	msg_format("%d of your %s %s destroyed!",(o_ptr->number)-1, o_name, (o_ptr->number>2?"were":"was"));
		o_ptr->number = 1;
	     }
	   okay = create_artifact(o_ptr, TRUE);
    }

	/* Failure */
	if (!okay)
	{
		/* Flush */
		if (flush_failure) flush();

		/* Message */
		msg_print("The enchantment failed.");
	}

	TFREE(o_name);
	/* Something happened */
	return (TRUE);
}


/*
 * Identify an object in the inventory (or on the floor)
 * This routine does *not* automatically combine objects.
 * Returns TRUE if something was identified, else FALSE.
 */
bool ident_spell(void)
{
	int                     item;

	object_type             *o_ptr;

	C_TNEW(o_name, ONAME_MAX, char);


	/* Get an item (from equip or inven or floor) */
	if (!get_item(&item, "Identify which item? ", TRUE, TRUE, TRUE))
	{
		if (item == -2) msg_print("You have nothing to identify.");
		TFREE(o_name);
		return (FALSE);
	}

	/* Get the item (in the pack) */
	if (item >= 0)
	{
		o_ptr = &inventory[item];
	}

	/* Get the item (on the floor) */
	else
	{
		o_ptr = &o_list[0 - item];
	}


	/* Identify it fully */
	object_aware(o_ptr);
	object_known(o_ptr);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Combine / Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	/* Window stuff */
	p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_PLAYER);

	/* Description */
	object_desc(o_name, o_ptr, TRUE, 3);

	/* Describe */
	if (item >= INVEN_WIELD)
	{
		msg_format("%^s: %s (%c).",
			   describe_use(item), o_name, index_to_label(item));
	}
	else if (item >= 0)
	{
		msg_format("In your pack: %s (%c).",
			   o_name, index_to_label(item));
	}
	else
	{
		msg_format("On the ground: %s.",
			   o_name);
	}

	TFREE(o_name);

	/* Something happened */
	return (TRUE);
}



/*
 * Fully "identify" an object in the inventory  -BEN-
 * This routine returns TRUE if an item was identified.
 */
bool identify_fully(void)
{
	int                     item;

	object_type             *o_ptr;

	C_TNEW(o_name, ONAME_MAX, char);


	/* Get an item (from equip or inven or floor) */
	if (!get_item(&item, "Identify which item? ", TRUE, TRUE, TRUE))
	{
		if (item == -2) msg_print("You have nothing to identify.");
		TFREE(o_name);
		return (FALSE);
	}

	/* Get the item (in the pack) */
	if (item >= 0)
	{
		o_ptr = &inventory[item];
	}

	/* Get the item (on the floor) */
	else
	{
		o_ptr = &o_list[0 - item];
	}


	/* Identify it fully */
	object_aware(o_ptr);
	object_known(o_ptr);

	/* Mark the item as fully known */
	o_ptr->ident |= (IDENT_MENTAL);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Combine / Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	/* Window stuff */
	p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_PLAYER);

	/* Handle stuff */
	handle_stuff();

	/* Description */
	object_desc(o_name, o_ptr, TRUE, 3);

	/* Describe */
	if (item >= INVEN_WIELD)
	{
		msg_format("%^s: %s (%c).",
			   describe_use(item), o_name, index_to_label(item));
	}
	else if (item >= 0)
	{
		msg_format("In your pack: %s (%c).",
			   o_name, index_to_label(item));
	}
	else
	{
		msg_format("On the ground: %s.",
			   o_name);
	}

	/* Describe it fully */
	identify_fully_aux(o_ptr, FALSE);

	TFREE(o_name);

	/* Success */
	return (TRUE);
}




/*
 * Hook for "get_item()".  Determine if something is rechargable.
 */
bool item_tester_hook_recharge(object_type *o_ptr)
{
	/* Recharge staffs */
	if (o_ptr->tval == TV_STAFF) return (TRUE);

	/* Recharge wands */
	if (o_ptr->tval == TV_WAND) return (TRUE);

	/* Hack -- Recharge rods */
	if (o_ptr->tval == TV_ROD) return (TRUE);

	/* Nope */
	return (FALSE);
}


/*
 * Recharge a wand/staff/rod from the pack or on the floor.
 *
 * Scroll of recharging --> recharge(60)
 *
 * recharge(20) = 1/6 failure for empty 10th level wand
 * recharge(60) = 1/10 failure for empty 10th level wand
 *
 * It is harder to recharge high level, and highly charged wands.
 *
 * XXX XXX XXX Beware of "sliding index errors".
 *
 * Should probably not "destroy" over-charged items, unless we
 * "replace" them by, say, a broken stick or some such.  The only
 * reason this is okay is because "scrolls of recharging" appear
 * BEFORE all staffs/wands/rods in the inventory.  Note that the
 * new "auto_sort_pack" option would correctly handle replacing
 * the "broken" wand with any other item (i.e. a broken stick).
 *
 * XXX XXX XXX Perhaps we should auto-unstack recharging stacks.
 */
bool recharge(int num)
{
	int                 i, t, item, lev;

	object_type             *o_ptr;


	/* Only accept legal items */
	item_tester_hook = item_tester_hook_recharge;

	/* Get an item (from inven or floor) */
	if (!get_item(&item, "Recharge which item? ", TRUE, TRUE, TRUE))
	{
		if (item == -2) msg_print("You have nothing to recharge.");
		return (FALSE);
	}

	/* Get the item (in the pack) */
	if (item >= 0)
	{
		o_ptr = &inventory[item];
	}

	/* Get the item (on the floor) */
	else
	{
		o_ptr = &o_list[0 - item];
	}


	/* Extract the object "level" */
	lev = k_info[o_ptr->k_idx].level;

	/* Recharge a rod */
	if (o_ptr->tval == TV_ROD)
	{
		/* Extract a recharge power */
		i = (100 - lev + num) / 5;

		/* Paranoia -- prevent crashes */
		if (i < 1) i = 1;

		/* Back-fire */
		if (rand_int(i) == 0)
		{
			/* Hack -- backfire */
			msg_print("The recharge backfires, draining the rod further!");

			/* Hack -- decharge the rod */
			if (o_ptr->pval < 10000) o_ptr->pval = (o_ptr->pval + 100) * 2;
		}

		/* Recharge */
		else
		{
			/* Rechange amount */
			t = (num * damroll(2, 4));

			/* Recharge by that amount */
			if (o_ptr->pval > t)
			{
				o_ptr->pval -= t;
			}

			/* Fully recharged */
			else
			{
				o_ptr->pval = 0;
			}
		}
	}

	/* Recharge wand/staff */
	else
	{
		/* Recharge power */
		i = (num + 100 - lev - (10 * o_ptr->pval)) / 15;

		/* Paranoia -- prevent crashes */
		if (i < 1) i = 1;

		/* Back-fire XXX XXX XXX */
		if (rand_int(i) == 0)
		{
			/* Dangerous Hack -- Destroy the item */
			msg_print("There is a bright flash of light.");

			/* Reduce and describe inventory */
			if (item >= 0)
			{
				inven_item_increase(item, -999);
				inven_item_describe(item);
				inven_item_optimize(item);
			}

			/* Reduce and describe floor item */
			else
			{
				floor_item_increase(0 - item, -999);
				floor_item_describe(0 - item);
				floor_item_optimize(0 - item);
			}
		}

		/* Recharge */
		else
		{
			/* Extract a "power" */
			t = (num / (lev + 2)) + 1;

			/* Recharge based on the power */
			if (t > 0) o_ptr->pval += 2 + randint(t);

			/* Hack -- we no longer "know" the item */
			o_ptr->ident &= ~(IDENT_KNOWN);

			/* Hack -- we no longer think the item is empty */
			o_ptr->ident &= ~(IDENT_EMPTY);
		}
	}

	/* Combine / Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	/* Window stuff */
	p_ptr->window |= (PW_INVEN);

	/* Something was done */
	return (TRUE);
}








/*
 * Apply a "project()" directly to all viewable monsters
 *
 * Note that affected monsters are NOT auto-tracked by this usage.
 */
static bool project_hack(int typ, int dam)
{
	int             i, x, y;
	
	int             flg = PROJECT_JUMP | PROJECT_KILL | PROJECT_HIDE;

	bool    obvious = FALSE;


	/* Affect all (nearby) monsters */
	for (i = 1; i < m_max; i++)
	{
		monster_type *m_ptr = &m_list[i];

		/* Paranoia -- Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Location */
		y = m_ptr->fy;
		x = m_ptr->fx;

		/* Require line of sight */
		if (!player_has_los_bold(y, x)) continue;

		/* Jump directly to the target monster */
		if (project(0, 0, y, x, dam, typ, flg)) obvious = TRUE;
	}

	/* Result */
	return (obvious);
}


/*
 * Speed monsters
 */
bool speed_monsters(void)
{
	return (project_hack(GF_OLD_SPEED, (skill_set[SKILL_DEVICE].value/2)));
}

/*
 * Slow monsters
 */
bool slow_monsters(int dam)
{
	return (project_hack(GF_OLD_SLOW, dam));
}

/*
 * Sleep monsters
 */
bool sleep_monsters(int dam)
{
	return (project_hack(GF_OLD_SLEEP, dam));
}


/*
 * Banish evil monsters
 */
bool banish_evil(int dist)
{
	return (project_hack(GF_AWAY_EVIL, dist));
}


/*
 * Turn undead
 */
#if 0
static bool turn_undead(int dam)
{
	return (project_hack(GF_TURN_UNDEAD, dam));
}
#endif


/*
 * Dispel undead monsters
 */
bool dispel_undead(int dam)
{
	return (project_hack(GF_DISP_UNDEAD, dam));
}

/*
 * Dispel evil monsters
 */
bool dispel_evil(int dam)
{
	return (project_hack(GF_DISP_EVIL, dam));
}

/*
 * Dispel good monsters
 */
bool dispel_good(int dam)
{
    return (project_hack(GF_DISP_GOOD, dam));
}

/*
 * Dispel all monsters
 */
bool dispel_monsters(int dam)
{
	return (project_hack(GF_DISP_ALL, dam));
}

/*
 * Dispel 'living' monsters
 */
bool dispel_living(int dam)
{
    return (project_hack(GF_DISP_LIVING, dam));
}

/*
 * Dispel demons
 */
bool dispel_demons(int dam)
{
    return (project_hack(GF_DISP_DEMON, dam));
}


/*
 * Wake up all monsters, and speed up "los" monsters.
 */
void aggravate_monsters(int who)
{
	int i;

	bool sleep = FALSE;
	bool speed = FALSE;

	/* Aggravate everyone nearby */
	for (i = 1; i < m_max; i++)
	{
		monster_type    *m_ptr = &m_list[i];
		monster_race    *r_ptr = &r_info[m_ptr->r_idx];

		/* Paranoia -- Skip dead monsters */
		if (!m_ptr->r_idx) continue;

        /* Skip aggravating monster (or player) */
		if (i == who) continue;

		/* Wake up nearby sleeping monsters */
		if (m_ptr->cdis < MAX_SIGHT * 2)
		{
			/* Wake up */
			if (m_ptr->csleep)
			{
				/* Wake up */
				m_ptr->csleep = 0;
				sleep = TRUE;
			}
		}

		/* Speed up monsters in line of sight */
		if (player_has_los_bold(m_ptr->fy, m_ptr->fx))
		{
			/* Speed up (instantly) to racial base + 10 */
			if (m_ptr->mspeed < r_ptr->speed + 10)
			{
				/* Speed up */
				m_ptr->mspeed = r_ptr->speed + 10;
				speed = TRUE;
			}
            if (m_ptr->smart & SM_ALLY)
            {
                if (randint(2)==1)
                {
                    m_ptr->smart &= ~SM_ALLY;
                }
            }
		}
	}

	/* Messages */
	if (speed) msg_print("You feel a sudden stirring nearby!");
	else if (sleep) msg_print("You hear a sudden stirring in the distance!");
}



/*
 * Delete all non-unique monsters of a given "type" from the level
 */
bool genocide(bool player_cast)
{
	int             i;

	char    typ;

	bool    result = FALSE;

	int             msec = delay_factor * delay_factor * delay_factor;

	/* Mega-Hack -- Get a monster symbol */
	(void)(get_com("Choose a monster race (by symbol) to genocide: ", &typ));

	/* Delete the monsters of that "type" */
	for (i = 1; i < m_max; i++)
	{
		monster_type    *m_ptr = &m_list[i];
		monster_race    *r_ptr = &r_info[m_ptr->r_idx];

		/* Paranoia -- Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Hack -- Skip Unique Monsters */
		if (r_ptr->flags1 & (RF1_UNIQUE)) continue;

		/* Skip "wrong" monsters */
		if (r_ptr->d_char != typ) continue;

		/* Skip Quest Monsters - Dean Anderson */
		if ((r_ptr->flags1 & RF1_GUARDIAN) || (r_ptr->flags1 & RF1_ALWAYS_GUARD)) continue;

		/* Delete the monster */
		delete_monster_idx(i,TRUE);

		if (player_cast)
		{
			/* Take damage */
			take_hit(randint(4), "the strain of casting Genocide");
		}

		/* Visual feedback */
		move_cursor_relative(py, px);

		/* Redraw */
		p_ptr->redraw |= (PR_HP);

		/* Window stuff */
		p_ptr->window |= (PW_PLAYER);

		/* Handle */
		handle_stuff();

		/* Fresh */
		Term_fresh();

		/* Delay */
		Term_xtra(TERM_XTRA_DELAY, msec);

		/* Take note */
		result = TRUE;
	}

	return (result);
}


/*
 * Delete all nearby (non-unique) monsters
 */
bool mass_genocide(bool player_cast)
{
	int             i;

	bool    result = FALSE;

	int             msec = delay_factor * delay_factor * delay_factor;


	/* Delete the (nearby) monsters */
	for (i = 1; i < m_max; i++)
	{
		monster_type    *m_ptr = &m_list[i];
		monster_race    *r_ptr = &r_info[m_ptr->r_idx];

		/* Paranoia -- Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Hack -- Skip unique monsters */
		if (r_ptr->flags1 & (RF1_UNIQUE)) continue;

		/* Skip Quest Monsters - Dean Anderson */
		if ((r_ptr->flags1 & RF1_GUARDIAN) || (r_ptr->flags1 & RF1_ALWAYS_GUARD)) continue;

		/* Skip distant monsters */
		if (m_ptr->cdis > MAX_SIGHT) continue;

		/* Delete the monster */
		delete_monster_idx(i,TRUE);

		if (player_cast)
		{
			/* Hack -- visual feedback */
			take_hit(randint(3), "the strain of casting Mass Genocide");
		}

		move_cursor_relative(py, px);

		/* Redraw */
		p_ptr->redraw |= (PR_HP);

		/* Window stuff */
		p_ptr->window |= (PW_PLAYER);

		/* Handle */
		handle_stuff();

		/* Fresh */
		Term_fresh();

		/* Delay */
		Term_xtra(TERM_XTRA_DELAY, msec);

		/* Note effect */
		result = TRUE;
	}

	return (result);
}



/*
 * Probe nearby monsters
 */
bool probing(void)
{
	int            i;

	bool    probe = FALSE;


	/* Probe all (nearby) monsters */
	for (i = 1; i < m_max; i++)
	{
		monster_type *m_ptr = &m_list[i];

		/* Paranoia -- Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Require line of sight */
		if (!player_has_los_bold(m_ptr->fy, m_ptr->fx)) continue;

		/* Probe visible monsters */
		if (m_ptr->ml)
		{
			C_TNEW(m_name, MNAME_MAX, char);

			/* Start the message */
			if (!probe) msg_print("Probing...");

			/* Get "the monster" or "something" */
			monster_desc(m_name, m_ptr, 0x04);

			/* Describe the monster */
			msg_format("%^s has %d hit points.", m_name, m_ptr->hp);

			/* Learn all of the non-spell, non-treasure flags */
			lore_do_probe(i);

			/* Probe worked */
			probe = TRUE;

			TFREE(m_name);
		}
	}

	/* Done */
	if (probe)
	{
		msg_print("That's all.");
	}

	/* Result */
	return (probe);
}



/*
 * The spell of destruction
 *
 * This spell "deletes" monsters (instead of "killing" them).
 *
 * Later we may use one function for both "destruction" and
 * "earthquake" by using the "full" to select "destruction".
 */
void destroy_area(int y1, int x1, int r, bool full)
{
	int y, x, k, t;

	cave_type *c_ptr;

	bool flag = FALSE;


	/* XXX XXX */
	full = full ? full : 0;

	/* Big area of affect */
	for (y = (y1 - r); y <= (y1 + r); y++)
	{
		for (x = (x1 - r); x <= (x1 + r); x++)
		{
			/* Skip illegal grids */
			if (!in_bounds(y, x)) continue;

			/* Extract the distance */
			k = distance(y1, x1, y, x);

			/* Stay in the circle of death */
			if (k > r) continue;

			/* Access the grid */
			c_ptr = &cave[y][x];

			/* Lose room and vault */
			c_ptr->info &= ~(CAVE_ROOM | CAVE_ICKY);

			/* Lose light and knowledge */
			c_ptr->info &= ~(CAVE_MARK | CAVE_GLOW | CAVE_TRAP);

			/* Hack -- Notice player affect */
			if ((x == px) && (y == py))
			{
				/* Hurt the player later */
				flag = TRUE;

				/* Do not hurt this grid */
				continue;
			}

			/* Hack -- Skip the epicenter */
			if ((y == y1) && (x == x1)) continue;

			/* Delete the monster (if any) */
			delete_monster(y, x);

			/* Destroy "valid" grids */
			if (cave_valid_bold(y, x))
			{
				/* Delete objects */
				delete_object(y, x);

				/* Wall (or floor) type */
				t = rand_int(200);

				/* Granite */
				if (t < 20)
				{
					/* Create granite wall */
					c_ptr->feat = FEAT_WALL_EXTRA;
				}

				/* Quartz */
				else if (t < 70)
				{
					/* Create quartz vein */
					c_ptr->feat = FEAT_QUARTZ;
				}

				/* Magma */
				else if (t < 100)
				{
					/* Create magma vein */
					c_ptr->feat = FEAT_MAGMA;
				}

				/* Floor */
				else
				{
					/* Create floor */
					c_ptr->feat = FEAT_FLOOR;
				}
			}
		}
	}


	/* Hack -- Affect player */
	if (flag)
	{
		/* Message */
		msg_print("There is a searing blast of light!");

		/* Blind the player */
		if (!p_ptr->resist_blind && !p_ptr->resist_lite)
		{
			/* Become blind */
			(void)set_blind(p_ptr->blind + 10 + randint(10));
		}
	}


	/* Mega-Hack -- Forget the view and lite */
	p_ptr->update |= (PU_UN_VIEW | PU_UN_LITE);

	/* Update stuff */
	p_ptr->update |= (PU_VIEW | PU_LITE | PU_FLOW);

	/* Update the monsters */
	p_ptr->update |= (PU_MONSTERS);

	/* Redraw map */
	p_ptr->redraw |= (PR_MAP);

	/* Window stuff */
	p_ptr->window |= (PW_OVERHEAD);
}


/*
 * Induce an "earthquake" of the given radius at the given location.
 *
 * This will turn some walls into floors and some floors into walls.
 *
 * The player will take damage and "jump" into a safe grid if possible,
 * otherwise, he will "tunnel" through the rubble instantaneously.
 *
 * Monsters will take damage, and "jump" into a safe grid if possible,
 * otherwise they will be "buried" in the rubble, disappearing from
 * the level in the same way that they do when genocided.
 *
 * Note that thus the player and monsters (except eaters of walls and
 * passers through walls) will never occupy the same grid as a wall.
 * Note that as of now (2.7.8) no monster may occupy a "wall" grid, even
 * for a single turn, unless that monster can pass_walls or kill_walls.
 * This has allowed massive simplification of the "monster" code.
 */
void earthquake(int cy, int cx, int r)
{
	int             i, t, y, x, yy, xx, dy, dx;

	int             damage = 0;

	int             sn = 0, sy = 0, sx = 0;

	bool    hurt = FALSE;

	cave_type       *c_ptr;

	bool    map[32][32];


	/* Paranoia -- Enforce maximum range */
	if (r > 12) r = 12;

	/* Clear the "maximal blast" area */
	for (y = 0; y < 32; y++)
	{
		for (x = 0; x < 32; x++)
		{
			map[y][x] = FALSE;
		}
	}

	/* Check around the epicenter */
	for (dy = -r; dy <= r; dy++)
	{
		for (dx = -r; dx <= r; dx++)
		{
			/* Extract the location */
			yy = cy + dy;
			xx = cx + dx;

			/* Skip illegal grids */
			if (!in_bounds(yy, xx)) continue;

			/* Skip distant grids */
			if (distance(cy, cx, yy, xx) > r) continue;

			/* Access the grid */
			c_ptr = &cave[yy][xx];

			/* Lose room and vault */
			c_ptr->info &= ~(CAVE_ROOM | CAVE_ICKY);

			/* Lose light and knowledge */
			c_ptr->info &= ~(CAVE_GLOW | CAVE_MARK | CAVE_TRAP);

			/* Skip the epicenter */
			if (!dx && !dy) continue;

			/* Skip most grids */
			if (rand_int(100) < 85) continue;

			/* Damage this grid */
			map[16+yy-cy][16+xx-cx] = TRUE;

			/* Hack -- Take note of player damage */
			if ((yy == py) && (xx == px)) hurt = TRUE;
		}
	}

	/* First, affect the player (if necessary) */
	if (hurt)
	{
		/* Check around the player */
		for (i = 0; i < 8; i++)
		{
			/* Access the location */
			y = py + ddy[i];
			x = px + ddx[i];

			/* Skip non-empty grids */
			if (!cave_empty_bold(y, x)) continue;

			/* Important -- Skip "quake" grids */
			if (map[16+y-cy][16+x-cx]) continue;

			/* Count "safe" grids */
			sn++;

			/* Randomize choice */
			if (rand_int(sn) > 0) continue;

			/* Save the safe location */
			sy = y; sx = x;
		}

		/* Random message */
		switch (randint(3))
		{
			case 1:
			{
				msg_print("The cave ceiling collapses!");
				break;
			}
			case 2:
			{
				msg_print("The cave floor twists in an unnatural way!");
				break;
			}
			default:
			{
				msg_print("The cave quakes!  You are pummeled with debris!");
				break;
			}
		}

		/* Hurt the player a lot */
		if (!sn)
		{
			/* Message and damage */
			msg_print("You are severely crushed!");
			damage = 300;
		}

		/* Destroy the grid, and push the player to safety */
		else
		{
			/* Calculate results */
			switch (randint(3))
			{
				case 1:
				{
					msg_print("You nimbly dodge the blast!");
					damage = 0;
					break;
				}
				case 2:
				{
					msg_print("You are bashed by rubble!");
					damage = damroll(10, 4);
					(void)set_stun(p_ptr->stun + randint(50));
					break;
				}
				case 3:
				{
					msg_print("You are crushed between the floor and ceiling!");
					damage = damroll(10, 4);
					(void)set_stun(p_ptr->stun + randint(50));
					break;
				}
			}

			/* Move the player to the safe location */
			move_to(sy, sx);
		}

		/* Important -- no wall on player */
		map[16+py-cy][16+px-cx] = FALSE;

		/* Take some damage */
		if (damage) take_hit(damage, "an earthquake");
	}


	/* Examine the quaked region */
	for (dy = -r; dy <= r; dy++)
	{
		for (dx = -r; dx <= r; dx++)
		{
			/* Extract the location */
			yy = cy + dy;
			xx = cx + dx;

			/* Skip unaffected grids */
			if (!map[16+yy-cy][16+xx-cx]) continue;

			/* Access the grid */
			c_ptr = &cave[yy][xx];

			/* Process monsters */
			if (c_ptr->m_idx)
			{
				monster_type *m_ptr = &m_list[c_ptr->m_idx];
				monster_race *r_ptr = &r_info[m_ptr->r_idx];

				/* Most monsters cannot co-exist with rock */
				if (!(r_ptr->flags2 & (RF2_KILL_WALL)) &&
				    !(r_ptr->flags2 & (RF2_PASS_WALL)))
				{
					C_TNEW(m_name, MNAME_MAX, char);

					/* Assume not safe */
					sn = 0;

					/* Monster can move to escape the wall */
					if (!(r_ptr->flags1 & (RF1_NEVER_MOVE)))
					{
						/* Look for safety */
						for (i = 0; i < 8; i++)
						{
							/* Access the grid */
							y = yy + ddy[i];
							x = xx + ddx[i];

							/* Skip non-empty grids */
							if (!cave_empty_bold(y, x) || (cave[y][x].feat == FEAT_WATER)) continue;

							/* Hack -- no safety on glyph of warding */
							if (cave[y][x].feat == FEAT_GLYPH) continue;
                if (cave[y][x].feat == FEAT_MINOR_GLYPH) continue;

                            /* ... nor on the Pattern */
                            if ((cave[y][x].feat <= FEAT_PATTERN_XTRA2) &&
                                (cave[y][x].feat >= FEAT_PATTERN_START))
                                    continue;

							/* Important -- Skip "quake" grids */
							if (map[16+y-cy][16+x-cx]) continue;

							/* Count "safe" grids */
							sn++;

							/* Randomize choice */
							if (rand_int(sn) > 0) continue;

							/* Save the safe grid */
							sy = y; sx = x;
						}
					}

					/* Describe the monster */
					monster_desc(m_name, m_ptr, 0);

					/* Scream in pain */
					msg_format("%^s wails out in pain!", m_name);

					/* Take damage from the quake */
					damage = (sn ? damroll(4, 8) : 200);

					/* Monster is certainly awake */
					m_ptr->csleep = 0;

					/* Apply damage directly */
					m_ptr->hp -= damage;

					/* Delete (not kill) "dead" monsters */
					if (m_ptr->hp < 0)
					{
						/* Message */
						msg_format("%^s is embedded in the rock!", m_name);

						/* Delete the monster */
						delete_monster(yy, xx);

						/* No longer safe */
						sn = 0;
					}

					TFREE(m_name);

					/* Hack -- Escape from the rock */
					if (sn)
					{
						int m_idx = cave[yy][xx].m_idx;

						/* Update the new location */
						cave[sy][sx].m_idx = m_idx;

						/* Update the old location */
						cave[yy][xx].m_idx = 0;

						/* Move the monster */
						m_ptr->fy = sy;
						m_ptr->fx = sx;

						/* Update the monster (new location) */
						update_mon(m_idx, TRUE);

						/* Redraw the old grid */
						lite_spot(yy, xx);

						/* Redraw the new grid */
						lite_spot(sy, sx);
					}
				}
			}
		}
	}


	/* Examine the quaked region */
	for (dy = -r; dy <= r; dy++)
	{
		for (dx = -r; dx <= r; dx++)
		{
			/* Extract the location */
			yy = cy + dy;
			xx = cx + dx;

			/* Skip unaffected grids */
			if (!map[16+yy-cy][16+xx-cx]) continue;

			/* Access the cave grid */
			c_ptr = &cave[yy][xx];

			/* Paranoia -- never affect player */
			if ((yy == py) && (xx == px)) continue;

			/* Destroy location (if valid) */
			if (cave_valid_bold(yy, xx))
			{
				bool floor = cave_floor_bold(yy, xx);

				/* Delete objects */
				delete_object(yy, xx);

				/* Wall (or floor) type */
				t = (floor ? rand_int(100) : 200);

				/* Granite */
				if (t < 20)
				{
					/* Create granite wall */
					c_ptr->feat = FEAT_WALL_EXTRA;
				}

				/* Quartz */
				else if (t < 70)
				{
					/* Create quartz vein */
					c_ptr->feat = FEAT_QUARTZ;
				}

				/* Magma */
				else if (t < 100)
				{
					/* Create magma vein */
					c_ptr->feat = FEAT_MAGMA;
				}

				/* Floor */
				else
				{
					/* Create floor */
					c_ptr->feat = FEAT_FLOOR;
				}
			}
		}
	}


	/* Mega-Hack -- Forget the view and lite */
	p_ptr->update |= (PU_UN_VIEW | PU_UN_LITE);

	/* Update stuff */
	p_ptr->update |= (PU_VIEW | PU_LITE | PU_FLOW);

	/* Monster may be able to be created in the floor squares created. */
	full_grid = MAX(full_grid, 2*r);

	/* Update the monsters */
	p_ptr->update |= (PU_DISTANCE);

	/* Update the health bar */
	p_ptr->redraw |= (PR_HEALTH);

	/* Redraw map */
	p_ptr->redraw |= (PR_MAP);

	/* Window stuff */
	p_ptr->window |= (PW_OVERHEAD);
}



/*
 * This routine clears the entire "temp" set.
 *
 * This routine will Perma-Lite all "temp" grids.
 *
 * This routine is used (only) by "lite_room()"
 *
 * Dark grids are illuminated.
 *
 * Also, process all affected monsters.
 *
 * SMART monsters always wake up when illuminated
 * NORMAL monsters wake up 1/4 the time when illuminated
 * STUPID monsters wake up 1/10 the time when illuminated
 */
static void cave_temp_room_lite(void)
{
	int i;

	/* Clear them all */
	for (i = 0; i < temp_n; i++)
	{
		int y = temp_y[i];
		int x = temp_x[i];

		cave_type *c_ptr = &cave[y][x];

		/* No longer in the array */
		c_ptr->info &= ~(CAVE_TEMP);

		/* Update only non-CAVE_GLOW grids */
		/* if (c_ptr->info & (CAVE_GLOW)) continue; */

		/* Perma-Lite */
		c_ptr->info |= (CAVE_GLOW);

		/* Process affected monsters */
		if (c_ptr->m_idx)
		{
			int chance = 25;

			monster_type    *m_ptr = &m_list[c_ptr->m_idx];

			monster_race    *r_ptr = &r_info[m_ptr->r_idx];

			/* Update the monster */
			update_mon(c_ptr->m_idx, FALSE);

			/* Stupid monsters rarely wake up */
			if (r_ptr->flags2 & (RF2_STUPID)) chance = 10;

			/* Smart monsters always wake up */
			if (r_ptr->flags2 & (RF2_SMART)) chance = 100;

			/* Sometimes monsters wake up */
			if (m_ptr->csleep && (rand_int(100) < chance))
			{
				/* Wake up! */
				m_ptr->csleep = 0;

				/* Notice the "waking up" */
				if (m_ptr->ml)
				{
					C_TNEW(m_name, MNAME_MAX, char);

					/* Acquire the monster name */
					monster_desc(m_name, m_ptr, 0);

					/* Dump a message */
					msg_format("%^s wakes up.", m_name);

					TFREE(m_name);
				}
			}
		}

		/* Note */
		note_spot(y, x);

		/* Redraw */
		lite_spot(y, x);
	}

	/* None left */
	temp_n = 0;
}



/*
 * This routine clears the entire "temp" set.
 *
 * This routine will "darken" all "temp" grids.
 *
 * In addition, some of these grids will be "unmarked".
 *
 * This routine is used (only) by "unlite_room()"
 *
 * Also, process all affected monsters
 */
static void cave_temp_room_unlite(void)
{
	int i;

	/* Clear them all */
	for (i = 0; i < temp_n; i++)
	{
		int y = temp_y[i];
		int x = temp_x[i];

		cave_type *c_ptr = &cave[y][x];

		/* No longer in the array */
		c_ptr->info &= ~(CAVE_TEMP);

		/* Darken the grid */
		c_ptr->info &= ~(CAVE_GLOW);

		/* Hack -- Forget "boring" grids */
		if (c_ptr->feat <= FEAT_INVIS)
		{
			/* Forget the grid */
			c_ptr->info &= ~(CAVE_MARK);

			/* Notice */
			note_spot(y, x);
		}

		/* Process affected monsters */
		if (c_ptr->m_idx)
		{
			/* Update the monster */
			update_mon(c_ptr->m_idx, FALSE);
		}

		/* Redraw */
		lite_spot(y, x);
	}

	/* None left */
	temp_n = 0;
}




/*
 * Mark area to be lit by lite_room() with CAVE_TEMP.
 */
static void cave_temp_room_aux(int y, int x)
{
	cave_type *c_ptr = &cave[y][x];

	/* Avoid infinite recursion */
	if (c_ptr->info & (CAVE_TEMP)) return;

	/* Paranoia -- verify space */
	if (temp_n == TEMP_MAX) return;

	/* Hack - don't check the nine squares surrounding the player, as they can
	 * always be lit (this needs at least two exceptions to do anything). */
	if (temp_n > 8)
	{
		int i,t;
		/* Do not "leave" the current room */
		for (i = t = 0; i < 10; i++)
		{
			int y2 = y+ddy[i], x2 = x+ddx[i];
			if (in_bounds(y2, x2) && cave_floor_bold(y2,x2) &&
				(cave[y2][x2].info & CAVE_TEMP)) t++;
		}

		/* At least one other adjacent non-wall square is marked. */
		if (t < 2) return;
	}

	/* Mark the grid as "seen" */
	c_ptr->info |= (CAVE_TEMP);

	/* Add it to the "seen" set */
	temp_y[temp_n] = y;
	temp_x[temp_n] = x;
	temp_n++;
#ifdef TESTING_cave_temp_room_aux
	move_cursor_relative(y,x);
	inkey();
#endif
}




/*
 * Illuminate any room containing the given location.
 */
static void lite_room(int y1, int x1)
{
	int i, x, y;

	/* Add the initial grid */
	cave_temp_room_aux(y1, x1);

	/* While grids are in the queue, add their neighbors */
	for (i = 0; i < temp_n; i++)
	{
		x = temp_x[i], y = temp_y[i];

		/* Walls get lit, but stop light */
		if (!cave_floor_bold(y, x)) continue;

		/* Spread adjacent */
		cave_temp_room_aux(y + 1, x);
		cave_temp_room_aux(y - 1, x);
		cave_temp_room_aux(y, x + 1);
		cave_temp_room_aux(y, x - 1);

		/* Spread diagonal */
		cave_temp_room_aux(y + 1, x + 1);
		cave_temp_room_aux(y - 1, x - 1);
		cave_temp_room_aux(y - 1, x + 1);
		cave_temp_room_aux(y + 1, x - 1);
	}

	/* Now, lite them all up at once */
	cave_temp_room_lite();
}


/*
 * Darken all rooms containing the given location
 */
void unlite_room(int y1, int x1)
{
	int i, x, y;

	/* Add the initial grid */
	cave_temp_room_aux(y1, x1);

	/* Spread, breadth first */
	for (i = 0; i < temp_n; i++)
	{
		x = temp_x[i], y = temp_y[i];

		/* Walls get dark, but stop darkness */
		if (!cave_floor_bold(y, x)) continue;

		/* Spread adjacent */
		cave_temp_room_aux(y + 1, x);
		cave_temp_room_aux(y - 1, x);
		cave_temp_room_aux(y, x + 1);
		cave_temp_room_aux(y, x - 1);

		/* Spread diagonal */
		cave_temp_room_aux(y + 1, x + 1);
		cave_temp_room_aux(y - 1, x - 1);
		cave_temp_room_aux(y - 1, x + 1);
		cave_temp_room_aux(y + 1, x - 1);
	}

	/* Now, darken them all at once */
	cave_temp_room_unlite();
}



/*
 * Hack -- call light around the player
 * Affect all monsters in the projection radius
 */
bool lite_area(int dam, int rad)
{
	int flg = PROJECT_GRID | PROJECT_KILL;

	/* Hack -- Message */
	if (!p_ptr->blind)
	{
		msg_print("You are surrounded by a white light.");
	}

	/* Hook into the "project()" function */
	(void)project(0, rad, py, px, dam, GF_LITE_WEAK, flg);

	/* Lite up the room */
	lite_room(py, px);

	/* Assume seen */
	return (TRUE);
}


/*
 * Hack -- call darkness around the player
 * Affect all monsters in the projection radius
 */
bool unlite_area(int dam, int rad)
{
	int flg = PROJECT_GRID | PROJECT_KILL;

	/* Hack -- Message */
	if (!p_ptr->blind)
	{
		msg_print("Darkness surrounds you.");
	}

	/* Hook into the "project()" function */
    (void)project(0, rad, py, px, dam, GF_DARK_WEAK, flg);

	/* Lite up the room */
	unlite_room(py, px);

	/* Assume seen */
	return (TRUE);
}



/*
 * Cast a ball spell
 * Stop if we hit a monster, act as a "ball"
 * Allow "target" mode to pass over monsters
 * Affect grids, objects, and monsters
 */
bool fire_ball(int typ, int dir, int dam, int rad)
{
	int tx, ty;

	int flg = PROJECT_STOP | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL;

	/* Use the given direction */
	tx = px + 99 * ddx[dir];
	ty = py + 99 * ddy[dir];

	/* Hack -- Use an actual "target" */
	if ((dir == 5) && target_okay())
	{
		flg &= ~(PROJECT_STOP);
		tx = target_col;
		ty = target_row;
	}

	/* Analyze the "dir" and the "target".  Hurt items on floor. */
	return (project(0, rad, ty, tx, dam, typ, flg));
}


/*
 * Hack -- apply a "projection()" in a direction (or at the target)
 */
static bool project_hook(int typ, int dir, int dam, int flg)
{
	int tx, ty;

	/* Pass through the target if needed */
	flg |= (PROJECT_THRU);

	/* Use the given direction */
	tx = px + ddx[dir];
	ty = py + ddy[dir];

	/* Hack -- Use an actual "target" */
	if ((dir == 5) && target_okay())
	{
		tx = target_col;
		ty = target_row;
	}

	/* Analyze the "dir" and the "target", do NOT explode */
	return (project(0, 0, ty, tx, dam, typ, flg));
}


/*
 * Cast a bolt spell
 * Stop if we hit a monster, as a "bolt"
 * Affect monsters (not grids or objects)
 */
bool fire_bolt(int typ, int dir, int dam)
{
	int flg = PROJECT_STOP | PROJECT_KILL;
	return (project_hook(typ, dir, dam, flg));
}

/*
 * Cast a beam spell
 * Pass through monsters, as a "beam"
 * Affect monsters (not grids or objects)
 */
bool fire_beam(int typ, int dir, int dam)
{
	int flg = PROJECT_BEAM | PROJECT_KILL;
	return (project_hook(typ, dir, dam, flg));
}

/*
 * Cast a bolt spell, or rarely, a beam spell
 */
bool fire_bolt_or_beam(int prob, int typ, int dir, int dam)
{
	if (rand_int(100) < prob)
	{
		return (fire_beam(typ, dir, dam));
	}
	else
	{
		return (fire_bolt(typ, dir, dam));
	}
}


/*
 * Some of the old functions
 */

bool lite_line(int dir)
{
	int flg = PROJECT_BEAM | PROJECT_GRID | PROJECT_KILL;
	return (project_hook(GF_LITE_WEAK, dir, damroll(6, 8), flg));
}

bool drain_life(int dir, int dam)
{
	int flg = PROJECT_STOP | PROJECT_KILL;
	return (project_hook(GF_OLD_DRAIN, dir, dam, flg));
}

bool wall_to_mud(int dir)
{
	int flg = PROJECT_BEAM | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL;
	return (project_hook(GF_KILL_WALL, dir, 20 + randint(30), flg));
}

bool wizard_lock(int dir)
{
	int flg = PROJECT_BEAM | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL;
    return (project_hook(GF_JAM_DOOR, dir, 20 + randint(30), flg));
}

bool destroy_door(int dir)
{
	int flg = PROJECT_BEAM | PROJECT_GRID | PROJECT_ITEM;
	return (project_hook(GF_KILL_DOOR, dir, 0, flg));
}

bool disarm_trap(int dir)
{
	int flg = PROJECT_BEAM | PROJECT_GRID | PROJECT_ITEM;
	return (project_hook(GF_KILL_TRAP, dir, 0, flg));
}

bool heal_monster(int dir)
{
	int flg = PROJECT_STOP | PROJECT_KILL;
	return (project_hook(GF_OLD_HEAL, dir, damroll(4, 6), flg));
}

bool speed_monster(int dir,int dam)
{
	int flg = PROJECT_STOP | PROJECT_KILL;
	return (project_hook(GF_OLD_SPEED, dir, dam, flg));
}

bool slow_monster(int dir,int dam)
{
	int flg = PROJECT_STOP | PROJECT_KILL;
	return (project_hook(GF_OLD_SLOW, dir, dam, flg));
}

bool sleep_monster(int dir,int dam)
{
	int flg = PROJECT_STOP | PROJECT_KILL;
	return (project_hook(GF_OLD_SLEEP, dir, dam, flg));
}

bool stasis_monster(int dir,int dam)
{
	int flg = PROJECT_STOP | PROJECT_KILL;
    return (project_hook(GF_STASIS, dir, dam, flg));
}

bool confuse_monster(int dir, int plev)
{
	int flg = PROJECT_STOP | PROJECT_KILL;
	return (project_hook(GF_OLD_CONF, dir, plev, flg));
}

bool stun_monster(int dir, int plev)
{
	int flg = PROJECT_STOP | PROJECT_KILL;
    return (project_hook(GF_STUN, dir, plev, flg));
}

bool poly_monster(int dir,int dam)
{
	int flg = PROJECT_STOP | PROJECT_KILL;
	return (project_hook(GF_OLD_POLY, dir, dam, flg));
}

bool clone_monster(int dir)
{
	int flg = PROJECT_STOP | PROJECT_KILL;
	return (project_hook(GF_OLD_CLONE, dir, 0, flg));
}

bool fear_monster(int dir, int plev)
{
	int flg = PROJECT_STOP | PROJECT_KILL;
	return (project_hook(GF_TURN_ALL, dir, plev, flg));
}

bool death_ray(int dir, int plev)
{
	int flg = PROJECT_STOP | PROJECT_KILL;
    return (project_hook(GF_DEATH_RAY, dir, plev, flg));
}

bool teleport_monster(int dir)
{
	int flg = PROJECT_BEAM | PROJECT_KILL;
	return (project_hook(GF_AWAY_ALL, dir, MAX_SIGHT * 5, flg));
}



/*
 * Hooks -- affect adjacent grids (radius 1 ball attack)
 */

bool door_creation(void)
{
	int flg = PROJECT_GRID | PROJECT_ITEM | PROJECT_HIDE;
	return (project(0, 1, py, px, 0, GF_MAKE_DOOR, flg));
}

bool trap_creation(void)
{
	int flg = PROJECT_GRID | PROJECT_ITEM | PROJECT_HIDE;
	return (project(0, 1, py, px, 0, GF_MAKE_TRAP, flg));
}

bool glyph_creation(void)
{   int flg = PROJECT_GRID | PROJECT_ITEM;
    return (project(0, 1, py, px, 0, GF_MAKE_GLYPH, flg));
}

bool wall_stone(void)
{
    int flg = PROJECT_GRID | PROJECT_ITEM;

    bool dummy = (project(0, 1, py, px, 0, GF_STONE_WALL, flg));

    /* Update stuff */
	p_ptr->update |= (PU_VIEW | PU_LITE | PU_FLOW);

	/* Update the monsters */
	p_ptr->update |= (PU_MONSTERS);

	/* Redraw map */
	p_ptr->redraw |= (PR_MAP);

	/* Window stuff */
	p_ptr->window |= (PW_OVERHEAD);

    return dummy;
}


bool destroy_doors_touch(void)
{
	int flg = PROJECT_GRID | PROJECT_ITEM | PROJECT_HIDE;
	return (project(0, 1, py, px, 0, GF_KILL_DOOR, flg));
}

bool sleep_monsters_touch(int dam)
{
	int flg = PROJECT_KILL | PROJECT_HIDE;
	return (project(0, 1, py, px, dam, GF_OLD_SLEEP, flg));
}







void call_chaos(int plev)
{
    int Chaos_type, dummy, dir;
    bool line_chaos = FALSE;

    int hurt_types[30] =
    { GF_ELEC,      GF_POIS,    GF_ACID,    GF_COLD,
      GF_FIRE,      GF_MISSILE, GF_ARROW,   GF_PLASMA,
      GF_HOLY_FIRE, GF_WATER,   GF_LITE,    GF_DARK,
      GF_FORCE,     GF_INERTIA, GF_MANA,    GF_METEOR,
      GF_ICE,       GF_CHAOS,   GF_NETHER,  GF_DISENCHANT,
      GF_SHARDS,    GF_SOUND,   GF_NEXUS,   GF_CONFUSION,
      GF_TIME,      GF_GRAVITY, GF_SHARD,  GF_NUKE,
      GF_HELL_FIRE, GF_DISINTEGRATE };

          Chaos_type = hurt_types[((randint (30))-1)];
	  if (randint(4)==1) line_chaos = TRUE;

			if (randint(6)==1)
	    {
				for (dummy = 1; dummy<10; dummy++)

				{
		if (dummy-5)
		    {
			if (line_chaos)
			      fire_beam(Chaos_type, dummy, 75);
			else
			    fire_ball(Chaos_type, dummy,
			    75, 2);
		     }
				}
	    }
        else if (randint(3)==1)

            {
                fire_ball(Chaos_type, 0, 300, 8);
            }
        else

			{
				if (!get_aim_dir(&dir)) return;
			if (line_chaos)
			      fire_beam(Chaos_type, dir, 150);
			else
			    fire_ball(Chaos_type, dir,
				    150, 3 + (plev/35));
			}
}

void activate_ty_curse(void)
{
    int i = 0;
    do
    {
    switch(randint(27))
	{
	    case 1: case 2: case 3: case 16: case 17:
		aggravate_monsters(1);
		if (randint(6)!=1) break;
	    case 4: case 5: case 6:
		activate_hi_summon();
		if (randint(6)!=1) break;
	    case 7: case 8: case 9: case 18:
	    (void) summon_specific(py, px, (dun_depth), 0);
		if (randint(6)!=1) break;
	    case 10: case 11: case 12:
		msg_print("You feel your life draining away...");
		lose_skills(10);
		if (randint(6)!=1) break;
	    case 13: case 14: case 15: case 19: case 20:
        if (p_ptr->free_act && (randint(100) < p_ptr->skill_sav))
        {
			skill_exp(SKILL_SAVE);
            /* Do nothing */ ;
        }
        else
        {
            msg_print("You feel like a statue!");
            if (p_ptr->free_act)
                set_paralyzed (p_ptr->paralyzed + randint(3));
            else
                set_paralyzed (p_ptr->paralyzed + randint(13));
        }
        if (randint(6)!=1) break;
	    case 21: case 22: case 23:
		(void)do_dec_stat((randint(6))-1);
		if (randint(6)!=1) break;
	    case 24:
		msg_print("Huh? Who am I? What am I doing here?");
		lose_all_info();
		break;
	    case 25:
		summon_reaver();
		break;
	    default:
		while (i<6)
		{
		do { (void)do_dec_stat(i); } while (randint(2)==1);
		i++;
		}
	}
    }   while (randint(3)==1);

}

void activate_hi_summon(void)
{
int i = 0;

    for (i = 0; i < (randint(9) + ((dun_depth) / 40)); i++)
    {
	switch(randint(26) + ((dun_depth) / 20) )
	{
	case 1: case 2:
	    (void) summon_specific(py, px, (dun_depth), SUMMON_ANT);
	    break;
	case 3: case 4:
	    (void) summon_specific(py, px, (dun_depth), SUMMON_SPIDER);
	    break;
	case 5: case 6:
	    (void) summon_specific(py, px, (dun_depth), SUMMON_HOUND);
	    break;
	case 7: case 8:
	    (void) summon_specific(py, px, (dun_depth), SUMMON_HYDRA);
	    break;
	case 9: case 10:
	    (void) summon_specific(py, px, (dun_depth), SUMMON_CTHULOID);
	    break;
	case 11: case 12:
	    (void) summon_specific(py, px, (dun_depth), SUMMON_UNDEAD);
	    break;
	case 13: case 14:
	(void) summon_specific(py, px, (dun_depth), SUMMON_DRAGON);
	    break;
	case 15: case 16:
	(void) summon_specific(py, px, (dun_depth), SUMMON_DEMON);
	    break;
	case 17:
	(void) summon_specific(py, px, (dun_depth), SUMMON_GOO);
	    break;
	case 18: case 19:
	(void) summon_specific(py, px, (dun_depth), SUMMON_UNIQUE);
	    break;
	case 20: case 21:
	(void) summon_specific(py, px, (dun_depth), SUMMON_HI_UNDEAD);
	    break;
	case 22: case 23:
	(void) summon_specific(py, px, (dun_depth), SUMMON_HI_DRAGON);
	    break;
	case 24: case 25:
	(void) summon_specific(py, px, 100, SUMMON_REAVER);
	    break;
	default:
	    (void) summon_specific(py, px,( ( ( (dun_depth) * 3) / 2 ) + 5 ), 0);
	}
    }
}

void summon_reaver(void)
{
    int i = 0;
    int max_reaver = ((dun_depth) / 50) + randint(6);

	    for (i = 0; i < max_reaver; i++)
			{
	       (void)summon_specific(py, px, 100, SUMMON_REAVER);
			}
}


void wall_breaker(int plev)
{

    int dummy = 5;

    if (randint(80+(plev))<70)
    {
	do { dummy = randint(9); }
	while ((dummy == 5) || (dummy == 0));
	wall_to_mud (dummy);
    }
    else if (randint(100)>30)
    {
	earthquake(py,px,1);
    }
    else
    {           for (dummy = 1; dummy<10; dummy++)

				{
		if (dummy-5)
		    {
			wall_to_mud(dummy);

		     }
				}
    }


}


void bless_weapon(void)
{
	int                     item;
    object_type             *o_ptr;
    u32b f1, f2, f3;

	C_TNEW(o_name, ONAME_MAX, char);


	/* Assume enchant weapon */
	item_tester_hook = item_tester_hook_weapon;


	/* Get an item (from equip or inven or floor) */
    if (!get_item(&item, "Bless which weapon? ", TRUE, TRUE, TRUE))
	{
		if (item == -2) msg_print("You have weapon to bless.");
		TFREE(o_name);
		return;
	}

	/* Get the item (in the pack) */
	if (item >= 0)
	{
		o_ptr = &inventory[item];
	}

	/* Get the item (on the floor) */
	else
	{
		o_ptr = &o_list[0 - item];
	}


	/* Description */
	object_desc(o_name, o_ptr, FALSE, 0);

        /* Extract the flags */
    object_flags(o_ptr, &f1, &f2, &f3);

    if (o_ptr->ident & (IDENT_CURSED))
    {

        if ( ( (f3 &  (TR3_HEAVY_CURSE)) && ( randint (100) < 33 ) )
            || (f3 & (TR3_PERMA_CURSE)) )
        {

            msg_format("The black aura on %s %s disrupts the blessing!",
                   ((item >= 0) ? "your" : "the"), o_name);
			TFREE(o_name);
            return;
        }

        msg_format("A malignant aura leaves %s %s.",
            ((item>=0)? "your" : "the"), o_name);

		/* Uncurse it */
		o_ptr->ident &= ~(IDENT_CURSED);

		/* Hack -- Assume felt */
		o_ptr->ident |= (IDENT_SENSE_CURSED);

		/* Recalculate the bonuses */
		p_ptr->update |= (PU_BONUS);

		/* Window stuff */
		p_ptr->window |= (PW_EQUIP);
    }

/* Next, we try to bless it. Artifacts have a 1/3 chance of being blessed,
otherwise, the operation simply disenchants them, godly power negating the
magic. Ok, the explanation is silly, but otherwise priests would always
bless every artifact weapon they find.
Ego weapons and normal weapons can be blessed automatically. */

    if (f3 & TR3_BLESSED)
    {
            msg_format("%s %s %s blessed already.",
                   ((item >= 0) ? "Your" : "The"), o_name,
                   ((o_ptr->number > 1) ? "were" : "was"));
			TFREE(o_name);
            return;
    }

    if (!(o_ptr->art_name || o_ptr->name1) || (randint(3)==1))
    {
	/* Describe */
	msg_format("%s %s shine%s!",
	       ((item >= 0) ? "Your" : "The"), o_name,
	       ((o_ptr->number > 1) ? "" : "s"));
	o_ptr->flags3 |= TR3_BLESSED;
    }
    else
    {

    bool dis_happened = FALSE;

    msg_print("The artifact resists your blessing!");

    /* Disenchant tohit */
    if (o_ptr->to_h > 0)
        {
            o_ptr->to_h--;
            dis_happened = TRUE;
        }

    if ((o_ptr->to_h > 5) && (rand_int(100) < 33)) o_ptr->to_h--;

	/* Disenchant todam */
    if (o_ptr->to_d > 0)
        {
            o_ptr->to_d--;
            dis_happened = TRUE;
        }
    if ((o_ptr->to_d > 5) && (rand_int(100) < 33)) o_ptr->to_d--;

	/* Disenchant toac */
    if (o_ptr->to_a > 0)
        {
            o_ptr->to_a--;
            dis_happened = TRUE;
        }
    if ((o_ptr->to_a > 5) && (rand_int(100) < 33)) o_ptr->to_a--;

    if (dis_happened)
        {
            msg_print("There is a static feeling in the air...");
            msg_format("%s %s %s disenchanted!",
                   ((item >= 0) ? "Your" : "The"), o_name,
                   ((o_ptr->number > 1) ? "were" : "was"));
        }

    }
	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Window stuff */
	p_ptr->window |= (PW_EQUIP | PW_PLAYER);

	TFREE(o_name);
}

 /*
  * Detect all "nonliving", "undead" or "demonic" monsters on current panel
  */
 bool detect_monsters_nonliving(void)
 {
   int             i, y, x;

   bool    flag = FALSE;


   /* Scan monsters */
   for (i = 1; i < m_max; i++)
   {
       monster_type *m_ptr = &m_list[i];
       monster_race *r_ptr = &r_info[m_ptr->r_idx];

       /* Skip dead monsters */
       if (!m_ptr->r_idx) continue;

       /* Location */
       y = m_ptr->fy;
       x = m_ptr->fx;

       /* Only detect nearby monsters */
       if (!panel_contains(y, x)) continue;

       /* Detect evil monsters */
           if ((r_ptr->flags3 & (RF3_NONLIVING)) ||
           (r_ptr->flags3 & (RF3_UNDEAD)) ||
           (r_ptr->flags3 & (RF3_CTHULOID)) ||
           (r_ptr->flags3 & (RF3_DEMON)))
       {
           /* Update monster recall window */
           if (monster_race_idx == m_ptr->r_idx)
           {
               /* Window stuff */
               p_ptr->window |= (PW_MONSTER);
           }

           /* Repair visibility later */
           repair_monsters = TRUE;

           /* Hack -- Detect monster */
           m_ptr->mflag |= (MFLAG_MARK | MFLAG_SHOW);

           /* Hack -- See monster */
           m_ptr->ml = TRUE;

           /* Redraw */
           lite_spot(y, x);

           /* Detect */
           flag = TRUE;
       }
   }

   /* Describe */
   if (flag)
   {
       /* Describe result */
       msg_print("You sense the presence of unnatural beings!");

	/* Update window */
	p_ptr->window |= PW_VISIBLE;
   }

   /* Result */
   return (flag);
 }



 /*
  * Confuse monsters
  */
 bool confuse_monsters(int dam)
 {
   return (project_hack(GF_OLD_CONF, dam));
 }


 /*
  * Charm monsters
  */
 bool charm_monsters(int dam)
 {
   return (project_hack(GF_CHARM, dam));
 }


 /*
  * Charm animals
  */
 bool charm_animals(int dam)
 {
   return (project_hack(GF_CONTROL_ANIMAL, dam));
 }


 /*
  * Stun monsters
  */
 bool stun_monsters(int dam)
 {
   return (project_hack(GF_STUN, dam));
 }


 /*
  * Stasis monsters
  */
 bool stasis_monsters(int dam)
 {
   return (project_hack(GF_STASIS, dam));
 }

 /*
  * Mindblast monsters
  */
 bool mindblast_monsters(int dam)
 {
   return (project_hack(GF_PSI, dam));
 }


 /*
  * Banish all monsters
  */
 bool banish_monsters(int dist)
 {
   return (project_hack(GF_AWAY_ALL, dist));
 }

 /*
  * Turn evil
  */
 bool turn_evil(int dam)
 {
   return (project_hack(GF_TURN_EVIL, dam));
 }

 /*
  * Turn everyone
  */
 bool turn_monsters(int dam)
 {
   return (project_hack(GF_TURN_ALL, dam));
 }


 /*

  * Death-ray all monsters (note: OBSCENELY powerful)
  */
#if 0
static bool deathray_monsters(int dam)
{
     return (project_hack(GF_DEATH_RAY, dam));
}
#endif




 bool charm_monster(int dir, int plev)
 {
   int flg = PROJECT_STOP | PROJECT_KILL;
   return (project_hook(GF_CHARM, dir, plev, flg));
 }

 bool control_one_undead(int dir, int plev)
 {
   int flg = PROJECT_STOP | PROJECT_KILL;
   return (project_hook(GF_CONTROL_UNDEAD, dir, plev, flg));
 }

 bool charm_animal(int dir, int plev)
 {
   int flg = PROJECT_STOP | PROJECT_KILL;
   return (project_hook(GF_CONTROL_ANIMAL, dir, plev, flg));
 }

 
 static int report_magics_aux(int dur)
{
    if (dur <= 5)
    {
        return 0;
    }
    else if (dur <= 10)
    {
        return 1;
    }
    else if (dur <= 20)
    {
        return 2;
    }
    else if (dur <= 50)
    {
        return 3;
    }
    else if (dur <= 100)
    {
        return 4;
    }
    else if (dur <= 200)
    {
        return 5;
    }
    else
    {
        return 6;
    }
}

static cptr report_magic_durations[] =
{
	"for a short time",
	"for a little while",
	"for a while",
	"for a long while",
	"for a long time",
	"for a very long time",
	"for an incredibly long time",
	"until you hit a monster"
};


void report_magics(void)
{
	int             i = 0, j, k;
	
	char Dummy[80];
	
	cptr    info[128];
	int     info2[128];
	
	if (p_ptr->blind)
	{
		info2[i]  = report_magics_aux(p_ptr->blind);
		info[i++] = "You cannot see";
	}
	if (p_ptr->confused)
	{
		info2[i]  = report_magics_aux(p_ptr->confused);
		info[i++] = "You are confused";
	}
	if (p_ptr->afraid)
	{
		info2[i]  = report_magics_aux(p_ptr->afraid);
		info[i++] = "You are terrified";
	}
	if (p_ptr->poisoned)
	{
		info2[i]  = report_magics_aux(p_ptr->poisoned);
		info[i++] = "You are poisoned";
	}
	if (p_ptr->image)
	{
		info2[i]  = report_magics_aux(p_ptr->image);
		info[i++] = "You are hallucinating";
	}
	
	if (p_ptr->blessed)
	{
		info2[i]  = report_magics_aux(p_ptr->blessed);
		info[i++] = "You feel rightous";
	}
	if (p_ptr->hero)
	{
		info2[i]  = report_magics_aux(p_ptr->hero);
		info[i++] = "You feel heroic";
	}
	if (p_ptr->shero)
	{
		info2[i]  = report_magics_aux(p_ptr->shero);
		info[i++] = "You are in a battle rage";
	}
	if (p_ptr->protevil)
	{
		info2[i]  = report_magics_aux(p_ptr->protevil);
		info[i++] = "You are protected from evil";
	}
	if (p_ptr->shield)
	{
		info2[i]  = report_magics_aux(p_ptr->shield);
		info[i++] = "You are protected by a mystic shield";
	}
	if (p_ptr->invuln)
	{
		info2[i]  = report_magics_aux(p_ptr->invuln);
		info[i++] = "You are invulnerable";
	}
	if (p_ptr->wraith_form)
	{
		info2[i]  = report_magics_aux(p_ptr->wraith_form);
		info[i++] = "You are incorporeal";
	}
	if (p_ptr->confusing)
	{
		info2[i]  = 7;
		info[i++] = "Your hands are glowing dull red.";
	}
	if (p_ptr->word_recall)
	{
		info2[i]  = report_magics_aux(p_ptr->word_recall);
		info[i++] = "You waiting to be recalled";
	}
	if (p_ptr->oppose_acid)
	{
		info2[i]  = report_magics_aux(p_ptr->oppose_acid);
		info[i++] = "You are resistant to acid";
	}
	if (p_ptr->oppose_elec)
	{
		info2[i]  = report_magics_aux(p_ptr->oppose_elec);
		info[i++] = "You are resistant to lightning";
	}
	if (p_ptr->oppose_fire)
	{
		info2[i]  = report_magics_aux(p_ptr->oppose_fire);
		info[i++] = "You are resistant to fire";
	}
	if (p_ptr->oppose_cold)
	{
		info2[i]  = report_magics_aux(p_ptr->oppose_cold);
		info[i++] = "You are resistant to cold";
	}
	if (p_ptr->oppose_pois)
	{
		info2[i]  = report_magics_aux(p_ptr->oppose_pois);
		info[i++] = "You are resistant to poison";
	}

	/* Save the screen */
	Term_save();
	
	/* Erase the screen */
	for (k = 1; k < 24; k++) prt("", k, 13);
	
	/* Label the information */
	prt("     Your Current Magic:", 1, 15);
	
	/* We will print on top of the map (column 13) */
	for (k = 2, j = 0; j < i; j++)
	{
		/* Show the info */
		sprintf( Dummy, "%s %s.", info[j],
			report_magic_durations[info2[j]] );
		prt(Dummy, k++, 15);
		
		/* Every 20 entries (lines 2 to 21), start over */
		if ((k == 22) && (j + 1 < i))
		{
			prt("-- more --", k, 15);
			inkey();
			for (; k > 2; k--) prt("", k, 15);
		}
	}
	
	/* Pause */
	prt("[Press any key to continue]", k, 13);
	inkey();

	/* Restore the screen */
	Term_load();
}

void teleport_swap(int dir)
{
	int tx, ty;
	cave_type * c_ptr;
	monster_type * m_ptr;
	monster_race * r_ptr;
	
	if ((dir == 5) && target_okay())
	{
		tx = target_col;
		ty = target_row;
	}
	else
	{
		tx = px + ddx[dir];
		ty = py + ddy[dir];
	}
	c_ptr = &cave[ty][tx];
	
	if (!c_ptr->m_idx)
	{
		msg_print("You can't trade places with that!");
	}
	else
	{
		m_ptr = &m_list[c_ptr->m_idx];
		r_ptr = &r_info[m_ptr->r_idx];
		
		if (r_ptr->flags3 & RF3_RES_TELE)
		{
			msg_print("Your teleportation is blocked!");
		}
		else
		{
			sound(SOUND_TELEPORT);
			
			cave[py][px].m_idx = c_ptr->m_idx;
			
			/* Update the old location */
			c_ptr->m_idx = 0;
			
			/* Move the monster */
			m_ptr->fy = (byte)py; 
			m_ptr->fx = (byte)px;
			
			/* Move the player */
			move_to(ty, tx);
			
			/* Update the monster (new location) */
			update_mon(cave[ty][tx].m_idx, TRUE);
			
			/* Handle stuff XXX XXX XXX */
			handle_stuff();
		}
	}
}

void alter_reality(void)
{
	msg_print("The world changes!");

	/* Leaving */
	change_level(dun_level, START_RANDOM);
}
