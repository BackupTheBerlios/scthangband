#define CAVE_C
/* File: cave.c */

/* Purpose: low level dungeon routines -BEN- */


#include "angband.h"


/*
 * Support for Adam Bolt's tileset, lighting and transparency effects
 * by Robert Ruehlmann (rr9@angband.org)
 */


/*
 * Approximate Distance between two points.
 *
 * When either the X or Y component dwarfs the other component,
 * this function is almost perfect, and otherwise, it tends to
 * over-estimate about one grid per fifteen grids of distance.
 *
 * Algorithm: hypot(dy,dx) = max(dy,dx) + min(dy,dx) / 2
 */
int distance(int y1, int x1, int y2, int x2)
{
	int dy, dx, d;

	/* Find the absolute y/x distance components */
	dy = (y1 > y2) ? (y1 - y2) : (y2 - y1);
	dx = (x1 > x2) ? (x1 - x2) : (x2 - x1);

	/* Hack -- approximate the distance */
	d = (dy > dx) ? (dy + (dx>>1)) : (dx + (dy>>1));

	/* Return the distance */
	return (d);
}


/*
 * A simple, fast, integer-based line-of-sight algorithm.  By Joseph Hall,
 * 4116 Brewster Drive, Raleigh NC 27606.  Email to jnh@ecemwl.ncsu.edu.
 *
 * Returns TRUE if a line of sight can be traced from (x1,y1) to (x2,y2).
 *
 * The LOS begins at the center of the tile (x1,y1) and ends at the center of
 * the tile (x2,y2).  If los() is to return TRUE, all of the tiles this line
 * passes through must be floor tiles, except for (x1,y1) and (x2,y2).
 *
 * We assume that the "mathematical corner" of a non-floor tile does not
 * block line of sight.
 *
 * Because this function uses (short) ints for all calculations, overflow may
 * occur if dx and dy exceed 90.
 *
 * Once all the degenerate cases are eliminated, the values "qx", "qy", and
 * "m" are multiplied by a scale factor "f1 = abs(dx * dy * 2)", so that
 * we can use integer arithmetic.
 *
 * We travel from start to finish along the longer axis, starting at the border
 * between the first and second tiles, where the y offset = .5 * slope, taking
 * into account the scale factor.  See below.
 *
 * Also note that this function and the "move towards target" code do NOT
 * share the same properties.  Thus, you can see someone, target them, and
 * then fire a bolt at them, but the bolt may hit a wall, not them.  However,
 * by clever choice of target locations, you can sometimes throw a "curve".
 *
 * Note that "line of sight" is not "reflexive" in all cases.
 *
 * Use the "projectable()" routine to test "spell/missile line of sight".
 *
 * Use the "update_view()" function to determine player line-of-sight.
 */
bool los(int y1, int x1, int y2, int x2)
{
	/* Delta */
	int dx, dy;

	/* Absolute */
	int ax, ay;

	/* Signs */
	int sx, sy;

	/* Fractions */
	int qx, qy;

	/* Scanners */
	int tx, ty;

	/* Scale factors */
	int f1, f2;

	/* Slope, or 1/Slope, of LOS */
	int m;


	/* Extract the offset */
	dy = y2 - y1;
	dx = x2 - x1;

	/* Extract the absolute offset */
	ay = ABS(dy);
	ax = ABS(dx);


	/* Handle adjacent (or identical) grids */
	if ((ax < 2) && (ay < 2)) return (TRUE);


	/* Paranoia -- require "safe" origin */
	/* if (!in_bounds(y1, x1)) return (FALSE); */


	/* Directly South/North */
	if (!dx)
	{
		/* South -- check for walls */
		if (dy > 0)
		{
			for (ty = y1 + 1; ty < y2; ty++)
			{
				if (!cave_floor_bold(ty, x1)) return (FALSE);
			}
		}

		/* North -- check for walls */
		else
		{
			for (ty = y1 - 1; ty > y2; ty--)
			{
				if (!cave_floor_bold(ty, x1)) return (FALSE);
			}
		}

		/* Assume los */
		return (TRUE);
	}

	/* Directly East/West */
	if (!dy)
	{
		/* East -- check for walls */
		if (dx > 0)
		{
			for (tx = x1 + 1; tx < x2; tx++)
			{
				if (!cave_floor_bold(y1, tx)) return (FALSE);
			}
		}

		/* West -- check for walls */
		else
		{
			for (tx = x1 - 1; tx > x2; tx--)
			{
				if (!cave_floor_bold(y1, tx)) return (FALSE);
			}
		}

		/* Assume los */
		return (TRUE);
	}


	/* Extract some signs */
	sx = (dx < 0) ? -1 : 1;
	sy = (dy < 0) ? -1 : 1;


	/* Vertical "knights" */
	if (ax == 1)
	{
		if (ay == 2)
		{
			if (cave_floor_bold(y1 + sy, x1)) return (TRUE);
		}
	}

	/* Horizontal "knights" */
	else if (ay == 1)
	{
		if (ax == 2)
		{
			if (cave_floor_bold(y1, x1 + sx)) return (TRUE);
		}
	}


	/* Calculate scale factor div 2 */
	f2 = (ax * ay);

	/* Calculate scale factor */
	f1 = f2 << 1;


	/* Travel horizontally */
	if (ax >= ay)
	{
		/* Let m = dy / dx * 2 * (dy * dx) = 2 * dy * dy */
		qy = ay * ay;
		m = qy << 1;

		tx = x1 + sx;

		/* Consider the special case where slope == 1. */
		if (qy == f2)
		{
			ty = y1 + sy;
			qy -= f1;
		}
		else
		{
			ty = y1;
		}

		/* Note (below) the case (qy == f2), where */
		/* the LOS exactly meets the corner of a tile. */
		while (x2 - tx)
		{
			if (!cave_floor_bold(ty, tx)) return (FALSE);

			qy += m;

			if (qy < f2)
			{
				tx += sx;
			}
			else if (qy > f2)
			{
				ty += sy;
				if (!cave_floor_bold(ty, tx)) return (FALSE);
				qy -= f1;
				tx += sx;
			}
			else
			{
				ty += sy;
				qy -= f1;
				tx += sx;
			}
		}
	}

	/* Travel vertically */
	else
	{
		/* Let m = dx / dy * 2 * (dx * dy) = 2 * dx * dx */
		qx = ax * ax;
		m = qx << 1;

		ty = y1 + sy;

		if (qx == f2)
		{
			tx = x1 + sx;
			qx -= f1;
		}
		else
		{
			tx = x1;
		}

		/* Note (below) the case (qx == f2), where */
		/* the LOS exactly meets the corner of a tile. */
		while (y2 - ty)
		{
			if (!cave_floor_bold(ty, tx)) return (FALSE);

			qx += m;

			if (qx < f2)
			{
				ty += sy;
			}
			else if (qx > f2)
			{
				tx += sx;
				if (!cave_floor_bold(ty, tx)) return (FALSE);
				qx -= f1;
				ty += sy;
			}
			else
			{
				tx += sx;
				qx -= f1;
				ty += sy;
			}
		}
	}

	/* Assume los */
	return (TRUE);
}






/*
 * Can the player "see" the given grid in detail?
 *
 * He must have vision, illumination, and line of sight.
 *
 * Note -- "CAVE_LITE" is only set if the "torch" has "los()".
 * So, given "CAVE_LITE", we know that the grid is "fully visible".
 *
 * Note that "CAVE_GLOW" makes little sense for a wall, since it would mean
 * that a wall is visible from any direction.  That would be odd.  Except
 * under wizard light, which might make sense.  Thus, for walls, we require
 * not only that they be "CAVE_GLOW", but also, that they be adjacent to a
 * grid which is not only "CAVE_GLOW", but which is a non-wall, and which is
 * in line of sight of the player.
 *
 * This extra check is expensive, but it provides a more "correct" semantics.
 *
 * Note that we should not run this check on walls which are "outer walls" of
 * the dungeon, or we will induce a memory fault, but actually verifying all
 * of the locations would be extremely expensive.
 *
 * Thus, to speed up the function, we assume that all "perma-walls" which are
 * "CAVE_GLOW" are "illuminated" from all sides.  This is correct for all cases
 * except "vaults" and the "buildings" in town.  But the town is a hack anyway,
 * and the player has more important things on his mind when he is attacking a
 * monster vault.  It is annoying, but an extremely important optimization.
 *
 * Note that "glowing walls" are only considered to be "illuminated" if the
 * grid which is next to the wall in the direction of the player is also a
 * "glowing" grid.  This prevents the player from being able to "see" the
 * walls of illuminated rooms from a corridor outside the room.
 */
bool player_can_see_bold(int y, int x)
{
	int xx, yy;

	cave_type *c_ptr;

	/* Blind players see nothing */
	if (p_ptr->blind) return (FALSE);

	/* Access the cave grid */
	c_ptr = &cave[y][x];

	/* Note that "torch-lite" yields "illumination" */
	if (c_ptr->info & (CAVE_LITE)) return (TRUE);

	/* Require line of sight to the grid */
	if (!player_has_los_bold(y, x)) return (FALSE);

	/* Require "perma-lite" of the grid */
	if (!(c_ptr->info & (CAVE_GLOW))) return (FALSE);

	/* Floors are simple */
	if (cave_floor_bold(y, x)) return (TRUE);

	/* Hack -- move towards player */
	yy = (y < py) ? (y + 1) : (y > py) ? (y - 1) : y;
	xx = (x < px) ? (x + 1) : (x > px) ? (x - 1) : x;

	/* Check for "local" illumination */
	if (cave[yy][xx].info & (CAVE_GLOW))
	{
		/* Assume the wall is really illuminated */
		return (TRUE);
	}

	/* Assume not visible */
	return (FALSE);
}



/*
 * Returns true if the player's grid is dark
 */
bool no_lite(void)
{
	return (!player_can_see_bold(py, px));
}




/*
 * Determine if a given location may be "destroyed"
 *
 * Used by destruction spells, and for placing stairs, etc.
 */
bool cave_valid_bold(int y, int x)
{
	cave_type *c_ptr = &cave[y][x];

	s16b this_o_idx, next_o_idx = 0;


	/* Forbid perma-grids */
	if (cave_perma_grid(c_ptr)) return (FALSE);

	/* Check objects */
	for (this_o_idx = c_ptr->o_idx; this_o_idx; this_o_idx = next_o_idx)
	{
		object_type *o_ptr;

		/* Acquire object */
		o_ptr = &o_list[this_o_idx];

		/* Acquire next object */
		next_o_idx = o_ptr->next_o_idx;

		/* Forbid artifact grids */
		if (allart_p(o_ptr)) return (FALSE);
	}

	/* Accept */
	return (TRUE);
}




/*
 * Hack -- Legal monster codes
 */
static cptr image_monster_hack = \
"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

static cptr image_monster_hack_ibm = \
"\006\026\037\311\321\326\334\341\340\342\344\347\351\350\352\353\355\357\361\364\366\365\372\371\373\374\260\200\256\251\201\330\203\276\204\245\265\217\205\275\346\253\303\213\214\271\327\215\360\336\376\375\267\222\223\302\313\310\312\315\316\317\314\323\324\225\322\332\333\331\236\226\227\257\230\231\232\270\233\234";


/*
 * Mega-Hack -- Hallucinatory monster
 */
static void image_monster(byte *ap, char *cp)
{
	if (use_graphics)
	{
		int n = strlen(image_monster_hack);
		(*cp) = (image_monster_hack[rand_int(n)]);

		/* Random color */
		(*ap) = randint(15);
	}
	else if (streq(ANGBAND_SYS, "ibm"))
	{
		int n = strlen(image_monster_hack_ibm);
		(*cp) = (image_monster_hack_ibm[rand_int(n)]);
		/* Random color */
		(*ap) = randint(15);
	}
	else
	{
		monster_race *r_ptr;
		do
		{
			r_ptr = r_info+rand_int(MAX_R_IDX);
		}
		while (is_fake_monster(r_ptr));

		(*cp) = r_ptr->gfx.xc;
		(*ap) = r_ptr->gfx.xa;
	}
}




/*
 * Hack -- Legal object codes
 */
static cptr image_object_hack = \
"?/|\\\"!$()_-=[]{},~";

static cptr image_object_hack_ibm = \
"����󧶴�Ɔ������������Ց���";

/*
 * Mega-Hack -- Hallucinatory object
 */
static void image_object(byte *ap, char *cp)
{
	int n = strlen(image_object_hack);

	if (use_graphics)
	{
		(*cp) = (image_object_hack[rand_int(n)]);
		/* Random color */
		(*ap) = randint(15);
	}
	else if (streq(ANGBAND_SYS, "ibm"))
	{
		n = strlen(image_object_hack_ibm);
		(*cp) = (image_object_hack_ibm[rand_int(n)]);
		/* Random color */
		(*ap) = randint(15);
	}
	else
	{
		(*cp) = object_kind_char(&k_info[randint(MAX_K_IDX-1)]);
		(*ap) = object_kind_attr(&k_info[randint(MAX_K_IDX-1)]);
	}
}


/*
 * Hack -- Random hallucination
 */
static void image_random(byte *ap, char *cp)
{
	/* Normally, assume monsters */
	if (rand_int(100) < 75)
	{
		image_monster(ap, cp);
	}

	/* Otherwise, assume objects */
	else
	{
		image_object(ap, cp);
	}
}


/*
 * Change the colour of a monster if the player uses the look function.
 *
 * Hack - colour-changing uniques don't reach this function when they're doing
 * something special.
 */
