#define INIT1_C
/* File: init1.c */

/* Purpose: Initialization (part 1) -BEN- */

#include "angband.h"


/*
 * This file is used to initialize various variables and arrays for the
 * Angband game.  Note the use of "fd_read()" and "fd_write()" to bypass
 * the common limitation of "read()" and "write()" to only 32767 bytes
 * at a time.
 *
 * Several of the arrays for Angband are built from "template" files in
 * the "lib/file" directory, from which quick-load binary "image" files
 * are constructed whenever they are not present in the "lib/data"
 * directory, or if those files become obsolete, if we are allowed.
 *
 * Warning -- the "ascii" file parsers use a minor hack to collect the
 * name and text information in a single pass.  Thus, the game will not
 * be able to load any template file with more than 20K of names or 60K
 * of text, even though technically, up to 64K should be legal.
 *
 * Note that if "ALLOW_TEMPLATES" is not defined, then a lot of the code
 * in this file is compiled out, and the game will not run unless valid
 * "binary template files" already exist in "lib/data".  Thus, one can
 * compile Angband with ALLOW_TEMPLATES defined, run once to create the
 * "*.raw" files in "lib/data", and then quit, and recompile without
 * defining ALLOW_TEMPLATES, which will both save 20K and prevent people
 * from changing the ascii template files in potentially dangerous ways.
 *
 * The code could actually be removed and placed into a "stand-alone"
 * program, but that feels a little silly, especially considering some
 * of the platforms that we currently support.
 */


#ifdef ALLOW_TEMPLATES


/*
 * Hack -- error tracking
 */
extern s16b error_idx;
extern s16b error_line;


/*
 * Hack -- size of the "fake" arrays
 */
extern u32b fake_name_size;
extern u32b fake_text_size;



/*** Helper arrays for parsing ascii template files ***/

/*
 * Monster Blow Methods
 */
static cptr r_info_blow_method[] =
{
	"",
	"HIT",
	"TOUCH",
	"PUNCH",
	"KICK",
	"CLAW",
	"BITE",
	"STING",
	"XXX1",
	"BUTT",
	"CRUSH",
	"ENGULF",
    "CHARGE",  /* WAS: XXX2 */
	"CRAWL",
	"DROOL",
	"SPIT",
	"XXX3",
	"GAZE",
	"WAIL",
	"SPORE",
	"WORSHIP",
	"BEG",
	"INSULT",
	"MOAN",
    "SHOW",  /* WAS: XXX5 */
	NULL
};


/*
 * Monster Blow Effects
 */
static cptr r_info_blow_effect[] =
{
	"",
	"HURT",
	"POISON",
	"UN_BONUS",
	"UN_POWER",
	"EAT_GOLD",
	"EAT_ITEM",
	"EAT_FOOD",
	"EAT_LITE",
	"ACID",
	"ELEC",
	"FIRE",
	"COLD",
	"BLIND",
	"CONFUSE",
	"TERRIFY",
	"PARALYZE",
	"LOSE_STR",
	"LOSE_INT",
	"LOSE_WIS",
	"LOSE_DEX",
	"LOSE_CON",
	"LOSE_CHR",
	"LOSE_ALL",
	"SHATTER",
	"EXP_10",
	"EXP_20",
	"EXP_40",
	"EXP_80",
	NULL
};


/*
 * Monster race flags
 */
static cptr r_info_flags1[] =
{
	"UNIQUE",
	"GUARDIAN",
	"MALE",
	"FEMALE",
	"CHAR_CLEAR",
	"CHAR_MULTI",
	"ATTR_CLEAR",
	"ATTR_MULTI",
	"ALWAYS_GUARD",
	"FORCE_MAXHP",
	"FORCE_SLEEP",
	"FORCE_EXTRA",
	"FRIEND",
	"FRIENDS",
	"ESCORT",
	"ESCORTS",
	"NEVER_BLOW",
	"NEVER_MOVE",
	"RAND_25",
	"RAND_50",
	"ONLY_GOLD",
	"ONLY_ITEM",
	"DROP_60",
	"DROP_90",
	"DROP_1D2",
	"DROP_2D2",
	"DROP_3D2",
	"DROP_4D2",
	"DROP_GOOD",
	"DROP_GREAT",
	"DROP_USEFUL",
	"DROP_CHOSEN"
};

/*
 * Monster race flags
 */
static cptr r_info_flags2[] =
{
	"STUPID",
	"SMART",
    "CAN_SPEAK", /* WAS: XXX1X2 */
    "REFLECTING",
	"INVISIBLE",
	"COLD_BLOOD",
	"EMPTY_MIND",
	"WEIRD_MIND",
	"MULTIPLY",
	"REGENERATE",
    "SHAPECHANGER", /*  WAS: XXX3X2 */
    "ATTR_ANY",     /*  WAS: XXX4X2 */
	"POWERFUL",
    "ELDRITCH_HORROR",
    "AURA_FIRE",
    "AURA_ELEC",
	"OPEN_DOOR",
	"BASH_DOOR",
	"PASS_WALL",
	"KILL_WALL",
	"MOVE_BODY",
	"KILL_BODY",
	"TAKE_ITEM",
	"KILL_ITEM",
      "RUN_AWAY",
	"BRAIN_1",
	"BRAIN_2",
	"BRAIN_3",
	"BRAIN_4",
	"BRAIN_5",
	"BRAIN_6",
	"BRAIN_7",
};

/*
 * Monster race flags
 */
static cptr r_info_flags3[] =
{
	"ORC",
	"TROLL",
	"GIANT",
	"DRAGON",
	"DEMON",
	"UNDEAD",
	"EVIL",
	"ANIMAL",
    "GREAT_OLD_ONE", /*WAS: XXX1X3 */
    "GOOD",
	"XXX3X3",
    "NONLIVING", /*WAS: XXX4X3 */
	"HURT_LITE",
	"HURT_ROCK",
	"HURT_FIRE",
	"HURT_COLD",
	"IM_ACID",
	"IM_ELEC",
	"IM_FIRE",
	"IM_COLD",
	"IM_POIS",
    "RES_TELE",
	"RES_NETH",
	"RES_WATE",
	"RES_PLAS",
	"RES_NEXU",
	"RES_DISE",
	"CTHULOID",
	"NO_FEAR",
	"NO_STUN",
	"NO_CONF",
	"NO_SLEEP"
};

/*
 * Monster race flags
 */
static cptr r_info_flags4[] =
{
	"SHRIEK",
	"ODD_ARTICLE",
	"XXX3X4",
    "BA_SHARD",  /* WAS: XXX4X4 */
	"ARROW_1",
	"ARROW_2",
	"ARROW_3",
	"ARROW_4",
	"BR_ACID",
	"BR_ELEC",
	"BR_FIRE",
	"BR_COLD",
	"BR_POIS",
	"BR_NETH",
	"BR_LITE",
	"BR_DARK",
	"BR_CONF",
	"BR_SOUN",
	"BR_CHAO",
	"BR_DISE",
	"BR_NEXU",
	"BR_TIME",
	"BR_INER",
	"BR_GRAV",
	"BR_SHAR",
	"BR_PLAS",
	"BR_WALL",
	"BR_MANA",
    "BA_NUKE", /* WAS: XXX5X4 */
    "BR_NUKE", /* WAS: XXX6X4 */
    "BA_CHAO", /* WAS: XXX7X4 */
    "BR_DISI",
};

/*
 * Monster race flags
 */
static cptr r_info_flags5[] =
{
	"BA_ACID",
	"BA_ELEC",
	"BA_FIRE",
	"BA_COLD",
	"BA_POIS",
	"BA_NETH",
	"BA_WATE",
	"BA_MANA",
	"BA_DARK",
	"DRAIN_MANA",
	"MIND_BLAST",
	"BRAIN_SMASH",
	"CAUSE_1",
	"CAUSE_2",
	"CAUSE_3",
	"CAUSE_4",
	"BO_ACID",
	"BO_ELEC",
	"BO_FIRE",
	"BO_COLD",
	"BO_POIS",
	"BO_NETH",
	"BO_WATE",
	"BO_MANA",
	"BO_PLAS",
	"BO_ICEE",
	"MISSILE",
	"SCARE",
	"BLIND",
	"CONF",
	"SLOW",
	"HOLD"
};

/*
 * Monster race flags
 */
static cptr r_info_flags6[] =
{
	"HASTE",
    "DREAD_CURSE",
	"HEAL",
	"XXX2X6",
	"BLINK",
	"TPORT",
	"XXX3X6",
	"XXX4X6",
	"TELE_TO",
	"TELE_AWAY",
	"TELE_LEVEL",
	"XXX5",
	"DARKNESS",
	"TRAPS",
	"FORGET",
	"XXX6X6",
    "S_KIN",
    "S_REAVER",
	"S_MONSTER",
	"S_MONSTERS",
	"S_ANT",
	"S_SPIDER",
	"S_HOUND",
	"S_HYDRA",
	"S_CTHULOID",
	"S_DEMON",
	"S_UNDEAD",
	"S_DRAGON",
	"S_HI_UNDEAD",
	"S_HI_DRAGON",
	"S_GOO",
	"S_UNIQUE"
};


/*
 * Object flags
 */
static cptr k_info_flags1[] =
{
	"STR",
	"INT",
	"WIS",
	"DEX",
	"CON",
	"CHR",
	"XXX1",
	"XXX2",
	"STEALTH",
	"SEARCH",
	"INFRA",
	"TUNNEL",
	"SPEED",
	"BLOWS",
    "CHAOTIC",
    "VAMPIRIC",
	"SLAY_ANIMAL",
	"SLAY_EVIL",
	"SLAY_UNDEAD",
	"SLAY_DEMON",
	"SLAY_ORC",
	"SLAY_TROLL",
	"SLAY_GIANT",
	"SLAY_DRAGON",
	"KILL_DRAGON",
    "VORPAL",
	"IMPACT",
    "BRAND_POIS",
	"BRAND_ACID",
	"BRAND_ELEC",
	"BRAND_FIRE",
	"BRAND_COLD"
};

/*
 * Object flags
 */
static cptr k_info_flags2[] =
{
	"SUST_STR",
	"SUST_INT",
	"SUST_WIS",
	"SUST_DEX",
	"SUST_CON",
	"SUST_CHR",
	"RAND_RESIST",
	"RAND_POWER",
	"IM_ACID",
	"IM_ELEC",
	"IM_FIRE",
	"IM_COLD",
	"RAND_EXTRA",
    "REFLECT",
	"FREE_ACT",
	"HOLD_LIFE",
	"RES_ACID",
	"RES_ELEC",
	"RES_FIRE",
	"RES_COLD",
	"RES_POIS",
    "RES_FEAR",
	"RES_LITE",
	"RES_DARK",
	"RES_BLIND",
	"RES_CONF",
	"RES_SOUND",
	"RES_SHARDS",
	"RES_NETHER",
	"RES_NEXUS",
	"RES_CHAOS",
	"RES_DISEN"
};

/*
 * Object flags
 */
