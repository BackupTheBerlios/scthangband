#define QUEST_C
/* File: quest.c */

/* Purpose: functions used by the random quest source
 * maybe some of these function are better placed somewhere else
 */

/*
 * Copyright (c) 1989 James E. Wilson, Christopher J. Stuart
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

/*
 *
 * Heino Vander Sanden
 * Heino.VanderSanden@AdValvas.Be
 *
 */

 #include "angband.h"

/*
 * Search quests for correct monster
 */
int get_quest_monster(void)
{
	int i;

	for (i = 0; i < MAX_Q_IDX; i++)
	{
		if ((q_list[i].level == dun_level) && (q_list[i].dungeon == cur_dungeon)) return (q_list[i].r_idx);
	}
	return 0;
}

#if 0
/*
 * Search quests for number of monsters
 */
static int get_max_monster(void)
{
	int i;

	for (i = 0; i < MAX_Q_IDX; i++)
	{
		if ((q_list[i].level == dun_level) && (q_list[i].dungeon == cur_dungeon)) return (q_list[i].max_num);
	}
	return 0;
}
#endif

/*
 * Get quest number
 */
int get_quest_number(void)
{
	int i;

	for (i=0; i < MAX_Q_IDX; i++)
	{
                if ((q_list[i].level == dun_level) &&(q_list[i].dungeon == cur_dungeon)) 
				{
					return (i);
				}
	}
	return -1;
}

/*
 * Print the number and monster to kill in this level
 */
void print_quest_message(void)
{
	int q_idx = get_quest_number();
	monster_race	*r_ptr = &r_info[q_list[q_idx].r_idx];
	int q_num = q_list[q_idx].max_num - q_list[q_idx].cur_num_known;
	byte flag = (q_list[q_idx].max_num == 1) ? MDF_DEF : MDF_NUMBER;

	msg_format("You still have to kill %v.", monster_desc_aux_f3, r_ptr,
		q_num, flag);
}

/* Array of places to find an inscription */
static cptr find_quest[5] =
{
	"You find the following inscription in the floor",
	"You see a message inscribed in the wall",
	"There is a sign saying",
	"Something is writen on the staircase",
    "You find a scroll with the following message",
};

/*
 * Discover quest
 */
void quest_discovery(void)
{
	int 	q_idx = get_quest_number();
	monster_race	*r_ptr = &r_info[q_list[q_idx].r_idx];
	int q_num = q_list[q_idx].max_num;

	/* Get a properly formatted name. Note that no monster will actually
	be given an article as only uniques are currently allowed to be
	solitary quest monsters. */
	byte flags = 0;
	if (q_num > 1)
		flags = MDF_NUMBER;
	else if (~r_ptr->flags1 & RF1_UNIQUE)
		flags = MDF_DEF;
	else
		flags = MDF_INDEF;

	msg_print (find_quest[rand_range(0,4)]);
	msg_print (NULL);
	msg_format("Beware, this level is protected by %v!", monster_desc_aux_f3,
		r_ptr, q_num, flags);
}

/*
 * Mark quest monsters out as being special.
 */
void set_guardians(void)
{
	monster_race *r_ptr;
	quest *q_ptr;

	/* Remove any previous quest monster status. */
	for (r_ptr = r_info; r_ptr < r_info+MAX_R_IDX; r_ptr++)
	{
		r_ptr->flags1 &= ~(RF1_GUARDIAN);
	}

	/* Set quest monster status for the current quest and any unique quests. */
	for (q_ptr = q_list; q_ptr < q_list+MAX_Q_IDX; q_ptr++)
	{
		r_ptr = r_info+q_ptr->r_idx;

		if ((get_quest_monster() == q_ptr->r_idx) ||
			(r_ptr->flags1 & RF1_UNIQUE))
		{
			r_ptr->flags1 |= RF1_GUARDIAN;
		}
	}
}

/*
 * Search the next quest level
 */
#if 0
static int next_quest_level(void)
{
	int i;

	for(i = p_ptr->max_dlv[cur_dungeon]; i < 127; i++)
	{
		if (is_quest(i))
			return i;
	}
	return 127;
}
#endif