static bool do_violet_unique(monster_race *r_ptr, byte *ap, char *cp)
{
	int a;

	/* This isn't the correct turn. */
	if (!violet_uniques) return FALSE;

	/* Multi-hued monsters are handled elsewhere. */
	if (r_ptr->flags1 & RF1_ATTR_MULTI) return FALSE;

	/* Uniques usually become violet. */
	if (r_ptr->flags1 & RF1_UNIQUE) a = TERM_VIOLET;

	/* Monsters which would otherwise be invisible become red. */
	else if ((*ap) == r_ptr->gfx.xa && (*cp) == r_ptr->gfx.xc) a = TERM_RED;

	/* Only the above types of monster are modified here. */
	else return FALSE;

	/* Monsters which are the colour in question anyway become yellow. */
	if (a == r_ptr->gfx.xa) a = TERM_YELLOW;

	/* Store the result. */
	(*ap) = a;
	(*cp) = r_ptr->gfx.xc;

	/* Let the game know that something has happened. */
	return TRUE;
}


/*
 * Extract the attr/char to display at the given (legal) map location
 *
 * Basically, we "paint" the chosen attr/char in several passes, starting
 * with any known "terrain features" (defaulting to darkness), then adding
 * any known "objects", and finally, adding any known "monsters".  This
 * is not the fastest method but since most of the calls to this function
 * are made for grids with no monsters or objects, it is fast enough.
 *
 * Note that this function, if used on the grid containing the "player",
 * will return the attr/char of the grid underneath the player, and not
 * the actual player attr/char itself, allowing a lot of optimization
 * in various "display" functions.
 *
 * Note that the "zero" entry in the feature/object/monster arrays are
 * used to provide "special" attr/char codes, with "monster zero" being
 * used for the player attr/char, "object zero" being used for the "stack"
 * attr/char, and "feature zero" being used for the "nothing" attr/char,
 * though this function makes use of only "feature zero".
 *
 * Note that monsters can have some "special" flags, including "ATTR_MULTI",
 * which means their color changes, and "ATTR_CLEAR", which means they take
 * the color of whatever is under them, and "CHAR_CLEAR", which means that
 * they take the symbol of whatever is under them.  Technically, the flag
 * "CHAR_MULTI" is supposed to indicate that a monster looks strange when
 * examined, but this flag is currently ignored.  All of these flags are
 * ignored if the "avoid_other" option is set, since checking for these
 * conditions is expensive and annoying on some systems.
 *
 * Currently, we do nothing with multi-hued objects, because there are
 * not any.  If there were, they would have to set "shimmer_objects"
 * when they were created, and then new "shimmer" code in "dungeon.c"
 * would have to be created handle the "shimmer" effect, and the code
 * in "cave.c" would have to be updated to create the shimmer effect.
 *
 * Note the effects of hallucination.  Objects always appear as random
 * "objects", monsters as random "monsters", and normal grids occasionally
 * appear as random "monsters" or "objects", but note that these random
 * "monsters" and "objects" are really just "colored ascii symbols".
 *
 * Note that "floors" and "invisible traps" (and "zero" features) are
 * drawn as "floors" using a special check for optimization purposes,
 * and these are the only features which get drawn using the special
 * lighting effects activated by "view_special_lite".
 *
 * Note the use of the "mimic" field in the "terrain feature" processing,
 * which allows any feature to "pretend" to be another feature.  This is
 * used to "hide" secret doors, and to make all "doors" appear the same,
 * and all "walls" appear the same, and "hidden" treasure stay hidden.
 * It is possible to use this field to make a feature "look" like a floor,
 * but the "special lighting effects" for floors will not be used.
 *
 * Note the use of the new "terrain feature" information.  Note that the
 * assumption that all interesting "objects" and "terrain features" are
 * memorized allows extremely optimized processing below.  Note the use
 * of separate flags on objects to mark them as memorized allows a grid
 * to have memorized "terrain" without granting knowledge of any object
 * which may appear in that grid.
 *
 * Note the efficient code used to determine if a "floor" grid is
 * "memorized" or "viewable" by the player, where the test for the
 * grid being "viewable" is based on the facts that (1) the grid
 * must be "lit" (torch-lit or perma-lit), (2) the grid must be in
 * line of sight, and (3) the player must not be blind, and uses the
 * assumption that all torch-lit grids are in line of sight.
 *
 * Note that floors (and invisible traps) are the only grids which are
 * not memorized when seen, so only these grids need to check to see if
 * the grid is "viewable" to the player (if it is not memorized).  Since
 * most non-memorized grids are in fact walls, this induces *massive*
 * efficiency, at the cost of *forcing* the memorization of non-floor
 * grids when they are first seen.  Note that "invisible traps" are
 * always treated exactly like "floors", which prevents "cheating".
 *
 * Note the "special lighting effects" which can be activated for floor
 * grids using the "view_special_lite" option (for "white" floor grids),
 * causing certain grids to be displayed using special colors.  If the
 * player is "blind", we will use "dark grey", else if the grid is lit
 * by the torch, and the "view_yellow_lite" option is set, we will use
 * "yellow", else if the grid is "dark", we will use "dark grey", else
 * if the grid is not "viewable", and the "view_bright_lite" option is
 * set, and the we will use "slate" (grey).  We will use "white" for all
 * other cases, in particular, for illuminated viewable floor grids.
 *
 * Note the "special lighting effects" which can be activated for wall
 * grids using the "view_granite_lite" option (for "white" wall grids),
 * causing certain grids to be displayed using special colors.  If the
 * player is "blind", we will use "dark grey", else if the grid is lit
 * by the torch, and the "view_yellow_lite" option is set, we will use
 * "yellow", else if the "view_bright_lite" option is set, and the grid
 * is not "viewable", or is "dark", or is glowing, but not when viewed
 * from the player's current location, we will use "slate" (grey).  We
 * will use "white" for all other cases, in particular, for correctly
 * illuminated viewable wall grids.
 *
 * Note that, when "view_granite_lite" is set, we use an inline version
 * of the "player_can_see_bold()" function to check the "viewability" of
 * grids when the "view_bright_lite" option is set, and we do NOT use
 * any special colors for "dark" wall grids, since this would allow the
 * player to notice the walls of illuminated rooms from a hallway that
 * happened to run beside the room.  The alternative, by the way, would
 * be to prevent the generation of hallways next to rooms, but this
 * would still allow problems when digging towards a room.
 *
 * Note that bizarre things must be done when the "attr" and/or "char"
 * codes have the "high-bit" set, since these values are used to encode
 * various "special" pictures in some versions, and certain situations,
 * such as "multi-hued" or "clear" monsters, cause the attr/char codes
 * to be "scrambled" in various ways.
 *
 * Note that eventually we may use the "&" symbol for embedded treasure,
 * and use the "*" symbol to indicate multiple objects, though this will
 * have to wait for Angband 2.8.0 or later.  Note that currently, this
 * is not important, since only one object or terrain feature is allowed
 * in each grid.  If needed, "k_info[0]" will hold the "stack" attr/char.
 *
 * Note the assumption that doing "x_ptr = &x_info[x]" plus a few of
 * "x_ptr->xxx", is quicker than "x_info[x].xxx", if this is incorrect
 * then a whole lot of code should be changed...  XXX XXX
 */
void map_info(int y, int x, byte *ap, char *cp, byte *tap, char *tcp)
{
	cave_type *c_ptr;

	feature_type *f_ptr;

	object_type *o_ptr;
	bool seen_obj;

	int feat;

	byte a;
	char c;

	/* Illegal locations are simple. */
	if (!in_bounds2(y, x))
	{
		f_ptr = f_info+FEAT_ILLEGAL;
		*ap = f_ptr->gfx.xa;
		*cp = f_ptr->gfx.xc;

		*tap = f_ptr->gfx.xa;
		*tcp = f_ptr->gfx.xc;

		return;
	}

	/* Get the cave */
	c_ptr = &cave[y][x];


	/* Feature code */
	feat = c_ptr->r_feat;

	/* Floors (etc) */
	if ((feat <= FEAT_INVIS) || (feat == FEAT_PATH))
	{
		/* Memorized (or visible) floor */
		if ((c_ptr->info & (CAVE_MARK)) ||
			(((c_ptr->info & (CAVE_LITE)) ||
				((c_ptr->info & (CAVE_GLOW)) &&
				(c_ptr->info & (CAVE_VIEW)))) &&
			!p_ptr->blind))
		{
			/* Access floor */
			f_ptr = &f_info[FEAT_FLOOR];

			/* Hack - path uses a different graphic */
			if (feat == FEAT_PATH) f_ptr = &f_info[FEAT_PATH];

			/* Normal char */
			c = f_ptr->gfx.xc;

			/* Normal attr */
			a = f_ptr->gfx.xa;

			/* Special lighting effects */
			if (view_special_lite && ((a == TERM_WHITE) || use_graphics))
			{
				/* Handle "blind" */
				if (p_ptr->blind)
				{
					if (use_graphics)
					{
						/* Use a dark tile */
						c++;
					}
					else
					{
						/* Use "dark grey" */
						a = TERM_L_DARK;
					}
				}

				/* Handle "torch-lit" grids */
				else if (c_ptr->info & (CAVE_LITE))
				{
					/* Torch lite */
					if (view_yellow_lite)
					{
						if (use_graphics)
						{
							/* Use a brightly lit tile */
							c += 2;
						}
						else
						{
							/* Use "yellow" */
							a = TERM_YELLOW;
						}
					}
				}

				/* Handle "dark" grids */
				else if (!(c_ptr->info & (CAVE_GLOW)))
				{
					if (use_graphics)
					{
						/* Use a dark tile */
						c++;
					}
					else
					{
						/* Use "dark grey" */
						a = TERM_L_DARK;
					}
				}

				/* Handle "out-of-sight" grids */
				else if (!(c_ptr->info & (CAVE_VIEW)))
				{
					/* Special flag */
					if (view_bright_lite)
					{
						if (use_graphics)
						{
							/* Use a dark tile */
							c++;
						}
						else
						{
							/* Use "grey" */
							a = TERM_SLATE;
						}
					}
				}
			}
		}

		/* Unknown */
		else
		{
			if (c_ptr->info & CAVE_TRAP)
			{
				f_ptr = &f_info[FEAT_NONE_TD];
			}
			else
			{
			/* Access darkness */
			f_ptr = &f_info[FEAT_NONE];
			}

			/* Normal attr */
			a = f_ptr->gfx.xa;

			/* Normal char */
			c = f_ptr->gfx.xc;
		}
	}

	/* Non floors */
	else
	{
		/* Memorized grids */
		if (c_ptr->info & (CAVE_MARK))
		{
			/* Apply "mimic" field */
			feat = f_info[feat].mimic;

			/* Access feature */
			f_ptr = &f_info[feat];

			/* Normal char */
			c = f_ptr->gfx.xc;

			/* Normal attr */
			a = f_ptr->gfx.xa;

			/* Special lighting effects */
			if (view_granite_lite && ((a == TERM_WHITE) || (use_graphics)) &&
				(((feat >= FEAT_SECRET) && (feat <= FEAT_PERM_SOLID) &&
				(feat !=FEAT_MAGMA_K) && (feat !=FEAT_QUARTZ_K) &&
				(feat !=FEAT_RUBBLE)) || ((feat == FEAT_TREE) || (feat == FEAT_BUSH)
				|| (feat == FEAT_WATER))))
			{
				/* Handle "blind" */
				if (p_ptr->blind)
				{
					if (use_graphics)
					{
						/* Use a dark tile */
						c++;
					}
					else
					{
						/* Use "dark grey" */
						a = TERM_L_DARK;
					}
				}

				/* Handle "torch-lit" grids */
				else if (c_ptr->info & (CAVE_LITE))
				{
					/* Torch lite */
					if (view_yellow_lite)
					{
						if (use_graphics)
						{
							/* Use a brightly lit tile */
							c += 2;
						}
						else
						{
							/* Use "yellow" */
							a = TERM_YELLOW;
						}
					}
				}

				/* Handle "view_bright_lite" */
				else if (view_bright_lite)
				{
					/* Not viewable */
					if (!(c_ptr->info & (CAVE_VIEW)))
					{
						if (use_graphics)
						{
							/* Use a lit tile */
						if ((feat == FEAT_TREE) || (feat == FEAT_BUSH) || (feat == FEAT_WATER))
							{
								c++;
							}
						}
						else
						{
							/* Use "grey" */
							a = TERM_SLATE;
						}
					}

					/* Not glowing */
					else if (!(c_ptr->info & (CAVE_GLOW)))
					{
						if (use_graphics)
						{
							/* Use a lit tile */
							if ((feat == FEAT_TREE) || (feat == FEAT_BUSH) || (feat == FEAT_WATER))
							{
								c++;
							}
						}
						else
						{
							/* Use "grey" */
							a = TERM_SLATE;
						}
					}

					/* Not glowing correctly */
					else
					{
						int xx, yy;

						/* Hack -- move towards player */
						yy = (y < py) ? (y + 1) : (y > py) ? (y - 1) : y;
						xx = (x < px) ? (x + 1) : (x > px) ? (x - 1) : x;

						/* Check for "local" illumination */
						if (!(cave[yy][xx].info & (CAVE_GLOW)))
						{
							if (use_graphics)
							{
								/* Use a lit tile */
							}
							else
							{
								/* Use "grey" */
								a = TERM_SLATE;
							}
						}
					}
				}
			}
		}

		/* Unknown */
		else
		{
			if (c_ptr->info & CAVE_TRAP)
			{
				f_ptr = &f_info[FEAT_NONE_TD];
			}
			else
			{
			/* Access darkness */
			f_ptr = &f_info[FEAT_NONE];
			}


			/* Normal attr */
			a = f_ptr->gfx.xa;

			/* Normal char */
			c = f_ptr->gfx.xc;
		}
	}

	/* Hack -- rare random hallucination, except on outer dungeon walls */
	if (p_ptr->image && (!rand_int(256)) && (c_ptr->r_feat < FEAT_PERM_SOLID))
	{
		/* Hallucinate */
		image_random(ap, cp);
	}

	/* Save the terrain info for the transparency effects */
	(*tap) = a;
	(*tcp) = c;

	/* Save the info */
	(*ap) = a;
	(*cp) = c;

	/* Objects */
	for (o_ptr = o_list+c_ptr->o_idx, seen_obj = FALSE; o_ptr != o_list;
		o_ptr = o_list+o_ptr->next_o_idx)
	{
		/* Only show memorised objects. */
		if (!o_ptr->marked)
		{
			continue;
		}
		/* Hack -- hallucination */
		else if (p_ptr->image)
		{
			image_object(ap, cp);
			break;
		}
		else if (seen_obj)
		{
			/* Use a special stack object, if required. */
			(*cp) = object_kind_char(k_info+OBJ_STACK);
			(*ap) = object_kind_attr(k_info+OBJ_STACK);
			break;
		}
		else
		{
			/* Normal char */
			(*cp) = object_char(o_ptr);

			/* Normal attr */
			(*ap) = object_attr(o_ptr);

			/* Done, if no special stack image is being used. */
			if (!show_piles) break;

			/* One object has been seen on this square. */
			seen_obj = TRUE;
		}
	}


	/* Handle monsters */
	if (c_ptr->m_idx)
	{
		monster_type *m_ptr = &m_list[c_ptr->m_idx];

		/* Visible monster */
		if (m_ptr->ml)
		{
			monster_race *r_ptr = &r_info[m_ptr->r_idx];

			/* Desired attr */
			a = r_ptr->gfx.xa;

			/* Desired char */
			c = r_ptr->gfx.xc;

			/* Ignore weird codes */
			if (avoid_other)
			{
				/* Use char */
				(*cp) = c;

				/* Use attr */
				(*ap) = a;
			}

			/* Special attr/char codes */
			else if ((a & 0x80) && (c & 0x80))
			{
				/* Use char */
				(*cp) = c;

				/* Use attr */
				(*ap) = a;
			}


			/* Hack - allow non-clear uniques to be set uniformly violet,
			or yellow if violet normally. */
			else if (do_violet_unique(r_ptr, ap, cp));
		/* Multi-hued monster */
		else if (r_ptr->flags1 & (RF1_ATTR_MULTI))
			{
		/* Is it a shapechanger? */
		if (r_ptr->flags2 & (RF2_SHAPECHANGER))
		{
			if (use_graphics)
			{
				image_monster(ap, cp);
			}
			else
			{
				(*cp) = (randint(25)==1?
				image_object_hack[randint(strlen(image_object_hack))]:
				image_monster_hack[randint(strlen(image_monster_hack))]);
			}
		}
		else
			(*cp) = c;

				/* Multi-hued attr */
		if (r_ptr->flags2 & (RF2_ATTR_ANY))
				(*ap) = randint(15);
		else switch (randint(7))
						{  case 1:
						(*ap)=TERM_RED;
						break;
					case 2:
						(*ap)=TERM_L_RED;
						break;
					case 3:
			(*ap)=TERM_WHITE;
						break;
					case 4:
						(*ap)=TERM_L_GREEN;
						break;
					case 5:
						(*ap)=TERM_BLUE;
						break;
					case 6:
			(*ap)=TERM_L_DARK;
						break;
			case 7:
			(*ap)=TERM_GREEN;
			break;
						}
			}

			/* Normal monster (not "clear" in any way) */
			else if (!(r_ptr->flags1 & (RF1_ATTR_CLEAR | RF1_CHAR_CLEAR)))
			{
				/* Use char */
				(*cp) = c;

				/* Use attr */
				(*ap) = a;
			}

			/* Hack -- Bizarre grid under monster */
			else if ((*ap & 0x80) || (*cp & 0x80))
			{
				/* Use char */
				(*cp) = c;

				/* Use attr */
				(*ap) = a;
			}

			/* Normal */
			else
			{
				/* Normal (non-clear char) monster */
				if (!(r_ptr->flags1 & (RF1_CHAR_CLEAR)))
				{
					/* Normal char */
					(*cp) = c;
				}

				/* Normal (non-clear attr) monster */
				else if (!(r_ptr->flags1 & (RF1_ATTR_CLEAR)))
				{
					/* Normal attr */
					(*ap) = a;
				}
			}

			/* Hack -- hallucination */
			if (p_ptr->image)
			{
				/* Hallucinatory monster */
				image_monster(ap, cp);
			}
		}
	}

	/* Handle "player" */
	if ((y == py) && (x == px))
	{
		monster_race *r_ptr = &r_info[0];

		/* Get the "player" attr */
		a = r_ptr->gfx.xa;

		/* Get the "player" char */
		c = r_ptr->gfx.xc;

		/* Save the info */
		(*ap) = a;
		(*cp) = c;

	}
}