static cptr k_info_flags3[] =
{
    "SH_FIRE",
    "SH_ELEC",
	"XXX3",
	"AUTO_CURSE",
    "NO_TELE",
    "NO_MAGIC",
    "WRAITH",
    "TY_CURSE",
	"EASY_KNOW",
	"HIDE_TYPE",
	"SHOW_MODS",
	"INSTA_ART",
	"FEATHER",
	"LITE",
	"SEE_INVIS",
	"TELEPATHY",
	"SLOW_DIGEST",
	"REGEN",
	"XTRA_MIGHT",
	"XTRA_SHOTS",
	"IGNORE_ACID",
	"IGNORE_ELEC",
	"IGNORE_FIRE",
	"IGNORE_COLD",
	"ACTIVATE",
	"DRAIN_EXP",
	"TELEPORT",
	"AGGRAVATE",
	"BLESSED",
	"CURSED",
	"HEAVY_CURSE",
	"PERMA_CURSE"
};

/* A list of the flags for explosion types understood by project(). */
cptr explode_flags[] =
{
	"ELEC",
	"POIS",
	"ACID",
	"COLD",
	"FIRE",
	"","","","",
	"MISSILE",
	"ARROW",
	"PLASMA",
	"",
	"WATER",
	"LITE",
	"DARK",
	"LITE_WEAK",
	"DARK_WEAK",
	"",
	"SHARDS",
	"SOUND",
	"CONFUSION",
	"FORCE",
	"INERTIA",
	"",
	"MANA",
	"METEOR",
	"ICE",
	"",
	"CHAOS",
	"NETHER",
	"DISENCHANT",
	"NEXUS",
	"TIME",
	"GRAVITY",
	"","","","",
	"KILL_WALL",
	"KILL_DOOR",
	"KILL_TRAP",
	"","",
	"MAKE_WALL",
	"MAKE_DOOR",
	"MAKE_TRAP",
	"","","",
	"OLD_CLONE",
	"OLD_POLY",
	"OLD_HEAL",
	"OLD_SPEED",
	"OLD_SLOW",
	"OLD_CONF",
	"OLD_SLEEP",
	"OLD_DRAIN",
	"","",
	"AWAY_UNDEAD",
	"AWAY_EVIL",
	"AWAY_ALL",
	"TURN_UNDEAD",
	"TURN_EVIL",
	"TURN_ALL",
	"DISP_UNDEAD",
	"DISP_EVIL",
	"DISP_ALL",
	"DISP_DEMON",
	"DISP_LIVING",
	"SHARD",
	"NUKE",
	"MAKE_GLYPH",
	"STASIS",
	"STONE_WALL",
	"DEATH_RAY",
	"STUN",
	"HOLY_FIRE",
	"HELL_FIRE",
	"DISINTEGRATE",
	"CHARM",
	"CONTROL_UNDEAD",
	"CONTROL_ANIMAL",
	"PSI",
	"PSI_DRAIN",
	"TELEKINESIS",
	"JAM_DOOR",
	"DOMINATION",
	"DISP_GOOD",
	NULL
};

/* A list of the types of death event in order */
static cptr death_flags[] =
{
	"NOTHING",
	"MONSTER",
	"OBJECT",
	"EXPLODE",
	"COIN",
	NULL
};

/* A list of the types of coins. This can't be taken from k_info.txt because of
 * the duplicate entries. */
cptr coin_types[] =
{
	"","","COPPER", /* 482 */
	"","","SILVER", /* 485 */
	"","","","","GOLD", /* 490 */
	"","","","","","MITHRIL", /* 496 */
	"ADAMANTIUM", /* 497 */
	NULL
};

/*
 * Check that the given string includes the current version number.
 */
static errr check_version(char *buf, header *head)
{
	int v1, v2, v3;

	/* Scan for the values */
	if ((3 != sscanf(buf+2, "%d.%d.%d", &v1, &v2, &v3)) ||
	    (v1 != head->v_major) ||
	    (v2 != head->v_minor) ||
	    (v3 != head->v_patch))
	{
		return ERR_VERSION;
	}
	else
	{
		return SUCCESS;
	}
}

/*
 * Compress %x strings into single characters within a NUL-terminated string.
 *
 * As my_fgets() only outputs printable characters and '\0's, the
 * range 1-31 can be used in any way desired.
 *
 * This does not replace unparsable characters (such as %f without preceding
 * %F), so object_desc() does not need to check this.
 *
 * NULs will be added from the end of the output string to the end of the
 * input (which is always at least as large).
 *
 * As the output characters are chosen without regard to their meaning, no
 * part of a compress string should be printed directly.
 */
static errr compress_string(char *buf)
{
	typedef struct compression_type compression_type;
	struct compression_type {
		cptr orig;
		char new;
	};
	char *i,*j;
	char require;
	bool changed = FALSE;
	compression_type *cm_ptr;
	compression_type compression[] = {
		/* Require the flag */
		{"%+A", CM_TRUE+CI_ARTICLE},
		{"%+K", CM_TRUE+CI_K_IDX},
		{"%+F", CM_TRUE+CI_FLAVOUR},
		{"%+P", CM_TRUE+CI_PLURAL},
		
		/* Require the absence of the flag */
		{"%-A", CM_FALSE+CI_ARTICLE},
		{"%-K", CM_FALSE+CI_K_IDX},
		{"%-F", CM_FALSE+CI_FLAVOUR},
		{"%-P", CM_FALSE+CI_PLURAL},
		
		/* No requirements regarding the flag. */
		{"%A", CM_NORM+CI_ARTICLE},
		{"%K", CM_NORM+CI_K_IDX},
		{"%F", CM_NORM+CI_FLAVOUR},
		{"%P", CM_NORM+CI_PLURAL},
		
		/* Insert a substring. */
		{"%a", CM_ACT+CI_ARTICLE},
		{"%k", CM_ACT+CI_K_IDX},
		{"%f", CM_ACT+CI_FLAVOUR},
		{"%p", CM_ACT+CI_PLURAL},
		
		/* Enable %s in strings */
		{"%%", '%'},
		
		/* Terminate */
		{"", 0}
	};
	/* Literal replacements. */
	for (i = j = buf; *i;)
	{
		for (cm_ptr = compression; cm_ptr->new; cm_ptr++)
		{
			if (!strncmp(cm_ptr->orig, i, strlen(cm_ptr->orig))) break;
		}
		/* New combinations */
		if (cm_ptr->new)
		{
			*(j++) = cm_ptr->new;
			i+=strlen(cm_ptr->orig);
			changed = TRUE;
		}
		/* Normal letters/numbers */
		else
		{
			*(j++) = *(i++);
		}
	}
	/* Finish off */
	for (; j < i; j++) *j = '\0';

	/* Verify that the CM_ACT flags are legal. */
	for (i = buf, require = 0x00;*i;i++)
	{
		byte flag;

		/* Normal characters */
		if (*i & 0xE0) continue;

		flag = 1<<find_ci(*i);
		
		switch (find_cm(*i))
		{
			case CM_FALSE:
			case CM_NORM:
				require &= ~flag;
				break;
			case CM_TRUE:
				require |= flag;
				break;
			case CM_ACT:
				if (~require & flag) return PARSE_ERROR_GENERIC;
				break;
		}
	}

	return SUCCESS;
}

/* A macro to give effect to the return code of the above. */
#define do_compress_string(buf) \
{ \
	errr err = compress_string(buf); \
	if (err) return err; \
}


/* A macro to indicate if a character is used to terminate a name. */
#define okchar(char) (char == '\0' || char == ':')

/*
 * Return the string within an array which also exists within a string, 
 * returning (for example) 1 for the first element.
 * Returns 0 if there isn't exactly one such string.
 */
static s16b find_string(char *buf, cptr *array)
{
	u16b i, value = 0;
	char *place = 0;

	/* Find a string surrounded by ':'s. */
	for (i = 0; array[i]; i++)
	{
		if (strlen(array[i]) && strstr(buf, array[i]))
		{
			char *tmp = strstr(buf, array[i]);
			/* Check that this can't be merely a substring */
			if (!okchar(*(tmp-1))) continue;
			if (!okchar(*(tmp+strlen(array[i])))) continue;
			/* Check that this is the only possibility. */
			if (value)
			{
				msg_print("Too many suitable strings!");
				return 0;
			}
			value = i + 1;
			place = tmp;
		}
		}

	/* None there. */
	if (!value) return 0;
	/* If exactly one has been found, remove it. Leave the ':'s, though. */
	for (i = 0; i < strlen(array[value-1]); i++)
	{
		*(place+i)=' ';
	}
	return value;
}

/* A few macros for use in the 'E' case in init_r_event_txt() */

/* If one copy of chr exists within buf, return the integer immediately after it. */
#define readnum(chr) ((!strchr(buf, chr)) ? -1 : (strchr(strchr(buf, chr)+1, chr)) ? -2 : atoi(strchr(buf, chr)+1))

/* A separate routine to remove used number strings */
#define clearnum(chr) {char *s; for (s = strchr(buf, chr); *s == chr || (*s >= '0' && *s <= '9'); s++) *s=' ';}

/* A routine to set x to be the number after a given letter, and then clear it from the text string. 
 * If there are no such flags, it does nothing. If there are more than one, it returns an error.
 * THIS ASSUMES THAT ALL VALID VALUES ARE NON-NEGATIVE
 */
#define readclearnum(x, chr) if (readnum(chr) == -2) \
	{msg_format("Too many '%c's!", chr);msg_print(NULL);return ERR_PARSE;} \
	else if (readnum(chr) > -1) {x=readnum(chr); clearnum(chr);}

/*
 * A wrapper around find_string for info files.
 * As the text entries are stored as offsets in a single string rather than
 * simply as an array of strings, this first turns the former into the latter.
 * It assumes that the entries have numbers within (0,max)
 */
#define find_string_info(x_name, x_info, max, w) \
{ \
	cptr array[max]; \
	for (i = 1; i < max; i++) \
		if (x_info[i].name) \
			array[i-1] = x_name+x_info[i].name; \
		else \
			array[i-1] = ""; \
	array[max-1] = 0; \
	w = find_string(buf, array); \
}

/*
 * A wrapper around find_string for a single string.
 */
static bool find_string_x(char *buf, cptr string)
{
	cptr array[2] = {string, 0};
	return (bool)find_string(buf, array);
}

/* Find a string beginning and ending with a given character. Complain if it contains one of a set of other
 * characters. Allow \\ to be used to force the following character to be treated as plain text.
 *
 * buf is the string these characters are copied from.
 * this is the character which starts and ends this type of string.
 * all contains the characters which start and end all types of string being searched for.
 * output contains the text strings for the array in question. 
 * this_size contains the current offset of the final character in output.
 * max_size contains the maximum offset allowed.
 * offset contains the offset for this event (should be 0 initially).
 */
static errr do_get_string(char *buf, char this, cptr all, char *output, u32b *this_size, u32b max_size, u16b *offset)
{
	char *q, *r, *s = buf, *t = strchr(buf, '\0'), *last;
	bool escaped = FALSE;
	do {
		s = strchr(s+1, this);
		
	} while (s && *(s-1) == '\\');

	/* No such string. */
	if (!s) return SUCCESS;
	
	/* Go to the first character in the string itself. */
	s++;

	while (t > s && (*t != this || *(t-1) == '\\'))
	{
		t--;
	}

	/* No explicit end, so use end of buf. */
	if (t == s) t = strchr(s, '\0');
	
	/* Now copy the string to temp, being careful of \\s and rogue termination characters. */
	for (r = q = s, last = t; r < t; r++, q++)
	{
		/* Copy the character after the \\ and reset the flag. */
		if (escaped)
		{
			*q = *r;
			escaped = FALSE;
		}
		/* Accept anything from the character after a \\, but not the \\ itself. */
		else if (*r == '\\')
		{
			q--;
			escaped = TRUE;
		}
		/* Normal parsing of termination characters depends on which string is parsed first, 
		 * so reject all of them. We accept this termination character because it's unambiguous. */
		else if (strchr(all, *r) && *r != this)
		{
			printf("Unexpected termination character!");
			return ERR_PARSE;
		}
		/* Everything else is fine. */
		else
		{
			if (q != r) *q = *r;
		}
	}
	/* Make s the string of interest for now. */
	*q = '\0';
	if (max_size-(*this_size) < strlen(s))
	{
		msg_print("Not enough space for string!");
		return ERR_MEMORY;
	}
	/* Advance and save the index. */
	if (!(*offset)) (*offset) = ++(*this_size);
	/* Append characters to the text. */
	strcpy(output + (*this_size), s);

	/* Advance the index. */
	(*this_size) += strlen(s);

	/* Remove string from the buffer. */
	for (r = s-1; r <= t; r++)
	{
		*r=' ';
	}
	return SUCCESS;
}


/* Clear the \\ characters from a string */
static void clear_escapes(char *buf)
{
	char *q, *r;
	bool escaped = FALSE;
	/* Remove \\ characters from the string. */
	for (r = q = buf; *r != '\0'; q++, r++)
	{
		if (escaped)
		{
			*q = *r;
			escaped = FALSE;
		}
		else if (*r == '\\')
		{
			q--;
			escaped = TRUE;
		}
		else
		{
			if (q != r) *q = *r;
		}
	}
	*q = '\0';
}

/*
 * Initialize the "death_event" array, by parsing part of the "r_info.txt" file
 */
errr init_r_event_txt(FILE *fp, char *buf)
{
	s16b r_idx = 0;

	/* Not ready yet */
	bool okay = FALSE;

	/* Current entry */
	death_event_type *d_ptr = NULL;


	/* Just before the first record */
	error_idx = -1;

	/* Just before the first line */
	error_line = -1;


	/* Start the "fake" stuff */
	event_head->name_size = 0;
	event_head->text_size = 0;

	/* Parse */
	while (0 == my_fgets(fp, buf, 1024))
	{
 		/* Advance the line number */
		error_line++;

		/* Skip comments and blank lines */
		if (!buf[0] || (buf[0] == '#')) continue;

		/* Verify correct "colon" format */
		if (buf[1] != ':') return ERR_PARSE;


		/* Process based on the initial character. */
		switch (*buf)
		{
			case 'V': /* Version */
		{
				if (check_version(buf, event_head))
					return ERR_VERSION;
				else
			okay = TRUE;
 				break;
		}

			case 'N': /* New/Number/Name (extracting number) */
		{
				/* Get the number */
			r_idx = atoi(buf+2);
				break;
		}

			case 'E': /* (Death) Events */
			{
				int i;
				u16b temp_offset = 0;

				/* Nothing can be done without a valid monster */
				if (!r_idx)
				{
					msg_print("No r_idx specified!");
					return ERR_MISSING;
				}

				/* Find an unused event slot */
				if ((unsigned)(++error_idx) >= MAX_DEATH_EVENTS)
					{
					msg_print("Too many events!");
						return ERR_MEMORY;
					}
				d_ptr = &death_event[error_idx];


				/* Look for a text string to avoid problems with keywords within it. 
				 * To avoid conflicts with other arbitrary text, a \\ before a character
				 * means that it can't terminate a string.
				 * This is the string which is printed when an event occurs.
				 */
				i = do_get_string(buf, '\"', "^\"", event_text, &(event_head->text_size), fake_text_size, &(d_ptr->text));
				if (i) return i;
				/* This is a second text string, the use of which depends on the type of event.
				 * It currently only supplies a name for a randart. We haven't yet found out
				 * what type of event it is, so we save the offset to a temporary variable to
				 * deal with later.
				 */
				{
					u32b temp_name_size = event_head->name_size;
					i = do_get_string(buf, '^', "^\"", event_name, &temp_name_size, fake_name_size, &temp_offset);
					if (i) return i;
					event_head->name_size = (u16b)temp_name_size;
				}
				/* Now the text strings have been removed, we remove the \\s from the file. Note that, if a name
				 * contains a \\ character, this must be listed as \\. Similarly, a " must be listed as \" and
				 * a ^ as \^. There should hopefully not be many of these.
				 */
				clear_escapes(buf);

				/* Also convert the %x sequences used in sequences for objects. */
				do_compress_string(buf);

				/* Save the monster index */
				d_ptr->r_idx = r_idx;

				/* Now find the name of an event type. */
				d_ptr->type = find_string(buf, death_flags);

				/* Check for the ONLY_ONE flag. */
				if (find_string_x(buf, "ONLY_ONE")) d_ptr->flags |= EF_ONLY_ONE;

				if (find_string_x(buf, "IF_PREV")) d_ptr->flags |= EF_IF_PREV;

				/* A single ONLY_ONE flag causes no problems, but having
				 * an IF_PREV flag without a previous entry with the same
				 * r_idx would be bad. */

				if (d_ptr->flags & EF_IF_PREV && (error_idx == 0 ||
				d_ptr->r_idx != (d_ptr-1)->r_idx))
				{
					msg_print("IF_PREV without previous entry.");
					return ERR_FLAG;
				}

				/* Look for flags compatible with the event type
				 * First parse arbitrary text strings to avoid having to
				 * deal with keywords within them.
				 *
				 * Then look for source-based strings (which should be
				 * capitalised). These will not generally have numerical
				 * alternatives.
				 *
				 * Then look for info-based strings. These will always
				 * have numerical alternatives to prevent confusion between
				 * them, and between them and source-based strings.
				 *
				 * Finally, look for other keyword-based parameters. These
				 * will use a single character to indicate them, so they
				 * must be dealt with after all text strings that might
				 * include such characters.
				 */

				switch (d_ptr->type)
			{
					case DEATH_OBJECT:
					{
						make_item_type *i_ptr = &d_ptr->par.item;
						if (find_string_x(buf, "ARTEFACT")) i_ptr->flags |= EI_ART;
						if (find_string_x(buf, "EGO")) i_ptr->flags |= EI_EGO;
						
						if (find_string_x(buf, "RANDOM"))
						{
							i_ptr->flags |= EI_RAND;
						}
						else if (i_ptr->flags & EI_ART)
						{
							find_string_info(a_name, a_info, MAX_A_IDX, i_ptr->x_idx);
						}
#ifdef ALLOW_EGO_DROP
						else if (i_ptr->flags & EI_EGO)
						{
							find_string_info(e_name, e_info, MAX_E_IDX, i_ptr->x_idx);
						}
#endif
						find_string_info(k_name, k_info, MAX_K_IDX, i_ptr->k_idx);
						if (lookup_kind(readnum('t'), readnum('s')))
						{
							byte t = 0,s = 0;
							readclearnum(t, 't');
							readclearnum(s, 's');
							i_ptr->k_idx = lookup_kind(t,s);
						}
						if (i_ptr->flags & EI_ART)
						{
							readclearnum(i_ptr->x_idx, 'a');
						}
#ifdef ALLOW_EGO_DROP
						else if (i_ptr->flags & EI_EGO)
						{
							readclearnum(i_ptr->x_idx, 'e');
						}
#endif
						readclearnum(i_ptr->min, '(');
						readclearnum(i_ptr->max, '-');

						/* Add the name of a randart, if provided. */
						if (i_ptr->flags & EI_RAND && i_ptr->flags & EI_ART)
						{
							if (temp_offset)
							{
								i_ptr->name = temp_offset;
								temp_offset = 0;
							}
						}
						/* Interpret no number parameter as being 1. */
						if (i_ptr->min == 0 && i_ptr->max == 0)
						{
							i_ptr->max=i_ptr->min=1;
						}

						/* Ensure that a possible x_idx field has been created for
						 * any non-random artefact or ego item */
						if (i_ptr->flags & EI_ART && ~i_ptr->flags & EI_RAND)
						{
							artifact_type *a_ptr = &a_info[i_ptr->x_idx];
							/* Ensure this is a real artefact. */
							if (!a_ptr->name)
							{
								msg_print("No valid artefact specified.");
								return ERR_PARSE;
							}
							/* Take an unstated k_idx to be that of the artefact. */
							else if (!i_ptr->k_idx)
							{
								i_ptr->k_idx = lookup_kind(a_ptr->tval, a_ptr->sval);
							}
							/* Ensure that any stated k_idx is the right one. */
							else if (k_info[i_ptr->k_idx].tval != a_ptr->tval ||
								k_info[i_ptr->k_idx].sval != a_ptr->sval)
							{
msg_format("(%d,%d) != (%d,%d) ", k_info[i_ptr->k_idx].tval,
 k_info[i_ptr->k_idx].sval, a_ptr->tval, a_ptr->sval);
								msg_print("Incompatible object and artefact.");
								return ERR_PARSE;
							}
						}
#ifdef ALLOW_EGO_DROP
						else if (i_ptr->flags & EI_EGO && ~i_ptr->flags & EI_RAND)
						{
							ego_item_type *e_ptr = &e_info[i_ptr->x_idx];
							/* Ensure this is a real ego item. */
							if (!e_ptr->name)
							{
								msg_print("No valid ego type specified.");
								return ERR_PARSE;
							}
							/* Ensure that the ego type is possible for this k_idx. */
						}
#endif

						/* Ensure that a possible k_idx field has been created. */
						if (k_info[i_ptr->k_idx].name == 0)
						{
							msg_print("No valid object specified.");
							return ERR_PARSE;
						}

						/* Ensure that RAND has not been used without a sensible thing
						 * to randomise. */
						if (i_ptr->flags & EI_RAND && ~i_ptr->flags & (EI_ART
#ifdef ALLOW_EGO_DROP
						| EI_EGO
#endif
						))
						{
							msg_print("Nothing valid to randomise.");
							return ERR_PARSE;
						}
						

						/* Prevent badly formatted ranges. */
						if (i_ptr->min > i_ptr->max || !i_ptr->min)
						{
							msg_print("Bad number parameter.");
							return ERR_FLAG;
						}
					break;
					}
					case DEATH_MONSTER:  
					{
						make_monster_type *i_ptr = &d_ptr->par.monster;
						i_ptr->strict = find_string_x(buf, "STRICT");
						find_string_info(r_name, r_info, MAX_R_IDX, i_ptr->num);
						readclearnum(i_ptr->num, 'n');
						readclearnum(i_ptr->radius, 'r');
						readclearnum(i_ptr->min, '(');
						readclearnum(i_ptr->max, '-');

						/* Interpret no number parameter as being 1. */
						if (i_ptr->min == 0 && i_ptr->max == 0)
						{
							i_ptr->max=i_ptr->min=1;
						}

						/* As the original square still has the original monster
						 * on it, parse a missing or 0 parameter as 1.
						 * Should there be a way of using the original
						 * square if requested? */
						if (!i_ptr->radius)
						{
							i_ptr->radius = 1;
						}

						/* Prevent badly formatted ranges. */
						if (i_ptr->min > i_ptr->max || !i_ptr->min)
						{
							msg_print("Bad number parameter.");
							return ERR_FLAG;
						}

						/* Prevent non-existant monster references. */
						if (!i_ptr->num || i_ptr->num >= MAX_R_IDX)
						{
							msg_print("No monster specified!");
							return ERR_FLAG;
						}
					break;
					}
					case DEATH_EXPLODE:
					{
						make_explosion_type *i_ptr = &d_ptr->par.explosion;
						i_ptr->method = find_string(buf, explode_flags);
						readclearnum(i_ptr->radius,'r');
						readclearnum(i_ptr->dice,'(');
						readclearnum(i_ptr->sides,'d');
						
						/* Require an explosion type */
						if (!i_ptr->method)
						{
							msg_print("No method indicated.");
							return ERR_FLAG;
						}
						/* Allow (d30) or (100) damage formats,
						 * but not no damage indicator at all. */
						if (!i_ptr->dice && !i_ptr->sides)
						{
							msg_print("No damage indicator.");
							return ERR_FLAG;
						}
						else if (!i_ptr->dice)
						{
							i_ptr->dice = 1;
						}
						else if (!i_ptr->sides)
						{
							i_ptr->sides = 1;
						}
					}
					break;
					case DEATH_COIN:
					{
						make_coin_type *i_ptr = &d_ptr->par.coin;
						i_ptr->metal = find_string(buf, coin_types)-1;
						if (i_ptr->metal == -1)
						{
							msg_print("No coin type!");
							return ERR_FLAG;
						}
					break;
					}
					case DEATH_NOTHING: /* No special parameters */
					break;
					default: /* i.e. no valid type specification */
					msg_print("No event type specified!");
					return ERR_MISSING;
			}
				/* Fill in the other general flags now the text
				 * strings have all been parsed. */
				readclearnum(d_ptr->num, 'p');
				readclearnum(d_ptr->denom,'/');

				/* Parse a missing probability entry as 1/1. */
				if (!d_ptr->num && !d_ptr->denom)
				{
					d_ptr->num=d_ptr->denom=1;
				}

				/* Complain about p0/x formats or probabilities over 1. */
				if (!d_ptr->num || d_ptr->num > d_ptr->denom)
				{
					msg_print("Bad probability specification.");
					return ERR_PARSE;
				}
				/* The type-specific text field should have either been reset, or not
				 * set in the first place. */
				if (temp_offset)
				{
					msg_print("Meaningless text field found.");
					return ERR_PARSE;
				}

				/* Finally check that everything has been read. */
				{
					cptr s;
					for (s = buf+2; *s != '\0'; s++)
						if (!strchr(": )\"", *s))
						{
							msg_print("Uninterpreted characters!");
							return ERR_PARSE;
						}
				}
				break;
		}		
			/* Ignore irrelevant lines */
			case 'D': case 'G': case 'I': case 'W': case 'B': case 'F': case 'S':
			break;
			default: /* Oops */
			return ERR_DIRECTIVE;
	}
	}

	/* Complete the "name" and "text" sizes */
	++event_head->name_size;
	++event_head->text_size;

	/* No version yet */
	if (!okay) return ERR_VERSION;

	/* Success */
	return SUCCESS;
}