/* A factor to convert between map and screen co-ordinates. */

#define Y_SCREEN_ADJ (panel_row_prt-PRT_MINY)
#define X_SCREEN_ADJ (panel_col_prt-PRT_MINX)

/*
 * Does the (x,y) point on the map appear on-screen?
 */
static bool panel_contains_prt(int y, int x)
{
	y -= Y_SCREEN_ADJ;
	x -= X_SCREEN_ADJ;
	if (y < PRT_MINY || y >= PRT_MAXY) return FALSE;
	if (x < PRT_MINX || x >= PRT_MAXX) return FALSE;
	return TRUE;
}


/*
 * Moves the cursor to a given MAP (y,x) location
 */
void move_cursor_relative(int row, int col)
{
	/* Real co-ords convert to screen positions */
	row -= Y_SCREEN_ADJ;
	col -= X_SCREEN_ADJ;

	/* Go there */
	Term_gotoxy(col, row);
}



/*
 * Hack - provide fake monochrome under certain circumstances.
 */
static void fake_colour(byte *a)
{
	if (!allow_fake_colour) return;

	/* Graphics prevent fake colours, except with main-ibm.c. */
	if (use_graphics && strcmp(ANGBAND_SYS, "ibm")) return;

	if (p_ptr->invuln) *a = TERM_WHITE;
	else if (p_ptr->wraith_form) *a = TERM_L_DARK;
}

/*
 * Place an attr/char pair at the given map coordinate, if legal.
 */
void print_rel(char c, byte a, int y, int x)
{
	/* Only do "legal" locations */
	if (panel_contains_prt(y, x))
	{
		/* Hack -- fake monochrome */
		fake_colour(&a);

		/* Draw the char using the attr */
		Term_draw(x-X_SCREEN_ADJ, y-Y_SCREEN_ADJ, a, c);
	}
}

/*
 * Activate various external displays based on the contents of a specified
 * map square.
 */
static void highlight_map_square(const int y, const int x)
{
	cave_type *c_ptr = &cave[y][x];
	monster_type *m_ptr = m_list+c_ptr->m_idx;
	object_type *o_ptr = o_list+c_ptr->o_idx;

	/* Track a monster, if any. */
	if (c_ptr->m_idx && m_ptr->ml)
	{
		health_track(c_ptr->m_idx);
		monster_race_track(m_ptr->r_idx);
	}

	/* Track an object, if any. */
	if (c_ptr->o_idx && o_ptr->marked)
	{
		object_track(o_ptr);
	}

	/* Track the square. */
	cave_track(y, x);
}

/*
 * Highlight a square in a particular window.
 */
void highlight_square(int win, int y, int x)
{
	static int win2=-1, y2, x2;

	/* Do nothing if the cursor remains within the same square. */
	if (win == win2 && y == y2 && x == x2) return;

	/* Remember the square for next time. */
	win2 = win;
	y2 = y;
	x2 = x;

	/* If over a known section of map, display details.
	 * Note that this does not check where the map is actually visible.
	 */
	if (character_dungeon && track_mouse && win == 0)
	{
		int ys = y + Y_SCREEN_ADJ;
		int xs = x + X_SCREEN_ADJ;
		if (panel_contains_prt(ys, xs) && in_bounds2(ys, xs))
		{
			highlight_map_square(ys, xs);
		}
	}
}




/*
 * Permanently learn the identity of the feature in a square.
 */
void mark_spot(int y, int x)
{
	/* Remember the feature in this square. */
	cave[y][x].r_feat = cave[y][x].feat;

	/* Remember that the feature should be displayed. */
	cave[y][x].info |= CAVE_MARK;
}


/*
 * Memorize interesting viewable object/features in the given grid
 *
 * This function should only be called on "legal" grids.
 *
 * This function will memorize the object and/or feature in the given
 * grid, if they are (1) viewable and (2) interesting.  Note that all
 * objects are interesting, all terrain features except floors (and
 * invisible traps) are interesting, and floors (and invisible traps)
 * are interesting sometimes (depending on various options involving
 * the illumination of floor grids).
 *
 * The automatic memorization of all objects and non-floor terrain
 * features as soon as they are displayed allows incredible amounts
 * of optimization in various places, especially "map_info()".
 *
 * Note that the memorization of objects is completely separate from
 * the memorization of terrain features, preventing annoying floor
 * memorization when a detected object is picked up from a dark floor,
 * and object memorization when an object is dropped into a floor grid
 * which is memorized but out-of-sight.
 *
 * This function should be called every time the "memorization" of
 * a grid (or the object in a grid) is called into question, such
 * as when an object is created in a grid, when a terrain feature
 * "changes" from "floor" to "non-floor", when any grid becomes
 * "illuminated" or "viewable", and when a "floor" grid becomes
 * "torch-lit".
 *
 * Note the relatively efficient use of this function by the various
 * "update_view()" and "update_lite()" calls, to allow objects and
 * terrain features to be memorized (and drawn) whenever they become
 * viewable or illuminated in any way, but not when they "maintain"
 * or "lose" their previous viewability or illumination.
 *
 * Note the butchered "internal" version of "player_can_see_bold()",
 * optimized primarily for the most common cases, that is, for the
 * non-marked floor grids.
 */
void note_spot(int y, int x)
{
	cave_type *c_ptr = &cave[y][x];

	s16b this_o_idx, next_o_idx = 0;


	/* Blind players see nothing */
	if (p_ptr->blind) return;

	/* Analyze non-torch-lit grids */
	if (!(c_ptr->info & (CAVE_LITE)))
	{
		/* Require line of sight to the grid */
		if (!(c_ptr->info & (CAVE_VIEW))) return;

		/* Require "perma-lite" of the grid */
		if (!(c_ptr->info & (CAVE_GLOW))) return;
	}


	/* Hack -- memorize objects */
	for (this_o_idx = c_ptr->o_idx; this_o_idx; this_o_idx = next_o_idx)
	{
		object_type *o_ptr = &o_list[this_o_idx];

		/* Acquire next object */
		next_o_idx = o_ptr->next_o_idx;

		/* Memorize legal objects */
		if (!hidden_p(o_ptr)) o_ptr->marked = TRUE;
	}


	/* Hack -- memorise features */
	c_ptr->r_feat = c_ptr->feat;

	/* Hack -- memorize grids */
	if (!(c_ptr->info & (CAVE_MARK)))
	{
		/* Handle floor grids first */
		if (c_ptr->feat <= FEAT_INVIS)
		{
			/* Option -- memorize all torch-lit floors */
			if (view_torch_grids && (c_ptr->info & (CAVE_LITE)))
			{
				/* Memorize */
				c_ptr->info |= (CAVE_MARK);
			}

			/* Option -- memorize all perma-lit floors */
			else if (view_perma_grids && (c_ptr->info & (CAVE_GLOW)))
			{
				/* Memorize */
				c_ptr->info |= (CAVE_MARK);
			}
		}

		/* Memorize normal grids */
		else if (cave_floor_grid(c_ptr))
		{
			/* Memorize */
			c_ptr->info |= (CAVE_MARK);
		}

		/* Memorize torch-lit walls */
		else if (c_ptr->info & (CAVE_LITE))
		{
			/* Memorize */
			c_ptr->info |= (CAVE_MARK);
		}

		/* Memorize certain non-torch-lit wall grids */
		else
		{
			int yy, xx;

			/* Hack -- move one grid towards player */
			yy = (y < py) ? (y + 1) : (y > py) ? (y - 1) : y;
			xx = (x < px) ? (x + 1) : (x > px) ? (x - 1) : x;

			/* Check for "local" illumination */
			if (cave[yy][xx].info & (CAVE_GLOW))
			{
				/* Memorize */
				c_ptr->info |= (CAVE_MARK);
			}
		}
	}
}

static byte priority_map[MAX_HGT][MAX_WID];

/*
 * Redraw (on the screen) a given MAP location
 *
 * This function should only be called on "legal" grids
 */
void lite_spot(int y, int x)
{
	/* Redraw if on screen */
	if (panel_contains_prt(y, x))
	{
		byte a;
		char c;

		byte ta;
		char tc;

		/* Examine the grid */
		map_info(y, x, &a, &c, &ta, &tc);

		/* Hack -- fake monochrome */
		fake_colour(&a);

		/* Hack -- Queue it */
		Term_queue_char(x-X_SCREEN_ADJ, y-Y_SCREEN_ADJ, a, c, ta, tc);
	}

	/* Recalculate the priority later if needed. */
	priority_map[y][x] = 0;

	/* Display map on extra windows (later). */
	p_ptr->window |= PW_OVERHEAD;
}