/*
 * Convert a "color letter" into an "actual" color
 * The colors are: dwsorgbuDWvyRGBU, as shown below
 */
static int color_char_to_attr(char c)
{
	switch (c)
	{
		case 'd': return (TERM_DARK);
		case 'w': return (TERM_WHITE);
		case 's': return (TERM_SLATE);
		case 'o': return (TERM_ORANGE);
		case 'r': return (TERM_RED);
		case 'g': return (TERM_GREEN);
		case 'b': return (TERM_BLUE);
		case 'u': return (TERM_UMBER);

		case 'D': return (TERM_L_DARK);
		case 'W': return (TERM_L_WHITE);
		case 'v': return (TERM_VIOLET);
		case 'y': return (TERM_YELLOW);
		case 'R': return (TERM_L_RED);
		case 'G': return (TERM_L_GREEN);
		case 'B': return (TERM_L_BLUE);
		case 'U': return (TERM_L_UMBER);
	}

	return (-1);
}



/*** Initialize from ascii template files ***/


/*
 * Initialize the "v_info" array, by parsing an ascii "template" file
 */
errr init_v_info_txt(FILE *fp, char *buf)
{
	int i;

	char *s;

	/* Not ready yet */
	bool okay = FALSE;

	/* Current entry */
	vault_type *v_ptr = NULL;


	/* Just before the first record */
	error_idx = -1;

	/* Just before the first line */
	error_line = -1;


	/* Prepare the "fake" stuff */
	v_head->name_size = 0;
	v_head->text_size = 0;

	/* Parse */
	while (0 == my_fgets(fp, buf, 1024))
	{
		/* Advance the line number */
		error_line++;

		/* Skip comments and blank lines */
		if (!buf[0] || (buf[0] == '#')) continue;

		/* Verify correct "colon" format */
		if (buf[1] != ':') return (1);


		/* Hack -- Process 'V' for "Version" */
		if (buf[0] == 'V')
		{
			int v1, v2, v3;

			/* Scan for the values */
			if ((3 != sscanf(buf, "V:%d.%d.%d", &v1, &v2, &v3)) ||
			    (v1 != v_head->v_major) ||
			    (v2 != v_head->v_minor) ||
			    (v3 != v_head->v_patch))
			{
				return (2);
			}

			/* Okay to proceed */
			okay = TRUE;

			/* Continue */
			continue;
		}

		/* No version yet */
		if (!okay) return (2);


		/* Process 'N' for "New/Number/Name" */
		if (buf[0] == 'N')
		{
			/* Find the colon before the name */
			s = strchr(buf+2, ':');

			/* Verify that colon */
			if (!s) return (1);

			/* Nuke the colon, advance to the name */
			*s++ = '\0';

			/* Paranoia -- require a name */
			if (!*s) return (1);

			/* Get the index */
			i = atoi(buf+2);

			/* Verify information */
			if (i <= error_idx) return (4);

			/* Verify information */
			if (i >= v_head->info_num) return (2);

			/* Save the index */
			error_idx = i;

			/* Point at the "info" */
			v_ptr = &v_info[i];

			/* Hack -- Verify space */
			if (v_head->name_size + strlen(s) + 8 > fake_name_size) 
			{
				return (7);
			}

			/* Advance and Save the name index */
			if (!v_ptr->name) v_ptr->name = ++v_head->name_size;

			/* Append chars to the name */
			strcpy(v_name + v_head->name_size, s);

			/* Advance the index */
			v_head->name_size += strlen(s);

			/* Next... */
			continue;
		}

		/* There better be a current v_ptr */
		if (!v_ptr) return (3);


		/* Process 'D' for "Description" */
		if (buf[0] == 'D')
		{
			/* Acquire the text */
			s = buf+2;

			/* Hack -- Verify space */
			if (v_head->text_size + strlen(s) + 8 > fake_text_size) 
			{
				return (7);
			}

			/* Advance and Save the text index */
			if (!v_ptr->text) v_ptr->text = ++v_head->text_size;

			/* Append chars to the name */
			strcpy(v_text + v_head->text_size, s);

			/* Advance the index */
			v_head->text_size += strlen(s);

			/* Next... */
			continue;
		}


		/* Process 'X' for "Extra info" (one line only) */
		if (buf[0] == 'X')
		{
			int typ, rat, hgt, wid;

			/* Scan for the values */
			if (4 != sscanf(buf+2, "%d:%d:%d:%d",
			                &typ, &rat, &hgt, &wid)) return (1);

			/* Save the values */
			v_ptr->typ = typ;
			v_ptr->rat = rat;
			v_ptr->hgt = hgt;
			v_ptr->wid = wid;

			/* Next... */
			continue;
		}


		/* Oops */
		return (6);
	}


	/* Complete the "name" and "text" sizes */
	++v_head->name_size;
	++v_head->text_size;


	/* No version yet */
	if (!okay) return (2);


	/* Success */
	return (0);
}



/*
 * Initialize the "f_info" array, by parsing an ascii "template" file
 */
errr init_f_info_txt(FILE *fp, char *buf)
{
	int i;

	char *s;

	/* Not ready yet */
	bool okay = FALSE;

	/* Current entry */
	feature_type *f_ptr = NULL;


	/* Just before the first record */
	error_idx = -1;

	/* Just before the first line */
	error_line = -1;


	/* Prepare the "fake" stuff */
	f_head->name_size = 0;
	f_head->text_size = 0;

	/* Parse */
	while (0 == my_fgets(fp, buf, 1024))
	{
		/* Advance the line number */
		error_line++;

		/* Skip comments and blank lines */
		if (!buf[0] || (buf[0] == '#')) continue;

		/* Verify correct "colon" format */
		if (buf[1] != ':') return (1);


		/* Hack -- Process 'V' for "Version" */
		if (buf[0] == 'V')
		{
			int v1, v2, v3;

			/* Scan for the values */
			if ((3 != sscanf(buf+2, "%d.%d.%d", &v1, &v2, &v3)) ||
			    (v1 != f_head->v_major) ||
			    (v2 != f_head->v_minor) ||
			    (v3 != f_head->v_patch))
			{
				return (2);
			}

			/* Okay to proceed */
			okay = TRUE;

			/* Continue */
			continue;
		}

		/* No version yet */
		if (!okay) return (2);


		/* Process 'N' for "New/Number/Name" */
		if (buf[0] == 'N')
		{
			/* Find the colon before the name */
			s = strchr(buf+2, ':');

			/* Verify that colon */
			if (!s) return (1);

			/* Nuke the colon, advance to the name */
			*s++ = '\0';

			/* Paranoia -- require a name */
			if (!*s) return (1);

			/* Get the index */
			i = atoi(buf+2);

			/* Verify information */
			if (i <= error_idx) return (4);

			/* Verify information */
			if (i >= f_head->info_num) return (2);

			/* Save the index */
			error_idx = i;

			/* Point at the "info" */
			f_ptr = &f_info[i];

			/* Hack -- Verify space */
			if (f_head->name_size + strlen(s) + 8 > fake_name_size) return (7);

			/* Advance and Save the name index */
			if (!f_ptr->name) f_ptr->name = ++f_head->name_size;

			/* Append chars to the name */
			strcpy(f_name + f_head->name_size, s);

			/* Advance the index */
			f_head->name_size += strlen(s);

			/* Default "mimic" */
			f_ptr->mimic = i;

			/* Next... */
			continue;
		}

		/* There better be a current f_ptr */
		if (!f_ptr) return (3);



		/* Process 'M' for "Mimic" (one line only) */
		if (buf[0] == 'M')
		{
			int mimic;

			/* Scan for the values */
			if (1 != sscanf(buf+2, "%d",
			                &mimic)) return (1);

			/* Save the values */
			f_ptr->mimic = mimic;

			/* Next... */
			continue;
		}


		/* Process 'G' for "Graphics" (one line only) */
		if (buf[0] == 'G')
		{
			int tmp;

			/* Paranoia */
			if (!buf[2]) return (1);
			if (!buf[3]) return (1);
			if (!buf[4]) return (1);

			/* Extract the color */
			tmp = color_char_to_attr(buf[4]);
			if (tmp < 0) return (1);

			/* Save the values */
			f_ptr->d_char = buf[2];
			f_ptr->d_attr = tmp;

			/* Next... */
			continue;
		}


		/* Oops */
		return (6);
	}


	/* Complete the "name" and "text" sizes */
	++f_head->name_size;
	++f_head->text_size;


	/* No version yet */
	if (!okay) return (2);


	/* Success */
	return (0);
}



/*
 * Grab one flag in an object_kind from a textual string
 */
static errr grab_one_kind_flag(object_kind *k_ptr, cptr what)
{
	int i;

	/* Check flags1 */
	for (i = 0; i < 32; i++)
	{
		if (streq(what, k_info_flags1[i]))
		{
			k_ptr->flags1 |= (1L << i);
			return (0);
		}
	}

	/* Check flags2 */
	for (i = 0; i < 32; i++)
	{
		if (streq(what, k_info_flags2[i]))
		{
			k_ptr->flags2 |= (1L << i);
			return (0);
		}
	}

	/* Check flags3 */
	for (i = 0; i < 32; i++)
	{
		if (streq(what, k_info_flags3[i]))
		{
			k_ptr->flags3 |= (1L << i);
			return (0);
		}
	}

	/* Oops */
	msg_format("Unknown object flag '%s'.", what);

	/* Error */
	return (1);
}



/*
 * Initialize the "k_info" array, by parsing an ascii "template" file
 */
errr init_k_info_txt(FILE *fp, char *buf)
{
	int i;

	char *s, *t;

	/* Not ready yet */
	bool okay = FALSE;

	/* Current entry */
	object_kind *k_ptr = NULL;

	/* Initialise the temporary tval-dependant flag list */
	object_kind kt_info[256];
	
	C_WIPE(kt_info, 256, object_kind);

	/* Just before the first record */
	error_idx = -1;

	/* Just before the first line */
	error_line = -1;


	/* Prepare the "fake" stuff */
	k_head->name_size = 0;
	k_head->text_size = 0;

	/* Parse */
	while (0 == my_fgets(fp, buf, 1024))
	{
		/* Advance the line number */
		error_line++;

		/* Skip comments and blank lines */
		if (!buf[0] || (buf[0] == '#')) continue;

		/* Verify correct "colon" format */
		if (buf[1] != ':') return (1);


		/* Hack -- Process 'V' for "Version" */
		if (buf[0] == 'V')
		{
			int v1, v2, v3;

			/* Scan for the values */
			if ((3 != sscanf(buf+2, "%d.%d.%d", &v1, &v2, &v3)) ||
			    (v1 != k_head->v_major) ||
			    (v2 != k_head->v_minor) ||
			    (v3 != k_head->v_patch))
			{
				return (2);
			}

			/* Okay to proceed */
			okay = TRUE;

			/* Continue */
			continue;
		}

		/* No version yet */
		if (!okay) return (2);


		/* Process 'N' for "New/Number/Name" */
		if (buf[0] == 'N')
		{
			/* Find the colon before the name */
			s = strchr(buf+2, ':');

			/* Verify that colon */
			if (!s) return (1);

			/* Nuke the colon, advance to the name */
			*s++ = '\0';

			/* Paranoia -- require a name */
			if (!*s) return (1);

			/* Get the index */
			i = atoi(buf+2);

			/* Verify information */
			if (i <= error_idx) return (4);

			/* Verify information */
			if (i >= k_head->info_num) return (2);

			/* Save the index */
			error_idx = i;

			/* Point at the "info" */
			k_ptr = &k_info[i];

			/* Compress the string */
			do_compress_string(s);

			/* Hack -- Verify space */
			if (k_head->name_size + strlen(s) + 8 > fake_name_size) return (7);

			/* Advance and Save the name index */
			if (!k_ptr->name) k_ptr->name = ++k_head->name_size;

			/* Append chars to the name */
			strcpy(k_name + k_head->name_size, s);

			/* Advance the index */
			k_head->name_size += strlen(s);

			/* Next... */
			continue;
		}

		/* Process 'T' for tval-dependant flags. */
		if (buf[0] == 'T')
		{
			object_kind *kt_ptr;
			
			/* Get the tval */
			i = atoi(buf+2);
			
			/* Verify information */
			if (i < 0 || i > 255) return PARSE_ERROR_OUT_OF_BOUNDS;
			
			/* Point at the "info" */
			kt_ptr = &kt_info[i];

			/* Advance to the first flag. */
			s = strchr(buf, '|');
			
			if (!s) return PARSE_ERROR_TOO_FEW_ARGUMENTS;
			
			s += strspn(s, "| ");

			/* Parse every other entry textually */
			while (*s)
			{
				/* Find the end of this entry */
				for (t = s; *t && (*t != ' ') && (*t != '|'); ++t) /* loop */;

				/* Nuke and skip any dividers */
				if (*t)
				{
					*t++ = '\0';
					while (*t == ' ' || *t == '|') t++;
				}

				/* Parse this entry */
				if (SUCCESS != grab_one_kind_flag(kt_ptr, s)) return PARSE_ERROR_INVALID_FLAG;

				/* Start the next entry */
				s = t;
			}

			/* This should not be used within an entry,
			 * so remove k_ptr to make sure. */
			k_ptr = 0;


			/* Next... */
			continue;
		}
			

		/* There better be a current k_ptr */
		if (!k_ptr) return (3);




		/* Process 'G' for "Graphics" (one line only) */
		if (buf[0] == 'G')
		{
			char sym, col;
			int tmp, p_id;

			/* Scan for the values */
			if (3 != sscanf(buf+2, "%c:%c:%d", &sym, &col, &p_id))
			{
				return (2);
			}

			/* Extract the attr */
			tmp = color_char_to_attr(col);

			/* Paranoia */
			if (tmp < 0) return (1);
			if (p_id < 0 || p_id > 255) return (1);

			/* Save the values */
			k_ptr->d_char = sym;
			k_ptr->d_attr = tmp;

			/* Hack - store p_id in k_ptr->u_idx until flavor_init() */
			k_ptr->u_idx = p_id;

			/* Next... */
			continue;
		}

		/* Process 'I' for "Info" (one line only) */
		if (buf[0] == 'I')
		{
			int tval, sval, pval;
			object_kind *kt_ptr;

			/* Scan for the values */
			if (3 != sscanf(buf+2, "%d:%d:%d",
			                &tval, &sval, &pval)) return (1);

			/* Include the tval-dependent flags. */
			kt_ptr = &kt_info[tval];

			k_ptr->flags1 |= kt_ptr->flags1;
			k_ptr->flags2 |= kt_ptr->flags2;
			k_ptr->flags3 |= kt_ptr->flags3;

			/* Save the values */
			k_ptr->tval = tval;
			k_ptr->sval = sval;
			k_ptr->pval = pval;

			/* Include the tval-dependent flags, if any. */
			k_ptr->flags1 |= kt_info[k_ptr->tval].flags1;
			k_ptr->flags2 |= kt_info[k_ptr->tval].flags2;
			k_ptr->flags3 |= kt_info[k_ptr->tval].flags3;

			/* Next... */
			continue;
		}

		/* Process 'W' for "More Info" (one line only) */
		if (buf[0] == 'W')
		{
			int level, extra, wgt;
			long cost;

			/* Scan for the values */
			if (4 != sscanf(buf+2, "%d:%d:%d:%ld",
			                &level, &extra, &wgt, &cost)) return (1);

			/* Save the values */
			k_ptr->level = level;
/*			k_ptr->extra = extra;*/
			k_ptr->weight = wgt;
			k_ptr->cost = cost;

			/* Next... */
			continue;
		}

		/* Process 'A' for "Allocation" (one line only) */
		if (buf[0] == 'A')
		{
			int i;

			/* XXX XXX XXX Simply read each number following a colon */
			for (i = 0, s = buf+1; s && (s[0] == ':') && s[1]; ++i)
			{
				/* Default chance */
				k_ptr->chance[i] = 1;

				/* Store the attack damage index */
				k_ptr->locale[i] = atoi(s+1);

				/* Find the slash */
				t = strchr(s+1, '/');

				/* Find the next colon */
				s = strchr(s+1, ':');

				/* If the slash is "nearby", use it */
				if (t && (!s || t < s))
				{
					int chance = atoi(t+1);
					if (chance > 0) k_ptr->chance[i] = chance;
				}
			}

			/* Next... */
			continue;
		}

		/* Hack -- Process 'P' for "power" and such */
		if (buf[0] == 'P')
		{
			int ac, hd1, hd2, th, td, ta;

			/* Scan for the values */
			if (6 != sscanf(buf+2, "%d:%dd%d:%d:%d:%d",
			                &ac, &hd1, &hd2, &th, &td, &ta)) return (1);

			k_ptr->ac = ac;
			k_ptr->dd = hd1;
			k_ptr->ds = hd2;
			k_ptr->to_h = th;
			k_ptr->to_d = td;
			k_ptr->to_a =  ta;

			/* Next... */
			continue;
		}

		/* Hack -- Process 'F' for flags */
		if (buf[0] == 'F')
		{
			/* Parse every entry textually */
			for (s = buf + 2; *s; )
			{
				/* Find the end of this entry */
				for (t = s; *t && (*t != ' ') && (*t != '|'); ++t) /* loop */;

				/* Nuke and skip any dividers */
				if (*t)
				{
					*t++ = '\0';
					while (*t == ' ' || *t == '|') t++;
				}

				/* Parse this entry */
				if (0 != grab_one_kind_flag(k_ptr, s)) return (5);

				/* Start the next entry */
				s = t;
			}

			/* Next... */
			continue;
		}


		/* Oops */
		return (6);
	}


	/* Complete the "name" and "text" sizes */
	++k_head->name_size;
	++k_head->text_size;


	/* No version yet */
	if (!okay) return (2);


	/* Success */
	return (0);
}