/*
 * Prints the map of the dungeon
 *
 * Note that, for efficiency, we contain an "optimized" version
 * of both "lite_spot()" and "print_rel()", and that we use the
 * "lite_spot()" function to display the player grid, if needed.
 *
 * reset controls whether the game should reset the priority map. This is
 * usually the case, as this function redraws the map for arbitrary changes,
 * but panel changes do not require this.
 */
void prt_map(bool reset)
{
	int cx, cy;

	bool v;

	/* Access the cursor state */
	(void)Term_get_cursor(&v);

	/* Hide the cursor */
	(void)Term_set_cursor(0);

	/* Dump the map */
	for (cy = PRT_MINY; cy < PRT_MAXY; cy++)
	{
		int y = cy+Y_SCREEN_ADJ;

		/* Scan the columns of row "y" */
		for (cx = PRT_MINX; cx < PRT_MAXX; cx++)
		{
			int x = cx+X_SCREEN_ADJ;

			byte a;
			char c;

			byte ta;
			char tc;

			/* Determine what is there */
			map_info(y, x, &a, &c, &ta, &tc);

			/* Hack -- fake monochrome */
			fake_colour(&a);

			/* Efficiency -- Redraw that grid of the map */
			Term_queue_char(cx, cy, a, c, ta, tc);
		}
	}

	/* Display player */
	lite_spot(py, px);

	/* Recalculate the priority later if needed. */
	if (reset) WIPE(priority_map, priority_map);

	/* Display map on extra windows (later). */
	p_ptr->window |= PW_OVERHEAD;

	/* Restore the cursor */
	(void)Term_set_cursor(v);
}





/*
 * Hack -- a priority function (see below)
 *
 * This uses the priorities of terrians which are never shown because they are
 * mimics, as these can be capable of being shown if f_info.txt is written in
 * an obfuscated way.
 */
static int get_priority(int y, int x)
{
	char c, dc;
	byte a, da;
	int i;
	feature_type *f_ptr = f_info+f_info[cave[y][x].feat].mimic;

	/* Extract the image of the square. */
	map_info(y, x, &a, &c, &da, &dc);

	/* Use the priority of this terrain type if the terrain is visible. */
	if (f_ptr->gfx.xc == c && f_ptr->gfx.xa == a) return f_ptr->priority;

	/* Otherwise, look to see if it looks like terrain. */
	for (i = 0; i < feature_priorities; i++)
	{
		f_ptr = priority_table[i];
		if (f_ptr->gfx.xc == c && f_ptr->gfx.xa == a) return f_ptr->priority;
	}

	/* Not the char/attr of a feature, so give a known priority. */
	return 100;
}

/*
 * Deduce the grid with the highest priority (as determined above) in the
 * specified square area of the level.
 */
static void get_best_priority(byte *a, char *c,
	int minx, int miny, int maxx, int maxy)
{
	int x, y, mx, my, mpri;
	byte da[1];
	char dc[1];

	for (mpri = my = mx = 0, x = minx; x <= maxx; x++)
	{
		for (y = miny; y <= maxy; y++)
		{
			int pri = priority_map[y][x];
			if (!pri) priority_map[y][x] = pri = get_priority(y, x);

			if (pri > mpri)
			{
				mpri = pri;
				mx = x;
				my = y;
			}
		}
	}

	/* Extract the image of the "best" grid in the area. */
	map_info(my, mx, a, c, da, dc);
}

/*
 * Display a "small-scale" map of the dungeon in the active Term
 *
 * Note that the "map_info()" function must return fully colorized
 * data or this function will not work correctly.
 *
 * Note that this function must "disable" the special lighting
 * effects so that the "priority" function will work.
 *
 * Note the use of a specialized "priority" function to allow this
 * function to work with any graphic attr/char mappings, and the
 * attempts to optimize this function where possible.
 *
 * If present, cy and cx are set to the player's co-ordinates.
 * If present, my and mx are set to the bottom right corner of the map.
 */
void display_map(int *cy, int *cx, int *my, int *mx)
{
	int x, y;

	byte ta;
	char tc;

	bool old_view_special_lite;
	bool old_view_granite_lite;

	int ratio, map_hgt, map_wid;

	Term_get_size(&map_wid, &map_hgt);

	ratio = MAX(MAX((cur_wid+map_wid-1)/map_wid, (cur_hgt+map_hgt-1)/map_hgt), 1);

	map_hgt = (cur_hgt+ratio-1)/ratio + 1;
	map_wid = (cur_wid+ratio-1)/ratio + 1;

	/* Save lighting effects */
	old_view_special_lite = view_special_lite;
	old_view_granite_lite = view_granite_lite;

	/* Draw the horizontal edges */
	mc_put_fmt(0, 0, "+%v+", repeat_string_f2, "-", map_wid-1);
	mc_put_fmt(map_hgt, 0, "+%v+", repeat_string_f2, "-", map_wid-1);

	/* Disable lighting effects */
	view_special_lite = FALSE;
	view_granite_lite = FALSE;

	/* Display each map line in order */
	for (y = 1; y < map_hgt; ++y)
	{
		/* Start a new line */
		Term_putch(0, y, TERM_WHITE, '|');

		/* Display the line */
		for (x = 1; x < map_wid; ++x)
		{
			/* Find the character to print for the squares this covers. */
			get_best_priority(&ta, &tc,
				(x-1)*ratio, (y-1)*ratio, x*ratio-1, y*ratio-1);

			/* Hack -- fake monochrome */
			fake_colour(&ta);

			/* Add the character */
			Term_addch(ta, tc);
		}

		/* Add the right-hand edge. */
		Term_addch(TERM_WHITE, '|');
	}

	/* Restore lighting effects */
	view_special_lite = old_view_special_lite;
	view_granite_lite = old_view_granite_lite;

	/* Edge of map */
	if (my) (*my) = map_hgt;
	if (mx) (*mx) = map_wid;

	/* Player location */
	if (cy) (*cy) = py / ratio + 1;
	if (cx) (*cx) = px / ratio + 1;
}



/*
 * Display a "small-scale" map of the wilderness
 */
void display_wild_map(uint xmin)
{
	s16b x,y;
	int i, j, num_towns;
	uint l;
	char wild_map_symbol;
	byte wild_map_attr;
	cptr tmp;
	C_TNEW(dungeon_has_guardians, MAX_CAVES, bool);

	/* First work out which dungeons have guardians left */
	for(i=0;i<MAX_CAVES;i++)
	{
		dungeon_has_guardians[i]=FALSE;
	}
	for(i=0;i<MAX_Q_IDX;i++)
	{
		if(q_list[i].level || (q_list[i].cur_num != q_list[i].max_num))/* If the quest exists then flag its dungeon*/
		{
			dungeon_has_guardians[q_list[i].dungeon]=TRUE;
		}
	}
	/* Now print out the colour-coded map */
	for(y=0;y<12;y++)
	{
		for(x=0;x<12;x++)
		{
			wild_type *w_ptr = &wild_grid[y][x];

			if((wildx == x) && (wildy == y))
			{
				wild_map_symbol = '@';
				wild_map_attr = TERM_YELLOW;
			}
			else if (w_ptr->dungeon < MAX_CAVES)
			{
				wild_map_symbol = dun_defs[w_ptr->dungeon].sym;
				if(dungeon_has_guardians[w_ptr->dungeon])
				{
					wild_map_attr = TERM_RED;
				}
				else if (is_town_p(y, x))
				{
					wild_map_attr = TERM_WHITE;
				}
				else
				{
					wild_map_attr = TERM_UMBER;
				}
			}
			else if((x == 0) || (y == 0) || (x == 11) || (y == 11))
			{
				wild_map_symbol = '~';
				wild_map_attr = TERM_BLUE;
			}
			else
			{
				wild_map_symbol = '^';
				wild_map_attr = TERM_GREEN;
			}
			Term_putch(xmin+x+1, y+2, wild_map_attr, wild_map_symbol);
		}
	}
	/* Print border */
	put_str("+------------+",1,xmin);
	for(y=0;y<12;y++)
	{
		put_str("|",y+2,xmin);
		put_str("|",y+2,xmin+13);
	}
	put_str("+------------+",14,xmin);

	/* Find the longest legend. */
	for (l = i = 0; i < MAX_TOWNS; i++)
	{
		l = MAX(l, strlen(town_name+town_defs[i].name));
	}

	/* Print legend */
	for (num_towns=i=0;i<MAX_TOWNS;i++)
	{
		dun_type *d_ptr = dun_defs+i;
		town_type *t_ptr = town_defs+i;

		/* Not a town. */
		if (!t_ptr->name) continue;

		if (l+xmin+23 > Term->wid)
			tmp = dun_name+d_ptr->shortname;
		else
			tmp = town_name+t_ptr->name;
		c_put_str(TERM_WHITE, format("%c = %s", d_ptr->sym, tmp),
			num_towns+1,xmin+19);
		num_towns++;
	}

	/* Print dungeon legend */
	for (i = j = 0; i < MAX_CAVES; i++)
	{
		dun_type *d_ptr = dun_defs+i;

		/* Already shown. */
		if (is_town_p(d_ptr->y, d_ptr->x)) continue;

		x = j % 2;
		x = (xmin+(x*Term->wid))/(1+x);
		y = MAX(16, num_towns+1) + j/2;

		tmp = dun_name+d_ptr->name;

		/* Trim an initial "the ". */
		if (prefix(tmp, "the ")) tmp += strlen("the ");

		/* If the name is too long, use the short name instead. */
		if ((strlen(tmp)+4)*2+xmin >= Term->wid)
			tmp = format("%c = %s", d_ptr->sym, dun_name+d_ptr->shortname);
		else
			tmp = format("%c = %s", d_ptr->sym, tmp);
		c_put_str(TERM_WHITE, tmp, y, x);

		j++;
	}

	c_put_str(TERM_UMBER,"* = dungeon entrance",num_towns+2,xmin+19);
	tmp = "(Places that still have guardians are marked in red)";
	if (xmin+strlen(tmp)+18 > Term->wid)
		tmp = "(red = has guardian)";
	c_put_str(TERM_RED,tmp,num_towns+4,xmin+18);
	c_put_str(TERM_YELLOW,"@ = you",num_towns+6,xmin+18);

	TFREE(dungeon_has_guardians);
}

/*
 * Display a "small-scale" map of the dungeon for the player
 *
 * Currently, the "player" is displayed on the map.  XXX XXX XXX
 */
void do_cmd_view_map(void)
{
	cptr str;
	int cy, cx, my, mx;

	/* Enter "icky" mode */
	character_icky = TRUE;

	/* Save the screen */
	Term_save();

	/* Note */
	prt("Please wait...", 0, 0);

	/* Flush */
	Term_fresh();

	/* Clear the screen */
	Term_clear();

	/* Display the map */
	if (dun_level == 0)
	{
		display_wild_map(1);
		Term_get_size(&mx, &my);
		my--;
		cy = -1;
	}
	else
	{
		display_map(&cy, &cx, &my, &mx);
	}

	/* Wait for it */
	str = "Hit any key to continue";
	put_str(str, my, (mx-strlen(str))/2);

	/* Hilite the player */
	if (cy >= 0) move_cursor(cy, cx);

	/* Get any key */
	inkey();

	/* Restore the screen */
	Term_load();

	/* Leave "icky" mode */
	character_icky = FALSE;
}