/*
 * First check that the entry is reasonable in all of its fields.
 *
 * Then add an arbitrary number of scroll entries derived from a single base
 * element if required. These are assigned s_ids between 1 and *scrolls.
 */
static errr u_finish_off(s16b scroll_base, s16b *scrolls, unident_type *u_ptr) \
{
	unident_type *ub_ptr = &u_info[scroll_base];
	s16b i;

	/* Nothing started. */
	if (!u_ptr) return SUCCESS;

	/* Check that the flags are appropriate. */
	if (u_ptr->s_id == SID_BASE)
	{
		if ((*scrolls && !scroll_base) || (!*scrolls && scroll_base))
		{
			msg_print("Both a SCROLL flag and a number are needed.");
			return ERR_PARSE;
		}
		else if (u_ptr->flags & UNID_BASE_ONLY && scroll_base)
		{
			msg_print("Scrolls cannot be BASE_ONLY.");
			return ERR_PARSE;
		}
		else if (u_ptr->flags & UNID_NO_BASE)
		{
			msg_print("This is a base entry itself.");
			return ERR_PARSE;
		}
	}
	else
	{
		if (*scrolls || scroll_base || u_ptr->flags & UNID_BASE_ONLY)
		{
			msg_print("Flag restricted to base items found.");
			return ERR_PARSE;
		}
	}

	/* No scrolls to write. */
	if (!(*scrolls)) return SUCCESS;
	
	for (i = 1; i <= (*scrolls); i++)
	{
		error_idx++;

		/* Hack -- Verify space */
		if (error_idx >= UB_U_IDX) return ERR_MEMORY;
		if ((u32b)u_head->name_size + MAX_SCROLL_LEN + 8 > fake_name_size) return ERR_MEMORY;

		/* Select the new entry. */
		u_ptr = &u_info[error_idx];

		/* Copy the structure across. */
		COPY(u_ptr, ub_ptr, unident_type);

		/* Set the s_id */
		u_ptr->s_id = i;

		/* Set the flags */
		u_ptr->flags = UNID_SCROLL_N;

		/* Advance and Save the name index */
		u_ptr->name = ++u_head->name_size;

		/* Save a default name. */
		strcpy(u_name+u_ptr->name, "Untitled");

		/* Advance the indices */
		u_head->name_size += MAX_SCROLL_LEN;
	}

	/* Don't do this again. */
	(*scrolls) = 0;
	
	return SUCCESS;
}

/* A macro for the above */
#define do_u_finish_off \
	err = u_finish_off(scroll_base, &scrolls, u_ptr); \
	if (!scrolls) scroll_base = 0; \
	if (err) return err;


/*
 * Initialize the "u_info" array, by parsing an ascii "template" file
 */
errr init_u_info_txt(FILE *fp, char *buf)
{
	char *s;
	errr err;

	/* Not adding any scrolls yet */
	s16b scrolls = 0, scroll_base = 0;

	/* Not ready yet */
	bool okay = FALSE;

	/* Current entry */
	unident_type *u_ptr = NULL;


	/* Hack - store p_id and s_id until they are abandoned. */
	byte u_ptr_p_id[UB_U_IDX];
	byte u_ptr_s_id[UB_U_IDX];
	C_WIPE(u_ptr_p_id, UB_U_IDX, byte);
	C_WIPE(u_ptr_s_id, UB_U_IDX, byte);

	/* Just before the first record */
	error_idx = -1;

	/* Just before the first line */
	error_line = -1;


	/* Prepare the "fake" stuff */
	u_head->name_size = 0;

	/* Parse */
	while (0 == my_fgets(fp, buf, 1024))
	{
		/* Advance the line number */
		error_line++;

		/* Skip comments and blank lines */
		if (!buf[0] || (buf[0] == '#')) continue;

		/* Verify correct "colon" format */
		if (buf[1] != ':') return (1);


		/* Hack -- Process 'V' for "Version" */
		if (buf[0] == 'V')
		{
			errr err = check_version(buf, u_head);
			if (err) return err;
			okay = TRUE;
			continue;
		}

		/* No version yet */
		if (!okay) return ERR_VERSION;


		/* Process 'N' for "Name" */
		if (buf[0] == 'N')
		{
			/* Add scrolls, if any. */
			do_u_finish_off;

			/* Find the colon before the name */
			s = strchr(buf, ':');

			/* Verify that colon */
			if (!s) return ERR_PARSE;

			/* Advance to the name */
			s++;

			/* Paranoia -- require a name */
			if (!*s) return ERR_PARSE;

			/* Increment the index */
			error_idx++;

			/* Paranoia */
			if (error_idx >= UB_U_IDX) return ERR_MEMORY;

			/* Point at the "info" */
			u_ptr = &u_info[error_idx];

			/* Compress the string */
			do_compress_string(s);

			/* Hack -- Verify space */
			if (u_head->name_size + strlen(s) + 8 > fake_name_size) return ERR_MEMORY;

			/* Advance and Save the name index */
			u_ptr->name = ++u_head->name_size;

			/* Append chars to the name */
			strcpy(u_name + u_head->name_size, s);

			/* Advance the index */
			u_head->name_size += strlen(s);

			/* Next... */
			continue;
		}

		/* Process 'M' for "Mimic"
		 * This creates a copy of one entry with s_id 0 with
		 * a different p_id. It must be after the original entry
		 * and not within an entry itself. */
		if (buf[0] == 'M')
		{
			int oldpid, newpid;
			s16b n;

			do_u_finish_off;

			if (2 != sscanf(buf+2, "%d:%d",
			                &oldpid, &newpid)) return ERR_PARSE;

			/* Check for valid indices */
			if (oldpid < 0 || oldpid > 255 || newpid < 0 || newpid > 255)
				return ERR_PARSE;

			/* Increment the index */
			error_idx++;
			
			/* Paranoia */
			if (error_idx >= UB_U_IDX) return ERR_MEMORY;

			/* Point at the "info" */
			u_ptr = &u_info[error_idx];
			
			/* Look for the original entry */
			for (n = 0; n < error_idx; n++)
			{
				unident_type *u2_ptr = &u_info[n];

				/* Only consider base entries. */
				if (u2_ptr->s_id != SID_BASE) continue;
				
				/* Look for the specified p_id. */
				if (u2_ptr->p_id != oldpid) continue;
				
				/* Copy the structure across. */
				COPY(u_ptr, u2_ptr, unident_type);
				
				/* Correct the p_id */
				u2_ptr->p_id = newpid;
				
				/* Finished */
				break;
			}
			
			/* Complain if none found. */
			if (n == error_idx) return ERR_MISSING;
			
			/* Next... */
			continue;
		}

		/* There better be a current u_ptr */
		if (!u_ptr) return (3);

		/* Process 'G' for "Graphics" (one line only) */
		if (buf[0] == 'G')
		{
			char sym, col;
			int p_id, s_id;
			s16b i;

			/* Scan for the values */
			if (4 != sscanf(buf+2, "%c:%c:%d:%d", &sym, &col, &p_id, &s_id)) return ERR_PARSE;

			/* Paranoia */
			if (color_char_to_attr(col) < 0)
			{
				msg_print("Illegal colour.");
				return ERR_PARSE;
			}
			if (!isgraph(sym))
			{
				msg_print("Illegal symbol.");
				return ERR_PARSE;
			}
			/* Extract the char */
			u_ptr->d_char = sym;

			/* Extract the attr */
			u_ptr->d_attr = color_char_to_attr(col);
			
			/* Verify indices' legality */
			if (p_id < 0 || p_id > 255 ||
				s_id < 0 || s_id > 255)
			{
				msg_print("Illegal index.");
				return ERR_PARSE;
			}

			/* Extract the primary index */
			u_ptr->p_id = p_id;
			
			/* Extract the secondary index */
			u_ptr->s_id = s_id;

			/* Verify uniqueness */
			for (i = 0; i < error_idx; i++)
			{
				unident_type *u2_ptr = &u_info[i];
				if (u2_ptr->p_id != u_ptr->p_id) continue;
				if (u2_ptr->s_id != u_ptr->s_id) continue;
				msg_print("Duplicated indices.");
				return ERR_PARSE;
			}

			/* Next... */
			continue;
		}

		/* Hack -- Process 'F' for flags */
		if (buf[0] == 'F')
		{
			/* Parse every entry textually */
			for (s = buf + 2; *s; )
			{
				char *t;

				/* Find the end of this entry */
				for (t = s; *t && (*t != ':'); ++t) /* loop */;

				/* Nuke and skip any dividers */
				if (*t)
				{
					*t++ = '\0';
					while (*t == ':') t++;
				}

				/* Parse this entry */
				if (!strcmp(s, "BASE_ONLY"))
				{
					u_ptr->flags |= UNID_BASE_ONLY;
				}
				else if (!strcmp(s, "SCROLL"))
				{
					scroll_base = u_ptr-u_info;
				}
				else if (!strcmp(s, "NO_BASE"))
				{
					u_ptr->flags |= UNID_NO_BASE;
				}
				else if (atoi(s))
				{
					scrolls = atoi(s);
				}
				else
				{
					return ERR_FLAG;
				}

				/* Start the next entry */
				s = t;
			}

			/* Next... */
			continue;
		}

		/* Oops */
		return ERR_DIRECTIVE;
	}

	do_u_finish_off;

	/* Complete the "name" size */
	++u_head->name_size;

	/* Set the "info" size. */
	u_head->info_num = error_idx+1;
	u_head->info_size = u_head->info_num * u_head->info_len;

	/* No version yet */
	if (!okay) return ERR_VERSION;

	/* Success */
	return SUCCESS;
}

/*
 * Grab one flag in an artifact_type from a textual string
 */
static errr grab_one_artifact_flag(artifact_type *a_ptr, cptr what)
{
	int i;

	/* Check flags1 */
	for (i = 0; i < 32; i++)
	{
		if (streq(what, k_info_flags1[i]))
		{
			a_ptr->flags1 |= (1L << i);
			return (0);
		}
	}

	/* Check flags2 */
	for (i = 0; i < 32; i++)
	{
		if (streq(what, k_info_flags2[i]))
		{
			a_ptr->flags2 |= (1L << i);
			return (0);
		}
	}

	/* Check flags3 */
	for (i = 0; i < 32; i++)
	{
		if (streq(what, k_info_flags3[i]))
		{
			a_ptr->flags3 |= (1L << i);
			return (0);
		}
	}

	/* Oops */
	msg_format("Unknown artifact flag '%s'.", what);

	/* Error */
	return (1);
}




/*
 * Initialize the "a_info" array, by parsing an ascii "template" file
 */
errr init_a_info_txt(FILE *fp, char *buf)
{
	int i;

	char *s, *t;

	/* Not ready yet */
	bool okay = FALSE;

	/* Current entry */
	artifact_type *a_ptr = NULL;


	/* Just before the first record */
	error_idx = -1;

	/* Just before the first line */
	error_line = -1;


	/* Parse */
	while (0 == my_fgets(fp, buf, 1024))
	{
		/* Advance the line number */
		error_line++;

		/* Skip comments and blank lines */
		if (!buf[0] || (buf[0] == '#')) continue;

		/* Verify correct "colon" format */
		if (buf[1] != ':') return (1);


		/* Hack -- Process 'V' for "Version" */
		if (buf[0] == 'V')
		{
			int v1, v2, v3;

			/* Scan for the values */
			if ((3 != sscanf(buf+2, "%d.%d.%d", &v1, &v2, &v3)) ||
			    (v1 != a_head->v_major) ||
			    (v2 != a_head->v_minor) ||
			    (v3 != a_head->v_patch))
			{
				return (2);
			}

			/* Okay to proceed */
			okay = TRUE;

			/* Continue */
			continue;
		}

		/* No version yet */
		if (!okay) return (2);


		/* Process 'N' for "New/Number/Name" */
		if (buf[0] == 'N')
		{
			/* Find the colon before the name */
			s = strchr(buf+2, ':');

			/* Verify that colon */
			if (!s) return (1);

			/* Nuke the colon, advance to the name */
			*s++ = '\0';

			/* Paranoia -- require a name */
			if (!*s) return (1);

			/* Get the index */
			i = atoi(buf+2);

			/* Verify information */
			if (i < error_idx) return (4);

			/* Verify information */
			if (i >= a_head->info_num) return (2);

			/* Save the index */
			error_idx = i;

			/* Point at the "info" */
			a_ptr = &a_info[i];

			/* Hack -- Verify space */
			if (a_head->name_size + strlen(s) + 8 > fake_name_size) return (7);

			/* Advance and Save the name index */
			if (!a_ptr->name) a_ptr->name = ++a_head->name_size;

			/* Append chars to the name */
			strcpy(a_name + a_head->name_size, s);

			/* Advance the index */
			a_head->name_size += strlen(s);

			/* Ignore everything */
			a_ptr->flags3 |= (TR3_IGNORE_ALL);

			/* Next... */
			continue;
		}

		/* There better be a current a_ptr */
		if (!a_ptr) return (3);



		/* Process 'I' for "Info" (one line only) */
		if (buf[0] == 'I')
		{
			int tval, sval, pval;

			/* Scan for the values */
			if (3 != sscanf(buf+2, "%d:%d:%d",
			                &tval, &sval, &pval)) return (1);

			/* Save the values */
			a_ptr->tval = tval;
			a_ptr->sval = sval;
			a_ptr->pval = pval;

			/* Next... */
			continue;
		}

		/* Process 'W' for "More Info" (one line only) */
		if (buf[0] == 'W')
		{
			int level, rarity, wgt;
			long cost;

			/* Scan for the values */
			if (4 != sscanf(buf+2, "%d:%d:%d:%ld",
			                &level, &rarity, &wgt, &cost)) return (1);

			/* Save the values */
			a_ptr->level = level;
			a_ptr->rarity = rarity;
			a_ptr->weight = wgt;
			a_ptr->cost = cost;

			/* Next... */
			continue;
		}

		/* Hack -- Process 'P' for "power" and such */
		if (buf[0] == 'P')
		{
			int ac, hd1, hd2, th, td, ta;

			/* Scan for the values */
			if (6 != sscanf(buf+2, "%d:%dd%d:%d:%d:%d",
			                &ac, &hd1, &hd2, &th, &td, &ta)) return (1);

			a_ptr->ac = ac;
			a_ptr->dd = hd1;
			a_ptr->ds = hd2;
			a_ptr->to_h = th;
			a_ptr->to_d = td;
			a_ptr->to_a =  ta;

			/* Next... */
			continue;
		}

		/* Hack -- Process 'F' for flags */
		if (buf[0] == 'F')
		{
			/* Parse every entry textually */
			for (s = buf + 2; *s; )
			{
				/* Find the end of this entry */
				for (t = s; *t && (*t != ' ') && (*t != '|'); ++t) /* loop */;

				/* Nuke and skip any dividers */
				if (*t)
				{
					*t++ = '\0';
					while ((*t == ' ') || (*t == '|')) t++;
				}

				/* Parse this entry */
				if (0 != grab_one_artifact_flag(a_ptr, s)) return (5);

				/* Start the next entry */
				s = t;
			}

			/* Next... */
			continue;
		}


		/* Oops */
		return (6);
	}


	/* Complete the "name" and "text" sizes */
	++a_head->name_size;
	++a_head->text_size;


	/* No version yet */
	if (!okay) return (2);


	/* Success */
	return (0);
}


/*
 * Grab one flag in a ego-item_type from a textual string
 */
static bool grab_one_ego_item_flag(ego_item_type *e_ptr, cptr what)
{
	int i;

	/* Check flags1 */
	for (i = 0; i < 32; i++)
	{
		if (streq(what, k_info_flags1[i]))
		{
			e_ptr->flags1 |= (1L << i);
			return (0);
		}
	}

	/* Check flags2 */
	for (i = 0; i < 32; i++)
	{
		if (streq(what, k_info_flags2[i]))
		{
			e_ptr->flags2 |= (1L << i);
			return (0);
		}
	}

	/* Check flags3 */
	for (i = 0; i < 32; i++)
	{
		if (streq(what, k_info_flags3[i]))
		{
			e_ptr->flags3 |= (1L << i);
			return (0);
		}
	}

	/* Oops */
	msg_format("Unknown ego-item flag '%s'.", what);

	/* Error */
	return (1);
}




/*
 * Initialize the "e_info" array, by parsing an ascii "template" file
 */
errr init_e_info_txt(FILE *fp, char *buf)
{
	int i;

	char *s, *t;

	/* Not ready yet */
	bool okay = FALSE;

	/* Current entry */
	ego_item_type *e_ptr = NULL;


	/* Just before the first record */
	error_idx = -1;

	/* Just before the first line */
	error_line = -1;


	/* Parse */
	while (0 == my_fgets(fp, buf, 1024))
	{
		/* Advance the line number */
		error_line++;

		/* Skip comments and blank lines */
		if (!buf[0] || (buf[0] == '#')) continue;

		/* Verify correct "colon" format */
		if (buf[1] != ':') return (1);


		/* Hack -- Process 'V' for "Version" */
		if (buf[0] == 'V')
		{
			int v1, v2, v3;

			/* Scan for the values */
			if ((3 != sscanf(buf+2, "%d.%d.%d", &v1, &v2, &v3)) ||
			    (v1 != e_head->v_major) ||
			    (v2 != e_head->v_minor) ||
			    (v3 != e_head->v_patch))
			{
				return (2);
			}

			/* Okay to proceed */
			okay = TRUE;

			/* Continue */
			continue;
		}

		/* No version yet */
		if (!okay) return (2);


		/* Process 'N' for "New/Number/Name" */
		if (buf[0] == 'N')
		{
			/* Find the colon before the name */
			s = strchr(buf+2, ':');

			/* Verify that colon */
			if (!s) return (1);

			/* Nuke the colon, advance to the name */
			*s++ = '\0';

			/* Paranoia -- require a name */
			if (!*s) return (1);

			/* Get the index */
			i = atoi(buf+2);

			/* Verify information */
			if (i < error_idx) return (4);

			/* Verify information */
			if (i >= e_head->info_num) return (2);

			/* Save the index */
			error_idx = i;

			/* Point at the "info" */
			e_ptr = &e_info[i];

			/* Hack -- Verify space */
			if (e_head->name_size + strlen(s) + 8 > fake_name_size) return (7);

			/* Advance and Save the name index */
			if (!e_ptr->name) e_ptr->name = ++e_head->name_size;

			/* Append chars to the name */
			strcpy(e_name + e_head->name_size, s);

			/* Advance the index */
			e_head->name_size += strlen(s);

			/* Next... */
			continue;
		}

		/* There better be a current e_ptr */
		if (!e_ptr) return (3);


		/* Process 'X' for "Xtra" (one line only) */
		if (buf[0] == 'X')
		{
			int slot, rating;

			/* Scan for the values */
			if (2 != sscanf(buf+2, "%d:%d",
			                &slot, &rating)) return (1);

			/* Save the values */
			e_ptr->slot = slot;
			e_ptr->rating = rating;

			/* Next... */
			continue;
		}

		/* Process 'W' for "More Info" (one line only) */
		if (buf[0] == 'W')
		{
			int level, rarity, pad2;
			long cost;

			/* Scan for the values */
			if (4 != sscanf(buf+2, "%d:%d:%d:%ld",
			                &level, &rarity, &pad2, &cost)) return (1);

			/* Save the values */
			e_ptr->level = level;
			e_ptr->rarity = rarity;
			/* e_ptr->weight = wgt; */
			e_ptr->cost = cost;

			/* Next... */
			continue;
		}

		/* Hack -- Process 'C' for "creation" */
		if (buf[0] == 'C')
		{
			int th, td, ta, pv;

			/* Scan for the values */
			if (4 != sscanf(buf+2, "%d:%d:%d:%d",
			                &th, &td, &ta, &pv)) return (1);

			e_ptr->max_to_h = th;
			e_ptr->max_to_d = td;
			e_ptr->max_to_a = ta;
			e_ptr->max_pval = pv;

			/* Next... */
			continue;
		}

		/* Hack -- Process 'F' for flags */
		if (buf[0] == 'F')
		{
			/* Parse every entry textually */
			for (s = buf + 2; *s; )
			{
				/* Find the end of this entry */
				for (t = s; *t && (*t != ' ') && (*t != '|'); ++t) /* loop */;

				/* Nuke and skip any dividers */
				if (*t)
				{
					*t++ = '\0';
					while ((*t == ' ') || (*t == '|')) t++;
				}

				/* Parse this entry */
				if (0 != grab_one_ego_item_flag(e_ptr, s)) return (5);

				/* Start the next entry */
				s = t;
			}

			/* Next... */
			continue;
		}

		/* Oops */
		return (6);
	}


	/* Complete the "name" and "text" sizes */
	++e_head->name_size;
	++e_head->text_size;


	/* No version yet */
	if (!okay) return (2);


	/* Success */
	return (0);
}


/*
 * Grab one (basic) flag in a monster_race from a textual string
 */
static errr grab_one_basic_flag(monster_race *r_ptr, cptr what)
{
	int i;

	/* Scan flags1 */
	for (i = 0; i < 32; i++)
	{
		if (streq(what, r_info_flags1[i]))
		{
			r_ptr->flags1 |= (1L << i);
			return (0);
		}
	}

	/* Scan flags2 */
	for (i = 0; i < 32; i++)
	{
		if (streq(what, r_info_flags2[i]))
		{
			r_ptr->flags2 |= (1L << i);
			return (0);
		}
	}

	/* Scan flags1 */
	for (i = 0; i < 32; i++)
	{
		if (streq(what, r_info_flags3[i]))
		{
			r_ptr->flags3 |= (1L << i);
			return (0);
		}
	}

	/* Oops */
	msg_format("Unknown monster flag '%s'.", what);

	/* Failure */
	return (1);
}