/*
 * Some comments on the cave grid flags.  -BEN-
 *
 *
 * One of the major bottlenecks in previous versions of Angband was in
 * the calculation of "line of sight" from the player to various grids,
 * such as monsters.  This was such a nasty bottleneck that a lot of
 * silly things were done to reduce the dependancy on "line of sight",
 * for example, you could not "see" any grids in a lit room until you
 * actually entered the room, and there were all kinds of bizarre grid
 * flags to enable this behavior.  This is also why the "call light"
 * spells always lit an entire room.
 *
 * The code below provides functions to calculate the "field of view"
 * for the player, which, once calculated, provides extremely fast
 * calculation of "line of sight from the player", and to calculate
 * the "field of torch lite", which, again, once calculated, provides
 * extremely fast calculation of "which grids are lit by the player's
 * lite source".  In addition to marking grids as "GRID_VIEW" and/or
 * "GRID_LITE", as appropriate, these functions maintain an array for
 * each of these two flags, each array containing the locations of all
 * of the grids marked with the appropriate flag, which can be used to
 * very quickly scan through all of the grids in a given set.
 *
 * To allow more "semantically valid" field of view semantics, whenever
 * the field of view (or the set of torch lit grids) changes, all of the
 * grids in the field of view (or the set of torch lit grids) are "drawn"
 * so that changes in the world will become apparent as soon as possible.
 * This has been optimized so that only grids which actually "change" are
 * redrawn, using the "temp" array and the "GRID_TEMP" flag to keep track
 * of the grids which are entering or leaving the relevent set of grids.
 *
 * These new methods are so efficient that the old nasty code was removed.
 *
 * Note that there is no reason to "update" the "viewable space" unless
 * the player "moves", or walls/doors are created/destroyed, and there
 * is no reason to "update" the "torch lit grids" unless the field of
 * view changes, or the "light radius" changes.  This means that when
 * the player is resting, or digging, or doing anything that does not
 * involve movement or changing the state of the dungeon, there is no
 * need to update the "view" or the "lite" regions, which is nice.
 *
 * Note that the calls to the nasty "los()" function have been reduced
 * to a bare minimum by the use of the new "field of view" calculations.
 *
 * I wouldn't be surprised if slight modifications to the "update_view()"
 * function would allow us to determine "reverse line-of-sight" as well
 * as "normal line-of-sight", which would allow monsters to use a more
 * "correct" calculation to determine if they can "see" the player.  For
 * now, monsters simply "cheat" somewhat and assume that if the player
 * has "line of sight" to the monster, then the monster can "pretend"
 * that it has "line of sight" to the player.
 *
 *
 * The "update_lite()" function maintains the "CAVE_LITE" flag for each
 * grid and maintains an array of all "CAVE_LITE" grids.
 *
 * This set of grids is the complete set of all grids which are lit by
 * the players light source, which allows the "player_can_see_bold()"
 * function to work very quickly.
 *
 * Note that every "CAVE_LITE" grid is also a "CAVE_VIEW" grid, and in
 * fact, the player (unless blind) can always "see" all grids which are
 * marked as "CAVE_LITE", unless they are "off screen".
 *
 *
 * The "update_view()" function maintains the "CAVE_VIEW" flag for each
 * grid and maintains an array of all "CAVE_VIEW" grids.
 *
 * This set of grids is the complete set of all grids within line of sight
 * of the player, allowing the "player_has_los_bold()" macro to work very
 * quickly.
 *
 *
 * The current "update_view()" algorithm uses the "CAVE_XTRA" flag as a
 * temporary internal flag to mark those grids which are not only in view,
 * but which are also "easily" in line of sight of the player.  This flag
 * is always cleared when we are done.
 *
 *
 * The current "update_lite()" and "update_view()" algorithms use the
 * "CAVE_TEMP" flag, and the array of grids which are marked as "CAVE_TEMP",
 * to keep track of which grids were previously marked as "CAVE_LITE" or
 * "CAVE_VIEW", which allows us to optimize the "screen updates".
 *
 * The "CAVE_TEMP" flag, and the array of "CAVE_TEMP" grids, is also used
 * for various other purposes, such as spreading lite or darkness during
 * "lite_room()" / "unlite_room()", and for calculating monster flow.
 *
 *
 * Any grid can be marked as "CAVE_GLOW" which means that the grid itself is
 * in some way permanently lit.  However, for the player to "see" anything
 * in the grid, as determined by "player_can_see()", the player must not be
 * blind, the grid must be marked as "CAVE_VIEW", and, in addition, "wall"
 * grids, even if marked as "perma lit", are only illuminated if they touch
 * a grid which is not a wall and is marked both "CAVE_GLOW" and "CAVE_VIEW".
 *
 *
 * To simplify various things, a grid may be marked as "CAVE_MARK", meaning
 * that even if the player cannot "see" the grid, he "knows" the terrain in
 * that grid.  This is used to "remember" walls/doors/stairs/floors when they
 * are "seen" or "detected", and also to "memorize" floors, after "wiz_lite()",
 * or when one of the "memorize floor grids" options induces memorization.
 *
 * Objects are "memorized" in a different way, using a special "marked" flag
 * on the object itself, which is set when an object is observed or detected.
 *
 *
 * A grid may be marked as "CAVE_ROOM" which means that it is part of a "room",
 * and should be illuminated by "lite room" and "darkness" spells.
 *
 *
 * A grid may be marked as "CAVE_ICKY" which means it is part of a "vault",
 * and should be unavailable for "teleportation" destinations.
 *
 *
 * The "view_perma_grids" allows the player to "memorize" every perma-lit grid
 * which is observed, and the "view_torch_grids" allows the player to memorize
 * every torch-lit grid.  The player will always memorize important walls,
 * doors, stairs, and other terrain features, as well as any "detected" grids.
 *
 * Note that the new "update_view()" method allows, among other things, a room
 * to be "partially" seen as the player approaches it, with a growing cone of
 * floor appearing as the player gets closer to the door.  Also, by not turning
 * on the "memorize perma-lit grids" option, the player will only "see" those
 * floor grids which are actually in line of sight.
 *
 * And my favorite "plus" is that you can now use a special option to draw the
 * "floors" in the "viewable region" brightly (actually, to draw the *other*
 * grids dimly), providing a "pretty" effect as the player runs around, and
 * to efficiently display the "torch lite" in a special color.
 *
 *
 * Some comments on the "update_view()" algorithm...
 *
 * The algorithm is very fast, since it spreads "obvious" grids very quickly,
 * and only has to call "los()" on the borderline cases.  The major axes/diags
 * even terminate early when they hit walls.  I need to find a quick way
 * to "terminate" the other scans.
 *
 * Note that in the worst case (a big empty area with say 5% scattered walls),
 * each of the 1500 or so nearby grids is checked once, most of them getting
 * an "instant" rating, and only a small portion requiring a call to "los()".
 *
 * The only time that the algorithm appears to be "noticeably" too slow is
 * when running, and this is usually only important in town, since the town
 * provides about the worst scenario possible, with large open regions and
 * a few scattered obstructions.  There is a special "efficiency" option to
 * allow the player to reduce his field of view in town, if needed.
 *
 * In the "best" case (say, a normal stretch of corridor), the algorithm
 * makes one check for each viewable grid, and makes no calls to "los()".
 * So running in corridors is very fast, and if a lot of monsters are
 * nearby, it is much faster than the old methods.
 *
 * Note that resting, most normal commands, and several forms of running,
 * plus all commands executed near large groups of monsters, are strictly
 * more efficient with "update_view()" that with the old "compute los() on
 * demand" method, primarily because once the "field of view" has been
 * calculated, it does not have to be recalculated until the player moves
 * (or a wall or door is created or destroyed).
 *
 * Note that we no longer have to do as many "los()" checks, since once the
 * "view" region has been built, very few things cause it to be "changed"
 * (player movement, and the opening/closing of doors, changes in wall status).
 * Note that door/wall changes are only relevant when the door/wall itself is
 * in the "view" region.
 *
 * The algorithm seems to only call "los()" from zero to ten times, usually
 * only when coming down a corridor into a room, or standing in a room, just
 * misaligned with a corridor.  So if, say, there are five "nearby" monsters,
 * we will be reducing the calls to "los()".
 *
 * I am thinking in terms of an algorithm that "walks" from the central point
 * out to the maximal "distance", at each point, determining the "view" code
 * (above).  For each grid not on a major axis or diagonal, the "view" code
 * depends on the "cave_floor_bold()" and "view" of exactly two other grids
 * (the one along the nearest diagonal, and the one next to that one, see
 * "update_view_aux()"...).
 *
 * We "memorize" the viewable space array, so that at the cost of under 3000
 * bytes, we reduce the time taken by "forget_view()" to one assignment for
 * each grid actually in the "viewable space".  And for another 3000 bytes,
 * we prevent "erase + redraw" ineffiencies via the "seen" set.  These bytes
 * are also used by other routines, thus reducing the cost to almost nothing.
 *
 * A similar thing is done for "forget_lite()" in which case the savings are
 * much less, but save us from doing bizarre maintenance checking.
 *
 * In the worst "normal" case (in the middle of the town), the reachable space
 * actually reaches to more than half of the largest possible "circle" of view,
 * or about 800 grids, and in the worse case (in the middle of a dungeon level
 * where all the walls have been removed), the reachable space actually reaches
 * the theoretical maximum size of just under 1500 grids.
 *
 * Each grid G examines the "state" of two (?) other (adjacent) grids, G1 & G2.
 * If G1 is lite, G is lite.  Else if G2 is lite, G is half.  Else if G1 and G2
 * are both half, G is half.  Else G is dark.  It only takes 2 (or 4) bits to
 * "name" a grid, so (for MAX_RAD of 20) we could use 1600 bytes, and scan the
 * entire possible space (including initialization) in one step per grid.  If
 * we do the "clearing" as a separate step (and use an array of "view" grids),
 * then the clearing will take as many steps as grids that were viewed, and the
 * algorithm will be able to "stop" scanning at various points.
 * Oh, and outside of the "torch radius", only "lite" grids need to be scanned.
 */

/*
 * Array of grids lit by player lite.
 */
static s16b lite_n;
static byte lite_y[LITE_MAX];
static byte lite_x[LITE_MAX];

/*
 * Actually erase the entire "lite" array, redrawing every grid
 */
void forget_lite(void)
{
	int i, x, y;

	/* None to forget */
	if (!lite_n) return;

	/* Clear them all */
	for (i = 0; i < lite_n; i++)
	{
		y = lite_y[i];
		x = lite_x[i];

		/* Forget "LITE" flag */
		cave[y][x].info &= ~(CAVE_LITE);

		/* Redraw */
		lite_spot(y, x);
	}

	/* None left */
	lite_n = 0;
}


/*
 * XXX XXX XXX
 *
 * This macro allows us to efficiently add a grid to the "lite" array,
 * note that we are never called for illegal grids, or for grids which
 * have already been placed into the "lite" array, and we are never
 * called when the "lite" array is full.
 */
#define cave_lite_hack(Y,X) \
	cave[Y][X].info |= (CAVE_LITE); \
	lite_y[lite_n] = (Y); \
	lite_x[lite_n] = (X); \
	lite_n++



/*
 * Update the set of grids "illuminated" by the player's lite.
 *
 * This routine needs to use the results of "update_view()"
 *
 * Note that "blindness" does NOT affect "torch lite".  Be careful!
 *
 * We optimize most lites (all non-artifact lites) by using "obvious"
 * facts about the results of "small" lite radius, and we attempt to
 * list the "nearby" grids before the more "distant" ones in the
 * array of torch-lit grids.
 *
 * We will correctly handle "large" radius lites, though currently,
 * it is impossible for the player to have more than radius 3 lite.
 *
 * We assume that "radius zero" lite is in fact no lite at all.
 *
 *     Torch     Lantern     Artifacts
 *     (etc)
 *                              ***
 *                 ***         *****
 *      ***       *****       *******
 *      *@*       **@**       ***@***
 *      ***       *****       *******
 *                 ***         *****
 *                              ***
 */
void update_lite(void)
{
	int i, x, y, min_x, max_x, min_y, max_y;


	/*** Special case ***/

	/* Hack -- Player has no lite */
	if (p_ptr->cur_lite <= 0)
	{
		/* Forget the old lite */
		forget_lite();

		/* Draw the player */
		lite_spot(py, px);

		/* All done */
		return;
	}


	/*** Save the old "lite" grids for later ***/

	/* Clear them all */
	for (i = 0; i < lite_n; i++)
	{
		y = lite_y[i];
		x = lite_x[i];

		/* Mark the grid as not "lite" */
		cave[y][x].info &= ~(CAVE_LITE);

		/* Mark the grid as "seen" */
		cave[y][x].info |= (CAVE_TEMP);

		/* Add it to the "seen" set */
		temp_y[temp_n] = y;
		temp_x[temp_n] = x;
		temp_n++;
	}

	/* None left */
	lite_n = 0;


	/*** Collect the new "lite" grids ***/

	/* Player grid */
	cave_lite_hack((byte)py,(byte) px);

	/* Radius 1 -- torch radius */
	if (p_ptr->cur_lite >= 1)
	{
		/* Adjacent grid */
		cave_lite_hack((byte)py+1,(byte) px);
		cave_lite_hack((byte)py-1,(byte) px);
		cave_lite_hack((byte)py,(byte) px+1);
		cave_lite_hack((byte)py,(byte) px-1);

		/* Diagonal grids */
		cave_lite_hack((byte)py+1,(byte) px+1);
		cave_lite_hack((byte)py+1,(byte) px-1);
		cave_lite_hack((byte)py-1,(byte) px+1);
		cave_lite_hack((byte)py-1,(byte) px-1);
	}

	/* Radius 2 -- lantern radius */
	if (p_ptr->cur_lite >= 2)
	{
		/* South of the player */
		if (cave_floor_bold(py+1, px))
		{
			cave_lite_hack((byte)py+2,(byte) px);
			cave_lite_hack((byte)py+2,(byte) px+1);
			cave_lite_hack((byte)py+2,(byte) px-1);
		}

		/* North of the player */
		if (cave_floor_bold((byte)py-1,(byte) px))
		{
			cave_lite_hack((byte)py-2,(byte) px);
			cave_lite_hack((byte)py-2,(byte) px+1);
			cave_lite_hack((byte)py-2,(byte) px-1);
		}

		/* East of the player */
		if (cave_floor_bold(py, px+1))
		{
			cave_lite_hack((byte)py,(byte) px+2);
			cave_lite_hack((byte)py+1,(byte) px+2);
			cave_lite_hack((byte)py-1,(byte) px+2);
		}

		/* West of the player */
		if (cave_floor_bold(py, px-1))
		{
			cave_lite_hack((byte)py,(byte) px-2);
			cave_lite_hack((byte)py+1,(byte) px-2);
			cave_lite_hack((byte)py-1,(byte) px-2);
		}
	}

	/* Radius 3+ -- artifact radius */
	if (p_ptr->cur_lite >= 3)
	{
		int d, p;

		/* Maximal radius */
		p = p_ptr->cur_lite;

		/* Paranoia -- see "LITE_MAX" */
		if (p > 5) p = 5;

		/* South-East of the player */
		if (cave_floor_bold(py+1, px+1))
		{
			cave_lite_hack(py+2, px+2);
		}

		/* South-West of the player */
		if (cave_floor_bold(py+1, px-1))
		{
			cave_lite_hack(py+2, px-2);
		}

		/* North-East of the player */
		if (cave_floor_bold(py-1, px+1))
		{
			cave_lite_hack(py-2, px+2);
		}

		/* North-West of the player */
		if (cave_floor_bold(py-1, px-1))
		{
			cave_lite_hack(py-2, px-2);
		}

		/* Maximal north */
		min_y = py - p;
		if (min_y < 0) min_y = 0;

		/* Maximal south */
		max_y = py + p;
		if (max_y > cur_hgt-1) max_y = cur_hgt-1;

		/* Maximal west */
		min_x = px - p;
		if (min_x < 0) min_x = 0;

		/* Maximal east */
		max_x = px + p;
		if (max_x > cur_wid-1) max_x = cur_wid-1;

		/* Scan the maximal box */
		for (y = min_y; y <= max_y; y++)
		{
			for (x = min_x; x <= max_x; x++)
			{
				int dy = (py > y) ? (py - y) : (y - py);
				int dx = (px > x) ? (px - x) : (x - px);

				/* Skip the "central" grids (above) */
				if ((dy <= 2) && (dx <= 2)) continue;

				/* Hack -- approximate the distance */
				d = (dy > dx) ? (dy + (dx>>1)) : (dx + (dy>>1));

				/* Skip distant grids */
				if (d > p) continue;

				/* Viewable, nearby, grids get "torch lit" */
				if (player_has_los_bold(y, x))
				{
					/* This grid is "torch lit" */
					cave_lite_hack(y, x);
				}
			}
		}
	}


	/*** Complete the algorithm ***/

	/* Draw the new grids */
	for (i = 0; i < lite_n; i++)
	{
		y = lite_y[i];
		x = lite_x[i];

		/* Update fresh grids */
		if (cave[y][x].info & (CAVE_TEMP)) continue;

		/* Note */
		note_spot(y, x);

		/* Redraw */
		lite_spot(y, x);
	}

	/* Clear them all */
	for (i = 0; i < temp_n; i++)
	{
		y = temp_y[i];
		x = temp_x[i];

		/* No longer in the array */
		cave[y][x].info &= ~(CAVE_TEMP);

		/* Update stale grids */
		if (cave[y][x].info & (CAVE_LITE)) continue;

		/* Redraw */
		lite_spot(y, x);
	}

	/* None left */
	temp_n = 0;
}