/*
 * Grab one (spell) flag in a monster_race from a textual string
 */
static errr grab_one_spell_flag(monster_race *r_ptr, cptr what)
{
	int i;

	/* Scan flags4 */
	for (i = 0; i < 32; i++)
	{
		if (streq(what, r_info_flags4[i]))
		{
			r_ptr->flags4 |= (1L << i);
			return (0);
		}
	}

	/* Scan flags5 */
	for (i = 0; i < 32; i++)
	{
		if (streq(what, r_info_flags5[i]))
		{
			r_ptr->flags5 |= (1L << i);
			return (0);
		}
	}

	/* Scan flags6 */
	for (i = 0; i < 32; i++)
	{
		if (streq(what, r_info_flags6[i]))
		{
			r_ptr->flags6 |= (1L << i);
			return (0);
		}
	}

	/* Oops */
	msg_format("Unknown monster flag '%s'.", what);

	/* Failure */
	return (1);
}




/*
 * Initialize the "r_info" array, by parsing an ascii "template" file
 */
errr init_r_info_txt(FILE *fp, char *buf)
{
	int i;

	char *s, *t;

	/* Not ready yet */
	bool okay = FALSE;

	/* Current entry */
	monster_race *r_ptr = NULL;


	/* Just before the first record */
	error_idx = -1;

	/* Just before the first line */
	error_line = -1;


	/* Start the "fake" stuff */
	r_head->name_size = 0;
	r_head->text_size = 0;

	/* Parse */
	while (0 == my_fgets(fp, buf, 1024))
	{
		/* Advance the line number */
		error_line++;

		/* Skip comments and blank lines */
		if (!buf[0] || (buf[0] == '#')) continue;

		/* Verify correct "colon" format */
		if (buf[1] != ':') return (1);


		/* Hack -- Process 'V' for "Version" */
		if (buf[0] == 'V')
		{
			int v1, v2, v3;

			/* Scan for the values */
			if ((3 != sscanf(buf+2, "%d.%d.%d", &v1, &v2, &v3)) ||
			    (v1 != r_head->v_major) ||
			    (v2 != r_head->v_minor) ||
			    (v3 != r_head->v_patch))
			{
				return (2);
			}

			/* Okay to proceed */
			okay = TRUE;

			/* Continue */
			continue;
		}

		/* No version yet */
		if (!okay) return (2);


		/* Process 'N' for "New/Number/Name" */
		if (buf[0] == 'N')
		{
			/* Find the colon before the name */
			s = strchr(buf+2, ':');

			/* Verify that colon */
			if (!s) return (1);

			/* Nuke the colon, advance to the name */
			*s++ = '\0';

			/* Paranoia -- require a name */
			if (!*s) return (1);

			/* Get the index */
			i = atoi(buf+2);

			/* Verify information */
			if (i < error_idx) return (4);

			/* Verify information */
			if (i >= r_head->info_num) return (2);

			/* Save the index */
			error_idx = i;

			/* Point at the "info" */
			r_ptr = &r_info[i];

			/* Hack -- Verify space */
			if (r_head->name_size + strlen(s) + 8 > fake_name_size) return (7);

			/* Advance and Save the name index */
			if (!r_ptr->name) r_ptr->name = ++r_head->name_size;

			/* Append chars to the name */
			strcpy(r_name + r_head->name_size, s);

			/* Advance the index */
			r_head->name_size += strlen(s);

			/* Next... */
			continue;
		}

		/* There better be a current r_ptr */
		if (!r_ptr) return (3);


		/* Process 'D' for "Description" */
		if (buf[0] == 'D')
		{
			/* Acquire the text */
			s = buf+2;

			/* Hack -- Verify space */
			if (r_head->text_size + strlen(s) + 8 > fake_text_size) return (7);

			/* Advance and Save the text index */
			if (!r_ptr->text) r_ptr->text = ++r_head->text_size;

			/* Append chars to the name */
			strcpy(r_text + r_head->text_size, s);

			/* Advance the index */
			r_head->text_size += strlen(s);

			/* Next... */
			continue;
		}

		/* Process 'G' for "Graphics" (one line only) */
		if (buf[0] == 'G')
		{
			char sym;
			int tmp;

			/* Paranoia */
			if (!buf[2]) return (1);
			if (!buf[3]) return (1);
			if (!buf[4]) return (1);

			/* Extract the char */
			sym = buf[2];

			/* Extract the attr */
			tmp = color_char_to_attr(buf[4]);

			/* Paranoia */
			if (tmp < 0) return (1);

			/* Save the values */
			r_ptr->d_char = sym;
			r_ptr->d_attr = tmp;

			/* Next... */
			continue;
		}

		/* Process 'I' for "Info" (one line only) */
		if (buf[0] == 'I')
		{
			int spd, atspd, hp1, hp2, aaf, ac, slp;

			/* Scan for the other values */
			if (7 != sscanf(buf+2, "%d:%d:%dd%d:%d:%d:%d",
			                &spd, &atspd, &hp1, &hp2, &aaf, &ac, &slp)) return (1);

			/* Save the values */
			r_ptr->speed = spd;
			r_ptr->num_blows = atspd;
			r_ptr->hdice = hp1;
			r_ptr->hside = hp2;
			r_ptr->aaf = aaf;
			r_ptr->ac = ac;
			r_ptr->sleep = slp;

			/* Next... */
			continue;
		}

		/* Process 'W' for "More Info" (one line only) */
		if (buf[0] == 'W')
		{
			int lev, rar, pad;
			long exp;

			/* Scan for the values */
			if (4 != sscanf(buf+2, "%d:%d:%d:%ld",
			                &lev, &rar, &pad, &exp)) return (1);

			/* Save the values */
			r_ptr->level = lev;
			r_ptr->rarity = rar;
/*			r_ptr->extra = pad;*/
			r_ptr->mexp = exp;

			/* Next... */
			continue;
		}

		/* Process 'B' for "Blows" (up to four lines) */
		if (buf[0] == 'B')
		{
			int n1, n2;

			/* Find the next empty blow slot (if any) */
			for (i = 0; i < 4; i++) if (!r_ptr->blow[i].method) break;

			/* Oops, no more slots */
			if (i == 4) return (1);

			/* Analyze the first field */
			for (s = t = buf+2; *t && (*t != ':'); t++) /* loop */;

			/* Terminate the field (if necessary) */
			if (*t == ':') *t++ = '\0';

			/* Analyze the method */
			for (n1 = 0; r_info_blow_method[n1]; n1++)
			{
				if (streq(s, r_info_blow_method[n1])) break;
			}

			/* Invalid method */
			if (!r_info_blow_method[n1]) return (1);

			/* Analyze the second field */
			for (s = t; *t && (*t != ':'); t++) /* loop */;

			/* Terminate the field (if necessary) */
			if (*t == ':') *t++ = '\0';

			/* Analyze effect */
			for (n2 = 0; r_info_blow_effect[n2]; n2++)
			{
				if (streq(s, r_info_blow_effect[n2])) break;
			}

			/* Invalid effect */
			if (!r_info_blow_effect[n2]) return (1);

			/* Analyze the third field */
			for (s = t; *t && (*t != 'd'); t++) /* loop */;

			/* Terminate the field (if necessary) */
			if (*t == 'd') *t++ = '\0';

			/* Save the method */
			r_ptr->blow[i].method = n1;

			/* Save the effect */
			r_ptr->blow[i].effect = n2;

			/* Extract the damage dice and sides */
			r_ptr->blow[i].d_dice = atoi(s);
			r_ptr->blow[i].d_side = atoi(t);

			/* Next... */
			continue;
		}

		/* Process 'F' for "Basic Flags" (multiple lines) */
		if (buf[0] == 'F')
		{
			/* Parse every entry */
			for (s = buf + 2; *s; )
			{
				/* Find the end of this entry */
				for (t = s; *t && (*t != ' ') && (*t != '|'); ++t) /* loop */;

				/* Nuke and skip any dividers */
				if (*t)
				{
					*t++ = '\0';
					while (*t == ' ' || *t == '|') t++;
				}

				/* Parse this entry */
				if (0 != grab_one_basic_flag(r_ptr, s)) return (5);

				/* Start the next entry */
				s = t;
			}

			/* Next... */
			continue;
		}

		/* Process 'S' for "Spell Flags" (multiple lines) */
		if (buf[0] == 'S')
		{
			/* Parse every entry */
			for (s = buf + 2; *s; )
			{
				/* Find the end of this entry */
				for (t = s; *t && (*t != ' ') && (*t != '|'); ++t) /* loop */;

				/* Nuke and skip any dividers */
				if (*t)
				{
					*t++ = '\0';
					while ((*t == ' ') || (*t == '|')) t++;
				}

				/* XXX XXX XXX Hack -- Read spell frequency */
				if (1 == sscanf(s, "1_IN_%d", &i))
				{
					/* Extract a "frequency" */
					r_ptr->freq_spell = r_ptr->freq_inate = 100 / i;

					/* Start at next entry */
					s = t;

					/* Continue */
					continue;
				}

				/* Parse this entry */
				if (0 != grab_one_spell_flag(r_ptr, s)) return (5);

				/* Start the next entry */
				s = t;
			}

			/* Next... */
			continue;
		}

		/* Ignore 'E' (death event) lines */
		if (buf[0] == 'E') continue;

		/* Oops */
		return (6);
	}


	/* Complete the "name" and "text" sizes */
	++r_head->name_size;
	++r_head->text_size;


	/* XXX XXX XXX The ghost is unused */

	/* Mega-Hack -- acquire "ghost" */
	r_ptr = &r_info[MAX_R_IDX-1];

	/* Acquire the next index */
	r_ptr->name = r_head->name_size;
	r_ptr->text = r_head->text_size;

	/* Save some space for the ghost info */
	r_head->name_size += 64;
	r_head->text_size += 64;

	/* Hack -- Default name/text for the ghost */
	strcpy(r_name + r_ptr->name, "Nobody, the Undefined Ghost");
	strcpy(r_text + r_ptr->text, "It seems strangely familiar...");

	/* Hack -- set the char/attr info */
	r_ptr->d_attr = r_ptr->x_attr = TERM_WHITE;
	r_ptr->d_char = r_ptr->x_char = 'G';

	/* Hack -- Try to prevent a few "potential" bugs */
	r_ptr->flags1 |= (RF1_UNIQUE);

	/* Hack -- Try to prevent a few "potential" bugs */
	r_ptr->flags1 |= (RF1_NEVER_MOVE | RF1_NEVER_BLOW);

	/* Hack -- Try to prevent a few "potential" bugs */
	r_ptr->hdice = r_ptr->hside = 1;

	/* Hack -- Try to prevent a few "potential" bugs */
	r_ptr->mexp = 1L;


	/* No version yet */
	if (!okay) return (2);


	/* Success */
	return (0);
}


#else	/* ALLOW_TEMPLATES */

#ifdef MACINTOSH
static int i = 0;
#endif

#endif	/* ALLOW_TEMPLATES */