/*
 * Array of grids viewable to the player
 */
static s16b view_n;
static byte view_y[VIEW_MAX];
static byte view_x[VIEW_MAX];


/*
 * Clear the viewable space
 */
void forget_view(void)
{
	int i;

	cave_type *c_ptr;

	/* None to forget */
	if (!view_n) return;

	/* Clear them all */
	for (i = 0; i < view_n; i++)
	{
		int y = view_y[i];
		int x = view_x[i];

		/* Access the grid */
		c_ptr = &cave[y][x];

		/* Forget that the grid is viewable */
		c_ptr->info &= ~(CAVE_VIEW);

		/* Update the screen */
		lite_spot(y, x);
	}

	/* None left */
	view_n = 0;
}



/*
 * This macro allows us to efficiently add a grid to the "view" array,
 * note that we are never called for illegal grids, or for grids which
 * have already been placed into the "view" array, and we are never
 * called when the "view" array is full.
 */
#define cave_view_hack(C,Y,X) \
	(C)->info |= (CAVE_VIEW); \
	view_y[view_n] = (Y); \
	view_x[view_n] = (X); \
	view_n++



/*
 * Helper function for "update_view()" below
 *
 * We are checking the "viewability" of grid (y,x) by the player.
 *
 * This function assumes that (y,x) is legal (i.e. on the map).
 *
 * Grid (y1,x1) is on the "diagonal" between (py,px) and (y,x)
 * Grid (y2,x2) is "adjacent", also between (py,px) and (y,x).
 *
 * Note that we are using the "CAVE_XTRA" field for marking grids as
 * "easily viewable".  This bit is cleared at the end of "update_view()".
 *
 * This function adds (y,x) to the "viewable set" if necessary.
 *
 * This function now returns "TRUE" if vision is "blocked" by grid (y,x).
 */
static bool update_view_aux(int y, int x, int y1, int x1, int y2, int x2)
{
	bool f1, f2, v1, v2, z1, z2, wall;

	cave_type *c_ptr;

	cave_type *g1_c_ptr;
	cave_type *g2_c_ptr;


	/* Access the grids */
	g1_c_ptr = &cave[y1][x1];
	g2_c_ptr = &cave[y2][x2];


	/* Check for walls */
	f1 = (cave_floor_grid(g1_c_ptr));
	f2 = (cave_floor_grid(g2_c_ptr));

	/* Totally blocked by physical walls */
	if (!f1 && !f2) return (TRUE);


	/* Check for visibility */
	v1 = (f1 && (g1_c_ptr->info & (CAVE_VIEW)));
	v2 = (f2 && (g2_c_ptr->info & (CAVE_VIEW)));

	/* Totally blocked by "unviewable neighbors" */
	if (!v1 && !v2) return (TRUE);


	/* Access the grid */
	c_ptr = &cave[y][x];


	/* Check for walls */
	wall = (!cave_floor_grid(c_ptr));


	/* Check the "ease" of visibility */
	z1 = (v1 && (g1_c_ptr->info & (CAVE_XTRA)));
	z2 = (v2 && (g2_c_ptr->info & (CAVE_XTRA)));

	/* Hack -- "easy" plus "easy" yields "easy" */
	if (z1 && z2)
	{
		c_ptr->info |= (CAVE_XTRA);

		cave_view_hack(c_ptr, y, x);

		return (wall);
	}

	/* Hack -- primary "easy" yields "viewed" */
	if (z1)
	{
		cave_view_hack(c_ptr, y, x);

		return (wall);
	}


	/* Hack -- "view" plus "view" yields "view" */
	if (v1 && v2)
	{
		/* c_ptr->info |= (CAVE_XTRA); */

		cave_view_hack(c_ptr, y, x);

		return (wall);
	}


	/* Mega-Hack -- the "los()" function works poorly on walls */
	if (wall)
	{
		cave_view_hack(c_ptr, y, x);

		return (wall);
	}


	/* Hack -- check line of sight */
	if (los(py, px, y, x))
	{
		cave_view_hack(c_ptr, y, x);

		return (wall);
	}


	/* Assume no line of sight. */
	return (TRUE);
}



/*
 * Calculate the viewable space
 *
 *  1: Process the player
 *  1a: The player is always (easily) viewable
 *  2: Process the diagonals
 *  2a: The diagonals are (easily) viewable up to the first wall
 *  2b: But never go more than 2/3 of the "full" distance
 *  3: Process the main axes
 *  3a: The main axes are (easily) viewable up to the first wall
 *  3b: But never go more than the "full" distance
 *  4: Process sequential "strips" in each of the eight octants
 *  4a: Each strip runs along the previous strip
 *  4b: The main axes are "previous" to the first strip
 *  4c: Process both "sides" of each "direction" of each strip
 *  4c1: Each side aborts as soon as possible
 *  4c2: Each side tells the next strip how far it has to check
 *
 * Note that the octant processing involves some pretty interesting
 * observations involving when a grid might possibly be viewable from
 * a given grid, and on the order in which the strips are processed.
 *
 * Note the use of the mathematical facts shown below, which derive
 * from the fact that (1 < sqrt(2) < 1.5), and that the length of the
 * hypotenuse of a right triangle is primarily determined by the length
 * of the longest side, when one side is small, and is strictly less
 * than one-and-a-half times as long as the longest side when both of
 * the sides are large.
 *
 *   if (manhatten(dy,dx) < R) then (hypot(dy,dx) < R)
 *   if (manhatten(dy,dx) > R*3/2) then (hypot(dy,dx) > R)
 *
 *   hypot(dy,dx) is approximated by (dx+dy+MAX(dx,dy)) / 2
 *
 * These observations are important because the calculation of the actual
 * value of "hypot(dx,dy)" is extremely expensive, involving square roots,
 * while for small values (up to about 20 or so), the approximations above
 * are correct to within an error of at most one grid or so.
 *
 * Observe the use of "full" and "over" in the code below, and the use of
 * the specialized calculation involving "limit", all of which derive from
 * the observations given above.  Basically, we note that the "circle" of
 * view is completely contained in an "octagon" whose bounds are easy to
 * determine, and that only a few steps are needed to derive the actual
 * bounds of the circle given the bounds of the octagon.
 *
 * Note that by skipping all the grids in the corners of the octagon, we
 * place an upper limit on the number of grids in the field of view, given
 * that "full" is never more than 20.  Of the 1681 grids in the "square" of
 * view, only about 1475 of these are in the "octagon" of view, and even
 * fewer are in the "circle" of view, so 1500 or 1536 is more than enough
 * entries to completely contain the actual field of view.
 *
 * Note also the care taken to prevent "running off the map".  The use of
 * explicit checks on the "validity" of the "diagonal", and the fact that
 * the loops are never allowed to "leave" the map, lets "update_view_aux()"
 * use the optimized "cave_floor_bold()" macro, and to avoid the overhead
 * of multiple checks on the validity of grids.
 *
 * Note the "optimizations" involving the "se","sw","ne","nw","es","en",
 * "ws","wn" variables.  They work like this: While travelling down the
 * south-bound strip just to the east of the main south axis, as soon as
 * we get to a grid which does not "transmit" viewing, if all of the strips
 * preceding us (in this case, just the main axis) had terminated at or before
 * the same point, then we can stop, and reset the "max distance" to ourself.
 * So, each strip (named by major axis plus offset, thus "se" in this case)
 * maintains a "blockage" variable, initialized during the main axis step,
 * and checks it whenever a blockage is observed.  After processing each
 * strip as far as the previous strip told us to process, the next strip is
 * told not to go farther than the current strip's farthest viewable grid,
 * unless open space is still available.  This uses the "k" variable.
 *
 * Note the use of "inline" macros for efficiency.  The "cave_floor_grid()"
 * macro is a replacement for "cave_floor_bold()" which takes a pointer to
 * a cave grid instead of its location.  The "cave_view_hack()" macro is a
 * chunk of code which adds the given location to the "view" array if it
 * is not already there, using both the actual location and a pointer to
 * the cave grid.  See above.
 *
 * By the way, the purpose of this code is to reduce the dependancy on the
 * "los()" function which is slow, and, in some cases, not very accurate.
 *
 * It is very possible that I am the only person who fully understands this
 * function, and for that I am truly sorry, but efficiency was very important
 * and the "simple" version of this function was just not fast enough.  I am
 * more than willing to replace this function with a simpler one, if it is
 * equally efficient, and especially willing if the new function happens to
 * derive "reverse-line-of-sight" at the same time, since currently monsters
 * just use an optimized hack of "you see me, so I see you", and then use the
 * actual "projectable()" function to check spell attacks.
 */
void update_view(void)
{
	int n, m, d, k, y, x, z;

	int se, sw, ne, nw, es, en, ws, wn;

	int full, over;

	int y_max = cur_hgt - 1;
	int x_max = cur_wid - 1;

	cave_type *c_ptr;


	/*** Initialize ***/

	/* Optimize */
	if (view_reduce_view && (dun_level <= 0))
	{
		/* Full radius (10) */
		full = MAX_SIGHT / 2;

		/* Octagon factor (15) */
		over = MAX_SIGHT * 3 / 4;
	}

	/* Normal */
	else
	{
		/* Full radius (20) */
		full = MAX_SIGHT;

		/* Octagon factor (30) */
		over = MAX_SIGHT * 3 / 2;
	}


	/*** Step 0 -- Begin ***/

	/* Save the old "view" grids for later */
	for (n = 0; n < view_n; n++)
	{
		y = view_y[n];
		x = view_x[n];

		/* Access the grid */
		c_ptr = &cave[y][x];

		/* Mark the grid as not in "view" */
		c_ptr->info &= ~(CAVE_VIEW);

		/* Mark the grid as "seen" */
		c_ptr->info |= (CAVE_TEMP);

		/* Add it to the "seen" set */
		temp_y[temp_n] = y;
		temp_x[temp_n] = x;
		temp_n++;
	}

	/* Start over with the "view" array */
	view_n = 0;


	/*** Step 1 -- adjacent grids ***/

	/* Now start on the player */
	y = py;
	x = px;

	/* Access the grid */
	c_ptr = &cave[y][x];

	/* Assume the player grid is easily viewable */
	c_ptr->info |= (CAVE_XTRA);

	/* Assume the player grid is viewable */
	cave_view_hack(c_ptr, y, x);


	/*** Step 2 -- Major Diagonals ***/

	/* Hack -- Limit */
	z = full * 2 / 3;

	/* Scan south-east */
	for (d = 1; d <= z; d++)
	{
		c_ptr = &cave[y+d][x+d];
		c_ptr->info |= (CAVE_XTRA);
		cave_view_hack(c_ptr, y+d, x+d);
		if (!cave_floor_grid(c_ptr)) break;
		if(!in_bounds2(y+d,x+d)) break;
	}

	/* Scan south-west */
	for (d = 1; d <= z; d++)
	{
		c_ptr = &cave[y+d][x-d];
		c_ptr->info |= (CAVE_XTRA);
		cave_view_hack(c_ptr, y+d, x-d);
		if (!cave_floor_grid(c_ptr)) break;
		if(!in_bounds2(y+d,x-d)) break;
	}

	/* Scan north-east */
	for (d = 1; d <= z; d++)
	{
		c_ptr = &cave[y-d][x+d];
		c_ptr->info |= (CAVE_XTRA);
		cave_view_hack(c_ptr, y-d, x+d);
		if (!cave_floor_grid(c_ptr)) break;
		if(!in_bounds2(y-d,x+d)) break;
	}

	/* Scan north-west */
	for (d = 1; d <= z; d++)
	{
		c_ptr = &cave[y-d][x-d];
		c_ptr->info |= (CAVE_XTRA);
		cave_view_hack(c_ptr, y-d, x-d);
		if (!cave_floor_grid(c_ptr)) break;
		if(!in_bounds2(y-d,x-d)) break;
	}


	/*** Step 3 -- major axes ***/

	/* Scan south */
	for (d = 1; d <= full; d++)
	{
		c_ptr = &cave[y+d][x];
		c_ptr->info |= (CAVE_XTRA);
		cave_view_hack(c_ptr, y+d, x);
		if (!cave_floor_grid(c_ptr)) break;
		if(!in_bounds2(y+d,x)) break;
	}

	/* Initialize the "south strips" */
	se = sw = d;

	/* Scan north */
	for (d = 1; d <= full; d++)
	{
		c_ptr = &cave[y-d][x];
		c_ptr->info |= (CAVE_XTRA);
		cave_view_hack(c_ptr, y-d, x);
		if (!cave_floor_grid(c_ptr)) break;
		if(!in_bounds2(y-d,x)) break;
	}

	/* Initialize the "north strips" */
	ne = nw = d;

	/* Scan east */
	for (d = 1; d <= full; d++)
	{
		c_ptr = &cave[y][x+d];
		c_ptr->info |= (CAVE_XTRA);
		cave_view_hack(c_ptr, y, x+d);
		if (!cave_floor_grid(c_ptr)) break;
		if(!in_bounds2(y,x+d)) break;
	}

	/* Initialize the "east strips" */
	es = en = d;

	/* Scan west */
	for (d = 1; d <= full; d++)
	{
		c_ptr = &cave[y][x-d];
		c_ptr->info |= (CAVE_XTRA);
		cave_view_hack(c_ptr, y, x-d);
		if (!cave_floor_grid(c_ptr)) break;
		if(!in_bounds2(y,x-d)) break;
	}

	/* Initialize the "west strips" */
	ws = wn = d;


	/*** Step 4 -- Divide each "octant" into "strips" ***/

	/* Now check each "diagonal" (in parallel) */
	for (n = 1; n <= over / 2; n++)
	{
		int ypn, ymn, xpn, xmn;


		/* Acquire the "bounds" of the maximal circle */
		z = over - n - n;
		if (z > full - n) z = full - n;
		while ((z + n + (n>>1)) > full) z--;


		/* Access the four diagonal grids */
		ypn = y + n;
		ymn = y - n;
		xpn = x + n;
		xmn = x - n;


		/* South strip */
		if (ypn < y_max)
		{
			/* Maximum distance */
			m = MIN(z, y_max - ypn);

			/* East side */
			if ((xpn <= x_max) && (n < se))
			{
				/* Scan */
				for (k = n, d = 1; d <= m; d++)
				{
					/* Check grid "d" in strip "n", notice "blockage" */
					if (update_view_aux(ypn+d, xpn, ypn+d-1, xpn-1, ypn+d-1, xpn))
					{
						if (n + d >= se) break;
					}

					/* Track most distant "non-blockage" */
					else
					{
						k = n + d;
					}
				}

				/* Limit the next strip */
				se = k + 1;
			}

			/* West side */
			if ((xmn >= 0) && (n < sw))
			{
				/* Scan */
				for (k = n, d = 1; d <= m; d++)
				{
					/* Check grid "d" in strip "n", notice "blockage" */
					if (update_view_aux(ypn+d, xmn, ypn+d-1, xmn+1, ypn+d-1, xmn))
					{
						if (n + d >= sw) break;
					}

					/* Track most distant "non-blockage" */
					else
					{
						k = n + d;
					}
				}

				/* Limit the next strip */
				sw = k + 1;
			}
		}


		/* North strip */
		if (ymn > 0)
		{
			/* Maximum distance */
			m = MIN(z, ymn);

			/* East side */
			if ((xpn <= x_max) && (n < ne))
			{
				/* Scan */
				for (k = n, d = 1; d <= m; d++)
				{
					/* Check grid "d" in strip "n", notice "blockage" */
					if (update_view_aux(ymn-d, xpn, ymn-d+1, xpn-1, ymn-d+1, xpn))
					{
						if (n + d >= ne) break;
					}

					/* Track most distant "non-blockage" */
					else
					{
						k = n + d;
					}
				}

				/* Limit the next strip */
				ne = k + 1;
			}

			/* West side */
			if ((xmn >= 0) && (n < nw))
			{
				/* Scan */
				for (k = n, d = 1; d <= m; d++)
				{
					/* Check grid "d" in strip "n", notice "blockage" */
					if (update_view_aux(ymn-d, xmn, ymn-d+1, xmn+1, ymn-d+1, xmn))
					{
						if (n + d >= nw) break;
					}

					/* Track most distant "non-blockage" */
					else
					{
						k = n + d;
					}
				}

				/* Limit the next strip */
				nw = k + 1;
			}
		}


		/* East strip */
		if (xpn < x_max)
		{
			/* Maximum distance */
			m = MIN(z, x_max - xpn);

			/* South side */
			if ((ypn <= x_max) && (n < es))
			{
				/* Scan */
				for (k = n, d = 1; d <= m; d++)
				{
					/* Check grid "d" in strip "n", notice "blockage" */
					if (update_view_aux(ypn, xpn+d, ypn-1, xpn+d-1, ypn, xpn+d-1))
					{
						if (n + d >= es) break;
					}

					/* Track most distant "non-blockage" */
					else
					{
						k = n + d;
					}
				}

				/* Limit the next strip */
				es = k + 1;
			}

			/* North side */
			if ((ymn >= 0) && (n < en))
			{
				/* Scan */
				for (k = n, d = 1; d <= m; d++)
				{
					/* Check grid "d" in strip "n", notice "blockage" */
					if (update_view_aux(ymn, xpn+d, ymn+1, xpn+d-1, ymn, xpn+d-1))
					{
						if (n + d >= en) break;
					}

					/* Track most distant "non-blockage" */
					else
					{
						k = n + d;
					}
				}

				/* Limit the next strip */
				en = k + 1;
			}
		}


		/* West strip */
		if (xmn > 0)
		{
			/* Maximum distance */
			m = MIN(z, xmn);

			/* South side */
			if ((ypn <= y_max) && (n < ws))
			{
				/* Scan */
				for (k = n, d = 1; d <= m; d++)
				{
					/* Check grid "d" in strip "n", notice "blockage" */
					if (update_view_aux(ypn, xmn-d, ypn-1, xmn-d+1, ypn, xmn-d+1))
					{
						if (n + d >= ws) break;
					}

					/* Track most distant "non-blockage" */
					else
					{
						k = n + d;
					}
				}

				/* Limit the next strip */
				ws = k + 1;
			}

			/* North side */
			if ((ymn >= 0) && (n < wn))
			{
				/* Scan */
				for (k = n, d = 1; d <= m; d++)
				{
					/* Check grid "d" in strip "n", notice "blockage" */
					if (update_view_aux(ymn, xmn-d, ymn+1, xmn-d+1, ymn, xmn-d+1))
					{
						if (n + d >= wn) break;
					}

					/* Track most distant "non-blockage" */
					else
					{
						k = n + d;
					}
				}

				/* Limit the next strip */
				wn = k + 1;
			}
		}
	}


	/*** Step 5 -- Complete the algorithm ***/

	/* Update all the new grids */
	for (n = 0; n < view_n; n++)
	{
		y = view_y[n];
		x = view_x[n];

		/* Access the grid */
		c_ptr = &cave[y][x];

		/* Clear the "CAVE_XTRA" flag */
		c_ptr->info &= ~(CAVE_XTRA);

		/* Update only newly viewed grids */
		if (c_ptr->info & (CAVE_TEMP)) continue;

		/* Note */
		note_spot(y, x);

		/* Redraw */
		lite_spot(y, x);
	}

	/* Wipe the old grids, update as needed */
	for (n = 0; n < temp_n; n++)
	{
		y = temp_y[n];
		x = temp_x[n];

		/* Access the grid */
		c_ptr = &cave[y][x];

		/* No longer in the array */
		c_ptr->info &= ~(CAVE_TEMP);

		/* Update only non-viewable grids */
		if (c_ptr->info & (CAVE_VIEW)) continue;

		/* Redraw */
		lite_spot(y, x);
	}

	/* None left */
	temp_n = 0;
}






/*
 * Hack -- provide some "speed" for the "flow" code
 * This entry is the "current index" for the "when" field
 * Note that a "when" value of "zero" means "not used".
 *
 * Note that the "cost" indexes from 1 to 127 are for
 * "old" data, and from 128 to 255 are for "new" data.
 *
 * This means that as long as the player does not "teleport",
 * then any monster up to 128 + MONSTER_FLOW_DEPTH will be
 * able to track down the player, and in general, will be
 * able to track down either the player or a position recently
 * occupied by the player.
 */




#ifdef MONSTER_FLOW

/*
 * Hack -- Allow us to treat the "seen" array as a queue
 */
static int flow_head = 0;
static int flow_tail = 0;


/*
 * Take note of a reachable grid.  Assume grid is legal.
 */
static void update_flow_aux(int y, int x, int n, int f)
{
	cave_type *c_ptr;

	int old_head = flow_head;


	/* Get the grid */
	c_ptr = &cave[y][x];

	/* Ignore "pre-stamped" entries */
	if (c_ptr->when == f) return;

	/* Ignore "walls" and "rubble" */
	if (c_ptr->feat >= FEAT_RUBBLE) return;

	/* Save the time-stamp */
	c_ptr->when = f;

	/* Save the flow cost */
	c_ptr->cost = n;

	/* Hack -- limit flow depth */
	if (n == MONSTER_FLOW_DEPTH) return;

	/* Enqueue that entry */
	temp_y[flow_head] = y;
	temp_x[flow_head] = x;

	/* Advance the queue */
	if (++flow_head == TEMP_MAX) flow_head = 0;

	/* Hack -- notice overflow by forgetting new entry */
	if (flow_head == flow_tail) flow_head = old_head;
}

#endif


/*
 * Hack -- fill in the "cost" field of every grid that the player
 * can "reach" with the number of steps needed to reach that grid.
 * This also yields the "distance" of the player from every grid.
 *
 * In addition, mark the "when" of the grids that can reach
 * the player with the incremented value of "flow_n".
 *
 * Hack -- use the "seen" array as a "circular queue".
 *
 * We do not need a priority queue because the cost from grid
 * to grid is always "one" and we process them in order.
 */
void update_flow(void)
{

#ifdef MONSTER_FLOW

	int x, y, d;

	static int flow_n = 0;

	/* Hack -- disabled */
	if (!flow_by_sound) return;

	/* Paranoia -- make sure the array is empty */
	if (temp_n) return;

	/* Cycle the old entries (once per 128 updates) */
	if (flow_n == 255)
	{
		/* Rotate the time-stamps */
		for (y = 0; y < cur_hgt; y++)
		{
			for (x = 0; x < cur_wid; x++)
			{
				int w = cave[y][x].when;
				cave[y][x].when = (w > 128) ? (w - 128) : 0;
			}
		}

		/* Restart */
		flow_n = 127;
	}

	/* Start a new flow (never use "zero") */
	flow_n++;


	/* Reset the "queue" */
	flow_head = flow_tail = 0;

	/* Add the player's grid to the queue */
	update_flow_aux(py, px, 0, flow_n);

	/* Now process the queue */
	while (flow_head != flow_tail)
	{
		/* Extract the next entry */
		y = temp_y[flow_tail];
		x = temp_x[flow_tail];

		/* Forget that entry */
		if (++flow_tail == TEMP_MAX) flow_tail = 0;

		/* Add the "children" */
		for (d = 0; d < 8; d++)
		{
			/* Add that child if "legal" */
			update_flow_aux(y+ddy_ddd[d], x+ddx_ddd[d], cave[y][x].cost+1,
				flow_n);
		}
	}

	/* Forget the flow info */
	flow_head = flow_tail = 0;

#endif

}







/*
 * Hack -- map the current panel (plus some) ala "magic mapping"
 */
void map_area(void)
{
	int             i, x, y, y1, y2, x1, x2;

	cave_type       *c_ptr;


	/* Pick an area to map */
	y1 = panel_row_min - randint(10);
	y2 = panel_row_max + randint(10);
	x1 = panel_col_min - randint(20);
	x2 = panel_col_max + randint(20);

	/* Speed -- shrink to fit legal bounds */
	if (y1 < 1) y1 = 1;
	if (y2 > cur_hgt-2) y2 = cur_hgt-2;
	if (x1 < 1) x1 = 1;
	if (x2 > cur_wid-2) x2 = cur_wid-2;

	/* Scan that area */
	for (y = y1; y <= y2; y++)
	{
		for (x = x1; x <= x2; x++)
		{
			c_ptr = &cave[y][x];

			/* All non-walls are "checked" */
			if (c_ptr->feat < FEAT_SECRET)
			{
				/* Memorize normal features */
				if (c_ptr->feat > FEAT_INVIS)
				{
					/* Memorize the object */
					c_ptr->info |= (CAVE_MARK);
				}

				/* Memorize known walls */
				for (i = 0; i < 8; i++)
				{
					c_ptr = &cave[y+ddy_ddd[i]][x+ddx_ddd[i]];

					/* Memorize walls (etc) */
					if (c_ptr->feat >= FEAT_SECRET)
					{
						/* Memorize the walls */
						c_ptr->info |= (CAVE_MARK);
					}
				}
			}
		}
	}

	/* Redraw map */
	p_ptr->redraw |= (PR_MAP);
}



/*
 * Light up the dungeon using "claravoyance"
 *
 * This function "illuminates" every grid in the dungeon, memorizes all
 * "objects", memorizes all grids as with magic mapping, and, under the
 * standard option settings (view_perma_grids but not view_torch_grids)
 * memorizes all floor grids too.
 *
 * Note that if "view_perma_grids" is not set, we do not memorize floor
 * grids, since this would defeat the purpose of "view_perma_grids", not
 * that anyone seems to play without this option.
 *
 * Note that if "view_torch_grids" is set, we do not memorize floor grids,
 * since this would prevent the use of "view_torch_grids" as a method to
 * keep track of what grids have been observed directly.
 */
void wiz_lite(void)
{
	int i, y, x;


	/* Memorize objects */
	for (i = 1; i < o_max; i++)
	{
		object_type *o_ptr = &o_list[i];

		/* Skip dead objects */
		if (!o_ptr->k_idx) continue;

		/* Skip held objects */
		if (o_ptr->held_m_idx) continue;

		/* Memorize */
		o_ptr->marked = TRUE;
	}

	/* Scan all normal grids */
	for (y = 1; y < cur_hgt-1; y++)
	{
		/* Scan all normal grids */
		for (x = 1; x < cur_wid-1; x++)
		{
			cave_type *c_ptr = &cave[y][x];

			/* Process all non-walls */
			if (c_ptr->feat < FEAT_SECRET)
			{
				/* Scan all neighbors */
				for (i = 0; i < 9; i++)
				{
					int yy = y + ddy_ddd[i];
					int xx = x + ddx_ddd[i];

					/* Get the grid */
					c_ptr = &cave[yy][xx];

					/* Perma-lite the grid */
					c_ptr->info |= (CAVE_GLOW);

					/* Memorize normal features */
					if (c_ptr->feat > FEAT_INVIS)
					{
						/* Memorize the grid */
						c_ptr->info |= (CAVE_MARK);
					}

					/* Normally, memorize floors (see above) */
					if (view_perma_grids && !view_torch_grids)
					{
						/* Memorize the grid */
						c_ptr->info |= (CAVE_MARK);
					}
				}
			}
		}
	}

	/* Update the monsters */
	p_ptr->update |= (PU_MONSTERS);

	/* Redraw map */
	p_ptr->redraw |= (PR_MAP);
}


/*
 * Forget the dungeon map (ala "Thinking of Maud...").
 */
void wiz_dark(void)
{
	int i, y, x;


	/* Forget every grid */
	for (y = 0; y < cur_hgt; y++)
	{
		for (x = 0; x < cur_wid; x++)
		{
			cave_type *c_ptr = &cave[y][x];

			/* Process the grid */
			c_ptr->info &= ~(CAVE_MARK | CAVE_TRAP);
			c_ptr->r_feat = FEAT_NONE;
		}
	}

	/* Forget all objects */
	for (i = 1; i < o_max; i++)
	{
		object_type *o_ptr = &o_list[i];

		/* Skip dead objects */
		if (!o_ptr->k_idx) continue;

		/* Skip held objects */
		if (o_ptr->held_m_idx) continue;

		/* Forget the object */
		o_ptr->marked = FALSE;
	}

	/* Mega-Hack -- Forget the view and lite */
	p_ptr->update |= (PU_UN_VIEW | PU_UN_LITE);

	/* Update the view and lite */
	p_ptr->update |= (PU_VIEW | PU_LITE);

	/* Update the monsters */
	p_ptr->update |= (PU_MONSTERS);

	/* Redraw map */
	p_ptr->redraw |= (PR_MAP);
}





/*
 * Change the "feat" flag for a grid, and notice/redraw the grid
 */
void cave_set_feat(int y, int x, int feat)
{
	cave_type *c_ptr = &cave[y][x];

	/* Change the feature */
	c_ptr->feat = feat;

	/* Notice */
	note_spot(y, x);

	/* Redraw */
	lite_spot(y, x);

	/* Window stuff, if needed. */
	if (y == py && x == px) p_ptr->window |= PW_FLOOR;
}



/*
 * Calculate "incremental motion". Used by project() and shoot().
 * Assumes that (*y,*x) lies on the path from (y1,x1) to (y2,x2).
 */
void mmove2(int *y, int *x, int y1, int x1, int y2, int x2)
{
	int dy, dx, dist, shift;

	/* Extract the distance travelled */
	dy = (*y < y1) ? y1 - *y : *y - y1;
	dx = (*x < x1) ? x1 - *x : *x - x1;

	/* Number of steps */
	dist = (dy > dx) ? dy : dx;

	/* We are calculating the next location */
	dist++;


	/* Calculate the total distance along each axis */
	dy = (y2 < y1) ? (y1 - y2) : (y2 - y1);
	dx = (x2 < x1) ? (x1 - x2) : (x2 - x1);

	/* Paranoia -- Hack -- no motion */
	if (!dy && !dx) return;


	/* Move mostly vertically */
	if (dy > dx)
	{

		/* Extract a shift factor */
		shift = (dist * dx + (dy-1) / 2) / dy;

		/* Sometimes move along the minor axis */
		(*x) = (x2 < x1) ? (x1 - shift) : (x1 + shift);

		/* Always move along major axis */
		(*y) = (y2 < y1) ? (y1 - dist) : (y1 + dist);
	}

	/* Move mostly horizontally */
	else
	{

		/* Extract a shift factor */
		shift = (dist * dy + (dx-1) / 2) / dx;

		/* Sometimes move along the minor axis */
		(*y) = (y2 < y1) ? (y1 - shift) : (y1 + shift);

		/* Always move along major axis */
		(*x) = (x2 < x1) ? (x1 - dist) : (x1 + dist);
	}
}



/*
 * Move from (ox,oy) to (nx,ny), stopping at the last square which was "okay".
 *
 * Return TRUE if the entire path (excluding the starting position) is "okay".
 */
bool move_in_direction(int *xx, int *yy, int x1, int y1, int x2, int y2,
	int (*okay)(int, int, int))
{
	int x, y, d, r;
	
	assert(okay); /* Caller */

	/* Set (*xx, *yy) to the square before the first on which okay() fail. */
	for (d = 0, x = x1, y = y1; ; d++)
	{
		r = (d) ? (*okay)(y, x, d) : MVD_CONTINUE;

		if (r == MVD_STOP_BEFORE_HERE) break;

		/* Save the new location if desired. */
		if (xx) *xx = x;
		if (yy) *yy = y;

		/* Okay for the entire distance. */
		if (x == x2 && y == y2) return TRUE;

		if (r == MVD_STOP_HERE) break;

		mmove2(&y, &x, y1, x1, y2, x2);
	}

	/* Not okay. */
	return FALSE;
}

/*
 * Return TRUE if (x,y) is a "floor" grid, part of the pattern or an explosive
 * rune.
 */
static int PURE mvd_projectable(int y, int x, int d)
{
	/* Too far. */
	if (d > MAX_RANGE || !in_bounds2(y, x)) return MVD_STOP_BEFORE_HERE;

	/* Floor */
	else if (cave_floor_bold(y, x)) return MVD_CONTINUE;

	/* Pattern or explosive rune. */
	else if ((cave[y][x].feat <= FEAT_PATTERN_XTRA2) &&
		(cave[y][x].feat >= FEAT_MINOR_GLYPH)) return MVD_CONTINUE;

	/* Bad terrain feature. */
	else return MVD_STOP_BEFORE_HERE;
}

/*
 * Determine if a bolt spell cast from (y1,x1) to (y2,x2) will arrive
 * at the final destination, assuming no monster gets in the way.
 *
 * This is slightly (but significantly) different from "los(y1,x1,y2,x2)".
 */
bool projectable(int y1, int x1, int y2, int x2)
{
	/* See "project()" */
	return move_in_direction(NULL, NULL, x1, y1, x2, y2, mvd_projectable);
}

/*
 * A general "find a location" function.
 * Obtains a location on the map which matches "accept" with v as its parameter.
 *
 * This isn`t very efficient as it calls accept at least once on every grid,
 * but it doesn't need to use large arrays to do this.
 */
bool rand_location(int *yp, int *xp, bool (*accept)(int, int, vptr), vptr v)
{
	int x, y, t;

	for (t = y = 0; y < cur_hgt; y++)
	{
		for (x = 0; x < cur_wid; x++)
		{
			if ((*accept)(y, x, v)) t++;
		}
	}

	/* No matching squares exist. */
	if (!t) return FALSE;

	t = rand_int(t);

	for (y = 0; y < cur_hgt; y++)
	{
		for (x = 0; x < cur_wid; x++)
		{
			if ((*accept)(y, x, v) && !t--)
			{
				*yp = y;
				*xp = x;
				return TRUE;
			}
		}
	}

	/* Paranoia. */
	assert(!"the acceptable grid list isn't constant!");
	return FALSE;
}

/*
 * Return TRUE if the new location is suitable for scatter() below.
 */
static bool PURE scatter_good(int ny, int nx, int y, int x, int d,
	bool (*accept)(int, int))
{
	/* Ignore illegal locations and outer walls */
	if (!in_bounds(ny, nx)) return FALSE;

	/* Ignore "excessively distant" locations */
	if ((d > 1) && (distance(y, x, ny, nx) > d)) return FALSE;

	/* Require "line of sight" */
	if (!los(y, x, ny, nx)) return FALSE;

	/* Require extra restriction, if any. */
	if (accept && !(*accept)(ny, nx)) return FALSE;

	return TRUE;
}

/*
 * Standard "find me a location" function
 *
 * Obtains a legal location within the given distance of the initial
 * location, and with "los()" from the source to destination location.
 *
 * This function is often called from inside a loop which searches for
 * locations while increasing the "d" distance.
 *
 * accept is an extra restriction which must be satisfied by the location,
 * if any.
 */
bool scatter(int *yp, int *xp, int y, int x, int d, bool (*accept)(int, int))
{
	int nx, ny, t;

	for (ny = y-d, t = 0; ny <= y+d; ny++)
	{
		for (nx = x-d; nx <= x+d; nx++)
		{
			if (scatter_good(ny, nx, y, x, d, accept)) t++;
		}
	}

	/* No valid locations found. */
	if (!t) return FALSE;

	/* Pick a location. */
	t = rand_int(t);

	for (ny = y-d; ny <= y+d; ny++)
	{
		for (nx = x-d; nx <= x+d; nx++)
		{
			/* Not the chosen location. */
			if (!scatter_good(ny, nx, y, x, d, accept) || t--) continue;

			(*yp) = ny;
			(*xp) = nx;
			return TRUE;
		}
	}

	/* Paranoia - scatter_good() should be TRUE as many times in each loop. */
	return FALSE;
}




/*
 * Track a new monster
 */
void health_track(int m_idx)
{
	/* Track a new guy */
	health_who = m_idx;

	/* Redraw (later) */
	p_ptr->redraw |= (PR_HEALTH);
}



/*
 * Hack -- track the given monster race
 */
void monster_race_track(int r_idx)
{
	/* Save this monster ID */
	monster_race_idx = r_idx;

	/* Window stuff */
	p_ptr->window |= (PW_MONSTER);
}



/*
 * Hack -- track the given object kind
 */
void object_kind_track(int k_idx)
{
	/* Save this monster ID */
	object_kind_idx = k_idx;

	/* Window stuff */
	p_ptr->window |= (PW_OBJECT);
}



/*
 * Track the given item.
 */
void object_track(object_type *o_ptr)
{
	/* object_track(0) means "forget it, whatever it was". */
	if (!o_ptr)
	{
		tracked_o_ptr = o_ptr;
	}
	/* As does a request to track a hidden object. */
	else if (hidden_p(o_ptr))
	{
		tracked_o_ptr = NULL;
	}
	/* Always remember real objects. */
	else if (o_ptr->k_idx)
	{
		/* Save the object ID */
		tracked_o_ptr = o_ptr;
	}
	/* A repeated call to a now-blank object means "forget it". */
	else if (o_ptr == tracked_o_ptr)
	{
		tracked_o_ptr = NULL;
	}
	/* A call to an untracked blank object is ignored totally. */
	else
	{
		return;
	}

	/* Window stuff */
	p_ptr->window |= (PW_OBJECT_DETAILS);
}

/*
 * Track the given floor square.
 */
void cave_track(const int y, const int x)
{
	/* Remember the new cave square (or 0 to clear). */
	tracked_co_ord.x = x;
	tracked_co_ord.y = y;

	p_ptr->window |= (PW_FLOOR);
}



/*
 * Something has happened to disturb the player.
 *
 * The first arg indicates a major disturbance, which affects search.
 *
 * The second arg is currently unused, but could induce output flush.
 *
 * All disturbance cancels repeated commands, resting, and running.
 */
void disturb(int stop_stealth)
{
	/* Cancel auto-commands */
	/* command_new = 0; */

	/* Cancel repeated commands */
	if (command_rep)
	{
		/* Cancel */
		command_rep = 0;

		/* Redraw the state (later) */
		p_ptr->redraw |= (PR_STATE);

		/* Calculate torch radius */
		p_ptr->update |= (PU_TORCH);
	}

	/* Cancel stealth if requested */
	if (stop_stealth && p_ptr->sneaking)
	{
		/* Cancel */
		do_cmd_toggle_sneak();
	}

	/* Flush the input if requested */
	if (flush_disturb) flush();
}





/*
 * Hack -- Check if a level is a "quest" level
 */
bool is_quest(void)
{
	int i;

	/* Town is never a quest */
	if (!dun_level) return (FALSE);

	/* Check quests */
	for (i = 0; i < MAX_Q_IDX; i++)
	{
		/* Check for quest */
		if ((q_list[i].level == dun_level) &&
			(q_list[i].dungeon == cur_dungeon)) return (TRUE);
	}

	/* Nope */
	return (FALSE);
}



